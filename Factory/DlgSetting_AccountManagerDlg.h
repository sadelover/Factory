#pragma once

#include "json/json.h"
// CDlgSetting_AccountManager �Ի���

class CDlgSetting_AccountManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting_AccountManagerDlg)

public:
	CDlgSetting_AccountManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetting_AccountManagerDlg();

// �Ի�������
	enum { IDD = IDD_CONN_SETTING_ACCOUNT_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	virtual BOOL OnInitDialog();
private:
	Json::Value m_jsonConfigParm;
	int m_nAutoQuitTimeout;
	BOOL m_rememberPwdEnable;
	BOOL m_bEnableMenuGroup;
};
