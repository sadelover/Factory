#pragma once
#include "afxwin.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"
#include "afxcmn.h"
#include "../ControlEx/SkinDlg.h"

// SelectDataComId dialog

class SelectDataComId : public CSkinDlg
{
	DECLARE_DYNAMIC(SelectDataComId)

public:
	SelectDataComId(CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog();
	virtual ~SelectDataComId();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void InitIDTable();
	void InitList();
	void UpdateList();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddid();
	afx_msg void OnBnClickedButtonDeleteid();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	vector<CString>     m_vecDataName;
	CListCtrlEx    m_list_select_id;
	CDataPointManager* m_refpointmap;

public:
	void	SetPointMap(CDataPointManager* pointmap);
};
