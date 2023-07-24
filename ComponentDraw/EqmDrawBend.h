#pragma once
#include "eqmdrawbase.h"
class DLLEXPORT CEqmDrawBend :
	public CEqmDrawBase
{
public:
	CEqmDrawBend(void);
	~CEqmDrawBend(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawBend*			Copy();
	virtual bool					PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction				PointIsSide(const Gdiplus::PointF& actPt);      //check if the mouse point at the side of equipment
	virtual void					DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool					ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool					CopyProperty(CEqmDrawBase *pBase);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);		//克隆目标的所有属性
	void							SetDefaultBitmap(Image* pBitmap);
	virtual bool					ShiftEqmtsUp();       
	virtual bool					ShiftEqmtsDown();     
	virtual bool					ShiftEqmtsLeft();     
	virtual bool					ShiftEqmtsRight();  
	virtual bool						ShiftEqmtsCtrlUp();        
	virtual bool						ShiftEqmtsCtrlDown();      
	virtual bool						ShiftEqmtsCtrlLeft();      
	virtual bool						ShiftEqmtsCtrlRight();  
	virtual bool					LeftRotate90();//左旋转90度
	virtual bool					RightRotate90();//右旋转90度
	void							SetRotateAngle(const int nRotateAngle);
	const int						GetRotateAngle();
	void RotetaBitMap( Image *pBitMap /*= NULL*/ );
	void ResetRotateBmp(Image *pBitMap );
	void							SetSize(const int nSize);
	const int						GetSize(void) const;
private:
	Image*							m_pBitmap;
	int								m_base_height;
	int								m_base_width ;
	float							m_nscale;
	int								m_nRotateAngle;//顺时针的角度
	int								m_nSize;
};

