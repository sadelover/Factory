#pragma once

#include "../ControlEx/SkinDlg.h"

#include "DataPoint/LogicPointManager.h"
#include "afxcmn.h"
// CPIFManageDlg 对话框

class CPIFManageDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CPIFManageDlg)

public:
	CPIFManageDlg(CWnd* pParent,CLogicPointManager&  logicPointMap);
	virtual ~CPIFManageDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PIF_MANAGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitList();
	CLogicPointManager&  m_LogicPtManager;
	bool m_bSaved;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listCtrl;
	afx_msg void OnNMDblclkListPif(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddPif();
	afx_msg void OnBnClickedButtonDeletePif();
	void ResetIndex();
	afx_msg void OnBnClickedButtonSaveLogic();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual afx_msg void OnBnClickedClose();
};
