#include "StdAfx.h"
#include "EqmDrawDiagnoseGraphicPlus.h"
#include <cassert>

CEqmDrawDiagnoseGraphicPlus::CEqmDrawDiagnoseGraphicPlus(void):
m_pBitmap(NULL),
m_nUpdateInterval(2000),
m_nScope(7),
m_nInterval(1),
m_nTimeExtendSpan(5)
{
	m_nscale = 1.0;
	m_base_height = 0;
	m_base_width = 0;
	m_nNormalPicID = -1;
	m_nAnimationOrPicID = -1;
	m_nLinkPageID = -1;
	m_nIsAnimation = 0;
	m_nShowMode = 0;
	//m_dScale = (double)m_nWidth/(double)m_nHeight;
}


CEqmDrawDiagnoseGraphicPlus::~CEqmDrawDiagnoseGraphicPlus(void)
{
}

void CEqmDrawDiagnoseGraphicPlus::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/ )
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
		pGraphics->DrawImage(m_pBitmap, m_pos.X, m_pos.Y, (Gdiplus::REAL)m_nWidth, (Gdiplus::REAL)m_nHeight);
	}
	return;
}

CEqmDrawDiagnoseGraphicPlus* CEqmDrawDiagnoseGraphicPlus::Copy()
{
	m_bNeedSave = true;
	return new CEqmDrawDiagnoseGraphicPlus(*this);
}

bool CEqmDrawDiagnoseGraphicPlus::PointInMe( const Gdiplus::PointF& actPt ) const
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

Direction CEqmDrawDiagnoseGraphicPlus::PointIsSide( const Gdiplus::PointF& actPt )
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

void CEqmDrawDiagnoseGraphicPlus::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
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

bool CEqmDrawDiagnoseGraphicPlus::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
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

bool CEqmDrawDiagnoseGraphicPlus::CopyProperty( CEqmDrawBase *pBase )
{
	return false;
}

void CEqmDrawDiagnoseGraphicPlus::SetDefaultBitmap( Image* pBitmap )
{
	assert(pBitmap);
	m_pBitmap = pBitmap;
}

bool CEqmDrawDiagnoseGraphicPlus::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawDiagnoseGraphicPlus::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawDiagnoseGraphicPlus::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawDiagnoseGraphicPlus::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

void CEqmDrawDiagnoseGraphicPlus::SetTimeScope( const int scope )
{
	m_nScope = scope;
	m_bNeedSave = true;
}

const int CEqmDrawDiagnoseGraphicPlus::GetTimeScope()
{
	return m_nScope;
}

void CEqmDrawDiagnoseGraphicPlus::SetTimeInterval( const int interval )
{
	m_nInterval = interval;
	m_bNeedSave = true;
}

const int CEqmDrawDiagnoseGraphicPlus::GetTimeInterval()
{
	return m_nInterval;
}

void CEqmDrawDiagnoseGraphicPlus::SetExtendTimeSpan( const int span )
{
	m_nTimeExtendSpan = span;
	m_bNeedSave = true;
}

const int CEqmDrawDiagnoseGraphicPlus::GetExtendTimeSpan()
{
	return m_nTimeExtendSpan;
}

void CEqmDrawDiagnoseGraphicPlus::SetNormalPicID( const int nID )
{
	m_nNormalPicID = nID;
	m_bNeedSave = true;
}

const int CEqmDrawDiagnoseGraphicPlus::GetNormalPicID()
{
	return m_nNormalPicID;
}

void CEqmDrawDiagnoseGraphicPlus::SetAnimationOrPicID( const int nID )
{
	m_nAnimationOrPicID = nID;
	m_bNeedSave = true;
}

const int CEqmDrawDiagnoseGraphicPlus::GetAnimationOrPicID()
{
	return m_nAnimationOrPicID;
}

void CEqmDrawDiagnoseGraphicPlus::SetLinkPageID( const int nID )
{
	m_nLinkPageID = nID;
	m_bNeedSave = true;
}

const int CEqmDrawDiagnoseGraphicPlus::GetLinkPageID()
{
	return m_nLinkPageID;
}

void CEqmDrawDiagnoseGraphicPlus::SetIsAnimation( const int nIsAnimation )
{
	m_nIsAnimation = nIsAnimation;
	m_bNeedSave = true;
}

const int CEqmDrawDiagnoseGraphicPlus::GetIsAnimation()
{
	return m_nIsAnimation;
}

void CEqmDrawDiagnoseGraphicPlus::InsertOneBoolBindInfo( const _tagBoolBindInfo boolBindInfo )
{
	m_vecBoolBind.push_back(boolBindInfo);
	m_bNeedSave = true;
}

void CEqmDrawDiagnoseGraphicPlus::DeleteOneBoolBindInfo( const CString strBoolPointName )
{
	vector<_tagBoolBindInfo>::iterator iter = m_vecBoolBind.begin();
	while (iter != m_vecBoolBind.end())
	{
		if ((*iter).strBoolPointName == strBoolPointName)
		{
			iter = m_vecBoolBind.erase(iter);
			m_bNeedSave = true;
			break;
		}
		iter++;
	}
}

void CEqmDrawDiagnoseGraphicPlus::InsertOneSubItem( const CString strBoolPointName, const _tagDiagnoseItemPlus item )
{
	vector<_tagBoolBindInfo>::iterator iter = m_vecBoolBind.begin();
	while (iter != m_vecBoolBind.end())
	{
		if ((*iter).strBoolPointName == strBoolPointName)
		{
			(*iter).vecDiagnoseItemPlus.push_back(item);
			m_bNeedSave = true;
			break;
		}
		iter++;
	}
}

void CEqmDrawDiagnoseGraphicPlus::DeleteOneSubItem( const CString strBoolPointName, const CString strItemName )
{
	vector<_tagBoolBindInfo>::iterator iter = m_vecBoolBind.begin();
	while (iter != m_vecBoolBind.end())
	{
		if ((*iter).strBoolPointName == strBoolPointName)
		{
			vector<_tagDiagnoseItemPlus>& vec = (*iter).vecDiagnoseItemPlus;
			vector<_tagDiagnoseItemPlus>::iterator iter_sub = vec.begin();
			while (iter_sub != vec.end())
			{
				if ((*iter_sub).strItemName == strItemName)
				{
					iter_sub = vec.erase(iter_sub);
					m_bNeedSave = true;
					goto Label;
				}
				iter_sub++;
			}
		}
		iter++;
	}
Label:
	{

	};
}

vector<_tagDiagnoseItemPlus>& CEqmDrawDiagnoseGraphicPlus::GetSubItemVec( const CString strBindBoolPointName )
{
	vector<_tagBoolBindInfo>::iterator iter = m_vecBoolBind.begin();
	while (iter != m_vecBoolBind.end())
	{
		if ((*iter).strBoolPointName == strBindBoolPointName)
		{
			return (*iter).vecDiagnoseItemPlus;
		}
		iter++;
	}
}

vector<_tagBoolBindInfo>& CEqmDrawDiagnoseGraphicPlus::GetBoolBindInfoVec()
{
	return m_vecBoolBind;
}

void CEqmDrawDiagnoseGraphicPlus::DeleteAllVecInfo()
{
	m_vecBoolBind.clear();
	m_bNeedSave = true;
}

void CEqmDrawDiagnoseGraphicPlus::SetShowMode( const int nShowMode )
{
	m_nShowMode = nShowMode;
	m_bNeedSave = true;
}

const int CEqmDrawDiagnoseGraphicPlus::GetShowMode()
{
	return m_nShowMode;
}

bool CEqmDrawDiagnoseGraphicPlus::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawDiagnoseGraphicPlus::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawDiagnoseGraphicPlus::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawDiagnoseGraphicPlus::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

Image* CEqmDrawDiagnoseGraphicPlus::GetDefaultBitmap()
{
	return m_pBitmap;
}

bool CEqmDrawDiagnoseGraphicPlus::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawDiagnoseGraphicPlus* pDest = (CEqmDrawDiagnoseGraphicPlus*)pBase;
		if(pDest)
		{
			SetDefaultBitmap(pDest->GetDefaultBitmap());
			SetTimeScope(pDest->GetTimeScope());
			SetTimeInterval(pDest->GetTimeInterval());
			SetExtendTimeSpan(pDest->GetExtendTimeSpan());
			SetNormalPicID(pDest->GetNormalPicID());

			SetAnimationOrPicID(pDest->GetAnimationOrPicID());
			SetLinkPageID(pDest->GetLinkPageID());
			SetIsAnimation(pDest->GetIsAnimation());
			SetShowMode(pDest->GetShowMode());

			SetBoolBindInfoVec(pDest->GetBoolBindInfoVec());
		}
		return true;
	}
	return false;
}

void CEqmDrawDiagnoseGraphicPlus::SetBoolBindInfoVec( vector<_tagBoolBindInfo>& vec )
{
	m_vecBoolBind = vec;
}

