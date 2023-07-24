
#pragma once

#include "../ComponentDraw/EqmDrawDefines.h"
#include "ViewTree.h"
#include "TreeManageDlg.h"
#include "./json/value.h"

class CMainFrame;
class CEqmDrawPage;
class CLogicDllThread;
class CUPPCServerDataAccess;
class CProjectSqlite;
class CDLLObject;

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// Construction
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	void ShowPageWnd(BOOL bShow);

	void            InitNode();
	void			InitTreeNodes(void);
	void			InsertRootNode(CString strName);
	HTREEITEM		InsertGroup(CString strName, const int nGroupID);
	HTREEITEM		InsertPage(CString strName, HTREEITEM hGroup, const int nPageID);
	void			Expand();
	void			SelectFirstPageItem(const int nPageID);
	void			DeleteAllItems();
	void			UpdateItemPage(const int nPageID);
	CMainFrame*		m_pMainFrame;
	CViewTree		m_wndFileView;
	bool			IsTreeItemPage(HTREEITEM hItem);
	bool			IsTreeItemGroup(HTREEITEM hItem);

	void		SetConnectServFlag(const bool bIsConnect);
	HTREEITEM	GetRootTactics(void);
	void		GetTreeInfo(vector<TREE_GROUP>& vecTreeInf);
	void		SetDeviceType(const int nDeviceType);
	bool		IsItemRootPage(const HTREEITEM hItem) const;
	bool		IsItemGroup(const HTREEITEM hItem) const;
	bool		IsItemPage(const HTREEITEM hItem) const;
	bool		IsItemRootTactics(const HTREEITEM hItem) const;
	bool		IsItemThread(const HTREEITEM hItem) const;
	bool		IsItemRootNode(const HTREEITEM hItem) const;
	bool		InitTacticsNode(void);
	bool		InitCalcNode(void);
	bool        ClearTreeNode(void);
	bool		IsItemRootCalc(const HTREEITEM hItem) const;
	bool		IsItemCalcItem(const HTREEITEM hItem) const;
	HTREEITEM	InsertNodeThread(const CString strName, const ROOT_TYPE emRootType, const int nStatus = -1);
	void		DeleteTreeNode(const HTREEITEM hNode);
	void		GetTreeNodeNumInfo(const HTREEITEM hRootNode, const HTREEITEM hCurNode, int& nAllNum, int& nCurSelNum);
	CString		GetCurSelTreeNodeName(const HTREEITEM hNode);
	void		TravelTreeInfo(const HTREEITEM hItem);
	bool		IsThreadNameExist(const CString strThreadName);
	static UINT WINAPI ThreadFunc_LoadPage(LPVOID lparam);
	bool		ShowPageByPageID(const int nPageID,const int nElementID,const int nImageID);

	//20180724////////////////////////////////////////////////////////////////////////
	bool		initPageDlgDBAccess();			//初始化页面对话框的数据库连接等信息
	

// Attributes
protected:

	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

protected:
	void FillFileView();

// Implementation
public:
	virtual ~CFileView();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);


	afx_msg void OnBnClickedButtonAddGroup();
	afx_msg void OnBnClickedButtonDelGroup();
	afx_msg void OnBnClickedButtonAddPage();
	afx_msg void OnBnClickedButtonDelPage();

	afx_msg void OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonMoveDown();
	afx_msg void OnNMRClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreePageProperty();
	afx_msg void OnImportPage();
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandingTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuAddGroup();
	afx_msg void OnMenuDelGroup();
	afx_msg void OnMenuEditGroup();
	afx_msg void OnMenuAddPage();
	afx_msg void OnMenuDelPage();
	afx_msg void OnMenuAddThread();
	afx_msg void OnMenuDelThread();
	afx_msg void OnMenuModifyThread();
	afx_msg void OnMenuRunThread();
	afx_msg void OnMenuStopThread();
	afx_msg void OnMenuConnDebug();
	afx_msg void OnUpdateMenuRunThread(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuStopThread(CCmdUI *pCmdUI);
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuGroupUp();
	afx_msg void OnMenuGroupDown();
	afx_msg void OnMenuPageUp();
	afx_msg void OnMenuPageDown();
	afx_msg void OnMenuPageProperty();
	afx_msg void OnMenuUpdateLib();
	afx_msg void OnMenuPageImport();
	afx_msg void OnMenuTemplatePageImport();
	afx_msg void OnMenuRunAllThread();
	afx_msg void OnMenuStopAllThread();
	afx_msg void OnUpdateMenuRunAllThread(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuStopAllThread(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuUpdateLib(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuAddThread(CCmdUI *pCmdUI);
	afx_msg void OnMenuModifyThreadPerio();
	afx_msg void OnUpdateMenuModifyThreadPerio(CCmdUI *pCmdUI);
	afx_msg void OnMenuPageAddtoTemplate();
	afx_msg void OnUpdateMenuGroupDel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuPageDel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuPageProperty(CCmdUI *pCmdUI);
	afx_msg void OnMenuCalcAdd();
	afx_msg void OnMenuCalcDel();
	afx_msg void OnMenuCalcEdit();
	afx_msg void OnMenuCalcRun();
	afx_msg void OnMenuCalcStop();
	afx_msg void OnMenuTacticsExport();
	afx_msg void OnMenuTacticsImport();
	afx_msg void OnUpdateMenuTacticsExport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuTacticsImport(CCmdUI *pCmdUI);
	afx_msg void OnMenuCalcExport();
	afx_msg void OnMenuCalcImport();
	afx_msg void OnUpdateMenuCalcxport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuCalcmport(CCmdUI *pCmdUI);

	afx_msg void OnMenuControlledPointSearch();
	afx_msg void OnUpdateMenuControlledPointSearch(CCmdUI *pCmdUI);

	afx_msg void OnMenuSearchStrategyInTimeAgeRun();
	afx_msg void OnUpdateMenuSearchStrategyInTimeAgeRun(CCmdUI *pCmdUI);

	afx_msg void OnMenuSearchStrategy();
	afx_msg void OnUpdateMenuSearchStrategy(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	void	SetGroupName();
	int		GetThreadPeriodicity(const CString& strName);
	void	SetTreeThreadItemImage(const HTREEITEM hItem, const int nValue);
	void	SetTreeCalcItemImage(const HTREEITEM hItem, const int nValue);
	void	SetTreeCalcItemText(const HTREEITEM hItem, const CalcModuleInfo& item);

	void	ConvertJsonToThreadInfo(const Json::Value& jsRoot);
	size_t	JsonGetThreadSize(const Json::Value& jsRoot);
	wstring	JsonGetThreadName(const Json::Value& jsRoot, int nCnt);
	int		JsonGetThreadCycle(const Json::Value& jsRoot, int nCnt);
	CString	JsonGetDllName(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt);
	CString	JsonGetDllNameOrig(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt);
	CString	JsonGetDllTime(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt);
	CString	JsonGetDllAuthor(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt);
	CString	JsonGetDllVersion(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt);
	CString	JsonGetDllDesc(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt);
	wstring JsonGetDllInputListValue(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt, int nRow, int nCol);
	wstring JsonGetDllOutputListValue(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt, int nRow, int nCol);

	CDLLObject*	GetDllObjectItem(const vector<CLogicDllThread*>& vecTactic, const wstring wstrThreadName, const CString strDllName);
	wstring GetDllInputListValue(const vector<CLogicDllThread*>& vecTactic, const wstring wstrThreadName, const CString strDllName, int nListCount, int nCol);
	wstring GetDllOutputListValue(const vector<CLogicDllThread*>& vecTactic, const wstring wstrThreadName, const CString strDllName, int nListCount, int nCol);
	HTREEITEM m_hRoot;
	HTREEITEM m_hRootGraphicLib;
	HTREEITEM m_hRootLogic;
	HTREEITEM m_hRootDebugTool;
	vector<TREE_INFO> m_vecTree;
	CImageList m_ImageList;
	CMenu			m_menu;
	CMenu			m_menu_group;
	CEqmDrawPage*   m_pSelectedPage;
	HTREEITEM		m_hSelectedItem;
	vector<int>::const_iterator		m_iterPageId;
	vector<TREE_GROUP>				m_vecTreeInfo;
	vector<TREE_GROUP>::iterator	m_iterTreeInfo;
	int				m_nGroupShowOrder;
	HTREEITEM	m_hRootPage;
	HTREEITEM	m_hRootTact;//线程

	HTREEITEM	m_hRootTact01;
	HTREEITEM	m_hRootTact02;
	HTREEITEM	m_hRootTact03;
	HTREEITEM	m_hRootTact04;
	HTREEITEM	m_hRootTact05;
	HTREEITEM	m_hRootCalc;
	HTREEITEM	m_hRootConfig;

	HTREEITEM	m_hRootComm;			//通讯菜单
	HTREEITEM	m_hRootCommSet;			//通讯设置菜单
	HTREEITEM	m_hRootCommDebug;		//通讯调试菜单
	HTREEITEM	m_hRootValue;			//点值监控
	HTREEITEM	m_hRootChart;			//实时曲线


	HTREEITEM	m_hRootProjectGlobalConfig;
	HTREEITEM	m_hRootModbusClientConfig;
	HTREEITEM	m_hRootSystemDefineConfig;
	HTREEITEM	m_hRootCloudSettingConfig;
	HTREEITEM   m_hRootServiceManageConfig;//进程管理,如modbusserviceclientcore
	HTREEITEM   m_hRootReportManageConfig;
	//HTREEITEM	m_hRootImgLib;
	//HTREEITEM	m_hRootTactLib;
	CUPPCServerDataAccess* m_pDataAccess;
	HTREEITEM	m_hCurSelItem;
	//策略双击选择时字体背景改成红色
	HTREEITEM	m_hLastDBCurSelItem;
	bool		m_bIsConnectServ;
	int			m_nDeviceType;
	CProjectSqlite*	m_pPrjSql;
	CString		m_strLogicPath;

	CImageList m_TreeIconImageList;
	void RemoveSubStr( const string& r, string& str );
	CWnd * m_SearchStrategyDlg;
	CWnd * m_FindControlledPointSearchInThreadDlg;
public:
	afx_msg void OnVpointBackup();
	afx_msg void OnVpointReload();
	afx_msg void OnUpdateVpointBackup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVpointReload(CCmdUI *pCmdUI);
	afx_msg void OnBackupLogicthreadStatus();
	afx_msg void OnRestoreLogicthreadStatus();
	afx_msg void OnRestartDomcore();
	/*
	 * @brief:查找与字符串匹配的Item
	 * @param[in]:item,起始项
	*/
	HTREEITEM FinItem_m_wndFileView(HTREEITEM  item, CString strtext);
	HTREEITEM GetTactHTI();
	CUPPCServerDataAccess * GetDataAccess();
	/*
	 * @brief:链接调试
	 * @details:为线程中的双击事件单独创建，因：原连接调试事件会导致界面刷新
	 *	导致打开的界面是空的。函数内还有一个特殊操作是为了策略设置参数界面读
	 *	读到值写的，是从initPageDlgDBAccess函数中摘出来的，不实用那段代码是
	 *	只能读到脚本值无法读点值。
	*/
	void ControlDebug(void);
};

