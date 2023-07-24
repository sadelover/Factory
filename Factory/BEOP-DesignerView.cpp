// BEOP-DesignerView.cpp : implementation of the CBEOPDesignerView class
//
#include "stdafx.h"     
#include "MainFrm.h"
#include "BEOP-Designer.h"
#include "BEOP-DesignerDoc.h"
#include "BEOP-DesignerView.h"
#include "PageDlg.h"
#include "TreeManageDlg.h"
#include "CommandManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TABCTRLID 1001
#define EDITCTRLID 1002
// CBEOPDesignerView
IMPLEMENT_DYNCREATE(CBEOPDesignerView, CView)
BEGIN_MESSAGE_MAP(CBEOPDesignerView, CView)
    // Standard printing commands
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOVE()
    ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_ZOOMOUT, &CBEOPDesignerView::OnZoomout)
	ON_COMMAND(ID_ZOOMIN, &CBEOPDesignerView::OnZoomin)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOUT, &CBEOPDesignerView::OnUpdateZoomout)
	ON_UPDATE_COMMAND_UI(ID_ZOOMIN, &CBEOPDesignerView::OnUpdateZoomin)
END_MESSAGE_MAP()
// CBEOPDesignerView construction/destruction
CBEOPDesignerView::CBEOPDesignerView()
: m_nPageCount(0)
,m_pTreeDlg(NULL)
,m_PageDlg(NULL)
{
	// TODO: add construction code here
	m_PageDlg = new CPageDlg();
	//m_pTreeDlg = new CTreeManageDlg();
}
CBEOPDesignerView::~CBEOPDesignerView()
{
    SAFE_DELETE(m_PageDlg);
	SAFE_DELETE(m_pTreeDlg);
}
BOOL CBEOPDesignerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}
// CBEOPDesignerView drawing
void CBEOPDesignerView::OnDraw(CDC* pDC)
{
	
}

// CBEOPDesignerView printing
BOOL CBEOPDesignerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBEOPDesignerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBEOPDesignerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// CBEOPDesignerView diagnostics
#ifdef _DEBUG
void CBEOPDesignerView::AssertValid() const
{
	CView::AssertValid();
}
void CBEOPDesignerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

// CBEOPDesignerView message handlers
int CBEOPDesignerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	//m_pTreeDlg->Create(IDD_DIALOG_TREE_DLG, this);
	//m_pTreeDlg->ShowWindow(SW_SHOW);
	m_PageDlg->Create(IDD_PAGEDLG, this);
	m_PageDlg->ShowWindow(SW_SHOW);
	m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
	return 0;
}

void CBEOPDesignerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	CRect rect;
	//m_pTreeDlg->GetWindowRect(&rect);
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	if (pMainFrame != NULL)
	{
		//m_pTreeDlg->MoveWindow(0,0,280,cy);
		m_PageDlg->MoveWindow(0,0, cx-rect.Width(), cy, TRUE);
		m_PageDlg->Invalidate(FALSE);
		m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
	}
}

BOOL CBEOPDesignerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;  
	GetClientRect(rect);  
	DWORD backColor = RGB(127, 127, 127);
	pDC->FillSolidRect(rect,backColor);
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}
HBRUSH CBEOPDesignerView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CView::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_EDIT )   
	{     //   IDC_XXX_STATIC　为你想要设为透明的控件ＩＤ   
		pDC->SetBkMode(TRANSPARENT);   
		if ( NULL != m_hBrush )
		{
			DeleteObject(m_hBrush);
			m_hBrush = NULL;
		}
		m_hBrush = ::CreateSolidBrush(RGB(199, 199, 199));
		return m_hBrush;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CBEOPDesignerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnLButtonDown(nFlags, point);
}

void CBEOPDesignerView::OnZoomout()
{
	// TODO: Add your command handler code here
	m_PageDlg->OnZoomOut();

	//added by david
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	int cursel = (pMainFrame->m_DrawContext.m_fScreenScale - 0.5)*10;
	int combosize = pMainFrame->GetComboSelSize();
	pMainFrame->SetComboSel(cursel > combosize ? combosize : cursel );
}
void CBEOPDesignerView::OnZoomin()
{
	// TODO: Add your command handler code here
	m_PageDlg->OnZoomIn();
	//added by david
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	int cursel = (pMainFrame->m_DrawContext.m_fScreenScale - 0.5)*10;
	int combosize = pMainFrame->GetComboSelSize();
	pMainFrame->SetComboSel(cursel > combosize ? combosize : cursel );
}
void CBEOPDesignerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);

	if (nChar == VK_ESCAPE){
		pMainFrame->EndFullScreen();
		return;
	}

	if ( NULL == pMainFrame->m_DrawContext.pPage ){
		return;
	}

	if ( EQMDRAW_MOVE == pMainFrame->m_DrawContext.m_nDrawMode && nChar == 'A'  && GetAsyncKeyState(VK_CONTROL) )//CTRL+A键
	{
		m_PageDlg->m_dlgCanvas.OnKeySelectAll();
		m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
		return;
	}
	if ( EQMDRAW_MOVE == pMainFrame->m_DrawContext.m_nDrawMode && nChar == VK_LEFT  && GetAsyncKeyState(VK_CONTROL) )
	{
		if(!pMainFrame->m_DrawContext.m_bIsChangeSize)
			CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
		pMainFrame->m_DrawProxy.ShiftEqmtsLeftCtrl(&pMainFrame->m_DrawContext);
		m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
		return;
	}
	if ( EQMDRAW_MOVE == pMainFrame->m_DrawContext.m_nDrawMode && nChar == VK_RIGHT  && GetAsyncKeyState(VK_CONTROL) )
	{
		if(!pMainFrame->m_DrawContext.m_bIsChangeSize)
			CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
		pMainFrame->m_DrawProxy.ShiftEqmtsRightCtrl(&pMainFrame->m_DrawContext);
		m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
		return;
	}
	if ( EQMDRAW_MOVE == pMainFrame->m_DrawContext.m_nDrawMode && nChar == VK_UP  && GetAsyncKeyState(VK_CONTROL) )
	{
		if(!pMainFrame->m_DrawContext.m_bIsChangeSize)
			CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
		pMainFrame->m_DrawProxy.ShiftEqmtsUpCtrl(&pMainFrame->m_DrawContext);
		m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
		return;
	}
	if ( EQMDRAW_MOVE == pMainFrame->m_DrawContext.m_nDrawMode && nChar == VK_DOWN  && GetAsyncKeyState(VK_CONTROL) )
	{
		if(!pMainFrame->m_DrawContext.m_bIsChangeSize)
			CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
		pMainFrame->m_DrawProxy.ShiftEqmtsDownCtrl(&pMainFrame->m_DrawContext);
		m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
		return;
	}
	switch ( nChar )
	{
	case 'O':
		OnZoomout();
		break;
	case 'I':
		OnZoomin();
		break;;
	case VK_DELETE:
		m_PageDlg->m_dlgCanvas.OnKeyDelete();
		break;
	case VK_UP:
		{
			if(!pMainFrame->m_DrawContext.m_bIsChangeSize)
				CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
			pMainFrame->m_DrawProxy.ShiftEqmtsUp(&pMainFrame->m_DrawContext);
		}
		break;
	case VK_DOWN:
		{
			if(!pMainFrame->m_DrawContext.m_bIsChangeSize)
				CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
			pMainFrame->m_DrawProxy.ShiftEqmtsDown(&pMainFrame->m_DrawContext);
		}
		break;
	case VK_LEFT:
		{
			if(!pMainFrame->m_DrawContext.m_bIsChangeSize)
				CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
			pMainFrame->m_DrawProxy.ShiftEqmtsLeft(&pMainFrame->m_DrawContext);
		}
		break;
	case VK_RIGHT:
		{
			if(!pMainFrame->m_DrawContext.m_bIsChangeSize)
				CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
			pMainFrame->m_DrawProxy.ShiftEqmtsRight(&pMainFrame->m_DrawContext);
		}
		break;
	case VK_RETURN:
		{
			pMainFrame->m_DrawProxy.KeyEnterDown();
		}
	/*case VK_END:
		m_wndLeft->InsertItem(m_nPageCount++, _T("页面XXX"));
		m_wndLeft->SetCurFocus(m_nPageCount-1);
		Invalidate(FALSE);
		break;
	case VK_HOME:
		m_wndLeft->DeleteItem(--m_nPageCount);
		m_wndLeft->SetCurFocus(m_nPageCount-1);*/
	default:
		break;
	}
	m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBEOPDesignerView::OnUpdateZoomout(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	pCmdUI->Enable(NULL != pMainFrame->m_DrawContext.pPage && pMainFrame->m_DrawContext.m_fScreenScale < 1.5);
}
void CBEOPDesignerView::OnUpdateZoomin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	pCmdUI->Enable(NULL != pMainFrame->m_DrawContext.pPage && pMainFrame->m_DrawContext.m_fScreenScale > 0.5);
}


void CBEOPDesignerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	pMainFrame->SetWindowTextW(pMainFrame->GetAppName().GetString());
}
