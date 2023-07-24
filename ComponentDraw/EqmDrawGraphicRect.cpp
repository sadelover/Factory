
#include "stdafx.h"
#include "EqmDrawGraphicRect.h"

#include "Tools/ColorDefines.h"
#include "Tools/CustomTools/CustomTools.h"


//////////////////////////////////////////////////////////////////////////
#define RECTBORDEREFAULTCOLOR	RGB_BLACK	//文本框默认字体颜色
CEqmDrawGraphicRect::CEqmDrawGraphicRect()
:CEqmDrawGraphic()
,m_rectborderIs3D(0)
{

}

CEqmDrawGraphicRect::~CEqmDrawGraphicRect()
{
}

CEqmDrawGraphicRect::CEqmDrawGraphicRect(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos,
										 int pageid, int ident, int linecolor, int filledcolor, 
										 int linewidth, int linestype, const string& pointlist,
										 int forwardpageid,  double transparency, int width /*= 0*/,
										 int height /*= 0*/,int ifborder )
										 :CEqmDrawGraphic(type, centerpos, pageid, ident,linecolor, filledcolor,
										 linewidth,  linestype,  pointlist,
										 forwardpageid,   transparency,  width , height, ifborder)	
										 ,m_rectborderIs3D(ifborder)

{
	ParsePointListFromString(pointlist, m_pointlist);
}

//////////////////////////////////////////////////////////////////////////

int CEqmDrawGraphicRect::GetWidth() const
{
	const int width = (int)abs(m_pointlist[0].X - m_pointlist[1].X);    
	return width;
}

int CEqmDrawGraphicRect::GetHeight() const
{
	const int height = (int)abs(m_pointlist[0].Y - m_pointlist[1].Y);        
	return height;
}
//////////////////////////////////////////////////////////////////////////

void CEqmDrawGraphicRect::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap )
{
	int x = 0, y = 0, nWidth = 0, nHeight = 0;
	x = m_pointlist[0].X< m_pointlist[1].X ? (int)m_pointlist[0].X:(int)m_pointlist[1].X;
	y = m_pointlist[0].Y<m_pointlist[1].Y ? (int)m_pointlist[0].Y: (int)m_pointlist[1].Y; 
	nWidth  = (int)abs(m_pointlist[0].X - m_pointlist[1].X);
	nHeight = (int)abs(m_pointlist[0].Y - m_pointlist[1].Y);
	
		Gdiplus::Color color(255, GetRValue(m_linecolor), GetGValue(m_linecolor), GetBValue(m_linecolor));
		Gdiplus::Pen pen(color, (float)m_linewidth);//init the pen color
		pen.SetDashStyle((Gdiplus::DashStyle)m_linestyle);
		pen.SetAlignment(Gdiplus::PenAlignmentInset);
		pGraphics->DrawRectangle(&pen, x, y, nWidth, nHeight );

	if ( NONEFILLCOLOR != m_filledcolor ){
		Gdiplus::Color fillColor(GetRValue(m_filledcolor), GetGValue(m_filledcolor), GetBValue(m_filledcolor));
		Gdiplus::HatchBrush	pHatchBrush(Gdiplus::HatchStyleCross, fillColor, fillColor);
		pGraphics->FillRectangle(&pHatchBrush, x+m_linewidth, y+m_linewidth, nWidth-2*m_linewidth, nHeight-2*m_linewidth );
	}
}

bool CEqmDrawGraphicRect::PointInMe(const Gdiplus::PointF& actPt ) const
{
	if ( m_pointlist.size() == 1 ){
		return false;
	}
	else
	{
		int x = 0, y = 0, nWidth = 0, nHeight = 0;
		x = m_pointlist[0].X<m_pointlist[1].X ?(int)m_pointlist[0].X: (int)m_pointlist[1].X;
		y = m_pointlist[0].Y<m_pointlist[1].Y ? (int)m_pointlist[0].Y: (int)m_pointlist[1].Y;
		nWidth  = (int)abs(m_pointlist[0].X - m_pointlist[1].X);
		nHeight = (int)abs(m_pointlist[0].Y - m_pointlist[1].Y);
		const int lineHW = m_linewidth/2;
		const bool ifleft =  (actPt.X <= x+ lineHW + SELECTOFFSET && 
			actPt.X >= x- lineHW-SELECTOFFSET&&
			actPt.Y <= y+nHeight+ lineHW +SELECTOFFSET&&
			actPt.Y >= y- lineHW -SELECTOFFSET);  //是否选中矩形左边
		const bool ifright = (actPt.X <= x+nWidth+ lineHW +SELECTOFFSET&&
			actPt.X >= x+nWidth-lineHW -SELECTOFFSET&&
			actPt.Y <= y+nHeight+ lineHW +SELECTOFFSET&&
			actPt.Y >= y- lineHW -SELECTOFFSET) ; //是否选中矩形右边
		const bool iftop = (actPt.X <= x+nWidth+ lineHW +SELECTOFFSET&&
			actPt.X >= x- lineHW -SELECTOFFSET&&
			actPt.Y <= y+lineHW +SELECTOFFSET&&
			actPt.Y >= y-lineHW -SELECTOFFSET);  //是否选中矩形上边
		const bool ifBottom = (actPt.X <= x+nWidth+ lineHW +SELECTOFFSET&&
			actPt.X >= x- lineHW -SELECTOFFSET&&
			actPt.Y <= y+nHeight+ lineHW +SELECTOFFSET&&
			actPt.Y >= y+nHeight-lineHW -SELECTOFFSET);
		if (ifleft
			|| ifright
			|| iftop
			|| ifBottom
			)
		{
			return true;
		}
		return false;
	}
}

Direction CEqmDrawGraphicRect::PointIsSide(const Gdiplus::PointF& actPt ) 
{

	{
		int x = 0, y = 0, nWidth = 0, nHeight = 0;
		x = m_pointlist[0].X<m_pointlist[1].X ? (int)m_pointlist[0].X: (int)m_pointlist[1].X;
		y = m_pointlist[0].Y<m_pointlist[1].Y ? (int)m_pointlist[0].Y: (int)m_pointlist[1].Y;
		nWidth  = (int)abs(m_pointlist[0].X - m_pointlist[1].X);
		nHeight = (int)abs(m_pointlist[0].Y - m_pointlist[1].Y);
		const int sideInternalHW = DEFAULT_SIDE_INTERNAL/2;
		if ( actPt.X <= x + sideInternalHW &&
			actPt.X >= x - sideInternalHW &&
			actPt.Y <= y+nHeight/2+ sideInternalHW &&
			actPt.Y >= y+nHeight/2- sideInternalHW )
		{
			return Dir_Left;
		}
		else if ( actPt.X <= x+nWidth + sideInternalHW &&
			actPt.X >= x+nWidth - sideInternalHW &&
			actPt.Y <= y+nHeight/2+ sideInternalHW &&
			actPt.Y >= y+nHeight/2- sideInternalHW )
		{
			return Dir_Right;
		}
		else if ( actPt.X <= x+nWidth/2+ sideInternalHW &&
			actPt.X >= x+nWidth/2-sideInternalHW &&
			actPt.Y <= y + sideInternalHW&&
			actPt.Y >= y-sideInternalHW )
		{
			return Dir_Up;
		}
		else if ( actPt.X <= x+nWidth/2+ sideInternalHW &&
			actPt.X >= x+nWidth/2-sideInternalHW &&
			actPt.Y <= y+nHeight+ sideInternalHW &&
			actPt.Y >= y+nHeight- sideInternalHW )
		{
			return Dir_Down;
		}
		else if ( actPt.X <= x +sideInternalHW &&
			actPt.X >= x-sideInternalHW &&
			actPt.Y <= y + sideInternalHW &&
			actPt.Y >= y-sideInternalHW )
		{
			return Dir_LeftUp;
		}
		else if ( actPt.X <= x + sideInternalHW &&
			actPt.X >= x-sideInternalHW &&
			actPt.Y <= y+nHeight+sideInternalHW &&
			actPt.Y >= y+nHeight- sideInternalHW )
		{
			return Dir_LeftDown;
		}
		else if ( actPt.X <= x+nWidth+sideInternalHW &&
			actPt.X >= x+nWidth- sideInternalHW &&
			actPt.Y <= y + sideInternalHW &&
			actPt.Y >= y-sideInternalHW  )
		{
			return Dir_RightUp;
		}
		else if ( actPt.X <= x+nWidth+sideInternalHW &&
			actPt.X >= x+nWidth- sideInternalHW &&
			actPt.Y <= y+nHeight+sideInternalHW &&
			actPt.Y >= y+nHeight- sideInternalHW )
		{
			return Dir_RightDown;
		}
		else
			return Dir_None;
	}
	return Dir_None;
}

void CEqmDrawGraphicRect::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
{
	int x = 0, y = 0, nWidth = 0, nHeight = 0;
	x = m_pointlist[0].X<m_pointlist[1].X ? (int)m_pointlist[0].X: (int)m_pointlist[1].X;
	y = m_pointlist[0].Y<m_pointlist[1].Y ? (int)m_pointlist[0].Y: (int)m_pointlist[1].Y;

	nWidth  = (int)abs(m_pointlist[0].X - m_pointlist[1].X);
	nHeight = (int)abs(m_pointlist[0].Y - m_pointlist[1].Y);

	Gdiplus::Pen pen(SELECTPENCOLOR, 1);
	Gdiplus::HatchBrush pHatchBrush(Gdiplus::HatchStyleCross, MAINSELECT_BRUSHCOLOR, MAINSELECT_BRUSHCOLOR);

	const int rectHW = RECTSIZE/2;

	const Gdiplus::Rect rectTopLeft(x- rectHW, y- rectHW, RECTSIZE, RECTSIZE);
	const Gdiplus::Rect rectLeftDown(x- rectHW, y + nHeight - rectHW, RECTSIZE, RECTSIZE);
	const Gdiplus::Rect rectRightDown(x+nWidth - rectHW,y+nHeight- rectHW, RECTSIZE, RECTSIZE);
	const Gdiplus::Rect rectRightTop(x+nWidth - rectHW, y-rectHW, RECTSIZE,RECTSIZE);
	const Gdiplus::Rect rectLeftMidHeight(x-rectHW, y+ nHeight/2- rectHW, RECTSIZE,RECTSIZE);
	const Gdiplus::Rect rectDownMidX(x+nWidth/2- rectHW, y+nHeight -rectHW, RECTSIZE,RECTSIZE);
	const Gdiplus::Rect rectRightMidY(x+nWidth- rectHW, y+nHeight/2-rectHW, RECTSIZE,RECTSIZE);
	const Gdiplus::Rect rectTopMidX(x+nWidth/2-rectHW, y-rectHW, RECTSIZE,RECTSIZE);

	if ( IsMainAlign() )
	{
		pGraphics->FillRectangle(&pHatchBrush, rectTopLeft);
		pGraphics->FillRectangle(&pHatchBrush, rectLeftDown);
		pGraphics->FillRectangle(&pHatchBrush, rectRightDown);
		pGraphics->FillRectangle(&pHatchBrush, rectRightTop);
		pGraphics->FillRectangle(&pHatchBrush, rectLeftMidHeight);
		pGraphics->FillRectangle(&pHatchBrush, rectDownMidX);
		pGraphics->FillRectangle(&pHatchBrush, rectRightMidY);
		pGraphics->FillRectangle(&pHatchBrush,rectTopMidX);
	}
	else{
		pGraphics->DrawRectangle(&pen, rectTopLeft);  //top ,left
		pGraphics->DrawRectangle(&pen, rectLeftDown);
		pGraphics->DrawRectangle(&pen, rectRightDown);
		pGraphics->DrawRectangle(&pen, rectRightTop);
		pGraphics->DrawRectangle(&pen, rectLeftMidHeight);
		pGraphics->DrawRectangle(&pen, rectDownMidX);
		pGraphics->DrawRectangle(&pen, rectRightMidY);
		pGraphics->DrawRectangle(&pen, rectTopMidX);
	}
}

bool CEqmDrawGraphicRect::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
{
	switch ( dir )
	{
	case Dir_Left:
		ChangeSizeLeft(actPt);        
		break;
	case Dir_Right:
		ChangeSizeRight(actPt);
		break;
	case Dir_Up:
		ChangeSizeUp(actPt);
		break;
	case Dir_Down:
		ChangeSizeDown(actPt);
		break;
	case Dir_LeftUp:
		ChangeSizeLeft(actPt);
		ChangeSizeUp(actPt);
		break;
	case Dir_LeftDown:
		ChangeSizeDown(actPt);
		ChangeSizeLeft(actPt);
		break;
	case Dir_RightUp:
		ChangeSizeRight(actPt);
		ChangeSizeUp(actPt);
		break;
	case Dir_RightDown:
		ChangeSizeRight(actPt);
		ChangeSizeDown(actPt);
		break;
	case Dir_None:
		break;
	default:
		//ASSERT(false);
		break;
	}
	m_bNeedSave = true;
	return true;
}

void CEqmDrawGraphicRect::ChangeSizeLeft(const Gdiplus::PointF& actPt)
{
	m_pointlist[0].X = actPt.X;
	m_bNeedSave = true;
}
void CEqmDrawGraphicRect::ChangeSizeRight(const Gdiplus::PointF& actPt)
{
	m_pointlist[1].X = actPt.X;
	m_bNeedSave = true;
}
void CEqmDrawGraphicRect::ChangeSizeUp(const Gdiplus::PointF& actPt)
{
	if ( m_pointlist[0].Y >= m_pointlist[1].Y )
		m_pointlist[1].Y = actPt.Y;
	else
		m_pointlist[0].Y = actPt.Y;
	m_bNeedSave = true;
}
void CEqmDrawGraphicRect::ChangeSizeDown(const Gdiplus::PointF& actPt)
{
	if ( m_pointlist[0].Y >= m_pointlist[1].Y )
		m_pointlist[0].Y = actPt.Y;
	else
		m_pointlist[1].Y = actPt.Y;
	m_bNeedSave = true;
}

const Gdiplus::PointF& CEqmDrawGraphicRect::GetPos() const
{
	UpdateCenterPos();

	return m_pos;
}

void CEqmDrawGraphicRect::UpdateCenterPos() const
{
	CEqmDrawGraphicRect* pThis = (CEqmDrawGraphicRect*)this;
	pThis->m_pos.X = m_pointlist[0].X + (m_pointlist[1].X - m_pointlist[0].X)/2;
	pThis->m_pos.Y = m_pointlist[0].Y + (m_pointlist[1].Y - m_pointlist[0].Y)/2;
}

void CEqmDrawGraphicRect::SetPos( const Gdiplus::PointF& centerpos )
{
	float offsetX = 0.0, offSetY = 0.0;
	m_pos.X = m_pointlist[0].X + (m_pointlist[1].X - m_pointlist[0].X)/2;
	m_pos.Y = m_pointlist[0].Y + (m_pointlist[1].Y - m_pointlist[0].Y)/2;
	offsetX = m_pos.X - centerpos.X;
	offSetY = m_pos.Y - centerpos.Y;
	for ( UINT  nIndex = 0; nIndex < m_pointlist.size(); nIndex++)
	{
		m_pointlist[nIndex].X -= offsetX;
		m_pointlist[nIndex].Y -= offSetY;
	}
	m_pos = centerpos;
	m_bNeedSave = true;
}
int CEqmDrawGraphicRect::GetRectBorderIs3D() const
{
	return m_rectborderIs3D;
}
void CEqmDrawGraphicRect::SetRectBorderIs3D(int  borderis3d)
{
	m_rectborderIs3D = borderis3d;
	m_bNeedSave = true;
}
CEqmDrawGraphicRect* CEqmDrawGraphicRect::Copy()
{
	m_bNeedSave = true;
	return new CEqmDrawGraphicRect(*this);
}

bool CEqmDrawGraphicRect::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//特有属性

		CEqmDrawGraphicRect* pDest = (CEqmDrawGraphicRect*)pBase;
		if(pDest)
		{
			SetRectBorderIs3D(pDest->GetRectBorderIs3D());
		}
		return true;
	}
	return false;
}

