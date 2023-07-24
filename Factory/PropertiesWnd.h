
#pragma once
#include "../ComponentDraw/EqmDrawButton.h"
#include "HistoryTrendItemSetDlg.h"
#include "../ComponentDraw/EqmDrawAreaChange.h"
#include "../ComponentDraw/EqmDrawScrawl.h"
#include "../ComponentDraw/EqmDrawNavigateWindow.h"
#include "../ComponentDraw/EqmDrawTemperDistri.h"
#include "../ComponentDraw/EqmDrawLevelRuler.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphicPlus.h"
#include "../ComponentDraw/EqmDrawDataAnalyse.h"

class CEqmDrawBend;
class CEqmDrawText;
class CEqmDrawDevice;
class CEqmDrawBase;
class CDataPointManager;
class CMainFrame;
class CEqmDrawPipe;
class CEqmDrawButton;
class CEqmDrawDashBoard;
class CEqmDrawMapNavigate;
class CEqmDrawDiagnoseGraphic;
class CEqmDrawTimeSelect;
class CEqmDrawLineGraphic;
class CEqmDrawPieGraphic;
class CEqmDrawBarGraphic;
class CEqmDrawEnergySaveRoi;
class CEqmDrawHistoryTrend;
class CEqmDrawButtonPlus;
class CEqmDrawEmbededPage;
class CEqmDrawShellExecute;
class CEqmDrawAreaChange;
class CEqmDrawLiquidLevel;
class CEqmDrawScrawl;
class CEqmDrawTimeBar;
class CEqmDrawProgressBar;
class CEqmDrawNavigateWindow;
class CEqmDrawDataReport;
class CEqmDrawTemperDistri;
class CEqmDrawLevelRuler;
class CEqmDrawPlaneTempeDistr;
class CEqmDrawDottedLineFrm;
class CEqmDrawGeneralComponent;
class CEqmDrawRectangle;
class CEqmDrawStraightLine;
class CEqmDrawPolygon;

enum	E_SHOW_MODE
{
	E_SHOW_NULL = 0,
	E_SHOW_TEXT,
	E_SHOW_DEVICE,
	E_SHOW_OTHER,
	E_SHOW_PIPE,
	E_SHOW_BUTTON,
	E_SHOW_BEND,
	E_SHOW_DASHBOARD,
	E_SHOW_DIAGNOSEGRAPHIC,
	E_SHOW_MAPNAVIGATE,
	E_SHOW_TIME_SELECT,
	E_SHOW_LINE_CHART,
	E_SHOW_BAR_CHART,
	E_SHOW_PIE_CHART,
	E_SHOW_ROI,
	E_SHOW_TREND,
	E_SHOW_CHECK_BUTTON,
	E_SHOW_EMBEDED,
	E_SHOW_SHELL_EXECUTE,
	E_SHOW_AREA_CHANGE,
	E_SHOW_SCRAWL,
	E_SHOW_LIQUID,
	E_SHOW_TIME_BAR,
	E_SHOW_PROGRESS,
	E_SHOW_NAVIGATE,
	E_SHOW_DATA_ANALYSIS,
	E_SHOW_REPORT,
	E_SHOW_TEMP_DIS,
	E_SHOW_RULER,
	E_SHOW_POLYGON,
	E_SHOW_PLANE_TEMPE_DISTR,
	E_SHOW_DOTLINE_FRAME,
	E_SHOW_GENERAL_COMPONENT
};

enum	E_POP_DLG_TYPE
{
	E_POP_DLG_NULL = 0,		//不弹出
	E_POP_DLG_POINT,		//点表选择
	E_POP_DLG_IMAGE,		//图片
	E_POP_DATE_TIME,
	E_POP_DLG_ADD_DEVICE_INFO,	//设备添加事件
	E_POP_DLG_ADD_DEVICE_DIAGNOSE,	//诊断关联
	E_POP_DLG_ADD_NAVI_ITEM,		//地图导航设置
	E_POP_DLG_SELECT_DATA, //SelectDataComId
	E_POP_DLG_ADD_DIAGNOSE_ITME,		//诊断图项
	E_POP_DLG_SELECT_PATH,
};

struct PosInfo
{
	PosInfo(int xx = 0,int yy =0,int ww=0,int hh=0):
	x(xx),
	y(yy),
	w(ww),
	h(hh)
	{
	}

	int x;
	int y;
	int w;
	int h;
public:
	bool IsEqual(PosInfo & p)
	{
		if ((w == p.w)&& (h == p.h)&& (x == p.x)&&(y == p.y))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
};

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CBTinValueProperty :public CMFCPropertyGridProperty  
{  
public:  
	CBTinValueProperty(const CString& strName,const COleVariant& varValue,LPCTSTR lpszDescr = NULL);  
	CBTinValueProperty(const CString& strName);
	  virtual ~CBTinValueProperty()  
	  {}  

	  virtual BOOL HasButton() const;  

	  virtual void OnClickButton(CPoint point);  

	  void	SetPointMap(CDataPointManager* pointmap);
	  void	SetMainFrame(CMainFrame* pMainFrame);
	  void	SetPopDlgType(E_POP_DLG_TYPE type);

protected:  
	CDataPointManager* m_refpointmap;
	CMainFrame*			m_pMainFrame;
	E_POP_DLG_TYPE		m_eDlgType;

public:
	int					m_data; //used to store the data to avoid transformation between string and value
}; 

class CPropertiesWnd : public CDockablePane
{
// Construction
public:
	CPropertiesWnd();

	void AdjustLayout();

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;
	NONCLIENTMETRICS info;
// Implementation
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

	void	ShowTextProperty(CEqmDrawText* pText);
	void	ShowDeviceProperty(CEqmDrawDevice* pDevice);
	void	ShowEqmDrawBase(CEqmDrawBase* pBase);

	void	ChangeTextProperty(CMFCPropertyGridProperty* pProp,CEqmDrawText* pText);
	void	ChangeDeviceProperty(CMFCPropertyGridProperty* pProp,CEqmDrawDevice* pDevice);
	void	ChangeEqmDrawBase(CMFCPropertyGridProperty* pProp,CEqmDrawBase* pBase);
public:
	void	SetEqmDrawBase(CEqmDrawBase* pBase);
	void	SetTextProperty(CEqmDrawText* pText);
	void	SetDeviceProperty(CEqmDrawDevice* pDevice);	
	

	void	SetPointMap(CDataPointManager* pointmap);
	void	ShowPropertyWnd(BOOL bShow);
	void	SetNull();//need to be called in every setXproperty;
protected:
	CEqmDrawText*	m_pText;
	CEqmDrawDevice* m_pDevice;
	CEqmDrawBase*	m_pBase;

	CDataPointManager* m_refpointmap;//not maintained


	CEqmDrawPipe*	m_pPipe;
	CEqmDrawButton* m_pButton;
	CEqmDrawBend* m_pBend;
	CEqmDrawDashBoard* m_pBoard;
	CEqmDrawDiagnoseGraphic* m_pDiagnoseGraphic;		//诊断图
	CEqmDrawMapNavigate* m_pMapNavigate;		//地图导航
	CEqmDrawTimeSelect * m_pTime;
	CEqmDrawLineGraphic *m_pLine;
	CEqmDrawPieGraphic	*m_pPie;
	CEqmDrawBarGraphic	*m_pBar;
	CEqmDrawEnergySaveRoi * m_pROI;
	CEqmDrawHistoryTrend *m_pTrend;
	CEqmDrawButtonPlus * m_pCheck;
	CEqmDrawEmbededPage *m_pEmbed;
	CEqmDrawShellExecute* m_pExecute;
	CEqmDrawScrawl *m_pScrawl;
	CEqmDrawLiquidLevel * m_pLiquid;
	CEqmDrawTimeBar * m_pTbar;
	CEqmDrawProgressBar *m_pProgrs;
	CEqmDrawNavigateWindow	*m_pNav;
	CEqmDrawDataAnalyse *m_pAnalysis;
	CEqmDrawDataReport * m_pReport;
	CEqmDrawTemperDistri* m_pTemp;
	CEqmDrawLevelRuler * m_pRuler;
	CEqmDrawPolygon * m_pPolygon;
	CEqmDrawPlaneTempeDistr* m_pPlaneTempeDistr;
	CEqmDrawDottedLineFrm* m_pDottedLineFrm;
	CEqmDrawGeneralComponent* m_pGeneralComponent;
	CEqmDrawRectangle	*m_pRectangle;
	CEqmDrawStraightLine	*m_pStraightLine;

public:
	CEqmDrawAreaChange * m_pArea;
	E_SHOW_MODE		m_eShowMode;

public:
	void	SetNullProperty();		//置空

	void	SetPipeProperty(CEqmDrawPipe * pPipe);
	void	ChangePipeProperty(CMFCPropertyGridProperty* pProp,CEqmDrawPipe * pPipe);

	void	SetButtonProperty(CEqmDrawButton * pButton);
	void	ChangeButtonProperty(CMFCPropertyGridProperty* pProp,CEqmDrawButton * pButton);

	void	SetBendProperty(CEqmDrawBend *pBend);
	void	ChangeBendProperty(CMFCPropertyGridProperty* pProp,CEqmDrawBend *pBend);

	void	SetBoardProperty(CEqmDrawDashBoard *pBoard);
	void	ChangeBoardProperty(CMFCPropertyGridProperty* pProp,CEqmDrawDashBoard *pBoard);

	void	SetDiagnoseGraphicProperty(CEqmDrawDiagnoseGraphic *pDiagnoseGraphic);
	void	ChangepDiagnoseGraphicProperty(CMFCPropertyGridProperty* pProp,CEqmDrawDiagnoseGraphic *pDiagnoseGraphic);
	void	SetTimePickerProperty(CEqmDrawTimeSelect *pTime);
	void	ChangeTimePickerProperty(CMFCPropertyGridProperty* pProp,CEqmDrawTimeSelect *pTime);

	void	SetMapNavigateProperty(CEqmDrawMapNavigate *pMapNavigate);
	void	ChangeMapNavigateProperty(CMFCPropertyGridProperty* pProp,CEqmDrawMapNavigate *pMapNavigate);

	void	SetLineChartProperty(CEqmDrawLineGraphic* pLine);
	void	ChangeLineChartProperty(CMFCPropertyGridProperty* pProp,CEqmDrawLineGraphic* pLine);

	void	SetBarChartProperty(CEqmDrawBarGraphic* pBar);
	void	ChangebarChartProperty(CMFCPropertyGridProperty* pProp,CEqmDrawBarGraphic* pBar);

	void	SetPieChartProperty(CEqmDrawPieGraphic* pPie);
	void	ChangePieChartProperty(CMFCPropertyGridProperty* pProp,CEqmDrawPieGraphic* pPie);

	void	SetRectangleProperty(CEqmDrawRectangle* pRectangle);
	void	ChangeRectangleProperty(CMFCPropertyGridProperty* pProp,CEqmDrawRectangle* pRectangle);

	void	SetTrendProperty(CEqmDrawHistoryTrend* pTrend);
	void	ChangeTrendProperty(CMFCPropertyGridProperty* pProp,CEqmDrawHistoryTrend* pTrend);

	void	SetCheckButtonProperty(CEqmDrawButtonPlus* pCheck);
	void	ChangeCheckButtonProperty(CMFCPropertyGridProperty* pProp,CEqmDrawButtonPlus* pCheck);

	void	SetEmbededProperty(CEqmDrawEmbededPage* pEmbed);
	void	ChangeEmbededProperty(CMFCPropertyGridProperty* pProp,CEqmDrawEmbededPage* pEmbed);

	void	SetShellExecuteProperty(CEqmDrawShellExecute* pExecute);
	void	ChangeShellExecuteProperty(CMFCPropertyGridProperty* pProp,CEqmDrawShellExecute* pExecute);
	
	void	SetAreaChangeProperty(CEqmDrawAreaChange* pArea);
	void	ChangeAreaChangeProperty(CMFCPropertyGridProperty* pProp,CEqmDrawAreaChange* pArea);

	void	SetStraightLineProperty(CEqmDrawStraightLine* pStraightLine);
	void	ChangeStraightLineProperty(CMFCPropertyGridProperty* pProp,CEqmDrawStraightLine* pStraightLine);

	void	SetLiquidProperty(CEqmDrawLiquidLevel* pLiquid);
	void	ChangeLiquidProperty(CMFCPropertyGridProperty* pProp,CEqmDrawLiquidLevel* pLiquid);

	void	SetTimeBarProperty(CEqmDrawTimeBar* pTbar);
	void	ChangeTimeBarProperty(CMFCPropertyGridProperty* pProp,CEqmDrawTimeBar* pTbar);

	void	SetProgressProperty(CEqmDrawProgressBar* pProgrs);
	void	ChangeProgressProperty(CMFCPropertyGridProperty* pProp,CEqmDrawProgressBar* pProgrs);
	
	void	SetNaviProperty(CEqmDrawNavigateWindow* pNavi);
	void	ChangeNaviProperty(CMFCPropertyGridProperty* pProp,CEqmDrawNavigateWindow* pNavi);

	void	SetAnlyProperty(CEqmDrawDataAnalyse* pAnly);
	void	ChangeAnlyProperty(CMFCPropertyGridProperty* pProp,CEqmDrawDataAnalyse* pAnaly);
	
	void	SetReportProperty(CEqmDrawDataReport* pReport);
	void	ChangeReportProperty(CMFCPropertyGridProperty* pProp,CEqmDrawDataReport* pReport);
	
	void	SetTemperProperty(CEqmDrawTemperDistri* pTemper);
	void	ChangeTemperProperty(CMFCPropertyGridProperty* pProp,CEqmDrawTemperDistri* pTemper);

	void	SetRulerProperty(CEqmDrawLevelRuler* pRuler);
	void	ChangeRulerProperty(CMFCPropertyGridProperty* pProp,CEqmDrawLevelRuler* pRuler);

	void SetPolygonProperty(CEqmDrawPolygon* pChart);
	void ChangePolygonProperty(CMFCPropertyGridProperty* pProp, CEqmDrawPolygon* pChart);

	void	SetPlaneTempeDistrProperty(CEqmDrawPlaneTempeDistr* pReport);
	void	ChangePlaneTempeDistrProperty(CMFCPropertyGridProperty* pProp, CEqmDrawPlaneTempeDistr* pReport);

	void	SetDottedLineFrmProperty(CEqmDrawDottedLineFrm* pDotFrm);
	void	ChangeDottedLineFrmProperty(CMFCPropertyGridProperty* pProp, CEqmDrawDottedLineFrm* pDotFrm);

	void	SetGeneralComponentProperty(CEqmDrawGeneralComponent* pGenCom);
	void	ChangeGeneralComponentProperty(CMFCPropertyGridProperty* pProp, CEqmDrawGeneralComponent* pGenCom);

protected:
	vector<CString>  m_vecPipePoints;
	vector<HistoryTrendInfo> m_vecTrendInfo;
	vector<_tagAreaInfo> m_vecAreaInfo;
	vector<CurveColumn> m_vecScrawl;

	vector<TemperDistriInfo> m_vecTemper;
	vector<PageInfo> m_vecPage;
	vector<LabelInfo> m_vecLabelInfo;
	vector<SectionInfo> m_vecSectionInfo;

	CString getMerged(vector<CString> & vec,CString sepa);

	CString relation2Cstring(_tagRelationExpress rel);
	int		rel2Int(CString rel);
	int		angle2Index(int angle);

	CString	GetDiagGraphPlusBindPtString(const _tagBoolBindInfo& bindInfo);
	CString	GetDataAnalysePointSetString(const DaPointInfo& daPtInfo);

public:
	PosInfo m_areaPos;

private:
	int	m_nColorIndex;
};

