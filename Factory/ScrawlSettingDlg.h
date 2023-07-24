#pragma once


#include "afxcmn.h"
#include "afxwin.h"
#include "ReportCtrl.h"
#include "../ComponentDraw/EqmDrawDefines.h"


class CMainFrame;


// CScrawlSettingDlg dialog

class CScrawlSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScrawlSettingDlg)

public:
	CScrawlSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScrawlSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SCRAWL_SETTING };

public:
	void	SetLayer(const int nLayer);
	void	SetShowWords(const CString strShowWords);
	void	SetCurveColumn(const vector<CurveColumn>& vecCurve, const vector<CurveColumn>& vecColumn);

	int		GetPicId(void) const;
	int		GetLayer(void) const;
	CString	GetShowWords(void) const;
	void	GetCurveColumn(vector<CurveColumn>& vecCurve, vector<CurveColumn>& vecColumn) const;

private:
	void	InitListControl(void);
	void	InitComboControl(void);
	void	InitListValue(void);
	int		m_nLayer;
	CMainFrame* m_pMainFrame;
	int		m_nColorIndexCurve;
	int		m_nColorIndexColumn;

	void	InsertIntoListControl(const CurveColumn& stCurveColumn, const ScrawlListType& listType);
	void	DeleteFromListControlAndMemory(const ScrawlListType& listType);
	void	OpenSettingDlg(const int nCurSel, const ScrawlListType& listType);
	vector<CurveColumn>	m_vecCurve;
	vector<CurveColumn>	m_vecColumn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNMDblclkListCurve(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListColumn(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnCurveAdd();
	afx_msg void OnBnClickedBtnCurveDel();
	afx_msg void OnBnClickedBtnColumnAdd();
	afx_msg void OnBnClickedBtnColumnDel();
	CReportCtrl m_listCurve;
	CReportCtrl m_listColumn;
	CComboBox m_comboLayer;
	CString m_strShowWords;
};
