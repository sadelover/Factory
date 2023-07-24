// ImportTipsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportTipsDlg.h"
#include "afxdialogex.h"


// CImportTipsDlg dialog

IMPLEMENT_DYNAMIC(CImportTipsDlg, CDialogEx)

CImportTipsDlg::CImportTipsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportTipsDlg::IDD, pParent)
	, m_strShow(_T(""))
{

}

CImportTipsDlg::~CImportTipsDlg()
{
}

void CImportTipsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHOW, m_strShow);
}


BEGIN_MESSAGE_MAP(CImportTipsDlg, CDialogEx)
END_MESSAGE_MAP()


// CImportTipsDlg message handlers
