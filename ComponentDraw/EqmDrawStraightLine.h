#pragma once
#include "EqmDrawBase.h"

enum _LineEndShape{
	Shape_None = 0,//��
	Shape_SolidCircle//ʵ��Բ
};

//����ֱ��
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
	virtual bool					DoMoveEqmt(Gdiplus::PointF prePt, Gdiplus::PointF actPt); //�ƶ��豸
	

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
	* �������ܣ������ݿ������ʱʹ�ã�����Ӧ����ǰ��
	* ���������nColor��RGB��ʽ����
	* �����������
	* ����ֵ  ��void
	*/
	void					SetLineColorStrRGB(string strColor);

	/*
	* �������ܣ������ݿ�洢ʱʹ�ã�����Ӧ����ǰ��
	* ���������nColor��RGB��ʽ����
	* �����������
	* ����ֵ  ��"#ff22ff#0.12"
	*/
	string					GetLineColorStrRGB(void);
	//����͸����
	void					SetTransparency(int nTransparency);
	//��ȡ͸����
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
	//��㵽�߶εľ���
	float GetDistancePointToLine(Gdiplus::PointF ptPoint, Gdiplus::PointF ptStart, Gdiplus::PointF ptEnd) const; 
	float GetAngle(float x0, float y0, float x1, float y1);
	//�յ� X
	REAL			m_nTerminalPointX;
	//�յ� Y
	REAL			m_nTerminalPointY;
	//����(���ߣ�ʵ��)
	DashStyle		m_nLineStyle;
	//�߿�
	REAL			m_fLineWidth;
	//�յ�˵���״�����С
	_LineEndShape				m_startShape;
	_LineEndShape				m_endShape;
	int				m_startShapeSize;
	int				m_endShapeSize;

	//����ɫ
	Gdiplus::Color	m_lineColor;
	//��͸����
	int m_nTransparency;

	//����
	Gdiplus::Pen*	m_pCommonBorderPen; 

	//	DashStyleSolid		ʵ�ߡ�
	//	DashStyleDash		���ߡ�
	//	DashStyleDot		���ߡ�
	//	DashStyleDashDot	��������ߡ�
	//	DashStyleDashDotDot	����ĵ㻮�ߡ�
	//	DashStyleCustom		ָ���û�������Զ������ߡ�
};

