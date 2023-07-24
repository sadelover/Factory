#include "StdAfx.h"
#include "EqmDrawTimeBar.h"
#include <cassert>

CEqmDrawTimeBar::CEqmDrawTimeBar(void):
m_pBitmap(NULL)
{
	m_nscale = 1.0;
	m_base_height = 0;
	m_base_width = 0;
	m_nColor = RGB(0,0,0);
	m_nQueryType = 4;
	//m_dScale = (double)m_nWidth/(double)m_nHeight;
}

CEqmDrawTimeBar::~CEqmDrawTimeBar(void)
{
}

void CEqmDrawTimeBar::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/ )
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

CEqmDrawTimeBar* CEqmDrawTimeBar::Copy()
{
	m_bNeedSave = true;
	return new CEqmDrawTimeBar(*this);
}

bool CEqmDrawTimeBar::PointInMe( const Gdiplus::PointF& actPt ) const
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

Direction CEqmDrawTimeBar::PointIsSide( const Gdiplus::PointF& actPt )
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

void CEqmDrawTimeBar::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
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

bool CEqmDrawTimeBar::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
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

bool CEqmDrawTimeBar::CopyProperty( CEqmDrawBase *pBase )
{
	return false;
}

void CEqmDrawTimeBar::SetDefaultBitmap( Image* pBitmap )
{
	assert(pBitmap);
	m_pBitmap = pBitmap;
}

bool CEqmDrawTimeBar::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawTimeBar::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawTimeBar::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawTimeBar::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

void CEqmDrawTimeBar::DeleteAllItems()
{
	m_vecItem.clear();
	m_bNeedSave = true;
}

void CEqmDrawTimeBar::InsertItem( const _tag_bar_item item )
{
	m_vecItem.push_back(item);
	m_bNeedSave = true;
}

void CEqmDrawTimeBar::SetBindPointStart( const CString strBindPoint )
{
	m_strBindPointStart = strBindPoint;
	m_bNeedSave = true;
}

const CString CEqmDrawTimeBar::GetBindPointStart()
{
	return m_strBindPointStart;
}

void CEqmDrawTimeBar::SetBarColor( const COLORREF nColor )
{
	m_nColor = nColor;
	m_bNeedSave = true;
}

const COLORREF CEqmDrawTimeBar::GetBarColor()
{
	return m_nColor;
}

const vector<_tag_bar_item>& CEqmDrawTimeBar::GetItemVec()
{
	return m_vecItem;
}

void CEqmDrawTimeBar::SetBindPointStatus( const CString strBindPoint )
{
	m_strBindPointStatus = strBindPoint;
}

const CString CEqmDrawTimeBar::GetBindPointStatus()
{
	return m_strBindPointStatus;
}

void CEqmDrawTimeBar::SetQueryType( const int nType )
{
	m_nQueryType = nType;
	m_bNeedSave = true;
}

const int CEqmDrawTimeBar::GetQueryType()
{
	return m_nQueryType;
}

bool CEqmDrawTimeBar::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawTimeBar::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawTimeBar::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawTimeBar::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawTimeBar::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawTimeBar* pDest = (CEqmDrawTimeBar*)pBase;
		if(pDest)
		{
			SetDefaultBitmap(pDest->GetDefaultBitmap());
			SetBindPointStart(pDest->GetBindPointStart());
			SetBindPointStatus(pDest->GetBindPointStatus());
			SetBarColor(pDest->GetBarColor());
			SetQueryType(pDest->GetQueryType());

			SetItemVec(pDest->GetItemVec());
		}
		return true;
	}
	return false;
}

Image* CEqmDrawTimeBar::GetDefaultBitmap()
{
	return m_pBitmap;
}

void CEqmDrawTimeBar::SetItemVec( const vector<_tag_bar_item>& vec )
{
	m_vecItem = vec;
}
