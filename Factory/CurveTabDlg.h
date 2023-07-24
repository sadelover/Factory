#pragma once
#include "afxwin.h"
#include "RealTimeTrendDlg.h"

#define DLG_WIDTH 775
#define DLG_HEIGHT 520

class CUPPCServerDataAccess;
class CPageDlg;

// CCurveTabDlg dialog

class CCurveTabDlg : public CDialog
{
	DECLARE_DYNAMIC(CCurveTabDlg)

public:
	CCurveTabDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCurveTabDlg();

// Dialog Data
	enum { IDD = IDD_CURVETAB_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_CurveTab;
	CRealTimeTrendDlg m_CurvePage1;
	CRealTimeTrendDlg m_CurvePage2;
	CRealTimeTrendDlg m_CurvePage3;
	CRealTimeTrendDlg m_CurvePage4;
	CRealTimeTrendDlg m_CurvePage5;
	CUPPCServerDataAccess*	m_pServerDataAccess;
	CPageDlg *m_pBEOPGatewayDlg;
	std::vector<wstring>  m_iniWatchPointList;
public:
	afx_msg void OnTcnSelchangeTab3(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL OnInitDialog();
	void InitData();
	void Exit();
	void DataPointVector(std::vector<wstring>  m_iniWatchPointList);
	void CurvePageInitPointView();
};
