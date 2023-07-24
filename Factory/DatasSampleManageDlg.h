#pragma once


// CDatasSampleManageDlg dialog

class CDatasSampleManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDatasSampleManageDlg)

public:
	CDatasSampleManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDatasSampleManageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_POINT_HISDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
