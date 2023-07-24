#include "stdafx.h"
#include "EqmDrawAreaChange.h"
#include "Tools/CustomTools/CustomTools.h"

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define OFFSETPICTURE 2 //鼠标单击事件

CEqmDrawAreaChange::CEqmDrawAreaChange( EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid, int id, int width /*= 0*/, int height /*= 0*/ )
	:	CEqmDrawBase(type, centerpos, pageid, id, width, height),
	m_fRotateAngel(0),
	m_strEqmName(_T("")),
	m_nLinkPageID(0),
	m_nPicID(0)
{
	m_eState = EQUIPMENT_START;
	m_bIsClicked = false;
	m_bShow = true;
	m_nscale = 1.0;
	m_pBmpName = NULL;
	m_dScale = (double)m_nWidth/(double)m_nHeight;
}

CEqmDrawAreaChange::CEqmDrawAreaChange()
	:	CEqmDrawBase(),
	m_fRotateAngel(0),
	m_strEqmName(TEXT("")),
	m_subtype(SUBTYPE_UNKNOWN),
	m_nLinkPageID(0),
	m_nPicID(0)
{
	m_eState = EQUIPMENT_START;
	m_bIsClicked = false;
	m_bShow = true;
	m_nscale = 1.0;
	m_pBmpName = NULL;
	//m_dScale = (double)m_nWidth/(double)m_nHeight;
}

CEqmDrawAreaChange::~CEqmDrawAreaChange()
{

}

void CEqmDrawAreaChange::SetEqmType( EQMDRAWTYPE type )
{
	__super::SetEqmType(type);
	m_bNeedSave = true;
}

void CEqmDrawAreaChange::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/ )
{
	const EQMDRAWTYPE picType = GetEqmType();
	if (picType == EQMDRAW_UNKNOWN)
	{
		return;
	}
	int nOffSet = 0;
	if ( m_bIsClicked )
	{
		nOffSet = OFFSETPICTURE;
	}
	pBitmap = GetBitmap();
	if (pBitmap)
	{
		RotetaBitMap(pBitmap);
		if (pBitmap)
		{
			m_base_height = pBitmap->GetHeight();
			m_base_width  = pBitmap->GetWidth();

			pGraphics->DrawImage(pBitmap, m_pos.X+nOffSet, m_pos.Y+nOffSet, (Gdiplus::REAL)GetWidth(), (Gdiplus::REAL)GetHeight());
			ResetRotateBmp(pBitmap);
			return;
		}
	}
	return;
}

bool CEqmDrawAreaChange::PointInMe( const Gdiplus::PointF& actPt ) const
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

Direction CEqmDrawAreaChange::PointIsSide( const Gdiplus::PointF& actPt )
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

void CEqmDrawAreaChange::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
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

bool CEqmDrawAreaChange::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
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

bool CEqmDrawAreaChange::LeftRotate90()
{
	int temp   = GetRotateAngle()+90;
	int width  = GetWidth();
	int height = GetHeight();
	float rotAngle = temp == 360 ? 0:temp;
	for (unsigned int uiIndex = 0; uiIndex < m_vecPortPoints.size(); uiIndex++)
	{
		Gdiplus::PointF pt = Rotate(m_vecPortPoints[uiIndex].X, m_vecPortPoints[uiIndex].Y, 0, 0, 90);
		m_vecPortPoints[uiIndex] = pt;

	}
	SetRotateAngle(rotAngle);
	SetWidth(height);
	SetHeight(width);
	m_bNeedSave = true;
	return true;
}

bool CEqmDrawAreaChange::RightRotate90()
{
	int temp = GetRotateAngle()-90;
	int width  = GetWidth();
	int height = GetHeight();
	float rotAngle = temp == -360 ? 0:temp;
	for (unsigned int uiIndex = 0; uiIndex < m_vecPortPoints.size(); uiIndex++)
	{
		Gdiplus::PointF pt = Rotate(m_vecPortPoints[uiIndex].X, m_vecPortPoints[uiIndex].Y, 0, 0, -90);
		m_vecPortPoints[uiIndex] = pt;

	}
	SetRotateAngle(rotAngle);
	SetWidth(height);
	SetHeight(width);
	m_bNeedSave = true;
	return true;
}

bool CEqmDrawAreaChange::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawAreaChange::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawAreaChange::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawAreaChange::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawAreaChange::CopyProperty( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		m_nHeight = pBase->GetHeight();
		m_nWidth  = pBase->GetWidth();
		m_layer = pBase->GetLayer();
		return true;
	}
	return false;
}

CEqmDrawAreaChange* CEqmDrawAreaChange::Copy()
{
	m_bNeedSave = true;
	return new  CEqmDrawAreaChange(*this);
}

void CEqmDrawAreaChange::SetBitmap( Image* pBitmap )
{
	ASSERT(pBitmap);

	m_pBmpName = pBitmap;
}

Image* CEqmDrawAreaChange::GetBitmap()
{
	if (m_pBmpName)
	{
		return m_pBmpName;
	}
	return NULL;
}

void CEqmDrawAreaChange::SetPicID( const int nPicID )
{
	m_nPicID = nPicID;
	m_bNeedSave = true;
}

void CEqmDrawAreaChange::RotetaBitMap( Image *pBitMap /*= NULL*/ )
{
	if ( NULL != pBitMap )
	{
		switch ( m_fRotateAngel )
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

void CEqmDrawAreaChange::ResetRotateBmp( Image *pBitMap /*= NULL*/ )
{
	if(!pBitMap) 
	{
		return;
	}
	switch ( m_fRotateAngel )
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

int CEqmDrawAreaChange::GetPicID()
{
	return m_nPicID;
}

int CEqmDrawAreaChange::GetRotateAngle() const
{
	return m_fRotateAngel;
}

void CEqmDrawAreaChange::SetRotateAngle( int angle )
{
	m_fRotateAngel= angle;
	m_bNeedSave = true;
}

void CEqmDrawAreaChange::SetAreaInfo( vector<_tagAreaInfo> vecAreaInfo )
{
	m_vecAreaInfo = vecAreaInfo;
}

vector<_tagAreaInfo> CEqmDrawAreaChange::GetAreaInfo()
{
	return m_vecAreaInfo;
}

void CEqmDrawAreaChange::AddAreaInfo( const CString strPointList, const int nAreaImageID , const int nLinkPage)
{
	_tagAreaInfo area;
	area.strPointList = strPointList;
	area.BnmID = nAreaImageID;
	area.nPageID = nLinkPage;
	m_vecAreaInfo.push_back(area);
}

bool CEqmDrawAreaChange::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawAreaChange::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawAreaChange::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawAreaChange::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawAreaChange::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//特有属性

		CEqmDrawAreaChange* pAreaChang = (CEqmDrawAreaChange*)pBase;
		if(pAreaChang)
		{
			SetRotateAngle(pAreaChang->GetRotateAngle());
			SetBitmap(pAreaChang->GetBitmap());
			SetPicID(pAreaChang->GetPicID());
			SetAreaInfo(pAreaChang->GetAreaInfo());
		}
		return true;
	}
	return false;
}
