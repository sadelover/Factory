#pragma once



enum EDateTimeError
{
	Null_Error,
	Date_Error,
	Time_Error,
};

// CInfoDlg dialog

class CInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfoDlg();
	virtual BOOL OnInitDialog();

	// Dialog Data
	enum { IDD = IDD_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	// Hide the information dialog.
	void HideInfoDlg();
};
