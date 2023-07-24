
#pragma once


#include "EqmDrawGraphic.h"

class DLLEXPORT CEqmDrawGraphicEllipse : public CEqmDrawGraphic
{
public:
    CEqmDrawGraphicEllipse();

    CEqmDrawGraphicEllipse(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid, 
        int ident,  int linecolor,int filledcolor,
        int linewidth, int linestype, const string& pointlist,
        int forwardpageid, double transparency, int width = 0, int height = 0);

    virtual ~CEqmDrawGraphicEllipse();

public:

    virtual int     GetWidth() const;
    virtual int     GetHeight() const;
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);
    virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL); 
    virtual bool PointInMe(const Gdiplus::PointF& actPt) const; 
	Direction PointIsSide(const Gdiplus::PointF& actPt ) ;
    virtual void DrawSelectEdge(Gdiplus::Graphics *pGraphics);
	bool ChangeSize( const Gdiplus::PointF& actPt, Direction dir );
	const Gdiplus::PointF& GetPos() const;
    virtual void	            SetPos(const Gdiplus::PointF& centerpos);

	void                UpdateCenterPos() const;

	void                ChangeSizeLeft(const Gdiplus::PointF& actPt);
	void                ChangeSizeRight(const Gdiplus::PointF& actPt);
	void                ChangeSizeUp(const Gdiplus::PointF& actPt);
	void                ChangeSizeDown(const Gdiplus::PointF& actPt);
	
	virtual CEqmDrawGraphicEllipse* Copy();
};