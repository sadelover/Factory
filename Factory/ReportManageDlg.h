#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "BEOPLogicManager.h"


// CReportManageDlg dialog

class CReportManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReportManageDlg)

public:
	CReportManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportManageDlg();
	CString         m_strServer;	      
	int             m_nServerPort;
	
// Dialog Data
	enum { IDD = IDD_DIG_REPORT_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	CButton m_btnAdd;
	CButton m_btnModify;
	CButton m_btnDelete;
	CListCtrl m_list_Report;
	CString	m_strSysPath;
	
	vector<int>m_nidList;
	vector<CString> m_strReportNameList;
	vector<CString> m_strReportDescriptionList;
	vector<int>   m_nFileTypeList;
	vector<CString> m_strReportContentList;
	CString			m_strReportName;
	CString			m_strReportInfo;
	CString         m_strFilePath;
	CString         m_strReportDescr;
	

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
     
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonDownloadReport();
	afx_msg void OnBnClickedButtonDownloadReportGen();
};
