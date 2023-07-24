
#include "stdafx.h"
#include "EqmDrawGraphic.h"
#include "Tools/ColorDefines.h"
#include "Tools/CustomTools/CustomTools.h"
//////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4244)


CEqmDrawGraphic::CEqmDrawGraphic()
{	
	m_linecolor = RGB_BLACK;
	m_linewidth = 1;
	m_linestyle = Line_Solid;
	m_filledcolor = NONEFILLCOLOR;
	m_forwardpageid = -1;
	m_dTransparent = 0;
	m_rectborderIs3D = 0;
}
CEqmDrawGraphic::CEqmDrawGraphic(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos,
								 int pageid, int ident, int linecolor, int filledcolor, 
								 int linewidth, int linestype, const string& pointlist,
								 int forwardpageid,  double transparency, int width /*= 0*/,
								 int height /*= 0*/,int ifborder )
								 :CEqmDrawBase(type, centerpos, pageid, ident, width, height),								
								 m_linecolor(linecolor),
								 m_filledcolor(filledcolor),
								 m_linewidth(linewidth),
								 m_linestyle(linestype),
								 //m_strpointlist(pointlist),
								 m_forwardpageid(forwardpageid),
								 m_dTransparent(transparency),
								 m_rectborderIs3D(ifborder)
{
	ParsePointListFromString(pointlist, m_pointlist);
}

CEqmDrawGraphic::~CEqmDrawGraphic()
{
}


int CEqmDrawGraphic::GetLineColor() const
{
	return m_linecolor;
}

void CEqmDrawGraphic::SetLineColor( int linecolor )
{
	m_linecolor = linecolor;
	m_bNeedSave = true;
}

int CEqmDrawGraphic::GetLineWidth() const
{
	return m_linewidth;
}

void CEqmDrawGraphic::SetLineWidth( int linewidth )
{
	m_linewidth = linewidth;
	m_bNeedSave = true;
}

int CEqmDrawGraphic::GetLineStyle() const
{
	return  m_linestyle;
}

void CEqmDrawGraphic::SetLineStyle( int linestyle )
{
	m_linestyle = linestyle;
	m_bNeedSave = true;
}

void CEqmDrawGraphic::SetFilledColor( int color )
{
	m_filledcolor =  color;
	m_bNeedSave = true;
}

int CEqmDrawGraphic::GetFilledColor() const
{
	return m_filledcolor;
}

string CEqmDrawGraphic::GetPointStringList() const
{
	string str;
	ParsePointListToString(m_pointlist, str);
	return  str;
}

void CEqmDrawGraphic::SetPointStringList( const string& strpointlist )
{
	ParsePointListFromString(strpointlist, m_pointlist);
}

vector<Gdiplus::PointF>& CEqmDrawGraphic::GetPointList()
{
	return m_pointlist;
}

void CEqmDrawGraphic::SetPointList( const vector<Gdiplus::PointF>& pointlist )
{
	m_pointlist = pointlist;
	m_bNeedSave = true;
}

int CEqmDrawGraphic::GetWidth() const
{
	return 0;
}

int CEqmDrawGraphic::GetHeight() const
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////
void CEqmDrawGraphic::SetForwardPageID( int pageid )
{
	m_forwardpageid = pageid;
	m_bNeedSave = true;
}

int CEqmDrawGraphic::GetForwardPageID() const
{
	return m_forwardpageid;
}

void CEqmDrawGraphic::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap )
{
	Gdiplus::Color color((DWORD)(255*(1-m_dTransparent)), GetRValue(m_linecolor), GetGValue(m_linecolor), GetBValue(m_linecolor));
	Gdiplus::Pen pen(color, m_linewidth);//init the pen color
	pen.SetDashStyle((Gdiplus::DashStyle)m_linestyle);
	pen.SetAlignment(Gdiplus::PenAlignmentInset);	
}

void CEqmDrawGraphic::SetTransparent( double dTransparent )
{
	m_dTransparent = dTransparent;
	m_bNeedSave = true;
}

double CEqmDrawGraphic::GetTransparent()
{
	return m_dTransparent;
}

//////////////////////////////////////////////////////////////////////////
bool CEqmDrawGraphic::PointInMe(const Gdiplus::PointF& actPt ) const
{	
	return false;	
}

Direction CEqmDrawGraphic::PointIsSide(const Gdiplus::PointF& actPt ) 
{
	return Dir_None;
}

void CEqmDrawGraphic::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
{
	//nothing
}

bool CEqmDrawGraphic::ChangeSize(const  Gdiplus::PointF& actPt, Direction dir )
{	
	return true;
}

void CEqmDrawGraphic::DrawPort( Gdiplus::Graphics *pGraphics )
{

}

void CEqmDrawGraphic::SetPos( const Gdiplus::PointF& centerpos )
{	
	m_pos = centerpos;
	m_bNeedSave = true;
}

void CEqmDrawGraphic::SetWidth(int width)
{
	switch ( GetEqmType() )
	{
	case GraphicType_RECTANGLE:
		if ( m_pointlist[0].X > m_pointlist[1].X )
			m_pointlist[0].X = m_pointlist[1].X + width;
		else
			m_pointlist[1].X = m_pointlist[0].X + width;
		break;
	case GraphicType_ELLIPSE:
		if ( m_pointlist[0].X > m_pointlist[1].X )
			m_pointlist[0].X = m_pointlist[1].X + width;
		else
			m_pointlist[1].X = m_pointlist[0].X + width;
		break;
	case GraphicType_POLYGON:
		//@todo
		break;
	default:
		break;
	}
	m_bNeedSave = true;
}

void CEqmDrawGraphic::SetHeight(int height)
{
	switch ( GetEqmType() )
	{
	case GraphicType_RECTANGLE:
		if ( m_pointlist[0].Y > m_pointlist[1].Y )
			m_pointlist[0].Y = m_pointlist[1].Y + height;
		else
			m_pointlist[1].Y = m_pointlist[0].Y + height;
		break;
	case GraphicType_ELLIPSE:
		if ( m_pointlist[0].Y > m_pointlist[1].Y )
			m_pointlist[0].Y = m_pointlist[1].Y + height;
		else
			m_pointlist[1].Y = m_pointlist[0].Y + height;
		break;
	case GraphicType_POLYGON://@todo
		break;
	default:
		break;
	}
	m_bNeedSave = true;
}

bool CEqmDrawGraphic::CopyProperty( CEqmDrawBase *pBase )
{
	CEqmDrawGraphic *pGraphic = dynamic_cast<CEqmDrawGraphic*>(pBase);
	if ( pGraphic && m_eEqmType == pGraphic->GetEqmType() )
	{
		m_linecolor   = pGraphic->GetLineColor();			//线的颜色
		m_filledcolor = pGraphic->GetFilledColor();			//填充颜色
		m_linewidth   = pGraphic->GetLineWidth();			//线宽
		m_linestyle   = pGraphic->GetLineStyle();			//线的类型
		m_dTransparent = pGraphic->GetTransparent();
		m_bNeedSave = true;
		return true;
	}
	return false;
}

bool CEqmDrawGraphic::ShiftEqmtsUp()
{
	for ( unsigned int uIndex = 0; uIndex < m_pointlist.size(); uIndex++ )
	{
		m_pointlist[uIndex].X = m_pointlist[uIndex].X;
		m_pointlist[uIndex].Y = m_pointlist[uIndex].Y-MOVEDISTANCEBYKEY;
	}
	m_bNeedSave = true;
	return true;
}

bool CEqmDrawGraphic::ShiftEqmtsDown()
{
	for ( unsigned int uIndex = 0; uIndex < m_pointlist.size(); uIndex++ )
	{
		m_pointlist[uIndex].X = m_pointlist[uIndex].X;
		m_pointlist[uIndex].Y = m_pointlist[uIndex].Y+MOVEDISTANCEBYKEY;
	}
	m_bNeedSave = true;
	return true;
}

bool CEqmDrawGraphic::ShiftEqmtsLeft()
{
	for ( unsigned int uIndex = 0; uIndex < m_pointlist.size(); uIndex++ )
	{
		m_pointlist[uIndex].X = m_pointlist[uIndex].X-MOVEDISTANCEBYKEY;
		m_pointlist[uIndex].Y = m_pointlist[uIndex].Y;
	}
	m_bNeedSave = true;
	return true;
}

bool CEqmDrawGraphic::ShiftEqmtsRight()
{
	for ( unsigned int uIndex = 0; uIndex < m_pointlist.size(); uIndex++ )
	{
		m_pointlist[uIndex].X = m_pointlist[uIndex].X+MOVEDISTANCEBYKEY;
		m_pointlist[uIndex].Y = m_pointlist[uIndex].Y;
	}
	m_bNeedSave = true;
	return true;
}

bool CEqmDrawGraphic::DoMoveEqmt( Gdiplus::PointF prePt, Gdiplus::PointF actPt )
{
	for ( unsigned int uiIndex = 0; uiIndex < m_pointlist.size(); uiIndex++ )
	{
		m_pointlist[uiIndex].X = m_pointlist[uiIndex].X+actPt.X - prePt.X;
		m_pointlist[uiIndex].Y = m_pointlist[uiIndex].Y+actPt.Y - prePt.Y;
	}
	m_bNeedSave = true;
	return true;
}

int CEqmDrawGraphic::GetBorderIs3D()
{
	return m_rectborderIs3D;
}
void CEqmDrawGraphic::SetBorderIs3D(int  borderis3d)
{
	m_rectborderIs3D = borderis3d;
	m_bNeedSave = true;
}

bool CEqmDrawGraphic::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//特有属性

		CEqmDrawGraphic* pDest = (CEqmDrawGraphic*)pBase;
		if(pDest)
		{
			SetLineColor(pDest->GetLineColor());
			SetLineWidth(pDest->GetLineWidth());
			SetLineStyle(pDest->GetLineStyle());
			SetFilledColor(pDest->GetFilledColor());
			SetPointStringList(pDest->GetPointStringList());

			SetPointList(pDest->GetPointList());
			SetWidth(pDest->GetWidth());
			SetHeight(pDest->GetHeight());
			SetForwardPageID(pDest->GetForwardPageID());

			SetTransparent(pDest->GetTransparent());
			SetBorderIs3D(pDest->GetBorderIs3D());
		}
		return true;
	}
	return false;
}
