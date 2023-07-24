

// CanvasDlg.cpp : implementation file
//
#include "stdafx.h"
#include "BEOP-Designer.h"
#include "CanvasDlg.h"
#include "GdiPlus.h"
#include "MainFrm.h"
#include "MetaFileInfoDlg.h"
#include "PipeInfoDlg.h"
#include "TextInfoDlg.h"
#include "GraphicInfoDlg.h"
#include "BEOP-DesignerView.h"
#include "PageDlg.h"
#include "../ComponentDraw/EqmDrawDevice.h"
#include "../ComponentDraw/EqmDrawPipe.h"
#include "../ComponentDraw/EqmDrawText.h"
#include "../ComponentDraw/EqmDrawGraphic.h"
#include "../ComponentDraw/EqmDrawGraphicRect.h"
#include "../ComponentDraw/EqmDrawGraphicLine.h"
#include "../ComponentDraw/EqmDrawGraphicEllipse.h"
#include "../ComponentDraw/EqmDrawGraphicPolygon.h"
#include "../ComponentDraw/EqmDrawBend.h"
#include "../ComponentDraw/EqmDrawDashBoard.h"
#include "../ComponentDraw/EqmDrawLineGraphic.h"
#include "../ComponentDraw/EqmDrawBarGraphic.h"
#include "../ComponentDraw/EqmDrawPieGraphic.h"
#include "../ComponentDraw/EqmDrawButton.h"
#include "../ComponentDraw/EqmDrawTimeSelect.h"
#include "../ComponentDraw/EqmDrawTimeBar.h"
#include "../ComponentDraw/EqmDrawProgressBar.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphic.h"
#include "../ComponentDraw/EqmDrawPolygon.h"
#include "../ComponentDraw/EqmDrawAreaChange.h"
#include "../ComponentDraw/EqmDrawShellExecute.h"
#include "../ComponentDraw/EqmDrawEmbededPage.h"
#include "../ComponentDraw/EqmDrawLiquidLevel.h"
#include "../ComponentDraw/EqmDrawButtonPlus.h"
#include "../ComponentDraw/EqmDrawScrawl.h"
#include "../ComponentDraw/EqmDrawEnergySaveRoi.h"
#include "../ComponentDraw/EqmDrawDataReport.h"
#include "../ComponentDraw/EqmDrawPlaneTempeDistr.h"
#include "../ComponentDraw/EqmDrawDottedLineFrm.h"
#include "../ComponentDraw/EqmDrawGeneralComponent.h"
#include "../ComponentDraw/EqmDrawRectangle.h"
#include "../ComponentDraw/EqmDrawStraightLine.h"
#include "Tools/CustomTools/CustomTools.h"
#include <process.h>   //add 10.9
#include "MetaFileSelectDlg.h"
#include "ChillerSelectDlg.h"
#include "ValveSelectDlg.h"
#include "ChartPropertyDlg.h"
#include "DashBoardPropertyDlg.h"
#include "BendPropertyDlg.h"
#include "MeterPanSelectDlg.h"
#include "MetaButtonSelectDlg.h"
//#include "MetaBtnSetDlg.h"
#include "TimeSelectPropertyDlg.h"
#include "TimeBarPropertyDlg.h"
#include "ProgressBarProperty.h"
#include "DiagnosePropertySettingDlg.h"
#include "DiagnoseChartPropertySettingPlusDlg.h"
#include "AreaChagePropertyDlg.h"
#include "ShellExecuteSettingDlg.h"
#include "SetEqmPositionDlg.h"
#include "EmbededPageSettingDlg.h"
#include "LiquidLevelSettingDlg.h"
#include "CheckButtonPlusSettingDlg.h"
#include "HistoryTrendSettingDlg.h"
#include "EnergySaveRoiSettingDlg.h"
#include "MapNavigateSettingDlg.h"
#include "NavigateWindowSettingDlg.h"
#include "LevelRulerSettingDlg.h"
#include "DataAnalyseSettingDlg.h"
#include "TemperDistriSettingDlg.h"
#include "DataReportSettingDlg.h"
#include "PlaneTempeDistrSettingDlg.h"
#include "GeneralComponentSettingDlg.h"
#include "BendConfigDlg.h"

#include "CommandManager.h"
#include "VPointSimpleSettingDlg.h"
#include "StandardEquip.h"
#include "Tools/DirectoryOperations.h"
#include "EqmRectangleConfigDlg.h"
// CCanvasDlg dialog
#pragma warning(disable:4244)
IMPLEMENT_DYNAMIC(CCanvasDlg, CDialog)

//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCanvasDlg, CDialog)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEWHEEL()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_SHOWWINDOW()  //bird
	ON_MESSAGE(WM_MOUSELEAVE,  &CCanvasDlg::OnMouseLeave)
    ON_COMMAND(ID_RMENU_SELETEALL, &CCanvasDlg::OnRmenuSeleteall)
    ON_COMMAND(ID_RMENU_DELETE, &CCanvasDlg::OnRmenuDelete)
    ON_COMMAND(ID_RMENU_PROPERTY, &CCanvasDlg::OnRmenuProperty)
    ON_COMMAND(ID_RMENU_LEFTROTATE, &CCanvasDlg::OnRmenuLeftrotate)
    ON_COMMAND(ID_RMENU_RIGHTROTATE, &CCanvasDlg::OnRmenuRightrotate)
    ON_UPDATE_COMMAND_UI(ID_RMENU_LEFTROTATE, &CCanvasDlg::OnUpdateRmenuLeftrotate)
    ON_UPDATE_COMMAND_UI(ID_RMENU_RIGHTROTATE, &CCanvasDlg::OnUpdateRmenuRightrotate)
	ON_COMMAND(ID_POSITION, &CCanvasDlg::OnPosition)
	ON_COMMAND(ID_RMENU_ADDTO_IMGLIB, &CCanvasDlg::OnRmenuAddtoImglib)
	ON_COMMAND(ID_RMENU_COPY, &CCanvasDlg::OnRmenuCopy)
	ON_COMMAND(ID_RMENU_PASTE, &CCanvasDlg::OnRmenuPaste)
	ON_COMMAND(IDM_ADD_BINDPOINT_TO_VPOINT_TABLE, &CCanvasDlg::OnAddBindpointToVpointTable)
	ON_COMMAND(ID_RMENU_CANCEL_DRAW, &CCanvasDlg::OnRmenuCanclDraw)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

CCanvasDlg::CCanvasDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCanvasDlg::IDD, pParent)
,m_hThreadDraw(NULL)
,m_bExistThreadDraw(false)
,m_bStartDragging(false)
,m_ptPaste(0, 0)
{
	m_scale = 1.0;
	m_trackleave = FALSE;
	m_bLButtonDown = false;
}

CCanvasDlg::~CCanvasDlg()
{
    m_bExistThreadDraw = false;
	if (m_hThreadDraw != NULL)
	{
		CloseHandle(m_hThreadDraw);
	}
}

void CCanvasDlg::OnDestroy()
{
  //  m_bExistThreadDraw = false;

    __super::OnDestroy();
}

void CCanvasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CCanvasDlg::OnInitDialog()
{
	GetSysPath(m_strSysPath);

    const BOOL ifok = __super::OnInitDialog();
    return ifok;
}

void CCanvasDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default	
	m_bLButtonDown = true;
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	if ( NULL == pMainFrame->m_DrawContext.pPage )
		return;
	CPoint actpoint = point;
	point.x /= pMainFrame->m_DrawContext.m_fScreenScale;
	point.y /= pMainFrame->m_DrawContext.m_fScreenScale;
	if ( -1 == pMainFrame->m_MaxEqmtID )
	{
		pMainFrame->m_MaxEqmtID = pMainFrame->m_DrawContext.pPage->GetObjectsMaxId() +1;
	}
	pMainFrame->m_DrawContext.actPt.X = point.x;
	pMainFrame->m_DrawContext.actPt.Y = point.y;
	switch ( pMainFrame->m_DrawContext.m_nDrawMode )
	{
	case EQMDRAW_STRAIGHTPIPE:
	case EQMDRAW_FOLDPIPE:
		pMainFrame->SetSaveFlag(true);
		CreatePipe(point);
		break;
	case GraphicType_LINE:
		pMainFrame->SetSaveFlag(true);
		Create2DLine(point);
		break;
	case GraphicType_RECTANGLE:
	case GraphicType_ARROW:
	case GraphicType_TRIANGLE:
	case GraphicType_ELLIPSE:
		pMainFrame->SetSaveFlag(true);
		pMainFrame->m_DrawContext.m_bIsCreate2DGraph = true;
		pMainFrame->m_DrawContext.prePoint.X = point.x;
		pMainFrame->m_DrawContext.prePoint.Y = point.y;
		break;
	case EQMDRAW_FORMAT_BRUSH:
		pMainFrame->SetSaveFlag(true);
		OnLButtonDownInFormatBrush(nFlags, point);
		break;
	case EQMDRAW_MOVE:
		pMainFrame->SetSaveFlag(true);
		OnLButtonDownInMoveMode(nFlags, point);
		break;
	default:
		break;
	}
	
	ShowPropertyWnd(actpoint);
	CDialog::OnLButtonDown(nFlags, point);
}
BOOL CCanvasDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
BOOL CCanvasDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;  
	GetClientRect(rect);  
	pDC->FillSolidRect(rect,RGB(219,219,219));
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
void CCanvasDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CBEOPDesignerView* pView = (CBEOPDesignerView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	if (pView->m_PageDlg->m_bLButtonDown)
	{
		m_bLButtonDown = true;
	}
	if (!m_trackleave)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		_TrackMouseEvent(&tme);
		m_trackleave = TRUE;
	}

    const bool ifLButDown =  ((nFlags & MK_LBUTTON)==MK_LBUTTON); //判断左键是否按下
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	
	
	
	point.x /= pMainFrame->m_DrawContext.m_fScreenScale;
	point.y /= pMainFrame->m_DrawContext.m_fScreenScale;

	/*CPoint actpoint = point;
	pMainFrame->m_DrawContext.actPt.X = actpoint.x;
	pMainFrame->m_DrawContext.actPt.Y = actpoint.y;*/
	pMainFrame->m_DrawContext.actPt.X = point.x;
	pMainFrame->m_DrawContext.actPt.Y = point.y;
	if (pMainFrame->m_DrawContext.pPage != NULL)
	{
		CEqmDrawBase *pEqmDrawBase = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
		int nPosEqmX = -1;
		int nPosEqmY = -1;
		int nWidth = -1;
		int nHeight = -1;
		if (pEqmDrawBase)
		{
			nPosEqmX = pEqmDrawBase->GetPos().X;
			nPosEqmY = pEqmDrawBase->GetPos().Y;
			nWidth = pEqmDrawBase->GetWidth();
			nHeight = pEqmDrawBase->GetHeight();
		}
		CString pointString;
		pointString.Format(L"MousePos:x=%d,y=%d|EqmPos:x=%d,y=%d|EqmSize:width=%d,height=%d\n",point.x,point.y,nPosEqmX,nPosEqmY,nWidth,nHeight);
		pMainFrame->m_wndStatusBar.SetPaneText(0,pointString);
	}

	

	///update the pos for property window
	//CEqmDrawAreaChange * pArea = static_cast<CEqmDrawAreaChange * >(pEqmDrawBase);
	CPropertiesWnd &pWind = pMainFrame->m_wndProperties;	
	//if (pArea)
	{
		if (/*pWind.m_pArea == pArea&&*/ pWind.m_eShowMode == E_SHOW_AREA_CHANGE)
		{
			if (pWind.m_pArea != NULL)
			{
				int x = pWind.m_pArea->GetPos().X;
				int y = pWind.m_pArea->GetPos().Y;
				int w = pWind.m_pArea->GetWidth();
				int h = pWind.m_pArea->GetHeight();
				PosInfo p(x,y,w,h);
				if (!p.IsEqual(pWind.m_areaPos))
				{
					pWind.SetAreaChangeProperty(pWind.m_pArea);				
				}	
			}							
		}
	}

	switch ( pMainFrame->m_DrawContext.m_nDrawMode )
	{
	case EQMDRAW_MOVE:
		{
			//pMainFrame->m_DrawContext.actPt.X = point.x;
			//pMainFrame->m_DrawContext.actPt.Y = point.y;	
			if(ifLButDown)
			{
				if (pView)
				{
					if (pView->m_PageDlg->m_bLButtonDown)
					{
						CRect rt;
						GetWindowRect(&rt);
						pMainFrame->m_DrawContext.ClearSelect();
						if (pView->m_PageDlg->m_PosLButtonDown.x < rt.left)
						{
							pMainFrame->m_DrawContext.PrePt.X = -200;
						}
						else if (pView->m_PageDlg->m_PosLButtonDown.x >= rt.left && pView->m_PageDlg->m_PosLButtonDown.x <= rt.right)
						{
							CPoint pt;
							pt.x = pView->m_PageDlg->m_PosLButtonDown.x;
							pt.y = pView->m_PageDlg->m_PosLButtonDown.y;
							ScreenToClient(&pt);
							pMainFrame->m_DrawContext.PrePt.X = pt.x/pMainFrame->m_DrawContext.m_fScreenScale;
						}
						else
						{
							pMainFrame->m_DrawContext.PrePt.X = 2000;
						}
						if (pView->m_PageDlg->m_PosLButtonDown.y < rt.top)
						{
							pMainFrame->m_DrawContext.PrePt.Y = -200;
						}
						else if (pView->m_PageDlg->m_PosLButtonDown.y >= rt.top && pView->m_PageDlg->m_PosLButtonDown.y <= rt.bottom)
						{
							CPoint pt;
							pt.x = pView->m_PageDlg->m_PosLButtonDown.x;
							pt.y = pView->m_PageDlg->m_PosLButtonDown.y;
							ScreenToClient(&pt);
							pMainFrame->m_DrawContext.PrePt.Y = pt.y/pMainFrame->m_DrawContext.m_fScreenScale;
						}
						else
						{
							pMainFrame->m_DrawContext.PrePt.Y = 1200;
						}
						pMainFrame->m_DrawContext.m_bDragging = FALSE;
						pMainFrame->m_DrawContext.m_ifSelectBox = true;
					}
				}
				Invalidate(FALSE);
			}
		}
		break;
	case EQMDRAW_CHILLER:
		SetCursor(AfxGetApp()->LoadCursor(IDC_CHILLER));
		return;
	case EQMDRAW_PUMP:
		SetCursor(AfxGetApp()->LoadCursor(IDC_PUMP));
		return;
	case EQMDRAW_COOLINGTOWER:
		SetCursor(AfxGetApp()->LoadCursor(IDC_CT));
		return;
	case EQMDRAW_VALVE:
		SetCursor(AfxGetApp()->LoadCursor(IDC_VALVE));
		return;
	case EQMDRAW_TEXT:
		SetCursor(AfxGetApp()->LoadCursor(IDC_TEXT));
		return;
	default:
		break;
	}
	//pMainFrame->m_DrawContext.actPt.X = point.x;
	//pMainFrame->m_DrawContext.actPt.Y = point.y;
	if ( pMainFrame->m_DrawProxy.DoMoveAct(ifLButDown, nFlags, &pMainFrame->m_DrawContext) )
	{	
		//CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
		m_pAct = pMainFrame->m_DrawContext.actPt;
		m_bStartDragging = true;
		Invalidate(FALSE);
	}

	if(!pMainFrame->m_DrawContext.m_ifSelectBox && !pView->m_PageDlg->m_bLButtonDown)
	{
		pMainFrame->m_DrawContext.PrePt = pMainFrame->m_DrawContext.actPt; 
	}
	if ( 1 == pMainFrame->m_DrawContext.vecSelectEqmt.size())
	{
		if ( pMainFrame->m_DrawContext.m_bStartChangeSize )
		{
			pMainFrame->m_DrawContext.actPt.X = point.x;
			pMainFrame->m_DrawContext.actPt.Y = point.y;
			pMainFrame->m_DrawContext.vecSelectEqmt[0]->ChangeSize(Gdiplus::PointF(point.x, point.y),
				(Direction)pMainFrame->m_DrawContext.m_ChangeDirection);
			m_pAct = Gdiplus::PointF(point.x, point.y);
			//CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
			Invalidate(FALSE);
		}
		else
		{
			ChangeCursorToChangeSize(pMainFrame, point);
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}
void CCanvasDlg::ChangeCursorToChangeSize( CMainFrame * pMainFrame, CPoint &point )
{
    pMainFrame->m_DrawContext.actPt.X = point.x;
    pMainFrame->m_DrawContext.actPt.Y = point.y;
    Direction temp = pMainFrame->m_DrawProxy.PickObjectSide(&pMainFrame->m_DrawContext);
    switch ( temp )
    {
    case Dir_Left:
    case Dir_Right:
        pMainFrame->m_DrawContext.m_bIsChangeSize = TRUE;
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
        break;
    case Dir_Up:
    case Dir_Down:
        pMainFrame->m_DrawContext.m_bIsChangeSize = TRUE;
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
        Invalidate(FALSE); //test
        break;
    case Dir_LeftUp:
    case Dir_RightDown:
        pMainFrame->m_DrawContext.m_bIsChangeSize = TRUE;
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
        break;
    case Dir_RightUp:
    case Dir_LeftDown:
        pMainFrame->m_DrawContext.m_bIsChangeSize = TRUE;
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
        break;
    default:
        //ADEBUG(_T("ChangeCursorToChangeSize  default") );
        pMainFrame->m_DrawContext.m_bIsChangeSize = FALSE;
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        break;
    }
}
void CCanvasDlg::CreateRectangle( CPoint &point, vector<Gdiplus::PointF> &vecPt )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	Gdiplus::PointF startPt, endPt;
	startPt.X = pMainFrame->m_DrawContext.prePoint.X > point.x ? point.x:pMainFrame->m_DrawContext.prePoint.X;
	startPt.Y = pMainFrame->m_DrawContext.prePoint.Y > point.y ? point.y:pMainFrame->m_DrawContext.prePoint.Y;
	endPt.X = pMainFrame->m_DrawContext.prePoint.X > point.x ? pMainFrame->m_DrawContext.prePoint.X:point.x;
	endPt.Y = pMainFrame->m_DrawContext.prePoint.Y > point.y ? pMainFrame->m_DrawContext.prePoint.Y:point.y;
	vecPt.push_back(startPt);
	vecPt.push_back(endPt);
	if(abs(startPt.X-endPt.X) < 5 || abs(startPt.Y-endPt.Y) < 5){
		pMainFrame->m_DrawContext.m_bIsCreate2DGraph = false;
		return;
	}
	CEqmDrawGraphic *pEqmDrawGraphic = new CEqmDrawGraphicRect(); //CEqmDrawGraphic
	pEqmDrawGraphic->SetLineWidth(2);
	pEqmDrawGraphic->SetLineColor(0);
	pEqmDrawGraphic->SetPointList(vecPt);
	pEqmDrawGraphic->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqmDrawGraphic->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);

	pEqmDrawGraphic->SetID(++pMainFrame->m_MaxEqmtID);
	pMainFrame->m_DrawContext.pPage->Add2DGraphic(pEqmDrawGraphic);
	CommandManager::Instance()->CallCommand(pEqmDrawGraphic,E_COMMAND_ADD);
	pMainFrame->m_DrawContext.m_bIsCreate2DGraph = false;
}

void CCanvasDlg::CreateEllipse( CPoint &point, vector<Gdiplus::PointF> &vecPt )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	Gdiplus::PointF startPt, endPt;
	startPt.X = pMainFrame->m_DrawContext.prePoint.X > point.x ? point.x:pMainFrame->m_DrawContext.prePoint.X;
	startPt.Y = pMainFrame->m_DrawContext.prePoint.Y > point.y ? point.y:pMainFrame->m_DrawContext.prePoint.Y;
	endPt.X = pMainFrame->m_DrawContext.prePoint.X > point.x ? pMainFrame->m_DrawContext.prePoint.X:point.x;
	endPt.Y = pMainFrame->m_DrawContext.prePoint.Y > point.y ? pMainFrame->m_DrawContext.prePoint.Y:point.y;
	vecPt.push_back(startPt);
	vecPt.push_back(endPt);
	if(abs(startPt.X-endPt.X)<5||abs(startPt.Y-endPt.Y)<5)
	{
		pMainFrame->m_DrawContext.m_bIsCreate2DGraph = false;
		return;
	}
	CEqmDrawGraphic *pEqmDrawGraphic = new CEqmDrawGraphicEllipse(); //CEqmDrawGraphic
	pEqmDrawGraphic->SetLineWidth(2);
	pEqmDrawGraphic->SetLineColor(0);
	pEqmDrawGraphic->SetPointList(vecPt);
	pEqmDrawGraphic->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqmDrawGraphic->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqmDrawGraphic->SetID(++pMainFrame->m_MaxEqmtID);

	pMainFrame->m_DrawContext.pPage->Add2DGraphic(pEqmDrawGraphic);
	CommandManager::Instance()->CallCommand(pEqmDrawGraphic,E_COMMAND_ADD);
	pMainFrame->m_DrawContext.m_bIsCreate2DGraph = false;
}

void CCanvasDlg::CreatePipe( CPoint point )
{
	CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return;
	}
	//如果在靠近边界时会获取到0,0。函数要获取一个网格，但是四角的网格坐标获取不到。
	CPoint ptNew = point;//GetClosestGridPosition(point);
	if ( pMainFrame->m_DrawContext.bIsCreatePipe )
	{
		CEqmDrawPipe *pipe = dynamic_cast<CEqmDrawPipe*>(pMainFrame->m_DrawContext.pNewEqmt);
		if(GetAsyncKeyState(VK_SHIFT)&0x8000)
		{
			Gdiplus::PointF pt;
			pt.X = abs(pipe->GetPointList()[pipe->GetPointList().size()-1].X-ptNew.x) > abs(pipe->GetPointList()[pipe->GetPointList().size()-1].Y - ptNew.y) ? ptNew.x : pipe->GetPointList()[pipe->GetPointList().size()-1].X;
			pt.Y = abs(pipe->GetPointList()[pipe->GetPointList().size()-1].X-ptNew.x) > abs(pipe->GetPointList()[pipe->GetPointList().size()-1].Y - ptNew.y) ? pipe->GetPointList()[pipe->GetPointList().size()-1].Y : ptNew.y;
			pipe->GetPointList().push_back(pt);
		}
		else{
			pipe->GetPointList().push_back(PointF(ptNew.x,ptNew.y));
		}
		pMainFrame->m_DrawContext.bIsCreatePipe = false;
		if ( NULL != pipe && pipe->GetPointList().size() > 1 ) //if the pipe contains less than two points,we will delete this pipe
		{
			pMainFrame->m_DrawContext.pPage->AddPipe(pipe);
			SetMetaSelect(pipe, pMainFrame);
			CommandManager::Instance()->CallCommand(pipe,E_COMMAND_ADD);
		}
		pMainFrame->m_DrawContext.pNewEqmt = NULL;
		Invalidate(FALSE);
	}
	else
	{
		pMainFrame->m_DrawContext.bIsCreatePipe = true;
		CEqmDrawPipe *pNewPipe = new CEqmDrawPipe();
		pNewPipe->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
		pNewPipe->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
		pNewPipe->SetID(++pMainFrame->m_MaxEqmtID);
		pNewPipe->GetPointList().push_back(Gdiplus::PointF(ptNew.x, ptNew.y));
		pNewPipe->SetPipeColor(pMainFrame->m_DrawContext.GetDefaultPipeColor());
		pNewPipe->SetPipeWidth(pMainFrame->m_DrawContext.GetDefaultPipeWidth());
		pMainFrame->m_DrawContext.pNewEqmt = pNewPipe;
		//CommandManager::Instance()->CallCommand(pNewPipe,E_COMMAND_ADD);
	}
}

bool CCanvasDlg::CreateDevice(CPoint &point, const int nID, const bool bIsPic)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawDevice *pEqm = new CEqmDrawDevice();
    ASSERT(pEqm);
    if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
    pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
    pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
    pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
    pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetPicID(nID);
	//const wchar_t* pName = pMainFrame->m_pPrjSqlite->GetPictureNameByID(nID);
	//if (pName)
	//{
	//	pEqm->SetEqmName(pName);
	//}
    Image* pBitmap = NULL;
	if (bIsPic)
	{
		pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nID, true);
	}
	else
	{
		pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIdFromAnimationImageLib(nID, true);
	}

    if(pBitmap == NULL) 
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}
	pEqm->SetBitmap(pBitmap);
	pEqm->SetWidth(pBitmap->GetWidth());
    pEqm->SetHeight(pBitmap->GetHeight());
	pEqm->SetImageMetaType(bIsPic);
    pMainFrame->m_DrawContext.pPage->AddDevice(pEqm);
	SetMetaSelect(pEqm, pMainFrame);
	CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	return true;
}

void CCanvasDlg::CreateText( CPoint &point )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawText *pEqmText = new CEqmDrawText();
	ASSERT(pEqmText);
	pEqmText->SetFontColor(0);
	pEqmText->SetFontSize(16);
	pEqmText->SetBorderType(1);
	pEqmText->SetFontName(L"微软雅黑");
	pEqmText->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqmText->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqmText->SetID(++pMainFrame->m_MaxEqmtID);
	pEqmText->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqmText->SetWidth(100);
	pEqmText->SetHeight(25);
	pEqmText->SetTextAttribute(TEXT_ATTR_NO_USE);
	pMainFrame->m_DrawContext.pPage->AddText(pEqmText);
	SetMetaSelect(pEqmText, pMainFrame);
	CommandManager::Instance()->CallCommand(pEqmText,E_COMMAND_ADD);
}

void CCanvasDlg::Create2DLine( CPoint point )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	if ( pMainFrame->m_DrawContext.m_bIsCreate2DGraph )
	{
		CEqmDrawGraphic *pLine = dynamic_cast<CEqmDrawGraphic *>(pMainFrame->m_DrawContext.pNewEqmt);
		if ( pLine )
		{
			if ( abs(pLine->GetPointList()[pLine->GetPointList().size()-1].X - point.x) > abs(pLine->GetPointList()[pLine->GetPointList().size()-1].Y - point.y) )
				point.y = pLine->GetPointList()[pLine->GetPointList().size()-1].Y;
			else
				point.x = pLine->GetPointList()[pLine->GetPointList().size()-1].X;
			if(pLine->GetPointList().size()>=2)
			{
				if((pLine->GetPointList()[pLine->GetPointList().size()-1].X==pLine->GetPointList()[pLine->GetPointList().size()-2].X)&&
					(pLine->GetPointList()[pLine->GetPointList().size()-1].X==point.x))
				{

				}
				else if((pLine->GetPointList()[pLine->GetPointList().size()-1].Y==pLine->GetPointList()[pLine->GetPointList().size()-2].Y)&&
					(pLine->GetPointList()[pLine->GetPointList().size()-1].Y==point.y))
				{

				}
				else
					pLine->GetPointList().push_back(Gdiplus::PointF(point.x, point.y));
			}
			else
			{
				pLine->GetPointList().push_back(Gdiplus::PointF(point.x, point.y));
			}
		}

		CommandManager::Instance()->CallCommand(pLine,E_COMMAND_ADD);
	}
	else
	{
		Gdiplus::PointF pt(point.x, point.y);

		if ( pt.X != 0 && pt.Y != 0 )
		{
			pMainFrame->m_DrawContext.m_bIsCreate2DGraph = true;
			CEqmDrawGraphic *pNewLine = new CEqmDrawGraphicLine();  //CEqmDrawGraphic
			pNewLine->SetLineWidth(2);
			pNewLine->SetLineColor(0);
			pNewLine->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
			pNewLine->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
			pNewLine->SetID(++pMainFrame->m_MaxEqmtID);
			pNewLine->GetPointList().push_back(pt);			

			pMainFrame->m_DrawContext.pNewEqmt = pNewLine;
			CommandManager::Instance()->CallCommand(pNewLine,E_COMMAND_ADD);
		}
	}
}

//单击鼠标左键。
void CCanvasDlg::OnLButtonDownInMoveMode( UINT nFlags, CPoint point )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	pMainFrame->m_DrawContext.m_ifSelectBox = false;
	
    if ( pMainFrame->m_DrawContext.m_bIsChangeSize )
	{
		pMainFrame->m_DrawContext.m_ChangeDirection = pMainFrame->m_DrawProxy.PickObjectSide(&pMainFrame->m_DrawContext);
		pMainFrame->m_DrawContext.m_bStartChangeSize = TRUE;
		m_pPre.X = point.x;
		m_pPre.Y = point.y;
	}
	else
	{
		if ( nFlags & MK_CONTROL )
		{
			pMainFrame->m_DrawContext.m_bDragging = TRUE;
			CEqmDrawBase *pEqmt = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
			if ( NULL == pEqmt )
			{
				return;
			}
			if ( !pEqmt->IsSelected())
			{
				pEqmt->SetSelected(true);
				if ( !pMainFrame->m_DrawContext.vecSelectEqmt.empty() )
				{
					pMainFrame->m_DrawContext.vecSelectEqmt[pMainFrame->m_DrawContext.vecSelectEqmt.size()-1]->SetMainAlign(false);
				}
				pEqmt->SetMainAlign(true);
				pMainFrame->m_DrawContext.vecSelectEqmt.push_back(pEqmt);
				pMainFrame->SetFocus();
			}
			else
			{
				/*CEqmDrawBase *pBase = pMainFrame->m_DrawContext.vecSelectEqmt[pMainFrame->m_DrawContext.vecSelectEqmt.size()-1];
				pBase->SetMainAlign(false);
				pEqmt->SetMainAlign(true);
				if ( pBase != pEqmt )
				{
				pMainFrame->m_DrawContext.vecSelectEqmt[pMainFrame->m_DrawContext.vecSelectEqmt.size()-1] = pEqmt;
				for ( size_t i = 0; i < pMainFrame->m_DrawContext.vecSelectEqmt.size(); i++ ){
				if ( (pEqmt == pMainFrame->m_DrawContext.vecSelectEqmt[i]) && (i != pMainFrame->m_DrawContext.vecSelectEqmt.size()-1) )
				{
				pMainFrame->m_DrawContext.vecSelectEqmt[i] = pBase;
				}
				}
				}*/

				for(size_t i = 0; i < pMainFrame->m_DrawContext.vecSelectEqmt.size(); i++ )
				{
					if(pEqmt == pMainFrame->m_DrawContext.vecSelectEqmt[i])
					{
						pMainFrame->m_DrawContext.vecSelectEqmt.erase(pMainFrame->m_DrawContext.vecSelectEqmt.begin()+i);
						pEqmt->SetSelected(false);
					}
				}
				if(pMainFrame->m_DrawContext.vecSelectEqmt.size()>0)
				{
					CEqmDrawBase *pBase = pMainFrame->m_DrawContext.vecSelectEqmt[pMainFrame->m_DrawContext.vecSelectEqmt.size()-1];
					pBase->SetMainAlign(true);
				}
			}

			SetCursorRange(point);
			Invalidate(FALSE);		
		}
		else
		{
			CBEOPDesignerView* pView = (CBEOPDesignerView*)(((CMainFrame*)AfxGetMainWnd())->GetActiveView());
			CEqmDrawBase *pEqmt = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
			if (NULL == pEqmt)
			{
				if (!pView->m_PageDlg->m_bLButtonDown)
				{
					pMainFrame->m_DrawContext.ClearSelect();
					pMainFrame->m_DrawContext.m_bDragging = FALSE;
					pMainFrame->m_DrawContext.PrePt.X     = point.x;
					pMainFrame->m_DrawContext.PrePt.Y     = point.y;
					pMainFrame->m_DrawContext.m_ifSelectBox = true;
					Invalidate(FALSE);
				}
			}
			else
			{
				if (pEqmt->IsSelected())
				{
					//start dragging
					pMainFrame->m_DrawContext.m_bDragging = TRUE;
					pMainFrame->m_DrawContext.PrePt.X     = point.x;
					pMainFrame->m_DrawContext.PrePt.Y     = point.y; 
					m_pPre = pMainFrame->m_DrawContext.PrePt;
				}
				else
				{
					pMainFrame->m_DrawContext.ClearSelect();

					pEqmt->SetSelected(true);
					pEqmt->SetMainAlign(true);
					pMainFrame->m_DrawContext.vecSelectEqmt.push_back(pEqmt);
					pMainFrame->SetFocus();
					Invalidate(FALSE);
				}

				SetCursorRange(point);
			}
		}
	}
}

void CCanvasDlg::OnLButtonUpSelectBox( UINT nFlags, CPoint point )
{
	bool bRepaint = false;

	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	if(!pMainFrame->m_DrawContext.m_ifSelectBox )
		return;
	
	Gdiplus::PointF prepoint = pMainFrame->m_DrawContext.PrePt;
	if (point.x != prepoint.X && point.y != prepoint.Y){
		bRepaint = true;
	}

	pMainFrame->m_DrawContext.m_ifSelectBox = false;
	//start to count items
	vector<CEqmDrawBase*> itemArray;
	pMainFrame->m_DrawProxy.PickObjectArray(&pMainFrame->m_DrawContext, itemArray);
	for (UINT itemIdx=0; itemIdx< itemArray.size(); itemIdx++)
	{
		CEqmDrawBase *pEqmt = itemArray[itemIdx];
		pEqmt->SetSelected(true);
		if ( !pMainFrame->m_DrawContext.vecSelectEqmt.empty() )
		{
			pMainFrame->m_DrawContext.vecSelectEqmt[pMainFrame->m_DrawContext.vecSelectEqmt.size()-1]->SetMainAlign(false);
		}
		pEqmt->SetMainAlign(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back(pEqmt);

		bRepaint = true;
	}

	if (bRepaint){
		Invalidate(FALSE);
	}
}

void CCanvasDlg::OnLButtonDownInFormatBrush(UINT nFlags, CPoint point)
{
	CMainFrame *pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	CEqmDrawBase* pFormatBrush = pMainFrm->m_pFormatBrush;
	if (NULL == pFormatBrush)
	{
		return;
	}


	CEqmDrawBase* pEqmt = pMainFrm->m_DrawProxy.PickObject(pMainFrm->m_DrawContext.pPage,(double)pMainFrm->m_DrawContext.actPt.X,(double)pMainFrm->m_DrawContext.actPt.Y);
	if (NULL == pEqmt)
	{	// click on blank
		pMainFrm->m_DrawContext.ClearSelect();
		pMainFrm->m_DrawContext.m_bDragging = FALSE;
		pMainFrm->m_DrawContext.PrePt.X     = point.x;
		pMainFrm->m_DrawContext.PrePt.Y     = point.y;
		pMainFrm->m_DrawContext.m_ifSelectBox = true;
		pMainFrm->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		Invalidate(FALSE);
		return;
	}

	if (pFormatBrush->GetEqmType() != pEqmt->GetEqmType())
	{	// choose different meta
		return;
	}
	CommandManager::Instance()->CallCommand(pEqmt,E_COMMAND_EDIT);
	pEqmt->CopyInFormatBrush(pFormatBrush);

	pEqmt->SetSelected(true);
	pMainFrm->m_DrawContext.vecSelectEqmt.push_back(pEqmt);

	Invalidate(FALSE);
}

void CCanvasDlg::OnKeySelectAll()
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	if (NULL == pMainFrame)
	{
		return;
	}
	if (NULL == pMainFrame->m_DrawContext.pPage)
	{
		return;
	}

	pMainFrame->m_DrawContext.ClearSelect();
	for (list<CEqmDrawDevice*>::iterator it = pMainFrame->m_DrawContext.pPage->GetDrawDeviceList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetDrawDeviceList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));
	}//设备
	for (list<CEqmDrawPipe*>::iterator it = pMainFrame->m_DrawContext.pPage->GetPagePipeList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetPagePipeList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));
	}//管道
	for (list<CEqmDrawText*>::iterator it = pMainFrame->m_DrawContext.pPage->GetPageTextList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetPageTextList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//文字
	for ( list<CEqmDrawGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->Get2DGraphicList().begin();
		it != pMainFrame->m_DrawContext.pPage->Get2DGraphicList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//二维图形
	for ( list<CEqmDrawBarGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->GetBarGraphList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetBarGraphList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//柱状图
	for ( list<CEqmDrawBend*>::iterator it = pMainFrame->m_DrawContext.pPage->GetBendList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetBendList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//弯头
	for ( list<CEqmDrawDashBoard*>::iterator it = pMainFrame->m_DrawContext.pPage->GetDashboardList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetDashboardList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//仪表盘
	for ( list<CEqmDrawLineGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->GetLineGraphList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetLineGraphList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//折线图
	for ( list<CEqmDrawPieGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->GetPieGraphList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetPieGraphList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//饼图
	for ( list<CEqmDrawButton*>::iterator it = pMainFrame->m_DrawContext.pPage->GetButtonList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetButtonList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//按钮
	for ( list<CEqmDrawTimeSelect*>::iterator it = pMainFrame->m_DrawContext.pPage->GetTimeSelectList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetTimeSelectList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//时间选择
	for ( list<CEqmDrawTimeBar*>::iterator it = pMainFrame->m_DrawContext.pPage->GetTimeBarList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetTimeBarList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//时间柱状图
	for ( list<CEqmDrawProgressBar*>::iterator it = pMainFrame->m_DrawContext.pPage->GetProgressBarList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetProgressBarList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//进度条
	for ( list<CEqmDrawDiagnoseGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->GetDiagnoseList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetDiagnoseList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//诊断图
	for ( list<CEqmDrawPolygon*>::iterator it = pMainFrame->m_DrawContext.pPage->GetPolygon().begin();
		it != pMainFrame->m_DrawContext.pPage->GetPolygon().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}//点阵诊断图

	//区域变化
	for ( list<CEqmDrawAreaChange*>::iterator it = pMainFrame->m_DrawContext.pPage->GetAreaChangeList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetAreaChangeList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}

	//打开程序
	for ( list<CEqmDrawShellExecute*>::iterator it = pMainFrame->m_DrawContext.pPage->GetShellExecuteList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetShellExecuteList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//嵌入式页面
	for ( list<CEqmDrawEmbededPage*>::iterator it = pMainFrame->m_DrawContext.pPage->GetEmbededPageList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetEmbededPageList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//液位
	for ( list<CEqmDrawLiquidLevel*>::iterator it = pMainFrame->m_DrawContext.pPage->GetLiquidLevelList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetLiquidLevelList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//选择按钮
	for ( list<CEqmDrawButtonPlus*>::iterator it = pMainFrame->m_DrawContext.pPage->GetCheckButtonPlusList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetCheckButtonPlusList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//涂鸦板
	for ( list<CEqmDrawScrawl*>::iterator it = pMainFrame->m_DrawContext.pPage->GetScrawlList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetScrawlList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//历史趋势
	for ( list<CEqmDrawHistoryTrend*>::iterator it = pMainFrame->m_DrawContext.pPage->GetHistoryTrendList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetHistoryTrendList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//方形
	for ( list<CEqmDrawRectangle*>::iterator it = pMainFrame->m_DrawContext.pPage->GetRectangleList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetRectangleList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}

	//直线
	for ( list<CEqmDrawStraightLine*>::iterator it = pMainFrame->m_DrawContext.pPage->GetStraightLineList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetStraightLineList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}

	//地图导航
	for ( list<CEqmDrawMapNavigate*>::iterator it = pMainFrame->m_DrawContext.pPage->GetMapNavigateList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetMapNavigateList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//导航窗口
	for ( list<CEqmDrawNavigateWindow*>::iterator it = pMainFrame->m_DrawContext.pPage->GetNavigateWindowList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetNavigateWindowList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//水平仪标尺
	for ( list<CEqmDrawLevelRuler*>::iterator it = pMainFrame->m_DrawContext.pPage->GetLevelRulerList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetLevelRulerList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//数据分析
	for ( list<CEqmDrawDataAnalyse*>::iterator it = pMainFrame->m_DrawContext.pPage->GetDataAnalyseList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetDataAnalyseList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//温度分布
	for ( list<CEqmDrawTemperDistri*>::iterator it = pMainFrame->m_DrawContext.pPage->GetTemperDistriList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetTemperDistriList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
	
	//数据报表
	for ( list<CEqmDrawDataReport*>::iterator it = pMainFrame->m_DrawContext.pPage->GetDataReportList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetDataReportList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}

	//平面温度分布
	for ( list<CEqmDrawPlaneTempeDistr*>::iterator it = pMainFrame->m_DrawContext.pPage->GetPlaneTempeDistrList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetPlaneTempeDistrList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}

	//虚线框
	for ( list<CEqmDrawDottedLineFrm*>::iterator it = pMainFrame->m_DrawContext.pPage->GetDottedLineFrmList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetDottedLineFrmList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}

	//通用组件
	for ( list<CEqmDrawGeneralComponent*>::iterator it = pMainFrame->m_DrawContext.pPage->GetGeneralComponentList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetGeneralComponentList().end(); it++)
	{
		(*it)->SetSelected(true);
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
	}
}
void CCanvasDlg::OnKeyDelete()
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	pMainFrame->m_DrawContext.UpdateCopiedList(pMainFrame->m_DrawContext.vecSelectEqmt);
	CommandManager::Instance()->CallCommand(pMainFrame->m_DrawContext.vecSelectEqmt,E_COMMAND_DELETE);
	for ( UINT nIndex = 0; nIndex < pMainFrame->m_DrawContext.vecSelectEqmt.size(); nIndex++ )
	{
		CEqmDrawBase* pElement = pMainFrame->m_DrawContext.vecSelectEqmt[nIndex];
		if (pElement != NULL)
		{
			//pMainFrame->m_pPrjSqlite->DeleteElement(pElement->GetEqmType(), pElement->GetPageID(),pElement->GetID());//不应在删除键直接从数据库删除
			//pMainFrame->m_DrawContext.pPage->DeleteBase(pElement);
			pElement->SetDeleteFlag(true);
			pElement->SetNeedSave(true);
		}
	}
	pMainFrame->m_wndProperties.SetNullProperty();
	pMainFrame->m_DrawContext.m_bStartChangeSize = false;
	pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
	pMainFrame->m_DrawContext.vecSelectEqmt.clear();
}

void CCanvasDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());     
	ASSERT(pMainFrame);  
	//if(!pMainFrame->m_bUpdateScreen)				//在线登陆调试期间不刷界面 防止崩溃
	//	return;

	if ( NULL == pMainFrame->m_DrawContext.pPage )
		return;
	CRect rc;
	GetClientRect(&rc);
	pMainFrame->m_DrawProxy.setRect(rc);
	pMainFrame->m_DrawProxy.init(this, NULL);
	pMainFrame->m_DrawContext.m_showType = SHOWTYPE_DRAW;
	pMainFrame->m_DrawContext.m_bIsChangeBackgroundColor = false;
	pMainFrame->m_DrawContext.type = REDRAW;
	pMainFrame->m_DrawProxy.draw(&pMainFrame->m_DrawContext);
}

void CCanvasDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default	
	ClipCursor(NULL);	//释放鼠标

	m_bLButtonDown = false;
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	point.x /= pMainFrame->m_DrawContext.m_fScreenScale;
	point.y /= pMainFrame->m_DrawContext.m_fScreenScale;
	if(pMainFrame->m_DrawContext.m_bDragging && m_bStartDragging && pMainFrame->m_DrawContext.vecSelectEqmt.size()>0)		//移动模式
	{
		CommandManager::Instance()->CallMoveCommand(pMainFrame->m_DrawContext.vecSelectEqmt,m_pPre, m_pAct);
		m_bStartDragging = false;			//开始拖动标志
	}

	if(pMainFrame->m_DrawContext.m_bStartChangeSize && pMainFrame->m_DrawContext.vecSelectEqmt.size()>0)		//改变大小
	{
		CommandManager::Instance()->CallChangeSizeCommand(pMainFrame->m_DrawContext.vecSelectEqmt[0],m_pPre, m_pAct, (Direction)(pMainFrame->m_DrawContext.m_ChangeDirection));
	}

	pMainFrame->m_DrawContext.m_bDragging = FALSE;
	pMainFrame->m_DrawContext.m_bStartChangeSize = FALSE;
	vector<Gdiplus::PointF> vecPt;
	switch ( pMainFrame->m_DrawContext.m_nDrawMode )
	{
	case EQMDRAW_CHILLER:
		{
			CChillerSelectDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				int nPicId = 0;
				InsertImageIntoOpenS3db(dlg.m_nID, nPicId);
				CreateDevice(point, nPicId);
			}
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_PUMP:
		{
			CreateDevice(point, 27);
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_COOLINGTOWER:
		{
			CreateDevice(point, 28);
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_VALVE:
		{
			CValveSelectDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				int nPicId = 0;
				InsertImageIntoOpenS3db(dlg.m_nID, nPicId);
				CreateDevice(point, nPicId);
			}
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_METAFILE:
		{
			CMetaFileSelectDlg dlg(this);
			dlg.SetWndType(e_picture);
			dlg.SetCurrentPicID(pMainFrame->m_nSelectPicID);
			if (dlg.DoModal() == IDOK)
			{
				vector<int>	vecId = dlg.GetVectorPicNum();
				vector<int>::const_iterator	iter;
				int i = 0;
				bool bIsPic = (e_picture == dlg.GetWndType()) ? true : false;
				int nUseId = 0;

				for (iter=vecId.begin(); iter!=vecId.end(); ++iter,++i)
				{
					point.x += i * 50;
					point.y += i * 50;

					nUseId = 0;
					if (bIsPic)
					{
						InsertImageIntoOpenS3db(*iter, nUseId);
						CreateDevice(point, nUseId, true);
					}
					else
					{
						CProjectSqlite* pPrjSql = pMainFrame->m_pPrjSqlite;
						if (pPrjSql != NULL)
						{
							if (pPrjSql->InsertAnimationIntoOpenS3dbFromId(*iter, nUseId))
							{
								CreateDevice(point, nUseId, false);
							}
						}
					}
				}
				pMainFrame->m_nSelectPicID = nUseId;
			}
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_METABUTTON://在界面中初始化一个按钮
		{
			CMetaButtonSelectDlg dlg(TRUE, this);
			if (dlg.DoModal() == IDOK)
			{
				int nPicId[5] = {0};
				nPicId[0] = dlg.m_nIDPicComm;
				nPicId[1] = dlg.m_nIDPicDisable;
				nPicId[2] = dlg.m_nIDPicDown;
				nPicId[3] = dlg.m_nIDPicOver;
				nPicId[4] = dlg.m_nICPicChecked;

				//InsertImageIntoOpenS3db(dlg.m_nIDPicComm, nPicId[0]);
				//InsertImageIntoOpenS3db(dlg.m_nIDPicDisable, nPicId[1]);
				//InsertImageIntoOpenS3db(dlg.m_nIDPicDown, nPicId[2]);
				//InsertImageIntoOpenS3db(dlg.m_nIDPicOver, nPicId[3]);
				//InsertImageIntoOpenS3db(dlg.m_nICPicChecked, nPicId[4]);

				//InsertImageIntoOpenS3dbFromFile(dlg.m_picInfo[0], nPicId[0]);
				//InsertImageIntoOpenS3dbFromFile(dlg.m_picInfo[1], nPicId[1]);
				//InsertImageIntoOpenS3dbFromFile(dlg.m_picInfo[2], nPicId[2]);
				//InsertImageIntoOpenS3dbFromFile(dlg.m_picInfo[3], nPicId[3]);
				//InsertImageIntoOpenS3dbFromFile(dlg.m_picInfo[4], nPicId[4]);

				CEqmDrawButton* pButton = CreateButton(point, nPicId[0], nPicId[1], nPicId[2], nPicId[3], dlg.m_nLinkPageID, dlg.m_strBindSetPointName, nPicId[4]);
				//CEqmDrawButton* pButton = CreateButtonNewStyle(point, nPicId[0], nPicId[1], nPicId[2], nPicId[3], dlg.m_nLinkPageID, dlg.m_strBindSetPointName, nPicId[4], dlg.m_picInfo[0].pBitmap);
				if (pButton != NULL)
				{
					pButton->SetLinkPageID(dlg.m_nLinkPageID);
					pButton->Set5PicID(nPicId[0], nPicId[1], nPicId[2], nPicId[3], nPicId[4]);
					pButton->SetBindSetPointName(dlg.m_strBindSetPointName);
					pButton->SetTextContent(dlg.m_strTextContent);
					pButton->SetTextColor(dlg.m_nTextColor);
					pButton->SetTextSize(dlg.m_nTextSize);
					pButton->SetSettingValue(dlg.m_nSettingValue);
					pButton->SetLayer(dlg.m_nLayer);
					pButton->SetOperation(dlg.m_strOperation);
					pButton->DeleteAllRelation();
					for (size_t i=0;i<dlg.m_vec.size();++i)
					{
						_tagRelationExpress express = dlg.m_vec[i];
						pButton->InsertExpress(express);
					}
					pButton->SetRelation(dlg.m_nRelation);
					pButton->SetDownloadEnable(dlg.m_strDownloadEnable);
					pButton->SetDownloadName(dlg.m_strDownloadName);
					Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(dlg.m_nIDPicComm, true);
					//Gdiplus::Bitmap* pBitmap = dlg.m_picInfo[0].pBitmap;
					ASSERT(pBitmap);
					pButton->SetDefaultBitmap(pBitmap);
				}
			}
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_BEND:
		{
			int nPicId = 0;
			InsertImageIntoOpenS3db(g_nPicIndexBend, nPicId);
			CreateBend(point, pMainFrame->m_nBendSize);

			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_DASHBOARD:
		{
			/*
					InsertImageIntoOpenS3db函数用途不明，debug时此函数必出错疑似废代码。
				新加仪表并未使用此函数，可以稳定运行。
			*/
			CMeterPanSelectDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				int nPicId = 0;
				if (0 == dlg.m_nDashBoardType)
				{
					InsertImageIntoOpenS3db(g_nPicIndexDashBoard1, nPicId);
					InsertImageIntoOpenS3db(g_nPicIndexPointer1, nPicId);
					CreateDashBoardByType(point, dlg.m_nDashBoardType);
				}
				else if (1 == dlg.m_nDashBoardType)
				{
					InsertImageIntoOpenS3db(g_nPicIndexDashBoard2, nPicId);
					InsertImageIntoOpenS3db(g_nPicIndexPointer2, nPicId);
					CreateDashBoardByType(point, dlg.m_nDashBoardType);
				}
				else if (2 == dlg.m_nDashBoardType)
				{
					InsertImageIntoOpenS3db(g_nPicIndexDashBoard2, nPicId);
					InsertImageIntoOpenS3db(g_nPicIndexPointer2, nPicId);
					CreateDashBoardByType(point, dlg.m_nDashBoardType);
				}
				else if (3 == dlg.m_nDashBoardType)
				{
					CreateDashBoardByType(point, dlg.m_nDashBoardType);
				}
				else
				{	// error, unknown type, set as type = 0
					InsertImageIntoOpenS3db(g_nPicIndexDashBoard1, nPicId);
					InsertImageIntoOpenS3db(g_nPicIndexPointer1, nPicId);
					CreateDashBoardByType(point, 0);
				}
			}

			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_LINE_CHART:
		{
			int nPicId = 0;
			InsertImageIntoOpenS3db(g_nPicIndexChartLine, nPicId);
			CreateLineGraphic(point);

			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_BAR_CHART:
		{
			int nPicId = 0;
			InsertImageIntoOpenS3db(g_nPicIndexChartBar, nPicId);
			CreateBarGraphic(point);

			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_PIE_CHART:
		{
			int nPicId = 0;
			InsertImageIntoOpenS3db(g_nPicIndexChartPie, nPicId);
			CreatePieGraphic(point);

			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_TIME_SELECT:
		{
			int nPicId = 0;
			//InsertImageIntoOpenS3db(g_nPicIndexTimeSelect, nPicId);
			CreateTimeSelect(point);

			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_SHELL_EXECUTE:
		{
			CShellExecuteSettingDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				int nPicId = 0;
				InsertImageIntoOpenS3db(dlg.m_nBKPicID, nPicId);
				CreateShellExecute(point,dlg.m_strServerIP,dlg.m_strFileName,dlg.m_strDownloadDirectory,nPicId,dlg.m_bAutoDownload, dlg.m_layer, dlg.m_strFileID);

				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	case EQMDRAW_EMBEDED_PAGE:
		{
			CEmbededPageSettingDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				int nPicId = 0;
				InsertImageIntoOpenS3db(dlg.m_nPicID, nPicId);
				CreateEmbededPage(point, nPicId,dlg.m_PageInfoVec,dlg.m_nInterval,dlg.m_nLayer);

				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	case EQMDRAW_TIME_BAR:
		{
			int nPicId = 0;
			InsertImageIntoOpenS3db(g_nPicIndexChartBar, nPicId);
			CreateTimeBar(point);

			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_PROGRESS_BAR:
		{
			int nPicId = 0;
			InsertImageIntoOpenS3db(g_nPicIndexProgressBar, nPicId);
			CreateProgressBar(point);

			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_DIAGNOSE_CHART:
		{
			int nPicId = 0;
			InsertImageIntoOpenS3db(g_nPicIndexDiagnoseChart, nPicId);
			CreateDiagnoseGraphic(point);

			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_POLYGON:
		{
			CreatePolygon(point);
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_TEXT:
		CreateText(point);
		pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		Invalidate(FALSE);
		break;
	case GraphicType_ARROW:
	case GraphicType_TRIANGLE:
		pMainFrame->m_DrawContext.m_bIsCreate2DGraph = false;
		Invalidate(FALSE);
		break;
	case GraphicType_RECTANGLE:
		CreateRectangle(point, vecPt);
		Invalidate(FALSE);
		break;
	case GraphicType_ELLIPSE:
		CreateEllipse(point, vecPt);
		Invalidate(FALSE);
		break;
	case EQMDRAW_MOVE:
		OnLButtonUpSelectBox(nFlags, point);
		break;
	case EQMDRAW_AREA_CHANGE:
		{
			CMetaFileSelectDlg dlg(this);
			dlg.SetWndType(e_picture);
			dlg.SetCurrentPicID(pMainFrame->m_nSelectPicID);
			if (dlg.DoModal() == IDOK)
			{
				if (dlg.m_nID == -1)
				{
					return;
				}

				int nPicId = 0;
				InsertImageIntoOpenS3db(dlg.m_nID, nPicId);
				CreateAreaChange(point, nPicId);
				pMainFrame->m_nSelectPicID = nPicId;
			}
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_LIQUID_LEVEL:
		{
			CLiquidLevelSettingDlg dlg;
			dlg.SetPointMap(&pMainFrame->m_plcpointmap);
			if (dlg.DoModal() == IDOK)
			{
				int nPicId = 0;
				InsertImageIntoOpenS3db(g_nPicIndexLiquidLevel, nPicId);
				CreateLiquidLevel(point, dlg.m_nLayer, nPicId, dlg.m_strBindPoint, dlg.m_nMax, dlg.m_nMin, dlg.m_fHighAlarmLine, dlg.m_fLowAlarmLine);

				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			}
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_CHKBTN_PLUS:
		{
			CCheckButtonPlusSettingDlg dlg;
			dlg.SetPointMap(&pMainFrame->m_plcpointmap);
			if (dlg.DoModal() == IDOK)
			{
				eButtonStyle eBtnStyle = dlg.GetButtonStyle();
				int nLayer = dlg.GetLayerId();
				COLORREF colorTxt = dlg.GetTextColor();
				int nTxtSize = dlg.GetTextSize();
				CString strSelVal = dlg.GetSelectValue();
				CString strUnSelVal = dlg.GetUnSelectValue();
				CString strBindPointName = dlg.GetBindPointName();
				CString strTxtContent = dlg.GetTextContent();
				int nGroupId = dlg.GetGroupId();
				CString strExpression = dlg.GetExpression();
				CString strImgPathSelCom = dlg.GetImagePathSelectComm();
				CString strImgPathSelOver = dlg.GetImagePathSelectOver();
				CString strImgPathUnSelCom = dlg.GetImagePathUnSelectComm();
				CString strImgPathUnSelOver = dlg.GetImagePathUnSelectOver();
				CString cstrOptionsDec = dlg.GetOptionsDec();
				CEqmDrawButtonPlus* pEqm = CreateCheckButtonPlus(point, eBtnStyle, nLayer, colorTxt, nTxtSize,
					strSelVal, strUnSelVal, strBindPointName, strTxtContent, nGroupId, strExpression,
					strImgPathSelCom, strImgPathSelOver, strImgPathUnSelCom, strImgPathUnSelOver, cstrOptionsDec);
				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;

				if (!pEqm->CheckExpression(strExpression))
				{
					MessageBox(_T("表达式格式错误！"), _T("Error"), MB_OK|MB_ICONERROR);
				}
			}
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_LINE:
		{
			int nPicId = 0;
			CreateStraightLine(point, nPicId);
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_HISTORY_TREND:
		{
			CHistoryTrendSettingDlg dlg;
			if (IDOK == dlg.DoModal())
			{
				int nPicId = 0;
				InsertImageIntoOpenS3db(dlg.GetPicId(), nPicId);
				CreateHistoryTrend(point, nPicId, dlg.GetLayer(), dlg.GetHistoryTrendType(), dlg.GetHistoryTrendVector());

				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	/*case EQMDRAW_ENERGY_SAVE_ROI:*/
	case EQMDRAW_RECTANGLE:
		{
			
			/*
			COleDateTime time = COleDateTime::GetCurrentTime();
			time.SetDateTime(time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0);
			InsertImageIntoOpenS3db(dlg.GetPicId(), nPicId);
			*/
			int nPicId = 0;
			CreateEnergySaveRoi(point, nPicId);
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_MAP_NAVIGATE:
		{
			CStandardEquip dlg;
			if (IDOK == dlg.DoModal())
			{
 				CreateMapNavigate(point, dlg.GetLayer(),dlg.m_nTempateGroupId,dlg.m_nTemplatePageId,dlg.m_strTemplatePageName,dlg.m_strTemplateParam, dlg.m_cstrTemplateName);
				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				
				Invalidate(FALSE);
			}
			break;
		}
	case EQMDRAW_NAVIGATE_WINDOW:
		{
			CNavigateWindowSettingDlg dlg;
			if (IDOK == dlg.DoModal())
			{
				CreateNavigateWindow(point, dlg.GetLayer(), dlg.GetPageInfo());

				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	case EQMDRAW_LEVEL_RULER:
		{
			CLevelRulerSettingDlg dlg;
			if (IDOK == dlg.DoModal())
			{
				CreateLevelRuler(point, dlg.m_nLayer,
					dlg.m_strName, dlg.m_dMax, dlg.m_dMin, dlg.m_dMainScaleInterval, dlg.m_dMinorScaleInterval, dlg.m_nDecimalPlace,
					dlg.GetLabelInfo(), dlg.GetSectionInfo());

				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	case EQMDRAW_DATA_ANALYSE:
		{
			CDataAnalyseSettingDlg dlg(&pMainFrame->m_plcpointmap);
			if (IDOK == dlg.DoModal())
			{
				CreateDataAnalyse(point, dlg.m_nLayer, dlg.GetDaPointInfo(), dlg.m_strPtTime, dlg.m_strPtCount, dlg.m_strPtStatus);
				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	case EQMDRAW_TEMPER_DISTRI:
		{
			CTemperDistriSettingDlg dlg(this);
			if (IDOK == dlg.DoModal())
			{
				CreateTemperDistri(point, dlg.m_nLayer, dlg.GetTemperDistriInfo());
				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	case EQMDRAW_DATA_REPORT:
		{
			CDataReportSettingDlg dlg;
			if (IDOK == dlg.DoModal())
			{
				CreateDataReport(point, dlg.m_nLayer, dlg.m_strPath);
				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	case EQMDRAW_PLANE_TEMPE_DISTR:
		{
			CPlaneTempeDistrSettingDlg dlg(&pMainFrame->m_plcpointmap);
			if (IDOK == dlg.DoModal())
			{
				CreatePlaneTempeDistr(point, dlg.m_nLayer, dlg.m_strPointName);
				SyncPlaneTemperDistri(dlg.m_strPointName);

				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	case EQMDRAW_DOTLINE_FRAME:
		{
			CreateDottedLineFrm(point, 4);
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			Invalidate(FALSE);
			break;
		}
	case EQMDRAW_GENERAL_COMPONENT:
		{
			CGeneralComponentSettingDlg dlg;
			if (IDOK == dlg.DoModal())
			{
				CreateGeneralComponent(point, dlg.m_nLayer, dlg.m_strCfgValue);
				pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
				Invalidate(FALSE);
			}
			break;
		}
	default:
		break;
	}
		
    //bug ,create connection point
    pMainFrame->m_DrawContext.m_bIsChangeSize = FALSE;
	CBEOPDesignerView* pView =  (CBEOPDesignerView*)pMainFrame->GetActiveView();
	if (pView)
	{
		if (pView->m_PageDlg)
		{
			pView->m_PageDlg->m_bLButtonDown = false;
		}
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CCanvasDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ShowPropertyDlg(point);
	ShowPropertyWnd(point);
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CCanvasDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
		ASSERT(pMainFrame);
	m_rightclicksavepoint=point;
	CPoint point2 = point;   
	point.x /= pMainFrame->m_DrawContext.m_fScreenScale;
	point.y /= pMainFrame->m_DrawContext.m_fScreenScale;
	//if ( pMainFrame->m_DrawContext.bIsCreatePipe )  //add the pipe to the page
	//{
	//	pMainFrame->m_DrawContext.bIsCreatePipe = false;
	//	CEqmDrawPipe * pipe = dynamic_cast<CEqmDrawPipe *>(pMainFrame->m_DrawContext.pNewEqmt);
	//	if ( NULL != pipe && pipe->GetPointList().size() > 1 ) //if the pipe contains less than two points,we will delete this pipe
	//	{
	//		pMainFrame->m_DrawContext.pPage->AddPipe(pipe);
	//	}
	//	else
	//	{
	//		delete pipe;
	//		pipe = NULL;
	//	}
	//	pMainFrame->m_DrawContext.pNewEqmt = NULL;
	//}
	/*else */if ( pMainFrame->m_DrawContext.m_bIsCreate2DGraph )
	{
		pMainFrame->m_DrawContext.m_bIsCreate2DGraph = false;
		CEqmDrawGraphic * pGraphic = dynamic_cast<CEqmDrawGraphic *>(pMainFrame->m_DrawContext.pNewEqmt);
		if ( NULL != pGraphic && pGraphic->GetPointList().size() > 1 ) //if the pipe contains less than two points,we will delete this pipe
		{
			pMainFrame->m_DrawContext.pPage->Add2DGraphic(pGraphic);
			CommandManager::Instance()->CallCommand(pGraphic,E_COMMAND_ADD);
		}
		else
		{
			delete pGraphic;
			pGraphic = NULL;
		}
		pMainFrame->m_DrawContext.pNewEqmt = NULL;
	}
	else
	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU_RMOUSE);
        ClientToScreen(&point2);
		TRACE("%d, %d \n", m_rightclicksavepoint.x, m_rightclicksavepoint.y);
		CMenu *pMenu = menu.GetSubMenu(0); 
		{
			pMenu->EnableMenuItem(ID_RMENU_DELETE, MF_BYCOMMAND|MF_GRAYED);
			pMenu->EnableMenuItem(ID_RMENU_SELETEALL, MF_BYCOMMAND|MF_GRAYED);
			pMenu->EnableMenuItem(ID_RMENU_LEFTROTATE, MF_BYCOMMAND|MF_GRAYED);
			pMenu->EnableMenuItem(ID_RMENU_RIGHTROTATE, MF_BYCOMMAND|MF_GRAYED);
			pMenu->EnableMenuItem(ID_RMENU_PROPERTY, MF_BYCOMMAND|MF_GRAYED);
			pMenu->EnableMenuItem(ID_POSITION, MF_BYCOMMAND|MF_GRAYED);
			pMenu->EnableMenuItem(ID_RMENU_ADDTO_IMGLIB, MF_BYCOMMAND|MF_GRAYED);
			pMenu->EnableMenuItem(ID_RMENU_COPY, MF_BYCOMMAND|MF_GRAYED);
			pMenu->EnableMenuItem(ID_RMENU_PASTE, MF_BYCOMMAND|MF_GRAYED);
			pMenu->EnableMenuItem(ID_RMENU_CANCEL_DRAW, MF_BYCOMMAND|MF_GRAYED);
		}		
		CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
		ASSERT(pMainFrame);

		const size_t nSize = pMainFrame->m_DrawContext.vecSelectEqmt.size();
		if (nSize > 0)
		{
			for(size_t i=0; i<nSize; ++i)
			{
				EQMDRAWTYPE m_eEqmType1 = pMainFrame->m_DrawContext.vecSelectEqmt[i]->GetEqmType() ;     
				if (m_eEqmType1!=EQMDRAW_STRAIGHTPIPE&&m_eEqmType1!=GraphicType_LINE&&m_eEqmType1!=GraphicType_RECTANGLE&&m_eEqmType1!=GraphicType_ELLIPSE&&m_eEqmType1!=EQMDRAW_TEXT)				 
				{
					pMenu->EnableMenuItem(ID_RMENU_LEFTROTATE, MF_BYCOMMAND|MF_ENABLED);
					pMenu->EnableMenuItem(ID_RMENU_RIGHTROTATE, MF_BYCOMMAND|MF_ENABLED);
				}
				if (m_eEqmType1 == EQMDRAW_DASHBOARD || m_eEqmType1 == EQMDRAW_LINE_CHART || m_eEqmType1 == EQMDRAW_BAR_CHART || m_eEqmType1 == EQMDRAW_PIE_CHART || m_eEqmType1 == EQMDRAW_METABUTTON || m_eEqmType1 == EQMDRAW_TIME_SELECT || m_eEqmType1 == EQMDRAW_TIME_BAR)
				{
				  pMenu->EnableMenuItem(ID_RMENU_LEFTROTATE, MF_BYCOMMAND|MF_GRAYED);
				  pMenu->EnableMenuItem(ID_RMENU_RIGHTROTATE, MF_BYCOMMAND|MF_GRAYED);
				}
				pMenu->EnableMenuItem(ID_RMENU_DELETE, MF_BYCOMMAND|MF_ENABLED);
				pMenu->EnableMenuItem(ID_RMENU_SELETEALL, MF_BYCOMMAND|MF_ENABLED);
				pMenu->EnableMenuItem(ID_RMENU_PROPERTY, MF_BYCOMMAND|MF_ENABLED);
				pMenu->EnableMenuItem(ID_POSITION, MF_BYCOMMAND|MF_ENABLED);
			}

			// copy item
			pMenu->EnableMenuItem(ID_RMENU_COPY, MF_BYCOMMAND|MF_ENABLED);
		}

		// paste itme
		if (!(pMainFrame->m_DrawContext.mVecSelectEqmtCopied).empty())
		{
			pMenu->EnableMenuItem(ID_RMENU_PASTE, MF_BYCOMMAND|MF_ENABLED);
		}

		// insert into lib
		CEqmDrawDevice *pEqmDraw = dynamic_cast<CEqmDrawDevice*>(pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage, (double)pMainFrame->m_DrawContext.actPt.X, (double)pMainFrame->m_DrawContext.actPt.Y));
		if (NULL != pEqmDraw)
		{
			if (EQMDRAW_METAFILE == pEqmDraw->GetEqmType())
			{
				pMenu->EnableMenuItem(ID_RMENU_ADDTO_IMGLIB, MF_BYCOMMAND|MF_ENABLED);
			}
		}

		if(pMainFrame->m_DrawContext.pNewEqmt != NULL)
		{
			pMenu->EnableMenuItem(ID_RMENU_CANCEL_DRAW, MF_BYCOMMAND|MF_ENABLED);
		}

		pMenu->EnableMenuItem(ID_RMENU_SELETEALL, MF_BYCOMMAND|MF_ENABLED);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_LEFTBUTTON| TPM_RIGHTBUTTON, point2.x, point2.y,this);
		menu.Detach();
	}
	Invalidate(FALSE);
	m_ptPaste = point;

	CDialog::OnRButtonDown(nFlags, point);
}

void CCanvasDlg::ShowPipeInfoDlg(void)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CEqmDrawBase *pEqmDrawBase = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
	CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
	CPipeInfoDlg dlg(this);

	CEqmDrawPipe* pPipe = (CEqmDrawPipe*)pEqmDrawBase;
	if (NULL == pPipe )
	{
		return;
	}
	dlg.m_nInnerColor = pPipe->GetPipeColor().innercolor;
	dlg.m_nSideColor  = pPipe->GetPipeColor().outercolor;
	dlg.m_pipewidth = pPipe->GetPipeWidth();
	dlg.m_vecDataName = pPipe->GetDataComIDVector();
	dlg.m_nDirection = (int)pPipe->GetDirection();
	dlg.m_layer = pPipe->GetLayer();
	dlg.m_nLogic = pPipe->m_nLogic;
	dlg.m_nPipeType = pPipe->GetPipeType();
	dlg.m_nPipeBitMapID = pPipe->GetPipeBitMapID();
	dlg.m_nPipeDenSity = pPipe->GetPipeDenSity();
	dlg.m_nPipeSpeed = pPipe->GetPipeSpeed();
	dlg.m_WaterFashion = pPipe->GetWaterFashion();

	dlg.SetPointmap(&pMainFrame->m_plcpointmap);
	if ( IDOK == dlg.DoModal() )
	{
		CommandManager::Instance()->CallCommand(pPipe,E_COMMAND_EDIT);
		PipeColor color;
		color.innercolor = dlg.m_nInnerColor;
		color.outercolor = dlg.m_nSideColor;
		pPipe->SetPipeColor(color);
		pPipe->SetDirection((_tagPipeDirection)dlg.m_nDirection);
		pPipe->SetPipeWidth(dlg.m_pipewidth);
		pMainFrame->m_DrawContext.SetDefaultPipeWidth(dlg.m_pipewidth);
		pPipe->SetDataComID(dlg.m_vecDataName);
		pPipe->SetLayer(dlg.m_layer);
		pPipe->SetPipeType(dlg.m_nPipeType);
		pPipe->SetPipeBitMapID(dlg.m_nPipeBitMapID);
		pPipe->SetPipeDenSity(dlg.m_nPipeDenSity);
		pPipe->SetPipeSpeed(dlg.m_nPipeSpeed);
		pPipe->SetDataPointList(dlg.m_vecDataName);
		pPipe->SetWaterFashion(dlg.m_WaterFashion);
		pPipe->m_nLogic = dlg.m_nLogic;
	}
	pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
}

void CCanvasDlg::ShowGraphicInfoDlg(void)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CEqmDrawBase *pEqmDrawBase = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
	CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
	CGraphicInfoDlg dlg(this);

	CEqmDrawGraphic* pg = dynamic_cast<CEqmDrawGraphic*>(pEqmDrawBase);
	if (!pg)
	{
		return;
	}
	dlg.m_bIsDashLine= pg->GetLineStyle();
	dlg.m_nLineWidth = pg->GetLineWidth();
	dlg.m_LineColor  = pg->GetLineColor();
	dlg.m_type       = pg->GetEqmType();
	dlg.m_FillColor = pg->GetFilledColor();
	//向前兼容
	if (dlg.m_FillColor >= MAXWORD)
	{
		//@todo:去掉project 中的 back color.
		//dlg.m_FillColor = pMainFrame->m_DrawContext.GetProject()->GetBackGroundColor();
	}
	dlg.m_layer = pg->GetLayer();

	//get the attibute
	if ( IDOK == dlg.DoModal()){   
		CommandManager::Instance()->CallCommand(pg,E_COMMAND_EDIT);
		pg->SetLineStyle(dlg.m_bIsDashLine);   //add 7.8
		pg->SetLineWidth(dlg.m_nLineWidth);
		pg->SetLineColor(dlg.m_LineColor);
		pg->SetFilledColor(dlg.m_FillColor);
		pg->SetLayer(dlg.m_layer);
	}

	pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
}
void CCanvasDlg::ShowTextInfoDlg()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CEqmDrawBase *pEqmDrawBase = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
	CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
	HWND handle = pView->m_PageDlg->m_dlgCanvas.m_hWnd;
	CWnd* pWnd = FromHandle(handle);
	CTextInfoDlg dlg(pWnd);

	CEqmDrawText* pText = dynamic_cast<CEqmDrawText*>(pEqmDrawBase);
	ASSERT(pText != NULL);
	if (pText == NULL)
	{
		return;
	}
	dlg.m_nID = pText->GetID();
	//dlg.m_strText = pText->GetShowString().c_str();
	dlg.m_strText = pText->GetInitialstring().c_str();
	dlg.m_nColor  = pText->GetFontColor();
	dlg.m_strPlcValue  = pText->GetPhysicalID().c_str();
	dlg.m_nFontSize = pText->GetFontSize();
	dlg.m_showmode = pText->GetTextShowMode();
	dlg.m_dTransparent = pText->GetTransparence();
	dlg.m_fontname = pText->GetFontName();
	dlg.m_TextBorderType = pText->GetBorderType();
	dlg.m_nBorderColor = pText->GetBorderColor();
	dlg.m_layer = pText->GetLayer();
	dlg.m_align = pText->GetAlign();
	dlg.m_strPlcValue = pText->GetPhysicalID().c_str();
	dlg.SetPointMap(&pMainFrame->m_plcpointmap);
	dlg.m_strTextModeString = pText->GetTextModeShowString();
	dlg.m_pointNum = pText->GetPointNum();
	dlg.m_strDateTime = pText->GetDateTime();
	dlg.m_strDateTimePoint = pText->GetDateTimePoint();
	dlg.m_nTextBKType = pText->GetTextBKType();
	dlg.m_nTextAttri = pText->GetTextAttribute();
	//dlg.m_strDeviceCfg = pText->GetDeviceCfg();
	//dlg.m_strTemperCfg = pText->GetTemperCfg();
	//dlg.m_bFaultDiagnosis = pText->GetFaultDiagnosis();
	dlg.m_strBindScript = pText->GetBindScript();
	dlg.m_cstrUnit = pText->GetUnit();
	if (IDOK == dlg.DoModal())
	{
		CommandManager::Instance()->CallCommand(pText,E_COMMAND_EDIT);
		pText->DeleteFont();
		pText->DeleteSolidBrush();
		pText->DeleteHatchBrush();
		pText->DeleteCommonBorderPen();
		pText->Delete3DPenSunken();
		pText->Delete3DPenHump();
		pText->SetInitialString((LPCTSTR)dlg.m_strText);
		pText->SetShowString(_T(""), SHOWTYPE_DRAW);
		pText->SetFontColor(dlg.m_nColor);
		pText->SetFontSize(dlg.m_nFontSize);
		pText->SetPhysicalID(dlg.m_strPlcValue.GetString());
		pText->SetTextShowMode(dlg.GetShowMode());
		pText->SetTransparence(dlg.m_dTransparent);
		pText->SetFontName(dlg.m_fontname);
		pText->SetBorderType(dlg.m_TextBorderType);
		pText->SetBorderColor(dlg.m_nBorderColor);
		pText->SetLayer(dlg.m_layer);
		pText->SetAlign(dlg.m_align);
		pText->SetTextModeShowString(dlg.m_strTextModeString);
		pText->SetPointNum(dlg.m_pointNum);
		pText->SetDateTime(dlg.m_strDateTime);
		pText->SetDateTimePoint(dlg.m_strDateTimePoint);
		pText->SetTextBKType(dlg.m_nTextBKType);
		pText->SetTextAttribute((TextAttri)(dlg.m_nTextAttri));
		//pText->SetDeviceCfg(dlg.m_strDeviceCfg);
		//pText->SetTemperCfg(dlg.m_strTemperCfg);
		//pText->SetFaultDiagnosis(dlg.m_bFaultDiagnosis);
		pText->SetBindScript(dlg.m_strBindScript);
		pText->SetUnit(dlg.m_cstrUnit);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
	}
}

void CCanvasDlg::ShowMetaFileDlg()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
	CEqmDrawBase *pEqmDrawBase = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
	CMetaFileInfoDlg dlg(this);
	CEqmDrawDevice* pDevice = dynamic_cast<CEqmDrawDevice*>(pEqmDrawBase);
	if(!pDevice)
		return;
	CommandManager::Instance()->CallCommand(pDevice,E_COMMAND_EDIT);
	dlg.SetDevice(pDevice);
	wstring strName = pDevice->GetEqmName();
	dlg.SetName(strName.c_str());
	dlg.SetPointMap(&pMainFrame->m_plcpointmap);
	dlg.m_strPointName = pDevice->GetPhysicalID().c_str();
	dlg.m_strSettingPointName = pDevice->GetEventInfo().strSettingPointName;
	dlg.m_nDeviceType = (int)pDevice->GetEventInfo().deviceType;
	dlg.m_nMouseEventType = (int)pDevice->GetEventInfo().mouseEventType;
	dlg.m_nDeviceIDNum = pDevice->GetEventInfo().nDeviceID;
	dlg.m_nLayer = (int)pDevice->GetLayer();
	dlg.m_nValue = pDevice->GetEventInfo().nValue;
	dlg.m_strShowName = pDevice->GetEventInfo().strShowName;
	dlg.m_strOutsideLink = pDevice->GetEventInfo().strOutLink;
	dlg.m_nBindType = pDevice->GetBindType();
	dlg.DoModal();
}

void CCanvasDlg::OnRmenuSeleteall()          
{
	// TODO: Add your command handler code here
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
	OnKeySelectAll();
}

void CCanvasDlg::OnRmenuDelete()
{
	// TODO: Add your command handler code here
	OnKeyDelete();
}

void CCanvasDlg::OnRmenuProperty()
{
	// TODO: Add your command handler code here
	ShowPropertyDlg(m_rightclicksavepoint);
	ShowPropertyWnd(m_rightclicksavepoint);
}


void CCanvasDlg::OnRmenuLeftrotate()
{
	// TODO: Add your command handler code here
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	for(UINT idx=0; idx< pMainFrame->m_DrawContext.vecSelectEqmt.size(); idx++)
	{	
		pMainFrame->m_DrawProxy.LeftRotate90(pMainFrame->m_DrawContext.vecSelectEqmt[idx]);
	}
	Invalidate(FALSE);
}

void CCanvasDlg::OnRmenuRightrotate()
{
	// TODO: Add your command handler code here
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	for(UINT idx=0; idx< pMainFrame->m_DrawContext.vecSelectEqmt.size(); idx++)
	{	
	pMainFrame->m_DrawProxy.RightRotate90(pMainFrame->m_DrawContext.vecSelectEqmt[idx]);
	}
	Invalidate(FALSE);
}

void CCanvasDlg::OnUpdateRmenuLeftrotate(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	pCmdUI->Enable(NULL != pMainFrame->m_DrawContext.pPage 
		&& (EQMDRAW_MOVE == pMainFrame->m_DrawContext.m_nDrawMode) &&pMainFrame-> m_DrawContext.vecSelectEqmt.size()>0);
}

void CCanvasDlg::OnUpdateRmenuRightrotate(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	pCmdUI->Enable(NULL != pMainFrame->m_DrawContext.pPage 
		&& (EQMDRAW_MOVE == pMainFrame->m_DrawContext.m_nDrawMode) &&pMainFrame-> m_DrawContext.vecSelectEqmt.size()>0);
}

void CCanvasDlg::TerminateThread()
{
	::TerminateThread(m_hThreadDraw, 0);
	WaitForSingleObject(m_hThreadDraw, INFINITE);
}

LRESULT CCanvasDlg::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	//CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	//CPoint curpos;
	//GetCursorPos(&curpos);
	//ScreenToClient(&curpos);
	//pMainFrame->m_DrawContext.actPt.X = curpos.x;
	//pMainFrame->m_DrawContext.actPt.Y = curpos.y;
	//OnLButtonUpSelectBox(0, curpos);
	m_trackleave = FALSE;
	return TRUE;
}

BOOL CCanvasDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::PreTranslateMessage(pMsg);
}

void CCanvasDlg::ShowPropertyDlg(const CPoint& point)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	CPoint actpoint = point;
	actpoint.x /= pMainFrame->m_DrawContext.m_fScreenScale;
	actpoint.y /= pMainFrame->m_DrawContext.m_fScreenScale;
	pMainFrame->m_DrawContext.actPt.X = actpoint.x;
	pMainFrame->m_DrawContext.actPt.Y = actpoint.y;
	CEqmDrawBase *pEqmDrawBase = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
	if ( NULL != pEqmDrawBase )
	{
		switch ( pEqmDrawBase->GetEqmType() )
		{
		case EQMDRAW_CHILLER:
		case EQMDRAW_PUMP:
		case EQMDRAW_COOLINGTOWER:
		case EQMDRAW_VALVE:
		case EQMDRAW_METAFILE:
			{
				ShowMetaFileDlg();
			}
			break;
		case EQMDRAW_TEXT:
			{
				ShowTextInfoDlg();				
			}
			break;
		case EQMDRAW_STRAIGHTPIPE:
		case EQMDRAW_FOLDPIPE:
			{
				ShowPipeInfoDlg();
			}
			break;
		case GraphicType_LINE:
		case GraphicType_RECTANGLE:
		case GraphicType_ELLIPSE:
			{
				ShowGraphicInfoDlg();
			}
			break;
		case EQMDRAW_METABUTTON://修改界面中的按钮的属性
			{
				CEqmDrawButton* pButton = dynamic_cast<CEqmDrawButton*>(pEqmDrawBase);
				if (pButton)
				{
					CMetaButtonSelectDlg dlg(FALSE, this);
					int nComm = 0;
					int nDown = 0;
					int nOver = 0;
					int nDisable = 0;
					int nChecked = 0;
					pButton->Get5PicID(nComm, nDisable, nDown, nOver,nChecked);
					dlg.m_nIDPicComm = nComm;
					dlg.m_nIDPicDisable = nDisable;
					dlg.m_nIDPicOver = nOver;
					dlg.m_nIDPicDown = nDown;
					dlg.m_nICPicChecked = nChecked;
					dlg.m_nLinkPageID = pButton->GetLinkPageID();
					dlg.m_strBindSetPointName = pButton->GetBindSetPointName();
					dlg.m_strTextContent = pButton->GetTextContent();
					dlg.m_nTextColor = pButton->GetTextColor();
					dlg.m_nTextSize = pButton->GetTextSize();
					dlg.m_nSettingValue = pButton->GetSettingValue();
					dlg.m_nLayer = pButton->GetLayer();
					dlg.m_strOperation = pButton->GetOperation();
					dlg.m_vec.clear();
					dlg.m_vec = pButton->GetRelationVec();
					dlg.m_nRelation = pButton->GetRelation();
					dlg.m_strDownloadEnable = pButton->GetDownloadEnable();
					dlg.m_strDownloadName = pButton->GetDownloadName();
					if (dlg.DoModal() == IDOK)
					{
						//int nPicId[5] = {0};
						//InsertImageIntoOpenS3db(dlg.m_nIDPicComm, nPicId[0]);
						//InsertImageIntoOpenS3db(dlg.m_nIDPicDisable, nPicId[1]);
						//InsertImageIntoOpenS3db(dlg.m_nIDPicDown, nPicId[2]);
						//InsertImageIntoOpenS3db(dlg.m_nIDPicOver, nPicId[3]);
						//InsertImageIntoOpenS3db(dlg.m_nICPicChecked, nPicId[4]);
						CommandManager::Instance()->CallCommand(pButton,E_COMMAND_EDIT);
						pButton->SetLinkPageID(dlg.m_nLinkPageID);
						pButton->Set5PicID(dlg.m_nIDPicComm,dlg.m_nIDPicDisable,dlg.m_nIDPicDown,dlg.m_nIDPicOver,dlg.m_nICPicChecked);
						pButton->SetBindSetPointName(dlg.m_strBindSetPointName);
						pButton->SetTextContent(dlg.m_strTextContent);
						pButton->SetTextColor(dlg.m_nTextColor);
						pButton->SetTextSize(dlg.m_nTextSize);
						pButton->SetSettingValue(dlg.m_nSettingValue);
						pButton->SetLayer(dlg.m_nLayer);
						pButton->SetOperation(dlg.m_strOperation);
						pButton->DeleteAllRelation();
						for (size_t i=0;i<dlg.m_vec.size();++i)
						{
							_tagRelationExpress express = dlg.m_vec[i];
							pButton->InsertExpress(express);
						}
						pButton->SetRelation(dlg.m_nRelation);
						pButton->SetDownloadEnable(dlg.m_strDownloadEnable);
						pButton->SetDownloadName(dlg.m_strDownloadName);
						Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(dlg.m_nIDPicComm, true);
						ASSERT(pBitmap);
						pButton->SetDefaultBitmap(pBitmap);
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_LINE_CHART:
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CEqmDrawLineGraphic* pLineGraphic = dynamic_cast<CEqmDrawLineGraphic*>(pEqmDrawBase);
				if (pLineGraphic)
				{
					CChartPropertyDlg dlg(pEqmDrawBase);
					dlg.SetPointMap(&pMainFrame->GetPointMap());
					dlg.m_nLayer = pLineGraphic->GetLayer();
					if (dlg.DoModal() == IDOK)
					{
						CommandManager::Instance()->CallCommand(pLineGraphic,E_COMMAND_EDIT);
						pLineGraphic->SetTitleName(dlg.m_strChartTitle);
						pLineGraphic->SetUpdateInterval(dlg.m_nUpdateInterval);
						pLineGraphic->SetXName(dlg.m_strXName);
						pLineGraphic->SetXUnit(dlg.m_strXUnit);
						pLineGraphic->SetYName(dlg.m_strYName);
						pLineGraphic->SetTextColor(dlg.m_nTextColor);
						pLineGraphic->SetLayer(dlg.m_nLayer);
					}
				}
			}
			break;
		case EQMDRAW_BAR_CHART:
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CEqmDrawBarGraphic* pBarGraphic = dynamic_cast<CEqmDrawBarGraphic*>(pEqmDrawBase);
				if (pBarGraphic)
				{
					CChartPropertyDlg dlg(pEqmDrawBase);
					dlg.SetPointMap(&pMainFrame->GetPointMap());
					dlg.m_nLayer = pBarGraphic->GetLayer();
					if (dlg.DoModal() == IDOK)
					{
						CommandManager::Instance()->CallCommand(pBarGraphic,E_COMMAND_EDIT);
						pBarGraphic->SetTitleName(dlg.m_strChartTitle);
						pBarGraphic->SetUpdateInterval(dlg.m_nUpdateInterval);
						pBarGraphic->SetXName(dlg.m_strXName);
						pBarGraphic->SetXUnit(dlg.m_strXUnit);
						pBarGraphic->SetYName(dlg.m_strYName);
						pBarGraphic->SetQueryType(dlg.m_nQueryType);
						pBarGraphic->SetTextColor(dlg.m_nTextColor);
						pBarGraphic->SetLayer(dlg.m_nLayer);
					}
				}
			}
			break;
		case EQMDRAW_PIE_CHART:
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CEqmDrawPieGraphic* pPieGraphic = dynamic_cast<CEqmDrawPieGraphic*>(pEqmDrawBase);
				if (pPieGraphic)
				{
					CChartPropertyDlg dlg(pEqmDrawBase);
					dlg.SetPointMap(&pMainFrame->GetPointMap());
					dlg.m_nLayer = pPieGraphic->GetLayer();
					if (dlg.DoModal() == IDOK)
					{
						CommandManager::Instance()->CallCommand(pPieGraphic,E_COMMAND_EDIT);
						pPieGraphic->SetTitleName(dlg.m_strChartTitle);
						pPieGraphic->SetUpdateInterval(dlg.m_nUpdateInterval);
						pPieGraphic->SetTextColor(dlg.m_nTextColor);
						pPieGraphic->SetLayer(dlg.m_nLayer);
					}
				}
			}
			break;
		case EQMDRAW_DIAGNOSE_CHART:
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = dynamic_cast<CEqmDrawDiagnoseGraphic*>(pEqmDrawBase);
				if (pDiagnoseGraphic)
				{
					CommandManager::Instance()->CallCommand(pDiagnoseGraphic,E_COMMAND_EDIT);
					CDiagnosePropertySettingDlg dlg(pDiagnoseGraphic);
					dlg.DoModal();
				}
			}
			break;
		case EQMDRAW_POLYGON:
			{
				CEqmDrawPolygon* pPolygon = dynamic_cast<CEqmDrawPolygon*>(pEqmDrawBase);
				if (pPolygon != NULL)
				{
					CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
					
  				 	CStandardEquip dlg;
					dlg.m_nLayer				= pPolygon->GetLayer();
					dlg.m_strTemplatePageName	= pPolygon->GetTemplatePageName();
					dlg.m_nTemplatePageId		= pPolygon->GetTemplatePageId();
					dlg.m_nTempateGroupId		= pPolygon->GetTemplateGroupId();
					dlg.m_strTemplateParam		= pPolygon->GetParam();
					dlg.m_cstrTemplateName		= pPolygon->GetTemplateName();
					//dlg.ppMapNavigate			= pMapNavigate;
					//dlg.m_bBanOnDisplay			= pMapNavigate->GetBanOnDisplay();//获取禁止显示的情况
					dlg.m_bCreate = false;

					if (IDOK == dlg.DoModal())
					{
                        pPolygon->SetTemplateGroupId(dlg.m_nTempateGroupId);
						pPolygon->SetTemplatePageId(dlg.m_nTemplatePageId);
						pPolygon->SetTemplatePageName(dlg.m_strTemplatePageName);
						pPolygon->SetParam(dlg.m_strTemplateParam);
						pPolygon->SetLayer(dlg.m_nLayer);
						pPolygon->SetTemplateName(dlg.m_cstrTemplateName);
						CommandManager::Instance()->CallCommand(pPolygon,E_COMMAND_EDIT);
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_DASHBOARD:
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CEqmDrawDashBoard* pDashBoard = dynamic_cast<CEqmDrawDashBoard*>(pEqmDrawBase);
				if (pDashBoard)
				{
					CDashBoardPropertyDlg dlg;
					dlg.m_nLayer = pDashBoard->GetLayer();
					dlg.SetDashBoardPointer(pDashBoard);
					dlg.SetPointMap(&pMainFrame->GetPointMap());
					dlg.m_strBindPointName = pDashBoard->GetBindPointName();
					dlg.m_dMinValue = pDashBoard->GetMinValue();
					dlg.m_dMaxValue = pDashBoard->GetMaxValue();
					dlg.m_nStyle = pDashBoard->GetStyle();
					dlg.m_strMaxValue = pDashBoard->GetMaxString();
					dlg.m_strMinValue = pDashBoard->GetMinString();
					dlg.m_bIsBindMax = pDashBoard->GetMaxBindFlag();
					dlg.m_bIsBindMin = pDashBoard->GetMinBindFlag();
					dlg.m_FontColor = pDashBoard->GetFontColor();
					dlg.m_DecimalPlaces = pDashBoard->GetDecimalPlaces();
					if (dlg.DoModal() == IDOK)
					{
						CommandManager::Instance()->CallCommand(pDashBoard,E_COMMAND_EDIT);
						pDashBoard->SetLayer(dlg.m_nLayer);
						pDashBoard->SetBindPointName(dlg.m_strBindPointName);
						pDashBoard->SetMinValue(dlg.m_dMinValue);
						pDashBoard->SetMaxValue(dlg.m_dMaxValue);
						pDashBoard->SetMaxString(dlg.m_strMaxValue);
						pDashBoard->SetMinString(dlg.m_strMinValue);
						pDashBoard->SetMaxBindFlag(dlg.m_bIsBindMax);
						pDashBoard->SetMinBindFlag(dlg.m_bIsBindMin);
						pDashBoard->SetFontColor(dlg.m_FontColor);
						pDashBoard->SetDecimalPlaces(dlg.m_DecimalPlaces);
					}
				}
			}
			break;
		case EQMDRAW_BEND:
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CEqmDrawBend* pBend = dynamic_cast<CEqmDrawBend*>(pEqmDrawBase);
				if (pBend)
				{
					CBendPropertyDlg dlg;
					dlg.m_nLayer = pBend->GetLayer();
					dlg.m_nRotateAngle = pBend->GetRotateAngle();
					dlg.m_nBendSize = pBend->GetSize();
					if (dlg.DoModal() == IDOK)
					{
						CommandManager::Instance()->CallCommand(pBend,E_COMMAND_EDIT);
						pBend->SetLayer(dlg.m_nLayer);
						pBend->SetRotateAngle(dlg.m_nRotateAngle);
						pBend->SetSize(dlg.m_nBendSize);
						int nBendSizeShow = 2 * dlg.m_nBendSize + 13;
						pBend->SetWidth(nBendSizeShow);
						pBend->SetHeight(nBendSizeShow);
						pMainFrame->m_nBendSize = dlg.m_nBendSize;
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_TIME_SELECT:
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CEqmDrawTimeSelect* pTimeSelect = dynamic_cast<CEqmDrawTimeSelect*>(pEqmDrawBase);
				if (pTimeSelect)
				{
					CTimeSelectPropertyDlg dlg;
					dlg.SetPointMap(&pMainFrame->GetPointMap());
					dlg.m_strBindPointName = pTimeSelect->GetPointName();
					dlg.m_rwProperty = pTimeSelect->GetRWProperty();
					dlg.m_radioDateTimeSelect = pTimeSelect->GetTimeDataFormat();
					if (dlg.DoModal() == IDOK)
					{
						CommandManager::Instance()->CallCommand(pTimeSelect,E_COMMAND_EDIT);
						pTimeSelect->SetPointName(dlg.m_strBindPointName);
						pTimeSelect->SetRWProperty(dlg.m_rwProperty);
						pTimeSelect->SetTimeDataFormat(dlg.m_radioDateTimeSelect);
						const CString strPointName = pTimeSelect->GetPointName();
						if (strPointName.GetLength() > 0)
						{
							DataPointEntry entry = dlg.m_pPointMap->GetEntry(strPointName.GetString());
							CString strFormat = entry.GetParam(1).c_str();
							if (strFormat.GetLength()>0)
							{
								if (strFormat.CompareNoCase(L"year:month") == 0)
								{
									pTimeSelect->SetWidth(140);
								}
								else if (strFormat.CompareNoCase(L"year:month:day") == 0)
								{
									pTimeSelect->SetWidth(140);
								}
								else if (strFormat.CompareNoCase(L"year:month:day hour:minute:second") == 0 || strFormat.CompareNoCase(L"hour:minute:second") == 0)
								{
									pTimeSelect->SetWidth(216);
								}
								else if (strFormat.CompareNoCase(L"year:month:day hour") == 0)
								{
									pTimeSelect->SetWidth(216);
								}
								else if (strFormat.CompareNoCase(L"hour:minute") == 0)
								{
									pTimeSelect->SetWidth(216);
								}
								else
								{
									pTimeSelect->SetWidth(216);
								}
							}
							Invalidate();
						}
					}
				}
			}
			break;
		case EQMDRAW_TIME_BAR:
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CEqmDrawTimeBar* pTimeBar = dynamic_cast<CEqmDrawTimeBar*>(pEqmDrawBase);
				if (pTimeBar)
				{
					CommandManager::Instance()->CallCommand(pTimeBar,E_COMMAND_EDIT);
					CTimeBarPropertyDlg dlg(pTimeBar,&pMainFrame->GetPointMap(),NULL);
					dlg.DoModal();
				}
			}
			break;
		case EQMDRAW_PROGRESS_BAR:
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CEqmDrawProgressBar* pProgressBar = dynamic_cast<CEqmDrawProgressBar*>(pEqmDrawBase);
				if (pProgressBar)
				{
					CommandManager::Instance()->CallCommand(pProgressBar,E_COMMAND_EDIT);
					CProgressBarProperty dlg(pProgressBar,&pMainFrame->GetPointMap(),NULL);
					dlg.DoModal();
				}
			}
			break;
		case EQMDRAW_AREA_CHANGE:
			{
				CEqmDrawAreaChange* pAreaChange = dynamic_cast<CEqmDrawAreaChange*>(pEqmDrawBase);
				if (pAreaChange)
				{
					CAreaChagePropertyDlg dlg;
					dlg.m_dXPos = pAreaChange->GetPos().X;
					dlg.m_dYPos = pAreaChange->GetPos().Y;
					dlg.m_dWidth = pAreaChange->GetWidth();
					dlg.m_dHeight = pAreaChange->GetHeight();
					dlg.m_layer = pAreaChange->GetLayer();
					dlg.SetAreaInfoVec(pAreaChange->GetAreaInfo());
					CommandManager::Instance()->CallCommand(pAreaChange,E_COMMAND_EDIT);
					if (dlg.DoModal() == IDOK)
					{
						pAreaChange->SetAreaInfo(dlg.m_vecAreaInfo);
						pAreaChange->SetLayer(dlg.m_layer);
					}
				}
			}
			break;
		case EQMDRAW_SHELL_EXECUTE:
			{
				CEqmDrawShellExecute* pShellExecute = dynamic_cast<CEqmDrawShellExecute*>(pEqmDrawBase);
				if (pShellExecute)
				{
					CShellExecuteSettingDlg dlg;
					dlg.m_strServerIP = pShellExecute->GetServerIP();
					dlg.m_strFileName = pShellExecute->GetFileName();
					dlg.m_strDownloadDirectory = pShellExecute->GetDownloadDirectory();
					dlg.m_nBKPicID = pShellExecute->GetBKPicID();
					dlg.m_bAutoDownload = pShellExecute->GetAutoDownload();
					dlg.m_layer = pShellExecute->GetLayer();
					dlg.m_strFileID = pShellExecute->GetFileID();
					if (dlg.DoModal() == IDOK)
					{
						CommandManager::Instance()->CallCommand(pShellExecute,E_COMMAND_EDIT);
						if (dlg.m_nBKPicID != pShellExecute->GetBKPicID())
						{
							CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
							Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(dlg.m_nBKPicID, false);
							if(pBitmap)
							{
								pShellExecute->SetWidth(pBitmap->GetWidth());
								pShellExecute->SetHeight(pBitmap->GetHeight());
								pShellExecute->SetDefaultBitmap(pBitmap);
							}
						}
						pShellExecute->SetServerIP(dlg.m_strServerIP);
						pShellExecute->SetFileName(dlg.m_strFileName);
						pShellExecute->SetDownloadDirectory(dlg.m_strDownloadDirectory);
						pShellExecute->SetBKPicID(dlg.m_nBKPicID);
						pShellExecute->SetAutoDownload(dlg.m_bAutoDownload);
						pShellExecute->SetLayer(dlg.m_layer);
						pShellExecute->SetFileID(dlg.m_strFileID);
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_EMBEDED_PAGE:
			{
				CEqmDrawEmbededPage* pEmbededPage = dynamic_cast<CEqmDrawEmbededPage*>(pEqmDrawBase);
				if (pEmbededPage)
				{
					CEmbededPageSettingDlg dlg;
					dlg.m_pEmbededPage = pEmbededPage;
					dlg.m_nPicID = pEmbededPage->GetPicID();
					dlg.m_nLayer = pEmbededPage->GetLayer();
					dlg.m_nInterval = pEmbededPage->GetChangeInterval();
					if (dlg.DoModal() == IDOK)
					{
						CommandManager::Instance()->CallCommand(pEmbededPage,E_COMMAND_EDIT);
						if (dlg.m_nPicID != pEmbededPage->GetPicID())
						{
							CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
							Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(dlg.m_nPicID, false);
							if(pBitmap)
							{
								pEmbededPage->SetWidth(pBitmap->GetWidth());
								pEmbededPage->SetHeight(pBitmap->GetHeight());
								pEmbededPage->SetDefaultBitmap(pBitmap);
							}
						}
						pEmbededPage->SetPicID(dlg.m_nPicID);
						pEmbededPage->SetLayer(dlg.m_nLayer);
						pEmbededPage->SetChangeInterval(dlg.m_nInterval);
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_LIQUID_LEVEL:
			{
				CEqmDrawLiquidLevel* pLiquidLevel = dynamic_cast<CEqmDrawLiquidLevel*>(pEqmDrawBase);
				if (pLiquidLevel != NULL)
				{
					CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
					CLiquidLevelSettingDlg dlg(this);
					dlg.SetPointMap(&pMainFrame->m_plcpointmap);
					//dlg.m_pEmbededPage = pEmbededPage;
					dlg.m_nPicID = pLiquidLevel->GetPicID();
					dlg.m_nLayer = pLiquidLevel->GetLayer();
					dlg.m_strBindPoint = pLiquidLevel->GetBindPoint();
					dlg.m_nMax = pLiquidLevel->GetMaxValue();
					dlg.m_nMin = pLiquidLevel->GetMinValue();
					dlg.m_fHighAlarmLine = pLiquidLevel->GetHighAlarmLine();
					dlg.m_fLowAlarmLine = pLiquidLevel->GetLowAlarmLine();
					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pLiquidLevel,E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\liquidLevel.png");
						if (pBitmap != NULL)
						{
							//pLiquidLevel->SetWidth(pBitmap->GetWidth());
							//pLiquidLevel->SetHeight(pBitmap->GetHeight());
							pLiquidLevel->SetDefaultBitmap(pBitmap);
						}
						//pLiquidLevel->SetPicID(dlg.m_nPicID);
						pLiquidLevel->SetLayer(dlg.m_nLayer);
						pLiquidLevel->SetBindPoint(dlg.m_strBindPoint);
						pLiquidLevel->SetMaxValue(dlg.m_nMax);
						pLiquidLevel->SetMinValue(dlg.m_nMin);
						pLiquidLevel->SetHighAlarmLine(dlg.m_fHighAlarmLine);
						pLiquidLevel->SetLowAlarmLine(dlg.m_fLowAlarmLine);
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_CHKBTN_PLUS:
			{
				CEqmDrawButtonPlus* pChkBtnPlus = dynamic_cast<CEqmDrawButtonPlus*>(pEqmDrawBase);
				if (pChkBtnPlus != NULL)
				{
					CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
					CCheckButtonPlusSettingDlg dlg(this);
					dlg.SetPointMap(&pMainFrame->m_plcpointmap);

					dlg.SetLayerId(pChkBtnPlus->GetLayer());
					dlg.SetButtonStyle(pChkBtnPlus->GetButtonStyle());
					dlg.SetTextColor(pChkBtnPlus->GetTextColor());
					dlg.SetTextSize(pChkBtnPlus->GetTextSize());
					dlg.SetSelectValue(pChkBtnPlus->GetValueForCheck());
					dlg.SetUnSelectValue(pChkBtnPlus->GetValueForUnCheck());
					dlg.SetBindPointName(pChkBtnPlus->GetBindPointName());
					dlg.SetTextContent(pChkBtnPlus->GetTextContent());
					dlg.SetGroupId(pChkBtnPlus->GetGroupID());
					dlg.SetExpression(pChkBtnPlus->GetExpression());
					dlg.SetOptionsDec(pChkBtnPlus->GetOptionsDec());

					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pChkBtnPlus,E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(dlg.GetImagePathSelectComm());
						if (pBitmap != NULL)
						{
							pChkBtnPlus->SetDefaultBitmap(pBitmap);
						}
						pChkBtnPlus->SetLayer(dlg.GetLayerId());
						pChkBtnPlus->SetButtonStyle(dlg.GetButtonStyle());
						pChkBtnPlus->SetTextColor(dlg.GetTextColor());
						pChkBtnPlus->SetTextSize(dlg.GetTextSize());
						pChkBtnPlus->SetValueForCheck(dlg.GetSelectValue());
						pChkBtnPlus->SetValueForUnCheck(dlg.GetUnSelectValue());
						pChkBtnPlus->SetBindPointName(dlg.GetBindPointName());
						pChkBtnPlus->SetTextContent(dlg.GetTextContent());
						pChkBtnPlus->SetGroupID(dlg.GetGroupId());
						pChkBtnPlus->SetExpression(dlg.GetExpression());
						if (!pChkBtnPlus->CheckExpression(dlg.GetExpression()))
						{
							MessageBox(_T("表达式格式错误！"), _T("Error"), MB_OK|MB_ICONERROR);
						}

						pChkBtnPlus->SetPathCheckCom(dlg.GetImagePathSelectComm());
						pChkBtnPlus->SetPathCheckOver(dlg.GetImagePathSelectOver());
						pChkBtnPlus->SetPathUncheckCom(dlg.GetImagePathUnSelectComm());
						pChkBtnPlus->SetPathUncheckOver(dlg.GetImagePathUnSelectOver());
						pChkBtnPlus->SetOptionsDec(dlg.GetOptionsDec());

						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_LINE:
			{
				//CEqmDrawScrawl* pScrawl = dynamic_cast<CEqmDrawScrawl*>(pEqmDrawBase);
				//if (pScrawl != NULL)
				//{
				//	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
				//	CScrawlSettingDlg dlg;
				//	dlg.SetLayer(pScrawl->GetLayer());
				//	dlg.SetShowWords(pScrawl->GetShowWords());

				//	vector<CurveColumn> vecCurve;
				//	vector<CurveColumn> vecColumn;
				//	vector<CurveColumn> vecCurCol;
				//	vector<CurveColumn>::const_iterator	iterCurCol;
				//	vecCurCol = pScrawl->GetCurveColumn();
				//	for (iterCurCol=vecCurCol.begin(); iterCurCol!=vecCurCol.end(); ++iterCurCol)
				//	{
				//		if (scrawlCurve == iterCurCol->scrawlType)
				//		{
				//			vecCurve.push_back(*iterCurCol);
				//		}
				//		else
				//		{
				//			vecColumn.push_back(*iterCurCol);
				//		}
				//	}
				//	dlg.SetCurveColumn(vecCurve, vecColumn);

				//	if (IDOK == dlg.DoModal())
				//	{
				//		CommandManager::Instance()->CallCommand(pScrawl,E_COMMAND_EDIT);
				//		Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\scrawl.png");
				//		if (pBitmap != NULL)
				//		{
				//			//pScrawl->SetWidth(pBitmap->GetWidth());
				//			//pScrawl->SetHeight(pBitmap->GetHeight());
				//			pScrawl->SetDefaultBitmap(pBitmap);
				//		}
				//		pScrawl->SetLayer(dlg.GetLayer());
				//		pScrawl->SetShowWords(dlg.GetShowWords());

				//		dlg.GetCurveColumn(vecCurve, vecColumn);
				//		vecCurCol = MergeScrawlVectors(vecCurve, vecColumn);
				//		pScrawl->SetCurveColumn(vecCurCol);

				//		Invalidate(FALSE);
				//	}
				//}
			}
			break;
		case EQMDRAW_HISTORY_TREND:
			{
				CEqmDrawHistoryTrend* pHistoryTrend = dynamic_cast<CEqmDrawHistoryTrend*>(pEqmDrawBase);
				if (pHistoryTrend != NULL)
				{
					CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
					CHistoryTrendSettingDlg dlg;

					int nLayer = pHistoryTrend->GetLayer();
					HistoryTrendType type = pHistoryTrend->GetHistoryTrendType();
					vector<HistoryTrendInfo> vecInf = pHistoryTrend->GetHistoryTrendVector();

					dlg.SetLayer(nLayer);
					dlg.SetHistoryTrendType(type);
					dlg.SetHistoryTrendVector(vecInf);

					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pHistoryTrend,E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\historyTrend.png");
						if (pBitmap != NULL)
						{
							pHistoryTrend->SetDefaultBitmap(pBitmap);
						}

						nLayer = dlg.GetLayer();
						type = dlg.GetHistoryTrendType();
						vecInf = dlg.GetHistoryTrendVector();

						pHistoryTrend->SetLayer(nLayer);
						pHistoryTrend->SetHistoryTrendType(type);
						pHistoryTrend->SetHistoryTrendVector(vecInf);
						Invalidate(FALSE);
					}
				}
			}
			break;
		/*case EQMDRAW_ENERGY_SAVE_ROI:*/
		case EQMDRAW_RECTANGLE:
			{
				CEqmDrawRectangle* pRectangle = dynamic_cast<CEqmDrawRectangle*>(pEqmDrawBase);
				CEqmRectangleConfigDlg dlg(pRectangle, this);
				dlg.DoModal();
			}
			break;
		case EQMDRAW_MAP_NAVIGATE://标准设备
			{
				CEqmDrawMapNavigate* pMapNavigate = dynamic_cast<CEqmDrawMapNavigate*>(pEqmDrawBase);
				if (pMapNavigate != NULL)
				{
					CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
					
  				 	CStandardEquip dlg;
					dlg.m_nLayer=pEqmDrawBase->GetLayer();
					dlg.m_strTemplatePageName	= pMapNavigate->GetEquipTemplatePageName();	//页面模版 泵名字
					dlg.m_nTemplatePageId		= pMapNavigate->GetEquipTemplatePageId();		// 450001415
					dlg.m_nTempateGroupId		= pMapNavigate->GetEquipTemplateGroupId();	//编号 3
					dlg.m_strTemplateParam		= pMapNavigate->GetParamListAllString();		//	参数
					dlg.m_cstrTemplateName		= pMapNavigate->GetTemplateName();
					dlg.ppMapNavigate			= pMapNavigate;
					dlg.m_bBanOnDisplay			= pMapNavigate->GetBanOnDisplay();//获取禁止显示的情况
					dlg.m_bCreate = false;

					if (IDOK == dlg.DoModal())
					{
					
						CommandManager::Instance()->CallCommand(pMapNavigate,E_COMMAND_EDIT);
						
						CString strTemFile;
						CString cstrTemlpateNameNo4db;
						int nWidth, nHeight;

						cstrTemlpateNameNo4db = dlg.m_cstrTemplateName.Left(dlg.m_cstrTemplateName.Find(L".4db"));
						/*strTemFile.Format(_T("%sPNG%d.png"), cstrTemlpateNameNo4db, dlg.m_nTemplatePageId);
						strTemFile = m_strSysPath + L"\\temp\\" + strTemFile;*/
						DirectoryOperations::MakeDirectory(m_strSysPath+L"\\temp",cstrTemlpateNameNo4db);
						strTemFile.Format(L"%s\\temp\\%s\\%d.png", m_strSysPath, cstrTemlpateNameNo4db, dlg.m_nTemplatePageId);
						
						pMainFrame->m_pPrjSqlite->ExtractTemplatePNGFile(dlg.m_cstrTemplateName, dlg.m_nTemplatePageId, strTemFile, nWidth, nHeight);
						Image* pBitmap = Image::FromFile(strTemFile);

						if (pBitmap != NULL)
						{
							pMapNavigate->SetDefaultBitmap(pBitmap);
						}
                        pMapNavigate->SetEquipTemplateGroupId(dlg.m_nTempateGroupId);
						pMapNavigate->SetEquipTemplatePageId(dlg.m_nTemplatePageId);
						pMapNavigate->SetEquipTemplatePageName(dlg.m_strTemplatePageName);
						pMapNavigate->SetParamListAllString(dlg.m_strTemplateParam);
						pMapNavigate->SetLayer(dlg.m_nLayer);
						pMapNavigate->SetTemplateName(dlg.m_cstrTemplateName);
						pMapNavigate->SetBanOnDisplay(dlg.m_bBanOnDisplay);//设置禁止显示的情况
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_NAVIGATE_WINDOW:
			{
				CEqmDrawNavigateWindow* pNaviWind = dynamic_cast<CEqmDrawNavigateWindow*>(pEqmDrawBase);
				if (pNaviWind != NULL)
				{
					CNavigateWindowSettingDlg dlg;
					dlg.SetLayer(pNaviWind->GetLayer());
					dlg.SetPageInfo(pNaviWind->GetPageInfo());

					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pNaviWind,E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\NavigateWindow.png");
						if (pBitmap != NULL)
						{
							pNaviWind->SetDefaultBitmap(pBitmap);
						}

						pNaviWind->SetLayer(dlg.GetLayer());
						pNaviWind->SetPageInfo(dlg.GetPageInfo());
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_LEVEL_RULER:
			{
				CEqmDrawLevelRuler* pLevelRuler = dynamic_cast<CEqmDrawLevelRuler*>(pEqmDrawBase);
				if (pLevelRuler != NULL)
				{
					CLevelRulerSettingDlg dlg;
					dlg.m_nLayer = pLevelRuler->GetLayer();
					dlg.m_strName = pLevelRuler->GetName();
					dlg.m_dMax = pLevelRuler->GetMax();
					dlg.m_dMin = pLevelRuler->GetMin();
					dlg.m_dMainScaleInterval = pLevelRuler->GetMainScaleInterval();
					dlg.m_dMinorScaleInterval = pLevelRuler->GetMinorScaleInterval();
					dlg.m_nDecimalPlace = pLevelRuler->GetDecimalPlace();
					dlg.SetLabelInfo(pLevelRuler->GetLabelInfo());
					dlg.SetSectionInfo(pLevelRuler->GetSectionInfo());

					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pLevelRuler,E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\LevelRuler.png");
						if (pBitmap != NULL)
						{
							pLevelRuler->SetDefaultBitmap(pBitmap);
						}

						pLevelRuler->SetLayer(dlg.m_nLayer);
						pLevelRuler->SetName(dlg.m_strName);
						pLevelRuler->SetMax(dlg.m_dMax);
						pLevelRuler->SetMin(dlg.m_dMin);
						pLevelRuler->SetMainScaleInterval(dlg.m_dMainScaleInterval);
						pLevelRuler->SetMinorScaleInterval(dlg.m_dMinorScaleInterval);
						pLevelRuler->SetDecimalPlace(dlg.m_nDecimalPlace);
						pLevelRuler->SetLabelInfo(dlg.GetLabelInfo());
						pLevelRuler->SetSectionInfo(dlg.GetSectionInfo());
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_DATA_ANALYSE:
			{
				CEqmDrawDataAnalyse* pDataAnalyse = dynamic_cast<CEqmDrawDataAnalyse*>(pEqmDrawBase);
				if (pDataAnalyse != NULL)
				{
					CDataAnalyseSettingDlg dlg(&pMainFrame->m_plcpointmap);
					dlg.m_nLayer = pDataAnalyse->GetLayer();

					dlg.SetDaPointInfo(pDataAnalyse->GetDaPointInfo());
					dlg.m_strPtTime = pDataAnalyse->GetPointTime();
					dlg.m_strPtCount = pDataAnalyse->GetPointCount();
					dlg.m_strPtStatus = pDataAnalyse->GetPointStatus();

					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pDataAnalyse, E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataAnalyse.png");
						if (pBitmap != NULL)
						{
							pDataAnalyse->SetDefaultBitmap(pBitmap);
						}

						pDataAnalyse->SetLayer(dlg.m_nLayer);

						pDataAnalyse->SetDaPointInfo(dlg.GetDaPointInfo());
						pDataAnalyse->SetPointTime(dlg.m_strPtTime);
						pDataAnalyse->SetPointCount(dlg.m_strPtCount);
						pDataAnalyse->SetPointStatus(dlg.m_strPtStatus);

						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_TEMPER_DISTRI:
			{
				CEqmDrawTemperDistri* pTemperDistri = dynamic_cast<CEqmDrawTemperDistri*>(pEqmDrawBase);
				if (pTemperDistri != NULL)
				{
					CTemperDistriSettingDlg dlg(this);
					dlg.m_nLayer = pTemperDistri->GetLayer();
					dlg.SetTemperDistriInfo(pTemperDistri->GetTemperDistri());

					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pTemperDistri, E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\TemperDistri.png");
						if (pBitmap != NULL)
						{
							pTemperDistri->SetDefaultBitmap(pBitmap);
						}

						pTemperDistri->SetLayer(dlg.m_nLayer);
						pTemperDistri->SetTemperDistri(dlg.GetTemperDistriInfo());
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_DATA_REPORT:
			{
				CEqmDrawDataReport* pDataReport = dynamic_cast<CEqmDrawDataReport*>(pEqmDrawBase);
				if (pDataReport != NULL)
				{
					CDataReportSettingDlg dlg;
					dlg.m_nLayer = pDataReport->GetLayer();
					dlg.m_strPath = pDataReport->GetPath();

					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pDataReport, E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataReport.png");
						if (pBitmap != NULL)
						{
							pDataReport->SetDefaultBitmap(pBitmap);
						}

						pDataReport->SetLayer(dlg.m_nLayer);
						pDataReport->SetPath(dlg.m_strPath);
						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_PLANE_TEMPE_DISTR:
			{
				CEqmDrawPlaneTempeDistr* pPlaneTempeDistr = dynamic_cast<CEqmDrawPlaneTempeDistr*>(pEqmDrawBase);
				if (pPlaneTempeDistr != NULL)
				{
					CPlaneTempeDistrSettingDlg dlg(&pMainFrame->m_plcpointmap);
					dlg.m_nLayer = pPlaneTempeDistr->GetLayer();
					dlg.m_strPointName = pPlaneTempeDistr->GetPointName();
					dlg.m_nCfgType = pPlaneTempeDistr->GetConfigType();

					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pPlaneTempeDistr, E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\PlaneTempeDistr.png");
						if (pBitmap != NULL)
						{
							pPlaneTempeDistr->SetDefaultBitmap(pBitmap);
						}

						pPlaneTempeDistr->SetLayer(dlg.m_nLayer);
						pPlaneTempeDistr->SetPointName(dlg.m_strPointName);
						pPlaneTempeDistr->SetConfigType(dlg.m_nCfgType);

						SyncPlaneTemperDistri(dlg.m_strPointName);

						Invalidate(FALSE);
					}
				}
			}
			break;
		case EQMDRAW_GENERAL_COMPONENT:
			{
				CEqmDrawGeneralComponent* pGeneralComponent = dynamic_cast<CEqmDrawGeneralComponent*>(pEqmDrawBase);
				if (pGeneralComponent != NULL)
				{
					CGeneralComponentSettingDlg dlg;
					dlg.m_nLayer = pGeneralComponent->GetLayer();
					dlg.m_strCfgValue = pGeneralComponent->GetConfigValue();

					if (IDOK == dlg.DoModal())
					{
						CommandManager::Instance()->CallCommand(pGeneralComponent, E_COMMAND_EDIT);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\GeneralComponent.png");
						if (pBitmap != NULL)
						{
							pGeneralComponent->SetDefaultBitmap(pBitmap);
						}

						pGeneralComponent->SetLayer(dlg.m_nLayer);
						pGeneralComponent->SetConfigValue(dlg.m_strCfgValue);
						Invalidate(FALSE);
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

void CCanvasDlg::ShowChartInfoDlg(CEqmDrawDevice* pDevice)
{
	ASSERT(pDevice);
	CommandManager::Instance()->CallCommand(pDevice,E_COMMAND_EDIT);
	CChartPropertyDlg dlg(pDevice);
	dlg.DoModal();
}

bool CCanvasDlg::CreateLineGraphic(CPoint &point)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawLineGraphic *pEqm = new CEqmDrawLineGraphic();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartLine.png");
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddLineGraphic(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateBarGraphic(CPoint &point)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawBarGraphic *pEqm = new CEqmDrawBarGraphic();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartBar.png");
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddBarGraphic(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

bool CCanvasDlg::CreatePieGraphic(CPoint &point)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawPieGraphic *pEqm = new CEqmDrawPieGraphic();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartPie.png");
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddPieGraphic(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateBend(CPoint &point, const int nBendSize)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawBend *pEqm = new CEqmDrawBend();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(4);

	pEqm->SetSize(nBendSize);
	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\bend.png");
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		int nBendSizeShow = 2 * nBendSize + 13;
		pEqm->SetWidth(nBendSizeShow);
		pEqm->SetHeight(nBendSizeShow);
		pMainFrame->m_DrawContext.pPage->AddBend(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

void CCanvasDlg::CreateDashBoard(CPoint &point, const int nID)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawDashBoard *pEqm = new CEqmDrawDashBoard();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nID);
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		if (nID == 52)
		{
			pEqm->SetStyle(0);
		}
		else if (nID == 76)
		{
			pEqm->SetStyle(1);
		}
		pMainFrame->m_DrawContext.pPage->AddDashboard(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return;
	}
}

bool CCanvasDlg::CreateDashBoardByType(CPoint &point, const int nDashBoardType)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawDashBoard *pEqm = new CEqmDrawDashBoard();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(4);

	Image* pBitmap = NULL;
	if(nDashBoardType == 0)
	{
		pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\meterpan_0.png");
	}
	else if(nDashBoardType == 1)
	{
		pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\meterpan_1.png");
	}
	else if(nDashBoardType == 2)
	{
		//仪表盘3图片地址
		pBitmap = Image::FromFile(m_strSysPath + L"\\\Images\\meterpan_2.png");
	}
	else if(nDashBoardType == 3)
	{
		//仪表盘4图片地址
		pBitmap = Image::FromFile(m_strSysPath + L"\\\Images\\meterpan_3.png");
	}
	else
	{	// error, unknown type, set as type = 0
		pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\meterpan_0.png");
	}

	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pEqm->SetStyle(nDashBoardType);		
		pMainFrame->m_DrawContext.pPage->AddDashboard(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}
CEqmDrawButton* CCanvasDlg::CreateButton( CPoint &point, const int nIDPicComm, const int nIDPicDisable, const int nIDPicDown, const int nIDPicOver, const int nLinkPageID, const CString strBindSetPointName, const int nIDPicChecked)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawButton *pEqm = new CEqmDrawButton();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return NULL;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return NULL;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->Set5PicID(nIDPicComm, nIDPicDisable, nIDPicDown, nIDPicOver, nIDPicChecked);
	pEqm->SetLinkPageID(nLinkPageID);
	pEqm->SetBindSetPointName(strBindSetPointName);
	Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nIDPicComm, true);
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddButton(pEqm);  
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
		return pEqm;
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return NULL;
	}
}

CEqmDrawButton* CCanvasDlg::CreateButtonNewStyle(CPoint &point, const int nIDPicComm, const int nIDPicDisable, const int nIDPicDown, const int nIDPicOver, const int nLinkPageID, const CString strBindSetPointName, const int nIDPicChecked, Image* pBitmap)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawButton *pEqm = new CEqmDrawButton();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return NULL;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return NULL;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->Set5PicID(nIDPicComm, nIDPicDisable, nIDPicDown, nIDPicOver, nIDPicChecked);
	pEqm->SetLinkPageID(nLinkPageID);
	pEqm->SetBindSetPointName(strBindSetPointName);
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddButton(pEqm);  
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
		return pEqm;
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return NULL;
	}
}

bool CCanvasDlg::CreateTimeSelect( CPoint &point)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawTimeSelect *pEqm = new CEqmDrawTimeSelect();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\timeSelect.png");
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(140);
		pEqm->SetHeight(37);
		pMainFrame->m_DrawContext.pPage->AddTimeSelect(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateTimeBar( CPoint &point )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawTimeBar *pEqm = new CEqmDrawTimeBar();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartBar.png");
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddTimeBar(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

void CCanvasDlg::CreatePolygon(CPoint point)
{
	CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return;
	}
	
	if ( pMainFrame->m_DrawContext.bIsCreatePolygon )
	{
		CEqmDrawPolygon *pPolygon = dynamic_cast<CEqmDrawPolygon*>(pMainFrame->m_DrawContext.pNewEqmt);
		if(pPolygon != NULL)
		{
			pPolygon->PushPoint(Gdiplus::PointF(point.x, point.y));
		}
		Invalidate(FALSE);
	}
	else
	{
		pMainFrame->m_DrawContext.bIsCreatePolygon = true;
		CEqmDrawPolygon *pNewPipe = new CEqmDrawPolygon();
		pNewPipe->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
		pNewPipe->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
		pNewPipe->SetID(++pMainFrame->m_MaxEqmtID);
		pNewPipe->SetLayer(4);
		pNewPipe->PushPoint(Gdiplus::PointF(point.x, point.y));
		pMainFrame->m_DrawContext.pNewEqmt = pNewPipe;
		pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_POLYGON;
		CommandManager::Instance()->CallCommand(pNewPipe,E_COMMAND_ADD);
	}
}

bool CCanvasDlg::CreateProgressBar( CPoint point )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawProgressBar *pEqm = new CEqmDrawProgressBar();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\progressBar.png");
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddProgressBar(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateDiagnoseGraphic( CPoint &point )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawDiagnoseGraphic *pEqm = new CEqmDrawDiagnoseGraphic();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartDiagnose.png");
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddDiagnoseGraphic(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateAreaChange( CPoint &point, const int nID )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawAreaChange *pEqm = new CEqmDrawAreaChange();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetPicID(nID);
	Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nID, true);
	if(pBitmap == NULL) 
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}
	pEqm->SetBitmap(pBitmap);
	pEqm->SetWidth(pBitmap->GetWidth());
	pEqm->SetHeight(pBitmap->GetHeight());
	pMainFrame->m_DrawContext.pPage->AddAreaChange(pEqm);
	SetMetaSelect(pEqm, pMainFrame);
	CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	return true;
}

void CCanvasDlg::ShowAreaChangeDlg()
{
	//有没有用？？？   严观微
}

bool CCanvasDlg::CreateShellExecute(CPoint &point, const CString strServerIP, const CString strFileName, const CString strDownloadDirectory, const int nBKPicID, const bool bAuto, const int nLayer, const CString strFileID)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawShellExecute *pEqm = new CEqmDrawShellExecute();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetServerIP(strServerIP);
	pEqm->SetFileID(strFileID);
	pEqm->SetFileName(strFileName);
	pEqm->SetDownloadDirectory(strDownloadDirectory);
	pEqm->SetBKPicID(nBKPicID);
	pEqm->SetAutoDownload(bAuto);
	pEqm->SetLayer(nLayer);
	Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nBKPicID, true);
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddShellExecute(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

void CCanvasDlg::OnPosition()
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	CPoint actpoint = m_rightclicksavepoint;
	actpoint.x /= pMainFrame->m_DrawContext.m_fScreenScale;
	actpoint.y /= pMainFrame->m_DrawContext.m_fScreenScale;
	pMainFrame->m_DrawContext.actPt.X = actpoint.x;
	pMainFrame->m_DrawContext.actPt.Y = actpoint.y;
	CEqmDrawBase *pEqmDrawBase = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
	if (pEqmDrawBase)
	{
		CSetEqmPositionDlg dlg;
		dlg.m_x = pEqmDrawBase->GetPos().X;
		dlg.m_y = pEqmDrawBase->GetPos().Y;
		dlg.m_width = pEqmDrawBase->GetWidth();
		dlg.m_height = pEqmDrawBase->GetHeight();
		if (dlg.DoModal() == IDOK)
		{
			pEqmDrawBase->SetWidth(dlg.m_width);
			pEqmDrawBase->SetHeight(dlg.m_height);
			pEqmDrawBase->SetPos(Gdiplus::PointF(dlg.m_x,dlg.m_y));
			Invalidate();
		}
	}
}

bool CCanvasDlg::CreateEmbededPage( CPoint &point, const int nPicID, const vector<PageIDName>& vec, const int nChangeInterval, const int nLayer )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CEqmDrawEmbededPage *pEqm = new CEqmDrawEmbededPage();
	ASSERT(pEqm);
	if(!pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetPicID(nPicID);
	pEqm->SetChangeInterval(nChangeInterval);
	pEqm->SetLayer(nLayer);
	pEqm->ClearPageList();
	for(size_t i=0;i<vec.size();++i)
	{
		pEqm->InsertPageList(vec[i].nPageID, vec[i].strPageName);
	}
	Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nPicID, true);
	if (pBitmap)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddEmbededPage(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateLiquidLevel(CPoint &point, const int nLayer, const int nPicID, const CString strBindPoint, const int nMax, const int nMin, const float fHighAlarmLine, const float fLowAlarmLine )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CEqmDrawLiquidLevel *pEqm = new CEqmDrawLiquidLevel();
	ASSERT(pEqm != NULL);
	if(NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	
	pEqm->SetLayer(nLayer);
	//pEqm->SetPicID(nPicID);
	pEqm->SetBindPoint(strBindPoint);
	pEqm->SetMaxValue(nMax);
	pEqm->SetMinValue(nMin);
	pEqm->SetHighAlarmLine(fHighAlarmLine);
	pEqm->SetLowAlarmLine(fLowAlarmLine);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\liquidLevel.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddLiquidLevel(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}

CEqmDrawButtonPlus* CCanvasDlg::CreateCheckButtonPlus(CPoint &point, const eButtonStyle eBtnStyle, const int nLayer, const COLORREF colorTxt, const int nTxtSize,
	const CString strSelVal, const CString strUnSelVal, const CString strBindPointName, const CString strTxtContent, const int nGroupId, const CString strExpression,
	const CString strSelCom, const CString strSelOver, const CString strUnSelCom, const CString strUnSelOver, const CString strOptionsDec)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CEqmDrawButtonPlus *pEqm = new CEqmDrawButtonPlus();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return NULL;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return NULL;
	}
	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetButtonStyle(eBtnStyle);
	pEqm->SetTextColor(colorTxt);
	pEqm->SetTextSize(nTxtSize);
	pEqm->SetValueForCheck(strSelVal);
	pEqm->SetValueForUnCheck(strUnSelVal);
	pEqm->SetBindPointName(strBindPointName);
	pEqm->SetTextContent(strTxtContent);
	pEqm->SetGroupID(nGroupId);
	pEqm->SetExpression(strExpression);

	pEqm->SetPathCheckCom(strSelCom);
	pEqm->SetPathCheckOver(strSelOver);
	pEqm->SetPathUncheckCom(strUnSelCom);
	pEqm->SetPathUncheckOver(strUnSelOver);
	pEqm->SetOptionsDec(strOptionsDec);
	Image* pBitmap = Image::FromFile(strSelCom);
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddCheckButtonPlus(pEqm);
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return NULL;
	}

	return pEqm;
}

bool CCanvasDlg::CreateDrawl(CPoint &point, const int nLayer, const int nPicID, const CString strShowWords, const vector<CurveColumn>& vecCurve, const vector<CurveColumn>& vecColumn)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	vector<CurveColumn>	vecCurCol;
	vector<CurveColumn>::const_iterator	iter;

	CEqmDrawScrawl *pEqm = new CEqmDrawScrawl();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetShowWords(strShowWords);
	vecCurCol = MergeScrawlVectors(vecCurve, vecColumn);
	pEqm->SetCurveColumn(vecCurCol);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\scrawl.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddScrawlBoard(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}

vector<CurveColumn> CCanvasDlg::MergeScrawlVectors(const vector<CurveColumn>& vec1, const vector<CurveColumn>& vec2)
{
	vector<CurveColumn>	vecRet;
	vector<CurveColumn>::const_iterator	iter;

	vecRet = vec1;
	for (iter=vec2.begin(); iter!=vec2.end(); ++iter)
	{
		vecRet.push_back(*iter);
	}

	return vecRet;
}

bool CCanvasDlg::CreateHistoryTrend(CPoint &point, const int nPicID, const int nLayer, const HistoryTrendType& type, const vector<HistoryTrendInfo>& vecHisTrendInf)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawHistoryTrend *pEqm = new CEqmDrawHistoryTrend();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetHistoryTrendType(type);
	pEqm->SetHistoryTrendVector(vecHisTrendInf);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\historyTrend.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddHistoryTrend(pEqm); 
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}
//bool CCanvasDlg::CreateRectangle(CPoint &point, const int nPicID)
bool CCanvasDlg::CreateEnergySaveRoi(CPoint &point, const int nPicID)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}
	
	/*CEqmDrawEnergySaveRoi *pEqm = new CEqmDrawEnergySaveRoi();*/
	CEqmDrawRectangle *pEqm = new CEqmDrawRectangle();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(4);

	pMainFrame->m_DrawContext.pPage->AddRectangle(pEqm);
	SetMetaSelect(pEqm, pMainFrame);
	CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	return true;
}

void CCanvasDlg::InsertImageIntoOpenS3db(const int nFindId, int& nUseId)
{
	CMainFrame *pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	CProjectSqlite* pPrjSql = pMainFrm->m_pPrjSqlite;
	ASSERT(pPrjSql != NULL);
	if (NULL == pPrjSql)
	{
		return;
	}

	//大概是从BEOPImageLib.s3db加载的列表中里找到导入的图
	PictureInfo* pSrcInfo = pPrjSql->GetPictureInfoByID(nFindId, false);
	ASSERT(pSrcInfo != NULL);
	if (NULL == pSrcInfo)
	{
		return;
	}

	USES_CONVERSION;
	//大概是从打开的4db中加载的资源中找图。
	PictureInfo* pDstInfo = pPrjSql->GetPictureInfo(pSrcInfo->szName, pSrcInfo->nSize, pSrcInfo->remark, pSrcInfo);
	if (NULL == pDstInfo)
	{	// 打开的s3db没有该图，直接插入数据库
		int nNewId = pPrjSql->GetMaxPictureNum(true) + 1;
		pPrjSql->InsertPictureData(nNewId, T2A(pSrcInfo->szName), Project::Tools::WideCharToAnsi(pSrcInfo->etype_equip.c_str()).c_str(), pSrcInfo->pBlock, pSrcInfo->nSize, true, 0, 0, T2A((pSrcInfo->etype_p).c_str()));

		// 插入内存结构
		pPrjSql->AddIntoPictureList(nNewId, pSrcInfo);

		nUseId = nNewId;
	}
	else
	{	// 打开的s3db有该图
		nUseId = pDstInfo->ID;
	}


/*
	USES_CONVERSION;
	PictureInfo* pDstInfo = pPrjSql->GetPictureInfoByID(nFindId, true);
	if (NULL == pDstInfo)
	{	// 打开的s3db没有该编号的图，直接插入数据库
		nUseId = nFindId;
		pPrjSql->InsertPictureData(nUseId, T2A(pSrcInfo->szName), Project::Tools::WideCharToAnsi(pSrcInfo->etype_equip.c_str()).c_str(), pSrcInfo->pBlock, pSrcInfo->nSize, true);

		// 插入内存结构
		pPrjSql->AddIntoPictureList(pSrcInfo->ID, pSrcInfo);
	}
	else
	{	// 打开的s3db有该编号的图
		int nCmp = wcscmp(pSrcInfo->szName, pDstInfo->szName);
		if (0 == nCmp && pSrcInfo->nSize == pDstInfo->nSize)
		{
			// 文件名和文件大小都相同，则认为是同一个文件，跳过
			nUseId = nFindId;
		}
		else
		{
			// 有该编号的图，但文件名或者文件大小不同，则取目标s3db最大编号+1，插入
			nUseId = pPrjSql->GetMaxLibImageNum(true) + 1;
			pPrjSql->InsertPictureData(nUseId, T2A(pSrcInfo->szName), Project::Tools::WideCharToAnsi(pSrcInfo->etype_equip.c_str()).c_str(), pSrcInfo->pBlock, pSrcInfo->nSize, true);

			// 插入内存结构
			pPrjSql->AddIntoPictureList(nUseId, pSrcInfo);
		}
	}
*/
}

void CCanvasDlg::InsertAnimationIntoOpenS3db(const int nAnimId, int& nUseId)
{
	CMainFrame *pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	CProjectSqlite* pPrjSql = pMainFrm->m_pPrjSqlite;
	ASSERT(pPrjSql != NULL);
	if (NULL == pPrjSql)
	{
		return;
	}


	try
	{
		const AnimationInfo* pSrcInfo = pPrjSql->GetAnimationByID(nAnimId, false);
		ASSERT(pSrcInfo != NULL);
		if (NULL == pSrcInfo)
		{	// 图源库没有该动画
			return;
		}

		const AnimationInfo* pDstInfo = pPrjSql->GetAnimationByID(nAnimId, true);
		if (NULL == pDstInfo)
		{	// 打开的s3db没有该编号的图，直接插入数据库
			nUseId = nAnimId;
			pPrjSql->SetAnimationDataEx(nUseId, pSrcInfo, true);
		}
		else
		{	// 打开的s3db有该编号的图
			if (0 == wcscmp(pSrcInfo->szName.c_str(), pDstInfo->szName.c_str())
				&& pSrcInfo->vecPicture.size() == pDstInfo->vecPicture.size()
				&& pSrcInfo->vecPicIDList.size() == pDstInfo->vecPicIDList.size()
				&& pSrcInfo->vecPicIDList.at(0) == pDstInfo->vecPicIDList.at(0)
				)
			{
				// 文件名相同，则认为是同一个文件，跳过
				nUseId = nAnimId;
			}
			else
			{
				// 有该编号的图，但文件名不同，则取目标s3db最大编号+1，插入
				nUseId = pPrjSql->GetMaxAnimationNum(true) + 1;
				pPrjSql->SetAnimationDataEx(nUseId, pSrcInfo, true);
			}
		}
	}
	catch (...)
	{
		return;
	}
}
/*
 *函数说明：创建标准设备的类，解压出标准设备图片
 *参数：
 *	point[in]:
 *	nLayer[in]:
 *	nTemplateGroupId[in]:
 *	nTemplatePageId[in]:
 *	strTemplateName[in]:
 *	cstrParamList[in]:
 *	cstrTemplateName[in]:模板名;例："template.4db";
 *
*/
bool CCanvasDlg::CreateMapNavigate(CPoint &point, const int nLayer,int nTemplateGroupId,  int nTemplatePageId, CString strTemplatePageName,CString cstrParamList, CString cstrTemplateName)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawMapNavigate *pEqm = new CEqmDrawMapNavigate();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);
	pEqm->SetEquipTemplateGroupId(nTemplateGroupId);
	pEqm->SetEquipTemplatePageId(nTemplatePageId);
	pEqm->SetEquipTemplatePageName(strTemplatePageName);
	pEqm->SetParamListAllString(cstrParamList);
	pEqm->SetTemplateName(cstrTemplateName);

	int nWidth, nHeight;
	CString strTemFile;
	CString cstrNo4db = cstrTemplateName.Left(cstrTemplateName.Find(L".4db"));

	DirectoryOperations::MakeDirectory(m_strSysPath+L"\\temp",cstrNo4db);
	strTemFile.Format(L"%s\\temp\\%s\\%d.png", m_strSysPath, cstrNo4db, nTemplatePageId);

	pMainFrame->m_pPrjSqlite->ExtractTemplatePNGFile(cstrTemplateName, nTemplatePageId, strTemFile, nWidth, nHeight);
	Image* pBitmap = Image::FromFile(strTemFile);
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddMapNavigate(pEqm);
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm, E_COMMAND_ADD);
	}
	else
	{
		AfxMessageBox(_T("Factory资源图片缺少TemplateIcon.png文件，请检查安装目录"));
		SAFE_DELETE(pEqm);
		return false;
	}
	return true;
}

bool CCanvasDlg::CreateNavigateWindow(CPoint &point, const int nLayer, const vector<PageInfo>& vecPageInfo)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawNavigateWindow *pEqm = new CEqmDrawNavigateWindow();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetPageInfo(vecPageInfo);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\NavigateWindow.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddNavigateWindow(pEqm);
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}

void CCanvasDlg::ShowSelectElementByID( const int nElementID )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	if (NULL == pMainFrame)
	{
		return;
	}
	if (NULL == pMainFrame->m_DrawContext.pPage)
	{
		return;
	}

	pMainFrame->m_DrawContext.ClearSelect();
	for (list<CEqmDrawDevice*>::iterator it = pMainFrame->m_DrawContext.pPage->GetDrawDeviceList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetDrawDeviceList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//设备
	for (list<CEqmDrawPipe*>::iterator it = pMainFrame->m_DrawContext.pPage->GetPagePipeList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetPagePipeList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//管道
	for (list<CEqmDrawText*>::iterator it = pMainFrame->m_DrawContext.pPage->GetPageTextList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetPageTextList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//文字
	for ( list<CEqmDrawGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->Get2DGraphicList().begin();
		it != pMainFrame->m_DrawContext.pPage->Get2DGraphicList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//二维图形
	for ( list<CEqmDrawBarGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->GetBarGraphList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetBarGraphList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//柱状图
	for ( list<CEqmDrawBend*>::iterator it = pMainFrame->m_DrawContext.pPage->GetBendList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetBendList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//弯头
	for ( list<CEqmDrawDashBoard*>::iterator it = pMainFrame->m_DrawContext.pPage->GetDashboardList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetDashboardList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//仪表盘
	for ( list<CEqmDrawLineGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->GetLineGraphList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetLineGraphList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//折线图
	for ( list<CEqmDrawPieGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->GetPieGraphList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetPieGraphList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//饼图
	for ( list<CEqmDrawButton*>::iterator it = pMainFrame->m_DrawContext.pPage->GetButtonList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetButtonList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//按钮
	for ( list<CEqmDrawTimeSelect*>::iterator it = pMainFrame->m_DrawContext.pPage->GetTimeSelectList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetTimeSelectList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//时间选择
	for ( list<CEqmDrawTimeBar*>::iterator it = pMainFrame->m_DrawContext.pPage->GetTimeBarList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetTimeBarList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//时间柱状图
	for ( list<CEqmDrawProgressBar*>::iterator it = pMainFrame->m_DrawContext.pPage->GetProgressBarList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetProgressBarList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//进度条
	for ( list<CEqmDrawDiagnoseGraphic*>::iterator it = pMainFrame->m_DrawContext.pPage->GetDiagnoseList().begin();
		it != pMainFrame->m_DrawContext.pPage->GetDiagnoseList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//诊断图
	for ( list<CEqmDrawPolygon*>::iterator it = pMainFrame->m_DrawContext.pPage->GetPolygon().begin();
		it != pMainFrame->m_DrawContext.pPage->GetPolygon().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			pMainFrame->m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//点阵诊断图
}

void CCanvasDlg::ShowPropertyWnd( const CPoint& point )
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	CPoint actpoint = point;
	actpoint.x /= pMainFrame->m_DrawContext.m_fScreenScale;
	actpoint.y /= pMainFrame->m_DrawContext.m_fScreenScale;
	pMainFrame->m_DrawContext.actPt.X = actpoint.x;
	pMainFrame->m_DrawContext.actPt.Y = actpoint.y;
	CEqmDrawBase *pEqmDrawBase = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
	if ( NULL != pEqmDrawBase )
	{
		switch ( pEqmDrawBase->GetEqmType() )
		{
		case EQMDRAW_TEXT:
			{
				pMainFrame->m_wndProperties.SetTextProperty((CEqmDrawText*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_STRAIGHTPIPE:
			{
				pMainFrame->m_wndProperties.SetPipeProperty((CEqmDrawPipe*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_METABUTTON:
			{
				pMainFrame->m_wndProperties.SetButtonProperty((CEqmDrawButton*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_BEND:
			{
				pMainFrame->m_wndProperties.SetBendProperty((CEqmDrawBend*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_DASHBOARD:
			{
				pMainFrame->m_wndProperties.SetBoardProperty((CEqmDrawDashBoard*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_DIAGNOSE_CHART:
			{
				pMainFrame->m_wndProperties.SetDiagnoseGraphicProperty((CEqmDrawDiagnoseGraphic*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_MAP_NAVIGATE:
			{
				pMainFrame->m_wndProperties.SetMapNavigateProperty((CEqmDrawMapNavigate*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_TIME_SELECT:
			{
				pMainFrame->m_wndProperties.SetTimePickerProperty((CEqmDrawTimeSelect*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_LINE_CHART:
			{
				pMainFrame->m_wndProperties.SetLineChartProperty((CEqmDrawLineGraphic*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_BAR_CHART:
			{
				pMainFrame->m_wndProperties.SetBarChartProperty((CEqmDrawBarGraphic*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_PIE_CHART:
			{
				pMainFrame->m_wndProperties.SetPieChartProperty((CEqmDrawPieGraphic*)pEqmDrawBase);
			}
			break;
		/*case EQMDRAW_ENERGY_SAVE_ROI:*/
		case EQMDRAW_RECTANGLE:
			{
				pMainFrame->m_wndProperties.SetRectangleProperty((CEqmDrawRectangle*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_HISTORY_TREND:
			{
				pMainFrame->m_wndProperties.SetTrendProperty((CEqmDrawHistoryTrend*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_CHKBTN_PLUS:
			{
				pMainFrame->m_wndProperties.SetCheckButtonProperty((CEqmDrawButtonPlus*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_EMBEDED_PAGE:
			{
				pMainFrame->m_wndProperties.SetEmbededProperty((CEqmDrawEmbededPage*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_SHELL_EXECUTE:
			{
				pMainFrame->m_wndProperties.SetShellExecuteProperty((CEqmDrawShellExecute*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_AREA_CHANGE:
			{
				pMainFrame->m_wndProperties.SetAreaChangeProperty((CEqmDrawAreaChange*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_LINE:
			{
				pMainFrame->m_wndProperties.SetStraightLineProperty((CEqmDrawStraightLine*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_LIQUID_LEVEL:
			{
				pMainFrame->m_wndProperties.SetLiquidProperty((CEqmDrawLiquidLevel*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_TIME_BAR:
			{
				pMainFrame->m_wndProperties.SetTimeBarProperty((CEqmDrawTimeBar*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_PROGRESS_BAR:
			{
				pMainFrame->m_wndProperties.SetProgressProperty((CEqmDrawProgressBar*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_NAVIGATE_WINDOW:
			{
				pMainFrame->m_wndProperties.SetNaviProperty((CEqmDrawNavigateWindow*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_DATA_ANALYSE:
			{
				pMainFrame->m_wndProperties.SetAnlyProperty((CEqmDrawDataAnalyse*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_DATA_REPORT:
			{
				pMainFrame->m_wndProperties.SetReportProperty((CEqmDrawDataReport*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_TEMPER_DISTRI:
			{
				pMainFrame->m_wndProperties.SetTemperProperty((CEqmDrawTemperDistri*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_LEVEL_RULER:
			{
				pMainFrame->m_wndProperties.SetRulerProperty((CEqmDrawLevelRuler*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_CHILLER:
		case EQMDRAW_PUMP:
		case EQMDRAW_COOLINGTOWER:
		case EQMDRAW_VALVE:
		case EQMDRAW_METAFILE:
			{
				pMainFrame->m_wndProperties.SetDeviceProperty((CEqmDrawDevice*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_POLYGON:
			{
				pMainFrame->m_wndProperties.SetPolygonProperty((CEqmDrawPolygon*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_PLANE_TEMPE_DISTR:
			{
				pMainFrame->m_wndProperties.SetPlaneTempeDistrProperty((CEqmDrawPlaneTempeDistr*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_DOTLINE_FRAME:
			{
				pMainFrame->m_wndProperties.SetDottedLineFrmProperty((CEqmDrawDottedLineFrm*)pEqmDrawBase);
			}
			break;
		case EQMDRAW_GENERAL_COMPONENT:
			{
				pMainFrame->m_wndProperties.SetGeneralComponentProperty((CEqmDrawGeneralComponent*)pEqmDrawBase);
			}
			break;
		default:
			pMainFrame->m_wndProperties.SetNullProperty();
			break;
		}
	}
	else
	{
		pMainFrame->m_wndProperties.SetNullProperty();
	}
}


void CCanvasDlg::OnRmenuAddtoImglib()
{
	// TODO: Add your command handler code here
	SetImageIntoImageLib(m_rightclicksavepoint);
}

void CCanvasDlg::SetImageIntoImageLib(const CPoint& point)
{
	CMainFrame *pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	CPoint actPoint = point;
	actPoint.x /= pMainFrm->m_DrawContext.m_fScreenScale;
	actPoint.y /= pMainFrm->m_DrawContext.m_fScreenScale;
	pMainFrm->m_DrawContext.actPt.X = actPoint.x;
	pMainFrm->m_DrawContext.actPt.Y = actPoint.y;
	CEqmDrawDevice *pEqmDraw = (CEqmDrawDevice*)pMainFrm->m_DrawProxy.PickObject(pMainFrm->m_DrawContext.pPage,(double)pMainFrm->m_DrawContext.actPt.X,(double)pMainFrm->m_DrawContext.actPt.Y);
	if (NULL != pEqmDraw)
	{
		switch (pEqmDraw->GetEqmType())
		{
		case EQMDRAW_METAFILE:
			{
				InsertImageIntoImageLib(pEqmDraw->GetPicID());

				vector<BindInfo>	vecBindInfo;
				vector<BindInfo>::const_iterator	iter;
				vecBindInfo = pEqmDraw->GetBindInfoVec();
				for (iter=vecBindInfo.begin(); iter!=vecBindInfo.end(); ++iter)
				{
					if (1 == iter->nFrame)
					{	// pic
						InsertImageIntoImageLib(iter->nPicID);
					}
					else
					{	// animation
						InsertAnimationIntoAnimationLib(iter->nPicID);
					}
				}

				MessageBox(_T("添加至图源库完成！"), _T("提示"), MB_OK|MB_ICONASTERISK);
			}
			break;
		default:
			break;
		}
	}

}

bool CCanvasDlg::InsertImageIntoImageLib(const int nPicId)
{
	CMainFrame *pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return false;
	}

	CProjectSqlite* pPrjSql = pMainFrm->m_pPrjSqlite;
	ASSERT(pPrjSql != NULL);
	if (NULL == pPrjSql)
	{
		return false;
	}


	PictureInfo* pSrcInfo = pPrjSql->GetPictureInfoByID(nPicId, true);
	ASSERT(pSrcInfo != NULL);
	if (NULL == pSrcInfo)
	{
		return false;
	}

	wchar_t szName[MAX_PATH] = {0};
	wcscpy_s(szName, MAX_PATH, pSrcInfo->szName);
	int		nSize		= pSrcInfo->nSize;
	wstring	strRemark	= pSrcInfo->remark;

	vector<PictureInfo>::const_iterator	iter;
	int		nCmp		= 0;
	bool	bEqualSize	= false;
	bool	bSameRemark	= false;
	bool	bFind		= false;
	for (iter=pPrjSql->m_vPictureListImage.begin(); iter!=pPrjSql->m_vPictureListImage.end(); ++iter)
	{
		nCmp = wcscmp(iter->szName, szName);
		bEqualSize = (iter->nSize == nSize) ? true : false;
		bSameRemark = (iter->remark == strRemark) ? true : false;
		if (0 == nCmp && bEqualSize && bSameRemark)
		{
			bFind = true;
			break;
		}
	}

	if (bFind)
	{
		return false;
	}

	USES_CONVERSION;
	// 打开的s3db没有该编号的图，直接插入数据库
	pPrjSql->InsertPictureData(nPicId, T2A(pSrcInfo->szName), Project::Tools::WideCharToAnsi(pSrcInfo->etype_equip.c_str()).c_str(), pSrcInfo->pBlock, pSrcInfo->nSize, false);

	// 插入内存结构
	pPrjSql->AddIntoPictureList(pSrcInfo->ID, pSrcInfo, false);


	return true;
}

bool CCanvasDlg::InsertAnimationIntoAnimationLib(const int nAnimId)
{
	CMainFrame *pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return false;
	}

	CProjectSqlite* pPrjSql = pMainFrm->m_pPrjSqlite;
	ASSERT(pPrjSql != NULL);
	if (NULL == pPrjSql)
	{
		return false;
	}


	try
	{
		const AnimationInfo* pSrcInfo = pPrjSql->GetAnimationByID(nAnimId, true);
		ASSERT(pSrcInfo != NULL);
		if (NULL == pSrcInfo)
		{
			return false;
		}

		const AnimationInfo* pDstInfo = pPrjSql->GetAnimationByID(nAnimId, false);
		if (NULL == pDstInfo)
		{	// 打开的s3db没有该编号的图，直接插入数据库
			pPrjSql->SetAnimationData(nAnimId, pSrcInfo, false);
		}
		else
		{	// 打开的s3db有该编号的图
			if (0 == wcscmp(pSrcInfo->szName.c_str(), pDstInfo->szName.c_str())
				&& pSrcInfo->vecPicture.size() == pDstInfo->vecPicture.size()
				&& pSrcInfo->vecPicIDList.size() == pDstInfo->vecPicIDList.size()
				&& pSrcInfo->vecPicIDList.at(0) == pDstInfo->vecPicIDList.at(0)
				)
			{
				// 文件名相同，则认为是同一个文件，跳过
			}
			else
			{
				// 有该编号的图，但文件名不同，则取目标s3db最大编号+1，插入
				int nUseId = pPrjSql->GetMaxLibAnimationNum(false) + 1;
				pPrjSql->SetAnimationData(nUseId, pSrcInfo, false);
			}
		}
	}
	catch (...)
	{
		return false;
	}


	return true;
}

void CCanvasDlg::InsertImageIntoOpenS3dbFromFile(PictureInfo picInf, int& nUseId)
{
	CMainFrame *pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	CProjectSqlite* pPrjSql = pMainFrm->m_pPrjSqlite;
	ASSERT(pPrjSql != NULL);
	if (NULL == pPrjSql)
	{
		return;
	}


	USES_CONVERSION;
	PictureInfo* pDstInfo = pPrjSql->GetPictureInfoByID(picInf.ID, true);
	if (NULL == pDstInfo)
	{	// 打开的s3db没有该编号的图，直接插入数据库
		nUseId = picInf.ID;
		pPrjSql->InsertPictureData(nUseId, T2A(picInf.szName), Project::Tools::WideCharToAnsi(picInf.etype_equip.c_str()).c_str(), picInf.pBlock, picInf.nSize, true);

		// 插入内存结构
		pPrjSql->AddIntoPictureList(picInf.ID, &picInf);
	}
	else
	{	// 打开的s3db有该编号的图
		int nCmp = wcscmp(picInf.szName, pDstInfo->szName);
		if (0 == nCmp && picInf.nSize == pDstInfo->nSize)
		{
			// 文件名和文件大小都相同，则认为是同一个文件，跳过
			nUseId = picInf.ID;
		}
		else
		{
			// 有该编号的图，但文件名或者文件大小不同，则取目标s3db最大编号+1，插入
			nUseId = pPrjSql->GetMaxLibImageNum(true) + 1;
			pPrjSql->InsertPictureData(nUseId, T2A(picInf.szName), Project::Tools::WideCharToAnsi(picInf.etype_equip.c_str()).c_str(), picInf.pBlock, picInf.nSize, true);

			// 插入内存结构
			pPrjSql->AddIntoPictureList(nUseId, &picInf);
		}
	}
}

bool CCanvasDlg::CreateLevelRuler(CPoint &point, const int nLayer,
	const CString strName, const int nMax, const int nMin, const int nMainScaleInterval, const int nMinorScaleInterval, const int nDecimalPlace,
	const vector<LabelInfo>& vecLabelInf, const vector<SectionInfo>& vecSectionInf)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawLevelRuler *pEqm = new CEqmDrawLevelRuler();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetName(strName);
	pEqm->SetMax(nMax);
	pEqm->SetMin(nMin);
	pEqm->SetMainScaleInterval(nMainScaleInterval);
	pEqm->SetMinorScaleInterval(nMinorScaleInterval);
	pEqm->SetDecimalPlace(nDecimalPlace);
	pEqm->SetLabelInfo(vecLabelInf);
	pEqm->SetSectionInfo(vecSectionInf);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\LevelRuler.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddLevelRuler(pEqm);
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateDataAnalyse(CPoint &point, const int nLayer, const vector<DaPointInfo>& vecDaPtInf, const CString strPtTime, const CString strPtCount, const CString strPtStatus)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawDataAnalyse *pEqm = new CEqmDrawDataAnalyse();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetDaPointInfo(vecDaPtInf);
	pEqm->SetPointTime(strPtTime);
	pEqm->SetPointCount(strPtCount);
	pEqm->SetPointStatus(strPtStatus);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataAnalyse.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddDataAnalyse(pEqm);
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm, E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateTemperDistri(CPoint &point, const int nLayer, const vector<TemperDistriInfo>& vecInf)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawTemperDistri *pEqm = new CEqmDrawTemperDistri();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetTemperDistri(vecInf);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\TemperDistri.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddTemperDistri(pEqm);
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm, E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateDataReport(CPoint &point, const int nLayer, const CString strPath)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawDataReport *pEqm = new CEqmDrawDataReport();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetPath(strPath);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataReport.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddDataReport(pEqm);
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm, E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}

bool CCanvasDlg::CreatePlaneTempeDistr(CPoint &point, const int nLayer, const CString strPtName)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawPlaneTempeDistr *pEqm = new CEqmDrawPlaneTempeDistr();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetPointName(strPtName);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\PlaneTempeDistr.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddPlaneTempeDistr(pEqm);
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm, E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateDottedLineFrm(CPoint &point, const int nLayer)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawDottedLineFrm *pEqm = new CEqmDrawDottedLineFrm();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);
	pEqm->SetWidth(100);
	pEqm->SetHeight(25);
	pMainFrame->m_DrawContext.pPage->AddDottedLineFrm(pEqm);
	SetMetaSelect(pEqm, pMainFrame);
	CommandManager::Instance()->CallCommand(pEqm, E_COMMAND_ADD);

	return true;
}

bool CCanvasDlg::CreateGeneralComponent(CPoint &point, const int nLayer, const CString strCfgVal)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawGeneralComponent *pEqm = new CEqmDrawGeneralComponent();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(nLayer);

	pEqm->SetConfigValue(strCfgVal);

	Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\GeneralComponent.png");
	if (pBitmap != NULL)
	{
		pEqm->SetDefaultBitmap(pBitmap);
		pEqm->SetWidth(pBitmap->GetWidth());
		pEqm->SetHeight(pBitmap->GetHeight());
		pMainFrame->m_DrawContext.pPage->AddGeneralComponent(pEqm);
		SetMetaSelect(pEqm, pMainFrame);
		CommandManager::Instance()->CallCommand(pEqm, E_COMMAND_ADD);
	}
	else
	{
		SAFE_DELETE(pEqm);
		return false;
	}

	return true;
}

bool CCanvasDlg::CreateStraightLine(CPoint &point, const int nPicID)
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawStraightLine *pEqm = new CEqmDrawStraightLine();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}
	if (pMainFrame->m_DrawContext.m_nDrawMode == EQMDRAW_UNKNOWN)
	{
		return false;
	}

	pEqm->SetPos(Gdiplus::PointF(point.x, point.y));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetLayer(4);
	pEqm->SetTerminalPointX((int)point.x+20);
	pEqm->SetTerminalPointY((int)point.y+20);

	pMainFrame->m_DrawContext.pPage->AddStraightLine(pEqm);
	SetMetaSelect(pEqm, pMainFrame);
	CommandManager::Instance()->CallCommand(pEqm,E_COMMAND_ADD);
	return true;
}

bool CCanvasDlg::CreateBendCtrl(const CPoint& point, const BendType& type, const int nId, const int nBendSize)
{
	CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (NULL == pMainFrame)
	{
		return false;
	}

	CEqmDrawDevice *pEqm = new CEqmDrawDevice();
	ASSERT(pEqm != NULL);
	if (NULL == pEqm)
	{
		return false;
	}

	if (EQMDRAW_UNKNOWN == pMainFrame->m_DrawContext.m_nDrawMode)
	{
		return false;
	}

	int nOffset = 0;
	if (nBendSize >= 6 && nBendSize <= 14)
	{
		nOffset = (14 - nBendSize);
	}
	pEqm->SetPos(Gdiplus::PointF(point.x + nOffset, point.y + nOffset));
	pEqm->SetEqmType(pMainFrame->m_DrawContext.m_nDrawMode);
	pEqm->SetPageID(pMainFrame->m_DrawContext.pPage->GetID());
	pEqm->SetID(++pMainFrame->m_MaxEqmtID);
	pEqm->SetPicID(nId);
	pEqm->SetImageMetaType(true);

	CString strImgPath;
	switch (type)
	{
	case bendThreeUp:
		strImgPath = L"\\Images\\bend_three_up.png";
		break;
	case bendThreeRight:
		strImgPath = L"\\Images\\bend_three_right.png";
		break;
	case bendThreeDown:
		strImgPath = L"\\Images\\bend_three_down.png";
		break;
	case bendThreeLeft:
		strImgPath = L"\\Images\\bend_three_left.png";
		break;
	case bendTwoLeftUp:
		strImgPath = L"\\Images\\bend_two_up_left.png";
		break;
	case bendTwoRightUp:
		strImgPath = L"\\Images\\bend_two_up_right.png";
		break;
	case bendTwoRightDown:
		strImgPath = L"\\Images\\bend_two_down_right.png";
		break;
	case bendTwoLeftDown:
		strImgPath = L"\\Images\\bend_two_down_left.png";
		break;
	default:
		break;
	}
	Image* pBitmap = Image::FromFile(m_strSysPath + strImgPath);
	if (pBitmap != NULL)
	{
		pEqm->SetBitmap(pBitmap);
		int nBendSizeShow = 2 * nBendSize + 13;
		pEqm->SetWidth(nBendSizeShow);
		pEqm->SetHeight(nBendSizeShow);
		pEqm->SetLayer(6);
		pEqm->SetEqmType(EQMDRAW_METAFILE);
		pMainFrame->m_DrawContext.pPage->AddDevice(pEqm); 
		pMainFrame->m_DrawContext.vecSelectEqmt.push_back(pEqm);
	}
	else
	{
		delete pEqm;
		pEqm = NULL;
		return false;
	}

	return true;
}

void CCanvasDlg::SetMetaSelect(CEqmDrawBase* pEqm, CMainFrame* pMainFrm)
{
	if (NULL == pEqm || NULL == pMainFrm)
	{
		return;
	}

	pEqm->SetSelected(true);
	pEqm->SetMainAlign(true);

	vector<CEqmDrawBase*>::const_iterator	iter;
	for (iter=pMainFrm->m_DrawContext.vecSelectEqmt.begin(); iter!=pMainFrm->m_DrawContext.vecSelectEqmt.end(); ++iter)
	{
		(*iter)->SetSelected(false);
		(*iter)->SetMainAlign(false);
	}
	pMainFrm->m_DrawContext.vecSelectEqmt.clear();
	pMainFrm->m_DrawContext.vecSelectEqmt.push_back(pEqm);
	pMainFrm->SetFocus();
}


void CCanvasDlg::OnRmenuCopy()
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	pMainFrm->MetaCopy();
}


void CCanvasDlg::OnRmenuPaste()
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	pMainFrm->MetaPaste();
	return;

	DrawContext& draw = pMainFrm->m_DrawContext;
	const int nCopyNum = draw.mVecSelectEqmtCopied.size();
	if (nCopyNum > 0)
	{
		draw.ClearSelect();
	}

	vector<CEqmDrawBase*>::const_iterator	iter;
	CEqmDrawBase* pDest = NULL;
	for (iter=draw.mVecSelectEqmtCopied.begin(); iter!=draw.mVecSelectEqmtCopied.end(); ++iter)
	{
		if (NULL == *iter)
		{
			continue;
		}

		pDest = (*iter)->Copy();
		if (NULL == pDest)
		{
			continue;
		}

		const int nID = pDest->GetPageID();
		pDest->SetID(++pMainFrm->m_MaxEqmtID);
		pDest->SetSelected(true);

		const int pageid = draw.pPage->GetID();
		pDest->SetPageID(pageid);

		Gdiplus::PointF pt(m_ptPaste.x, m_ptPaste.y);
		pDest->SetPos(pt);

		draw.pPage->AddBase(pDest);
		draw.vecSelectEqmt.push_back(pDest);
	}

	CommandManager::Instance()->CallCommand(draw.vecSelectEqmt, E_COMMAND_PASTE);
	pMainFrm->SaveDeviceProperty();
	draw.type = FLASHSCREEN;

	Invalidate(FALSE);
}

void CCanvasDlg::SetCursorRange(const CPoint& point)
{
	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	Gdiplus::PointF ptClick(point.x, point.y);
	const int nScaledWidth	= 1920;
	const int nScaledHeight	= 955;
	int	nMetaX				= 0;
	int	nMetaY				= 0;
	int nMetaWidth			= 0;
	int nMetaHeight			= 0;
	int	nMetaXX				= 0;
	int	nMetaYY				= 0;
	double fScale			= pMainFrm->m_DrawContext.m_fScreenScale;

	CRect rect;
	vector<CEqmDrawBase*>::const_iterator	iter;

	const int nSelectSize = pMainFrm->m_DrawContext.vecSelectEqmt.size();
	if (1 == nSelectSize)
	{
		iter = pMainFrm->m_DrawContext.vecSelectEqmt.begin();
		if (iter != pMainFrm->m_DrawContext.vecSelectEqmt.end())
		{
			nMetaX		= (*iter)->GetPos().X;
			nMetaY		= (*iter)->GetPos().Y;
			nMetaWidth	= (*iter)->GetWidth();
			nMetaHeight	= (*iter)->GetHeight();

			if (EQMDRAW_TEXT == (*iter)->GetEqmType())
			{
				nMetaY	-= nMetaHeight / 2;
			}
		}
	}
	else if (nSelectSize > 1)
	{
		iter = pMainFrm->m_DrawContext.vecSelectEqmt.begin();
		if (iter != pMainFrm->m_DrawContext.vecSelectEqmt.end())
		{
			nMetaX		= (*iter)->GetPos().X;
			nMetaY		= (*iter)->GetPos().Y;
			nMetaXX		= (*iter)->GetPos().X + (*iter)->GetWidth();
			nMetaYY		= (*iter)->GetPos().Y + (*iter)->GetHeight();
		}
		for (iter = pMainFrm->m_DrawContext.vecSelectEqmt.begin();
			iter != pMainFrm->m_DrawContext.vecSelectEqmt.end();
			++iter)
		{
			if (nMetaX > (*iter)->GetPos().X)
			{
				nMetaX = (*iter)->GetPos().X;
			}
			if (nMetaXX < (*iter)->GetPos().X + (*iter)->GetWidth())
			{
				nMetaXX = (*iter)->GetPos().X + (*iter)->GetWidth();
			}

			if (EQMDRAW_TEXT == (*iter)->GetEqmType())
			{
				if (nMetaY > (*iter)->GetPos().Y - (*iter)->GetHeight()/2)
				{
					nMetaY = (*iter)->GetPos().Y - (*iter)->GetHeight()/2;
				}
				if (nMetaYY < (*iter)->GetPos().Y + (*iter)->GetHeight()/2)
				{
					nMetaYY = (*iter)->GetPos().Y + (*iter)->GetHeight()/2;
				}
			}
			else
			{
				if (nMetaY > (*iter)->GetPos().Y)
				{
					nMetaY = (*iter)->GetPos().Y;
				}
				if (nMetaYY < (*iter)->GetPos().Y + (*iter)->GetHeight())
				{
					nMetaYY = (*iter)->GetPos().Y + (*iter)->GetHeight();
				}
			}
		}
		nMetaWidth = nMetaXX - nMetaX;
		nMetaHeight = nMetaYY - nMetaY;
	}

		/*
		iter = pMainFrm->m_DrawContext.vecSelectEqmt.begin();
		if (iter != pMainFrm->m_DrawContext.vecSelectEqmt.end())
		{
			nMetaX		= (*iter)->GetPos().X;
			nMetaY		= (*iter)->GetPos().Y;
			nMetaWidth	= (*iter)->GetWidth();
			nMetaHeight	= (*iter)->GetHeight();
		}
		else
		{
			return;
		}

		for (iter = pMainFrm->m_DrawContext.vecSelectEqmt.begin();
			iter != pMainFrm->m_DrawContext.vecSelectEqmt.end();
			++iter)
		{
			if (nMetaX > (*iter)->GetPos().X)
			{
				nMetaX = (*iter)->GetPos().X;
			}
			if (nMetaX + nMetaWidth < (*iter)->GetPos().X + (*iter)->GetWidth())
			{
				nMetaWidth = (*iter)->GetPos().X + (*iter)->GetWidth() - nMetaX;
			}

			if (EQMDRAW_TEXT == (*iter)->GetEqmType())
			{
				if (nMetaY > (*iter)->GetPos().Y - (*iter)->GetHeight()/2)
				{
					nMetaY = (*iter)->GetPos().Y - (*iter)->GetHeight()/2;
				}
				if (nMetaY + nMetaHeight < (*iter)->GetPos().Y + (*iter)->GetHeight()/2)
				{
					nMetaHeight = (*iter)->GetPos().Y + (*iter)->GetHeight()/2 - nMetaY;
				}
			}
			else
			{
				if (nMetaY > (*iter)->GetPos().Y)
				{
					nMetaY = (*iter)->GetPos().Y;
				}
				if (nMetaY + nMetaHeight < (*iter)->GetPos().Y + (*iter)->GetHeight())
				{
					nMetaHeight = (*iter)->GetPos().Y + (*iter)->GetHeight() - nMetaY;
				}
			}
		}
	}
	*/
	rect.left	= point.x - nMetaX;
	rect.top	= point.y - nMetaY;
	rect.right	= nScaledWidth - (nMetaWidth - rect.left);
	rect.bottom	= nScaledHeight - (nMetaHeight - rect.top);

	rect.left	*= fScale;
	rect.top	*= fScale;
	rect.right	*= fScale;
	rect.bottom	*= fScale;

	if (rect.left >= rect.right || rect.top >= rect.bottom)
	{	// error
		return;
	}

	ClientToScreen(&rect);
	ClipCursor(&rect);		//限制鼠标移动范围在矩形框中
}

void CCanvasDlg::SyncPlaneTemperDistri(const CString strPtList)
{
	const CString strFlag(_T(","));
	vector<CString>	vecPoint;
	vector<CString>::const_iterator	iterBindPt;
	CString	strSrc(strPtList + strFlag);
	CString	strTemp;
	int nPos = -1;


	while (true)
	{
		nPos = strSrc.Find(strFlag);
		if (-1 == nPos)
		{	// 未发现分隔符，退出
			break;
		}
		if (0 == nPos)
		{	// 首字符为分隔符
			strSrc = strSrc.Mid(1);
			continue;
		}

		strTemp = strSrc.Left(nPos);
		strSrc = strSrc.Mid(nPos + 1);

		vecPoint.push_back(strTemp);
	}

	if (vecPoint.empty())
	{
		return;
	}


	// 同步操作
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	if (NULL == pMainFrame)
	{
		return;
	}
	if (NULL == pMainFrame->m_DrawContext.pPage)
	{
		return;
	}

	CString	strBindPtName;
	list<CEqmDrawText*>::iterator iterEqmText;
	for (iterEqmText = pMainFrame->m_DrawContext.pPage->GetPageTextList().begin();
		iterEqmText != pMainFrame->m_DrawContext.pPage->GetPageTextList().end();
		iterEqmText++)
	{
		// 在当前页中，遍历文本图元，取出绑定点的点名（非时间变量名）
		strBindPtName.Format(L"%s", (*iterEqmText)->GetPhysicalID().c_str());

		// 在平面温度分布图元绑定点列表中，遍历
		for (iterBindPt=vecPoint.begin(); iterBindPt!=vecPoint.end(); iterBindPt++)
		{
			if (*iterBindPt == strBindPtName)
			{
				(*iterEqmText)->SetTextAttribute(TEXT_ATTR_TEMPERA);
				(*iterEqmText)->SetNeedSave(true);
				break;
			}
		}
	}
}

CPoint CCanvasDlg::GetClosestGridPosition(const CPoint point) const
{
	CPoint ptRet;
	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return ptRet;
	}


	int nWidth = 0;
	int nHeight = 0;
	int nOffsetX = 0;
	int nOffsetY = 0;
	int nScreenWidth = static_cast<int>(pMainFrm->m_DrawContext.GetScreenWidth());
	int nScreenHeight = static_cast<int>(pMainFrm->m_DrawContext.GetScreenHeight());
	if (FULLSCREEN == pMainFrm->m_DrawContext.pPage->GetPageType())
	{
		nWidth = nScreenWidth;
		nHeight = nScreenHeight;
	}
	else
	{
		nWidth = pMainFrm->m_DrawContext.pPage->GetPageWidth();
		nHeight = pMainFrm->m_DrawContext.pPage->GetPageHeight();
		nOffsetX = (nScreenWidth - nWidth) / 2;
		nOffsetY = (nScreenHeight - nHeight) / 2;
	}

	int nOffsetWidth = nWidth + nOffsetX;
	for (int i=CELLINTERVAL + nOffsetX; i<nOffsetWidth; i+=CELLINTERVAL)
	{
		if (i > point.x)
		{
			if (i - point.x < point.x - (i - CELLINTERVAL))
			{
				ptRet.x = i;
			}
			else
			{
				ptRet.x = i - CELLINTERVAL;
			}
			break;
		}
	}
	int nOffsetHeight = nHeight + nOffsetY;
	for (int j=CELLINTERVAL + nOffsetY; j<nOffsetHeight; j+=CELLINTERVAL)
	{
		if (j > point.y)
		{
			if (j - point.y < point.y - (j - CELLINTERVAL))
			{
				ptRet.y = j;
			} 
			else
			{
				ptRet.y = j - CELLINTERVAL;
			}
			break;
		}
	}


	return ptRet;
}


void CCanvasDlg::OnAddBindpointToVpointTable()
{
	// TODO: Add your command handler code here
	CPoint point = m_rightclicksavepoint;
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	CPoint actpoint = point;
	actpoint.x /= pMainFrame->m_DrawContext.m_fScreenScale;
	actpoint.y /= pMainFrame->m_DrawContext.m_fScreenScale;
	pMainFrame->m_DrawContext.actPt.X = actpoint.x;
	pMainFrame->m_DrawContext.actPt.Y = actpoint.y;
	CEqmDrawBase *pEqmDrawBase = pMainFrame->m_DrawProxy.PickObject(pMainFrame->m_DrawContext.pPage,(double)pMainFrame->m_DrawContext.actPt.X,(double)pMainFrame->m_DrawContext.actPt.Y);
	if ( NULL != pEqmDrawBase )
	{
		if ( pEqmDrawBase->GetEqmType()==EQMDRAW_TEXT )
		{
			CEqmDrawText* pText = dynamic_cast<CEqmDrawText*>(pEqmDrawBase);
			ASSERT(pText != NULL);
			if (pText == NULL)
			{
				return;
			}

			CString strDateTimePoint = pText->GetDateTimePoint();

			CVPointSimpleSettingDlg dlg;
			dlg.m_strName = strDateTimePoint;
			if (IDOK == dlg.DoModal())
			{
				CString	strVpRw(dlg.GetVPointRwAttri());
				std::string strPP, strDesc;
				Project::Tools::WideCharToUtf8(strDateTimePoint.GetString(), strPP);

				Project::Tools::WideCharToUtf8(dlg.m_strDescription.GetString(), strDesc);



				if(!pMainFrame->m_pOnlinePrjSqlite->GetPointExist(strPP.data()))
				{
					int nID = pMainFrame->m_pOnlinePrjSqlite->GetMaxIDInPointTable()+1;
					pMainFrame->m_pOnlinePrjSqlite->InsertRecordToOPCPoint(nID,0, strPP.c_str(),"vpoint",0, strDesc.c_str(),"",0,0,0,0,"","","","","","","","","","","2","","","","",0,0,0);
				}
			}
		}
	}

	
}

void CCanvasDlg::OnRmenuCanclDraw()
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	if(pMainFrame->m_DrawContext.pNewEqmt !=  NULL)
	{
		delete pMainFrame->m_DrawContext.pNewEqmt;
		pMainFrame->m_DrawContext.pNewEqmt = NULL;
		pMainFrame->m_DrawContext.bIsCreatePolygon = false;
		pMainFrame->m_DrawContext.bIsCreatePipe = false;
	}
}
