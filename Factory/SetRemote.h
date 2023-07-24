#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <hash_map>
// CSetRemote dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;

// CSetRemote dialog

class CSetRemote : public CDialog
{
	DECLARE_DYNAMIC(CSetRemote)

public:
	CSetRemote(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetRemote();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETREMOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingRemote();
	bool InitData();
	virtual BOOL OnInitDialog();
public:
	BOOL m_bRemoteSetOPC;
	BOOL m_bRemoteSetModbus;
	BOOL m_bRemoteSetBacnet;
	BOOL m_bRemoteSetPLC;
	BOOL m_bRemoteSetMysql;
	BOOL m_bRemoteSetSQLserver;
	BOOL m_bRemoteSetSqlite;
	CUPPCServerDataAccess*	m_pDataAccess;
};
