#include "StdAfx.h"
#include "EqmDrawLineGraphic.h"
#include <cassert>

CEqmDrawLineGraphic::CEqmDrawLineGraphic(void):
m_pBitmap(NULL),
m_nUpdateInterval(2000),
m_nType(eBarNormal),
m_dMaxValue(0.f),
m_dMinValue(0.f),
m_nMaxCheck(1),
m_nMinCheck(1),
m_colorText(RGB(0,0,0))
{
	m_nscale = 1.0;
	m_base_height = 0;
	m_base_width = 0;
	//m_dScale = (double)m_nWidth/(double)m_nHeight;
}


CEqmDrawLineGraphic::~CEqmDrawLineGraphic(void)
{
}

void CEqmDrawLineGraphic::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/ )
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

CEqmDrawLineGraphic* CEqmDrawLineGraphic::Copy()
{
	m_bNeedSave = true;
	return new CEqmDrawLineGraphic(*this);
}

bool CEqmDrawLineGraphic::PointInMe( const Gdiplus::PointF& actPt ) const
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

Direction CEqmDrawLineGraphic::PointIsSide( const Gdiplus::PointF& actPt )
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

void CEqmDrawLineGraphic::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
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

bool CEqmDrawLineGraphic::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
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

bool CEqmDrawLineGraphic::CopyProperty( CEqmDrawBase *pBase )
{
	return false;
}

void CEqmDrawLineGraphic::SetDefaultBitmap( Image* pBitmap )
{
	assert(pBitmap);
	m_pBitmap = pBitmap;
}

bool CEqmDrawLineGraphic::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawLineGraphic::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawLineGraphic::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawLineGraphic::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

void CEqmDrawLineGraphic::SetTitleName( const CString strName )
{
	m_strTitleName = strName;
	m_bNeedSave = true;
}

const CString CEqmDrawLineGraphic::GetTitleName()
{
	return m_strTitleName;
}

void CEqmDrawLineGraphic::SetUpdateInterval( const int nUpdateInterval )
{
	m_nUpdateInterval = nUpdateInterval;
	m_bNeedSave = true;
}

const int CEqmDrawLineGraphic::GetUpdateInterval()
{
	return m_nUpdateInterval;
}

void CEqmDrawLineGraphic::SetXName( const CString strName )
{
	m_strXName = strName;
	m_bNeedSave = true;
}

const CString CEqmDrawLineGraphic::GetXName()
{
	return m_strXName;
}

void CEqmDrawLineGraphic::SetXUnit( const CString strXUnit )
{
	m_strXUnit = strXUnit;
	m_bNeedSave = true;
}

const CString CEqmDrawLineGraphic::GetXUnit()
{
	return m_strXUnit;
}

void CEqmDrawLineGraphic::InsertGraphItem( _Graph_Item_Property item )
{
	m_vecItem.push_back(item);
	m_bNeedSave = true;
}

void CEqmDrawLineGraphic::DeleteGraphItem( const CString strItemName )
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

_Graph_Item_Property* CEqmDrawLineGraphic::GetGraphItem( const int nIndex )
{
	ASSERT(nIndex>=0 && nIndex<(int)m_vecItem.size());

	return &m_vecItem[nIndex];
}

_Graph_Item_Property* CEqmDrawLineGraphic::GetGraphItem( const CString strItemName )
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

void CEqmDrawLineGraphic::ClearGraphItem()
{
	m_vecItem.clear();
	m_bNeedSave = true;
}

void CEqmDrawLineGraphic::SetYName( const CString strName )
{
	m_strYName = strName;
	m_bNeedSave = true;
}

const CString CEqmDrawLineGraphic::GetYName()
{
	return m_strYName;
}

vector<_Graph_Item_Property>& CEqmDrawLineGraphic::GetItemVector()
{
	return m_vecItem;
}

void CEqmDrawLineGraphic::SetType( const E_BAR_TYPE eType )
{
	m_nType = eType;
	m_bNeedSave = true;
}

const E_BAR_TYPE CEqmDrawLineGraphic::GetType()
{
	return m_nType;
}

void CEqmDrawLineGraphic::SetMaxValue( const double dValue )
{
	m_dMaxValue = dValue;
	m_bNeedSave = true;
}

const double CEqmDrawLineGraphic::GetMaxValue()
{
	return m_dMaxValue;
}

bool CEqmDrawLineGraphic::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawLineGraphic::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawLineGraphic::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawLineGraphic::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

void CEqmDrawLineGraphic::SetTextColor( const COLORREF color )
{
	m_colorText = color;
	m_bNeedSave = true;
}

const COLORREF CEqmDrawLineGraphic::GetTextColor()
{
	return m_colorText;
}

void CEqmDrawLineGraphic::SetMinValue( const double dValue )
{
	m_dMinValue = dValue;
	m_bNeedSave = true;
}

const double CEqmDrawLineGraphic::GetMinValue()
{
	return m_dMinValue;
}

void CEqmDrawLineGraphic::SetMaxCheck( const int nCheck )
{
	m_nMaxCheck = nCheck;
	m_bNeedSave = true;
}

const int CEqmDrawLineGraphic::GetMaxCheck()
{
	return m_nMaxCheck;
}

void CEqmDrawLineGraphic::SetMinCheck( const int nCheck )
{
	m_nMinCheck = nCheck;
	m_bNeedSave = true;
}

const int CEqmDrawLineGraphic::GetMinCheck()
{
	return m_nMinCheck;
}

Image* CEqmDrawLineGraphic::GetDefaultBitmap()
{
	return m_pBitmap;
}

bool CEqmDrawLineGraphic::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawLineGraphic* pDest = (CEqmDrawLineGraphic*)pBase;
		if(pDest)
		{
			SetDefaultBitmap(pDest->GetDefaultBitmap());
			SetTitleName(pDest->GetTitleName());
			SetUpdateInterval(pDest->GetUpdateInterval());
			SetXName(pDest->GetXName());
			SetXUnit(pDest->GetXUnit());

			SetYName(pDest->GetYName());
			SetType(pDest->GetType());
			SetMaxValue(pDest->GetMaxValue());
			SetMinValue(pDest->GetMinValue());

			SetTextColor(pDest->GetTextColor());
			SetMaxCheck(pDest->GetMaxCheck());
			SetMinCheck(pDest->GetMinCheck());

			SetItemVector(pDest->GetItemVector());
		}
		return true;
	}
	return false;
}

void CEqmDrawLineGraphic::SetItemVector( vector<_Graph_Item_Property>& vec )
{
	m_vecItem = vec;
}
