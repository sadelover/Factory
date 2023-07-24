#include "StdAfx.h"
#include "EqmDrawHistoryTrend.h"


const int g_nWidth = 960;
const int g_nHeight = 460;


CEqmDrawHistoryTrend::CEqmDrawHistoryTrend(void)
	: m_pBitmap(NULL)
	, m_nBaseHeight(0)
	, m_nBaseWidth(0)
	, m_historyTrendType(enumCurve)
	//, m_bIsAutoCalcRange(false)
	//, m_strBindPointName(_T(""))
	//, m_strRemarks(_T(""))
	//, m_color(RGB(0, 0, 0))
	//, m_nCycle(0)
	//, m_nLineWidth(0)
	//, m_fScale(0.6)
{
	m_fscale = (double)m_nWidth / (double)m_nHeight;
}

CEqmDrawHistoryTrend::~CEqmDrawHistoryTrend(void)
{
}

CEqmDrawHistoryTrend* CEqmDrawHistoryTrend::Copy(void)
{
	m_bNeedSave = true;
	return new CEqmDrawHistoryTrend(*this);
}

bool CEqmDrawHistoryTrend::PointInMe(const Gdiplus::PointF& actPt) const
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

Direction CEqmDrawHistoryTrend::PointIsSide(const Gdiplus::PointF& actPt)
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
		return Dir_None;
}

void CEqmDrawHistoryTrend::DrawSelectEdge(Gdiplus::Graphics *pGraphics)
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

bool CEqmDrawHistoryTrend::ChangeSize(const Gdiplus::PointF& actPt, Direction dir)
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
				if (m_nWidth < g_nWidth)
				{
					m_pos.X = m_posru.X - g_nWidth;
					m_posld.X = m_posru.X - g_nWidth;
					m_nWidth = g_nWidth;
				}
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
			if (m_nWidth < g_nWidth)
			{
				m_posrd.X = m_pos.X + g_nWidth;
				m_posru.X = m_pos.X + g_nWidth;
				m_nWidth = g_nWidth;
			}
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
				if (m_nHeight < g_nHeight)
				{
					m_pos.Y = m_posld.Y - g_nHeight;
					m_posru.Y = m_posld.Y - g_nHeight;
					m_nHeight = g_nHeight;
				}
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
			if (m_nHeight < g_nHeight)
			{
				m_posld.Y = m_pos.Y + g_nHeight;
				m_posrd.Y = m_pos.Y + g_nHeight;
				m_nHeight = g_nHeight;
			}
			m_dScale = (double)m_nWidth / (double)m_nHeight;
		}

		break;
	case Dir_LeftUp:
		{
			if (actPt.X < m_posrd.X && actPt.Y < m_posrd.Y)
			{
				m_nWidth = abs(actPt.X - m_posrd.X);
				if (m_nWidth < g_nWidth)
				{
					m_nWidth = g_nWidth;
				}
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
				if (m_nWidth < g_nWidth)
				{
					m_nWidth = g_nWidth;
				}
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
				if (m_nWidth < g_nWidth)
				{
					m_nWidth = g_nWidth;
				}
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
				if (m_nWidth < g_nWidth)
				{
					m_nWidth = g_nWidth;
				}
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

bool CEqmDrawHistoryTrend::CopyProperty(CEqmDrawBase *pBase)
{
	return false;
}

void CEqmDrawHistoryTrend::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/)
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

void CEqmDrawHistoryTrend::SetDefaultBitmap(Image* pBitmap)
{
	if (pBitmap != NULL)
	{
		m_pBitmap = pBitmap;
	}
}

bool CEqmDrawHistoryTrend::ShiftEqmtsUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawHistoryTrend::ShiftEqmtsDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawHistoryTrend::ShiftEqmtsLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawHistoryTrend::ShiftEqmtsRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

void CEqmDrawHistoryTrend::SetWidth(int width)
{
	if (width < g_nWidth)
	{
		width = g_nWidth;
	}
	CEqmDrawBase::SetWidth(width);
}

void CEqmDrawHistoryTrend::SetHeight(int height)
{
	if (height < g_nHeight)
	{
		height = g_nHeight;
	}
	CEqmDrawBase::SetHeight(height);
}

bool CEqmDrawHistoryTrend::ShiftEqmtsCtrlUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawHistoryTrend::ShiftEqmtsCtrlDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawHistoryTrend::ShiftEqmtsCtrlLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawHistoryTrend::ShiftEqmtsCtrlRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}


//////////////////////////////////////////////////////////////////////////

void CEqmDrawHistoryTrend::SetHistoryTrendType(const HistoryTrendType& hisTrendType)
{
	m_historyTrendType = hisTrendType;
}

//void CEqmDrawHistoryTrend::SetStartTime(const COleDateTime time)
//{
//	m_tmStart = time;
//}
//
//void CEqmDrawHistoryTrend::SetEndTime(const COleDateTime time)
//{
//	m_tmEnd = time;
//}
//
//void CEqmDrawHistoryTrend::SetIsAutoCalcRange(const bool bFlag)
//{
//	m_bIsAutoCalcRange = bFlag;
//}
//
//void CEqmDrawHistoryTrend::SetBindPointName(const CString strBindPointName)
//{
//	m_strBindPointName = strBindPointName;
//}
//
//void CEqmDrawHistoryTrend::SetRemarks(const CString strRemarks)
//{
//	m_strRemarks = strRemarks;
//}
//
//void CEqmDrawHistoryTrend::SetImageColor(const COLORREF color)
//{
//	m_color = color;
//}
//
//void CEqmDrawHistoryTrend::SetUpdateCycle(const int nCycle)
//{
//	m_nCycle = nCycle;
//}
//
//void CEqmDrawHistoryTrend::SetLineWidth(const int nLineWidth)
//{
//	m_nLineWidth = nLineWidth;
//}
//
//void CEqmDrawHistoryTrend::SetScale(const float fScale)
//{
//	m_fScale = fScale;
//}

void CEqmDrawHistoryTrend::SetHistoryTrendVector(const vector<HistoryTrendInfo>& vecHisTrendInfo)
{
	m_vecHistoryTrendInfo = vecHisTrendInfo;
}

int	CEqmDrawHistoryTrend::GetPicId(void)
{
	return g_nPicIndexHistoryTrend;
}

vector<HistoryTrendInfo> CEqmDrawHistoryTrend::GetHistoryTrendVector(void)
{
	return m_vecHistoryTrendInfo;
}

HistoryTrendType CEqmDrawHistoryTrend::GetHistoryTrendType(void)
{
	return m_historyTrendType;
}

bool CEqmDrawHistoryTrend::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawHistoryTrend* pDest = (CEqmDrawHistoryTrend*)pBase;
		if(pDest)
		{
			SetDefaultBitmap(pDest->GetDefaultBitmap());
			SetHistoryTrendType(pDest->GetHistoryTrendType());
			SetHistoryTrendVector(pDest->GetHistoryTrendVector());
		}
		return true;
	}
	return false;
}

Image* CEqmDrawHistoryTrend::GetDefaultBitmap()
{
	return m_pBitmap;
}

//COleDateTime CEqmDrawHistoryTrend::GetStartTime(void)
//{
//	return m_tmStart;
//}
//
//COleDateTime CEqmDrawHistoryTrend::GetEndTime(void)
//{
//	return m_tmEnd;
//}
//
//bool CEqmDrawHistoryTrend::GetIsAutoCalcRange(void)
//{
//	return m_bIsAutoCalcRange;
//}
//
//CString CEqmDrawHistoryTrend::GetBindPointName(void)
//{
//	return m_strBindPointName;
//}
//
//CString CEqmDrawHistoryTrend::GetRemarks(void)
//{
//	return m_strRemarks;
//}
//
//COLORREF CEqmDrawHistoryTrend::GetImageColor(void)
//{
//	return m_color;
//}
//
//int CEqmDrawHistoryTrend::GetUpdateCycle(void)
//{
//	return m_nCycle;
//}
//
//int CEqmDrawHistoryTrend::GetLineWidth(void)
//{
//	return m_nLineWidth;
//}
//
//float CEqmDrawHistoryTrend::GetScale(void)
//{
//	return m_fScale;
//}
