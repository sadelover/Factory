#pragma once


// CImportHisDataDlg dialog

class CImportHisDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportHisDataDlg)

public:
	CImportHisDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportHisDataDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORT_HISTORY_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoadfile();
	afx_msg void OnBnClickedButtonLoadfile2();
	afx_msg void OnBnClickedButtonLoadfile3();
	CString m_strEditFileName1;
	CString m_strEditFileName3;
	afx_msg void OnBnClickedButtonImportHistory();
	CString m_strEditFileName2;

	static unsigned __stdcall RunDownloadDataFileThread( void * pParam );
};
