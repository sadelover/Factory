// PointView.cpp : 实现文件
//
#include "stdafx.h"
#include "SetParameterDlg.h"
#include "BEOP-Designer.h"
#include "../ServerDataAccess/ServerDataAccess.h"
#include <sstream>
#include "Tools/Util/UtilString.h"
#include "SelectPointDlg.h"
#include "ControlEx/WarningDlg.h"
#include "SetConstValueDlg.h"
#include "BEOPLogicManager.h"
#include "InputLogicPeriodDlg.h"
#include "LogicDllThread.h"
#include "ReplacePointName.h"
#include "json/json.h"
#include "HttpOperation.h"
#include "MainFrm.h"

static const int STATIC_SCREEN_WIDTH = 1024;
static const int STATIC_SCREEN_HEIGHT = 768;

// CPointView
IMPLEMENT_DYNAMIC(SetParameterDlg, CDialog)

BEGIN_MESSAGE_MAP(SetParameterDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDOK, &SetParameterDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SelectParameterInput, &SetParameterDlg::OnNMDblclkListSelectparameterinput)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SelectParameterOutput, &SetParameterDlg::OnNMDblclkListSelectparameteroutput)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SelectParameterInput, &SetParameterDlg::OnLvnItemchangedListSelectparameterinput)
	ON_BN_CLICKED(IDCANCEL, &SetParameterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_THREAD, &SetParameterDlg::OnBnClickedButtonModifyThread)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_STR, &SetParameterDlg::OnBnClickedButtonReplaceStr)
	ON_BN_CLICKED(IDC_BUTTON1, &SetParameterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &SetParameterDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

SetParameterDlg::SetParameterDlg(CBEOPLogicManager *pLogicManager,CUPPCServerDataAccess *pDataAccess, CWnd* pParent /*=NULL*/)
	: CDialog(SetParameterDlg::IDD, pParent)
	, m_strCurSelDll(_T(""))
	, m_strIntroduction(_T(""))
	,m_pLogicManager(pLogicManager)
	,m_pDataAccess(pDataAccess)
	, m_strThreadName(_T(""))
	, m_strDllDeclare(_T(""))
	,m_strCalcIp(L"127.0.0.1")
	,m_nCalcPort(5000)
	,m_pThread(NULL)
{
	m_strWatchPointLineList.clear();
	m_nIndex        = 0;
	m_hEventClose_Point   = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_strParamPointNameList.clear();
	m_nthreadState = kThreadState::RUN;
}

SetParameterDlg::~SetParameterDlg()
{
	//KillTimer(1);
	TerminateThread(m_pThread->m_hThread,0);
}

void SetParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SelDLL, m_strCurSelDll);
	DDX_Control(pDX, IDC_LIST_SelectParameterOutput, m_OutputParaList);
	DDX_Control(pDX, IDC_LIST_SelectParameterInput, m_InputParaList);

	DDX_Text(pDX, IDC_EDIT_DllJianJie, m_strIntroduction);
	DDX_Control(pDX, IDC_COMBO_THREADLIST, m_comboThread);
	//DDX_CBString(pDX, IDC_COMBO_THREADLIST, m_strThreadName);
	DDX_Text(pDX, IDC_EDIT_DllShuoMing, m_strDllDeclare);
}
/*
	CLoadingDlg message handlers
	界面初始化
*/

BOOL SetParameterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

//	m_pointmanager->Init();
	
	m_OutputParaList.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT|LVS_EDITLABELS); 
	m_OutputParaList.InsertColumn(0,L"编号",LVCFMT_LEFT,40,0);
	m_OutputParaList.InsertColumn(1,L"参数名称",LVCFMT_LEFT,100,0);
	m_OutputParaList.InsertColumn(2,L"参数值(点位)",LVCFMT_LEFT,120,0); 
	m_OutputParaList.InsertColumn(3,L"输出条件(点位)",LVCFMT_LEFT,100,0); 
	m_OutputParaList.InsertColumn(4,L"类型",LVCFMT_LEFT,60,0); 
	m_OutputParaList.InsertColumn(5,L"当前值",LVCFMT_LEFT,60,0);
	m_OutputParaList.InsertColumn(6,L"意义",LVCFMT_LEFT,200,0);
	m_OutputParaList.InsertColumn(7,L"类型可改",LVCFMT_LEFT,30,0);
	
	m_InputParaList.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
	m_InputParaList.InsertColumn(0,L"编号",LVCFMT_LEFT,40,0);
	m_InputParaList.InsertColumn(1,L"参数名称",LVCFMT_LEFT,80,0);
	m_InputParaList.InsertColumn(2,L"参数值(点位)",LVCFMT_LEFT,80,0); 
	m_InputParaList.InsertColumn(3,L"类型",LVCFMT_LEFT,60,0); 
	m_InputParaList.InsertColumn(4,L"当前值",LVCFMT_LEFT,60,0);
	m_InputParaList.InsertColumn(5,L"意义",LVCFMT_LEFT,200,0); 
	m_InputParaList.InsertColumn(6,L"类型可改",LVCFMT_LEFT,30,0);
	//添加线程列表内容
	int nThreadIndexInCombox = 0;
	for(int i=0; i<m_pLogicManager->m_vecDllThreadList.size(); i++)
	{
		wstring & threadName = m_pLogicManager->m_vecDllThreadList[i]->GetName();
		m_comboThread.InsertString(i, threadName.c_str());
		if(threadName == m_strThreadName.GetString()){
			nThreadIndexInCombox = i;
		}
	}
	m_comboThread.SetCurSel(nThreadIndexInCombox);

	int  iInputParaCount = m_vecCurDllInputParameterList.size();
	m_InputParaList.ShowWindow(iInputParaCount>0);


	vector<wstring> vecPtName;
	CString strTemp;
	int cur_i = 0;
	for(int i=0;i<iInputParaCount;i++)
	{
		strTemp.Format(L"%d", i+1);
		m_InputParaList.InsertItem(i, L"");
		m_InputParaList.SetItemText(i,0, strTemp);
		m_InputParaList.SetItemText(i,1, m_vecCurDllInputParameterList[i][0].c_str());
		m_InputParaList.SetItemText(i,2, m_vecCurDllInputParameterList[i][1].c_str());
		wstring wstrType = m_vecCurDllInputParameterList[i][2];
		if(wstrType==L"strconst")
			wstrType=L"const";
		m_InputParaList.SetItemText(i,3, wstrType.c_str());
		m_InputParaList.SetItemText(i,5, m_vecCurDllInputParameterList[i][3].c_str());
		if(m_vecCurDllInputParameterList[i].size()>=5)
			m_InputParaList.SetItemText(i,6, m_vecCurDllInputParameterList[i][4].c_str());
		else
		{
			m_InputParaList.SetItemText(i,6, L"1");
		}

		
		if (_T("point") == wstrType)
		{
			vecPtName.push_back(m_vecCurDllInputParameterList[i][1]);
		}
		else if (_T("const") == wstrType || _T("strconst") == wstrType)
		{
			m_InputParaList.SetItemText(i, 4, m_vecCurDllInputParameterList[i][1].c_str());
		}
	}

	// read current input value
	if (m_pDataAccess != NULL)
	{
		map<wstring, float> mapRes;
		map<wstring, float>::const_iterator iter;
		if (m_pDataAccess->ReadRealTimeDataInputValue(vecPtName, mapRes))
		{	// set spec col
			for (iter=mapRes.begin(); iter!=mapRes.end(); iter++)
			{
				SetInputListCurrentValue(iter->first, iter->second);
			}
		}
	}

	int  iOutputParaCount = m_vecCurDllOutputParameterList.size();
	m_OutputParaList.ShowWindow(iOutputParaCount>0);
	strTemp;
	cur_i = 0;
	for(int i=0;i<iOutputParaCount;i++)
	{
		strTemp.Format(L"%d", i+1);
		m_OutputParaList.InsertItem(i, L"");
		m_OutputParaList.SetItemText(i,0, strTemp);
		m_OutputParaList.SetItemText(i,1, m_vecCurDllOutputParameterList[i][0].c_str());
		m_OutputParaList.SetItemText(i,2, m_vecCurDllOutputParameterList[i][1].c_str());

		m_OutputParaList.SetItemText(i,3, m_vecCurDllOutputParameterList[i][4].c_str());

		m_OutputParaList.SetItemText(i,4, m_vecCurDllOutputParameterList[i][2].c_str());
		m_OutputParaList.SetItemText(i,6, m_vecCurDllOutputParameterList[i][3].c_str());
		wstring wstrType = m_vecCurDllOutputParameterList[i][2];
		if(wstrType==L"strconst")
			wstrType=L"const";
		if (_T("point") == wstrType)
		{
			vecPtName.push_back(m_vecCurDllOutputParameterList[i][1]);
		}
		else if (_T("const") == wstrType || _T("strconst") == wstrType)
		{
			m_OutputParaList.SetItemText(i, 5, m_vecCurDllOutputParameterList[i][1].c_str());
		}
		

		
	}
	//拿到未更改的线程名
	m_strOriginalThreadName = m_strThreadName;
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	m_strCalcIp = pFrame->m_strCalcIp;
	m_nCalcPort = pFrame->m_nCalcPort;
	m_pThread = AfxBeginThread(Thread_OutputPointValue_RealTime,(LPVOID)this);
	return TRUE;
}

void SetParameterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

//	CDialog::OnOK();
}

//修改数据 并存入mysql
void SetParameterDlg::OnNMDblclkListSelectparameterinput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_nthreadState = kThreadState::STOP;
	POSITION pos = m_InputParaList.GetFirstSelectedItemPosition(); 
	int nItem = m_InputParaList.GetNextSelectedItem(pos); 
	if(nItem>m_vecCurDllInputParameterList.size()-1)
		return;


	CString strValue  = m_InputParaList.GetItemText(nItem,2);
	CString strVName  = m_InputParaList.GetItemText(nItem,1);
	CString strVType  = m_InputParaList.GetItemText(nItem,3);
	CString strVExp	  = m_InputParaList.GetItemText(nItem,5);
	CString strCanTypeModify	  = m_InputParaList.GetItemText(nItem,6);

	CSetConstValueDlg csv;
	std::vector<CString> allPointDescr;
	std::vector<int>	 allPointIndex;
	vector<DataPointEntry>  m_pointlist = m_pointmanager->GetPointList();
	const size_t size = m_pointlist.size();
	for(size_t i=0;i<size;++i)
	{
		wstring wstrName = m_pointlist[i].GetShortName();
		CString strRemarkName = wstrName.c_str();
		allPointDescr.push_back(strRemarkName);
		int nIndex = m_pointlist[i].GetPointIndex();
		allPointIndex.push_back(nIndex);
	}
	csv.vecPointList = allPointDescr;
	csv.vecPointIndex = allPointIndex;

	
	csv.m_dSelCurValue = strValue;
	csv.m_strSetValue  = strValue;
	csv.m_sSelStrName  = strVName;
	csv.m_ConValue     = _T("1");
	csv.m_sValueType   = strVType;

	if(strCanTypeModify==L"0")
	{
		csv.m_bCanModifyType = false;
	}
	else
	{
		csv.m_bCanModifyType = true;
	}
//////////////////////////////////////////////////////////////////////////
	//获取结果
	if (csv.DoModal() == IDOK)
	{
		strVType = csv.m_sValueType;
		if (true/*strVType == _T("strconst")*/)
		{
			strValue = csv.m_strSetValue;
			m_InputParaList.SetItemText(nItem,2,strValue);
			m_InputParaList.SetItemText(nItem,3, strVType);

			//m_InputParaList.SetItemText(nItem,4, strVName+_T("值"));
		}
		else
		{
			int nValue = 0;
			BOOL ConstValue = UtilString::CheckStringIsNumber(csv.m_strSetValue.GetString(), nValue);
			if (ConstValue)
			{
				strValue = csv.m_strSetValue;
				m_InputParaList.SetItemText(nItem,2,strValue);
				m_InputParaList.SetItemText(nItem,3, strVType);
				
				m_InputParaList.SetItemText(nItem,5, strVName+_T("值"));
			}else
			{		
				strValue = csv.m_strSetValue;
				DataPointEntry entry = m_pointmanager->GetEntry(strValue.GetString());
				wstring		strPointName	= entry.GetShortName();
				wstring		strPointRemark	= entry.GetDescription();
				wstring		strSourceType	= entry.GetSourceType();

				m_InputParaList.SetItemText(nItem,2, strPointName.c_str());
				m_InputParaList.SetItemText(nItem,3, strSourceType.c_str());
				m_InputParaList.SetItemText(nItem,4, strPointName.c_str());
				m_InputParaList.SetItemText(nItem,5, strPointRemark.c_str());

			}
		}


		m_InputParaList.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		m_InputParaList.SetFocus();

		SaveParameterToDBAndServer(m_strCurSelDll,L"0", strVName, csv.m_strSetValue,csv.m_sValueType,L"");
	}
	UpdateData(FALSE);
	m_nthreadState = kThreadState::RUN;
}


void SetParameterDlg::OnNMDblclkListSelectparameteroutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_nthreadState = kThreadState::STOP;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	POSITION pos = m_OutputParaList.GetFirstSelectedItemPosition(); 
	int nItem = m_OutputParaList.GetNextSelectedItem(pos); 

	if(nItem>m_vecCurDllOutputParameterList.size()-1)
		return;


	CString strValue  = m_OutputParaList.GetItemText(nItem,2);
	CString strVName  = m_OutputParaList.GetItemText(nItem,1);
	CString strVType  = m_OutputParaList.GetItemText(nItem,4);
	CString ConValue  = m_OutputParaList.GetItemText(nItem,3);

	CSetConstValueDlg csv;
	std::vector<CString> allPointDescr;
	std::vector<int>	 allPointIndex;
	vector<DataPointEntry>  m_pointlist = m_pointmanager->GetPointList();
	const size_t size = m_pointlist.size();
	for(size_t i=0;i<size;++i)
	{
		wstring wstrName = m_pointlist[i].GetShortName();
		CString strRemarkName = wstrName.c_str();
		allPointDescr.push_back(strRemarkName);
		int nIndex = m_pointlist[i].GetPointIndex();
		allPointIndex.push_back(nIndex);
	}
	csv.vecPointList = allPointDescr;
	csv.vecPointIndex = allPointIndex;
	
	csv.m_sValueType = strVType;
	csv.m_dSelCurValue  = strValue;
	csv.m_strSetValue	= strValue;
	csv.m_sSelStrName   = strVName;
	csv.m_ConValue   = ConValue;
	csv.m_bCanModifyType = true;
	//////////////////////////////////////////////////////////////////////////
	//获取结果
	if (csv.DoModal() == IDOK)
	{
		strVType = csv.m_sValueType;
		int nValue = 0;
		BOOL ConstValue = UtilString::CheckStringIsNumber(csv.m_strSetValue.GetString(), nValue);
		CString  Condion_config =  csv.m_ConValue;
		if (Condion_config == _T(""))
		{
			Condion_config = _T("1");
		}
		if (ConstValue)
		{
			strValue = csv.m_strSetValue;

			m_OutputParaList.SetItemText(nItem,2,strValue);
			m_OutputParaList.SetItemText(nItem,3, Condion_config);
			m_OutputParaList.SetItemText(nItem,4, strVType);

			m_OutputParaList.SetItemText(nItem,6, strVName+_T("值"));
		}else
		{
			strValue = csv.m_strSetValue;
				DataPointEntry entry = m_pointmanager->GetEntry(strValue.GetString());
				wstring		strPointName	= entry.GetShortName();
				wstring		strPointRemark	= entry.GetDescription();
				wstring		strSourceType	= entry.GetSourceType();

				m_OutputParaList.SetItemText(nItem,2, strPointName.c_str());
				m_OutputParaList.SetItemText(nItem,4, strVType);
				m_OutputParaList.SetItemText(nItem,6, strPointRemark.c_str());
			

			m_OutputParaList.SetItemText(nItem,3, Condion_config);
		}
	
		SaveParameterToDBAndServer(m_strCurSelDll,L"1", strVName, csv.m_strSetValue,csv.m_sValueType,Condion_config);

		m_OutputParaList.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		m_OutputParaList.SetFocus();
		UpdateData(FALSE);
	}
	m_nthreadState = kThreadState::RUN;
}


void SetParameterDlg::OnLvnItemchangedListSelectparameterinput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void SetParameterDlg::SaveParameterToDBAndServer(CString strDllName,  CString strSetType, CString strVariableName, CString strLinkDefine, CString strLinkType, CString strCondition)
{
	if(strSetType==L"2")
	{//设置调用周期
		double fPeriod = _wtof(strVariableName.GetString());
		if(fPeriod>0.f)
		{
			m_pLogicManager->SaveSpanTimetoDB(strDllName, fPeriod);
		}
		//pDLLObject->SetTimeSpan(ParamDlg.m_dNewSpanTime);
		//保存周期到数据库
	}
	else if(strSetType==L"0")
	{
		m_pLogicManager->SaveParametertoDB(0, strDllName, strVariableName, strLinkDefine, strLinkType, strCondition);
	}
	else if(strSetType==L"1")
	{
		m_pLogicManager->SaveParametertoDB(1, strDllName, strVariableName, strLinkDefine, strLinkType, strCondition);
	}

	//更新到在线数据库
	if (NULL == m_pDataAccess)
	{
		MessageBox(_T("请先连接数据库！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}
	else
	{
		m_pDataAccess->InsertLogicParameters(strDllName.GetString(), strDllName.GetString(), strSetType.GetString(), strVariableName.GetString(), strLinkDefine.GetString(), strLinkType.GetString(), strCondition.GetString());
	}
	
	//m_ControlLogicList.SetItemText(iTem,0, ParamDlg.m_strCurSelDll);
}



void SetParameterDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void SetParameterDlg::OnBnClickedButtonModifyThread()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_comboThread.GetLBText(m_comboThread.GetCurSel(),m_strThreadName);
	m_pLogicManager->SaveLogicThreadNameToDB(m_strCurSelDll, m_strThreadName);
}

void SetParameterDlg::SetInputListCurrentValue(const wstring wstrName, const float fVal)
{
	CString strName;
	CString strVal;
	strName.Format(L"%s", wstrName.c_str());
	strVal.Format(L"%0.3f", fVal);

	int nSum = m_InputParaList.GetItemCount();
	for (int i=0; i<nSum; i++)
	{
		CString strType = m_InputParaList.GetItemText(i, 3);
		if (m_InputParaList.GetItemText(i, 2) == strName && strType==_T("point"))
		{
			m_InputParaList.SetItemText(i, 4, strVal);
			break;
		}
	}
}


BOOL SetParameterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x80;
		switch (pMsg->wParam)
		{
		case 'c':
		case 'C':
			if (bCtrl)
			{
				CString strVal(GetListCtrlSelectedValue());
				SetClipboardCustomValue(strVal);
			}  
			break;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL SetParameterDlg::SetClipboardCustomValue(const CString strVal)
{
	char buf[50000] = {0};
	USES_CONVERSION;
	strcpy_s(buf, 50000, T2A((LPTSTR)(LPCTSTR)strVal));

	DWORD dwLength = strlen(buf);
	HANDLE hGlobalMemory = GlobalAlloc(GHND, dwLength + 1);
	if (NULL == hGlobalMemory)
	{
		return FALSE;
	}
	LPBYTE lpGlobalMemory = (LPBYTE)GlobalLock(hGlobalMemory);
	if (NULL == lpGlobalMemory)
	{
		return FALSE;
	}

	memcpy(lpGlobalMemory, buf, dwLength);
	lpGlobalMemory[dwLength] = '\0';

	GlobalUnlock(hGlobalMemory);
	HWND hWnd = GetSafeHwnd();
	if (NULL == hWnd)
	{
		return FALSE;
	}

	::OpenClipboard(hWnd);
	::EmptyClipboard();
	::SetClipboardData(CF_TEXT, hGlobalMemory);
	::CloseClipboard();
	return TRUE;
}

CString	SetParameterDlg::GetListCtrlSelectedValue(void)
{
	CString strRet;
	CString strEnter(_T("\r\n"));
	POSITION pos = m_InputParaList.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		TRACE(_T("No items were selected!\n"));
		return strRet;
	}

	while (pos != NULL)
	{
		int nItem = m_InputParaList.GetNextSelectedItem(pos);
		TRACE(_T("Item %d was selected!\n"), nItem);
		CString str1 = m_InputParaList.GetItemText(nItem, 0);
		CString str2 = m_InputParaList.GetItemText(nItem, 1);
		CString str3 = m_InputParaList.GetItemText(nItem, 2);
		CString str4 = m_InputParaList.GetItemText(nItem, 3);
		CString str5 = m_InputParaList.GetItemText(nItem, 4);
		CString str6 = m_InputParaList.GetItemText(nItem, 5);
		CString strTmp;
		strTmp.Format(_T("%s\t%s\t%s\t%s\t%s\t%s%s"), str1, str2, str3, str4, str5, str6, strEnter);
		strRet += strTmp;
	}
	return strRet;
}


void SetParameterDlg::OnBnClickedButtonReplaceStr()
{
	// TODO: Add your control notification handler code here
	CReplacePointName replaceNameDlg;
	if(replaceNameDlg.DoModal()==IDOK)
	{

	}
}


void SetParameterDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//保存策略说明
	m_pLogicManager->SaveDeclareParameterToDB(m_strCurSelDll, m_strDllDeclare, m_strDllOriginName, m_strOriginalThreadName);
	//保存所在线程
	if(m_strThreadName != m_strOriginalThreadName)
	{
		m_pLogicManager->SaveLogicThreadNameToDB(m_strCurSelDll, m_strThreadName);
	}
	OnCancel();
}


void SetParameterDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

UINT SetParameterDlg::Thread_OutputPointValue_RealTime(LPVOID lpParameter)
{
	SetParameterDlg* pThis = (SetParameterDlg*)lpParameter;
	CListCtrl& outListCtl = pThis->m_OutputParaList;
	CUPPCServerDataAccess*	pDataAccess = pThis->m_pDataAccess;
	while(true)
	{
		if(pThis == NULL){
			return 0;
		}
		int nRowMax = outListCtl.GetItemCount();
		for(int i=0; i<nRowMax; i++)
		{	
			if(kThreadState::STOP == pThis->m_nthreadState)
			{
				Sleep(3000);
				continue;
			}

			if(pDataAccess==NULL)
				continue;

			//获取点名
			wstring wstrName(outListCtl.GetItemText(i, 2).GetString());
			if(wstrName.size() == 0){
				continue;
			}

			//点名或脚本
			if(wstrName.find(L"<%") == EOF)
			{
				if(false == IsPointNameValid(wstrName.c_str())){
					continue;
				}

				wstring wstrValue;
				const bool bRes = pDataAccess->GetValue(wstrName, wstrValue);
				if(bRes)
				{
					//设置当前值列
					try{
						outListCtl.SetItemText(i,5, wstrValue.c_str());
					}
					catch (CException* e){
					}	
				}	
			}
			else
			{
				//脚本计算
				CString cstrValue;
				Json::Value myRoot;
				myRoot["str"] = Project::Tools::WideCharToUtf8(wstrName.c_str());
				string szBuf = myRoot.toStyledString();
				CString cstrResult;
				CHttpOperation http(pThis->m_strCalcIp, pThis->m_nCalcPort, _T("/tool/evalStringExpression"));
				bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), cstrResult,  _T("Content-Type: application/json"));//
				if(bRet)
				{
					Json::Reader jsReader;
					string strResult_utf8;
					Project::Tools::WideCharToUtf8(cstrResult.GetString(), strResult_utf8);
					Json::Value myReturnRoot;
					if (!jsReader.parse(strResult_utf8, myReturnRoot))
					{
						continue;
					}
					else
					{
						Json::Value &dataObj = myReturnRoot["err"];
						if(dataObj.isInt())
						{
							int nErr = dataObj.asInt();
							
							if(nErr==1)
							{
								cstrValue = L"计算失败";
							}
							else
							{
								Json::Value &dataObj = myReturnRoot["data"];
								if(dataObj.isDouble())
								{
									cstrValue.Format(_T("%.4f"), dataObj.asDouble());
								}
								else if (dataObj.isInt())
								{
									cstrValue.Format(_T("%d"), dataObj.asInt());
								}else if (dataObj.isString())
								{
									wstring wstr = Project::Tools::AnsiToWideChar(dataObj.asString().data());
									cstrValue.Format(_T("%s"), wstr.c_str());
								}
							}
						}
					}
				}
				else{
					cstrValue = L" ";
				}
				//设置当前值列
				try{
					outListCtl.SetItemText(i,5,cstrValue);
				}
				catch (CException* e){
				}	
			}//END 脚本计算
		}//END 列表循环
		Sleep(3000);
	}
}
