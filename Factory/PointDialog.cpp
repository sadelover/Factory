// PointView.cpp : 实现文件
//
#include "stdafx.h"
#include "PointDialog.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include <sstream>
#include "ChangeValueDlg.h"
#include "Tools/Util/UtilString.h"
#include "PageDlg.h"
#include "PointHisdataDlg.h"
#include "BEOP-DesignerView.h"
#include "MainFrm.h"

typedef unsigned long           u32;

static const int STATIC_SCREEN_WIDTH = 1024;
static const int STATIC_SCREEN_HEIGHT = 768;
static HANDLE	g_hEventScan = NULL;


// CPointView
IMPLEMENT_DYNAMIC(CPointDialog, CDialog)

BEGIN_MESSAGE_MAP(CPointDialog, CDialog)
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ALL, &CPointDialog::OnNMDblclkListAll)
	ON_BN_CLICKED(IDC_BUTTON_ADDWATCH, &CPointDialog::OnBnClickedButtonAddwatch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WATCH, &CPointDialog::OnNMDblclkListWatch)
	ON_BN_CLICKED(IDC_BUTTON_ADDPOINT, &CPointDialog::OnBnClickedButtonAddpoint)
	ON_BN_CLICKED(IDC_BUTTON_AddWatchLine, &CPointDialog::OnBnClickedButtonaddwatchline)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CPointDialog::OnBnClickedButtonDelete)
	ON_MESSAGE(WM_MYMSG_PRINT_POINT, OnMyMsgPrintPoint)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ALL, &CPointDialog::OnLvnItemchangedListAll)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CPointDialog::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDCANCEL, &CPointDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CPointDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_REALTIME, &CPointDialog::OnBnClickedButtonExportRealtime)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTREALTIME, &CPointDialog::OnBnClickedButtonImportrealtime)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALL, &CPointDialog::OnNMClickListAll)
	//ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ALL, &CPointDialog::OnLvnColumnclickListAll)
	ON_STN_CLICKED(IDC_STATIC_POINTCOUNT, &CPointDialog::OnStnClickedStaticPointcount)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_CLEAR, &CPointDialog::OnBnClickedButtonSearchClear)
	ON_BN_CLICKED(IDC_BUTTON_MULTI_MODIFY, &CPointDialog::OnBnClickedButtonMultiModify)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY, &CPointDialog::OnBnClickedButtonHistory)
END_MESSAGE_MAP()

CPointDialog::CPointDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPointDialog::IDD, pParent)
	, m_strNameSearch(_T(""))
	, m_bScanRun(true)
{
	m_strWatchPointLineList.clear();
	m_Print_nIndex        = 0;
	m_bExitThread = false;
	m_bThreadExitFinished = false;
	InitializeCriticalSection(&m_CriticalSection_PrintPointInfo);
	m_pDataAccess = NULL;
	m_nCurSearchIndex= 0;
	m_nSearchMax = 0;
	m_nSearchMin = 100000;
	m_pGatewayDlg = NULL;
	m_hThreadPointAndMode = NULL;
	m_bRefreshOK = false;
	m_EventStopThread = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_nSetPointValueTimeout = 10;
	g_hEventScan = CreateEvent(NULL, FALSE, TRUE, NULL);


	m_HisQueryDateFrom = COleDateTime::GetCurrentTime();
	m_HisQueryDateTo = COleDateTime::GetCurrentTime();
	m_HisQueryTimeFrom = COleDateTime(m_HisQueryDateFrom.GetYear(),m_HisQueryDateFrom.GetMonth(), m_HisQueryDateFrom.GetDay(), 0,0,0 );

	m_HisQueryTimeTo = COleDateTime::GetCurrentTime();
	m_HisQueryTimePeriod = 0;

}

CPointDialog::~CPointDialog()
{
	//KillTimer(1);

}


bool CPointDialog::ThreadExitFinish()
{
	return m_bThreadExitFinished;
}

void CPointDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WATCH, m_ListWatch);
	DDX_Control(pDX, IDC_LIST_ALL, m_ListAll);
	DDX_Control(pDX, IDC_STATIC_POINTCOUNT, m_StaticPointCount);
	DDX_Text(pDX, IDC_EDIT_NAME_SEARCH, m_strNameSearch);
}

// CLoadingDlg message handlers
BOOL CPointDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_ListAll.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT|LVS_EDITLABELS); 
	m_ListAll.InsertColumn(0,L"编号",LVCFMT_LEFT,50,0);
	m_ListAll.InsertColumn(1,L"名称",LVCFMT_LEFT,200,0);
	m_ListAll.InsertColumn(2,L"类型",LVCFMT_LEFT,100,0); 
	m_ListAll.InsertColumn(3,L"值",LVCFMT_LEFT,100,0); 
	m_ListAll.ShowWindow(SW_SHOW);
	
	m_ListWatch.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT|LVS_EDITLABELS); 

	m_ListWatch.InsertColumn(0,L"编号",LVCFMT_LEFT,50,0);
	m_ListWatch.InsertColumn(1,L"名称",LVCFMT_LEFT,200,0);
	m_ListWatch.InsertColumn(2,L"类型",LVCFMT_LEFT,100,0); 
	m_ListWatch.InsertColumn(3,L"值",LVCFMT_LEFT,100,0);

	m_ListWatch.ShowWindow(SW_SHOW);
	return TRUE;
}

void CPointDialog::InitData()
{

	//m_pointmanager->Init();

	ReadWatchPointFromDB();
	//const vector<DataPointEntry>& vecPointList = m_pointmanager->GetPointList();
	m_strPointValueList.resize(m_pGatewayDlg->m_vecServerPointList.size(), L"");
	CString strTemp;
	int cur_i = 0;
	for(int i=0;i<m_pGatewayDlg->m_vecServerPointList.size();i++)
	{
		strTemp.Format(L"%d", i+1);
		m_ListAll.InsertItem(i, L"");
		m_ListAll.SetItemText(i,0, strTemp);
		m_ListAll.SetItemText(i,1, m_pGatewayDlg->m_vecServerPointList[i].GetShortName().c_str());
		m_ListAll.SetItemText(i,2, m_pGatewayDlg->m_vecServerPointList[i].GetSourceType().c_str());

		if (IsPushback(m_strWatchPointNameList,m_pGatewayDlg->m_vecServerPointList[i].GetShortName()))
		{	
			strTemp.Format(L"%d", i+1);
			m_ListWatch.InsertItem(cur_i, L"");
			m_ListWatch.SetItemText(cur_i,0, strTemp);
			m_ListWatch.SetItemText(cur_i,1, m_pGatewayDlg->m_vecServerPointList[i].GetShortName().c_str());
			m_ListWatch.SetItemText(cur_i,2, m_pGatewayDlg->m_vecServerPointList[i].GetSourceType().c_str());

			cur_i++;
		}
	}
	
	strTemp.Format(L"点数总计:%d", m_pGatewayDlg->m_vecServerPointList.size());
	m_StaticPointCount.SetWindowTextW(strTemp);

	StartThreads(0,0);
	StartScanThread();
}

void CPointDialog::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{//loading过程中
	//	if(m_strInfoLoading.GetLength()<=50)
	//		m_strInfoLoading += _T(".");
	//	UpdateLoginInfo();
		int a = 0;
	}
}

LRESULT CPointDialog::OnMyMsgPrintPoint(WPARAM wParam,LPARAM lParam)
{
	EnterCriticalSection(&m_CriticalSection_PrintPointInfo);

	if (m_strNameSearch != _T(""))
	{
		int nLenListAll = m_ListAll.GetItemCount();
		for (int j=0; j<nLenListAll; j++)
		{
			if (m_ListAll.GetItemText(j, 1) == m_Print_lpszName.c_str())
			{
				m_ListAll.SetItemText(j, 3, m_Print_lpszValue.c_str());
				break;
			}
		}
	}
	else
	{
		wstring strCurValue = m_ListAll.GetItemText(m_Print_nIndex, 3);
		if(strCurValue.c_str()!= m_Print_lpszValue.c_str())
		{
			m_ListAll.SetItemText(m_Print_nIndex,3,m_Print_lpszValue.c_str());
		}
	}

	for(int i = 0;i<m_ListWatch.GetItemCount();i++)
	{
		if(m_ListWatch.GetItemText(i,1)== m_Print_lpszName.c_str())
		{
			m_ListWatch.SetItemText(i,3,m_Print_lpszValue.c_str());
		}
	}

	LeaveCriticalSection(&m_CriticalSection_PrintPointInfo);


	return 1;
}

LRESULT CPointDialog::StartThreads( WPARAM wParam,LPARAM lParam )
{
	m_hThreadPointAndMode = CreateThread(NULL, 0, ThreadFuncPoint, this, 0, NULL); 
	assert(m_hThreadPointAndMode);
	return 0; 
}


void  CPointDialog::ExitThreads()
{
	m_bExitThread = true;
	if(m_hThreadPointAndMode)
	{
		::WaitForSingleObject(m_EventStopThread, 3000);
		CloseHandle(m_hThreadPointAndMode);
		m_hThreadPointAndMode = NULL;
	}

}

DWORD WINAPI CPointDialog::ThreadFuncPoint( LPVOID lpParameter )
{
	CPointDialog* pDlg = (CPointDialog*)lpParameter;
	assert(pDlg);
	int nTimeCount = 0;
	while(TRUE)
	{
		Sleep(3000);
		CUPPCServerDataAccess* pDataAccess = pDlg->m_pDataAccess;
		if (pDlg->m_bExitThread)
		{
			pDlg->m_bThreadExitFinished = true;
			SetEvent(pDlg->m_EventStopThread);
			return 0;
		}

		//如果界面隐藏时候，停止刷新
		if(pDlg->IsWindowVisible() == FALSE)
		{
			continue;
		}

		nTimeCount = 0;
		if(pDataAccess==NULL)
			continue;

		const vector<DataPointEntry>& vecPointList = pDlg->m_pGatewayDlg->m_vecServerPointList;
		const size_t size = vecPointList.size();

		for (size_t i=0;i<size;++i)
		{
			if (pDlg->m_bExitThread)
			{
				pDlg->m_bThreadExitFinished = true;
				SetEvent(pDlg->m_EventStopThread);
				return 0;
			}

			if(i>= vecPointList.size())
				continue;

			const wstring strConfig = vecPointList[i].GetParam(1);//1代表配置过的点
			const wstring strName =  vecPointList[i].GetShortName();
			const wstring strType = vecPointList[i].GetSourceType();
			if (strName.length()>0)
			{ 
				wstring strValue;
				const bool bRes = pDataAccess->GetValue(strName, strValue);
				if(bRes)
				{
					if(strValue != pDlg->m_strPointValueList[i] || pDlg->m_bRefreshOK)
					{
						EnterCriticalSection(&(pDlg->m_CriticalSection_PrintPointInfo));
						pDlg->m_strPointValueList[i] = strValue;
						pDlg->m_Print_lpszName = strName;
						pDlg->m_Print_lpszTypel = strType;
						pDlg->m_Print_lpszValue = strValue;
						pDlg->m_Print_nIndex = i;
						LeaveCriticalSection(&(pDlg->m_CriticalSection_PrintPointInfo));
						if (pDlg->m_bExitThread)
						{
							return 0;
						}
						else
						{
							::SendMessage(pDlg->m_hWnd, WM_MYMSG_PRINT_POINT, 0,0);
						}
						
					}
					
				}
			}
		}

		pDlg->m_bRefreshOK = false;
		if (pDlg->m_bExitThread)
		{
			pDlg->m_bThreadExitFinished = true;
			SetEvent(pDlg->m_EventStopThread);
			return 0;
		}
	}

	pDlg->m_bThreadExitFinished = true;
	return 0;
}

void CPointDialog::OnNMDblclkListAll(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString strName = m_ListAll.GetItemText(pNMItemActivate->iItem,1);
	CString strType = m_ListAll.GetItemText(pNMItemActivate->iItem,2);
	if(strName==L"")
		return;

	CString strValue = m_ListAll.GetItemText(pNMItemActivate->iItem,3);
	CChangeValueDlg dlg(m_pDataAccess, strName, strValue, TRUE, m_nSetPointValueTimeout, this);

	if (dlg.DoModal() == IDOK)
	{
		m_ListAll.SetItemText(pNMItemActivate->iItem,3,dlg.GetValue().GetString() );
	}
	*pResult = 0;

}
void CPointDialog::OnBnClickedButtonAddwatch()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_ListAll.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
		return;
	else 
	{ 
		while (pos) 
		{ 
			int nItem = m_ListAll.GetNextSelectedItem(pos); 
			TRACE1("Item %d was selected!\n", nItem); 
			// you could do your own processing on nItem here 

			wstring sPointName = m_ListAll.GetItemText(nItem,1).GetString();
			if (!IsPushback(m_strWatchPointNameList,sPointName))
			{

				m_strWatchPointNameList.push_back(m_ListAll.GetItemText(nItem,1).GetString());

				int curitem = m_ListWatch.InsertItem(m_ListWatch.GetItemCount(),L"");
				m_ListWatch.SetItemText(curitem, 0 , m_ListAll.GetItemText(nItem,0).GetString());
				m_ListWatch.SetItemText(curitem, 1 , m_ListAll.GetItemText(nItem,1).GetString());
				m_ListWatch.SetItemText(curitem, 2 , m_ListAll.GetItemText(nItem,2).GetString());
				m_ListWatch.SetItemText(curitem, 3 , m_ListAll.GetItemText(nItem,3).GetString());

				CString strName = m_strWatchPointNameList.back().c_str();
				InsertWatchPointToDB(strName);
			}

		} 
	}

}
bool CPointDialog::IsPushback(std::vector<wstring> &PointList,wstring PointName)
{
	if (PointList.empty())
	{
		return false;
	}

	bool ISpush = false;
	for (int i=0;i<PointList.size();++i)
	{
		if (PointList[i] == PointName)
		{
			ISpush = true;
			break;
		}
	}

	return ISpush;
}

void CPointDialog::OnNMDblclkListWatch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString strName = m_ListWatch.GetItemText(pNMItemActivate->iItem,1);
	if(strName==L"")
		return;

	CString strValue = m_ListWatch.GetItemText(pNMItemActivate->iItem,3);
	CChangeValueDlg dlg(m_pDataAccess, strName, strValue, TRUE, m_nSetPointValueTimeout, this);
	if (dlg.DoModal() == IDOK)
	{
		m_ListWatch.SetItemText(pNMItemActivate->iItem,3,dlg.GetValue().GetString() );
	}
}


void CPointDialog::OnBnClickedButtonAddpoint()
{
	// TODO: 在此添加控件通知处理程序代码
	// 按钮显示为“新增单点”——>“刷新”
	RefreshFromDB();
	m_bRefreshOK = true;

	if (m_strNameSearch != _T(""))
	{
		OnBnClickedButtonSearch();
	}
}


BOOL CPointDialog::InsertWatchPointToDB(CString strPointName)
{
	m_pDataAccess->SetPointWatching(strPointName.GetString(), true);
	return TRUE;
}


BOOL CPointDialog::DeleteWatchPointFromDB(CString strPointName)
{
	m_pDataAccess->SetPointWatching(strPointName.GetString(), false);
	return TRUE;

}


BOOL CPointDialog::ReadWatchPointFromDB()
{
	m_strWatchPointNameList.clear();

	m_pDataAccess->ReadWatchingPoinitList(m_strWatchPointNameList);
	return TRUE;
}

void CPointDialog::OnBnClickedButtonaddwatchline()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT iSeclectSum = m_ListWatch.GetSelectedCount();
	POSITION pt = m_ListWatch.GetFirstSelectedItemPosition();
	CString csText = _T("");
	int iTem = -1;

	CRealTimeTrendDlg *pDlg1 = &(m_pGatewayDlg->m_PaneRealtimeTrend1);

	for (UINT i = 0; i < iSeclectSum; i++)
	{
		csText = _T("");
		iTem = m_ListWatch.GetNextSelectedItem(pt);
		csText = m_ListWatch.GetItemText(iTem,1);
		if (pDlg1->m_iniWatchPointList.size()<5)
		{
			if (!IsPushback(pDlg1->m_iniWatchPointList,csText.GetString()))
			{
				pDlg1->m_iniWatchPointList.push_back(csText.GetString());
			}
			
		}
		else
		{
			if (!IsPushback(pDlg1->m_iniWatchPointList,csText.GetString()))
			{
				pDlg1->m_iniWatchPointList[4] = csText.GetString();
			}
		}
		
	}

}


void CPointDialog::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT iSeclectSum = m_ListWatch.GetSelectedCount();
	POSITION pt = m_ListWatch.GetFirstSelectedItemPosition();
	CString csText = _T("");
	int iTem = -1;
	for (UINT i = 0; i < iSeclectSum; i++)
	{
		csText = _T("");
		iTem = m_ListWatch.GetNextSelectedItem(pt);
		csText = m_ListWatch.GetItemText(iTem,1);
		m_ListWatch.DeleteItem(iTem);

		for (int k = 0; k< m_strWatchPointNameList.size();++k)
		{
			if (m_strWatchPointNameList[k] == csText.GetString())
			{
				m_strWatchPointNameList.erase(m_strWatchPointNameList.begin()+k);
				DeleteWatchPointFromDB(csText);
				break;
			}
		}
	}
}


void CPointDialog::OnLvnItemchangedListAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CPointDialog::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_ListAll.SetFocus();
	if(m_strNameSearch.GetLength()==0)
		return;

	const vector<DataPointEntry>& vecPointVec = GetSearchDataVector(m_strNameSearch);
	ResetPointListInterface(vecPointVec);
	ResetSearchValue(vecPointVec);


	/*
	const vector<DataPointEntry>& vecPointList = m_pGatewayDlg->m_vecServerPointList;
	if(vecPointList.size()<=0 || m_nCurSearchIndex>=vecPointList.size() -1)
		return;

	m_strNameSearch.MakeLower();
	bool bFind = false;
	m_ListAll.SetItemState(m_ListAll.SetSelectionMark(m_nCurSearchIndex),0,LVIS_SELECTED);			//取消其他选中
	for (size_t i=m_nCurSearchIndex+1;i<vecPointList.size();++i)
	{
		const wstring strName =  vecPointList[i].GetShortName();
		CString strCName = strName.c_str();

		strCName.MakeLower();
		if(strCName.Find(m_strNameSearch)>=0)
		{
			m_nCurSearchIndex = i;
			if(m_nCurSearchIndex > m_nSearchMax)
				m_nSearchMax = m_nCurSearchIndex;
			bFind = true;
			break;
		}
	}
	if(!bFind)
	{	// if not find in next, research in previous part
		for (size_t i=0;i<=m_nCurSearchIndex;++i)
		{
			const wstring strName =  vecPointList[i].GetShortName();
			CString strCName = strName.c_str();

			strCName.MakeLower();
			if(strCName.Find(m_strNameSearch)>=0)
			{
				m_nCurSearchIndex = i;
				if(m_nCurSearchIndex > m_nSearchMax)
					m_nSearchMax = m_nCurSearchIndex;
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			m_ListAll.EnsureVisible(m_nSearchMax, FALSE);
			m_ListAll.SetItemState(m_nSearchMax, 0, -1);
			m_ListAll.SetSelectionMark(m_nSearchMax);
			AfxMessageBox(CLanguageSwitch::GetInstance()->GetGlobalString(_T("CPointDialog"),_T("Not_Find")));
		}
		else
		{
			m_ListAll.EnsureVisible(m_nCurSearchIndex, FALSE);
			m_ListAll.SetItemState(m_nCurSearchIndex, LVNI_FOCUSED|LVIS_SELECTED, LVNI_FOCUSED|LVIS_SELECTED);
			m_ListAll.SetSelectionMark(m_nCurSearchIndex);
		}
	}
	else
	{
		m_ListAll.EnsureVisible(m_nCurSearchIndex, FALSE);
		m_ListAll.SetItemState(m_nCurSearchIndex, LVNI_FOCUSED|LVIS_SELECTED, LVNI_FOCUSED|LVIS_SELECTED);
		m_ListAll.SetSelectionMark(m_nCurSearchIndex);
	}
	*/
}

void CPointDialog::OnBnClickedButtonSearchClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strNameSearch = _T("");
	UpdateData(FALSE);
	ResetPointListInterface(m_pGatewayDlg->m_vecServerPointList);
	m_bRefreshOK = true;
}

vector<DataPointEntry> CPointDialog::GetSearchDataVector(CString strSearch)
{
	vector<DataPointEntry> vecRes;
	vector<DataPointEntry> vecPointList(m_pGatewayDlg->m_vecServerPointList);
	vector<DataPointEntry>::const_iterator	iter;

	strSearch.MakeLower();

	const CString strBlank(_T(" "));
	vector<CString> vecSearch;
	vector<CString>::const_iterator itSearch;
	BOOL bSingle = TRUE;

	CString strSearchEx(strSearch);
	strSearchEx.TrimRight(strBlank);
	strSearchEx += strBlank;
	while (TRUE)
	{
		int nPos = strSearchEx.Find(strBlank);
		if (-1 == nPos)
		{
			break;
		}
		CString strItem(strSearchEx.Left(nPos));
		if (_T("") != strItem)
		{
			vecSearch.push_back(strItem);
		}
		strSearchEx = strSearchEx.Mid(nPos+1);
	}
	bSingle = (vecSearch.size() == 1) ? TRUE : FALSE;

	if (bSingle)
	{
		for (iter=vecPointList.begin(); iter!=vecPointList.end(); iter++)
		{
			CString strCName;
			strCName.Format(_T("%s"), (iter->GetShortName()).c_str());
			strCName.MakeLower();
			if (strCName.Find(strSearch) >= 0)
			{
				vecRes.push_back(*iter);
			}
		}
	}
	else
	{
		for (iter=vecPointList.begin(); iter!=vecPointList.end(); iter++)
		{
			CString strCName;
			strCName.Format(_T("%s"), (iter->GetShortName()).c_str());
			strCName.MakeLower();

			BOOL bFit = TRUE;
			for (itSearch=vecSearch.begin(); itSearch!=vecSearch.end(); itSearch++)
			{
				if (strCName.Find(*itSearch) < 0)
				{
					bFit = FALSE;
					break;
				}
			}
			if (bFit)
			{
				vecRes.push_back(*iter);
			}
		}
	}

	return vecRes;
}

void CPointDialog::ResetPointListInterface(const vector<DataPointEntry>& vecData)
{
	m_ListAll.DeleteAllItems();
	ASSERT(0 == m_ListAll.GetItemCount());

	vector<DataPointEntry>::const_iterator	iter;
	int i = 0;
	CString strTemp;
	for (iter=vecData.begin(); iter!=vecData.end(); iter++,i++)
	{
		strTemp.Format(_T("%d"), i+1);
		m_ListAll.InsertItem(i, _T(""));
		m_ListAll.SetItemText(i, 0, strTemp);
		m_ListAll.SetItemText(i, 1, iter->GetShortName().c_str());
		m_ListAll.SetItemText(i, 2, iter->GetSourceType().c_str());
	}
}

void CPointDialog::ResetSearchValue(const vector<DataPointEntry>& vecData)
{
	if (NULL == m_pDataAccess)
	{
		return;
	}

	vector<DataPointEntry>::const_iterator	iter;
	int i = 0;
	CString strTemp;
	for (iter=vecData.begin(); iter!=vecData.end(); iter++,i++)
	{
		wstring wstrPtName(iter->GetShortName());
		wstring	wstrVal;
		const bool bRes = m_pDataAccess->GetValue(wstrPtName, wstrVal);
		if (bRes)
		{
			CString strVal(wstrVal.c_str());
			m_ListAll.SetItemText(i, 3, strVal);
		}
	}
}

void CPointDialog::RefreshFromDB()
{
	ReadWatchPointFromDB();
	m_pGatewayDlg->RefreshServerPointList();
	m_ListAll.DeleteAllItems();
	m_ListWatch.DeleteAllItems();
	m_strPointValueList.resize(m_pGatewayDlg->m_vecServerPointList.size(), L"");
	CString strTemp;
	int cur_i = 0;
	for(int i=0;i<m_pGatewayDlg->m_vecServerPointList.size();i++)
	{
		strTemp.Format(L"%d", i+1);
		m_ListAll.InsertItem(i, L"");
		m_ListAll.SetItemText(i,0, strTemp);
		m_ListAll.SetItemText(i,1, m_pGatewayDlg->m_vecServerPointList[i].GetShortName().c_str());
		m_ListAll.SetItemText(i,2, m_pGatewayDlg->m_vecServerPointList[i].GetSourceType().c_str());

		if (IsPushback(m_strWatchPointNameList,m_pGatewayDlg->m_vecServerPointList[i].GetShortName()))
		{	
			strTemp.Format(L"%d", i+1);
			m_ListWatch.InsertItem(cur_i, L"");
			m_ListWatch.SetItemText(cur_i,0, strTemp);
			m_ListWatch.SetItemText(cur_i,1, m_pGatewayDlg->m_vecServerPointList[i].GetShortName().c_str());
			m_ListWatch.SetItemText(cur_i,2, m_pGatewayDlg->m_vecServerPointList[i].GetSourceType().c_str());

			cur_i++;
		}
	}

	strTemp.Format(_T("点数:%d"), m_pGatewayDlg->m_vecServerPointList.size());
	m_StaticPointCount.SetWindowTextW(strTemp);
}


void CPointDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CPointDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
//	CDialog::OnOK();
}
/*
**	函数描述：按钮Export Data的服务函数 
*/
void CPointDialog::OnBnClickedButtonExportRealtime()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			return;
		}
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
		setlocale( LC_ALL, "chs" );

		CStdioFile filehandle;
		if(filehandle.Open(strFilePath, CFile::modeCreate|CFile::modeWrite))
		{
			vector<CRealTimeDataEntry> entrylist;
			m_pDataAccess->ReadRealtimeData_Input(entrylist);

			//filehandle.WriteString(_T("\"time\",\"pointname\",\"value\"\n"));
			CString strOut;
			wstring strTime,strName,strValue;
			for(int i=0; i<entrylist.size(); ++i)
			{
				Project::Tools::SysTime_To_String(entrylist[i].GetTimestamp(),strTime);
				Project::Tools::UTF8ToWideChar(entrylist[i].GetName(),strName);
				strValue = entrylist[i].GetValue();
				strOut.Format(_T("\"%s\",\"%s\",\"%s\"\n"),strTime.c_str(),strName.c_str(),strValue.c_str());
				filehandle.WriteString(strOut);
			}
			filehandle.Close();
			setlocale( LC_ALL, old_locale );
			AfxMessageBox(_T("导出CSV文件成功"));
		}
		else
		{
			setlocale( LC_ALL, old_locale );
			AfxMessageBox(_T("导出CSV文件失败"));
		}	
	}
}

void CPointDialog::OnBnClickedButtonImportrealtime()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgOpenFile(TRUE,NULL,L"*.csv",OFN_HIDEREADONLY|OFN_HIDEREADONLY,L"*.csv");
	if(dlgOpenFile.DoModal()==IDOK)
	{
		CString strFilePath = dlgOpenFile.GetPathName();	
		
		FILE* pfile = fopen(Project::Tools::WideCharToAnsi(strFilePath).c_str(), "r");
		if (pfile)
		{
			fseek(pfile,0,SEEK_END);
			u32 dwsize = ftell(pfile);
			rewind(pfile);
			char* filebuffer = new char[dwsize + 1];
			int nCharSize = sizeof(char);
			memset(filebuffer, 0, (dwsize+1)*nCharSize);
			size_t dwReadSize = fread(filebuffer, nCharSize, dwsize, pfile);
			memset(filebuffer+dwReadSize*nCharSize, 0, (dwsize-dwReadSize)*nCharSize);
			std::map<u32, std::string> StringMap;
			char* pBegin = filebuffer;
			char* pEnd = strchr(filebuffer, '\n');
			u32 uiIndex = 1;
			bool bFirst = true;
			while (pEnd != NULL)
			{
				std::string strbuff;
				strbuff.insert(0, pBegin, pEnd-pBegin);
				if (!strbuff.empty())
				{
					RemoveSubStr("\"",strbuff);
					StringMap[uiIndex] = strbuff;
				}
				pBegin = pEnd + 1;
				pEnd = strchr(pEnd + 1, '\n');
				++uiIndex;
			}
	
			delete[] filebuffer;
			fclose(pfile);
		
			//
			vector<CRealTimeDataEntry> entrylist;
			std::map<u32, std::string>::iterator iter = StringMap.begin();
			string strName_Utf8;
			SYSTEMTIME st;
			for (; iter != StringMap.end(); ++iter)
			{		
				int nColumn = 0;
				wstring strData = Project::Tools::AnsiToWideChar(iter->second.c_str());
				vector<wstring> vecData;
				Project::Tools::SplitStringByChar(strData.c_str(),L',',vecData);
				nColumn = vecData.size();
				if(nColumn == 3)
				{
					CRealTimeDataEntry entry;
					Project::Tools::WideCharToUtf8(vecData[1],strName_Utf8);
					Project::Tools::String_To_SysTime(Project::Tools::WideCharToAnsi(vecData[0].c_str()),st);

					entry.SetName(strName_Utf8);
					entry.SetTimestamp(st);
					entry.SetValue(vecData[2]);
					entrylist.push_back(entry);
				}
				else
				{
					continue;
				}
			}

			if(entrylist.size()<=0)
			{

				AfxMessageBox(_T("CSV解析点数为0个，请检查格式"));
			}

			//插入数据库
			if(m_pDataAccess->UpdatePointDataList(entrylist))
			{
				m_pDataAccess->UpdatePointBuffer(entrylist);	//更新buffer
				CString strTemp;
				strTemp.Format(_T("导入CSV文件成功，成功导入%d个点"), entrylist.size());
				AfxMessageBox(strTemp);
			}
			else
			{
				AfxMessageBox(_T("导入CSV文件失败"));
			}
		}
		else
		{
			AfxMessageBox(_T("打开CSV文件失败"));
		}
	}

}

void CPointDialog::RemoveSubStr( const string& r, string& str )
{
	int rlen = r.length();
	int strlen = str.length();

	if (rlen <= 0 || strlen <=0)
		return;
	int i,j,pos;
	for (i = 0; i < strlen; ++i) {
		pos = i;
		for (j = 0; j < rlen; ++j) {
			if (str[i] != r[j]) {
				break;
			}
			else if ( j == (rlen-1)){
				str.erase(pos,rlen);
				strlen = str.length();// After trimming, the length of string has changed.
				i = -1;
			}
			else
				++i;
		}
	}
}

void CPointDialog::OnNMClickListAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	m_nCurSearchIndex = pNMItemActivate->iItem;
}

static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{ 
	// lParamSort contains a pointer to the list view control.
	CString &lp1 = *((CString *)lParam1);
	CString &lp2 = *((CString *)lParam2);
	int &sort = *(int *)lParamSort;
	if (sort == 0)
	{
		return lp1.CompareNoCase(lp2);
	}
	else
	{
		return lp2.CompareNoCase(lp1);
	}
}

void CPointDialog::OnLvnColumnclickListAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int Length = m_ListAll.GetItemCount();
	CArray<CString,CString> ItemData;
	ItemData.SetSize(Length);
	for (int i = 0; i < Length; i++)
	{
		ItemData[i] = m_ListAll.GetItemText(i,pNMLV->iSubItem);
		m_ListAll.SetItemData(i,(DWORD_PTR)&ItemData[i]);//设置排序关键字
	}
	static int sort = 0;
	static int SubItem = 0;
	if (SubItem != pNMLV->iSubItem)
	{
		sort = 0;
		SubItem = pNMLV->iSubItem;
	}
	else
	{
		if (sort == 0)
		{
			sort = 1;
		}
		else
		{
			sort = 0;
		}
	}
	m_ListAll.SortItems(MyCompareProc,(DWORD_PTR)&sort);//排序
	*pResult = 0;
}




void CPointDialog::OnStnClickedStaticPointcount()
{
	// TODO: Add your control notification handler code here
}


void CPointDialog::OnBnClickedButtonMultiModify()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_ListAll.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}

	vector<int>		vecIndex;
	vector<CString> vecPtName;
	const int nItemSum = m_ListAll.GetItemCount();
	while (pos != NULL)
	{
		int nIndex = m_ListAll.GetNextSelectedItem(pos);
		if (nIndex>=0 && nIndex<nItemSum)
		{
			CString strPtName(m_ListAll.GetItemText(nIndex, 1));
			vecPtName.push_back(strPtName);
			vecIndex.push_back(nIndex);
		}
	}

	CChangeValueDlg dlg(m_pDataAccess, vecPtName.at(0), _T(""), FALSE, m_nSetPointValueTimeout, this);
	dlg.SetMultiName(vecPtName);
	if (IDOK == dlg.DoModal())
	{
		CString	strSetVal(dlg.GetValue());
		vector<int>::const_iterator	iter;
		for (iter=vecIndex.begin(); iter!=vecIndex.end(); iter++)
		{
			m_ListAll.SetItemText(*iter, 3, strSetVal);
		}
	}
}

void CPointDialog::StartScanThread(void)
{
	HANDLE hThread = NULL;
	unsigned threadID = 0;

	hThread = (HANDLE)_beginthreadex(NULL, 0, &ScanPointTableInfo, this, 0, &threadID);
	Sleep(1);
}

void CPointDialog::EndScanThread(void)
{
	DWORD dwWaitResult = WaitForSingleObject(g_hEventScan, 0);
	switch (dwWaitResult)
	{
	case WAIT_OBJECT_0:
		m_bScanRun = false;
		break;
	default:
		TRACE("End wait error (%d)\n", GetLastError());
	}
	SetEvent(g_hEventScan);
	CloseHandle(g_hEventScan);
}

unsigned int WINAPI CPointDialog::ScanPointTableInfo(void* pArguments)
{
	CPointDialog* pDlg = static_cast<CPointDialog*>(pArguments);
	ASSERT(pDlg != NULL);
	if (NULL == pDlg)
	{
		return 0;
	}

	while (pDlg->m_bScanRun)
	{
		TRACE("Scan now\n");
		DWORD dwWaitResult = WaitForSingleObject(g_hEventScan, INFINITE);
		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			pDlg->MonitorPointsInfo();
			break;
		default:
			SetEvent(g_hEventScan);
			TRACE("Scan wait error (%d)\n", GetLastError());
			return 0;
		}
		SetEvent(g_hEventScan);
		Sleep(10000);
	}
	return 0;
}

void CPointDialog::MonitorPointsInfo(void)
{
	if (NULL == m_pDataAccess || NULL == m_pGatewayDlg)
	{
		return;
	}

	m_vecCheck.clear();
	m_pDataAccess->ReadServerPointsList(m_vecCheck);
	bool bNoChange = CheckPointsInfo(m_pGatewayDlg->m_vecServerPointList, m_vecCheck);
	if (!bNoChange)
	{
		m_pGatewayDlg->m_vecServerPointList = m_vecCheck;

		m_ListAll.DeleteAllItems();
		m_ListWatch.DeleteAllItems();
		m_strPointValueList.resize(m_pGatewayDlg->m_vecServerPointList.size(), L"");
		CString strTemp;
		int cur_i = 0;
		for(int i=0;i<m_pGatewayDlg->m_vecServerPointList.size();i++)
		{
			strTemp.Format(L"%d", i+1);
			m_ListAll.InsertItem(i, L"");
			m_ListAll.SetItemText(i,0, strTemp);
			m_ListAll.SetItemText(i,1, m_pGatewayDlg->m_vecServerPointList[i].GetShortName().c_str());
			m_ListAll.SetItemText(i,2, m_pGatewayDlg->m_vecServerPointList[i].GetSourceType().c_str());

			if (IsPushback(m_strWatchPointNameList,m_pGatewayDlg->m_vecServerPointList[i].GetShortName()))
			{	
				strTemp.Format(L"%d", i+1);
				m_ListWatch.InsertItem(cur_i, L"");
				m_ListWatch.SetItemText(cur_i,0, strTemp);
				m_ListWatch.SetItemText(cur_i,1, m_pGatewayDlg->m_vecServerPointList[i].GetShortName().c_str());
				m_ListWatch.SetItemText(cur_i,2, m_pGatewayDlg->m_vecServerPointList[i].GetSourceType().c_str());
				cur_i++;
			}
		}

		strTemp.Format(_T("点数:%d"), m_pGatewayDlg->m_vecServerPointList.size());
		m_StaticPointCount.SetWindowTextW(strTemp);
	}
}

bool CPointDialog::CheckPointsInfo(const vector<DataPointEntry>& vec1, const vector<DataPointEntry>& vec2)
{
	size_t nSize1 = vec1.size();
	size_t nSize2 = vec2.size();
	if (nSize1 != nSize2)
	{
		return false;
	}

	vector<DataPointEntry>::const_iterator iter1, iter2;
	iter1 = vec1.begin();
	iter2 = vec2.begin();

	while (iter1 != vec1.end() 
		&& iter2 != vec2.end() 
		&& iter1->GetShortName() == iter2->GetShortName())
	{
		++iter1;
		++iter2;
	}
	if (iter1 != vec1.end())
	{
		return false;
	}

	return true;
}


void CPointDialog::OnBnClickedButtonHistory()
{
	// TODO: Add your control notification handler code here
	

	POSITION pos = m_ListAll.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}

	vector<int>		vecIndex;
	vector<CString> vecPtName;
	const int nItemSum = m_ListAll.GetItemCount();
	while (pos != NULL)
	{
		int nIndex = m_ListAll.GetNextSelectedItem(pos);
		if (nIndex>=0 && nIndex<nItemSum)
		{
			CString strPtName(m_ListAll.GetItemText(nIndex, 1));
			vecPtName.push_back(strPtName);
			vecIndex.push_back(nIndex);
		}
	}

	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(pFrame->GetActiveView());
	if(pView && m_pDataAccess != NULL)
	{
		CPointHisdataDlg dlg;

		dlg.m_DateFrom = m_HisQueryDateFrom;
		dlg.m_DateTo = m_HisQueryDateTo;
		dlg.m_TimeFrom = m_HisQueryTimeFrom;
		dlg.m_TimeTo = m_HisQueryTimeTo;
		dlg.m_nTimePeriod = m_HisQueryTimePeriod;

		dlg.m_strServer = m_pDataAccess->GetServerAddress();
		dlg.m_nServerPort = m_pDataAccess->GetServerPort();
		dlg.m_strPointNameList = vecPtName;
		dlg.DoModal();
		m_HisQueryDateFrom  =dlg.m_DateFrom     ;
		m_HisQueryDateTo    =dlg.m_DateTo     ;
		m_HisQueryTimeFrom  =dlg.m_TimeFrom       ;
		m_HisQueryTimeTo    =dlg.m_TimeTo       ;
		m_HisQueryTimePeriod=dlg.m_nTimePeriod    ;


	}


	

}
