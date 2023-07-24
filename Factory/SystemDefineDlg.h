#pragma once
#include "./json/json.h"
#include "afxwin.h"
#include "SystemDefineDlg_Child_List.h"
#include "SystemDefineDlg_Child_Script.h"

// CSystemDefineDlg dialog
class CSystemDefineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemDefineDlg)

public:
	CSystemDefineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSystemDefineDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SYSTEM_DEFINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSystemDefine;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	CSystemDefineDlg_Child_List		m_ChildDlg_List;
	CSystemDefineDlg_Child_Script	m_ChildDlg_Script;

	afx_msg void OnBnClickedButtonScript();
	
};
