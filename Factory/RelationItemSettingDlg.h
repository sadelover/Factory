#pragma once

#include "afxwin.h"

// CRelationItemSettingDlg 对话框

class CRelationItemSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CRelationItemSettingDlg)

public:
	CRelationItemSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRelationItemSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RELATION_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRelationBindPointSelect();
	afx_msg void OnBnClickedOk();
	CString m_strPointName;
	CComboBox m_combo_relation_item;
	double m_fResult;
	int m_nRelation;
};
