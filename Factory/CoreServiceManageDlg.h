#pragma once


#include "gridctrl/GridCtrl.h"
// CCoreServiceManageDlg dialog

class CCoreServiceManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCoreServiceManageDlg)

public:
	CCoreServiceManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCoreServiceManageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CORE_SERVICE_MANAGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnBnClickedButtonAddItem2();
	BOOL m_bEnableModbusClient;
	BOOL m_bEnableKeysight;
	BOOL m_bEnableModbusServer;
	virtual BOOL OnInitDialog();

	CString m_strServer;
	CString m_strServerIP;
	int m_nServerPort;



	CGridCtrl m_Grid;
	afx_msg void OnBnClickedButtonAddItem();
	afx_msg void OnBnClickedButtonDeleteItem();

	void RefreshTable();
	afx_msg void OnBnClickedOk();
};
