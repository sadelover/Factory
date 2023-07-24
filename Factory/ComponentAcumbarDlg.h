#pragma once
#include "afxwin.h"
#include "ComponentDlgBase.h"

#include "json/json.h"
#include "./Tools/CustomTools/CustomTools.h"
#include "gridctrl/GridCtrl.h"

// ComponentAcumbarDlg 对话框
//自定义组件子窗口 叠加柱状图组件
class ComponentAcumbarDlg : public CComponentDlgBase
{
	DECLARE_DYNAMIC(ComponentAcumbarDlg)

public:
	ComponentAcumbarDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ComponentAcumbarDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_accumbar };

	virtual void RefreshControls( Json::Value jsonRoot);
	virtual Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "accumbar";}
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
private:
	afx_msg void OnBnClickedButtonSetGridRow();
	afx_msg void OnBnClickedButtonAddPoint();
	/*
	 *	@brief 设置表格行数并绘制表头行数
	*/
	void SetGridRowCount(void);
	// 显示类型
	int m_radio_ShowStyle;
	// 值过滤-最大值
	int m_edit_nMax;
	// 值过滤-最小值
	int m_edit_nMin;
	// 需要被显示的列表
	CGridCtrl m_grid_list;
	//设置行的编辑框
	int m_nGridRow;

};
