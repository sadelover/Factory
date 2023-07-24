// InputParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputParamDlg.h"
#include "afxdialogex.h"


// CInputParamDlg dialog

IMPLEMENT_DYNAMIC(CInputParamDlg, CDialogEx)

CInputParamDlg::CInputParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputParamDlg::IDD, pParent)
	, m_strParamValue(_T(""))
{

}

CInputParamDlg::~CInputParamDlg()
{
}

void CInputParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PARAM_NAME, m_strParamValue);
}


BEGIN_MESSAGE_MAP(CInputParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputParamDlg message handlers


void CInputParamDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();

}
