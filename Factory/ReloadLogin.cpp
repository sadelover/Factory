// ReloadLogin.cpp : implementation file
//

#include "stdafx.h"
#include "ReloadLogin.h"
#include "afxdialogex.h"


// CReloadLogin dialog

IMPLEMENT_DYNAMIC(CReloadLogin, CDialogEx)

CReloadLogin::CReloadLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReloadLogin::IDD, pParent)
	, m_strPassWord(_T(""))
{

}

CReloadLogin::~CReloadLogin()
{
}

void CReloadLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT_PASSWORD, m_strPassWord);

}


BEGIN_MESSAGE_MAP(CReloadLogin, CDialogEx)
END_MESSAGE_MAP()


// CReloadLogin message handlers
