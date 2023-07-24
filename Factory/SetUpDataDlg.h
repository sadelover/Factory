#pragma once

#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ComponentDraw/CEqmDrawPage.h"
#include "../ControlEx/CommonButton.h"

// CSetUpDataDlg dialog

class CEqmDrawPage;
class CMainFrame;
class CCanvasDlg;

class CSetUpDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetUpDataDlg)

public:
	CSetUpDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetUpDataDlg();

// Dialog Data
	enum { IDD = IDD_SETUPDATADLG };

private:
	void	EnableBackgroundControls(const BOOL bFlag);
	void	InitWebComboBox(void);
	CString	GetWebComboValue(void) const;
	void	SetWebComboValue(const CString strWebShow);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strEditOldValue;
	CString m_strEditNewValue;
	afx_msg void OnBnClickedButtonChooseBkPic();
	void	SetPage(CEqmDrawPage* pPage);

public:
	int m_nPicID;
	int m_nStartColor;
	int m_nEndColor;

	int m_nWidth;
	int m_nHeight;
	bool m_bObserverTopShow;
	PAGETYPE m_nPageType;

	bool m_bBackgroundImageModified; //背景图被改变
private:
	CEqmDrawPage* m_pPage;
	CMainFrame*	m_pMainFrm;

	CCanvasDlg*	m_pCanvasDlg;
	CString		m_strWebName[3];
	CString		m_strWebValue[3];

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticColorStart();
	afx_msg void OnStnClickedStaticColorEnd();
	CButton m_checkShow;
	afx_msg void OnBnClickedRadioFixedpage();
	afx_msg void OnBnClickedRadioFlottingpage();
	CComboBox m_combo_page_size;
	afx_msg void OnCbnSelchangeComboPageSize();
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	afx_msg void OnBnClickedCheckBk();
	BOOL m_bEnableBk;
	afx_msg void OnBnClickedRadioWebpage();
	int m_nPageTp;
	//CEdit m_editWebAddr;
	CString m_strWebAddr;
	CComboBox m_comboWebAddr;
	CString m_strPageId;
	CComboBox m_comboPageRight;
	int m_nPageMinRight;	//页面访问权限
	CString m_strTemplateVarsDefine;
	afx_msg void OnBnClickedRadioTemplatePage();
};
