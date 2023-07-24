#pragma once


// CInputParamDlg dialog

class CInputParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputParamDlg)

public:
	CInputParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputParamDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_INPUT_PARAMETER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strParamValue;
	afx_msg void OnBnClickedOk();
};
