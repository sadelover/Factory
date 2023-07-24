/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：ComponentBigDataAnalysisDlg.h
* 文件标识：
* 摘    要：“通用组件设置”子窗口“大数据相关性分析组件”
*
* 取代版本：1.0 
* 原作者  ：Mark
* 完成日期：2020年  月  日
*/
#pragma once
#include "afxwin.h"
#include "ComponentDlgBase.h"
#include "json/json.h"
#include "gridctrl/GridCtrl.h"
#include "DataPoint/DataPointEntry.h"
#include "PointParamEditDlg.h"

// CComponentBigDataAnalysis 对话框

class CComponentBigDataAnalysis : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentBigDataAnalysis)

public:
	CComponentBigDataAnalysis(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComponentBigDataAnalysis();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_BigDataAnalysis };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "BigDataAnalysis";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSetinputline();
	afx_msg void OnBnClickedButtonSetoutputline();
	//输入部分
	CGridCtrl m_gridInput;
	//输出部分
	CGridCtrl m_gridOutput;
	// 数据样本名
	CString m_cstrBindName;
	// 输入侧行数
	int m_nInputRow;
	// 输出侧行数
	int m_nOutputRow;
};
