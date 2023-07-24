#pragma once

#include "EditListCtrl.h"

#include "DB_BasicIO/DatabaseSession.h"
#include "DB_BasicIO/RealTimeDataAccess.h"
#include "DataPoint/DataPointManager.h"
#include "DataPoint/LogicPointManager.h"
#include "DataPoint/AlarmPointManager.h"
#include "../ComponentDraw/sqlite/SqliteAcess.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "afxcmn.h"
#include "afxwin.h"

class CUPPCServerDataAccess;
class CDataPointManager;
class CPageDlg;

#define POINT_LISTCTRL_ID (0x1009)
#define WM_MYMSG_PRINT_POINT WM_USER+22
// CPointView йсм╪

class CPointDialog : public CDialog
{
	DECLARE_DYNAMIC(CPointDialog)
	DECLARE_MESSAGE_MAP()

public:
	CPointDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPointDialog();
	bool ThreadExitFinish();


	// Dialog Data
	enum { IDD = IDD_DIALOG_Point};
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnMyMsgPrintPoint(WPARAM wParam,LPARAM lParam);
	void ReadRealDBparam(gDataBaseParam &dbParam);
	void RefreshFromDB();
	void InitData();

	int	m_nSetPointValueTimeout;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
public:
	void	StartScanThread(void);
	void	EndScanThread(void);

private:
	vector<DataPointEntry>	GetSearchDataVector(CString strSearch);
	void					ResetPointListInterface(const vector<DataPointEntry>& vecData);
	void					ResetSearchValue(const vector<DataPointEntry>& vecData);

	static unsigned int WINAPI ScanPointTableInfo(void* pArguments);
	void				MonitorPointsInfo(void);
	bool				CheckPointsInfo(const vector<DataPointEntry>& vec1, const vector<DataPointEntry>& vec2);
	bool				m_bScanRun;
	vector<DataPointEntry>	m_vecCheck;

	//	CEditListCtrl* m_pEditListCtrl;

	bool                 m_bExitThread;
	bool				 m_bThreadExitFinished;
public:

	wstring m_Print_lpszName;
	wstring m_Print_lpszTypel;
	wstring  m_Print_lpszValue;
	int m_Print_nIndex;
	int m_nCurSearchIndex;
	int	m_nSearchMax;
	int m_nSearchMin;

	wstring					m_wstrFilePath;
	HANDLE					m_hThreadPointAndMode;
	HANDLE					m_EventStopThread;
public:
	LRESULT StartThreads( WPARAM wParam,LPARAM lParam );
	static DWORD WINAPI ThreadFuncPoint(LPVOID lpParameter);
	void  ExitThreads();

	std::vector<wstring> m_strWatchPointNameList;
	std::vector<wstring> m_strWatchPointLineList;
	std::vector<wstring> m_strPointValueList;

	bool IsPushback(std::vector<wstring> &PointList,wstring PointName);
	void	RemoveSubStr(const string& r, string& str);
public:
	CListCtrl m_ListWatch;
	CListCtrl m_ListAll;
	afx_msg void OnNMDblclkListAll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddwatch();
	afx_msg void OnNMDblclkListWatch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddpoint();

	CStatic m_StaticPointCount;
	CRITICAL_SECTION m_CriticalSection_PrintPointInfo;
public:
	BOOL InsertWatchPointToDB(CString strPointName);
	BOOL ReadWatchPointFromDB();
	BOOL DeleteWatchPointFromDB(CString strPointName);

	afx_msg void OnBnClickedButtonaddwatchline();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnLvnItemchangedListAll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSearch();
	CString m_strNameSearch;



	afx_msg void OnBnClickedButtonSearchClear();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();

public:
	CUPPCServerDataAccess*	m_pDataAccess;
	//CDataPointManager*		m_pointmanager;
	CPageDlg *m_pGatewayDlg;
	bool			m_bRefreshOK;
	afx_msg void OnBnClickedButtonExportRealtime();
	afx_msg void OnBnClickedButtonImportrealtime();
	afx_msg void OnNMClickListAll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListAll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedStaticPointcount();
	afx_msg void OnBnClickedButtonMultiModify();
	afx_msg void OnBnClickedButtonHistory();

	COleDateTime m_HisQueryDateFrom;
	COleDateTime m_HisQueryDateTo;
	COleDateTime m_HisQueryTimeFrom;
	COleDateTime m_HisQueryTimeTo;
	int m_HisQueryTimePeriod;
};
