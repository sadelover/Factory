#pragma once


// CNewProcessMonitorDlg dialog

class CNewProcessMonitorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewProcessMonitorDlg)

public:
	CNewProcessMonitorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewProcessMonitorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strProcessName;
	CString m_strDirName;
};
