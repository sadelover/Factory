#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <hash_map>

// CSetDTU dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;

class CSetDTU : public CDialog
{
	DECLARE_DYNAMIC(CSetDTU)

public:
	CSetDTU(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetDTU();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETDTU };
public:
	CUPPCServerDataAccess*	m_pDataAccess;
	BOOL					m_bDTUEnable;
	BOOL					m_bDTURecCmd;
	BOOL					m_bDTUChecked;
	BOOL					m_bDisableSendAll;
	BOOL					m_bTCPEnable;
	int						m_nDTUPort;
	int						m_nTCPServerPort;
	int						m_nDTUMinType;
	int                     m_nDTUType;
	int						m_nSendAll;
	int                     m_nTCPSendFilePackageSize;
	int						m_nTCPSendFilePackageInterval;
	CString					m_strTCPName;
	CIPAddressCtrl			m_ITCPServerIP;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool EnableControl();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingDtu();
	afx_msg void OnCbnSelchangeComboDtuType();
	afx_msg void OnCbnSelchangeComboDtuminntype();
	afx_msg void OnBnClickedCheckDtucheck();
	afx_msg void OnBnClickedCheckDisable();
	afx_msg void OnBnClickedCheckSendAll();
	bool InitData();
	CString m_strTCPIP;
};
