#pragma once

//#include "afxdtctl.h"

// CTimeSelectPropertyDlg 对话框

class CDataPointManager;

class CTimeSelectPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeSelectPropertyDlg)

public:
	CTimeSelectPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimeSelectPropertyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TIME_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonTimePointSelect();
	afx_msg void OnBnClickedButtonTimeSelectRw();
	void SetPointMap( CDataPointManager* pPointMap );

	CDataPointManager*				m_pPointMap;
	CString							m_strBindPointName;
	int								m_rwProperty;//读写选择
	int								m_radioDateTimeSelect;//时间数据格式选择

};
