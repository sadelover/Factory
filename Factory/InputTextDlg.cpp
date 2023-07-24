// InputTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputTextDlg.h"
#include "afxdialogex.h"


// CInputTextDlg dialog

IMPLEMENT_DYNAMIC(CInputTextDlg, CDialogEx)

CInputTextDlg::CInputTextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputTextDlg::IDD, pParent)
	, m_strText(_T(""))
{

}

CInputTextDlg::~CInputTextDlg()
{
}

void CInputTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PARAM_NAME, m_strText);
}


BEGIN_MESSAGE_MAP(CInputTextDlg, CDialogEx)
END_MESSAGE_MAP()


// CInputTextDlg message handlers
