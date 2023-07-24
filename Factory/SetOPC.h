#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <hash_map>
// CSetOPC dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;


class CSetOPC : public CDialog
{
	DECLARE_DYNAMIC(CSetOPC)

public:
	CSetOPC(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetOPC();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETOPC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingOpc();
	bool InitData();
	virtual BOOL OnInitDialog();
public:
	int		 m_nOPCMaxPoint;
	int		 m_nOPCReadMutil;
	int		 m_nOPCCmdSleep;
	int		 m_nOPCCmdSleepFromDevice;
	int		 m_nOPCPollSleep;
	int		 m_nOPCPollSleepFromDevice;
	int		 m_nOPCReconnectInterval;
	int		 m_nOPCCheckReconnect;
	BOOL	 m_bEnableSecurity;
	BOOL	 m_bEnableOPCThread;
	BOOL	 m_bRemoteSetOPC;
	BOOL	 m_bDisableQuality;
	CString  m_strOPCServerIP;
	CUPPCServerDataAccess*	m_pDataAccess;


	//write by julian



	int m_nOPCupdaterate;
	int m_nOPCmultiadd;
	int m_nOPCasync20interval;
	
	int m_nOPClanguageid;
	BOOL m_bOPCasync20mode;
	int m_nOPCreconnectignore;
	int m_nOPCmainpollsleep;


};
