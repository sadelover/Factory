#pragma once


// CSubItemPropertySettingDlg �Ի���

class CSubItemPropertySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSubItemPropertySettingDlg)

public:
	CSubItemPropertySettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSubItemPropertySettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SUBITEM_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPicID;
	CString m_strPointName;
	CString m_strDescription;
	afx_msg void OnBnClickedButtonSubitemPicSelect();
	afx_msg void OnBnClickedButtonSubitemPointSelect();
	afx_msg void OnBnClickedOk();
};
