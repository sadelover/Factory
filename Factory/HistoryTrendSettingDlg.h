#pragma once


#include "afxwin.h"
#include "atlcomtime.h"
#include "../ComponentDraw/EqmDrawHistoryTrend.h"
#include "afxcmn.h"
#include "ReportCtrl.h"

class CMainFrame;


// CHistoryTrendSettingDlg dialog

class CHistoryTrendSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryTrendSettingDlg)

public:
	CHistoryTrendSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHistoryTrendSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_HISTORY_TREND_SETTING };

public:
	void						SetLayer(const int nLayer);
	void						SetHistoryTrendType(const HistoryTrendType& hisTrendType);
	void						SetHistoryTrendVector(const vector<HistoryTrendInfo>& vecHisTrendInfo);

	int							GetPicId(void) const;
	int							GetLayer(void) const;
	HistoryTrendType			GetHistoryTrendType(void) const;
	vector<HistoryTrendInfo>	GetHistoryTrendVector(void) const;

private:
	void	InitControls(void);
	void	InitData(void);

	void	OpenSettingDlg(const int nCurSel, const HistoryTrendType& hisTrendType);


	CMainFrame* m_pMainFrame;
	int	m_nColorIndex;
	vector<HistoryTrendInfo>	m_vecHistoryTrendInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboLayer;
	CComboBox m_comboImageType;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnHisAdd();
	afx_msg void OnBnClickedBtnHisDel();
	afx_msg void OnNMDblclkListHistoryTrend(NMHDR *pNMHDR, LRESULT *pResult);
	CReportCtrl m_listHistoryTrend;
	int m_nLayer;
	int m_nImageType;
};
