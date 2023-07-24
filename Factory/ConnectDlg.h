#pragma once


#include "afxcmn.h"
#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"


// CConnectDlg dialog

class CConnectDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(const CString strDbIp, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strDbIp;
	CIPAddressCtrl m_ipDatabase;
	afx_msg void OnBnClickedCheckLocalhost();
	BOOL m_bIsLocalhost;
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
};
