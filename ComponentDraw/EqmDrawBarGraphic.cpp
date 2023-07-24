#include "StdAfx.h"
#include "EqmDrawBarGraphic.h"
#include <cassert>

CEqmDrawBarGraphic::CEqmDrawBarGraphic(void):
m_pBitmap(NULL),
m_nUpdateInterval(2000),
m_nType(eBarNormal),
m_dMaxValue(0.f),
m_colorText(RGB(0,0,0))
{
	m_nscale = 1.0;
	m_base_height = 0;
	m_base_width = 0;
	m_nQueryType = 4;
	//m_dScale = (double)m_nWidth/(double)m_nHeight;
}


CEqmDrawBarGraphic::~CEqmDrawBarGraphic(void)
{
}

void CEqmDrawBarGraphic::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/ )
{
	const EQMDRAWTYPE picType = GetEqmType();
	if (picType == EQMDRAW_UNKNOWN)
	{
		return;
	}
	if (m_pBitmap)
	{
		m_base_width  = m_pBitmap->GetWidth();
		m_base_height = m_pBitmap->GetHeight();
		pGraphics->DrawImage(m_pBitmap, m_pos.X, m_pos.Y, (Gdiplus::REAL)GetWidth(), (Gdiplus::REAL)GetHeight());
	}
	return;
}

CEqmDrawBarGraphic* CEqmDrawBarGraphic::Copy()
{
	m_bNeedSave = true;
	return new CEqmDrawBarGraphic(*this);
}

bool CEqmDrawBarGraphic::PointInMe( const Gdiplus::PointF& actPt ) const
{
	if ( actPt.X <= m_pos.X + GetWidth() &&
		actPt.X >= m_pos.X &&
		actPt.Y <= m_pos.Y + GetHeight() &&
		actPt.Y >= m_pos.Y)
	{
		return true;
	}
	return false;
}

Direction CEqmDrawBarGraphic::PointIsSide( const Gdiplus::PointF& actPt )
{
	if ( actPt.X <= m_pos.X &&
		actPt.X >= m_pos.X-DEFAULT_SIDE_INTERNAL &&
		actPt.Y <= m_pos.Y+GetHeight()+DEFAULT_SIDE_INTERNAL/2 &&
		actPt.Y >= m_pos.Y-DEFAULT_SIDE_INTERNAL/2)
	{
		return Dir_Left;
	}
	else if ( actPt.X <= m_pos.X+GetWidth()+DEFAULT_SIDE_INTERNAL &&
		actPt.X >= m_pos.X+GetWidth() &&
		actPt.Y <= m_pos.Y+GetHeight()+DEFAULT_SIDE_INTERNAL/2 &&
		actPt.Y >= m_pos.Y-DEFAULT_SIDE_INTERNAL/2)
	{
		return Dir_Right;
	}
	else if ( actPt.X <= m_pos.X+GetWidth()+DEFAULT_SIDE_INTERNAL/2&&
		actPt.X >= m_pos.X-DEFAULT_SIDE_INTERNAL/2&&
		actPt.Y <= m_pos.Y&&
		actPt.Y >= m_pos.Y-DEFAULT_SIDE_INTERNAL )
	{
		return Dir_Up;
	}
	else if ( actPt.X <= m_pos.X+GetWidth()+DEFAULT_SIDE_INTERNAL/2&&
		actPt.X >= m_pos.X-DEFAULT_SIDE_INTERNAL/2&&
		actPt.Y <= m_pos.Y+GetHeight()+DEFAULT_SIDE_INTERNAL&&
		actPt.Y >= m_pos.Y+GetHeight() )
	{
		return Dir_Down;
	}
	else if ( actPt.X <= m_pos.X&&
		actPt.X >= m_pos.X-DEFAULT_SIDE_INTERNAL&&
		actPt.Y <= m_pos.Y&&
		actPt.Y >= m_pos.Y-DEFAULT_SIDE_INTERNAL )
	{
		return Dir_LeftUp;
	}
	else if ( actPt.X <= m_pos.X&&
		actPt.X >= m_pos.X-DEFAULT_SIDE_INTERNAL&&
		actPt.Y <= m_pos.Y+GetHeight()+DEFAULT_SIDE_INTERNAL&&
		actPt.Y >= m_pos.Y+GetHeight() )
	{
		return Dir_LeftDown;
	}
	else if ( actPt.X <= m_pos.X+GetWidth()+DEFAULT_SIDE_INTERNAL&&
		actPt.X >= m_pos.X+GetWidth()&&
		actPt.Y <= m_pos.Y&&
		actPt.Y >= m_pos.Y-DEFAULT_SIDE_INTERNAL  )
	{
		return Dir_RightUp;
	}
	else if ( actPt.X <= m_pos.X+GetWidth()+DEFAULT_SIDE_INTERNAL&&
		actPt.X >= m_pos.X+GetWidth()&&
		actPt.Y <= m_pos.Y+GetHeight()+DEFAULT_SIDE_INTERNAL&&
		actPt.Y >= m_pos.Y+GetHeight() )
	{
		return Dir_RightDown;
	}
	else
		return Dir_None;
}

void CEqmDrawBarGraphic::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
{
	Gdiplus::Pen pen(SELECTPENCOLOR, 1);

	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X-RECTSIZE,
		m_pos.Y-RECTSIZE,RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X-RECTSIZE,
		m_pos.Y+GetHeight(),RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X+GetWidth(),
		m_pos.Y+GetHeight(),RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X+GetWidth(),
		m_pos.Y-RECTSIZE,RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X-RECTSIZE,
		m_pos.Y+GetHeight()/2-RECTSIZE/2,RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X+GetWidth()/2-RECTSIZE/2,
		m_pos.Y+GetHeight(),RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X+GetWidth(),
		m_pos.Y+GetHeight()/2-RECTSIZE/2,RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X+GetWidth()/2-RECTSIZE/2,
		m_pos.Y-RECTSIZE,RECTSIZE,RECTSIZE));
	if ( m_bIsMainAlign )
	{     
		Gdiplus::HatchBrush hatchBrush(Gdiplus::HatchStyleCross, MAINSELECT_BRUSHCOLOR, MAINSELECT_BRUSHCOLOR);

		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X-RECTSIZE,
			m_pos.Y-RECTSIZE,RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X-RECTSIZE,
			m_pos.Y+GetHeight(),RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X+GetWidth(),
			m_pos.Y+GetHeight(),RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X+GetWidth(),
			m_pos.Y-RECTSIZE,RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X-RECTSIZE,
			m_pos.Y+GetHeight()/2-RECTSIZE/2,RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X+GetWidth()/2-RECTSIZE/2,
			m_pos.Y+GetHeight(),RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X+GetWidth(),
			m_pos.Y+GetHeight()/2-RECTSIZE/2,RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X+GetWidth()/2-RECTSIZE/2,
			m_pos.Y-RECTSIZE,RECTSIZE,RECTSIZE));
	}
}

bool CEqmDrawBarGraphic::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
{
	if(m_dScale == 0)
	{
		m_dScale = (double)m_nWidth/(double)m_nHeight;
	}
	switch ( dir )
	{
	case Dir_Left:
		{
			if(actPt.X < m_posru.X)
			{
				m_pos.X = actPt.X;
				m_posld.X = actPt.X;
				m_nWidth = m_posru.X - actPt.X;
				m_dScale = (double)m_nWidth/(double)m_nHeight;
			}
			break;
		}
	case Dir_Right:
		if(actPt.X > m_pos.X)
		{
			m_nWidth = abs(actPt.X-m_pos.X);
			m_posrd.X = actPt.X;
			m_posru.X = actPt.X;
			m_dScale = (double)m_nWidth/(double)m_nHeight;
		}
		break;
	case Dir_Up:
		{
			if(actPt.Y < m_posld.Y)
			{
				m_pos.Y = actPt.Y;
				m_posru.Y = actPt.Y;
				m_nHeight = m_posld.Y - actPt.Y;
				m_dScale = (double)m_nWidth/(double)m_nHeight;
			}
			break;
		}
	case Dir_Down:
		if(actPt.Y > m_pos.Y)
		{
			m_nHeight = abs(actPt.Y-m_pos.Y);
			m_posld.Y = actPt.Y;
			m_posrd.Y = actPt.Y;
			m_dScale = (double)m_nWidth/(double)m_nHeight;
		}

		break;
	case Dir_LeftUp:
		{
			if(actPt.X < m_posrd.X && actPt.Y < m_posrd.Y)
			{
				m_nWidth = abs(actPt.X - m_posrd.X);
				m_nHeight = m_nWidth/m_dScale;

				m_pos.X = m_posrd.X - m_nWidth;
				m_pos.Y = m_posrd.Y - m_nHeight;

				m_posru.X = m_posrd.X;
				m_posru.Y = m_posrd.Y - m_nHeight;

				m_posld.X = m_posrd.X - m_nWidth;
				m_posld.Y = m_posrd.Y;
			}
			break;
		}
	case Dir_LeftDown:
		{
			if(actPt.X < m_posru.X && actPt.Y > m_posru.Y)
			{
				m_nWidth = abs(actPt.X - m_posru.X);
				m_nHeight = m_nWidth/m_dScale;

				m_pos.X = m_posru.X - m_nWidth;
				m_pos.Y = m_posru.Y;

				m_posld.X = m_posru.X - m_nWidth;
				m_posld.Y = m_posru.Y + m_nHeight;

				m_posrd.X = m_posru.X;
				m_posrd.Y = m_posru.Y + m_nHeight;
			}
			break;
		}
	case Dir_RightUp:
		{
			if(actPt.X > m_posld.X && actPt.Y < m_posld.Y)
			{
				m_nWidth = abs(actPt.X - m_posld.X);
				m_nHeight = m_nWidth/m_dScale;

				m_pos.X = m_posld.X;
				m_pos.Y = m_posld.Y - m_nHeight;

				m_posru.X = m_posld.X + m_nWidth;
				m_posru.Y = m_posld.Y - m_nHeight;

				m_posrd.X = m_posld.X + m_nWidth;
				m_posrd.Y = m_posld.Y;
			}
			break;
		}
	case Dir_RightDown:
		{
			if(actPt.X > m_pos.X && actPt.Y > m_pos.Y)
			{
				m_nWidth = abs(actPt.X - m_pos.X);
				m_nHeight = m_nWidth/m_dScale;

				m_posru.X = m_pos.X + m_nWidth;
				m_posru.Y = m_pos.Y;

				m_posrd.X = m_pos.X + m_nWidth;
				m_posrd.Y = m_pos.Y + m_nHeight;

				m_posld.X = m_pos.X;
				m_posld.Y = m_pos.Y + m_nHeight;
			}
			break;
		}
	default:
		break;
	}
	m_bNeedSave = true;
	return true;
}

bool CEqmDrawBarGraphic::CopyProperty( CEqmDrawBase *pBase )
{
	return false;
}

void CEqmDrawBarGraphic::SetDefaultBitmap( Image* pBitmap )
{
	m_pBitmap = pBitmap;
}

bool CEqmDrawBarGraphic::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawBarGraphic::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawBarGraphic::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawBarGraphic::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

void CEqmDrawBarGraphic::SetTitleName( const CString strName )
{
	m_strTitleName = strName;
	m_bNeedSave = true;
}

const CString CEqmDrawBarGraphic::GetTitleName()
{
	return m_strTitleName;
}

void CEqmDrawBarGraphic::SetUpdateInterval( const int nUpdateInterval )
{
	m_nUpdateInterval = nUpdateInterval;
	m_bNeedSave = true;
}

const int CEqmDrawBarGraphic::GetUpdateInterval()
{
	return m_nUpdateInterval;
}

void CEqmDrawBarGraphic::SetXName( const CString strName )
{
	m_strXName = strName;
	m_bNeedSave = true;
}

const CString CEqmDrawBarGraphic::GetXName()
{
	return m_strXName;
}

void CEqmDrawBarGraphic::SetXUnit( const CString strXUnit )
{
	m_strXUnit = strXUnit;
	m_bNeedSave = true;
}

const CString CEqmDrawBarGraphic::GetXUnit()
{
	return m_strXUnit;
}

void CEqmDrawBarGraphic::InsertGraphItem( _Graph_Item_Property item )
{
	m_vecItem.push_back(item);
	m_bNeedSave = true;
}

void CEqmDrawBarGraphic::DeleteGraphItem( const CString strItemName )
{
	vector<_Graph_Item_Property>::iterator it = m_vecItem.begin();
	while (it != m_vecItem.end())
	{
		if ((*it).strItemName == strItemName)
		{
			it = m_vecItem.erase(it);
			m_bNeedSave = true;
		}
		else
		{
			++it;
		}
	}
}

_Graph_Item_Property* CEqmDrawBarGraphic::GetGraphItem( const int nIndex )
{
	ASSERT(nIndex>=0 && nIndex<(int)m_vecItem.size());

	return &m_vecItem[nIndex];
}

_Graph_Item_Property* CEqmDrawBarGraphic::GetGraphItem( const CString strItemName )
{
	const size_t size = m_vecItem.size();
	for (size_t i=0;i<size;++i)
	{
		if (m_vecItem[i].strItemName == strItemName)
		{
			return &m_vecItem[i];
		}
	}
	return NULL;
}

void CEqmDrawBarGraphic::ClearGraphItem()
{
	m_vecItem.clear();
	m_bNeedSave = true;
}

void CEqmDrawBarGraphic::SetYName( const CString strName )
{
	m_strYName = strName;
	m_bNeedSave = true;
}

const CString CEqmDrawBarGraphic::GetYName()
{
	return m_strYName;
}

vector<_Graph_Item_Property>& CEqmDrawBarGraphic::GetItemVector()
{
	return m_vecItem;
}

void CEqmDrawBarGraphic::SetType( const E_BAR_TYPE eType )
{
	m_nType = eType;
	m_bNeedSave = true;
}

const E_BAR_TYPE CEqmDrawBarGraphic::GetType()
{
	return m_nType;
}

void CEqmDrawBarGraphic::SetMaxValue( const double dMaxValue )
{
	m_dMaxValue = dMaxValue;
	m_bNeedSave = true;
}

const double CEqmDrawBarGraphic::GetMaxValue()
{
	return m_dMaxValue;
}

void CEqmDrawBarGraphic::SetStartPoint( const CString strPoint )
{
	m_strStartPoint = strPoint;
}

const CString CEqmDrawBarGraphic::GetStartPoint()
{
	return m_strStartPoint;
}

void CEqmDrawBarGraphic::SetQueryType( const int nType )
{
	m_nQueryType = nType;
	m_bNeedSave = true;
}

const int CEqmDrawBarGraphic::GetQueryType()
{
	return m_nQueryType;
}

bool CEqmDrawBarGraphic::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawBarGraphic::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawBarGraphic::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawBarGraphic::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

void CEqmDrawBarGraphic::SetTextColor( const COLORREF color )
{
	m_colorText = color;
}

const COLORREF CEqmDrawBarGraphic::GetTextColor()
{
	return m_colorText;
}

bool CEqmDrawBarGraphic::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawBarGraphic* pDest = (CEqmDrawBarGraphic*)pBase;
		if(pDest)
		{
			SetDefaultBitmap(pDest->GetDefaultBitmap());
			SetTitleName(pDest->GetTitleName());
			SetUpdateInterval(pDest->GetUpdateInterval());
			SetXName(pDest->GetXName());
			SetXUnit(pDest->GetXUnit());

			SetYName(pDest->GetYName());
			SetItemVector(pDest->GetItemVector());
			SetType(pDest->GetType());
			SetMaxValue(pDest->GetMaxValue());

			SetStartPoint(pDest->GetStartPoint());
			SetQueryType(pDest->GetQueryType());
			SetTextColor(pDest->GetTextColor());
		}
		return true;
	}
	return false;
}

void CEqmDrawBarGraphic::SetItemVector( vector<_Graph_Item_Property>& vec )
{
	m_vecItem = vec;
}

Image* CEqmDrawBarGraphic::GetDefaultBitmap()
{
	return m_pBitmap;
}
