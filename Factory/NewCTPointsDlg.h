#pragma once



// CNewCTPointsDlg dialog

class CNewCTPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewCTPointsDlg)

public:
	CNewCTPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewCTPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_ADD_CTPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nSingleCTowerCount;
	afx_msg void OnEnChangeEditNumSingleCtpoint();
	CString m_nRoomNameCT;
	int m_nSatartNumCT;
};
