
#include "stdafx.h"
#include "EqmDrawGraphicLine.h"

//////////////////////////////////////////////////////////////////////////
CEqmDrawGraphicLine::CEqmDrawGraphicLine()
{
}

CEqmDrawGraphicLine::CEqmDrawGraphicLine(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos,
                                         int pageid, int ident, int linecolor, int filledcolor, 
                                         int linewidth, int linestype, const string& pointlist,
                                         int forwardpageid,  double transparency, int width /*= 0*/,
                                         int height /*= 0*/ )
                                         :CEqmDrawGraphic(type, centerpos, pageid, ident,linecolor, filledcolor,
                                         linewidth,  linestype,  pointlist,
                                         forwardpageid,   transparency,  width , height  )	


{
    ParsePointListFromString(pointlist, m_pointlist);
}


CEqmDrawGraphicLine::~CEqmDrawGraphicLine()
{
}
//////////////////////////////////////////////////////////////////////////

int CEqmDrawGraphicLine::GetWidth() const
{
    int width = 0;

    int minX, maxX;
    minX = maxX = (int)m_pointlist[0].X;
    for ( UINT nIndex = 1; nIndex < m_pointlist.size(); nIndex++ )
    {
        minX = static_cast<int>(minX > m_pointlist[nIndex].X ? m_pointlist[nIndex].X : minX);
        maxX = static_cast<int>(maxX > m_pointlist[nIndex].X ? maxX : m_pointlist[nIndex].X);
    }
    width = maxX - minX;

    return width;
}

int CEqmDrawGraphicLine::GetHeight() const
{
    int height = 0;

    int minY, maxY;
    minY = maxY = static_cast<int>(m_pointlist[0].Y);
    for ( UINT nIndex = 1; nIndex < m_pointlist.size(); nIndex++ )
    {
        minY = static_cast<int>(minY > m_pointlist[nIndex].Y ? m_pointlist[nIndex].Y : minY);
        maxY = static_cast<int>(maxY > m_pointlist[nIndex].Y ? maxY : m_pointlist[nIndex].Y);
    }
    height = maxY - minY;

    return height;
}
//////////////////////////////////////////////////////////////////////////
void CEqmDrawGraphicLine::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap )
{
    Gdiplus::Color color(static_cast<BYTE>(255*(1-m_dTransparent)), GetRValue(m_linecolor), GetGValue(m_linecolor), GetBValue(m_linecolor));
    Gdiplus::Pen pen(color, static_cast<Gdiplus::REAL>(m_linewidth));//init the pen color
    pen.SetDashStyle((Gdiplus::DashStyle)m_linestyle);
    pen.SetAlignment(Gdiplus::PenAlignmentInset);	

    Gdiplus::PointF *pointList = new Gdiplus::PointF[(int)m_pointlist.size()];  //init the point list
    for ( unsigned int uiIndex = 0; uiIndex < m_pointlist.size(); uiIndex++ )
    {
        pointList[uiIndex].X = m_pointlist[uiIndex].X;
        pointList[uiIndex].Y = m_pointlist[uiIndex].Y;
    }

    pGraphics->DrawLines(&pen, pointList, (int)m_pointlist.size());

    delete []pointList;
}

bool CEqmDrawGraphicLine::PointInMe(const  Gdiplus::PointF& actPt ) const
{
    if ( m_pointlist.size() == 1 )
    {
        return false;
    }
    for ( unsigned int uiIndex = 1; uiIndex < m_pointlist.size(); uiIndex++ )
    {

        if ( m_pointlist[uiIndex-1].X == m_pointlist[uiIndex].X )// 如果是条竖线
        {

            if ( actPt.X <= m_pointlist[uiIndex].X + m_linewidth/2 + SELECTOFFSET/2 &&
                actPt.X >= m_pointlist[uiIndex].X - m_linewidth/2 - SELECTOFFSET/2 &&
                ((actPt.Y <= m_pointlist[uiIndex].Y &&
                actPt.Y >= m_pointlist[uiIndex-1].Y &&
                m_pointlist[uiIndex-1].Y < m_pointlist[uiIndex].Y)||
                (actPt.Y <= m_pointlist[uiIndex-1].Y &&
                actPt.Y >= m_pointlist[uiIndex].Y &&
                m_pointlist[uiIndex-1].Y > m_pointlist[uiIndex].Y)))
            {
                return true;
            }
        }
        else                                                      //如果是条横线
        {
            if ( actPt.Y <= m_pointlist[uiIndex].Y + m_linewidth/2 + SELECTOFFSET/2 &&
                actPt.Y >= m_pointlist[uiIndex].Y - m_linewidth/2 - SELECTOFFSET/2 &&
                ((actPt.X <= m_pointlist[uiIndex].X &&
                actPt.X >= m_pointlist[uiIndex-1].X &&
                m_pointlist[uiIndex].X > m_pointlist[uiIndex-1].X)||
                (actPt.X <= m_pointlist[uiIndex-1].X &&
                actPt.X >= m_pointlist[uiIndex].X &&
                m_pointlist[uiIndex].X < m_pointlist[uiIndex-1].X)))
            {
                return true;
            }
        }
    }

    return false;
}

void CEqmDrawGraphicLine::DrawPort( Gdiplus::Graphics *pGraphics )
{
    //if ( GraphicType_LINE == GetEqmType() )
    Gdiplus::Color color(RGB(255, 0, 0));
    Gdiplus::Pen pen(color, 2.f);
    int nWidth = 2, nHeight = 2;
    for ( unsigned int uiIndex = 0; uiIndex < m_pointlist.size(); uiIndex++ )
    {
        Gdiplus::Rect rc1(static_cast<INT>(m_pointlist[uiIndex].X-nWidth/2), static_cast<INT>(m_pointlist[uiIndex].Y-nHeight/2), nWidth, nHeight);
        pGraphics->DrawArc(&pen,rc1,0,360);
    }

}

void CEqmDrawGraphicLine::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
{
    Gdiplus::Pen pen(SELECTPENCOLOR, 1);
    Gdiplus::HatchBrush *pHatchBrush = new Gdiplus::HatchBrush(Gdiplus::HatchStyleCross, BRUSHCOLOR, BRUSHCOLOR);
    if( NULL == pHatchBrush )
        return;
    pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pointlist[0].X-RECTSIZE/2, 
        m_pointlist[0].Y-RECTSIZE/2,RECTSIZE,RECTSIZE));
    for ( unsigned int uiIndex = 1; uiIndex < m_pointlist.size(); uiIndex++ )
    {
        pGraphics->DrawRectangle(&pen,
            Gdiplus::RectF(m_pointlist[uiIndex-1].X + (m_pointlist[uiIndex].X-m_pointlist[uiIndex-1].X)/2-RECTSIZE/2,
            m_pointlist[uiIndex-1].Y+(m_pointlist[uiIndex].Y-m_pointlist[uiIndex-1].Y)/2-RECTSIZE/2,RECTSIZE,RECTSIZE));
        pGraphics->DrawRectangle(&pen,
            Gdiplus::RectF(m_pointlist[uiIndex].X-RECTSIZE/2,m_pointlist[uiIndex].Y-RECTSIZE/2,RECTSIZE,RECTSIZE));
    }

    if ( IsMainAlign() )
    {
        pGraphics->FillRectangle(pHatchBrush, Gdiplus::RectF(m_pointlist[0].X-RECTSIZE/2,
            m_pointlist[0].Y-RECTSIZE/2,RECTSIZE,RECTSIZE));
        for ( unsigned int uiIndex = 1; uiIndex < m_pointlist.size(); uiIndex++ )
        {
            pGraphics->FillRectangle(pHatchBrush,
                Gdiplus::RectF(m_pointlist[uiIndex-1].X+(m_pointlist[uiIndex].X-m_pointlist[uiIndex-1].X)/2-RECTSIZE/2,                  m_pointlist[uiIndex-1].Y+(m_pointlist[uiIndex].Y-m_pointlist[uiIndex-1].Y)/2-RECTSIZE/2,RECTSIZE,                        RECTSIZE));

            pGraphics->FillRectangle(pHatchBrush, Gdiplus::RectF(m_pointlist[uiIndex].X-RECTSIZE/2,
                m_pointlist[uiIndex].Y-RECTSIZE/2,RECTSIZE,RECTSIZE));
        }
    }

    SAFE_DELETE(pHatchBrush);
}
//////////////////////////////////////////////////////////////////////////
void CEqmDrawGraphicLine::UpdateCenterPos() const
{
    CEqmDrawGraphicLine* pThis = (CEqmDrawGraphicLine*)this;

    int minX, maxX, minY, maxY;
    minX = maxX = static_cast<int>(m_pointlist[0].X);
    minY = maxY = static_cast<int>(m_pointlist[0].Y);
    for ( UINT nIndex = 1; nIndex < m_pointlist.size(); nIndex++ )
    {
        minX = static_cast<int>(minX > m_pointlist[nIndex].X ? m_pointlist[nIndex].X : minX);
        maxX = static_cast<int>(maxX > m_pointlist[nIndex].X ? maxX : m_pointlist[nIndex].X);
        minY = static_cast<int>(minY > m_pointlist[nIndex].Y ? m_pointlist[nIndex].Y : minY);
        maxY = static_cast<int>(maxY > m_pointlist[nIndex].Y ? maxY : m_pointlist[nIndex].Y);
    }
    pThis->m_pos.X = static_cast<Gdiplus::REAL>(minX + (maxX - minX)/2);
    pThis->m_pos.Y = static_cast<Gdiplus::REAL>(minY + (maxY - minY)/2);
}

const Gdiplus::PointF& CEqmDrawGraphicLine::GetPos() const
{
    UpdateCenterPos();
    
    return m_pos;
}



void CEqmDrawGraphicLine::SetPos( const Gdiplus::PointF& centerpos )
{
    Gdiplus::PointF pt = GetPos();
    float offsetX = pt.X - centerpos.X;
    float offsetY = pt.Y - centerpos.Y;
    for ( UINT nIndex = 0; nIndex < m_pointlist.size(); nIndex++ )
    {
        m_pointlist[nIndex].X -= offsetX;
        m_pointlist[nIndex].Y -= offsetY;
    }
	m_bNeedSave = true;
    m_pos = centerpos;
}

bool CEqmDrawGraphicLine::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
{
	if ( Dir_None == dir )
	{
		return false;
	}

	if ( m_pointcount == 1 )
	{
		switch ( dir )
		{
		case Dir_Left:
		case Dir_Right:
			m_pointlist[m_pointindex].X = actPt.X;
			break;
		case Dir_Up:
		case Dir_Down:
			m_pointlist[m_pointindex].Y = actPt.Y;
			break;
		}
	}
	else if ( m_pointcount == 2 )
	{
		switch ( dir )
		{
		case Dir_Left:
		case Dir_Right:
			m_pointlist[m_pointindex].X   = actPt.X;
			m_pointlist[m_pointindex+1].X = actPt.X;
			break;
		case Dir_Up:
		case Dir_Down:
			m_pointlist[m_pointindex].Y   = actPt.Y;
			m_pointlist[m_pointindex+1].Y = actPt.Y;
			break;
		}
	}
	m_bNeedSave = true;
	return true;
}

Direction CEqmDrawGraphicLine::PointIsSide( const Gdiplus::PointF& actPt )
{
	if ( m_pointlist.size() <= 1 ){
		return Dir_None;
	}

	//if the mouse choose the first point of the line
	if ( actPt.X <= m_pointlist[0].X + m_linewidth &&
		actPt.X >= m_pointlist[0].X - m_linewidth &&
		actPt.Y <= m_pointlist[0].Y + m_linewidth &&
		actPt.Y >= m_pointlist[0].Y - m_linewidth )  
	{
		m_pointindex = 0;
		m_pointcount = 1;
		if ( m_pointlist[0].X == m_pointlist[1].X ){
			return Dir_Up;
		}
		else{
			return Dir_Left;
		}
	}

	//if the mouse choose the last point of the line
	int nPointCount = (int)m_pointlist.size();
	if ( actPt.X <= m_pointlist[nPointCount-1].X + m_linewidth &&
		actPt.X >= m_pointlist[nPointCount-1].X - m_linewidth &&
		actPt.Y <= m_pointlist[nPointCount-1].Y + m_linewidth &&
		actPt.Y >= m_pointlist[nPointCount-1].Y - m_linewidth)    
	{
		m_pointindex = nPointCount - 1;
		m_pointcount = 1;
		if ( m_pointlist[nPointCount-1].X == m_pointlist[nPointCount-2].X )		{
			return Dir_Up;
		}
		else{			
			return Dir_Left;
		}
	}

	for ( unsigned int uiIndex = 1; uiIndex < m_pointlist.size(); uiIndex++ )
	{
		if ( m_pointlist[uiIndex].X == m_pointlist[uiIndex-1].X )
		{
			Gdiplus::PointF centerPt(m_pointlist[uiIndex].X, m_pointlist[uiIndex-1].Y + (m_pointlist[uiIndex].Y-m_pointlist[uiIndex-1].Y)/2);
			if ( actPt.X <= centerPt.X + m_linewidth/2.0 + DEFAULT_SIDE_INTERNAL &&
				actPt.X >= centerPt.X - m_linewidth/2.0 - DEFAULT_SIDE_INTERNAL &&
				actPt.Y <= centerPt.Y + DEFAULT_SIDE_INTERNAL &&
				actPt.Y >= centerPt.Y - DEFAULT_SIDE_INTERNAL)
			{
				m_pointindex = (uiIndex -1 );
				m_pointcount = 2;
				return Dir_Left;
			}
		}
		else
		{
			Gdiplus::PointF centerPt(m_pointlist[uiIndex-1].X + (m_pointlist[uiIndex].X - m_pointlist[uiIndex-1].X)/2, m_pointlist[uiIndex].Y);
			if ( actPt.X <= centerPt.X + DEFAULT_SIDE_INTERNAL &&
				actPt.X >= centerPt.X - DEFAULT_SIDE_INTERNAL &&
				actPt.Y <= centerPt.Y + m_linewidth/2.0 + DEFAULT_SIDE_INTERNAL &&
				actPt.Y >= centerPt.Y - m_linewidth/2.0 - DEFAULT_SIDE_INTERNAL)
			{
				m_pointindex = (uiIndex -1 );
				m_pointcount = 2;
				return Dir_Up;
			}
		}
	}

	return Dir_None;
}

CEqmDrawGraphicLine* CEqmDrawGraphicLine::Copy()
{
	m_bNeedSave = true;
	return new CEqmDrawGraphicLine(*this);
}