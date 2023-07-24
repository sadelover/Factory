

#include "stdafx.h"
#include "EqmDrawDevice.h"
#include "Tools/CustomTools/CustomTools.h"
//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
#define DEFALTANGEL 0
#define DEFAULTSCALE 1

#define DEFAULTWIDTH 60
#define DEFAULTHEIGHT 40
#pragma warning(disable:4244)

#define SCALE_MAX	5
#define SCALE_MIN	0.1


#define OFFSETPICTURE 2 //鼠标单击事件

CEqmDrawDevice::CEqmDrawDevice()
:	CEqmDrawBase(),
	m_fRotateAngel(DEFALTANGEL),
	m_strEqmName(TEXT("")),
	m_subtype(SUBTYPE_UNKNOWN),
	m_dStepInterval(DEFALTANGEL),
	m_nLinkPageID(0),
	m_nPicID(0)
{
	SetWidth(DEFAULTWIDTH);
	SetHeight(DEFAULTHEIGHT);

	m_dUpperLimit = 0;
	m_dDownLimit  = 0;
	m_vecPortPoints.clear();
	m_bIsClicked = false;
    m_bShow = true;
	m_nscale = 1.0;
	SetDefaultValue();
	m_nTimeTotal = 0;
	m_pBmpName = NULL;
	//m_dScale = (double)m_nWidth/(double)m_nHeight;
}

CEqmDrawDevice::CEqmDrawDevice( EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid,
							   int id, int datacomid, int rotateangle, 
							   wstring strEqmName, EQUIPMENTSUBTYPE subtype, int width, string strPointList,
							   int height, double dStepInterval, double dUpperLimit, double dDownLimit, int nBindType)
:	CEqmDrawBase(type, centerpos, pageid, id, width, height),
	m_fRotateAngel(rotateangle),
	m_strEqmName(strEqmName),
	m_subtype(subtype),
	m_dStepInterval(dStepInterval),
	m_dUpperLimit(dUpperLimit),
	m_dDownLimit(dDownLimit),
	m_nBindType(nBindType),
	m_nLinkPageID(0),
	m_nPicID(0),
	m_bIsPicture(true)
	
{
    SetDataComID(datacomid);
	ParsePointListFromString(strPointList, m_vecPortPoints);
    
	m_eState = EQUIPMENT_START;
	m_bIsClicked = false;
	m_bShow = true;
	m_nValue = 0;
	SetDefaultValue();
	m_nscale = 1.0;
	m_nTimeTotal = 0;
	m_pBmpName = NULL;
	m_dScale = (double)m_nWidth/(double)m_nHeight;
	
}

int CEqmDrawDevice::GetRotateAngle() const
{
	return m_fRotateAngel;
}

void CEqmDrawDevice::SetRotateAngle( int angle )
{
	m_fRotateAngel= angle;
	m_bNeedSave = true;
}

wstring CEqmDrawDevice::GetEqmName() const
{
	return m_strEqmName;
}

void CEqmDrawDevice::SetEqmName(const wstring& strEqmName)
{
	m_strEqmName = strEqmName;
	m_bNeedSave = true;
}

EQUIPMENTSUBTYPE CEqmDrawDevice::GetEquipmentSubType() const
{
	return m_subtype;
}

void CEqmDrawDevice::SetEquipmentSubType( EQUIPMENTSUBTYPE subtype )
{
	m_subtype = subtype;
	m_bNeedSave = true;
}

void	CEqmDrawDevice::SetEqmType(EQMDRAWTYPE type)
{
    __super::SetEqmType(type);
	m_bNeedSave = true;
}

void CEqmDrawDevice::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap)
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
		if (pBitmap && m_nValue == -99)
		{
			m_base_height = pBitmap->GetHeight();
			m_base_width  = pBitmap->GetWidth();

			pGraphics->DrawImage(pBitmap, m_pos.X+nOffSet, m_pos.Y+nOffSet, (Gdiplus::REAL)GetWidth(), (Gdiplus::REAL)GetHeight());
			ResetRotateBmp(pBitmap);
			return;
		}
	}
	else
	{
		CString m_strSysPath;
		Project::Tools::GetSysPath(m_strSysPath);
		m_strSysPath += L"\\Images\\Error\\404.png";
		Image bitImage(m_strSysPath);
		UINT width = bitImage.GetWidth();
		UINT height = bitImage.GetHeight();
		pGraphics->DrawImage(&bitImage, m_pos.X+nOffSet, m_pos.Y+nOffSet, (Gdiplus::REAL)GetWidth(), (Gdiplus::REAL)GetHeight());
	}
	return;
}

CEqmDrawDevice::~CEqmDrawDevice()
{
}

bool CEqmDrawDevice::PointInMe(const Gdiplus::PointF& actPt) const
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

Direction CEqmDrawDevice::PointIsSide(const Gdiplus::PointF& actPt) 
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

void CEqmDrawDevice::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
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

bool CEqmDrawDevice::ChangeSize(const  Gdiplus::PointF& actPt, Direction dir )
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

double CEqmDrawDevice::GetStepInterval()
{	
	return m_dStepInterval;
}

void CEqmDrawDevice::SetStepInterval( double stepInterval )
{
	m_dStepInterval = stepInterval;
	m_bNeedSave = true;
}
bool CEqmDrawDevice::GetIsClicked()
{
	return m_bIsClicked;
}

void CEqmDrawDevice::SetIsClicked( bool bIsClicked )
{
	m_bIsClicked = bIsClicked;
}

void CEqmDrawDevice::RotetaBitMap( Image *pBitMap /*= NULL*/ )
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

void CEqmDrawDevice::ResetRotateBmp(Image *pBitMap )
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

void CEqmDrawDevice::SetUpperLimit( double dUpperLimit )
{
	m_dUpperLimit = dUpperLimit;
	m_bNeedSave = true;
}

double CEqmDrawDevice::GetUpperLimit()
{
	return m_dUpperLimit;
}

void CEqmDrawDevice::SetDownLimit( double dDownLimit )
{
	m_dDownLimit = dDownLimit;
	m_bNeedSave = true;
}

double CEqmDrawDevice::GetDownLimit()
{
	return m_dDownLimit;
}

bool CEqmDrawDevice::CopyProperty( CEqmDrawBase *pBase )
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

bool CEqmDrawDevice::LeftRotate90()
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

bool CEqmDrawDevice::RightRotate90()
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

bool CEqmDrawDevice::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawDevice::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawDevice::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawDevice::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

CEqmDrawDevice* CEqmDrawDevice::Copy()
{
	m_bNeedSave = true;
	return new  CEqmDrawDevice(*this);
}

void CEqmDrawDevice::SetDeviceValue( const int nValue )
{
	m_nValue = nValue;
}

void CEqmDrawDevice::SetEventInfo( const CString& strName, const DeviceTypeForMeta deviceType, const MouseEventTypeForMeta mouseEventType, const int nDeviceID, const int nPageID, const int nValue, const CString strShowName, const CString strOutLink)
{
	m_eventInfo.strSettingPointName = strName;
	m_eventInfo.deviceType = deviceType;
	m_eventInfo.mouseEventType = mouseEventType;
	m_eventInfo.nDeviceID = nDeviceID;
	m_eventInfo.nPageID = nPageID;
	m_eventInfo.nValue = nValue;
	m_eventInfo.strShowName = strShowName;
	m_eventInfo.strOutLink = strOutLink;
	m_bNeedSave = true;
}

EventInfo& CEqmDrawDevice::GetEventInfo()
{
	return m_eventInfo;
}

void CEqmDrawDevice::ParseEvent( const CString& strEvent )
{
	vector<wstring> vecContent;
	SplitStringByChar(strEvent, L',', vecContent);
	if (vecContent.size() == 8)
	{
		m_eventInfo.strSettingPointName = vecContent[0].c_str();
		m_eventInfo.deviceType = (DeviceTypeForMeta)_wtoi(vecContent[1].c_str());
		m_eventInfo.mouseEventType = (MouseEventTypeForMeta)_wtoi(vecContent[2].c_str());
		m_eventInfo.nDeviceID = _wtoi(vecContent[3].c_str());
		m_eventInfo.nPageID = _wtoi(vecContent[4].c_str());
		m_eventInfo.nValue = _wtoi(vecContent[5].c_str());
		m_eventInfo.strShowName = vecContent[6].c_str();
		m_eventInfo.strOutLink = vecContent[7].c_str();
	}

	if (vecContent.size() == 7)
	{
		m_eventInfo.strSettingPointName = vecContent[0].c_str();
		m_eventInfo.deviceType = (DeviceTypeForMeta)_wtoi(vecContent[1].c_str());
		m_eventInfo.mouseEventType = (MouseEventTypeForMeta)_wtoi(vecContent[2].c_str());
		m_eventInfo.nDeviceID = _wtoi(vecContent[3].c_str());
		m_eventInfo.nPageID = _wtoi(vecContent[4].c_str());
		m_eventInfo.nValue = _wtoi(vecContent[5].c_str());
		m_eventInfo.strShowName = vecContent[6].c_str();
	}
	if (vecContent.size() == 6)
	{
		m_eventInfo.strSettingPointName = vecContent[0].c_str();
		m_eventInfo.deviceType = (DeviceTypeForMeta)_wtoi(vecContent[1].c_str());
		m_eventInfo.mouseEventType = (MouseEventTypeForMeta)_wtoi(vecContent[2].c_str());
		m_eventInfo.nDeviceID = _wtoi(vecContent[3].c_str());
		m_eventInfo.nPageID = _wtoi(vecContent[4].c_str());
		m_eventInfo.nValue = _wtoi(vecContent[5].c_str());
		m_eventInfo.strShowName = L"";
	}
}

const int CEqmDrawDevice::GetDeviceValue()
{
	return m_nValue;
}

void CEqmDrawDevice::SetDefaultValue()
{
	m_nValue = -99;
}


void CEqmDrawDevice::AddBindInfo(const int nValue, const int nFrame, const int nInterval, const int nPicID)
{
	BindInfo info;
	info.nValue = nValue;
	info.nFrame = nFrame;
	info.nPicID = nPicID;
	info.nInterval = nInterval;
	m_vecBindInfo.push_back(info);
}

void CEqmDrawDevice::ParseParam10(const CString& strParam10 )
{
	int nValue = 0;
	int nPicID = 0;
	int nFrame = 0;
	int nInterval = 0;

	vector<wstring> vecContent;
	SplitStringByChar(strParam10, L'|', vecContent);
	for (size_t i=0;i<vecContent.size();++i)
	{
		if (vecContent[i].size() > 0)
		{
			vector<wstring> vecContentSub;
			SplitStringByChar(vecContent[i].c_str(), L',', vecContentSub);
			nValue = _wtoi(vecContentSub[0].c_str());
			nPicID = _wtoi(vecContentSub[1].c_str());
			nFrame = _wtoi(vecContentSub[2].c_str());
			nInterval = _wtoi(vecContentSub[3].c_str());
			AddBindInfo(nValue,nFrame,nInterval,nPicID);
		}
	}
	m_bNeedSave = true;
}

void CEqmDrawDevice::ParseParam10( const CString& strParam10,const int nOffSet )
{
	int nValue = 0;
	int nPicID = 0;
	int nFrame = 0;
	int nInterval = 0;

	vector<wstring> vecContent;
	SplitStringByChar(strParam10, L'|', vecContent);
	for (size_t i=0;i<vecContent.size();++i)
	{
		if (vecContent[i].size() > 0)
		{
			vector<wstring> vecContentSub;
			SplitStringByChar(vecContent[i].c_str(), L',', vecContentSub);
			nValue = _wtoi(vecContentSub[0].c_str());
			nPicID = _wtoi(vecContentSub[1].c_str()) + nOffSet;
			nFrame = _wtoi(vecContentSub[2].c_str());
			nInterval = _wtoi(vecContentSub[3].c_str());
			AddBindInfo(nValue,nFrame,nInterval,nPicID);
		}
	}
	m_bNeedSave = true;
}

PlayInfo& CEqmDrawDevice::GetPlayInfo( const int nValue, bool& bOK)
{
	const size_t size = m_vecPlayInfo.size();
	for (size_t i=0;i<size;++i)
	{
		if (m_vecPlayInfo[i].nValue == nValue)
		{
			bOK = true;
			return m_vecPlayInfo[i];
		}
	}
	bOK = false;
}

MouseEventTypeForMeta CEqmDrawDevice::GetEventType() const
{
	return m_eventInfo.mouseEventType;
}

bool CEqmDrawDevice::IsEventEmpty() const
{
	if (GetEqmType() == EQMDRAW_METAFILE)
	{
		return (m_eventInfo.mouseEventType == enumNullWnd);
	}
	else
	{
		return false;
	}
}


void CEqmDrawDevice::SetBindString( const wstring strBind )
{
	m_bindString = strBind;
	m_bNeedSave = true;
}

wstring CEqmDrawDevice::GetBindString()
{
	return m_bindString;
}

void CEqmDrawDevice::SetBitmap( Image* pBitmap )
{
	ASSERT(pBitmap);

	m_pBmpName = pBitmap;
}

Image* CEqmDrawDevice::GetBitmap()
{
	if (m_pBmpName)
	{
		return m_pBmpName;
	}
	return NULL;
}

void	CEqmDrawDevice::SetLinkPageID(const int nPageID)
{
	m_nLinkPageID = nPageID;
	m_bNeedSave = true;
}

int CEqmDrawDevice::GetLinkPageID()
{
	return m_nLinkPageID;
}

void CEqmDrawDevice::SetPicID( const int nPicID )
{
	m_nPicID = nPicID;
	m_bNeedSave = true;
}

int CEqmDrawDevice::GetPicID()
{
	return m_nPicID;
}

vector<BindInfo> CEqmDrawDevice::GetBindInfoVec()
{
	return m_vecBindInfo;
}


wstring    CEqmDrawDevice:: GetOutsideLink()
{
	return m_strOutsideLink ;
}

void  CEqmDrawDevice::SetOutsideLink(wstring wstrLink)
{
	m_strOutsideLink = wstrLink;
}

void CEqmDrawDevice::ClearDiagnoseVec()
{
	m_bNeedSave = true;
	m_vecDiagnosePage.clear();
}

void CEqmDrawDevice::InsertDiagnoseVec( const int nPageID, const CString strPageName )
{
	m_bNeedSave = true;
	PageIDName item;
	item.nPageID = nPageID;
	item.strPageName = strPageName;
	m_vecDiagnosePage.push_back(item);
}

const vector<PageIDName>& CEqmDrawDevice::GetDiagnosePageVec()
{
	return m_vecDiagnosePage;
}

const bool CEqmDrawDevice::IsForDiagnose() const
{
	return m_vecDiagnosePage.size()>0?true:false;
}

void CEqmDrawDevice::InitDiagnoseVec( const int nItemCount )
{
	for (int i=0;i<nItemCount;++i)
	{
		vector<NameValue> vec;
		m_strDiagnoseItemPointNameVec.push_back(vec);
		m_bChangedVec.push_back(false);
	}
}

void CEqmDrawDevice::InsertDiagnosePageItemName( const int nItem, const NameValue st )
{
	if (nItem<(int)m_strDiagnoseItemPointNameVec.size())
	{
		m_strDiagnoseItemPointNameVec[nItem].push_back(st);
	}
}

void CEqmDrawDevice::ClearDiagnosePageItemVec()
{
	m_strDiagnoseItemPointNameVec.clear();
}

vector<vector<NameValue>>& CEqmDrawDevice::GetDiagnosePageItemVec()
{
	return m_strDiagnoseItemPointNameVec;
}

const bool CEqmDrawDevice::IsDiagnoseAnimation()
{
	for (size_t i=0;i<m_bChangedVec.size();++i)
	{
		if (m_bChangedVec[i])
		{
			return true;
		}
	}
	return false;
}

const int CEqmDrawDevice::GetDiagnosePageIndex( const int nPageID )
{
	for (size_t i=0;i<m_vecDiagnosePage.size();++i)
	{
		if (m_vecDiagnosePage[i].nPageID == nPageID)
		{
			return i;
		}
	}
	return -1;
}

bool CEqmDrawDevice::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawDevice::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawDevice::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawDevice::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

void CEqmDrawDevice::SetImageMetaType(const bool bIsPic)
{
	m_bIsPicture = bIsPic;
}

bool CEqmDrawDevice::GetImageMetaType(void) const
{
	return m_bIsPicture;
}

bool CEqmDrawDevice::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//特有属性

		CEqmDrawDevice* pDevice = (CEqmDrawDevice*)pBase;
		if(pDevice)
		{
			SetRotateAngle(pDevice->GetRotateAngle());
			SetEqmName(pDevice->GetEqmName());
			SetEquipmentSubType(pDevice->GetEquipmentSubType());
			SetStepInterval(pDevice->GetStepInterval());
			SetIsClicked(pDevice->GetIsClicked());
			SetUpperLimit(pDevice->GetUpperLimit());
			SetDownLimit(pDevice->GetDownLimit());
			
			SetDeviceValue(pDevice->GetDeviceValue());
			SetBindInfoVec(pDevice->GetBindInfoVec());  
			
			SetBindString(pDevice->GetBindString());
			SetBitmap(pDevice->GetBitmap());
			SetLinkPageID(pDevice->GetLinkPageID());
			SetOutsideLink(pDevice->GetOutsideLink());
			SetPicID(pDevice->GetPicID());
			SetImageMetaType(pDevice->GetImageMetaType());
			
			SetDiagnosePageVec(pDevice->GetDiagnosePageVec());
			SetDiagnosePageItemVec(pDevice->GetDiagnosePageItemVec());

			m_eventInfo = pDevice->m_eventInfo;
		}

		return true;
	}
	return false;
}

void CEqmDrawDevice::SetBindInfoVec( vector<BindInfo> vec )
{
	m_vecBindInfo = vec;
}

void CEqmDrawDevice::SetDiagnosePageVec(const vector<PageIDName>& vec )
{
	m_vecDiagnosePage = vec;
}

void CEqmDrawDevice::SetDiagnosePageItemVec( vector<vector<NameValue>>& vec )
{
	m_strDiagnoseItemPointNameVec = vec;
}

bool CEqmDrawDevice::CopyInFormatBrush(CEqmDrawBase *pBase)
{
	CEqmDrawDevice* pDevice = dynamic_cast<CEqmDrawDevice*>(pBase);
	if (pDevice != NULL && pDevice->GetEqmType() == m_eEqmType)
	{
		SetHeight(pDevice->GetHeight());
		SetWidth(pDevice->GetWidth());
		SetLayer(pDevice->GetLayer());

		SetPicID(pDevice->GetPicID());
		SetBitmap(pDevice->GetBitmap());
		SetBindInfoVec(pDevice->GetBindInfoVec());

		m_bNeedSave = true;
		return true;
	}
	return false;
}
int CEqmDrawDevice::GetBindType()
{
	return m_nBindType;
}
void CEqmDrawDevice::SetBindType(int nBindType)
{
	m_nBindType=nBindType;
}