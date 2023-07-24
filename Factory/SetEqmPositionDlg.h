#pragma once


// CSetEqmPositionDlg 对话框

class CSetEqmPositionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetEqmPositionDlg)

public:
	CSetEqmPositionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetEqmPositionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_POSITION_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_x;
	int m_y;
	afx_msg void OnBnClickedOk();
	int m_width;
	int m_height;
	afx_msg void OnBnClickedButtonCopyParams();
	afx_msg void OnBnClickedButtonSetParams();
};
