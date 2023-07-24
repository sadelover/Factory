#pragma once

#include "afxcmn.h"


class CSheetDeviceArea;

// CPageDeviceShow dialog

class CPageDeviceShow : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageDeviceShow)

public:
	CPageDeviceShow();
	virtual ~CPageDeviceShow();

// Dialog Data
	enum { IDD = IDD_PAGE_DEVICE_SHOW };

public:
	CSheetDeviceArea*	m_pSheet;

private:
	void	InitControls(void);
	void	RefreshList(void);	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnNMDblclkListDevice(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listDevice;
};
