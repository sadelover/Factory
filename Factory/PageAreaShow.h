#pragma once
#include "afxcmn.h"


class CSheetDeviceArea;

// CPageAreaShow dialog

class CPageAreaShow : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageAreaShow)

public:
	CPageAreaShow();
	virtual ~CPageAreaShow();

// Dialog Data
	enum { IDD = IDD_PAGE_AREA_SHOW };

private:
	void	InitControls(void);
	void	RefreshList(void);
	CSheetDeviceArea*	m_pSheet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnNMDblclkListRegion(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listRegion;
};
