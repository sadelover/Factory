#pragma once


// CChillerSelectDlg 对话框
class CProjectSqlite;

class CChillerSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CChillerSelectDlg)

public:
	CChillerSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChillerSelectDlg();
	int			m_nID;
// 对话框数据
	enum { IDD = IDD_DIALOG_CHILLER_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void	SetEqmPicID();
	CProjectSqlite* m_pProjectSqlite;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
