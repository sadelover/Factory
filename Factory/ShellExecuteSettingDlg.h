#pragma once

#include "afxwin.h"

// CShellExecuteSettingDlg �Ի���

class CShellExecuteSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CShellExecuteSettingDlg)

public:
	CShellExecuteSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShellExecuteSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHELL_EXECUTE_SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strServerIP;
	CString m_strFileName;
	CString m_strDownloadDirectory;
	int m_nBKPicID;
	bool m_bAutoDownload;
	afx_msg void OnBnClickedButtonShellExecutePicSelece();
	afx_msg void OnBnClickedButtonShellExecuteDirectorySelect();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CComboBox m_comboLayer;
	int       m_layer;
	CString m_strFileID;
};
