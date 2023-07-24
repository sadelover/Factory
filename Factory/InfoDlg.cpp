// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InfoDlg.h"
#include "afxdialogex.h"


// CInfoDlg dialog

IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
{

}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
END_MESSAGE_MAP()


// CInfoDlg message handlers

void CInfoDlg::HideInfoDlg()
{
	CDialog::OnCancel();
}
BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CLanguageSwitch::GetInstance()->ReFreshDlgResource(IDD_INFO,this);
	CenterWindow();
	return TRUE;
}