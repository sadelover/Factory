
#include "stdafx.h"
#include "FileView.h"

#include "BEOP-designer.h"
#include "GroupDlg.h"
#include "MainFrm.h"
#include "../ComponentDraw/EqmDrawProject.h"
#include "../ComponentDraw/CEqmDrawPage.h"
#include "../ComponentDraw/EqmDrawDevice.h"
#include "PageDlg.h"
#include "AddPageDlg.h"
#include "BEOP-DesignerView.h"
#include "CanvasDlg.h"
#include "SetUpDataDlg.h"
#include "ImportPageSelectDlg.h"
#include "BEOPGatewayDlg.h"
#include "AddDllThreadNameDlg.h"
#include "LogicDllThread.h"
#include "BEOPLogicManager.h"
#include "ConnectDlg.h"
#include "ThreadPeriodicitySet.h"
#include "TemplateDlg.h"
#include "ImportTemplatePageDlg.h"
#include "TreeGroupSettingDlg.h"
#include "CalcPointProperty.h"
#include "LogicParameter.h"
#include "./json/writer.h"
#include "./json/reader.h"
#include "ModbusComConfigDlg.h"
#include "SystemDefineDlg.h"
#include "CloudSettingDlg.h"
#include "CoreServiceManageDlg.h"
#include "ReportManageDlg.h"
#include "ReloadLogin.h"
#include "../ControlEx/WarningDlg.h"
#include "DlgSetting.h"
#include <algorithm>
#include "FindControlledPointSearchInThreadDlg.h"
#include "HttpOperation.h"
#include "SearchStrategyInTimeAgeRunDlg.h"
#include "SearchStrategyDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef unsigned long           u32;
/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
	m_hRoot = NULL;
	m_pMainFrame=NULL;
	m_pSelectedPage=NULL;
	m_hSelectedItem=NULL;
	m_hRootPage=NULL;
	m_hRootTact=NULL;
	m_hRootCalc=NULL;
	m_pDataAccess=NULL;
	m_hCurSelItem=NULL;
	m_bIsConnectServ=false;
	m_nGroupShowOrder=0;
	m_nDeviceType=0;
	m_pPrjSql=NULL;

	m_hRootConfig = NULL;
	m_hRootModbusClientConfig = NULL;

	m_hRootSystemDefineConfig = NULL;
	m_hRootCloudSettingConfig = NULL;

	m_hRootServiceManageConfig = NULL;

	m_hRootReportManageConfig =NULL;

	m_hRootComm=NULL;
	m_hRootValue=NULL;
	m_hRootChart=NULL;
	m_SearchStrategyDlg = NULL;
	m_FindControlledPointSearchInThreadDlg = NULL;
	m_hLastDBCurSelItem = NULL;
}

CFileView::~CFileView()
{
	if(m_SearchStrategyDlg == NULL)
		delete m_SearchStrategyDlg;
	if(m_FindControlledPointSearchInThreadDlg == NULL)
		delete m_FindControlledPointSearchInThreadDlg;
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(IDC_BUTTON_ADD_GROUP, OnBnClickedButtonAddGroup)
	ON_COMMAND(IDC_BUTTON_DEL_GROUP, OnBnClickedButtonDelGroup)
	ON_COMMAND(IDC_BUTTON_ADD_PAGE, OnBnClickedButtonAddPage)
	ON_COMMAND(IDC_BUTTON_DEL_PAGE, OnBnClickedButtonDelPage)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnNMDblclkTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, OnNMRClickTree)
	ON_NOTIFY(NM_CLICK, IDC_TREE, OnNMClickTree)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE, OnNMCustomdrawTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE, OnTvnItemexpandingTree)
	ON_COMMAND(IDC_BUTTON_MOVE_UP, OnBnClickedButtonMoveUp)
	ON_COMMAND(IDC_BUTTON_MOVE_DOWN, OnBnClickedButtonMoveDown)
	ON_COMMAND(ID_TREE_PAGE_PROPERTY, OnTreePageProperty)
	ON_COMMAND(ID_IMPORT_PAGE, OnImportPage)
	ON_COMMAND(ID_MENU_GROUP_ADD, OnMenuAddGroup)
	ON_COMMAND(ID_MENU_GROUP_DEL, OnMenuDelGroup)
	ON_COMMAND(ID_MENU_GROUP_EDIT, OnMenuEditGroup)
	ON_COMMAND(ID_MENU_PAGE_ADD, OnMenuAddPage)
	ON_COMMAND(ID_MENU_PAGE_DEL, OnMenuDelPage)
	ON_COMMAND(ID_MENU_ADD_THREAD, OnMenuAddThread)
	ON_COMMAND(ID_MENU_DEL_THREAD, OnMenuDelThread)
	ON_COMMAND(ID_MENU_MODIFY_THREAD, OnMenuModifyThread)
	ON_COMMAND(ID_MENU_RUN_THREAD, OnMenuRunThread)
	ON_COMMAND(ID_MENU_STOP_THREAD, OnMenuStopThread)
	ON_COMMAND(ID_MENU_CONN_DEBUG, OnMenuConnDebug)
	ON_UPDATE_COMMAND_UI(ID_MENU_RUN_THREAD, OnUpdateMenuRunThread)
	ON_UPDATE_COMMAND_UI(ID_MENU_STOP_THREAD, OnUpdateMenuStopThread)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnTvnSelchangedTree)
	ON_COMMAND(ID_MENU_GROUP_UP, OnMenuGroupUp)
	ON_COMMAND(ID_MENU_GROUP_DOWN, OnMenuGroupDown)
	ON_COMMAND(ID_MENU_PAGE_UP, OnMenuPageUp)
	ON_COMMAND(ID_MENU_PAGE_DOWN, OnMenuPageDown)
	ON_COMMAND(ID_MENU_PAGE_PROPERTY, OnMenuPageProperty)
	ON_COMMAND(ID_MENU_UPDATE_LIB, OnMenuUpdateLib)
	ON_COMMAND(ID_MENU_PAGE_IMPORT, OnMenuPageImport)
	ON_COMMAND(ID_MENU_TEMPLATE_PAGE_IMPORT, OnMenuTemplatePageImport)
	ON_COMMAND(ID_MENU_RUN_ALL_THREAD, OnMenuRunAllThread)
	ON_COMMAND(ID_MENU_STOP_ALL_THREAD, OnMenuStopAllThread)
	ON_UPDATE_COMMAND_UI(ID_MENU_RUN_ALL_THREAD, OnUpdateMenuRunAllThread)
	ON_UPDATE_COMMAND_UI(ID_MENU_STOP_ALL_THREAD, OnUpdateMenuStopAllThread)
	ON_UPDATE_COMMAND_UI(ID_MENU_UPDATE_LIB, OnUpdateMenuUpdateLib)
	ON_UPDATE_COMMAND_UI(ID_MENU_ADD_THREAD, OnUpdateMenuAddThread)
	ON_COMMAND(ID_MENU_MODIFY_THREAD_PERIO, OnMenuModifyThreadPerio)
	ON_UPDATE_COMMAND_UI(ID_MENU_MODIFY_THREAD_PERIO, OnUpdateMenuModifyThreadPerio)
	ON_COMMAND(ID_MENU_PAGE_ADDTO_TEMPLATE, OnMenuPageAddtoTemplate)
	ON_UPDATE_COMMAND_UI(ID_MENU_GROUP_DEL, OnUpdateMenuGroupDel)
	ON_UPDATE_COMMAND_UI(ID_MENU_PAGE_DEL, OnUpdateMenuPageDel)
	ON_UPDATE_COMMAND_UI(ID_MENU_PAGE_PROPERTY, OnUpdateMenuPageProperty)
	ON_COMMAND(ID_MENU_CALC_ADD, &CFileView::OnMenuCalcAdd)
	ON_COMMAND(ID_MENU_CALC_DEL, &CFileView::OnMenuCalcDel)
	ON_COMMAND(ID_MENU_CALC_EDIT, &CFileView::OnMenuCalcEdit)
	ON_COMMAND(ID_MENU_CALC_RUN, &CFileView::OnMenuCalcRun)
	ON_COMMAND(ID_MENU_CALC_STOP, &CFileView::OnMenuCalcStop)
	ON_COMMAND(ID_MENU_TACTICS_EXPORT, OnMenuTacticsExport)
	ON_COMMAND(ID_MENU_TACTICS_IMPORT, OnMenuTacticsImport)
	ON_UPDATE_COMMAND_UI(ID_MENU_TACTICS_EXPORT, OnUpdateMenuTacticsExport)
	ON_UPDATE_COMMAND_UI(ID_MENU_TACTICS_IMPORT, OnUpdateMenuTacticsImport)
	ON_COMMAND(ID_MENU_CALC_EXPORT, OnMenuCalcExport)
	ON_COMMAND(ID_MENU_CALC_IMPORT, OnMenuCalcImport)
	ON_UPDATE_COMMAND_UI(ID_MENU_CALC_EXPORT, OnUpdateMenuCalcxport)
	ON_UPDATE_COMMAND_UI(ID_MENU_CALC_IMPORT, OnUpdateMenuCalcmport)
	ON_COMMAND(IDM_BACKUP_LOGICTHREAD_STATUS, &CFileView::OnBackupLogicthreadStatus)
	ON_COMMAND(IDM_RESTORE_LOGICTHREAD_STATUS, &CFileView::OnRestoreLogicthreadStatus)
	ON_COMMAND(IDM_RESTART_DOMCORE, &CFileView::OnRestartDomcore)

	ON_COMMAND(ID_MENU_CONTROLLED_POINT_SEARCH, OnMenuControlledPointSearch)
	ON_UPDATE_COMMAND_UI(ID_MENU_CONTROLLED_POINT_SEARCH, OnUpdateMenuControlledPointSearch)

	ON_COMMAND(ID_MENU_SEARCH_STRATEGY_IN_TIME_AGE_RUN, OnMenuSearchStrategyInTimeAgeRun)
	ON_UPDATE_COMMAND_UI(ID_MENU_SEARCH_STRATEGY_IN_TIME_AGE_RUN, OnUpdateMenuSearchStrategyInTimeAgeRun)

	ON_COMMAND(ID_MENU_SEARCH_STRATEGY, OnMenuSearchStrategy)
	ON_UPDATE_COMMAND_UI(ID_MENU_SEARCH_STRATEGY, OnUpdateMenuSearchStrategy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, IDC_TREE))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	//HICON hIcon[2];
	//hIcon[0] = theApp.LoadIcon(IDI_ICON_TREE_COM1);
	//hIcon[1] = theApp.LoadIcon(IDI_ICON_TREE_COM2);
	//hIcon[2] = theApp.LoadIcon(IDI_ICON_TREE_COM3);
	//
	//m_TreeIconImageList.Create(16, 16, ILC_COLOR32, 3, 3);
	//for(int i=0; i<3; i++)
	//	m_TreeIconImageList.Add(hIcon[i]);


	m_FileViewImages.Create(IDB_BITMAP_TREE, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
	//m_wndFileView.SetImageList(&m_TreeIconImageList, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillFileView();
	AdjustLayout();

	if (NULL == m_pMainFrame)
	{
		m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	}

	Project::Tools::GetSysPath(m_strLogicPath);
	m_strLogicPath += _T("\\syslogic\\");

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	m_menu.LoadMenu(IDR_MENU_TREE);
	m_menu_group.LoadMenu(IDR_MENU_GROUP_IMPORT);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);

			CMenu* pSubMenu = NULL;
			if (IsItemRootPage(hTreeItem))
			{	// root page
				pSubMenu = m_menu.GetSubMenu(1);
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			}
			else if (IsItemGroup(hTreeItem))
			{	// group
				m_hSelectedItem = hTreeItem;

				pSubMenu = m_menu.GetSubMenu(2);
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			}
			else if (IsItemPage(hTreeItem))
			{	// page
				if(m_pMainFrame->m_bLoadingPage)
				{
					return;
				}

				m_wndFileView.Expand(hTreeItem,TVE_EXPAND);
				const int nPageID = (int)m_wndFileView.GetItemData(hTreeItem);
				CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
				if (pPage != NULL)
				{
					m_pSelectedPage = pPage;
					m_hSelectedItem = hTreeItem;
				}

				pSubMenu = m_menu.GetSubMenu(3);
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			}
			else if (IsItemRootTactics(hTreeItem))
			{	// tactics
				pSubMenu = m_menu.GetSubMenu(4);
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			}
			else if (IsItemThread(hTreeItem))
			{	// thread
				m_hCurSelItem = hTreeItem;
				pSubMenu = m_menu.GetSubMenu(5);
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			}
			else if (IsItemRootCalc(hTreeItem))
			{	// calc root
				pSubMenu = m_menu.GetSubMenu(7);
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			}
			else if (IsItemCalcItem(hTreeItem))
			{	// calc item
				m_hCurSelItem = hTreeItem;
				pSubMenu = m_menu.GetSubMenu(8);
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			}
		}
	}

	pWndTree->SetFocus();
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = IDB_BITMAP_TREE;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= ILC_COLOR32;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


void CFileView::OnBnClickedButtonAddGroup()
{
	if (m_hRootPage)
	{
		CGroupDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			if (NULL == m_pMainFrame)
			{
				m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
			}
			const int nGroupID = m_pMainFrame->m_project.AddGroup(dlg.m_strGroupName.GetString());
			if (nGroupID != -1)
			{
				HTREEITEM hGroup = InsertGroup(dlg.m_strGroupName, nGroupID);
				m_wndFileView.SetFocus();
				m_wndFileView.SelectItem(hGroup);
				m_wndFileView.Expand(m_hRootPage, TVE_EXPAND);
				m_pMainFrame->SetSaveFlag(true);
			}
		}
	}
	else
	{
		MessageBox(L"请先创建工程!");
	}
}

void CFileView::OnBnClickedButtonDelGroup()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(L"确定要删除组和所包含的页面吗?",L"删除组",MB_OKCANCEL) == IDOK)
	{
		HTREEITEM hItem = m_wndFileView.GetSelectedItem();
		HTREEITEM hItemParent = NULL;
		vector<HTREEITEM> vec;
		BOOL bRes = FALSE;

		hItemParent = m_wndFileView.GetParentItem(hItem);
		if (hItemParent == m_hRootPage)
		{
			int nGroupID =  (int)m_wndFileView.GetItemData(hItem);
			m_pMainFrame->m_pPrjSqlite->DeleteGroup(nGroupID);
			m_pMainFrame->m_project.DeleteGroup(nGroupID);
			HTREEITEM hChild = m_wndFileView.GetChildItem(hItem);
			while (hChild)
			{
				vec.push_back(hChild);
				int nPageID = (int)m_wndFileView.GetItemData(hChild);
				CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
				ASSERT(pPage);
				m_pMainFrame->m_pPrjSqlite->DeletePage(pPage);
				m_pMainFrame->m_project.DeletePage(pPage);
				hChild = m_wndFileView.GetNextItem(hChild, TVGN_NEXT);
			}
			const size_t size = vec.size();
			for (size_t i=0;i<size;++i)
			{
				bRes = m_wndFileView.DeleteItem(vec[i]);
				ASSERT(bRes);
			}
			bRes = m_wndFileView.DeleteItem(hItem);
			m_pMainFrame->m_project.DeleteGroup(nGroupID);
			ASSERT(bRes);

			m_wndFileView.SelectItem(NULL);
			m_pMainFrame->m_DrawContext.pPage = NULL;
			m_pMainFrame->m_DrawContext.ResetContext();

			m_pMainFrame->SetSaveFlag(true);
		}
		else
		{
			MessageBox(L"请选中组节点!");
			return;
		}
	}
}

void CFileView::OnBnClickedButtonAddPage()
{
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	HTREEITEM hItemParent = NULL;

	if (hItem == NULL)
	{
		MessageBox(L"没有选中节点!");
		return;
	}
	else
	{
		if (hItem == m_hRootPage)
		{
			MessageBox(L"选中的是项目节点!请选择组节点!");
			return;
		}
		else if (m_wndFileView.GetParentItem(hItem) == m_hRootPage)
		{
			CAddPageDlg dlg;
			CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
			ASSERT(pView);
			if (dlg.DoModal() == IDOK)
			{
				const int nWidth = dlg.m_nWidth;
				const int nHeight = dlg.m_nHeight;
				const PAGETYPE page_type = dlg.m_page_type;
				HTREEITEM hPage = InsertPage(dlg.m_strNewPageName.GetString(), hItem, (DWORD)(m_pMainFrame->m_MaxPageID));
				m_wndFileView.Expand(hItem, TVE_EXPAND);
				m_wndFileView.SetFocus();
				m_wndFileView.SelectItem(hPage);
				int nPageMaxOrder = m_pMainFrame->m_project.GetMaxPageShowOrder() + 1;
				int x = (m_pMainFrame->m_project.GetScreenWidth()-nWidth)/2;
				int y = (m_pMainFrame->m_project.GetScreenHeight()-nHeight)/2;
				m_pMainFrame->m_DrawContext.pPage = new CEqmDrawPage(m_pMainFrame->m_MaxPageID, (LPCTSTR)dlg.m_strNewPageName, m_pMainFrame->m_project.GetProjectID(), nPageMaxOrder,page_type,nWidth,nHeight,x,y,RGB(158,158,158));
				m_pMainFrame->m_DrawContext.pPage->SetPageWidth(nWidth);
				m_pMainFrame->m_DrawContext.pPage->SetPageHeight(nHeight);
				m_pMainFrame->m_DrawContext.pPage->SetPageType(page_type);
				CString strItemText = m_wndFileView.GetItemText(hItem);
				const int nGroupID = m_pMainFrame->m_project.GetGroupIDByGroupName(strItemText.GetString());
				m_pMainFrame->m_DrawContext.pPage->SetGroupID(nGroupID); 
				m_pMainFrame->m_DrawContext.pPage->SetPageShowTopInObserver(dlg.m_bTop);

				m_pMainFrame->m_DrawContext.pPage->SetBKImageID(dlg.m_nBkImageId);
				Image* pImage = m_pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(dlg.m_nBkImageId, true);
				m_pMainFrame->m_DrawContext.pPage->SetBkImage(pImage);

				m_pMainFrame->m_project.AddPage(m_pMainFrame->m_DrawContext.pPage);
				m_pMainFrame->m_project.SetCurPageID(m_pMainFrame->m_MaxPageID);
				m_pMainFrame->m_MaxPageID++;

				m_pMainFrame->ShowCurrentPage();
				m_pMainFrame->SetSaveFlag(true);

				// show just added page
				if (!((m_pMainFrame->m_DrawContext.pPage)->m_bDetailLoaded))
				{
					m_pMainFrame->m_LoadingProjectBar.ShowWindow(TRUE);
					m_pMainFrame->m_LoadingProjectBar.CenterWindow(m_pMainFrame);
					m_pMainFrame->m_LoadingProjectBar.m_StaticProgress.SetWindowTextW(L"");
					m_pMainFrame->m_LoadingProjectBar.m_ProgressCtrl.SetRange(0,15);
					m_pMainFrame->m_LoadingProjectBar.m_ProgressCtrl.SetPos(0);
					m_pMainFrame->m_pPrjSqlite->SetPage(m_pMainFrame->m_DrawContext.pPage);

					m_pMainFrame->m_bLoadingPage = true;
					UINT dwThreadId = 0;
					HANDLE m_threadhandle_LoadPage = (HANDLE)_beginthreadex(NULL,
						0,
						ThreadFunc_LoadPage, 
						(LPVOID)(m_pMainFrame),
						0,
						&dwThreadId);
				}
				else
				{
					m_pMainFrame->m_DrawContext.pPage->SetStartColor(RGB(158, 158, 158));
					m_pMainFrame->ShowCurrentPage();	
				}

				m_pMainFrame->m_DrawContext.pPage->SetWebAddress(dlg.m_strWebAddr);

				DWORD dwScreenWidth = m_pMainFrame->m_DrawContext.GetScreenWidth();
				DWORD dwScreenHeight = m_pMainFrame->m_DrawContext.GetScreenHeight();

				m_pMainFrame->m_DrawContext.ClearSelect();
				pView->m_PageDlg->m_rtCanvas.right  = static_cast<long>(m_pMainFrame->m_DrawContext.m_fScreenScale*(LONG)dwScreenWidth);
				pView->m_PageDlg->m_rtCanvas.bottom = static_cast<long>(m_pMainFrame->m_DrawContext.m_fScreenScale*(LONG)dwScreenHeight);
				pView->m_PageDlg->SendMessage(WM_SIZE);
				pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_SHOW);
			}
		}
		else
		{
			hItemParent = m_wndFileView.GetParentItem(hItem);
			if (hItemParent != NULL)
			{
				if (m_wndFileView.GetParentItem(hItemParent) == m_hRootPage)
				{
					MessageBox(L"选中的是页面节点!请选择组节点!");
					return;
				} 
			}
		}
	}
}

void CFileView::OnBnClickedButtonDelPage()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(L"确定要删除页面吗?",L"删除页面", MB_OKCANCEL) == IDOK)
	{
		HTREEITEM hItem = m_wndFileView.GetSelectedItem();
		HTREEITEM hParent = NULL;
		if (hItem)
		{
			HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
			if (hParent == m_hRootPage)
			{
				MessageBox(L"请选中页面节点!");
				return;
			}
		}
		else
		{
			MessageBox(L"没选中节点!");
			return;
		}
		if(m_wndFileView.GetChildItem(hItem) == NULL)
		{
			int nPageID = (int)m_wndFileView.GetItemData(hItem);
			vector<TREE_INFO>::iterator iter = m_vecTree.begin();
			while (iter != m_vecTree.end())
			{
				if (iter->nPageID == nPageID)
				{
					iter = m_vecTree.erase(iter);
				}
				else
				{
					++iter;
				}
			}
			CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
			if(pPage)
			{
				m_pMainFrame->m_pPrjSqlite->DeletePage(pPage);
				m_pMainFrame->m_project.DeletePage(pPage);
				m_wndFileView.DeleteItem(hItem);

				m_wndFileView.SelectItem(NULL);
				m_pMainFrame->m_DrawContext.pPage = NULL;
				m_pMainFrame->m_DrawContext.ResetContext();

				CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
				ASSERT(pView != NULL);
				if (pView != NULL)
				{
					pView->m_PageDlg->m_dlgCanvas.Invalidate(TRUE);
				}
			}
		}

		m_pMainFrame->SetSaveFlag(true);
	}
}

void CFileView::InsertRootNode( CString strName )
{
	if (m_hRoot == NULL)
	{
		m_hRoot = m_wndFileView.InsertItem(strName, 1, 1, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRoot);
	}

}

HTREEITEM CFileView::InsertGroup( CString strName, const int nGroupID )
{
	HTREEITEM hGroup = m_wndFileView.InsertItem(strName, 3, 3, m_hRootPage, TVI_LAST);
	ASSERT(hGroup);
	BOOL bRes = m_wndFileView.SetItemData(hGroup, (DWORD)nGroupID);
	ASSERT(bRes);
	return hGroup;
}

HTREEITEM CFileView::InsertPage( CString strName, HTREEITEM hGroup, const int nPageID)
{
	ASSERT(hGroup);
	HTREEITEM hPage = m_wndFileView.InsertItem(strName, 4, 5, hGroup, TVI_LAST);
	ASSERT(hPage);
	TREE_INFO info;
	info.hItem = hPage;
	info.nPageID = nPageID;
	m_vecTree.push_back(info);
	BOOL bRes = m_wndFileView.SetItemData(hPage, (DWORD)nPageID);
	ASSERT(bRes);
	m_wndFileView.Expand(hGroup, TVE_EXPAND);
	return hPage;
}

void CFileView::OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		*pResult = 0;
		return;
	}
	//策略双击选择时字体背景改成红色，这里为清空选项
	if(m_hLastDBCurSelItem != NULL)
	{
		m_wndFileView.SetItemData(m_hLastDBCurSelItem, 0);
		m_hLastDBCurSelItem = NULL;
	}
	

	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	HTREEITEM hItemParent = m_wndFileView.GetParentItem(hItem);
	bool bIsExpand = TVIS_EXPANDED & m_wndFileView.GetItemState(hItem, TVIS_EXPANDED);
	UINT uExpandFlag = TVE_EXPAND;

	if (hItemParent != NULL)
	{	// second level node
		if (m_wndFileView.GetParentItem(hItemParent) == m_hRootPage)
		{	// third level node -- page
			if(m_pMainFrame->m_bLoadingPage)
			{
				return;
			}
			const DWORD nPageID = m_wndFileView.GetItemData(hItem);	
			m_pMainFrame->m_project.SetCurPageID((int)nPageID);
			CEqmDrawPage *pPage = m_pMainFrame->m_project.GetCurrentPage();
			if(!pPage->m_bDetailLoaded)
			{
				m_pMainFrame->m_LoadingProjectBar.ShowWindow(TRUE);
				m_pMainFrame->m_LoadingProjectBar.CenterWindow(m_pMainFrame);
				m_pMainFrame->m_LoadingProjectBar.m_StaticProgress.SetWindowTextW(L"");
				m_pMainFrame->m_LoadingProjectBar.m_ProgressCtrl.SetRange(0,15);
				m_pMainFrame->m_LoadingProjectBar.m_ProgressCtrl.SetPos(0);
				m_pMainFrame->m_pPrjSqlite->SetPage(pPage);

				m_pMainFrame->m_bLoadingPage = true;
				UINT dwThreadId = 0;
				HANDLE m_threadhandle_LoadPage = (HANDLE)_beginthreadex(NULL,
					0,
					ThreadFunc_LoadPage, 
					(LPVOID)(m_pMainFrame),
					0,
					&dwThreadId);
			}
			else
			{
				m_pMainFrame->m_DrawContext.pPage = pPage;
				//m_pMainFrame->m_DrawContext.pPage->SetStartColor(RGB(158, 158, 158));
				//m_pMainFrame->ShowCurrentPage();	
			}
			DWORD screenwidth = 0;
			DWORD screenheight = 0;

			screenwidth = m_pMainFrame->m_DrawContext.GetScreenWidth();
			screenheight = m_pMainFrame->m_DrawContext.GetScreenHeight();

			m_pMainFrame->m_DrawContext.ClearSelect();
			pView->m_PageDlg->m_rtCanvas.bottom = static_cast<long>(m_pMainFrame->m_DrawContext.m_fScreenScale*(LONG)screenheight);  //edit by hun 8.1
			pView->m_PageDlg->m_rtCanvas.right  = static_cast<long>(m_pMainFrame->m_DrawContext.m_fScreenScale*(LONG)screenwidth);
			pView->m_PageDlg->SendMessage(WM_SIZE);
			pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_SHOW);
			pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);

			m_pMainFrame->m_nPasteXCount = 1;
			m_pMainFrame->m_nPasteYCount = 1;
		}
		else if (hItemParent == m_hRootPage)
		{	// group node
			(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
			m_wndFileView.Expand(hItem, uExpandFlag);

			pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);
		}
		else if (hItemParent == m_hRootTact)//双击策略的子项
		{	// thread node
			// refresh panel arbiter
			int nThreadAllNum = 0;
			int nThreadCurSelNum = 0;
			//策略双击选择时字体背景改成红色，这里为赋值方便绘制函数判断
			m_hLastDBCurSelItem = hItem;
			m_wndFileView.SetItemData(m_hLastDBCurSelItem, 1000);

			//初始化
			ControlDebug();
			GetTreeNodeNumInfo(m_hRootTact, hItem, nThreadAllNum, nThreadCurSelNum);
			CString strCurNodeName = GetCurSelTreeNodeName(hItem);
			pView->m_PageDlg->m_paneArbiter.SetThreadNum(nThreadAllNum, nThreadCurSelNum);
			pView->m_PageDlg->m_paneArbiter.SetThreadCurSelName(strCurNodeName);
			pView->m_PageDlg->m_paneArbiter.m_pointmanager = &(m_pMainFrame->GetPointMap());
			pView->m_PageDlg->m_paneArbiter.RefreshDLLsOfThread();

			pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_SHOW);
			pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);
		}
		else if (hItemParent == m_hRootCalc)
		{	// calc node
			CString strSelName = GetCurSelTreeNodeName(hItem);
			strSelName = strSelName.Left(strSelName.Find(_T("(")));
			vector<CalcModuleInfo>& vecCalc = m_pPrjSql->m_vecCalcModuleInfo;
			vector<CalcModuleInfo>::iterator	iter;

			pView->m_PageDlg->m_paneCalc.ClearCalcItemVector();
			for (iter=vecCalc.begin(); iter!=vecCalc.end(); iter++)
			{
				if (strSelName == iter->strName)
				{
					pView->m_PageDlg->m_paneCalc.SetCalcItemVector(iter->vecItem);
					break;
				}
			}
			pView->m_PageDlg->m_paneCalc.m_strPointName = strSelName;

			pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCalc.ShowWindow(SW_SHOW);
			pView->m_PageDlg->m_paneCalc.UpdateData(FALSE);
			pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);
		}
	}

	if (hItem == m_hRootPage)
	{	// root page
		(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
		m_wndFileView.Expand(hItem, uExpandFlag);

		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);
	}
	else if (hItem == m_hRootTact)
	{	// tactics node
		(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
		m_wndFileView.Expand(hItem, uExpandFlag);

		//pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
		//pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_SHOW);
		pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);
	}
	else if (hItem == m_hRootComm || hItem == m_hRootConfig || hItem == m_hRootCalc)		//通讯
	{	// group node
		(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
		m_wndFileView.Expand(hItem, uExpandFlag);

		pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);
	}
	else if (hItem == m_hRootCommDebug)		//通讯调试
	{	// group node
		(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
		m_wndFileView.Expand(hItem, uExpandFlag);

		//初始化
		initPageDlgDBAccess();

		pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);	
		pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_SHOW);
		pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);
	}
	else if (hItem == m_hRootCommSet)	//通讯设置
	{	// group node
		(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
		m_wndFileView.Expand(hItem, uExpandFlag);

		//初始化
		initPageDlgDBAccess();

		pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);	
		pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_SHOW);
		pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);
	}
	else if (hItem == m_hRootValue)		//点值监控
	{	// group node
		(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
		m_wndFileView.Expand(hItem, uExpandFlag);

		//初始化
		initPageDlgDBAccess();

		/*pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);	
		pView->m_PageDlg->m_paneChart.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);*/
		pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_SHOW);
	}
	else if (hItem == m_hRootChart)	//实时曲线
	{	// group node
		(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
		m_wndFileView.Expand(hItem, uExpandFlag);

		//初始化
		initPageDlgDBAccess();

		pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneChart.ShowWindow(SW_SHOW);
		pView->m_PageDlg->m_paneCommDebug.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCommSet.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_pPanePoint->ShowWindow(SW_HIDE);
	}
	*pResult = 0; 
}

void CFileView::OnNMCustomdrawTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);
	switch (pNMCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	case CDDS_ITEMPREPAINT:
		if(pNMCD->nmcd.lItemlParam == 1000)
		{
			pNMCD->clrTextBk = RGB(239, 81, 81); // 这样设置背景颜色
		}
		break;
	}
	*pResult = 0;
}

UINT WINAPI CFileView::ThreadFunc_LoadPage( LPVOID lparam )
{
	CMainFrame *pMainFrame = (CMainFrame *) lparam;

	if (!(pMainFrame->m_bLoadIndependent))
	{
		pMainFrame->m_bLoadIndependent = true;

		CProgressCtrl*	pProgressCtrl	= &(pMainFrame->m_LoadingProjectBar.m_ProgressCtrl);
		CStatic*		pStatic			= &(pMainFrame->m_LoadingProjectBar.m_StaticProgress);
		bool			bIsEnglish		= pMainFrame->m_pPrjSqlite->GetIsEnglishLanguage();
		if (pProgressCtrl != NULL && pStatic != NULL)
		{
			pProgressCtrl->StepIt();
			if (bIsEnglish)
			{
				pStatic->SetWindowText(_T("loading pictures lib."));
			}
			else
			{
				pStatic->SetWindowText(_T("加载图片库..."));
			}
		}
		pMainFrame->m_pPrjSqlite->LoadPictureLibSqlite();
		pMainFrame->m_pPrjSqlite->LoadPictureLibSqlite(false);

		if (pProgressCtrl != NULL && pStatic != NULL)
		{
			pProgressCtrl->StepIt();
			if (bIsEnglish)
			{
				pStatic->SetWindowText(_T("loading animation."));
			}
			else
			{
				pStatic->SetWindowText(_T("加载动画..."));
			}
		}
		pMainFrame->m_pPrjSqlite->LoadAnimationSqlite();
		pMainFrame->m_pPrjSqlite->LoadAnimationSqlite(false);
	}
	pMainFrame->m_pPrjSqlite->SetShowErrMessage(false);
	pMainFrame->m_pPrjSqlite->LoadPageDetails(&(pMainFrame->m_LoadingProjectBar.m_ProgressCtrl),&(pMainFrame->m_LoadingProjectBar.m_StaticProgress));
	pMainFrame->m_LoadingProjectBar.ShowWindow(FALSE);
	pMainFrame->m_DrawContext.pPage = pMainFrame->m_project.GetCurrentPage();
	//pMainFrame->m_DrawContext.pPage->SetStartColor(RGB(158, 158, 158));

	int nBkImgId = pMainFrame->m_DrawContext.pPage->GetBKImageID();
	Image* pImage = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nBkImgId, true);
	if (pImage)
	{
		pMainFrame->m_DrawContext.pPage->SetBkImage(pImage);
	}

	// show current canvas
	pMainFrame->m_DrawContext.ClearSelect();
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
	ASSERT(pView != NULL);
	if (pView != NULL)
	{
		pView->m_PageDlg->SendMessage(WM_SIZE);
		pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_SHOW);
		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);
	}

	pMainFrame->RedrawWindow();
	pMainFrame->m_bLoadingPage = false;
	if(pMainFrame->m_bCheckImageMode && pMainFrame->m_nElementID>-1)
	{
		pMainFrame->ShowSelectElementByID(pMainFrame->m_nElementID);
	}
	return 1;
}

void CFileView::Expand()
{
	ASSERT(m_hRootPage != NULL);
	m_wndFileView.Expand(m_hRootPage, TVE_EXPAND);
	ASSERT(m_hRootTact != NULL);
	m_wndFileView.Expand(m_hRootTact, TVE_EXPAND);
	ASSERT(m_hRootCalc != NULL);
	m_wndFileView.Expand(m_hRootCalc, TVE_EXPAND);
	ASSERT(m_hRootComm != NULL);
	m_wndFileView.Expand(m_hRootComm, TVE_EXPAND);
	ASSERT(m_hRootValue != NULL);
	m_wndFileView.Expand(m_hRootValue, TVE_EXPAND);
	ASSERT(m_hRootChart != NULL);
	m_wndFileView.Expand(m_hRootChart, TVE_EXPAND);
}

void CFileView::SelectFirstPageItem(const int nPageID)
{
	const size_t size = m_vecTree.size();
	for (size_t i=0;i<size;++i)
	{
		if (nPageID == m_vecTree[i].nPageID)
		{
			BOOL bRes = m_wndFileView.SelectItem(m_vecTree[i].hItem);
			ASSERT(bRes);
			break;
		}
	}
}

void CFileView::DeleteAllItems()
{
	if(m_hRootPage)
	{
		HTREEITEM hItem = m_wndFileView.GetChildItem(m_hRootPage);
		vector<HTREEITEM> vec_group;
		while (hItem)
		{
			vec_group.push_back(hItem);
			int nGroupID =  (int)m_wndFileView.GetItemData(hItem);
			m_pMainFrame->m_project.DeleteGroup(nGroupID);
			HTREEITEM hChild = m_wndFileView.GetChildItem(hItem);
			vector<HTREEITEM> vec;
			while (hChild)
			{
				vec.push_back(hChild);
				int nPageID = (int)m_wndFileView.GetItemData(hChild);
				CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
				if(pPage)
				{
					m_pMainFrame->m_project.DeletePage(pPage);
				}
				hChild = m_wndFileView.GetNextItem(hChild, TVGN_NEXT);
			}
			const size_t size = vec.size();
			for (size_t i=0;i<size;++i)
			{
				BOOL bRes = m_wndFileView.DeleteItem(vec[i]);
				ASSERT(bRes);
			}
			hItem = m_wndFileView.GetNextItem(hItem, TVGN_NEXT);
		}
		{
			const size_t size_group = vec_group.size();
			for (size_t j=0;j<size_group;++j)
			{
				int nGroupID = (int)m_wndFileView.GetItemData(vec_group[j]);
				BOOL bRes = m_wndFileView.DeleteItem(vec_group[j]);
				m_pMainFrame->m_project.DeleteGroup(nGroupID);
				ASSERT(bRes);
			}
		}
		{
			BOOL bRes = m_wndFileView.DeleteItem(m_hRootPage);
			m_hRootPage = NULL;
		}
		m_vecTree.clear();
	}

	//
	m_wndFileView.DeleteItem(m_hRootTact);
	m_hRootTact = NULL;

	m_wndFileView.DeleteItem(m_hRootCalc);
	m_hRootCalc = NULL;

	m_wndFileView.DeleteItem(m_hRootModbusClientConfig);
	m_hRootModbusClientConfig = NULL;

	m_wndFileView.DeleteItem(m_hRootSystemDefineConfig);
	m_hRootSystemDefineConfig = NULL;

	m_wndFileView.DeleteItem(m_hRootCloudSettingConfig);
	m_hRootCloudSettingConfig = NULL;

	m_wndFileView.DeleteItem(m_hRootServiceManageConfig);
	m_hRootServiceManageConfig = NULL;

	m_wndFileView.DeleteItem(m_hRootReportManageConfig);
	m_hRootReportManageConfig = NULL;

	m_wndFileView.DeleteItem(m_hRootConfig);
	m_hRootConfig = NULL;

	m_wndFileView.DeleteItem(m_hRootComm);
	m_hRootComm = NULL;

	m_wndFileView.DeleteItem(m_hRootValue);
	m_hRootValue = NULL;

	m_wndFileView.DeleteItem(m_hRootChart);
	m_hRootChart = NULL;

	//m_wndFileView.DeleteItem(m_hRootImgLib);
	//m_hRootImgLib = NULL;

	//m_wndFileView.DeleteItem(m_hRootTactLib);
	//m_hRootTactLib = NULL;
}

void CFileView::UpdateItemPage( const int nPageID )
{
	const size_t size = m_vecTree.size();
	for (size_t i=0;i<size;++i)
	{
		if (nPageID == m_vecTree[i].nPageID)
		{
			CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
			ASSERT(pPage);
			wstring strName = pPage->GetPageName();
			m_wndFileView.SetItemText(m_vecTree[i].hItem, strName.c_str());
		}
	}
}

void CFileView::OnBnClickedButtonMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSelectItem = m_wndFileView.GetSelectedItem();
	int nSelectPageID = (int)m_wndFileView.GetItemData(hSelectItem);
	int nNextPageID = 0;
	if (IsTreeItemPage(hSelectItem))
	{
		HTREEITEM hPrev = m_wndFileView.GetPrevSiblingItem(hSelectItem);
		if (hPrev)
		{
			nNextPageID = (int)m_wndFileView.GetItemData(hPrev);
			HTREEITEM hNewItem =  m_wndFileView.InsertItem(m_wndFileView.GetItemText(hPrev),4,5,m_wndFileView.GetParentItem(hSelectItem),hSelectItem);
			assert(hNewItem);
			m_wndFileView.SetItemData(hNewItem, (DWORD)nNextPageID);
			vector<TREE_INFO>::iterator iter = m_vecTree.begin();
			while (iter != m_vecTree.end())
			{
				if (iter->nPageID == nNextPageID)
				{
					iter = m_vecTree.erase(iter);
				}
				else
				{
					++iter;
				}
			}
			m_wndFileView.DeleteItem(hPrev);
			TREE_INFO info;
			info.hItem = hNewItem;
			info.nPageID = nNextPageID;
			m_vecTree.push_back(info);
			m_pMainFrame->m_project.SwapTwoElementInPageList(nSelectPageID, nNextPageID);
			m_wndFileView.SetFocus();
			m_wndFileView.SelectItem(hSelectItem);
		}

		m_pMainFrame->SetSaveFlag(true);
	}
}

void CFileView::OnBnClickedButtonMoveDown()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSelectItem = m_wndFileView.GetSelectedItem();
	int nSelectPageID = (int)m_wndFileView.GetItemData(hSelectItem);
	int nNextPageID = 0;
	if (IsTreeItemPage(hSelectItem))
	{
		HTREEITEM hNext = m_wndFileView.GetNextSiblingItem(hSelectItem);
		if (hNext)
		{
			nNextPageID = (int)m_wndFileView.GetItemData(hNext);
			HTREEITEM hNewItem =  m_wndFileView.InsertItem(m_wndFileView.GetItemText(hSelectItem),4,5,m_wndFileView.GetParentItem(hSelectItem),hNext);
			assert(hNewItem);
			m_wndFileView.SetItemData(hNewItem, (DWORD)nSelectPageID);
			vector<TREE_INFO>::iterator iter = m_vecTree.begin();
			while (iter != m_vecTree.end())
			{
				if (iter->nPageID == nSelectPageID)
				{
					iter = m_vecTree.erase(iter);
				}
				else
				{
					++iter;
				}
			}
			m_wndFileView.DeleteItem(hSelectItem);
			TREE_INFO info;
			info.hItem = hNewItem;
			info.nPageID = nSelectPageID;
			m_vecTree.push_back(info);
			m_pMainFrame->m_project.SwapTwoElementInPageList(nSelectPageID, nNextPageID);
			m_wndFileView.SetFocus();
			m_wndFileView.SelectItem(hNewItem);
		}

		m_pMainFrame->SetSaveFlag(true);
	}
}

bool CFileView::IsTreeItemPage( HTREEITEM hItem )
{
	if (hItem == NULL)
	{
		return false;
	}
	if (hItem == m_hRootPage)
	{
		return false;
	}
	HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
	if (hParent)
	{
		if (hParent == m_hRootPage)
		{
			return false;
		}
		if (m_wndFileView.GetParentItem(hParent) == m_hRootPage)
		{
			return true;
		}
	}
	return false;
}

void CFileView::OnNMRClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//CPoint pt;
	//GetCursorPos(&pt);
	//::ScreenToClient(m_wndFileView.m_hWnd, &pt);
	//UINT uFlags;
	//HTREEITEM hItem = m_wndFileView.HitTest(pt, &uFlags);

	//if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	//{
	//	m_wndFileView.SelectItem(hItem);
	//}
	//HTREEITEM hSelectItem = m_wndFileView.GetSelectedItem();
	//CMenu* pSubMenu = NULL;
	//CPoint oPoint(0, 0);
	//GetCursorPos(&oPoint);
	//if (IsItemRootPage(hSelectItem))
	//{	// root page
	//	pSubMenu = m_menu.GetSubMenu(1);
	//	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	//}
	//else if (IsItemGroup(hSelectItem))
	//{	// group
	//	m_hSelectedItem = hSelectItem;

	//	pSubMenu = m_menu.GetSubMenu(2);
	//	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	//}
	//else if (IsItemPage(hSelectItem))
	//{	// page
	//	if(m_pMainFrame->m_bLoadingPage)
	//	{
	//		return;
	//	}

	//	m_wndFileView.Expand(hSelectItem,TVE_EXPAND);
	//	const int nPageID = (int)m_wndFileView.GetItemData(hSelectItem);
	//	CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
	//	if (pPage != NULL)
	//	{
	//		m_pSelectedPage = pPage;
	//		m_hSelectedItem = hSelectItem;
	//	}

	//	pSubMenu = m_menu.GetSubMenu(3);
	//	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	//}
	//else if (IsItemRootTactics(hSelectItem))
	//{	// tactics
	//	pSubMenu = m_menu.GetSubMenu(4);
	//	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	//}
	//else if (IsItemThread(hSelectItem))
	//{	// thread
	//	m_hCurSelItem = hSelectItem;
	//	pSubMenu = m_menu.GetSubMenu(5);
	//	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	//}
	//else if (IsItemRootTacticsLib(hSelectItem))
	//{
	//	pSubMenu = m_menu.GetSubMenu(6);
	//	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	//}
/*
	if (IsTreeItemPage(hSelectItem))
	{
		if(m_pMainFrame->m_bLoadingPage)
		{
			return;
		}

		m_wndFileView.Expand(hSelectItem,TVE_EXPAND);
		const int nPageID = (int)m_wndFileView.GetItemData(hSelectItem);
		CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
		if (pPage)
		{
			if(!pPage->m_bDetailLoaded)
			{
				m_pMainFrame->m_LoadingProjectBar.ShowWindow(TRUE);
				m_pMainFrame->m_LoadingProjectBar.CenterWindow(m_pMainFrame);
				m_pMainFrame->m_LoadingProjectBar.m_StaticProgress.SetWindowTextW(L"");
				m_pMainFrame->m_LoadingProjectBar.m_ProgressCtrl.SetRange(0,15);
				m_pMainFrame->m_LoadingProjectBar.m_ProgressCtrl.SetPos(0);
				m_pMainFrame->m_pPrjSqlite->SetPage(pPage);

				m_pMainFrame->m_bLoadingPage = true;
				UINT dwThreadId = 0;
				HANDLE m_threadhandle_LoadPage = (HANDLE)_beginthreadex(NULL,
					0,
					ThreadFunc_LoadPage, 
					(LPVOID)(m_pMainFrame),
					0,
					&dwThreadId);


			}
			m_pSelectedPage = pPage;
			m_hSelectedItem = hSelectItem;
			CPoint oPoint;
			GetCursorPos(&oPoint);
			CMenu* pSubMenu = m_menu.GetSubMenu(0);
			pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
		}
	}
	if(IsTreeItemGroup(hSelectItem))
	{
		m_wndFileView.Expand(hSelectItem,TVE_EXPAND);
		m_hSelectedItem = hSelectItem;
		CPoint oPoint;
		GetCursorPos(&oPoint);
		CMenu* pSubMenu = m_menu_group.GetSubMenu(0);
		pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
*/
	*pResult = 0;
}

void CFileView::OnTreePageProperty()
{
	// TODO: 在此添加命令处理程序代码
	CSetUpDataDlg dlg;
	dlg.SetPage(m_pSelectedPage);
	dlg.m_strEditOldValue = m_pSelectedPage->GetPageName().c_str();
	dlg.m_strEditNewValue = m_pSelectedPage->GetPageName().c_str();
	dlg.m_nPageType = m_pSelectedPage->GetPageType();
	dlg.m_nPicID = m_pSelectedPage->GetBKImageID();
	dlg.m_strWebAddr = m_pSelectedPage->GetWebAddress();
	CString strPageId;
	strPageId.Format(_T("%d"), m_pSelectedPage->GetID());
	dlg.m_strPageId = strPageId;
	if (dlg.DoModal() == IDOK)
	{
		int x = (m_pMainFrame->m_project.GetScreenWidth()-dlg.m_nWidth)/2;
		int y = (m_pMainFrame->m_project.GetScreenHeight()-dlg.m_nHeight)/2;
		m_pSelectedPage->SetPagexPosition(x);
		m_pSelectedPage->SetPageyPosition(y);
		m_pSelectedPage->SetPageName(dlg.m_strEditNewValue.GetString());
		m_pSelectedPage->SetStartColor(dlg.m_nStartColor);
		m_pSelectedPage->SetEndColor(dlg.m_nEndColor);
		m_pSelectedPage->SetBKImageID(dlg.m_nPicID);
		m_pSelectedPage->SetPageWidth(dlg.m_nWidth);
		m_pSelectedPage->SetPageHeight(dlg.m_nHeight);
		m_pSelectedPage->SetPageShowTopInObserver(dlg.m_bObserverTopShow);
		m_pSelectedPage->SetPageType(dlg.m_nPageType);

		m_pSelectedPage->SetBKImageID(dlg.m_nPicID);
		Image* pImage = m_pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(dlg.m_nPicID, true);
		m_pSelectedPage->SetBkImage(pImage);
		m_pSelectedPage->SetWebAddress(dlg.m_strWebAddr);
		m_pSelectedPage->SetPageMinRight(dlg.m_nPageMinRight);

		CBEOPDesignerView* pView = (CBEOPDesignerView*)m_pMainFrame->GetActiveView();
		ASSERT(pView);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(TRUE);
		m_wndFileView.SetItemText(m_hSelectedItem, dlg.m_strEditNewValue);
		m_pMainFrame->SetSaveFlag(true);

		//如果背景图有效，且被更改，且处于连接调试状态，则请求后台更新图片
		if(dlg.m_bBackgroundImageModified && dlg.m_bEnableBk && m_bIsConnectServ)
		{
			//post请求 
			CString strServerIP = m_pMainFrame->m_wndFileView.GetDataAccess()->GetServerAddress();
			int nServerPort = m_pMainFrame->m_wndFileView.GetDataAccess()->GetServerPort();

			CString strLogResult;
			Json::Value myRoot;
			myRoot["imageId"]=dlg.m_nPicID;
			string szBuf = myRoot.toStyledString();

			CHttpOperation http_log(strServerIP, nServerPort, _T("/projectImages/update"));
			http_log.SetInternetTimeOut(300000,300000);
			bool bRet_log = http_log.HttpPostGetRequestEx(0, szBuf.data(), strLogResult,  _T("Content-Type: application/json"));
			if(bRet_log)
			{
				if(strLogResult.Find(L"true") != -1);
				{
					AfxMessageBox(L"输出缓存成功");
				}
			}

		}

	}
}

bool CFileView::IsTreeItemGroup( HTREEITEM hItem )
{
	if (hItem == NULL)
	{
		return false;
	}
	if (hItem == m_hRootPage)
	{
		return false;
	}
	HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
	if (hParent == m_hRootPage)
	{
		return true;
	}
	return false;
}

void CFileView::OnImportPage()
{
	// TODO: 在此添加命令处理程序代码
	CImportPageSelectDlg dlg(false);
	dlg.m_hItem = m_hSelectedItem;
	dlg.DoModal();
}

void CFileView::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{	
	CPoint pt;
	GetCursorPos(&pt);
	::ScreenToClient(m_wndFileView.m_hWnd, &pt);
	UINT uFlags;
	HTREEITEM hItem1 = m_wndFileView.HitTest(pt, &uFlags);

	if(hItem1 && (TVHT_ONITEM & uFlags)) //判断是否有HTREEITEM
	{

		if(m_pMainFrame->m_pPrjSqlite ==NULL)
		{
			AfxMessageBox(_T("请先打开db文件"));
			return ;
		}
		if(hItem1== m_hRootModbusClientConfig)
		{
			CModbusComConfigDlg dlg;
			dlg.m_strConfig = m_pMainFrame->m_pPrjSqlite->GetModbusComConfig().c_str();
			if(dlg.DoModal()==IDOK)
			{
				m_pMainFrame->m_pPrjSqlite->SaveModbusComConfig(dlg.m_strConfig.GetString());
			}
		}else if(hItem1==m_hRootSystemDefineConfig)
		{
			CSystemDefineDlg dlg;
			dlg.m_strSystemDefine = m_pMainFrame->m_pPrjSqlite->GetSystemDefineConfig().c_str();
			if(dlg.DoModal()==IDOK)
			{
				m_pMainFrame->m_pPrjSqlite->SaveSystemDefineConfig(dlg.m_strSystemDefine.GetString());
			}
		}else if(hItem1== m_hRootReportManageConfig)
		{
			if(m_pDataAccess==NULL)
			{
				CWarningDlg WarnDlg2;
				WarnDlg2.InitWarn(_T("提示"), _T("请先连接在线调试"), FALSE);
				WarnDlg2.DoModal();
				return;
			}
			CReportManageDlg dlg;
			dlg.m_strServer = m_pDataAccess->GetServerAddress();
			dlg.m_nServerPort = m_pDataAccess->GetServerPort();
			
			dlg.DoModal();
			
		   
		}
		else if(hItem1==m_hRootCloudSettingConfig)
		{
			CCloudSettingDlg dlg;
			string strTemp;
			wstring wstrTemp;

			wstring wstrAll = m_pMainFrame->m_pPrjSqlite->GetValueInLocalConfigTable(_T("CloudSetting")).c_str();
			string strAll;
			Project::Tools::WideCharToUtf8(wstrAll, strAll);
			Json::Value myRoot(strAll);

			Json::Reader jsReader;

			if (strAll.length()>0 && jsReader.parse(strAll, myRoot))
			{
				if(!myRoot.isNull() && myRoot.size()>0)
				{
					int nCloudEnable = myRoot["CloudSettingEnable"].asInt();
					if(nCloudEnable==1)
					{
						dlg.m_bEnableCloud = TRUE;
					}
					else
					{
						dlg.m_bEnableCloud = FALSE;
					}
					strTemp = myRoot["CloudSettingProjectName"].asString();
					Project::Tools::UTF8ToWideChar(strTemp, wstrTemp);
					dlg.m_strProjectName = wstrTemp.c_str();

					strTemp = myRoot["CloudSettingUserName"].asString();
					Project::Tools::UTF8ToWideChar(strTemp, wstrTemp);
					dlg.m_strMQUserName = wstrTemp.c_str();

					strTemp = myRoot["CloudSettingPwd"].asString();
					Project::Tools::UTF8ToWideChar(strTemp, wstrTemp);
					dlg.m_strPwd = wstrTemp.c_str();

					strTemp = myRoot["CloudSettingServerAddress"].asString();
					Project::Tools::UTF8ToWideChar(strTemp, wstrTemp);
					dlg.m_strMQTTServerAddress = wstrTemp.c_str();

					strTemp = myRoot["CloudSettingTimeSpan"].asString();
					wstring strTimeSpan;
					Project::Tools::UTF8ToWideChar(strTemp, strTimeSpan);

					if(strTimeSpan.length()==0)
					{
						dlg.m_nTimeSpan = 60;
					}
					else
					{
						dlg.m_nTimeSpan = _wtoi(strTimeSpan.c_str());
					}


					Json::Value jv = myRoot["CloudCommandDownloadEnable"];
					if(!jv.isNull())
					{
						dlg.m_bPermitCommand = (jv.asInt()==1);
					}
				}
				
			}
			
			
			

			//modify
			if(dlg.DoModal()==IDOK)
			{
				Json::Value myRoot;
				
				
				if(dlg.m_bEnableCloud)
				{
					myRoot["CloudSettingEnable"] = 1;
				}
				else
				{
					myRoot["CloudSettingEnable"] = 0;
				}

				if(dlg.m_bPermitCommand)
				{
					myRoot["CloudCommandDownloadEnable"] = 1;
				}
				else
				{
					myRoot["CloudCommandDownloadEnable"] = 0;
				}

				Project::Tools::WideCharToUtf8(dlg.m_strProjectName.GetString(), strTemp);
				myRoot["CloudSettingProjectName"] = strTemp;

				Project::Tools::WideCharToUtf8(dlg.m_strPwd.GetString(), strTemp);
				myRoot["CloudSettingPwd"] = strTemp;

				Project::Tools::WideCharToUtf8(dlg.m_strMQUserName.GetString(), strTemp);
				myRoot["CloudSettingUserName"] = strTemp;

				Project::Tools::WideCharToUtf8(dlg.m_strMQTTServerAddress.GetString(), strTemp);
				myRoot["CloudSettingServerAddress"] = strTemp;

				int nTimeSpan = dlg.m_nTimeSpan;
				if(nTimeSpan<=0)
				{
					nTimeSpan = 60;
				}
				CString strTT;
				strTT.Format(_T("%d"), nTimeSpan);
				Project::Tools::WideCharToUtf8(strTT.GetString(), strTemp);
				myRoot["CloudSettingTimeSpan"] = strTemp;

				wstring wstrAll;
				Project::Tools::UTF8ToWideChar(myRoot.toStyledString(), wstrAll);
				m_pMainFrame->m_pPrjSqlite->SetValueInLocalConfigTable(_T("CloudSetting"), wstrAll);

			}
		}
		//进程服务管理 
		if(hItem1== m_hRootServiceManageConfig)
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if(pFrame->m_wndFileView.GetDataAccess())
			{
				CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
				if (NULL == pView || NULL == pView->m_PageDlg->m_paneArbiter.m_pDataAccess)
				{
					return;
				}
				CCoreServiceManageDlg dlg;
				dlg.m_bEnableModbusServer = pView->m_PageDlg->m_paneArbiter.m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"hostprocess", L"domModbusServer", L"domModbusServer.exe") ==L"1";
				dlg.m_bEnableModbusClient = pView->m_PageDlg->m_paneArbiter.m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"hostprocess", L"domModbusClientCore", L"domModbusClientCore.exe") ==L"1";
				dlg.m_bEnableKeysight = pView->m_PageDlg->m_paneArbiter.m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"hostprocess", L"domKeysightCore", L"domKeysightCore.exe") ==L"1";

				if(dlg.DoModal()==IDOK)
				{
					if(dlg.m_bEnableKeysight)
					{
						pView->m_PageDlg->m_paneArbiter.m_pDataAccess->WriteCoreDebugItemValue(L"hostprocess", L"domKeysightCore", L"domKeysightCore.exe", L"1");

					}
					else
					{
						pView->m_PageDlg->m_paneArbiter.m_pDataAccess->RemoveCoreDebugItem(L"hostprocess", L"domKeysightCore", L"domKeysightCore.exe");
					}

					if(dlg.m_bEnableModbusClient)
					{
						pView->m_PageDlg->m_paneArbiter.m_pDataAccess->WriteCoreDebugItemValue(L"hostprocess", L"domModbusClientCore", L"domModbusClientCore.exe", L"1");

					}
					else
					{
						pView->m_PageDlg->m_paneArbiter.m_pDataAccess->RemoveCoreDebugItem(L"hostprocess", L"domModbusClientCore", L"domModbusClientCore.exe");
					}

					if(dlg.m_bEnableModbusServer)
					{
						pView->m_PageDlg->m_paneArbiter.m_pDataAccess->WriteCoreDebugItemValue(L"hostprocess", L"domModbusServer", L"domModbusServer.exe", L"1");

					}
					else
					{
						pView->m_PageDlg->m_paneArbiter.m_pDataAccess->RemoveCoreDebugItem(L"hostprocess", L"domModbusServer", L"domModbusServer.exe");
					}
				}//end if(dlg.DoModal()==IDOK) 进程服务管理
			}
			else
			{
				AfxMessageBox(L"请先连接在线调试");
			}
		}	
		else if(hItem1== m_hRootProjectGlobalConfig)
		{
			CDlgSetting dlgsetting;
			dlgsetting.DoModal();
		}
	}
}

void CFileView::OnTvnItemexpandingTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	CPoint pt;
	GetCursorPos(&pt);
	::ScreenToClient(m_wndFileView.m_hWnd, &pt);
	UINT uFlags;
	HTREEITEM hItem1 = m_wndFileView.HitTest(pt, &uFlags);

	if(TVHT_ONITEM & uFlags)
	{
		*pResult = TRUE;
	}
	else
	{
		*pResult = 0;
	}	
}
//初始化FileWnd列表
void CFileView::InitTreeNodes(void)
{
	// insert default nodes
	if (NULL == m_hRootPage)
	{
		//init icon resources


		m_hRootPage = m_wndFileView.InsertItem(_T("页面"), 1, 1, TVI_ROOT, TVI_LAST);//
		ASSERT(m_hRootPage != NULL);
		if (NULL == m_hRootPage)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootPage, 1);
		ASSERT(bRes);

		if (1 == m_nDeviceType)
		{	// iphone4-6
			const int nGroupID = m_pMainFrame->m_project.AddGroup(L"iphone4-6");
			if (nGroupID != -1)
			{
				HTREEITEM hGroup = InsertGroup(_T("iphone4-6"), nGroupID);

				const int nWidth = 1920;
				const int nHeight = 955;
				const PAGETYPE page_type = FULLSCREEN;
				const int x = (m_pMainFrame->m_project.GetScreenWidth()-nWidth)/2;
				const int y = (m_pMainFrame->m_project.GetScreenHeight()-nHeight)/2;

				HTREEITEM hPage = InsertPage(_T("monitor"), hGroup, (DWORD)(m_pMainFrame->m_MaxPageID));
				int nPageMaxOrder = m_pMainFrame->m_project.GetMaxPageShowOrder() + 1;
				m_pMainFrame->m_DrawContext.pPage = new CEqmDrawPage(m_pMainFrame->m_MaxPageID, L"monitor", m_pMainFrame->m_project.GetProjectID(), nPageMaxOrder,page_type,nWidth,nHeight,x,y,RGB(219,219,219));
				m_pMainFrame->m_DrawContext.pPage->SetPageWidth(nWidth);
				m_pMainFrame->m_DrawContext.pPage->SetPageHeight(nHeight);
				m_pMainFrame->m_DrawContext.pPage->SetPageType(page_type);
				m_pMainFrame->m_DrawContext.pPage->SetGroupID(nGroupID); 
				m_pMainFrame->m_DrawContext.pPage->SetPageShowTopInObserver(true);
				m_pMainFrame->m_project.AddPage(m_pMainFrame->m_DrawContext.pPage);
				m_pMainFrame->m_project.SetCurPageID(m_pMainFrame->m_MaxPageID);
				m_pMainFrame->m_MaxPageID++;
				++nPageMaxOrder;

				hPage = InsertPage(_T("trend"), hGroup, (DWORD)(m_pMainFrame->m_MaxPageID));
				m_pMainFrame->m_DrawContext.pPage = new CEqmDrawPage(m_pMainFrame->m_MaxPageID, L"trend", m_pMainFrame->m_project.GetProjectID(), nPageMaxOrder,page_type,nWidth,nHeight,x,y,RGB(219,219,219));
				m_pMainFrame->m_DrawContext.pPage->SetPageWidth(nWidth);
				m_pMainFrame->m_DrawContext.pPage->SetPageHeight(nHeight);
				m_pMainFrame->m_DrawContext.pPage->SetPageType(page_type);
				m_pMainFrame->m_DrawContext.pPage->SetGroupID(nGroupID); 
				m_pMainFrame->m_DrawContext.pPage->SetPageShowTopInObserver(true);
				m_pMainFrame->m_project.AddPage(m_pMainFrame->m_DrawContext.pPage);
				m_pMainFrame->m_project.SetCurPageID(m_pMainFrame->m_MaxPageID);
				m_pMainFrame->m_MaxPageID++;

				m_wndFileView.Expand(m_hRootPage, TVE_EXPAND);
				m_wndFileView.Expand(hGroup, TVE_EXPAND);
				m_pMainFrame->SetSaveFlag(true);
			}
		}
	}
	if (NULL == m_hRootTact)
	{
		m_hRootTact = m_wndFileView.InsertItem(_T("策略::离线"), 2, 2, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootTact != NULL);
		if (NULL == m_hRootTact)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootTact, -10000);
		ASSERT(bRes);
	}
	if (NULL == m_hRootCalc)
	{
		m_hRootCalc = m_wndFileView.InsertItem(_T("计算模块"), 2, 2, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootCalc != NULL);
		if (NULL == m_hRootCalc)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootCalc, -40000);
		ASSERT(bRes);
	}
	if (NULL == m_hRootConfig)
	{
		m_hRootConfig = m_wndFileView.InsertItem(_T("通用配置"), 46, 46, TVI_ROOT, TVI_LAST);

		BOOL bRes = m_wndFileView.SetItemData(m_hRootConfig, -40000);
		ASSERT(bRes);


		m_hRootProjectGlobalConfig = m_wndFileView.InsertItem(_T("项目配置"), 42, 42, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootProjectGlobalConfig, -40000);
		ASSERT(bRes);

		m_hRootModbusClientConfig = m_wndFileView.InsertItem(_T("仪表接口配置"), 42, 42, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootModbusClientConfig, -40000);
		ASSERT(bRes);

		m_hRootSystemDefineConfig = m_wndFileView.InsertItem(_T("设备系统定义配置"), 32, 32, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootSystemDefineConfig, -40000);
		ASSERT(bRes);

		m_hRootCloudSettingConfig = m_wndFileView.InsertItem(_T("数据云连接配置"), 32, 32, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootCloudSettingConfig, -40000);
		ASSERT(bRes);

		m_hRootServiceManageConfig = m_wndFileView.InsertItem(_T("进程服务管理"), 32, 32, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootServiceManageConfig, -40000);
		ASSERT(bRes);

		m_hRootReportManageConfig = m_wndFileView.InsertItem(_T("后台文件附件管理"), 32, 32, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootReportManageConfig, -40000);
		ASSERT(bRes);

	}
	if (NULL == m_hRootComm)
	{
		m_hRootComm = m_wndFileView.InsertItem(_T("通讯"), 45, 45, TVI_ROOT, TVI_LAST);

		BOOL bRes = m_wndFileView.SetItemData(m_hRootComm, -40000);
		ASSERT(bRes);

		m_hRootCommSet = m_wndFileView.InsertItem(_T("通讯与调试"), 32, 32, m_hRootComm, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootCommSet, -40000);
		ASSERT(bRes);

		m_hRootCommDebug = m_wndFileView.InsertItem(_T("通讯测试"), 32, 32, m_hRootComm, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootCommDebug, -40000);
		ASSERT(bRes);
	}
	if (NULL == m_hRootValue)
	{
		m_hRootValue = m_wndFileView.InsertItem(_T("点值监控"), 44, 44, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootValue != NULL);
		if (NULL == m_hRootValue)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootValue, -40000);
		ASSERT(bRes);
	}
	if (NULL == m_hRootChart)
	{
		m_hRootChart = m_wndFileView.InsertItem(_T("实时曲线"), 43, 43, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootChart != NULL);
		if (NULL == m_hRootChart)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootChart, -40000);
		ASSERT(bRes);
	}
	//if (NULL == m_hRootImgLib)
	//{
	//	m_hRootImgLib = m_wndFileView.InsertItem(_T("图源库"), 11, 11, TVI_ROOT, TVI_LAST);
	//	ASSERT(m_hRootImgLib != NULL);
	//	if (NULL == m_hRootImgLib)
	//	{
	//		return;
	//	}
	//	BOOL bRes = m_wndFileView.SetItemData(m_hRootImgLib, -20000);
	//	ASSERT(bRes);
	//}
	//if (NULL == m_hRootTactLib)
	//{
	//	m_hRootTactLib = m_wndFileView.InsertItem(_T("策略库"), 30, 30, TVI_ROOT, TVI_LAST);
	//	ASSERT(m_hRootTactLib != NULL);
	//	if (NULL == m_hRootTactLib)
	//	{
	//		return;
	//	}
	//	BOOL bRes = m_wndFileView.SetItemData(m_hRootTactLib, -30000);
	//	ASSERT(bRes);
	//}

	// initialize tactics node
	InitTacticsNode();

	// initialize calc node
	InitCalcNode();

	// initialize tacitcs library
	//InitTacitcsLib();

	// initialize image library
	//InitImgLib();
}
void  CFileView::InitNode()
{
	// insert default nodes
	if (NULL == m_hRootPage)
	{
		//init icon resources


		m_hRootPage = m_wndFileView.InsertItem(_T("页面"), 1, 1, TVI_ROOT, TVI_LAST);//
		ASSERT(m_hRootPage != NULL);
		if (NULL == m_hRootPage)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootPage, 1);
		ASSERT(bRes);

		if (1 == m_nDeviceType)
		{	// iphone4-6
			const int nGroupID = m_pMainFrame->m_project.AddGroup(L"iphone4-6");
			if (nGroupID != -1)
			{
				HTREEITEM hGroup = InsertGroup(_T("iphone4-6"), nGroupID);

				const int nWidth = 1920;
				const int nHeight = 955;
				const PAGETYPE page_type = FULLSCREEN;
				const int x = (m_pMainFrame->m_project.GetScreenWidth()-nWidth)/2;
				const int y = (m_pMainFrame->m_project.GetScreenHeight()-nHeight)/2;

				HTREEITEM hPage = InsertPage(_T("monitor"), hGroup, (DWORD)(m_pMainFrame->m_MaxPageID));
				int nPageMaxOrder = m_pMainFrame->m_project.GetMaxPageShowOrder() + 1;
				m_pMainFrame->m_DrawContext.pPage = new CEqmDrawPage(m_pMainFrame->m_MaxPageID, L"monitor", m_pMainFrame->m_project.GetProjectID(), nPageMaxOrder,page_type,nWidth,nHeight,x,y,RGB(219,219,219));
				m_pMainFrame->m_DrawContext.pPage->SetPageWidth(nWidth);
				m_pMainFrame->m_DrawContext.pPage->SetPageHeight(nHeight);
				m_pMainFrame->m_DrawContext.pPage->SetPageType(page_type);
				m_pMainFrame->m_DrawContext.pPage->SetGroupID(nGroupID); 
				m_pMainFrame->m_DrawContext.pPage->SetPageShowTopInObserver(true);
				m_pMainFrame->m_project.AddPage(m_pMainFrame->m_DrawContext.pPage);
				m_pMainFrame->m_project.SetCurPageID(m_pMainFrame->m_MaxPageID);
				m_pMainFrame->m_MaxPageID++;
				++nPageMaxOrder;

				hPage = InsertPage(_T("trend"), hGroup, (DWORD)(m_pMainFrame->m_MaxPageID));
				m_pMainFrame->m_DrawContext.pPage = new CEqmDrawPage(m_pMainFrame->m_MaxPageID, L"trend", m_pMainFrame->m_project.GetProjectID(), nPageMaxOrder,page_type,nWidth,nHeight,x,y,RGB(219,219,219));
				m_pMainFrame->m_DrawContext.pPage->SetPageWidth(nWidth);
				m_pMainFrame->m_DrawContext.pPage->SetPageHeight(nHeight);
				m_pMainFrame->m_DrawContext.pPage->SetPageType(page_type);
				m_pMainFrame->m_DrawContext.pPage->SetGroupID(nGroupID); 
				m_pMainFrame->m_DrawContext.pPage->SetPageShowTopInObserver(true);
				m_pMainFrame->m_project.AddPage(m_pMainFrame->m_DrawContext.pPage);
				m_pMainFrame->m_project.SetCurPageID(m_pMainFrame->m_MaxPageID);
				m_pMainFrame->m_MaxPageID++;

				m_wndFileView.Expand(m_hRootPage, TVE_EXPAND);
				m_wndFileView.Expand(hGroup, TVE_EXPAND);
				m_pMainFrame->SetSaveFlag(true);
			}
		}
	}
	if (NULL == m_hRootTact)
	{
		m_hRootTact = m_wndFileView.InsertItem(_T("策略::离线"), 2, 2, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootTact != NULL);
		if (NULL == m_hRootTact)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootTact, -10000);
		ASSERT(bRes);
	}

	if (NULL == m_hRootCalc)
	{
		m_hRootCalc = m_wndFileView.InsertItem(_T("计算模块"), 2, 2, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootCalc != NULL);
		if (NULL == m_hRootCalc)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootCalc, -40000);
		ASSERT(bRes);
	}
	if (NULL == m_hRootConfig)
	{
		m_hRootConfig = m_wndFileView.InsertItem(_T("通用配置"), 46, 46, TVI_ROOT, TVI_LAST);

		BOOL bRes = m_wndFileView.SetItemData(m_hRootConfig, -40000);
		ASSERT(bRes);


		m_hRootModbusClientConfig = m_wndFileView.InsertItem(_T("仪表接口配置"), 42, 42, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootModbusClientConfig, -40000);
		ASSERT(bRes);

		m_hRootSystemDefineConfig = m_wndFileView.InsertItem(_T("设备系统定义配置"), 32, 32, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootSystemDefineConfig, -40000);
		ASSERT(bRes);

		m_hRootCloudSettingConfig = m_wndFileView.InsertItem(_T("数据云连接配置"), 32, 32, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootCloudSettingConfig, -40000);
		ASSERT(bRes);

		m_hRootServiceManageConfig = m_wndFileView.InsertItem(_T("进程服务管理"), 32, 32, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootServiceManageConfig, -40000);
		ASSERT(bRes);

		m_hRootReportManageConfig = m_wndFileView.InsertItem(_T("后台文件附件管理"), 32, 32, m_hRootConfig, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootReportManageConfig, -40000);
		ASSERT(bRes);

	}
	if (NULL == m_hRootComm)
	{
		m_hRootComm = m_wndFileView.InsertItem(_T("通讯"), 45, 45, TVI_ROOT, TVI_LAST);

		BOOL bRes = m_wndFileView.SetItemData(m_hRootComm, -40000);
		ASSERT(bRes);

		m_hRootCommSet = m_wndFileView.InsertItem(_T("通讯与调试"), 32, 32, m_hRootComm, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootCommSet, -40000);
		ASSERT(bRes);

		m_hRootCommDebug = m_wndFileView.InsertItem(_T("通讯测试"), 32, 32, m_hRootComm, TVI_LAST);
		bRes = m_wndFileView.SetItemData(m_hRootCommDebug, -40000);
		ASSERT(bRes);
	}
	if (NULL == m_hRootValue)
	{
		m_hRootValue = m_wndFileView.InsertItem(_T("点值监控"), 44, 44, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootValue != NULL);
		if (NULL == m_hRootValue)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootValue, -40000);
		ASSERT(bRes);
	}
	if (NULL == m_hRootChart)
	{
		m_hRootChart = m_wndFileView.InsertItem(_T("实时曲线"), 43, 43, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootChart != NULL);
		if (NULL == m_hRootChart)
		{
			return;
		}
		BOOL bRes = m_wndFileView.SetItemData(m_hRootChart, -40000);
		ASSERT(bRes);
	}
}
bool CFileView::IsItemRootPage(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (hItem == m_hRootPage)
	{
		return true;
	}

	return false;
}

bool CFileView::IsItemGroup(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (IsItemRootNode(hItem))
	{
		return false;
	}

	HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
	if (hParent == m_hRootPage)
	{
		return true;
	}

	return false;
}

bool CFileView::IsItemPage(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (IsItemRootNode(hItem))
	{
		return false;
	}

	HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
	if (hParent != NULL)
	{
		if (IsItemRootNode(hParent))
		{
			return false;
		}

		if (m_wndFileView.GetParentItem(hParent) == m_hRootPage)
		{
			return true;
		}
	}

	return false;
}

bool CFileView::IsItemRootTactics(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (hItem == m_hRootTact)
	{
		return true;
	}

	return false;
}

bool CFileView::IsItemThread(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (IsItemRootNode(hItem))
	{
		return false;
	}

	HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
	if (hParent == m_hRootTact)
	{
		return true;
	}

	return false;
}

bool CFileView::IsItemRootNode(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (hItem == m_hRootPage || hItem == m_hRootTact || hItem == m_hRootCalc || hItem == m_hRootComm || hItem == m_hRootValue || hItem == m_hRootChart /*|| hItem == m_hRootImgLib || hItem == m_hRootTactLib*/)
	{
		return true;
	}

	return false;
}

void CFileView::OnMenuAddGroup()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonAddGroup();
}


void CFileView::OnMenuDelGroup()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonDelGroup();
}


void CFileView::OnMenuEditGroup()
{
	// TODO: Add your command handler code here
	SetGroupName();
}


void CFileView::OnMenuAddPage()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonAddPage();
}


void CFileView::OnMenuDelPage()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonDelPage();
}



void CFileView::OnMenuAddThread()
{
	// TODO: Add your command handler code here
	CAddDllThreadNameDlg dlg(true);
	if (dlg.DoModal() == IDOK)
	{
		CString strDllName;
		strDllName = dlg.m_strDllName;
		//USES_CONVERSION;
		//string str(W2A(strDllName));
		string str;
		Project::Tools::WideCharToUtf8(strDllName.GetString(),str);
		for(int i=0;i<str.length();i++)
		{
			if(((str[i]<='Z')&&(str[i]>='A'))||((str[i]<='z')&&(str[i]>='a'))||((str[i]<='9')&&(str[i]>='0'))||(str[i]=='_'))
			{

			}
			else
			{
				AfxMessageBox(L"策略线程名字存在非法字符!");
				return;
			}
		}
		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
		if (NULL == pView)
		{
			return;
		}

		if (IsThreadNameExist(dlg.m_strDllName))
		{
			AfxMessageBox(L"策略线程已存在，创建失败!");
			return;
		}

		HTREEITEM hThread = NULL;
		if (m_hRootTact != NULL)
		{
			const int nWidth = dlg.m_nWidth;
			const int nHeight = dlg.m_nHeight;
			const PAGETYPE page_type = dlg.m_page_type;
			CString strDllNameEx;
			CString strStatus;
			int nPeriod = GetThreadPeriodicity(dlg.m_strDllName);
			if (m_bIsConnectServ)
			{
				strStatus = _T("Off");
			}
			else
			{
				strStatus = _T("Disconnect");
			}
			strDllNameEx.Format(_T("%s::(%s)(%d)"), dlg.m_strDllName, strStatus, nPeriod);
			hThread = InsertNodeThread(strDllNameEx, root_tactics);
			m_wndFileView.SetItemData(hThread, -10001);
			m_wndFileView.Expand(m_hRootTact, TVE_EXPAND);
			m_wndFileView.SetFocus();
			m_wndFileView.SortChildren(m_hRootTact);//排序
			m_wndFileView.SelectItem(hThread);
			m_pMainFrame->ShowCurrentPage();
		}
		else
		{
			MessageBox(L"创建线程节点失败!");
			return;
		}

		CLogicDllThread *pDllThread = pView->m_PageDlg->m_paneArbiter.FindDLLThreadByName(dlg.m_strDllName.GetString());
		if (NULL == pDllThread)
		{
			pDllThread = new CLogicDllThread(dlg.m_strDllName.GetString());
			pView->m_PageDlg->m_paneArbiter.m_pLogicManager->m_vecDllThreadList.push_back(pDllThread);
		}

		//写到数据库
		if (m_bIsConnectServ)
		{
			pView->m_PageDlg->m_paneArbiter.m_pDataAccess->WriteCoreDebugItemValue(dlg.m_strDllName.GetString(), L"0");
		}

		pView->m_PageDlg->m_paneArbiter.RefreshDLLsOfThread();

		// select new add thread and show its interface
		int nThreadAllNum = 0;
		int nThreadCurSelNum = 0;
		HTREEITEM hItem = m_wndFileView.GetSelectedItem();

		GetTreeNodeNumInfo(m_hRootTact, hItem, nThreadAllNum, nThreadCurSelNum);
		CString strCurNodeName = GetCurSelTreeNodeName(hThread);
		pView->m_PageDlg->m_paneArbiter.SetThreadNum(nThreadAllNum, nThreadCurSelNum);
		pView->m_PageDlg->m_paneArbiter.SetThreadCurSelName(strCurNodeName);
		pView->m_PageDlg->m_paneArbiter.RefreshDLLsOfThread();
		m_pMainFrame->SetSaveFlag(true);
	}
}

void CFileView::OnMenuDelThread()
{
	// TODO: Add your command handler code here
	CString strthreadName;
	strthreadName = GetCurSelTreeNodeName(m_hCurSelItem);
	m_wndFileView.DeleteItem(m_hCurSelItem);

	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		return;
	}

	//修改s3db
	strthreadName = strthreadName.Left(strthreadName.Find(':'));
	pView->m_PageDlg->m_paneArbiter.m_pLogicManager->DeleteThreadName(strthreadName);

	pView->m_PageDlg->m_paneArbiter.RefreshDLLsOfThread();

	m_pMainFrame->SetSaveFlag(true);
}

void CFileView::OnMenuModifyThread()
{
	// TODO: Add your command handler code here
	CString strThreadName;
	CString strThread;
	CString strThreadNameNew;

	strThreadName = GetCurSelTreeNodeName(m_hCurSelItem);
	strThread = strThreadName;
	strThreadName = strThreadName.Left(strThreadName.Find(':'));

	CAddDllThreadNameDlg dlg(false);
	dlg.m_strDllName = strThreadName;
	if (dlg.DoModal() == IDOK)
	{
		strThreadNameNew = dlg.m_strDllName;
		if (strThreadName != strThreadNameNew && IsThreadNameExist(strThreadNameNew))
		{
			AfxMessageBox(L"策略线程已存在，修改名称失败!");
			return;
		}
		else if (strThreadName == strThreadNameNew)
		{
			return;
		}

		strThread.Replace(strThreadName, strThreadNameNew);
		m_wndFileView.SetItemText(m_hCurSelItem, strThread);
		m_wndFileView.SortChildren(m_hRootTact);//排序
		
		//修改mysql中的状态表的点名
		m_pDataAccess->UpdateUnit01ThreadName(strThreadName.GetString(), strThreadNameNew.GetString());

		//修改s3db
		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
		if (NULL == pView)
		{
			return;
		}
		pView->m_PageDlg->m_paneArbiter.m_pLogicManager->UpdateThreadName(strThreadName, strThreadNameNew);

		CLogicDllThread *pDLLThread = pView->m_PageDlg->m_paneArbiter.FindDLLThreadByName(strThreadName.GetString());
		if (pDLLThread == NULL)
			return;
		pDLLThread->SetName(strThreadNameNew.GetString());

		m_pMainFrame->SetSaveFlag(true);
	}
}

void CFileView::OnMenuRunThread()
{
	// TODO: Add your command handler code here
	CString strthreadName;
	strthreadName = GetCurSelTreeNodeName(m_hCurSelItem);
	strthreadName = strthreadName.Left(strthreadName.Find(':'));

	CString strShow;
	int nPeriod = GetThreadPeriodicity(strthreadName);

	assert(m_pDataAccess != NULL);
	if (m_pDataAccess != NULL)
	{
		m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), L"1");
		m_pMainFrame->m_pPrjSqlite->SaveBackupOneLogicStatus(strthreadName, 1, nPeriod);
	}

	strShow.Format(_T("%s::(On)(%d)"), strthreadName, nPeriod);
	m_wndFileView.SetItemText(m_hCurSelItem, strShow);
	SetTreeThreadItemImage(m_hCurSelItem, 1);
}

void CFileView::OnMenuStopThread()
{
	// TODO: Add your command handler code here
	CString strthreadName;
	strthreadName = GetCurSelTreeNodeName(m_hCurSelItem);
	strthreadName = strthreadName.Left(strthreadName.Find(':'));

	CString strShow;
	int nPeriod = GetThreadPeriodicity(strthreadName);
	assert(m_pDataAccess != NULL);
	if (m_pDataAccess != NULL)
	{
		m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), L"0");
		m_pMainFrame->m_pPrjSqlite->SaveBackupOneLogicStatus(strthreadName, 0, nPeriod);
	}

	strShow.Format(_T("%s::(Off)(%d)"), strthreadName, nPeriod);
	m_wndFileView.SetItemText(m_hCurSelItem, strShow);
	SetTreeThreadItemImage(m_hCurSelItem, 0);
}
bool CFileView::InitTacticsNode(void)
{
	if (NULL == m_pMainFrame)
	{
		m_pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	}
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		return false;
	}
	pView->m_PageDlg->m_paneArbiter.InitLogicMgrPointer();

	//StartUpdateThread();

	//读入点表开始
	m_pMainFrame->m_Onlineplcpointmap.Clear();
	m_pMainFrame->m_OnlinelogicPotManager.Clear();
	m_pMainFrame->m_OnlineAlarmPotManager.Clear();

	if (NULL == m_pMainFrame->m_pOnlinePrjSqlite)
	{
		m_pMainFrame->InitOnlinePrjSqlite();
	}
	m_pMainFrame->m_pOnlinePrjSqlite->SetFilePath(m_pMainFrame->m_strDBFileName.c_str());

	m_pMainFrame->m_pOnlinePrjSqlite->SetOwner(eFactory);
	m_pMainFrame->m_pOnlinePrjSqlite->LoadProjectConfig();
	m_pMainFrame->m_pOnlinePrjSqlite->LoadProjectSqlite_opcid3();
	//读入点表结束

	pView->m_PageDlg->m_paneArbiter.m_pointmanager = &(m_pMainFrame->GetOnlinePointMap());


	// firstly, delete from m_hRootTact tree
	//DeleteTreeNode(m_hRootTact);

	// secondly, insert into m_hRootTact tree
	vector<CString> vecName;
	vector<CString>::const_iterator iter;
	HTREEITEM hThread = NULL;
	vecName.clear();
	pView->m_PageDlg->m_paneArbiter.GetInitThreadName(vecName);
	//线程列表盘排序
	sort(vecName.begin(), vecName.end());
	//将线程列表添加到“策略”列表
	for (iter=vecName.begin(); iter!=vecName.end(); ++iter)
	{
		hThread = InsertNodeThread(*iter, root_tactics);
		m_wndFileView.SetItemData(hThread, -10001);
	}
	m_wndFileView.Invalidate(FALSE);
	m_wndFileView.Expand(m_hRootTact, TVE_EXPAND);
	pView->m_PageDlg->m_paneArbiter.InitListTitle();
	//pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
	pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);


	return true;
}

bool CFileView::InitCalcNode(void)
{
	if (NULL == m_pPrjSql)
	{
		m_pPrjSql = m_pMainFrame->m_pPrjSqlite;
		if (NULL == m_pPrjSql)
		{
			return false;
		}
	}

	vector<CalcModuleInfo>& vecCalcModInf = m_pPrjSql->m_vecCalcModuleInfo;
	vector<CalcModuleInfo>::const_iterator	iter;
	HTREEITEM hThread = NULL;
	for (iter=vecCalcModInf.begin(); iter!=vecCalcModInf.end(); iter++)
	{
		hThread = InsertNodeThread(iter->strName, root_calc, iter->nRunState);
		SetTreeCalcItemText(hThread, *iter);
	}
	m_wndFileView.Expand(m_hRootCalc, TVE_EXPAND);

	return true;
}
bool  CFileView::ClearTreeNode(void)
{
    DeleteTreeNode(m_hRootTact);
	return true;
}
bool CFileView::IsItemRootCalc(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (hItem == m_hRootCalc)
	{
		return true;
	}

	return false;
}

bool CFileView::IsItemCalcItem(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (IsItemRootNode(hItem))
	{
		return false;
	}

	HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
	if (hParent == m_hRootCalc)
	{
		return true;
	}

	return false;
}


HTREEITEM CFileView::InsertNodeThread(const CString strName, const ROOT_TYPE emRootType, const int nStatus)
{
	HTREEITEM hParent = NULL;
	int nImage = 0;
	int nSelImage = 0;
	switch (emRootType)
	{
	case root_page:
		{
			hParent = m_hRootPage;
			nImage = 3;
			nSelImage = 3;
			break;
		}
	case root_tactics:
		{
			hParent = m_hRootTact;
			//nSelImage = 9;
			switch (nStatus)
			{
			case -1:
				nImage = 41;
				break;
			case 0:
				nImage = 40;
				break;
			case 1:
				nImage = 39;
				break;
			default:
				nImage = 41;
				break;
			}
			nSelImage = nImage;
			break;
		}
	case root_calc:
		{
			hParent = m_hRootCalc;
			nSelImage = 43;
			switch (nStatus)
			{
			case -1:
				nImage = 46;
				break;
			case 0:
				nImage = 45;
				break;
			case 1:
				nImage = 44;
				break;
			default:
				nImage = 46;
				break;
			}
			break;
		}
	//case root_img_lib:
	//	{
	//		hParent = m_hRootImgLib;
	//		break;
	//	}
	//case root_tactics_lib:
	//	{
	//		hParent = m_hRootTactLib;
	//		nImage = 32;
	//		nSelImage = 33;
	//		break;
	//	}
	default:
		return NULL;
	}

	HTREEITEM hThread = m_wndFileView.InsertItem(strName, nImage, nSelImage, hParent, TVI_LAST);
	ASSERT(hThread);

	return hThread;
}

void CFileView::DeleteTreeNode(const HTREEITEM hNode)
{
	// Delete all of the children within hNode.
	if (m_wndFileView.ItemHasChildren(hNode))
	{
		HTREEITEM hNextItem = NULL;
		HTREEITEM hChildItem = m_wndFileView.GetChildItem(hNode);

		while (hChildItem != NULL)
		{
			hNextItem = m_wndFileView.GetNextItem(hChildItem, TVGN_NEXT);
			m_wndFileView.DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
}

void CFileView::GetTreeNodeNumInfo(const HTREEITEM hRootNode, const HTREEITEM hCurNode, int& nAllNum, int& nCurSelNum)
{
	nAllNum = 0;
	nCurSelNum = 0;
	bool bIsFind = false;


	if (m_wndFileView.ItemHasChildren(hRootNode))
	{
		HTREEITEM hItem = m_wndFileView.GetChildItem(hRootNode);
		while (hItem != NULL)
		{
			if (hCurNode == hItem)
			{
				bIsFind = true;
			}
			if (!bIsFind)
			{
				++nCurSelNum;
			}

			hItem = m_wndFileView.GetNextSiblingItem(hItem);
			++nAllNum;
		}
	}
	else
	{
		nAllNum = 0;
		nCurSelNum = 0;
	}
}

CString CFileView::GetCurSelTreeNodeName(const HTREEITEM hNode)
{
	return m_wndFileView.GetItemText(hNode);
}

void CFileView::GetTreeInfo(vector<TREE_GROUP>& vecTreeInf)
{
	// clear map struct
	m_vecTreeInfo.clear();

	// travel tree control
	m_nGroupShowOrder = 1;
	TravelTreeInfo(m_hRootPage);

	// get map struct info
	vecTreeInf = m_vecTreeInfo;
}

void CFileView::TravelTreeInfo(const HTREEITEM hItem)
{
	const HTREEITEM hParent			= m_wndFileView.GetParentItem(hItem);
	const HTREEITEM hGrandParent	= m_wndFileView.GetParentItem(hParent);
	int	nCurVal		= 0;
	int	nParentVal	= 0;
	TREE_GROUP	stGroup;

	if (hItem != m_hRootPage)
	{
		if (hParent == m_hRootPage)
		{	// group nodes
			stGroup.nGroupId = m_wndFileView.GetItemData(hItem);
			stGroup.nGroupShowOrder = m_nGroupShowOrder++;
			stGroup.vecPage.clear();
			m_vecTreeInfo.push_back(stGroup);
		}
		else if (hGrandParent == m_hRootPage)
		{	// page nodes
			nCurVal = m_wndFileView.GetItemData(hItem);
			nParentVal = m_wndFileView.GetItemData(hParent);

			for (m_iterTreeInfo=m_vecTreeInfo.begin(); m_iterTreeInfo!=m_vecTreeInfo.end(); ++m_iterTreeInfo)
			{
				if (m_iterTreeInfo->nGroupId == nParentVal)
				{
					bool bFindPage = false;
					for (m_iterPageId=m_iterTreeInfo->vecPage.begin(); m_iterPageId!=m_iterTreeInfo->vecPage.end(); ++m_iterPageId)
					{
						if (*m_iterPageId == nCurVal)
						{
							bFindPage = true;
							break;
						}
					}
					if (!bFindPage)
					{
						m_iterTreeInfo->vecPage.push_back(nCurVal);
					}
				}
			}
		}
	}

	if (m_wndFileView.ItemHasChildren(hItem))
	{
		HTREEITEM hChildItem = m_wndFileView.GetChildItem(hItem);
		while (hChildItem != NULL)
		{
			TravelTreeInfo(hChildItem);
			hChildItem = m_wndFileView.GetNextItem(hChildItem, TVGN_NEXT);
		}
	}
}

bool CFileView::IsThreadNameExist(const CString strThreadName)
{
	bool bFind = false;

	if (m_wndFileView.ItemHasChildren(m_hRootTact))
	{
		HTREEITEM hItem = m_wndFileView.GetChildItem(m_hRootTact);
		CString strthreadName__;
		while (hItem != NULL)
		{
			strthreadName__ = m_wndFileView.GetItemText(hItem);
			strthreadName__ = strthreadName__.Left(strthreadName__.Find(':'));

			if (strthreadName__ == strThreadName)
			{
				bFind = true;
				break;
			}

			hItem = m_wndFileView.GetNextSiblingItem(hItem);
		}
	}

	return bFind;
}

void CFileView::SetConnectServFlag(const bool bIsConnect)
{
	m_bIsConnectServ = bIsConnect;
}

void CFileView::OnMenuConnDebug()
{
	// TODO: Add your command handler code here
	if(m_pDataAccess != NULL)
		return;

	CString strPath;
	Project::Tools::GetSysPath(strPath);
	strPath += _T("\\debugtool.ini");

	TCHAR szBuf[MAX_PATH] = {0};
	GetPrivateProfileString(_T("beopdebugtool"), _T("server"), _T("127.0.0.1"), szBuf, MAX_PATH, strPath);

	CString strDbIp;
	strDbIp.Format(_T("%s"), szBuf);

	CConnectDlg dlg(szBuf);
	if (IDOK == dlg.DoModal())
	{
		USES_CONVERSION;
		string strIp;
		strIp = T2A(dlg.m_strDbIp.GetBuffer());
		dlg.m_strDbIp.ReleaseBuffer();

		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
		if(pView==NULL)
			return;

		pView->m_PageDlg->m_paneArbiter.InitLogicMgrPointer();

		gDataBaseParam realDBParam;
		realDBParam.strDBIP = strIp;
		realDBParam.strDBName = "";
		realDBParam.strRealTimeDBName = Project::Tools::WideCharToUtf8(m_pMainFrame->GetMySQLDatabaseName().GetString());
		realDBParam.strUserName = "root";
		realDBParam.nPort = 3306;
		realDBParam.strDBIP2 = "";
		m_pDataAccess = new CUPPCServerDataAccess();
		m_pDataAccess->SetDBparam(realDBParam);
		if(!(m_pDataAccess->InitConnection()))
		{
			return;
		}

		pView->m_PageDlg->m_paneArbiter.m_pDataAccess = m_pDataAccess;
		pView->m_PageDlg->m_paneArbiter.m_pLogicManager->m_pDataAccess = m_pDataAccess;
		if (NULL == m_pMainFrame)
		{
			m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
		}
		pView->m_PageDlg->m_paneArbiter.m_pointmanager = &(m_pMainFrame->GetPointMap());


		// firstly, delete from m_hRootTact tree
		DeleteTreeNode(m_hRootTact);

		// secondly, insert into m_hRootTact tree
		vector<pair<CString, int>> vecName;
		vector<pair<CString, int>>::const_iterator iter;
		HTREEITEM hThread = NULL;
		vecName.clear();
		pView->m_PageDlg->m_paneArbiter.InitData(vecName);
		sort(vecName.begin(),vecName.end());
		for (iter=vecName.begin(); iter!=vecName.end(); ++iter)
		{
			hThread = InsertNodeThread(iter->first, root_tactics, iter->second);
			m_wndFileView.SetItemData(hThread, -10001);
		}
		m_wndFileView.Invalidate(FALSE);
		m_wndFileView.Expand(m_hRootTact, TVE_EXPAND);
		//pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);


		// set flag
		SetConnectServFlag(true);

		// show config button
		//pView->m_PageDlg->m_paneArbiter.GetDlgItem(IDC_BUTTON_Config)->EnableWindow(SW_SHOW);

		// rename tactics node name
		CString strTacticsName;
		strTacticsName = m_wndFileView.GetItemText(m_hRootTact);
		strTacticsName = strTacticsName.Left(strTacticsName.Find(_T("::")));
		strTacticsName += _T("::在线::");
		strTacticsName += dlg.m_strDbIp;
		m_wndFileView.SetItemText(m_hRootTact, strTacticsName);

		// write info into ini file
		WritePrivateProfileString(_T("beopdebugtool"), _T("server"), dlg.m_strDbIp, strPath);
	}
}

void CFileView::OnUpdateMenuRunThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
}

void CFileView::OnUpdateMenuStopThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
}

HTREEITEM CFileView::GetRootTactics(void)
{
	return m_hRootTact;
}

void CFileView::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CFileView::OnMenuGroupUp()
{
	// TODO: Add your command handler code here
}

void CFileView::OnMenuGroupDown()
{
	// TODO: Add your command handler code here
}


void CFileView::OnMenuPageUp()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonMoveUp();
}


void CFileView::OnMenuPageDown()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonMoveDown();
}


void CFileView::OnMenuPageProperty()
{
	// TODO: Add your command handler code here
	OnTreePageProperty();
}


void CFileView::OnMenuUpdateLib()
{
	// TODO: Add your command handler code here
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		return;
	}

	int nThreadAllNum = 0;
	int nThreadCurSelNum = 0;
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();

	GetTreeNodeNumInfo(m_hRootTact, hItem, nThreadAllNum, nThreadCurSelNum);
	pView->m_PageDlg->m_paneArbiter.SetThreadNum(nThreadAllNum, nThreadCurSelNum);
	pView->m_PageDlg->m_paneArbiter.OnBnClickedButtonRefreshLogicdll();

	m_pMainFrame->SetSaveFlag(true);
}


void CFileView::OnMenuPageImport()
{
	// TODO: Add your command handler code here
	CImportPageSelectDlg dlg(false);
	dlg.m_hItem = m_hSelectedItem;
	dlg.DoModal();
}


void CFileView::OnMenuRunAllThread()
{
	// TODO: Add your command handler code here
	if (IDYES != MessageBox(_T("确认全部运行？"), _T("提示"), MB_YESNO | MB_ICONWARNING))
	{
		return;
	}

	assert(m_hRootTact != NULL);
	if (NULL == m_hRootTact)
	{
		return;
	}

	CString strthreadName;
	CString strNameAll;
	CString strShow;
	if (m_wndFileView.ItemHasChildren(m_hRootTact))
	{
		HTREEITEM hItem = m_wndFileView.GetChildItem(m_hRootTact);
		while (hItem != NULL)
		{
			strNameAll = GetCurSelTreeNodeName(hItem);
			strthreadName = strNameAll.Left(strNameAll.Find(':'));
			assert(m_pDataAccess != NULL);
			if (m_pDataAccess != NULL)
			{
				m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), _T("1"));
			}

			int nPeriod = GetThreadPeriodicity(strthreadName);
			strShow.Format(_T("%s::(On)(%d)"), strthreadName, nPeriod);
			m_wndFileView.SetItemText(hItem, strShow);
			SetTreeThreadItemImage(hItem, 1);

			hItem = m_wndFileView.GetNextSiblingItem(hItem);
		}
	}
}


void CFileView::OnMenuStopAllThread()
{
	// TODO: Add your command handler code here
	if (IDYES != MessageBox(_T("确认全部停止？"), _T("提示"), MB_YESNO | MB_ICONWARNING))
	{
		return;
	}

	assert(m_hRootTact != NULL);
	if (NULL == m_hRootTact)
	{
		return;
	}

	CString strthreadName;
	CString strNameAll;
	CString strShow;
	if (m_wndFileView.ItemHasChildren(m_hRootTact))
	{
		HTREEITEM hItem = m_wndFileView.GetChildItem(m_hRootTact);
		while (hItem != NULL)
		{
			strthreadName = GetCurSelTreeNodeName(hItem);
			strthreadName = strthreadName.Left(strthreadName.Find(':'));
			assert(m_pDataAccess != NULL);
			if (m_pDataAccess != NULL)
			{
				m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), _T("0"));
			}

			int nPeriod = GetThreadPeriodicity(strthreadName);
			strShow.Format(_T("%s::(Off)(%d)"), strthreadName, nPeriod);
			m_wndFileView.SetItemText(hItem, strShow);
			SetTreeThreadItemImage(hItem, 0);

			hItem = m_wndFileView.GetNextSiblingItem(hItem);
		}
	}
}


void CFileView::OnUpdateMenuRunAllThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
	if(m_bIsConnectServ == false)
		AfxMessageBox(L"未连接调试。");
}


void CFileView::OnUpdateMenuStopAllThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
	if(m_bIsConnectServ == false)
		AfxMessageBox(L"未连接调试。");
}


void CFileView::OnUpdateMenuUpdateLib(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
	if(m_bIsConnectServ == false)
		AfxMessageBox(L"未连接调试。");
}


void CFileView::OnUpdateMenuAddThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFileView::OnMenuModifyThreadPerio()
{
	// TODO: Add your command handler code here
	CString strThreadNameAll;
	CString strThreadName;
	wstring	strName;
	int nPeriodicity = 0;
	strThreadNameAll = GetCurSelTreeNodeName(m_hCurSelItem);
	strThreadName = strThreadNameAll.Left(strThreadNameAll.Find(':'));
	strName = strThreadName.GetString();
	nPeriodicity = GetThreadPeriodicity(strName.c_str());

	CThreadPeriodicitySet dlg;
	dlg.m_nThreadPeriodicity = nPeriodicity;
	if (dlg.DoModal() == IDOK)
	{
		nPeriodicity = dlg.m_nThreadPeriodicity;
		if (!IsThreadNameExist(strThreadName))
		{
			AfxMessageBox(L"策略线程不存在，修改周期失败！");
			return;
		}

		// 修改sqlite
		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
		ASSERT(NULL != pView);
		if (NULL == pView)
		{
			return;
		}
		pView->m_PageDlg->m_paneArbiter.m_pLogicManager->UpdateThreadPeriodicity(nPeriodicity, strThreadName);

		// 修改MySql -- unit02
		vector<CDLLObject*>& vecDll = pView->m_PageDlg->m_paneArbiter.m_pLogicManager->vecImportDLLList;
		vector<CDLLObject*>::const_iterator	iterDll;
		wstring	strDllName;
		wstring	strPeriod;
		wchar_t	szPeriod[16] = {0};
		wsprintf(szPeriod, L"%d", nPeriodicity);
		strPeriod = szPeriod;
		for (iterDll=vecDll.begin(); iterDll!=vecDll.end(); ++iterDll)
		{
			if ((*iterDll)->GetDllThreadName() == strThreadName)
			{
				strDllName = (*iterDll)->GetDllName().GetString();
				m_pDataAccess->InsertLogicParameters(strDllName, strDllName, L"2", strPeriod, _T(""), _T(""), _T(""));
			}
		}

		CString strPeriodVal(strPeriod.c_str());
		CString strShow;
		strShow = strThreadNameAll.Left(strThreadNameAll.ReverseFind('(') + 1);
		strShow += strPeriodVal;
		strShow += _T(")");
		m_wndFileView.SetItemText(m_hCurSelItem, strShow);
	}
}


void CFileView::OnUpdateMenuModifyThreadPerio(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
}

bool CFileView::ShowPageByPageID( const int nPageID,const int nElementID,const int nImageID )
{
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		return false;
	}

	if(m_pMainFrame->m_bLoadingPage)
	{
		return false;
	}
	m_pMainFrame->m_project.SetCurPageID((int)nPageID);
	CEqmDrawPage *pPage = m_pMainFrame->m_project.GetCurrentPage();
	if(pPage)
	{
		m_pMainFrame->m_nElementID = nElementID;
		m_pMainFrame->m_bCheckImageMode = true;
		if(!pPage->m_bDetailLoaded)
		{
			m_pMainFrame->m_LoadingProjectBar.ShowWindow(TRUE);
			m_pMainFrame->m_LoadingProjectBar.CenterWindow(m_pMainFrame);
			m_pMainFrame->m_LoadingProjectBar.m_StaticProgress.SetWindowTextW(L"");
			m_pMainFrame->m_LoadingProjectBar.m_ProgressCtrl.SetRange(0,15);
			m_pMainFrame->m_LoadingProjectBar.m_ProgressCtrl.SetPos(0);
			m_pMainFrame->m_pPrjSqlite->SetPage(pPage);

			m_pMainFrame->m_bLoadingPage = true;
			UINT dwThreadId = 0;
			HANDLE m_threadhandle_LoadPage = (HANDLE)_beginthreadex(NULL,
				0,
				ThreadFunc_LoadPage, 
				(LPVOID)(m_pMainFrame),
				0,
				&dwThreadId);
		}
		else
		{
			m_pMainFrame->m_DrawContext.pPage = pPage;
			m_pMainFrame->ShowCurrentPage();	
		}
		DWORD screenwidth = 0;
		DWORD screenheight = 0;

		screenwidth = m_pMainFrame->m_DrawContext.GetScreenWidth();
		screenheight = m_pMainFrame->m_DrawContext.GetScreenHeight();

		m_pMainFrame->m_DrawContext.ClearSelect();
		pView->m_PageDlg->m_rtCanvas.bottom = static_cast<long>(m_pMainFrame->m_DrawContext.m_fScreenScale*(LONG)screenheight);  //edit by hun 8.1
		pView->m_PageDlg->m_rtCanvas.right  = static_cast<long>(m_pMainFrame->m_DrawContext.m_fScreenScale*(LONG)screenwidth);
		pView->m_PageDlg->SendMessage(WM_SIZE);
		pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_SHOW);
		pView->m_PageDlg->m_dlgCanvas.ShowSelectElementByID(m_pMainFrame->m_nElementID);
		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);

	}
}

void CFileView::OnMenuTemplatePageImport()
{
	CImportTemplatePageDlg dlg;
	dlg.m_hItem = m_hSelectedItem;
	dlg.DoModal();
}


void CFileView::OnMenuPageAddtoTemplate()
{
	// TODO: Add your command handler code here
	// load page info
	ASSERT(m_pMainFrame != NULL);
	if (NULL == m_pMainFrame)
	{
		return;
	}

	if (!m_pMainFrame->m_bLoadIndependent)
	{
		m_pMainFrame->m_pPrjSqlite->LoadPictureLibSqlite();
		m_pMainFrame->m_pPrjSqlite->LoadAnimationSqlite();
		m_pMainFrame->m_pPrjSqlite->LoadPictureLibSqlite(false);
		m_pMainFrame->m_pPrjSqlite->LoadAnimationSqlite(false);
		m_pMainFrame->m_bLoadIndependent = true;
	}

	if (!m_pMainFrame->m_DrawContext.vecSelectEqmt.empty())
	{
		m_pMainFrame->m_DrawContext.ClearSelect();
	}

	//
	ASSERT(m_hSelectedItem != NULL);
	if (NULL == m_hSelectedItem)
	{
		return;
	}

	DWORD_PTR dwPageId = m_wndFileView.GetItemData(m_hSelectedItem);
	CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID((int)dwPageId);
	ASSERT(pPage != NULL);
	if (NULL == pPage)
	{
		return;
	}

	pPage->ClearAll();
	m_pMainFrame->m_pPrjSqlite->LoadPageDetails(pPage, NULL, NULL);
	pPage->m_bDetailLoaded = true;

	CTemplateDlg dlg(dwPageId, pPage);
	dlg.DoModal();
}

void CFileView::SetDeviceType(const int nDeviceType)
{
	m_nDeviceType = nDeviceType;
}


void CFileView::OnUpdateMenuGroupDel(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


void CFileView::OnUpdateMenuPageDel(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


void CFileView::OnUpdateMenuPageProperty(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


void CFileView::OnMenuCalcAdd()
{
	// TODO: 在此添加命令处理程序代码
	CCalcPointProperty dlg(TRUE);
	if (IDOK == dlg.DoModal())
	{
		int nCalcStat = 0;
		CalcModuleInfo info;
		info.strName = dlg.m_strName;
		info.nPeriod = dlg.m_nPeriod;
		info.nRunState = nCalcStat;

		(m_pPrjSql->m_vecCalcModuleInfo).push_back(info);
		HTREEITEM hThread = InsertNodeThread(dlg.m_strName, root_calc, nCalcStat);
		SetTreeCalcItemText(hThread, info);
		m_wndFileView.Expand(m_hRootCalc, TVE_EXPAND);

		m_pPrjSql->InsertOneIntoCalcMod(dlg.m_strName, dlg.m_nPeriod, (bool)nCalcStat, m_pMainFrame->m_bSyncDec);
	}
}


void CFileView::OnMenuCalcDel()
{
	// TODO: 在此添加命令处理程序代码
	CString strSelName = GetCurSelTreeNodeName(m_hCurSelItem);
	strSelName = strSelName.Left(strSelName.Find(_T("(")));

	CString strShow;
	strShow.Format(_T("确认%s删除？"), strSelName);
	if (IDYES != MessageBox(strShow, _T("提示"), MB_YESNO | MB_ICONWARNING))
	{
		return;
	}
	m_wndFileView.DeleteItem(m_hCurSelItem);

	BOOL bFind = FALSE;
	vector<CalcModuleInfo>& vecCalc = m_pPrjSql->m_vecCalcModuleInfo;
	vector<CalcModuleInfo>::const_iterator	iter;
	for (iter=vecCalc.begin(); iter!=vecCalc.end(); iter++)
	{
		if (strSelName == iter->strName)
		{
			vecCalc.erase(iter);
			bFind = TRUE;
			break;
		}
	}

	if (bFind)
	{
		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
		ASSERT(pView != NULL);
		if (pView != NULL)
		{
			pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);
		}

		m_pPrjSql->DeleteOneFromCalcMod(strSelName, m_pMainFrame->m_bSyncDec);
	}
}


void CFileView::OnMenuCalcEdit()
{
	// TODO: 在此添加命令处理程序代码
	CString strSelName = GetCurSelTreeNodeName(m_hCurSelItem);
	strSelName = strSelName.Left(strSelName.Find(_T("(")));
	int nPeriod = 0;

	vector<CalcModuleInfo>& vecCalc = m_pPrjSql->m_vecCalcModuleInfo;
	vector<CalcModuleInfo>::iterator	iter;
	for (iter=vecCalc.begin(); iter!=vecCalc.end(); iter++)
	{
		if (strSelName == iter->strName)
		{
			nPeriod = iter->nPeriod;
			break;
		}
	}

	CCalcPointProperty dlg(FALSE);
	dlg.m_strName = strSelName;
	dlg.m_nPeriod = nPeriod;
	if (IDOK == dlg.DoModal())
	{
		for (iter=vecCalc.begin(); iter!=vecCalc.end(); iter++)
		{
			if (strSelName == iter->strName)
			{
				iter->strName = dlg.m_strName;
				iter->nPeriod = dlg.m_nPeriod;
				break;
			}
		}

		SetTreeCalcItemText(m_hCurSelItem, *iter);
		m_pPrjSql->UpdateCalcMod(dlg.m_strName, dlg.m_nPeriod, m_pMainFrame->m_bSyncDec);
	}
}


void CFileView::OnMenuCalcRun()
{
	// TODO: 在此添加命令处理程序代码
	CString strSelName = GetCurSelTreeNodeName(m_hCurSelItem);
	strSelName = strSelName.Left(strSelName.Find(_T("(")));

	vector<CalcModuleInfo>& vecCalc = m_pPrjSql->m_vecCalcModuleInfo;
	vector<CalcModuleInfo>::iterator	iter;
	for (iter=vecCalc.begin(); iter!=vecCalc.end(); iter++)
	{
		if (strSelName == iter->strName)
		{
			iter->nRunState = 1;
			SetTreeCalcItemImage(m_hCurSelItem, 1);
			SetTreeCalcItemText(m_hCurSelItem, *iter);

			m_pPrjSql->EnableCalcMod(strSelName, true, m_pMainFrame->m_bSyncDec);
			break;
		}
	}
}


void CFileView::OnMenuCalcStop()
{
	// TODO: 在此添加命令处理程序代码
	CString strSelName = GetCurSelTreeNodeName(m_hCurSelItem);
	strSelName = strSelName.Left(strSelName.Find(_T("(")));

	vector<CalcModuleInfo>& vecCalc = m_pPrjSql->m_vecCalcModuleInfo;
	vector<CalcModuleInfo>::iterator	iter;
	for (iter=vecCalc.begin(); iter!=vecCalc.end(); iter++)
	{
		if (strSelName == iter->strName)
		{
			iter->nRunState = 0;
			SetTreeCalcItemImage(m_hCurSelItem, 0);
			SetTreeCalcItemText(m_hCurSelItem, *iter);

			m_pPrjSql->EnableCalcMod(strSelName, false, m_pMainFrame->m_bSyncDec);
			break;
		}
	}
}

void CFileView::OnMenuTacticsExport()
{
	if (NULL == m_pMainFrame)
	{
		m_pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	}
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		return;
	}

	Json::Value jsValRoot;
	Json::Value jsValThread;
	Json::Value jsValDllItem;
	Json::Value jsValCfg;
	Json::StyledWriter jsSW;

	vector<CLogicDllThread*> vecTactics = pView->m_PageDlg->m_paneArbiter.m_pLogicManager->m_vecDllThreadList;
	vector<CLogicDllThread*>::const_iterator	iterThread;
	vector<vec_wstr>::const_iterator			iterLogic;
	int nCnt = 0;

	jsValRoot.clear();
	USES_CONVERSION;
	for (iterThread=vecTactics.begin(); iterThread!=vecTactics.end(); iterThread++,nCnt++)
	{
		jsValThread.clear();
		jsValThread["name"] = T2A((*iterThread)->GetName().c_str());
		jsValThread["cycle"] = static_cast<int>((*iterThread)->GetDllObject(0)->GetTimeSpan());
		jsValThread["dllList"].clear();

		int nDllSize = (*iterThread)->GetDllCount();
		for (int i=0; i<nDllSize; i++)
		{
			CDLLObject* pDll = (*iterThread)->GetDllObject(i);
			if (NULL == pDll)
			{
				continue;
			}

			jsValDllItem.clear();
			jsValDllItem["name"] = T2A(pDll->GetDllName());
			jsValDllItem["nameOrig"] = T2A(pDll->GetDllOriginName());
			jsValDllItem["time"] = T2A(pDll->GetDllImportTime());
			jsValDllItem["author"] = T2A(pDll->GetDllAuthor());
			jsValDllItem["version"] = T2A(pDll->GetDllVersion());
			jsValDllItem["desc"] = T2A(pDll->GetDllDescription());
			jsValDllItem["inputList"].clear();
			jsValDllItem["outputList"].clear();

			vector<vec_wstr> vecInput;
			pView->m_PageDlg->m_paneArbiter.m_pLogicManager->GetInputParameterfromDB(pDll->GetDllName(), vecInput);
			for (iterLogic=vecInput.begin(); iterLogic!=vecInput.end(); iterLogic++)
			{
				jsValCfg.clear();
				jsValCfg.append(T2A((*iterLogic)[0].c_str()));
				jsValCfg.append(T2A((*iterLogic)[1].c_str()));
				jsValCfg.append(T2A((*iterLogic)[2].c_str()));
				jsValCfg.append(T2A((*iterLogic)[3].c_str()));
				jsValDllItem["inputList"].append(jsValCfg);
			}

			vector<vec_wstr> vecOutput;
			pView->m_PageDlg->m_paneArbiter.m_pLogicManager->GetOutputParameterfromDB(pDll->GetDllName(), vecOutput);
			for (iterLogic=vecOutput.begin(); iterLogic!=vecOutput.end(); iterLogic++)
			{
				jsValCfg.clear();
				jsValCfg.append(T2A((*iterLogic)[0].c_str()));
				jsValCfg.append(T2A((*iterLogic)[1].c_str()));
				jsValCfg.append(T2A((*iterLogic)[2].c_str()));
				jsValCfg.append(T2A((*iterLogic)[3].c_str()));
				jsValCfg.append(T2A((*iterLogic)[4].c_str()));
				jsValDllItem["outputList"].append(jsValCfg);
			}

			jsValThread["dllList"].append(jsValDllItem);
		}
		jsValRoot["threads"].append(jsValThread);
	}

	size_t nBufSize = strlen(jsSW.write(jsValRoot).c_str()) + 1;
	char* pOutBuf = new char[nBufSize];
	if (NULL == pOutBuf)
	{
		MessageBox(_T("导出失败！"), _T("提示"), MB_OK|MB_ICONERROR);
		return;
	}
	memset(pOutBuf, 0, nBufSize);
	strcpy_s(pOutBuf, nBufSize, jsSW.write(jsValRoot).c_str());

	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("Text File (*.txt)|*.txt|")_T("All Files (*.*)|*.*||"), NULL);
	if (IDOK == dlg.DoModal())
	{
		CString strName = dlg.GetFileName();
		if (0 == strName.GetLength())
		{
			return;
		}
		CString strPathName = dlg.GetPathName();
		if (PathFileExists(strPathName))
		{
			if (IDNO == MessageBox(_T("文件存在，是否覆盖？"), _T("提示"), MB_YESNO|MB_ICONEXCLAMATION))
			{
				SAFE_DELETE_ARRAY(pOutBuf);
				return;
			}
		}

		CFile file;
		file.Open(strPathName, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
		file.Write(pOutBuf, nBufSize);         
		file.Flush();
		MessageBox(_T("导出成功！"), _T("提示"), MB_OK|MB_ICONASTERISK);
	}
	SAFE_DELETE_ARRAY(pOutBuf);
}

void CFileView::OnMenuTacticsImport()
{
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("Text File (*.txt)|*.txt|")_T("All Files (*.*)|*.*||"), NULL);
	if (IDOK == dlg.DoModal())
	{
		CString strName = dlg.GetFileName();
		if (0 == strName.GetLength())
		{
			return;
		}
		CString strPathName = dlg.GetPathName();
		CFile file;
		file.Open(strPathName, CFile::modeRead | CFile::shareExclusive);

		size_t nSize = file.GetLength() + 1;
		char* pBuf = new char[nSize];
		if (NULL == pBuf)
		{
			MessageBox(_T("导入失败！"), _T("提示"), MB_OK|MB_ICONERROR);
			return;
		}
		memset(pBuf, 0, nSize);
		file.Read(pBuf, nSize);
		file.Close();

		ifstream ifs;
		Json::Value jsValRoot;
		Json::Reader jsReader;
		if (!jsReader.parse(pBuf, jsValRoot))
		{
			return;
		}
		ConvertJsonToThreadInfo(jsValRoot);
		SAFE_DELETE_ARRAY(pBuf);
		MessageBox(_T("导入完成！"), _T("提示"), MB_OK|MB_ICONASTERISK);
	}
}

void CFileView::OnUpdateMenuTacticsExport(CCmdUI *pCmdUI)
{
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnUpdateMenuTacticsImport(CCmdUI *pCmdUI)
{
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnMenuCalcExport()
{
	MessageBox(_T("Calc Export"));
}

void CFileView::OnMenuCalcImport()
{
	MessageBox(_T("Calc Import"));
}

void CFileView::OnUpdateMenuCalcxport(CCmdUI *pCmdUI)
{
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnUpdateMenuCalcmport(CCmdUI *pCmdUI)
{
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


void CFileView::OnMenuControlledPointSearch()
{
	CFindControlledPointSearchInThreadDlg* dlg = (CFindControlledPointSearchInThreadDlg *)m_FindControlledPointSearchInThreadDlg;
	if(dlg == NULL)
	{
		dlg = new CFindControlledPointSearchInThreadDlg(); 
		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
		CBEOPLogicManager* pLogic = pView->m_PageDlg->m_paneArbiter.m_pLogicManager;
		vector<CLogicDllThread *> logicList = pLogic->m_vecDllThreadList;
		dlg->Set_m_pLogicManager(pLogic);
		dlg->Set_m_logicList(logicList);
		dlg->SetCFileView(this);
		dlg->Create(IDD_CONTROLLED_POINT_SERACH,this);
		dlg->SetWindowPos(NULL,1000,100,0,0,SWP_NOSIZE);
	}
	dlg->ShowWindow(SW_SHOW);
}

void CFileView::OnUpdateMenuControlledPointSearch(CCmdUI *pCmdUI)
{
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnMenuSearchStrategyInTimeAgeRun()
{
	CSearchStrategyInTimeAgeRunDlg dlg;
	dlg.DoModal();
}

void CFileView::OnUpdateMenuSearchStrategyInTimeAgeRun(CCmdUI *pCmdUI)
{
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnMenuSearchStrategy()
{
	CSearchStrategyDlg* dlg = dynamic_cast<CSearchStrategyDlg *>(m_SearchStrategyDlg);
	if(dlg == NULL)
	{
		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
		dlg = new CSearchStrategyDlg(pView->m_PageDlg->m_paneArbiter.m_pLogicManager, &pView->m_PageDlg->m_paneArbiter.m_pLogicManager->m_vecDllThreadList, this, m_hRootTact); 
		dlg->Create(IDD_DIALOG_SEARCH_STRATEGY,this);
		dlg->SetWindowPos(NULL,1000,100,0,0,SWP_NOSIZE);
	}
	dlg->ShowWindow(SW_SHOW);
}

void CFileView::OnUpdateMenuSearchStrategy(CCmdUI *pCmdUI)
{
	if (1 == m_nDeviceType)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::ShowPageWnd( BOOL bShow )
{
	ShowPane(bShow,FALSE,TRUE);
}

void CFileView::SetGroupName()
{
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	int nGroupId =  (int)m_wndFileView.GetItemData(hItem);
	CTreeGroupSettingDlg dlg;
	dlg.m_strGroupName = m_wndFileView.GetItemText(hItem);
	if (IDOK == dlg.DoModal())
	{
		CString strNewName(dlg.m_strGroupName);
		m_pMainFrame->m_project.SetGroupNameByID(nGroupId, strNewName.GetString());
		m_wndFileView.SetItemText(hItem, strNewName);
	}
}

int CFileView::GetThreadPeriodicity(const CString& strName)
{
	int nPeriodicity = 0;
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(NULL != pView);
	if (NULL == pView)
	{
		return nPeriodicity;
	}

	vector<CLogicDllThread*>& vecThread = pView->m_PageDlg->m_paneArbiter.m_pLogicManager->m_vecDllThreadList;
	vector<CLogicDllThread*>::const_iterator iterThread;
	for (iterThread=vecThread.begin(); iterThread!=vecThread.end(); ++iterThread)
	{
		if ((*iterThread)->GetName() == strName.GetString())
		{	// find it
			CDLLObject* pDllObj = (*iterThread)->GetDllObject(0);
			//ASSERT(NULL != pDllObj);
			if (NULL != pDllObj)
			{
				nPeriodicity = pDllObj->GetTimeSpan();
			}
			break;
		}
	}

	return nPeriodicity;
}

void CFileView::SetTreeThreadItemImage(const HTREEITEM hItem, const int nValue)
{
	if (NULL == hItem)
	{
		return;
	}

	int nImage = 41;
	int nImageSel = 9;
	switch (nValue)
	{
	case -1:
		nImage = 41;
		break;
	case 0:
		nImage = 40;
		break;
	case 1:
		nImage = 39;
		break;
	default:
		nImage = 41;
		break;
	}
	m_wndFileView.SetItemImage(hItem, nImage, nImageSel);
}

void CFileView::SetTreeCalcItemImage(const HTREEITEM hItem, const int nValue)
{
	if (NULL == hItem)
	{
		return;
	}

	int nImage = 46;
	int nImageSel = 43;
	switch (nValue)
	{
	case -1:
		nImage = 46;
		break;
	case 0:
		nImage = 45;
		break;
	case 1:
		nImage = 44;
		break;
	default:
		nImage = 46;
		break;
	}
	m_wndFileView.SetItemImage(hItem, nImage, nImageSel);
}

void CFileView::SetTreeCalcItemText(const HTREEITEM hItem, const CalcModuleInfo& item)
{
	if (NULL == hItem)
	{
		return;
	}

	CString	strShowStat;
	CString strShowName;

	strShowStat = ((0 == item.nRunState) ? _T("Off") : _T("On"));
	strShowName.Format(_T("%s(%s)(%d)"), item.strName, strShowStat, item.nPeriod);
	m_wndFileView.SetItemText(hItem, strShowName);
}

void CFileView::ConvertJsonToThreadInfo(const Json::Value& jsRoot)
{
	if (NULL == m_pMainFrame)
	{
		m_pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
		if (NULL == m_pMainFrame)
		{
			return;
		}
	}
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		return;
	}
	CBEOPLogicManager* pLogic = pView->m_PageDlg->m_paneArbiter.m_pLogicManager;
	ASSERT(pLogic != NULL);
	if (NULL == pLogic)
	{
		return;
	}

	//Json::Value jsValDllItem;
	//Json::Value jsValCfg;

	vector<CLogicDllThread*> vecTactics = pLogic->m_vecDllThreadList;
	vector<CLogicDllThread*>::iterator	iterThread;
	vector<vec_wstr>::const_iterator	iterLogic;
	int nCnt = 0;
	CString strDllFlag(_T(".dll"));
	BOOL bChange = FALSE;
	BOOL bInsert = FALSE;

	vector<CString> vecSelDllName;		// 策略名+".dll"
	vector<CString> vecOriginDllName;	// dll名（不含".dll"）
	vector<CString> vecSelDllPath;		// dll全路径+名称

	Json::Value jsInput;
	Json::Value jsOutput;

	// add create part
	size_t nJsSize = JsonGetThreadSize(jsRoot);
	for (int m=0; m<nJsSize; m++)
	{
		wstring wThreadName = JsonGetThreadName(jsRoot, m);
		int nCycle = JsonGetThreadCycle(jsRoot, m);
		bInsert = FALSE;
		vecSelDllName.clear();
		vecOriginDllName.clear();
		vecSelDllPath.clear();

		CLogicDllThread* pDLLThread = vecTactics.at(m);
		if (NULL != pDLLThread)
		{	// load input/output info
			pLogic->LoadDllThreadRelationship(pDLLThread);
		}

		Json::Value jsDlLst = jsRoot["threads"][m]["dllList"];
		size_t nDllSize = jsDlLst.size();
		for (int n=0; n<nDllSize; n++)
		{
			CString strDllNameNew = JsonGetDllName(jsRoot, m, n);
			CString strDllNameOrgNew = JsonGetDllNameOrig(jsRoot, m, n);
			CString	strTimeNew = JsonGetDllTime(jsRoot, m, n);
			CString strAuthorNew = JsonGetDllAuthor(jsRoot, m, n);
			CString strVerNew = JsonGetDllVersion(jsRoot, m, n);
			CString strDescNew = JsonGetDllDesc(jsRoot, m, n);

			// 如有则更新，如没有则新增
			CDLLObject* pDll = GetDllObjectItem(vecTactics, wThreadName, strDllNameOrgNew);
			if (NULL == pDll)
			{	// insert
				bChange = TRUE;

				CString strDllPath(m_strLogicPath + strDllNameOrgNew + _T(".dll"));
				if (PathFileExists(strDllPath))
				{
					bInsert = TRUE;
					vecSelDllName.push_back(strDllNameOrgNew + _T(".dll"));
					vecOriginDllName.push_back(strDllNameOrgNew);
					vecSelDllPath.push_back(strDllPath);
				}
			}
			else
			{	// update
				CString strDllNameOld = pDll->GetDllName();
				CString strDllNameOrigOld = pDll->GetDllOriginName();
				CString strTimeOld = pDll->GetDllImportTime();
				CString strAuthorOld = pDll->GetDllAuthor();
				CString strVerOld = pDll->GetDllVersion();
				CString strDescOld = pDll->GetDllDescription();

				if (strDllNameNew != strDllNameOld)
				{
					pDll->SetDllName(strDllNameNew);

					CString strOld = strDllNameOld.TrimRight(strDllFlag);
					CString strNew = strDllNameNew.TrimRight(strDllFlag);

					pLogic->ExitDllByName(strOld + strDllFlag);
					pLogic->UpdateTacticName(strOld, strNew);
					pLogic->UpdateTacticParamConfig(strOld, strNew);
					bChange = TRUE;
				}
				if (strDllNameOrgNew != strDllNameOrigOld
					|| strTimeNew != strTimeOld
					|| strAuthorNew != strAuthorOld
					|| strVerNew != strVerOld
					|| strDescNew != strDescOld)
				{
					pDll->SetDllOriginName(strDllNameOrgNew);
					pDll->SetDllImportTime(strTimeNew);
					pDll->SetDllAuthor(strAuthorNew);
					pDll->SetDllVersion(strVerNew);
					pDll->SetDllDescription(strDescNew);
					pLogic->UpdateTacticInfo(strDllNameNew, strDllNameOrgNew, strTimeNew, strAuthorNew, strVerNew, strDescNew);
					bChange = TRUE;
				}


				jsInput.clear();
				jsInput = jsDlLst[n]["inputList"];
				size_t nInputSize = jsInput.size();
				for (int nRow=0; nRow<nInputSize; nRow++)
				{
					wstring wstrName = JsonGetDllInputListValue(jsRoot, m, n, nRow, 0);
					wstring wstrVal = JsonGetDllInputListValue(jsRoot, m, n, nRow, 1);
					wstring wstrType = JsonGetDllInputListValue(jsRoot, m, n, nRow, 2);
					wstring wstrDesc = JsonGetDllInputListValue(jsRoot, m, n, nRow, 3);

					wstring wstrNameMem = GetDllInputListValue(vecTactics, wThreadName, strDllNameOrigOld, nRow, 0);
					wstring wstrValMem = GetDllInputListValue(vecTactics, wThreadName, strDllNameOrigOld, nRow, 1);
					wstring wstrTypeMem = GetDllInputListValue(vecTactics, wThreadName, strDllNameOrigOld, nRow, 2);
					wstring wstrDescMem = GetDllInputListValue(vecTactics, wThreadName, strDllNameOrigOld, nRow, 3);

					if ((wstrName != wstrNameMem && wstrNameMem != L"")
						|| (wstrVal != wstrValMem && wstrValMem != L"")
						|| (wstrType != wstrTypeMem && wstrTypeMem != L"")
						|| (wstrDesc != wstrDescMem && wstrDescMem != L"")
						)
					{
						CString strName;
						CString strVal;
						CString strType;
						strName.Format(_T("%s"), wstrName.c_str());
						strVal.Format(_T("%s"), wstrVal.c_str());
						strType.Format(_T("%s"), wstrType.c_str());
						pLogic->SaveParametertoDB(0, strDllNameNew, strName, strVal, strType);
						bChange = TRUE;
					}
				}

				jsOutput.clear();
				jsOutput = jsDlLst[n]["outputList"];
				size_t nOutputSize = jsOutput.size();
				for (int nRow=0; nRow<nOutputSize; nRow++)
				{
					wstring wstrName = JsonGetDllOutputListValue(jsRoot, m, n, nRow, 0);
					wstring wstrVal = JsonGetDllOutputListValue(jsRoot, m, n, nRow, 1);
					wstring wstrType = JsonGetDllOutputListValue(jsRoot, m, n, nRow, 2);
					wstring wstrDesc = JsonGetDllOutputListValue(jsRoot, m, n, nRow, 3);
					wstring wstrCond = JsonGetDllOutputListValue(jsRoot, m, n, nRow, 4);

					wstring wstrNameMem = GetDllOutputListValue(vecTactics, wThreadName, strDllNameOrigOld, nRow, 0);
					wstring wstrValMem = GetDllOutputListValue(vecTactics, wThreadName, strDllNameOrigOld, nRow, 1);
					wstring wstrTypeMem = GetDllOutputListValue(vecTactics, wThreadName, strDllNameOrigOld, nRow, 2);
					wstring wstrDescMem = GetDllOutputListValue(vecTactics, wThreadName, strDllNameOrigOld, nRow, 3);
					wstring wstrCondMem = GetDllOutputListValue(vecTactics, wThreadName, strDllNameOrigOld, nRow, 4);//

					if ((wstrName != wstrNameMem && wstrNameMem != L"")
						|| (wstrVal != wstrValMem && wstrValMem != L"")
						|| (wstrType != wstrTypeMem && wstrTypeMem != L"")
						|| (wstrDesc != wstrDescMem && wstrDescMem != L"")
						|| (wstrCond != wstrCondMem && wstrCondMem != L"")
						)
					{
						CString strName;
						CString strVal;
						CString strType;
						CString	strCond;
						strName.Format(_T("%s"), wstrName.c_str());
						strVal.Format(_T("%s"), wstrVal.c_str());
						strType.Format(_T("%s"), wstrType.c_str());
						strCond.Format(_T("%s"), wstrCond.c_str());
						pLogic->SaveParametertoDB(1, strDllNameNew, strName, strVal, strType, strCond);
						bChange = TRUE;
					}
				}
			}
		}
		if (bInsert)
		{
			CString strThName;
			strThName.Format(_T("%s"), wThreadName.c_str());
			pLogic->LoadAndSaveDll(vecSelDllPath, vecSelDllName, vecOriginDllName, strThName);
		}
	}

	if (bChange)
	{
		pView->m_PageDlg->m_paneArbiter.RefreshDLLsOfThread();
	}

	return;


	// edit part
	for (iterThread=vecTactics.begin(); iterThread!=vecTactics.end(); iterThread++,nCnt++)
	{
		// thread info(name,cycle) not support modify now
		//(*iterThread)->SetName(JsonGetThreadName(jsRoot, nCnt));

		int nDllSize = (*iterThread)->GetDllCount();
		for (int i=0; i<nDllSize; i++)
		{
			CDLLObject* pDll = (*iterThread)->GetDllObject(i);
			if (NULL == pDll)
			{
				continue;
			}

			CString strDllNameOld = pDll->GetDllName();
			CString strDllNameOrigOld = pDll->GetDllOriginName();
			CString strDllNameNew = JsonGetDllName(jsRoot, nCnt, i);
			CString	strDllNameOrigNew = JsonGetDllNameOrig(jsRoot, nCnt, i);
			if (strDllNameOld != strDllNameNew)
			{
				pDll->SetDllName(strDllNameNew);

				CString strOld = strDllNameOld.TrimRight(strDllFlag);
				CString strNew = strDllNameNew.TrimRight(strDllFlag);

				pLogic->ExitDllByName(strOld + strDllFlag);
				pLogic->UpdateTacticName(strOld, strNew);
				pLogic->UpdateTacticParamConfig(strOld, strNew);
				bChange = TRUE;
			}
			if (strDllNameOrigOld != strDllNameOrigNew)
			{
				pDll->SetDllOriginName(strDllNameOrigNew);
				pLogic->UpdateTacticNameOriginal(strDllNameOrigOld, strDllNameOrigNew);
				bChange = TRUE;
			}

			int nRow = 0;
			vector<vec_wstr> vecInput;
			pLogic->GetInputParameterfromDB(pDll->GetDllName(), vecInput);
			for (iterLogic=vecInput.begin(); iterLogic!=vecInput.end(); iterLogic++,nRow++)
			{
				wstring wstrVName = JsonGetDllInputListValue(jsRoot, nCnt, i, nRow, 0);
				wstring wstrPName = JsonGetDllInputListValue(jsRoot, nCnt, i, nRow, 1);
				wstring wstrPType = JsonGetDllInputListValue(jsRoot, nCnt, i, nRow, 2);
				wstring wstrVExplain = JsonGetDllInputListValue(jsRoot, nCnt, i, nRow, 3);
				if ((*iterLogic)[0] != wstrVName || (*iterLogic)[1] != wstrPName || (*iterLogic)[2] != wstrPType)
				{
					CString strVName;
					CString strPName;
					CString strPType;
					strVName.Format(_T("%s"), wstrVName.c_str());
					strPName.Format(_T("%s"), wstrPName.c_str());
					strPType.Format(_T("%s"), wstrPType.c_str());
					pLogic->SaveParametertoDB(0, strDllNameNew, strVName, strPName, strPType);
					bChange = TRUE;
				}
			}

			nRow = 0;
			vector<vec_wstr> vecOutput;
			pLogic->GetOutputParameterfromDB(pDll->GetDllName(), vecOutput);
			for (iterLogic=vecOutput.begin(); iterLogic!=vecOutput.end(); iterLogic++,nRow++)
			{
				wstring wstrVName = JsonGetDllOutputListValue(jsRoot, nCnt, i, nRow, 0);
				wstring wstrPName = JsonGetDllOutputListValue(jsRoot, nCnt, i, nRow, 1);
				wstring wstrPType = JsonGetDllOutputListValue(jsRoot, nCnt, i, nRow, 2);
				wstring wstrVExplain = JsonGetDllOutputListValue(jsRoot, nCnt, i, nRow, 3);
				wstring wstrCondition = JsonGetDllOutputListValue(jsRoot, nCnt, i, nRow, 4);
				if ((*iterLogic)[0] != wstrVName || (*iterLogic)[1] != wstrPName || (*iterLogic)[2] != wstrPType)
				{
					CString strVName;
					CString strPName;
					CString strPType;
					CString	strCond;
					strVName.Format(_T("%s"), wstrVName.c_str());
					strPName.Format(_T("%s"), wstrPName.c_str());
					strPType.Format(_T("%s"), wstrPType.c_str());
					strCond.Format(_T("%s"), wstrCondition.c_str());
					pLogic->SaveParametertoDB(1, strDllNameNew, strVName, strPName, strPType, strCond);
					bChange = TRUE;
				}
			}
		}
	}

	if (bChange)
	{
		pView->m_PageDlg->m_paneArbiter.RefreshDLLsOfThread();
	}

	return;

	Json::Value jsValThreads = jsRoot["threads"];
	Json::Value jsValDllLst;
	Json::Value jsValInput;
	Json::Value jsValOutput;
	size_t nSize = jsValThreads.size();
	for (size_t i=0; i<nSize; i++)
	{
		string threadName = jsValThreads[i]["name"].asString();
		int cycle = jsValThreads[i]["cycle"].asInt();

		jsValDllLst.clear();
		jsValDllLst = jsValThreads[i]["dllList"];
		size_t nDllSize = jsValDllLst.size();
		for (size_t j=0; j<nDllSize; j++)
		{
			string name = jsValDllLst[j]["name"].asString();
			string nameOrig = jsValDllLst[j]["nameOrig"].asString();

			jsValInput.clear();
			jsValInput = jsValDllLst[j]["inputList"];
			size_t nInputSize = jsValInput.size();

			jsValOutput.clear();
			jsValOutput = jsValDllLst[j]["outputList"];
			// ...
		}
	}
}

size_t CFileView::JsonGetThreadSize(const Json::Value& jsRoot)
{
	return jsRoot["threads"].size();
}

wstring CFileView::JsonGetThreadName(const Json::Value& jsRoot, int nCnt)
{
	Json::Value jsValThreads = jsRoot["threads"];
	string threadName = jsValThreads[nCnt]["name"].asString();

	USES_CONVERSION;
	wstring wstrName(A2T(threadName.c_str()));
	return wstrName;
}

int CFileView::JsonGetThreadCycle(const Json::Value& jsRoot, int nCnt)
{
	Json::Value jsValThreads = jsRoot["threads"];
	int nCycle = jsValThreads[nCnt]["cycle"].asInt();
	return nCycle;
}

CString CFileView::JsonGetDllName(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt)
{
	Json::Value jsValThreads = jsRoot["threads"];
	Json::Value jsValDllLst = jsValThreads[nThreadCnt]["dllList"];
	string dllName = jsValDllLst[nDllCnt]["name"].asString();

	USES_CONVERSION;
	CString strName;
	strName.Format(_T("%s"), A2T(dllName.c_str()));
	return strName;
}

CString CFileView::JsonGetDllNameOrig(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt)
{
	Json::Value jsValThreads = jsRoot["threads"];
	Json::Value jsValDllLst = jsValThreads[nThreadCnt]["dllList"];
	string dllName = jsValDllLst[nDllCnt]["nameOrig"].asString();

	USES_CONVERSION;
	CString strName;
	strName.Format(_T("%s"), A2T(dllName.c_str()));
	return strName;
}

CString	CFileView::JsonGetDllTime(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt)
{
	Json::Value jsValThreads = jsRoot["threads"];
	Json::Value jsValDllLst = jsValThreads[nThreadCnt]["dllList"];
	string dllTime = jsValDllLst[nDllCnt]["time"].asString();

	USES_CONVERSION;
	CString strTime;
	strTime.Format(_T("%s"), A2T(dllTime.c_str()));
	return strTime;
}

CString	CFileView::JsonGetDllAuthor(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt)
{
	Json::Value jsValThreads = jsRoot["threads"];
	Json::Value jsValDllLst = jsValThreads[nThreadCnt]["dllList"];
	string dllAuthor = jsValDllLst[nDllCnt]["author"].asString();

	USES_CONVERSION;
	CString strAuthor;
	strAuthor.Format(_T("%s"), A2T(dllAuthor.c_str()));
	return strAuthor;
}

CString	CFileView::JsonGetDllVersion(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt)
{
	Json::Value jsValThreads = jsRoot["threads"];
	Json::Value jsValDllLst = jsValThreads[nThreadCnt]["dllList"];
	string dllVer = jsValDllLst[nDllCnt]["version"].asString();

	USES_CONVERSION;
	CString strVer;
	strVer.Format(_T("%s"), A2T(dllVer.c_str()));
	return strVer;
}

CString	CFileView::JsonGetDllDesc(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt)
{
	Json::Value jsValThreads = jsRoot["threads"];
	Json::Value jsValDllLst = jsValThreads[nThreadCnt]["dllList"];
	string dllDesc = jsValDllLst[nDllCnt]["desc"].asString();

	USES_CONVERSION;
	CString strDesc;
	strDesc.Format(_T("%s"), A2T(dllDesc.c_str()));
	return strDesc;
}

wstring CFileView::JsonGetDllInputListValue(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt, int nRow, int nCol)
{
	if (nCol > 3)
	{
		return L"";
	}

	Json::Value jsValThreads = jsRoot["threads"];
	Json::Value jsValDllLst = jsValThreads[nThreadCnt]["dllList"];
	Json::Value jsInput = jsValDllLst[nDllCnt]["inputList"];

	string value = jsInput[nRow][nCol].asString();
	USES_CONVERSION;
	wstring wstrVal = A2T(value.c_str());
	return wstrVal;
}

wstring CFileView::JsonGetDllOutputListValue(const Json::Value& jsRoot, int nThreadCnt, int nDllCnt, int nRow, int nCol)
{
	if (nCol > 4)
	{
		return L"";
	}

	Json::Value jsValThreads = jsRoot["threads"];
	Json::Value jsValDllLst = jsValThreads[nThreadCnt]["dllList"];
	Json::Value jsOutput = jsValDllLst[nDllCnt]["outputList"];

	string value = jsOutput[nRow][nCol].asString();
	USES_CONVERSION;
	wstring wstrVal = A2T(value.c_str());
	return wstrVal;
}

CDLLObject*	CFileView::GetDllObjectItem(const vector<CLogicDllThread*>& vecTactic, const wstring wstrThreadName, const CString strDllName)
{
	CDLLObject* pDllObj = NULL;
	vector<CLogicDllThread*>::const_iterator	iter;
	for (iter=vecTactic.begin(); iter!=vecTactic.end(); iter++)
	{
		if ((*iter)->GetName() == wstrThreadName)
		{
			pDllObj = (*iter)->GetDllObject(strDllName);
			break;
		}
	}
	return pDllObj;
}

wstring CFileView::GetDllInputListValue(const vector<CLogicDllThread*>& vecTactic, const wstring wstrThreadName, const CString strDllName, int nListCount, int nCol)
{
	if (nCol > 3)
	{
		return L"";
	}

	CDLLObject* pDllObj = NULL;
	vector<CLogicDllThread*>::const_iterator	iter;
	for (iter=vecTactic.begin(); iter!=vecTactic.end(); iter++)
	{
		if ((*iter)->GetName() == wstrThreadName)
		{
			pDllObj = (*iter)->GetDllObject(strDllName);
			break;
		}
	}
	if (NULL == pDllObj)
	{
		return L"";
	}
	if ((pDllObj->m_vecCurDllInputParameterList).size() <= 0)
	{
		return L"";
	}
	CLogicParameter* pParm = (pDllObj->m_vecCurDllInputParameterList).at(nListCount);
	if (NULL == pParm)
	{
		return L"";
	}

	wstring wstrVal;
	switch (nCol)
	{
	case 0:
		wstrVal = pParm->GetName();
		break;
	case 1:
		wstrVal = pParm->GetSettingValue();
		break;
	case 2:
		wstrVal = pParm->GetType();
		break;
	default:
		break;
	}
	return wstrVal;
}

wstring CFileView::GetDllOutputListValue(const vector<CLogicDllThread*>& vecTactic, const wstring wstrThreadName, const CString strDllName, int nListCount, int nCol)
{
	if (nCol > 4)
	{
		return L"";
	}

	CDLLObject* pDllObj = NULL;
	vector<CLogicDllThread*>::const_iterator	iter;
	for (iter=vecTactic.begin(); iter!=vecTactic.end(); iter++)
	{
		if ((*iter)->GetName() == wstrThreadName)
		{
			pDllObj = (*iter)->GetDllObject(strDllName);
			break;
		}
	}
	if (NULL == pDllObj)
	{
		return L"";
	}
	if ((pDllObj->m_vecCurDllOutputParameterList).size() <= 0)
	{
		return L"";
	}
	CLogicParameter* pParm = (pDllObj->m_vecCurDllOutputParameterList).at(nListCount);
	if (NULL == pParm)
	{
		return L"";
	}

	wstring wstrVal;
	switch (nCol)
	{
	case 0:
		wstrVal = pParm->GetName();
		break;
	case 1:
		wstrVal = pParm->GetSettingValue();
		break;
	case 2:
		wstrVal = pParm->GetType();
		break;
	case 3:
		break;
	default:
		break;
	}
	return wstrVal;
}

CUPPCServerDataAccess * CFileView::GetDataAccess()
{
	return m_pDataAccess;
}

void CFileView::ControlDebug(void)
{
	// TODO: Add your command handler code here
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	//if(pView && pView->m_PageDlg->m_pDataAccess == NULL)
	//{
	//	//if(m_pDataAccess && m_pDataAccess->IsMySQLConnected())
	//	//{
	//		pView->m_PageDlg->m_pDataAccess = m_pDataAccess;
	//		pView->m_PageDlg->InitData();
	//		return;
	//}
		if (pView &&  m_pDataAccess == NULL)
		{
			CString strPath;
			Project::Tools::GetSysPath(strPath);
			strPath += _T("\\debugtool.ini");

			TCHAR szBuf[MAX_PATH] = {0};
			GetPrivateProfileString(_T("beopdebugtool"), _T("server"), _T("127.0.0.1"), szBuf, MAX_PATH, strPath);

			CString strDbIp;
			strDbIp.Format(_T("%s"), szBuf);

			CConnectDlg dlg(szBuf);
			if (IDOK == dlg.DoModal())
			{
				USES_CONVERSION;
				string strIp;
				strIp = T2A(dlg.m_strDbIp.GetBuffer());
				dlg.m_strDbIp.ReleaseBuffer();

				if(pView==NULL)
					return;

				pView->m_PageDlg->m_paneArbiter.InitLogicMgrPointer();

				gDataBaseParam realDBParam;
				realDBParam.strDBIP = strIp;
				realDBParam.strDBName = "";
				realDBParam.strRealTimeDBName = Project::Tools::WideCharToUtf8(m_pMainFrame->GetMySQLDatabaseName().GetString());
				realDBParam.strUserName = "root";
				realDBParam.nPort = 3306;
				realDBParam.strDBIP2 = "";
				m_pDataAccess = new CUPPCServerDataAccess();
				m_pDataAccess->SetDBparam(realDBParam);
				if(!(m_pDataAccess->InitConnection()))
				{
					return;
				}

				pView->m_PageDlg->m_paneArbiter.m_pDataAccess = m_pDataAccess;
				pView->m_PageDlg->m_paneArbiter.m_pLogicManager->m_pDataAccess = m_pDataAccess;
				if (NULL == m_pMainFrame)
				{
					m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
				}
				pView->m_PageDlg->m_paneArbiter.m_pointmanager = &(m_pMainFrame->GetPointMap());

				// 刷新树中的启停状态图标和标题
				vector<pair<CString, int>> vecName;
				vector<pair<CString, int>>::const_iterator iter;
				HTREEITEM hThread = NULL;
				vecName.clear();
				pView->m_PageDlg->m_paneArbiter.InitData(vecName);
				sort(vecName.begin(),vecName.end());
				auto item_name = vecName.begin();
				for(;item_name!=vecName.end();item_name++)
				{			
					HTREEITEM hitem = FinItem_m_wndFileView(m_hRootTact,item_name->first.Left(item_name->first.Find(L"::")));
					if(hitem != NULL)
					{
						SetTreeThreadItemImage(hitem, item_name->second);
						m_wndFileView.SetItemText(hitem, item_name->first);
					}
				}
				// set flag
				SetConnectServFlag(true);

				// rename tactics node name
				CString strTacticsName;
				strTacticsName = m_wndFileView.GetItemText(m_hRootTact);
				strTacticsName = strTacticsName.Left(strTacticsName.Find(_T("::")));
				strTacticsName += _T("::在线::");
				strTacticsName += dlg.m_strDbIp;
				m_wndFileView.SetItemText(m_hRootTact, strTacticsName);

				// write info into ini file
				WritePrivateProfileString(_T("beopdebugtool"), _T("server"), dlg.m_strDbIp, strPath);
		
				//为策略参数设置窗口的列表的当前值添加下列代码，没有是只能执行一次
				pView->m_PageDlg->m_pDataAccess = m_pDataAccess;
				pView->m_PageDlg->InitData();

				UpdateData(FALSE);
			}
		}
		else if(pView && pView->m_PageDlg->m_pDataAccess == NULL)
		{
			pView->m_PageDlg->m_pDataAccess = m_pDataAccess;
			pView->m_PageDlg->InitData();
			return;
		}
	//}
}

//void CFileView::OnVpointBackup()
//{
//	// TODO: Add your command handler code here
//
//	if(m_pDataAccess==NULL)
//	{
//		AfxMessageBox(_T("请先连接core进入在线调试"));
//		return;
//	}
//
//	CWarningDlg WarnDlg;
//	WarnDlg.InitWarn(_T("警告"), _T("将当前系统实时的点值保存并覆盖db文件中的点表初值, 是否确定?"), FALSE);
//	if(WarnDlg.DoModal()!=IDOK)
//	{
//		return ;
//	}
//
//	if (m_pDataAccess != NULL)
//	{
//		vector<CRealTimeDataEntry> tempEntryList;
//		if(m_pDataAccess->ReadRealtimeData_Input(tempEntryList))
//		{
//			//save all into s3db
//			m_pMainFrame->m_pPrjSqlite->SaveBackupRealtimePointValuesStatus(tempEntryList);
//			
//		}
//
//		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
//		CBEOPLogicManager * plogicmm = pView->m_PageDlg->m_paneArbiter.m_pLogicManager;
//
//
//		std::vector<CLogicDllThread*>::const_iterator iter;
//		wstring strDllThreadName;
//		wstring strStatusOnline;
//		vector<CString> strThreadNameList;
//		vector<int>   nThreadStatusList;
//		vector<int>   nThreadTimespanList;
//		for (iter=plogicmm->m_vecDllThreadList.begin(); iter!=plogicmm->m_vecDllThreadList.end(); ++iter)
//		{
//			strDllThreadName = (*iter)->GetName();
//			if (strDllThreadName != L"")
//			{
//				int nFlag = -1;
//				if (m_pDataAccess->ReadOrCreateCoreDebugItemValue(strDllThreadName)==L"1")
//				{
//					strStatusOnline = L"::(On)";
//					nFlag = 1;
//				}
//				else
//				{
//					strStatusOnline = L"::(Off)";
//					nFlag = 0;
//				}
//
//				int nPeriod = 0;
//				CDLLObject* pDllObj = (*iter)->GetDllObject(0);
//				ASSERT(NULL != pDllObj);
//				if (NULL != pDllObj)
//				{
//					nPeriod = pDllObj->GetTimeSpan();
//				}
//				strThreadNameList.push_back(strDllThreadName.c_str());
//				nThreadStatusList.push_back(nFlag);
//				nThreadTimespanList.push_back(nPeriod);
//			}
//		}
//
//		m_pMainFrame->m_pPrjSqlite->SaveBackupLogicStatus(strThreadNameList, nThreadStatusList, nThreadTimespanList);
//
//
//
//		CString strTemp;
//		strTemp.Format(_T("成功保存%d个点的初值"), tempEntryList.size());
//
//		CWarningDlg WarnDlg2;
//		WarnDlg2.InitWarn(_T("提示"), strTemp, FALSE);
//		WarnDlg2.DoModal();
//	}
//
//}
/*
**	函数描述：备份当前虚点实时值
*/
void CFileView::OnVpointBackup()
{
	// TODO: Add your command handler code here

	if(Project::Tools::FindAppInProces(L"domcore.exe") == FALSE)
	{
		AfxMessageBox(_T("备份失败！domcore未启动动。"));
		return;
	}
	if(m_pDataAccess==NULL)
	{
		AfxMessageBox(_T("备份失败！请先连接core进入在线调试。"));
		return;
	}
	else if (m_pDataAccess != NULL)
	{
		vector<CRealTimeDataEntry> tempEntryList;
		if(m_pDataAccess->ReadRealtimeData_Input(tempEntryList))//获取实时值数据
		{
			//获取点表
			CDataPointManager point_manager = m_pMainFrame->m_pPrjSqlite->GetPointMap();
			map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
			/*通过比较提出虚拟点的点的表*/
			auto nT = tempEntryList.begin();
			vector<CRealTimeDataEntry> vpointList;
			while(nT != tempEntryList.end())
			{
				map<int, DataPointEntry>::const_iterator it = datalist.begin();
				/*bool bFindVpoint = false;*/
				for (;it != datalist.end(); it++)
				{
					const DataPointEntry& entryMap = it->second;
					string wstrToStr = Project::Tools::tstringToString(entryMap.GetShortName());
					if ( (wstrToStr == nT->GetName()) && (entryMap.GetSourceType() == L"vpoint"))//判断为虚拟点并跳出
					{
						vpointList.push_back(*nT);
						break;
					}
				}
				nT++;
			}
			/*将实时虚拟点之保存至csv文件*/
			if(vpointList.size() >0)
			{
				wstring wstrRealTime;
				Project::Tools::GetDateTime(wstrRealTime);
				CString cstrRealTime = wstrRealTime.c_str();
				/*cstrRealTime.Delete(cstrRealTime.Find(L" "));*/
				cstrRealTime.Remove(L'-');
				cstrRealTime.Remove(L' ');
				cstrRealTime.Remove(L':');
				CString cstrFileName = L"RealVpointValue";
				cstrFileName += cstrRealTime;
				CFileDialog dlg(FALSE, _T(""), cstrFileName/*_T("RealVpointValue")*/, 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
				if (dlg.DoModal() == IDOK)
				{
					CString strFilePath = dlg.GetPathName();
					CString strName = dlg.GetFileName();
					if (strName.GetLength() == 0)
					{
						AfxMessageBox(_T("文件名称不能为空"));
						return;
					}
					char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
					setlocale( LC_ALL, "chs" );

					CStdioFile filehandle;
					if(filehandle.Open(strFilePath, CFile::modeCreate|CFile::modeWrite))
					{
						/*vector<CRealTimeDataEntry> entrylist;原读取实时值位置
						m_pDataAccess->ReadRealtimeData_Input(entrylist);*/

						//filehandle.WriteString(_T("\"time\",\"pointname\",\"value\"\n"));
						CString strOut;
						wstring strTime,strName,strValue;
						for(int i=0; i<vpointList.size(); ++i)
						{
							Project::Tools::SysTime_To_String(vpointList[i].GetTimestamp(),strTime);
							Project::Tools::UTF8ToWideChar(vpointList[i].GetName(),strName);
							strValue = vpointList[i].GetValue();
							strOut.Format(_T("\"%s\",\"%s\",\"%s\"\n"),strTime.c_str(),strName.c_str(),strValue.c_str());
							filehandle.WriteString(strOut);
						}
						filehandle.Close();
						setlocale( LC_ALL, old_locale );
						AfxMessageBox(_T("导出CSV文件成功"));
					}
					else
					{
						setlocale( LC_ALL, old_locale );
						AfxMessageBox(_T("导出CSV文件失败"));
					}	
				}//END if (dlg.DoModal() == IDOK)
			}//END if(tempEntryList.size() >0) 
			else
			{
					AfxMessageBox(L"无虚拟点");
			}
		}//END if(m_pDataAccess->ReadRealtimeData_Input(tempEntryList)) 确保读取到实时值
	}//END else if (m_pDataAccess != NULL) 判断连接调试
}

//void CFileView::OnVpointReload()
//{
//	// TODO: Add your command handler code here
//
//	if(m_pDataAccess==NULL)
//	{
//		AfxMessageBox(_T("请先连接core进入在线调试"));
//		return;
//	}
//	//CReloadLogin dlg;
//	//if (dlg.DoModal()==IDOK)
//	//{
//	//	if (dlg.m_strPassWord!=_T("DOM.cloud-2016"))
//	//	{
//	//		AfxMessageBox(_T("密码错误！"));
//	//		return;
//	//	}
//	//}
//	//else
//	//{
//	//	return;
//	//}
//	CWarningDlg WarnDlg;
//	WarnDlg.InitWarn(_T("警告"), _T("将从当前db文件备份中重新初始化点表(仅vpoint类型)的初值, 是否确定?"), FALSE);
//	if(WarnDlg.DoModal()==IDOK)
//	{
//		vector<CRealTimeDataEntry> tempEntryList;
//
//		//load all from s3db
//		int nAllGoodCount = 0;
//		m_pMainFrame->m_pPrjSqlite->GetBackupRealtimePointValuesStatus(tempEntryList);
//		CDataPointManager point_manager = m_pMainFrame->m_pPrjSqlite->GetPointMap();
//		for(int i=0;i<tempEntryList.size();i++)
//		{
//			std::string strPointName = tempEntryList[i].GetName();
//			wstring wstrPointName ;
//			Project::Tools::UTF8ToWideChar(strPointName, wstrPointName);
//			DataPointEntry ptEntry = point_manager.GetEntry(wstrPointName);
//			if(ptEntry.GetSourceType()==L"vpoint")
//			{
//				m_pDataAccess->UpdatePointData(tempEntryList[i]);
//				nAllGoodCount+=1;
//			}
//		}
//
//		CString strTemp;
//		strTemp.Format(_T("成功重新初始化%d个点(仅vpoint类型)的实时值"), tempEntryList.size());
//		CWarningDlg WarnDlg2;
//		WarnDlg2.InitWarn(_T("提示"), strTemp, FALSE);
//		WarnDlg2.DoModal();
//	}
//}
void CFileView::RemoveSubStr( const string& r, string& str )
{
	int rlen = r.length();
	int strlen = str.length();

	if (rlen <= 0 || strlen <=0)
		return;
	int i,j,pos;
	for (i = 0; i < strlen; ++i) {
		pos = i;
		for (j = 0; j < rlen; ++j) {
			if (str[i] != r[j]) {
				break;
			}
			else if ( j == (rlen-1)){
				str.erase(pos,rlen);
				strlen = str.length();// After trimming, the length of string has changed.
				i = -1;
			}
			else
				++i;
		}
	}
}
/*
**	函数描述：重载虚拟点实时值
*/
void CFileView::OnVpointReload()
{
	// TODO: Add your command handler code here
	if(Project::Tools::FindAppInProces(L"domcore.exe") == FALSE)
	{
		AfxMessageBox(_T("载入失败！domcore未启动动。"));
		return;
	}
	if(m_pDataAccess==NULL)
	{
		AfxMessageBox(_T("载入失败！请先连接core进入在线调试。"));
		return;
	}
	
	else if ( m_pDataAccess != NULL)
	{
		CFileDialog dlgOpenFile(TRUE,NULL,L"*.csv",OFN_HIDEREADONLY|OFN_HIDEREADONLY,L"*.csv");
		if(dlgOpenFile.DoModal()==IDOK)
		{
			CString strFilePath = dlgOpenFile.GetPathName();	

			FILE* pfile = fopen(Project::Tools::WideCharToAnsi(strFilePath).c_str(), "r");
			if (pfile)
			{
				fseek(pfile,0,SEEK_END);
				u32 dwsize = ftell(pfile);
				rewind(pfile);
				char* filebuffer = new char[dwsize + 1];
				int nCharSize = sizeof(char);
				memset(filebuffer, 0, (dwsize+1)*nCharSize);
				size_t dwReadSize = fread(filebuffer, nCharSize, dwsize, pfile);
				memset(filebuffer+dwReadSize*nCharSize, 0, (dwsize-dwReadSize)*nCharSize);
				std::map<u32, std::string> StringMap;
				char* pBegin = filebuffer;
				char* pEnd = strchr(filebuffer, '\n');
				u32 uiIndex = 1;
				bool bFirst = true;
				while (pEnd != NULL)
				{
					std::string strbuff;
					strbuff.insert(0, pBegin, pEnd-pBegin);
					if (!strbuff.empty())
					{
						RemoveSubStr("\"",strbuff);
						StringMap[uiIndex] = strbuff;
					}
					pBegin = pEnd + 1;
					pEnd = strchr(pEnd + 1, '\n');
					++uiIndex;
				}

				delete[] filebuffer;
				fclose(pfile);

				//
				vector<CRealTimeDataEntry> entrylist;
				std::map<u32, std::string>::iterator iter = StringMap.begin();
				string strName_Utf8;
				SYSTEMTIME st;
				for (; iter != StringMap.end(); ++iter)
				{		
					int nColumn = 0;
					wstring strData = Project::Tools::AnsiToWideChar(iter->second.c_str());
					vector<wstring> vecData;
					Project::Tools::SplitStringByChar(strData.c_str(),L',',vecData);
					nColumn = vecData.size();
					if(nColumn == 3)
					{
						CRealTimeDataEntry entry;
						Project::Tools::WideCharToUtf8(vecData[1],strName_Utf8);
						Project::Tools::String_To_SysTime(Project::Tools::WideCharToAnsi(vecData[0].c_str()),st);

						entry.SetName(strName_Utf8);
						entry.SetTimestamp(st);
						entry.SetValue(vecData[2]);
						entrylist.push_back(entry);
					}
					else
					{
						continue;
					}
				}

				if(entrylist.size()<=0)
				{

					AfxMessageBox(_T("CSV解析点数为0个，请检查格式"));
				}

				//插入数据库
				vector<int> nErrorNumberList;
				if(entrylist.size() > 1000)
				{
					//几个1k插入几轮
					vector<CRealTimeDataEntry>::iterator iItemStart
						= entrylist.begin(); 
					int nCount = (int)(entrylist.size()/1000);//执行次数
					for(int i=0;i<nCount;i++)
					{
						vector<CRealTimeDataEntry> vSmall(iItemStart, iItemStart+1000);
						bool bSucceed = m_pDataAccess->UpdatePointDataList(vSmall);
						iItemStart+=1000;
						if(false == bSucceed){
							nErrorNumberList.push_back(i);
						}
						Sleep(100);//表格超过8880个会存不进去，延时一段时间保证core可以取走已经存储的点
					}

					//不够1k的最后一轮插入
					iItemStart = entrylist.begin()+(nCount*1000);
					if(entrylist.end() != iItemStart)
					{
						vector<CRealTimeDataEntry> vSmall(iItemStart, entrylist.end());
						bool bSucceed = m_pDataAccess->UpdatePointDataList(vSmall);
						if(false == bSucceed){
							nErrorNumberList.push_back(nCount);
						}
					}
					
					//弹出存储状态信息
					if(nErrorNumberList.size()>0)
					{
						if(nErrorNumberList.size() == (nCount+1)){
							AfxMessageBox(L"全部存储失败");
							return;
						}
						CString strMsg;
						for(int i=0;i<nErrorNumberList.size();i++)
						{
							CString cstrTemp;
							cstrTemp.Format(L"第%d个后1000个存储失败\r\n",nErrorNumberList[i]);
						}
						AfxMessageBox(strMsg);
					}
					else{
						AfxMessageBox(L"保存成功");
					}
				}
				else
				{
					if(m_pDataAccess->UpdatePointDataList(entrylist))
					{
						m_pDataAccess->UpdatePointBuffer(entrylist);	//更新buffer
						CString strTemp;
						strTemp.Format(_T("导入CSV文件成功，成功导入%d个点"), entrylist.size());
						AfxMessageBox(strTemp);
					}
					else
					{
						AfxMessageBox(_T("导入CSV文件失败"));
					}
				}
			}
			else
			{
				AfxMessageBox(_T("打开CSV文件失败"));
			}
		}
	}
}

void CFileView::OnUpdateVpointBackup(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pDataAccess!=NULL);
}


void CFileView::OnUpdateVpointReload(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pDataAccess!=NULL);
}


void CFileView::OnBackupLogicthreadStatus()
{
	// TODO: Add your command handler code here
	if(m_pDataAccess==NULL)
	{
		AfxMessageBox(_T("请先连接core进入在线调试"));
		return;
	}

	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("警告"), _T("将当前系统策略的开启状态保存并覆盖db文件中的状态, 是否确定?"), FALSE);
	if(WarnDlg.DoModal()!=IDOK)
	{
		return ;
	}

	if (m_pDataAccess != NULL)
	{

		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
		CBEOPLogicManager * plogicmm = pView->m_PageDlg->m_paneArbiter.m_pLogicManager;


		std::vector<CLogicDllThread*>::const_iterator iter;
		wstring strDllThreadName;
		wstring strStatusOnline;
		vector<CString> strThreadNameList;
		vector<int>   nThreadStatusList;
		vector<int>   nThreadTimespanList;
		for (iter=plogicmm->m_vecDllThreadList.begin(); iter!=plogicmm->m_vecDllThreadList.end(); ++iter)
		{
			strDllThreadName = (*iter)->GetName();
			if (strDllThreadName != L"")
			{
				int nFlag = -1;
				if (m_pDataAccess->ReadOrCreateCoreDebugItemValue(strDllThreadName)==L"1")
				{
					strStatusOnline = L"::(On)";
					nFlag = 1;
				}
				else
				{
					strStatusOnline = L"::(Off)";
					nFlag = 0;
				}

				int nPeriod = 0;
				CDLLObject* pDllObj = (*iter)->GetDllObject(0);
				ASSERT(NULL != pDllObj);
				if (NULL != pDllObj)
				{
					nPeriod = pDllObj->GetTimeSpan();
				}
				strThreadNameList.push_back(strDllThreadName.c_str());
				nThreadStatusList.push_back(nFlag);
				nThreadTimespanList.push_back(nPeriod);
			}
		}

		m_pMainFrame->m_pPrjSqlite->SaveBackupLogicStatus(strThreadNameList, nThreadStatusList, nThreadTimespanList);



		CString strTemp;
		strTemp.Format(_T("成功保存%d个策略线程的初值"), strThreadNameList.size());

		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTemp, FALSE);
		WarnDlg2.DoModal();
	}

}

HTREEITEM   CFileView::FinItem_m_wndFileView(HTREEITEM  item, CString strtext)   
{  
	HTREEITEM  hfind;  
	HTREEITEM hNextItem = item;
	//空树，直接返回NULL
	if(hNextItem ==  NULL)  
		return  NULL;  
	//遍历查找
	while(hNextItem!=NULL)  
	{  
		//当前节点即所需查找节点
		CString cstrName = m_wndFileView.GetItemText(hNextItem);
		cstrName = cstrName.Left(cstrName.Find(L":"));
		if( cstrName == strtext)  
			return hNextItem;  
		//查找当前节点的子节点
		if(m_wndFileView.ItemHasChildren(hNextItem))//如果指定的项目有子项，则返回非零值。  
		{  
			hNextItem = m_wndFileView.GetChildItem(hNextItem);//获取子项地址
			//递归调用查找子节点下节点
			hfind = FinItem_m_wndFileView(hNextItem,strtext);  
			if(hfind)  
			{  
				return  hfind;  
			}  
			else   //子节点中未发现所需节点，继续查找兄弟节点
				hNextItem = m_wndFileView.GetNextSiblingItem(m_wndFileView.GetParentItem(hNextItem));  
		}  
		else
		{   //若无子节点，继续查找兄弟节点
			hNextItem = m_wndFileView.GetNextSiblingItem(hNextItem);  
		}  
	}  
	return hNextItem;  
}

HTREEITEM CFileView::GetTactHTI()
{
	return m_hRootTact;
}

void CFileView::OnRestoreLogicthreadStatus()
{
	if(m_pDataAccess==NULL)
	{
		AfxMessageBox(_T("请先连接core进入在线调试"));
		return;
	}

	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("警告"), _T("将当前db文件中的策略状态覆盖实时的策略开启状态，可能会直接开启策略运行和关闭，是否确定?"), FALSE);
	if(WarnDlg.DoModal()!=IDOK)
	{
		return ;
	}

	if (m_pDataAccess != NULL)
	{

		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
		CBEOPLogicManager * plogicmm = pView->m_PageDlg->m_paneArbiter.m_pLogicManager;


		vector<CString> strThreadNameList;
		vector<int>   nThreadStatusList;
		vector<int>   nThreadTimespanList;
		m_pMainFrame->m_pPrjSqlite->GetBackupLogicStatus(strThreadNameList, nThreadStatusList, nThreadTimespanList);
		if((strThreadNameList.size() == 0) && (nThreadStatusList.size() == 0) && (nThreadTimespanList.size() == 0))
		{
			AfxMessageBox(L"数据库中无备份策略运行状态。");
			return ;
		}
		else if((strThreadNameList.size() != nThreadStatusList.size()) && (strThreadNameList.size() != nThreadTimespanList.size()))
		{
			AfxMessageBox(L"数据库中备份有误，策略与状态数量不对等。");
			return ;
		}
		std::vector<CLogicDllThread*>::const_iterator iter;
		wstring strDllThreadName;
		wstring strStatusOnline;
		for (iter=plogicmm->m_vecDllThreadList.begin(); iter!=plogicmm->m_vecDllThreadList.end(); ++iter)
		{
			strDllThreadName = (*iter)->GetName();
			if (strDllThreadName != L"")
			{
				bool bFound = false;
				int i =0;
				for( i=0;i<strThreadNameList.size();i++)
				{
					if(strThreadNameList[i]== strDllThreadName.c_str())
					{
						bFound = true;
						break;
					}
				}

				if(!bFound)
					continue;
				
				HTREEITEM hItem = FinItem_m_wndFileView(m_hRootTact,strDllThreadName.c_str());
				if(hItem != NULL)
				{
					if (nThreadStatusList[i]==1)
					{
						m_pDataAccess->WriteCoreDebugItemValue(strDllThreadName, L"1");

						CString strShow;
						int nPeriod = GetThreadPeriodicity(strDllThreadName.c_str());
						strShow.Format(_T("%s::(On)(%d)"), strDllThreadName.c_str(), nPeriod);
						m_wndFileView.SetItemText(hItem, strShow);
						SetTreeThreadItemImage(hItem, 1);
					}
					else
					{
						m_pDataAccess->WriteCoreDebugItemValue(strDllThreadName, L"0");

						CString strShow;
						int nPeriod = GetThreadPeriodicity(strDllThreadName.c_str());
						strShow.Format(_T("%s::(Off)(%d)"), strDllThreadName.c_str(), nPeriod);
						m_wndFileView.SetItemText(hItem, strShow);
						SetTreeThreadItemImage(hItem, 0);
					}
				}
				//m_wndFileView.UpdateWindow();
				pView->m_PageDlg->m_paneArbiter.m_pLogicManager->UpdateThreadPeriodicity(nThreadTimespanList[i], strDllThreadName.c_str());

				// 修改MySql -- unit02
				vector<CDLLObject*>& vecDll = pView->m_PageDlg->m_paneArbiter.m_pLogicManager->vecImportDLLList;
				vector<CDLLObject*>::const_iterator	iterDll;
				wstring	strDllName;
				wstring	strPeriod;
				wchar_t	szPeriod[16] = {0};
				wsprintf(szPeriod, L"%d", nThreadTimespanList[i]);
				strPeriod = szPeriod;
				
				m_pDataAccess->InsertLogicParameters(strDllThreadName.c_str(), strDllThreadName.c_str(), L"2", strPeriod, _T(""), _T(""), _T(""));
					
				
			}
		}

		
		m_wndFileView.UpdateWindow();
		CString strTemp;
		strTemp.Format(_T("成功更新%d个策略线程的初值"), strThreadNameList.size());

		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTemp, FALSE);
		WarnDlg2.DoModal();
	}
}


void CFileView::OnRestartDomcore()
{
	// TODO: Add your command handler code here

}

bool CFileView::initPageDlgDBAccess()
{
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	if(pView && pView->m_PageDlg->m_pDataAccess == NULL)
	{	
		if(m_pDataAccess && m_pDataAccess->IsMySQLConnected())
		{
			pView->m_PageDlg->m_pDataAccess = m_pDataAccess;
			pView->m_PageDlg->InitData();
			return true;
		}
		else		//重新连接数据库
		{
			CString strPath;
			Project::Tools::GetSysPath(strPath);
			strPath += _T("\\debugtool.ini");

			TCHAR szBuf[MAX_PATH] = {0};
			GetPrivateProfileString(_T("beopdebugtool"), _T("server"), _T("127.0.0.1"), szBuf, MAX_PATH, strPath);

			CString strDbIp;
			strDbIp.Format(_T("%s"), szBuf);

			CConnectDlg dlg(szBuf);
			if (IDOK == dlg.DoModal())
			{
				USES_CONVERSION;
				string strIp;
				strIp = T2A(dlg.m_strDbIp.GetBuffer());
				dlg.m_strDbIp.ReleaseBuffer();

				gDataBaseParam realDBParam;
				realDBParam.strDBIP = strIp;
				realDBParam.strDBName = "";
				realDBParam.strRealTimeDBName = Project::Tools::WideCharToUtf8(m_pMainFrame->GetMySQLDatabaseName());
				realDBParam.strUserName = "root";
				realDBParam.nPort = 3306;
				realDBParam.strDBIP2 = "";

				m_pDataAccess = new CUPPCServerDataAccess();
				m_pDataAccess->SetDBparam(realDBParam);
				if(!(m_pDataAccess->InitConnection()))
				{
					return false;
				}

				pView->m_PageDlg->m_paneArbiter.m_pDataAccess = m_pDataAccess;
				pView->m_PageDlg->m_paneArbiter.m_pLogicManager->m_pDataAccess = m_pDataAccess;

				// // 刷新树中的启停状态图标和标题
				vector<pair<CString, int>> vecName;
				vector<pair<CString, int>>::const_iterator iter;
				HTREEITEM hThread = NULL;
				vecName.clear();
				pView->m_PageDlg->m_paneArbiter.InitData(vecName);
				sort(vecName.begin(),vecName.end());

				auto item_name = vecName.begin();
				for(;item_name!=vecName.end();item_name++)
				{			
					HTREEITEM hitem = FinItem_m_wndFileView(m_hRootTact,item_name->first.Left(item_name->first.Find(L"::")));
					if(hitem != NULL)
					{
						SetTreeThreadItemImage(hitem, item_name->second);
						m_wndFileView.SetItemText(hitem, item_name->first);
					}
				}
				// set flag
				SetConnectServFlag(true);

				pView->m_PageDlg->m_pDataAccess = m_pDataAccess;
				pView->m_PageDlg->InitData();

				CString strTacticsName;
				strTacticsName = m_wndFileView.GetItemText(m_hRootTact);
				strTacticsName = strTacticsName.Left(strTacticsName.Find(_T("::")));
				strTacticsName += _T("::在线::");
				strTacticsName += dlg.m_strDbIp;
				m_wndFileView.SetItemText(m_hRootTact, strTacticsName);
				// write info into ini file
				WritePrivateProfileString(_T("beopdebugtool"), _T("server"), dlg.m_strDbIp, strPath);
				UpdateData(FALSE);
				return true;
			}
			
			
			
		}
	}
	return false;
}

