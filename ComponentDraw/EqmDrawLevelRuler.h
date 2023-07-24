#pragma once
#include "eqmdrawbase.h"

typedef struct tagLabelInfo
{
	CString	strName;
	double	dValue;
	CString	strBindPtName;
	int		nLinkPageId;
	CString	strLinkPageName;
	bool	bIsUp;

	tagLabelInfo(void)
	{
		dValue = 0;
		nLinkPageId = 0;
		bIsUp = false;
	}
}LabelInfo;

typedef struct tagSectionInfo
{
	CString	strName;
	double	fMax;
	double	fMin;

	tagSectionInfo(void)
	{
		fMax = 0;
		fMin = 0;
	}
}SectionInfo;


class DLLEXPORT CEqmDrawLevelRuler : public CEqmDrawBase
{
public:
	CEqmDrawLevelRuler(void);
	~CEqmDrawLevelRuler(void);

public:
	virtual CEqmDrawLevelRuler*		Copy(void);
	virtual void					Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual bool					PointInMe(const Gdiplus::PointF& actPt) const;	//check if the mouse point at the equipment
	virtual Direction				PointIsSide(const Gdiplus::PointF& actPt);		//check if the mouse point at the side of equipment
	virtual void					DrawSelectEdge(Gdiplus::Graphics *pGraphics);	//Draw the select edge when the mouse selects the equipment
	virtual bool					ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool					CopyProperty(CEqmDrawBase *pBase);
		virtual bool				ClonePropertyByDest(CEqmDrawBase *pBase);
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
	CString							GetName(void) const;
	double							GetMax(void) const;
	double							GetMin(void) const;
	double							GetMainScaleInterval(void) const;
	double							GetMinorScaleInterval(void) const;
	int								GetDecimalPlace(void) const;
	vector<LabelInfo>				GetLabelInfo(void) const;
	vector<SectionInfo>				GetSectionInfo(void) const;

	void							SetName(const CString strName);
	void							SetMax(const double dMax);
	void							SetMin(const double dMin);
	void							SetMainScaleInterval(const double dMainInterval);
	void							SetMinorScaleInterval(const double dMinorInterval);
	void							SetDecimalPlace(const int nDeciPlace);
	void							SetLabelInfo(const vector<LabelInfo>& vecLabelInfo);
	void							SetSectionInfo(const vector<SectionInfo>& vecSectionInfo);

private:
	CString							m_strName;
	double							m_dMax;
	double							m_dMin;
	double							m_dMainScaleInterval;
	double							m_dMinorScaleInterval;
	int								m_nDecimalPlace;
	vector<LabelInfo>				m_vecLabelInfo;
	vector<SectionInfo>				m_vecSectionInfo;
};

