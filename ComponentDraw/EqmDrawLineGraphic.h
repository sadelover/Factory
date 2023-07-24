#pragma once
#include "eqmdrawbase.h"
class DLLEXPORT CEqmDrawLineGraphic : public CEqmDrawBase
{
public:
	CEqmDrawLineGraphic(void);
	~CEqmDrawLineGraphic(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawLineGraphic*		Copy();
	virtual bool						PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction					PointIsSide(const Gdiplus::PointF& actPt);      //check if the mouse point at the side of equipment
	virtual void						DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool						ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool						CopyProperty(CEqmDrawBase *pBase);
	virtual bool						ClonePropertyByDest(CEqmDrawBase *pBase);
	void								SetDefaultBitmap(Image* pBitmap);
	Image*								GetDefaultBitmap();
	virtual bool						ShiftEqmtsUp();       
	virtual bool						ShiftEqmtsDown();     
	virtual bool						ShiftEqmtsLeft();     
	virtual bool						ShiftEqmtsRight();  
	virtual bool						ShiftEqmtsCtrlUp();        
	virtual bool						ShiftEqmtsCtrlDown();      
	virtual bool						ShiftEqmtsCtrlLeft();      
	virtual bool						ShiftEqmtsCtrlRight(); 

	void								SetTitleName(const CString strName);
	const CString						GetTitleName();
	void								SetUpdateInterval(const int nUpdateInterval);
	const int							GetUpdateInterval();
	void								SetXName(const CString strName);
	const CString						GetXName();
	void								SetXUnit(const CString strXUnit);
	const CString						GetXUnit();
	void								SetYName(const CString strName);
	const CString						GetYName();
	void								InsertGraphItem(_Graph_Item_Property item);
	void								DeleteGraphItem(const CString strItemName);
	_Graph_Item_Property*				GetGraphItem(const int nIndex);
	_Graph_Item_Property*				GetGraphItem(const CString strItemName);
	void								ClearGraphItem();
	vector<_Graph_Item_Property>&		GetItemVector();
	void								SetItemVector(vector<_Graph_Item_Property>& vec);
	void								SetType(const E_BAR_TYPE eType);
	const E_BAR_TYPE					GetType();
	void								SetMaxValue(const double dValue);
	const double						GetMaxValue();
	void								SetMinValue(const double dValue);
	const double						GetMinValue();
	void								SetTextColor(const COLORREF color);
	const COLORREF						GetTextColor();
	void								SetMaxCheck(const int nCheck);
	const int							GetMaxCheck();
	void								SetMinCheck(const int nCheck);
	const int							GetMinCheck();

private:
	CString								m_strTitleName;
	int									m_nUpdateInterval;
	CString								m_strXName;
	CString								m_strXUnit;
	CString								m_strYName;
	vector<_Graph_Item_Property>		m_vecItem;
	Image*								m_pBitmap;
	int									m_base_height;
	int									m_base_width ;
	float								m_nscale;
	E_BAR_TYPE							m_nType;
	double								m_dMaxValue;
	double								m_dMinValue;
	COLORREF							m_colorText;
	int									m_nMaxCheck;
	int									m_nMinCheck;

		
};

