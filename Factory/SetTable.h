#pragma once
#include "afxcmn.h"
#include "SetBacnet.h"
#include "SetDatabase.h"
#include "SetDebug.h"
#include "SetDTU.h"
#include "SetModbus.h"
#include "SetOPC.h"
#include "SetRemote.h"
#include "SetSiemens.h"
#include "SetWatch.h"
#include "afxcmn.h"
#include "afxwin.h"

#include "SetRunParam.h"
#include <hash_map>
// CSetTable dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;


class CSetTable : public CDialog
{
	DECLARE_DYNAMIC(CSetTable)

public:
	CSetTable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetTable();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_SETTABLE };
	CSetBacnet m_SetBacnet;
	CSetDatabase m_SetDatabaseset;
	CSetDebug m_SetDebug;
	CSetDTU m_SetDTU;
	CSetModbus m_SetModbus;
	CSetOPC m_SetOPC;
	CSetRemote m_SetRemote;
	CSetSiemens m_SetSiemens;
	CSetWatch m_SetWatch;
	CTabCtrl m_SetTab;
	CSetRunParam m_SetRunParam;
	CUPPCServerDataAccess*	m_pDataAccess;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
	bool InitData();
};
