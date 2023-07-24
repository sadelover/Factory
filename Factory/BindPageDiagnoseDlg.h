#pragma once

#include "afxwin.h"

// CBindPageDiagnoseDlg 对话框

class CBindPageDiagnoseDlg : public CDialog
{
	DECLARE_DYNAMIC(CBindPageDiagnoseDlg)

public:
	CBindPageDiagnoseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBindPageDiagnoseDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DIAGNOSE_BIND_PAGE_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_page_diagnose;
	virtual BOOL OnInitDialog();
	CString m_str_page_diagnose;
	CString m_strPageID;
	afx_msg void OnBnClickedOk();
	CComboBox m_combo_page_list;
	CString m_str_combo_page_name;
	void Init();
	afx_msg void OnCbnSelchangeComboPageList();
};
