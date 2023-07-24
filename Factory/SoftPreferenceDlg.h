#pragma once



// CSoftPreferenceDlg dialog

class CSoftPreferenceDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoftPreferenceDlg)

public:
	CSoftPreferenceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSoftPreferenceDlg();

// Dialog Data
	enum { IDD = IDD_SOFT_PREFERENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFilepos();
	CString m_strImageLibPath;
};
