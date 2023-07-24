#pragma once


// CValveSelectDlg 对话框
class CProjectSqlite;

class CMeterPanSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CMeterPanSelectDlg)

public:
	CMeterPanSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMeterPanSelectDlg();
	int			m_nID;
	int			m_nDashBoardType;
// 对话框数据
	enum { IDD = IDD_DIALOG_METER_PAN_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void	SetEqmName();
	CProjectSqlite* m_pProjectSqlite;
	CString	m_strMeterPan1Path;							//仪表盘1图片路径
	CString	m_strMeterPan2Path;							//仪表盘2图片路径
	CString	m_strMeterPan3Path;							//仪表盘3图片路径
	CString	m_strMeterPan4Path;							//仪表盘4图片路径
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
};
