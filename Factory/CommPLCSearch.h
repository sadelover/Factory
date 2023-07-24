#pragma once
#include "ViewTree.h"

// CCommPLCSearch dialog
class CS7UDPCtrl;
class CCommPLCSearch : public CDialog
{
	DECLARE_DYNAMIC(CCommPLCSearch)

public:
	CCommPLCSearch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommPLCSearch();
	virtual BOOL OnInitDialog();

	bool Exit();
	bool InitData(bool bConnected = true);

	bool			AddLog(CString strLog);
	static LRESULT OnRecData(const char* buffer,DWORD userdata);

// Dialog Data
	enum { IDD = IDD_DIALOG_PLC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedButtonConnectPlc();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonConnectPlcMaual();
	DECLARE_MESSAGE_MAP()

public:
	CViewTree	m_wndFileView;
	HTREEITEM	m_hRoot;
	HTREEITEM	m_hSelectNode;


	CIPAddressCtrl			m_IPLCIP;
	CString					m_strPLCIP;
	int						m_nSlack;
	int						m_nSlot;
	CString					m_strPlcPointAddress;
	double					m_dValue;
	CS7UDPCtrl*				m_pS7UDPCtrl;
	bool					m_bConnectOK;
	CEdit					m_EditLog;
	CString					m_strLOG;
	afx_msg void OnStnClickedStatic2();
};
