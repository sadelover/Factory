#pragma once


// CInputConditionDlg dialog

class CInputConditionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputConditionDlg)

public:
	CInputConditionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputConditionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_INPUT_CONDITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCondition;
};
