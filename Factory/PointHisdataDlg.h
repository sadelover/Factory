#pragma once

#include "gridctrl/GridCtrl.h"
#include "afxdtctl.h"
#include "atlcomtime.h"
#include "json/json.h"
#include "afxwin.h"
// CPointHisdataDlg dialog

class CPointHisdataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPointHisdataDlg)

public:
	CPointHisdataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPointHisdataDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_POINT_HISDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


	CGridCtrl m_Grid;

public:
	afx_msg void OnBnClickedButtonQuery();
	COleDateTime m_DateFrom;
	COleDateTime m_DateTo;
	COleDateTime m_TimeFrom;
	COleDateTime m_TimeTo;



	COleDateTime m_tStart;
	COleDateTime m_tEnd;
	CCellID m_m_idCurrentCell;

	CString m_strServer;
	CString m_strResult;
	int m_nServerPort;
	int m_nRowIndexMin;
	int m_nRowIndexMax;
	int m_nColumnMin ;
	int m_nColumnMax;

	vector<CString> m_strPointNameList ;
	int m_nTimePeriod;
	afx_msg void OnBnClickedButtonDownload();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonRemove();
	bool SaveHistoryData(Json::Value requestModifyList,CString strNewValue);
	void DisplayAllData();
	afx_msg void OnCbnSelchangeComboStep();
	CListBox m_lstPointName;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRemoveInRange();
	afx_msg void OnBnClickedButtonImportFromText();
};
