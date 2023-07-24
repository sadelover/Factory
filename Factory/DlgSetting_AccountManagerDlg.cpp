// DlgSetting_AccountManager.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSetting_AccountManagerDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"

// CDlgSetting_AccountManager 对话框

IMPLEMENT_DYNAMIC(CDlgSetting_AccountManagerDlg, CDialogEx)

CDlgSetting_AccountManagerDlg::CDlgSetting_AccountManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetting_AccountManagerDlg::IDD, pParent)
	, m_nAutoQuitTimeout(0)
	, m_rememberPwdEnable(FALSE)
	, m_bEnableMenuGroup(FALSE)
{

}

CDlgSetting_AccountManagerDlg::~CDlgSetting_AccountManagerDlg()
{
}

void CDlgSetting_AccountManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AUTO_QUIT_TIMEOUT, m_nAutoQuitTimeout);
	DDX_Check(pDX, IDC_CHECK2, m_rememberPwdEnable);
	DDX_Check(pDX, IDC_CHECK_NAV_MENU_GORUP_ENABLE, m_bEnableMenuGroup);
}


BEGIN_MESSAGE_MAP(CDlgSetting_AccountManagerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Save, &CDlgSetting_AccountManagerDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CDlgSetting_AccountManager 消息处理程序


void CDlgSetting_AccountManagerDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	if(pFrame->m_pPrjSqlite)
	{
		//退出时间处理
		if((m_nAutoQuitTimeout>=0) && (m_nAutoQuitTimeout < 65535))
		{
			if((m_nAutoQuitTimeout > 0) && (m_nAutoQuitTimeout < 65535))
			{
				m_jsonConfigParm["auto_log_out"] = 1;
				m_jsonConfigParm["auto_log_out_timeout"] = m_nAutoQuitTimeout;
			}
			else if (m_nAutoQuitTimeout ==0)
			{
				m_jsonConfigParm["auto_log_out"] = 0;
				m_jsonConfigParm["auto_log_out_timeout"] = 0;
			}
		}
		//记住密码处理
		if(m_rememberPwdEnable ==  TRUE)
		{
			m_jsonConfigParm["remember_pwd_enable"] = 1;
		}else
		{
			m_jsonConfigParm["remember_pwd_enable"] = 0;
		}

		//开始存储
		string strTemp = m_jsonConfigParm.toStyledString();
		wstring wstrJsonConfigParam;
		Project::Tools::UTF8ToWideChar(strTemp, wstrJsonConfigParam);
		pFrame->m_pPrjSqlite->SetValueInLocalConfigTable(_T("account_manage_config"), wstrJsonConfigParam);



		//导航使用二级菜单
		pFrame->m_project.m_DBSetting.bEnableMenuGroup = m_bEnableMenuGroup;
		CString strErr;
		bool bSuccess = pFrame->m_pPrjSqlite->UpdateDBSettingPara(pFrame->m_project.m_DBSetting, strErr);
		if(!bSuccess)
		{
			CString strTemp;
			strTemp.Format(_T("项目配置保存失败。原因:\r\n%s"), strErr);
			AfxMessageBox(strTemp);
		}
	}
}


BOOL CDlgSetting_AccountManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_jsonConfigParm.clear();

	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();

	wstring wstrJsonRead = pFrame->m_pPrjSqlite->GetValueInLocalConfigTable(_T("account_manage_config"));

	if(wstrJsonRead.size()>0)
	{
		Json::Reader jsReader;
		string strTemp;
		Project::Tools::WideCharToUtf8(wstrJsonRead, strTemp);
		jsReader.parse(strTemp.c_str(), m_jsonConfigParm);

		m_nAutoQuitTimeout = m_jsonConfigParm["auto_log_out_timeout"].asInt();
		if(m_jsonConfigParm["remember_pwd_enable"].asInt() == 1)
			m_rememberPwdEnable = TRUE;
		else
			m_rememberPwdEnable = FALSE;
	}
	else
	{
		string sJson = 
"{\
\"auto_log_out\" : 0,\
\"auto_log_out_timeout\" : 0,\
\"remember_pwd_enable\" : 0\
}";
		Json::Reader jsReader;
		jsReader.parse(sJson.c_str(), m_jsonConfigParm);
		m_nAutoQuitTimeout = m_jsonConfigParm["auto_log_out_timeout"].asInt();
		if(m_jsonConfigParm["remember_pwd_enable"].asInt() == 1)
			m_rememberPwdEnable = TRUE;
		else
			m_rememberPwdEnable = FALSE;
	}

	//导航使用二级菜单
	m_bEnableMenuGroup = pFrame->m_project.m_DBSetting.bEnableMenuGroup;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
