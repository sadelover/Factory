#pragma once
#include "afxwin.h"
// CCanvasDlg dialog
#include "Tools/CustomTools/CustomTools.h"
#include "afxtoolbarfontcombobox.h"
#include "../ComponentDraw/EqmDrawDefines.h"
#include "ScrawlSettingDlg.h"
#include "../ComponentDraw/EqmDrawHistoryTrend.h"
#include "../ComponentDraw/EqmDrawMapNavigate.h"
#include "../ComponentDraw/EqmDrawNavigateWindow.h"
#include "../ComponentDraw/EqmDrawLevelRuler.h"
#include "../ComponentDraw/EqmDrawDataAnalyse.h"
#include "../ComponentDraw/EqmDrawTemperDistri.h"
#include "StandardEquip.h"
#include "json/json.h"
#include "json/reader.h"

#define ID_TIMER_ANIMATION (WM_USER+0x1000)

class CMainFrame;
class CEqmDrawBase;
class CEqmDrawDevice;
class CEqmDrawButton;
class CEqmDrawAreaChange;
enum eButtonStyle;
class CEqmDrawButtonPlus;

#define ANIMATION_TIMER 50
#pragma warning(disable:4996)
class CCanvasDlg : public CDialog
{
	DECLARE_DYNAMIC(CCanvasDlg)
    DECLARE_MESSAGE_MAP()
public:
	CCanvasDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCanvasDlg();
// Dialog Data
	enum { IDD = IDD_CANVASDLG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    void ChangeCursorToChangeSize( CMainFrame * pMainFrame, CPoint &point );
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
    afx_msg void OnRmenuSeleteall();
    afx_msg void OnRmenuDelete();
    afx_msg void OnRmenuProperty();
    afx_msg void OnRmenuLeftrotate();
    afx_msg void OnRmenuRightrotate();
    afx_msg void OnUpdateRmenuLeftrotate(CCmdUI *pCmdUI);
    afx_msg void OnUpdateRmenuRightrotate(CCmdUI *pCmdUI);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnPosition();
	afx_msg void OnRmenuAddtoImglib();
	afx_msg void OnRmenuCopy();
	afx_msg void OnRmenuPaste();
	afx_msg void OnAddBindpointToVpointTable();
	afx_msg void OnRmenuCanclDraw();
public:
	void CreatePipe(CPoint point);
	CEqmDrawButton* CreateButton(CPoint &point, const int nIDPicComm, const int nIDPicDisable, const int nIDPicDown, const int nIDPicOver, const int nLinkPageID, const CString strBindSetPointName, const int nIDChecked);
	CEqmDrawButton* CreateButtonNewStyle(CPoint &point, const int nIDPicComm, const int nIDPicDisable, const int nIDPicDown, const int nIDPicOver, const int nLinkPageID, const CString strBindSetPointName, const int nIDChecked, Image* pBitmap);

	void ShowPropertyWnd(const CPoint& point);
	void ShowSelectElementByID(const int nElementID);

	bool CreateBendCtrl(const CPoint& point, const BendType& type, const int nId, const int nBendSize);

	void OnLButtonDownInMoveMode(UINT nFlags, CPoint point);
	void OnLButtonUpSelectBox(UINT nFlags, CPoint point);
	void OnLButtonDownInFormatBrush(UINT nFlags, CPoint point);

    void StartThreadDraw();
    void SuspendThreadDraw();
    void ResumeThreadDraw();

    void OnTimerDraw();

    static UINT WINAPI ThreadProcDraw(LPVOID param);
	static UINT WINAPI Fun1Proc(LPVOID lpParameter);

	void	OnKeySelectAll();
	void	OnKeyDelete();
	void	TerminateThread();

	LPPOINT lpoint;
	CPoint m_rightclicksavepoint;
	int m_base_height ;
	int m_base_width  ;
	double m_scale;
	bool m_bLButtonDown;
	Gdiplus::PointF m_pPre;			//修改前坐标
	Gdiplus::PointF m_pAct;			//修改后坐标
	bool    m_bStartDragging;		//开始拖动
	//////////////////////////////////////////////////////////////////////////
	vector<CEqmDrawBase *>		m_selectEuVect;			//ctrl左键选中的设备
    Mutex						m_mutex;
	HANDLE						m_hThreadDraw;
	bool						m_bExistThreadDraw;
	bool	m_trackleave;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void InsertImageIntoOpenS3db(const int nFindId, int& nUseId);
	void InsertAnimationIntoOpenS3db(const int nAnimId, int& nUseId);

	bool InsertImageIntoImageLib(const int nPicId);
	bool InsertAnimationIntoAnimationLib(const int nAnimId);

	void InsertImageIntoOpenS3dbFromFile(PictureInfo picInf, int& nUseId);


private:
	void	ShowPropertyDlg(const CPoint& point);
	
	void	ShowTextInfoDlg(void);
	void	ShowGraphicInfoDlg(void);
	void	ShowPipeInfoDlg(void);
	void	ShowMetaFileDlg();
	void	ShowAreaChangeDlg();
	void	ShowChartInfoDlg(CEqmDrawDevice* pDevice);

	void CreateRectangle( CPoint &point, vector<Gdiplus::PointF> &vecPt );
	void CreateEllipse( CPoint &point, vector<Gdiplus::PointF> &vecPt );
	
	bool CreateDevice(CPoint &point, const int nID, const bool bIsPic = true);
	bool CreateAreaChange(CPoint &point, const int nID);
	void CreateText( CPoint &point );
	void Create2DLine( CPoint point);
	bool CreateLineGraphic(CPoint &point);
	bool CreateBarGraphic(CPoint &point);
	bool CreatePieGraphic(CPoint &point);
	bool CreateDiagnoseGraphic(CPoint &point);
	bool CreateBend(CPoint &point, const int nBendSize);
	void CreateDashBoard(CPoint &point, const int nID);
	bool CreateDashBoardByType(CPoint &point, const int nDashBoardType);
	bool CreateTimeSelect(CPoint &point);
	bool CreateTimeBar(CPoint &point);
	void CreatePolygon(CPoint point);

	bool CreateProgressBar( CPoint point );
	bool CreateShellExecute( CPoint &point, const CString strServerIP, const CString strFileName, const CString strDownloadDirectory, const int nBKPicID, const bool bAuto, const int nLayer, const CString strFileID);
	bool CreateEmbededPage(CPoint &point, const int nPicID, const vector<PageIDName>& vec, const int nChangeInterval, const int nLayer);
	bool CCanvasDlg::CreateLiquidLevel(CPoint &point, const int nLayer, const int nPicID, const CString strBindPoint, const int nMax, const int nMin, const float fHighAlarmLine, const float fLowAlarmLine);
	CEqmDrawButtonPlus* CreateCheckButtonPlus(CPoint &point, const eButtonStyle eBtnStyle, const int nLayer, const COLORREF colorTxt, const int nTxtSize,
		const CString strSelVal, const CString strUnSelVal, const CString strBindPointName, const CString strTxtContent, const int nGroupId, const CString strExpression,
		const CString strSelCom, const CString strSelOver, const CString strUnSelCom, const CString strUnSelOver, const CString strOptionsDec);
	bool CreateDrawl(CPoint &point, const int nLayer, const int nPicID, const CString strShowWords, const vector<CurveColumn>& vecCurve, const vector<CurveColumn>& vecColumn);
	vector<CurveColumn> MergeScrawlVectors(const vector<CurveColumn>& vec1, const vector<CurveColumn>& vec2);
	bool CreateHistoryTrend(CPoint &point, const int nPicID, const int nLayer, const HistoryTrendType& type, const vector<HistoryTrendInfo>& vecHisTrendInf);
	bool CreateEnergySaveRoi(CPoint &point, const int nPicID);
	bool CreateMapNavigate(CPoint &point, const int nLayer,int nTemplateGroupId,  int nTemplatePageId, CString strTemplatePageName,CString cstrParamList, CString cstrTemplateName);
	
	bool CreateNavigateWindow(CPoint &point, const int nLayer, const vector<PageInfo>& vecPageInfo);
	bool CreateLevelRuler(CPoint &point, const int nLayer,
		const CString strName, const int nMax, const int nMin, const int nMainScaleInterval, const int nMinorScaleInterval, const int nDecimalPlace,
		const vector<LabelInfo>& vecLabelInf, const vector<SectionInfo>& vecSectionInf);
	bool CreateDataAnalyse(CPoint &point, const int nLayer, const vector<DaPointInfo>& vecDaPtInf, const CString strPtTime, const CString strPtCount, const CString strPtStatus);
	bool CreateTemperDistri(CPoint &point, const int nLayer, const vector<TemperDistriInfo>& vecInf);
	bool CreateDataReport(CPoint &point, const int nLayer, const CString strPath);
	bool CreatePlaneTempeDistr(CPoint &point, const int nLayer, const CString strPtName);
	bool CreateDottedLineFrm(CPoint &point, const int nLayer);
	bool CreateGeneralComponent(CPoint &point, const int nLayer, const CString strCfgVal);
	bool CreateStraightLine(CPoint &point, const int nPicID);
	void SetImageIntoImageLib(const CPoint& point);

	void SetMetaSelect(CEqmDrawBase* pEqm, CMainFrame* pMainFrm);

	void SetCursorRange(const CPoint& point);

	void SyncPlaneTemperDistri(const CString strPtList);

	CPoint GetClosestGridPosition(const CPoint point) const;

	CString	m_strSysPath;
	CPoint	m_ptPaste;
	
};