// ImportTemplateBatchSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportTemplateBatchSettingDlg.h"
#include "afxdialogex.h"


// CImportTemplateBatchSettingDlg dialog

IMPLEMENT_DYNAMIC(CImportTemplateBatchSettingDlg, CDialogEx)

CImportTemplateBatchSettingDlg::CImportTemplateBatchSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportTemplateBatchSettingDlg::IDD, pParent)
	, m_strModelSrcName(_T(""))
	, m_strBatchHolderNum(_T(""))
	, m_strBatchNewPageName(_T(""))
	, m_strPrePointName(_T(""))
	, m_strReplacePointName(_T(""))
{

}

CImportTemplateBatchSettingDlg::~CImportTemplateBatchSettingDlg()
{
}

void CImportTemplateBatchSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MODEL_SRC_NAME, m_strModelSrcName);
	DDX_Text(pDX, IDC_EDIT_BATCH_HOLDER_NUM, m_strBatchHolderNum);
	DDX_Text(pDX, IDC_EDIT_BATCH_NEW_PAGE, m_strBatchNewPageName);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_FROM, m_strPrePointName);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_TO, m_strReplacePointName);
}


BEGIN_MESSAGE_MAP(CImportTemplateBatchSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CImportTemplateBatchSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CImportTemplateBatchSettingDlg message handlers


BOOL CImportTemplateBatchSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CImportTemplateBatchSettingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
