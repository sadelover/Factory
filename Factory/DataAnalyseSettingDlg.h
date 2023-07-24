#pragma once


#include "afxwin.h"
#include "afxcmn.h"
#include "../ComponentDraw/EqmDrawDataAnalyse.h"

class CDataPointManager;
class CMainFrame;

// CDataAnalyseSettingDlg dialog

class CDataAnalyseSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataAnalyseSettingDlg)

public:
	CDataAnalyseSettingDlg(CDataPointManager* pDataPointMgr, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataAnalyseSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DATA_ANALYSE };

public:
	vector<DaPointInfo>	GetDaPointInfo(void) const;
	void	SetDaPointInfo(const vector<DaPointInfo>& data);

private:
	void	InitControls(void);
	void	InitData(void);

	CDataPointManager*	m_pDataPointMgr;
	vector<DaPointInfo>	m_vecDaPtInfo;
	CMainFrame*			m_pMainFrm;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CComboBox m_comboLayer;
	int m_nLayer;
	afx_msg void OnNMDblclkListPointInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnBrowseTime();
	afx_msg void OnBnClickedBtnBrowseCount();
	afx_msg void OnBnClickedBtnBrowseStatus();
	CListCtrl m_listPtInfo;
	CString m_strPtTime;
	CString m_strPtCount;
	CString m_strPtStatus;
};
