#pragma once


#include "DebugToolFileTransfer.h"
#include "afxwin.h"

// CPaneServerManager 对话框

using std::pair;

class CUPPCServerDataAccess;
class CBEOPGatewayDlg;

class CPaneServerManager : public CDialogEx
{
	DECLARE_DYNAMIC(CPaneServerManager)

public:
	CPaneServerManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPaneServerManager();

	bool InitData();
	bool Exit();

// 对话框数据
	enum { IDD = IDD_DIALOG_SERVERMAMAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonChangeCoreMode();
	afx_msg void OnBnClickedButtonWriteToServer();
	afx_msg void OnBnClickedButtonGetdbFromServer();

	CString GetLocalIP() ;
	
	CUPPCServerDataAccess*	m_pDataAccess;

	CDebugToolFileTransfer *m_pFileTransfer;
	CDebugToolFileTransfer *m_pFileTransferRecv;
	virtual BOOL OnInitDialog();
	CStatic m_ProgressStatic;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_StaticMode;


	//CEdit m_EditDBName;
	CEdit m_EditServer;
	afx_msg void OnBnClickedCheckAlwayslog();
	CButton m_CheckAlwaysLog;
	CEdit m_EditLog;
	CString m_strLOG;

	CBEOPGatewayDlg *m_pGatewayDlg;

	SYSTEMTIME m_tLogRefreshTo;
	vector< pair<wstring,wstring> > m_pairLogInfo;
	BOOL m_bAutoScroll;
	afx_msg void OnBnClickedButtonModifyRedundency();
	CString m_strRedundencyIP;
	afx_msg void OnBnClickedButtonResetcore();
	CEdit m_EditFilter;
	afx_msg void OnBnClickedCheckFilter();
	CButton m_CheckFilter;

	int m_nNoResponseTimeCount;
	wstring m_strLastDownloadResponse;
	afx_msg void OnBnClickedButtonLoadfile();
	CString m_strDownloadFile;
};
