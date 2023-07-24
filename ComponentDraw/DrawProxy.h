#pragma once

/*
Author:		Juneay
Time:		2010-11-14
Comments:	��ͼ�����࣬��Ҫ������洫�������ݣ������õײ��ͼ��ڻ���ͼ��
*/

#if !defined(AFX_MYDBDRAW_H)
#define AFX_MYDBDRAW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** 
*	\brief ˫�����ͼ��
*
*	�����˫�����ͼ��������
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


#define DEFAULTSIDELINEWIDTH 6 //�ܵ���Ĭ�Ͽ�ȡ�

//����ʵ�ֵ�������ʱ������ҳ���id
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
	void  UpdateScaleFactor();						///< ���ڴ�С�ı�ʱ������������
	RectF GetViewRect();							///< ��ȡָ������������С
	RectF GetSceneRect();							///< ��ȡ����������ʵ�ʴ�С
	void  ReflashScreen();	///< ˢ�½���

    void DrawPipe(UINT layer);
    void DrawCreate2DGraph(UINT layer);
    void DrawCreatePipe(UINT layer );
    void Draw2DGraphic(UINT layer );
    void DrawDevices(UINT layer );
	void DrawText(UINT layer);

	//�Ϲ�΢�����ʾ��Ԫ��
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
	///< ��д�����麯��������ϵת��
	virtual void prepareDraw();				
	
	///< ��д�����麯�������ƾ�̬����
	virtual void drawStaticScene();			

	///< ��д�����麯�������ƶ�̬����
	virtual void drawDynamicScene();		
	
	//�Ƿ�ѡ�����豸�ı߿�
	Direction  PickObjectSide(void *pDataSt = NULL);
	
	//ִ���豸�ƶ�����
	BOOL DoMoveAct(const bool bDown, UINT nFlags, DrawContext* pDataSt);
	
	//�����
	void LeftAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//�Ҷ���
	void RightAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//�϶���
	void TopAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//�¶���
	void BottomAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//ˮƽ����
	void CrossAlign(vector<CEqmDrawBase *> & vecEqmt);
	
	//��ֱ����
	void DownAlign(vector<CEqmDrawBase *> & vecEqmt);
		
	//�ȿ�
	void MakeSameWidth(vector<CEqmDrawBase *> &vecEqmt);

	//�ȸ�
	void MakeSameHeight(vector<CEqmDrawBase *> &vecEqmt);

	//�ȴ�С
	void MakeSameSize(vector<CEqmDrawBase *> &vecEqmt);
	
	//�ȷ��//��ʽˢ
	void FormatBrush(vector<CEqmDrawBase *> &vecEqmt);
	
	//����ת
	void LeftRotate90(CEqmDrawBase * pEqmt);//����ת90��
	
	//����ת
	void RightRotate90(CEqmDrawBase * pEqmt);//����ת90��
	
	//�������ϼ�
	void ShiftEqmtsUp(DrawContext*	pDataSt);        
	
	//�������¼�
	void ShiftEqmtsDown(DrawContext* pDataSt);      
	
	//���������
	void ShiftEqmtsLeft(DrawContext* pDataSt);      
	
	//�������Ҽ�
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
	//�Ϲ�΢���Ӻ���
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
	REAL			m_dScaleFactor;///< ��������
};

Gdiplus::RectF GetRectFrom2Points(float x0, float y0, float x1, float y1);

#endif // !defined(AFX_DRAWPROXY_H)