#pragma once


// CTimeBarItemPropertyDlg 对话框

class CTimeBarItemPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeBarItemPropertyDlg)

public:
	CTimeBarItemPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimeBarItemPropertyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TIME_BAR_ITEM_PROPERTY_DLG };

	CString m_strIndex;
	CString m_strName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
