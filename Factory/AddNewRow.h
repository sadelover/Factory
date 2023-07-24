#pragma once

#include "DataPoint/DataPointEntry.h"
#include "../ControlEx/ListCtrlEx.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "afxwin.h"
// AddNewRow dialog

class AddNewRow : public CSkinDlg
{
	DECLARE_DYNAMIC(AddNewRow)

public:
	 // standard constructor
	AddNewRow(CWnd* pParent = NULL); 
	virtual ~AddNewRow();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_Add_New_Row };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()

public:
	void SetDestListCtrl(CListCtrlEx* m_l);

public:
	UINT		m_nIndex;
	CString		m_strShortName;
	CString		m_strPlcName;
	CString		m_strRwProperty;
	CString		m_strDescription;
	CString		m_strUnit;
	double		m_nPLCPointMaxVal;
	double		m_nPLCPointMinVal;

	CListCtrlEx* m_listctrl;
	CComboBox	m_combotype;
	CString		m_combotypestring;
	
private:
	CCommonButton	m_btnOK;
	CCommonButton	m_btnCancel;
	CCommonButton	m_btnClear;

public:
	double m_maxmaxval;
	double m_minminval;

	CStringArray arraytypestring;
};
