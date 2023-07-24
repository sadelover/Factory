#pragma once
#include "afxdtctl.h"
#include "afxcmn.h"
#include "afxwin.h"


// CSearchStrategyInTimeAgeRunDlg 对话框
#define  WM_UPDATEDATA  WM_USER + 5 
class CSearchStrategyInTimeAgeRunDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchStrategyInTimeAgeRunDlg)

public:
	CSearchStrategyInTimeAgeRunDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearchStrategyInTimeAgeRunDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SEARCH_STRATEGY_IN_TIME_AGE_RUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	//从strLogFilePath文件中读出
	void ReadLogFileContents(CString& strLogFilePath, CString& strLogFileContents);
	//在strLogFileContents内容中找到最接近且小于time的时间并返回给closestTime
	//strLogFileContents来自log文件，格式稳定不变
	void SearchClosestTime(CString& strLogFileContents, COleDateTime& selectTime, COleDateTime& closestTime);

	// 日期选择器
	CDateTimeCtrl m_DTPicker_date;
	// 时间选择器
	CDateTimeCtrl m_DTPicker_time;
	// 编辑框
	//CString m_edit_findStrategy;
	CString m_edit_log;
	// 进度条
	CProgressCtrl m_progressctrl1;
	
	//策略与获取到的最接近选择时间的数据
	struct LogAndFindTime 
	{
		CString strlogName;
		COleDateTime time;
		CString strLogPath;
	};
	CListBox m_list_strategy;
	vector<LogAndFindTime> m_findTimeList;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLbnDblclkListStrategy();
	//搜索按钮执行得函数
	void BnClickedButtonSearch(void);
	
};
UINT Thread_ButtonSearch(PVOID param);