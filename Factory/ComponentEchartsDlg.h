/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：CComponentEchartsDlg.h
* 文件标识：
* 摘    要：“通用组件设置”子窗口“迷你折线图”
*
* 取代版本：1.0 
* 原作者  ：Mark
* 完成日期：2020年2月25日
*/
#pragma once
#include "afxwin.h"
#include "json/json.h"
#include "afxcolorbutton.h"
#include "ComponentDlgBase.h"
// CComponentEcharts 对话框

class CComponentEcharts : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentEcharts)

public:
	CComponentEcharts(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComponentEcharts();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_Echarts };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "echarts";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 绑定点
	CString m_strBindPoint;
	// 标题预留高度
	int m_nTitleRsvdHeight;
	// 标题字体大小
	int m_nTitleFontSize;
	// 标题对齐
	CComboBox m_comboxTitleAlignment;
	// 标题字体粗细
	CComboBox m_comboxTitleFontWeight;
	// x轴刻度标签预留宽度
	int m_nXWidth;
	// Y轴刻度标签预留宽度
	int m_nYWidth;
	// 纵坐标取值范围0~500（配置成多大就是0~max）
	int m_nMax;
	// 标题颜色
	CMFCColorButton m_colorbtTitleColor;
	// X轴刻度标签颜色
	CMFCColorButton m_colorbtXColor;
	// Y轴刻度标签颜色
	CMFCColorButton m_colorbtYColor;
	// 网格线颜色
	CMFCColorButton m_colorbtGridLineColor;
	// 网格区域颜色
	CMFCColorButton m_colorbtGridAreaColor;

	UINT	CstrRGBToUIntBGR(CString cstrRGB);
	string	UIntBGRToStrRGB(UINT nBGRColor);
	CString UIntBGRToCStrRGB(UINT nBGRColor);

	string	GetTitleAlignmentStr(void);
	void	SetTitleAlignmentComBox(string str);
	string	GetTitleFontWeightStr(void);
	void	SetTitleFontWeightComBox(string str);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonSeletepoint();
	afx_msg void OnBnClickedButtonClearpoint();
};
