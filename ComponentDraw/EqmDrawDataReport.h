#pragma once

#include "eqmdrawbase.h"


class DLLEXPORT CEqmDrawDataReport : public CEqmDrawBase
{
public:
	CEqmDrawDataReport(void);
	~CEqmDrawDataReport(void);

public:
	virtual CEqmDrawDataReport*		Copy(void);
	virtual void					Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual bool					PointInMe(const Gdiplus::PointF& actPt) const;	//check if the mouse point at the equipment
	virtual Direction				PointIsSide(const Gdiplus::PointF& actPt);		//check if the mouse point at the side of equipment
	virtual void					DrawSelectEdge(Gdiplus::Graphics *pGraphics);	//Draw the select edge when the mouse selects the equipment
	virtual bool					ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool					CopyProperty(CEqmDrawBase *pBase);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);
	void							SetDefaultBitmap(Image* pBitmap);
	Image*							GetDefaultBitmap();
	virtual bool					ShiftEqmtsUp(void);
	virtual bool					ShiftEqmtsDown(void);
	virtual bool					ShiftEqmtsLeft(void);
	virtual bool					ShiftEqmtsRight(void);
	virtual bool					ShiftEqmtsCtrlUp(void);
	virtual bool					ShiftEqmtsCtrlDown(void);
	virtual bool					ShiftEqmtsCtrlLeft(void);
	virtual bool					ShiftEqmtsCtrlRight(void);

private:
	Image*							m_pBitmap;
	int								m_nBaseHeight;
	int								m_nBaseWidth;
	float							m_fscale;


public:
	CString							GetPath(void) const;
	void							SetPath(const CString strPath);

private:
	CString	m_strPath;
};

