// PageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BEOP-Designer.h"
#include "PageDlg.h"
#include "MainFrm.h"

// CPageDlg dialog

IMPLEMENT_DYNAMIC(CPageDlg, CDialog)

CPageDlg::CPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPageDlg::IDD, pParent)
{
	m_PosLButtonDown.x = 0;
	m_PosLButtonDown.y = 0;

	m_rtCanvas = CRect(0, 0, 500, 300);
	m_pDataAccess = NULL;
	m_vecServerPointList.clear();
	m_EventStopThread = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hupdatedatahandle = NULL;
	m_bExitThread = false;
}

CPageDlg::~CPageDlg()
{
}

void CPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CPageDlg message handlers

BOOL CPageDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CDialog::OnEraseBkgnd(pDC);
	CRect rt;
	GetClientRect(&rt);
	DWORD backColor = RGB(191, 191, 191);
	pDC->FillSolidRect(rt,backColor);
	return TRUE;
}

void CPageDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
	switch (nSBCode)
	{
	case SB_BOTTOM:
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*10);
		scrollinfo.nPos = scrollinfo.nMax;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_TOP:
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*10);
		scrollinfo.nPos = scrollinfo.nMin;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_LINEUP:
		scrollinfo.nPos -= 1;
		if (scrollinfo.nPos<scrollinfo.nMin){
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,10);
		break;
	case SB_LINEDOWN:
		scrollinfo.nPos += 1;
		if (scrollinfo.nPos>scrollinfo.nMax){
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,-10);
		break;
	case SB_PAGEUP:
		scrollinfo.nPos -= 5;
		if (scrollinfo.nPos<scrollinfo.nMin){
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,10*5);
		break;
	case SB_PAGEDOWN:
		scrollinfo.nPos += 5;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,-10*5);
		break;
	case SB_ENDSCROLL:
		// MessageBox("SB_ENDSCROLL");
		break;
	case SB_THUMBPOSITION:
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*10);
		// scrollinfo.nPos = nPos;
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_THUMBTRACK:
		ScrollWindow(0,(scrollinfo.nPos-nPos)*10);
		scrollinfo.nPos = nPos;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPageDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
	switch (nSBCode)
	{
	case SB_LEFT:
		ScrollWindow((scrollinfo.nPos-scrollinfo.nMin)*10,0);
		scrollinfo.nPos = scrollinfo.nMin;
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		break;
	case SB_RIGHT:
		ScrollWindow((scrollinfo.nPos-scrollinfo.nMax)*10,0);
		scrollinfo.nPos = scrollinfo.nMax;
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		break;
	case SB_LINELEFT:
		scrollinfo.nPos -= 1;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		ScrollWindow(10,0);
		break;
	case SB_LINERIGHT:
		scrollinfo.nPos += 1;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		ScrollWindow(-10,0);
		break;
	case SB_PAGELEFT:
		scrollinfo.nPos -= 5;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		ScrollWindow(10*5,0);
		break;
	case SB_PAGERIGHT:
		scrollinfo.nPos += 5;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		ScrollWindow(-10*5,0);
		break;
	case SB_THUMBPOSITION:
		break;
	case SB_THUMBTRACK:
		ScrollWindow((scrollinfo.nPos-nPos)*10,0);
		scrollinfo.nPos = nPos;
		SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL);
		break;
	case SB_ENDSCROLL:
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CPageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//set vertical scroll max
	m_dlgCanvas.Create(IDD_CANVASDLG, this);
	m_dlgCanvas.ShowWindow(TRUE);
	m_paneArbiter.Create(IDD_DIALOG_ARBITER, this);
	m_paneArbiter.ShowWindow(FALSE);
	m_paneCalc.Create(IDD_DIALOG_CALC_MODULE, this);
	m_paneCalc.ShowWindow(FALSE);

	//
	m_paneChart.Create(IDD_CURVETAB_DIALOG, this);
	m_paneChart.ShowWindow(FALSE);

	m_paneCommDebug.Create(IDD_DIALOG_COM, this);
	m_paneCommDebug.ShowWindow(FALSE);

	m_paneCommSet.Create(IDD_DIALOG_SETTABLE, this);
	m_paneCommSet.ShowWindow(FALSE);

	m_pPanePoint = new CPointDialog;
	m_pPanePoint->Create(IDD_DIALOG_Point);
	m_pPanePoint->ShowWindow(FALSE);
	
	CRect rect;
	GetClientRect(&rect);
	m_rtCanvas = rect;
	int xRange = rect.Width()/2 - m_rtCanvas.Width()/2 > 0 ? (rect.Width()/2 - m_rtCanvas.Width()/2 > 50 ? 0 : 50 - rect.Width()/2 + m_rtCanvas.Width()/2) : -rect.Width()/2 + m_rtCanvas.Width()/2 +50;
	int yRange = rect.Height()/2 - m_rtCanvas.Height()/2 > 0 ? (rect.Height()/2 - m_rtCanvas.Height()/2 > 30 ? 0 : 30 - rect.Width()/2 + m_rtCanvas.Width()/2) : -rect.Width()/2 + m_rtCanvas.Width()/2+30;
	SetScrollRange(SB_HORZ,  0,  2*xRange/10); 
	SetScrollPos(SB_HORZ, xRange);
	SetScrollRange(SB_VERT,  0,  2*yRange/10);  
	SetScrollPos(SB_VERT, yRange);
	return TRUE;
}

void CPageDlg::OnSize(UINT nType, int cx, int cy)
{
	if ( m_dlgCanvas.m_hWnd )
	{
		CRect rect;
		GetClientRect(&rect);
		
		int xRange = rect.Width()/2 - m_rtCanvas.Width()/2 > 0 ? (rect.Width()/2 - m_rtCanvas.Width()/2 > 80 ? 0 : 80 - rect.Width()/2 + m_rtCanvas.Width()/2) : -rect.Width()/2 + m_rtCanvas.Width()/2 +80;
		int yRange = rect.Height()/2 - m_rtCanvas.Height()/2 > 0 ? (rect.Height()/2 - m_rtCanvas.Height()/2 > 50 ? 0 : 50 - rect.Height()/2 + m_rtCanvas.Height()/2) : -rect.Height()/2 + m_rtCanvas.Height()/2+50;
		SetScrollRange(SB_HORZ,  0,  2*xRange/10); 
		SetScrollPos(SB_HORZ, xRange/10);
		SetScrollRange(SB_VERT,  0,  2*yRange/10);  
		SetScrollPos(SB_VERT, yRange/10);
		int x = rect.CenterPoint().x - m_rtCanvas.Width()/2;
		int y = rect.CenterPoint().y - m_rtCanvas.Height()/2;
		m_dlgCanvas.MoveWindow(x ,y ,m_rtCanvas.Width(),m_rtCanvas.Height(),TRUE);

		m_dlgCanvas.Invalidate(FALSE);
	}
	CDialog::OnSize(nType, cx, cy);
}

BOOL CPageDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);

	if ( zDelta ){
		scrollinfo.nPos = scrollinfo.nPos == scrollinfo.nMax ? scrollinfo.nPos : scrollinfo.nPos + 1;
	}
	else{
		scrollinfo.nPos = scrollinfo.nPos == scrollinfo.nMin ? scrollinfo.nPos : scrollinfo.nPos - 1;
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CPageDlg::OnZoomOut()
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	if(  NULL == pMainFrame->m_DrawContext.pPage ) return;
	pMainFrame->m_DrawContext.m_fScreenScale += 0.1;
	if ( pMainFrame->m_DrawContext.m_fScreenScale > 1.5 )
	{
		pMainFrame->m_DrawContext.m_fScreenScale = 1.5;
	}

	DWORD screenwidth = 0;
	DWORD screenheight = 0;

	m_rtCanvas.bottom = pMainFrame->m_DrawContext.GetScaledheight();
	m_rtCanvas.right  = pMainFrame->m_DrawContext.GetScaledWidth();

	SendMessage(WM_SIZE);

	ResetArbiterPosition();
}

void CPageDlg::OnZoomIn()
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	if(  NULL == pMainFrame->m_DrawContext.pPage ) return;
	pMainFrame->m_DrawContext.m_fScreenScale -= 0.1;
	if ( pMainFrame->m_DrawContext.m_fScreenScale < 0.5 )
	{
		pMainFrame->m_DrawContext.m_fScreenScale = 0.5;
	}
	DWORD screenwidth = 0;
	DWORD screenheight = 0;

	/*if (pMainFrame->m_DrawContext.pPage->GetPageType() == FULLSCREEN)
	{
		screenwidth = pMainFrame->m_DrawContext.GetScreenWidth();
		screenheight = pMainFrame->m_DrawContext.GetScreenHeight();
	}
	else
	{
		screenwidth = pMainFrame->m_DrawContext.pPage->GetPageWidth();
		screenheight = pMainFrame->m_DrawContext.pPage->GetPageHeight();
	}
	m_rtCanvas.bottom = static_cast<long>(pMainFrame->m_DrawContext.m_fScreenScale*(long)screenheight);
	m_rtCanvas.right  = static_cast<long>(pMainFrame->m_DrawContext.m_fScreenScale*(long)screenwidth);*/

	m_rtCanvas.bottom = pMainFrame->m_DrawContext.GetScaledheight();
	m_rtCanvas.right  = pMainFrame->m_DrawContext.GetScaledWidth();

	SendMessage(WM_SIZE);
	
	ResetArbiterPosition();
}

void CPageDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	point.x /= pMainFrame->m_DrawContext.m_fScreenScale;
	point.y /= pMainFrame->m_DrawContext.m_fScreenScale;
	pMainFrame->m_DrawContext.m_bDragging = FALSE;
	pMainFrame->m_DrawContext.m_bStartChangeSize = FALSE;
	vector<Gdiplus::PointF> vecPt;
	switch ( pMainFrame->m_DrawContext.m_nDrawMode )
	{
	case EQMDRAW_MOVE:
		m_dlgCanvas.OnLButtonUpSelectBox(nFlags, point);
		break;
	default:
		break;
	}
	m_bLButtonDown = false;
	m_dlgCanvas.m_bLButtonDown = false;
	CDialog::OnLButtonUp(nFlags, point);
}


void CPageDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	if (m_dlgCanvas.m_bLButtonDown)
	{
		CPoint pt = point;
		ClientToScreen(&pt);
		m_dlgCanvas.ScreenToClient(&pt);
		pMainFrame->m_DrawContext.actPt.X = pt.x/pMainFrame->m_DrawContext.m_fScreenScale;
		pMainFrame->m_DrawContext.actPt.Y = pt.y/pMainFrame->m_DrawContext.m_fScreenScale;	
		pMainFrame->m_DrawContext.m_bDragging = FALSE;
		pMainFrame->m_DrawContext.m_ifSelectBox = true;
		m_dlgCanvas.Invalidate(FALSE);
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CPageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bLButtonDown = true;
	CPoint pt = point;
	ClientToScreen(&pt);
	m_PosLButtonDown.x = pt.x;
	m_PosLButtonDown.y = pt.y;
	CDialog::OnLButtonDown(nFlags, point);
}

void CPageDlg::ResetArbiterPosition(void)
{
	CRect rect;
	m_paneArbiter.GetWindowRect(rect);

	::MoveWindow(m_paneArbiter.GetSafeHwnd(), 0, 0, rect.Width(), rect.Height(), TRUE);
}

bool CPageDlg::InitData()
{	
	m_pDataAccess->ReadServerPointsList(m_vecServerPointList);

	StartUpdateThread();

	//新实时曲线表指针传递;
	m_paneChart.m_pServerDataAccess = m_pDataAccess;
	m_paneChart.m_pBEOPGatewayDlg = this;
	m_paneChart.InitData();
		
	m_pPanePoint->m_pDataAccess = m_pDataAccess;
	m_pPanePoint->m_pGatewayDlg = this;
	m_pPanePoint->InitData();

	//
	m_paneCommSet.m_pDataAccess = m_pDataAccess;
	m_paneCommSet.InitData();

	m_paneCommDebug.InitData();
	if(m_pPanePoint->m_strWatchPointLineList.size()>0)
		m_paneChart.DataPointVector(m_pPanePoint->m_strWatchPointLineList);
	m_paneChart.CurvePageInitPointView();
	////m_CurveTable.m_CurvePage1.InitPointView();

	return true;
}

UINT WINAPI CPageDlg::ThreadFuncUpdateData( LPVOID lparam )
{
	CPageDlg* pThis = (CPageDlg*)lparam;
	if (pThis != NULL)
	{
		while(TRUE)
		{
			if(pThis->m_bExitThread)
			{
				SetEvent(pThis->m_EventStopThread);
				return 0;
			}

			//更新点值 , 注意只在主界面线程中做
			pThis->m_pDataAccess->UpdatePhyPointValRecord();

			if(pThis->m_bExitThread)
			{
				SetEvent(pThis->m_EventStopThread);
				return 0;
			}

			Sleep(2*1000);
		}
	}
	return 0;
}

void CPageDlg::StartUpdateThread()
{
	m_hupdatedatahandle = (HANDLE)_beginthreadex(NULL, 0, ThreadFuncUpdateData, this, NORMAL_PRIORITY_CLASS, NULL);
}

bool CPageDlg::RefreshServerPointList()
{
	if(m_pDataAccess)
	{
		vector<DataPointEntry> vecServerPointList;
		if(m_pDataAccess->ReadServerPointsList(vecServerPointList))
		{
			m_vecServerPointList = vecServerPointList;
			return true;
		}
	}
	return false;
}
