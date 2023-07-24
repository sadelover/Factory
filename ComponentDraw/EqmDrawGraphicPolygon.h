
#pragma once


#include "EqmDrawGraphic.h"

class DLLEXPORT CEqmDrawGraphicPolygon : public CEqmDrawGraphic
{
public:
    CEqmDrawGraphicPolygon();

    CEqmDrawGraphicPolygon(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid, 
        int ident,  int linecolor,int filledcolor,
        int linewidth, int linestype, const string& pointlist,
        int forwardpageid, double transparency, int width = 0, int height = 0);

    virtual ~CEqmDrawGraphicPolygon();

public:
	void CreateObjects();
	void DestoryObjects();
    virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL); 
    virtual bool PointInMe(const Gdiplus::PointF& actPt) const; 
	virtual CEqmDrawGraphicPolygon* Copy();
private:

	Gdiplus::Pen*			m_pPolygonPen;
	Gdiplus::SolidBrush*	m_pBKBrush;
};

