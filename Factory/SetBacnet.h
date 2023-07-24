#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <hash_map>
// CSetBacnet dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;

class CSetBacnet : public CDialog
{
	DECLARE_DYNAMIC(CSetBacnet)

public:
	CSetBacnet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetBacnet();

// Dialog Data
	//enum { IDD = IDD_DIALOG_SETWATCH };
	enum { IDD = IDD_DIALOG_SETBACNET };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingBacnet();
	bool ChangeEnviromentVariables( CString szName,CString szNewValue);
	bool InitData();
	virtual BOOL OnInitDialog();
public:
	int m_nBacnetReadMode;
	int m_nBacnetReadAllInterval;
	int m_nBacnetReadTypeInterval;
	int m_nBacnetReadPointInterval;

	CComboBox m_comBacnetIP;
	CUPPCServerDataAccess*	m_pDataAccess;
	int m_nBacnetreadtimeout;
	CString m_strBacnetReadCountPerTime;
	afx_msg void OnEnChangeEditBacnetReadPercount();
	afx_msg void OnEnChangeEditModbusInterval2();
	int m_nBacnetWritePriority;
};
