

#ifndef CEQMDRAWPAGE_H__
#define CEQMDRAWPAGE_H__

#pragma  once

#include "Tools/DllDefine.h"
#include  <atlimage.h>
#include "EqmDrawDefines.h"


#pragma warning(disable:4251)

#include <list>

using namespace std;

enum PAGETYPE
{
	PAGETYPE_UNKNOWN = 0,
	FULLSCREEN,			// 
	FLOATING,		// 
	WEBTYPE,
	TEMPLATE,
};

// this class defines a page of the monitor view application 
// a page contains a list of picture devices, texts, and pipes lines.
//
//
class CEqmDrawBase;
class CEqmDrawDevice;
class CEqmDrawText;
class CEqmDrawGraphic;
class CEqmDrawPipe;

//严观微添加
class CEqmDrawBend;
class CEqmDrawDashBoard;
class CEqmDrawLineGraphic;
class CEqmDrawBarGraphic;
class CEqmDrawPieGraphic;
class CEqmDrawButton;
class CEqmDrawTimeSelect;
class CEqmDrawTimeBar;
class CEqmDrawProgressBar;
class CEqmDrawDiagnoseGraphic;
class CEqmDrawPolygon;
class CEqmDrawAreaChange;
class CEqmDrawShellExecute;
class CEqmDrawEmbededPage;
class CEqmDrawLiquidLevel;
class CEqmDrawButtonPlus;
class CEqmDrawScrawl;
class CEqmDrawHistoryTrend;
class CEqmDrawEnergySaveRoi;
class CEqmDrawMapNavigate;
class CEqmDrawNavigateWindow;
class CEqmDrawLevelRuler;
class CEqmDrawDataAnalyse;
class CEqmDrawTemperDistri;
class CEqmDrawDataReport;
class CEqmDrawPlaneTempeDistr;
class CEqmDrawDottedLineFrm;
class CEqmDrawGeneralComponent;
class CEqmDrawRectangle;
class CEqmDrawStraightLine;
//严观微添加osg元素
class COSGHistoryCurve;			//历史曲线
class COSGPlusCompareBar;		//柱状
class COSGPlusBarChart;			//柱状
class COSGPlusButton;			//按钮
class COSGPlusImage;			//图片
class COSGPlusLineChart;		//折线图
class COSGPlusMeterPan;			//仪表盘
class COSGPlusStatic;			//文字
class COSGPlusTimeBar;			//时间序列图
class COSGPlusDiagnoseChart;	//诊断图
class COSGPlusDiagnoseChartPlus;//点阵诊断图
class COSGPlusMouseImageChanger;//鼠标移动事件
class COSGPlusShellExecute;	
class COSGPlusMapNavigate;
class COSGPlusEmbededPage;
class COSGPlusButtonPlus;
class COSGPlusScrawl;
class COSGPlusCurve;
class COSGPlusROIChart;

class DLLEXPORT CEqmDrawPage
{	
public:
	CEqmDrawPage();
	CEqmDrawPage(int pageid, const wstring& name, int projectid, int pageshoworder,
		PAGETYPE mtype,int mwidth,int mheight,int mxposition,int myposition,
		DWORD mbackgroundColor  = DEFAULTBACKGROUNDCOLOR);

	~CEqmDrawPage();
public:

	list<CEqmDrawDevice*>				m_devicelist;
	list<CEqmDrawPipe*>					m_pipelist;
	list<CEqmDrawText*>					m_textlist;
	list<CEqmDrawGraphic*>				m_graphiclist;
	list<CEqmDrawTimeSelect*>			m_timeselectlist;
	list<CEqmDrawTimeBar*>				m_timebarlist;
	list<CEqmDrawProgressBar*>			m_progressbarlist;
	list<CEqmDrawDiagnoseGraphic*>		m_diagnoselist;
	list<CEqmDrawPolygon*>				m_polygonList;
	list<CEqmDrawAreaChange*>			m_areachangelist;
	list<CEqmDrawShellExecute*>			m_shellexecutelist;
	list<CEqmDrawEmbededPage*>			m_embededpagelist;
	list<CEqmDrawLiquidLevel*>			m_liquidLevellist;
	list<CEqmDrawButtonPlus*>			m_chkBtnPluslist;
	list<CEqmDrawScrawl*>				m_scrawllist;
	list<CEqmDrawHistoryTrend*>			m_historyTrendlist;
	list<CEqmDrawEnergySaveRoi*>		m_energySaveRoilist;
	list<CEqmDrawMapNavigate*>			m_mapNavigatelist;
	list<CEqmDrawNavigateWindow*>		m_navigateWindowlist;
	list<CEqmDrawLevelRuler*>			m_levelRulerlist;
	list<CEqmDrawDataAnalyse*>			m_dataAnalyselist;
	list<CEqmDrawTemperDistri*>			m_temperDistrilist;
	list<CEqmDrawDataReport*>			m_dataReportlist;
	list<CEqmDrawPlaneTempeDistr*>		m_planeTempeDistrlist;
	list<CEqmDrawDottedLineFrm*>		m_dottedLineFrmlist;
	list<CEqmDrawGeneralComponent*>		m_generalComponentlist;
	list<CEqmDrawRectangle*>			m_rectangleList;
	list<CEqmDrawStraightLine*>			m_straightLineList;
	//严观微添加管道弯头，仪表，折线图，柱状图，饼图成员变量
	list<CEqmDrawBend*>				m_bendlist;
	list<CEqmDrawDashBoard*>		m_dashboardlist;
	list<CEqmDrawLineGraphic*>		m_linechartlist;
	list<CEqmDrawBarGraphic*>		m_barchartlist;
	list<CEqmDrawPieGraphic*>		m_piechartlist;
	list<CEqmDrawButton*>           m_buttonlist;
	//osg元素列表   柱状图
	list<COSGPlusCompareBar*>			m_osg_compare_bar_chart_list;
	list<COSGPlusBarChart*>				m_osg_bar_chart_list;
	list<COSGPlusLineChart*>			m_osg_line_chart_list;
	list<COSGPlusMeterPan*>				m_osg_meter_pan_list;
	list<COSGPlusButton*>				m_osg_button_list;
	list<COSGPlusTimeBar*>				m_osg_time_bar_list;
	list<COSGPlusDiagnoseChart*>		m_osg_diagnose_chart_list;
	list<COSGPlusDiagnoseChartPlus*>	m_osg_diagnose_chart_list_plus;
	list<COSGPlusMouseImageChanger*>	m_osg_mouse_image_changer_list;
	list<COSGPlusShellExecute*>			m_osg_shellexecute_list;
	list<COSGPlusMapNavigate*>			m_osg_mapnavigate_list;
	list<COSGPlusEmbededPage*>			m_osg_embededpage_list;
	list<COSGPlusButtonPlus*>			m_osg_button_plus_list;
	list<COSGPlusScrawl*>				m_osg_scrawl_list;
	list<COSGPlusCurve*>				m_osg_history_trend_list;
	list<COSGPlusROIChart*>				m_osg_roi_list;


	list<CEqmDrawDevice*>&					GetDrawDeviceList();
	list<CEqmDrawText*>&					GetPageTextList();
	list<CEqmDrawPipe*>&					GetPagePipeList();
	list<CEqmDrawGraphic*>&					Get2DGraphicList();
	list<CEqmDrawTimeSelect*>&				GetTimeSelectList();
	//严观微添加获取新增元素的函数
	list<CEqmDrawBend*>&					GetBendList();
	list<CEqmDrawDashBoard*>&				GetDashboardList();
	list<CEqmDrawLineGraphic*>&				GetLineGraphList();
	list<CEqmDrawBarGraphic*>&				GetBarGraphList();
	list<CEqmDrawPieGraphic*>&				GetPieGraphList();
	list<CEqmDrawButton*>&					GetButtonList();
	list<CEqmDrawTimeBar*>&					GetTimeBarList();
	list<CEqmDrawProgressBar*>&				GetProgressBarList();
	list<CEqmDrawDiagnoseGraphic*>&			GetDiagnoseList();
	list<CEqmDrawPolygon*>&					GetPolygon();
	list<CEqmDrawAreaChange*>&				GetAreaChangeList();
	list<CEqmDrawShellExecute*>&			GetShellExecuteList();
	list<CEqmDrawEmbededPage*>&				GetEmbededPageList();
	list<CEqmDrawLiquidLevel*>&				GetLiquidLevelList();
	list<CEqmDrawButtonPlus*>&				GetCheckButtonPlusList();
	list<CEqmDrawScrawl*>&					GetScrawlList();
	list<CEqmDrawHistoryTrend*>&			GetHistoryTrendList();
	list<CEqmDrawEnergySaveRoi*>&			GetEnergySaveRoiList();
	list<CEqmDrawMapNavigate*>&				GetMapNavigateList();
	list<CEqmDrawNavigateWindow*>&			GetNavigateWindowList();
	list<CEqmDrawLevelRuler*>&				GetLevelRulerList();
	list<CEqmDrawDataAnalyse*>&				GetDataAnalyseList();
	list<CEqmDrawTemperDistri*>&			GetTemperDistriList();
	list<CEqmDrawDataReport*>&				GetDataReportList();
	list<CEqmDrawPlaneTempeDistr*>&			GetPlaneTempeDistrList();
	list<CEqmDrawDottedLineFrm*>&			GetDottedLineFrmList();
	list<CEqmDrawGeneralComponent*>&		GetGeneralComponentList();
	list<CEqmDrawRectangle*>&				GetRectangleList();
	list<CEqmDrawStraightLine*>&			GetStraightLineList();
	//严观微添加获取osg元素的函数
	list<COSGPlusCompareBar*>&				GetOsgCompareBarChartList();
	list<COSGPlusBarChart*>&				GetOsgBarChartList();
	list<COSGPlusLineChart*>&				GetOsgLineChartList();
	list<COSGPlusMeterPan*>&				GetOsgMeterPanList();
	list<COSGPlusButton*>&					GetOsgButtonList();
	list<COSGPlusTimeBar*>&					GetOsgTimeBarList();
	list<COSGPlusDiagnoseChart*>&			GetOsgDiagnoseChartList();
	list<COSGPlusDiagnoseChartPlus*>&		GetOsgDiagnoseChartListPlus();
	list<COSGPlusMouseImageChanger*>&		GetMouseImageChangerList();	
	list<COSGPlusShellExecute*>&			GetOsgShellExecuteList();
	list<COSGPlusMapNavigate*>&				GetOsgMapNavigateList();
	list<COSGPlusEmbededPage*>&				GetOsgEmbededPageList();
	list<COSGPlusButtonPlus*>&				GetOsgButtonPlusList();
	list<COSGPlusScrawl*>&					GetOsgScrawlList();
	list<COSGPlusCurve*>&					GetOsgHistoryTrendList();
	list<COSGPlusROIChart*>&				GetOSGROIChartList();
	

	void					AddDevice(CEqmDrawDevice* device);
	void					AddText(CEqmDrawText* text);
	void					AddPipe(CEqmDrawPipe* pile);
	void					Add2DGraphic(CEqmDrawGraphic* graphic);
	void					AddBase(CEqmDrawBase* pBase);
	//严观微添加增加新元素的的函数
	void					AddBend(CEqmDrawBend* pBend);
	void					AddDashboard(CEqmDrawDashBoard* pDashboard);
	void					AddLineGraphic(CEqmDrawLineGraphic* pLineGraphic);
	void					AddBarGraphic(CEqmDrawBarGraphic* pBarGraphic);
	void					AddPieGraphic(CEqmDrawPieGraphic* pPieGraphic);
	void					AddButton(CEqmDrawButton* pButton);
	void					AddTimeSelect(CEqmDrawTimeSelect* pTimeSelect);
	void					AddTimeBar(CEqmDrawTimeBar* pTimeBar);
	void					AddProgressBar(CEqmDrawProgressBar* pProgressBar);
	void					AddDiagnoseGraphic(CEqmDrawDiagnoseGraphic* pDiagnoseGraphic);
	void					AddPolygon(CEqmDrawPolygon* pDiagnoseGraphic);
	void					AddAreaChange(CEqmDrawAreaChange* pAreaChange);
	bool					FindAreaChange(const int nPageID, const int nID);
	bool					FindOSGMouseImageChanger(const int nPageID, const int nID);
	void					AddShellExecute(CEqmDrawShellExecute* pShellExecute);
	void					AddEmbededPage(CEqmDrawEmbededPage* pEmbededPage);
	void					AddLiquidLevel(CEqmDrawLiquidLevel* pLiquidLevel);
	void					AddCheckButtonPlus(CEqmDrawButtonPlus* pChkBtnPlus);
	void					AddScrawlBoard(CEqmDrawScrawl* pScrawl);
	void					AddHistoryTrend(CEqmDrawHistoryTrend* pHistTrend);
	void					AddEnergySaveRoi(CEqmDrawEnergySaveRoi* pEnergySaveRoi);
	void					AddMapNavigate(CEqmDrawMapNavigate* pMapNavigate);
	void					AddNavigateWindow(CEqmDrawNavigateWindow* pNavigateWindow);
	void					AddLevelRuler(CEqmDrawLevelRuler* pLevelRuler);
	void					AddDataAnalyse(CEqmDrawDataAnalyse* pDataAnalyse);
	void					AddTemperDistri(CEqmDrawTemperDistri* p);
	void					AddDataReport(CEqmDrawDataReport* p);
	void					AddPlaneTempeDistr(CEqmDrawPlaneTempeDistr* p);
	void					AddDottedLineFrm(CEqmDrawDottedLineFrm* p);
	void					AddGeneralComponent(CEqmDrawGeneralComponent* p);
	//严观微添加增加osg元素的函数
	void					AddOsgCompareBarChart(COSGPlusCompareBar* pOsgBarChart);
	void					AddOsgBarChart( COSGPlusBarChart* pOsgBarChart );
	void					AddOsgLineChart(COSGPlusLineChart* pOsgLineChart);
	void					AddOsgMeterPan(COSGPlusMeterPan* pOsgMeterPan);
	void					AddOsgButton(COSGPlusButton* pButton);
	void					AddOsgTimeBar(COSGPlusTimeBar* pTimeBar);
	void					AddOsgDiagnoseChart(COSGPlusDiagnoseChart* pChart);
	void					AddOsgDiagnoseChartPlus(COSGPlusDiagnoseChartPlus* pChart);
	void					AddMouseImageChanger(COSGPlusMouseImageChanger* pMouseChage);
	void					AddOsgShellExecute(COSGPlusShellExecute* pShellExecute);
	void					AddOsgEmbededPage(COSGPlusEmbededPage* pEmbededPage);
	void					AddOsgCheckButtonPlus(COSGPlusButtonPlus* pChkBtnPlus);
	void					AddOsgScrawl(COSGPlusScrawl* pScrawl);
	void					AddOsgHistoryTrend(COSGPlusCurve* pHistoryTrend);
	void					AddROIChart(COSGPlusROIChart* pChart);
	//Mark添加增加osg元素的函数
	void					AddRectangle(CEqmDrawRectangle* p);
	void					AddStraightLine(CEqmDrawStraightLine *p);
    void					DeleteBase(CEqmDrawBase* pBase);
	void					DeleteDevice(CEqmDrawDevice* device);
	void					DeletePipe(CEqmDrawPipe* text);
	void					DeleteText(CEqmDrawText* pile);
	void					Delete2DGraphic(CEqmDrawGraphic* graphic);
	void					DeleteButton(CEqmDrawButton* button);
	void					DeleteTimeSelect(CEqmDrawTimeSelect* timeselect);
	void					DeleteTimeBar(CEqmDrawTimeBar* timebar);
	void					DeleteProgressBar(CEqmDrawProgressBar* progressbar);
	void					DeleteDiagnoseGraphic(CEqmDrawDiagnoseGraphic* pDiagnoseGraphic);
	void					DeletePolygon(CEqmDrawPolygon* pPolygon);
	void					DeleteAreaChange(CEqmDrawAreaChange* pAreaChange);
	void					DeleteShellExecute(CEqmDrawShellExecute* pShellExecute);
	void					DeleteEmbededPage(CEqmDrawEmbededPage* pEmbededPage);
	void					DeleteLiquidLevel(CEqmDrawLiquidLevel* pLiquidLevel);
	void					DeleteCheckButtonPlus(CEqmDrawButtonPlus* pChkBtnPlus);
	void					DeleteScrawlBoard(CEqmDrawScrawl* pScrawl);
	void					DeleteHistoryTrend(CEqmDrawHistoryTrend* pHistTrend);
	void					DeleteEnergySaveRoi(CEqmDrawEnergySaveRoi* pEnergySaveRoi);
	void					DeleteMapNavigate(CEqmDrawMapNavigate* pMapNavigate);
	void					DeleteNavigateWindow(CEqmDrawNavigateWindow* pNavigateWindow);
	void					DeleteLevelRuler(CEqmDrawLevelRuler* pLevelRuler);
	void					DeleteDataAnalyse(CEqmDrawDataAnalyse* pDataAnalyse);
	void					DeleteTemperDistri(CEqmDrawTemperDistri* p);
	void					DeleteDataReport(CEqmDrawDataReport* p);
	void					DeletePlaneTempeDistr(CEqmDrawPlaneTempeDistr* p);
	void					DeleteDottedLineFrm(CEqmDrawDottedLineFrm* p);
	void					DeleteGeneralComponent(CEqmDrawGeneralComponent* p);
	//严观微增加删除新元素的函数
	void					DeleteBend(CEqmDrawBend* pBend);
	void					DeleteDashboard(CEqmDrawDashBoard* pDashboard);
	void					DeleteLineGraphic(CEqmDrawLineGraphic* pLineGraphic);
	void					DeleteBarGraphic(CEqmDrawBarGraphic* pBarGraphic);
	void					DeletePieGraphic(CEqmDrawPieGraphic* pPieGraphic);
	//mark增加删除新元素的函数
	void					DeleteRectangle(CEqmDrawRectangle* p);
	void					DeleteStraightLine(CEqmDrawStraightLine* p);
	int						GetID() const ;
	void					SetID(int pageid);
    UINT					GetObjectsMaxId() const;

	wstring					GetPageName() const;
	void					SetPageName(const wstring& pagename);

	CEqmDrawDevice*			GetDevice(int id);
	CEqmDrawPipe *			GetPipe(int id);
	CEqmDrawText *			GetText(int id);
	CEqmDrawGraphic*		Get2DGraphic(int id);

	CEqmDrawBase*			GetElementBase(const EQMDRAWTYPE type,int id);

//add by golding
	bool					FindText(wstring strPointName, vector<CEqmDrawText *> & textList);
	bool					FindDevice(wstring strPointName, vector<CEqmDrawDevice *> & deviceList);

	//严观微增加通过id获取新元素的函数
	CEqmDrawBend*					GetBend(const int nID);
	CEqmDrawDashBoard*				GetDashBoard(const int nID);
	CEqmDrawLineGraphic*			GetLineGraphic(const int nID);
	CEqmDrawBarGraphic*				GetBarGraphic(const int nID);
	CEqmDrawPieGraphic*				GetPieGraphic(const int nID);
	CEqmDrawButton*					GetButton(const int nID);
	CEqmDrawTimeSelect*				GetTimeSelect(const int nID);
	CEqmDrawTimeBar*				GetTimeBar(const int nID);
	CEqmDrawProgressBar*			GetProgressBar(const int nID);
	CEqmDrawDiagnoseGraphic*		GetDiagnoseGraphic(const int nID);
	CEqmDrawPolygon*				GetPolygon(const int nID);
	CEqmDrawAreaChange*				GetAreaChange(const int nID);
	CEqmDrawShellExecute*			GetShellExecute(const int nID);
	CEqmDrawEmbededPage*			GetEmbededPage(const int nID);
	CEqmDrawLiquidLevel*			GetLiquidLevel(const int nID);
	CEqmDrawButtonPlus*				GetCheckButtonPlus(const int nID);
	CEqmDrawScrawl*					GetScrawl(const int nID);
	CEqmDrawHistoryTrend*			GetHistoryTrend(const int nID);
	CEqmDrawEnergySaveRoi*			GetEnergySaveRoi(const int nID);
	CEqmDrawMapNavigate*			GetMapNavigate(const int nID);
	CEqmDrawNavigateWindow*			GetNavigateWindow(const int nID);
	CEqmDrawLevelRuler*				GetLevelRuler(const int nID);
	CEqmDrawDataAnalyse*			GetDataAnalyse(const int nID);
	CEqmDrawTemperDistri*			GetTemperDistri(const int nID);
	CEqmDrawDataReport*				GetDataReport(const int nID);
	CEqmDrawPlaneTempeDistr*		GetPlaneTempeDistr(const int nID);
	CEqmDrawDottedLineFrm*			GetDottedLineFrm(const int nID);
	CEqmDrawGeneralComponent*		GetGeneralComponent(const int nID);
	CEqmDrawRectangle*				GetRectangle(const int nID);
	CEqmDrawStraightLine*			GetStraightLine(const int nID);	

	int						GetProjectID() const;
	void					SetProjectID(int projectid);
	void					ClearAll();

	PAGETYPE				GetPageType() const;
	void					SetPageType( PAGETYPE mtype );

	int						GetPageWidth() const;
	void					SetPageWidth(int mwidth);

	int						GetPageHeight() const;
	void					SetPageHeight(int mheight);

	int						GetPagexPosition() const;
	void					SetPagexPosition(int mxposition);

	int						GetPageyPosition() const;
	void					SetPageyPosition(int myposition);
	void					SetGroupID(const int nGroupID);
	const int				GetGroupID();
	void					SetStartColor(const int nStartColor);
	const int				GetStartColor();
	void					SetEndColor(const int nEndColor);
	const int				GetEndColor();
	void					SetBKImageID(const int nBKImageID);
	const int				GetBKImageID();
	CEqmDrawPage*			Copy();
	void					SetPageShowTopInObserver(const bool bShow);
	const bool				GetPageShowTopInObserver();

	void					SetBkImage(Image* pImage);
	Image*					GetBkImage(void) const;

	void					SetWebAddress(const CString strWebAddr);
	CString					GetWebAddress(void) const;

	void					SetPageShowOrder(const int nPageOrder);
	int						GetPageShowOrder(void) const;

	void					SetPageMinRight(const int nValue);
	int						GetPageMinRight(void) const;

	int m_nWarningCount;
private:	
	
	CEqmDrawPage& operator =(const CEqmDrawPage&);
	int						m_pageid;		//pageid
	wstring					m_pagename;	//pagename
	int						m_projectid;	//project id
	PAGETYPE				m_type;
	int						m_width;
	int						m_height;
	int						m_xposition;
	int						m_yposition;
	int						m_nStartColor;
	int						m_nEndColor;
	int						m_nBKImageID;
	bool					m_bShowTopInObserver;
	Image*					m_pBkImage;
	CString					m_strWedAddress;
	int						m_nPageOrder;
	int                     m_nPageMinRight;

public:
	int   m_GroupID;
	bool	m_bIsMainPage;
	bool	m_bNeedSave;
	bool    m_bDetailLoaded;
	CEqmDrawText* m_pPickText;
};

#endif
