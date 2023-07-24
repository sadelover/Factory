// TreeGroupSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TreeGroupSettingDlg.h"
#include "afxdialogex.h"


// CTreeGroupSettingDlg dialog

IMPLEMENT_DYNAMIC(CTreeGroupSettingDlg, CDialogEx)

CTreeGroupSettingDlg::CTreeGroupSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTreeGroupSettingDlg::IDD, pParent)
	, m_strGroupName(_T(""))
{

}

CTreeGroupSettingDlg::~CTreeGroupSettingDlg()
{
}

void CTreeGroupSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GROUP_NAME, m_strGroupName);
}


BEGIN_MESSAGE_MAP(CTreeGroupSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTreeGroupSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTreeGroupSettingDlg message handlers


BOOL CTreeGroupSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTreeGroupSettingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
