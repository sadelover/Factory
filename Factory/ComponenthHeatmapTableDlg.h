#pragma once
#include "json/json.h"
#include "ComponentDlgBase.h"
#include "Tools/CustomTools/CustomTools.h"
#include "gridctrl/GridCtrl.h"

// CComponenthHeatmapTableDlg �Ի���
#define DEFAULT_NUMBER 20
class CComponenthHeatmapTableDlg : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponenthHeatmapTableDlg)

public:
	CComponenthHeatmapTableDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComponenthHeatmapTableDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_Heatmap_table };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "heatmap-table";}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSetXCount();
	afx_msg void OnBnClickedButtonSetYCount();
	afx_msg void OnBnClickedButtonInsertPoint();
	// X����ϵ
	CGridCtrl m_grid_x;
	// Y������ϵ
	CGridCtrl m_grid_y;
	// X��������
	int m_nXCount;
	// Y��������
	int m_nYCount;
	// �󶨵�
	CString m_pointName;
	// colorValueStart
	int m_nColorValueStart;
	// colorValueEnd
	int m_nColorValueEnd;
	
};
