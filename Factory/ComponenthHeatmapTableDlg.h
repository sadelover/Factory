#pragma once
#include "json/json.h"
#include "ComponentDlgBase.h"
#include "Tools/CustomTools/CustomTools.h"
#include "gridctrl/GridCtrl.h"

// CComponenthHeatmapTableDlg 对话框
#define DEFAULT_NUMBER 20
class CComponenthHeatmapTableDlg : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponenthHeatmapTableDlg)

public:
	CComponenthHeatmapTableDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComponenthHeatmapTableDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_Heatmap_table };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "heatmap-table";}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSetXCount();
	afx_msg void OnBnClickedButtonSetYCount();
	afx_msg void OnBnClickedButtonInsertPoint();
	// X坐标系
	CGridCtrl m_grid_x;
	// Y轴坐标系
	CGridCtrl m_grid_y;
	// X坐标数量
	int m_nXCount;
	// Y坐标数量
	int m_nYCount;
	// 绑定点
	CString m_pointName;
	// colorValueStart
	int m_nColorValueStart;
	// colorValueEnd
	int m_nColorValueEnd;
	
};
