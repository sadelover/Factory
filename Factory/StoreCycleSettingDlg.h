#pragma once

#include "afxwin.h"

// CStoreCycleSettingDlg 对话框

class CStoreCycleSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CStoreCycleSettingDlg)

public:
	CStoreCycleSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStoreCycleSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_STORE_CYCLE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_cycle;
	CString m_str_combo_cycle;
	int m_nCycle;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
