// ProjectCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectCheckDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "BEOP-DesignerView.h"
#include "TreeManageDlg.h"
// CProjectCheckDlg dialog

IMPLEMENT_DYNAMIC(CProjectCheckDlg, CSkinDlg)

CProjectCheckDlg::CProjectCheckDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CProjectCheckDlg::IDD, pParent)
	, m_pProjectSqlite(NULL)
	, m_nShowMode(0)
{

}

CProjectCheckDlg::~CProjectCheckDlg()
{
}

void CProjectCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ERR_INFO, m_listErrInfo);
	DDX_Control(pDX, IDC_BUTTON_CHECK_IMAGE, m_btnCheckImage);
	DDX_Control(pDX, IDC_BUTTON_CHECK_POINT, m_btnCheckPoint);
	DDX_Control(pDX, IDC_BUTTON_CHECK_OVERLOAP, m_btnCheckOver);
	DDX_Control(pDX, IDC_BUTTON_EXPORT, m_btnExport);
}


BEGIN_MESSAGE_MAP(CProjectCheckDlg, CSkinDlg)
	ON_BN_CLICKED(IDC_BUTTON_CHECK_IMAGE, &CProjectCheckDlg::OnBnClickedButtonCheckImage)
	ON_BN_CLICKED(IDC_BUTTON_CHECK_POINT, &CProjectCheckDlg::OnBnClickedButtonCheckPoint)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CProjectCheckDlg::OnBnClickedButtonExport)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ERR_INFO, &CProjectCheckDlg::OnLvnItemchangedListErrInfo)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ERR_INFO, &CProjectCheckDlg::OnNMDblclkListErrInfo)
	ON_BN_CLICKED(IDC_BUTTON_CHECK_OVERLOAP, &CProjectCheckDlg::OnBnClickedButtonCheckOverloap)
END_MESSAGE_MAP()


// CProjectCheckDlg message handlers


void CProjectCheckDlg::OnBnClickedButtonCheckImage()
{
	// TODO: Add your control notification handler code here
	if(m_pProjectSqlite)
	{
		m_nShowMode = 0;
		UpdateControlState(false);
		SetWindowText(_T("正在组态校验(图片)...."));
		HANDLE handle = (HANDLE)_beginthreadex(NULL,0, ThreadFunc, this, NORMAL_PRIORITY_CLASS, NULL);
		CloseHandle(handle);
	}
}

void CProjectCheckDlg::OnBnClickedButtonCheckPoint()
{
	// TODO: Add your control notification handler code here
	if(m_pProjectSqlite)
	{
		m_nShowMode = 1;
		UpdateControlState(false);
		SetWindowText(_T("正在组态校验(点表)...."));
		HANDLE handle = (HANDLE)_beginthreadex(NULL,0, ThreadFunc, this, NORMAL_PRIORITY_CLASS, NULL);
		CloseHandle(handle);
	}
}

void CProjectCheckDlg::OnBnClickedButtonExport()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE, L"csv", L"exportdefault",
		OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,
		L"csv File(*.csv)|*.csv||", this); 
	if (dlg.DoModal() == IDOK)
	{
		CString filename = dlg.GetPathName();
		setlocale( LC_CTYPE, ("chs"));
		CStdioFile filehandle;
		if(filehandle.Open(filename, CFile::modeCreate|CFile::modeWrite))
		{
			const CString cstrLineEnd = L"\n";
			const CString cstrComma = L",";
			int columncount = m_listErrInfo.GetHeaderCtrl()->GetItemCount();
			for (unsigned int i = 0; i < columncount; i++){
				HDITEM hdi;
				TCHAR  lpBuffer[256];
				bool   fFound = false;

				hdi.mask = HDI_TEXT;
				hdi.pszText = lpBuffer;
				hdi.cchTextMax = 256;
				m_listErrInfo.GetHeaderCtrl()->GetItem(i, &hdi);
				filehandle.WriteString(lpBuffer);
				filehandle.WriteString(cstrComma);
			}
			filehandle.WriteString(cstrLineEnd);

			for (unsigned int i = 0; i < m_listErrInfo.GetItemCount(); i++)
			{
				for (unsigned int j = 0; j < columncount; j++){
					CString itemdata = m_listErrInfo.GetItemText(i, j);
					filehandle.WriteString(itemdata.GetString());
					filehandle.WriteString(cstrComma);
				}
				filehandle.WriteString(cstrLineEnd);
			}

			filehandle.Close();
		}
	}
}

void CProjectCheckDlg::SetProjectSqlPointer( CProjectSqlite* pProjectSqlite )
{
	m_pProjectSqlite = pProjectSqlite;
}

void CProjectCheckDlg::RefreshList( vector<ProjectCheckInfo> vecCheck )
{
	m_listErrInfo.DeleteAllItems();
	while(m_listErrInfo.DeleteColumn(0));

	if(m_nShowMode == 0)
	{
		m_listErrInfo.InsertColumn(0,_T("页面ID"),LVCFMT_LEFT,100);
		m_listErrInfo.InsertColumn(1,_T("页面名称"),LVCFMT_LEFT,200);
		m_listErrInfo.InsertColumn(2,_T("元素ID"),LVCFMT_LEFT,100);
		m_listErrInfo.InsertColumn(3,_T("图片ID"),LVCFMT_LEFT,100);
		m_listErrInfo.InsertColumn(4,_T("错误信息"),LVCFMT_LEFT,100);
		SetWindowText(_T("组态校验(图片)"));
	}
	else if(m_nShowMode == 1)
	{
		m_listErrInfo.InsertColumn(0,_T("页面ID"),LVCFMT_LEFT,100);
		m_listErrInfo.InsertColumn(1,_T("页面名称"),LVCFMT_LEFT,200);
		m_listErrInfo.InsertColumn(2,_T("元素ID"),LVCFMT_LEFT,100);
		m_listErrInfo.InsertColumn(3,_T("点位名称"),LVCFMT_LEFT,100);
		m_listErrInfo.InsertColumn(4,_T("错误信息"),LVCFMT_LEFT,100);
		SetWindowText(_T("组态校验(点位)"));
	}
	else if(m_nShowMode == 2)
	{
		m_listErrInfo.InsertColumn(0,_T("页面ID"),LVCFMT_LEFT,100);
		m_listErrInfo.InsertColumn(1,_T("页面名称"),LVCFMT_LEFT,200);
		m_listErrInfo.InsertColumn(2,_T("元素ID"),LVCFMT_LEFT,100);
		m_listErrInfo.InsertColumn(3,_T("错误信息"),LVCFMT_LEFT,200);
		SetWindowText(_T("组态校验(图元重叠)"));
	}

	for(int i=0; i<vecCheck.size(); ++i)
	{
		m_listErrInfo.InsertItem(i, L"");
		CString str;
		str.Format(_T("%d"),vecCheck[i].nPageID);
		m_listErrInfo.SetItemText(i, 0, str);
		m_listErrInfo.SetItemText(i, 1, vecCheck[i].strPageName.c_str());
		str.Format(_T("%d"),vecCheck[i].nElementID);
		m_listErrInfo.SetItemText(i, 2, str);

		if(m_nShowMode == 0)
		{
			str.Format(_T("%d"),vecCheck[i].nImageID);
			m_listErrInfo.SetItemText(i, 3, str);
			switch(vecCheck[i].nErrType)
			{
				case 0:
				m_listErrInfo.SetItemText(i, 4, _T("事件图片缺失"));
				break;
				case 1:
					m_listErrInfo.SetItemText(i, 4, _T("事件绑定图片缺失"));
					break;
				case 2:
					m_listErrInfo.SetItemText(i, 4, _T("按钮图片缺失"));
					break;
				case 3:
					m_listErrInfo.SetItemText(i, 4, _T("元素背景图片缺失"));
					break;
				case 4:
					m_listErrInfo.SetItemText(i, 4, _T("弯头图片缺失"));
					break;
				case 5:
					m_listErrInfo.SetItemText(i, 4, _T("仪表盘图片缺失"));
					break;
				case 6:
					m_listErrInfo.SetItemText(i, 4, _T("图表图片缺失"));
					break;
				case 7:
					m_listErrInfo.SetItemText(i, 4, _T("历史趋势图片缺失"));
					break;
				case 8:
					m_listErrInfo.SetItemText(i, 4, _T("ROI图片缺失"));
					break;
				default:
					m_listErrInfo.SetItemText(i, 4, _T("图片缺失"));
					break;
			}
		}
		else if(m_nShowMode == 1)
		{
			m_listErrInfo.SetItemText(i, 3, vecCheck[i].strPointName.c_str());
			m_listErrInfo.SetItemText(i, 4, _T("点位缺失"));
		}
		else if(m_nShowMode == 2)
		{
			m_listErrInfo.SetItemText(i, 3, vecCheck[i].strErrInfo.c_str());
		}
	}
}

bool CProjectCheckDlg::InitList()
{
	m_listErrInfo.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT|LVS_EDITLABELS); 
	m_listErrInfo.InsertColumn(0,_T("页面ID"),LVCFMT_LEFT,100);
	m_listErrInfo.InsertColumn(1,_T("页面名称"),LVCFMT_LEFT,200);
	m_listErrInfo.InsertColumn(2,_T("元素ID"),LVCFMT_LEFT,100);
	m_listErrInfo.InsertColumn(3,_T("图片ID"),LVCFMT_LEFT,100);
	m_listErrInfo.InsertColumn(4,_T("错误信息"),LVCFMT_LEFT,100);
	return true;
}

BOOL CProjectCheckDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();
	InitList();
	return TRUE;  
}

void CProjectCheckDlg::OnLvnItemchangedListErrInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

}

void CProjectCheckDlg::OnNMDblclkListErrInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CString strPageID = m_listErrInfo.GetItemText(pNMItemActivate->iItem,0);
	CString strElementID = m_listErrInfo.GetItemText(pNMItemActivate->iItem,2);
	CString strImageID = m_listErrInfo.GetItemText(pNMItemActivate->iItem,3);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
	if(pView)
	{
		//pView->m_pTreeDlg->ShowPageByPageID(_wtoi(strPageID),_wtoi(strElementID),_wtoi(strImageID));
		pMainFrame->m_wndFileView.ShowPageByPageID(_wtoi(strPageID),_wtoi(strElementID),_wtoi(strImageID));
	}
}

UINT	WINAPI CProjectCheckDlg::ThreadFunc( LPVOID lpvoid )
{
	CProjectCheckDlg* pthis = (CProjectCheckDlg*)lpvoid;
	if (pthis)
	{
		pthis->UpdateList();
		pthis->UpdateControlState(true);
	}

	return 0;
}

void CProjectCheckDlg::UpdateList()
{
	if(m_pProjectSqlite)
	{
		if(m_nShowMode == 0)
		{
			m_pProjectSqlite->LoadPictureLibSqlite();
			m_pProjectSqlite->LoadAllProjectImageInfo();
			vector<ProjectCheckInfo> vecCheck = m_pProjectSqlite->CompareAndReturnNotExist();
			RefreshList(vecCheck);
			if(vecCheck.size() == 0)
			{
				MessageBox(_T("图片资源无错误"));
			}
		}
		else if(m_nShowMode == 1)
		{
			m_pProjectSqlite->LoadProjectSqlite_opcid3();
			m_pProjectSqlite->LoadAllProjectPointInfo();
			vector<ProjectCheckInfo> vecCheck = m_pProjectSqlite->CompareAndReturnNotExistPoint();
			RefreshList(vecCheck);
			if(vecCheck.size() == 0)
			{
				MessageBox(_T("点表资源无错误"));
			}
		}
		else if(m_nShowMode == 2)
		{
			vector<ProjectCheckInfo> vecCheck = m_pProjectSqlite->CompareAndReturnOverlapElement();
			RefreshList(vecCheck);
			if(vecCheck.size() == 0)
			{
				MessageBox(_T("未发现重叠图元"));
			}
		}
	}
}

bool CProjectCheckDlg::UpdateControlState( bool bState )
{
	((CButton*)GetDlgItem(IDC_BUTTON_CHECK_IMAGE))->EnableWindow(bState);
	((CButton*)GetDlgItem(IDC_BUTTON_CHECK_POINT))->EnableWindow(bState);
	((CButton*)GetDlgItem(IDC_BUTTON_EXPORT))->EnableWindow(bState);
	((CButton*)GetDlgItem(IDC_BUTTON_CHECK_OVERLOAP))->EnableWindow(bState);
	return true;
}


void CProjectCheckDlg::OnBnClickedButtonCheckOverloap()
{
	// TODO: Add your control notification handler code here
	if(m_pProjectSqlite)
	{
		m_nShowMode = 2;
		UpdateControlState(false);
		SetWindowText(_T("正在组态校验(重叠图元)...."));
		HANDLE handle = (HANDLE)_beginthreadex(NULL,0, ThreadFunc, this, NORMAL_PRIORITY_CLASS, NULL);
		CloseHandle(handle);
	}
}
