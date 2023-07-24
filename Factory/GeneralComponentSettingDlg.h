/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：GeneralComponentSettingDlg.h
* 文件标识：
* 摘    要：“通用组件设置”窗口
*
* 当前版本： 1.1
* 作 者： Mark
* 完成日期： 2020年11月11日
*
* 取代版本：1.0 
* 原作者  ：Mark
* 完成日期：2020年2月25日
*/
#pragma once


#include "afxwin.h"
#include "ComponentEnergyDlg.h"//能源管理组件 energy
#include "ComponentParamQueryDlg.h"//数据查询组件 ParamQuery
#include "ComponentReportHistoryDlg.h"//报表下载组件
#include "ComponentEchartsDlg.h"//迷你折线图 echarts
#include "ComponentBigDataAnalysisDlg.h"//大数据相关性分析组件 BigDataAnalysis
#include "ComponentEvaluationBarDlg.h"//评估条组件 evaluationBar
#include "ComponentModelControlDlg.h"//模式控制自定义组件
#include "ComponentAcumbarDlg.h"//叠加柱状图组件
#include "ComponenthHeatmapTableDlg.h"//通用表格热图组件

// CGeneralComponentSettingDlg 对话框

class CGeneralComponentSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGeneralComponentSettingDlg)

public:
	CGeneralComponentSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGeneralComponentSettingDlg();
// 对话框数据
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING };

	virtual BOOL OnInitDialog();

	int m_nLayer;//图层
	CString m_strCfgValue;//Json包，这个窗口的主要入口参数
	//CString m_strCfgValue_Backup;//进入界面后备份的json脚本
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	/**
	 *	@breif ComBox切换事件服务函数
	*/
	afx_msg void OnCbnSelchangeComboTemplate();
	/**
	 * @breif “切换视图”按键服务函数
	*/
	afx_msg void OnBnClickedSwitchCodeView();
	/**
	 * @breif ok
	*/
	afx_msg void OnBnClickedOk();

	//处理鼠标和键盘事件
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	/**
	 *	@brief 通过字符串去比对出具体是combox的哪个索引指向的界面
	 *	@details 字符串来自于解析的组件json包中的type关键字
	 *	@return	返回一个与comboBox中对应的索引
	*/
	int		GetIndexInComboBoxType(string& strType);

	/**
	 * @brief 选择显示的界面,初始化界面
	 * @param[in] nCurSel combo box的索引，应通过combo box的方法获取索引作为参数
	 * @param[in] JsonRoot 初始化界面使用的数据
	*/
	void	SwitchShowWindow(int nCurSel, Json::Value JsonRoot);
	/**
	 * @brief 选择显示的界面，不初始化界面
	*/
	void	SwitchShowWindow(int nCurSel);
	/**
	 * @brief 隐藏所有的子窗口
	*/
	void	HideAllChildDlg(void);

	//组件功能外的控件初始化
	void	InitControls(void);

	/**
	 * @brief 组件初始化
	 * @details 在此处增加新组件即可
	*/
	void	InitComponent(void);
	//界面索引 仅在初始化combo box时使用 基本可以删除
	typedef enum {
		None = 0,
		energy,
		ParamQuery,
		reportHistory,
		echarts,
		BigDataAnalysis,
		evaluationBar,
		modelControl,
		accumbar,
		heatmap_table
	} eTempalteComboBoxIndex;
	//组件子窗口类声明 所有组件均继承自CComponentDlgBase类，
	CComponentEnergy			m_ChildPanel_Energy;//能源管理组件 energy
	CComponentParamQuery		m_ChildPanel_ParamQuery;//数据查询组件 ParamQuery
	CComponentReportHistory		m_ChildPanel_ReportHistory;//报表下载组件
	CComponentEcharts			m_ChildPanel_Echarts;//迷你折线图 echarts
	CComponentBigDataAnalysis	m_ChildPanel_BigDataAnalysis;//大数据相关性分析组件 BigDataAnalysis
	CComponentEvaluationBar		m_ChildPanel_EvaluationBar;//评估条组件 evaluationBar
	CComponentModelControlDlg	m_ChildPanel_ModeControl;//模式控制自定义组件
	ComponentAcumbarDlg			m_ChildPanel_Accumbar;//叠加柱状图组件
	CComponenthHeatmapTableDlg	m_ChildPanel_HeatmapTable;//通用表格热图组件

	//图层选择控件
	CComboBox m_comboLayer;
	//组件类型选择框
	CComboBox m_comboTempalte;
	//组件组合框上一次的选择
	int m_nSelectedTempalte;

//获取编辑框显示的状态
#define IsJsonEditVisible()	GetDlgItem(IDC_EDIT_CFG_VALUE)->IsWindowVisible()
};

