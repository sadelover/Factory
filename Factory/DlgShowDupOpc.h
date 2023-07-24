#pragma once


// CDlgShowDupOpc dialog

#include "DataPoint/DataPointEntry.h"
#include "../ControlEx/ListCtrlEx.h"
#include "../ControlEx/SkinStatic.h"
#include "../ControlEx/SkinDlg.h"

class CDlgShowDupOpc : public CSkinDlg
{
    DECLARE_DYNAMIC(CDlgShowDupOpc)
    DECLARE_MESSAGE_MAP()

public:
	CDlgShowDupOpc(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShowDupOpc();

// Dialog Data
	enum { IDD = IDD_SHOW_DUP_OPC };
    typedef vector<DataPointEntry> VEC_ENTRY;

    virtual BOOL    OnInitDialog();
    void        SetVecEntryDuplicate(const VEC_ENTRY& vecentry);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    void InitList();
    void UpdateList();

protected:
    VEC_ENTRY           mVecEntryDuplicate; //bird,for save duplicate item
public:
	CListCtrlEx m_listctrl_dup_opc;
	CStringArray	m_arrayProperty;	//读写属性
	CStringArray	m_arrayUnit;		//单位
	CStringArray	m_arraytype;		//类型
	VEC_ENTRY        mVecEntry;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnLvnItemchangedListDupOpc(NMHDR *pNMHDR, LRESULT *pResult);
};
