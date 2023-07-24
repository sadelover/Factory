#pragma once


// CNewStatisticsPointsDlg dialog

class CNewStatisticsPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewStatisticsPointsDlg)

public:
	CNewStatisticsPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewStatisticsPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ADD_NEW_STATISTICS};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNewRoomName;
	afx_msg void OnBnClickedOk();
};
