#pragma once

#include "../ControlEx/SkinDlg.h"

#include "afxbutton.h"
#include "../ControlEx/CommonButton.h"
#include "afxcmn.h"
#include "afxwin.h"

#include <shlobj.h>
#include <shellapi.h>
#include <TlHelp32.h>

#include "../DB_BasicIO/CommonDBAccess.h"
// CDlgLoad 对话框


class CDlgLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLoad)

public:
	CDlgLoad(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLoad();

// 对话框数据
	enum { IDD = IDD_LOAD };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	//afx_msg void OnBnClickedButtonChoosefile();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:

	CString m_strIP;
	//int m_nPort;
	//CString m_strUserName;
	//CString m_strRealDBName;
	//CString m_strDBName;
	//CString m_strDataSourceName;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();


	BOOL IsIPValid();
//	CCommonButton m_BtnOK;
//	CCommonButton m_BtnCancel;
	CCommonButton m_BtnUpload;
	CProgressCtrl m_ProgressCtrl;
	CStatic m_ProgessStatic;
//	CRedundencyManager *m_pRedundencyManager;

	CString m_strBakFilePath;


private:
	BOOL  m_bAutoLogin;
public:
//	BOOL m_bSimulatee;

	afx_msg void OnBnClickedCancel();
};
