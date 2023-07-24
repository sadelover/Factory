#pragma once

#include "afxwin.h"
#include "AddAreaDlg.h"
#include "../ComponentDraw/EqmDrawAreaChange.h"

// CAreaChagePropertyDlg dialog

class CAreaChagePropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CAreaChagePropertyDlg)

public:
	CAreaChagePropertyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAreaChagePropertyDlg();
	virtual BOOL OnInitDialog();
	void	InitList();
	void	UpdateList();

	void	SetAreaInfoVec(vector<_tagAreaInfo> vecAreaInfo);
	vector<_tagAreaInfo>	GetAreaInfoVec();

	void        InitComboLayer();
// Dialog Data
	enum { IDD = IDD_DIALOG_AREA_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_listAreaChange;
	int m_dXPos;
	int m_dYPos;
	int m_dWidth;
	int m_dHeight;
	int m_layer;
	CComboBox	m_comboLayer;
	afx_msg void OnBnClickedButtonAddArea();
	afx_msg void OnBnClickedButtonDeleteArea();
	afx_msg void OnBnClickedOk();
	vector<_tagAreaInfo>	m_vecAreaInfo;
	afx_msg void OnNMDblclkListAreas(NMHDR *pNMHDR, LRESULT *pResult);
};
