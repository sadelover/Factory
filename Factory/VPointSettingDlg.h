#pragma once

#include "../ControlEx/SkinDlg.h"

#include "afxwin.h"
// CVPointSettingDlg 对话框

class CVPointSettingDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CVPointSettingDlg)

public:
	CVPointSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVPointSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_VPOINT_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strParam1;
	CString m_strParam2;
	CString m_strParam3;
	CString m_strParam4;
	CString m_strParam5;
	CString m_strParam6;
	CString m_strParam7;
	CString m_strParam8;
	CString m_strParam9;
	CString m_strParam10;
	afx_msg void OnBnClickedButtonParam2();
	afx_msg void OnBnClickedButtonParam3();
	afx_msg void OnBnClickedButtonParam4();
	afx_msg void OnBnClickedButtonParam5();
	afx_msg void OnBnClickedButtonParam6();
	afx_msg void OnBnClickedButtonParam7();
	afx_msg void OnBnClickedButtonParam8();
	afx_msg void OnBnClickedButtonParam9();
	afx_msg void OnBnClickedButtonParam10();
	virtual BOOL OnInitDialog();
private:
	void InitParam1Combo();
	bool IsNameValid(const CString strName);
public:
	CComboBox m_combo_param1;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditVpointName();
	afx_msg void OnCbnSelchangeComboRelation();
	CString m_strHighAlarm;
	CString m_strHighHighAlarm;
	CString m_strLowAlarm;
	CString m_strLowLowAlarm;
};
