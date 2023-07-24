#pragma once


#include "afxwin.h"
#include "atlcomtime.h"
#include "afxcmn.h"


// CTacticLogQueryDlg 对话框

class CTacticLogQueryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTacticLogQueryDlg)

public:
	CTacticLogQueryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTacticLogQueryDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TACTIC_LOG };

public:
	CString	m_strDllName;
	CString m_strDllNameOrg;

private:
	CString	m_strDbIp;
	CString CTacticLogQueryDlg::UTF_8ToGB2312(CString cstr);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDtnDatetimechangeDatetimeTactLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnQuery();
	COleDateTime m_dateLog;
	CString m_strTactLog;
	CEdit m_Edit_Log;
	afx_msg void OnBnClickedBtnClearTillNow();
	COleDateTime m_tLocateTime;
	BOOL m_bLocateTime;


	BOOL m_bLogDBExist;
	afx_msg void OnBnClickedButtonKeywordSearch();
private:
	// 需要搜索的关键字
	CString m_cstrKeyWordSearch;
	// 关键字出现的行数组成的树
	CTreeCtrl m_treeKeywordLine;
public:
	afx_msg void OnTvnSelchangedTreeKeywordLine(NMHDR *pNMHDR, LRESULT *pResult);
};
