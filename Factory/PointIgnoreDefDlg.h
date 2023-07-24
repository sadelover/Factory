#pragma once


// CPointIgnoreDefDlg 对话框

class CPointIgnoreDefDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPointIgnoreDefDlg)

public:
	CPointIgnoreDefDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPointIgnoreDefDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_POINT_IGNORE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditPoint();
	CString m_point_ignore_list;
};
