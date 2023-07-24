// ImportHisDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportHisDataDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CImportHisDataDlg dialog

IMPLEMENT_DYNAMIC(CImportHisDataDlg, CDialogEx)

CImportHisDataDlg::CImportHisDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportHisDataDlg::IDD, pParent)
	, m_strEditFileName1(_T(""))
	, m_strEditFileName3(_T(""))
	, m_strEditFileName2(_T(""))
{

}

CImportHisDataDlg::~CImportHisDataDlg()
{
}

void CImportHisDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strEditFileName1);
	DDX_Text(pDX, IDC_EDIT_FILENAME3, m_strEditFileName3);
	DDX_Text(pDX, IDC_EDIT_FILENAME2, m_strEditFileName2);
}


BEGIN_MESSAGE_MAP(CImportHisDataDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE, &CImportHisDataDlg::OnBnClickedButtonLoadfile)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE2, &CImportHisDataDlg::OnBnClickedButtonLoadfile2)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE3, &CImportHisDataDlg::OnBnClickedButtonLoadfile3)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_HISTORY, &CImportHisDataDlg::OnBnClickedButtonImportHistory)
END_MESSAGE_MAP()


// CImportHisDataDlg message handlers

void CImportHisDataDlg::OnBnClickedButtonLoadfile()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	//CImportHistoryDataDlg dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xls;*.xlsx)|*.xls;*.xlsx|")_T("CSV File (*.csv)|*.csv|")_T("XUP File (*.xup)|*.xup|"), NULL);
	//dlg.m_strEditFileName1= strSys.GetString();
	UpdateData(FALSE);
}
void CImportHisDataDlg::OnBnClickedButtonLoadfile2()
{
	// TODO: Add your control notification handler code here
}

void CImportHisDataDlg::OnBnClickedButtonLoadfile3()
{
	// TODO: Add your control notification handler code here
}


void CImportHisDataDlg::OnBnClickedButtonImportHistory()
{
	// TODO: Add your control notification handler code here
}
