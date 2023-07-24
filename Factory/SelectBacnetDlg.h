#pragma once
#include "afxwin.h"


// CSelectBacnetDlg dialog

class CSelectBacnetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectBacnetDlg)

public:
	CSelectBacnetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectBacnetDlg();
	virtual BOOL OnInitDialog();

	void	InitList();
	void	ShowList();
// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_BACNET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CListCtrl		m_listctrl;
	vector<wstring>	m_vecBacnetList;
	vector<wstring>	m_vecSelectBacnetList;
};
