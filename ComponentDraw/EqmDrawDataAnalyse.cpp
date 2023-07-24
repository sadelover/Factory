#include "StdAfx.h"
#include "EqmDrawDataAnalyse.h"


CEqmDrawDataAnalyse::CEqmDrawDataAnalyse(void)
	: m_pBitmap(NULL)
{
	m_fscale = (double)m_nWidth / (double)m_nHeight;
}


CEqmDrawDataAnalyse::~CEqmDrawDataAnalyse(void)
{
}

CEqmDrawDataAnalyse* CEqmDrawDataAnalyse::Copy(void)
{
	m_bNeedSave = true;
	return new CEqmDrawDataAnalyse(*this);
}

void CEqmDrawDataAnalyse::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/)
{
	const EQMDRAWTYPE picType = GetEqmType();
	if (picType == EQMDRAW_UNKNOWN)
	{
		return;
	}
	if (m_pBitmap != NULL)
	{
		m_nBaseWidth  = m_pBitmap->GetWidth();
		m_nBaseHeight = m_pBitmap->GetHeight();
		pGraphics->DrawImage(m_pBitmap, m_pos.X, m_pos.Y, (Gdiplus::REAL)GetWidth(), (Gdiplus::REAL)GetHeight());
	}
	return;
}

bool CEqmDrawDataAnalyse::PointInMe(const Gdiplus::PointF& actPt) const
{
	if (actPt.X <= m_pos.X + GetWidth()
		&& actPt.X >= m_pos.X
		&& actPt.Y <= m_pos.Y + GetHeight()
		&& actPt.Y >= m_pos.Y)
	{
		return true;
	}
	return false;
}

Direction CEqmDrawDataAnalyse::PointIsSide(const Gdiplus::PointF& actPt)
{
	if (actPt.X <= m_pos.X
		&& actPt.X >= m_pos.X-DEFAULT_SIDE_INTERNAL
		&& actPt.Y <= m_pos.Y + GetHeight() + DEFAULT_SIDE_INTERNAL / 2
		&& actPt.Y >= m_pos.Y - DEFAULT_SIDE_INTERNAL / 2)
	{
		return Dir_Left;
	}
	else if (actPt.X <= m_pos.X + GetWidth() + DEFAULT_SIDE_INTERNAL
		&& actPt.X >= m_pos.X + GetWidth()
		&& actPt.Y <= m_pos.Y + GetHeight() + DEFAULT_SIDE_INTERNAL / 2
		&& actPt.Y >= m_pos.Y - DEFAULT_SIDE_INTERNAL / 2)
	{
		return Dir_Right;
	}
	else if (actPt.X <= m_pos.X + GetWidth() + DEFAULT_SIDE_INTERNAL / 2
		&& actPt.X >= m_pos.X - DEFAULT_SIDE_INTERNAL / 2
		&& actPt.Y <= m_pos.Y
		&& actPt.Y >= m_pos.Y - DEFAULT_SIDE_INTERNAL)
	{
		return Dir_Up;
	}
	else if (actPt.X <= m_pos.X + GetWidth() + DEFAULT_SIDE_INTERNAL / 2
		&& actPt.X >= m_pos.X - DEFAULT_SIDE_INTERNAL / 2
		&& actPt.Y <= m_pos.Y + GetHeight() + DEFAULT_SIDE_INTERNAL
		&& actPt.Y >= m_pos.Y + GetHeight())
	{
		return Dir_Down;
	}
	else if (actPt.X <= m_pos.X
		&& actPt.X >= m_pos.X - DEFAULT_SIDE_INTERNAL
		&& actPt.Y <= m_pos.Y
		&& actPt.Y >= m_pos.Y - DEFAULT_SIDE_INTERNAL)
	{
		return Dir_LeftUp;
	}
	else if (actPt.X <= m_pos.X
		&& actPt.X >= m_pos.X - DEFAULT_SIDE_INTERNAL
		&& actPt.Y <= m_pos.Y + GetHeight() + DEFAULT_SIDE_INTERNAL
		&& actPt.Y >= m_pos.Y + GetHeight())
	{
		return Dir_LeftDown;
	}
	else if (actPt.X <= m_pos.X + GetWidth() + DEFAULT_SIDE_INTERNAL
		&& actPt.X >= m_pos.X + GetWidth()
		&& actPt.Y <= m_pos.Y
		&& actPt.Y >= m_pos.Y - DEFAULT_SIDE_INTERNAL)
	{
		return Dir_RightUp;
	}
	else if (actPt.X <= m_pos.X + GetWidth() + DEFAULT_SIDE_INTERNAL
		&& actPt.X >= m_pos.X + GetWidth()
		&& actPt.Y <= m_pos.Y + GetHeight() + DEFAULT_SIDE_INTERNAL
		&& actPt.Y >= m_pos.Y + GetHeight())
	{
		return Dir_RightDown;
	}
	else
	{
		return Dir_None;
	}
}

void CEqmDrawDataAnalyse::DrawSelectEdge(Gdiplus::Graphics *pGraphics)
{
	Gdiplus::Pen pen(SELECTPENCOLOR, 1);

	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X - RECTSIZE,
		m_pos.Y - RECTSIZE, RECTSIZE, RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X - RECTSIZE,
		m_pos.Y + GetHeight(), RECTSIZE, RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X + GetWidth(),
		m_pos.Y + GetHeight(), RECTSIZE, RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X + GetWidth(),
		m_pos.Y - RECTSIZE, RECTSIZE, RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X - RECTSIZE,
		m_pos.Y + GetHeight() / 2 - RECTSIZE / 2, RECTSIZE, RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X + GetWidth() / 2 - RECTSIZE / 2,
		m_pos.Y + GetHeight(), RECTSIZE, RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X + GetWidth(),
		m_pos.Y + GetHeight() / 2 - RECTSIZE / 2, RECTSIZE, RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X + GetWidth() / 2 - RECTSIZE / 2,
		m_pos.Y - RECTSIZE, RECTSIZE, RECTSIZE));
	if (m_bIsMainAlign)
	{     
		Gdiplus::HatchBrush hatchBrush(Gdiplus::HatchStyleCross, MAINSELECT_BRUSHCOLOR, MAINSELECT_BRUSHCOLOR);

		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X - RECTSIZE,
			m_pos.Y - RECTSIZE, RECTSIZE, RECTSIZE));					   
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X - RECTSIZE,
			m_pos.Y + GetHeight(), RECTSIZE, RECTSIZE));				   
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X + GetWidth(),
			m_pos.Y + GetHeight(), RECTSIZE, RECTSIZE));				   
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X + GetWidth(),
			m_pos.Y - RECTSIZE, RECTSIZE, RECTSIZE));					   
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X - RECTSIZE,
			m_pos.Y + GetHeight() / 2 - RECTSIZE / 2, RECTSIZE, RECTSIZE));	   
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X + GetWidth() / 2 - RECTSIZE / 2,
			m_pos.Y + GetHeight(), RECTSIZE, RECTSIZE));				   
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X + GetWidth(),
			m_pos.Y + GetHeight() / 2 - RECTSIZE / 2, RECTSIZE, RECTSIZE));	   
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X + GetWidth() / 2 - RECTSIZE / 2,
			m_pos.Y - RECTSIZE, RECTSIZE, RECTSIZE));
	}
}

bool CEqmDrawDataAnalyse::ChangeSize(const Gdiplus::PointF& actPt, Direction dir)
{
	if (m_dScale == 0)
	{
		m_dScale = (double)m_nWidth/(double)m_nHeight;
	}
	switch (dir)
	{
	case Dir_Left:
		{
			if (actPt.X < m_posru.X)
			{
				m_pos.X = actPt.X;
				m_posld.X = actPt.X;
				m_nWidth = m_posru.X - actPt.X;
				m_dScale = (double)m_nWidth / (double)m_nHeight;
			}
			break;
		}
	case Dir_Right:
		if (actPt.X > m_pos.X)
		{
			m_nWidth = abs(actPt.X - m_pos.X);
			m_posrd.X = actPt.X;
			m_posru.X = actPt.X;
			m_dScale = (double)m_nWidth / (double)m_nHeight;
		}
		break;
	case Dir_Up:
		{
			if (actPt.Y < m_posld.Y)
			{
				m_pos.Y = actPt.Y;
				m_posru.Y = actPt.Y;
				m_nHeight = m_posld.Y - actPt.Y;
				m_dScale = (double)m_nWidth / (double)m_nHeight;
			}
			break;
		}
	case Dir_Down:
		if (actPt.Y > m_pos.Y)
		{
			m_nHeight = abs(actPt.Y - m_pos.Y);
			m_posld.Y = actPt.Y;
			m_posrd.Y = actPt.Y;
			m_dScale = (double)m_nWidth / (double)m_nHeight;
		}

		break;
	case Dir_LeftUp:
		{
			if (actPt.X < m_posrd.X && actPt.Y < m_posrd.Y)
			{
				m_nWidth = abs(actPt.X - m_posrd.X);
				m_nHeight = m_nWidth / m_dScale;

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
			if (actPt.X < m_posru.X && actPt.Y > m_posru.Y)
			{
				m_nWidth = abs(actPt.X - m_posru.X);
				m_nHeight = m_nWidth / m_dScale;

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
			if (actPt.X > m_posld.X && actPt.Y < m_posld.Y)
			{
				m_nWidth = abs(actPt.X - m_posld.X);
				m_nHeight = m_nWidth / m_dScale;

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
			if (actPt.X > m_pos.X && actPt.Y > m_pos.Y)
			{
				m_nWidth = abs(actPt.X - m_pos.X);
				m_nHeight = m_nWidth / m_dScale;

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

bool CEqmDrawDataAnalyse::CopyProperty(CEqmDrawBase *pBase)
{
	return false;
}

bool CEqmDrawDataAnalyse::ClonePropertyByDest(CEqmDrawBase *pBase)
{
	if (pBase != NULL && m_eEqmType == pBase->GetEqmType())
	{
		CopyBaseProperty(pBase);

		//×¨ÓÐÊôÐÔ

		return true;
	}

	return false;
}

void CEqmDrawDataAnalyse::SetDefaultBitmap(Image* pBitmap)
{
	if (pBitmap != NULL)
	{
		m_pBitmap = pBitmap;
	}
}

Image* CEqmDrawDataAnalyse::GetDefaultBitmap()
{
	return m_pBitmap;
}

bool CEqmDrawDataAnalyse::ShiftEqmtsUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawDataAnalyse::ShiftEqmtsDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawDataAnalyse::ShiftEqmtsLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawDataAnalyse::ShiftEqmtsRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawDataAnalyse::ShiftEqmtsCtrlUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawDataAnalyse::ShiftEqmtsCtrlDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawDataAnalyse::ShiftEqmtsCtrlLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawDataAnalyse::ShiftEqmtsCtrlRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}


//////////////////////////////////////////////////////////////////////////

vector<DaPointInfo> CEqmDrawDataAnalyse::GetDaPointInfo(void) const
{
	return m_vecDaPtInfo;
}

CString CEqmDrawDataAnalyse::GetPointTime(void) const
{
	return m_strPtTime;
}

CString CEqmDrawDataAnalyse::GetPointCount(void) const
{
	return m_strPtCount;
}

CString CEqmDrawDataAnalyse::GetPointStatus(void) const
{
	return m_strPtStatus;
}

void CEqmDrawDataAnalyse::SetDaPointInfo(const vector<DaPointInfo>& data)
{
	m_vecDaPtInfo = data;
}

void CEqmDrawDataAnalyse::SetPointTime(const CString strPtTime)
{
	m_strPtTime = strPtTime;
}

void CEqmDrawDataAnalyse::SetPointCount(const CString strPtCount)
{
	m_strPtCount = strPtCount;
}

void CEqmDrawDataAnalyse::SetPointStatus(const CString strPtStatus)
{
	m_strPtStatus = strPtStatus;
}
