#pragma once
#include "eqmdrawbase.h"


typedef enum tagHistTrendType
{
	enumCurve	= 0,
	enumColumn
}HistoryTrendType;

typedef struct tagHistTrendInf
{
	int					nCount;
	COleDateTime		dateTimeStart;
	COleDateTime		dateTimeEnd;
	CString				strBindPointName;
	CString				strRemarks;
	COLORREF			colorImage;
	int					nLineWidth;
	bool				bIsCalcRange;
	int					nUpdateCycle;
	float				fScale;

	tagHistTrendInf(void)
	{
		nCount			= 0;
		colorImage		= RGB(0, 0, 0);
		nLineWidth		= 1;
		bIsCalcRange	= false;
		nUpdateCycle	= 1000;
		fScale			= 0.6;
	}
}HistoryTrendInfo;


class DLLEXPORT CEqmDrawHistoryTrend : public CEqmDrawBase
{
public:
	CEqmDrawHistoryTrend(void);
	~CEqmDrawHistoryTrend(void);

public:
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawHistoryTrend*	Copy(void);
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

	virtual void					SetWidth(int width);
	virtual void					SetHeight(int height);

public:
	void							SetHistoryTrendType(const HistoryTrendType& hisTrendType);
	//void							SetStartTime(const COleDateTime time);
	//void							SetEndTime(const COleDateTime time);
	//void							SetIsAutoCalcRange(const bool bFlag);
	//void							SetBindPointName(const CString strBindPointName);
	//void							SetRemarks(const CString strRemarks);
	//void							SetImageColor(const COLORREF color);
	//void							SetUpdateCycle(const int nCycle);
	//void							SetLineWidth(const int nLineWidth);
	//void							SetScale(const float fScale);
	void							SetHistoryTrendVector(const vector<HistoryTrendInfo>& vecHisTrendInfo);

	int								GetPicId(void);
	HistoryTrendType				GetHistoryTrendType(void);
	//COleDateTime					GetStartTime(void);
	//COleDateTime					GetEndTime(void);
	//bool							GetIsAutoCalcRange(void);
	//CString						GetBindPointName(void);
	//CString						GetRemarks(void);
	//COLORREF						GetImageColor(void);
	//int							GetUpdateCycle(void);
	//int							GetLineWidth(void);
	//float							GetScale(void);
	vector<HistoryTrendInfo>		GetHistoryTrendVector(void);

private:
	Image*							m_pBitmap;
	int								m_nBaseHeight;
	int								m_nBaseWidth ;
	float							m_fscale;

	HistoryTrendType				m_historyTrendType;
	//COleDateTime					m_tmStart;
	//COleDateTime					m_tmEnd;
	//bool							m_bIsAutoCalcRange;
	//CString						m_strBindPointName;
	//CString						m_strRemarks;
	//COLORREF						m_color;
	//int							m_nCycle;
	//int							m_nLineWidth;
	//float							m_fScale;
	vector<HistoryTrendInfo>		m_vecHistoryTrendInfo;
};
