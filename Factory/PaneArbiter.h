#pragma once


#include "afxcmn.h"
#include <vector>
#include "../Tools/CustomTools/CustomTools.h"
#include "../DB_BasicIO/DatabaseSession.h"
#include "dllobject.h"
#include "DataPoint/DataPointManager.h"
#include "DataPoint/LogicPointManager.h"
#include "DataPoint/AlarmPointManager.h"
#include "afxwin.h"


const int AllLogStoreSize = 5000;
// CPaneArbiter 对话框
#define WM_MYMSG_INIT_FINISH (WM_USER +0x200)
class CUPPCServerDataAccess;
class CBEOPDataLinkManager;
class CBEOPLogicManager;
class CLogicDllThread;

typedef struct tag_sys_tactics
{
	CString strFileTitle;
	CString strPathName;
}SYS_TACTICS;


class CPaneArbiter : public CDialogEx
{

	DECLARE_DYNAMIC(CPaneArbiter)
public:
	CPaneArbiter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPaneArbiter();
	enum { IDD = IDD_DIALOG_ARBITER };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void  Exit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedButtonConfig();

	bool GetWatchPointUpdate();
	void SetWatchPointUpdate(bool bSet);

	bool DisplayDllName(std::vector<CString> &vecSelDllName);

	void InitData(vector<pair<CString,int>>& vecName);
	void InitListTitle(void);
	void GetInitThreadName(vector<CString>& vecName);

	void InitLogicMgrPointer(void);
	void SetThreadNum(const int nThreadAllNum, const int nThreadCurSelNum);
	void SetThreadCurSelName(const CString strCurSelName);
private:
	int		m_nThreadNum;
	int		m_nThreadCurSelNum;
	CString	m_strCurSelName;
	bool	m_bIsLstTitleInit;

public:
	afx_msg void OnBnClickedButtonLoadcal();
	afx_msg void OnBnClickedButtongatewayUnloadcal();
	afx_msg void OnBnClickedButtonImportconfig();

	void RefreshDLLsOfThread();
	CLogicDllThread * FindDLLThreadByName(wstring strDllThreadName);

	afx_msg void OnNMDblclkListControllogic(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListControllogic(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void	InitListCtrlHead(void);

public:

	bool m_bWatchPointUpdate;

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CDataPointManager*   m_pointmanager;
	CBEOPLogicManager *  m_pLogicManager;
	CUPPCServerDataAccess*	m_pDataAccess;

private:
	int		m_iListCount;

public:
	CString m_curLog;

	CListCtrl m_ControlLogicList;
	std::vector<CString> m_wholeLog;
	CString              m_strWholeLog;
	COleDateTime m_tDateFrom;
	COleDateTime m_tDateTo;
	COleDateTime m_tTimeFrom;
	COleDateTime m_tTimeTo;

	COleDateTime m_tStart;
	COleDateTime m_tEnd;

	int m_nSeletectedPeriod;

	afx_msg void OnBnClickedButtonCalRunserver();
	afx_msg void OnBnClickedButtonCalPauseserver();
	afx_msg void OnBnClickedButtonRefreshLogicdll();
	bool ReplaceDllByOriginName(CString strDllFilePath);
	CListBox m_lstLogicThread;
	
	CString m_strLogicRelation;
	afx_msg void OnBnClickedButtonAddFlow();
	afx_msg void OnBnClickedButtonDelFlow();
	afx_msg void OnBnClickedButtonRenameFlow();
	afx_msg void OnBnClickedButtonModifyRelation();

public:
	bool IsDllInThread(CLogicDllThread *pDllThread,wstring wsDllName);
	bool IsVNameInDll(CLogicDllThread *pDllThread,wstring wsVName,wstring wsDllName,wstring wsInOut);
	bool IsThreadNameExist(CString strThreadName);
	afx_msg void OnBnClickedBtnLoadSysCal();
	afx_msg void OnBnClickedBtnChangeName();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnQueryLog();
	afx_msg void OnBnClickedBtnHistorytest();
	afx_msg void OnBnClickedBtnCopyLogic();
	void SaveParameterToDBAndServer(CString strDllName, CString strSetType, CString strVariableName, CString strLinkDefine, CString strLinkType, CString strCondition);
	afx_msg void OnEnChangeEditRelationship();
};
