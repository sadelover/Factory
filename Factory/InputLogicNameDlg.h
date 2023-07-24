#pragma once


#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"

// CInputLogicNameDlg 对话框

class CInputLogicNameDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CInputLogicNameDlg)

public:
	CInputLogicNameDlg(bool bIsCreate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInputLogicNameDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUT_LOGIC_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	bool	m_bIsCreate;	// true:新建策略, false:修改策略
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	CString m_strLogicName;
};
