#pragma once
#include "atlcomtime.h"
#include "afxwin.h"


// CCompletionData 对话框

class CCompletionData : public CDialogEx
{
	DECLARE_DYNAMIC(CCompletionData)

public:
	CCompletionData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCompletionData();

// 对话框数据
	enum { IDD = IDD_DIALOG_COMPLETION_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CComboBox m_TimeDivisionCon;
	DECLARE_MESSAGE_MAP()
public:
	CString m_CompletionDataList;
	COleDateTime m_tFrom;
	COleDateTime m_tTo;
	int m_nPeriodType;
	afx_msg void OnEnChangeEditPoint();
	// 时间分割选择控制
	
	virtual BOOL OnInitDialog();
};
