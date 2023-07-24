#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <hash_map>
// CCommunicationDlg dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;

// CSetWatch dialog

class CSetWatch : public CDialog
{
	DECLARE_DYNAMIC(CSetWatch)

public:
	CSetWatch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetWatch();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETWATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingWatch();
	afx_msg void OnBnClickedCheckEnableWatch();
	bool EnableWatchControl(bool bEnableWatch);
	bool InitData();
	virtual BOOL OnInitDialog();
public:
	BOOL m_bDisableWatch;
	BOOL m_bCoreNotRun;
	BOOL m_bServertimeTimeOut;
	BOOL m_bPLCTimeOut;
	BOOL m_bDTUTimeOut;
	CString m_strDTURouterIP;
	CUPPCServerDataAccess*	m_pDataAccess;
};
