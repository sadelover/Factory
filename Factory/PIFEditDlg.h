#pragma once


// CPIFEditDlg 对话框
#include "../ControlEx/SkinDlg.h"

#include "afxwin.h"

class CPIFEditDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CPIFEditDlg)

public:
	CPIFEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPIFEditDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PIF_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitTypeCombo();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonParam1();
	afx_msg void OnBnClickedButtonParam2();
	afx_msg void OnBnClickedButtonParam3();
	afx_msg void OnBnClickedButtonParam4();
	afx_msg void OnBnClickedButtonParam5();
	afx_msg void OnBnClickedButtonParam6();
	afx_msg void OnBnClickedButtonParam7();
	afx_msg void OnBnClickedButtonParam8();
	afx_msg void OnBnClickedButtonParam9();
	CString m_strName;
	CString m_strType;
	CString m_strRemark;
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
	CString m_strParam11;
	CString m_strParam12;
	CString m_strParam13;
	CString m_strParam14;
	CString m_strParam15;
	CString m_strParam16;
	CString m_strParam17;
	CString m_strParam18;
	CString m_strParam19;
	CString m_strParam20;
	CString m_strParam21;
	CString m_strParam22;
	CString m_strParam23;
	CString m_strParam24;
	CString m_strParam25;
	CString m_strParam26;
	CString m_strParam27;
	CString m_strParam28;
	afx_msg void OnBnClickedOk();
	CComboBox m_combo_type;
	afx_msg void OnBnClickedButtonParam10();
	afx_msg void OnBnClickedButtonParam11();
	afx_msg void OnBnClickedButtonParam12();
	afx_msg void OnBnClickedButtonParam13();
	afx_msg void OnBnClickedButtonParam14();
	afx_msg void OnBnClickedButtonParam15();
	afx_msg void OnBnClickedButtonParam16();
	afx_msg void OnBnClickedButtonParam17();
	afx_msg void OnBnClickedButtonParam18();
	afx_msg void OnBnClickedButtonParam19();
	afx_msg void OnBnClickedButtonParam20();
	afx_msg void OnBnClickedButtonParam21();
	afx_msg void OnBnClickedButtonParam22();
	afx_msg void OnBnClickedButtonParam23();
	afx_msg void OnBnClickedButtonParam24();
	afx_msg void OnBnClickedButtonParam25();
	afx_msg void OnBnClickedButtonParam26();
	afx_msg void OnBnClickedButtonParam27();
	afx_msg void OnBnClickedButtonParam28();
	afx_msg void OnEnChangeEditPifName();
	afx_msg void OnEnChangeEditPifRemark();
	afx_msg void OnCbnSelchangeComboType();
};
