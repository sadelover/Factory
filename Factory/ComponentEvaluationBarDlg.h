/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：ComponentEvaluationBarDlg.h
* 文件标识：
* 摘    要：“通用组件设置”子窗口“评估条组件”
*
* 取代版本：1.0 
* 原作者  ：Mark
* 完成日期：2020年2月25日
*/

#pragma once
#include "afxwin.h"
#include "json/json.h"
#include "afxcolorbutton.h"
#include "../DataPoint/DataPointManager.h"
#include "ComponentDlgBase.h"
// CComponentEvaluationBar 对话框

class CComponentEvaluationBar : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentEvaluationBar)

public:
	CComponentEvaluationBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComponentEvaluationBar();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_EvaluationBar };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "evaluationBar";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedCheckBgallow();
	afx_msg void OnBnClickedButtonSeletepoint();
	afx_msg void OnBnClickedButtonClearpoint();

	UINT	CstrRGBToUIntBGR(CString cstrRGB);
	string	UIntBGRToStrRGB(UINT nBGRColor);
	CString UIntBGRToCStrRGB(UINT nBGRColor);

	// 标题
	CString m_cstrTitle;
	// 绑定点名
	CString m_cstrBindingPoint;
	// 起始值
	float m_fStart;
	// 结束
	float m_fEnd;
	// 刻度
	CString m_cstrSegments;
	// 分区颜色
	CString m_cstrAreaColor;
	// 第N个分区
	CString m_cstrArea;
	// 分区颜色
	CMFCColorButton m_colorBTArreaColor;
	// 刻度线、数字、标题的颜色
	CMFCColorButton m_colorbtThemesColor;
	// 背景色
	CMFCColorButton m_colorbtBGColor;
	// 背景色允许位
	BOOL m_bBGclolor;
	//评估条样式
	int m_nSubType;


	// 箭头颜色控件
	CMFCColorButton m_colorbtArrow;
};
