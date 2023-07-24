#pragma once
#include "eqmdrawbase.h"
class DLLEXPORT CEqmDrawTimeSelect :
	public CEqmDrawBase
{
public:
	CEqmDrawTimeSelect(void);
	~CEqmDrawTimeSelect(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawTimeSelect*		Copy();
	virtual bool					PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction				PointIsSide(const Gdiplus::PointF& actPt);      //check if the mouse point at the side of equipment
	virtual void					DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool					ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool					CopyProperty(CEqmDrawBase *pBase);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);
	void							SetDefaultBitmap(Image* pBitmap);
	Image*							GetDefaultBitmap();
	virtual bool					ShiftEqmtsUp();       
	virtual bool					ShiftEqmtsDown();     
	virtual bool					ShiftEqmtsLeft();     
	virtual bool					ShiftEqmtsRight();  
	virtual bool						ShiftEqmtsCtrlUp();        
	virtual bool						ShiftEqmtsCtrlDown();      
	virtual bool						ShiftEqmtsCtrlLeft();      
	virtual bool						ShiftEqmtsCtrlRight(); 

	void							SetPointName(const CString strPointName);
	const CString					GetPointName();


	void							SetRWProperty(const int nRW);
	const int						GetRWProperty();

	void							SetTimeDataFormat(const int nSelect);
	const int						GetTimeDataFormat();
private:
	Image*							m_pBitmap;
	int								m_base_height;
	int								m_base_width ;
	float							m_nscale;

	CString							m_strBindPointName;
	int								m_rwProperty;
	int								m_nTimeDataFormat;//选择日期格式
};