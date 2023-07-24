#pragma once

#include "../ComponentDraw/EqmDrawDefines.h"
#include "../ControlEx/SkinStatic.h"
#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"

// CGraphicInfoDlg dialog

class CGraphicInfoDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CGraphicInfoDlg)
	DECLARE_MESSAGE_MAP()
public:
	CGraphicInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphicInfoDlg();
	// Dialog Data
	enum { IDD = IDD_GRAPHICINFODLG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void InitComboLayer();

public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnStnClickedStaticLinecolor();
	afx_msg void OnCbnSelchangeCmbtransparent();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnStnClickedStaticFillcolor();

public:

	CSkinStatic m_stcLineColor;
	CSkinStatic m_stcFillColor;
	CButton m_bIsFillColor;
	CComboBox m_comboLayer;
	CCommonButton m_btn_ok;
	CCommonButton m_btn_cancle;
	
	EQMDRAWTYPE m_type;
	BOOL m_bIsDashLine;
	int m_nLineWidth;
	DWORD m_LineColor;
	DWORD m_FillColor;
				
	int     m_layer;
};