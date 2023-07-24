#pragma once


#include "afxwin.h"


// CVPointSimpleSettingDlg dialog

class CVPointSimpleSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVPointSimpleSettingDlg)

public:
	CVPointSimpleSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVPointSimpleSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_VPOINT_SIMPLE_SETTING };

public:
	CString	GetVPointName(void) const;
	CString	GetVPointRwAttri(void) const;

private:
	void	InitControls(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CString m_strName;
	CComboBox m_comboRw;
	CString m_strRw;
	CString m_strDescription;
};
