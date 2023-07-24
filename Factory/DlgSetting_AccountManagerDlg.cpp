// DlgSetting_AccountManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgSetting_AccountManagerDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"

// CDlgSetting_AccountManager �Ի���

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


// CDlgSetting_AccountManager ��Ϣ�������


void CDlgSetting_AccountManagerDlg::OnBnClickedButtonSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	if(pFrame->m_pPrjSqlite)
	{
		//�˳�ʱ�䴦��
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
		//��ס���봦��
		if(m_rememberPwdEnable ==  TRUE)
		{
			m_jsonConfigParm["remember_pwd_enable"] = 1;
		}else
		{
			m_jsonConfigParm["remember_pwd_enable"] = 0;
		}

		//��ʼ�洢
		string strTemp = m_jsonConfigParm.toStyledString();
		wstring wstrJsonConfigParam;
		Project::Tools::UTF8ToWideChar(strTemp, wstrJsonConfigParam);
		pFrame->m_pPrjSqlite->SetValueInLocalConfigTable(_T("account_manage_config"), wstrJsonConfigParam);



		//����ʹ�ö����˵�
		pFrame->m_project.m_DBSetting.bEnableMenuGroup = m_bEnableMenuGroup;
		CString strErr;
		bool bSuccess = pFrame->m_pPrjSqlite->UpdateDBSettingPara(pFrame->m_project.m_DBSetting, strErr);
		if(!bSuccess)
		{
			CString strTemp;
			strTemp.Format(_T("��Ŀ���ñ���ʧ�ܡ�ԭ��:\r\n%s"), strErr);
			AfxMessageBox(strTemp);
		}
	}
}


BOOL CDlgSetting_AccountManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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

	//����ʹ�ö����˵�
	m_bEnableMenuGroup = pFrame->m_project.m_DBSetting.bEnableMenuGroup;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
