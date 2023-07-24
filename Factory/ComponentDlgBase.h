#pragma once
#include "json/json.h"

// CComponentDlgBase 对话框
//自定义组件基类
class CComponentDlgBase : public CDialogEx
{
	DECLARE_DYNAMIC(CComponentDlgBase)

public:
	CComponentDlgBase(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComponentDlgBase();

	//用于初始化组件的界面
	virtual void RefreshControls( Json::Value jsonRoot){};
	//用于从组件中获取到json配置内容
	virtual Json::Value GenerateJson(){Json::Value jv;return jv;};
	//用于判断组件的类型
	virtual bool IsMe(string & strStyle){return strStyle == "none";}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
};
