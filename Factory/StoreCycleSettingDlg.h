#pragma once

#include "afxwin.h"

// CStoreCycleSettingDlg �Ի���

class CStoreCycleSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CStoreCycleSettingDlg)

public:
	CStoreCycleSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStoreCycleSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_STORE_CYCLE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_cycle;
	CString m_str_combo_cycle;
	int m_nCycle;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
