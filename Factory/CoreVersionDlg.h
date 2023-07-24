#pragma once
#include "afxwin.h"


// CCoreVersionDlg dialog

class CCoreVersionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCoreVersionDlg)

public:
	CCoreVersionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCoreVersionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_VERSIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCoreVersion;
	CString m_strLogicVersion;
	CString m_strPysiteVersion;
};
