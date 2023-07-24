#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CHistoryDataDownloadDlg dialog
class CMainFrame;
class CHistoryDataDownloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryDataDownloadDlg)

public:
	CHistoryDataDownloadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHistoryDataDownloadDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_HISDATA_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	COleDateTime m_DateFrom;
	COleDateTime m_DateTo;
	COleDateTime m_TimeFrom;
	COleDateTime m_TimeTo;

	COleDateTime m_tStart;
	COleDateTime m_tEnd;

	CString m_strServerIP;
	CString m_strSaveFileName;

	int m_nTimePeriod;


	CMainFrame *m_pFrame;

	HANDLE m_hThread;

	vector<CString> m_strPointNameList;
public:
	afx_msg void OnBnClickedOk();

	static UINT WINAPI	   RunDownloadThread( void * pParam );

	bool DownloadData(COleDateTime tFrom, COleDateTime tTo, CStdioFile *pFile);
	CProgressCtrl m_Progress;

	HANDLE m_EventStopThread;
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedButtonStop();
	CString m_strPointList;
};
