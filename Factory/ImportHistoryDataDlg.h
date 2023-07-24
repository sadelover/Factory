#pragma once


// CImportHistoryDataDlg dialog

class CImportHistoryDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportHistoryDataDlg)

public:
	CImportHistoryDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportHistoryDataDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORT_HISTORY_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strInfo;
	afx_msg void OnBnClickedButtonImportHistory();

	static unsigned __stdcall RunDownloadDataFileThread( void * pParam );
	afx_msg void OnBnClickedButtonLoadfile();
	CString m_strEditFileName1;

	CString m_strDownloadFile;
	CString m_strServerIP;
	int m_nServerPort;
	 
	afx_msg void OnBnClickedButtonLoadfile2();
	afx_msg void OnBnClickedButtonLoadfile3();
	CString m_strEditFileName2;
	CString m_strEditFileName3;
	CString m_strEditFileName4;
	CString m_strEditFileName5;
	CString m_strEditFileName6;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strInfo2;
	CString m_strInfo3;
	afx_msg void OnBnClickedButtonLoadfile4();
	afx_msg void OnBnClickedButtonLoadfile5();
	afx_msg void OnBnClickedButtonLoadfile6();
};
