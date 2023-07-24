// OPCSetting.cpp : implementation file
//

#include "stdafx.h"
#include "OPCSetting.h"
#include "afxdialogex.h"


// COPCSetting dialog

IMPLEMENT_DYNAMIC(COPCSetting, CDialog)

COPCSetting::COPCSetting(CWnd* pParent /*=NULL*/)
	: CDialog(COPCSetting::IDD, pParent)
	, m_strOPCServerIP(_T("127.0.0.1"))
	, m_strOPCFiler(_T("*"))
	, m_nBrowseType(2)
	, m_nOPCMultiLoad(100)
	, m_nLoadInterval(10)
	, m_bReadDescription(TRUE)
	, m_nMultiAdd(1000)
	, m_strFilterType(_T("Native"))
{
	
}

COPCSetting::~COPCSetting()
{
}

void COPCSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPCSERVERIP, m_strOPCServerIP);
	DDX_Text(pDX, IDC_EDIT_OPCSERVERIP2, m_strOPCFiler);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nBrowseType);
	DDX_Text(pDX, IDC_EDIT_OPCMULTILOAD, m_nOPCMultiLoad);
	DDX_Text(pDX, IDC_EDIT_LOAD_INTERVAL, m_nLoadInterval);
	DDX_Check(pDX, IDC_CHECK1, m_bReadDescription);
	DDX_Text(pDX, IDC_EDIT_OPCMULTIADD, m_nMultiAdd);
	DDX_CBString(pDX, IDC_COMBO_DATATYPE, m_strFilterType);
}


BEGIN_MESSAGE_MAP(COPCSetting, CDialog)
	ON_BN_CLICKED(IDOK, &COPCSetting::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL COPCSetting::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


// COPCSetting message handlers


void COPCSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
