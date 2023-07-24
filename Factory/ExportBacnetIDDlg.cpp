// ExportBacnetIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExportBacnetIDDlg.h"
#include "afxdialogex.h"


// CExportBacnetIDDlg dialog

IMPLEMENT_DYNAMIC(CExportBacnetIDDlg, CDialog)

CExportBacnetIDDlg::CExportBacnetIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportBacnetIDDlg::IDD, pParent)
	, m_nBacnetStart(0)
	, m_nBacnetEnd(0)
{

}

CExportBacnetIDDlg::~CExportBacnetIDDlg()
{
}

void CExportBacnetIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_START, m_nBacnetStart);
	DDX_Text(pDX, IDC_EDIT_END, m_nBacnetEnd);
}


BEGIN_MESSAGE_MAP(CExportBacnetIDDlg, CDialog)
END_MESSAGE_MAP()

BOOL CExportBacnetIDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


// CExportBacnetIDDlg message handlers
