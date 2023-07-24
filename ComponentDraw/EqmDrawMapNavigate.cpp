#include "StdAfx.h"
#include "EqmDrawMapNavigate.h"
#include "Tools/CustomTools/CustomTools.h"
//////////////////////////////////////////////////////////////////////////
//CCoorInfo::CCoorInfo(void) : m_enumFaceType(east), m_dValue(0)
//{
//}
//
//CCoorInfo::~CCoorInfo(void)
//{
//}
//
//CString	CCoorInfo::GetCoordinateName(void) const
//{
//	CString	strShow;
//	switch (m_enumFaceType)
//	{
//	case east:
//		strShow.Format(_T("E%0.4f"), m_dValue);
//		break;
//	case west:
//		strShow.Format(_T("W%0.4f"), m_dValue);
//		break;
//	case south:
//		strShow.Format(_T("S%0.4f"), m_dValue);
//		break;
//	case north:
//		strShow.Format(_T("N%0.4f"), m_dValue);
//		break;
//	default:
//		break;
//	}
//
//	return strShow;
//}
//
//FaceType CCoorInfo::GetFaceType(void) const
//{
//	return m_enumFaceType;
//}
//
//double CCoorInfo::GetValue(void) const
//{
//	return m_dValue;
//}
//
//void CCoorInfo::SetCoordinate(const FaceType& factType, const double dValue)
//{
//	m_enumFaceType = factType;
//	m_dValue = dValue;
//}

CMapPageInfo::CMapPageInfo(void)
	: m_nPageId(0)
	, m_dLongitude1(0)
	, m_dLongitude2(0)
	, m_dLatitude1(0)
	, m_dLatitude2(0)
{
	
}

CMapPageInfo::~CMapPageInfo(void)
{
}

//CCoorInfo CMapPageInfo::GetLongitude(const int nNum) const
//{
//	CCoorInfo corInf;
//	switch (nNum)
//	{
//	case 1:
//		corInf = m_longitude1;
//		break;
//	case 2:
//		corInf = m_longitude2;
//		break;
//	default:
//		break;
//	}
//
//	return corInf;
//}
//
//CCoorInfo CMapPageInfo::GetLatitude(const int nNum) const
//{
//	CCoorInfo corInf;
//	switch (nNum)
//	{
//	case 1:
//		corInf = m_latitude1;
//		break;
//	case 2:
//		corInf = m_latitude2;
//		break;
//	default:
//		break;
//	}
//
//	return corInf;
//}

int CMapPageInfo::GetPageId(void) const
{
	return m_nPageId;
}

//CString CMapPageInfo::GetLongitudeName(const int nNum) const
//{
//	CString	strShow;
//	switch (nNum)
//	{
//	case 1:
//		strShow = m_longitude1.GetCoordinateName();
//		break;
//	case 2:
//		strShow = m_longitude2.GetCoordinateName();
//		break;
//	default:
//		break;
//	}
//
//	return strShow;
//}
//
//CString CMapPageInfo::GetLatitudeName(const int nNum) const
//{
//	CString	strShow;
//	switch (nNum)
//	{
//	case 1:
//		strShow = m_latitude1.GetCoordinateName();
//		break;
//	case 2:
//		strShow = m_latitude2.GetCoordinateName();
//		break;
//	default:
//		break;
//	}
//
//	return strShow;
//}
//
//void CMapPageInfo::SetLongitude(const int nNum, const CCoorInfo corVal)
//{
//	switch (nNum)
//	{
//	case 1:
//		m_longitude1 = corVal;
//		break;
//	case 2:
//		m_longitude2 = corVal;
//		break;
//	default:
//		break;
//	}
//}
//
//void CMapPageInfo::SetLatitude(const int nNum, const CCoorInfo corVal)
//{
//	switch (nNum)
//	{
//	case 1:
//		m_latitude1 = corVal;
//		break;
//	case 2:
//		m_latitude2 = corVal;
//		break;
//	default:
//		break;
//	}
//}

void CMapPageInfo::SetPageId(const int nPageId)
{
	m_nPageId = nPageId;
}

//void CMapPageInfo::SetLongitudeFromName(const int nNum, const CString strLongitudeName)
//{
//	CString		strName(strLongitudeName);
//	CString		strFaceType;
//	FaceType	faceType;
//	CString		strVal;
//	double		dVal = 0;
//	CCoorInfo	corInf;
//
//	strFaceType = strName.Left(1);
//	if (_T("E") == strFaceType || _T("e") == strFaceType)
//	{
//		faceType = east;
//	}
//	else if (_T("W") == strFaceType || _T("w") == strFaceType)
//	{	
//		faceType = west;
//	}
//	else
//	{	// error face value
//		return;
//	}
//
//	strVal = strName.Mid(1, strName.GetLength()-1);
//	dVal = _ttol(strVal);
//
//	corInf.SetCoordinate(faceType, dVal);
//
//	switch (nNum)
//	{
//	case 1:
//		m_longitude1 = corInf;
//		break;
//	case 2:
//		m_longitude2 = corInf;
//		break;
//	default:
//		break;
//	}
//}
//
//void CMapPageInfo::SetLatitudeFromName(const int nNum, const CString strLatitudeName)
//{
//	CString		strName(strLatitudeName);
//	CString		strFaceType;
//	FaceType	faceType;
//	CString		strVal;
//	double		dVal = 0;
//	CCoorInfo	corInf;
//
//	strFaceType = strName.Left(1);
//	if (_T("S") == strFaceType || _T("s") == strFaceType)
//	{
//		faceType = south;
//	}
//	else if (_T("N") == strFaceType || _T("n") == strFaceType)
//	{	
//		faceType = north;
//	}
//	else
//	{	// error face value
//		return;
//	}
//
//	strVal = strName.Mid(1, strName.GetLength()-1);
//	dVal = _ttol(strVal);
//
//	corInf.SetCoordinate(faceType, dVal);
//
//	switch (nNum)
//	{
//	case 1:
//		m_latitude1 = corInf;
//		break;
//	case 2:
//		m_latitude2 = corInf;
//		break;
//	default:
//		break;
//	}
//}

double CMapPageInfo::GetLongitudeValue(const int nNum) const
{
	double dValue = 0;
	switch (nNum)
	{
	case 1:
		dValue = m_dLongitude1;
		break;
	case 2:
		dValue = m_dLongitude2;
		break;
	default:
		break;
	}

	return dValue;
}

double CMapPageInfo::GetLatitudeValue(const int nNum) const
{
	double dValue = 0;
	switch (nNum)
	{
	case 1:
		dValue = m_dLatitude1;
		break;
	case 2:
		dValue = m_dLatitude2;
		break;
	default:
		break;
	}

	return dValue;
}

void CMapPageInfo::SetLongitudeValue(const int nNum, const double dVal)
{
	switch (nNum)
	{
	case 1:
		m_dLongitude1 = dVal;
		break;
	case 2:
		m_dLongitude2 = dVal;
		break;
	default:
		break;
	}
}

void CMapPageInfo::SetLatitudeValue(const int nNum, const double dVal)
{
	switch (nNum)
	{
	case 1:
		m_dLatitude1 = dVal;
		break;
	case 2:
		m_dLatitude2 = dVal;
		break;
	default:
		break;
	}
}


//////////////////////////////////////////////////////////////////////////


CEqmDrawMapNavigate::CEqmDrawMapNavigate(void)
	: m_pBitmap(NULL)
	, m_nScaleRuler(0)
	, m_bBanOnDispay(false)
{
	m_fscale = (double)m_nWidth / (double)m_nHeight;

	m_fRotateAngel = 0.0;
	m_cstrTemplateName = L"";
}


CEqmDrawMapNavigate::~CEqmDrawMapNavigate(void)
{
}

CEqmDrawMapNavigate* CEqmDrawMapNavigate::Copy(void)
{
	m_bNeedSave = true;
	return new CEqmDrawMapNavigate(*this);
}

void CEqmDrawMapNavigate::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/)
{
	const EQMDRAWTYPE picType = GetEqmType();
	if (picType == EQMDRAW_UNKNOWN)
	{
		return;
	}
	if (m_pBitmap != NULL)
	{
		RotetaBitMap(m_pBitmap);
		m_nBaseWidth  = m_pBitmap->GetWidth();
		m_nBaseHeight = m_pBitmap->GetHeight();
		pGraphics->DrawImage(m_pBitmap, m_pos.X, m_pos.Y, (Gdiplus::REAL)GetWidth(), (Gdiplus::REAL)GetHeight());
		ResetRotateBmp(m_pBitmap);
	}
	return;
}

bool CEqmDrawMapNavigate::PointInMe(const Gdiplus::PointF& actPt) const
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

Direction CEqmDrawMapNavigate::PointIsSide(const Gdiplus::PointF& actPt)
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

void CEqmDrawMapNavigate::DrawSelectEdge(Gdiplus::Graphics *pGraphics)
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

bool CEqmDrawMapNavigate::ChangeSize(const Gdiplus::PointF& actPt, Direction dir)
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

bool CEqmDrawMapNavigate::CopyProperty(CEqmDrawBase *pBase)
{
	return false;
}

void CEqmDrawMapNavigate::SetDefaultBitmap(Image* pBitmap)
{
	if (pBitmap != NULL)
	{
		m_pBitmap = pBitmap;
	}
}

bool CEqmDrawMapNavigate::ShiftEqmtsUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawMapNavigate::ShiftEqmtsDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	return true;
}

bool CEqmDrawMapNavigate::ShiftEqmtsLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawMapNavigate::ShiftEqmtsRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	return true;
}

bool CEqmDrawMapNavigate::ShiftEqmtsCtrlUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawMapNavigate::ShiftEqmtsCtrlDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	return true;
}

bool CEqmDrawMapNavigate::ShiftEqmtsCtrlLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

bool CEqmDrawMapNavigate::ShiftEqmtsCtrlRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	return true;
}

void CEqmDrawMapNavigate::SetScaleRuler(const int nScaleRuler)
{
	m_nScaleRuler = nScaleRuler;
}

void CEqmDrawMapNavigate::SetMapPageInfo(const vector<CMapPageInfo>& vecMapPageInfo)
{
	m_vecMapPageInfo = vecMapPageInfo;
}

int CEqmDrawMapNavigate::GetPicId(void) const
{
	return g_nPicIndexMapNavigate;
}

int CEqmDrawMapNavigate::GetScaleRuler(void) const
{
	return m_nScaleRuler;
}

vector<CMapPageInfo> CEqmDrawMapNavigate::GetMapPageInfo(void) const
{
	return m_vecMapPageInfo;
}

Image* CEqmDrawMapNavigate::GetDefaultBitmap()
{
	return m_pBitmap;
}

void CEqmDrawMapNavigate::SetEquipName(const CString strEquipName )
{
	m_strEquipName= strEquipName;
}
void CEqmDrawMapNavigate::SetRoomName(const CString strRoomName )
{
	m_strRoomName= strRoomName;

}
void CEqmDrawMapNavigate::SetEquipType(int nEquipType)
{
	m_nEquiptype =nEquipType;
}
void CEqmDrawMapNavigate::SetEquipNo(int nEquipNo)
{
	m_nEquipNo = nEquipNo;
}
void CEqmDrawMapNavigate::SetEquipStyle(int nEquipStyle)
{
	m_nEquipStyle = nEquipStyle;
}

void  CEqmDrawMapNavigate::SetEquipTemplatePageId(const int nID)
{
	m_nTemplatePageId = nID;
}

void  CEqmDrawMapNavigate::SetEquipTemplateGroupId(const int nID)
{
	m_nTemplateGroupId = nID;
}

void  CEqmDrawMapNavigate::SetEquipTemplatePageName(CString strName)
{
	m_strTemplatePageName = strName;
}
void  CEqmDrawMapNavigate::SetEquipParamList(vector<CString> paramList)
{
	m_strParamList = paramList;
}
CString CEqmDrawMapNavigate::GetEquipName(void) const
{
	return m_strEquipName;
}
CString CEqmDrawMapNavigate::GetRoomName(void) const
{
	return m_strRoomName;

}
int CEqmDrawMapNavigate::GetEquipType(void)const
{
	return m_nEquiptype;
}
int CEqmDrawMapNavigate::GetEquipNo(void)const
{
	return m_nEquipNo;
}

int CEqmDrawMapNavigate::GetEquipStyle(void)const
{
	return m_nEquipStyle;
}

int CEqmDrawMapNavigate::GetEquipTemplatePageId(void) const
{
	return m_nTemplatePageId;
}

int CEqmDrawMapNavigate::GetEquipTemplateGroupId(void) const
{
	return m_nTemplateGroupId;
}

void CEqmDrawMapNavigate::SetTemplateName(const CString cstrTemplateName)
{
	m_cstrTemplateName = cstrTemplateName;
}

CString CEqmDrawMapNavigate::GetTemplateName(void) const
{
	return m_cstrTemplateName;
}

void CEqmDrawMapNavigate::SetBanOnDisplay(const bool bEnable)
{
	m_bBanOnDispay = bEnable;
}
bool CEqmDrawMapNavigate::GetBanOnDisplay(void)const
{
	return m_bBanOnDispay;
}

CString CEqmDrawMapNavigate::GetEquipTemplatePageName(void) const
{
	return m_strTemplatePageName;
}



CString CEqmDrawMapNavigate::GetParamListAllString()
{
	CString strAll = L"";
	for(int i=0;i<m_strParamList.size();i++)
	{
		strAll+=m_strParamList[i];
		if((i+1)< m_strParamList.size())
		    {
				strAll+=_T(",");
		}
	}

	

	return strAll;
}

void CEqmDrawMapNavigate::SetParamListAllString(CString strAll)
{
	m_strParamList.clear();
	Project::Tools::SplitStringByChar(strAll, ',', m_strParamList);

}


bool CEqmDrawMapNavigate::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//ÌØÓÐÊôÐÔ

		CEqmDrawMapNavigate* pDest = (CEqmDrawMapNavigate*)pBase;
		if(pDest)
		{
			SetDefaultBitmap(pDest->GetDefaultBitmap());
			SetScaleRuler(pDest->GetScaleRuler());
			SetMapPageInfo(pDest->GetMapPageInfo());
		}
		return true;
	}
	return false;
}

int CEqmDrawMapNavigate::GetRotateAngle()
{
	return m_fRotateAngel;
}

void CEqmDrawMapNavigate::SetRotateAngle( int angle )
{
	m_fRotateAngel= angle;
	m_bNeedSave = true;
}

void CEqmDrawMapNavigate::RotetaBitMap( Image *pBitMap /*= NULL*/ )
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

void CEqmDrawMapNavigate::ResetRotateBmp(Image *pBitMap )
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



bool CEqmDrawMapNavigate::LeftRotate90()
{
	int temp = GetRotateAngle()-90;
	int width  = GetWidth();
	int height = GetHeight();
	float rotAngle = temp == -360 ? 0:temp;

	SetRotateAngle(rotAngle);
	SetWidth(height);
	SetHeight(width);
	m_bNeedSave = true;
	return true;
}

bool CEqmDrawMapNavigate::RightRotate90()
{
	int temp   = GetRotateAngle()+90;
	int width  = GetWidth();
	int height = GetHeight();
	float rotAngle = temp == 360 ? 0:temp;

	SetRotateAngle(rotAngle);
	SetWidth(height);
	SetHeight(width);
	m_bNeedSave = true;
	return true;
}
