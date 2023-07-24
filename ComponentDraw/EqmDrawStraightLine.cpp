#include "StdAfx.h"
#include "EqmDrawStraightLine.h"
#include "Tools/CustomTools/CustomTools.h"
#include <iostream>
CEqmDrawStraightLine::CEqmDrawStraightLine(void)
	:m_nTerminalPointX(0.0)
	,m_nTerminalPointY(0.0)
	,m_nLineStyle(DashStyleSolid)
	,m_fLineWidth(5.0)
	,m_lineColor(255,0,255,255)
	,m_nTransparency(100)
	,m_pCommonBorderPen(NULL)
	,m_startShape(Shape_None)
	,m_endShape(Shape_None)
	,m_startShapeSize(10)
	,m_endShapeSize(10)
{
}


CEqmDrawStraightLine::~CEqmDrawStraightLine(void)
{
}
CEqmDrawStraightLine* CEqmDrawStraightLine::Copy(void)
{
	m_bNeedSave = true;
	return new CEqmDrawStraightLine(*this);
}

void CEqmDrawStraightLine::Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap /*= NULL*/)
{
	ASSERT(pGraphics);
	if(m_pCommonBorderPen == NULL)
	{
		m_pCommonBorderPen = new Gdiplus::Pen(m_lineColor, m_fLineWidth);
		m_pCommonBorderPen->SetDashStyle(m_nLineStyle);
	}

	pGraphics->DrawLine(m_pCommonBorderPen, (INT)m_pos.X, (INT)m_pos.Y, (INT)m_nTerminalPointX, (INT)m_nTerminalPointY);
	
	int X1 = m_pos.X;
	int Y1 = m_pos.Y ;
	int halfStartShapeSize = m_startShapeSize/2;
	int halfEndShapeSize = m_endShapeSize/2;
	switch(m_startShape)
	{
	case Shape_None :
		break;
	case Shape_SolidCircle :
		{
			Gdiplus::SolidBrush*	SolidBrush = new Gdiplus::SolidBrush(m_lineColor);		//画刷
			pGraphics->FillEllipse(SolidBrush, X1-halfStartShapeSize, Y1 - halfStartShapeSize, m_startShapeSize, m_startShapeSize);
			delete SolidBrush;
			break;
		}
	}

	switch(m_endShape)
	{
	case Shape_None :
		break;
	case Shape_SolidCircle :
		Gdiplus::SolidBrush*	SolidBrush = new Gdiplus::SolidBrush(m_lineColor);		//画刷
		pGraphics->FillEllipse(SolidBrush, (int)m_nTerminalPointX-halfEndShapeSize, (int)m_nTerminalPointY - halfEndShapeSize, m_endShapeSize, m_endShapeSize);
		delete SolidBrush;
		break;
	}

	if(m_pCommonBorderPen)
	{
		delete m_pCommonBorderPen;
		m_pCommonBorderPen = NULL;
	}
	return;
}

bool CEqmDrawStraightLine::PointInMe(const Gdiplus::PointF& actPt) const
{
	REAL halfWidth = m_fLineWidth/2;

	if((m_pos.Y == m_nTerminalPointY) )//横线
	{
		if( m_pos.X < m_nTerminalPointX )//左到右
		{//矩形范围判断
			if( (actPt.X > (m_pos.X-halfWidth)) &&
				(actPt.Y > (m_pos.Y-halfWidth)) &&
				(actPt.X < (m_nTerminalPointX+halfWidth)) &&
				(actPt.Y < (m_nTerminalPointY+halfWidth)) )
			{
				return true;
			}
		}
		else//右到左
		{
			if( (actPt.X < (m_pos.X+halfWidth)) &&
				(actPt.Y < (m_pos.Y+halfWidth)) &&
				(actPt.X > (m_nTerminalPointX-halfWidth)) &&
				(actPt.Y > (m_nTerminalPointY-halfWidth)) )
			{
				return true;
			}
		}
	}
	else if( (m_pos.X == m_nTerminalPointX)/* && (m_pos.Y < m_nTerminalPointY)*/ )//竖线
	{
		if( m_pos.Y < m_nTerminalPointY )//上到下
		{//矩形范围判断
			if( (actPt.X > (m_pos.X-halfWidth)) &&
				(actPt.Y > (m_pos.Y-halfWidth)) &&
				(actPt.X < (m_nTerminalPointX+halfWidth)) &&
				(actPt.Y < (m_nTerminalPointY+halfWidth)) )
			{
				return true;
			}
		}
		else//下到上
		{
			if( (actPt.X < (m_pos.X+halfWidth)) &&
				(actPt.Y < (m_pos.Y+halfWidth)) &&
				(actPt.X > (m_nTerminalPointX-halfWidth)) &&
				(actPt.Y > (m_nTerminalPointY-halfWidth)) )
			{
				return true;
			}
		}
	}
	else
	{
		//计算点到线段的距离
		PointF endPoint(m_nTerminalPointX, m_nTerminalPointY);
		float distance_pointToLine = GetDistancePointToLine(actPt, m_pos, endPoint);
		if ( (distance_pointToLine <= halfWidth) && (distance_pointToLine>= -halfWidth) )
		{
			return true;
		}
	}
	return false;
}
Direction CEqmDrawStraightLine::PointIsSide(const Gdiplus::PointF& actPt)
{
	//if the mouse choose the first point of the line
	if ( actPt.X <= m_pos.X + m_fLineWidth &&
		actPt.X >= m_pos.X - m_fLineWidth &&
		actPt.Y <= m_pos.Y + m_fLineWidth &&
		actPt.Y >= m_pos.Y - m_fLineWidth)  
	{
		return Dir_Left;
	}

	//if the mouse choose the last point of the line
	if ( actPt.X <= m_nTerminalPointX + m_fLineWidth &&
		actPt.X >= m_nTerminalPointX - m_fLineWidth &&
		actPt.Y <= m_nTerminalPointY + m_fLineWidth &&
		actPt.Y >= m_nTerminalPointY - m_fLineWidth)    
	{
		return Dir_Right;
	}
}

void CEqmDrawStraightLine::DrawSelectEdge(Gdiplus::Graphics *pGraphics)
{
	if(m_bIsMainAlign)
	{
		Gdiplus::HatchBrush hatchBrush(Gdiplus::HatchStyleCross, MAINSELECT_BRUSHCOLOR, MAINSELECT_BRUSHCOLOR);

		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos.X - RECTSIZE/2,
			m_pos.Y - RECTSIZE/2, RECTSIZE, RECTSIZE));					   
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_nTerminalPointX- RECTSIZE/2,
			m_nTerminalPointY - RECTSIZE/2, RECTSIZE, RECTSIZE));				   
	}
	else
	{
		Gdiplus::Pen pen(SELECTPENCOLOR, 1);
		pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos.X - RECTSIZE/2,
			m_pos.Y - RECTSIZE/2, RECTSIZE, RECTSIZE));
		pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_nTerminalPointX - RECTSIZE/2,
			m_nTerminalPointY - RECTSIZE/2, RECTSIZE, RECTSIZE));
	}
}

bool CEqmDrawStraightLine::ChangeSize(const Gdiplus::PointF& actPt, Direction dir)
{
	switch (dir)
	{
	case Dir_Left://选中第一点
		//鼠标位置	actPt
		//被拖拽	m_pos
		//原定		m_nTerminalPoint
		if(GetAsyncKeyState(VK_SHIFT)&0x8000)
		{			

			m_pos.X = abs(m_nTerminalPointX - actPt.X)>abs(m_nTerminalPointY - actPt.Y) ? actPt.X : m_nTerminalPointX;
			m_pos.Y = abs(m_nTerminalPointX - actPt.X)>abs(m_nTerminalPointY - actPt.Y) ? m_nTerminalPointY : actPt.Y;
		}
		else
		{
			m_pos.X   = actPt.X;
			m_pos.Y   = actPt.Y;
		}
		break;
	case Dir_Right:
		/*m_nTerminalPointX = actPt.X;
		m_nTerminalPointY = actPt.Y;*/
		{
			//鼠标位置	actPt
			//被拖拽	m_nTerminalPoint
			//原定		m_pos
			if(GetAsyncKeyState(VK_SHIFT)&0x8000)
			{
				m_nTerminalPointX = abs(m_pos.X - actPt.X)>abs(m_pos.Y - actPt.Y) ? actPt.X : m_pos.X;
				m_nTerminalPointY = abs(m_pos.X - actPt.X)>abs(m_pos.Y - actPt.Y) ? m_pos.Y : actPt.Y;
			}
			else
			{
				m_nTerminalPointX  = actPt.X;
				m_nTerminalPointY  = actPt.Y;
			}
		}
		break;
	default:
		break;
	}


	m_bNeedSave = true;
	return true;
}
bool CEqmDrawStraightLine::CopyProperty(CEqmDrawBase *pBase)
{
	return false;
}

bool CEqmDrawStraightLine::ClonePropertyByDest(CEqmDrawBase *pBase)
{
	if (pBase != NULL && m_eEqmType == pBase->GetEqmType())
	{
		CopyBaseProperty(pBase);

		//专有属性

		return true;
	}

	return false;
}

bool CEqmDrawStraightLine::ShiftEqmtsUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	m_nTerminalPointY -= MOVEDISTANCEBYKEY;
	return true;
}

bool CEqmDrawStraightLine::ShiftEqmtsDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	m_nTerminalPointY += MOVEDISTANCEBYKEY;
	return true;
}

bool CEqmDrawStraightLine::ShiftEqmtsLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	m_nTerminalPointX -= MOVEDISTANCEBYKEY;
	return true;
}

bool CEqmDrawStraightLine::ShiftEqmtsRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	m_nTerminalPointX += MOVEDISTANCEBYKEY;
	return true;
}

bool CEqmDrawStraightLine::ShiftEqmtsCtrlUp(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	m_nTerminalPointY -= MOVEDISTANCEBYKEYCTRL;
	return true;
}

bool CEqmDrawStraightLine::ShiftEqmtsCtrlDown(void)
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	m_nTerminalPointY += MOVEDISTANCEBYKEYCTRL;
	return true;
}

bool CEqmDrawStraightLine::ShiftEqmtsCtrlLeft(void)
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	m_nTerminalPointX -= MOVEDISTANCEBYKEYCTRL;

	return true;
}

bool CEqmDrawStraightLine::ShiftEqmtsCtrlRight(void)
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	m_nTerminalPointX += MOVEDISTANCEBYKEYCTRL;
	return true;
}

bool CEqmDrawStraightLine::DoMoveEqmt(Gdiplus::PointF prePt, Gdiplus::PointF actPt)
{
	if (Project::Tools::IsDoubleEqual(m_pos.X, m_nTerminalPointX))
	{
		const float y_min = min(m_pos.Y+actPt.Y-prePt.Y, m_nTerminalPointY+actPt.Y-prePt.Y);
		const float y_max = max(m_pos.Y+actPt.Y-prePt.Y, m_nTerminalPointY+actPt.Y-prePt.Y);
		if (m_pos.X+actPt.X-prePt.X>1920 || m_pos.X+actPt.X-prePt.X<0 || y_min<0 || y_max>955)
		{
			return false;
		}
	}
	if (Project::Tools::IsDoubleEqual(m_pos.Y, m_nTerminalPointY))
	{
		const float x_min = min(m_pos.X+actPt.X-prePt.X, m_nTerminalPointX+actPt.X-prePt.X);
		const float x_max = max(m_pos.X+actPt.X-prePt.X, m_nTerminalPointX+actPt.X-prePt.X);
		if (x_max>1920 || x_min<0 || m_pos.Y+actPt.Y-prePt.Y<0 || m_pos.Y+actPt.Y-prePt.Y>955)
		{
			return false;
		}
	}

	m_pos.X = m_pos.X + actPt.X - prePt.X;
	m_pos.Y = m_pos.Y + actPt.Y - prePt.Y;

	m_nTerminalPointX = m_nTerminalPointX + actPt.X - prePt.X;
	m_nTerminalPointY = m_nTerminalPointY + actPt.Y - prePt.Y;

	m_bNeedSave = true;
	return true;
}

float CEqmDrawStraightLine::GetDistancePointToLine(Gdiplus::PointF ptPoint, Gdiplus::PointF ptStart, Gdiplus::PointF ptEnd) const
{
	//x1,y1 x2,y2是线段的坐标，x,y是点的坐标。
	float& x1 = ptStart.X;
	float& y1 = ptStart.Y;
	float& x2 = ptEnd.X;
	float& y2 = ptEnd.Y;
	float& x = ptPoint.X;
	float& y = ptPoint.Y;

	double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);
	if (cross <= 0) return sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));

	double d2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	if (cross >= d2) return sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));

	double r = cross / d2;
	double px = x1 + (x2 - x1) * r;
	double py = y1 + (y2 - y1) * r;
	return sqrt((x - px) * (x - px) + (py - y) * (py - y));
}

float CEqmDrawStraightLine::GetAngle(float x0, float y0, float x1, float y1)
{
	double tmp = abs(y1 - y0) / abs(x1 - x0);
	double deg1 = atan(tmp);  //弧度 
	deg1 = deg1 * (double)180 / 3.14159265358979323846;
	return (float)deg1;
}

/*
 * 参数接口
*/
void CEqmDrawStraightLine::SetTerminalPointX(int nX)
{
	m_nTerminalPointX = (REAL)nX;
}

int CEqmDrawStraightLine::GetTerminalPointX(void)
{
	return (int)m_nTerminalPointX;
}

void CEqmDrawStraightLine::SetTerminalPointY(int nY)
{
	m_nTerminalPointY = (REAL)nY;
}

int CEqmDrawStraightLine::GetTerminalPointY(void)
{
	return (int)m_nTerminalPointY;
}

void CEqmDrawStraightLine::SetLineStyle(UINT nStyle)
{
	if(nStyle<=5)
	{
		m_nLineStyle = (DashStyle)nStyle;
	}
}

UINT CEqmDrawStraightLine::GetLineStyle(void)
{
	return m_nLineStyle;
}

void CEqmDrawStraightLine::SetLineWidth(REAL fWidth)
{
	m_fLineWidth = fWidth;
}

REAL CEqmDrawStraightLine::GetLineWidth(void)
{
	return m_fLineWidth;
}

void CEqmDrawStraightLine::SetLineColor(ULONG color)
{
	m_lineColor.SetFromCOLORREF(color);
}

ULONG CEqmDrawStraightLine::GetLineColor(void)
{
	return m_lineColor.ToCOLORREF();
}

void CEqmDrawStraightLine::SetLineColorStrRGB(string strColor)
{
	vector<string> strColorList;
	float fT;
	UINT A=255;
	Project::Tools::SplitStringByChar(strColor, '#',strColorList);
	int nColor=stoi(strColorList[0],NULL,16);
	if(strColorList.size() == 2)
	{
		fT = stof (strColorList[1]);
		m_nTransparency  = fT*100;
		A = (UINT)(fT*255);
	}
	UINT R = (nColor & 0xFF0000) >> 16;
	UINT G = (nColor & 0x00FF00) >>8;
	UINT B = (nColor & 0x0000FF);

	Gdiplus::Color tempColor(A,R,G,B);
	m_lineColor.SetValue(tempColor.GetValue());
}

string CEqmDrawStraightLine::GetLineColorStrRGB(void)
{
	int nColor = m_lineColor.ToCOLORREF();
	UINT R = (nColor & 0x0000FF) << 16;
	UINT G = (nColor & 0x00FF00);
	UINT B = (nColor & 0xFF0000)>>16;
	CString cstrColor;
	cstrColor.Format(L"#%06X#%.2f",R|G|B,((float)m_nTransparency/(float)100));
	string strColor;
	Project::Tools::WideCharToUtf8(cstrColor.GetString(),strColor);
	return strColor;
}

void CEqmDrawStraightLine::SetTransparency(int nTransparency)
{
	if(nTransparency>=0 && nTransparency<=100)
	{
		m_nTransparency = nTransparency;
		UINT A = (UINT)(((float)nTransparency/100)*255);
		Gdiplus::Color tempColor (A,m_lineColor.GetRed(),m_lineColor.GetGreen(),m_lineColor.GetBlue());
		m_lineColor.SetValue(tempColor.GetValue());
	}
}

int CEqmDrawStraightLine::GetTransparency(void)
{
	return m_nTransparency;
}

void CEqmDrawStraightLine::SetStartShape(_LineEndShape shape)
{
	m_startShape = shape;
}

_LineEndShape CEqmDrawStraightLine::GetStartShape(void)
{
	return m_startShape;
}

void CEqmDrawStraightLine::SetEndShape(_LineEndShape shape)
{
	m_endShape = shape;
}

_LineEndShape CEqmDrawStraightLine::GetEndShape(void)
{
	return m_endShape;
}

void CEqmDrawStraightLine::SetStartShapeSize(int shapeSize)
{
	m_startShapeSize = shapeSize;
}

int CEqmDrawStraightLine::GetStartShapeSize(void)
{
	return m_startShapeSize;
}

void CEqmDrawStraightLine::SetEndShapeSize(int shapeSize)
{
	m_endShapeSize = shapeSize;
}

int CEqmDrawStraightLine::GetEndShapeSize(void)
{
	return m_endShapeSize;
}
