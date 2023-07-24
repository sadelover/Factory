#pragma once



// CNewCHPointsDlg dialog

class CNewCHPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewCHPointsDlg)

public:
	CNewCHPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewCHPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_ADD_CHPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nSingleChillerCount;
	int m_nsinglestartChNum;
	CString m_strRoomNameCH;
};
