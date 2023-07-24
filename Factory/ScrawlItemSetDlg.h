#pragma once


#include "afxwin.h"


class CDataPointManager;


// CScrawlItemSetDlg dialog

class CScrawlItemSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScrawlItemSetDlg)

public:
	CScrawlItemSetDlg(CDataPointManager* pPointMgr, CWnd* pParent = NULL);   // standard constructor
	virtual ~CScrawlItemSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SCRAWL_ITEM_SET };

public:
	void	SetPointName(const CString strName);
	void	SetColor(const COLORREF color);
	void	SetMaxValue(const int nMax);
	void	SetMinValue(const int nMin);
	void	SetRemarks(const CString strRemarks);

	CString	GetPointName(void) const;
	COLORREF GetColor(void) const;
	int		GetMaxValue(void) const;
	int		GetMinValue(void) const;
	CString	GetRemarks(void) const;

private:
	CString		m_strPointName;
	COLORREF	m_color;
	CDataPointManager*	m_pPointMgr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnBindPoint();
	int m_nMax;
	int m_nMin;
	CString m_strRemarks;
};
