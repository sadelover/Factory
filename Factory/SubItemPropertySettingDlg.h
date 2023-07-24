#pragma once


// CSubItemPropertySettingDlg 对话框

class CSubItemPropertySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSubItemPropertySettingDlg)

public:
	CSubItemPropertySettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSubItemPropertySettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SUBITEM_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPicID;
	CString m_strPointName;
	CString m_strDescription;
	afx_msg void OnBnClickedButtonSubitemPicSelect();
	afx_msg void OnBnClickedButtonSubitemPointSelect();
	afx_msg void OnBnClickedOk();
};
