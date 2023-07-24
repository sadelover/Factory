#pragma once

#include "gridctrl/GridCtrl.h"
#include "afxwin.h"
// CCoreStatusDlg dialog


class CCoreStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCoreStatusDlg)

public:
	CCoreStatusDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCoreStatusDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CORE_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonYear();
	afx_msg void OnBnClickedButtonMonth();
	afx_msg void OnBnClickedButtonWeek();
	afx_msg void OnBnClickedButtonDay();
private:
	void ReadLogFile(void);
	void RestGrid(void);
	vector<CString> m_LogList;
	CGridCtrl m_Grid;
};
