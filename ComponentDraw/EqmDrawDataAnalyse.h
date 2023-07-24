#pragma once
#include "eqmdrawbase.h"


typedef struct tagDaPointInf
{
	CString	strPointName;
	CString	strPointRemarks;
}DaPointInfo;


class DLLEXPORT CEqmDrawDataAnalyse : public CEqmDrawBase
{
public:
	CEqmDrawDataAnalyse(void);
	~CEqmDrawDataAnalyse(void);

public:
	virtual CEqmDrawDataAnalyse*	Copy(void);
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
	vector<DaPointInfo>				GetDaPointInfo(void) const;
	CString							GetPointTime(void) const;
	CString							GetPointCount(void) const;
	CString							GetPointStatus(void) const;

	void							SetDaPointInfo(const vector<DaPointInfo>& data);
	void							SetPointTime(const CString strPtTime);
	void							SetPointCount(const CString strPtCount);
	void							SetPointStatus(const CString strPtStatus);

private:
	vector<DaPointInfo>				m_vecDaPtInfo;
	CString							m_strPtTime;
	CString							m_strPtCount;
	CString							m_strPtStatus;
};

