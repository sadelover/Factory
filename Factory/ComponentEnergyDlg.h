/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：ComponentEnergyDlg.h
* 文件标识：
* 摘    要：“通用组件设置”子窗口“能源管理”
*
* 取代版本：1.0 
* 原作者  ：Mark
* 完成日期：2020年2月25日
*/
#pragma once

#include "afxwin.h"
#include "json/json.h"
#include "ComponentDlgBase.h"
// CComponentEnergy 对话框

class CComponentEnergy : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentEnergy)

public:
	CComponentEnergy(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComponentEnergy();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_Energy };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "energy";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
};
