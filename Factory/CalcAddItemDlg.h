#pragma once


class CMainFrame;


// CCalcAddItemDlg 对话框

class CCalcAddItemDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalcAddItemDlg)

public:
	CCalcAddItemDlg(const BOOL bIsAdd, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalcAddItemDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CALC_ADD_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

private:
	void	SetResultText(CString strShow);
	BOOL	m_bIsAdd;
	CString	m_strTempFold;
	CMainFrame* m_pMainFrm;
	CString	m_strCalcIp;
	int		m_nCalcPort;
	CString	m_strCalcPostName;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnTest();
	CString m_strName;
	CString m_strContent;
	CString m_strShow;
};
