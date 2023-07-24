#pragma once

#include "../ControlEx/SkinDlg.h"

#include "afxwin.h"
#include "../ControlEx/CommonButton.h"
#include "gridctrl/GridCtrl.h"
#include "afxcmn.h"

// CDlgSetting 对话框

class CDlgSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetting();

// 对话框数据
	enum { IDD = IDD_CONN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangeTabChild(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTabChild(NMHDR *pNMHDR, LRESULT *pResult);
private:	
	// 页面管理
	CTabCtrl m_tab_ctrl;
	typedef vector<CDialogEx*> ChildDlgList;
	ChildDlgList m_childDlgList;
	//子页面
	//CDlgSetting_EditDlg m_dlgchild_edit;
	//CDlgSetting_AccountManagerDlg m_dlgChild_account;
};
