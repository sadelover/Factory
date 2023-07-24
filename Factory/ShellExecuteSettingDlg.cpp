// ShellExecuteSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShellExecuteSettingDlg.h"
#include "afxdialogex.h"
#include "MetaFileSelectDlg.h"
#include "PathDialog.h"
#include "../ComponentDraw/EqmDrawDefines.h"
#include "MainFrm.h"
// CShellExecuteSettingDlg 对话框

IMPLEMENT_DYNAMIC(CShellExecuteSettingDlg, CDialog)

CShellExecuteSettingDlg::CShellExecuteSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShellExecuteSettingDlg::IDD, pParent)
	, m_strServerIP(_T(""))
	, m_strFileName(_T(""))
	, m_strDownloadDirectory(_T(""))
	, m_nBKPicID(-1)
	, m_bAutoDownload(true)
	, m_layer(E_LAYER_5)
	, m_strFileID(_T(""))
{

}

CShellExecuteSettingDlg::~CShellExecuteSettingDlg()
{
}

void CShellExecuteSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHELL_EXECUTE_SERVER_IP, m_strServerIP);
	DDX_Text(pDX, IDC_EDIT_SHELL_EXECUTE_FILE_NAME, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_SHELL_EXECUTE_DOWNLOAD_DIRECTORY, m_strDownloadDirectory);
	DDX_Text(pDX, IDC_EDIT_SHELL_EXECUTE_BK_PIC_ID, m_nBKPicID);
	DDX_Control(pDX, IDC_COMBO_SHELL_EXECUTE_COMBO_LAYER, m_comboLayer);
	DDX_Text(pDX, IDC_EDIT_SHELL_EXECUTE_FILE_ID, m_strFileID);
}


BEGIN_MESSAGE_MAP(CShellExecuteSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SHELL_EXECUTE_PIC_SELECE, &CShellExecuteSettingDlg::OnBnClickedButtonShellExecutePicSelece)
	ON_BN_CLICKED(IDC_BUTTON_SHELL_EXECUTE_DIRECTORY_SELECT, &CShellExecuteSettingDlg::OnBnClickedButtonShellExecuteDirectorySelect)
	ON_BN_CLICKED(IDOK, &CShellExecuteSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CShellExecuteSettingDlg 消息处理程序

void CShellExecuteSettingDlg::OnBnClickedButtonShellExecutePicSelece()
{
	CMetaFileSelectDlg dlg;
	dlg.SetWndType(e_picture);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		m_nBKPicID = dlg.m_nID;
		pFrame->m_nSelectPicID = m_nBKPicID;
	}
	UpdateData(FALSE);
}


void CShellExecuteSettingDlg::OnBnClickedButtonShellExecuteDirectorySelect()
{
	UpdateData(TRUE);
	CPathDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_strDownloadDirectory = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


BOOL CShellExecuteSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_comboLayer.ResetContent();
	m_comboLayer.AddString(L"1层");
	m_comboLayer.AddString(L"2层");
	m_comboLayer.AddString(L"3层");
	m_comboLayer.AddString(L"4层");
	m_comboLayer.AddString(L"5层");
	m_comboLayer.AddString(L"6层");
	m_comboLayer.AddString(L"7层");
	m_comboLayer.AddString(L"8层");
	m_comboLayer.AddString(L"9层");
	m_comboLayer.AddString(L"10层");
	m_comboLayer.SetCurSel(m_layer);

	if (m_bAutoDownload)
	{
		 ((CButton*)GetDlgItem(IDC_CHECK_SHELL_EXECUTE_AUTO_DOWNLOAD))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_SHELL_EXECUTE_AUTO_DOWNLOAD))->SetCheck(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CShellExecuteSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateData(FALSE);
	if (((CButton*)GetDlgItem(IDC_CHECK_SHELL_EXECUTE_AUTO_DOWNLOAD))->GetCheck() == 1)
	{
		m_bAutoDownload = true;
	}
	else
	{
		m_bAutoDownload = false;
	}
	m_layer = m_comboLayer.GetCurSel();
	if(m_strServerIP.GetLength() == 0)
	{
		AfxMessageBox(L"服务器地址不允许为空!");
		return;
	}
	CDialog::OnOK();
}
