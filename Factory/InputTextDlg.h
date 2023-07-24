#pragma once


// CInputTextDlg dialog

class CInputTextDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputTextDlg)

public:
	CInputTextDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputTextDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_INPUT_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
};
