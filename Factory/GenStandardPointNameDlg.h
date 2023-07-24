#pragma once

#include "gridctrl/GridCtrl.h"
// CGenStandardPointNameDlg dialog

class CGenStandardPointNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGenStandardPointNameDlg)

public:
	CGenStandardPointNameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGenStandardPointNameDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_GEN_STANDARD_POINTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


	CGridCtrl m_Grid;
public:
	afx_msg void OnBnClickedButtonGen();
	afx_msg void OnBnClickedButtonQuery();
	int m_nRows;
	virtual BOOL OnInitDialog();
};
