#pragma once

#include "afxwin.h"

// CRelationItemSettingDlg �Ի���

class CRelationItemSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CRelationItemSettingDlg)

public:
	CRelationItemSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRelationItemSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RELATION_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
