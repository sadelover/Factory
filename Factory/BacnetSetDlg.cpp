// BacnetSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BacnetSetDlg.h"
#include "afxdialogex.h"


// CBacnetSetDlg dialog

IMPLEMENT_DYNAMIC(CBacnetSetDlg, CDialog)

CBacnetSetDlg::CBacnetSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBacnetSetDlg::IDD, pParent)
	, m_nBacnetScanInterval(30)
	, m_nBacnetTimeOut(5000)
	, m_nCharacterType(0)
	, m_nBacnetRetry(0)
	, m_nMutilReadCount(10)
{

}

CBacnetSetDlg::~CBacnetSetDlg()
{
}

void CBacnetSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_nBacnetScanInterval);
	DDX_Text(pDX, IDC_EDIT6, m_nBacnetTimeOut);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nCharacterType);
	DDX_Text(pDX, IDC_EDIT7, m_nBacnetRetry);
	DDX_Text(pDX, IDC_EDIT_READ_LIMIT, m_nMutilReadCount);
}


BEGIN_MESSAGE_MAP(CBacnetSetDlg, CDialog)
END_MESSAGE_MAP()

BOOL CBacnetSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


// CBacnetSetDlg message handlers
