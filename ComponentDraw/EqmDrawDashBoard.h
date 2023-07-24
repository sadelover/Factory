#pragma once
#include "eqmdrawbase.h"

class DLLEXPORT CEqmDrawDashBoard :
	public CEqmDrawBase
{
public:
	CEqmDrawDashBoard(void);
	~CEqmDrawDashBoard(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawDashBoard*		Copy();
	virtual bool					PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction				PointIsSide(const Gdiplus::PointF& actPt);      //check if the mouse point at the side of equipment
	virtual void					DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool					ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool					CopyProperty(CEqmDrawBase *pBase);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);		//克隆目标的所有属性
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
	const CString					GetBindPointName();
	void							SetBindPointName(const CString strBindPointName);
	void							SetStyle(const int nStyle);
	const int 						GetStyle();
	void							SetMaxValue(const double dMax);
	const double					GetMaxValue();
	void							SetMinValue(const double dMin);
	const double					GetMinValue();
	void							SetMaxString(const CString strMaxVal);
	CString							GetMaxString(void) const;
	void							SetMinString(const CString strMinVal);
	CString							GetMinString(void) const;
	void							SetMaxBindFlag(const bool bIsBind);
	bool							GetMaxBindFlag(void) const;
	void							SetMinBindFlag(const bool bIsBind);
	bool							GetMinBindFlag(void) const;
	COLORREF						GetFontColor(void);
	void							SetFontColor(const COLORREF setColorValue);
	int								GetDecimalPlaces(void);
	void							SetDecimalPlaces(const COLORREF setDecimalPlaces);

private:
	CString							m_strBindPointName;
	Image*							m_pBitmap;
	int								m_base_height;
	int								m_base_width ;
	float							m_nscale;
	double							m_dMaxValue;
	double							m_dMinValue;
	CString							m_strMaxValue;
	CString							m_strMinValue;
	bool							m_bIsMaxBindPoint;
	bool							m_bIsMinBindPoint;
	int								m_nStyle;
	// 仪表盘字体颜色
	COLORREF m_FontColor;
	// 小数位数
	int m_DecimalPlaces;
};
