#pragma once

#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"

// CGroupDlg �Ի���

class CGroupDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CGroupDlg)

public:
	CGroupDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGroupDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_GROUP_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strGroupName;
	afx_msg void OnBnClickedOk();
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
};
