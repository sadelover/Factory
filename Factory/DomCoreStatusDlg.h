#pragma once


// CDomCoreStatusDlg dialog

class CDomCoreStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDomCoreStatusDlg)

public:
	CDomCoreStatusDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDomCoreStatusDlg();

// Dialog Data
	enum { IDD = IDD_DLG_DEV_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
