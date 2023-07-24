#pragma once
#include "eqmdrawbase.h"


class DLLEXPORT CEqmDrawEnergySaveRoi : public CEqmDrawBase
{
public:
	CEqmDrawEnergySaveRoi(void);
	~CEqmDrawEnergySaveRoi(void);

public:
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawEnergySaveRoi*	Copy(void);
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

	void							SetBindPointName(const CString strBindPointName);
	void							SetCost(const double dCost);
	void							SetStartTime(const COleDateTime time);
	void							SetEndTime(const COleDateTime time);
	void							SetOccurTime(const COleDateTime time);

	int								GetPicId(void);
	CString							GetBindPointName(void);
	double							GetCost(void);
	COleDateTime					GetStartTime(void);
	COleDateTime					GetEndTime(void);
	COleDateTime					GetOccurTime(void);

private:
	Image*							m_pBitmap;
	int								m_nBaseHeight;
	int								m_nBaseWidth ;
	float							m_fScale;

	CString							m_strBindPointName;
	double							m_dCost;
	COleDateTime					m_tmStart;
	COleDateTime					m_tmEnd;
	COleDateTime					m_tmOccur;
};
