#include "StdAfx.h"
#include "EqmDrawButton.h"


CEqmDrawButton::CEqmDrawButton(void):	
m_pBitmap(NULL)
,m_base_height(0)
,m_base_width(0)
,m_nscale(0)
,m_nIDPicComm(-1)
,m_nIDPicOver(-1)
,m_nIDPicDown(-1)
,m_nIDPicDisable(-1)
,m_nLinkPageID(-1)
,m_nIDPicChecked(-1)
{
	m_strSettingValue = _T("Text");
	m_strTextContent = L"";
	m_nTextColor = RGB(255,255,255);
	m_nTextSize = 22;
	//m_dScale = (double)m_nWidth/(double)m_nHeight;
}

CEqmDrawButton::~CEqmDrawButton(void)
{
}

void CEqmDrawButton::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/ )
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
	if (m_strTextContent.GetLength() > 0)
	{
		Gdiplus::StringFormat strFomat;
		strFomat.SetAlignment(Gdiplus::StringAlignmentCenter);

		const float centerX = GetPos().X;
		const float centerY = GetPos().Y+GetHeight()/2-m_nTextSize/2;
		const float rectW = m_nWidth ;
		const float rectH = m_nHeight;
		Gdiplus::RectF rect0(centerX, centerY, rectW, rectH);

		Gdiplus::Font font(L"Î¢ÈíÑÅºÚ", m_nTextSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel,NULL);
		Gdiplus::Color color(255, GetRValue(m_nTextColor), GetGValue(m_nTextColor), GetBValue(m_nTextColor));
		Gdiplus::SolidBrush solidBrush(color);
		pGraphics->DrawString(m_strTextContent.GetString(), -1, &font, rect0, &strFomat, &solidBrush);
	}
	return;
}

CEqmDrawButton* CEqmDrawButton::Copy()
{
	m_bNeedSave = true;
	return new CEqmDrawButton(*this);
}

bool CEqmDrawButton::PointInMe( const Gdiplus::PointF& actPt ) const
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

Direction CEqmDrawButton::PointIsSide( const Gdiplus::PointF& actPt )
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

void CEqmDrawButton::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
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

bool CEqmDrawButton::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
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

bool CEqmDrawButton::CopyProperty( CEqmDrawBase *pBase )
{
	return false;
}

void CEqmDrawButton::SetDefaultBitmap( Image* pBitmap )
{
	if(pBitmap)
	{
		m_pBitmap = pBitmap;
	}
}

bool CEqmDrawButton::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawButton::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawButton::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawButton::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

void CEqmDrawButton::Set5PicID( const int nIDPicComm, const int nIDPicDisable, const int nIDPicDown, const int nIDPicOver, const int nIDPicChecked)
{
	m_nIDPicComm = nIDPicComm;
	m_nIDPicDisable = nIDPicDisable;
	m_nIDPicDown = nIDPicDown;
	m_nIDPicOver = nIDPicOver;
	m_nIDPicChecked = nIDPicChecked;
	m_bNeedSave = true;
}

void CEqmDrawButton::SetLinkPageID( const int nLinkPageID )
{
	m_nLinkPageID = nLinkPageID;
	m_bNeedSave = true;
}

void CEqmDrawButton::Get5PicID( int& nIDPicComm, int& nIDPicDisable, int& nIDPicDown, int& nIDPicOver, int& nIDPicChecked)
{
	nIDPicComm = m_nIDPicComm;
	nIDPicDisable = m_nIDPicDisable;
	nIDPicDown = m_nIDPicDown;
	nIDPicOver = m_nIDPicOver;
	nIDPicChecked = m_nIDPicChecked;
}

const int CEqmDrawButton::GetLinkPageID()
{
	return m_nLinkPageID;
}

void CEqmDrawButton::SetBindSetPointName( const CString strName )
{
	m_strBindSetPointName = strName;
	m_bNeedSave = true;
}

const CString CEqmDrawButton::GetBindSetPointName()
{
	return m_strBindSetPointName;
}
/*==========2019-9-11================*/

void CEqmDrawButton::SetSettingValue( const CString strValue )
{
	m_strSettingValue = strValue;
	m_bNeedSave = true;
}

const CString CEqmDrawButton::GetSettingValue()
{
	return m_strSettingValue;
}
/*=====================================*/
void CEqmDrawButton::SetTextContent( const CString strText )
{
	m_strTextContent = strText;
	m_bNeedSave = true;
}

const CString CEqmDrawButton::GetTextContent()
{
	return m_strTextContent;
}

void CEqmDrawButton::SetTextColor( const COLORREF nColor )
{
	m_nTextColor = nColor;
	m_bNeedSave = true;
}

const COLORREF CEqmDrawButton::GetTextColor()
{
	return m_nTextColor;
}

void CEqmDrawButton::SetTextSize( const int nSize )
{
	m_nTextSize = nSize;
	m_bNeedSave = true;
}

const int CEqmDrawButton::GetTextSize()
{
	return m_nTextSize;
}

void CEqmDrawButton::SetOperation( const CString strOperation )
{
	m_strOperationContent = strOperation;
	m_bNeedSave = true;
}

const CString CEqmDrawButton::GetOperation()
{
	return m_strOperationContent;
}

void CEqmDrawButton::InsertExpress( const _tagRelationExpress express )
{
	m_vecRelation.vecExpress.push_back(express);
	m_bNeedSave = true;
}

void CEqmDrawButton::DeleteExpress( const CString strPointName )
{
	vector<_tagRelationExpress>::iterator iter = m_vecRelation.vecExpress.begin();
	while (iter != m_vecRelation.vecExpress.end())
	{
		if ((*iter).strPointName == strPointName)
		{
			iter = m_vecRelation.vecExpress.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
	}
	m_bNeedSave = true;
}

void CEqmDrawButton::SetRelation( const int nRelation )
{
	m_vecRelation.nRelationStatus = nRelation;
	m_bNeedSave = true;
}

const int CEqmDrawButton::GetRelation()
{
	return m_vecRelation.nRelationStatus;
}

vector<_tagRelationExpress>& CEqmDrawButton::GetRelationVec()
{
	return m_vecRelation.vecExpress;
}

void CEqmDrawButton::DeleteAllRelation()
{
	m_vecRelation.vecExpress.clear();
	m_bNeedSave = true;
}

bool CEqmDrawButton::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawButton::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawButton::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawButton::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawButton::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawButton* pDest = (CEqmDrawButton*)pBase;
		if(pDest)
		{
			int nComm = 0;
			int nDisable = 0;
			int nDown = 0;
			int nOver = 0;
			int nChecked = 0;
			pDest->Get5PicID(nComm,nDisable,nDown,nOver,nChecked);
			Set5PicID(nComm,nDisable,nDown,nOver,nChecked);

			SetLinkPageID(pDest->GetLinkPageID());
			SetBindSetPointName(pDest->GetBindSetPointName());
			SetSettingValue(pDest->GetSettingValue());
			SetTextContent(pDest->GetTextContent());

			SetTextColor(pDest->GetTextColor());
			SetTextSize(pDest->GetTextSize());
			SetOperation(pDest->GetOperation());
			SetRelation(pDest->GetRelation());

			SetRelationVec(pDest->GetRelationVec());
			SetDefaultBitmap(pDest->GetDefaultBitmap());
		}
		return true;
	}
	return false;
}

void CEqmDrawButton::SetRelationVec( vector<_tagRelationExpress>& vec )
{
	m_vecRelation.vecExpress = vec;
}

Image* CEqmDrawButton::GetDefaultBitmap()
{
	return m_pBitmap;
}

void CEqmDrawButton::SetDownloadEnable(const CString strVal)
{
	m_strDownloadEnable = strVal;
}

CString CEqmDrawButton::GetDownloadEnable(void) const
{
	return m_strDownloadEnable;
}

void CEqmDrawButton::SetDownloadName(const CString strVal)
{
	m_strDownloadName = strVal;
}

CString CEqmDrawButton::GetDownloadName(void) const
{
	return m_strDownloadName;
}
