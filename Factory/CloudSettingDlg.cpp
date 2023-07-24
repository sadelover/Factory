// CloudSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CloudSettingDlg.h"
#include "afxdialogex.h"



// CCloudSettingDlg dialog

IMPLEMENT_DYNAMIC(CCloudSettingDlg, CDialogEx)

CCloudSettingDlg::CCloudSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCloudSettingDlg::IDD, pParent)
	, m_strProjectName(_T(""))
	, m_strMQTTServerAddress(_T(""))
	, m_strPwd(_T(""))
	, m_nTimeSpan(60)
	, m_bEnableCloud(FALSE)
	, m_bPermitCommand(FALSE)
	, m_strMQUserName(_T(""))
{

}

CCloudSettingDlg::~CCloudSettingDlg()
{
}

void CCloudSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strProjectName);
	DDX_Text(pDX, IDC_EDIT_MQ_SERVER_ADDRESS, m_strMQTTServerAddress);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDX_Text(pDX, IDC_EDIT_TIMESPAN, m_nTimeSpan);
	DDX_Check(pDX, IDC_CHECK_ENABLE_CLOUD, m_bEnableCloud);
	DDX_Check(pDX, IDC_CHECK_ENABLE_COMMAND, m_bPermitCommand);
	DDX_Text(pDX, IDC_EDIT_MQ_USERNAME, m_strMQUserName);
}


BEGIN_MESSAGE_MAP(CCloudSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCloudSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCloudSettingDlg message handlers


void CCloudSettingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bEnableCloud)
	{
		if(m_strMQTTServerAddress.GetLength()<=0)
		{
			AfxMessageBox(_T("服务器地址非法，请修正后确认"));
			return;
		}

		if(m_strProjectName.GetLength()<=0)
		{
			AfxMessageBox(_T("项目名非法，请修正后确认"));
			return;
		}

		if(m_strPwd.GetLength()<=0)
		{
			AfxMessageBox(_T("授权密码非法，请修正后确认"));
			return;
		}

		if(m_nTimeSpan<=5)
		{
			AfxMessageBox(_T("上传周期过短，最少5秒间隔"));
			return;
		}
	}

	CDialogEx::OnOK();
}
