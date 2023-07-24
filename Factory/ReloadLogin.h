#pragma once


// CReloadLogin dialog

class CReloadLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CReloadLogin)

public:
	CReloadLogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReloadLogin();

// Dialog Data
	enum { IDD = IDD_DIG_RELOAD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPassWord;
	
};
