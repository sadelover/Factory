#pragma once

#include "afxwin.h"
//#include "afxcmn.h"
//#include "afxwin.h"

class CEqmDrawTimeBar;
class CDataPointManager;

// CTimeBarPropertyDlg 对话框

class CTimeBarPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeBarPropertyDlg)

public:
	CTimeBarPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	CTimeBarPropertyDlg(CEqmDrawTimeBar* pTimeBar, CDataPointManager* pMap, CWnd* pParent = NULL);   
	virtual ~CTimeBarPropertyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TIME_BAR_PROPERTY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEqmDrawTimeBar* m_pTimeBar;
	CDataPointManager* m_pMap;
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticTimeBarColor();
	afx_msg void OnBnClickedButton1TimeBarSelectPoint();
	void	InitListControl();
	CListCtrl m_list;
	afx_msg void OnBnClickedButtonTimeBarAddItem();
	afx_msg void OnBnClickedButtonTimeBarDeleteItem();
	afx_msg void OnBnClickedButtonTimeBarUpItem();
	afx_msg void OnBnClickedButtonTimeBarDownItem();
	afx_msg void OnNMDblclkListTimeBarItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	CComboBox m_combo_layer;
	afx_msg void OnBnClickedButton1TimeBarSelectPoint3();
	CComboBox m_combo_query_type;
};
