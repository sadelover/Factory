#pragma once


// CImportTipsDlg dialog

class CImportTipsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportTipsDlg)

public:
	CImportTipsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportTipsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORT_TIPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strShow;
};
