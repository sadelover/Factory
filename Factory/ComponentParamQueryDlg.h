/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：ComponentParamQueryDlg.h
* 文件标识：
* 摘    要：“通用组件设置”子窗口“参数快速查询组件”
*
* 取代版本：1.0 
* 原作者  ：Mark
* 完成日期：2020年2月25日
*/
#pragma once
#include "afxwin.h"
#include "ComponentDlgBase.h"
#include "json/json.h"
#include "gridctrl/GridCtrl.h"
#include "DataPoint/DataPointEntry.h"
#include "PointParamEditDlg.h"
#include "afxcmn.h"
#include "ComponentDlgBase.h"
// CComponentParamQuery 对话框

class CComponentParamQuery : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentParamQuery)

public:
	CComponentParamQuery(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComponentParamQuery();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_ParamQuery };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "ParamQuery";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
	
private:
	/**
	 *	@breif m_treeCtrl重命名事件（连点三下）
	*/
	afx_msg void OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	/**
	 *	@breif 添加一个根项
	*/
	afx_msg void OnBnClickedButtonAddRoot();
	/**
	 *	@breif 添加一个子项
	*/
	afx_msg void OnBnClickedButtonAddSub();
	/**
	 *	@breif 删除项
	*/
	afx_msg void OnBnClickedButtonDeleteItem();
	/**
	 *	@breif 插入点
	*/
	afx_msg void OnBnClickedButtonSeletepoint();

	/**
	 *	@brief 获取一个m_treeCtrl其中一项所在第几层
	 *	@param[in]	m_treeCtrl获取的项的变量
	 *	@return		0：不存在 ,1:第一层，以此类推。
	*/
	int GetTreeItemLevel(HTREEITEM item);

	//树控件
	CTreeCtrl m_treeCtrl;
	//m_treeCtrl 根列表
	vector<HTREEITEM> m_rootList;
};
