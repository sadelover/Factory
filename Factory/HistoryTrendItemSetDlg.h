#pragma once


#include "afxwin.h"
#include "atlcomtime.h"
#include "../ComponentDraw/EqmDrawHistoryTrend.h"

class CDataPointManager;

// CHistoryTrendItemSetDlg dialog

class CHistoryTrendItemSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryTrendItemSetDlg)

public:
	CHistoryTrendItemSetDlg(CDataPointManager* pDataPointMgr, CWnd* pParent = NULL);   // standard constructor
	virtual ~CHistoryTrendItemSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_HISTORY_TREND_ITEM_SET };

public:
	void				SetHistoryTrendInfo(const HistoryTrendInfo& historyTrendInfo);
	HistoryTrendInfo	GetHistoryTrendInfo(void);

private:
	void	InitData(void);

	CDataPointManager*	m_pDataPointMgr;
	HistoryTrendInfo	m_historyTrendInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnBindPoint();
	afx_msg void OnStnClickedImageColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	COleDateTime m_dateStart;
	COleDateTime m_dateEnd;
	COleDateTime m_timeStart;
	COleDateTime m_timeEnd;
	BOOL m_bIsAutoCalcRange;
	CString m_strBindPointName;
	CButton m_btnBindPoint;
	CString m_strRemarks;
	int		m_nUpdateCycle;
	int		m_nLineWidth;
	float	m_fScale;
	COLORREF m_colorImage;
};
