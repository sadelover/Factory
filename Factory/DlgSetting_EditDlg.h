#pragma once

#include "../ControlEx/CommonButton.h"
#include "gridctrl/GridCtrl.h"

// CDlgSetting_EditDlg 对话框

class CDlgSetting_EditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting_EditDlg)

public:
	CDlgSetting_EditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetting_EditDlg();

// 对话框数据
	enum { IDD = IDD_CONN_SETTING_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonModify();
private:
	vector<wstring> m_wstrNameList;
	vector<wstring> m_wstrValueList;
	CGridCtrl m_Grid;
};
