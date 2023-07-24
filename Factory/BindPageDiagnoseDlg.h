#pragma once

#include "afxwin.h"

// CBindPageDiagnoseDlg �Ի���

class CBindPageDiagnoseDlg : public CDialog
{
	DECLARE_DYNAMIC(CBindPageDiagnoseDlg)

public:
	CBindPageDiagnoseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBindPageDiagnoseDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DIAGNOSE_BIND_PAGE_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
