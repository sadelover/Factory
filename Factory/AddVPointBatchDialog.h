#pragma once
#include "afxwin.h"


// CAddVPointBatchDialog 对话框

class CAddVPointBatchDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAddVPointBatchDialog)

public:
	CAddVPointBatchDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddVPointBatchDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_POINT_ADD_BATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_strVpointBatchlist;
};
