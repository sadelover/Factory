#pragma once

#include "../ComponentDraw/EqmDrawDiagnoseGraphic.h"
#include "afxcmn.h"

// CDiagnoseItemPropertySettingDlg 对话框

class CDiagnoseItemPropertySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDiagnoseItemPropertySettingDlg)

public:
	CDiagnoseItemPropertySettingDlg(_tagDiagnoseItem* pItem, CWnd* pParent = NULL);
	virtual ~CDiagnoseItemPropertySettingDlg();

	



// 对话框数据
	enum { IDD = IDD_DIALOG_DIAGNOSE_ITEM_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDiagnoseItemPointSelect();
	afx_msg void OnStnClickedStaticDiagnoseItemColor();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	_tagDiagnoseItem* m_pItem;
	COLORREF m_nColor;
public:
	virtual BOOL OnInitDialog();
	void	Init();
	CListCtrl m_list_subitem;
	afx_msg void OnBnClickedButtonDiagnoseSubitemAdd();
	afx_msg void OnBnClickedButtonDiagnoseSubitemDelete();
	afx_msg void OnNMDblclkListDiagnoseSubitem(NMHDR *pNMHDR, LRESULT *pResult);
};
