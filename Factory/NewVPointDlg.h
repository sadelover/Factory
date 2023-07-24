#pragma once


// CNewVPointDlg 对话框

class CNewVPointDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewVPointDlg)

public:
	CNewVPointDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewVPointDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADD_VPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPointID;
	int m_nPointType;
	CString m_strPointDef;
	afx_msg void OnBnClickedOk();
};
