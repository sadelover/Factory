#pragma once


// CReportInfo dialog

class CReportInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CReportInfo)

public:
	CReportInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportInfo();

// Dialog Data
	enum { IDD = IDD_DIALOG_REPORT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strReportName;
	CString m_strReportDescription;
	int m_nFileType;
	int m_nTimeType;
	CString m_strTimeConfig;
	afx_msg void OnCbnSelchangeComboFileType();
	afx_msg void OnCbnSelchangeComboTimeType();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditTimeConfig();
	afx_msg void OnEnChangeEditReportDescription();
};
