// SubItemPropertySettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SubItemPropertySettingDlg.h"
#include "afxdialogex.h"
#include "MetaFileSelectDlg.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"
// CSubItemPropertySettingDlg 对话框

IMPLEMENT_DYNAMIC(CSubItemPropertySettingDlg, CDialog)

CSubItemPropertySettingDlg::CSubItemPropertySettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubItemPropertySettingDlg::IDD, pParent)
	, m_strPicID(_T(""))
	, m_strPointName(_T(""))
	, m_strDescription(_T(""))
{

}

CSubItemPropertySettingDlg::~CSubItemPropertySettingDlg()
{
}

void CSubItemPropertySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SUBITEM_PICID, m_strPicID);
	DDX_Text(pDX, IDC_EDIT_SUBITEM_POINT_NAME, m_strPointName);
	DDX_Text(pDX, IDC_EDIT_SUBITEM_DESCRIPTION, m_strDescription);
}


BEGIN_MESSAGE_MAP(CSubItemPropertySettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SUBITEM_PIC_SELECT, &CSubItemPropertySettingDlg::OnBnClickedButtonSubitemPicSelect)
	ON_BN_CLICKED(IDC_BUTTON_SUBITEM_POINT_SELECT, &CSubItemPropertySettingDlg::OnBnClickedButtonSubitemPointSelect)
	ON_BN_CLICKED(IDOK, &CSubItemPropertySettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSubItemPropertySettingDlg 消息处理程序


void CSubItemPropertySettingDlg::OnBnClickedButtonSubitemPicSelect()
{
	CMetaFileSelectDlg dlg;
	dlg.SetWndType(e_picture);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		m_strPicID.Format(L"%d",dlg.m_nID);
		UpdateData(FALSE);
		pFrame->m_nSelectPicID = dlg.m_nID;
	}
}


void CSubItemPropertySettingDlg::OnBnClickedButtonSubitemPointSelect()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pMainFrm->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strPointName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CSubItemPropertySettingDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialog::OnOK();
}
