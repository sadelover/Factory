#pragma once
#include "afxwin.h"
#include "LogicDllThread.h"
#include "BEOPLogicManager.h"
#include "BEOP-DesignerView.h"
#include "gridctrl\gridctrl.h"
#include "afxcmn.h"
// CSearchLogicOutputDlg dialog

class CSearchLogicOutputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchLogicOutputDlg)

public:
	CSearchLogicOutputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchLogicOutputDlg();
// Dialog Data
	enum { IDD = IDD_DIALOG_SEARCH_LOGIC_OUTPUT };
public:
	afx_msg void OnSearch();
	afx_msg void OnUpdateSearch(CCmdUI *pCmdUI);
	CBEOPLogicManager * m_pLogicManager;
	CString m_strServer;
	int m_nServerPort;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSelectPoint();
	afx_msg void OnEnChangeEditReplaceFrom();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnCbnSelchangeComboThreadlist();
	afx_msg void OnBnClickedButtonGoStrategy();
	afx_msg void OnClose();
private:
	CComboBox m_ThreadList;
	CString m_strResultLog;
	CString m_strLog;
	CString m_strPointName;
	CListCtrl m_listCtrl;
};
