

#include "stdafx.h"
#include "EqmDrawBase.h"
#include <sstream>
using std::ostringstream;

#pragma warning(disable:4244)
CEqmDrawBase::CEqmDrawBase()
:m_layer(E_LAYER_10)
,m_frameCurrrent(0)
,m_frameMax(1)
,m_eState(EQUIPMENT_START)
,m_iDataComID(DEFAULTCOMID)
,m_bDeleteFlag(false)
{
	m_pos.X = 0;
	m_pos.Y = 0;
	m_eEqmType = EQMDRAW_UNKNOWN;
	m_iPageID = 0;
	m_iID = 0;
	m_bIsMainAlign = false;
	m_bIsSelect    = false;
	m_nWidth = 0;
	m_nHeight = 0;
	m_bShow   = true;
    m_layer = E_LAYER_5;
	m_advancedstate = 0;
	m_bNeedSave = true;
	m_dScale = 0;
}

CEqmDrawBase::CEqmDrawBase( EQMDRAWTYPE type, const Gdiplus::PointF& pos, 
							int pageid, int ident, int width, int height, UINT layer )
:m_eEqmType(type),
m_pos(pos),
m_iPageID(pageid),
m_iID(ident),
m_nWidth(width),
m_nHeight(height),
m_bIsMainAlign(false),
m_bIsSelect(false),
m_bShow(true)
,m_layer(layer)
,m_frameCurrrent(0)
,m_frameMax(1)
,m_eState(EQUIPMENT_START)
,m_iDataComID(DEFAULTCOMID)
,m_bDeleteFlag(false)
{	
	m_advancedstate = 0;
	m_bNeedSave = false;

	m_posru.X = m_pos.X + m_nWidth;
	m_posru.Y = m_pos.Y;

	m_posld.X = m_pos.X;
	m_posld.Y = m_pos.Y + m_nHeight;

	m_posrd.X = m_pos.X + m_nWidth;
	m_posrd.Y = m_pos.Y + m_nHeight;

	m_dScale = (double)m_nWidth/(double)m_nHeight;
}

const Gdiplus::PointF& CEqmDrawBase::GetPos() const
{
	return m_pos;
}

void CEqmDrawBase::SetPos( const Gdiplus::PointF& pos )
{
	PointF ptPos = pos;
	if (pos.X < 0)
	{
		ptPos.X = 0;
	}
	if (pos.X + m_nWidth > 1920)
	{
		int nX = 1920 - m_nWidth;
		if (nX < 0)
		{
			nX = 0;
		}
		ptPos.X = nX;
	}
	if (pos.Y < 0)
	{
		ptPos.Y = 0;
	}
	if (pos.Y + m_nHeight > 955)
	{
		int nY = 955 - m_nHeight;
		if (nY < 0)
		{
			nY = 0;
		}
		ptPos.Y = nY;
	}


	m_pos = ptPos;
	m_posru.X = m_pos.X + m_nWidth;
	m_posru.Y = m_pos.Y;

	m_posld.X = m_pos.X;
	m_posld.Y = m_pos.Y + m_nHeight;

	m_posrd.X = m_pos.X + m_nWidth;
	m_posrd.Y = m_pos.Y + m_nHeight;
	m_bNeedSave = true;
}

EQMDRAWTYPE CEqmDrawBase::GetEqmType() const
{
	return m_eEqmType;
}

void CEqmDrawBase::SetEqmType( EQMDRAWTYPE type )
{
	m_eEqmType = type;
	m_bNeedSave = true;
}


int CEqmDrawBase::GetPageID() const
{
	return m_iPageID;
}

void CEqmDrawBase::SetPageID( int pageid )
{
	m_iPageID = pageid;
	m_bNeedSave = true;
}

int CEqmDrawBase::GetID() const
{
	return m_iID;
}

void CEqmDrawBase::SetID( int ID )
{
	m_iID= ID;
	m_bNeedSave = true;
}

bool CEqmDrawBase::IsMainAlign()
{
	return m_bIsMainAlign;
}

void CEqmDrawBase::SetMainAlign( bool bIsMainAlign )
{
	m_bIsMainAlign = bIsMainAlign;
	m_bNeedSave = true;
}

bool CEqmDrawBase::IsSelected()
{
	return m_bIsSelect;
}

void CEqmDrawBase::SetSelected( bool bSelected )
{
	m_bIsSelect = bSelected;
}

int CEqmDrawBase::GetWidth() const
{
	return m_nWidth;
}

void CEqmDrawBase::SetWidth( int width )
{
	m_nWidth = width;
	m_bNeedSave = true;
	m_posru.X = m_pos.X + m_nWidth;
	m_posru.Y = m_pos.Y;

	m_posld.X = m_pos.X;
	m_posld.Y = m_pos.Y + m_nHeight;

	m_posrd.X = m_pos.X + m_nWidth;
	m_posrd.Y = m_pos.Y + m_nHeight;

	m_dScale = (double)m_nWidth/(double)m_nHeight;
}

int CEqmDrawBase::GetHeight() const
{
	return m_nHeight;
}

void CEqmDrawBase::SetHeight( int height )
{
	m_nHeight = height;
	m_bNeedSave = true;
	m_posru.X = m_pos.X + m_nWidth;
	m_posru.Y = m_pos.Y;

	m_posld.X = m_pos.X;
	m_posld.Y = m_pos.Y + m_nHeight;

	m_posrd.X = m_pos.X + m_nWidth;
	m_posrd.Y = m_pos.Y + m_nHeight;

	m_dScale = (double)m_nWidth/(double)m_nHeight;
}

CEqmDrawBase::~CEqmDrawBase()
{

}

bool CEqmDrawBase::GetShow()
{
	return m_bShow;
}

void CEqmDrawBase::SetShow( bool bShow )
{
	m_bShow = bShow;
}

Gdiplus::PointF CEqmDrawBase::Rotate( int x, int y, int x0, int y0, double angle )
{
	m_bNeedSave = true;
	if (angle==90)
	{
		return Gdiplus::PointF(-y+y0+x0,x-x0+y0);
	}
	else if (angle ==-90)
	{
		return Gdiplus::PointF(y-y0+x0,-x+x0+y0);
	}
	else 
		return Gdiplus::PointF(x,y);
}

double CEqmDrawBase::GetDimension( int x1, int y1, int x2, int y2 )
{
	double fdis = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
	return sqrt(fdis);
}

Gdiplus::PointF CEqmDrawBase::GetPortPoint( Gdiplus::PointF actPt )
{
	return Gdiplus::PointF(0, 0);
}

void CEqmDrawBase::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
{
	
}

void CEqmDrawBase::DrawPort( Gdiplus::Graphics *pGraphics )
{

}

void CEqmDrawBase::ParsePointListFromString(const string& str, vector<Gdiplus::PointF> &vecPoint) const
{
	if (str.empty())
		return;

	vecPoint.clear();

	//将vector<Gpoint>转换为string
	char* strcopy = _strdup(str.c_str());
	char* token = NULL;
	char* nexttoken;
	const char* sep = ";";
	token = strtok_s(strcopy, sep, &nexttoken);

	while(token != NULL)
	{
		float xpos =0.0, ypos = 0.0;
		sscanf_s(token, "(%f,%f)", &xpos, &ypos);
		vecPoint.push_back(Gdiplus::PointF(xpos, ypos));	
		token = strtok_s(NULL, sep, &nexttoken);
	}
    
    free(strcopy);
}

void CEqmDrawBase::ParsePointListToString(const vector<Gdiplus::PointF>& vecPoint, string & str) const 
{
	if (vecPoint.empty())
		return;

	//将string转换为vector<Gpoint>
	ostringstream pointliststream;

	vector<Gdiplus::PointF>::const_iterator it = vecPoint.begin();
	for (; it != vecPoint.end(); it++)
	{
		pointliststream << "(" 
			<< (*it).X << ","
			<< (*it).Y << ");";
	}

	str = pointliststream.str();
}

bool CEqmDrawBase::IfMeInRect( Gdiplus::RectF rect0, Gdiplus::RectF rectItem )
{
	BOOL ifIntersect = rect0.IntersectsWith(rectItem);
	return (ifIntersect>0);
}

bool CEqmDrawBase::IfMeContainRect(Gdiplus::RectF rect0, Gdiplus::RectF rectItem)
{
	return rect0.Contains(rectItem) ? true : false;
}

bool CEqmDrawBase::LeftRotate90()
{
	return true;
}

bool CEqmDrawBase::RightRotate90()
{
	return true;
}

bool CEqmDrawBase::ShiftEqmtsUp()
{
	return true;
}

bool CEqmDrawBase::ShiftEqmtsDown()
{
	return true;
}

bool CEqmDrawBase::ShiftEqmtsLeft()
{
	return true;
}

bool CEqmDrawBase::ShiftEqmtsRight()
{
	return true;
}

bool CEqmDrawBase::DoMoveEqmt(Gdiplus::PointF prePt, Gdiplus::PointF actPt)
{	
	float x_min = GetPos().X+actPt.X - prePt.X;
	float x_max = GetPos().X+actPt.X - prePt.X+m_nWidth;
	float y_min = GetPos().Y+actPt.Y - prePt.Y;
	float y_max = GetPos().Y+actPt.Y - prePt.Y+m_nHeight;
	if (x_min < 0)
	{
		x_min = 0;
		x_max = x_min + m_nWidth;
	}
	if (x_max > 1920)
	{
		x_max = 1920;
		x_min = 1920 - m_nWidth;
	}
	if (y_min < 0)
	{
		y_min = 0;
		y_max = y_min + m_nHeight;
	}
	if (y_max > 955)
	{
		y_max = 955;
		y_min = y_max - m_nHeight;
	}
	//if ( x_min<0 || x_max>1920 || y_min<0 || y_max>955 )
	//{
	//	return false;
	//}
	SetPos(Gdiplus::PointF(x_min, y_min));
	return true;
}

UINT CEqmDrawBase::GetLayer() const       //bird add 20111008
{
    return m_layer;
}

void CEqmDrawBase::SetLayer(UINT layer)
{
    m_layer = layer;
	m_bNeedSave = true;
}
UINT CEqmDrawBase::GetStyle() const       //bird add 20111008
{
	return m_Style;
}

void CEqmDrawBase::SetStyle(UINT Style)
{
	m_Style = Style;
	m_bNeedSave = true;
}
bool CEqmDrawBase::IsTopLayer() const
{
	return m_layer == E_LAYER_10;
}

void CEqmDrawBase::SetCurrentFrame(int cur)
{
    m_frameCurrrent = cur;
}
int CEqmDrawBase::GetCurrentFrame()
{
    return m_frameCurrrent;
}
void  CEqmDrawBase::SetNextFrame()
{
    m_frameCurrrent++;
    if(m_frameCurrrent >= m_frameMax)
	{
        m_frameCurrrent = 0;
	}
}

int CEqmDrawBase::GetDataComID() const
{
    return m_iDataComID;
}

void CEqmDrawBase::SetDataComID( int id )
{
    m_iDataComID = id;
	m_bNeedSave = true;
}

void CEqmDrawBase::FastDrawLines( HDC hdc, HPEN hpen, const vector<CPoint>& pointlist )
{
	HGDIOBJ prevPen = ::SelectObject(hdc, hpen);
	if (pointlist.size() > 0 )
	{
		CPoint point0 = pointlist[0];
		::MoveToEx(hdc, point0.x, point0.y, NULL);
		for (unsigned int i = 1; i < pointlist.size(); i++)
		{
			::LineTo(hdc, pointlist[i].x, pointlist[i].y);
		}
	}
	::SelectObject(hdc, prevPen);
}

void CEqmDrawBase::FastDrawLine( HDC hdc, HPEN hpen, CPoint pointfrom, CPoint pointto )
{
	HGDIOBJ prevPen = ::SelectObject(hdc, hpen);
	::MoveToEx(hdc, pointfrom.x, pointfrom.y, NULL);
	::LineTo(hdc, pointto.x, pointto.y);
	::SelectObject(hdc, prevPen);
}

CPoint CEqmDrawBase::PointFToCPoint( Gdiplus::PointF pointf )
{
	CPoint result;
	result.x = pointf.X;
	result.y = pointf.Y;

	return result;
}

wstring CEqmDrawBase::GetPhysicalID() const
{
	return m_strPhysicalID;
}

void CEqmDrawBase::SetPhysicalID( const wstring strID )
{
	m_strPhysicalID = strID;
	m_bNeedSave = true;
}

void CEqmDrawBase::SetNeedSave(const bool bNeed)
{
	m_bNeedSave = bNeed;
}

const bool CEqmDrawBase::GetNeedSave()
{
	return m_bNeedSave;
}

bool CEqmDrawBase::ShiftEqmtsCtrlUp()
{
	return true;
}

bool CEqmDrawBase::ShiftEqmtsCtrlDown()
{
	return true;
}

bool CEqmDrawBase::ShiftEqmtsCtrlLeft()
{
	return true;
}

bool CEqmDrawBase::ShiftEqmtsCtrlRight()
{
	return true;
}

bool CEqmDrawBase::ChangeSize( const Gdiplus::PointF& actPt, Direction dir )
{
	return true;
}

bool CEqmDrawBase::ChangeSizeByOriginal( const Gdiplus::PointF& actPt, Direction dir,const Gdiplus::PointF& prePt,int nWidth, int nHeight )
{

	return true;
}

bool CEqmDrawBase::CopyBaseProperty( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		SetPos(pBase->GetPos());
		SetPageID(pBase->GetPageID());
		SetID(pBase->GetID());
		SetMainAlign(pBase->IsMainAlign());
		SetSelected(pBase->IsSelected());
		SetWidth(pBase->GetWidth());
		SetHeight(pBase->GetHeight());
		SetShow(pBase->GetShow());
		SetLayer(pBase->GetLayer());
		SetDataComID(pBase->GetDataComID());
		SetPhysicalID(pBase->GetPhysicalID());	
		return true;
	}
	return false;
}

void CEqmDrawBase::SetDeleteFlag(const bool bDeleteFlag)
{
	m_bDeleteFlag = bDeleteFlag;
}

bool CEqmDrawBase::GetDeleteFlag(void)
{
	return m_bDeleteFlag;
}

bool CEqmDrawBase::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	return true;
}

bool CEqmDrawBase::CopyInFormatBrush(CEqmDrawBase *pBase)
{
	return false;
}
