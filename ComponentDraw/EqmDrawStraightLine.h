#pragma once
#include "EqmDrawBase.h"

enum _LineEndShape{
	Shape_None = 0,//无
	Shape_SolidCircle//实心圆
};

//绘制直线
class DLLEXPORT CEqmDrawStraightLine :public CEqmDrawBase
{
public:
	CEqmDrawStraightLine(void);
	~CEqmDrawStraightLine(void);
	
	virtual CEqmDrawStraightLine*	Copy(void);
	virtual void					Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual bool					PointInMe(const Gdiplus::PointF& actPt) const;	//check if the mouse point at the equipment
	virtual Direction				PointIsSide(const Gdiplus::PointF& actPt);		//check if the mouse point at the side of equipment
	virtual void					DrawSelectEdge(Gdiplus::Graphics *pGraphics);	//Draw the select edge when the mouse selects the equipment
	virtual bool					ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool					CopyProperty(CEqmDrawBase *pBase);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);
	virtual bool					ShiftEqmtsUp(void);
	virtual bool					ShiftEqmtsDown(void);
	virtual bool					ShiftEqmtsLeft(void);
	virtual bool					ShiftEqmtsRight(void);
	virtual bool					ShiftEqmtsCtrlUp(void);
	virtual bool					ShiftEqmtsCtrlDown(void);
	virtual bool					ShiftEqmtsCtrlLeft(void);
	virtual bool					ShiftEqmtsCtrlRight(void);
	virtual bool					DoMoveEqmt(Gdiplus::PointF prePt, Gdiplus::PointF actPt); //移动设备
	

	void					SetTerminalPointX(int nX);
	int						GetTerminalPointX(void);
	void					SetTerminalPointY(int nY);
	int						GetTerminalPointY(void);
	void					SetLineStyle(UINT nStyle);
	UINT					GetLineStyle(void);
	void					SetLineWidth(REAL fWidth);
	REAL					GetLineWidth(void);

	void					SetLineColor(ULONG color);
	ULONG					GetLineColor(void);

	/*
	* 函数介绍：从数据库里读出时使用，数据应用于前端
	* 输入参数：nColor：RGB格式数据
	* 输出参数：无
	* 返回值  ：void
	*/
	void					SetLineColorStrRGB(string strColor);

	/*
	* 函数介绍：向数据库存储时使用，数据应用于前端
	* 输入参数：nColor：RGB格式数据
	* 输出参数：无
	* 返回值  ："#ff22ff#0.12"
	*/
	string					GetLineColorStrRGB(void);
	//设置透明度
	void					SetTransparency(int nTransparency);
	//获取透明度
	int						GetTransparency(void);

	void					SetStartShape(_LineEndShape shape);
	_LineEndShape						GetStartShape(void);
	void					SetEndShape(_LineEndShape shape);
	_LineEndShape						GetEndShape(void);
	void					SetStartShapeSize(int shapeSize);
	int						GetStartShapeSize(void);
	void					SetEndShapeSize(int shapeSize);
	int						GetEndShapeSize(void);

	
private:
	//求点到线段的距离
	float GetDistancePointToLine(Gdiplus::PointF ptPoint, Gdiplus::PointF ptStart, Gdiplus::PointF ptEnd) const; 
	float GetAngle(float x0, float y0, float x1, float y1);
	//终点 X
	REAL			m_nTerminalPointX;
	//终点 Y
	REAL			m_nTerminalPointY;
	//线形(虚线，实线)
	DashStyle		m_nLineStyle;
	//线宽
	REAL			m_fLineWidth;
	//终点端点形状极其大小
	_LineEndShape				m_startShape;
	_LineEndShape				m_endShape;
	int				m_startShapeSize;
	int				m_endShapeSize;

	//线颜色
	Gdiplus::Color	m_lineColor;
	//线透明度
	int m_nTransparency;

	//画笔
	Gdiplus::Pen*	m_pCommonBorderPen; 

	//	DashStyleSolid		实线。
	//	DashStyleDash		虚线。
	//	DashStyleDot		点线。
	//	DashStyleDashDot	交替的虚线。
	//	DashStyleDashDotDot	交替的点划线。
	//	DashStyleCustom		指定用户定义的自定义虚线。
};

