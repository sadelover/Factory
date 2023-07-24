#include "StdAfx.h"
#include "EqmDrawBend.h"


CEqmDrawBend::CEqmDrawBend(void):	
m_pBitmap(NULL)
,m_base_height(0)
,m_base_width(0)
,m_nscale(0)
,m_nRotateAngle(0)
,m_nSize(14)
{
	//m_dScale = (double)m_nWidth/(double)m_nHeight;
}

CEqmDrawBend::~CEqmDrawBend(void)
{
}

void CEqmDrawBend::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/ )
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
		RotetaBitMap(m_pBitmap);
		pGraphics->DrawImage(m_pBitmap, m_pos.X, m_pos.Y, (Gdiplus::REAL)GetWidth(), (Gdiplus::REAL)GetHeight());
		ResetRotateBmp(m_pBitmap);
	}
	return;
}

CEqmDrawBend* CEqmDrawBend::Copy()
{
	m_bNeedSave = true;
	return new CEqmDrawBend(*this);
}

bool CEqmDrawBend::PointInMe( const Gdiplus::PointF& actPt ) const
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

Direction CEqmDrawBend::PointIsSide( const Gdiplus::PointF& actPt )
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

void CEqmDrawBend::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
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

bool CEqmDrawBend::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
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

bool CEqmDrawBend::CopyProperty( CEqmDrawBase *pBase )
{
	return false;
}

void CEqmDrawBend::SetRotateAngle( const int nRotateAngle )
{
	m_nRotateAngle = nRotateAngle;
	m_bNeedSave = true;
}

const int CEqmDrawBend::GetRotateAngle()
{
	return m_nRotateAngle;
}

void CEqmDrawBend::SetDefaultBitmap( Image* pBitmap )
{
	if(pBitmap)
	{
		m_pBitmap = pBitmap;
	}
}

bool CEqmDrawBend::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawBend::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawBend::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawBend::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

void CEqmDrawBend::RotetaBitMap( Image *pBitMap /*= NULL*/ )
{
	if ( NULL != pBitMap )
	{
		switch ( m_nRotateAngle )
		{
		case _Rotate90FlipNone:
			pBitMap->RotateFlip(Gdiplus::Rotate90FlipNone);
			break;
		case _Rotate180FlipNone:
			pBitMap->RotateFlip(Gdiplus::Rotate180FlipNone);
			break;
		case _Rotate270FlipNone:
			pBitMap->RotateFlip(Gdiplus::Rotate270FlipNone);
			break;
		case _Rotate90FlipXY:
			pBitMap->RotateFlip(Gdiplus::Rotate270FlipNone);
			break;
		case _Rotate180FlipXY:
			pBitMap->RotateFlip(Gdiplus::Rotate180FlipNone);
			break;
		case _Rotate270FlipXY:
			pBitMap->RotateFlip(Gdiplus::Rotate90FlipNone);
			break;
		default:
			break;
		}
		m_bNeedSave = true;
	}
}

void CEqmDrawBend::ResetRotateBmp(Image *pBitMap )
{
	if(!pBitMap) return;

	switch ( m_nRotateAngle )
	{
	case _Rotate90FlipNone:
		pBitMap->RotateFlip(Gdiplus::Rotate270FlipNone);
		break;
	case _Rotate180FlipNone:
		pBitMap->RotateFlip(Gdiplus::Rotate180FlipNone);
		break;
	case _Rotate270FlipNone:
		pBitMap->RotateFlip(Gdiplus::Rotate90FlipNone);
		break;
	case _Rotate90FlipXY:
		pBitMap->RotateFlip(Gdiplus::Rotate90FlipNone);
		break;
	case _Rotate180FlipXY:
		pBitMap->RotateFlip(Gdiplus::Rotate180FlipNone);
		break;
	case _Rotate270FlipXY:
		pBitMap->RotateFlip(Gdiplus::Rotate270FlipNone);
		break;
	default:
		break;
	}
}

bool CEqmDrawBend::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawBend::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawBend::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawBend::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawBend::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawBend* pDest = (CEqmDrawBend*)pBase;
		if(pDest)
		{
			SetRotateAngle(pDest->GetRotateAngle());
		}
		return true;
	}
	return false;
}

bool CEqmDrawBend::LeftRotate90()
{
	int temp	= GetRotateAngle() - 90;
	int width	= GetWidth();
	int height	= GetHeight();
	float rotAngle = (temp == -360) ? 0 : temp;

	SetRotateAngle(rotAngle);
	SetWidth(height);
	SetHeight(width);
	m_bNeedSave = true;

	return true;
}

bool CEqmDrawBend::RightRotate90()
{
	int temp	= GetRotateAngle() + 90;
	int width	= GetWidth();
	int height	= GetHeight();
	float rotAngle = (temp == 360) ? 0 : temp;

	SetRotateAngle(rotAngle);
	SetWidth(height);
	SetHeight(width);
	m_bNeedSave = true;

	return true;
}

void CEqmDrawBend::SetSize(const int nSize)
{
	m_nSize = nSize;
}

const int CEqmDrawBend::GetSize(void) const
{
	return m_nSize;
}
