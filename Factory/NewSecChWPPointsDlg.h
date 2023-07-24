#pragma once



// CNewSecChWPPointsDlg dialog

class CNewSecChWPPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewSecChWPPointsDlg)

public:
	CNewSecChWPPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewSecChWPPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_ADD_SecChWP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nSingleSecChWPCount;
	CString m_strRoomNameSecChWP;
	int m_nStartNumSecChWP;
};
