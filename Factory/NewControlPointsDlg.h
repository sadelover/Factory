#pragma once


// CNewControlPointsDlg dialog

class CNewControlPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewControlPointsDlg)

public:
	CNewControlPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewControlPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_New_ControlEquipment };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_nstrControlRoomName;
};
