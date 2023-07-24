#pragma once
#include "afxdtctl.h"


// CSelectTimeDlg dialog

class CSelectTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectTimeDlg)

public:
	CSelectTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectTimeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_Date;
	CDateTimeCtrl m_Time;

	COleDateTime m_tSelect;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
