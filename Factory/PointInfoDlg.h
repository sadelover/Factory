#pragma once


// CPointInfoDlg dialog

class CPointInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CPointInfoDlg)

public:
	CPointInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPointInfoDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_POINT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strShortName;
	CString m_strRemark;
	CString m_strUnit;
	CString m_strSource;
	CString m_strParam1;
	int m_nNum;
	double m_nAvg;
	double m_nMax;
	double m_nMin;
	afx_msg void OnStnClickedStatic002();
};
