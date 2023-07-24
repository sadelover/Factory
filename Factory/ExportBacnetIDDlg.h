#pragma once


// CExportBacnetIDDlg dialog

class CExportBacnetIDDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportBacnetIDDlg)

public:
	CExportBacnetIDDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportBacnetIDDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_nBacnetStart;
	DWORD m_nBacnetEnd;
};
