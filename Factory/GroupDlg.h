#pragma once

#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"

// CGroupDlg 对话框

class CGroupDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CGroupDlg)

public:
	CGroupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroupDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GROUP_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strGroupName;
	afx_msg void OnBnClickedOk();
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
};
