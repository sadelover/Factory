#pragma once

#include "afxcmn.h"
#include <vector>
#include "TreeCtrlEx.h"
#include <map>

// CTreeManageDlg 对话框

class CMainFrame;
class CEqmDrawPage;
class CLogicDllThread;
class CUPPCServerDataAccess;

struct TREE_INFO
{
	int nPageID;
	HTREEITEM hItem;
};

typedef enum tag_root_type
{
	root_page	= 0,
	root_tactics,
	root_calc
	//root_img_lib,
	//root_tactics_lib
}ROOT_TYPE;

typedef struct tag_tree_group
{
	int			nGroupId;
	int			nGroupShowOrder;
	vector<int>	vecPage;

	tag_tree_group()
	{
		nGroupId		= 0;
		nGroupShowOrder	= 0;
		vecPage.clear();
	}
}TREE_GROUP;


class CTreeManageDlg : public CDialog
{
	DECLARE_DYNAMIC(CTreeManageDlg)

public:
	CTreeManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTreeManageDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 对话框数据
	enum { IDD = IDD_DIALOG_TREE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonAddGroup();
	afx_msg void OnBnClickedButtonDelGroup();
	afx_msg void OnBnClickedButtonAddPage();
	afx_msg void OnBnClickedButtonDelPage();
	void			InsertRootNode(CString strName);
	HTREEITEM		InsertGroup(CString strName, const int nGroupID);
	HTREEITEM		InsertPage(CString strName, HTREEITEM hGroup, const int nPageID);
	void			Expand();
	void			SelectFirstPageItem(const int nPageID);
	void			DeleteAllItems();
	void			UpdateItemPage(const int nPageID);
	CMainFrame*		m_pMainFrame;
	bool			IsTreeItemPage(HTREEITEM hItem);
	bool			IsTreeItemGroup(HTREEITEM hItem);

	CTreeCtrlEx	m_treectrl;

	static UINT WINAPI ThreadFunc_LoadPage(LPVOID lparam);

	bool	ShowPageByPageID(const int nPageID,const int nElementID,const int nImageID);

private:

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

// added to change tree struct
public:
	void		InitTreeNodes(void);
	void		SetConnectServFlag(const bool bIsConnect);
	HTREEITEM	GetRootTactics(void);
	//bool		InitTacitcsLib(void);
	void		GetTreeInfo(vector<TREE_GROUP>& vecTreeInf);
	void		SetDeviceType(const int nDeviceType);

private:
	bool		IsItemRootPage(const HTREEITEM hItem) const;
	bool		IsItemGroup(const HTREEITEM hItem) const;
	bool		IsItemPage(const HTREEITEM hItem) const;
	bool		IsItemRootTactics(const HTREEITEM hItem) const;
	bool		IsItemThread(const HTREEITEM hItem) const;
	bool		IsItemRootNode(const HTREEITEM hItem) const;
	//bool		IsItemRootTacticsLib(const HTREEITEM hItem) const;

	bool		InitTacticsNode(void);
	HTREEITEM	InsertNodeThread(const CString strName, const ROOT_TYPE emRootType);
	void		DeleteTreeNode(const HTREEITEM hNode);
	void		GetTreeNodeNumInfo(const HTREEITEM hRootNode, const HTREEITEM hCurNode, int& nAllNum, int& nCurSelNum);
	CString		GetCurSelTreeNodeName(const HTREEITEM hNode);
	void		TravelTreeInfo(const HTREEITEM hItem);
	//bool		InitImgLib(void);

	bool		IsThreadNameExist(const CString strThreadName);

	HTREEITEM	m_hRootPage;
	HTREEITEM	m_hRootTact;
	//HTREEITEM	m_hRootImgLib;
	//HTREEITEM	m_hRootTactLib;
	CUPPCServerDataAccess* m_pDataAccess;
	HTREEITEM	m_hCurSelItem;
	bool		m_bIsConnectServ;
	bool		m_bExportPageLoad;
	int			m_nDeviceType;

public:
	afx_msg void OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonMoveDown();
	afx_msg void OnNMRClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreePageProperty();
	afx_msg void OnImportPage();
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandingTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuAddGroup();
	afx_msg void OnMenuDelGroup();
	afx_msg void OnMenuAddPage();
	afx_msg void OnMenuDelPage();
	afx_msg void OnMenuAddThread();
	afx_msg void OnMenuDelThread();
	afx_msg void OnMenuModifyThread();
	afx_msg void OnMenuRunThread();
	afx_msg void OnMenuStopThread();
	afx_msg void OnMenuConnDebug();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUpdateMenuRunThread(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuStopThread(CCmdUI *pCmdUI);
	virtual void OnOK();
	virtual void OnCancel();
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
	afx_msg void OnBackupLogicthreadStatus();
	afx_msg void OnRestoreLogicthreadStatus();
};
