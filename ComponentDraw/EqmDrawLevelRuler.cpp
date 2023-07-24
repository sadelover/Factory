#include "StdAfx.h"
#include "EqmDrawLevelRuler.h"


CEqmDrawLevelRuler::CEqmDrawLevelRuler(void)
	: m_pBitmap(NULL)
	, m_dMax(0)
	, m_dMin(0)
	, m_dMainScaleInterval(0)
	, m_dMinorScaleInterval(0)
	, m_nDecimalPlace(0)
{
	m_fscale = (double)m_nWidth / (double)m_nHeight;
}


CEqmDrawLevelRuler::~CEqmDrawLevelRuler(void)
{
}

CEqmDrawLevelRuler* CEqmDrawLevelRuler::Copy(void)
{
	m_bNeedSave = true;
	return new CEqmDrawLevelRuler(*this);
}

void CEqmDrawLevelRuler::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/)
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

bool CEqmDrawLevelRuler::PointInMe(const Gdiplus::PointF& actPt) const
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

Direction CEqmDrawLevelRuler::PointIsSide(const Gdiplus::PointF& actPt)
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

void CEqmDrawLevelRuler::DrawSelectEdge(Gdiplus::Graphics *pGraphics)
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

bool CEqmDrawLevelRuler::ChangeSize(const Gdiplus::PointF& actPt, Direction dir)
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

bool CEqmDrawLevelRuler::CopyProperty(CEqmDrawBase *pBase)
{
	return false;
}

void CEqmDrawLevelRuler::SetDefaultBitmap(Image* pBitmap)
{
	if (pBitmap != NULL)
	{
		m_pBitmap = pBitmap;
	}
}

bool CEqmDrawLevelRuler::ShiftEqmtsUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawLevelRuler::ShiftEqmtsDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawLevelRuler::ShiftEqmtsLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawLevelRuler::ShiftEqmtsRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawLevelRuler::ShiftEqmtsCtrlUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawLevelRuler::ShiftEqmtsCtrlDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawLevelRuler::ShiftEqmtsCtrlLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawLevelRuler::ShiftEqmtsCtrlRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}


//////////////////////////////////////////////////////////////////////////


CString CEqmDrawLevelRuler::GetName(void) const
{
	return m_strName;
}

double CEqmDrawLevelRuler::GetMax(void) const
{
	return m_dMax;
}

double CEqmDrawLevelRuler::GetMin(void) const
{
	return m_dMin;
}

double CEqmDrawLevelRuler::GetMainScaleInterval(void) const
{
	return m_dMainScaleInterval;
}

double CEqmDrawLevelRuler::GetMinorScaleInterval(void) const
{
	return m_dMinorScaleInterval;
}

int CEqmDrawLevelRuler::GetDecimalPlace(void) const
{
	return m_nDecimalPlace;
}

vector<LabelInfo> CEqmDrawLevelRuler::GetLabelInfo(void) const
{
	return m_vecLabelInfo;
}

vector<SectionInfo> CEqmDrawLevelRuler::GetSectionInfo(void) const
{
	return m_vecSectionInfo;
}

void CEqmDrawLevelRuler::SetName(const CString strName)
{
	m_strName = strName;
}

void CEqmDrawLevelRuler::SetMax(const double dMax)
{
	m_dMax = dMax;
}

void CEqmDrawLevelRuler::SetMin(const double dMin)
{
	m_dMin = dMin;
}

void CEqmDrawLevelRuler::SetMainScaleInterval(const double dMainInterval)
{
	m_dMainScaleInterval = dMainInterval;
}

void CEqmDrawLevelRuler::SetMinorScaleInterval(const double dMinorInterval)
{
	m_dMinorScaleInterval = dMinorInterval;
}

void CEqmDrawLevelRuler::SetDecimalPlace(const int nDeciPlace)
{
	m_nDecimalPlace = nDeciPlace;
}

void CEqmDrawLevelRuler::SetLabelInfo(const vector<LabelInfo>& vecLabelInfo)
{
	m_vecLabelInfo = vecLabelInfo;
}

void CEqmDrawLevelRuler::SetSectionInfo(const vector<SectionInfo>& vecSectionInfo)
{
	m_vecSectionInfo = vecSectionInfo;
}

Image* CEqmDrawLevelRuler::GetDefaultBitmap()
{
	return m_pBitmap;
}

bool CEqmDrawLevelRuler::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawLevelRuler* pDest = (CEqmDrawLevelRuler*)pBase;
		if(pDest)
		{
			SetDefaultBitmap(pDest->GetDefaultBitmap());
			SetName(pDest->GetName());
			SetMax(pDest->GetMax());
			SetMin(pDest->GetMin());
			SetMainScaleInterval(pDest->GetMainScaleInterval());

			SetMinorScaleInterval(pDest->GetMinorScaleInterval());
			SetDecimalPlace(pDest->GetDecimalPlace());
			SetLabelInfo(pDest->GetLabelInfo());
			SetSectionInfo(pDest->GetSectionInfo());
		}
		return true;
	}
	return false;
}
