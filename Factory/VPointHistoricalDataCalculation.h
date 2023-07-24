#pragma once

#include "gridctrl/GridCtrl.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "HttpOperation.h"
#include "DataPoint/DataPointEntry.h"
#include "json/json.h"
#include "DataPoint\DataPointManager.h"

// CVPointHistoricalDataCalculation 对话框
//历史数据补算窗口
const CString period_m1 = L"m1";
const CString period_m5 = L"m5";
const CString period_h1 = L"h1";
const CString period_d1 = L"d1";

class CVPointHistoricalDataCalculation : public CDialogEx
{
	DECLARE_DYNAMIC(CVPointHistoricalDataCalculation)

public:
	CVPointHistoricalDataCalculation(CWnd* pParent = NULL);   // 标准构造函数
	CVPointHistoricalDataCalculation(vector<DataPointEntry> &vec, CDataPointManager *pointManager, CWnd* pParent = NULL);   // 标准构造函数
	CVPointHistoricalDataCalculation(CString cstrPointName, CString cstrPointScript, CDataPointManager *pointManager, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVPointHistoricalDataCalculation();

	//界面中表格上面的信息打印
	void ShowMsg(CString& cstrMsg);
	void ShowMsg(const wchar_t* cstrMsg);
// 对话框数据
	enum { IDD = IDD_DIALOG_VPOINT_HSITORICAL_DATA_CALCULATION };
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCalculation();
	afx_msg void OnBnClickedButtonInsert();
	afx_msg LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonPointListCtl();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	//需要补算的点
	//补算公式
	struct PointStruct
	{
		CString cstrName;
		CString cstrScript;
		PointStruct(CString name,CString script):cstrName(name),cstrScript(script){}
	};
	vector<PointStruct> m_pointList;//补算点集合

	CDataPointManager* m_pointManager;
	//数据表控制
	CGridCtrl m_gridInput;
	//起止时间控制
	CDateTimeCtrl m_dateTime_from;
	CDateTimeCtrl m_dateTime_time_from;
	CDateTimeCtrl m_dateTime_to;
	CDateTimeCtrl m_dateTime_time_to;
	// 时间分割列表控件
	CComboBox m_combo_period;
	// static控件
	CStatic m_static_Msg;
	// ListBox点列表显示控件
	CListBox m_list_point;
	//http 参数
	CString m_CalcIp;
	int m_nCalcPort;

private:
	/*
	 *	@brief: 从界面获取参数，打包为json。调用pysite接口获取数据并插入界面
	*/
	bool CalculationAndDisplayPoints(void);
	/*
	 *	@brief: 只负责发单点调用和插入
	*/
	bool CalculationAndDisplayOnePoint(Json::Value& jsonSendPacket, int nIndex);
	/*
	 * @brief: 界面数据打包发送
	*/
	void InsertDatabase(void);
	/*
	 *	@brief: 设置列表行数并设置行号
	 *	@param[in]: nRow 不需加一，函数内已经加了
	*/
	void GridSetRow(int nRow);
	/*
	 * @brief: 表格控件清空方法
	*/
	void GridClear(void);
	/*
	 * @brief: 表格增加一组点名点值显示列
	 * @Param[in]:cstrPointName 新增一组时，需要一列时间和一列点值，点值的头需要显示点名。
	*/
	void GridAddGroup(CString cstrPointName, int nRow);
	/*
	 * @brief: 列表控件刷新,如果有新增的也会刷新进去
	*/
	void RefreshListCtrl(void);
	/*
	 * @brief: 计算和插入线程
	*/
	static UINT Thread_Calculation(LPVOID pParam);
	static UINT Thread_Insert(LPVOID pParam);
};

