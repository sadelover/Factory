#pragma once
#include "afxcmn.h"

#include "DebugToolFileTransfer.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "afxwin.h"
// CDownLoadFileDlg dialog

class CUPPCServerDataAccess;

class CDownLoadFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownLoadFileDlg)

public:
	CDownLoadFileDlg(CString strPath,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDownLoadFileDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_DOWNLOAD_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoadfile();
	afx_msg void OnBnClickedButtonWritepointsToServer();
	afx_msg void OnBnClickedButtonWriteToServer();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool SaveToCsvFile(const CString strFilePathName, const vector<DataPointEntry>& vecPoint);
	bool	CreateConnection(CString strServerIP);


	static unsigned __stdcall RunDownloadDBFileThread( void * pParam );
public:
	CString m_strDownloadFile;
	CProgressCtrl m_pProgess;
	CString m_strServerIP;

	CDebugToolFileTransfer *m_pFileTransfer;
	CDebugToolFileTransfer *m_pFileTransferRecv;

	int						m_nTimeCount;

	const CString			m_strErrTitle;
	const UINT				m_unErrType;

	int m_nNoResponseTimeCount;
	int m_nFileSize;
	wstring m_strLastDownloadResponse;
	CStatic m_ProgressStatic;
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
};
