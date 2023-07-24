#include "StdAfx.h"
#include "EqmDrawButtonPlus.h"


CEqmDrawButtonPlus::CEqmDrawButtonPlus(void):	
m_pBitmap(NULL)
,m_base_height(0)
,m_base_width(0)
,m_nscale(0)
,m_eButtonStyle(E_RADIO_BUTTON)
,m_nGroupID(-1)
,m_nTextSize(14)
,m_nTextColor(RGB(0,0,0))
,m_cstrOptionsDec(L"")
{
	//m_dScale = (double)m_nWidth/(double)m_nHeight;
}

CEqmDrawButtonPlus::~CEqmDrawButtonPlus(void)
{
}

void CEqmDrawButtonPlus::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/ )
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

		const float fX = GetPos().X + m_nWidth + 5;
		float fY = 0;
		const float rectW = m_nWidth ;
		const float rectH = m_nHeight;
		if (E_RADIO_BUTTON == m_eButtonStyle)
		{
			fY = GetPos().Y;
		} 
		else if (E_CHECK_BUTTON == m_eButtonStyle)
		{
			fY = GetPos().Y - 4;
		}
		else
		{	// unknown button type
			return;
		}
		Gdiplus::RectF outRect;
		Gdiplus::Font font(L"Î¢ÈíÑÅºÚ", m_nTextSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel,NULL);
		pGraphics->MeasureString(m_strTextContent.GetString(), m_strTextContent.GetLength(), &font, Gdiplus::RectF(0,0,0,0), &strFomat, &outRect);
		Gdiplus::Color color(255, GetRValue(m_nTextColor), GetGValue(m_nTextColor), GetBValue(m_nTextColor));
		Gdiplus::SolidBrush solidBrush(color);
		Gdiplus::RectF rect0(fX, fY, outRect.Width, outRect.Height);
		pGraphics->DrawString(m_strTextContent.GetString(), -1, &font, rect0, &strFomat, &solidBrush);
	}
}

CEqmDrawButtonPlus* CEqmDrawButtonPlus::Copy()
{
	m_bNeedSave = true;
	return new CEqmDrawButtonPlus(*this);
}

bool CEqmDrawButtonPlus::PointInMe( const Gdiplus::PointF& actPt ) const
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

Direction CEqmDrawButtonPlus::PointIsSide( const Gdiplus::PointF& actPt )
{
	return Dir_None;
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

void CEqmDrawButtonPlus::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
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

bool CEqmDrawButtonPlus::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
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

bool CEqmDrawButtonPlus::CopyProperty( CEqmDrawBase *pBase )
{
	return false;
}

void CEqmDrawButtonPlus::SetDefaultBitmap( Image* pBitmap )
{
	if(pBitmap)
	{
		m_pBitmap = pBitmap;
	}
}

bool CEqmDrawButtonPlus::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawButtonPlus::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawButtonPlus::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawButtonPlus::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawButtonPlus::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawButtonPlus::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawButtonPlus::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawButtonPlus::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

void CEqmDrawButtonPlus::SetTextColor( const COLORREF color )
{
	m_nTextColor = color;
}

void CEqmDrawButtonPlus::SetTextSize( const int nSize )
{
	m_nTextSize = nSize;
}

void CEqmDrawButtonPlus::SetTextContent( const CString strContent )
{
	m_strTextContent = strContent;
}

void CEqmDrawButtonPlus::SetButtonStyle( const eButtonStyle style )
{
	m_eButtonStyle = style;
}

void CEqmDrawButtonPlus::SetValueForUnCheck( const CString strValue )
{
	m_strSettingValueForUncheck = strValue;
}

void CEqmDrawButtonPlus::SetValueForCheck( const CString strValue )
{
	m_strSettingValueForCheck = strValue;
}

void CEqmDrawButtonPlus::SetBindPointName( const CString strPointName )
{
	m_strBindPointName = strPointName;
	m_bNeedSave = true;
}

void CEqmDrawButtonPlus::SetGroupID( const int nGroupID )
{
	m_nGroupID = nGroupID;
}

void CEqmDrawButtonPlus::SetExpression( const CString strExpression )
{
	m_strEnableExpression = strExpression;
}

void CEqmDrawButtonPlus::SetPathUncheckCom( const CString strFullPath )
{
	m_strPathUncheckCom = strFullPath;
}

void CEqmDrawButtonPlus::SetPathUncheckOver( const CString strFullPath )
{
	m_strPathUncheckOver = strFullPath;
}

void CEqmDrawButtonPlus::SetPathCheckCom( const CString strFullPath )
{
	m_strPathCheckCom = strFullPath;
}

void CEqmDrawButtonPlus::SetPathCheckOver( const CString strFullPath )
{
	m_strPathCheckOver = strFullPath;
}

void CEqmDrawButtonPlus::SetOptionsDec(const CString cstrOptionsDec)
{
	m_cstrOptionsDec = cstrOptionsDec;
}

const COLORREF CEqmDrawButtonPlus::GetTextColor()
{
	return m_nTextColor;
}

const int CEqmDrawButtonPlus::GetTextSize()
{
	return m_nTextSize;
}

const CString CEqmDrawButtonPlus::GetTextContent()
{
	return m_strTextContent;
}

const eButtonStyle CEqmDrawButtonPlus::GetButtonStyle()
{
	return m_eButtonStyle;
}

const CString CEqmDrawButtonPlus::GetValueForUnCheck()
{
	return m_strSettingValueForUncheck;
}

const CString CEqmDrawButtonPlus::GetValueForCheck()
{
	return m_strSettingValueForCheck;
}

const CString CEqmDrawButtonPlus::GetBindPointName()
{
	return m_strBindPointName;
}

const int CEqmDrawButtonPlus::GetGroupID()
{
	return m_nGroupID;
}

const CString CEqmDrawButtonPlus::GetExpression()
{
	return m_strEnableExpression;
}

const CString CEqmDrawButtonPlus::GetPathUncheckCom()
{
	return m_strPathUncheckCom;
}

const CString CEqmDrawButtonPlus::GetPathUncheckOver()
{
	return m_strPathUncheckOver;
}

const CString CEqmDrawButtonPlus::GetPathCheckCom()
{
	return m_strPathCheckCom;
}

const CString CEqmDrawButtonPlus::GetPathCheckOver()
{
	return m_strPathCheckOver;
}

const CString CEqmDrawButtonPlus::GetExpressLeftOp()
{
	CString strResult;
	if (m_strEnableExpression.GetLength()>0)
	{
		int nPosStart = m_strEnableExpression.Find(L'[',0);
		if (nPosStart >= 0)
		{
			int nPosEnd = m_strEnableExpression.Find(L']',0);
			strResult = m_strEnableExpression.Mid(nPosStart+1,nPosEnd-nPosStart-1);
		}
	}
	return strResult;
}

const CString CEqmDrawButtonPlus::GetExpressRightOp()
{
	CString strResult;
	if (m_strEnableExpression.GetLength()>0)
	{
		int nPosStart = m_strEnableExpression.ReverseFind((L']'));
		if (nPosStart == m_strEnableExpression.GetLength() - 1)
		{
			int nPosEnd = m_strEnableExpression.ReverseFind(L'[');
			strResult = m_strEnableExpression.Mid(nPosStart+1,nPosStart-nPosEnd-1);
		}
		else
		{
			if (m_strEnableExpression.Find(L">",0) >= 0)
			{
				int nPos = -1;
				if (m_strEnableExpression.Find(L"=",0) >= 0)
				{
					nPos = m_strEnableExpression.Find(L"=",0);
				}
				else
				{
					nPos = m_strEnableExpression.Find(L">",0);
				}
				strResult = m_strEnableExpression.Right(m_strEnableExpression.GetLength()-nPos-1);
			}
			else if (m_strEnableExpression.Find(L"<",0) >= 0)
			{
				int nPos = -1;
				if (m_strEnableExpression.Find(L"=",0) >= 0)
				{
					nPos = m_strEnableExpression.Find(L"=",0);
				}
				else
				{
					nPos = m_strEnableExpression.Find(L"<",0);
				}
				strResult = m_strEnableExpression.Right(m_strEnableExpression.GetLength()-nPos-1);
			}
			else if (m_strEnableExpression.Find(L">=",0) >= 0)
			{
				int nPos = m_strEnableExpression.Find(L"=",0);
				strResult = m_strEnableExpression.Right(m_strEnableExpression.GetLength()-nPos-1);
			}
			else if (m_strEnableExpression.Find(L"<=",0) >= 0)
			{
				int nPos = m_strEnableExpression.Find(L"=",0);
				strResult = m_strEnableExpression.Right(m_strEnableExpression.GetLength()-nPos-1);
			}
			else if (m_strEnableExpression.Find(L"!=",0) >= 0)
			{
				int nPos = m_strEnableExpression.Find(L"=",0);
				strResult = m_strEnableExpression.Right(m_strEnableExpression.GetLength()-nPos-1);
			}
			else if (m_strEnableExpression.Find(L"==",0) >= 0)
			{
				int nPos = m_strEnableExpression.ReverseFind(L'=');
				strResult = m_strEnableExpression.Right(m_strEnableExpression.GetLength()-nPos-1);
			}
		}
	}
	return strResult;
}

const CString CEqmDrawButtonPlus::GetExpressMidOp()
{
	CString strResult;
	if (m_strEnableExpression.GetLength()>0)
	{
		if (m_strEnableExpression.Find(L">",0) >= 0)
		{
			if (m_strEnableExpression.Find(L"=",0) >= 0)
			{
				strResult = ">=";
			}
			else
			{
				strResult = ">";
			}
		}
		else if (m_strEnableExpression.Find(L"<",0) >= 0)
		{
			if (m_strEnableExpression.Find(L"=",0) >= 0)
			{
				strResult = "<=";
			}
			else
			{
				strResult = "<";
			}
		}
		else if (m_strEnableExpression.Find(L">=",0) >= 0)
		{
			strResult = ">=";
		}
		else if (m_strEnableExpression.Find(L"<=",0) >= 0)
		{
			strResult = "<=";
		}
		else if (m_strEnableExpression.Find(L"!=",0) >= 0)
		{
			strResult = "!=";
		}
		else if (m_strEnableExpression.Find(L"==",0) >= 0)
		{
			strResult = "==";
		}
	}
	return strResult;
}

const CString CEqmDrawButtonPlus::GetOptionsDec()
{
	return m_cstrOptionsDec;
}

const bool CEqmDrawButtonPlus::CheckExpression( const CString strExpression )
{
	bool bOK = true;
	if (strExpression.GetLength() > 0)
	{
		int nPos1 = strExpression.Find(L'[',0);
		int nPos2 = strExpression.Find(L']',1);
		if (nPos1 < 0)
		{
			bOK = false;
		}
		else
		{
			if (nPos2 < 0)
			{
				bOK = false;
			}
		}
		if (strExpression.Find(L">",0) < 0)
		{
			if (strExpression.Find(L">=",0) < 0)
			{
				if (strExpression.Find(L"<",0) < 0)
				{
					if (strExpression.Find(L"<=",0) < 0)
					{
						if (strExpression.Find(L"==",0) < 0)
						{
							if (strExpression.Find(L"!=",0) < 0)
							{
								bOK = false;
							}
						}
					}
				}
			}
		}
		int nPos3 = strExpression.ReverseFind(L']');
		int nPos4 = strExpression.ReverseFind(L'[');
		if (nPos3 > 0 && nPos4 > 0)
		{
			CString strMid = m_strEnableExpression.Mid(nPos4+1,nPos3-nPos4-1);
			if (strMid.FindOneOf(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")<0)
			{
				bOK = false;
			}
		}
		else if (nPos3 < 0 && nPos4 > 0)
		{
			bOK = false;
		}
		else if (nPos3 > 0 && nPos4 < 0)
		{
			bOK = false;
		}
	}
	return bOK;
}

bool CEqmDrawButtonPlus::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawButtonPlus* pDest = (CEqmDrawButtonPlus*)pBase;
		if(pDest)
		{
			SetDefaultBitmap(pDest->GetDefaultBitmap());
			SetTextColor(pDest->GetTextColor());
			SetTextSize(pDest->GetTextSize());
			SetTextContent(pDest->GetTextContent());

			SetButtonStyle(pDest->GetButtonStyle());
			SetValueForUnCheck(pDest->GetValueForUnCheck());
			SetValueForCheck(pDest->GetValueForCheck());
			SetBindPointName(pDest->GetBindPointName());

			SetGroupID(pDest->GetGroupID());
			SetExpression(pDest->GetExpression());
			SetPathUncheckCom(pDest->GetPathUncheckCom());
			SetPathUncheckOver(pDest->GetPathUncheckOver());
			SetPathCheckCom(pDest->GetPathCheckCom());
			SetPathCheckOver(pDest->GetPathCheckOver());
		}
		return true;
	}
	return false;
}

Image* CEqmDrawButtonPlus::GetDefaultBitmap()
{
	return m_pBitmap;
}
