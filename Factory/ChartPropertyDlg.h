#pragma once

#include "atltime.h"
#include "afxcmn.h"
#include "ReportCtrl.h"
#include "afxwin.h"
#include "../ComponentDraw/EqmDrawDefines.h"

// CChartPropertyDlg 对话框
class CEqmDrawBase;
class CDataPointManager;
class CEqmDrawLineGraphic;
class CEqmDrawBarGraphic;
class CEqmDrawPieGraphic;

class CChartPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CChartPropertyDlg)

public:
	CChartPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	CChartPropertyDlg(CEqmDrawBase* pBase, CWnd* pParent = NULL);
	virtual ~CChartPropertyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHART_PROPERTY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strChartTitle;						//图表显示的标题
	int m_nUpdateInterval;							//更新时间间隔
	CString m_strXName;								//横轴显示的名称
	CString m_strXUnit;								//横轴单位
	CString m_strYName;								//纵轴显示的名称
	CReportCtrl m_listGraphItem;					//图表项的列表控件
	CComboBox m_comboXUnit;							//下拉组合框控件
	E_BAR_TYPE m_eBarType;							//柱状图的类型
	CString m_strStartPoint;						//绑定的起始时间点
	CString m_strEndPoint;							//绑定的结束时间点
	afx_msg void OnBnClickedButtonAddItem();
	afx_msg void OnBnClickedButtonDeleteItem();
	void	SetPointMap(CDataPointManager* pPointMap);
	int m_nQueryType;
	COLORREF m_nTextColor;
private:
	void	ShowWindowTitle();
	void	ShowDlgItem();
	void	EnableLeftBottomControls(const BOOL bShow);
	void	InitListCtrl();
	void	InitComboCtrl();
	void	InitBarComboCtrl();
	void	EnableCompareBarControls(const BOOL bShow);
	void	EnableItemControls(const BOOL bShow);
	void	EnableMaxValueControl(const BOOL bShow);
	void	EnableTimePointSelectControls(const BOOL bShow);
	CDataPointManager* m_pPointMap;
	CEqmDrawBase* m_pBase;
	CEqmDrawLineGraphic* m_pLineGraphic;
	CEqmDrawBarGraphic* m_pBarGraphic;
	CEqmDrawPieGraphic* m_pPieGraphic;
	int	m_nColorIndex;
public:
	afx_msg void OnNMDblclkListGraphItemList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	int IsValidDate( COleDateTime startDate, COleDateTime endDate, COleDateTime startTime, COleDateTime endTime );
	CComboBox m_combo_bar_type;
	afx_msg void OnCbnSelchangeComboBarType();
	afx_msg void OnBnClickedButtonAssoPoint1();
	afx_msg void OnBnClickedButtonAssoPoint2();
	CString m_strAssoPoint1;
	CString m_strAssoPoint2;
	CString m_strNameItem1;
	CString m_strNameItem2;
	CString m_strMaxValue;
	afx_msg void OnBnClickedButtonStartPoint();
	afx_msg void OnBnClickedButtonStartPointClear();
	CComboBox m_combo_query_type;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticTextColor();
	double m_dYMax;
	double m_dYMin;
	int m_nCheckMax;
	int m_nCheckMin;
	CComboBox m_comboLayer;
	int m_nLayer;
};
