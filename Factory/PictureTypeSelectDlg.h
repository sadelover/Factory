#pragma once
#include "afxwin.h"


// CPictureTypeSelectDlg dialog

class CPictureTypeSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CPictureTypeSelectDlg)

public:
	CPictureTypeSelectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPictureTypeSelectDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORT_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listPictureType;
	int m_nSelectType;
};
