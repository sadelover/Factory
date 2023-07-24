#pragma once
#include "afxwin.h"
#include "BEOP-DesignerView.h"
#include "BEOPLogicManager.h"
#include "LogicDllThread.h"
#include "LogicParameter.h"
#include "afxcmn.h"
#include "MainFrm.h"

// CControlledPointSearchDlg 对话框
class CMainFrame;
class CLogicDllThread;
class CBEOPLogicManager;
class CDLLObject;
class CLogicParameter;

#define Progress_MAX 1000

class CFindControlledPointSearchInThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFindControlledPointSearchInThreadDlg)

public:
	CFindControlledPointSearchInThreadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindControlledPointSearchInThreadDlg();
	void Set_m_logicList(vector<CLogicDllThread *>& logicList);
	void Set_m_pLogicManager(CBEOPLogicManager * pLogicManager);
	void GetCulThreadStrategyName(CString &cstrThreadName,CString &cstrStrategyName);
	void SetProgressPos(int nPos);
	void SearchPoint();
	void m_cstrFindPointThreadList_Add(CString cstrThreadName);
	void SetCFileView(CFileView* pFileView);
// 对话框数据
	enum { IDD = IDD_CONTROLLED_POINT_SERACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkListThread();
	//搜索按钮点击事件
	afx_msg void OnBnClickedButtonSearch();
private:
	//暂时弃用
	//afx_msg void OnBnClickedOk();
	static UINT SearchPointThread(LPVOID pParam);
	//搜索使用的点名
	CString m_cstrSearchPoint;
	// 列表控件
	CListBox m_listBoxThread;
	// 搜索进度条
	CProgressCtrl m_ProgressSearch;

	vector<CLogicDllThread *> m_logicList;
	//
	CBEOPLogicManager *  m_pLogicManager;
	CFileView* m_pCFileView;
};
