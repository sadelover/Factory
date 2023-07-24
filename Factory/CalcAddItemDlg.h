#pragma once


class CMainFrame;


// CCalcAddItemDlg �Ի���

class CCalcAddItemDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalcAddItemDlg)

public:
	CCalcAddItemDlg(const BOOL bIsAdd, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCalcAddItemDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CALC_ADD_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
