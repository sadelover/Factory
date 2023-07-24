#pragma once
#include "afxdtctl.h"


// CExportHistoricalDataAtPreciseTimeDLg 对话框

class CExportHistoricalDataAtPreciseTimeDLg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportHistoricalDataAtPreciseTimeDLg)

public:
	CExportHistoricalDataAtPreciseTimeDLg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExportHistoricalDataAtPreciseTimeDLg();

// 对话框数据
	enum { IDD = IDD_Export_HistoricalData_At_PreciseTime };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 日期控制类
	CDateTimeCtrl m_CDateControl;
	// 时间控制类
	CDateTimeCtrl m_CTimeControl;
	//保存按钮服务函数
	afx_msg void OnBnClickedSaveAs();
};
