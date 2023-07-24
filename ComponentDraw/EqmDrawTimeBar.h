#pragma once
#include "eqmdrawbase.h"
#include <vector>
using namespace std;

struct _tag_bar_item
{
	int nIndex;
	CString strName;
};

class DLLEXPORT CEqmDrawTimeBar :
	public CEqmDrawBase
{
public:
	CEqmDrawTimeBar(void);
	~CEqmDrawTimeBar(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawTimeBar*		Copy();
	virtual bool					PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction				PointIsSide(const Gdiplus::PointF& actPt);      //check if the mouse point at the side of equipment
	virtual void					DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool					ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool					CopyProperty(CEqmDrawBase *pBase);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);
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

	void							SetBindPointStart(const CString strBindPoint);
	const CString					GetBindPointStart();
	void							SetBindPointStatus(const CString strBindPoint);
	const CString					GetBindPointStatus();

	void							SetBarColor(const COLORREF nColor);
	const COLORREF					GetBarColor();

	void							InsertItem(const _tag_bar_item item);
	void							DeleteAllItems();

	void							SetQueryType(const int nType);
	const int						GetQueryType();

	const vector<_tag_bar_item>&	GetItemVec();
	void							SetItemVec(const vector<_tag_bar_item>& vec);

private:
	Image*							m_pBitmap;
	int								m_base_height;
	int								m_base_width ;
	float							m_nscale;

	CString							m_strBindPointStart;
	CString							m_strBindPointStatus;

	COLORREF						m_nColor;
	vector<_tag_bar_item>			m_vecItem;

	int								m_nQueryType;
};