// NewProcessMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProcessMonitorDlg.h"
#include "afxdialogex.h"


// CNewProcessMonitorDlg dialog

IMPLEMENT_DYNAMIC(CNewProcessMonitorDlg, CDialogEx)

CNewProcessMonitorDlg::CNewProcessMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewProcessMonitorDlg::IDD, pParent)
	, m_strProcessName(_T(""))
	, m_strDirName(_T(""))
{

}

CNewProcessMonitorDlg::~CNewProcessMonitorDlg()
{
}

void CNewProcessMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_SINGLE_CHPOINT, m_strProcessName);
	DDX_Text(pDX, IDC_EDIT_NUM_CHSTART, m_strDirName);
}


BEGIN_MESSAGE_MAP(CNewProcessMonitorDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewProcessMonitorDlg message handlers
