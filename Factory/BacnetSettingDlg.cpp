// BacnetSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BacnetSettingDlg.h"
#include "afxdialogex.h"


// CBacnetSettingDlg dialog

IMPLEMENT_DYNAMIC(CBacnetSettingDlg, CDialog)

CBacnetSettingDlg::CBacnetSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBacnetSettingDlg::IDD, pParent)
	, m_nBacnetMode(0)
	, m_nBacnetScanInterval(1000)
	, m_nCharacterType(0)
	, m_nMutilReadCount(10)
{

}

CBacnetSettingDlg::~CBacnetSettingDlg()
{
}

void CBacnetSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_nBacnetMode);
	DDX_Text(pDX, IDC_EDIT2, m_nBacnetScanInterval);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nCharacterType);
	DDX_Text(pDX, IDC_EDIT_READ_LIMIT, m_nMutilReadCount);
}


BEGIN_MESSAGE_MAP(CBacnetSettingDlg, CDialog)
END_MESSAGE_MAP()

BOOL CBacnetSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


// CBacnetSettingDlg message handlers
