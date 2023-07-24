#pragma once


// CValveSelectDlg 对话框
class CProjectSqlite;

class CValveSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CValveSelectDlg)

public:
	CValveSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CValveSelectDlg();
	int			m_nID;
// 对话框数据
	enum { IDD = IDD_DIALOG_VALVE_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void	SetEqmName();
	CProjectSqlite* m_pProjectSqlite;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
};
