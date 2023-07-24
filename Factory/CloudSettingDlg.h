#pragma once


// CCloudSettingDlg dialog

class CCloudSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCloudSettingDlg)

public:
	CCloudSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCloudSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CLOUDSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strProjectName;
	CString m_strMQTTServerAddress;
	CString m_strPwd;
	int m_nTimeSpan;
	BOOL m_bEnableCloud;
	afx_msg void OnBnClickedOk();
	BOOL m_bPermitCommand;
	CString m_strMQUserName;
};
