// OPCSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OPCSettingDlg.h"
#include "afxdialogex.h"


// COPCSettingDlg dialog

IMPLEMENT_DYNAMIC(COPCSettingDlg, CDialog)

COPCSettingDlg::COPCSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COPCSettingDlg::IDD, pParent)
	, m_strOPCServerIP(_T(""))
	, m_strOPCFiler(_T(""))
	, m_nBrowseType(0)
	, m_nOPCMultiLoad(0)
	, m_nLoadInterval(0)
{

}

COPCSettingDlg::~COPCSettingDlg()
{
}

void COPCSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPCSERVERIP, m_strOPCServerIP);
	DDX_Text(pDX, IDC_EDIT1, m_strOPCFiler);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nBrowseType);
	DDX_Text(pDX, IDC_EDIT3, m_nOPCMultiLoad);
	//DDX_Text(pDX, IDC_EDIT4, m_nLoadInterval);
}


BEGIN_MESSAGE_MAP(COPCSettingDlg, CDialog)
END_MESSAGE_MAP()

BOOL COPCSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


// COPCSettingDlg message handlers
