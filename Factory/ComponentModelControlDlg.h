/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：ComponentModelControlDlg.h
* 文件标识：
* 摘    要：“通用组件设置”子窗口“模式控制自定义组件”
*
* 取代版本：1.0 
* 原作者  ：Mark
* 完成日期：2020年2月25日
*/
#pragma once
#include "afxwin.h"
#include "json/json.h"
#include "ComponentDlgBase.h"
// CComponentModelControlDlg 对话框

class CComponentModelControlDlg : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentModelControlDlg)

public:
	CComponentModelControlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComponentModelControlDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_ModelControl };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "modelControl";}
	CString m_cstrJsonPacket;//接收或返回的json包

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();

	int m_nModelType;// 模式类型
};
