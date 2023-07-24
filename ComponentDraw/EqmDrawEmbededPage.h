#pragma once
#include "eqmdrawbase.h"

class DLLEXPORT CEqmDrawEmbededPage :
	public CEqmDrawBase
{
public:
	CEqmDrawEmbededPage(void);
	~CEqmDrawEmbededPage(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawEmbededPage*		Copy();
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

	void							SetPicID(const int nPicID);
	const int						GetPicID();

	void							InsertPageList(const int nPageID, const CString strPageName);
	void							ClearPageList();
	vector<PageIDName>&				GetPageList();
	void							SetPageList(vector<PageIDName>& vec);

	void							SetChangeInterval(const int nInterval);
	const int						GetChangeInterval();
	void							InsertEqmPageVec(CEqmDrawPage* pPage);
	vector<CEqmDrawPage*>&			GetEqmDrawPageVec();

private:
	Image*							m_pBitmap;
	int								m_base_height;
	int								m_base_width ;
	float							m_nscale;

	int								m_nDefaultPicID;
	vector<PageIDName>				m_PageVec;
	int								m_nChangeInterval;
	vector<CEqmDrawPage*>			m_pEqmPageVec;
};
