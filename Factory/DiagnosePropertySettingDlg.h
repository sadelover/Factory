#pragma once

#include "afxcmn.h"
#include "ReportCtrl.h"

// CDiagnosePropertySettingDlg 对话框
class CEqmDrawDiagnoseGraphic; 

class CDiagnosePropertySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDiagnosePropertySettingDlg)

public:
	CDiagnosePropertySettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	CDiagnosePropertySettingDlg(CEqmDrawDiagnoseGraphic* pDiagnoseChart, CWnd* pParent = NULL);
	virtual ~CDiagnosePropertySettingDlg();
	CEqmDrawDiagnoseGraphic* m_pDiagnoseChart;
	void InitList();
// 对话框数据
	enum { IDD = IDD_DIALOG_DIAGNOSE_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CReportCtrl m_list_diagnose_item;
	afx_msg void OnNMDblclkListDiagnoseItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDiagnoseItemAdd();
	afx_msg void OnBnClickedButtonDiagnoseItemDelete();
	int	m_nColorIndex;
};
