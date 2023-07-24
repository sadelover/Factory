#pragma once
#include "eqmdrawbase.h"

class DLLEXPORT CEqmDrawLiquidLevel :
	public CEqmDrawBase
{
public:
	CEqmDrawLiquidLevel(void);
	~CEqmDrawLiquidLevel(void);
	virtual CEqmDrawLiquidLevel*	Copy(void);
	virtual void		Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual bool		PointInMe(const Gdiplus::PointF& actPt) const;	//check if the mouse point at the equipment
	virtual Direction	PointIsSide(const Gdiplus::PointF& actPt);		//check if the mouse point at the side of equipment
	virtual void		DrawSelectEdge(Gdiplus::Graphics *pGraphics);	//Draw the select edge when the mouse selects the equipment
	virtual bool		ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool		CopyProperty(CEqmDrawBase *pBase);
	virtual bool		ClonePropertyByDest(CEqmDrawBase *pBase);
	virtual bool		ShiftEqmtsUp();       
	virtual bool		ShiftEqmtsDown();     
	virtual bool		ShiftEqmtsLeft();     
	virtual bool		ShiftEqmtsRight();  
	virtual bool		ShiftEqmtsCtrlUp();        
	virtual bool		ShiftEqmtsCtrlDown();      
	virtual bool		ShiftEqmtsCtrlLeft();      
	virtual bool		ShiftEqmtsCtrlRight(); 

	void				SetDefaultBitmap(Image* pBitmap);
	Image*				GetDefaultBitmap();
	void				SetPicID(const int nPicID);
	void				SetBindPoint(const CString strBindPoint);
	void				SetMaxValue(const int nMaxVal);
	void				SetMinValue(const int nMinVal);
	void				SetHighAlarmLine(const float fHighAlarmLine);
	void				SetLowAlarmLine(const float LowAlarmLine);
	const int			GetPicID(void);
	const CString		GetBindPoint(void);
	const int			GetMaxValue(void);
	const int			GetMinValue(void);
	const float			GetHighAlarmLine(void);
	const float			GetLowAlarmLine(void);

private:
	Image*				m_pBitmap;
	int					m_base_height;
	int					m_base_width ;

	int					m_nDefaultPicID;
	CString				m_strBindPoint;
	int					m_nMaxValue;
	int					m_nMinValue;
	float				m_fHighAlarmLine;
	float				m_fLowAlarmLine;
};

