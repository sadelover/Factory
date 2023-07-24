#ifndef eqmdrawgraphic_h__
#define eqmdrawgraphic_h__
#include "EqmDrawBase.h"
#pragma warning(disable:4251)
#include <string>
#include <vector>
using  std::vector;
using  std::string;
//
//#define SELECTOFFSET 4 //选取2D图形边线的偏移距离
#define SELECTOFFSET 10 //选取2D图形边线的偏移距离
//////////////////////////////////////////////////////////////////////////

class DLLEXPORT CEqmDrawGraphic : public CEqmDrawBase
{
public:
	virtual ~CEqmDrawGraphic();

protected:
	CEqmDrawGraphic();

	CEqmDrawGraphic(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid, 
		int ident,  int linecolor,int filledcolor,
		int linewidth, int linestype, const string& pointlist,
		int forwardpageid, double transparency, int width = 0, int height = 0,int ifborder=0);
public:
	//get and set function	

	int		GetLineColor() const;
	void	SetLineColor(int linecolor);

	int		GetLineWidth() const;
	void	SetLineWidth(int linewidth);

	int		GetLineStyle() const;
	void	SetLineStyle(int linestyle);

	void	SetFilledColor(int color);
	int		GetFilledColor() const;

	string	GetPointStringList() const;
	void	SetPointStringList(const string& strpointlist);

	vector<Gdiplus::PointF>&  GetPointList();
	void	SetPointList(const vector<Gdiplus::PointF>& pointlist);

	virtual int    GetWidth() const;
	void   SetWidth(int width);
	void   SetHeight(int height);
	virtual int    GetHeight() const;

	int	   GetForwardPageID() const;
	void   SetForwardPageID(int pageid);

	void   SetTransparent(double dTransparent);
	double GetTransparent();

	int    GetBorderIs3D();
	void    SetBorderIs3D(int borderis3d);

	//virtual const Gdiplus::PointF&	    GetPos() const; ;
	virtual void	SetPos(const Gdiplus::PointF& centerpos);
public:
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);             //draw the equipment

	virtual bool PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment

	virtual Direction PointIsSide(const Gdiplus::PointF& actPt) ;      //check if the mouse point at the side of equipment

	virtual void DrawSelectEdge(Gdiplus::Graphics *pGraphics) ;	 //Draw the select edge when the mouse selects the equipment

	virtual bool ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size

	virtual void DrawPort(Gdiplus::Graphics *pGraphics);  //draw port

	virtual bool CopyProperty(CEqmDrawBase *pBase);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);
	virtual bool DoMoveEqmt(Gdiplus::PointF prePt, Gdiplus::PointF actPt); //移动设备
	virtual bool ShiftEqmtsUp();        //按下向上键
	virtual bool ShiftEqmtsDown();      //按下向下键
	virtual bool ShiftEqmtsLeft();      //按下向左键
	virtual bool ShiftEqmtsRight();     //按下向右键
private:

protected:
	vector<Gdiplus::PointF>	m_pointlist;
	double  m_dTransparent;         //透明度
	int		m_linecolor;			//线的颜色
	int		m_filledcolor;			//填充颜色
	int		m_linewidth;			//线宽
	int		m_linestyle;			//线的类型
	int     m_rectborderIs3D;       //矩形边框是否3D
private:


	int		m_forwardpageid;
};

#endif
