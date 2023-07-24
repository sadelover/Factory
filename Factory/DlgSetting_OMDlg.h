#pragma once

#include "json/json.h"

class CButtonPlus :public CButton
{

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

// CDlgSetting_OMDlg 对话框

class CDlgSetting_OMDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting_OMDlg)

public:
	CDlgSetting_OMDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetting_OMDlg();

// 对话框数据
	enum { IDD = IDD_CONN_SETTING_OM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();
	
private:
	// 天气显示 （显示则配置1，不显示则不配置）
	vector<CButtonPlus*> checkBtnList;
	BOOL m_bWeatherDisplay;
	CButtonPlus m_check_0;
	CButtonPlus m_check_1;
	CButtonPlus m_check_2;
	CButtonPlus m_check_3;
	CButtonPlus m_check_4;
	CButtonPlus m_check_5;
	CButtonPlus m_check_6;
	CButtonPlus m_check_7;
	CButtonPlus m_check_8;
	CButtonPlus m_check_9;
	CButtonPlus m_check_10;
	CButtonPlus m_check_11;
	CButtonPlus m_check_12;
	CButtonPlus m_check_13;
	CButtonPlus m_check_14;
	CButtonPlus m_check_15;
};

