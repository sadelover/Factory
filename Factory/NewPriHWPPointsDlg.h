#pragma once
#include "afxwin.h"



// CNewPriHWPPointsDlg dialog

class CNewPriHWPPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewPriHWPPointsDlg)

public:
	CNewPriHWPPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewPriHWPPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_ADD_PriHWP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	int m_nSinglePriHWPCount;
	CString m_strRoomNamePriHWP;
	int m_nStartNumPriHWP;
};
