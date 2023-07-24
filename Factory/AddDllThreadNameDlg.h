#pragma once


#include "afxwin.h"
#include "../ComponentDraw/CEqmDrawPage.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"

// CAddDllThreadNameDlg dialog

class CAddDllThreadNameDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CAddDllThreadNameDlg)

public:
	CAddDllThreadNameDlg(bool bIsAddNew, CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddDllThreadNameDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_THREAD };

private:
	bool	m_bIsAddNew;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDllName;
	int m_nWidth;
	int m_nHeight;
	PAGETYPE m_page_type;
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	afx_msg void OnEnChangeEditName();
	afx_msg void OnBnClickedOk();
};
