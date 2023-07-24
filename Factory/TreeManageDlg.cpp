// TreeManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TreeManageDlg.h"
#include "afxdialogex.h"
#include "GroupDlg.h"
#include "MainFrm.h"
#include "../ComponentDraw/EqmDrawProject.h"
#include "../ComponentDraw/CEqmDrawPage.h"
#include "../ComponentDraw/EqmDrawDefines.h"
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
#include "../ControlEx/WarningDlg.h"
#include "HttpOperation.h"
#include "Tools/CustomTools/CustomTools.h"
#include "json/json.h"

// CTreeManageDlg 对话框

IMPLEMENT_DYNAMIC(CTreeManageDlg, CDialog)

CTreeManageDlg::CTreeManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeManageDlg::IDD, pParent)
	,m_hRoot(NULL),
	m_pMainFrame(NULL)
	,m_pSelectedPage(NULL)
	,m_hSelectedItem(NULL)
	,m_hRootPage(NULL)
	,m_hRootTact(NULL)
	//,m_hRootImgLib(NULL)
	//,m_hRootTactLib(NULL)
	,m_pDataAccess(NULL)
	,m_hCurSelItem(NULL)
	,m_bIsConnectServ(false)
	,m_nGroupShowOrder(0)
	,m_bExportPageLoad(false)
	,m_nDeviceType(0)
{

}

CTreeManageDlg::~CTreeManageDlg()
{
	SAFE_DELETE(m_pDataAccess);
}

void CTreeManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_treectrl);
}


BEGIN_MESSAGE_MAP(CTreeManageDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD_GROUP, &CTreeManageDlg::OnBnClickedButtonAddGroup)
	ON_BN_CLICKED(IDC_BUTTON_DEL_GROUP, &CTreeManageDlg::OnBnClickedButtonDelGroup)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PAGE, &CTreeManageDlg::OnBnClickedButtonAddPage)
	ON_BN_CLICKED(IDC_BUTTON_DEL_PAGE, &CTreeManageDlg::OnBnClickedButtonDelPage)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, &CTreeManageDlg::OnNMDblclkTree)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CTreeManageDlg::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, &CTreeManageDlg::OnBnClickedButtonMoveDown)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, &CTreeManageDlg::OnNMRClickTree)
	ON_COMMAND(ID_TREE_PAGE_PROPERTY, &CTreeManageDlg::OnTreePageProperty)
	ON_COMMAND(ID_IMPORT_PAGE, &CTreeManageDlg::OnImportPage)
	ON_NOTIFY(NM_CLICK, IDC_TREE, &CTreeManageDlg::OnNMClickTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE, &CTreeManageDlg::OnTvnItemexpandingTree)
	ON_COMMAND(ID_MENU_GROUP_ADD, &CTreeManageDlg::OnMenuAddGroup)
	ON_COMMAND(ID_MENU_GROUP_DEL, &CTreeManageDlg::OnMenuDelGroup)
	ON_COMMAND(ID_MENU_PAGE_ADD, &CTreeManageDlg::OnMenuAddPage)
	ON_COMMAND(ID_MENU_PAGE_DEL, &CTreeManageDlg::OnMenuDelPage)
	ON_COMMAND(ID_MENU_ADD_THREAD, &CTreeManageDlg::OnMenuAddThread)
	ON_COMMAND(ID_MENU_DEL_THREAD, &CTreeManageDlg::OnMenuDelThread)
	ON_COMMAND(ID_MENU_MODIFY_THREAD, &CTreeManageDlg::OnMenuModifyThread)
	ON_COMMAND(ID_MENU_RUN_THREAD, &CTreeManageDlg::OnMenuRunThread)
	ON_COMMAND(ID_MENU_STOP_THREAD, &CTreeManageDlg::OnMenuStopThread)
	ON_COMMAND(ID_MENU_CONN_DEBUG, &CTreeManageDlg::OnMenuConnDebug)
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(ID_MENU_RUN_THREAD, &CTreeManageDlg::OnUpdateMenuRunThread)
	ON_UPDATE_COMMAND_UI(ID_MENU_STOP_THREAD, &CTreeManageDlg::OnUpdateMenuStopThread)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CTreeManageDlg::OnTvnSelchangedTree)
	ON_COMMAND(ID_MENU_GROUP_UP, &CTreeManageDlg::OnMenuGroupUp)
	ON_COMMAND(ID_MENU_GROUP_DOWN, &CTreeManageDlg::OnMenuGroupDown)
	ON_COMMAND(ID_MENU_PAGE_UP, &CTreeManageDlg::OnMenuPageUp)
	ON_COMMAND(ID_MENU_PAGE_DOWN, &CTreeManageDlg::OnMenuPageDown)
	ON_COMMAND(ID_MENU_PAGE_PROPERTY, &CTreeManageDlg::OnMenuPageProperty)
	ON_COMMAND(ID_MENU_UPDATE_LIB, &CTreeManageDlg::OnMenuUpdateLib)
	ON_COMMAND(ID_MENU_PAGE_IMPORT, &CTreeManageDlg::OnMenuPageImport)
	ON_COMMAND(ID_MENU_TEMPLATE_PAGE_IMPORT, &CTreeManageDlg::OnMenuTemplatePageImport)
	ON_COMMAND(ID_MENU_RUN_ALL_THREAD, &CTreeManageDlg::OnMenuRunAllThread)
	ON_COMMAND(ID_MENU_STOP_ALL_THREAD, &CTreeManageDlg::OnMenuStopAllThread)
	ON_UPDATE_COMMAND_UI(ID_MENU_RUN_ALL_THREAD, &CTreeManageDlg::OnUpdateMenuRunAllThread)
	ON_UPDATE_COMMAND_UI(ID_MENU_STOP_ALL_THREAD, &CTreeManageDlg::OnUpdateMenuStopAllThread)
	ON_UPDATE_COMMAND_UI(ID_MENU_UPDATE_LIB, &CTreeManageDlg::OnUpdateMenuUpdateLib)
	ON_UPDATE_COMMAND_UI(ID_MENU_ADD_THREAD, &CTreeManageDlg::OnUpdateMenuAddThread)
	ON_COMMAND(ID_MENU_MODIFY_THREAD_PERIO, &CTreeManageDlg::OnMenuModifyThreadPerio)
	ON_UPDATE_COMMAND_UI(ID_MENU_MODIFY_THREAD_PERIO, &CTreeManageDlg::OnUpdateMenuModifyThreadPerio)
	ON_COMMAND(ID_MENU_PAGE_ADDTO_TEMPLATE, &CTreeManageDlg::OnMenuPageAddtoTemplate)
	ON_UPDATE_COMMAND_UI(ID_MENU_GROUP_DEL, &CTreeManageDlg::OnUpdateMenuGroupDel)
	ON_UPDATE_COMMAND_UI(ID_MENU_PAGE_DEL, &CTreeManageDlg::OnUpdateMenuPageDel)
	ON_UPDATE_COMMAND_UI(ID_MENU_PAGE_PROPERTY, &CTreeManageDlg::OnUpdateMenuPageProperty)
	ON_COMMAND(IDM_BACKUP_LOGICTHREAD_STATUS, &CTreeManageDlg::OnBackupLogicthreadStatus)
	ON_COMMAND(IDM_RESTORE_LOGICTHREAD_STATUS, &CTreeManageDlg::OnRestoreLogicthreadStatus)
END_MESSAGE_MAP()


// CTreeManageDlg 消息处理程序


BOOL CTreeManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!m_ImageList.Create(16, 16, ILC_COLOR32, 0, 0))
	{
		return FALSE;
	}
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_TREE);
	m_ImageList.Add(&bitmap, RGB(0, 0, 0));
	m_treectrl.SetImageList(&m_ImageList, TVSIL_NORMAL);

	CRect rect_client;
	GetClientRect(&rect_client);
	CRect rect;
	int nButtonWidth = 0;
	int nButtonHeight = 0;
	GetDlgItem(IDC_BUTTON_ADD_GROUP)->GetWindowRect(&rect);
	nButtonWidth = rect.Width();
	nButtonHeight = rect.Height();
	GetDlgItem(IDC_BUTTON_ADD_GROUP)->MoveWindow(2,3,nButtonWidth,nButtonHeight);
	GetDlgItem(IDC_BUTTON_DEL_GROUP)->MoveWindow(4+nButtonWidth,3,nButtonWidth,nButtonHeight);
	GetDlgItem(IDC_BUTTON_ADD_PAGE)->MoveWindow(6+nButtonWidth*2,3,nButtonWidth,nButtonHeight);
	GetDlgItem(IDC_BUTTON_DEL_PAGE)->MoveWindow(8+nButtonWidth*3,3,nButtonWidth,nButtonHeight);
	GetDlgItem(IDC_BUTTON_MOVE_UP)->MoveWindow(10+nButtonWidth*4,3,nButtonWidth,nButtonHeight);
	GetDlgItem(IDC_BUTTON_MOVE_DOWN)->MoveWindow(12+nButtonWidth*5,3,nButtonWidth,nButtonHeight);

	GetDlgItem(IDC_STATIC_PICTURE)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_PICTURE)->MoveWindow(0,3*2+nButtonHeight, rect_client.Width(), rect.Height());   

	m_menu.LoadMenu(IDR_MENU_TREE);
	m_menu_group.LoadMenu(IDR_MENU_GROUP_IMPORT);

	return TRUE;  // return TRUE unless you set the focus to a control	
	// 异常: OCX 属性页应返回 FALSE
}


void CTreeManageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	int nButtonWidth = 0;
	int nButtonHeight = 0;
	if (GetDlgItem(IDC_BUTTON_ADD_GROUP))
	{
		GetDlgItem(IDC_BUTTON_ADD_GROUP)->GetWindowRect(&rect);
		nButtonWidth = rect.Width();
		nButtonHeight = rect.Height();
	}
	CRect rect_client;
	GetClientRect(&rect_client);
	if (GetDlgItem(IDC_TREE))
	{
		GetDlgItem(IDC_TREE)->MoveWindow(0,0/*3*2+nButtonHeight+6*/,rect_client.Width(), cy/*-(3*2+nButtonHeight+6)*/);
	}
}


void CTreeManageDlg::OnBnClickedButtonAddGroup()
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
				m_treectrl.SetFocus();
				m_treectrl.SelectItem(hGroup);
				m_treectrl.Expand(m_hRootPage, TVE_EXPAND);
				m_pMainFrame->SetSaveFlag(true);
			}
		}
	}
	else
	{
		MessageBox(L"请先创建工程!");
	}
}

void CTreeManageDlg::OnBnClickedButtonDelGroup()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(L"确定要删除组和所包含的页面吗?",L"删除组",MB_OKCANCEL) == IDOK)
	{
		HTREEITEM hItem = m_treectrl.GetSelectedItem();
		HTREEITEM hItemParent = NULL;
		vector<HTREEITEM> vec;
		BOOL bRes = FALSE;

		hItemParent = m_treectrl.GetParentItem(hItem);
		if (hItemParent == m_hRootPage)
		{
			int nGroupID =  (int)m_treectrl.GetItemData(hItem);
			m_pMainFrame->m_pPrjSqlite->DeleteGroup(nGroupID);
			m_pMainFrame->m_project.DeleteGroup(nGroupID);
			HTREEITEM hChild = m_treectrl.GetChildItem(hItem);
			while (hChild)
			{
				vec.push_back(hChild);
				int nPageID = (int)m_treectrl.GetItemData(hChild);
				CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
				ASSERT(pPage);
				m_pMainFrame->m_pPrjSqlite->DeletePage(pPage);
				m_pMainFrame->m_project.DeletePage(pPage);
				hChild = m_treectrl.GetNextItem(hChild, TVGN_NEXT);
			}
			const size_t size = vec.size();
			for (size_t i=0;i<size;++i)
			{
				bRes = m_treectrl.DeleteItem(vec[i]);
				ASSERT(bRes);
			}
			bRes = m_treectrl.DeleteItem(hItem);
			m_pMainFrame->m_project.DeleteGroup(nGroupID);
			ASSERT(bRes);
			int nPageID = m_pMainFrame->SetOnePage();
			if (nPageID != -1)
			{
				SelectFirstPageItem(nPageID);
			}

			m_pMainFrame->SetSaveFlag(true);
		}
		else
		{
			MessageBox(L"请选中组节点!");
			return;
		}
	}
}

void CTreeManageDlg::OnBnClickedButtonAddPage()
{
	HTREEITEM hItem = m_treectrl.GetSelectedItem();
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
		else if (m_treectrl.GetParentItem(hItem) == m_hRootPage)
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
				m_treectrl.Expand(hItem, TVE_EXPAND);
				m_treectrl.SetFocus();
				m_treectrl.SelectItem(hPage);
				int x = (m_pMainFrame->m_project.GetScreenWidth()-nWidth)/2;
				int y = (m_pMainFrame->m_project.GetScreenHeight()-nHeight)/2;
				m_pMainFrame->m_DrawContext.pPage = new CEqmDrawPage(m_pMainFrame->m_MaxPageID, (LPCTSTR)dlg.m_strNewPageName, m_pMainFrame->m_project.GetProjectID(), 0,page_type,nWidth,nHeight,x,y,RGB(219,219,219));
				m_pMainFrame->m_DrawContext.pPage->SetPageWidth(nWidth);
				m_pMainFrame->m_DrawContext.pPage->SetPageHeight(nHeight);
				m_pMainFrame->m_DrawContext.pPage->SetPageType(page_type);
				CString strItemText = m_treectrl.GetItemText(hItem);
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
			hItemParent = m_treectrl.GetParentItem(hItem);
			if (hItemParent != NULL)
			{
				if (m_treectrl.GetParentItem(hItemParent) == m_hRootPage)
				{
					MessageBox(L"选中的是页面节点!请选择组节点!");
					return;
				} 
			}
		}
	}
}

void CTreeManageDlg::OnBnClickedButtonDelPage()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(L"确定要删除页面吗?",L"删除页面", MB_OKCANCEL) == IDOK)
	{
		HTREEITEM hItem = m_treectrl.GetSelectedItem();
		HTREEITEM hParent = NULL;
		if (hItem)
		{
			HTREEITEM hParent = m_treectrl.GetParentItem(hItem);
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
		if(m_treectrl.GetChildItem(hItem) == NULL)
		{
			int nPageID = (int)m_treectrl.GetItemData(hItem);
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
				m_treectrl.DeleteItem(hItem);
				nPageID = m_pMainFrame->SetOnePage();
				if (nPageID != -1)
				{
					SelectFirstPageItem(nPageID);
				}
			}
		}

		m_pMainFrame->SetSaveFlag(true);
	}
}

void CTreeManageDlg::InsertRootNode( CString strName )
{
	if (m_hRoot == NULL)
	{
		m_hRoot = m_treectrl.InsertItem(strName, 1, 1, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRoot);
	}

}

HTREEITEM CTreeManageDlg::InsertGroup( CString strName, const int nGroupID )
{
	HTREEITEM hGroup = m_treectrl.InsertItem(strName, 3, 3, m_hRootPage, TVI_LAST);
	ASSERT(hGroup);
	BOOL bRes = m_treectrl.SetItemData(hGroup, (DWORD)nGroupID);
	ASSERT(bRes);
	return hGroup;
}

HTREEITEM CTreeManageDlg::InsertPage( CString strName, HTREEITEM hGroup, const int nPageID)
{
	ASSERT(hGroup);

	HTREEITEM hPage = m_treectrl.InsertItem(strName, 4, 5, hGroup, TVI_LAST);
	ASSERT(hPage);
	TREE_INFO info;
	info.hItem = hPage;
	info.nPageID = nPageID;
	m_vecTree.push_back(info);
	BOOL bRes = m_treectrl.SetItemData(hPage, (DWORD)nPageID);
	ASSERT(bRes);
	m_treectrl.Expand(hGroup, TVE_EXPAND);
	return hPage;
}


void CTreeManageDlg::OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		*pResult = 0;
		return;
	}

	HTREEITEM hItem = m_treectrl.GetSelectedItem();
	HTREEITEM hItemParent = m_treectrl.GetParentItem(hItem);
	bool bIsExpand = TVIS_EXPANDED & m_treectrl.GetItemState(hItem, TVIS_EXPANDED);
	UINT uExpandFlag = TVE_EXPAND;

	if (hItemParent != NULL)
	{	// second level node
		if (m_treectrl.GetParentItem(hItemParent) == m_hRootPage)
		{	// third level node -- page
			if(m_pMainFrame->m_bLoadingPage)
			{
				return;
			}
			const DWORD nPageID = m_treectrl.GetItemData(hItem);	
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
				m_pMainFrame->m_DrawContext.pPage->SetStartColor(RGB(158, 158, 158));
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
			pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
		}
		else if (hItemParent == m_hRootPage)
		{	// group node
			(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
			m_treectrl.Expand(hItem, uExpandFlag);

			pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
		}
		else if (hItemParent == m_hRootTact)
		{	// thread node
			// refresh panel arbiter
			int nThreadAllNum = 0;
			int nThreadCurSelNum = 0;

			GetTreeNodeNumInfo(m_hRootTact, hItem, nThreadAllNum, nThreadCurSelNum);
			CString strCurNodeName = GetCurSelTreeNodeName(hItem);
			pView->m_PageDlg->m_paneArbiter.SetThreadNum(nThreadAllNum, nThreadCurSelNum);
			pView->m_PageDlg->m_paneArbiter.SetThreadCurSelName(strCurNodeName);

			// change img
			//int nImgIndex = 2;
			//HTREEITEM hItemLop = m_treectrl.GetChildItem(hItemParent);
			//while (hItemLop != NULL)
			//{
			//	if (hItem == hItemLop)
			//	{
			//		nImgIndex = 1;
			//	}
			//	else
			//	{
			//		nImgIndex = 2;
			//	}
			//	m_treectrl.SetItemImage(hItemLop, nImgIndex, nImgIndex);
			//	hItemLop = m_treectrl.GetNextSiblingItem(hItemLop);
			//}

			pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
			pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_SHOW);
		}
	}

	if (hItem == m_hRootPage)
	{	// root page
		(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
		m_treectrl.Expand(hItem, uExpandFlag);

		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
	}
	else if (hItem == m_hRootTact)
	{	// tactics node
		(bIsExpand) ? (uExpandFlag = TVE_COLLAPSE) : (uExpandFlag = TVE_EXPAND);
		m_treectrl.Expand(hItem, uExpandFlag);

		//pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
		//pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_SHOW);
	}


	*pResult = 0;
}


UINT WINAPI CTreeManageDlg::ThreadFunc_LoadPage( LPVOID lparam )
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
	pMainFrame->m_DrawContext.pPage->SetStartColor(RGB(158, 158, 158));

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
	}

	pMainFrame->RedrawWindow();
	pMainFrame->m_bLoadingPage = false;
	if(pMainFrame->m_bCheckImageMode && pMainFrame->m_nElementID>-1)
	{
		pMainFrame->ShowSelectElementByID(pMainFrame->m_nElementID);
	}
	return 1;
}

void CTreeManageDlg::Expand()
{
	ASSERT(m_hRootPage != NULL);
	m_treectrl.Expand(m_hRootPage, TVE_EXPAND);
	ASSERT(m_hRootTact != NULL);
	m_treectrl.Expand(m_hRootTact, TVE_EXPAND);
	//ASSERT(m_hRootImgLib != NULL);
	//m_treectrl.Expand(m_hRootImgLib, TVE_EXPAND);
	//ASSERT(m_hRootTactLib != NULL);
	//m_treectrl.Expand(m_hRootTactLib, TVE_EXPAND);
}

void CTreeManageDlg::SelectFirstPageItem(const int nPageID)
{
	const size_t size = m_vecTree.size();
	for (size_t i=0;i<size;++i)
	{
		if (nPageID == m_vecTree[i].nPageID)
		{
			BOOL bRes = m_treectrl.SelectItem(m_vecTree[i].hItem);
			ASSERT(bRes);
			break;
		}
	}
}

void CTreeManageDlg::DeleteAllItems()
{
	if(m_hRootPage)
	{
		HTREEITEM hItem = m_treectrl.GetChildItem(m_hRootPage);
		vector<HTREEITEM> vec_group;
		while (hItem)
		{
			vec_group.push_back(hItem);
			int nGroupID =  (int)m_treectrl.GetItemData(hItem);
			m_pMainFrame->m_project.DeleteGroup(nGroupID);
			HTREEITEM hChild = m_treectrl.GetChildItem(hItem);
			vector<HTREEITEM> vec;
			while (hChild)
			{
				vec.push_back(hChild);
				int nPageID = (int)m_treectrl.GetItemData(hChild);
				CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
				if(pPage)
				{
					m_pMainFrame->m_project.DeletePage(pPage);
				}
				hChild = m_treectrl.GetNextItem(hChild, TVGN_NEXT);
			}
			const size_t size = vec.size();
			for (size_t i=0;i<size;++i)
			{
				BOOL bRes = m_treectrl.DeleteItem(vec[i]);
				ASSERT(bRes);
			}
			hItem = m_treectrl.GetNextItem(hItem, TVGN_NEXT);
		}
		{
			const size_t size_group = vec_group.size();
			for (size_t j=0;j<size_group;++j)
			{
				int nGroupID = (int)m_treectrl.GetItemData(vec_group[j]);
				BOOL bRes = m_treectrl.DeleteItem(vec_group[j]);
				m_pMainFrame->m_project.DeleteGroup(nGroupID);
				ASSERT(bRes);
			}
		}
		{
			BOOL bRes = m_treectrl.DeleteItem(m_hRootPage);
			m_hRootPage = NULL;
		}
		m_vecTree.clear();
	}

	//
	m_treectrl.DeleteItem(m_hRootTact);
	m_hRootTact = NULL;

	//m_treectrl.DeleteItem(m_hRootImgLib);
	//m_hRootImgLib = NULL;

	//m_treectrl.DeleteItem(m_hRootTactLib);
	//m_hRootTactLib = NULL;
}

void CTreeManageDlg::UpdateItemPage( const int nPageID )
{
	const size_t size = m_vecTree.size();
	for (size_t i=0;i<size;++i)
	{
		if (nPageID == m_vecTree[i].nPageID)
		{
			CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
			ASSERT(pPage);
			wstring strName = pPage->GetPageName();
			m_treectrl.SetItemText(m_vecTree[i].hItem, strName.c_str());
		}
	}
}

void CTreeManageDlg::OnBnClickedButtonMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSelectItem = m_treectrl.GetSelectedItem();
	int nSelectPageID = (int)m_treectrl.GetItemData(hSelectItem);
	int nNextPageID = 0;
	if (IsTreeItemPage(hSelectItem))
	{
		HTREEITEM hPrev = m_treectrl.GetPrevSiblingItem(hSelectItem);
		if (hPrev)
		{
			nNextPageID = (int)m_treectrl.GetItemData(hPrev);
			HTREEITEM hNewItem =  m_treectrl.InsertItem(m_treectrl.GetItemText(hPrev),4,5,m_treectrl.GetParentItem(hSelectItem),hSelectItem);
			assert(hNewItem);
			m_treectrl.SetItemData(hNewItem, (DWORD)nNextPageID);
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
			m_treectrl.DeleteItem(hPrev);
			TREE_INFO info;
			info.hItem = hNewItem;
			info.nPageID = nNextPageID;
			m_vecTree.push_back(info);
			m_pMainFrame->m_project.SwapTwoElementInPageList(nSelectPageID, nNextPageID);
			m_treectrl.SetFocus();
			m_treectrl.SelectItem(hSelectItem);
		}

		m_pMainFrame->SetSaveFlag(true);
	}
}

void CTreeManageDlg::OnBnClickedButtonMoveDown()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSelectItem = m_treectrl.GetSelectedItem();
	int nSelectPageID = (int)m_treectrl.GetItemData(hSelectItem);
	int nNextPageID = 0;
	if (IsTreeItemPage(hSelectItem))
	{
		HTREEITEM hNext = m_treectrl.GetNextSiblingItem(hSelectItem);
		if (hNext)
		{
			nNextPageID = (int)m_treectrl.GetItemData(hNext);
			HTREEITEM hNewItem =  m_treectrl.InsertItem(m_treectrl.GetItemText(hSelectItem),4,5,m_treectrl.GetParentItem(hSelectItem),hNext);
			assert(hNewItem);
			m_treectrl.SetItemData(hNewItem, (DWORD)nSelectPageID);
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
			m_treectrl.DeleteItem(hSelectItem);
			TREE_INFO info;
			info.hItem = hNewItem;
			info.nPageID = nSelectPageID;
			m_vecTree.push_back(info);
			m_pMainFrame->m_project.SwapTwoElementInPageList(nSelectPageID, nNextPageID);
			m_treectrl.SetFocus();
			m_treectrl.SelectItem(hNewItem);
		}

		m_pMainFrame->SetSaveFlag(true);
	}
}

bool CTreeManageDlg::IsTreeItemPage( HTREEITEM hItem )
{
	if (hItem == NULL)
	{
		return false;
	}
	if (hItem == m_hRootPage)
	{
		return false;
	}
	HTREEITEM hParent = m_treectrl.GetParentItem(hItem);
	if (hParent)
	{
		if (hParent == m_hRootPage)
		{
			return false;
		}
		if (m_treectrl.GetParentItem(hParent) == m_hRootPage)
		{
			return true;
		}
	}
	return false;
}

void CTreeManageDlg::OnNMRClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint pt;
	GetCursorPos(&pt);
	::ScreenToClient(m_treectrl.m_hWnd, &pt);
	UINT uFlags;
	HTREEITEM hItem = m_treectrl.HitTest(pt, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		m_treectrl.SelectItem(hItem);
	}
	HTREEITEM hSelectItem = m_treectrl.GetSelectedItem();
	CMenu* pSubMenu = NULL;
	CPoint oPoint(0, 0);
	GetCursorPos(&oPoint);
	if (IsItemRootPage(hSelectItem))
	{	// root page
		pSubMenu = m_menu.GetSubMenu(1);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
	else if (IsItemGroup(hSelectItem))
	{	// group
		m_hSelectedItem = hSelectItem;

		pSubMenu = m_menu.GetSubMenu(2);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
	else if (IsItemPage(hSelectItem))
	{	// page
		if(m_pMainFrame->m_bLoadingPage)
		{
			return;
		}

		m_treectrl.Expand(hSelectItem,TVE_EXPAND);
		const int nPageID = (int)m_treectrl.GetItemData(hSelectItem);
		CEqmDrawPage* pPage = m_pMainFrame->m_project.GetPageByID(nPageID);
		if (pPage != NULL)
		{
			m_pSelectedPage = pPage;
			m_hSelectedItem = hSelectItem;
		}

		pSubMenu = m_menu.GetSubMenu(3);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
	else if (IsItemRootTactics(hSelectItem))
	{	// tactics
		pSubMenu = m_menu.GetSubMenu(4);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
	else if (IsItemThread(hSelectItem))
	{	// thread
		m_hCurSelItem = hSelectItem;
		pSubMenu = m_menu.GetSubMenu(5);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
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

		m_treectrl.Expand(hSelectItem,TVE_EXPAND);
		const int nPageID = (int)m_treectrl.GetItemData(hSelectItem);
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
		m_treectrl.Expand(hSelectItem,TVE_EXPAND);
		m_hSelectedItem = hSelectItem;
		CPoint oPoint;
		GetCursorPos(&oPoint);
		CMenu* pSubMenu = m_menu_group.GetSubMenu(0);
		pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
*/
	*pResult = 0;
}

void CTreeManageDlg::OnTreePageProperty()
{
	// TODO: 在此添加命令处理程序代码
	CSetUpDataDlg dlg;
	dlg.SetPage(m_pSelectedPage);
	dlg.m_strEditOldValue = m_pSelectedPage->GetPageName().c_str();
	dlg.m_strEditNewValue = m_pSelectedPage->GetPageName().c_str();
	dlg.m_nPageType = m_pSelectedPage->GetPageType();
	dlg.m_nPicID = m_pSelectedPage->GetBKImageID();
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

		CBEOPDesignerView* pView = (CBEOPDesignerView*)m_pMainFrame->GetActiveView();
		ASSERT(pView);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(TRUE);
		m_treectrl.SetItemText(m_hSelectedItem, dlg.m_strEditNewValue);
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

			}

		}
	}
}

bool CTreeManageDlg::IsTreeItemGroup( HTREEITEM hItem )
{
	if (hItem == NULL)
	{
		return false;
	}
	if (hItem == m_hRootPage)
	{
		return false;
	}
	HTREEITEM hParent = m_treectrl.GetParentItem(hItem);
	if (hParent == m_hRootPage)
	{
		return true;
	}
	return false;
}


void CTreeManageDlg::OnImportPage()
{
	// TODO: 在此添加命令处理程序代码
	CImportPageSelectDlg dlg(false);
	dlg.m_hItem = m_hSelectedItem;
	dlg.DoModal();
}


void CTreeManageDlg::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	::ScreenToClient(m_treectrl.m_hWnd, &pt);
	UINT uFlags = 0;
	HTREEITEM hItem = m_treectrl.HitTest(pt, &uFlags);
	if ((NULL == hItem) || !(TVHT_ONITEM & uFlags))
	{
		return;
	}
	m_treectrl.SelectItem(hItem);

	if (NULL == m_pMainFrame)
	{
		m_pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
		ASSERT(m_pMainFrame != NULL);
		if (NULL == m_pMainFrame)
		{
			return;
		}
	}

	DWORD_PTR dwVal = m_treectrl.GetItemData(hItem);
	switch (dwVal)
	{
	case -20001:
		{	// pipe
			m_pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_STRAIGHTPIPE == m_pMainFrame->m_DrawContext.m_nDrawMode ? EQMDRAW_MOVE : EQMDRAW_STRAIGHTPIPE;
			PipeColor pipe_color;
			m_pMainFrame->m_DrawContext.SetPipeColor(pipe_color);
		}
		break;
	case -20002:
		{	// line
			m_pMainFrame->m_DrawContext.m_nDrawMode = GraphicType_LINE == m_pMainFrame->m_DrawContext.m_nDrawMode ? EQMDRAW_MOVE : GraphicType_LINE;
			m_pMainFrame->m_DrawContext.SetLineWidth(2);
			m_pMainFrame->m_DrawContext.SetLineColor(0);
		}
		break;
	case -20003:
		{	// rectangle
			m_pMainFrame->m_DrawContext.m_nDrawMode = GraphicType_RECTANGLE == m_pMainFrame->m_DrawContext.m_nDrawMode ? EQMDRAW_MOVE : GraphicType_RECTANGLE;
			m_pMainFrame->m_DrawContext.SetLineWidth(2);
			m_pMainFrame->m_DrawContext.SetLineColor(0);
		}
		break;
	case -20004:
		{	// circle
			m_pMainFrame->m_DrawContext.m_nDrawMode = GraphicType_ELLIPSE == m_pMainFrame->m_DrawContext.m_nDrawMode ? EQMDRAW_MOVE : GraphicType_ELLIPSE;
		}
		break;
	case -20005:
		{	// text
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_TEXT != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_TEXT : EQMDRAW_MOVE;
		}
		break;
	case -20006:
		{	// chiller
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_CHILLER != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_CHILLER : EQMDRAW_MOVE;
		}
		break;
	case -20007:
		{	// pump
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_PUMP != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_PUMP : EQMDRAW_MOVE;
		}
		break;
	case -20008:
		{	// valve
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_VALVE != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_VALVE : EQMDRAW_MOVE;
		}
		break;
	case -20009:
		{	// cooling tower
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_COOLINGTOWER != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_COOLINGTOWER : EQMDRAW_MOVE;
		}
		break;
	case -20010:
		{	// meta
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_METAFILE != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_METAFILE : EQMDRAW_MOVE;
		}
		break;
	case -20011:
		{	// meta button
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_METABUTTON != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_METABUTTON : EQMDRAW_MOVE;
		}
		break;
	case -20012:
		{	// bend
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_BEND != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_BEND : EQMDRAW_MOVE;
		}
		break;
	case -20013:
		{	// dash board
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_DASHBOARD != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_DASHBOARD : EQMDRAW_MOVE;
		}
		break;
	case -20014:
		{	// line chart
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_LINE_CHART != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_LINE_CHART : EQMDRAW_MOVE;
		}
		break;
	case -20015:
		{	// bar chart
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_BAR_CHART != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_BAR_CHART : EQMDRAW_MOVE;
		}
		break;
	case -20016:
		{	// pie chart
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_PIE_CHART != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_PIE_CHART : EQMDRAW_MOVE;
		}
		break;
	case -20017:
		{	// time select
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_TIME_SELECT != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_TIME_SELECT : EQMDRAW_MOVE;
		}
		break;
	case 20018:
		{	// time bar
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_TIME_BAR != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_TIME_BAR : EQMDRAW_MOVE;
		}
		break;
	case -20019:
		{	// progress bar
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_PROGRESS_BAR != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_PROGRESS_BAR : EQMDRAW_MOVE;
		}
		break;
	case -20020:
		{	// diagnose chart
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_DIAGNOSE_CHART != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_DIAGNOSE_CHART : EQMDRAW_MOVE;
		}
		break;
	case -20021:
		{	// diagnose char plus
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_POLYGON != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_POLYGON : EQMDRAW_MOVE;
		}
		break;
	case -20022:
		{	// area change
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_AREA_CHANGE != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_AREA_CHANGE : EQMDRAW_MOVE;
		}
		break;
	case -20023:
		{	// shell execute
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_SHELL_EXECUTE != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_SHELL_EXECUTE : EQMDRAW_MOVE;
		}
		break;
	case -20024:
		{	// embeded page
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_EMBEDED_PAGE != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_EMBEDED_PAGE : EQMDRAW_MOVE;
		}
		break;
	case -20025:
		{	// liquid level
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_LIQUID_LEVEL != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_LIQUID_LEVEL : EQMDRAW_MOVE;
		}
		break;
	case -20026:
		{	// check button plus
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_CHKBTN_PLUS != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_CHKBTN_PLUS : EQMDRAW_MOVE;
		}
		break;
	case -20027:
		{	//straight line
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_LINE != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_LINE : EQMDRAW_MOVE;
		}
		break;
	case -20028:
		{	// history trend
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_HISTORY_TREND != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_HISTORY_TREND : EQMDRAW_MOVE;
		}
		break;
	case -20029:
		{	// energy save ROI
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_RECTANGLE != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_RECTANGLE : EQMDRAW_MOVE;
		}
		break;
	case -20030:
		{	// map navigate
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_MAP_NAVIGATE != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_MAP_NAVIGATE : EQMDRAW_MOVE;
		}
		break;
	case -20031:
		{	// navigate window
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_NAVIGATE_WINDOW != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_NAVIGATE_WINDOW : EQMDRAW_MOVE;
		}
		break;
	case -20032:
		{	// level ruler
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_LEVEL_RULER != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_LEVEL_RULER : EQMDRAW_MOVE;
		}
		break;
	case -20033:
		{	// data analyse
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_DATA_ANALYSE != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_DATA_ANALYSE : EQMDRAW_MOVE;
		}
		break;
	case -20034:
		{	// temperature distribution
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_TEMPER_DISTRI != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_TEMPER_DISTRI : EQMDRAW_MOVE;
		}
		break;
	case -20035:
		{	// data report
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_DATA_REPORT != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_DATA_REPORT : EQMDRAW_MOVE;
		}
		break;
	case -20036:
		{	// plane temperature distribution
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_PLANE_TEMPE_DISTR != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_PLANE_TEMPE_DISTR : EQMDRAW_MOVE;
		}
		break;
	case -20037:
		{	// dotted line frame
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_DOTLINE_FRAME != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_DOTLINE_FRAME : EQMDRAW_MOVE;
		}
		break;
	case -20038:
		{	// general component
			m_pMainFrame->m_DrawContext.m_nDrawMode = (NULL != m_pMainFrame->m_DrawContext.pPage && EQMDRAW_GENERAL_COMPONENT != m_pMainFrame->m_DrawContext.m_nDrawMode) ? EQMDRAW_GENERAL_COMPONENT : EQMDRAW_MOVE;
		}
		break;
	default:
		break;
	}

	*pResult = 0;
}


void CTreeManageDlg::OnTvnItemexpandingTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	CPoint pt;
	GetCursorPos(&pt);
	::ScreenToClient(m_treectrl.m_hWnd, &pt);
	UINT uFlags;
	HTREEITEM hItem1 = m_treectrl.HitTest(pt, &uFlags);

	if(TVHT_ONITEM & uFlags)
	{
		*pResult = TRUE;
	}
	else
	{
		*pResult = 0;
	}	
}

void CTreeManageDlg::InitTreeNodes(void)
{
	// insert default nodes
	if (NULL == m_hRootPage)
	{
		m_hRootPage = m_treectrl.InsertItem(_T("页面"), 1, 1, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootPage != NULL);
		if (NULL == m_hRootPage)
		{
			return;
		}
		BOOL bRes = m_treectrl.SetItemData(m_hRootPage, 1);
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
				m_pMainFrame->m_DrawContext.pPage = new CEqmDrawPage(m_pMainFrame->m_MaxPageID, L"monitor", m_pMainFrame->m_project.GetProjectID(), 0,page_type,nWidth,nHeight,x,y,RGB(219,219,219));
				m_pMainFrame->m_DrawContext.pPage->SetPageWidth(nWidth);
				m_pMainFrame->m_DrawContext.pPage->SetPageHeight(nHeight);
				m_pMainFrame->m_DrawContext.pPage->SetPageType(page_type);
				m_pMainFrame->m_DrawContext.pPage->SetGroupID(nGroupID); 
				m_pMainFrame->m_DrawContext.pPage->SetPageShowTopInObserver(true);
				m_pMainFrame->m_project.AddPage(m_pMainFrame->m_DrawContext.pPage);
				m_pMainFrame->m_project.SetCurPageID(m_pMainFrame->m_MaxPageID);
				m_pMainFrame->m_MaxPageID++;

				hPage = InsertPage(_T("trend"), hGroup, (DWORD)(m_pMainFrame->m_MaxPageID));
				m_pMainFrame->m_DrawContext.pPage = new CEqmDrawPage(m_pMainFrame->m_MaxPageID, L"trend", m_pMainFrame->m_project.GetProjectID(), 0,page_type,nWidth,nHeight,x,y,RGB(219,219,219));
				m_pMainFrame->m_DrawContext.pPage->SetPageWidth(nWidth);
				m_pMainFrame->m_DrawContext.pPage->SetPageHeight(nHeight);
				m_pMainFrame->m_DrawContext.pPage->SetPageType(page_type);
				m_pMainFrame->m_DrawContext.pPage->SetGroupID(nGroupID); 
				m_pMainFrame->m_DrawContext.pPage->SetPageShowTopInObserver(true);
				m_pMainFrame->m_project.AddPage(m_pMainFrame->m_DrawContext.pPage);
				m_pMainFrame->m_project.SetCurPageID(m_pMainFrame->m_MaxPageID);
				m_pMainFrame->m_MaxPageID++;

				m_treectrl.Expand(m_hRootPage, TVE_EXPAND);
				m_treectrl.Expand(hGroup, TVE_EXPAND);
				m_pMainFrame->SetSaveFlag(true);
			}
		}
	}
	if (NULL == m_hRootTact)
	{
		m_hRootTact = m_treectrl.InsertItem(_T("策略::离线"), 7, 7, TVI_ROOT, TVI_LAST);
		ASSERT(m_hRootTact != NULL);
		if (NULL == m_hRootTact)
		{
			return;
		}
		BOOL bRes = m_treectrl.SetItemData(m_hRootTact, -10000);
		ASSERT(bRes);

		
	}
	//if (NULL == m_hRootImgLib)
	//{
	//	m_hRootImgLib = m_treectrl.InsertItem(_T("图源库"), 11, 11, TVI_ROOT, TVI_LAST);
	//	ASSERT(m_hRootImgLib != NULL);
	//	if (NULL == m_hRootImgLib)
	//	{
	//		return;
	//	}
	//	BOOL bRes = m_treectrl.SetItemData(m_hRootImgLib, -20000);
	//	ASSERT(bRes);
	//}
	//if (NULL == m_hRootTactLib)
	//{
	//	m_hRootTactLib = m_treectrl.InsertItem(_T("策略库"), 30, 30, TVI_ROOT, TVI_LAST);
	//	ASSERT(m_hRootTactLib != NULL);
	//	if (NULL == m_hRootTactLib)
	//	{
	//		return;
	//	}
	//	BOOL bRes = m_treectrl.SetItemData(m_hRootTactLib, -30000);
	//	ASSERT(bRes);
	//}

	// initialize tactics node
	InitTacticsNode();

	// initialize tacitcs library
	//InitTacitcsLib();

	// initialize image library
	//InitImgLib();
}

bool CTreeManageDlg::IsItemRootPage(const HTREEITEM hItem) const
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

bool CTreeManageDlg::IsItemGroup(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (IsItemRootNode(hItem))
	{
		return false;
	}

	HTREEITEM hParent = m_treectrl.GetParentItem(hItem);
	if (hParent == m_hRootPage)
	{
		return true;
	}

	return false;
}

bool CTreeManageDlg::IsItemPage(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (IsItemRootNode(hItem))
	{
		return false;
	}

	HTREEITEM hParent = m_treectrl.GetParentItem(hItem);
	if (hParent != NULL)
	{
		if (IsItemRootNode(hParent))
		{
			return false;
		}

		if (m_treectrl.GetParentItem(hParent) == m_hRootPage)
		{
			return true;
		}
	}

	return false;
}

bool CTreeManageDlg::IsItemRootTactics(const HTREEITEM hItem) const
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

bool CTreeManageDlg::IsItemThread(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (IsItemRootNode(hItem))
	{
		return false;
	}

	HTREEITEM hParent = m_treectrl.GetParentItem(hItem);
	if (hParent == m_hRootTact)
	{
		return true;
	}

	return false;
}

bool CTreeManageDlg::IsItemRootNode(const HTREEITEM hItem) const
{
	if (NULL == hItem)
	{
		return false;
	}

	if (hItem == m_hRootPage || hItem == m_hRootTact /*|| hItem == m_hRootImgLib || hItem == m_hRootTactLib*/)
	{
		return true;
	}

	return false;
}

//bool CTreeManageDlg::IsItemRootTacticsLib(const HTREEITEM hItem) const
//{
//	if (NULL == hItem)
//	{
//		return false;
//	}
//
//	if (hItem == m_hRootTactLib)
//	{
//		return true;
//	}
//
//	return false;
//}

void CTreeManageDlg::OnMenuAddGroup()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonAddGroup();
}


void CTreeManageDlg::OnMenuDelGroup()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonDelGroup();
}


void CTreeManageDlg::OnMenuAddPage()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonAddPage();
}


void CTreeManageDlg::OnMenuDelPage()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonDelPage();
}


void CTreeManageDlg::OnMenuAddThread()
{
	// TODO: Add your command handler code here
	CAddDllThreadNameDlg dlg(true);
	if (dlg.DoModal() == IDOK)
	{
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
			if (m_bIsConnectServ)
			{
				strDllNameEx = dlg.m_strDllName + _T("::(Off)");
			}
			else
			{
				strDllNameEx = dlg.m_strDllName + _T("::(disconnect)");
			}
			hThread = InsertNodeThread(strDllNameEx, root_tactics);
			m_treectrl.SetItemData(hThread, -10001);
			m_treectrl.Expand(m_hRootTact, TVE_EXPAND);
			m_treectrl.SetFocus();
			m_treectrl.SelectItem(hThread);

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
		HTREEITEM hItem = m_treectrl.GetSelectedItem();

		GetTreeNodeNumInfo(m_hRootTact, hItem, nThreadAllNum, nThreadCurSelNum);
		CString strCurNodeName = GetCurSelTreeNodeName(hThread);
		pView->m_PageDlg->m_paneArbiter.SetThreadNum(nThreadAllNum, nThreadCurSelNum);
		pView->m_PageDlg->m_paneArbiter.SetThreadCurSelName(strCurNodeName);

		m_pMainFrame->SetSaveFlag(true);
	}
}


void CTreeManageDlg::OnMenuDelThread()
{
	// TODO: Add your command handler code here
	CString strthreadName;
	strthreadName = GetCurSelTreeNodeName(m_hCurSelItem);
	m_treectrl.DeleteItem(m_hCurSelItem);

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


void CTreeManageDlg::OnMenuModifyThread()
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
		m_treectrl.SetItemText(m_hCurSelItem, strThread);

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


void CTreeManageDlg::OnMenuRunThread()
{
	// TODO: Add your command handler code here
	CString strthreadName;
	strthreadName = GetCurSelTreeNodeName(m_hCurSelItem);
	strthreadName = strthreadName.Left(strthreadName.Find(':'));

	assert(m_pDataAccess != NULL);
	if (m_pDataAccess != NULL)
	{
		m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), L"1");
	}

	m_treectrl.SetItemText(m_hCurSelItem, strthreadName += _T("::(On)"));
}


void CTreeManageDlg::OnMenuStopThread()
{
	// TODO: Add your command handler code here
	CString strthreadName;
	strthreadName = GetCurSelTreeNodeName(m_hCurSelItem);
	strthreadName = strthreadName.Left(strthreadName.Find(':'));

	assert(m_pDataAccess != NULL);
	if (m_pDataAccess != NULL)
	{
		m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), L"0");
	}

	m_treectrl.SetItemText(m_hCurSelItem, strthreadName += _T("::(Off)"));
}


bool CTreeManageDlg::InitTacticsNode(void)
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
	for (iter=vecName.begin(); iter!=vecName.end(); ++iter)
	{
		hThread = InsertNodeThread(*iter, root_tactics);
		m_treectrl.SetItemData(hThread, -10001);
	}
	m_treectrl.Invalidate(FALSE);
	m_treectrl.Expand(m_hRootTact, TVE_EXPAND);
	pView->m_PageDlg->m_paneArbiter.InitListTitle();
	//pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
	pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);


	return true;
}

HTREEITEM CTreeManageDlg::InsertNodeThread(const CString strName, const ROOT_TYPE emRootType)
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
			nImage = 8;
			nSelImage = 9;
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

	HTREEITEM hThread = m_treectrl.InsertItem(strName, nImage, nSelImage, hParent, TVI_LAST);
	ASSERT(hThread);

	return hThread;
}

//bool CTreeManageDlg::InitTacitcsLib(void)
//{
//	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
//	ASSERT(pView != NULL);
//	if (NULL == pView)
//	{
//		return false;
//	}
//
//	// delete from tactics library
//	DeleteTreeNode(m_hRootTactLib);
//
//	// insert into tactics library
//	vector<CString> vecThreadMark;
//	vector<CString>::const_iterator iter;
//
//	pView->m_PageDlg->m_paneArbiter.m_pLogicManager->ReadDLLMarkfromDB(m_pMainFrame->m_strDBFileName, vecThreadMark);
//	HTREEITEM hThread = NULL;
//	for (iter=vecThreadMark.begin(); iter!=vecThreadMark.end(); ++iter)
//	{
//		hThread = InsertNodeThread(*iter, root_tactics_lib);
//		m_treectrl.SetItemData(hThread, 30001);
//	}
//	m_treectrl.Expand(m_hRootTact, TVE_EXPAND);
//
//	return true;
//}

void CTreeManageDlg::DeleteTreeNode(const HTREEITEM hNode)
{
	// Delete all of the children within hNode.
	if (m_treectrl.ItemHasChildren(hNode))
	{
		HTREEITEM hNextItem = NULL;
		HTREEITEM hChildItem = m_treectrl.GetChildItem(hNode);

		while (hChildItem != NULL)
		{
			hNextItem = m_treectrl.GetNextItem(hChildItem, TVGN_NEXT);
			m_treectrl.DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
}

void CTreeManageDlg::GetTreeNodeNumInfo(const HTREEITEM hRootNode, const HTREEITEM hCurNode, int& nAllNum, int& nCurSelNum)
{
	nAllNum = 0;
	nCurSelNum = 0;
	bool bIsFind = false;


	if (m_treectrl.ItemHasChildren(hRootNode))
	{
		HTREEITEM hItem = m_treectrl.GetChildItem(hRootNode);
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

			hItem = m_treectrl.GetNextSiblingItem(hItem);
			++nAllNum;
		}
	}
	else
	{
		nAllNum = 0;
		nCurSelNum = 0;
	}
}

CString CTreeManageDlg::GetCurSelTreeNodeName(const HTREEITEM hNode)
{
	return m_treectrl.GetItemText(hNode);
}

void CTreeManageDlg::GetTreeInfo(vector<TREE_GROUP>& vecTreeInf)
{
	// clear map struct
	m_vecTreeInfo.clear();

	// travel tree control
	m_nGroupShowOrder = 1;
	TravelTreeInfo(m_hRootPage);

	// get map struct info
	vecTreeInf = m_vecTreeInfo;
}

void CTreeManageDlg::TravelTreeInfo(const HTREEITEM hItem)
{
	const HTREEITEM hParent			= m_treectrl.GetParentItem(hItem);
	const HTREEITEM hGrandParent	= m_treectrl.GetParentItem(hParent);
	int	nCurVal		= 0;
	int	nParentVal	= 0;
	TREE_GROUP	stGroup;

	if (hItem != m_hRootPage)
	{
		if (hParent == m_hRootPage)
		{	// group nodes
			stGroup.nGroupId = m_treectrl.GetItemData(hItem);
			stGroup.nGroupShowOrder = m_nGroupShowOrder++;
			stGroup.vecPage.clear();
			m_vecTreeInfo.push_back(stGroup);
		}
		else if (hGrandParent == m_hRootPage)
		{	// page nodes
			nCurVal = m_treectrl.GetItemData(hItem);
			nParentVal = m_treectrl.GetItemData(hParent);

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

	if (m_treectrl.ItemHasChildren(hItem))
	{
		HTREEITEM hChildItem = m_treectrl.GetChildItem(hItem);
		while (hChildItem != NULL)
		{
			TravelTreeInfo(hChildItem);
			hChildItem = m_treectrl.GetNextItem(hChildItem, TVGN_NEXT);
		}
	}
}

//bool CTreeManageDlg::InitImgLib(void)
//{
//	HTREEITEM hImgItem	= NULL;
//
//	hImgItem = m_treectrl.InsertItem(_T("管道"), 12, 12, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20001);
//	//hImgItem = m_treectrl.InsertItem(_T("直线"), nImage, nSelImage, m_hRootImgLib, TVI_LAST);
//	//m_treectrl.SetItemData(hImgItem, -20002);
//	//hImgItem = m_treectrl.InsertItem(_T("矩形"), nImage, nSelImage, m_hRootImgLib, TVI_LAST);
//	//m_treectrl.SetItemData(hImgItem, -20003);
//	//hImgItem = m_treectrl.InsertItem(_T("圆形"), nImage, nSelImage, m_hRootImgLib, TVI_LAST);
//	//m_treectrl.SetItemData(hImgItem, -20004);
//	hImgItem = m_treectrl.InsertItem(_T("文字"), 13, 13, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20005);
//
//	//hImgItem = m_treectrl.InsertItem(_T("冷机"), nImage, nSelImage, m_hRootImgLib, TVI_LAST);
//	//m_treectrl.SetItemData(hImgItem, -20006);
//	//hImgItem = m_treectrl.InsertItem(_T("水泵"), nImage, nSelImage, m_hRootImgLib, TVI_LAST);
//	//m_treectrl.SetItemData(hImgItem, -20007);
//	//hImgItem = m_treectrl.InsertItem(_T("阀门"), nImage, nSelImage, m_hRootImgLib, TVI_LAST);
//	//m_treectrl.SetItemData(hImgItem, -20008);
//	//hImgItem = m_treectrl.InsertItem(_T("冷却塔"), nImage, nSelImage, m_hRootImgLib, TVI_LAST);
//	//m_treectrl.SetItemData(hImgItem, -20009);
//	hImgItem = m_treectrl.InsertItem(_T("自定义图元"), 14, 14, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20010);
//
//	hImgItem = m_treectrl.InsertItem(_T("按钮"), 15, 15, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20011);
//	hImgItem = m_treectrl.InsertItem(_T("管道弯头"), 16, 16, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20012);
//	hImgItem = m_treectrl.InsertItem(_T("仪表"), 17, 17, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20013);
//	hImgItem = m_treectrl.InsertItem(_T("折线图"), 18, 18, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20014);
//	hImgItem = m_treectrl.InsertItem(_T("柱状图"), 19, 19, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20015);
//
//	hImgItem = m_treectrl.InsertItem(_T("饼图"), 20, 20, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20016);
//	hImgItem = m_treectrl.InsertItem(_T("时间选择"), 21, 21, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20017);
//	hImgItem = m_treectrl.InsertItem(_T("时间序列柱状图"), 22, 22, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20018);
//	hImgItem = m_treectrl.InsertItem(_T("进度条"), 23, 23, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20019);
//	hImgItem = m_treectrl.InsertItem(_T("诊断图"), 24, 24, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20020);
//
//	hImgItem = m_treectrl.InsertItem(_T("点阵诊断图"), 25, 25, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20021);
//	hImgItem = m_treectrl.InsertItem(_T("区域变化"), 26, 26, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20022);
//	hImgItem = m_treectrl.InsertItem(_T("打开程序"), 27, 27, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20023);
//	hImgItem = m_treectrl.InsertItem(_T("嵌入式页面"), 28, 28, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20024);
//	hImgItem = m_treectrl.InsertItem(_T("液位"), 29, 29, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20025);
//	hImgItem = m_treectrl.InsertItem(_T("选择按钮"), 34, 34, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20026);
//	hImgItem = m_treectrl.InsertItem(_T("涂鸦板"), 35, 35, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20027);
//	hImgItem = m_treectrl.InsertItem(_T("历史趋势"), 36, 36, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20028);
//	hImgItem = m_treectrl.InsertItem(_T("节能措施ROI"), 37, 37, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20029);
//	hImgItem = m_treectrl.InsertItem(_T("地图导航"), 38, 38, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20030);
//	hImgItem = m_treectrl.InsertItem(_T("导航窗口"), 39, 39, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20031);
//	hImgItem = m_treectrl.InsertItem(_T("水平仪标尺"), 40, 40, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20032);
//	hImgItem = m_treectrl.InsertItem(_T("数据分析"), 41, 41, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20033);
//	hImgItem = m_treectrl.InsertItem(_T("温度分布"), 42, 42, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20034);
//	hImgItem = m_treectrl.InsertItem(_T("数据报表"), 43, 43, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20035);
//	hImgItem = m_treectrl.InsertItem(_T("平面温度分布"), 44, 44, m_hRootImgLib, TVI_LAST);
//	m_treectrl.SetItemData(hImgItem, -20036);
//
//	return true;
//}

bool CTreeManageDlg::IsThreadNameExist(const CString strThreadName)
{
	bool bFind = false;

	if (m_treectrl.ItemHasChildren(m_hRootTact))
	{
		HTREEITEM hItem = m_treectrl.GetChildItem(m_hRootTact);
		CString strthreadName__;
		while (hItem != NULL)
		{
			strthreadName__ = m_treectrl.GetItemText(hItem);
			strthreadName__ = strthreadName__.Left(strthreadName__.Find(':'));

			if (strthreadName__ == strThreadName)
			{
				bFind = true;
				break;
			}

			hItem = m_treectrl.GetNextSiblingItem(hItem);
		}
	}

	return bFind;
}

void CTreeManageDlg::SetConnectServFlag(const bool bIsConnect)
{
	m_bIsConnectServ = bIsConnect;
}

void CTreeManageDlg::OnMenuConnDebug()
{
	// TODO: Add your command handler code here
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
		vector<pair<CString,int>> vecName;
		vector<pair<CString,int>>::const_iterator iter;
		HTREEITEM hThread = NULL;
		vecName.clear();
		pView->m_PageDlg->m_paneArbiter.InitData(vecName);
		for (iter=vecName.begin(); iter!=vecName.end(); ++iter)
		{
			hThread = InsertNodeThread(iter->first, root_tactics);
			m_treectrl.SetItemData(hThread, -10001);
		}
		m_treectrl.Invalidate(FALSE);
		m_treectrl.Expand(m_hRootTact, TVE_EXPAND);
		//pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
		pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);


		// set flag
		SetConnectServFlag(true);

		// show config button
		//pView->m_PageDlg->m_paneArbiter.GetDlgItem(IDC_BUTTON_Config)->EnableWindow(SW_SHOW);

		// rename tactics node name
		CString strTacticsName;
		strTacticsName = m_treectrl.GetItemText(m_hRootTact);
		strTacticsName = strTacticsName.Left(strTacticsName.Find(_T("::")));
		strTacticsName += _T("::在线::");
		strTacticsName += dlg.m_strDbIp;
		m_treectrl.SetItemText(m_hRootTact, strTacticsName);

		// write info into ini file
		WritePrivateProfileString(_T("beopdebugtool"), _T("server"), dlg.m_strDbIp, strPath);
	}
}


void CTreeManageDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: Add your message handler code here

	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child Windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}


void CTreeManageDlg::OnUpdateMenuRunThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
}


void CTreeManageDlg::OnUpdateMenuStopThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
}


void CTreeManageDlg::OnOK()
{
}


void CTreeManageDlg::OnCancel()
{
}

HTREEITEM CTreeManageDlg::GetRootTactics(void)
{
	return m_hRootTact;
}


void CTreeManageDlg::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;
}


void CTreeManageDlg::OnMenuGroupUp()
{
	// TODO: Add your command handler code here
}


void CTreeManageDlg::OnMenuGroupDown()
{
	// TODO: Add your command handler code here
}


void CTreeManageDlg::OnMenuPageUp()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonMoveUp();
}


void CTreeManageDlg::OnMenuPageDown()
{
	// TODO: Add your command handler code here
	OnBnClickedButtonMoveDown();
}


void CTreeManageDlg::OnMenuPageProperty()
{
	// TODO: Add your command handler code here
	OnTreePageProperty();
}


void CTreeManageDlg::OnMenuUpdateLib()
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
	HTREEITEM hItem = m_treectrl.GetSelectedItem();

	GetTreeNodeNumInfo(m_hRootTact, hItem, nThreadAllNum, nThreadCurSelNum);
	pView->m_PageDlg->m_paneArbiter.SetThreadNum(nThreadAllNum, nThreadCurSelNum);
	pView->m_PageDlg->m_paneArbiter.OnBnClickedButtonRefreshLogicdll();

	m_pMainFrame->SetSaveFlag(true);
}


void CTreeManageDlg::OnMenuPageImport()
{
	// TODO: Add your command handler code here
	CImportPageSelectDlg dlg(false);
	dlg.m_hItem = m_hSelectedItem;
	dlg.DoModal();
}


void CTreeManageDlg::OnMenuRunAllThread()
{
	// TODO: Add your command handler code here
	assert(m_hRootTact != NULL);
	if (NULL == m_hRootTact)
	{
		return;
	}

	CString strthreadName;
	if (m_treectrl.ItemHasChildren(m_hRootTact))
	{
		HTREEITEM hItem = m_treectrl.GetChildItem(m_hRootTact);
		while (hItem != NULL)
		{
			strthreadName = GetCurSelTreeNodeName(hItem);
			strthreadName = strthreadName.Left(strthreadName.Find(':'));
			assert(m_pDataAccess != NULL);
			if (m_pDataAccess != NULL)
			{
				m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), _T("1"));
			}
			m_treectrl.SetItemText(hItem, strthreadName += _T("::(On)"));

			hItem = m_treectrl.GetNextSiblingItem(hItem);
		}
	}
}


void CTreeManageDlg::OnMenuStopAllThread()
{
	// TODO: Add your command handler code here
	assert(m_hRootTact != NULL);
	if (NULL == m_hRootTact)
	{
		return;
	}

	CString strthreadName;
	if (m_treectrl.ItemHasChildren(m_hRootTact))
	{
		HTREEITEM hItem = m_treectrl.GetChildItem(m_hRootTact);
		while (hItem != NULL)
		{
			strthreadName = GetCurSelTreeNodeName(hItem);
			strthreadName = strthreadName.Left(strthreadName.Find(':'));
			assert(m_pDataAccess != NULL);
			if (m_pDataAccess != NULL)
			{
				m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), _T("0"));
			}
			m_treectrl.SetItemText(hItem, strthreadName += _T("::(Off)"));

			hItem = m_treectrl.GetNextSiblingItem(hItem);
		}
	}
}


void CTreeManageDlg::OnUpdateMenuRunAllThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
}


void CTreeManageDlg::OnUpdateMenuStopAllThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
}


void CTreeManageDlg::OnUpdateMenuUpdateLib(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
}


void CTreeManageDlg::OnUpdateMenuAddThread(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CTreeManageDlg::OnMenuModifyThreadPerio()
{
	// TODO: Add your command handler code here
	CString strThreadName;
	wstring	strName;
	int		nPeriodicity = 0;
	strThreadName = GetCurSelTreeNodeName(m_hCurSelItem);
	strThreadName = strThreadName.Left(strThreadName.Find(':'));
	strName = strThreadName.GetString();

	CThreadPeriodicitySet dlg;
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	ASSERT(NULL != pView);
	if (NULL == pView)
	{
		return;
	}
	vector<CLogicDllThread*>& vecThread = pView->m_PageDlg->m_paneArbiter.m_pLogicManager->m_vecDllThreadList;
	vector<CLogicDllThread*>::const_iterator iterThread;
	for (iterThread=vecThread.begin(); iterThread!=vecThread.end(); ++iterThread)
	{
		if ((*iterThread)->GetName() == strName)
		{	// find it
			CDLLObject* pDllObj = (*iterThread)->GetDllObject(0);
			ASSERT(NULL != pDllObj);
			if (NULL != pDllObj)
			{
				nPeriodicity = pDllObj->GetTimeSpan();
			}
			break;
		}
	}

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
	}
}


void CTreeManageDlg::OnUpdateMenuModifyThreadPerio(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConnectServ);
}

bool CTreeManageDlg::ShowPageByPageID( const int nPageID,const int nElementID,const int nImageID )
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

void CTreeManageDlg::OnMenuTemplatePageImport()
{
	CImportTemplatePageDlg dlg;
	dlg.m_hItem = m_hSelectedItem;
	dlg.DoModal();
}


void CTreeManageDlg::OnMenuPageAddtoTemplate()
{
	// TODO: Add your command handler code here
	// load page info
	if (!m_bExportPageLoad)
	{
		m_pMainFrame->m_pPrjSqlite->LoadPictureLibSqlite();
		m_pMainFrame->m_pPrjSqlite->LoadAnimationSqlite();

		m_bExportPageLoad = true;
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

	DWORD_PTR dwPageId = m_treectrl.GetItemData(m_hSelectedItem);
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

void CTreeManageDlg::SetDeviceType(const int nDeviceType)
{
	m_nDeviceType = nDeviceType;
}


void CTreeManageDlg::OnUpdateMenuGroupDel(CCmdUI *pCmdUI)
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


void CTreeManageDlg::OnUpdateMenuPageDel(CCmdUI *pCmdUI)
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


void CTreeManageDlg::OnUpdateMenuPageProperty(CCmdUI *pCmdUI)
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

BOOL CTreeManageDlg::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message== WM_KEYDOWN)
	{
		if((GetKeyState( VK_CONTROL) & 0xFF00 ) == 0xFF00)   
		{  
			if( pMsg->wParam == 'S' || pMsg->wParam == 's')  
			{  
				m_pMainFrame->SaveProjectContain();
				return true;  
			} 
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CTreeManageDlg::OnBackupLogicthreadStatus()
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


void CTreeManageDlg::OnRestoreLogicthreadStatus()
{
	// TODO: Add your command handler code here
}
