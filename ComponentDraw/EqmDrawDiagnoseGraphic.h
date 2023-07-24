#pragma once
#include "eqmdrawbase.h"

struct _tagDiagnoseItemInLine 
{
	_tagDiagnoseItemInLine()
	{
		nID = -1;
	};
	int		nID;
	CString strPointName;
	CString strDescription;
};

struct _tagDiagnoseItem 
{
	_tagDiagnoseItem()
	{
		color = RGB(255,255,255);
	};
	CString								strItemName;
	CString								strPointName;
	COLORREF							color;
	vector<_tagDiagnoseItemInLine>		vecItemInLine;
};

class DLLEXPORT CEqmDrawDiagnoseGraphic : public CEqmDrawBase
{
public:
	CEqmDrawDiagnoseGraphic(void);
	~CEqmDrawDiagnoseGraphic(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawDiagnoseGraphic*	Copy();
	virtual bool						PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction					PointIsSide(const Gdiplus::PointF& actPt);      //check if the mouse point at the side of equipment
	virtual void						DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool						ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool						CopyProperty(CEqmDrawBase *pBase);
	virtual bool						ClonePropertyByDest(CEqmDrawBase *pBase);		//克隆目标的所有属性
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

	void								SetUpdateInterval(const int nUpdateInterval);
	const int							GetUpdateInterval();
	void								InsertSubItem(_tagDiagnoseItem& item, const _tagDiagnoseItemInLine subItem);
	void								DeleteSubItem(_tagDiagnoseItem& item, const CString SubItemName);
	void								DeleteAllSubItem(_tagDiagnoseItem& item);
	void								InsertItem(const _tagDiagnoseItem item);
	void								DeleteItem(const CString itemName);
	void								DeleteAll();
	vector<_tagDiagnoseItem>&			GetItemVec();
	void								SetItemVec(vector<_tagDiagnoseItem>& vec);
	void								GetSubItemVec( const CString itemName, vector<_tagDiagnoseItemInLine>& vec);
	const int							GetSubItemNum(const _tagDiagnoseItem& item);
	_tagDiagnoseItem*					GetItem( const CString itemName);

private:
	int									m_nUpdateInterval;
	Image*								m_pBitmap;
	int									m_base_height;
	int									m_base_width ;
	float								m_nscale;
	vector<_tagDiagnoseItem>			m_vecDiagnoseItem;
};

