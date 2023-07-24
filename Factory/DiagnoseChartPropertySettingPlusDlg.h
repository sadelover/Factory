#pragma once

#include "afxcmn.h"
#include "ReportCtrl.h"
#include "afxwin.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphicPlus.h"
// CDiagnoseChartPropertySettingPlusDlg 对话框

class CEqmDrawDiagnoseGraphicPlus;

class CDiagnoseChartPropertySettingPlusDlg : public CDialog
{
	DECLARE_DYNAMIC(CDiagnoseChartPropertySettingPlusDlg)

public:
	CDiagnoseChartPropertySettingPlusDlg(CWnd* pParent = NULL);   // 标准构造函数 
	virtual ~CDiagnoseChartPropertySettingPlusDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DIAGNOSE_CHART_PLUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonDiagnoseChartPlusItemAdd();
	afx_msg void OnBnClickedButtonDiagnoseChartPlusItemDelete();
	afx_msg void OnNMDblclkListDiagnoseChartPlusItem(NMHDR *pNMHDR, LRESULT *pResult);
	CReportCtrl m_list_diagnose_chart_plus_item;
	void InitList();
	virtual BOOL OnInitDialog();
	int	m_nColorIndex;
	CComboBox m_combo_link_page;
	afx_msg void OnBnClickedButtonDiagnoseChartPlusNormalbind();
	afx_msg void OnBnClickedButtonDiagnoseChartPlusAbnormalbind();
	int m_nDiagnoseNormalID;
	int m_nDiagnoseAbnormalID;
	int m_bIsAnimationByAbnormal;
	int m_nLinkPageID;
	int m_nTimeExtendScope;
	int m_nTimeScope;
	int m_nTimeInterval;
	vector<_tagBoolBindInfo>	m_vecBoolBind;
	CListCtrl m_list_item;
	int m_nShowMode;
	afx_msg void OnBnClickedButtonDiagnoseChartPlusItemAdd2();
	afx_msg void OnBnClickedButtonDiagnoseChartPlusItemDelete2();
	afx_msg void OnNMDblclkListDiagnoseChartPlusBindItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListDiagnoseChartPlusBindItem(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CString m_strCurSelPointName;
public:
	afx_msg void OnBnClickedRadioBreviary();
	afx_msg void OnBnClickedRadioDetail();
	afx_msg void OnLvnItemchangedListDiagnoseChartPlusBindItem(NMHDR *pNMHDR, LRESULT *pResult);
};
