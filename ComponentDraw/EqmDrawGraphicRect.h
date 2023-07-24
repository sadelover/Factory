#pragma once
#include "EqmDrawGraphic.h"

class DLLEXPORT CEqmDrawGraphicRect : public CEqmDrawGraphic
{
public:
	CEqmDrawGraphicRect();

	CEqmDrawGraphicRect(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid, 
		int ident,  int linecolor,int filledcolor,
		int linewidth, int linestype, const string& pointlist,
		int forwardpageid, double transparency, int width = 0, int height = 0,
		int ifborder =0);

	virtual ~CEqmDrawGraphicRect();

	virtual int         GetWidth() const;
	virtual int         GetHeight() const;

	virtual void        Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL); 
	virtual bool        PointInMe(const Gdiplus::PointF& actPt) const; 
	virtual Direction   PointIsSide(const Gdiplus::PointF& actPt) ;

	virtual void        DrawSelectEdge(Gdiplus::Graphics *pGraphics);

	virtual bool        ChangeSize(const Gdiplus::PointF& actPt, Direction dir);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);
	virtual const Gdiplus::PointF&	    GetPos() const;;
	virtual void	                    SetPos(const Gdiplus::PointF& centerpos);

	int    GetRectBorderIs3D() const;
	void    SetRectBorderIs3D(int borderis3d);

	virtual CEqmDrawGraphicRect* Copy();
protected:
	void                UpdateCenterPos() const;

	void                ChangeSizeLeft(const Gdiplus::PointF& actPt);
	void                ChangeSizeRight(const Gdiplus::PointF& actPt);
	void                ChangeSizeUp(const Gdiplus::PointF& actPt);
	void                ChangeSizeDown(const Gdiplus::PointF& actPt);

	int         m_rectborderIs3D;
	int        m_borderColor;  //±ß¿ò±³¾°É«
};
