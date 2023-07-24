#pragma once

#include "gridctrl/GridCtrl.h"
#include "afxwin.h"

// CDataSampleManageDlg dialog

class CDataSampleManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataSampleManageDlg)

public:
	CDataSampleManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataSampleManageDlg();
// Dialog Data
	enum { IDD = IDD_DIALOG_DATA_SAMPLE_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	int sampleid;						//Ñù±¾id	

public:
	afx_msg void OnBnClickedButtonQuery();


	COleDateTime m_DateFrom;
	COleDateTime m_DateTo;
	COleDateTime m_TimeFrom;
	COleDateTime m_TimeTo;



	COleDateTime m_tStart;
	COleDateTime m_tEnd;

	
	CString m_strServer;
	int m_nServerPort;
    vector<CString>m_strPointNameList;
	vector<CString>m_strSampleNameList;

	CGridCtrl m_Grid;										
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboSamplelist();

	afx_msg void OnBnClickedButtonDownload();
	CButton m_CDownLoad;
	CComboBox m_comboSampeList;
	afx_msg void OnBnClickedButtonCreateSample();
	afx_msg void OnBnClickedButtonImportSample();
	afx_msg void OnBnClickedButtonDeleteSample();
	bool LoadExcelSampleFile(CString strSamplePath);
	bool ReadSampleDataFromCSV(CString strSamplePath,vector<vector<CString>>&vecSampleList,vector<vector<CString>>&vecInputList,vector<vector<CString>>&vecOutputList);
};
