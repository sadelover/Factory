// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "../ComponentDraw/EqmDrawProject.h"
#include "../ComponentDraw/DrawProxy.h"
#include "../ComponentDraw/EqmDrawDefines.h"
#include "DataPoint/LogicDataPoint.h"
#include "DataPoint/AlarmDataPoint.h"
#include "DataPoint/LogicPointManager.h"
#include "DataPoint/AlarmPointManager.h"
#include "DataPointConfigDlg.h"
#include "ComboToolbar.h"
#include "../ControlEx/CustomMenu.h"
#include "../ControlEx/ToolBarEx.h"
#include "ComboToolbar.h"
#include <list>                              //add 9.21
#include "LayerManageDlg.h"
#include <set>
#include "../ComponentDraw/ProjectSqlite.h"
#include "LoadingProjectDlg.h"
#include "ProjectCheckDlg.h"
#include "PropertiesWnd.h"
#include "FileView.h"

typedef struct tagQueryVer
{
	CString strIp;
	int		nPort;
	CString	strDir;
	CString	strVer;

	tagQueryVer()
	{
		nPort = 5000;
	}
}QueryVersion;

class CImageReplaceDlg;
class CNoModelDlg;

class CMainFrame : public CFrameWndEx
{
    DECLARE_DYNCREATE(CMainFrame)
    DECLARE_MESSAGE_MAP()
protected: // create from serialization only
	CMainFrame();
// Attributes
public:
// Operations
public:
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CMFCStatusBar  m_wndStatusBar;
	CMFCToolBar	m_wndToolBar;
	CMFCToolBar	m_wndToolBarLayout;
	CImageList	m_ImageListToolbar;
	CImageList	m_ImageListToolbar_Hot;
	CImageList	m_ImageListToolbar_Disable;
	CProjectSqlite* m_pPrjSqlite;
	CMFCToolBarComboBoxButton  *m_comboButton;
	CFileView         m_wndFileView;
	CPropertiesWnd    m_wndProperties;
	CEqmDrawBase*	m_pFormatBrush;
	
	// Generated message map functions

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileCreateproject();
	afx_msg void OnFileOpenprojectSqlite();
	afx_msg void OnFileOpenprojectSqlite_OnlyPointTable();
	afx_msg void OnUpdateFileCreateproject(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpenprojectSqlite(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpenprojectSqlite_OnlyPointTable(CCmdUI* pCmdUI);
	afx_msg void OnSaveProject();
	afx_msg void OnUpdateSaveProject(CCmdUI *pCmdUI);
	afx_msg void OnCloseproject();
	afx_msg void OnUpdateCloseproject(CCmdUI *pCmdUI);
	afx_msg void OnPipe();
	afx_msg void OnLine();
	afx_msg void OnRectangle();
    afx_msg void OnMove();
    afx_msg void OnUpdateMove(CCmdUI *pCmdUI);
    afx_msg void OnLeftalign();
    afx_msg void OnUpdateLeftalign(CCmdUI *pCmdUI);
    afx_msg void OnRightalign();
    afx_msg void OnUpdateRightalign(CCmdUI *pCmdUI);
    afx_msg void OnTopalign();
    afx_msg void OnUpdateTopalign(CCmdUI *pCmdUI);
    afx_msg void OnDownalign();
    afx_msg void OnUpdateDownalign(CCmdUI *pCmdUI);
    afx_msg void OnHorizonalign();
    afx_msg void OnUpdateHorizonalign(CCmdUI *pCmdUI);
    afx_msg void OnVerticalalign();
    afx_msg void OnUpdateVerticalalign(CCmdUI *pCmdUI);
    afx_msg void OnMakeSameWidth();
    afx_msg void OnUpdateMakeSameWidth(CCmdUI *pCmdUI);
	afx_msg void OnMakeSameHeight();
	afx_msg void OnUpdateMakeSameHeight(CCmdUI *pCmdUI);
	afx_msg void OnMakesamesize();
	afx_msg void OnUpdateMakesamesize(CCmdUI *pCmdUI);
	afx_msg void OnMetaBrush();
	afx_msg void OnUpdateMetaBrush(CCmdUI *pCmdUI);
	afx_msg void OnGrids();
	afx_msg void OnUpdateGrids(CCmdUI *pCmdUI);
	afx_msg void OnMakeBend();
	afx_msg void OnUpdateMakeBend(CCmdUI *pCmdUI);
	afx_msg void OnShowPointValueDlg();
	afx_msg void OnUpdateShowPointValueDlg(CCmdUI *pCmdUI);
    afx_msg void OnRightrotate();
    afx_msg void OnUpdateRightrotate(CCmdUI *pCmdUI);
    afx_msg void OnLeftrotate();
    afx_msg void OnUpdateLeftrotate(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
    afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
    afx_msg void OnEditPaste();
    afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnFormatbrush();
    afx_msg void OnUpdateFormatbrush(CCmdUI *pCmdUI);
    afx_msg void OnOpcpoint();
    afx_msg void OnUpdateOpcpoint(CCmdUI *pCmdUI);
	afx_msg void OnShowToolBar();
	afx_msg void OnUpdateShowPropertyWnd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowToolBar(CCmdUI *pCmdUI);
	afx_msg void OnToolBarLayout();
	afx_msg void OnUpdateToolBarLayout(CCmdUI *pCmdUI);
    afx_msg void OnUpdatePipe(CCmdUI *pCmdUI);
    afx_msg void OnUpdateLine(CCmdUI *pCmdUI);
    afx_msg void OnUpdateRectangle(CCmdUI *pCmdUI);
    afx_msg void OnClose();
    afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
    afx_msg void OnAddDbTableField();
    afx_msg void OnUpdateAddDbTableField(CCmdUI *pCmdUI);
    afx_msg void OnComboSelChange();
    afx_msg void OnUpdateComboZoom(CCmdUI *pCmdUI);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);           //画菜单的
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);  //菜单大小
	afx_msg void OnText();
	afx_msg void OnUpdateText(CCmdUI *pCmdUI);
	afx_msg void OnChiller();
	afx_msg void OnUpdateChiller(CCmdUI *pCmdUI);
	afx_msg void OnPump();
	afx_msg void OnUpdatePump(CCmdUI *pCmdUI);
	afx_msg void OnValve();
	afx_msg void OnUpdateValve(CCmdUI *pCmdUI);
	afx_msg void OnCoolingTower();
	afx_msg void OnUpdateCoolingTower(CCmdUI *pCmdUI);
	afx_msg void OnMeta();
	afx_msg void OnUpdateMeta(CCmdUI *pCmdUI);
	afx_msg void OnMetaButton();
	afx_msg void OnUpdateMetaButton(CCmdUI *pCmdUI);
	afx_msg void OnBend();
	afx_msg void OnUpdateBend(CCmdUI *pCmdUI);
	afx_msg void OnLineChart();
	afx_msg void OnUpdateLineChart(CCmdUI *pCmdUI);
	afx_msg void OnBarChart();
	afx_msg void OnUpdateBarChart(CCmdUI *pCmdUI);
	afx_msg void OnPieChart();
	afx_msg void OnUpdatePieChart(CCmdUI *pCmdUI);
	afx_msg void OnDashBoard();
	afx_msg void OnUpdateDashBoard(CCmdUI *pCmdUI);
	afx_msg void OnTimeSelect();
	afx_msg void OnUpdateTimeSelect(CCmdUI *pCmdUI);
	afx_msg void OnTimeBar();
	afx_msg void OnUpdateTimeBar(CCmdUI *pCmdUI);
	afx_msg void OnProgressBar();
	afx_msg void OnUpdateProgressBar(CCmdUI *pCmdUI);
	afx_msg void OnDiagnoseChart();
	afx_msg void OnUpdateDiagnoseChart(CCmdUI *pCmdUI);
	afx_msg void OnBarPolygon();
	afx_msg void OnUpdateBarPolygon(CCmdUI *pCmdUI);
	afx_msg void OnAreaChange();
	afx_msg void OnUpdateAreaChange(CCmdUI *pCmdUI);
	afx_msg void OnShellExecute();
	afx_msg void OnUpdateShellExecute(CCmdUI *pCmdUI);
	afx_msg void OnEmbededPage();
	afx_msg void OnUpdateEmbededPage(CCmdUI *pCmdUI);
	afx_msg void OnLiquidLevel();
	afx_msg void OnUpdateLiquidLevel(CCmdUI *pCmdUI);
	afx_msg void OnCheckButtonPlus();
	afx_msg void OnUpdateCheckButtonPlus(CCmdUI *pCmdUI);
	afx_msg void OnNoticeBoard();
	afx_msg void OnUpdateNoticeBoard(CCmdUI *pCmdUI);
	afx_msg void OnHistoryTrendAnalysis();
	afx_msg void OnUpdateHistoryTrendAnalysis(CCmdUI *pCmdUI);
	afx_msg void OnEnergySaveRoiAnalysis();
	afx_msg void OnUpdateEnergySaveRoiAnalysis(CCmdUI *pCmdUI);
	afx_msg void OnMapNavigate();
	afx_msg void OnUpdateMapNavigate(CCmdUI *pCmdUI);
	afx_msg void OnNavigateWindow();
	afx_msg void OnUpdateNavigateWindow(CCmdUI *pCmdUI);
	afx_msg void OnLevelRuler();
	afx_msg void OnUpdateLevelRuler(CCmdUI *pCmdUI);
	afx_msg void OnDataAnalyse();
	afx_msg void OnUpdateDataAnalyse(CCmdUI *pCmdUI);
	afx_msg void OnTemperDistri();
	afx_msg void OnUpdateTemperDistri(CCmdUI *pCmdUI);
	afx_msg void OnDataReport();
	afx_msg void OnUpdateDataReport(CCmdUI *pCmdUI);
	afx_msg void OnPlaneTempeDistr();
	afx_msg void OnUpdatePlaneTempeDistr(CCmdUI *pCmdUI);
	afx_msg void OnDottedLineFrame();
	afx_msg void OnUpdateDottedLineFrame(CCmdUI *pCmdUI);
	afx_msg void OnGeneralComponent();
	afx_msg void OnUpdateGeneralComponent(CCmdUI *pCmdUI);

	afx_msg void OnUpdateComboBo(CCmdUI *pCmdUI);

	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);

private:
    void				ResetMarks();
    CEqmDrawProject &	GetProject();
public:
    void				LoadProjectFile(const std::wstring& filePathName, bool bClear = false);
	void				LoadNewProjectFile(const std::wstring& filePathName, bool bClear = false);
	void                StartLoadThread();
    bool				SaveProjectFile();
    void				SetComboSel(int cursel);
    int					GetComboSel();
    int					GetComboSelSize();
	DWORD				GetSelfProcessId(CString strExecuteName);  
	bool				CloseProjectBeforeOpen();
	CDataPointManager&	GetPointMap();
	CDataPointManager&	GetOnlinePointMap();
	void				ShowCurrentPage();
	COLORREF			m_colorArray[8];
	static UINT WINAPI ThreadFunc_LoadProject(LPVOID lparam);
	void				SetSaveFlag(const bool bNeedSave);
	bool				GetSaveFlag(void);
	void				ShowSelectElementByID(const int nElementID);
	void				SetPointTableFlag(const bool bNeedSave);
	bool				GetPointTableFlag(void) const;
	bool				CheckFileName(const CString strPath);
	BOOL				CheckDatabaseIntegrity(const CString strPathName, const int nType);
	void				SaveProjectContain(void);
	void				SaveProjectOperate(void);
	CString				GetAppName(){ return m_AppName;};

public:
	HANDLE                      m_threadhandle_LoadProject;

	static HCURSOR				m_hCursor;
	CEqmDrawProject				m_project;
	DrawContext					m_DrawContext;
	DrawProxy					m_DrawProxy;
	bool						m_bIsInitGDI;
	bool						m_bIsCreateProject;    //是否创建项目
	int							m_MaxEqmtID;
	CDataPointManager			m_plcpointmap;
	CLogicPointManager			m_logicPotManager;
	CAlarmPointManager			m_AlarmPotManager;
    CFont*						m_comboFont;
	CString						m_projectName;
	int							m_dlgstate;
	int							m_MaxPageID;
	bool						m_bLoadingPage;
	CLoadingProjectDlg          m_LoadingProjectBar;
	bool						m_bUpdateScreen;
	bool						m_bLoadIndependent;
	//在线调试资源
	CDataPointManager			m_Onlineplcpointmap;
	CLogicPointManager			m_OnlinelogicPotManager;
	CAlarmPointManager			m_OnlineAlarmPotManager;
	CProjectSqlite*				m_pOnlinePrjSqlite;
	CEqmDrawProject				m_Onlineproject;
	CProjectCheckDlg			m_pProjectCheckDlg;
	bool						m_bFirstInitDlg;
	bool						m_bCheckImageMode;
	int							m_nElementID;
	int							m_nSelectPicID;
	int							m_nItemNameLen;
	CImageReplaceDlg*			m_pImgReplageDlg;	// 图元替换信息页面

	// 复制粘贴
	int							m_nPasteXCount;
	int							m_nPasteYCount;
	int							m_nPasteOffset;
	int							m_nPageLuX;
	int							m_nPageLuY;
	int							m_nPageRdX;
	int							m_nPageRdY;
	int							m_nSelLuX;
	int							m_nSelLuY;
	int							m_nSelRdX;
	int							m_nSelRdY;
	bool						m_bFirstPaste;
	DWORD						m_dwSyncIp;
	int							m_nSyncPort;
	CString						m_strSyncUsername;
	CString						m_strSyncPassword;
	CString						m_strSyncDbName;
	int							m_nBendSize;
	CString						m_strCalcIp;
	int							m_nCalcPort;

	CString						m_strCloudServiceIp;
	int							m_nCloudServicePort;

	CString						m_strCalcPostName;
	bool						m_bSyncDec;
	CString                     m_strMySQLDatabaseName;

private:
	bool                                m_bOnlyOpenPointTable;
	BOOL								m_ProjectOpen;
	CString								m_AppName;	
	UINT								m_uiMenuID;
	bool								m_bIsNeedFlashMenu;
	HBRUSH								m_hBrush;
	CMenu								m_topMenu;        //自绘菜单栏
	DWORD								m_processId;
	std::wstring						m_str_proId;
	CLayerManageDlg*					m_pLayerManageDlg;
	Gdiplus::GdiplusStartupInput		gdiplusStartupInput;
	unsigned long 						pGdiToken;
	CString								m_strFilePath;
	bool								m_bNeedSave;
	bool								m_bPtTableNeedSave;

public:

	CString GetConfigVersion();
	bool SaveConfig(DataBaseSetting &dbset);
	DataBaseSetting LoadConfig();


	//用于存放连接服务器的db文件信息
	DataBaseSetting m_dbset;
	wstring m_strDBFileName;


public:
	afx_msg void OnCircle();
	afx_msg void OnUpdateCircle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZoomout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZoomin(CCmdUI *pCmdUI);

public:
	void					SaveDeviceProperty();
	BOOL					GetLoadIfFail();
	bool					IsFullScreen() const;
	void					SetFullScreen(bool bfullscreen);
	afx_msg void			OnFullscreen();
	void					EndFullScreen();
	CString					GetDBFilePath();
	int						SetOnePage();
	void					InitOnlinePrjSqlite(void);
	void					LoadUserToolImage();
	void					LoadUserToolImage(CString strPath);
	void					ReplaceUserToolImage();
	void					ReplaceUserToolImage(UINT uiCmd, int nImageIndex, CString strTip);
	void					MetaCopy(void);
	void					MetaPaste(void);
	void					OpenProjectContain(const CString strPathName);

private:
	vector<CEqmDrawBase *>				m_selectEuVect;        //ctrl左键选中的设备
	bool								IfLoadFail;
	vector<Gdiplus::PointF>				m_vecPointList; 
	CDataPointConfigDlg*				m_pPointMapDlg;
	bool								m_bFullScreen;
	WINDOWPLACEMENT						m_savewindowplacement;
	CRect								m_fullscreen_rect;
	vector<BendInfo>					m_vecBendInfo;

private:
	void	SetPageListInfo(void);

	void	GetVersionFromHttp(const CString strCfgPathName);
	static UINT WINAPI ThreadFuncFindVersion(LPVOID lparam);
	QueryVersion	m_stQueryVer;

	void	DeleteAreaItem(const int nIndex);
	void	DeleteDeviceItem(const int nIndex);

	BOOL	SetRecentOpenFileList(const CString strPathName);
	void	SyncDecFile(void);

private:
	// ftp function
	static UINT WINAPI ThreadFuncSyncVersion(LPVOID lparam);
	BOOL SyncVersionByFtp(void);
	BOOL CreateSpecDir(const CString strPath);
	BOOL CopySpecDir(CString strSrc, CString strDst);
	void DeleteDirContain(CString strDir);
	BOOL GetFtpFileEx(CString strRemoteFile, CString strLocalFile, CString strIp, CString strUser, CString strPwd, int nPort);
	BOOL ReadFtpFile(CString strRemoteFile, CString strIp, CString strUser, CString strPwd, int nPort, CString& strVersion);
	BOOL	m_bFtpEnable;
	CString	m_strFtpIp;
	CString	m_strFtpUser;
	CString	m_strFtpPwd;
	int		m_nFtpPort;
	CNoModelDlg* m_pNoModelDlg;

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNewdevice();
	afx_msg void OnLayerManage();
	afx_msg void OnShowPropertyWnd();
	afx_msg void OnShowPageWnd();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	afx_msg void OnDestroy();
	afx_msg void OnDownLoadFile();
	afx_msg void OnUpdateDownLoadFile(CCmdUI *pCmdUI);
	afx_msg void OnPagePercent();
	afx_msg void OnUpdatePagePercent(CCmdUI *pCmdUI);
	afx_msg void OnJiami();
	afx_msg void OnJiemi();
	afx_msg void OnWebEncrypt();
	afx_msg void OnWebDescrypt();
	afx_msg void OnPageBatchGen();
	afx_msg void OnPageBatchReplacepoint();
	afx_msg void OnPageBatchReplaceLink();
	afx_msg void OnUpdatePageBatchGen(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePageBatchReplacepoint(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePageBatchReplaceLink(CCmdUI *pCmdUI);
	afx_msg void OnImageMgr();
	afx_msg void OnUpdateImageMgr(CCmdUI *pCmdUI);
	afx_msg void OnImageCheck();
	afx_msg void OnUpdateImageCheck(CCmdUI *pCmdUI);
	afx_msg void OnS3dbCheck();
	afx_msg void OnUpdateS3dbCheck(CCmdUI *pCmdUI);
	afx_msg void OnComboBox();
	afx_msg void ClickComboBox();
	bool GetFactorySoftVersion(int &nVersionPrefix1,int &nVersionPrefix2,int &nVersionPrefix3,int &nVersionPrefix4);;
	void GetVersionPrefix(CString strVersion, int &nPrefix1, int &nPrefix2, int &nPrefix3, int &nPrefix4);

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	afx_msg void OnSoftPreference();
	afx_msg void OnOpenImageMgr();
	afx_msg void OnUpdateOpenImageMgr(CCmdUI *pCmdUI);
	afx_msg void OnOpenImageReplaceInfo();
	afx_msg void OnUpdateOpenImageReplaceInfo(CCmdUI *pCmdUI);
	afx_msg void OnImgLean();
	afx_msg void OnUpdateImgLean(CCmdUI *pCmdUI);
	afx_msg void OnImgLeanCore();
	afx_msg void OnUpdateImgLeanCore(CCmdUI *pCmdUI);
	afx_msg void OnDevAreaSync();
	afx_msg void OnUpdateDevAreaSync(CCmdUI *pCmdUI);
	afx_msg void OnPageDevAreaSync();
	afx_msg void OnUpdatePageDevAreaSync(CCmdUI *pCmdUI);
	afx_msg void OnVersionHistory();
	afx_msg void OnUpdateVersionHistory(CCmdUI *pCmdUI);
	afx_msg void OnVpointBackup();
	afx_msg void OnVpointReload();
	afx_msg void OnUpdateVpointBackup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVpointReload(CCmdUI *pCmdUI);
	afx_msg void OnRestartDomcore();
	afx_msg void OnStopDomcore();
	afx_msg void OnRestartDomlogic();
	afx_msg void OnStopDomlogic();
	afx_msg void OnRestartDomall();
	afx_msg void OnStopDomall();
	afx_msg void OnUpdateRestartDomcore(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStopDomcore(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRestartDomlogic(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStopDomlogic(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRestartDomall(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStopDomall(CCmdUI *pCmdUI);
	afx_msg void OnSearch();
	afx_msg void OnUpdateSearch(CCmdUI *pCmdUI);

	BOOL KillProcessByName(const TCHAR *lpszProcessName) ;
	BOOL KillProcess(DWORD dwPid);
	afx_msg void OnCoreStatus();
	afx_msg void OnUpdateCoreStatus(CCmdUI *pCmdUI);
	afx_msg void OnLoginCore();
	afx_msg void OnUpdateLoginCore(CCmdUI *pCmdUI);

	bool AnalysisResponse(CString strResponse);
	afx_msg void OnModeSite();
	afx_msg void OnModeSumulate();
	afx_msg void OnUpdateModeSumulate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeSite(CCmdUI *pCmdUI);
	afx_msg void OnOprecordHistory();
	afx_msg void OnUpdateOprecordHistory(CCmdUI *pCmdUI);
	afx_msg void OnSampleManager();
	afx_msg void OnUpdateSampleManager(CCmdUI *pCmdUI);

	afx_msg void OnImportHistory();
	afx_msg void OnUpdateImportHistory(CCmdUI *pCmdUI);
	afx_msg void OnHistoryExport();
	afx_msg void OnUpdateHistoryExport(CCmdUI *pCmdUI);
	afx_msg void OnChangePointNameForHistoricalData();
	afx_msg void OnUpdateChangePointNameForHistoricalData(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWarningConfigRestore(CCmdUI *pCmdUI);
	afx_msg void OnWarningConfigRestore();
	afx_msg void OnWarningConfigBackup();

	wstring GetDatabaseNameFromPysite(CString strIP);
	CString GetMySQLDatabaseName();

	afx_msg void On4dbUploadToCloud();
	afx_msg void On4dbUpdateFromCloud();
	afx_msg void OnUpdate4dbUploadToCloud(CCmdUI *pCmdUI);
	afx_msg void OnViewPointPane();
	afx_msg void OnUpdateViewPointPane(CCmdUI *pCmdUI);
	afx_msg void OnGenStandardPoints();
	afx_msg void OnUpdateGenStandardPoints(CCmdUI *pCmdUI);
	afx_msg void OnHisdataManager();
	afx_msg void OnUpdateHisdataManager(CCmdUI *pCmdUI);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	afx_msg void OnBatchModifyPipeWidth();
	afx_msg void OnUpdateBatchModifyPipeWidth(CCmdUI *pCmdUI);
	afx_msg void OnCheckMissImage();
	afx_msg void OnUpdateCheckMissImage(CCmdUI *pCmdUI);
	afx_msg void OnCloudUpdateSyslogic();
	afx_msg void OnUpdateCloudUpdateSyslogic(CCmdUI *pCmdUI);
	afx_msg void OnViewCoreVersions();
	afx_msg void OnUpdateViewCoreVersions(CCmdUI *pCmdUI);
	afx_msg void OnAddVpointBatch();
	afx_msg void OnCompletionData();
	afx_msg void OnUpdateCompletionData(CCmdUI *pCmdUI);
	afx_msg void OnExportHistoricalDataAtPreciseTime();
	afx_msg void OnUpdateExportHistoricalDataAtPreciseTime(CCmdUI *pCmdUI);

	afx_msg void OnDatabaseRestore();
	afx_msg void OnUpdateDatabaseRestore(CCmdUI *pCmdUI);

	afx_msg void OnDatabaseBuckupManager();
	afx_msg void OnUpdateDatabaseBuckupManager(CCmdUI *pCmdUI);
};
UINT Thread_DatabaseRestore(LPVOID pParam);