#include "StdAfx.h"
#include "EqmDrawDottedLineFrm.h"


CEqmDrawDottedLineFrm::CEqmDrawDottedLineFrm(void) : m_pCommonBorderPen(NULL)
{
}


CEqmDrawDottedLineFrm::~CEqmDrawDottedLineFrm(void)
{
	DeleteCommonBorderPen();
}

CEqmDrawDottedLineFrm* CEqmDrawDottedLineFrm::Copy(void)
{
	CEqmDrawDottedLineFrm *pNewCopy =  new CEqmDrawDottedLineFrm(*this);
	pNewCopy->m_pCommonBorderPen = NULL;
	m_bNeedSave = true;
	return pNewCopy;
}

void CEqmDrawDottedLineFrm::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/)
{
	const EQMDRAWTYPE picType = GetEqmType();
	if (EQMDRAW_UNKNOWN == picType)
	{
		return;
	}

	ASSERT(pGraphics != NULL);
	if (NULL == pGraphics)
	{
		return;
	}

	CreateCommonBorderPen();
	ASSERT(m_pCommonBorderPen != NULL);
	if (m_pCommonBorderPen != NULL)
	{
		pGraphics->DrawRectangle(m_pCommonBorderPen, (int)m_pos.X, (int)m_pos.Y, m_nWidth, m_nHeight);
	}
}

bool CEqmDrawDottedLineFrm::PointInMe(const Gdiplus::PointF& actPt) const
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

Direction CEqmDrawDottedLineFrm::PointIsSide(const Gdiplus::PointF& actPt)
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

void CEqmDrawDottedLineFrm::DrawSelectEdge(Gdiplus::Graphics *pGraphics)
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

bool CEqmDrawDottedLineFrm::ChangeSize(const Gdiplus::PointF& actPt, Direction dir)
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

bool CEqmDrawDottedLineFrm::CopyProperty(CEqmDrawBase *pBase)
{
	return false;
}

bool CEqmDrawDottedLineFrm::ClonePropertyByDest(CEqmDrawBase *pBase)
{
	if (pBase != NULL && m_eEqmType == pBase->GetEqmType())
	{
		CopyBaseProperty(pBase);

		//×¨ÓÐÊôÐÔ

		return true;
	}

	return false;
}

bool CEqmDrawDottedLineFrm::ShiftEqmtsUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawDottedLineFrm::ShiftEqmtsDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawDottedLineFrm::ShiftEqmtsLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawDottedLineFrm::ShiftEqmtsRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawDottedLineFrm::ShiftEqmtsCtrlUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawDottedLineFrm::ShiftEqmtsCtrlDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawDottedLineFrm::ShiftEqmtsCtrlLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawDottedLineFrm::ShiftEqmtsCtrlRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}


//////////////////////////////////////////////////////////////////////////


void CEqmDrawDottedLineFrm::CreateCommonBorderPen(void)
{
	if (NULL == m_pCommonBorderPen)
	{
		//Gdiplus::Color color_topleft(16, 16, 16);		
		m_pCommonBorderPen = new Gdiplus::Pen(Gdiplus::Color::Black, 1);
		ASSERT(m_pCommonBorderPen);
		m_pCommonBorderPen->SetWidth(2);
		m_pCommonBorderPen->SetDashStyle(DashStyleDash);
		m_pCommonBorderPen->SetAlignment(Gdiplus::PenAlignmentInset);
	}

}

void CEqmDrawDottedLineFrm::DeleteCommonBorderPen(void)
{
	if (m_pCommonBorderPen != NULL)
	{
		delete m_pCommonBorderPen;
		m_pCommonBorderPen = NULL;
	}
}
