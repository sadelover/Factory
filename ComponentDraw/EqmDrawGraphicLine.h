
#pragma once

#include "EqmDrawGraphic.h"

class DLLEXPORT CEqmDrawGraphicLine : public CEqmDrawGraphic
{
public:
    CEqmDrawGraphicLine();

    CEqmDrawGraphicLine(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid, 
						int ident,  int linecolor,int filledcolor,
						int linewidth, int linestype, const string& pointlist,
						int forwardpageid = 0, double transparency = 0.0, int width = 0, int height = 0);

    virtual ~CEqmDrawGraphicLine();
	
    virtual int     GetWidth() const;
    virtual int     GetHeight() const;

    virtual void    Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL); 
    virtual bool    PointInMe(const Gdiplus::PointF& actPt) const; 
	
	virtual Direction	PointIsSide(const Gdiplus::PointF& actPt) ;

    virtual void    DrawSelectEdge(Gdiplus::Graphics *pGraphics);
	
	//change the equipment size
	virtual bool ChangeSize(const Gdiplus::PointF& actPt, Direction dir);

    virtual const Gdiplus::PointF&	    GetPos() const;;
    virtual void	                    SetPos(const Gdiplus::PointF& centerpos);


    virtual void    DrawPort(Gdiplus::Graphics *pGraphics); 

	virtual CEqmDrawGraphicLine* Copy();

protected:
    void            UpdateCenterPos() const;

private:
	int	m_pointindex;

	int m_pointcount;
};

