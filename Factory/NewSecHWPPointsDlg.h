#pragma once



// CNewSecHWPPointsDlg dialog

class CNewSecHWPPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewSecHWPPointsDlg)

public:
	CNewSecHWPPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewSecHWPPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_ADD_SecHWP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nSingleSecHWPCount;
	afx_msg void OnEnChangeEditNumSingleSechwppoint();
	CString m_strRoomNameSecHWP;
	int m_nStartNameSecHWP;
};
