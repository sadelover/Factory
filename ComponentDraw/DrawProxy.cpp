#include "StdAfx.h"
#include "DrawProxy.h"
#include <assert.h>
#include "EqmDrawDefines.h"
#include "CEqmDrawPage.h"
#include "EqmDrawProject.h"

#include "EqmDrawDevice.h"
#include "EqmDrawPipe.h"
#include "EqmDrawText.h"
#include "EqmDrawGraphic.h"
#include "EqmDrawBend.h"
#include "EqmDrawDashBoard.h"
#include "EqmDrawLineGraphic.h"
#include "EqmDrawBarGraphic.h"
#include "EqmDrawPieGraphic.h"
#include "EqmDrawButton.h"
#include "EqmDrawTimeSelect.h"
#include "EqmDrawTimeBar.h"
#include "EqmDrawProgressBar.h"
#include "EqmDrawDiagnoseGraphic.h"
#include "EqmDrawPolygon.h"
#include "EqmDrawAreaChange.h"
#include "EqmDrawShellExecute.h"
#include "EqmDrawEmbededPage.h"
#include "EqmDrawLiquidLevel.h"
#include "EqmDrawButtonPlus.h"
#include "EqmDrawScrawl.h"
#include "EqmDrawHistoryTrend.h"
#include "EqmDrawEnergySaveRoi.h"
#include "EqmDrawMapNavigate.h"
#include "EqmDrawNavigateWindow.h"
#include "EqmDrawLevelRuler.h"
#include "EqmDrawDataAnalyse.h"
#include "EqmDrawTemperDistri.h"
#include "EqmDrawDataReport.h"
#include "EqmDrawPlaneTempeDistr.h"
#include "EqmDrawDottedLineFrm.h"
#include "EqmDrawGeneralComponent.h"
#include "Tools/CustomTools/CustomTools.h"
#include "EqmDrawRectangle.h"
#include "EqmDrawStraightLine.h"
#include <algorithm>

#pragma warning(disable:4244)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define			VER_MARGIN		10
#define			HOR_MARGIN		10

bool CompareX(const CEqmDrawBase* p1, const CEqmDrawBase* p2)
{
	return p1->GetPos().X < p2->GetPos().X;
}

bool CompareY(const CEqmDrawBase* p1, const CEqmDrawBase* p2)
{
	return p1->GetPos().Y < p2->GetPos().Y;
}

//////////////////////////////////////////////////////////////////////////

DrawProxy::DrawProxy()
{
	m_dScaleFactor = 1.f;
	for (int i=0;i<E_LAYER_MAX;++i)
	{
		m_bShow[i] = true;
	}
}

DrawProxy::~DrawProxy()
{

}

/** 
*	在此进行的设置会影响静态画图对象和动态画图对象
*/
void DrawProxy::prepareDraw()
{
	//反锯齿
	m_pGC->SetSmoothingMode(SmoothingModeHighQuality);
}

void DrawProxy::drawStaticScene()
{
	/* 在此添加静态场景的绘图代码,静态场景包括设置背景颜色
	等不需要在每一个绘图周期都进行重绘的部分。
	
	该函数中的绘图代码只在绘图区大小发生改变时才会被调用，
	将静态场景的绘制放在该函数内可以避免不必要的重绘，
	从而可以提升绘图性能。要改变绘图区的大小可以调用基类的setRect()函数，
	如在窗口大小发生改变时，可以响应WM_SIZE消息并在OnSize()响应函数中调用双缓冲绘图类的
	setRect()函数更新绘图区大小。
	
	在绘图过程中既可以使用gdi中的设备dc绘图也可以使用gdiplus中的m_pGC对象进行绘图，
	在CDBDraw基类中提供了m_pDC和m_pGC成员分别供gdi和gdiplus绘图使用。
	
	在采用m_pDC或者m_pGC绘图时，绘制的图形都是绘制在后备缓冲区中
	然后再从后备缓冲区拷贝前端缓冲区进行显示，从而实现双缓冲。
	
	当然，双缓冲的实现过程都已经完成封装在CDBDraw基类中，
	派生类中看不到任何双缓冲的痕迹，在使用m_pDC或者m_pGC绘图时，
	就感觉时直接在窗口dc或者绘图对象中绘图一样。
	这种封装不仅不损失绘图的性能同时能够在多方面提升绘图的性能。*/
	
	// 窗口大小改变时更新缩放因子
	UpdateScaleFactor();
	Color colorBk;
	colorBk.SetFromCOLORREF(m_backGroudColor);
	m_pGC->Clear(colorBk);
	//DrawGrid();
}

void DrawProxy::drawDynamicScene()
{
	if ( NULL == m_pDrawContext || NULL == m_pDrawContext->pPage )
	{
		return;
	}
	switch ( m_pDrawContext->type )
	{
	case REDRAW:
	case FLASHSCREEN:
		ReflashScreen();
		break;
	default:
		break;
	}		
}

void DrawProxy::UpdateScaleFactor()
{
	//计算放大比例
	RectF ViewRect = GetViewRect();
	RectF SceneRect = GetSceneRect();
	REAL dWidthFactor = (REAL)ViewRect.Width / SceneRect.Width;
	REAL dHeightFactor = (REAL)ViewRect.Height / SceneRect.Height;

	//以长宽缩放因子中小者为缩放因子,保证图形不出界且不变形
	m_dScaleFactor = (dWidthFactor < dHeightFactor) ? dWidthFactor : dHeightFactor;
}

/** 
*	\return 返回窗口视区矩形
*/
RectF DrawProxy::GetViewRect()
{
	REAL ViewWidth = m_rect.Width() - 2.f * HOR_MARGIN;
	REAL ViewHeight = m_rect.Height() - 2.f * VER_MARGIN;

	return RectF(HOR_MARGIN, VER_MARGIN, ViewWidth, ViewHeight);
}

/** 
*	\return 返回整个场景的实际大小
*/
RectF DrawProxy::GetSceneRect()
{
	RectF rect;
	rect.X = (REAL)(-GOAL_WIDTH - FIELD_WIDTH / 2.);
	rect.Y = (REAL)(FIELD_HEIGHT / 2.);
	rect.Width = (REAL)(2. * GOAL_WIDTH + FIELD_WIDTH);
	rect.Height = (REAL)(FIELD_HEIGHT);	
	return rect;
}


BOOL DrawProxy::DoMoveAct(const bool bDown, UINT nFlags, DrawContext* pDataSt )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;
	if ( pDrawElement->m_bIsChangeSize )
	{
		return FALSE;
	}

	CEqmDrawBase *pe = PickObject(pDrawElement->pPage, pDrawElement->actPt.X, pDrawElement->actPt.Y);
	if(pDrawElement->m_nDrawMode == EQMDRAW_STRAIGHTPIPE || 
	   pDrawElement->m_nDrawMode == EQMDRAW_FOLDPIPE ||
	   pDrawElement->m_nDrawMode == GraphicType_LINE ||
	   pDrawElement->m_nDrawMode == GraphicType_RECTANGLE||
       pDrawElement->m_nDrawMode == GraphicType_ELLIPSE||
	   pDrawElement->m_nDrawMode == GraphicType_POLYGON||
	   pDrawElement->m_nDrawMode == EQMDRAW_POLYGON) //画水管模式
	{
		if(pDrawElement->bIsCreatePipe 
            || pDrawElement->m_bIsCreate2DGraph
			|| pDrawElement->bIsCreatePolygon )
		{
			pDrawElement->curPoint.X = pDrawElement->actPt.X;
			pDrawElement->curPoint.Y = pDrawElement->actPt.Y;
			return TRUE;
		}

	}
	else if(pDrawElement->m_bDragging )//移动模式
	{
        //dragging object
		if (nFlags & MK_CONTROL)
		{	// forbid move when control is clicked down
			return FALSE;
		}

		for ( int nIndex = 0; nIndex < (int)pDrawElement->vecSelectEqmt.size(); nIndex++)
		{
			if ( pDrawElement->vecSelectEqmt[nIndex]!=NULL && bDown)
			{
				if (!pDrawElement->vecSelectEqmt[nIndex]->DoMoveEqmt(pDrawElement->PrePt, pDrawElement->actPt))
				{
					//AfxMessageBox(L"有图元超出绘图边界!");
					return FALSE;
				}
			}
		}
		return TRUE;
	}
	else
	{
		pDrawElement->m_bDragging = FALSE;//是否在拖拽中
		return FALSE;
	}
	return  FALSE;
}


void DrawProxy::ReflashScreen()
{
	EQMDRAWTYPE drawtype = m_pDrawContext->m_nDrawMode;
	SHOWTYPE showtype = m_pDrawContext->m_showType;
	if ( EQMDRAW_MOVE != drawtype && showtype == SHOWTYPE_RUN)
	{
        m_pDrawContext->ClearSelect();
	}
	CEqmDrawPage* pPage = m_pDrawContext->pPage;
	if (pPage != NULL && m_pGC != NULL)
	{
		const int nWidth = pPage->GetPageWidth();
		const int nHeight = pPage->GetPageHeight();

		const int nXStart = pPage->GetPagexPosition();
		const int nYStart = pPage->GetPageyPosition();

		DWORD color1 = pPage->GetStartColor();
		DWORD color2 = pPage->GetEndColor();
		if (0 == color2)
		{
			color2 = color1;
		}
		int r1 = GetRValue(color1);
		int g1 = GetGValue(color1);
		int b1 = GetBValue(color1);
		int r2 = GetRValue(color2);
		int g2 = GetGValue(color2);
		int b2 = GetBValue(color2);
		int iRotation = 0;

		//Gdiplus::Color color_gdiplus;
		//color_gdiplus.SetFromCOLORREF(color1);
		//Gdiplus::SolidBrush brush(color_gdiplus);
		Gdiplus::LinearGradientBrush linGrBrush(Gdiplus::Rect(nXStart,nYStart,nWidth,nHeight),
			Gdiplus::Color(255, r1, g1, b1),
			Gdiplus::Color(255, r2, g2, b2),
			(Gdiplus::REAL)(90 - iRotation));
		m_pGC->FillRectangle(&linGrBrush,nXStart,nYStart,nWidth,nHeight);

		if (0 != pPage->GetBKImageID())
		{
			Image* pBkImg = pPage->GetBkImage();
			if (pBkImg != NULL)
			{
				m_pGC->DrawImage(pBkImg, nXStart, nYStart, nWidth, nHeight);
			}
		}
	}

	// show grids
	DrawGrid();

    //layer sort
    for(UINT layer = E_LAYER_1; layer < E_LAYER_MAX; layer++)
	{
		if (!m_bShow[layer])
		{
			continue;
		}
        DrawPipe(layer);
        DrawCreatePipe(layer);
        Draw2DGraphic(layer);
		DrawCreate2DGraph(layer);
        DrawDevices(layer);
        DrawText(layer);
		DrawBend(layer);
		DrawDashBoard(layer);
		DrawLineGraphic(layer);
		DrawBarGraphic(layer);
		DrawPieGraphic(layer);
		DrawButton(layer);
		DrawTimeSelect(layer);
		DrawTimeBar(layer);
		DrawProgressBar(layer);
		DrawSelected(layer);
		DrawDiagnoseGraphic(layer);
		DrawPolygon(layer);
		DrawCreatePolygon(layer);
		DrawAreaChange(layer);
		DrawShellExecute(layer);
		DrawEmbededPage(layer);
		DrawLiquidLevel(layer);
		DrawCheckButtonPlus(layer);
		DrawScrawl(layer);
		DrawHistoryTrend(layer);
		DrawEnergySaveRoi(layer);
		DrawMapNavigate(layer);
		DrawNavigateWindow(layer);
		DrawLevelRuler(layer);
		DrawDataAnalyse(layer);
		DrawTemperDistri(layer);
		DrawDataReport(layer);
		DrawPlaneTempeDistr(layer);
		DrawDottedLineFrm(layer);
		DrawGeneralComponent(layer);
		DrawRectangle(layer);
		DrawStraightLine(layer);
   }
	DrawMouseSelectBox(*m_pDrawContext);
}

void DrawProxy::DrawPipe(UINT layer )
{
    CEqmDrawPipe* pPipe = NULL;
    list<CEqmDrawPipe*>& pipeList = m_pDrawContext->pPage->GetPagePipeList();
    for (list<CEqmDrawPipe*>::iterator it = pipeList.begin();
        it != pipeList.end(); it++)
    {
        pPipe = *it;
        if(!pPipe)
            continue;
        if ( pPipe->IsSelected() || pPipe->GetLayer() != layer || pPipe->GetDeleteFlag() == true)
		{
            continue;
		}
		pPipe->Draw(m_pGC);
    }
}

//todo: layer
void DrawProxy::DrawCreate2DGraph(UINT layer )
{
    if ( m_pDrawContext->m_bIsCreate2DGraph )//绘制2D图形时动态显示绘制过程
    {
		Gdiplus::Color color;
		color.SetFromCOLORREF(m_pDrawContext->GetLineColor());
        Gdiplus::Pen pen(color, m_pDrawContext->GetLineWidth());
        pen.SetDashStyle(Gdiplus::DashStyleSolid);
        pen.SetAlignment(PenAlignmentInset);
        switch ( m_pDrawContext->m_nDrawMode )
        {
        case GraphicType_LINE:
            {
				CEqmDrawGraphic *pLine = dynamic_cast<CEqmDrawGraphic*>(m_pDrawContext->pNewEqmt);
				if ( pLine ){
					pLine->Draw(m_pGC);
				}
				Gdiplus::PointF pt = pLine->GetPointList()[pLine->GetPointList().size()-1];
                if ( abs(pt.X - m_pDrawContext->curPoint.X) >= abs(pt.Y - m_pDrawContext->curPoint.Y) )
                    m_pGC->DrawLine(&pen, pt.X, pt.Y, m_pDrawContext->curPoint.X, pt.Y);
                else
                    m_pGC->DrawLine(&pen, pt.X, pt.Y, pt.X, m_pDrawContext->curPoint.Y);

            }
            break;
        case GraphicType_RECTANGLE:
			{
                int x = 0, y = 0, width = 0, height = 0;
                x = m_pDrawContext->prePoint.X >= m_pDrawContext->curPoint.X ?m_pDrawContext->curPoint.X:m_pDrawContext->prePoint.X;
                y = m_pDrawContext->prePoint.Y >= m_pDrawContext->curPoint.Y ?m_pDrawContext->curPoint.Y:m_pDrawContext->prePoint.Y;
                width  = abs(m_pDrawContext->prePoint.X - m_pDrawContext->curPoint.X);
                height = abs(m_pDrawContext->prePoint.Y - m_pDrawContext->curPoint.Y);
                m_pGC->DrawRectangle(&pen, x, y, width, height);
            }
            break;
		case GraphicType_ELLIPSE:
			{
				int x = 0, y = 0, width = 0, height = 0;
				x = m_pDrawContext->prePoint.X >= m_pDrawContext->curPoint.X ?m_pDrawContext->curPoint.X:m_pDrawContext->prePoint.X;
				y = m_pDrawContext->prePoint.Y >= m_pDrawContext->curPoint.Y ?m_pDrawContext->curPoint.Y:m_pDrawContext->prePoint.Y;
				width  = abs(m_pDrawContext->prePoint.X - m_pDrawContext->curPoint.X);
				height = abs(m_pDrawContext->prePoint.Y - m_pDrawContext->curPoint.Y);
				m_pGC->DrawEllipse(&pen, x, y, width, height);
			}
			break;
		case GraphicType_POLYGON:
			{
				//@todo
			}
			break;
        default:
            break;
        }
    }
}

void DrawProxy::DrawCreatePipe(UINT layer )
{
    if ( m_pDrawContext->bIsCreatePipe )
    {
		int innercolor = m_pDrawContext->GetDefaultPipeColor().innercolor;
		Gdiplus::Pen PenLineDown(Gdiplus::Color(GetRValue(innercolor),
			GetGValue(innercolor),
			GetBValue(innercolor)),
			m_pDrawContext->GetDefaultPipeWidth() - 4);

		int outercolor = m_pDrawContext->GetDefaultPipeColor().outercolor;
		Gdiplus::Pen PenLineDownSide(Gdiplus::Color(GetRValue(outercolor), 
			GetGValue(outercolor),
			GetBValue(outercolor)),
			m_pDrawContext->GetDefaultPipeWidth());

		PenLineDownSide.SetAlignment(PenAlignmentInset);

        CEqmDrawPipe * pipe = dynamic_cast<CEqmDrawPipe*>(m_pDrawContext->pNewEqmt);
        if (pipe 
            && pipe->GetLayer() == layer){
			
            const vector<Gdiplus::PointF>& vecPointList = pipe->GetPointList();
            const UINT pointNum = (UINT)vecPointList.size();
                 
            if ( pointNum < 3 )
			{
                pipe->Draw(m_pGC);
                const Gdiplus::PointF& ptLast = vecPointList[pointNum-1];
				if(GetAsyncKeyState(VK_SHIFT)&0x8000)
				{
					Gdiplus::PointF pt;
					const Gdiplus::PointF& ptLast = vecPointList[pointNum-1];
					pt.X = abs(ptLast.X - m_pDrawContext->actPt.X) > abs(ptLast.Y - m_pDrawContext->actPt.Y) ? m_pDrawContext->actPt.X : ptLast.X;
					pt.Y = abs(ptLast.X-m_pDrawContext->actPt.X) > abs(ptLast.Y - m_pDrawContext->actPt.Y) ? ptLast.Y : m_pDrawContext->actPt.Y;
					m_pGC->DrawLine(&PenLineDownSide, ptLast, pt);
					m_pGC->DrawLine(&PenLineDown, ptLast, pt);
				}
				else
				{
					m_pGC->DrawLine(&PenLineDownSide, ptLast, m_pDrawContext->actPt);
					m_pGC->DrawLine(&PenLineDown, ptLast, m_pDrawContext->actPt);
				}               
            }
            else
            {
                const unsigned int ptNum = (UINT)pipe->GetPointList().size();
                Gdiplus::PointF * arrPt = new Gdiplus::PointF[ptNum+1];
                for ( unsigned int uiIndex = 0; uiIndex < ptNum; uiIndex++ )
                {
                    arrPt[uiIndex] = pipe->GetPointList()[uiIndex];
                }
                if ( arrPt[ptNum-1].X == arrPt[ptNum-2].X )//垂直
                {
                    if ( abs(m_pDrawContext->actPt.X - arrPt[ptNum-1].X) > abs(m_pDrawContext->actPt.Y - arrPt[ptNum-1].Y) ) //添加横线
                    {
                        arrPt[ptNum-1].Y = m_pDrawContext->actPt.Y;
                        arrPt[ptNum] = m_pDrawContext->actPt;
                        m_pGC->DrawLines(&PenLineDownSide, arrPt, ptNum+1);
                        m_pGC->DrawLines(&PenLineDown, arrPt, ptNum+1);
                    }
                    else//添加竖线
                    {
                        arrPt[ptNum-1].X = m_pDrawContext->actPt.X;
                        arrPt[ptNum-1].Y = m_pDrawContext->actPt.Y;
                        arrPt[ptNum-2].X = m_pDrawContext->actPt.X;
                        m_pGC->DrawLines(&PenLineDownSide, arrPt, ptNum);
                        m_pGC->DrawLines(&PenLineDown, arrPt, ptNum);
                    }
                }
                else//水平
                {
                    if ( abs(m_pDrawContext->actPt.X - arrPt[ptNum-1].X) > abs(m_pDrawContext->actPt.Y - arrPt[ptNum-1].Y) ) //添加横线
                    {
                        arrPt[ptNum-1].Y = m_pDrawContext->actPt.Y;
                        arrPt[ptNum-1].X = m_pDrawContext->actPt.X;
                        arrPt[ptNum-2].Y = m_pDrawContext->actPt.Y;
                        m_pGC->DrawLines(&PenLineDownSide, arrPt, ptNum);
                        m_pGC->DrawLines(&PenLineDown, arrPt, ptNum);
                    }
                    else//添加竖线
                    {
                        arrPt[ptNum-1].X = m_pDrawContext->actPt.X;
                        arrPt[ptNum] = m_pDrawContext->actPt;
                        m_pGC->DrawLines(&PenLineDownSide, arrPt, ptNum+1);
                        m_pGC->DrawLines(&PenLineDown, arrPt, ptNum+1);
                    }
                }
                delete []arrPt;	
            }
        }
    }
}

void DrawProxy::Draw2DGraphic(UINT layer )
{
    CEqmDrawGraphic* pDevice = NULL;
    list<CEqmDrawGraphic*>& graList = m_pDrawContext->pPage->Get2DGraphicList();
    for ( list<CEqmDrawGraphic*>::iterator it = graList.begin();
        it != graList.end(); it++)
    {
        pDevice = *it;
        if(!pDevice){
            ASSERT(false);
            continue;
        }
        if(pDevice->IsSelected()
            || pDevice->GetLayer() != layer 
			|| pDevice->GetDeleteFlag() == true)
            continue;

        if ( GraphicType_RECTANGLE == pDevice->GetEqmType() && 
            INVALID_FORWARDPAGEID != pDevice->GetForwardPageID() &&   //如果矩形框是导航页面的话在运行模式未选中的情况下不显示
            !pDevice->GetShow() &&
            SHOWTYPE_RUN == m_pDrawContext->m_showType)
            continue;
        pDevice->Draw(m_pGC);
    }
}

void DrawProxy::DrawDevices(UINT layer )
{
    CEqmDrawDevice* pDevice = NULL;
    list<CEqmDrawDevice*>& deviceList = m_pDrawContext->pPage->GetDrawDeviceList();
    for (list<CEqmDrawDevice*>::iterator it = deviceList.begin();
        it != deviceList.end(); it++)
    {
        pDevice = *it;
        if(!pDevice)
            continue;
		if ( pDevice->IsSelected() || !pDevice->GetShow()|| pDevice->GetLayer() != layer || pDevice->GetDeleteFlag() == true)
		{
			continue;
		}
        pDevice->Draw(m_pGC, NULL);
	}
}

void DrawProxy::DrawSelected(UINT layer )
{
    CEqmDrawBase* pBase =NULL;
    CEqmDrawDevice *pDevice = NULL;
    for ( unsigned int nIndex = 0; nIndex < m_pDrawContext->vecSelectEqmt.size(); nIndex++ )
    {
        pBase = m_pDrawContext->vecSelectEqmt[nIndex];
        pDevice  = dynamic_cast<CEqmDrawDevice*>(pBase);
        Image *pBitmap = NULL;
        if(!pBase || pBase->GetLayer() != layer || pBase->GetDeleteFlag() == true)
		{
			continue;
		}
		if ( pDevice )
        {
            pDevice->SetNextFrame();
        }
        pBase->Draw(m_pGC, pBitmap);
        pBase->DrawSelectEdge(m_pGC);    
    }
}

void DrawProxy::DrawText(UINT layer)
{
    CEqmDrawText* pText = NULL;
    list<CEqmDrawText*>& textList = m_pDrawContext->pPage->GetPageTextList();
    for (list<CEqmDrawText*>::iterator it = textList.begin();it != textList.end(); it++)
    {
        pText = *it;
        if(!pText)
		{
            continue;
		}
        if ( pText->IsSelected() || pText->GetLayer() != layer || pText->GetDeleteFlag() == true)
		{
            continue;
		}
        pText->Draw(m_pGC);
	}
}

//when mouse dragging ,draw mouse select box
void DrawProxy::DrawMouseSelectBox(DrawContext& rDrawElement)
{

	if(EQMDRAW_MOVE != rDrawElement.m_nDrawMode
		|| !rDrawElement.m_ifSelectBox)    
		return;

	Gdiplus::Pen pen(Gdiplus::Color::Black, 1);
	pen.SetDashStyle(DashStyleDash);
	pen.SetAlignment(PenAlignmentInset);
	Gdiplus::RectF rectF = GetRectFrom2Points(rDrawElement.PrePt.X, rDrawElement.PrePt.Y, rDrawElement.actPt.X, rDrawElement.actPt.Y);

    CString cstr;
    cstr.Format(_T("DrawMouseSelectBox p0=%d,%d, p1=%d,%d"),
        (int)rDrawElement.PrePt.X, (int)rDrawElement.PrePt.Y,
        (int)rDrawElement.actPt.X, (int)rDrawElement.actPt.Y
        );
	m_pGC->DrawRectangle(&pen, rectF.X, rectF.Y, rectF.Width, rectF.Height);

}

void DrawProxy::LeftAlign( vector<CEqmDrawBase *> & vecEqmt )
{
	int x = 0;
	for ( int nIndex = 0; nIndex < (int)vecEqmt.size(); nIndex++ )
	{
		if ( vecEqmt[nIndex]->IsMainAlign() )
		{
			x = vecEqmt[nIndex]->GetPos().X;
			break;
		}	
	}
	for ( int i = 0; i < (int)vecEqmt.size(); i++ )
	{
		Gdiplus::PointF pt;
		pt.X = x;
		pt.Y = vecEqmt[i]->GetPos().Y;
		vecEqmt[i]->SetPos(pt);
	}
}

void DrawProxy::RightAlign( vector<CEqmDrawBase *> & vecEqmt )
{
	int x = 0;
	int	w = 0;
	for ( int nIndex = 0; nIndex < (int)vecEqmt.size(); nIndex++ )
	{
		if ( vecEqmt[nIndex]->IsMainAlign() )
		{
			x = vecEqmt[nIndex]->GetPos().X;
			w = vecEqmt[nIndex]->GetWidth();
			break;
		}		
	}
	for ( int i = 0; i < (int)vecEqmt.size(); i++ )
	{
		Gdiplus::PointF pt;
		pt.X = x + w - vecEqmt[i]->GetWidth();
		pt.Y = vecEqmt[i]->GetPos().Y;
		vecEqmt[i]->SetPos(pt);
	}
}

void DrawProxy::TopAlign( vector<CEqmDrawBase *> & vecEqmt )
{
	int y = 0;
	int h = 0;
	for ( int nIndex = 0; nIndex < (int)vecEqmt.size(); nIndex++ )
	{
		if ( vecEqmt[nIndex]->IsMainAlign() )
		{
			y = vecEqmt[nIndex]->GetPos().Y;
			h = vecEqmt[nIndex]->GetHeight();
			break;
		}		
	}
	for ( int i = 0; i < (int)vecEqmt.size(); i++ )
	{
		Gdiplus::PointF pt;
		if (EQMDRAW_TEXT == vecEqmt[i]->GetEqmType())
		{
			pt.Y = y - h/2 + vecEqmt[i]->GetHeight()/2;
		}
		else
		{
			pt.Y = y;
		}
		pt.X = vecEqmt[i]->GetPos().X;
		vecEqmt[i]->SetPos(pt);
	}
}

void DrawProxy::BottomAlign( vector<CEqmDrawBase *> & vecEqmt )
{
	int y = 0;
	int h = 0;
	for ( int nIndex = 0; nIndex < (int)vecEqmt.size(); nIndex++ )
	{
		if ( vecEqmt[nIndex]->IsMainAlign() )
		{
			y = vecEqmt[nIndex]->GetPos().Y;
			h = vecEqmt[nIndex]->GetHeight();
			break;
		}	
	}
	for ( int i = 0; i < (int)vecEqmt.size(); i++ )
	{
		Gdiplus::PointF pt;
		if (EQMDRAW_TEXT == vecEqmt[i]->GetEqmType())
		{
			pt.Y = y  + h/2 - vecEqmt[i]->GetHeight()/2;
		}
		else
		{
			pt.Y = y + h - vecEqmt[i]->GetHeight();
		}
		pt.X = vecEqmt[i]->GetPos().X;
		vecEqmt[i]->SetPos(pt);
	}
}

void DrawProxy::CrossAlign( vector<CEqmDrawBase *> & vecEqmt )
{
	const int nSize = vecEqmt.size();
	if (nSize < 2)
	{
		return;
	}


	float fXMax = 0;
	float fXMin = 0;

	vector<CEqmDrawBase*>::const_iterator	iter;
	for (iter=vecEqmt.begin(); iter!=vecEqmt.end(); ++iter)
	{
		if (iter == vecEqmt.begin())
		{
			fXMax = (*iter)->GetPos().X;
			fXMin = (*iter)->GetPos().X;
		}
		else
		{
			if ((*iter)->GetPos().X > fXMax)
			{
				fXMax = (*iter)->GetPos().X;
			}

			if ((*iter)->GetPos().X < fXMin)
			{
				fXMin = (*iter)->GetPos().X;
			}
		}
	}

	float fInterval = (fXMax - fXMin) / (nSize - 1);
	float fX = 0;

	int i = 0;
	sort(vecEqmt.begin(), vecEqmt.end(), CompareX);
	for (iter=vecEqmt.begin(); iter!=vecEqmt.end(); ++iter,++i)
	{
		fX = fXMin + i * fInterval;
		(*iter)->SetPos(Gdiplus::PointF(fX, (*iter)->GetPos().Y));
	}

	//int i=0, j=0;
	//if ( vecEqmt.size() < 2 )
	//	return;
	//for ( i = 1; i < (int)vecEqmt.size(); i++ )
	//{
	//	CEqmDrawBase *pEqmt = vecEqmt[i];
	//	for ( j = i-1; j >= 0&& vecEqmt[j]->GetPos().X > pEqmt->GetPos().X; j-- )
	//	{
	//		vecEqmt[j+1] = vecEqmt[j];  
	//	}
	//	vecEqmt[j+1] = pEqmt;
	//}
	//int x = (vecEqmt[vecEqmt.size()-1]->GetPos().X - vecEqmt[0]->GetPos().X)/((int)vecEqmt.size()-1);
	//for ( int nIndex = 1; nIndex < (int)vecEqmt.size()-1; nIndex++ )
	//{
	//	vecEqmt[nIndex]->SetPos(Gdiplus::PointF(vecEqmt[nIndex-1]->GetPos().X+x, vecEqmt[nIndex]->GetPos().Y));
	//}
}

void DrawProxy::DownAlign( vector<CEqmDrawBase *> & vecEqmt )
{
	const int nSize = vecEqmt.size();
	if (nSize < 2)
	{
		return;
	}


	float fYMax = 0;
	float fYMin = 0;

	vector<CEqmDrawBase*>::const_iterator	iter;
	for (iter=vecEqmt.begin(); iter!=vecEqmt.end(); ++iter)
	{
		if (iter == vecEqmt.begin())
		{
			fYMax = (*iter)->GetPos().Y;
			fYMin = (*iter)->GetPos().Y;
		}
		else
		{
			if ((*iter)->GetPos().Y > fYMax)
			{
				fYMax = (*iter)->GetPos().Y;
			}

			if ((*iter)->GetPos().Y < fYMin)
			{
				fYMin = (*iter)->GetPos().Y;
			}
		}
	}

	float fInterval = (fYMax - fYMin) / (nSize - 1);
	float fY = 0;

	int i = 0;
	sort(vecEqmt.begin(), vecEqmt.end(), CompareY);
	for (iter=vecEqmt.begin(); iter!=vecEqmt.end(); ++iter,++i)
	{
		fY = fYMin + i * fInterval;
		(*iter)->SetPos(Gdiplus::PointF((*iter)->GetPos().X, fY));
	}

	//int i=0, j=0;
	//if ( vecEqmt.size() < 2 )
	//	return;
	//for ( i = 1; i < (int)vecEqmt.size(); i++ )
	//{
	//	CEqmDrawBase *pEqmt = vecEqmt[i];
	//	for ( j = i-1; j >= 0&& vecEqmt[j]->GetPos().Y > pEqmt->GetPos().Y; j-- )
	//	{
	//		vecEqmt[j+1] = vecEqmt[j];  
	//	}
	//	vecEqmt[j+1] = pEqmt;
	//}
	//int y = (vecEqmt[vecEqmt.size()-1]->GetPos().Y - vecEqmt[0]->GetPos().Y)/((int)vecEqmt.size()-1);
	//for ( int nIndex = 1; nIndex < (int)vecEqmt.size()-1; nIndex++ )
	//{
	//	vecEqmt[nIndex]->SetPos(Gdiplus::PointF(vecEqmt[nIndex]->GetPos().X, vecEqmt[nIndex-1]->GetPos().Y+y ));
	//}
}

Direction DrawProxy::PickObjectSide( void *pDataSt /*= NULL*/ )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;

	if ( pDrawElement->vecSelectEqmt.empty() )
	{
		return Dir_None;
	}
	const Direction dir =  pDrawElement->vecSelectEqmt[0]->PointIsSide(pDrawElement->actPt);
    return dir;
}

void DrawProxy::MakeSameWidth( vector<CEqmDrawBase *> &vecEqmt )
{
	int width = 0, height = 0;
	for ( int nIndex = 0; nIndex < (int)vecEqmt.size(); nIndex++ )
	{
		if ( vecEqmt[nIndex]->IsMainAlign() )
		{
			width = vecEqmt[nIndex]->GetWidth();
			break;
		}	
	}
	for ( unsigned int i = 0; i < vecEqmt.size(); i++ )
	{
		vecEqmt[i]->SetWidth(width);
	}
}

void DrawProxy::MakeSameHeight( vector<CEqmDrawBase *> &vecEqmt )
{
	int width = 0, height = 0;
	for ( int nIndex = 0; nIndex < (int)vecEqmt.size(); nIndex++ )
	{
		if ( vecEqmt[nIndex]->IsMainAlign() )
		{
			height = vecEqmt[nIndex]->GetHeight();
			break;
		}	
	}
	for ( unsigned int i = 0; i < vecEqmt.size(); i++ )
	{
		vecEqmt[i]->SetHeight(height);
	}
}

void DrawProxy::MakeSameSize( vector<CEqmDrawBase *> &vecEqmt )
{
	int width = 0, height = 0;
	for ( int nIndex = 0; nIndex < (int)vecEqmt.size(); nIndex++ )
	{
		if ( vecEqmt[nIndex]->IsMainAlign() )
		{
			width = vecEqmt[nIndex]->GetWidth();
			height = vecEqmt[nIndex]->GetHeight();
			break;
		}	
	}
	for ( unsigned int i = 0; i < vecEqmt.size(); i++ )
	{
		vecEqmt[i]->SetWidth(width);
		vecEqmt[i]->SetHeight(height);
	}
}


/*
*author: juneay
*date:2010-12-8
*comment:左旋转组件90度
*param: CEqmDrawBase * pEqmt
*/
void DrawProxy::LeftRotate90( CEqmDrawBase * pEqmt )
{
	pEqmt->LeftRotate90();
}

/*
*author: juneay
*date:2010-12-8
*comment:右旋转组件90度
*param: CEqmDrawBase * pEqmt
*/
void DrawProxy::RightRotate90( CEqmDrawBase * pEqmt )
{
	pEqmt->RightRotate90();
}

void DrawProxy::ShiftEqmtsUp(DrawContext* pDataSt )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;
	if ( pDrawElement->m_bIsChangeSize )
	{
		return;
	}
	for ( unsigned int uiIndex = 0; uiIndex < pDrawElement->vecSelectEqmt.size(); uiIndex++ )
	{
		pDrawElement->vecSelectEqmt[uiIndex]->ShiftEqmtsUp();
	}

}

void DrawProxy::ShiftEqmtsDown(DrawContext* pDataSt )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;
	if ( pDrawElement->m_bIsChangeSize )
	{
		return;
	}
	for ( unsigned int uiIndex = 0; uiIndex < pDrawElement->vecSelectEqmt.size(); uiIndex++ )
	{
		pDrawElement->vecSelectEqmt[uiIndex]->ShiftEqmtsDown();
	}
}

void DrawProxy::ShiftEqmtsLeft(DrawContext* pDataSt )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;
	if ( pDrawElement->m_bIsChangeSize )
	{
		return;
	}
	for ( unsigned int uiIndex = 0; uiIndex < pDrawElement->vecSelectEqmt.size(); uiIndex++ )
	{
		pDrawElement->vecSelectEqmt[uiIndex]->ShiftEqmtsLeft();
	}
}

void DrawProxy::ShiftEqmtsRight(DrawContext* pDataSt )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;
	if ( pDrawElement->m_bIsChangeSize )
	{
		return;
	}
	for ( unsigned int uiIndex = 0; uiIndex < pDrawElement->vecSelectEqmt.size(); uiIndex++ )
	{
		pDrawElement->vecSelectEqmt[uiIndex]->ShiftEqmtsRight();
	}
}

void DrawProxy::ShiftEqmtsUpCtrl(DrawContext* pDataSt )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;
	if ( pDrawElement->m_bIsChangeSize )
	{
		return;
	}
	for ( unsigned int uiIndex = 0; uiIndex < pDrawElement->vecSelectEqmt.size(); uiIndex++ )
	{
		pDrawElement->vecSelectEqmt[uiIndex]->ShiftEqmtsCtrlUp();
	}

}

void DrawProxy::ShiftEqmtsDownCtrl(DrawContext* pDataSt )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;
	if ( pDrawElement->m_bIsChangeSize )
	{
		return;
	}
	for ( unsigned int uiIndex = 0; uiIndex < pDrawElement->vecSelectEqmt.size(); uiIndex++ )
	{
		pDrawElement->vecSelectEqmt[uiIndex]->ShiftEqmtsCtrlDown();
	}
}

void DrawProxy::ShiftEqmtsLeftCtrl(DrawContext* pDataSt )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;
	if ( pDrawElement->m_bIsChangeSize )
	{
		return;
	}
	for ( unsigned int uiIndex = 0; uiIndex < pDrawElement->vecSelectEqmt.size(); uiIndex++ )
	{
		pDrawElement->vecSelectEqmt[uiIndex]->ShiftEqmtsCtrlLeft();
	}
}

void DrawProxy::ShiftEqmtsRightCtrl(DrawContext* pDataSt )
{
	DrawContext * pDrawElement = (DrawContext *)pDataSt;
	if ( pDrawElement->m_bIsChangeSize )
	{
		return;
	}
	for ( unsigned int uiIndex = 0; uiIndex < pDrawElement->vecSelectEqmt.size(); uiIndex++ )
	{
		pDrawElement->vecSelectEqmt[uiIndex]->ShiftEqmtsCtrlRight();
	}
}

/*
*author:juneay
*date:2010-12-13
*comment:检测是否选中矩形框
*/
CEqmDrawGraphic * DrawProxy::IsPickRectangle( Gdiplus::PointF pt, void *pDateSt /*= NULL*/ )
{
	DrawContext *pPicElmt = static_cast<DrawContext *>(pDateSt); //读取传入的绘图参数
	if ( NULL == pPicElmt || NULL == pPicElmt->pPage )
	{
		return NULL;//检测是否传入的参数为空
	}

	for ( list<CEqmDrawGraphic*>::iterator it = pPicElmt->pPage->Get2DGraphicList().begin();
		it != pPicElmt->pPage->Get2DGraphicList().end(); it++ )
	{
		if ( GraphicType_RECTANGLE == (*it)->GetEqmType() && INVALID_FORWARDPAGEID != (*it)->GetForwardPageID() )
		{
			Gdiplus::PointF ptStart,ptEnd;
			ptStart.X = (*it)->GetPointList()[0].X > (*it)->GetPointList()[1].X ? (*it)->GetPointList()[1].X : (*it)->GetPointList()[0].X;
			ptStart.Y = (*it)->GetPointList()[0].Y > (*it)->GetPointList()[1].Y ? (*it)->GetPointList()[1].Y : (*it)->GetPointList()[0].Y;

			ptEnd.X = (*it)->GetPointList()[0].X > (*it)->GetPointList()[1].X ? (*it)->GetPointList()[0].X : (*it)->GetPointList()[1].X;
			ptEnd.Y = (*it)->GetPointList()[0].Y > (*it)->GetPointList()[1].Y ? (*it)->GetPointList()[0].Y : (*it)->GetPointList()[1].Y;
 
			if ( pt.X > ptStart.X&& pt.X < ptEnd.X && pt.Y > ptStart.Y && pt.Y < ptEnd.Y )
			{
				return (*it);
			}
		}		
	}
	return NULL;
}

//bird add
//use select box to select several items
void DrawProxy::PickObjectArray(DrawContext *pDrawElement , std::vector<CEqmDrawBase*>& rItemArray)
{
	ASSERT(pDrawElement);
	if ( NULL == pDrawElement || NULL == pDrawElement->pPage )
	{
		return ;
	}

	unsigned int i =0;

	rItemArray.clear();
	const Gdiplus::RectF rectSelect = GetRectFrom2Points(pDrawElement->PrePt.X, pDrawElement->PrePt.Y, pDrawElement->actPt.X, pDrawElement->actPt.Y);

	//device list
	PickObjectArrayDevice(pDrawElement, rItemArray, rectSelect);

	//PipeList
	PickObjectArrayPipe(pDrawElement, rItemArray, rectSelect);    

	//2DGraphicList //todo
	PickObjectArrayGraphic(pDrawElement , rItemArray, rectSelect);

	//TextList
	PickObjectArrayText(pDrawElement , rItemArray, rectSelect);

	PickObjectArrayBend(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayDashBoard(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayLineGraphic(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayBarGraphic(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayPieGraphic(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayButton(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayTimeSelect(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayTimeBar(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayProgressBar(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayDiagnoseGrapgic(pDrawElement , rItemArray, rectSelect);
	PickObjectArraypPolygon(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayAreaChange(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayShellExecute(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayEmbededPage(pDrawElement , rItemArray, rectSelect);
	PickObjectArrayLiquidLevel(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayCheckButtonPlus(pDrawElement, rItemArray, rectSelect);
	//PickObjectArrayScrawl(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayHistoryTrend(pDrawElement, rItemArray, rectSelect);
	//PickObjectArrayEnergySaveRoi(pDrawElement, rItemArray, rectSelect);//PickObjectArrayRectangle 功能重复
	PickObjectArrayMapNavigate(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayNavigateWindow(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayLevelRuler(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayDataAnalyse(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayTemperDistri(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayDataReport(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayPlaneTempeDistr(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayDottedLineFrm(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayGeneralComponent(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayRectangle(pDrawElement, rItemArray, rectSelect);
	PickObjectArrayStraightLine(pDrawElement, rItemArray, rectSelect);
}

void  DrawProxy::PickObjectArrayDevice(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
    list<CEqmDrawDevice*>& deviceList = pDrawElement->pPage->GetDrawDeviceList();

	for (list<CEqmDrawDevice*>::iterator it = deviceList.begin();
		it != deviceList.end(); it++)
	{
		CEqmDrawDevice* pDevice = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pDevice->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pDevice->GetWidth(),  pDevice->GetHeight() );
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()])
		{
			rItemArray.push_back(*it);
		}
	}
}

void  DrawProxy::PickObjectArrayPipe(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
    list<CEqmDrawPipe*>& pipeList = pDrawElement->pPage->GetPagePipeList();

	for (list<CEqmDrawPipe*>::iterator it = pipeList.begin();
		it != pipeList.end(); it++)
	{
		CEqmDrawPipe* pDevice = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		int startX = pDevice->GetPointList()[0].X;
		int startY = pDevice->GetPointList()[0].Y;
		for ( unsigned int uiIndex = 1; uiIndex < pDevice->GetPointList().size(); uiIndex++ )
		{
			int nWidth = startX == pDevice->GetPointList()[uiIndex].X ? DEFAULTSIDELINEWIDTH: abs(startX-pDevice->GetPointList()[uiIndex].X);
			int nHeight = startY == pDevice->GetPointList()[uiIndex].Y ? DEFAULTSIDELINEWIDTH: abs(startY-pDevice->GetPointList()[uiIndex].Y);
			startX = startX > pDevice->GetPointList()[uiIndex].X ? pDevice->GetPointList()[uiIndex].X: startX;
			startY = startY > pDevice->GetPointList()[uiIndex].Y ? pDevice->GetPointList()[uiIndex].Y: startY;
			Gdiplus::RectF rectItem(startX,startY, nWidth, nHeight);
			if ((*it)->IfMeContainRect(rectSelect, rectItem) && m_bShow[(*it)->GetLayer()])
			{
				rItemArray.push_back(pDevice);
				break;
			}
			startY = pDevice->GetPointList()[uiIndex].Y;
			startX = pDevice->GetPointList()[uiIndex].X;
		}		
	}
}

void DrawProxy::PickObjectArrayGraphic(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{

	CEqmDrawGraphic* pEqmGraphic = NULL;
	for ( list<CEqmDrawGraphic*>::iterator it = pDrawElement->pPage->Get2DGraphicList().begin();it != pDrawElement->pPage->Get2DGraphicList().end(); it++)
	{
		pEqmGraphic = *it;
		if(!pEqmGraphic) continue;
		if(!(*it)->GetDeleteFlag())
		{
			Gdiplus::PointF pt0;
			Gdiplus::PointF pt1;
			Gdiplus::RectF rectGraph;

			const EQMDRAWTYPE type = pEqmGraphic->GetEqmType();             
			switch ( type )
			{
			case GraphicType_LINE: 
				{
					bool ifVertical;
					UINT lineWidth;

					for ( UINT nIndex = 1; nIndex < pEqmGraphic->GetPointList().size(); nIndex++ )
					{
						pt0 = pEqmGraphic->GetPointList()[nIndex-1];
						pt1 = pEqmGraphic->GetPointList()[nIndex];
						ifVertical = (pt0.X == pt1.X);
						lineWidth  = pEqmGraphic->GetLineWidth();
						if ( ifVertical )
						{
							pt1.X += lineWidth/2;
							pt0.X -= lineWidth/2;
						}
						else
						{
							pt0.Y -= lineWidth/2;
							pt1.Y += lineWidth/2;
						}
						rectGraph = GetRectFrom2Points(pt0.X, pt0.Y, pt1.X, pt1.Y);
						if(rectSelect.IntersectsWith(rectGraph)  && m_bShow[pEqmGraphic->GetLayer()])
						{
							rItemArray.push_back(pEqmGraphic);
							break; // added by david.
						}
					}			
				}
				break;
			case GraphicType_RECTANGLE: 
				{              
					pt0 = pEqmGraphic->GetPointList()[0];
					pt1 = pEqmGraphic->GetPointList()[1];
					rectGraph = GetRectFrom2Points(pt0.X, pt0.Y, pt1.X, pt1.Y);     
					if(rectSelect.IntersectsWith(rectGraph) && m_bShow[pEqmGraphic->GetLayer()])
					{
						rItemArray.push_back(pEqmGraphic);
					}
				}
				break;
			case GraphicType_ELLIPSE: //todo
				{              
					pt0 = pEqmGraphic->GetPointList()[0];
					pt1 = pEqmGraphic->GetPointList()[1];
					rectGraph = GetRectFrom2Points(pt0.X, pt0.Y, pt1.X, pt1.Y);     
					if(rectSelect.IntersectsWith(rectGraph) && m_bShow[pEqmGraphic->GetLayer()] )
					{
						rItemArray.push_back(pEqmGraphic);
					}
				}
				break;
			case GraphicType_TRIANGLE: //todo
			case GraphicType_ARROW: //todo
			default:
				break;
			}
		}
	}
}

void DrawProxy::PickObjectArrayText(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{

	for (list<CEqmDrawText*>::iterator it = pDrawElement->pPage->GetPageTextList().begin();
		it != pDrawElement->pPage->GetPageTextList().end(); it++)
	{
		CEqmDrawText* pDevice = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pDevice->GetPos();
		RECT rect;
		rect.left = (int)rectSelect.GetLeft();
		rect.top = (int)rectSelect.GetTop();
		rect.right = (int)rectSelect.GetRight();
		rect.bottom = (int)rectSelect.GetBottom();
		POINT point;
		point.x = (int)pt.X;
		point.y = (int)pt.Y;
		if ( PtInRect(&rect, point) && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(pDevice);
		}	
	}
}

void DrawProxy::FormatBrush( vector<CEqmDrawBase *> &vecEqmt )
{
	if ( vecEqmt.size() > 1  )
	{
		CEqmDrawBase *pBase = NULL;
		for ( UINT nIndex = 0; nIndex < vecEqmt.size(); nIndex++ )
		{
			if ( vecEqmt[nIndex]->IsMainAlign() )
			{
				pBase = vecEqmt[nIndex];
				break;
			}
		}
		if ( pBase )
		{
			for ( UINT nIndex = 0; nIndex < vecEqmt.size(); nIndex++ )
			{
				vecEqmt[nIndex]->CopyProperty(pBase);
			}
		}
	}
}

void DrawProxy::DrawGrid()
{
	if ( m_bChooseGrid )
	{
		DWORD screenwidth = 0;
		DWORD screenheight = 0;

		if (m_pDrawContext->pPage->GetPageType() == FULLSCREEN)
		{
			screenwidth = m_pDrawContext->GetScreenWidth();
			screenheight = m_pDrawContext->GetScreenHeight();
		}
		else
		{
			screenwidth = m_pDrawContext->pPage->GetPageWidth();
			screenheight = m_pDrawContext->pPage->GetPageHeight();
		}
		for (unsigned int i = CELLINTERVAL; i < screenwidth; i += CELLINTERVAL )
		{
			for (unsigned int j = CELLINTERVAL; j < screenheight;j += CELLINTERVAL )
			{
				m_pDC->SetPixel(CPoint(i, j), RGB(0, 0, 0));
			}
		}
	}
}

void DrawProxy::KeyEnterDown()
{
	if ( m_pDrawContext->bIsCreatePolygon )
	{	CEqmDrawPolygon * pPolygon = dynamic_cast<CEqmDrawPolygon*>(m_pDrawContext->pNewEqmt);
		if ( NULL != pPolygon && pPolygon->PointSize()>2) //if the pipe contains less than two points,we will delete this pipe
		{
			m_pDrawContext->pPage->AddPolygon(pPolygon);
			m_pDrawContext->bIsCreatePolygon = false;
			m_pDrawContext->pNewEqmt = NULL;
		}
	}
}

CEqmDrawBase* DrawProxy::PickObject( CEqmDrawPage *pPage, double fX, double fY)
{
	if(pPage==NULL)
		return NULL;

	unsigned int i =0;
	PointF actPt(fX, fY);
    //pick layer top 1st
    for(int layerIdx = E_LAYER_MAX-1; layerIdx >= E_LAYER_1; layerIdx --)
	{
			//if the mouse point at text
			for (list<CEqmDrawText*>::iterator it = pPage->GetPageTextList().begin(); it != pPage->GetPageTextList().end(); it++)
			{
				CEqmDrawText *pText = (*it);
				if (pText->GetLayer() == layerIdx )
				{
					if ( pText->PointInMe(actPt) && m_bShow[pText->GetLayer()] && !pText->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			//if the mouse point at pipe
			list<CEqmDrawPipe*>& pipeList = pPage->GetPagePipeList();
			for (list<CEqmDrawPipe*>::iterator it = pipeList.begin();it != pipeList.end(); it++)
			{
				CEqmDrawPipe* pPipe = *it;
				if (pPipe->GetLayer() == layerIdx )
				{
					if ( pPipe->PointInMe(actPt) && m_bShow[pPipe->GetLayer()] && !pPipe->GetDeleteFlag())
					{
						return pPipe;
					}
				}
			}

			//if the mouse point at graphic
			for ( list<CEqmDrawGraphic*>::iterator it = pPage->Get2DGraphicList().begin();it != pPage->Get2DGraphicList().end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx )
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{
						return *it;
					}
				}
			}

			//严观微添加
			for (list<CEqmDrawBend*>::iterator it = pPage->GetBendList().begin(); it != pPage->GetBendList().end(); it++)
			{
				CEqmDrawBend *pBend = (*it);
				if (pBend->GetLayer() == layerIdx )
				{
					if ( pBend->PointInMe(actPt) && m_bShow[pBend->GetLayer()] && !pBend->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}
			for (list<CEqmDrawDashBoard*>::iterator it = pPage->GetDashboardList().begin(); it != pPage->GetDashboardList().end(); it++)
			{
				CEqmDrawDashBoard* pDashBoard = (*it);
				if (pDashBoard->GetLayer() == layerIdx )
				{
					if ( pDashBoard->PointInMe(actPt) && m_bShow[pDashBoard->GetLayer()] && !pDashBoard->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}
			for (list<CEqmDrawLineGraphic*>::iterator it = pPage->GetLineGraphList().begin(); it != pPage->GetLineGraphList().end(); it++)
			{
				CEqmDrawLineGraphic* pLineGraphic = (*it);
				if (pLineGraphic->GetLayer() == layerIdx )
				{
					if ( pLineGraphic->PointInMe(actPt) && m_bShow[pLineGraphic->GetLayer()] && !pLineGraphic->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}
			for (list<CEqmDrawBarGraphic*>::iterator it = pPage->GetBarGraphList().begin(); it != pPage->GetBarGraphList().end(); it++)
			{
				CEqmDrawBarGraphic* pBarGraphic = (*it);
				if (pBarGraphic->GetLayer() == layerIdx )
				{
					if ( pBarGraphic->PointInMe(actPt) && m_bShow[pBarGraphic->GetLayer()] && !pBarGraphic->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}
			for (list<CEqmDrawPieGraphic*>::iterator it = pPage->GetPieGraphList().begin(); it != pPage->GetPieGraphList().end(); it++)
			{
				CEqmDrawPieGraphic* pPieGraphic = (*it);
				if (pPieGraphic->GetLayer() == layerIdx )
				{
					if ( pPieGraphic->PointInMe(actPt) && m_bShow[pPieGraphic->GetLayer()] && !pPieGraphic->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}
			for (list<CEqmDrawButton*>::iterator it = pPage->GetButtonList().begin(); it != pPage->GetButtonList().end(); it++)
			{
				CEqmDrawButton* pButton = (*it);
				if (pButton->GetLayer() == layerIdx )
				{
					if ( pButton->PointInMe(actPt) && m_bShow[pButton->GetLayer()] && !pButton->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}
			for (list<CEqmDrawTimeSelect*>::iterator it = pPage->GetTimeSelectList().begin(); it != pPage->GetTimeSelectList().end(); it++)
			{
				CEqmDrawTimeSelect* pTimeSelect = (*it);
				if (pTimeSelect->GetLayer() == layerIdx )
				{
					if ( pTimeSelect->PointInMe(actPt) && m_bShow[pTimeSelect->GetLayer()] && !pTimeSelect->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}
			for (list<CEqmDrawTimeBar*>::iterator it = pPage->GetTimeBarList().begin(); it != pPage->GetTimeBarList().end(); it++)
			{
				CEqmDrawTimeBar* pTimeBar = (*it);
				if (pTimeBar->GetLayer() == layerIdx )
				{
					if ( pTimeBar->PointInMe(actPt) && m_bShow[pTimeBar->GetLayer()] && !pTimeBar->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}
			for (list<CEqmDrawProgressBar*>::iterator it = pPage->GetProgressBarList().begin(); it != pPage->GetProgressBarList().end(); it++)
			{
				CEqmDrawProgressBar* pProgressBar = (*it);
				if (pProgressBar->GetLayer() == layerIdx )
				{
					if ( pProgressBar->PointInMe(actPt) && m_bShow[pProgressBar->GetLayer()] && !pProgressBar->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}
			for (list<CEqmDrawDiagnoseGraphic*>::iterator it = pPage->GetDiagnoseList().begin(); it != pPage->GetDiagnoseList().end(); it++)
			{
				CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = (*it);
				if (pDiagnoseGraphic->GetLayer() == layerIdx )
				{
					if ( pDiagnoseGraphic->PointInMe(actPt) && m_bShow[pDiagnoseGraphic->GetLayer()] && !pDiagnoseGraphic->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}		
			for (list<CEqmDrawPolygon*>::iterator it = pPage->GetPolygon().begin(); it != pPage->GetPolygon().end(); it++)
			{
				CEqmDrawPolygon* pPolygon = (*it);
				if (pPolygon->GetLayer() == layerIdx )
				{
					if ( pPolygon->PointInMe(actPt) && m_bShow[pPolygon->GetLayer()] && !pPolygon->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawAreaChange*>& areaList = pPage->GetAreaChangeList();
			for (list<CEqmDrawAreaChange*>::iterator it = areaList.begin(); it != areaList.end(); it++)
			{
				CEqmDrawAreaChange* pAreaChange = (*it);
				if (pAreaChange->GetLayer() == layerIdx )
				{
					if ( pAreaChange->PointInMe(actPt) && m_bShow[pAreaChange->GetLayer()] && !pAreaChange->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}	

			list<CEqmDrawShellExecute*>& executeList = pPage->GetShellExecuteList();
			for (list<CEqmDrawShellExecute*>::iterator it = executeList.begin(); it != executeList.end(); it++)
			{
				CEqmDrawShellExecute* pShellExecute = (*it);
				if (pShellExecute->GetLayer() == layerIdx )
				{
					if ( pShellExecute->PointInMe(actPt) && m_bShow[pShellExecute->GetLayer()] && !pShellExecute->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawEmbededPage*>& embededpageList = pPage->GetEmbededPageList();
			for (list<CEqmDrawEmbededPage*>::iterator it = embededpageList.begin(); it != embededpageList.end(); it++)
			{
				CEqmDrawEmbededPage* pEmbededPage = (*it);
				if (pEmbededPage->GetLayer() == layerIdx )
				{
					if ( pEmbededPage->PointInMe(actPt) && m_bShow[pEmbededPage->GetLayer()] && !pEmbededPage->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawLiquidLevel*>& liquidLevelList = pPage->GetLiquidLevelList();
			for (list<CEqmDrawLiquidLevel*>::iterator it = liquidLevelList.begin(); it != liquidLevelList.end(); it++)
			{
				CEqmDrawLiquidLevel* pLiquidLevel = (*it);
				if (pLiquidLevel->GetLayer() == layerIdx )
				{
					if (pLiquidLevel->PointInMe(actPt) && m_bShow[pLiquidLevel->GetLayer()] && !pLiquidLevel->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawButtonPlus*>& chkBtnPlusList = pPage->GetCheckButtonPlusList();
			for (list<CEqmDrawButtonPlus*>::iterator it = chkBtnPlusList.begin(); it != chkBtnPlusList.end(); it++)
			{
				CEqmDrawButtonPlus* pChkBtnPlus = (*it);
				if (pChkBtnPlus->GetLayer() == layerIdx )
				{
					if (pChkBtnPlus->PointInMe(actPt) && m_bShow[pChkBtnPlus->GetLayer()] && !pChkBtnPlus->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawScrawl*>& scrawlList = pPage->GetScrawlList();
			for (list<CEqmDrawScrawl*>::iterator it = scrawlList.begin(); it != scrawlList.end(); it++)
			{
				CEqmDrawScrawl* pScrawl = (*it);
				if (pScrawl->GetLayer() == layerIdx)
				{
					if (pScrawl->PointInMe(actPt) && m_bShow[pScrawl->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawHistoryTrend*>& historyTrendList = pPage->GetHistoryTrendList();
			for (list<CEqmDrawHistoryTrend*>::iterator it = historyTrendList.begin(); it != historyTrendList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawEnergySaveRoi*>& energySaveRoiList = pPage->GetEnergySaveRoiList();
			for (list<CEqmDrawEnergySaveRoi*>::iterator it = energySaveRoiList.begin(); it != energySaveRoiList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawMapNavigate*>& mapNavigateList = pPage->GetMapNavigateList();
			for (list<CEqmDrawMapNavigate*>::iterator it = mapNavigateList.begin(); it != mapNavigateList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}	

			list<CEqmDrawNavigateWindow*>& naviWindList = pPage->GetNavigateWindowList();
			for (list<CEqmDrawNavigateWindow*>::iterator it = naviWindList.begin(); it != naviWindList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawLevelRuler*>& levelRulerList = pPage->GetLevelRulerList();
			for (list<CEqmDrawLevelRuler*>::iterator it = levelRulerList.begin(); it != levelRulerList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawDataAnalyse*>& dataAnalyseList = pPage->GetDataAnalyseList();
			for (list<CEqmDrawDataAnalyse*>::iterator it = dataAnalyseList.begin(); it != dataAnalyseList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawTemperDistri*>& temperDistriList = pPage->GetTemperDistriList();
			for (list<CEqmDrawTemperDistri*>::iterator it = temperDistriList.begin(); it != temperDistriList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawDataReport*>& dataReportList = pPage->GetDataReportList();
			for (list<CEqmDrawDataReport*>::iterator it = dataReportList.begin(); it != dataReportList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawPlaneTempeDistr*>& planeTempeDistrList = pPage->GetPlaneTempeDistrList();
			for (list<CEqmDrawPlaneTempeDistr*>::iterator it = planeTempeDistrList.begin(); it != planeTempeDistrList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawDottedLineFrm*>& dottedLineList = pPage->GetDottedLineFrmList();
			for (list<CEqmDrawDottedLineFrm*>::iterator it = dottedLineList.begin(); it != dottedLineList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawGeneralComponent*>& generalComponentList = pPage->GetGeneralComponentList();
			for (list<CEqmDrawGeneralComponent*>::iterator it = generalComponentList.begin(); it != generalComponentList.end(); it++)
			{
				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{	
						return *it;
					}
				}
			}

			list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
			for (list<CEqmDrawDevice* >::reverse_iterator it=deviceList.rbegin(); it!=deviceList.rend(); it++)
			{
				if (NULL == (*it))
				{
					continue;
				}

				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{
						return *it;
					}
				}
			}

			list<CEqmDrawRectangle*>& rectangleList = pPage->GetRectangleList();
			for (list<CEqmDrawRectangle* >::reverse_iterator it=rectangleList.rbegin(); it!=rectangleList.rend(); it++)
			{
				if (NULL == (*it))
				{
					continue;
				}

				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{
						return *it;
					}
				}
			}

			list<CEqmDrawStraightLine*>& straightLineList = pPage->GetStraightLineList();
			for (list<CEqmDrawStraightLine* >::reverse_iterator it=straightLineList.rbegin(); it!=straightLineList.rend(); it++)
			{
				if (NULL == (*it))
				{
					continue;
				}

				if ((*it)->GetLayer() == layerIdx)
				{
					if ((*it)->PointInMe(actPt) && m_bShow[(*it)->GetLayer()] && !(*it)->GetDeleteFlag())
					{
						return *it;
					}
				}
			}
	}
    return NULL;
}

void DrawProxy::DrawBend( UINT layer )
{
	CEqmDrawBend* pBend = NULL;
	list<CEqmDrawBend*>& bendList = m_pDrawContext->pPage->GetBendList();
	for (list<CEqmDrawBend*>::iterator it = bendList.begin();it != bendList.end(); it++)
	{
		pBend = *it;
		if(pBend)
		{
			if ( pBend->IsSelected() || !pBend->GetShow()|| pBend->GetLayer() != layer || pBend->GetDeleteFlag() == true)
			{
				continue;
			}
			pBend->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::DrawDashBoard( UINT layer )
{
	CEqmDrawDashBoard* pDashBoard = NULL;
	list<CEqmDrawDashBoard*>& dashBoardList = m_pDrawContext->pPage->GetDashboardList();
	for (list<CEqmDrawDashBoard*>::iterator it = dashBoardList.begin();it != dashBoardList.end(); it++)
	{
		pDashBoard = *it;
		if(pDashBoard)
		{
			if ( pDashBoard->IsSelected() || !pDashBoard->GetShow()|| pDashBoard->GetLayer() != layer || pDashBoard->GetDeleteFlag() == true)
			{
				continue;
			}
			pDashBoard->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::DrawLineGraphic( UINT layer )
{
	CEqmDrawLineGraphic* pLineGraphic = NULL;
	list<CEqmDrawLineGraphic*>& lineGraphicList = m_pDrawContext->pPage->GetLineGraphList();
	for (list<CEqmDrawLineGraphic*>::iterator it = lineGraphicList.begin();it != lineGraphicList.end(); it++)
	{
		pLineGraphic = *it;
		if(pLineGraphic)
		{
			if ( pLineGraphic->IsSelected() || !pLineGraphic->GetShow()|| pLineGraphic->GetLayer() != layer || pLineGraphic->GetDeleteFlag() == true)
			{
				continue;
			}
			pLineGraphic->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::DrawBarGraphic( UINT layer )
{
	CEqmDrawBarGraphic* pBarGraphic = NULL;
	list<CEqmDrawBarGraphic*>& barGraphicList = m_pDrawContext->pPage->GetBarGraphList();
	for (list<CEqmDrawBarGraphic*>::iterator it = barGraphicList.begin();it != barGraphicList.end(); it++)
	{
		pBarGraphic = *it;
		if(pBarGraphic)
		{
			if ( pBarGraphic->IsSelected() || !pBarGraphic->GetShow()|| pBarGraphic->GetLayer() != layer || pBarGraphic->GetDeleteFlag() == true)
			{
				continue;
			}
			pBarGraphic->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::DrawPieGraphic( UINT layer )
{
	CEqmDrawPieGraphic* pPieGraphic = NULL;
	list<CEqmDrawPieGraphic*>& pieGraphicList = m_pDrawContext->pPage->GetPieGraphList();
	for (list<CEqmDrawPieGraphic*>::iterator it = pieGraphicList.begin();it != pieGraphicList.end(); it++)
	{
		pPieGraphic = *it;
		if(pPieGraphic)
		{
			if ( pPieGraphic->IsSelected() || !pPieGraphic->GetShow()|| pPieGraphic->GetLayer() != layer || pPieGraphic->GetDeleteFlag() == true)
			{
				continue;
			}
			pPieGraphic->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::DrawButton( UINT layer )
{
	CEqmDrawButton* pButton = NULL;
	list<CEqmDrawButton*>& ButtonList = m_pDrawContext->pPage->GetButtonList();
	for (list<CEqmDrawButton*>::iterator it = ButtonList.begin();it != ButtonList.end(); it++)
	{
		pButton = *it;
		if(pButton)
		{
			if ( pButton->IsSelected() || !pButton->GetShow()|| pButton->GetLayer() != layer || pButton->GetDeleteFlag() == true)
			{
				continue;
			}
			pButton->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayBend( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawBend*>& bendList = pDrawElement->pPage->GetBendList();

	for (list<CEqmDrawBend*>::iterator it = bendList.begin();it != bendList.end(); it++)
	{
		CEqmDrawBend* pBend = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pBend->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pBend->GetWidth(), pBend->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)   && m_bShow[(*it)->GetLayer()])
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::PickObjectArrayDashBoard( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawDashBoard*>& dashBoardList = pDrawElement->pPage->GetDashboardList();

	for (list<CEqmDrawDashBoard*>::iterator it = dashBoardList.begin();it != dashBoardList.end(); it++)
	{
		CEqmDrawDashBoard* pDashBoard = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pDashBoard->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pDashBoard->GetWidth(), pDashBoard->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)   && m_bShow[(*it)->GetLayer()])
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::PickObjectArrayLineGraphic( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawLineGraphic*>& lineGraphicList = pDrawElement->pPage->GetLineGraphList();

	for (list<CEqmDrawLineGraphic*>::iterator it = lineGraphicList.begin();it != lineGraphicList.end(); it++)
	{
		CEqmDrawLineGraphic* pLineGraphic = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pLineGraphic->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pLineGraphic->GetWidth(), pLineGraphic->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)   && m_bShow[(*it)->GetLayer()])
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::PickObjectArrayBarGraphic( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawBarGraphic*>& barGraphicList = pDrawElement->pPage->GetBarGraphList();

	for (list<CEqmDrawBarGraphic*>::iterator it = barGraphicList.begin();it != barGraphicList.end(); it++)
	{
		CEqmDrawBarGraphic* pBarGraphic = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pBarGraphic->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pBarGraphic->GetWidth(), pBarGraphic->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)   && m_bShow[(*it)->GetLayer()])
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::PickObjectArrayPieGraphic( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawPieGraphic*>& pieGraphicList = pDrawElement->pPage->GetPieGraphList();

	for (list<CEqmDrawPieGraphic*>::iterator it = pieGraphicList.begin();it != pieGraphicList.end(); it++)
	{
		CEqmDrawPieGraphic* pPieGraphic = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pPieGraphic->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pPieGraphic->GetWidth(), pPieGraphic->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::PickObjectArrayButton( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawButton*>& buttonList = pDrawElement->pPage->GetButtonList();

	for (list<CEqmDrawButton*>::iterator it = buttonList.begin();it != buttonList.end(); it++)
	{
		CEqmDrawButton* pButton = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pButton->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pButton->GetWidth(), pButton->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawTimeSelect( UINT layer )
{
	CEqmDrawTimeSelect* pTimeSelect = NULL;
	list<CEqmDrawTimeSelect*>& timeSelectList = m_pDrawContext->pPage->GetTimeSelectList();
	for (list<CEqmDrawTimeSelect*>::iterator it = timeSelectList.begin();it != timeSelectList.end(); it++)
	{
		pTimeSelect = *it;
		if(pTimeSelect)
		{
			if ( pTimeSelect->IsSelected() || !pTimeSelect->GetShow()|| pTimeSelect->GetLayer() != layer || pTimeSelect->GetDeleteFlag()== true)
			{
				continue;
			}
			pTimeSelect->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayTimeSelect( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawTimeSelect*>& timeSelectList = pDrawElement->pPage->GetTimeSelectList();

	for (list<CEqmDrawTimeSelect*>::iterator it = timeSelectList.begin();it != timeSelectList.end(); it++)
	{
		CEqmDrawTimeSelect* pTimeSelect = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pTimeSelect->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pTimeSelect->GetWidth(), pTimeSelect->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawTimeBar( UINT layer )
{
	CEqmDrawTimeBar* pTimeBar = NULL;
	list<CEqmDrawTimeBar*>& timeBarList = m_pDrawContext->pPage->GetTimeBarList();
	for (list<CEqmDrawTimeBar*>::iterator it = timeBarList.begin();it != timeBarList.end(); it++)
	{
		pTimeBar = *it;
		if(pTimeBar)
		{
			if ( pTimeBar->IsSelected() || !pTimeBar->GetShow()|| pTimeBar->GetLayer() != layer || pTimeBar->GetDeleteFlag() == true)
			{
				continue;
			}
			pTimeBar->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayTimeBar( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawTimeBar*>& timeBarList = pDrawElement->pPage->GetTimeBarList();

	for (list<CEqmDrawTimeBar*>::iterator it = timeBarList.begin();it != timeBarList.end(); it++)
	{
		CEqmDrawTimeBar* pTimeBar = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pTimeBar->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pTimeBar->GetWidth(), pTimeBar->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawProgressBar( UINT layer )
{
	CEqmDrawProgressBar* pProgressBar = NULL;
	list<CEqmDrawProgressBar*>& ProgressBarList = m_pDrawContext->pPage->GetProgressBarList();
	for (list<CEqmDrawProgressBar*>::iterator it = ProgressBarList.begin();it != ProgressBarList.end(); it++)
	{
		pProgressBar = *it;
		if(pProgressBar)
		{
			if ( pProgressBar->IsSelected() || !pProgressBar->GetShow()|| pProgressBar->GetLayer() != layer || pProgressBar->GetDeleteFlag() == true)
			{
				continue;
			}
			pProgressBar->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayProgressBar( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawProgressBar*>& ProgressBarList = pDrawElement->pPage->GetProgressBarList();

	for (list<CEqmDrawProgressBar*>::iterator it = ProgressBarList.begin();it != ProgressBarList.end(); it++)
	{
		CEqmDrawProgressBar* pProgressBar = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pProgressBar->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pProgressBar->GetWidth(), pProgressBar->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawDiagnoseGraphic( UINT layer )
{
	CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = NULL;
	list<CEqmDrawDiagnoseGraphic*>& DiagnoseGraphicList = m_pDrawContext->pPage->GetDiagnoseList();
	for (list<CEqmDrawDiagnoseGraphic*>::iterator it = DiagnoseGraphicList.begin();it != DiagnoseGraphicList.end(); it++)
	{
		pDiagnoseGraphic = *it;
		if(pDiagnoseGraphic)
		{
			if ( pDiagnoseGraphic->IsSelected() || !pDiagnoseGraphic->GetShow()|| pDiagnoseGraphic->GetLayer() != layer
				|| pDiagnoseGraphic->GetDeleteFlag() == true)
			{
				continue;
			}
			pDiagnoseGraphic->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayDiagnoseGrapgic( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawDiagnoseGraphic*>& DiagnoseGraphicList = pDrawElement->pPage->GetDiagnoseList();

	for (list<CEqmDrawDiagnoseGraphic*>::iterator it = DiagnoseGraphicList.begin();it != DiagnoseGraphicList.end(); it++)
	{
		CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pDiagnoseGraphic->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pDiagnoseGraphic->GetWidth(), pDiagnoseGraphic->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawPolygon( UINT layer )
{
	CEqmDrawPolygon* pPolygon = NULL;
	list<CEqmDrawPolygon*>& pPolygonList = m_pDrawContext->pPage->GetPolygon();
	for (list<CEqmDrawPolygon*>::iterator it = pPolygonList.begin();it != pPolygonList.end(); it++)
	{
		pPolygon = *it;
		if(pPolygon)
		{
			if ( pPolygon->IsSelected() || !pPolygon->GetShow()|| pPolygon->GetLayer() != layer
				|| pPolygon->GetDeleteFlag() == true)
			{
				continue;
			}
			pPolygon->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::DrawCreatePolygon(UINT layer)
{
	if ( m_pDrawContext->bIsCreatePolygon )
	{
		CEqmDrawPolygon * pPolygon = dynamic_cast<CEqmDrawPolygon*>(m_pDrawContext->pNewEqmt);
		if (pPolygon && pPolygon->GetLayer() == layer)
		{
			pPolygon->DrawInCreateMode(m_pGC, m_pDrawContext->actPt);
		}
	}
}

void DrawProxy::PickObjectArraypPolygon( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawPolygon*>& pPloygonList = pDrawElement->pPage->GetPolygon();

	for (list<CEqmDrawPolygon*>::iterator it = pPloygonList.begin();it != pPloygonList.end(); it++)
	{
		CEqmDrawPolygon* pPolygon = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pPolygon->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pPolygon->GetWidth(), pPolygon->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawAreaChange( UINT layer )
{
	CEqmDrawAreaChange* pAreaChange = NULL;
	list<CEqmDrawAreaChange*>& areaList = m_pDrawContext->pPage->GetAreaChangeList();
	for (list<CEqmDrawAreaChange*>::iterator it = areaList.begin();
		it != areaList.end(); it++)
	{
		pAreaChange = *it;
		if(!pAreaChange)
			continue;
		if ( pAreaChange->IsSelected() || !pAreaChange->GetShow()|| pAreaChange->GetLayer() != layer
			|| pAreaChange->GetDeleteFlag() == true)
		{
			continue;
		}
		pAreaChange->Draw(m_pGC, NULL);
	}
}

void DrawProxy::PickObjectArrayAreaChange( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawAreaChange*>& AreaChangeList = pDrawElement->pPage->GetAreaChangeList();

	for (list<CEqmDrawAreaChange*>::iterator it = AreaChangeList.begin();it != AreaChangeList.end(); it++)
	{
		CEqmDrawAreaChange* pAreaChange = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pAreaChange->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pAreaChange->GetWidth(), pAreaChange->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawShellExecute( UINT layer )
{
	CEqmDrawShellExecute* pShellExecute = NULL;
	list<CEqmDrawShellExecute*>& shellExecuteList = m_pDrawContext->pPage->GetShellExecuteList();
	for (list<CEqmDrawShellExecute*>::iterator it = shellExecuteList.begin();it != shellExecuteList.end(); it++)
	{
		pShellExecute = *it;
		if(pShellExecute)
		{
			if ( pShellExecute->IsSelected() || !pShellExecute->GetShow()|| pShellExecute->GetLayer() != layer
				|| pShellExecute->GetDeleteFlag() == true)
			{
				continue;
			}
			pShellExecute->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayShellExecute( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawShellExecute*>& shellExecuteList = pDrawElement->pPage->GetShellExecuteList();

	for (list<CEqmDrawShellExecute*>::iterator it = shellExecuteList.begin();it != shellExecuteList.end(); it++)
	{
		CEqmDrawShellExecute* pShellExecute = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pShellExecute->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pShellExecute->GetWidth(), pShellExecute->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawEmbededPage( UINT layer )
{
	CEqmDrawEmbededPage* pEmbededPage = NULL;
	list<CEqmDrawEmbededPage*>& embededpageList = m_pDrawContext->pPage->GetEmbededPageList();
	for (list<CEqmDrawEmbededPage*>::iterator it = embededpageList.begin();it != embededpageList.end(); it++)
	{
		pEmbededPage = *it;
		if(pEmbededPage)
		{
			if ( pEmbededPage->IsSelected() || !pEmbededPage->GetShow()|| pEmbededPage->GetLayer() != layer
				|| pEmbededPage->GetDeleteFlag() == true)
			{
				continue;
			}
			pEmbededPage->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayEmbededPage( DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect )
{
	list<CEqmDrawEmbededPage*>& embededPageList = pDrawElement->pPage->GetEmbededPageList();

	for (list<CEqmDrawEmbededPage*>::iterator it = embededPageList.begin();it != embededPageList.end(); it++)
	{
		CEqmDrawEmbededPage* pEmbededPage = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pEmbededPage->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pEmbededPage->GetWidth(), pEmbededPage->GetHeight());
		if( (*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()] )
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawLiquidLevel(UINT layer)
{
	CEqmDrawLiquidLevel* pLiquidLevel = NULL;
	list<CEqmDrawLiquidLevel*>& liquidLevelList = m_pDrawContext->pPage->GetLiquidLevelList();
	for (list<CEqmDrawLiquidLevel*>::iterator it = liquidLevelList.begin();it != liquidLevelList.end(); it++)
	{
		pLiquidLevel = *it;
		if (pLiquidLevel != NULL)
		{
			if (pLiquidLevel->IsSelected() || !pLiquidLevel->GetShow()|| pLiquidLevel->GetLayer() != layer
				|| pLiquidLevel->GetDeleteFlag() == true)
			{
				continue;
			}
			pLiquidLevel->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayLiquidLevel(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawLiquidLevel*>& liquidLevelList = pDrawElement->pPage->GetLiquidLevelList();

	for (list<CEqmDrawLiquidLevel*>::iterator it=liquidLevelList.begin();it!=liquidLevelList.end(); it++)
	{
		CEqmDrawLiquidLevel* pLiquidLevel = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pLiquidLevel->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pLiquidLevel->GetWidth(), pLiquidLevel->GetHeight());
		if((*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()])
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawCheckButtonPlus(UINT layer)
{
	CEqmDrawButtonPlus* pChkBtnPlus = NULL;
	list<CEqmDrawButtonPlus*>& chkBtnPlusList = m_pDrawContext->pPage->GetCheckButtonPlusList();
	for (list<CEqmDrawButtonPlus*>::iterator it = chkBtnPlusList.begin();it != chkBtnPlusList.end(); it++)
	{
		pChkBtnPlus = *it;
		if (pChkBtnPlus != NULL)
		{
			if (pChkBtnPlus->IsSelected() || !pChkBtnPlus->GetShow()|| pChkBtnPlus->GetLayer() != layer
				|| pChkBtnPlus->GetDeleteFlag() == true)
			{
				continue;
			}
			pChkBtnPlus->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayCheckButtonPlus(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawButtonPlus*>& chkBtnPlusList = pDrawElement->pPage->GetCheckButtonPlusList();

	for (list<CEqmDrawButtonPlus*>::iterator it=chkBtnPlusList.begin();it!=chkBtnPlusList.end(); it++)
	{
		CEqmDrawButtonPlus* pChkBtnPlus = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pChkBtnPlus->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pChkBtnPlus->GetWidth(), pChkBtnPlus->GetHeight());
		if((*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()])
		{
			rItemArray.push_back(*it);
		}
	}
}

void DrawProxy::DrawScrawl(UINT layer)
{
	CEqmDrawScrawl* pScrawl = NULL;
	list<CEqmDrawScrawl*>& scrawlList = m_pDrawContext->pPage->GetScrawlList();
	for (list<CEqmDrawScrawl*>::iterator it = scrawlList.begin();it != scrawlList.end(); it++)
	{
		pScrawl = *it;
		if (pScrawl != NULL)
		{
			if (pScrawl->IsSelected() || !pScrawl->GetShow()|| pScrawl->GetLayer() != layer
				|| pScrawl->GetDeleteFlag() == true)
			{
				continue;
			}
			pScrawl->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayScrawl(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawScrawl*>& scrawlList = pDrawElement->pPage->GetScrawlList();

	for (list<CEqmDrawScrawl*>::iterator it=scrawlList.begin();it!=scrawlList.end(); it++)
	{
		CEqmDrawScrawl* pScrawl = *it;
		if((*it)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt =  pScrawl->GetPos();
		const Gdiplus::RectF rectItem(pt.X,pt.Y, pScrawl->GetWidth(), pScrawl->GetHeight());
		if((*it)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*it)->GetLayer()])
		{
			rItemArray.push_back(*it);
		}
	}
}

Gdiplus::RectF GetRectFrom2Points(float x0, float y0, float x1, float y1)
{
	float x = 0, y = 0, nWidth = 0, nHeight = 0;
	x = x0< x1 ? x0 : x1;
	y = y0< y1 ? y0: y1;
	nWidth  = fabs(x0 - x1);
	nHeight = fabs(y0 - y1);
	Gdiplus::RectF rect(x, y, nWidth, nHeight);
	return rect;
}

void DrawProxy::DrawHistoryTrend(UINT layer)
{
	list<CEqmDrawHistoryTrend*>& listHistoryTrend = m_pDrawContext->pPage->GetHistoryTrendList();
	list<CEqmDrawHistoryTrend*>::const_iterator	iter;

	for (iter=listHistoryTrend.begin(); iter!=listHistoryTrend.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayHistoryTrend(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawHistoryTrend*>& listHistoryTrend = pDrawElement->pPage->GetHistoryTrendList();
	list<CEqmDrawHistoryTrend*>::const_iterator	iter;

	for (iter=listHistoryTrend.begin(); iter!=listHistoryTrend.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawEnergySaveRoi(UINT layer)
{
	list<CEqmDrawEnergySaveRoi*>& listEnergySaveRoi = m_pDrawContext->pPage->GetEnergySaveRoiList();
	list<CEqmDrawEnergySaveRoi*>::const_iterator iter;

	for (iter=listEnergySaveRoi.begin(); iter!=listEnergySaveRoi.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayEnergySaveRoi(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawRectangle*>& listEnergySaveRoi = pDrawElement->pPage->GetRectangleList();
	list<CEqmDrawRectangle*>::const_iterator	iter;

	for (iter=listEnergySaveRoi.begin(); iter!=listEnergySaveRoi.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawMapNavigate(UINT layer)
{
	list<CEqmDrawMapNavigate*>& listMapNavigate = m_pDrawContext->pPage->GetMapNavigateList();
	list<CEqmDrawMapNavigate*>::const_iterator iter;

	for (iter=listMapNavigate.begin(); iter!=listMapNavigate.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayMapNavigate(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawMapNavigate*>& listMapNavigate = pDrawElement->pPage->GetMapNavigateList();
	list<CEqmDrawMapNavigate*>::const_iterator	iter;

	for (iter=listMapNavigate.begin(); iter!=listMapNavigate.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawNavigateWindow(UINT layer)
{
	list<CEqmDrawNavigateWindow*>& listNaviWind = m_pDrawContext->pPage->GetNavigateWindowList();
	list<CEqmDrawNavigateWindow*>::const_iterator iter;

	for (iter=listNaviWind.begin(); iter!=listNaviWind.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayNavigateWindow(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawNavigateWindow*>& listNaviWind = pDrawElement->pPage->GetNavigateWindowList();
	list<CEqmDrawNavigateWindow*>::const_iterator	iter;

	for (iter=listNaviWind.begin(); iter!=listNaviWind.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawLevelRuler(UINT layer)
{
	list<CEqmDrawLevelRuler*>& listLevelRuler = m_pDrawContext->pPage->GetLevelRulerList();
	list<CEqmDrawLevelRuler*>::const_iterator iter;

	for (iter=listLevelRuler.begin(); iter!=listLevelRuler.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayLevelRuler(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawLevelRuler*>& listLevelRuler = pDrawElement->pPage->GetLevelRulerList();
	list<CEqmDrawLevelRuler*>::const_iterator	iter;

	for (iter=listLevelRuler.begin(); iter!=listLevelRuler.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawDataAnalyse(UINT layer)
{
	list<CEqmDrawDataAnalyse*>& listDataAnalyse = m_pDrawContext->pPage->GetDataAnalyseList();
	list<CEqmDrawDataAnalyse*>::const_iterator iter;

	for (iter=listDataAnalyse.begin(); iter!=listDataAnalyse.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayDataAnalyse(DrawContext* pDrawElement, std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawDataAnalyse*>& listDataAnalyse = pDrawElement->pPage->GetDataAnalyseList();
	list<CEqmDrawDataAnalyse*>::const_iterator	iter;

	for (iter=listDataAnalyse.begin(); iter!=listDataAnalyse.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawTemperDistri(UINT layer)
{
	list<CEqmDrawTemperDistri*>& listContain = m_pDrawContext->pPage->GetTemperDistriList();
	list<CEqmDrawTemperDistri*>::const_iterator iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayTemperDistri(DrawContext* pDrawElement, std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawTemperDistri*>& listContain = pDrawElement->pPage->GetTemperDistriList();
	list<CEqmDrawTemperDistri*>::const_iterator	iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawDataReport(UINT layer)
{
	list<CEqmDrawDataReport*>& listContain = m_pDrawContext->pPage->GetDataReportList();
	list<CEqmDrawDataReport*>::const_iterator iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayDataReport(DrawContext* pDrawElement, std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawDataReport*>& listContain = pDrawElement->pPage->GetDataReportList();
	list<CEqmDrawDataReport*>::const_iterator	iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawPlaneTempeDistr(UINT layer)
{
	list<CEqmDrawPlaneTempeDistr*>& listContain = m_pDrawContext->pPage->GetPlaneTempeDistrList();
	list<CEqmDrawPlaneTempeDistr*>::const_iterator iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayPlaneTempeDistr(DrawContext* pDrawElement, std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawPlaneTempeDistr*>& listContain = pDrawElement->pPage->GetPlaneTempeDistrList();
	list<CEqmDrawPlaneTempeDistr*>::const_iterator	iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawDottedLineFrm(UINT layer)
{
	list<CEqmDrawDottedLineFrm*>& listContain = m_pDrawContext->pPage->GetDottedLineFrmList();
	list<CEqmDrawDottedLineFrm*>::const_iterator iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				||(*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayDottedLineFrm(DrawContext* pDrawElement, std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawDottedLineFrm*>& listContain = pDrawElement->pPage->GetDottedLineFrmList();
	list<CEqmDrawDottedLineFrm*>::const_iterator	iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::DrawGeneralComponent(UINT layer)
{
	list<CEqmDrawGeneralComponent*>& listContain = m_pDrawContext->pPage->GetGeneralComponentList();
	list<CEqmDrawGeneralComponent*>::const_iterator iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}
void DrawProxy::DrawRectangle(UINT layer)
{
	list<CEqmDrawRectangle*>& listContain = m_pDrawContext->pPage->GetRectangleList();
	list<CEqmDrawRectangle*>::const_iterator iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::DrawStraightLine(UINT layer)
{
	list<CEqmDrawStraightLine*>& listContain = m_pDrawContext->pPage->GetStraightLineList();
	list<CEqmDrawStraightLine*>::const_iterator iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			if ((*iter)->IsSelected() || !(*iter)->GetShow() || (*iter)->GetLayer() != layer
				|| (*iter)->GetDeleteFlag() == true)
			{
				continue;
			}
			(*iter)->Draw(m_pGC, NULL);	
		}
	}
}

void DrawProxy::PickObjectArrayGeneralComponent(DrawContext* pDrawElement, std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawGeneralComponent*>& listContain = pDrawElement->pPage->GetGeneralComponentList();
	list<CEqmDrawGeneralComponent*>::const_iterator	iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::PickObjectArrayRectangle(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawRectangle*>& listContain = pDrawElement->pPage->GetRectangleList();
	list<CEqmDrawRectangle*>::const_iterator	iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}

void DrawProxy::PickObjectArrayStraightLine(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect)
{
	list<CEqmDrawStraightLine*>& listContain = pDrawElement->pPage->GetStraightLineList();
	list<CEqmDrawStraightLine*>::const_iterator	iter;

	for (iter=listContain.begin(); iter!=listContain.end(); ++iter)
	{
		if((*iter)->GetDeleteFlag()){
			continue;
		}
		const Gdiplus::PointF pt = (*iter)->GetPos();
		const Gdiplus::RectF rectItem(pt.X, pt.Y, (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->IfMeContainRect(rectSelect, rectItem)  && m_bShow[(*iter)->GetLayer()])
		{
			rItemArray.push_back(*iter);
		}
	}
}
