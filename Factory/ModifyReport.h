#pragma once


// CModifyReport dialog

class CModifyReport : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyReport)

public:
	CModifyReport(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyReport();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODIFY_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString m_strNewReportName;
};
