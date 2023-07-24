#pragma once



// CNewCWPPointsDlg dialog

class CNewCWPPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewCWPPointsDlg)

public:
	CNewCWPPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewCWPPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_ADD_CWPPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nSingleCWPowerCount;
	CString m_strRoomNameCWP;
	int m_nStartNumCWP;
};
