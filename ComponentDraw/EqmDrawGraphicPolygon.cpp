
#include "stdafx.h"
#include "EqmDrawGraphicPolygon.h"

//////////////////////////////////////////////////////////////////////////
CEqmDrawGraphicPolygon::CEqmDrawGraphicPolygon()
{
	m_pPolygonPen = NULL;
	m_pBKBrush = NULL;
}

CEqmDrawGraphicPolygon::CEqmDrawGraphicPolygon(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos,
                                               int pageid, int ident, int linecolor, int filledcolor, 
                                               int linewidth, int linestype, const string& pointlist,
                                               int forwardpageid,  double transparency, int width /*= 0*/,
                                               int height /*= 0*/ )
                                               :CEqmDrawGraphic(type, centerpos, pageid, ident,linecolor, filledcolor,
                                               linewidth,  linestype,  pointlist,
                                               forwardpageid,   transparency,  width , height  )	


{
	m_pPolygonPen = NULL;
	m_pBKBrush = NULL;
    ParsePointListFromString(pointlist, m_pointlist);
}


CEqmDrawGraphicPolygon::~CEqmDrawGraphicPolygon()
{
	DestoryObjects();
}
//////////////////////////////////////////////////////////////////////////

void CEqmDrawGraphicPolygon::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap )
{

}

//////////////////////////////////////////////////////////////////////////
bool CEqmDrawGraphicPolygon::PointInMe(const Gdiplus::PointF& actPt ) const
{
    return false;
}

CEqmDrawGraphicPolygon* CEqmDrawGraphicPolygon::Copy()
{
	m_bNeedSave = true;
	return  new CEqmDrawGraphicPolygon(*this);
}

void CEqmDrawGraphicPolygon::CreateObjects()
{
	if (m_pPolygonPen == NULL)
	{
		Gdiplus::Color colorPen;
		colorPen.SetFromCOLORREF(m_linecolor);
		m_pPolygonPen = new Gdiplus::Pen(colorPen,(Gdiplus::REAL)m_linewidth);
		assert(m_pPolygonPen);
		if (m_linestyle == 0)
		{
			m_pPolygonPen->SetDashStyle(Gdiplus::DashStyleSolid);
		}
		else
		{
			m_pPolygonPen->SetDashStyle(Gdiplus::DashStyleDash);
		}
	}
	if (m_pBKBrush == NULL)
	{
		m_pBKBrush = new Gdiplus::SolidBrush(Gdiplus::Color((BYTE)(255*(1-m_dTransparent)), GetRValue(m_filledcolor), GetGValue(m_filledcolor), GetBValue(m_filledcolor)));
		assert(m_pBKBrush);
	}
}

void CEqmDrawGraphicPolygon::DestoryObjects()
{
	if (m_pPolygonPen)
	{
		delete m_pPolygonPen;
		m_pPolygonPen = NULL;
	}
	if (m_pBKBrush)
	{
		delete m_pBKBrush;
		m_pBKBrush = NULL;
	}
}







