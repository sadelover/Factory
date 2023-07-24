#pragma once
#include "afxcmn.h"
#include "ComunicationDlg.h"
#include "CommOPCSearch.h"
#include "CommPLCSearch.h"
#include "CommBacnetSearch.h"
// CComManager dialog

class CComManager : public CDialog
{
	DECLARE_DYNAMIC(CComManager)

public:
	CComManager(CWnd* pParent = NULL);   // standard constructor
	virtual ~CComManager();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_COM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	bool InitData(bool bConnected = true);
	bool Exit();
public:
	CTabCtrl			m_tabCom;
	CComunicationDlg	m_BacnetDlg;
	CCommOPCSearch		m_OPCDlg;
	CCommPLCSearch      m_PlcDlg;
	CCommBacnetSearch	m_pBacnetNewDlg;

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
