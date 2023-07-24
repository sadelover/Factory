#pragma once



// CNewPriChWPPointsDlg dialog

class CNewPriChWPPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewPriChWPPointsDlg)

public:
	CNewPriChWPPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewPriChWPPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_ADD_PriChWP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nSinglePriChWPCount;
	CString m_strRoomNamePriChWP;
	int m_nStartNumPriChWP;
};
