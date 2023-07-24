#pragma once

/*
Author:		Juneay
Time:		2010-11-14
Comments:	绘图代理类，主要处理界面传来的数据，并调用底层绘图借口绘制图形
*/

#if !defined(AFX_MYDBDRAW_H)
#define AFX_MYDBDRAW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** 
*	\brief 双缓冲绘图类
*
*	该类从双缓冲绘图基类派生
*/
#include "DrawObject.h"
#include "EqmDrawDefines.h"

#include <vector>

using namespace std;

#define				FIELD_WIDTH				(440.f)
#define				FIELD_HEIGHT			(280.f)
#define				CNT_RADIUS				(75.f / 2.f)
#define				GOAL_WIDTH				(20.f)
#define				GOAL_HEIGHT				(60.f)
#define				ROBOT_EDGE				(7.5f)
#define				GOALAREA_WIDTH			(25.f)
#define				GOALAREA_HEIGHT			(80.f)
#define				PNTYAREA_WIDTH			(60.f)
#define				PNTYAREA_HEIGHT			(120.f)
#define				PK_RADIUS				(20.f)
#define				PK_CENTERX				(150.f)
#define				CORNER_SIDE				(7.f)


#define DEFAULTSIDELINEWIDTH 6 //管道的默认宽度。

//用于实现导航功能时，导航页面的id
#define INVALID_FORWARDPAGEID -1

//////////////////////////////////////////////////////////////////////////
class CEqmDrawBase;
class CEqmDrawDevice;
class CEqmDrawText;
class CEqmDrawGraphic;
class CEqmDrawPipe;

class DLLEXPORT  DrawProxy : public ComponentDraw
{
public:
	DrawProxy();
	virtual ~DrawProxy();

private:
	void  UpdateScaleFactor();						///< 窗口大小改变时更新缩放因子
	RectF GetViewRect();							///< 获取指定窗口视区大小
	RectF GetSceneRect();							///< 获取整个场景的实际大小
	void  ReflashScreen();	///< 刷新界面

    void DrawPipe(UINT layer);
    void DrawCreate2DGraph(UINT layer);
    void DrawCreatePipe(UINT layer );
    void Draw2DGraphic(UINT layer );
    void DrawDevices(UINT layer );
	void DrawText(UINT layer);

	//严观微添加显示新元素
	void	DrawBend(UINT layer );
	void	DrawDashBoard(UINT layer );
	void	DrawLineGraphic(UINT layer );
	void	DrawBarGraphic(UINT layer );
	void	DrawPieGraphic(UINT layer );
	void	DrawButton(UINT layer );
	void	DrawTimeSelect(UINT layer);
    void	DrawTimeBar(UINT layer);
	void	DrawProgressBar(UINT layer);
	void	DrawSelected(UINT layer );
	void	DrawDiagnoseGraphic(UINT layer);
	void	DrawPolygon(UINT layer);
	void	DrawCreatePolygon(UINT layer);
	void	DrawAreaChange(UINT layer);
	void	DrawShellExecute(UINT layer);
	void	DrawEmbededPage(UINT layer);
	void	DrawLiquidLevel(UINT layer);
	void	DrawCheckButtonPlus(UINT layer);
	void	DrawScrawl(UINT layer);
	void	DrawHistoryTrend(UINT layer);
	void	DrawEnergySaveRoi(UINT layer);
	void	DrawMapNavigate(UINT layer);
	void	DrawNavigateWindow(UINT layer);
	void	DrawLevelRuler(UINT layer);
	void	DrawDataAnalyse(UINT layer);
	void	DrawTemperDistri(UINT layer);
	void	DrawDataReport(UINT layer);
	void	DrawPlaneTempeDistr(UINT layer);
	void	DrawDottedLineFrm(UINT layer);
	void	DrawGeneralComponent(UINT layer);
	void	DrawRectangle(UINT layer);
	void	DrawStraightLine(UINT layer);

public:
	CEqmDrawBase* PickObject(CEqmDrawPage *pPage, double fX, double fY);
	///< 重写基类虚函数，坐标系转换
	virtual void prepareDraw();				
	
	///< 重写基类虚函数，绘制静态场景
	virtual void drawStaticScene();			

	///< 重写基类虚函数，绘制动态场景
	virtual void drawDynamicScene();		
	
	//是否选中了设备的边框
	Direction  PickObjectSide(void *pDataSt = NULL);
	
	//执行设备移动操作
	BOOL DoMoveAct(const bool bDown, UINT nFlags, DrawContext* pDataSt);
	
	//左对齐
	void LeftAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//右对齐
	void RightAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//上对齐
	void TopAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//下对齐
	void BottomAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//水平对齐
	void CrossAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//垂直对齐
	void DownAlign(vector<CEqmDrawBase *> & vecEqmt);
		
	//等宽
	void MakeSameWidth(vector<CEqmDrawBase *> &vecEqmt);

	//等高
	void MakeSameHeight(vector<CEqmDrawBase *> &vecEqmt);

	//等大小
	void MakeSameSize(vector<CEqmDrawBase *> &vecEqmt);
	
	//等风格//格式刷
	void FormatBrush(vector<CEqmDrawBase *> &vecEqmt);
	
	//左旋转
	void LeftRotate90(CEqmDrawBase * pEqmt);//左旋转90度
	
	//右旋转
	void RightRotate90(CEqmDrawBase * pEqmt);//右旋转90度
	
	//按下向上键
	void ShiftEqmtsUp(DrawContext*	pDataSt);        
	
	//按下向下键
	void ShiftEqmtsDown(DrawContext* pDataSt);      
	
	//按下向左键
	void ShiftEqmtsLeft(DrawContext* pDataSt);      
	
	//按下向右键
	void ShiftEqmtsRight(DrawContext* pDataSt);     

	void ShiftEqmtsUpCtrl(DrawContext*	pDataSt);        
	void ShiftEqmtsDownCtrl(DrawContext* pDataSt);      
	void ShiftEqmtsLeftCtrl(DrawContext* pDataSt);      
	void ShiftEqmtsRightCtrl(DrawContext* pDataSt);     
	CEqmDrawGraphic * IsPickRectangle(Gdiplus::PointF pt, void *pDateSt = NULL);

	void PickObjectArray(DrawContext *pDrawElement , std::vector<CEqmDrawBase*>& rItemArray);

	void PickObjectArrayDevice(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayPipe(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayGraphic(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayText(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	//严观微增加函数
	void PickObjectArrayBend(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayDashBoard(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayLineGraphic(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
    void PickObjectArrayBarGraphic(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayPieGraphic(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayButton(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayTimeSelect(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayTimeBar(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayProgressBar(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayDiagnoseGrapgic(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArraypPolygon(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayAreaChange(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayShellExecute(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayEmbededPage(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayLiquidLevel(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayCheckButtonPlus(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayScrawl(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayHistoryTrend(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayEnergySaveRoi(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayMapNavigate(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayNavigateWindow(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayLevelRuler(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayDataAnalyse(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayTemperDistri(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayDataReport(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayPlaneTempeDistr(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayDottedLineFrm(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayGeneralComponent(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayRectangle(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void PickObjectArrayStraightLine(DrawContext* pDrawElement , std::vector<CEqmDrawBase*>& rItemArray, const Gdiplus::RectF& rectSelect);
	void DrawMouseSelectBox(DrawContext& rDrawElement);

	void DrawGrid();

	void KeyEnterDown();

public:
	bool   m_bShow[E_LAYER_MAX];
protected:
	


private:
	REAL			m_dScaleFactor;///< 缩放因子
};

Gdiplus::RectF GetRectFrom2Points(float x0, float y0, float x1, float y1);

#endif // !defined(AFX_DRAWPROXY_H)