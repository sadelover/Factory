#pragma once
#include "afxwin.h"
#include "BEOP-DesignerView.h"
#include "BEOPLogicManager.h"
#include "LogicDllThread.h"
#include "LogicParameter.h"
#include "afxcmn.h"
#include "MainFrm.h"

// CSearchStrategyDlg 对话框

class CSearchStrategyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchStrategyDlg)

public:
	CSearchStrategyDlg(CBEOPLogicManager * pLogicManager, vector<CLogicDllThread *> * pLogicList, CFileView* pFileView, HTREEITEM pTreeRoot, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearchStrategyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SEARCH_STRATEGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnNMDblclkListStrategy(NMHDR *pNMHDR, LRESULT *pResult);
	
	void SearchStartegy();
private:
	HTREEITEM FindHTREEITEM(HTREEITEM  item, CString strtext);
	static UINT Thread_ButtonSearch(LPVOID pParam);
	// 被搜索策略名，可带空格
	CString m_strStrategyNameList;
	// 策略列表
	vector<CLogicDllThread *> * m_pLogicList;
	//	策略管理
	CBEOPLogicManager *  m_pLogicManager;
	//  FileView 左侧的浮动窗
	CFileView* m_pFileView;
	// FileView 树状结构的根指针
	HTREEITEM m_pTreeRoot;
	//列表控件用于显示搜索到的控件
	CListCtrl m_list_strategy;
	//策略包
	struct  StrategyStruct
	{
		CString strThreadName;
		CString strStrategyName;
		StrategyStruct(CString threadName, CString strategyName):strThreadName(threadName),strStrategyName(strategyName){}
	};
	vector<StrategyStruct> m_foundStrategyList;

	CProgressCtrl m_ProgressCtrl;
};
