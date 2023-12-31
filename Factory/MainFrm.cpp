
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "BEOP-Designer.h"
#include "MainFrm.h"
#include "BEOP-DesignerView.h"
#include "PageDlg.h"
#include "AddPageDlg.h"
#include "NewProjectDlg.h"
#include "DlgShowDupOpc.h"
#include <locale.h>
#include <sstream>

#include "../ComponentDraw/EqmDrawProject.h"
#include "../ComponentDraw/EqmDrawDevice.h"
#include "../ComponentDraw/EqmDrawPipe.h"
#include "../ComponentDraw/EqmDrawGraphic.h"
#include "../ComponentDraw/EqmDrawText.h"
#include "../ComponentDraw/EqmDrawGraphicRect.h"
#include "../ComponentDraw/EqmDrawGraphicLine.h"
#include "../ComponentDraw/EqmDrawGraphicEllipse.h"
#include "../ComponentDraw/EqmDrawGraphicPolygon.h"
#include "../ComponentDraw/EqmDrawDashBoard.h"
#include "../ComponentDraw/EqmDrawBend.h"
#include "../ComponentDraw/EqmDrawButton.h"
#include "../ComponentDraw/EqmDrawShellExecute.h"
#include "../ComponentDraw/EqmDrawEmbededPage.h"
#include "../ComponentDraw/EqmDrawLiquidLevel.h"
#include "../ComponentDraw/ProjectSqlite.h"

#include "../ComponentDraw/EqmDrawLineGraphic.h"
#include "../ComponentDraw/EqmDrawBarGraphic.h"
#include "../ComponentDraw/EqmDrawPieGraphic.h"
#include "../ComponentDraw/EqmDrawTimeSelect.h"
#include "../ComponentDraw/EqmDrawTimeBar.h"
#include "../ComponentDraw/EqmDrawProgressBar.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphic.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphicPlus.h"
#include "../ComponentDraw/EqmDrawAreaChange.h"
#include "../ComponentDraw/EqmDrawButtonPlus.h"
#include "../ComponentDraw/EqmDrawScrawl.h"
#include "../ComponentDraw/EqmDrawEnergySaveRoi.h"
#include "../ComponentDraw/EqmDrawPipe.h"
#include "../ComponentDraw/EqmDrawPolygon.h"
#include "InputPipeWidthDlg.h"


#include "Tools/ColorDefines.h"
#include "Tools/CustomTools/CustomTools.h"
#include "Tools/Util/UtilsFile.h"
#include "Tools/Util/UtilString.h"
#include "Tools/Util/Crc32.h"
#include <Windows.h>
#include "PIFManageDlg.h"
#include "../ComponentDraw/UtilDrawNumber.h"
#include "MetaFileSelectDlg.h"
#include "../ComponentDraw/sqlite/SqliteAcess.h"
#include "TreeManageDlg.h"
#include "DlgSetting.h"
#include "BEOPGatewayDlg.h"
#include "../ControlEx/WarningDlg.h"
#include "dlgload.h"
#include "PageReplacePointName.h"
#include "PageReplaceLinkName.h"
#include <algorithm>
#include "HttpOperation.h"
#include "ImageCheckDlg.h"
#include "DownLoadFileDlg.h"
#include "SoftPreferenceDlg.h"
#include "FileDialogEx.h"

#include "CommandManager.h"
#include "ImageReplaceDlg.h"
#include "Connect.h"
#include "BendConfigDlg.h"
#include "CheckInfoDlg.h"
#include "NoModelDlg.h"
#include "SaveVersionDlg.h"
#include "VersionHistoryDlg.h"
#include "json/reader.h"
#include  "OpRecordHistoryDlg.h"
#include "DataSampleManageDlg.h"
#include "ImportHistoryDataDlg.h"
#include "CoreStatusDlg.h"
#include "SearchLogicOutputDlg.h"
#include "HistoryDataDownloadDlg.h"
#include "GenStandardPointNameDlg.h"
#include "PointHisdataDlg.h"
#include "CoreVersionDlg.h"
#include "AddVPointBatchDialog.h"
//add Mark
#include "../Tools/CustomTools/CustomTools.h"
#include "CompletionData.h"
#include <direct.h>
#include "json/json.h"
#include "PointDialog.h"
#include "FileView.h"
#include "ConnectDlg.h"
#include "Tools/DirectoryOperations.h"
#include "ChangePointNameForHistoricalDataDlg.h"
#include "Tools/DirectoryOperations.h"
#include "DatabaseBackupManagerDlg.h"

#pragma comment(lib, "version.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC
#endif                            //输出内存泄露


using std::ostringstream;

HCURSOR CMainFrame::m_hCursor = NULL;
#define TOOLBARIMAGESIZE	16  //工具狂的图标的长度，正方形.

#define MENUSTARTID 40000
#define MAXPAGECOUNT 100

const wstring c_wstr_default_file_sufix = _T("s3db");
const wstring c_wstr_4db_file_sufix = _T("4db");


const UINT c_timer_interval_auto_save_ms = 2*60* 1000;  //2分钟自动保存一次。

enum SCREENRESOLUTION{
	SR_1024X768,
	SR_1280X1024,
	SR_1366X768,
	SR_1600X900,
	SR_1600X1200,
	SR_1920X1080,
	SR_1920X1200,
	SR_COUNT
};

const CPoint srPoint[SR_COUNT] = {
	CPoint(1024, 700),
	CPoint(1280, 954),
	CPoint(1366, 700),
	CPoint(1600, 840),
	CPoint(1600, 1140),
	CPoint(1920, 955),
	CPoint(1920, 1140)
};

enum ZOMMFACTOR{
	ZF_50PERCENT,
	ZF_60PERCENT,
	ZF_70PERCENT,
	ZF_80PERCENT,
	ZF_90PERCENT,
	ZF_100PERCENT,
	ZF_110PERCENT,
	ZF_120PERCENT,
	ZF_130PERCENT,
	ZF_140PERCENT,
	ZF_150PERCENT,
	ZF_COUNT
};


const double fZoomFactor[ZF_COUNT] = {
	0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5
};

bool OrderBy(const GroupPage& m1, const GroupPage& m2)
{
	return m1.nGroupShowOrder < m2.nGroupShowOrder;
}


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
    ON_WM_CREATE()
    ON_WM_CLOSE()
    ON_WM_DROPFILES()

	ON_COMMAND(ID_FILE_CREATEPROJECT, &CMainFrame::OnFileCreateproject)
	ON_COMMAND(ID_FILE_OPENPROJECT_XML, &CMainFrame::OnFileOpenprojectSqlite)
	ON_COMMAND(ID_FILE_OPEN_POINTTABLE, &CMainFrame::OnFileOpenprojectSqlite_OnlyPointTable)
	ON_UPDATE_COMMAND_UI(ID_FILE_CREATEPROJECT, &CMainFrame::OnUpdateFileCreateproject)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENPROJECT_XML, &CMainFrame::OnUpdateFileOpenprojectSqlite)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_POINTTABLE, &CMainFrame::OnUpdateFileOpenprojectSqlite_OnlyPointTable)
	
	ON_COMMAND(ID_SAVE_PROJECT, &CMainFrame::OnSaveProject)
	ON_UPDATE_COMMAND_UI(ID_SAVE_PROJECT, &CMainFrame::OnUpdateSaveProject)

	ON_COMMAND(ID_CloseProject, &CMainFrame::OnCloseproject)
	ON_UPDATE_COMMAND_UI(ID_CloseProject, &CMainFrame::OnUpdateCloseproject)

	ON_COMMAND(ID_PIPE, &CMainFrame::OnPipe)
	//ON_COMMAND(ID_LINE, &CMainFrame::OnLine)
	//ON_COMMAND(ID_RECTANGLE, &CMainFrame::OnRectangle)
	ON_COMMAND(ID_MOVE, &CMainFrame::OnMove)

	ON_UPDATE_COMMAND_UI(ID_MOVE, &CMainFrame::OnUpdateMove)
	ON_COMMAND(ID_LEFTALIGN, &CMainFrame::OnLeftalign)
	ON_UPDATE_COMMAND_UI(ID_LEFTALIGN, &CMainFrame::OnUpdateLeftalign)
	ON_COMMAND(ID_RIGHTALIGN, &CMainFrame::OnRightalign)
	ON_UPDATE_COMMAND_UI(ID_RIGHTALIGN, &CMainFrame::OnUpdateRightalign)
	ON_COMMAND(ID_TOPALIGN, &CMainFrame::OnTopalign)
	ON_UPDATE_COMMAND_UI(ID_TOPALIGN, &CMainFrame::OnUpdateTopalign)
	ON_COMMAND(ID_DOWNALIGN, &CMainFrame::OnDownalign)
	ON_UPDATE_COMMAND_UI(ID_DOWNALIGN, &CMainFrame::OnUpdateDownalign)
	ON_COMMAND(ID_HORIZONALIGN, &CMainFrame::OnHorizonalign)
	ON_UPDATE_COMMAND_UI(ID_HORIZONALIGN, &CMainFrame::OnUpdateHorizonalign)
	ON_COMMAND(ID_VERTICALALIGN, &CMainFrame::OnVerticalalign)
	ON_UPDATE_COMMAND_UI(ID_VERTICALALIGN, &CMainFrame::OnUpdateVerticalalign)
	ON_COMMAND(ID_MAKESAMEWIDTH, &CMainFrame::OnMakeSameWidth)
	ON_UPDATE_COMMAND_UI(ID_MAKESAMEWIDTH, &CMainFrame::OnUpdateMakeSameWidth)
	ON_COMMAND(ID_MAKESAMEHEIGHT, &CMainFrame::OnMakeSameHeight)
	ON_UPDATE_COMMAND_UI(ID_MAKESAMEHEIGHT, &CMainFrame::OnUpdateMakeSameHeight)
	ON_COMMAND(ID_MAKESAMESIZE, &CMainFrame::OnMakesamesize)
	ON_UPDATE_COMMAND_UI(ID_MAKESAMESIZE, &CMainFrame::OnUpdateMakesamesize)
	ON_COMMAND(ID_META_BRUSH, &CMainFrame::OnMetaBrush)
	ON_UPDATE_COMMAND_UI(ID_META_BRUSH, &CMainFrame::OnUpdateMetaBrush)
	ON_COMMAND(ID_GRIDS, &CMainFrame::OnGrids)
	ON_UPDATE_COMMAND_UI(ID_GRIDS, &CMainFrame::OnUpdateGrids)
	ON_COMMAND(ID_MAKE_BEND, &CMainFrame::OnMakeBend)
	ON_UPDATE_COMMAND_UI(ID_MAKE_BEND, &CMainFrame::OnUpdateMakeBend)
	ON_COMMAND(ID_DLG_POINTVALUE, &CMainFrame::OnShowPointValueDlg)
	ON_UPDATE_COMMAND_UI(ID_DLG_POINTVALUE, &CMainFrame::OnUpdateShowPointValueDlg)

	ON_COMMAND(ID_RIGHTROTATE, &CMainFrame::OnRightrotate)
	ON_UPDATE_COMMAND_UI(ID_RIGHTROTATE, &CMainFrame::OnUpdateRightrotate)
	ON_COMMAND(ID_LEFTROTATE, &CMainFrame::OnLeftrotate)
	ON_UPDATE_COMMAND_UI(ID_LEFTROTATE, &CMainFrame::OnUpdateLeftrotate)

	ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CMainFrame::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMainFrame::OnUpdateEditPaste)
	ON_COMMAND(ID_FORMATBRUSH, &CMainFrame::OnFormatbrush)
	ON_UPDATE_COMMAND_UI(ID_FORMATBRUSH, &CMainFrame::OnUpdateFormatbrush)

	ON_COMMAND(ID_OPCPOINT, &CMainFrame::OnOpcpoint)
	ON_UPDATE_COMMAND_UI(ID_OPCPOINT, &CMainFrame::OnUpdateOpcpoint)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTY_WND, &CMainFrame::OnUpdateShowPropertyWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, &CMainFrame::OnUpdateShowToolBar)
	ON_UPDATE_COMMAND_UI(ID_PIPE, &CMainFrame::OnUpdatePipe)
	//ON_UPDATE_COMMAND_UI(ID_LINE, &CMainFrame::OnUpdateLine)
	//ON_UPDATE_COMMAND_UI(ID_RECTANGLE, &CMainFrame::OnUpdateRectangle)
	// add for tooltip, by david.
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)


	ON_COMMAND(ID_ADD_DB_TABLE_FIELD, &CMainFrame::OnAddDbTableField)
	ON_UPDATE_COMMAND_UI(ID_ADD_DB_TABLE_FIELD, &CMainFrame::OnUpdateAddDbTableField)
	ON_CBN_SELCHANGE(ID_COMBO_ZOOM, OnComboSelChange )
	ON_UPDATE_COMMAND_UI(ID_COMBO_ZOOM, &CMainFrame::OnUpdateComboZoom)
	//ON_COMMAND(ID_CIRCLE, &CMainFrame::OnCircle)
	//ON_UPDATE_COMMAND_UI(ID_CIRCLE, &CMainFrame::OnUpdateCircle)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_UPDATE_COMMAND_UI(ID_ZOOMOUT, &CMainFrame::OnUpdateZoomout)
	ON_UPDATE_COMMAND_UI(ID_ZOOMIN, &CMainFrame::OnUpdateZoomin)
	ON_COMMAND(ID_FULLSCREEN, &CMainFrame::OnFullscreen)
	ON_WM_GETMINMAXINFO()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_LAYER_MANAGE, &CMainFrame::OnLayerManage)
	ON_COMMAND(ID_VIEW_PROPERTY_WND,&CMainFrame::OnShowPropertyWnd)
	ON_COMMAND(ID_VIEW_PAGE,&CMainFrame::OnShowPageWnd)
	ON_COMMAND(ID_VIEW_TOOLBAR,&CMainFrame::OnShowToolBar)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()

	ON_COMMAND(ID_VIEW_TOOLBAR_LAYOUT, &CMainFrame::OnToolBarLayout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_LAYOUT, &CMainFrame::OnUpdateToolBarLayout)

	ON_COMMAND(ID_TEXT, &CMainFrame::OnText)
	ON_UPDATE_COMMAND_UI(ID_TEXT, &CMainFrame::OnUpdateText)

	//ON_COMMAND(ID_CHILLER, &CMainFrame::OnChiller)
	//ON_UPDATE_COMMAND_UI(ID_CHILLER, &CMainFrame::OnUpdateChiller)

	//ON_COMMAND(ID_PUMP, &CMainFrame::OnPump)
	//ON_UPDATE_COMMAND_UI(ID_PUMP, &CMainFrame::OnUpdatePump)

	//ON_COMMAND(ID_VALVE, &CMainFrame::OnValve)
	//ON_UPDATE_COMMAND_UI(ID_VALVE, &CMainFrame::OnUpdateValve)
	
	//ON_COMMAND(ID_COOLING_TOWER, &CMainFrame::OnCoolingTower)
	//ON_UPDATE_COMMAND_UI(ID_COOLING_TOWER, &CMainFrame::OnUpdateCoolingTower)

	ON_COMMAND(ID_META, &CMainFrame::OnMeta)
	ON_UPDATE_COMMAND_UI(ID_META, &CMainFrame::OnUpdateMeta)

	ON_COMMAND(ID_META_BUTTON, &CMainFrame::OnMetaButton)
	ON_UPDATE_COMMAND_UI(ID_META_BUTTON, &CMainFrame::OnUpdateMetaButton)

	ON_COMMAND(ID_BEND, &CMainFrame::OnBend)
	ON_UPDATE_COMMAND_UI(ID_BEND, &CMainFrame::OnUpdateBend)

	ON_COMMAND(ID_LINE_CHART, &CMainFrame::OnLineChart)
	ON_UPDATE_COMMAND_UI(ID_LINE_CHART, &CMainFrame::OnUpdateLineChart)

	ON_COMMAND(ID_BAR_CHART, &CMainFrame::OnBarChart)
	ON_UPDATE_COMMAND_UI(ID_BAR_CHART, &CMainFrame::OnUpdateBarChart)

	ON_COMMAND(ID_PIE_CHART, &CMainFrame::OnPieChart)
	ON_UPDATE_COMMAND_UI(ID_PIE_CHART, &CMainFrame::OnUpdatePieChart)

	ON_COMMAND(ID_DASHBOARD, &CMainFrame::OnDashBoard)
	ON_UPDATE_COMMAND_UI(ID_DASHBOARD, &CMainFrame::OnUpdateDashBoard)


	ON_COMMAND(ID_DOWNLOAD_FILE, &CMainFrame::OnDownLoadFile)
	ON_UPDATE_COMMAND_UI(ID_DOWNLOAD_FILE, &CMainFrame::OnUpdateDownLoadFile)

	ON_COMMAND(ID_PERCENT, &CMainFrame::OnPagePercent)
	ON_UPDATE_COMMAND_UI(ID_PERCENT, &CMainFrame::OnUpdatePagePercent)

	ON_COMMAND(ID_TIME_SELECT, &CMainFrame::OnTimeSelect)
	ON_UPDATE_COMMAND_UI(ID_TIME_SELECT, &CMainFrame::OnUpdateTimeSelect)

	ON_COMMAND(ID_TIME_BAR, &CMainFrame::OnTimeBar)
	ON_UPDATE_COMMAND_UI(ID_TIME_BAR, &CMainFrame::OnUpdateTimeBar)

	ON_COMMAND(ID_PROGRESS_BAR, &CMainFrame::OnProgressBar)
	ON_UPDATE_COMMAND_UI(ID_PROGRESS_BAR, &CMainFrame::OnUpdateProgressBar)

	ON_COMMAND(ID_DIAGNOSE, &CMainFrame::OnDiagnoseChart)
	ON_UPDATE_COMMAND_UI(ID_DIAGNOSE, &CMainFrame::OnUpdateDiagnoseChart)

	ON_COMMAND(ID_JIAMI, &CMainFrame::OnJiami)
	ON_COMMAND(ID_JIEMI, &CMainFrame::OnJiemi)

	ON_COMMAND(ID_WEB_ENCRYPT, &CMainFrame::OnWebEncrypt)
	ON_COMMAND(ID_WEB_DESCRYPT, &CMainFrame::OnWebDescrypt)

	ON_COMMAND(ID_BAR_POLYGON, &CMainFrame::OnBarPolygon)
	ON_UPDATE_COMMAND_UI(ID_BAR_POLYGON, &CMainFrame::OnUpdateBarPolygon)

	ON_COMMAND(ID_AREA_CHANGE, &CMainFrame::OnAreaChange)
	ON_UPDATE_COMMAND_UI(ID_AREA_CHANGE, &CMainFrame::OnUpdateAreaChange)

	ON_COMMAND(ID_SHELL_EXECUTE, &CMainFrame::OnShellExecute)
	ON_UPDATE_COMMAND_UI(ID_SHELL_EXECUTE, &CMainFrame::OnUpdateShellExecute)

	ON_COMMAND(ID_EMBEDED_PAGE, &CMainFrame::OnEmbededPage)
	ON_UPDATE_COMMAND_UI(ID_EMBEDED_PAGE, &CMainFrame::OnUpdateEmbededPage)

	ON_COMMAND(ID_LIQUID_LEVEL, &CMainFrame::OnLiquidLevel)
	ON_UPDATE_COMMAND_UI(ID_LIQUID_LEVEL, &CMainFrame::OnUpdateLiquidLevel)

	ON_COMMAND(ID_CHKBTN_PLUS, &CMainFrame::OnCheckButtonPlus)
	ON_UPDATE_COMMAND_UI(ID_CHKBTN_PLUS, &CMainFrame::OnUpdateCheckButtonPlus)
	//直线绘制
	ON_COMMAND(ID_STRAIGHT_LINE, &CMainFrame::OnNoticeBoard)
	ON_UPDATE_COMMAND_UI(ID_STRAIGHT_LINE, &CMainFrame::OnUpdateNoticeBoard)

	ON_COMMAND(ID_HISTORY_TREND, &CMainFrame::OnHistoryTrendAnalysis)
	ON_UPDATE_COMMAND_UI(ID_HISTORY_TREND, &CMainFrame::OnUpdateHistoryTrendAnalysis)
	//矩形绘制
	ON_COMMAND(ID_ENERGY_SAVE_ROI, &CMainFrame::OnEnergySaveRoiAnalysis)
	ON_UPDATE_COMMAND_UI(ID_ENERGY_SAVE_ROI, &CMainFrame::OnUpdateEnergySaveRoiAnalysis)

	ON_COMMAND(ID_MAP_NAVIGATE, &CMainFrame::OnMapNavigate)
	ON_UPDATE_COMMAND_UI(ID_MAP_NAVIGATE, &CMainFrame::OnUpdateMapNavigate)

	ON_COMMAND(ID_NAVIGATE_WINDOW, &CMainFrame::OnNavigateWindow)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_WINDOW, &CMainFrame::OnUpdateNavigateWindow)

	ON_COMMAND(ID_LEVEL_RULER, &CMainFrame::OnLevelRuler)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_RULER, &CMainFrame::OnUpdateLevelRuler)

	ON_COMMAND(ID_DATA_ANALYSE, &CMainFrame::OnDataAnalyse)
	ON_UPDATE_COMMAND_UI(ID_DATA_ANALYSE, &CMainFrame::OnUpdateDataAnalyse)

	ON_COMMAND(ID_TEMPER_DISTRI, &CMainFrame::OnTemperDistri)
	ON_UPDATE_COMMAND_UI(ID_TEMPER_DISTRI, &CMainFrame::OnUpdateTemperDistri)

	ON_COMMAND(ID_DATA_REPORT, &CMainFrame::OnDataReport)
	ON_UPDATE_COMMAND_UI(ID_DATA_REPORT, &CMainFrame::OnUpdateDataReport)

	ON_COMMAND(ID_PLANE_TEMPE_DISTR, &CMainFrame::OnPlaneTempeDistr)
	ON_UPDATE_COMMAND_UI(ID_PLANE_TEMPE_DISTR, &CMainFrame::OnUpdatePlaneTempeDistr)

	ON_COMMAND(ID_DOTLINE_FRAME, &CMainFrame::OnDottedLineFrame)
	ON_UPDATE_COMMAND_UI(ID_DOTLINE_FRAME, &CMainFrame::OnUpdateDottedLineFrame)

	ON_COMMAND(ID_GENERAL_COMPONENT, &CMainFrame::OnGeneralComponent)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_COMPONENT, &CMainFrame::OnUpdateGeneralComponent)

	ON_COMMAND(IDM_PAGE_BATCH_GEN, &CMainFrame::OnPageBatchGen)
	ON_COMMAND(IDM_PAGE_BATCH_REPLACEPOINT, &CMainFrame::OnPageBatchReplacepoint)
	ON_COMMAND(IDM_PAGE_BATCH_REPLACE_LINK, &CMainFrame::OnPageBatchReplaceLink)
	ON_UPDATE_COMMAND_UI(IDM_PAGE_BATCH_GEN, &CMainFrame::OnUpdatePageBatchGen)
	ON_UPDATE_COMMAND_UI(IDM_PAGE_BATCH_REPLACEPOINT, &CMainFrame::OnUpdatePageBatchReplacepoint)
	ON_UPDATE_COMMAND_UI(IDM_PAGE_BATCH_REPLACE_LINK, &CMainFrame::OnUpdatePageBatchReplaceLink)
	ON_COMMAND(ID_IMAGE_MGR, &CMainFrame::OnImageMgr)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_MGR, &CMainFrame::OnUpdateImageMgr)
	ON_COMMAND(ID_IMAGE_CHECK, &CMainFrame::OnImageCheck)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_CHECK, &CMainFrame::OnUpdateImageCheck)
	ON_COMMAND(ID_S3DB_CHECK, &CMainFrame::OnS3dbCheck)
	ON_UPDATE_COMMAND_UI(ID_S3DB_CHECK, &CMainFrame::OnUpdateS3dbCheck)

	ON_CBN_SELCHANGE(ID_PAGE_PERCENT,&CMainFrame::OnComboBox)
	ON_COMMAND(ID_PAGE_PERCENT,&CMainFrame::ClickComboBox)
	ON_UPDATE_COMMAND_UI(ID_PAGE_PERCENT, &CMainFrame::OnUpdateComboBo)
	ON_COMMAND(IDM_SOFT_PREFERENCE,&CMainFrame::OnSoftPreference)

	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMainFrame::OnUpdateEditUndo)

	ON_COMMAND(ID_EDIT_REDO, &CMainFrame::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CMainFrame::OnUpdateEditRedo)
	ON_COMMAND(ID_OPEN_IMAGE_MGR, &CMainFrame::OnOpenImageMgr)
	ON_UPDATE_COMMAND_UI(ID_OPEN_IMAGE_MGR, &CMainFrame::OnUpdateOpenImageMgr)
	ON_COMMAND(ID_OPEN_IMAGE_REPLACE_INFO, &CMainFrame::OnOpenImageReplaceInfo)
	ON_UPDATE_COMMAND_UI(ID_OPEN_IMAGE_REPLACE_INFO, &CMainFrame::OnUpdateOpenImageReplaceInfo)
	ON_COMMAND(ID_IMG_LEAN, &CMainFrame::OnImgLean)
	ON_UPDATE_COMMAND_UI(ID_IMG_LEAN, &CMainFrame::OnUpdateImgLean)
	ON_COMMAND(ID_IMG_LEAN_CORE, &CMainFrame::OnImgLeanCore)
	ON_UPDATE_COMMAND_UI(ID_IMG_LEAN_CORE, &CMainFrame::OnUpdateImgLeanCore)
	ON_COMMAND(ID_DEV_AREA_SYNC, &CMainFrame::OnDevAreaSync)
	ON_UPDATE_COMMAND_UI(ID_DEV_AREA_SYNC, &CMainFrame::OnUpdateDevAreaSync)
	ON_COMMAND(ID_PAGE_DEV_AREA_SYNC, &CMainFrame::OnPageDevAreaSync)
	ON_UPDATE_COMMAND_UI(ID_PAGE_DEV_AREA_SYNC, &CMainFrame::OnUpdatePageDevAreaSync)
	ON_COMMAND(ID_VER_HIST, &CMainFrame::OnVersionHistory)
	ON_UPDATE_COMMAND_UI(ID_VER_HIST, &CMainFrame::OnUpdateVersionHistory)

	ON_COMMAND(IDM_VPOINT_BACKUP, &CMainFrame::OnVpointBackup)
	ON_COMMAND(IDM_VPOINT_RELOAD, &CMainFrame::OnVpointReload)
	ON_UPDATE_COMMAND_UI(IDM_VPOINT_BACKUP, &CMainFrame::OnUpdateVpointBackup)
	ON_UPDATE_COMMAND_UI(IDM_VPOINT_RELOAD, &CMainFrame::OnUpdateVpointReload)
	ON_COMMAND(IDM_RESTART_DOMCORE, &CMainFrame::OnRestartDomcore)
	ON_COMMAND(IDM_STOP_DOMCORE, &CMainFrame::OnStopDomcore)
	ON_COMMAND(IDM_RESTART_DOMLOGIC, &CMainFrame::OnRestartDomlogic)
	ON_COMMAND(IDM_STOP_DOMLOGIC, &CMainFrame::OnStopDomlogic)
	ON_COMMAND(IDM_RESTART_DOMALL, &CMainFrame::OnRestartDomall)
	ON_COMMAND(IDM_STOP_DOMALL, &CMainFrame::OnStopDomall)
	ON_UPDATE_COMMAND_UI(IDM_RESTART_DOMCORE, &CMainFrame::OnUpdateRestartDomcore)
	ON_UPDATE_COMMAND_UI(IDM_STOP_DOMCORE, &CMainFrame::OnUpdateStopDomcore)
	ON_UPDATE_COMMAND_UI(IDM_RESTART_DOMLOGIC, &CMainFrame::OnUpdateRestartDomlogic)
	ON_UPDATE_COMMAND_UI(IDM_STOP_DOMLOGIC, &CMainFrame::OnUpdateStopDomlogic)
	ON_UPDATE_COMMAND_UI(IDM_RESTART_DOMALL, &CMainFrame::OnUpdateRestartDomall)
	ON_UPDATE_COMMAND_UI(IDM_STOP_DOMALL, &CMainFrame::OnUpdateStopDomall)
	ON_COMMAND(IDM_SEARCH, &CMainFrame::OnSearch)
	ON_UPDATE_COMMAND_UI(IDM_SEARCH, &CMainFrame::OnUpdateSearch)
	ON_COMMAND(IDM_LOGIN_CORE, &CMainFrame::OnLoginCore)
	ON_UPDATE_COMMAND_UI(IDM_LOGIN_CORE, &CMainFrame::OnUpdateLoginCore)
	ON_COMMAND(IDM_MODE_SITE, &CMainFrame::OnModeSite)
	ON_COMMAND(IDM_MODE_SUMULATE, &CMainFrame::OnModeSumulate)
	ON_UPDATE_COMMAND_UI(IDM_MODE_SUMULATE, &CMainFrame::OnUpdateModeSumulate)
	ON_UPDATE_COMMAND_UI(IDM_MODE_SITE, &CMainFrame::OnUpdateModeSite)
	ON_COMMAND(IDM_OPRECORD_HISTORY, &CMainFrame::OnOprecordHistory)
	ON_UPDATE_COMMAND_UI(IDM_OPRECORD_HISTORY, &CMainFrame::OnUpdateOprecordHistory)
	ON_COMMAND(IDM_SAMPLE_MANAGER, &CMainFrame::OnSampleManager)
	ON_UPDATE_COMMAND_UI(IDM_SAMPLE_MANAGER, &CMainFrame::OnUpdateSampleManager)
	ON_COMMAND(IDM_CORE_STATUS, &CMainFrame::OnCoreStatus)
	ON_UPDATE_COMMAND_UI(IDM_CORE_STATUS, &CMainFrame::OnUpdateCoreStatus)
	ON_COMMAND(IDM_HISDATA_IMPORT, &CMainFrame::OnImportHistory)
	ON_UPDATE_COMMAND_UI(IDM_HISDATA_IMPORT, &CMainFrame::OnUpdateImportHistory)

	ON_COMMAND(IDM_HISTORY_EXPORT, &CMainFrame::OnHistoryExport)
	ON_UPDATE_COMMAND_UI(IDM_HISTORY_EXPORT, &CMainFrame::OnUpdateHistoryExport)
	ON_COMMAND(IDM_CHANGE_POINT_NAME_FOR_HISTORICAL_DATA, &CMainFrame::OnChangePointNameForHistoricalData)
	ON_UPDATE_COMMAND_UI(IDM_CHANGE_POINT_NAME_FOR_HISTORICAL_DATA, &CMainFrame::OnUpdateChangePointNameForHistoricalData)
	ON_COMMAND(IDM_WARNING_CONFIG_RESTORE, &CMainFrame::OnWarningConfigRestore)
	ON_UPDATE_COMMAND_UI(IDM_WARNING_CONFIG_RESTORE, &CMainFrame::OnUpdateWarningConfigRestore)
	ON_COMMAND(IDM_WARNING_CONFIG_BACKUP, &CMainFrame::OnWarningConfigBackup)
	ON_COMMAND(IDM_4DB_UPLOAD_TO_CLOUD, &CMainFrame::On4dbUploadToCloud)
	ON_COMMAND(IDM_4DB_UPDATE_FROM_CLOUD, &CMainFrame::On4dbUpdateFromCloud)
	ON_UPDATE_COMMAND_UI(IDM_4DB_UPLOAD_TO_CLOUD, &CMainFrame::OnUpdate4dbUploadToCloud)
	ON_COMMAND(IDM_VIEW_POINT_PANE, &CMainFrame::OnViewPointPane)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_POINT_PANE, &CMainFrame::OnUpdateViewPointPane)
	ON_COMMAND(IDM_GEN_STANDARD_POINTS, &CMainFrame::OnGenStandardPoints)
	ON_UPDATE_COMMAND_UI(IDM_GEN_STANDARD_POINTS, &CMainFrame::OnUpdateGenStandardPoints)
	ON_COMMAND(IDM_HISDATA_MANAGER, &CMainFrame::OnHisdataManager)
	ON_UPDATE_COMMAND_UI(IDM_HISDATA_MANAGER, &CMainFrame::OnUpdateHisdataManager)
	ON_COMMAND(IDM_BATCH_MODIFY_PIPE_WIDTH, &CMainFrame::OnBatchModifyPipeWidth)
	ON_UPDATE_COMMAND_UI(IDM_BATCH_MODIFY_PIPE_WIDTH, &CMainFrame::OnUpdateBatchModifyPipeWidth)
	ON_COMMAND(IDM_CHECK_MISS_IMAGE, &CMainFrame::OnCheckMissImage)
	ON_UPDATE_COMMAND_UI(IDM_CHECK_MISS_IMAGE, &CMainFrame::OnUpdateCheckMissImage)
	ON_COMMAND(IDM_CLOUD_UPDATE_SYSLOGIC, &CMainFrame::OnCloudUpdateSyslogic)
	ON_UPDATE_COMMAND_UI(IDM_CLOUD_UPDATE_SYSLOGIC, &CMainFrame::OnUpdateCloudUpdateSyslogic)
	ON_COMMAND(IDM_VIEW_CORE_VERSIONS, &CMainFrame::OnViewCoreVersions)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_CORE_VERSIONS, &CMainFrame::OnUpdateViewCoreVersions)
	ON_COMMAND(IDM_ADD_VPOINT_BATCH, &CMainFrame::OnAddVpointBatch)
	ON_COMMAND(IMD_COMPLETION_DATA, &CMainFrame::OnCompletionData)
	ON_UPDATE_COMMAND_UI(IMD_COMPLETION_DATA, &CMainFrame::OnUpdateCompletionData)
	ON_COMMAND(IDM_Export_HistoricalData_At_PreciseTime, &CMainFrame::OnExportHistoricalDataAtPreciseTime)
	ON_UPDATE_COMMAND_UI(IDM_Export_HistoricalData_At_PreciseTime, &CMainFrame::OnUpdateExportHistoricalDataAtPreciseTime)
	ON_COMMAND(ID_DATABASE_RESTORE, &CMainFrame::OnDatabaseRestore)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_RESTORE, &CMainFrame::OnUpdateDatabaseRestore)
	ON_COMMAND(ID_DATABASE_BACKUP_MANAGER, &CMainFrame::OnDatabaseBuckupManager)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_BACKUP_MANAGER, &CMainFrame::OnUpdateDatabaseBuckupManager)
	END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	IDS_TIMER,
	//IDS_PROGRESS,
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
:m_hBrush(NULL)
,m_bIsNeedFlashMenu(false)
,m_DrawContext(&m_project)
,m_comboFont(NULL)
,m_dlgstate(2)
,m_bFullScreen(false)
,m_pLayerManageDlg(NULL)
,m_pPointMapDlg(NULL)
,m_pPrjSqlite(NULL)
,m_pOnlinePrjSqlite(NULL)
,m_bOnlyOpenPointTable(false)
,m_bLoadingPage(false)
,m_bUpdateScreen(true)
,m_bLoadIndependent(false)
,m_bNeedSave(false)
,m_bCheckImageMode(false)
,m_nElementID(-1)
,m_bPtTableNeedSave(false)
,m_nSelectPicID(-1)
,m_comboButton(NULL)
,m_pFormatBrush(NULL)
,m_nPasteXCount(1)
,m_nPasteYCount(1)
,m_nPasteOffset(40)
,m_nPageLuX(0)
,m_nPageLuY(0)
,m_nPageRdX(0)
,m_nPageRdY(0)
,m_nSelLuX(0)
,m_nSelLuY(0)
,m_nSelRdX(0)
,m_nSelRdY(0)
,m_bFirstPaste(true)
,m_nItemNameLen(200)
,m_pImgReplageDlg(NULL)
,m_dwSyncIp(0)
,m_nSyncPort(0)
,m_nBendSize(14)
,m_nCalcPort(5000)
,m_bFtpEnable(FALSE)
,m_nFtpPort(21)
,m_pNoModelDlg(NULL)
,m_bSyncDec(false)
{
	// TODO: add member initialization code here
	pGdiToken = NULL;
	GdiplusStartup(&pGdiToken, &gdiplusStartupInput, NULL);
	m_ProjectOpen = FALSE;
	CBEOPDesignerApp *theApp = (CBEOPDesignerApp *)AfxGetApp();
	m_AppName = theApp->m_wstr_app_name.c_str();
	m_MaxPageID = 0;
	m_MaxEqmtID = -1;
	m_bIsCreateProject = FALSE;
	
	CBitmap bpcold,bphot, bpdisable;
	bpcold.LoadBitmap(IDB_TOOLBARHOT2);
	bphot.LoadBitmap(IDB_TOOLBARHOT2);
	bpdisable.LoadBitmap(IDB_TOOLBARDISABLED);
	m_ImageListToolbar.Create(TOOLBARIMAGESIZE, TOOLBARIMAGESIZE, ILC_COLOR32 | ILC_MASK, 1, 1);
	m_ImageListToolbar_Hot.Create(TOOLBARIMAGESIZE, TOOLBARIMAGESIZE, ILC_COLOR32 | ILC_MASK, 1, 1);
	m_ImageListToolbar_Disable.Create(TOOLBARIMAGESIZE, TOOLBARIMAGESIZE, ILC_COLOR32 | ILC_MASK, 1, 1);

	m_ImageListToolbar.Add(&bpcold, RGB_BLACK);
	m_ImageListToolbar_Hot.Add(&bphot, RGB_BLACK);
	m_ImageListToolbar_Disable.Add(&bpdisable, RGB_BLACK);

	m_uiMenuID = MENUSTARTID;
	m_processId = GetCurrentProcessId();
	CString   strTest;   
	strTest.Format(_T("%d"),m_processId); 
	m_str_proId = strTest.GetString();

	m_threadhandle_LoadProject = NULL;
	//读取默认颜色
	{
		CString strSys;
		GetSysPath(strSys);
		strSys += L"\\factory.ini";
		m_colorArray[0] = GetPrivateProfileInt(L"USER",L"color01",0,strSys);
		m_colorArray[1] = GetPrivateProfileInt(L"USER",L"color02",0,strSys);
		m_colorArray[2] = GetPrivateProfileInt(L"USER",L"color03",0,strSys);
		m_colorArray[3] = GetPrivateProfileInt(L"USER",L"color04",0,strSys);
		m_colorArray[4] = GetPrivateProfileInt(L"USER",L"color05",0,strSys);
		m_colorArray[5] = GetPrivateProfileInt(L"USER",L"color06",0,strSys);
		m_colorArray[6] = GetPrivateProfileInt(L"USER",L"color07",0,strSys);
		m_colorArray[7] = GetPrivateProfileInt(L"USER",L"color08",0,strSys);

		m_nItemNameLen = GetPrivateProfileInt(_T("BeopFactory"), _T("NameLen"), 200, strSys);

		// get synchronous
		m_dwSyncIp = GetPrivateProfileInt(_T("BeopFactory"), _T("SyncIp"), 0, strSys);
		m_nSyncPort = GetPrivateProfileInt(_T("BeopFactory"), _T("SyncPort"), 3306, strSys);

		TCHAR szBuf[MAX_PATH] = {0};
		GetPrivateProfileString(_T("BeopFactory"), _T("SyncUsername"), NULL, szBuf, MAX_PATH, strSys);
		m_strSyncUsername.Format(_T("%s"), szBuf);

		GetPrivateProfileString(_T("BeopFactory"), _T("SyncPassword"), NULL, szBuf, MAX_PATH, strSys);
		m_strSyncPassword.Format(_T("%s"), szBuf);

		GetPrivateProfileString(_T("BeopFactory"), _T("SyncDbName"), NULL, szBuf, MAX_PATH, strSys);
		m_strSyncDbName.Format(_T("%s"), szBuf);

		GetPrivateProfileString(_T("BeopFactory"), _T("CalcIp"), NULL, szBuf, MAX_PATH, strSys);
		m_strCalcIp.Format(_T("%s"), szBuf);
		if(m_strCalcIp==_T(""))
		{
			m_strCalcIp = _T("127.0.0.1");
		}

		m_nCalcPort = GetPrivateProfileInt(_T("BeopFactory"), _T("CalcPort"), 5000, strSys);
		if(m_nCalcPort==0)
		{
			m_nCalcPort = 5000;
		}

		GetPrivateProfileString(_T("BeopFactory"), _T("CalcPostName"), NULL, szBuf, MAX_PATH, strSys);
		m_strCalcPostName.Format(_T("%s"), szBuf);
		if(m_strCalcPostName==_T(""))
		{
			m_strCalcPostName = _T("calc_module_interface");
		}

		// ftp parm
		CString strAppName(_T("FtpServer"));
		TCHAR szFtpBuf[MAX_PATH] = {0};
		GetPrivateProfileString(strAppName, _T("Ip"), _T(""), szFtpBuf, MAX_PATH, strSys);
		m_strFtpIp.Format(_T("%s"), szFtpBuf);

		wmemset(szFtpBuf, 0, MAX_PATH);
		GetPrivateProfileString(strAppName, _T("Username"), _T(""), szFtpBuf, MAX_PATH, strSys);
		m_strFtpUser.Format(_T("%s"), szFtpBuf);

		wmemset(szFtpBuf, 0, MAX_PATH);
		GetPrivateProfileString(strAppName, _T("Password"), _T(""), szFtpBuf, MAX_PATH, strSys);
		m_strFtpPwd.Format(_T("%s"), szFtpBuf);

		m_nFtpPort = GetPrivateProfileInt(strAppName, _T("Port"), 21, strSys);

		int nFtpEnable = GetPrivateProfileInt(strAppName, _T("FtpEnable"), 0, strSys);
		m_bFtpEnable = ((0 == nFtpEnable) ? FALSE : TRUE);
	}

	// 读取版本信息
	/*CString strPath;
	GetSysPath(strPath);
	strPath += _T("\\factory.ini");
	GetVersionFromHttp(strPath);*/

	m_pProjectCheckDlg.Create(IDD_DIALOG_PROJECT_CHECK, NULL);
	m_pProjectCheckDlg.ShowWindow(FALSE);

	m_pNoModelDlg = new CNoModelDlg(this);
	if (m_pNoModelDlg != NULL)
	{
		m_pNoModelDlg->Create(IDD_DIALOG_NO_MODEL);
		m_pNoModelDlg->ShowWindow(SW_HIDE);
	}

	m_strCloudServiceIp = _T("dom.inwhile.com");
	m_nCloudServicePort = 80;
}

CMainFrame::~CMainFrame()
{
	if (m_comboFont)
	{
		delete m_comboFont;
		m_comboFont = NULL;
	}
	if(m_pLayerManageDlg)
	{
		m_pLayerManageDlg->DestroyWindow();
		delete m_pLayerManageDlg;
		m_pLayerManageDlg = NULL;
	}
	if (m_pPointMapDlg)
	{
		m_pPointMapDlg->DestroyWindow();
		delete m_pPointMapDlg;
		m_pPointMapDlg = NULL;
	}
	if (m_pPrjSqlite)
	{
		delete m_pPrjSqlite;
		m_pPrjSqlite = NULL;
	}
	if(m_pOnlinePrjSqlite)
	{
		delete m_pOnlinePrjSqlite;
		m_pOnlinePrjSqlite = NULL;
	}
	if (m_comboButton != NULL)
	{
		delete m_comboButton;
		m_comboButton= NULL;
	}
	if (m_pImgReplageDlg != NULL)
	{
		delete m_pImgReplageDlg;
		m_pImgReplageDlg = NULL;
	}

	if (m_pNoModelDlg != NULL)
	{
		delete m_pNoModelDlg;
		m_pNoModelDlg = NULL;
	}

	UtilDrawNumber::DestroyInstance();
	GdiplusShutdown(pGdiToken);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |CBRS_TOP|CBRS_TOOLTIPS) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),IDR_MAINFRAME) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME,0,0, FALSE,0,0,IDB_TOOLBARHOT))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME,IDB_TOOLBARHOT,0, FALSE,IDB_TOOLBARDISABLED,0,IDB_TOOLBARHOT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, _T("Customize"));

	//
	m_comboButton = new CMFCToolBarComboBoxButton(ID_PAGE_PERCENT,
		-1,// 工具条位图中的图像序号
		CBS_DROPDOWNLIST,                                             //组合框类型
		60);                                                //组合框的宽度
	m_comboButton->EnableWindow(TRUE);
	m_comboButton->SetCenterVert();
	m_comboButton->SetDropDownHeight(16);//设置下拉列表的高度
	m_comboButton->SetFlatMode();

	//添加内容
	m_comboButton->AddItem(_T("50%"));
	m_comboButton->AddItem(_T("60%"));
	m_comboButton->AddItem(_T("70%"));
	m_comboButton->AddItem(_T("80%"));
	m_comboButton->AddItem(_T("90%"));
	m_comboButton->AddItem(_T("100%"));
	m_comboButton->AddItem(_T("110%"));
	m_comboButton->AddItem(_T("120%"));
	m_comboButton->AddItem(_T("130%"));
	m_comboButton->AddItem(_T("140%"));
	m_comboButton->AddItem(_T("150%"));
	m_comboButton->SelectItem(5);//默认选中

	//添加到工具栏
	m_wndToolBar.InsertSeparator();//先添加分隔条
	m_wndToolBar.InsertButton(*m_comboButton);

	//
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_PAGE_PERCENT),TEXT("百分比"));

	if (!m_wndToolBarLayout.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), IDR_TOOLBAR_LAYOUT) ||
		!m_wndToolBarLayout.LoadToolBar(IDR_TOOLBAR_LAYOUT,IDB_TOOLBARHOT_ALIGN,0, FALSE,IDB_TOOLBAALIGN_DISABLE,0,IDB_TOOLBARHOT_ALIGN))
	{
		TRACE0("Failed to create layout toolbar\n");
		return -1;      // fail to create
	}

    //load accelarate
    HANDLE m_hAccel =::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME) );
    

	m_topMenu.Attach(this->GetMenu()->GetSafeHmenu());
//	m_topMenu.ChangeMenuItem(&m_topMenu,TRUE);
//	m_topMenu.SetMenuImageList();

	
	m_LoadingProjectBar.Create(IDD_DIALOG_LOADING, this);
	m_LoadingProjectBar.ShowWindow(FALSE);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to be dockable

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarLayout.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndToolBarLayout);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MAINFRAME, IDB_TOOLBARHOT2);
	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}
	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	//CMFCToolBar::SetSizes(CSize(24,24),CSize(24,24));

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, _T("Customize"), ID_VIEW_TOOLBAR);


	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	// 启用菜单个性化(最近使用的命令)
	
	if (m_pLayerManageDlg == NULL)
	{
		m_pLayerManageDlg = new CLayerManageDlg(this);
		ASSERT(m_pLayerManageDlg);
		m_pLayerManageDlg->Create(IDD_DIALOG_LAYER_MANAGE,this);
		m_pLayerManageDlg->SetWindowPos(NULL,1000,100,0,0,SWP_NOSIZE);
		m_pLayerManageDlg->ShowWindow(SW_SHOW);
	}
	if (m_pPrjSqlite == NULL)
	{
		m_pPrjSqlite = new CProjectSqlite(m_project, m_plcpointmap, m_logicPotManager,m_AlarmPotManager, 0);
		ASSERT(m_pPrjSqlite);
	}
	if (NULL == m_pImgReplageDlg)
	{
		m_pImgReplageDlg = new CImageReplaceDlg(this);
		m_pImgReplageDlg->Create(IDD_DIALOG_IMG_REPLACE, NULL);
		m_pImgReplageDlg->ShowWindow(SW_HIDE);
	}

	SetComboSel(5);

	SyncVersionByFtp();

	SetWindowText(m_AppName.GetString() );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style &= ~(LONG)FWS_ADDTOTITLE;

	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnFileCreateproject()
{
	wstring wstrSaveProjectName;
	int		nDeviceType = 0;
	int		nFileType = 0;
	CNewProjectDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		wstrSaveProjectName = dlg.m_ProjectName;
		nDeviceType = dlg.GetDeviceType();
		nFileType = dlg.GetFileType();
	}
	else
	{
		return;
	}

	if(!CloseProjectBeforeOpen() )
	{
		return;
	}

	//SetWindowText(dlg.m_ProjectName);
	m_project.ClearPageList();
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	ASSERT(pView);
	pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);

	wstring wstrFILE_NAME = _T("project.");
	wstrFILE_NAME += c_wstr_default_file_sufix;
	CString cstrFileSufixOption;
	cstrFileSufixOption.Format(_T("*.%s|*.%s|") , c_wstr_default_file_sufix.c_str(), c_wstr_default_file_sufix.c_str() );
	CString cstr;

	CString strSufix;
	if (0 == nFileType)
	{
		strSufix.Format(_T("%s"), c_wstr_default_file_sufix.c_str());
		m_pPrjSqlite->SetFileEncryptType(true);
	}
	else if (1 == nFileType)
	{
		strSufix.Format(_T("%s"), c_wstr_4db_file_sufix.c_str());
		m_pPrjSqlite->SetFileEncryptType(false);
	}
	else
	{
		return;
	}
	m_strFilePath.Format(L"%s\\%s.%s",dlg.m_strFileDir, dlg.m_ProjectName, strSufix);
	if (PathFileExists(m_strFilePath))
	{
		CString strShow;
		strShow.Format(_T("文件 %s 已存在，是否覆盖？"), m_strFilePath);
		if (IDYES != MessageBox(strShow, _T("提示"), MB_YESNO|MB_ICONWARNING))
		{
			return;
		}
	}

	CString strAppTitle;
	strAppTitle.Format(_T("%s - %s"), m_AppName, m_strFilePath);
	SetWindowText(strAppTitle.GetString() );

	//
	bool	bIsCreateOnly = false;
	CString	strSysBEOPFileName;
	CString strExePath;
	Project::Tools::GetSysPath(strExePath);
	strSysBEOPFileName.Format(L"%s\\config\\beopsys.%s", strExePath, strSufix);
	bIsCreateOnly = true;

	if(::CopyFile(strSysBEOPFileName, m_strFilePath, FALSE)==0)
	{
		AfxMessageBox(L"项目文件创建失败!");
		return;
	}

	if (bIsCreateOnly)
	{	// create with no template
		//pView->m_pTreeDlg->DeleteAllItems();
		m_wndFileView.DeleteAllItems();
		m_pPrjSqlite->SetFilePath(m_strFilePath.GetString());
	    LoadProjectFile(m_strFilePath.GetString(), true);
		m_project.SetProjectID(1);
		m_project.SetProjectName(wstrSaveProjectName);
		m_project.SetScreenWidth(srPoint[5].x);
		m_project.SetScreenHeight(srPoint[5].y);
		m_project.SetVersion(L"2.2");
		m_project.SetDeviceType(nDeviceType);

		m_strDBFileName = m_strFilePath;
		//pView->m_pTreeDlg->SetDeviceType(nDeviceType);
		//pView->m_pTreeDlg->InitTreeNodes();

		m_wndFileView.SetDeviceType(nDeviceType);
		//m_wndFileView.InitTreeNodes();
		//m_wndFileView.InitNode();
		m_wndFileView.ClearTreeNode();
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		DWORD nYear = tm.wYear;
		DWORD nMonth = tm.wMonth;
		DWORD nDay = tm.wDay;
		DWORD nHour = tm.wHour;
		DWORD nMinute = tm.wMinute;
		DWORD nSecond = tm.wSecond;

		CString strTime;
		strTime.Format(L"%.4d-%.2d-%.2d %.2d:%.2d:%.2d",nYear,nMonth,nDay,nHour,nMinute,nSecond);
		m_project.SetxupTime(strTime.GetString());

		m_MaxPageID = 0;
		m_ProjectOpen = TRUE;		
		m_DrawContext.pPage = NULL;
		ShowCurrentPage();
		m_bIsCreateProject = TRUE;
	}
	else
	{	// use template
		m_bOnlyOpenPointTable = false;
		//pView->m_pTreeDlg->DeleteAllItems();
		m_wndFileView.DeleteAllItems();
		if (!CheckFileName(m_strFilePath))
		{
			return;
		}
		m_pPrjSqlite->SetFilePath(m_strFilePath.GetString());
		StartLoadThread();

		m_strDBFileName = m_strFilePath.GetString();
	}

	
	pView->m_PageDlg->m_paneArbiter.Exit();
}

void CMainFrame::OnFileOpenprojectSqlite()
{
	// TODO: Add your command handler code here
	wstring wstrFILE_NAME = _T("project.");
	wstrFILE_NAME += c_wstr_default_file_sufix;
	CString cstrFileSufixOption;
	cstrFileSufixOption.Format(_T("*.%s|*.%s|*.%s|*.%s||") , c_wstr_4db_file_sufix.c_str(), c_wstr_4db_file_sufix.c_str(), c_wstr_default_file_sufix.c_str(), c_wstr_default_file_sufix.c_str());
	CFileDialogEx dlg(TRUE, wstrFILE_NAME.c_str(), wstrFILE_NAME.c_str(), 0, cstrFileSufixOption, this);
	dlg.SetOpenType(0);
	dlg.SetProjectSqlite(m_pPrjSqlite);
	if ( IDOK == dlg.DoModal() )
	{
		OpenProjectContain(dlg.GetPathName());
	}
}

void CMainFrame::GetVersionPrefix(CString strVersion, int &nPrefix1, int &nPrefix2, int &nPrefix3, int &nPrefix4)
{
	int nDot1 = strVersion.Find(L".");
	if(nDot1<0)
		return;

	int nDot2 = strVersion.Find(L".", nDot1+1);
	if(nDot2<0 || nDot2<=nDot1)
		return;

	int nDot3 = strVersion.Find(L".", nDot2+1);
	if(nDot3<0 || nDot3<=nDot2)
		return;

	CString strTemp1 = strVersion.Left(nDot1);
	CString strTemp2 = strVersion.Mid(nDot1+1, nDot2-nDot1-1);
	CString strTemp3 = strVersion.Mid(nDot2+1, nDot3-nDot2-1); 
	CString strTemp4 = strVersion.Mid(nDot3+1); 

	nPrefix1 = _wtoi(strTemp1.GetString());
	nPrefix2 = _wtoi(strTemp2.GetString());
	nPrefix3 = _wtoi(strTemp3.GetString());
	nPrefix4 = _wtoi(strTemp4.GetString());
}

void CMainFrame::OnFileOpenprojectSqlite_OnlyPointTable()
{
	// TODO: Add your command handler code here
	wstring wstrFILE_NAME = _T("project.");
	wstrFILE_NAME += c_wstr_default_file_sufix;
	CString cstrFileSufixOption;
	cstrFileSufixOption.Format(_T("*.%s|*.%s|*.%s|*.%s||") , c_wstr_default_file_sufix.c_str(), c_wstr_default_file_sufix.c_str(), c_wstr_4db_file_sufix.c_str(), c_wstr_4db_file_sufix.c_str());
	CString cstr;
	CFileDialogEx dlg(TRUE, wstrFILE_NAME.c_str(), wstrFILE_NAME.c_str(), 0, cstrFileSufixOption, this);
	dlg.SetOpenType(0);
	dlg.SetProjectSqlite(m_pPrjSqlite);
	if ( IDOK == dlg.DoModal() )
	{	
		if(!CloseProjectBeforeOpen() )
		{
			return;
		}

		m_bOnlyOpenPointTable = true;
		m_ProjectOpen = TRUE;
		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
		ASSERT(pView);
		//pView->m_pTreeDlg->DeleteAllItems();
		m_wndFileView.DeleteAllItems();
		cstr =  dlg.GetPathName();
		m_strFilePath = cstr;

		//pView->m_pTreeDlg->m_pMainFrame = this;
		m_wndFileView.m_pMainFrame = this;
		m_plcpointmap.Clear();
		m_logicPotManager.Clear();
		m_AlarmPotManager.Clear();
		if (m_pPointMapDlg)
		{
			m_pPointMapDlg->ClearListAll();
		}

		m_pPrjSqlite->SetOwner(eFactory);
		string strLastEditSoftVersion = m_pPrjSqlite->GetProjectLastEditSoftVersion();

		m_pPrjSqlite->LoadProjectSqlite_opcid3();

		if (m_pPointMapDlg == NULL)
		{
			m_pPointMapDlg = new CDataPointConfigDlg(m_plcpointmap);
			m_pPointMapDlg->Create(IDD_DATA_POINT_CONFIG, this);
		}
		else
		{
			m_pPointMapDlg->SetPointMap(m_plcpointmap);
		}

		m_pPointMapDlg->ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnUpdateFileCreateproject(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_ProjectOpen);
}

void CMainFrame::OnUpdateFileOpenprojectSqlite(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_ProjectOpen);
}

void CMainFrame::OnUpdateFileOpenprojectSqlite_OnlyPointTable(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_ProjectOpen);
}

void CMainFrame::OnSaveProject()
{
	// TODO: Add your command handler code here
	SaveProjectContain();
}

void CMainFrame::OnUpdateSaveProject(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}

void CMainFrame::OnCloseproject()
{
	if (m_bLoadingPage)
	{
		MessageBox(_T("页面正在加载，当前不可退出！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		return;
	}

	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	ASSERT(pView);
//	OnSaveProject();  //golding delete 2014-02-28
	//pView->m_pTreeDlg->DeleteAllItems();
	//pView->m_pTreeDlg->SetConnectServFlag(false);

	m_wndFileView.DeleteAllItems();
	m_wndFileView.SetConnectServFlag(false);
	pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
	pView->m_PageDlg->m_paneArbiter.ShowWindow(SW_HIDE);
	pView->m_PageDlg->m_paneArbiter.Exit();
	pView->m_PageDlg->m_paneCalc.ShowWindow(SW_HIDE);
	SAFE_DELETE(pView->m_PageDlg->m_paneArbiter.m_pLogicManager);
	m_project.ClearProject();
	m_plcpointmap.Clear();
	if (m_pPointMapDlg)
	{
		m_pPointMapDlg->ClearListAll();
	}
	ResetMarks();	
	SetWindowText(m_AppName);
	ShowCurrentPage();
	m_bIsCreateProject = FALSE;
	m_selectEuVect.clear();
	m_strFilePath = L"";
	m_bLoadIndependent = false;
	m_pFormatBrush = NULL;
	m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
	m_wndProperties.SetNull();
	//m_pPrjSqlite=NULL;
	
}

void CMainFrame::OnUpdateCloseproject(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}

void CMainFrame::ResetMarks()
{
	m_DrawContext.pPage = NULL;
	m_ProjectOpen = FALSE;
	m_MaxPageID = -1;	
	m_DrawContext.ResetContext();
	CommandManager::Instance()->ClearAllCommands();
}

void CMainFrame::ShowCurrentPage()
{
	//CBEOPDesignerView* pview = (CBEOPDesignerView*)GetActiveView();
	//if ( NULL != m_DrawContext.pPage )
	//{
	//	m_DrawContext.ClearSelect();
	//	CString strTitle(L"当前显示页面：");
	//	strTitle += m_DrawContext.pPage->GetPageName().c_str();
	//	SetWindowText(strTitle);

	//	int x = (m_project.GetScreenWidth()-m_DrawContext.pPage->GetPageWidth())/2;
	//	int y = (m_project.GetScreenHeight()-m_DrawContext.pPage->GetPageHeight())/2;
	//	m_DrawContext.pPage->SetPagexPosition(x);
	//	m_DrawContext.pPage->SetPageyPosition(y);

	//	pview->m_PageDlg->Invalidate(TRUE);
	//	pview->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
	//}
}

bool CMainFrame::CloseProjectBeforeOpen()
{
	if (m_ProjectOpen)
	{				
		
		SaveProjectContain();
		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
		ASSERT(pView);
	    
		pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
		m_plcpointmap.Clear();
		if (m_pPointMapDlg)
		{
			m_pPointMapDlg->ClearListAll();
		}
		ResetMarks();	
		SetWindowText(m_AppName);
		ShowCurrentPage();
		m_bIsCreateProject = FALSE;
		m_strFilePath = L"";

	}
	return true;
}

BOOL CMainFrame::GetLoadIfFail()
{
	return IfLoadFail;
}

UINT WINAPI CMainFrame::ThreadFunc_LoadProject( LPVOID lparam )
{
	CMainFrame *pThis = (CMainFrame *) lparam;
	pThis->LoadProjectFile(pThis->m_strFilePath.GetString(), false);
	if (pThis->m_pPointMapDlg)
	{
		pThis->m_pPointMapDlg->MakeAllList();
	}

	CString cstrRunningPath;
	Project::Tools::GetSysPath(cstrRunningPath);
	//检查temp目录存在
	if(DirectoryOperations::CheckFolderExist(cstrRunningPath, L"temp") == false)
	{
		DirectoryOperations::MakeDirectory(cstrRunningPath, L"temp");
	}
	else
	{
		//删除temp目录下所有文件
		DirectoryOperations::DeleteDirectory(cstrRunningPath+L"\\temp\\");
	}

	return 1;
}

void CMainFrame::StartLoadThread()
{
	UINT dwThreadId = 0;

	m_LoadingProjectBar.ShowWindow(TRUE);
	m_LoadingProjectBar.CenterWindow(this);
	m_LoadingProjectBar.m_StaticProgress.SetWindowTextW(L"");
	m_LoadingProjectBar.m_ProgressCtrl.SetRange(0,100);
	m_LoadingProjectBar.m_ProgressCtrl.SetPos(0);

	m_threadhandle_LoadProject = (HANDLE)_beginthreadex(NULL,
		0,
		ThreadFunc_LoadProject, 
		(LPVOID)this,
		0,
		&dwThreadId);
}

void CMainFrame::LoadProjectFile( const std::wstring& filePathName, bool bClear)
{
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	ASSERT(pView);
	//pView->m_pTreeDlg->m_pMainFrame = this;
	m_wndFileView.m_pMainFrame = this;
	m_plcpointmap.Clear();
	m_logicPotManager.Clear();
	m_AlarmPotManager.Clear();
	if (m_pPointMapDlg)
	{
		m_pPointMapDlg->ClearListAll();
	}

	m_pPrjSqlite->SetFilePath(filePathName.c_str());
	if (bClear)
	{
		//m_pPrjSqlite->DeleteProjectConfig();
		m_pPrjSqlite->UpdateProjectConfigXupTime();
		int rc = m_pPrjSqlite->DeleteAllTables();
		assert(0 == rc);
	}
	m_pPrjSqlite->SetOwner(eFactory);
	m_pPrjSqlite->LoadProjectSqlite(&(m_LoadingProjectBar.m_ProgressCtrl), & (m_LoadingProjectBar.m_StaticProgress));
	m_MaxEqmtID = m_pPrjSqlite->GetMaxElementID();
	m_MaxPageID = m_pPrjSqlite->GetMaxPageID()+1;
	m_ProjectOpen = TRUE;
	m_DrawContext.ResetContext();

	m_wndFileView.SetDeviceType(m_project.GetDeviceType());
	m_wndFileView.InitTreeNodes();
	 //m_wndFileView.InitNode();
	const size_t size_group = m_project.m_groupgage.size();
	if (m_project.GetProjectName().size()>0)
	{
		//pView->m_pTreeDlg->InsertRootNode(m_project.GetProjectName().c_str());
		//pView->m_pTreeDlg->SetDeviceType(m_project.GetDeviceType());
		//pView->m_pTreeDlg->InitTreeNodes();
	}
	sort(m_project.m_groupgage.begin(), m_project.m_groupgage.end(), OrderBy);
	if (size_group > 0)
	{
		for (size_t i=0;i<size_group;++i)
		{
			const int nGroupID = m_project.m_groupgage[i].GroupID;
			const wstring strGroupName = m_project.m_groupgage[i].GroupName;
			//HTREEITEM hGroup = pView->m_pTreeDlg->InsertGroup(strGroupName.c_str(), nGroupID);
			HTREEITEM hGroup1 = m_wndFileView.InsertGroup(strGroupName.c_str(), nGroupID);
			ASSERT(hGroup1);
			vector<CEqmDrawPage*> vec_page = m_project.GetPageList();
			const size_t size_page = vec_page.size();
			if (size_page > 0)
			{
				for (size_t j=0;j<size_page;++j)
				{
					if (vec_page[j])
					{
						if (vec_page[j]->GetGroupID() == nGroupID)
						{
							//HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(vec_page[j]->GetPageName().c_str(), hGroup, vec_page[j]->GetID());
							HTREEITEM hPage =m_wndFileView.InsertPage(vec_page[j]->GetPageName().c_str(), hGroup1, vec_page[j]->GetID());
							ASSERT(hPage);
						}
					}
				}
			}
			else
			{
				m_DrawContext.pPage = NULL;
			}
		}
	}

	// insert tree nodes below tactics root
	const int nTacticsNum = -1;
	vector<CEqmDrawPage*> vec_page = m_project.GetPageList();
	const size_t size_page = vec_page.size();
	CString strThreadNameEx;
	if (size_page > 0)
	{
		for (size_t j=0;j<size_page;++j)
		{
			if (vec_page[j])
			{
				if (vec_page[j]->GetGroupID() >= 0)
				{
					continue;
				}
				else if (vec_page[j]->GetGroupID() == nTacticsNum)
				{
					//HTREEITEM hParent = pView->m_pTreeDlg->GetRootTactics();
					HTREEITEM hParent = m_wndFileView.GetRootTactics();
					strThreadNameEx.Format(_T("%s::(Off)"), vec_page[j]->GetPageName().c_str());
					//HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(strThreadNameEx, hParent, vec_page[j]->GetID());
					HTREEITEM hPage = m_wndFileView.InsertPage(strThreadNameEx, hParent, vec_page[j]->GetID());
					ASSERT(hPage);
				}
			}
		}

		// show first display page
		//m_DrawContext.pPage = m_project.GetPageByID(m_project.GetCurPageID());

		DWORD screenwidth = 0;
		DWORD screenheight = 0;

		screenwidth = m_DrawContext.GetScreenWidth();
		screenheight = m_DrawContext.GetScreenHeight();

		pView->m_PageDlg->m_rtCanvas.bottom = static_cast<long>(m_DrawContext.m_fScreenScale*(LONG)screenheight);  //edit by hun 8.1
		pView->m_PageDlg->m_rtCanvas.right  = static_cast<long>(m_DrawContext.m_fScreenScale*(LONG)screenwidth);
		pView->m_PageDlg->SendMessage(WM_SIZE);
		pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_SHOW);
		//pView->m_pTreeDlg->Expand();
		//pView->m_pTreeDlg->SetFocus();

		m_wndFileView.Expand();
		m_wndFileView.SetFocus();
		//m_wndFileView.SelectFirstPageItem(m_project.GetCurPageID());
		//pView->m_pTreeDlg->SelectFirstPageItem(m_project.GetCurPageID());
	}
	else
	{
		m_DrawContext.pPage = NULL;
	}

	CString strAppTitle;
	strAppTitle.Format(_T("%s - %s"), m_AppName,filePathName.c_str());
	SetWindowText(strAppTitle.GetString() );
	ShowCurrentPage();
	m_bIsCreateProject = FALSE;

	m_LoadingProjectBar.m_ProgressCtrl.SetPos(0);
	m_LoadingProjectBar.m_StaticProgress.SetWindowText(L"");
	m_LoadingProjectBar.ShowWindow(FALSE);
}
void CMainFrame::LoadNewProjectFile( const std::wstring& filePathName, bool bClear)
{
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	ASSERT(pView);
	//pView->m_pTreeDlg->m_pMainFrame = this;
	m_wndFileView.m_pMainFrame = this;
	m_plcpointmap.Clear();
	m_logicPotManager.Clear();
	m_AlarmPotManager.Clear();
	if (m_pPointMapDlg)
	{
		m_pPointMapDlg->ClearListAll();
	}

	m_pPrjSqlite->SetFilePath(filePathName.c_str());
	if (bClear)
	{
		//m_pPrjSqlite->DeleteProjectConfig();
		m_pPrjSqlite->UpdateProjectConfigXupTime();
		int rc = m_pPrjSqlite->DeleteAllTables();
		assert(0 == rc);
	}
	m_pPrjSqlite->SetOwner(eFactory);
	m_pPrjSqlite->LoadProjectSqlite(&(m_LoadingProjectBar.m_ProgressCtrl), & (m_LoadingProjectBar.m_StaticProgress));
	m_MaxEqmtID = m_pPrjSqlite->GetMaxElementID();
	m_MaxPageID = m_pPrjSqlite->GetMaxPageID()+1;
	m_ProjectOpen = TRUE;
	m_DrawContext.ResetContext();

	m_wndFileView.SetDeviceType(m_project.GetDeviceType());
	//m_wndFileView.InitTreeNodes();
	m_wndFileView.InitNode();
	const size_t size_group = m_project.m_groupgage.size();
	if (m_project.GetProjectName().size()>0)
	{
		//pView->m_pTreeDlg->InsertRootNode(m_project.GetProjectName().c_str());
		//pView->m_pTreeDlg->SetDeviceType(m_project.GetDeviceType());
		//pView->m_pTreeDlg->InitTreeNodes();
	}
	sort(m_project.m_groupgage.begin(), m_project.m_groupgage.end(), OrderBy);
	if (size_group > 0)
	{
		for (size_t i=0;i<size_group;++i)
		{
			const int nGroupID = m_project.m_groupgage[i].GroupID;
			const wstring strGroupName = m_project.m_groupgage[i].GroupName;
			//HTREEITEM hGroup = pView->m_pTreeDlg->InsertGroup(strGroupName.c_str(), nGroupID);
			HTREEITEM hGroup1 = m_wndFileView.InsertGroup(strGroupName.c_str(), nGroupID);
			ASSERT(hGroup1);
			vector<CEqmDrawPage*> vec_page = m_project.GetPageList();
			const size_t size_page = vec_page.size();
			if (size_page > 0)
			{
				for (size_t j=0;j<size_page;++j)
				{
					if (vec_page[j])
					{
						if (vec_page[j]->GetGroupID() == nGroupID)
						{
							//HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(vec_page[j]->GetPageName().c_str(), hGroup, vec_page[j]->GetID());
							HTREEITEM hPage =m_wndFileView.InsertPage(vec_page[j]->GetPageName().c_str(), hGroup1, vec_page[j]->GetID());
							ASSERT(hPage);
						}
					}
				}
			}
			else
			{
				m_DrawContext.pPage = NULL;
			}
		}
	}

	// insert tree nodes below tactics root
	const int nTacticsNum = -1;
	vector<CEqmDrawPage*> vec_page = m_project.GetPageList();
	const size_t size_page = vec_page.size();
	CString strThreadNameEx;
	if (size_page > 0)
	{
		for (size_t j=0;j<size_page;++j)
		{
			if (vec_page[j])
			{
				if (vec_page[j]->GetGroupID() >= 0)
				{
					continue;
				}
				else if (vec_page[j]->GetGroupID() == nTacticsNum)
				{
					//HTREEITEM hParent = pView->m_pTreeDlg->GetRootTactics();
					HTREEITEM hParent = m_wndFileView.GetRootTactics();
					strThreadNameEx.Format(_T("%s::(Off)"), vec_page[j]->GetPageName().c_str());
					//HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(strThreadNameEx, hParent, vec_page[j]->GetID());
					HTREEITEM hPage = m_wndFileView.InsertPage(strThreadNameEx, hParent, vec_page[j]->GetID());
					ASSERT(hPage);
				}
			}
		}

		// show first display page
		//m_DrawContext.pPage = m_project.GetPageByID(m_project.GetCurPageID());

		DWORD screenwidth = 0;
		DWORD screenheight = 0;

		screenwidth = m_DrawContext.GetScreenWidth();
		screenheight = m_DrawContext.GetScreenHeight();

		pView->m_PageDlg->m_rtCanvas.bottom = static_cast<long>(m_DrawContext.m_fScreenScale*(LONG)screenheight);  //edit by hun 8.1
		pView->m_PageDlg->m_rtCanvas.right  = static_cast<long>(m_DrawContext.m_fScreenScale*(LONG)screenwidth);
		pView->m_PageDlg->SendMessage(WM_SIZE);
		pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_SHOW);
		//pView->m_pTreeDlg->Expand();
		//pView->m_pTreeDlg->SetFocus();

		m_wndFileView.Expand();
		m_wndFileView.SetFocus();
		//m_wndFileView.SelectFirstPageItem(m_project.GetCurPageID());
		//pView->m_pTreeDlg->SelectFirstPageItem(m_project.GetCurPageID());
	}
	else
	{
		m_DrawContext.pPage = NULL;
	}

	CString strAppTitle;
	strAppTitle.Format(_T("%s - %s"), m_AppName,filePathName.c_str());
	SetWindowText(strAppTitle.GetString() );
	ShowCurrentPage();
	m_bIsCreateProject = FALSE;

	m_LoadingProjectBar.m_ProgressCtrl.SetPos(0);
	m_LoadingProjectBar.m_StaticProgress.SetWindowText(L"");
	m_LoadingProjectBar.ShowWindow(FALSE);
}
bool CMainFrame::SaveProjectFile()
{	
	ASSERT(m_strFilePath.GetLength() > 0);
	m_pPrjSqlite->SetFilePath(m_strFilePath.GetString());

	CString strEditSoftVersion;
	int nSoftVersionPrefix1 = 0, nSoftVersionPrefix2 = 0,nSoftVersionPrefix3 =0,nSoftVersionPrefix4 = 0;
	GetFactorySoftVersion(nSoftVersionPrefix1, nSoftVersionPrefix2, nSoftVersionPrefix3, nSoftVersionPrefix4);
	strEditSoftVersion.Format(L"%d.%d.%d.%d",  nSoftVersionPrefix1, nSoftVersionPrefix2, nSoftVersionPrefix3, nSoftVersionPrefix4);
	string strEditSoftVersion_ansi = Project::Tools::WideCharToAnsi(strEditSoftVersion.GetString());

	bool bSave = m_pPrjSqlite->SaveProjectSqlite(strEditSoftVersion_ansi);//

	if (GetPointTableFlag())
	{
		m_pPrjSqlite->SaveProjectSqlite_opcid3();
		SetPointTableFlag(false);
	}

	CheckDatabaseIntegrity(m_strFilePath, 1);
	return bSave;
}	

void CMainFrame::OnMove()
{
	// TODO: Add your command handler code here
	m_DrawContext.m_nDrawMode = m_DrawContext.m_nDrawMode == EQMDRAW_MOVE ? EQMDRAW_UNKNOWN : EQMDRAW_MOVE;
	m_DrawContext.bIsCreatePipe = false;
	m_DrawContext.m_bIsCreate2DGraph = false;
}

void CMainFrame::OnUpdateMove(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_DrawContext.m_nDrawMode == EQMDRAW_MOVE);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnChiller()
{
	// TODO: Add your command handler code here
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_CHILLER != m_DrawContext.m_nDrawMode) ? EQMDRAW_CHILLER : EQMDRAW_MOVE;
	
}

void CMainFrame::OnValve()
{
	// TODO: Add your command handler code here
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_VALVE != m_DrawContext.m_nDrawMode) ? EQMDRAW_VALVE : EQMDRAW_MOVE;
}

void CMainFrame::OnPump()
{
	// TODO: Add your command handler code here
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_PUMP != m_DrawContext.m_nDrawMode) ? EQMDRAW_PUMP : EQMDRAW_MOVE;
}

void CMainFrame::OnPipe()
{
	// TODO: Add your command handler code here
	m_DrawContext.m_nDrawMode = EQMDRAW_STRAIGHTPIPE == m_DrawContext.m_nDrawMode ? EQMDRAW_MOVE : EQMDRAW_STRAIGHTPIPE;

	PipeColor pipe_color;

	m_DrawContext.SetPipeColor(pipe_color);
}
void CMainFrame::OnUpdatePipe(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(EQMDRAW_STRAIGHTPIPE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnUpdateLine(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(GraphicType_LINE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnUpdateRectangle(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(GraphicType_RECTANGLE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnText()
{
	// TODO: Add your command handler code here
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_TEXT != m_DrawContext.m_nDrawMode) ? EQMDRAW_TEXT : EQMDRAW_MOVE;
}


void CMainFrame::OnLine()
{
	// TODO: Add your command handler code here
	m_DrawContext.m_nDrawMode = GraphicType_LINE == m_DrawContext.m_nDrawMode ? EQMDRAW_MOVE : GraphicType_LINE;

	m_DrawContext.SetLineWidth(2);
	m_DrawContext.SetLineColor(0);
}


void CMainFrame::OnRectangle()
{
	// TODO: Add your command handler code here
	m_DrawContext.m_nDrawMode = GraphicType_RECTANGLE == m_DrawContext.m_nDrawMode ? EQMDRAW_MOVE : GraphicType_RECTANGLE;

	m_DrawContext.SetLineWidth(2);
	m_DrawContext.SetLineColor(0);
}

void CMainFrame::OnLeftalign()
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.LeftAlign(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateLeftalign(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI->Enable( NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode));
}

void CMainFrame::OnRightalign()
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.RightAlign(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateRightalign(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable( NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode));
	
	pCmdUI->Enable( NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode)
		);
}

void CMainFrame::OnTopalign()
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.TopAlign(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateTopalign(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI->Enable( NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode)
		);
}

void CMainFrame::OnDownalign()
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.BottomAlign(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateDownalign(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	/*BOOL ifupdate =true,iffind = false;
	for(int i=0;i<m_DrawContext.vecSelectEqmt.size();i++)
	{
		if(
			m_DrawContext.vecSelectEqmt[i]->GetEqmType()!=EQMDRAW_STRAIGHTPIPE
			)
			 iffind = true;
		else
			iffind = false;
		ifupdate=ifupdate&&iffind;
	}*/
	pCmdUI->Enable( NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode)
		);
}

void CMainFrame::OnHorizonalign()
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.CrossAlign(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateHorizonalign(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI->Enable( NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode)
		);
}

void CMainFrame::OnVerticalalign()
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.DownAlign(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateVerticalalign(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI->Enable( NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode)
		);
}

void CMainFrame::OnMakeSameWidth()
{
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.MakeSameWidth(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateMakeSameWidth(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode));
}

void CMainFrame::OnMakeSameHeight()
{
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.MakeSameHeight(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateMakeSameHeight(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode));
}

void CMainFrame::OnMakesamesize()
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.MakeSameSize(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateMakesamesize(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI->Enable( NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode)
		);
}

void CMainFrame::OnMetaBrush()
{
	m_DrawContext.m_nDrawMode = EQMDRAW_FORMAT_BRUSH;

	int nSelectCount = m_DrawContext.vecSelectEqmt.size();
	if (0 == nSelectCount)
	{
		MessageBox(_T("请先选择所要复制的图元！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		m_pFormatBrush = NULL;
		m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		return;
	}
	else if (nSelectCount > 1)
	{
		MessageBox(_T("格式刷只能有1个源图元！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		m_pFormatBrush = NULL;
		m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		return;
	}

	try
	{
		m_pFormatBrush = m_DrawContext.vecSelectEqmt.at(0);
	}
	catch (...)
	{
		m_pFormatBrush = NULL;
		return;
	}

	EQMDRAWTYPE drawType = m_pFormatBrush->GetEqmType();
	if (EQMDRAW_TEXT != drawType && EQMDRAW_METAFILE != drawType)
	{
		MessageBox(_T("格式刷目前只支持文字、自定义图元！"), _T("错误"), MB_OK|MB_ICONERROR);
		m_pFormatBrush = NULL;
		m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		return;
	}
}

void CMainFrame::OnUpdateMetaBrush(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode));
}

void CMainFrame::OnGrids()
{
	bool bFlag = m_DrawProxy.GetChooseGrid();
	m_DrawProxy.SetChooseGrid(!bFlag);
	m_DrawProxy.SetRectChanged(true);
	m_DrawProxy.init(&((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas, NULL);
	m_DrawProxy.draw(&m_DrawContext);
}

void CMainFrame::OnUpdateGrids(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode));
}


/*
 *  函数说明：工具条"点值监控"启动服务函数
 *	输入参数：
 *	输出参数：
 *	返回值：
*/
void CMainFrame::OnShowPointValueDlg()
{
	CMainFrame*		m_pMainFrame;
	CUPPCServerDataAccess* m_pDataAccess;

	m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pDataAccess = NULL;

	CBEOPDesignerView *pViewForPointValueDLg = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
	if(pViewForPointValueDLg && pViewForPointValueDLg->m_PageDlg->m_pDataAccess == NULL)
	{	
		if(m_pDataAccess && m_pDataAccess->IsMySQLConnected())
		{
			pViewForPointValueDLg->m_PageDlg->m_pDataAccess = m_pDataAccess;
			pViewForPointValueDLg->m_PageDlg->InitData();
			return ;
		}
		else		//重新连接数据库
		{
			CString strPath;
			Project::Tools::GetSysPath(strPath);
			strPath += _T("\\debugtool.ini");

			TCHAR szBuf[MAX_PATH] = {0};
			GetPrivateProfileString(_T("beopdebugtool"), _T("server"), _T("127.0.0.1"), szBuf, MAX_PATH, strPath);

			CString strDbIp;
			strDbIp.Format(_T("%s"), szBuf);

			CConnectDlg dlg(szBuf);
			if (IDOK == dlg.DoModal())
			{
				USES_CONVERSION;
				string strIp;
				strIp = T2A(dlg.m_strDbIp.GetBuffer());
				dlg.m_strDbIp.ReleaseBuffer();

				gDataBaseParam realDBParam;
				realDBParam.strDBIP = strIp;
				realDBParam.strDBName = "";
				realDBParam.strRealTimeDBName = Project::Tools::WideCharToUtf8(m_pMainFrame->GetMySQLDatabaseName());
				realDBParam.strUserName = "root";
				realDBParam.nPort = 3306;
				realDBParam.strDBIP2 = "";

				m_pDataAccess = new CUPPCServerDataAccess();
				m_pDataAccess->SetDBparam(realDBParam);
				if(!(m_pDataAccess->InitConnection()))
				{
					return;
				}
				pViewForPointValueDLg->m_PageDlg->m_pDataAccess = m_pDataAccess;
				pViewForPointValueDLg->m_PageDlg->InitData();
			}
		}
	}
	pViewForPointValueDLg->m_PageDlg->m_pPanePoint->ShowWindow(SW_SHOW);
}
/*
 *  函数说明：工具条"点值监控"启动服务函数
 *	输入参数：
 *	输出参数：
 *	返回值：
*/
void CMainFrame::OnUpdateShowPointValueDlg(CCmdUI *pCmdUI)
{
	//pCmdUI->SetCheck(m_wndFileView.IsWindowVisible());
	pCmdUI->Enable(m_ProjectOpen);
}
void CMainFrame::OnMakeBend()
{
	CBendConfigDlg dlg(m_nBendSize);
	if (IDOK == dlg.DoModal())
	{
		m_nBendSize = dlg.m_nBendSize;
	}

	// 构建点矩阵
	int nWidth = 0;
	int nHeight = 0;
	if (FULLSCREEN == m_DrawContext.pPage->GetPageType())
	{
		nWidth = m_DrawContext.GetScreenWidth();
		nHeight = m_DrawContext.GetScreenHeight();
	}
	else
	{
		nWidth = m_DrawContext.pPage->GetPageWidth();
		nHeight = m_DrawContext.pPage->GetPageHeight();
	}

	int** pMatrix = new int*[nWidth];
	memset(pMatrix, 0, nWidth*sizeof(int));
	int nHeSize = nHeight * sizeof(int);
	for (int i=0; i<nWidth; i++)
	{
		pMatrix[i] = new int[nHeight];
		memset(pMatrix[i], 0, nHeSize);
	}


	// 将管道点填充到矩阵中
	list<CEqmDrawPipe*> listPipe(m_DrawContext.pPage->m_pipelist);
	list<CEqmDrawPipe*>::const_iterator itPipe;
	vector<Gdiplus::PointF> vecPoint;
	vector<Gdiplus::PointF>::const_iterator itPt;
	CPoint ptBegin;
	CPoint ptEnd;
	int nFirst = 0;
	int nLast = 0;
	for (itPipe=listPipe.begin(); itPipe!=listPipe.end(); itPipe++)
	{
		vecPoint = (*itPipe)->GetPointList();
		itPt = vecPoint.begin();
		ptBegin.x = (*itPt).X;
		ptBegin.y = (*itPt).Y;

		itPt = vecPoint.end();
		itPt--;
		ptEnd.x = (*itPt).X;
		ptEnd.y = (*itPt).Y;

		if (ptBegin.x == ptEnd.x)
		{	// 纵向管道
			int nX = ptBegin.x;
			if (ptBegin.y < ptEnd.y)
			{
				nFirst = ptBegin.y;
				nLast = ptEnd.y;
			}
			else
			{
				nFirst = ptEnd.y;
				nLast = ptBegin.y;
			}
			for (int i=nFirst; i<=nLast; i+=CELLINTERVAL)
			{
				pMatrix[nX][i] += 1;
			}
		}
		else if (ptBegin.y == ptEnd.y)
		{	// 横向管道
			int nY = ptBegin.y;
			if (ptBegin.x < ptEnd.x)
			{
				nFirst = ptBegin.x;
				nLast = ptEnd.x;
			}
			else
			{
				nFirst = ptEnd.x;
				nLast = ptBegin.x;
			}
			for (int i=nFirst; i<=nLast; i+=CELLINTERVAL)
			{
				pMatrix[i][nY] += 1;
			}
		}
		else
		{
			// error
		}
	}


	// 计算出管道交点，判断弯头类型
	BendType type = bendUnknown;
	BendInfo bendInf;
	m_vecBendInfo.clear();
	
	for (int y=0; y<nHeight; y+=CELLINTERVAL)
	{
		for (int x=0; x<nWidth; x+=CELLINTERVAL)
		{
			if (2 == pMatrix[x][y])
			{
				bendInf.point.x = x;
				bendInf.point.y = y;

				int nUp = 0;
				int nRight = 0;
				int nDown = 0;
				int nLeft = 0;

				if (y < CELLINTERVAL)
				{
					nUp = -1;
				}
				else
				{
					nUp = pMatrix[x][y-CELLINTERVAL];
				}

				if (x > nWidth - CELLINTERVAL)
				{
					nRight = -1;
				} 
				else
				{
					nRight = pMatrix[x+CELLINTERVAL][y];
				}

				if (y > nHeight - CELLINTERVAL)
				{
					nDown = -1;
				} 
				else
				{
					nDown = pMatrix[x][y+CELLINTERVAL];
				}

				if (x < CELLINTERVAL)
				{
					nLeft = -1;
				} 
				else
				{
					nLeft = pMatrix[x-CELLINTERVAL][y];
				}

				if (1 == nUp && 1 == nRight && 0 == nDown && 1 == nLeft)
				{
					bendInf.type = bendThreeUp;
				}
				else if (1 == nUp && 1 == nRight && 1 == nDown && 0 == nLeft)
				{
					bendInf.type = bendThreeRight;
				}
				else if (0 == nUp && 1 == nRight && 1 == nDown && 1 == nLeft)
				{
					bendInf.type = bendThreeDown;
				}
				else if (1 == nUp && 0 == nRight && 1 == nDown && 1 == nLeft)
				{
					bendInf.type = bendThreeLeft;
				}
				else if (1 == nUp && 1 == nRight && 1 == nDown && 1 == nLeft)
				{
					bendInf.type = bendCross;
				}
				else if (0 == nUp && 1 == nRight && 1 == nDown && 0 == nLeft)
				{
					bendInf.type = bendTwoLeftUp;
				}
				else if (0 == nUp && 0 == nRight && 1 == nDown && 1 == nLeft)
				{
					bendInf.type = bendTwoRightUp;
				}
				else if (1 == nUp && 0 == nRight && 0 == nDown && 1 == nLeft)
				{
					bendInf.type = bendTwoRightDown;
				}
				else if (1 == nUp && 1 == nRight && 0 == nDown && 0 == nLeft)
				{
					bendInf.type = bendTwoLeftDown;
				}

				m_vecBendInfo.push_back(bendInf);
			}
		}
	}

	for (int i=0; i<nWidth; i++)
	{
		SAFE_DELETE_ARRAY(pMatrix[i]);
	}
	SAFE_DELETE_ARRAY(pMatrix);


	// 清空选择项
	vector<CEqmDrawBase*>::const_iterator itBase;
	for (itBase=m_DrawContext.vecSelectEqmt.begin(); itBase!=m_DrawContext.vecSelectEqmt.end(); ++itBase)
	{
		(*itBase)->SetSelected(false);
		(*itBase)->SetMainAlign(false);
	}
	m_DrawContext.vecSelectEqmt.clear();


	// 三通弯头贴图
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		return;
	}

	vector<BendInfo>::const_iterator itBend;
	CPoint ptPos;
	int nPicId = 0;
	int nUseId = 0;
	bool bTypeFlag = false;
	bool bFresh = false;
	for (itBend=m_vecBendInfo.begin(); itBend!=m_vecBendInfo.end(); itBend++)
	{
		bTypeFlag = false;
		switch (itBend->type)
		{
		case bendThreeUp:
			nPicId = 18000180;
			ptPos = itBend->point;
			ptPos.x -= 25;
			ptPos.y -= 24;
			break;
		case bendThreeRight:
			nPicId = 18200182;
			ptPos = itBend->point;
			ptPos.x -= 14;
			ptPos.y -= 25;
			break;
		case bendThreeDown:
			nPicId = 17900179;
			ptPos = itBend->point;
			ptPos.x -= 25;
			ptPos.y -= 14;
			break;
		case bendThreeLeft:
			nPicId = 18100181;
			ptPos = itBend->point;
			ptPos.x -= 24;
			ptPos.y -= 25;
			break;
		case bendTwoLeftUp:
			nPicId = 18700187;
			ptPos = itBend->point;
			ptPos.x -= 17;
			ptPos.y -= 16;
			break;
		case bendTwoRightUp:
			nPicId = 18500185;
			ptPos = itBend->point;
			ptPos.x -= 24;
			ptPos.y -= 16;
			break;
		case bendTwoRightDown:
			nPicId = 18600186;
			ptPos = itBend->point;
			ptPos.x -= 24;
			ptPos.y -= 24;
			break;
		case bendTwoLeftDown:
			nPicId = 18800188;
			ptPos = itBend->point;
			ptPos.x -= 17;
			ptPos.y -= 24;
			break;
		case bendUnknown:
		case bendCross:
		default:
			bTypeFlag = true;
			break;
		}
		if (bTypeFlag)
		{
			continue;
		}
		bFresh = true;
		nUseId = 0;
		(pView->m_PageDlg->m_dlgCanvas).InsertImageIntoOpenS3db(nPicId, nUseId);
		(pView->m_PageDlg->m_dlgCanvas).CreateBendCtrl(ptPos, itBend->type, nUseId, m_nBendSize);
	}
	if (bFresh)
	{
		CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt, E_COMMAND_ADD);
		(pView->m_PageDlg->m_dlgCanvas).Invalidate(FALSE);
	}
}

void CMainFrame::OnUpdateMakeBend(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(NULL != m_DrawContext.pPage && (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode));
}

void CMainFrame::OnRightrotate()
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.RightRotate90(m_DrawContext.vecSelectEqmt[0]);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateRightrotate(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(NULL != m_DrawContext.pPage 
		&& (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode) && m_DrawContext.vecSelectEqmt.size() == 1
		&& (m_DrawContext.vecSelectEqmt[0]->GetEqmType() == EQMDRAW_METAFILE
			|| m_DrawContext.vecSelectEqmt[0]->GetEqmType() == EQMDRAW_BEND
			|| m_DrawContext.vecSelectEqmt[0]->GetEqmType() == EQMDRAW_AREA_CHANGE)
		);
}

void CMainFrame::OnLeftrotate()
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	SaveDeviceProperty();
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.LeftRotate90(m_DrawContext.vecSelectEqmt[0]);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateLeftrotate(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(NULL != m_DrawContext.pPage 
		&& (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode) && m_DrawContext.vecSelectEqmt.size() == 1
		&& (m_DrawContext.vecSelectEqmt[0]->GetEqmType() == EQMDRAW_METAFILE
			|| m_DrawContext.vecSelectEqmt[0]->GetEqmType() == EQMDRAW_BEND
			|| m_DrawContext.vecSelectEqmt[0]->GetEqmType() == EQMDRAW_AREA_CHANGE)
		);
}

void CMainFrame::OnEditCopy()
{
	// TODO: Add your command handler code here
	if(EQMDRAW_MOVE != m_DrawContext.m_nDrawMode )
		return;
	m_DrawContext.mVecSelectEqmtCopied = m_DrawContext.vecSelectEqmt;
	CommandManager::Instance()->CallCommand(m_DrawContext.mVecSelectEqmtCopied,E_COMMAND_COPY);
	m_nPasteXCount = 1;
	m_nPasteYCount = 1;
	m_bFirstPaste = true;

	m_nPageLuX = m_DrawContext.pPage->GetPagexPosition();
	m_nPageLuY = m_DrawContext.pPage->GetPageyPosition();
	m_nPageRdX = m_nPageLuX + m_DrawContext.pPage->GetPageWidth();
	m_nPageRdY = m_nPageLuY + m_DrawContext.pPage->GetPageHeight();
	vector<CEqmDrawBase*>::const_iterator	iter;

	// 计算选择图元的整体边界
	for (iter=m_DrawContext.mVecSelectEqmtCopied.begin(); iter!=m_DrawContext.mVecSelectEqmtCopied.end(); ++iter)
	{
		if (iter == m_DrawContext.mVecSelectEqmtCopied.begin())
		{
			m_nSelLuX = (*iter)->GetPos().X;
			m_nSelLuY = (*iter)->GetPos().Y;
			m_nSelRdX = (*iter)->GetPos().X + (*iter)->GetWidth();
			m_nSelRdY = (*iter)->GetPos().Y + (*iter)->GetHeight();
		}
		else
		{
			if ((*iter)->GetPos().X < m_nSelLuX)
			{
				m_nSelLuX = (*iter)->GetPos().X;
			}
			if ((*iter)->GetPos().Y < m_nSelLuY)
			{
				m_nSelLuY = (*iter)->GetPos().Y;
			}
			if ((*iter)->GetPos().X + (*iter)->GetWidth() > m_nSelRdX)
			{
				m_nSelRdX = (*iter)->GetPos().X + (*iter)->GetWidth();
			}
			if ((*iter)->GetPos().Y + (*iter)->GetHeight() > m_nSelRdY)
			{
				m_nSelRdY = (*iter)->GetPos().Y + (*iter)->GetHeight();
			}
		}
	}

	// 计算偏移量
	if (m_nSelRdX + m_nPasteOffset > m_nPageRdX)
	{
		m_nPasteXCount = -((m_nSelLuX - m_nPageLuX) / m_nPasteOffset);
	}
	if (m_nSelRdY + m_nPasteOffset > m_nPageRdY)
	{
		m_nPasteYCount = -((m_nSelLuY - m_nPageLuY) / m_nPasteOffset);
	}
}

void CMainFrame::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI->Enable(NULL != m_DrawContext.pPage 
		&& (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode) && m_DrawContext.vecSelectEqmt.size() > 0);
}

void CMainFrame::OnEditPaste()
{
	// TODO: Add your command handler code here
	const UINT copiedNum = (UINT)m_DrawContext.mVecSelectEqmtCopied.size();
	if(copiedNum)
	{
		m_DrawContext.ClearSelect();
	}

	int nPosX = m_nSelRdX + m_nPasteXCount * m_nPasteOffset;
	int nPosY = m_nSelRdY + m_nPasteYCount * m_nPasteOffset;
	if (nPosX > m_nPageRdX)
	{
		m_nPasteXCount = -((m_nSelLuX - m_nPageLuX) / m_nPasteOffset);
	}
	if (nPosY > m_nPageRdY)
	{
		m_nPasteYCount = -((m_nSelLuY - m_nPageLuY) / m_nPasteOffset);
	}

	vector<CEqmDrawBase*>::const_iterator	iter;
	for (iter=m_DrawContext.mVecSelectEqmtCopied.begin(); iter!=m_DrawContext.mVecSelectEqmtCopied.end(); ++iter)
	{
		CEqmDrawBase* pBase = *iter;
		if(!pBase) continue;

		CEqmDrawBase* pDest = pBase->Copy();   
		if(!pDest) continue;
		const int nID = pDest->GetPageID();
		pDest->SetID(++m_MaxEqmtID);
		pDest->SetSelected(true); //if selected ,draw bugs.

		const int pageid = m_DrawContext.pPage->GetID();
		pDest->SetPageID(pageid);

		Gdiplus::PointF pt = pDest->GetPos();
		if (nID != pageid)
		{	// 跨页粘贴
			if (m_bFirstPaste)
			{
				// 第一次跨页粘贴，粘贴相同坐标
			}
			else
			{
				pt.X += m_nPasteOffset * (m_nPasteXCount - 1);
				pt.Y += m_nPasteOffset * (m_nPasteYCount - 1);
			}
		}
		else
		{	// 同页粘贴
			pt.X += m_nPasteOffset * m_nPasteXCount;
			pt.Y += m_nPasteOffset * m_nPasteYCount;
		}

		pDest->SetPos(pt);
		m_DrawContext.pPage->AddBase(pDest);
		m_DrawContext.vecSelectEqmt.push_back(pDest);
	}
	m_bFirstPaste = false;
	m_nPasteXCount++;
	m_nPasteYCount++;
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_PASTE);
	SaveDeviceProperty();

	m_DrawContext.type = FLASHSCREEN;
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(NULL != m_DrawContext.pPage 
		&& m_DrawContext.mVecSelectEqmtCopied.size() > 0);
}

void CMainFrame::OnFormatbrush()
{
	// TODO: Add your command handler code here
	CommandManager::Instance()->CallCommand(m_DrawContext.vecSelectEqmt,E_COMMAND_EDIT);
	m_DrawProxy.FormatBrush(m_DrawContext.vecSelectEqmt);
	((CBEOPDesignerView*)GetActiveView())->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMainFrame::OnUpdateFormatbrush(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(NULL != m_DrawContext.pPage
		&& (EQMDRAW_MOVE == m_DrawContext.m_nDrawMode) && (m_DrawContext.vecSelectEqmt.size() >1) );
}

void CMainFrame::OnOpcpoint()
{
	// TODO: Add your command handler code here
	if (m_pPointMapDlg == NULL)
	{
		m_pPointMapDlg = new CDataPointConfigDlg(m_plcpointmap);//创建一个对象
		m_pPointMapDlg->Create(IDD_DATA_POINT_CONFIG, this);//加载出点表管理的对话框
	}
	
	m_pPrjSqlite->LoadProjectSqlite_opcid3(); //reload point table//重新从数据库加载了m_plcPointmap导致其他地方添加的点都会被冲掉
	m_pPointMapDlg->SetPointMap(m_plcpointmap);
	m_pPointMapDlg->MakeAllList();
	m_pPointMapDlg->ShowList(m_pPointMapDlg->m_allentrylist);
	m_pPointMapDlg->RefreshIgnorePointList();
	m_pPointMapDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateOpcpoint(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}

void CMainFrame::OnClose()
{
	if (m_bLoadingPage)
	{
		MessageBox(_T("页面正在加载，当前不可退出！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		return;
	}

	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	ASSERT(pView);

	if (GetSaveFlag())
	{
		const int nRet = MessageBox(L"是否需要保存？",L"提示",MB_YESNOCANCEL);
		if (IDYES == nRet)
		{	// do save
			SaveProjectContain();
		}
		else if (IDNO == nRet)
		{
			// no save
		}
		else if (IDCANCEL == nRet)
		{
			// cancel
			return;
		}
		else
		{	// unknown
			return;
		}
	}
	SetComboSel(5);
	pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);
	m_project.ClearProject();
	m_plcpointmap.Clear();
	if (m_pPointMapDlg)
	{
		m_pPointMapDlg->ClearListAll();
	}
	ResetMarks();	
	SetWindowText(m_AppName);
	ShowCurrentPage();
	m_bIsCreateProject = FALSE;
	m_selectEuVect.clear();
	EmptyClipboard();
	CFrameWndEx::OnClose();
}

BOOL CMainFrame::OnToolTipNotify( UINT id, NMHDR *pNMHDR,LRESULT *pResult )
{
		if ((pNMHDR == NULL) || (pResult == NULL)){
		return FALSE;
	}

	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString tiptext(_T(""));
	UINT_PTR nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)(nID));
	}
	
	// will not be an seperator.
	if (nID != 0)
	{
		if (!tiptext.LoadString((UINT)nID))
		{
			return FALSE;
		}
	}

	_tcsncpy_s(pTTTW->szText, tiptext.GetString(), _countof(pTTTW->szText));
	*pResult = 0;
	return TRUE;    // message was handled
}

void CMainFrame::OnAddDbTableField()
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnUpdateAddDbTableField(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::SetComboSel( int cursel )
{
	//m_wndToolBar.m_ComboBox.SetCurSel(cursel);
	
	CMFCToolBarComboBoxButton *pStcCombo = CMFCToolBarComboBoxButton::GetByCmd(ID_PAGE_PERCENT,FALSE);
	if(pStcCombo)
	{
		pStcCombo->SelectItem(cursel);
	}
}

int CMainFrame::GetComboSel()
{
	//return m_wndToolBar.m_ComboBox.GetCurSel();
	
	CMFCToolBarComboBoxButton *pStcCombo = CMFCToolBarComboBoxButton::GetByCmd(ID_PAGE_PERCENT,FALSE);
	if(pStcCombo)
	{
		return pStcCombo->GetCurSel();
	}
	return 0;
}

int CMainFrame::GetComboSelSize()
{
	//return m_wndToolBar.m_ComboBox.GetCount();
	
	CMFCToolBarComboBoxButton *pStcCombo = CMFCToolBarComboBoxButton::GetByCmd(ID_PAGE_PERCENT,FALSE);
	if(pStcCombo)
	{
		return pStcCombo->GetCount();
	}
	return 0;
}

void CMainFrame::OnComboSelChange()
{
	/*int cursel = m_wndToolBar.m_ComboBox.GetCurSel();
	double newsel = 0.5 + cursel*0.1;
	m_DrawContext.m_fScreenScale = newsel;


	CBEOPDesignerView* pview = (CBEOPDesignerView*)GetActiveView();

	
	pview->m_PageDlg->m_rtCanvas.bottom = m_DrawContext.GetScaledheight();
	pview->m_PageDlg->m_rtCanvas.right  = m_DrawContext.GetScaledWidth();

	pview->m_PageDlg->PostMessage(WM_SIZE);*/
}

void CMainFrame::OnUpdateComboZoom( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	m_topMenu.DrawItem(lpDrawItemStruct);
	CFrameWndEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	m_topMenu.MeasureItem(lpMeasureItemStruct);
	CFrameWndEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CMainFrame::OnCircle()
{
	// TODO: Add your command handler code here
	m_DrawContext.m_nDrawMode = GraphicType_ELLIPSE == m_DrawContext.m_nDrawMode ? EQMDRAW_MOVE : GraphicType_ELLIPSE;
}

void CMainFrame::OnUpdateCircle(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(GraphicType_ELLIPSE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}


void CMainFrame::OnUpdateZoomout(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnUpdateZoomin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

//////////////////////////////////////////////////////////////////////////
void CMainFrame::SaveDeviceProperty()
{
	if (m_DrawContext.vecSelectEqmt.empty())
	{
		return;
	}
	m_selectEuVect.clear();
	m_selectEuVect = m_DrawContext.vecSelectEqmt;
	for ( int i = 0; i < (int)m_selectEuVect.size(); i++ )
	{
		CEqmDrawBase* pBase = m_selectEuVect[i];
		if(!pBase) continue;
		CEqmDrawBase* pDest = pBase->Copy();
		//@todo
	}
}

void CMainFrame::OnFullscreen()
{
	// TODO: 在此添加命令处理程序代码
	GetWindowPlacement(&m_savewindowplacement);   
	
	CRect WindowRect;
	GetWindowRect(&WindowRect);
	CRect ClientRect;   
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect);

	ClientToScreen(&ClientRect);
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	
	m_fullscreen_rect.left = WindowRect.left - ClientRect.left;   
	m_fullscreen_rect.top = WindowRect.top - ClientRect.top;
	m_fullscreen_rect.right = WindowRect.right - ClientRect.right + nFullWidth;
	m_fullscreen_rect.bottom = WindowRect.bottom - ClientRect.bottom + nFullHeight;


	m_bFullScreen=TRUE; // 设置全屏显示标志为 TRUE   
	//进入全屏显示状态   
	WINDOWPLACEMENT wndpl;   
	wndpl.length = sizeof(WINDOWPLACEMENT);   
	wndpl.flags = 0;   
	wndpl.showCmd = SW_SHOWNORMAL;   
	wndpl.rcNormalPosition = m_fullscreen_rect;   
	SetWindowPlacement(&wndpl);
}

void CMainFrame::SetFullScreen( bool bfullscreen )
{
	m_bFullScreen = bfullscreen;
}

bool CMainFrame::IsFullScreen() const
{
	return m_bFullScreen;
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bFullScreen){
		lpMMI -> ptMaxSize.x = m_fullscreen_rect.Width();
		lpMMI -> ptMaxSize.y = m_fullscreen_rect.Height();   
		lpMMI -> ptMaxPosition.x = m_fullscreen_rect.Width();   
		lpMMI -> ptMaxPosition.y = m_fullscreen_rect.Height();   
		//最大Track尺寸也要改变   
		lpMMI -> ptMaxTrackSize.x = m_fullscreen_rect.Width();   
		lpMMI -> ptMaxTrackSize.y = m_fullscreen_rect.Height();   
	}

	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::EndFullScreen()
{
	//ShowWindow(SW_HIDE); 
	SetFullScreen(false);
	SetWindowPlacement(&m_savewindowplacement);
	ShowWindow(SW_MAXIMIZE);
}

void CMainFrame::OnLayerManage()
{
	// TODO: 在此添加命令处理程序代码
	m_pLayerManageDlg->ShowWindow(SW_SHOW);
}


void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nID == SC_RESTORE)
	{
		if (m_pLayerManageDlg && m_pLayerManageDlg->m_bShow)
		{
			m_pLayerManageDlg->ShowWindow(SW_SHOW);
		}
	}
	CFrameWndEx::OnSysCommand(nID, lParam);
}

CDataPointManager& CMainFrame::GetPointMap()
{
	return m_plcpointmap;
}



void CMainFrame::OnDestroy()
{
	CFrameWndEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CMainFrame::OnUpdateText( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_TEXT == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnUpdateChiller( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_CHILLER == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnUpdatePump( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_PUMP == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnUpdateValve( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_VALVE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnCoolingTower()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_COOLINGTOWER != m_DrawContext.m_nDrawMode) ? EQMDRAW_COOLINGTOWER : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateCoolingTower( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_COOLINGTOWER == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnMeta()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_METAFILE != m_DrawContext.m_nDrawMode) ? EQMDRAW_METAFILE : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateMeta( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_METAFILE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

CString CMainFrame::GetDBFilePath()
{
	return m_strFilePath;
}

void CMainFrame::OnMetaButton()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_METABUTTON != m_DrawContext.m_nDrawMode) ? EQMDRAW_METABUTTON : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateMetaButton( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_METABUTTON == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

int CMainFrame::SetOnePage()
{
	int nPageID = 0;

	vector<CEqmDrawPage*> vec = m_project.GetPageList();
	const size_t size = vec.size();
	if (size> 0)
	{
		for (size_t i=0;i<size;++i)
		{
			if (vec[i])
			{
				m_DrawContext.pPage = vec[i];
				nPageID = m_DrawContext.pPage->GetID();
			}
		}
	}
	else
	{
		CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
		ASSERT(pView);
		m_DrawContext.pPage = NULL;
		pView->m_PageDlg->m_dlgCanvas.ShowWindow(SW_HIDE);

		CString strAppTitle;
		strAppTitle.Format(_T("%s - %s"), m_AppName, m_project.GetProjectName().c_str());
		SetWindowText(strAppTitle.GetString() );

		nPageID = -1;
	}
	ShowCurrentPage();
	return nPageID;
}

void CMainFrame::OnBend()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_BEND != m_DrawContext.m_nDrawMode) ? EQMDRAW_BEND : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateBend( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_BEND == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnLineChart()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_LINE_CHART != m_DrawContext.m_nDrawMode) ? EQMDRAW_LINE_CHART : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateLineChart( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_LINE_CHART == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnBarChart()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_BAR_CHART != m_DrawContext.m_nDrawMode) ? EQMDRAW_BAR_CHART : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateBarChart( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_BAR_CHART == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnPieChart()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_PIE_CHART != m_DrawContext.m_nDrawMode) ? EQMDRAW_PIE_CHART : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdatePieChart( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_PIE_CHART == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnDashBoard()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_DASHBOARD != m_DrawContext.m_nDrawMode) ? EQMDRAW_DASHBOARD : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateDashBoard( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_DASHBOARD == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}


void CMainFrame::OnTimeSelect()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_TIME_SELECT != m_DrawContext.m_nDrawMode) ? EQMDRAW_TIME_SELECT : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateTimeSelect( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_TIME_SELECT == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnTimeBar()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_TIME_BAR != m_DrawContext.m_nDrawMode) ? EQMDRAW_TIME_BAR : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateTimeBar( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_TIME_BAR == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnProgressBar()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_PROGRESS_BAR != m_DrawContext.m_nDrawMode) ? EQMDRAW_PROGRESS_BAR : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateProgressBar( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_PROGRESS_BAR == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}


void CMainFrame::OnJiami()
{
	CString cstrFileSufixOption;
	cstrFileSufixOption.Format(_T("*.%s|*.%s|") , c_wstr_default_file_sufix.c_str(), c_wstr_default_file_sufix.c_str() );
	CFileDialog dlg(TRUE, L"project", L"project", 0, cstrFileSufixOption, NULL);
	dlg.m_ofn.lpstrTitle = _T("加密文件");
	CString cstr;
	if ( IDOK == dlg.DoModal() )
	{	
		cstr =  dlg.GetPathName();
		if (m_strFilePath == cstr)
		{
			AfxMessageBox(L"文件已经被打开，请关闭当前项目再试!");
			return;
		}
		if (NULL == m_pPrjSqlite)
		{
			return;
		}
		CSqliteAcess access(cstr); // keep
		access.Encrypt();
	}
}


void CMainFrame::OnJiemi()
{
	CString cstrFileSufixOption;
	cstrFileSufixOption.Format(_T("*.%s|*.%s|") , c_wstr_default_file_sufix.c_str(), c_wstr_default_file_sufix.c_str() );
	CFileDialog dlg(TRUE, L"project", L"project", 0, cstrFileSufixOption, NULL);
	dlg.m_ofn.lpstrTitle = _T("解密文件");
	CString cstr;
	if ( IDOK == dlg.DoModal() )
	{	
		cstr =  dlg.GetPathName();
		if (m_strFilePath == cstr)
		{
			AfxMessageBox(L"文件已经被打开，请关闭当前项目再试!");
			return;
		}
		if (NULL == m_pPrjSqlite)
		{
			return;
		}
		CSqliteAcess access(cstr); // keep
		access.Decode();
	}
}

void CMainFrame::OnWebEncrypt()
{
	TCHAR szFilters[]= _T("dec Files (*.dec)|*.dec|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("dec"), _T("*.dec"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilters);
	dlg.m_ofn.lpstrTitle = _T("加密文件");
	if (IDOK == dlg.DoModal())
	{
		CString strSelPathName =  dlg.GetPathName();
		if (m_strFilePath == strSelPathName)
		{
			MessageBox(_T("文件已经被打开，请关闭当前项目再试！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}

		CString strNewPathName(strSelPathName);
		int nLen = strNewPathName.GetLength();
		if (_T(".dec") == strNewPathName.Mid(nLen - 4, 4))
		{
			strNewPathName = strNewPathName.Left(nLen - 4);
		}
		if (::CopyFile(strSelPathName, strNewPathName, FALSE))
		{
			if (NULL == m_pPrjSqlite)
			{
				return;
			}
			CSqliteAcess access(strNewPathName); // keep
			if (SQLITE_OK == access.Encrypt())
			{
				MessageBox(_T("加密完成！"), _T("成功"), MB_OK|MB_ICONASTERISK);
			}
		}
	}
}


void CMainFrame::OnWebDescrypt()
{
	if(m_strFilePath.GetLength()<=0)
	{
		MessageBox(_T("请先打开一个项目文件！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	int nXX = m_strFilePath.Find(L".s3db");
	if(nXX<0)
	{
		MessageBox(_T("本项目文件不是旧版文件，不能导出为4db！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}
	
	
	nXX = m_strFilePath.ReverseFind('.');



	CString strNewPathName(m_strFilePath.Left(nXX) + _T(".4db"));
	if (::CopyFile(m_strFilePath, strNewPathName, FALSE))
	{
		if (NULL == m_pPrjSqlite)
		{
			return;
		}
		CSqliteAcess access(strNewPathName); // keep
		if (SQLITE_OK == access.Decode())
		{
			CString strTemp;
			strTemp.Format(_T("导出到%s完成！"), strNewPathName);
			MessageBox(strTemp.GetString(), _T("成功"), MB_OK|MB_ICONASTERISK);
		}
	}
	
}

void CMainFrame::OnDiagnoseChart()
{
	return;
	//m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_DIAGNOSE_CHART != m_DrawContext.m_nDrawMode) ? EQMDRAW_DIAGNOSE_CHART : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateDiagnoseChart( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(FALSE);
	//pCmdUI->SetCheck(EQMDRAW_DIAGNOSE_CHART == m_DrawContext.m_nDrawMode);
	//pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnBarPolygon()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_POLYGON != m_DrawContext.m_nDrawMode) ? EQMDRAW_POLYGON : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateBarPolygon( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_POLYGON == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

CString CMainFrame::GetConfigVersion()
{
	if (NULL == m_pPrjSqlite)
	{
		return _T("");
	}
	CSqliteAcess access(m_strDBFileName.c_str(), m_pPrjSqlite->GetFileEncryptType());

	ostringstream sqlstream;
	sqlstream << "select * from project_config;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	CString version = _T("");
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if(access.getColumn_Text(4) != NULL)
		{
			string   version_temp(const_cast<char*>(access.getColumn_Text(4)));
			CString  ver(UtilString::ConvertMultiByteToWideChar(version_temp).c_str());
			version = ver;
		}
	}
	access.SqlFinalize();
	access.CloseDataBase();
	return version;
}


DataBaseSetting CMainFrame::LoadConfig()
{
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	DataBaseSetting dbset;
	if (NULL == m_pPrjSqlite)
	{
		return dbset;
	}

	CSqliteAcess access(pFrame->m_strDBFileName.c_str(), m_pPrjSqlite->GetFileEncryptType());
	//m_project.ClearProject();
	ostringstream sqlstream;
	sqlstream << "select * from project_config;";
	string sql_ = sqlstream.str();
	//char *ex_sql = const_cast<char*>();
	int re = 0;
	re = access.SqlQuery(sql_.c_str());

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		int iProjectID = access.getColumn_Int(0);
		if(access.getColumn_Text(1) != NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			wstring   name = UtilString::ConvertMultiByteToWideChar(name_temp);
		}
		int ScreenWidth = access.getColumn_Int(2);
		int ScreenHeight = access.getColumn_Int(3);
		int curVersion =   access.getColumn_Int(4);
		if(access.getColumn_Text(5) != NULL)
		{
			string   xupTime_temp(const_cast<char*>(access.getColumn_Text(5)));
			wstring   xupTime = UtilString::ConvertMultiByteToWideChar(xupTime_temp);
		}
		if(access.getColumn_Text(6) != NULL)
		{
			string  dbip(const_cast<char*>(access.getColumn_Text(6)));
			dbset.strDBIP = dbip;
		}
		if(access.getColumn_Text(7) != NULL)
		{
			string  dbname(const_cast<char*>(access.getColumn_Text(7)));
			dbset.strDBName = dbname;
		}
		if(access.getColumn_Text(8) != NULL)
		{
			string  rtdbname(const_cast<char*>(access.getColumn_Text(8)));
			dbset.strRealTimeDBName = rtdbname;
		}
		if(access.getColumn_Text(9) != NULL)
		{
			string  uname(const_cast<char*>(access.getColumn_Text(9)));
			dbset.strUserName = uname;
		}
		dbset.nAutoLogin = access.getColumn_Int(10);
		dbset.DTUPortNumber = access.getColumn_Int(11);
		dbset.DTUBaudrate = access.getColumn_Int(12);
		if(access.getColumn_Text(13)!=NULL)
		{
			string  dbip2(const_cast<char*>(access.getColumn_Text(13)));
			dbset.strDBIP2 = dbip2;
		}
		dbset.nPort = access.getColumn_Int(14);
	}
	access.SqlFinalize();
	access.CloseDataBase();
	return dbset;

}


bool CMainFrame::SaveConfig(DataBaseSetting &dbset)
{
	if (NULL == m_pPrjSqlite)
	{
		return false;
	}
	CSqliteAcess access(m_strDBFileName.c_str(), m_pPrjSqlite->GetFileEncryptType());
	ostringstream sqlstream;
	sqlstream << "select * from project_config;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int  iCount = 0;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		iCount = access.getColumn_Int(0);
	}
	access.SqlFinalize();


	//////////////////////////////////////////////////////////////////////////
	//更新sql
	ostringstream sqlstream_set;
	sqlstream_set << "update project_config set HostIP = '"<< dbset.strDBIP <<"', DataBaseName =  '"<< dbset.strDBName <<"',RealTimeDBName = '"<< dbset.strRealTimeDBName <<"',UserName = '"\
		<< dbset.strUserName <<"', AutoLogin = "<< dbset.nAutoLogin << ", DTUPortNumber = "<< dbset.DTUPortNumber << ", DTUBaudrate = "<< dbset.DTUBaudrate <<", HostIP2 = '"<< dbset.strDBIP2 << "',port = "<< dbset.nPort<<";";
	sql_ = sqlstream_set.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	access.CloseDataBase();
	if (re == SQLITE_OK)
	{
		return true;
	}else
	{
		return false;
	}
}

void CMainFrame::OnAreaChange()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_AREA_CHANGE != m_DrawContext.m_nDrawMode) ? EQMDRAW_AREA_CHANGE : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateAreaChange( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_AREA_CHANGE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}


void CMainFrame::OnPageBatchGen()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnPageBatchReplacepoint()
{
	// TODO: 在此添加命令处理程序代码
	CPageReplacePointName dlg(m_DrawContext.pPage);
	dlg.DoModal();
}


void CMainFrame::OnPageBatchReplaceLink()
{
	// TODO: 在此添加命令处理程序代码
	CPageReplaceLinkName dlg(m_DrawContext.pPage);
	dlg.DoModal();
}


void CMainFrame::OnUpdatePageBatchGen(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_ProjectOpen);
}


void CMainFrame::OnUpdatePageBatchReplacepoint(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_ProjectOpen);
}

void CMainFrame::OnUpdatePageBatchReplaceLink(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_ProjectOpen);
}

CDataPointManager& CMainFrame::GetOnlinePointMap()
{
	return m_Onlineplcpointmap;
}

void CMainFrame::OnShellExecute()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_SHELL_EXECUTE != m_DrawContext.m_nDrawMode) ? EQMDRAW_SHELL_EXECUTE : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateShellExecute( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_SHELL_EXECUTE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnEmbededPage()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_EMBEDED_PAGE != m_DrawContext.m_nDrawMode) ? EQMDRAW_EMBEDED_PAGE : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateEmbededPage( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(EQMDRAW_EMBEDED_PAGE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnLiquidLevel()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_LIQUID_LEVEL != m_DrawContext.m_nDrawMode) ? EQMDRAW_LIQUID_LEVEL : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateLiquidLevel(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_LIQUID_LEVEL == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnCheckButtonPlus()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_CHKBTN_PLUS != m_DrawContext.m_nDrawMode) ? EQMDRAW_CHKBTN_PLUS : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateCheckButtonPlus(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_CHKBTN_PLUS == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnNoticeBoard()
{
	//m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_NOTICE_BOARD != m_DrawContext.m_nDrawMode) ? EQMDRAW_NOTICE_BOARD : EQMDRAW_MOVE;
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_LINE != m_DrawContext.m_nDrawMode) ? EQMDRAW_LINE : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateNoticeBoard(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_LINE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnHistoryTrendAnalysis()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_HISTORY_TREND != m_DrawContext.m_nDrawMode) ? EQMDRAW_HISTORY_TREND : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateHistoryTrendAnalysis(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_HISTORY_TREND == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnEnergySaveRoiAnalysis()
{
	/*m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_ENERGY_SAVE_ROI != m_DrawContext.m_nDrawMode) ? EQMDRAW_ENERGY_SAVE_ROI : EQMDRAW_MOVE;*/
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_RECTANGLE != m_DrawContext.m_nDrawMode) ? EQMDRAW_RECTANGLE : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateEnergySaveRoiAnalysis(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_RECTANGLE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnMapNavigate()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_MAP_NAVIGATE != m_DrawContext.m_nDrawMode) ? EQMDRAW_MAP_NAVIGATE : EQMDRAW_MOVE;


}

void CMainFrame::OnUpdateMapNavigate(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_MAP_NAVIGATE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnNavigateWindow()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_NAVIGATE_WINDOW != m_DrawContext.m_nDrawMode) ? EQMDRAW_NAVIGATE_WINDOW : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateNavigateWindow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_NAVIGATE_WINDOW == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnLevelRuler()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_LEVEL_RULER != m_DrawContext.m_nDrawMode) ? EQMDRAW_LEVEL_RULER : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateLevelRuler(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_LEVEL_RULER == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnDataAnalyse()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_DATA_ANALYSE != m_DrawContext.m_nDrawMode) ? EQMDRAW_DATA_ANALYSE : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateDataAnalyse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_DATA_ANALYSE == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnTemperDistri()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_TEMPER_DISTRI != m_DrawContext.m_nDrawMode) ? EQMDRAW_TEMPER_DISTRI : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateTemperDistri(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_TEMPER_DISTRI == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnDataReport()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_DATA_REPORT != m_DrawContext.m_nDrawMode) ? EQMDRAW_DATA_REPORT : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateDataReport(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_DATA_REPORT == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnPlaneTempeDistr()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_PLANE_TEMPE_DISTR != m_DrawContext.m_nDrawMode) ? EQMDRAW_PLANE_TEMPE_DISTR : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdatePlaneTempeDistr(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_PLANE_TEMPE_DISTR == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnDottedLineFrame()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_DOTLINE_FRAME != m_DrawContext.m_nDrawMode) ? EQMDRAW_DOTLINE_FRAME : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateDottedLineFrame(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_DOTLINE_FRAME == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::OnGeneralComponent()
{
	m_DrawContext.m_nDrawMode = (NULL != m_DrawContext.pPage && EQMDRAW_GENERAL_COMPONENT != m_DrawContext.m_nDrawMode) ? EQMDRAW_GENERAL_COMPONENT : EQMDRAW_MOVE;
}

void CMainFrame::OnUpdateGeneralComponent(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(EQMDRAW_GENERAL_COMPONENT == m_DrawContext.m_nDrawMode);
	pCmdUI->Enable(NULL != m_DrawContext.pPage);
}

void CMainFrame::InitOnlinePrjSqlite(void)
{
   	if (NULL == m_pOnlinePrjSqlite)
	{
		m_pOnlinePrjSqlite = new CProjectSqlite(m_Onlineproject, m_Onlineplcpointmap, m_OnlinelogicPotManager,m_OnlineAlarmPotManager);
		ASSERT(m_pPrjSqlite);
		if(m_pPrjSqlite)
		    m_pOnlinePrjSqlite->SetFileEncryptType(m_pPrjSqlite->GetFileEncryptType());
	}
}

void CMainFrame::SetPageListInfo(void)
{
	// get tree info
	vector<TREE_GROUP>						vecTreeInfo;
	vector<TREE_GROUP>::const_iterator		iterTreeInfo;
	vector<int>								vecPageInfo;
	vector<int>::const_iterator				iterPageInfo;
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	ASSERT(pView);
	//pView->m_pTreeDlg->GetTreeInfo(vecTreeInfo);
	m_wndFileView.GetTreeInfo(vecTreeInfo);

	// reset m_project.m_pagelist groupid
	const vector<CEqmDrawPage*>& pagelist	= m_project.GetPageList();
	int				nPageListSize			= 0;
	CEqmDrawPage*	pPage					= NULL;
	int				nPageId					= 0;
	int				nGroupId				= 0;
	bool			bFind					= false;

	nPageListSize = pagelist.size();
	for (UINT pageIdx=0; pageIdx<nPageListSize; ++pageIdx)
	{
		pPage = pagelist[pageIdx];
		nPageId = pPage->GetID();

		for (iterTreeInfo=vecTreeInfo.begin(); iterTreeInfo!=vecTreeInfo.end(); ++iterTreeInfo)
		{
			if (bFind)
			{
				break;
			}

			vecPageInfo = iterTreeInfo->vecPage;
			for (iterPageInfo=vecPageInfo.begin(); iterPageInfo!=vecPageInfo.end(); ++iterPageInfo)
			{
				if (*iterPageInfo == nPageId)
				{
					nGroupId = iterTreeInfo->nGroupId;
					bFind = true;
					break;
				}
			}
		}

		if (bFind)
		{
			bFind = false;

			if (pPage->GetGroupID() != nGroupId)
			{
				pPage->SetGroupID(nGroupId);
			}
		}
	}


	// resort m_project.m_pagelist
	vector<int>	vecPage;
	for (iterTreeInfo=vecTreeInfo.begin(); iterTreeInfo!=vecTreeInfo.end(); ++iterTreeInfo)
	{
		vecPageInfo = iterTreeInfo->vecPage;
		for (iterPageInfo=vecPageInfo.begin(); iterPageInfo!=vecPageInfo.end(); ++iterPageInfo)
		{
			vecPage.push_back(*iterPageInfo);
		}
	}
	m_project.ResortVecPage(vecPage);


	// resort m_project.m_groupgage
	vector<int> vecGroup;
	for (iterTreeInfo=vecTreeInfo.begin(); iterTreeInfo!=vecTreeInfo.end(); ++iterTreeInfo)
	{
		vecGroup.push_back(iterTreeInfo->nGroupId);
	}
	m_project.ResortVecGroup(vecGroup);
}

void CMainFrame::SetSaveFlag(const bool bNeedSave)
{
	m_bNeedSave = bNeedSave;
}

bool CMainFrame::GetSaveFlag(void)
{
	return m_bNeedSave;
}


void CMainFrame::OnImageMgr()
{
	// TODO: Add your command handler code here
	ASSERT(m_pPrjSqlite != NULL);
	if (m_pPrjSqlite != NULL)
	{
		if (!m_bLoadIndependent)
		{
			m_pPrjSqlite->LoadPictureLibSqlite(false);
			m_pPrjSqlite->LoadAnimationSqlite(false);
			m_pPrjSqlite->LoadPictureLibSqlite(true);
			m_pPrjSqlite->LoadAnimationSqlite(true);
			m_bLoadIndependent = true;
		}

		CMetaFileSelectDlg dlg;
		dlg.SetWndType(e_picture);
		dlg.SetBtnOkShow(false);
		dlg.SetShowMode(0);
		dlg.DoModal();
	}
}


void CMainFrame::OnUpdateImageMgr(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}

void CMainFrame::OnImageCheck()
{
	// TODO: Add your command handler code here

	// load dest s3db for check image info
	ASSERT(m_pPrjSqlite != NULL);
	if (m_pPrjSqlite != NULL)
	{
		m_pProjectCheckDlg.SetProjectSqlPointer(m_pPrjSqlite);
		m_pProjectCheckDlg.ShowWindow(TRUE);
	}
	else
	{
		MessageBox(_T("Sqlite指针失效！"), _T("错误"), MB_OK|MB_ICONERROR);
	}
}


void CMainFrame::OnUpdateImageCheck(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}

void CMainFrame::OnS3dbCheck()
{
	// TODO: Add your command handler code here
	CheckDatabaseIntegrity(m_strFilePath, 2);
}

void CMainFrame::OnUpdateS3dbCheck(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}

void CMainFrame::GetVersionFromHttp(const CString strCfgPathName)
{
	TCHAR szBuf[MAX_PATH] = {0};

	m_stQueryVer.nPort = GetPrivateProfileInt(_T("BeopFactory"), _T("VersionPort"), 5000, strCfgPathName);

	GetPrivateProfileString(_T("BeopFactory"), _T("VersionIp"), _T("127.0.0.1"), szBuf, MAX_PATH, strCfgPathName);
	m_stQueryVer.strIp.Format(_T("%s"), szBuf);

	memset(szBuf, 0, MAX_PATH);
	GetPrivateProfileString(_T("BeopFactory"), _T("VersionDir"), _T("beopversion"), szBuf, MAX_PATH, strCfgPathName);
	m_stQueryVer.strDir.Format(_T("%s"), szBuf);

	memset(szBuf, 0, MAX_PATH);
	GetPrivateProfileString(_T("BeopFactory"), _T("VersionValue"), _T("2014-06-01"), szBuf, MAX_PATH, strCfgPathName);
	m_stQueryVer.strVer.Format(_T("%s"), szBuf);

	_beginthreadex(NULL, 0, ThreadFuncFindVersion, (LPVOID)&m_stQueryVer, 0, NULL);
	Sleep(1);
}

UINT WINAPI CMainFrame::ThreadFuncFindVersion(LPVOID lparam)
{
	QueryVersion* p = (QueryVersion*)lparam;

	CHttpOperation http(p->strIp, p->nPort, p->strDir);
	CString	strGetVer;
	CString	strErr(_T("failure: software not found"));
	if (http.HttpPostGetRequest(strGetVer))
	{
		if (p->strVer != strGetVer && strGetVer != strErr)
		{
			AfxMessageBox(_T("Factory存在新版本，可从FTP下载更新。"));
		}
	}


	return 0;
}




bool CMainFrame::GetFactorySoftVersion(int &nVersionPrefix1,int &nVersionPrefix2,int &nVersionPrefix3,int &nVersionPrefix4)
{
	CString version;
	VS_FIXEDFILEINFO *pVerInfo = NULL;
	DWORD dwTemp, dwSize, dwHandle = 0;
	BYTE *pData = NULL;
	UINT uLen;


	try {

		TCHAR szFullPath[MAX_PATH];

		::GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
		dwSize = ::GetFileVersionInfoSize((LPTSTR) szFullPath, &dwTemp);
		if (dwSize == 0) throw 1;


		pData = new BYTE[dwSize];
		if (pData == NULL) throw 1;


		if (!::GetFileVersionInfo((LPTSTR) szFullPath, dwHandle, dwSize, pData))
			throw 1;


		if (!VerQueryValue(pData, _T("\\"), (void **) &pVerInfo, &uLen)) 
			throw 1;


		DWORD verMS = pVerInfo->dwProductVersionMS;
		DWORD verLS = pVerInfo->dwProductVersionLS;


		nVersionPrefix1 = HIWORD(verMS);
		nVersionPrefix2 = LOWORD(verMS);
		nVersionPrefix3 = HIWORD(verLS);
		nVersionPrefix4 = LOWORD(verLS);

		delete pData;
		TRACE1("version - %s\n", version);

		return true;
	}
	catch(...) {
		return false;
	}
}

void CMainFrame::ShowSelectElementByID( const int nElementID )
{
	
	if (NULL == m_DrawContext.pPage)
	{
		return;
	}

	m_DrawContext.ClearSelect();
	for (list<CEqmDrawDevice*>::iterator it = m_DrawContext.pPage->GetDrawDeviceList().begin();
		it != m_DrawContext.pPage->GetDrawDeviceList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//设备
	for (list<CEqmDrawPipe*>::iterator it = m_DrawContext.pPage->GetPagePipeList().begin();
		it != m_DrawContext.pPage->GetPagePipeList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//管道
	for (list<CEqmDrawText*>::iterator it = m_DrawContext.pPage->GetPageTextList().begin();
		it != m_DrawContext.pPage->GetPageTextList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//文字
	for ( list<CEqmDrawGraphic*>::iterator it = m_DrawContext.pPage->Get2DGraphicList().begin();
		it != m_DrawContext.pPage->Get2DGraphicList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//二维图形
	for ( list<CEqmDrawBarGraphic*>::iterator it = m_DrawContext.pPage->GetBarGraphList().begin();
		it != m_DrawContext.pPage->GetBarGraphList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//柱状图
	for ( list<CEqmDrawBend*>::iterator it = m_DrawContext.pPage->GetBendList().begin();
		it != m_DrawContext.pPage->GetBendList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//弯头
	for ( list<CEqmDrawDashBoard*>::iterator it = m_DrawContext.pPage->GetDashboardList().begin();
		it != m_DrawContext.pPage->GetDashboardList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//仪表盘
	for ( list<CEqmDrawLineGraphic*>::iterator it = m_DrawContext.pPage->GetLineGraphList().begin();
		it != m_DrawContext.pPage->GetLineGraphList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//折线图
	for ( list<CEqmDrawPieGraphic*>::iterator it = m_DrawContext.pPage->GetPieGraphList().begin();
		it != m_DrawContext.pPage->GetPieGraphList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//饼图
	for ( list<CEqmDrawButton*>::iterator it = m_DrawContext.pPage->GetButtonList().begin();
		it != m_DrawContext.pPage->GetButtonList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//按钮
	for ( list<CEqmDrawTimeSelect*>::iterator it = m_DrawContext.pPage->GetTimeSelectList().begin();
		it != m_DrawContext.pPage->GetTimeSelectList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//时间选择
	for ( list<CEqmDrawTimeBar*>::iterator it = m_DrawContext.pPage->GetTimeBarList().begin();
		it != m_DrawContext.pPage->GetTimeBarList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//时间柱状图
	for ( list<CEqmDrawProgressBar*>::iterator it = m_DrawContext.pPage->GetProgressBarList().begin();
		it != m_DrawContext.pPage->GetProgressBarList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//进度条
	for ( list<CEqmDrawDiagnoseGraphic*>::iterator it = m_DrawContext.pPage->GetDiagnoseList().begin();
		it != m_DrawContext.pPage->GetDiagnoseList().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));	
		}
	}//多边形
	for ( list<CEqmDrawPolygon*>::iterator it = m_DrawContext.pPage->GetPolygon().begin();
		it != m_DrawContext.pPage->GetPolygon().end(); it++)
	{
		if((*it)->GetID() == nElementID)
		{
			(*it)->SetSelected(true);
			m_DrawContext.vecSelectEqmt.push_back((*it));
		}
	}//点阵诊断图
}

void CMainFrame::SetPointTableFlag(const bool bNeedSave)
{
	m_bPtTableNeedSave = bNeedSave;
}

bool CMainFrame::GetPointTableFlag(void) const
{
	return m_bPtTableNeedSave;
}

BOOL CMainFrame::LoadFrame( UINT nIDResource, DWORD dwDefaultStyle /*= WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE*/, CWnd* pParentWnd /*= NULL*/, CCreateContext* pContext /*= NULL*/ )
{
	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
	// enable customization button for all user toolbars
	return TRUE;
}

void CMainFrame::LoadUserToolImage()
{

}

void CMainFrame::LoadUserToolImage( CString strPath )
{

}

void CMainFrame::ReplaceUserToolImage()
{

}

void CMainFrame::ReplaceUserToolImage( UINT uiCmd, int nImageIndex, CString strTip )
{

}

BOOL CMainFrame::CreateDockingWindows()
{
	// Create file view
	if (!m_wndFileView.Create(_T("FileWnd"), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

	// Create properties window
	if (!m_wndProperties.Create(_T("PropertyWnd"), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

void CMainFrame::OnShowPropertyWnd()
{
	m_wndProperties.ShowPropertyWnd(TRUE);
}

void CMainFrame::OnUpdateShowPropertyWnd( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateShowToolBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnShowToolBar()
{
	m_wndToolBar.ShowPane(TRUE,FALSE,TRUE);
}

void CMainFrame::OnToolBarLayout()
{
	m_wndToolBarLayout.ShowPane(TRUE, FALSE, TRUE);
}

void CMainFrame::OnUpdateToolBarLayout(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnDownLoadFile()
{
	if(m_wndFileView.GetDataAccess()==NULL)
	{
		AfxMessageBox(_T("请先连接core进入在线调试"));
		return;
	}

	CDownLoadFileDlg dlg(m_strFilePath);
	dlg.m_strServerIP = m_wndFileView.GetDataAccess()->GetServerAddress();
	dlg.DoModal();
}

void CMainFrame::OnUpdateDownLoadFile( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnPagePercent()
{

}

void CMainFrame::OnUpdatePagePercent( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(m_ProjectOpen);
}

void CMainFrame::OnComboBox()
{
	CMFCToolBarComboBoxButton *pStcCombo = CMFCToolBarComboBoxButton::GetByCmd(ID_PAGE_PERCENT,TRUE);
	if(pStcCombo)
	{
		int cursel = pStcCombo->GetCurSel();
		double newsel = 0.5 + cursel*0.1;
		m_DrawContext.m_fScreenScale = newsel;
		CBEOPDesignerView* pview = (CBEOPDesignerView*)GetActiveView();	
		pview->m_PageDlg->m_rtCanvas.bottom = m_DrawContext.GetScaledheight();
		pview->m_PageDlg->m_rtCanvas.right  = m_DrawContext.GetScaledWidth();

		pview->m_PageDlg->PostMessage(WM_SIZE);

		pview->m_PageDlg->ResetArbiterPosition();
	}
}

void CMainFrame::ClickComboBox()
{

}

void CMainFrame::OnUpdateComboBo( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(TRUE);
}



void CMainFrame::OnSoftPreference()
{
	// TODO: Add your command handler code here
	CSoftPreferenceDlg dlg;

	CString cstrFile;
	Project::Tools::GetSysPath(cstrFile);
	CString exepath = cstrFile +  L"\\factory.ini";
	wchar_t strOption[1024];
	GetPrivateProfileString(L"BeopFactory", L"ImageLibPath", L"", strOption, 1024, exepath.GetString());
	wstring strImageLibPath = strOption;
	if(strImageLibPath.length()<=0)
	{
		strImageLibPath = cstrFile + L"\\beopimagelib.s3db";
	}

	dlg.m_strImageLibPath = strImageLibPath.c_str();

	if(dlg.DoModal()==IDOK)
	{
		WritePrivateProfileString (_T("BeopFactory"),_T("ImageLibPath"),dlg.m_strImageLibPath, exepath.GetString());

		m_pPrjSqlite->SetImageLibPath(dlg.m_strImageLibPath.GetString());
	}

}

bool CMainFrame::CheckFileName(const CString strPath)
{
	// deny Chinese char
	const CString strFlag(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-"));
	CString	strChar;

	int nPathLen = strPath.GetLength();
	CString	strName;
	strName = strPath.Right(nPathLen - strPath.ReverseFind(_T('\\')) - 1);

	int	nNameLen = strName.GetLength();
	for (int i=0; i<nNameLen; ++i)
	{
		strChar = strName.Mid(i, 1);
		if (-1 == strFlag.Find(strChar))
		{	// find other char
			MessageBox(_T("项目名称只能使用字母、数字、下划线、横线！"), _T("错误"), MB_OK|MB_ICONERROR);
			return false;
		}
	}

	return true;
}

void CMainFrame::OnShowPageWnd()
{
	m_wndFileView.ShowPageWnd(TRUE);
}

void CMainFrame::OnEditUndo()
{
	CommandManager::Instance()->Undo();
}

void CMainFrame::OnUpdateEditUndo( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(CommandManager::Instance()->CanUndo());
	//pCmdUI->Enable(FALSE);
}

void CMainFrame::OnEditRedo()
{
	CommandManager::Instance()->Redo();
}

void CMainFrame::OnUpdateEditRedo( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(CommandManager::Instance()->CanRedo());
	//pCmdUI->Enable(FALSE);
}

void CMainFrame::MetaCopy(void)
{
	OnEditCopy();
}

void CMainFrame::MetaPaste(void)
{
	OnEditPaste();
}


void CMainFrame::OnOpenImageMgr()
{
	// TODO: Add your command handler code here
	ASSERT(m_pPrjSqlite != NULL);
	if (m_pPrjSqlite != NULL)
	{
		if (!m_bLoadIndependent)
		{
			m_pPrjSqlite->LoadPictureLibSqlite(false);
			m_pPrjSqlite->LoadAnimationSqlite(false);
			m_pPrjSqlite->LoadPictureLibSqlite(true);
			m_pPrjSqlite->LoadAnimationSqlite(true);
			m_bLoadIndependent = true;
		}

		CMetaFileSelectDlg dlg;
		dlg.SetWndType(e_picture);
		dlg.SetBtnOkShow(false);
		dlg.SetShowMode(1);
		dlg.DoModal();
	}
}


void CMainFrame::OnUpdateOpenImageMgr(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}


void CMainFrame::OnOpenImageReplaceInfo()
{
	// TODO: Add your command handler code here
	if (m_pImgReplageDlg != NULL)
	{
		m_pImgReplageDlg->ShowWindow(SW_NORMAL);
	}
}


void CMainFrame::OnUpdateOpenImageReplaceInfo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}


void CMainFrame::OnImgLean()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, NULL, _T(""), OFN_HIDEREADONLY|OFN_HIDEREADONLY, _T("db Files (*.s3db;*.4db)|*.s3db;*.4db|"));
	if (IDOK == dlg.DoModal())
	{
		wchar_t szPathName[MAX_PATH] = {0};
		CString strPathName = dlg.GetPathName();
		wcscpy_s(szPathName, MAX_PATH, strPathName.GetBuffer());
		strPathName.ReleaseBuffer();

		m_pPrjSqlite->SetFilePath(szPathName);

		if(strPathName.Find(_T(".4db"))>=0)
			m_pPrjSqlite->SetFileEncryptType(false);//不加密

		m_pPrjSqlite->ImageLeanFunc();

		if (!m_pPrjSqlite->VaccumDatabase())
		{
			MessageBox(_T("压缩失败！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}

		MessageBox(_T("处理完成！"), _T("完成"), MB_OK|MB_ICONASTERISK);
	}
}


void CMainFrame::OnUpdateImgLean(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_ProjectOpen);
}


void CMainFrame::OnImgLeanCore()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, NULL, _T(""), OFN_HIDEREADONLY|OFN_HIDEREADONLY, _T("db Files (*.s3db;*.4db)|*.s3db;*.4db|"));
	if (IDOK == dlg.DoModal())
	{
		wchar_t szPathName[MAX_PATH] = {0};
		CString strPathName = dlg.GetPathName();
		wcscpy_s(szPathName, MAX_PATH, strPathName.GetBuffer());
		strPathName.ReleaseBuffer();

		m_pPrjSqlite->SetFilePath(szPathName);

		m_pPrjSqlite->ImageLeanCoreFunc();

		if (!m_pPrjSqlite->VaccumDatabase())
		{
			MessageBox(_T("压缩失败！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}

		MessageBox(_T("处理完成！"), _T("完成"), MB_OK|MB_ICONASTERISK);
	}
}


void CMainFrame::OnUpdateImgLeanCore(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_ProjectOpen);
}


void CMainFrame::OnDevAreaSync()
{
	// TODO: Add your command handler code here
	CConnect dlg(m_dwSyncIp, m_nSyncPort, m_strSyncUsername, m_strSyncPassword, m_strSyncDbName);
	dlg.DoModal();
}


void CMainFrame::OnUpdateDevAreaSync(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bLoadIndependent);
}


void CMainFrame::OnPageDevAreaSync()
{
	// TODO: Add your command handler code here
	ASSERT(NULL != m_pPrjSqlite);
	if (NULL == m_pPrjSqlite)
	{
		return;
	}


	CEqmDrawPage* pPage = m_DrawContext.pPage;
	ASSERT(NULL != pPage);
	if (NULL == pPage)
	{
		return;
	}

	int nCurPageId = pPage->GetID();
	CString strCurPageName;
	strCurPageName.Format(L"%s", pPage->GetPageName().c_str());


	vector<TextAreaInfo>::const_iterator itArea;
	vector<CString> vecAreaName;
	vector<int> vecDel;
	int i = 0;
	for (itArea=m_pPrjSqlite->m_vecTextAreaInf.begin(); itArea!=m_pPrjSqlite->m_vecTextAreaInf.end(); itArea++,i++)
	{
		if (itArea->strPageName == strCurPageName)
		{
			vecAreaName.push_back(itArea->strRegion);
			vecDel.push_back(i);
		}
	}
	for (int i=vecDel.size()-1; i>=0; --i)
	{
		DeleteAreaItem(vecDel.at(i));
	}


	vector<TextDeviceInfo>::const_iterator itText;
	vector<CString>::const_iterator itAreaName;
	vecDel.clear();
	i = 0;
	for (itText=m_pPrjSqlite->m_vecTextDevInf.begin(); itText!=m_pPrjSqlite->m_vecTextDevInf.end(); itText++,i++)
	{
		for (itAreaName=vecAreaName.begin(); itAreaName!=vecAreaName.end(); itAreaName++)
		{
			if (itText->strRegion == *itAreaName)
			{
				vecDel.push_back(i);
				break;
			}
		}
	}
	for (int i=vecDel.size()-1; i>=0; --i)
	{
		DeleteDeviceItem(vecDel.at(i));
	}


	list<CEqmDrawText*>& listText = pPage->GetPageTextList();
	list<CEqmDrawText*>::iterator iterText;
	CString strDevCfg;
	for (iterText=listText.begin(); iterText!=listText.end(); iterText++)
	{
		if (_T("") != (*iterText)->GetDeviceCfg())
		{
			CString strBindPageName((*iterText)->GetDeviceCfg());
			wstring wstrBindPageName(strBindPageName.GetBuffer(strBindPageName.GetLength()));
			strBindPageName.ReleaseBuffer();
			int nPageId = m_project.GetPageIDByPageName(wstrBindPageName);

			TextDeviceInfo devInf;
			devInf.strRegion = _T("Area_") + strBindPageName;
			devInf.nBindPageId = nPageId;
			devInf.strBindPageName = strBindPageName;
			devInf.nBindTextId = (*iterText)->GetID();
			m_pPrjSqlite->m_vecTextDevInf.push_back(devInf);

			TextAreaInfo areaInf;
			areaInf.nPageId = nCurPageId;
			areaInf.strPageName = strCurPageName;
			areaInf.strRegion = _T("Area_") + strBindPageName;
			m_pPrjSqlite->m_vecTextAreaInf.push_back(areaInf);
		}
	}


	MessageBox(_T("当前打开页面区域同步完成！"), _T("完成"), MB_OK|MB_ICONASTERISK);
}


void CMainFrame::OnUpdatePageDevAreaSync(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bLoadIndependent);
}


void CMainFrame::OnVersionHistory()
{
	// TODO: 在此添加命令处理程序代码
	CVersionHistoryDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnUpdateVersionHistory(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_ProjectOpen);
}


void CMainFrame::DeleteAreaItem(const int nIndex)
{
	ASSERT(NULL != m_pPrjSqlite);
	if (NULL == m_pPrjSqlite)
	{
		return;
	}

	vector<TextAreaInfo>::const_iterator iter;
	int i = 0;
	for (iter=m_pPrjSqlite->m_vecTextAreaInf.begin(); iter!=m_pPrjSqlite->m_vecTextAreaInf.end(); iter++,i++)
	{
		if (nIndex == i)
		{
			m_pPrjSqlite->m_vecTextAreaInf.erase(iter);
			break;
		}
	}
}

void CMainFrame::DeleteDeviceItem(const int nIndex)
{
	ASSERT(NULL != m_pPrjSqlite);
	if (NULL == m_pPrjSqlite)
	{
		return;
	}

	vector<TextDeviceInfo>::const_iterator iter;
	int i = 0;
	for (iter=m_pPrjSqlite->m_vecTextDevInf.begin(); iter!=m_pPrjSqlite->m_vecTextDevInf.end(); iter++,i++)
	{
		if (nIndex == i)
		{
			m_pPrjSqlite->m_vecTextDevInf.erase(iter);
			break;
		}
	}
}

void CMainFrame::OpenProjectContain(const CString strPathName)
{
	IfLoadFail = false;
	BOOL bFileExist = PathFileExists(strPathName);
	if (!bFileExist)
	{
		return;
	}

	if (m_pPrjSqlite != NULL)
	{
		m_pPrjSqlite->ParseFileType(strPathName);
	}

	CloseProjectBeforeOpen();
	OnCloseproject();
	m_bOnlyOpenPointTable = false;
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	ASSERT(pView);
	//pView->m_pTreeDlg->DeleteAllItems();
	m_wndFileView.DeleteAllItems();
	m_strFilePath = strPathName;

	CString strDecPath(strPathName + _T(".dec"));
	if (PathFileExists(strDecPath))
	{
		m_bSyncDec = true;
	}

	string strLastEditSoftVersion = m_pPrjSqlite->GetProjectLastEditSoftVersion();
	wstring wstrLastEditSoftVersion = Project::Tools::AnsiToWideChar(strLastEditSoftVersion.c_str());
	int nPrefix1InDB = 0, nPrefix2InDB = 0, nPrefix3InDB = 0, nPrefix4InDB = 0;
	GetVersionPrefix(wstrLastEditSoftVersion.c_str(), nPrefix1InDB, nPrefix2InDB, nPrefix3InDB, nPrefix4InDB);

	int nPrefix1 = 0, nPrefix2 = 0, nPrefix3 = 0, nPrefix4 = 0;
	GetFactorySoftVersion(nPrefix1, nPrefix2, nPrefix3, nPrefix4);

	bool bVersionOverdue = false;
	if(nPrefix1< nPrefix1InDB)
	{
		bVersionOverdue = true;
	}
	else if(nPrefix1 > nPrefix1InDB)
	{
		bVersionOverdue = false;
	}
	else  //相等
	{
		if(nPrefix2< nPrefix2InDB)
		{
			bVersionOverdue = true;
		}
		else if(nPrefix2 > nPrefix2InDB)
		{
			bVersionOverdue = false;
		}
		else  //相等
		{
			if(nPrefix3< nPrefix3InDB)
			{
				bVersionOverdue = true;
			}
			else if(nPrefix3 > nPrefix3InDB)
			{
				bVersionOverdue = false;
			}
			else  //相等
			{
				if(nPrefix4< nPrefix4InDB)
				{
					bVersionOverdue = true;
				}
				else if(nPrefix4 > nPrefix4InDB)
				{
					bVersionOverdue = false;
				}
				else  //相等
				{
					bVersionOverdue = false;
				}
			}
		}
	}

	if(bVersionOverdue)
	{
		CString strTemp;
		strTemp.Format(L"该项目文件是由一个更高版本的Factory(%s)所生成和编辑的，请升级软件版本后才能打开.", wstrLastEditSoftVersion.c_str());
		AfxMessageBox(strTemp);
		return;
	}

	if (SetRecentOpenFileList(strPathName))
	{
		IfLoadFail = true;
	}

	CheckDatabaseIntegrity(m_strFilePath, 0);

	StartLoadThread();

	m_strDBFileName = m_strFilePath.GetString();
}

BOOL CMainFrame::SetRecentOpenFileList(const CString strPathName)
{
	CString strExtFlag(_T("s3db"));
	CString strExtFlag2(_T("4db"));
	CString strExt(strPathName.Mid(strPathName.ReverseFind('.') + 1));
	if (0 != strExt.Compare(strExtFlag) && 0 != strExt.Compare(strExtFlag2))
	{
		return FALSE;
	}

	CRecentFileList* pFileList = theApp.m_pRecFileList;
	if (NULL == pFileList) 
	{
		return FALSE;
	}
	int nMRUSize =pFileList->GetSize();
	CString strMRUFile;
	CString strFileExten;
	CStringArray strFileArray;
	int nArrSize = 0;

	for(int i=nMRUSize-1; i>=0; i--)
	{
		strMRUFile = pFileList->m_arrNames[i];
		if (strPathName == strMRUFile)
		{
			continue;
		}

		strFileExten = strMRUFile.Mid(strMRUFile.ReverseFind('.') + 1);
		if (0 == strFileExten.Compare(strExtFlag) || 0 == strFileExten.Compare(strExtFlag2))
		{
			strFileArray.Add(strMRUFile);
		}
	}
	strFileArray.Add(strPathName);

	for(int i=0; i<nMRUSize; i++)
	{
		pFileList->m_arrNames[i].Empty();
	}

	nArrSize = strFileArray.GetSize();
	for (int i=0; i<nArrSize; i++)
	{
		pFileList->Add(strFileArray.GetAt(i));
	}
	pFileList->WriteList();

	return TRUE;
}

BOOL CMainFrame::CheckDatabaseIntegrity(const CString strPathName, const int nType)
{
	if (NULL == m_pPrjSqlite)
	{
		return FALSE;
	}
	BOOL bRet = TRUE;
	CSqliteAcess access(strPathName.GetString(), m_pPrjSqlite->GetFileEncryptType());
	ostringstream sqlstream;
	sqlstream << "pragma integrity_check;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (TRUE)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		const char* pResult = access.getColumn_Text(0);
		const int nRet = strcmp(pResult, "ok");
		if (0 != nRet)
		{	// error
			bRet = FALSE;
			USES_CONVERSION;
			CString strShow;
			if (0 == nType)
			{	// open
				strShow = _T("项目打开失败，本文件已经损坏，请联系产品技术支持。\r\n\r\n");
			}
			else if (1 == nType)
			{	// save
				strShow = _T("项目保存失败，本文件已经损坏，请联系产品技术支持。\r\n\r\n");
			}
			else if (2 == nType)
			{	// save
				strShow = _T("项目校验失败，本文件已经损坏，请联系产品技术支持。\r\n\r\n");
			}
			CString strRet;
			strRet.Format(_T("%s"), A2T(pResult));
			strShow += strRet;

			CCheckInfoDlg dlg;
			dlg.m_strShow = strShow;
			dlg.DoModal();
		}
	}
	if (bRet && 2 == nType)
	{
		MessageBox(_T("校验成功，文件正常。"), _T("提示"), MB_OK|MB_ICONASTERISK);
	}

	access.SqlFinalize();
	return bRet;
}

UINT WINAPI CMainFrame::ThreadFuncSyncVersion(LPVOID lparam)
{
	CMainFrame* pMainFrm = (CMainFrame*)lparam;
	if (NULL == pMainFrm)
	{
		return 0;
	}

	TCHAR szAppDir[MAX_PATH] = {0};
	TCHAR szVersion[MAX_PATH] = {0};
	TCHAR szUpdateDir[MAX_PATH] = {0};
	TCHAR szSyncFunc[MAX_PATH] = {0};
	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	PathRemoveFileSpec(szAppDir);
	PathCombine(szVersion, szAppDir, _T("version.txt"));
	PathCombine(szUpdateDir, szAppDir, _T("update"));
	PathCombine(szSyncFunc, szAppDir, _T("FactoryWatch.exe"));

	CFile fileVer;
	CFileException ex;
	if (!fileVer.Open(szVersion, CFile::modeRead|CFile::shareDenyWrite, &ex))
	{
		TCHAR szError[1024] = {0};
		ex.GetErrorMessage(szError, 1024);
		return FALSE;
	}
	DWORD dwVerLocal = 0;
	DWORD dwVerRemote = 0;
	char szVerLocal[MAX_PATH] = {0};
	fileVer.Read(szVerLocal, MAX_PATH);
	dwVerLocal = atoi(szVerLocal);
	fileVer.Close();
	if (0 == dwVerLocal)
	{
		return 0;
	}

	CString strIp(pMainFrm->m_strFtpIp);
	CString strUser(pMainFrm->m_strFtpUser);
	CString strPwd(pMainFrm->m_strFtpPwd);
	int nPort = pMainFrm->m_nFtpPort;

	CString strVerFile(_T("update/beopfactory/version.txt"));
	CString strRemoteFile(_T("update/beopfactory/"));
	CString strUpdatePath;
	CString strLocalPath;
	CString strVersion;
	CString strSyncFunc;
	if (pMainFrm->ReadFtpFile(strVerFile, strIp, strUser, strPwd, nPort, strVersion))
	{
		dwVerRemote = _ttoi((LPCTSTR)strVersion);
		if (dwVerLocal < dwVerRemote)
		{
			if (IDOK == pMainFrm->MessageBox(_T("有新版本可用，是否立即更新？"), _T("提示"), MB_OKCANCEL|MB_ICONQUESTION))
			{
				pMainFrm->m_pNoModelDlg->ShowWindow(SW_SHOW);

				strUpdatePath.Format(_T("%s"), szUpdateDir);
				if (!pMainFrm->CreateSpecDir(strUpdatePath))
				{
					return FALSE;
				}

				strLocalPath.Format(_T("%s\\beopfactory"), szUpdateDir);
				if (!pMainFrm->CreateSpecDir(strLocalPath))
				{
					return FALSE;
				}
				pMainFrm->DeleteDirContain(strLocalPath);

				strRemoteFile.TrimRight(_T("/"));
				if (!pMainFrm->GetFtpFileEx(strRemoteFile, strLocalPath, strIp, strUser, strPwd, nPort))
				{
					return FALSE;
				}

				Sleep(1000);
				strSyncFunc.Format(_T("%s"), szSyncFunc);
				int nShellRet = (int)ShellExecute(NULL, _T("open"), strSyncFunc, NULL, NULL, SW_SHOWNORMAL);
				if (nShellRet > 32)
				{
					ExitProcess(0);
					//exit(0);
					//PostQuitMessage(0);
					return FALSE;
				}

				//CString strSrc;
				//CString strDst;
				//strSrc.Format(_T("%s"), strLocalPath);
				//strDst.Format(_T("%s"), szAppDir);
				//if (CopySpecDir(strSrc, strDst))
				//{
				//	MessageBox(_T("更新成功！"), _T("提示"), MB_OK|MB_ICONASTERISK);
				//}
			}
		}
	}

	return 0;
}

BOOL CMainFrame::SyncVersionByFtp(void)
{
	if (!m_bFtpEnable)
	{
		return FALSE;
	}

	_beginthreadex(NULL, 0, ThreadFuncSyncVersion, this, 0, NULL);
	Sleep(1);

	return TRUE;
}

BOOL CMainFrame::CreateSpecDir(const CString strPath)
{
	CFileStatus rStatus;
	if (!CFile::GetStatus(strPath, rStatus))
	{	//文件夹不存在, 创建
		if (!CreateDirectory((LPCTSTR)strPath, NULL))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMainFrame::CopySpecDir(CString strSrc, CString strDst)
{
    CreateDirectory(strDst, NULL);
    CFileFind finder;
    CString stPath;
    BOOL ret = FALSE;
    stPath.Format(_T("%s\\*.*"), strSrc);
    BOOL bWorking = finder.FindFile(stPath);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        if (finder.IsDirectory() && !finder.IsDots())
        {
            CopySpecDir(finder.GetFilePath(), strDst+_T("\\")+finder.GetFileName());
        }
        else
        {
            CString stSrcFile(finder.GetFilePath());
            BOOL result = (GetFileAttributes(stSrcFile) & FILE_ATTRIBUTE_DIRECTORY);
            if (!result)
            {
				CString strOld(finder.GetFilePath());
				CString strNew(strDst+_T("\\")+finder.GetFileName());
				ret = CopyFile(strOld, strNew, FALSE);
            }
        }
    }
    return ret;
}

void CMainFrame::DeleteDirContain(CString strDir)
{
	if (strDir.IsEmpty())
	{
		RemoveDirectory(strDir);
		return;
	}

	CFileFind ff;
	BOOL bFound = ff.FindFile(strDir+ _T("\\*"), 0);
	while (bFound)
	{
		bFound = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory())
		{
			DeleteDirContain(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else
		{
			DeleteFile(ff.GetFilePath());
		}
	}
	ff.Close();
}

BOOL CMainFrame::GetFtpFileEx(CString strRemoteFile, CString strLocalFile, CString strIp, CString strUser, CString strPwd, int nPort)
{
	CInternetSession* lpInetSession = NULL;
	CFtpConnection* lpFtpConnection = NULL;
	lpInetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	if (NULL == lpInetSession)
	{
		return FALSE;
	}

	CString strFlag(_T("*.*"));
	CString strLine(_T("/"));
	try
	{
		lpFtpConnection = lpInetSession->GetFtpConnection(strIp, strUser, strPwd, nPort);
		if (NULL == lpFtpConnection)
		{
			lpInetSession->Close();
			delete lpInetSession;
			return FALSE;
		}

		CFtpFileFind finder(lpFtpConnection);
		BOOL bWork = finder.FindFile(strRemoteFile);
		while (bWork)
		{
			bWork = finder.FindNextFile();
			if (finder.IsDots())
				continue;

			CString strRemoteTemp;
			CString strLocalTemp;
			CString strFileName(finder.GetFileName());
			if (finder.IsDirectory())
			{
				CString strLocDir;
				CString strPath(finder.GetFilePath());
				strPath.TrimLeft(_T("/"));
				strPath.TrimRight(_T("/"));
				if (strPath == strRemoteFile)
				{	// top
					strRemoteTemp.Format(_T("%s/%s"), strRemoteFile, strFlag);
					strLocalTemp.Format(_T("%s/%s"), strLocalFile, strFlag);
					strLocDir = strLocalFile;
				}
				else
				{
					//strRemoteTemp.Format(_T("%s/%s"), strPath, strFlag);
					strRemoteTemp.Format(_T("%s/%s/%s"), strRemoteFile, strFileName, strFlag);
					strLocalTemp = strLocalFile;
					strLocalTemp.TrimRight(strFlag);
					strLocalTemp.TrimRight(strLine);
					strLocalTemp += strLine + strFileName;
					strLocDir = strLocalTemp;
					strLocalTemp += _T("/") + strFlag;
				}

				if (!CreateSpecDir(strLocDir))
				{
					lpFtpConnection->Close();
					lpInetSession->Close();
					delete lpFtpConnection;
					delete lpInetSession;
					return FALSE;
				}

				GetFtpFileEx(strRemoteTemp, strLocalTemp, strIp, strUser, strPwd, nPort);
			}
			else
			{
				strRemoteTemp = strRemoteFile;
				strRemoteTemp.TrimRight(strFlag);
				strRemoteTemp.TrimRight(strLine);
				strRemoteTemp += strLine + strFileName;

				strLocalTemp = strLocalFile;
				strLocalTemp.TrimRight(strFlag);
				strRemoteTemp.TrimRight(strLine);
				strLocalTemp += strLine + strFileName;

				BOOL bRes = lpFtpConnection->GetFile(strRemoteTemp, strLocalTemp, FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 1);
				if (!bRes)
				{
					DWORD dErr = GetLastError();
				}
			}
		}
	}
	catch(CInternetException* lpEx)
	{
		lpEx->ReportError();
		lpEx->Delete();
		lpFtpConnection->Close();
		lpInetSession->Close();
		delete lpFtpConnection;
		delete lpInetSession;
		return FALSE;
	}
	lpFtpConnection->Close();
	lpInetSession->Close();
	delete lpFtpConnection;
	delete lpInetSession;

	return TRUE;
}

BOOL CMainFrame::ReadFtpFile(CString strRemoteFile, CString strIp, CString strUser, CString strPwd, int nPort, CString& strVersion)
{
	CInternetSession* lpInetSession = NULL;
	CFtpConnection* lpFtpConnection = NULL;
	CInternetFile* pFile = NULL;
	lpInetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	if (NULL == lpInetSession)
	{
		return FALSE;
	}

	try
	{
		lpFtpConnection = lpInetSession->GetFtpConnection(strIp, strUser, strPwd, nPort);
		if (NULL == lpFtpConnection)
		{
			lpInetSession->Close();
			delete lpInetSession;
			return FALSE;
		}

		pFile = lpFtpConnection->OpenFile(strRemoteFile);
		if (NULL == pFile)
		{
			lpFtpConnection->Close();
			lpInetSession->Close();
			delete lpFtpConnection;
			delete lpInetSession;
			return FALSE;
		}

		char szBuf[MAX_PATH] = {};
		pFile->Read(szBuf, MAX_PATH);

		USES_CONVERSION;
		strVersion.Format(_T("%s"), A2T(szBuf));
	}
	catch(CInternetException* lpEx)
	{
		//lpEx->ReportError();
		lpEx->Delete();
		if (pFile != NULL)
		{
			pFile->Close();
			delete pFile;
			pFile = NULL;
		}
		if (lpFtpConnection != NULL)
		{
			lpFtpConnection->Close();
			delete lpFtpConnection;
			lpFtpConnection = NULL;
		}
		if (lpInetSession != NULL)
		{
			lpInetSession->Close();
			delete lpInetSession;
			lpInetSession = NULL;
		}
		return FALSE;
	}

	if (pFile != NULL)
	{
		pFile->Close();
		delete pFile;
		pFile = NULL;
	}
	if (lpFtpConnection != NULL)
	{
		lpFtpConnection->Close();
		delete lpFtpConnection;
		lpFtpConnection = NULL;
	}
	if (lpInetSession != NULL)
	{
		lpInetSession->Close();
		delete lpInetSession;
		lpInetSession = NULL;
	}

	return TRUE;
}

void CMainFrame::SyncDecFile(void)
{
	if (NULL == m_pPrjSqlite)
	{
		return;
	}

	CString strDecPath(m_strFilePath + _T(".dec"));
	if (!PathFileExists(strDecPath))
	{
		return;
	}
	m_pPrjSqlite->SaveProjectSqlite_CalcModuleInfo_Dec();
}

void CMainFrame::SaveProjectContain(void)
{
	CSaveVersionDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		SaveProjectOperate();
	}
}

void CMainFrame::SaveProjectOperate(void)
{
	CWaitCursor waitcursor;
	assert(m_strFilePath.GetLength() > 0);

	// change list_page groupid here
	SetPageListInfo();

	bool bSave = SaveProjectFile();

	if(m_wndFileView.GetDataAccess())
	{
		CString strServer = m_wndFileView.GetDataAccess()->GetServerAddress();
		if(strServer==L"localhost" || strServer==L"127.0.0.1")
		{
			Json::Value myRoot;
			CHttpOperation http(strServer, m_wndFileView.GetDataAccess()->GetServerPort(), _T("system/initProject"));
			CString strResult;
			bool bRet = http.HttpPostGetRequest( strResult);

			CHttpOperation http2(strServer, m_wndFileView.GetDataAccess()->GetServerPort(), _T("system/initFiles"));
			bool bRet2 = http2.HttpPostGetRequest( strResult);
		}
	}

	//SyncDecFile();
	if(bSave == true)//保存成功
	{
		SetSaveFlag(false);
		AfxMessageBox(L"保存成功");
	}
	else
		AfxMessageBox(L"保存失败");

	CommandManager::Instance()->ClearAllCommands();
}



void CMainFrame::OnVpointBackup()
{
	// TODO: Add your command handler code here
	m_wndFileView.OnVpointBackup();
}


void CMainFrame::OnVpointReload()
{
	// TODO: Add your command handler code here
	m_wndFileView.OnVpointReload();
}


void CMainFrame::OnUpdateVpointBackup(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(m_wndFileView.!=NULL);
	
}


void CMainFrame::OnUpdateVpointReload(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(m_wndFileView.m_pDataAccess!=NULL);

}


void CMainFrame::OnRestartDomcore()
{
	// TODO: Add your command handler code here
	/*CString strSys;
	GetSysPath(strSys);


	if(m_wndFileView.GetDataAccess()==NULL)
	{
	AfxMessageBox(_T("请先连接core进入在线调试"));
	return;
	}

	wstring wstrPath = m_wndFileView.GetDataAccess()->ReadOrCreateCoreDebugItemValue(_T("corefilepath"));
	CString strPath = wstrPath.c_str();
	CString	strDir;
	strDir = strPath.Left(strPath.ReverseFind(_T('\\'))+1);

	KillProcessByName(_T("domcore.exe"));

	CString strCoreExePath = strDir+ _T("domcore.exe");
	int nRet = (int)::ShellExecute(NULL,L"open",strCoreExePath,NULL,NULL,SW_SHOWNORMAL);*/

	if(m_wndFileView.GetDataAccess()==NULL)
	{
		AfxMessageBox(_T("请先连接core进入在线调试"));
		return;
	}

	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("警告"), _T("本操作将影响系统后台, 是否确定?"), FALSE);
	if(WarnDlg.DoModal()!=IDOK)
	{
		return ;
	}

	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strCalcIp, m_nCalcPort, _T("dom/restartDomCore"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequest( strResult);

	CString strTips;

	if(bRet && AnalysisResponse(strResult))
	{
		//记录一个重启log
		CString cstrLogPath;
		Project::Tools::GetCoreLogPath(cstrLogPath);
		cstrLogPath += L"\\CRITICAL.txt";
		SYSTEMTIME stNow;
		GetLocalTime(&stNow);
		wstring strTime;
		Project::Tools::SysTime_To_String(stNow,strTime);
		CString cstrLogContents(strTime.c_str());
		cstrLogContents += L"\tFactory\tdomcore\t Menu Restart\r\n";
		CStdioFile fileCtl;
		if(fileCtl.Open(cstrLogPath , CFile::modeWrite | CFile::typeText | CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeCreate) == TRUE)
		{

			fileCtl.SeekToEnd();
			fileCtl.WriteString(cstrLogContents);
			fileCtl.Close();
		}

		strTips = _T("重启core成功！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	else
	{
		strTips = _T("重启core失败！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("警告"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}

	

	return;

}


void CMainFrame::OnStopDomcore()
{
	// TODO: Add your command handler code here
	if(m_wndFileView.GetDataAccess()==NULL)
	{
		AfxMessageBox(_T("请先连接core进入在线调试"));
		return;
	}

	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("警告"), _T("本操作将影响系统后台, 是否确定?"), FALSE);
	if(WarnDlg.DoModal()!=IDOK)
	{
		return ;
	}

	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strCalcIp, m_nCalcPort, _T("dom/destroyDomCore"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequest( strResult);
	CString strShow;
	CString strRet(_T("\r\n")); 
	CString strTips;
	if(bRet && AnalysisResponse(strResult))
	{
		strTips = _T("重启成功！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	else
	{
		strTips = _T("重启失败！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("警告"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}

	return;

}


void CMainFrame::OnRestartDomlogic()
{
	// TODO: Add your command handler code here
	if(m_wndFileView.GetDataAccess()==NULL)
	{
		AfxMessageBox(_T("请先连接core进入在线调试"));
		return;
	}

	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("警告"), _T("本操作将影响系统后台, 是否确定?"), FALSE);
	if(WarnDlg.DoModal()!=IDOK)
	{
		return ;
	}

	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strCalcIp, m_nCalcPort, _T("dom/restartDomLogic"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequest(strResult);
	CString strShow;
	CString strRet(_T("\r\n")); 
	CString strTips;
	if(bRet && AnalysisResponse(strResult))
	{
		//记录一个重启log
		CString cstrLogPath;
		Project::Tools::GetCoreLogPath(cstrLogPath);
		cstrLogPath += L"\\CRITICAL.txt";
		SYSTEMTIME stNow;
		GetLocalTime(&stNow);
		wstring strTime;
		Project::Tools::SysTime_To_String(stNow,strTime);
		CString cstrLogContents(strTime.c_str());
		cstrLogContents += L"\tFactory\tdomlogic\t Menu Restart\r\n";
		CStdioFile fileCtl;
		if(fileCtl.Open(cstrLogPath , CFile::modeWrite | CFile::typeText | CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeCreate) == TRUE)
		{

			fileCtl.SeekToEnd();
			fileCtl.WriteString(cstrLogContents);
			fileCtl.Close();
		}

		strTips = _T("重启成功！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	else
	{
		strTips = _T("重启失败！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("警告"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}

	return;

}


void CMainFrame::OnStopDomlogic()
{
	// TODO: Add your command handler code here
	if(m_wndFileView.GetDataAccess()==NULL)
	{
		AfxMessageBox(_T("请先连接core进入在线调试"));
		return;
	}

	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("警告"), _T("本操作将影响系统后台, 是否确定?"), FALSE);
	if(WarnDlg.DoModal()!=IDOK)
	{
		return ;
	}

	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strCalcIp, m_nCalcPort, _T("dom/destroyDomLogic"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequest( strResult);
	CString strShow;
	CString strRet(_T("\r\n")); 
	CString strTips;
	if(bRet && AnalysisResponse(strResult))
	{
		strTips = _T("重启成功！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	else
	{
		strTips = _T("重启失败！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("警告"), strTips, TRUE);
		WarnDlg2.DoModal();


		return;
	}

	return;
}


void CMainFrame::OnRestartDomall()
{
	// TODO: Add your command handler code here
	if(m_wndFileView.GetDataAccess()==NULL)
	{
		AfxMessageBox(_T("请先连接core进入在线调试"));
		return;
	}
	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("警告"), _T("本操作将影响系统后台, 是否确定?"), FALSE);
	if(WarnDlg.DoModal()!=IDOK)
	{
		return ;
	}

	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strCalcIp, m_nCalcPort, _T("dom/restartDomAll"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequest(strResult);
	CString strShow;
	CString strRet(_T("\r\n")); 
	CString strTips;
	if(bRet && AnalysisResponse(strResult))
	{

		//记录一个重启log
		CString cstrLogPath;
		Project::Tools::GetCoreLogPath(cstrLogPath);
		cstrLogPath += L"\\CRITICAL.txt";
		SYSTEMTIME stNow;
		GetLocalTime(&stNow);
		wstring strTime;
		Project::Tools::SysTime_To_String(stNow,strTime);
		CString cstrLogContents(strTime.c_str());
		cstrLogContents += L"\tFactory\tALL\t Menu Restart\r\n";
		CStdioFile fileCtl;
		if(fileCtl.Open(cstrLogPath , CFile::modeWrite | CFile::typeText | CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeCreate) == TRUE)
		{

			fileCtl.SeekToEnd();
			fileCtl.WriteString(cstrLogContents);
			fileCtl.Close();
		}

		strTips = _T("重启成功！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	else
	{
		strTips = _T("重启失败！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("警告"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}

	return;
}


void CMainFrame::OnStopDomall()
{
	// TODO: Add your command handler code here
	if(m_wndFileView.GetDataAccess()==NULL)
	{
		AfxMessageBox(_T("请先连接core进入在线调试"));
		return;
	}

	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("警告"), _T("本操作将影响系统后台, 是否确定?"), FALSE);
	if(WarnDlg.DoModal()!=IDOK)
	{
		return ;
	}

	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strCalcIp, m_nCalcPort, _T("dom/destroyDomAll"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequest(strResult);
	CString strShow;
	CString strRet(_T("\r\n")); 
	CString strTips;
	if(bRet && AnalysisResponse(strResult))
	{

		//记录一个重启log
		CString cstrLogPath;
		Project::Tools::GetCoreLogPath(cstrLogPath);
		cstrLogPath += L"\\CRITICAL.txt";
		SYSTEMTIME stNow;
		GetLocalTime(&stNow);
		wstring strTime;
		Project::Tools::SysTime_To_String(stNow,strTime);
		CString cstrLogContents(strTime.c_str());
		cstrLogContents += L"\tFactory\tALL\t Menu Stop\r\n";
		CStdioFile fileCtl;
		if(fileCtl.Open(cstrLogPath , CFile::modeWrite | CFile::typeText | CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeCreate) == TRUE)
		{

			fileCtl.SeekToEnd();
			fileCtl.WriteString(cstrLogContents);
			fileCtl.Close();
		}

		strTips = _T("重启成功！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	else
	{
		strTips = _T("重启失败！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("警告"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	
	return;
}


void CMainFrame::OnUpdateRestartDomcore(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


void CMainFrame::OnUpdateStopDomcore(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


void CMainFrame::OnUpdateRestartDomlogic(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


void CMainFrame::OnUpdateStopDomlogic(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


void CMainFrame::OnUpdateRestartDomall(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


void CMainFrame::OnUpdateStopDomall(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


// [Added by thinkhy 09/12/20]
// Description: Kill process(es) by PID.
// Reference:   http://www.vckbase.com/document/viewdoc/?id=1882
// RETVALUE:    SUCCESS   TRUE
//              FAILED    FALSE
BOOL CMainFrame::KillProcess(DWORD dwPid)
{
	HANDLE hPrc;

	if( 0 == dwPid) return FALSE;

	hPrc = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwPid);  // Opens handle to the process.

	if( !TerminateProcess(hPrc,0) ) // Terminates a process.
	{
		CloseHandle( hPrc );
		return FALSE;
	}
	else
		WaitForSingleObject(hPrc, 2000); // At most ,waite 2000  millisecond.

	CloseHandle(hPrc);
	return TRUE;
}


// [Added by thinkhy 09/12/20]
// Description: Kill process(es) by Name.
// Reference:   http://bbs.51testing.com/thread-65884-1-1.html
// RETVALUE:    SUCCESS   TRUE
//              FAILED    FALSE
BOOL CMainFrame::KillProcessByName(const TCHAR *lpszProcessName) {
	unsigned int   pid = -1;
	BOOL    retval = TRUE;

	if (lpszProcessName == NULL)
		return -1;

	DWORD dwRet = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS,0 );
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof( PROCESSENTRY32 );
	int flag = Process32First( hSnapshot, &processInfo );

	// Find the process with name as same as lpszProcessName
	while (flag != 0)
	{
		if (_tcscmp(processInfo.szExeFile, lpszProcessName) == 0) {
			// Terminate the process.
			pid = processInfo.th32ProcessID;
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);

			if (TerminateProcess(hProcess, 0) != TRUE) { // Failed to terminate it.
				retval = FALSE;
				break;
			}
		}

		flag = Process32Next(hSnapshot, &processInfo); 
	} // while (flag != 0)

	CloseHandle(hSnapshot);

	if (pid == -1)
		return FALSE;

	return retval;
}


void CMainFrame::OnLoginCore()
{
	// TODO: Add your command handler code here

	m_wndFileView.OnMenuConnDebug();
}


void CMainFrame::OnUpdateLoginCore(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}


bool CMainFrame::AnalysisResponse(CString strResponse)
{
	if(strResponse.GetLength()==0)
	{
		return false;
	}
	

	Json::Reader jsReader;
	string strResult_utf8;
	Project::Tools::WideCharToUtf8(strResponse.GetString(), strResult_utf8);
	Json::Value myResponse;
	if (!jsReader.parse(strResult_utf8, myResponse))
	{
		return false;
	}

	if(myResponse.isObject())
	{
		Json::Value bStatus = myResponse["status"];
		if(bStatus.isBool())
		{
			if(bStatus.asBool()==true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

void CMainFrame::OnModeSite()
{
	// TODO: Add your command handler code here
	CWarningDlg dlg;
	dlg.InitWarn(_T("提示"), _T("切换到现场模式后core将重启，重启后数据将与实际设备交互，可能需要等待1分钟左右，是否确认？"));
	if(dlg.DoModal()!=IDOK)
		return;

	
	m_wndFileView.GetDataAccess()->WriteCoreDebugItemValue(_T("sitemode"), L"1");
	Sleep(3000);

	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strCalcIp, m_nCalcPort, _T("dom/destroyDomCore"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequest(strResult);
	CString strShow;
	CString strRet(_T("\r\n")); 
	CString strTips;
	if(bRet && AnalysisResponse(strResult))
	{
		strTips = _T("重启成功！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	else
	{
		strTips = _T("重启失败！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("警告"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	
}


void CMainFrame::OnModeSumulate()
{
	// TODO: Add your command handler code here

	m_wndFileView.GetDataAccess()->WriteCoreDebugItemValue(_T("sitemode"), L"0");
	Sleep(3000);

	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strCalcIp, m_nCalcPort, _T("dom/destroyDomCore"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequest(strResult);
	CString strShow;
	CString strRet(_T("\r\n")); 
	CString strTips;
	if(bRet && AnalysisResponse(strResult))
	{
		strTips = _T("重启core成功！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("提示"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}
	else
	{
		strTips = _T("重启core失败！");
		CWarningDlg WarnDlg2;
		WarnDlg2.InitWarn(_T("警告"), strTips, TRUE);
		WarnDlg2.DoModal();
		return;
	}

}


void CMainFrame::OnUpdateModeSumulate(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
	if(m_wndFileView.GetDataAccess())
	{
		wstring strMode = m_wndFileView.GetDataAccess()->ReadOrCreateCoreDebugItemValue(_T("sitemode"));
		pCmdUI->SetCheck(strMode==L"0");
	}
}


void CMainFrame::OnUpdateModeSite(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
	if(m_wndFileView.GetDataAccess())
	{
		wstring strMode = m_wndFileView.GetDataAccess()->ReadOrCreateCoreDebugItemValue(_T("sitemode"));
		pCmdUI->SetCheck(strMode==L"1");
	}
}


void CMainFrame::OnOprecordHistory()
{
	// TODO: Add your command handler code here
	COpRecordHistoryDlg dlg;
	dlg.m_strServer = m_wndFileView.GetDataAccess()->GetServerAddress();
	dlg.m_nServerPort = m_wndFileView.GetDataAccess()->GetServerPort();

	dlg.DoModal();

}


void CMainFrame::OnUpdateOprecordHistory(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


void CMainFrame::OnSampleManager()
{
	// TODO: Add your command handler code here
	CDataSampleManageDlg dlg;
	dlg.m_strServer = m_wndFileView.GetDataAccess()->GetServerAddress();
	dlg.m_nServerPort = m_wndFileView.GetDataAccess()->GetServerPort();
	dlg.DoModal();
}


void CMainFrame::OnUpdateSampleManager(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}

void CMainFrame::OnSearch()
{
	// TODO: Add your command handler code here
#if 0
	CSearchLogicOutputDlg dlg;
	dlg.m_strServer = m_wndFileView.GetDataAccess()->GetServerAddress();
	dlg.m_nServerPort = m_wndFileView.GetDataAccess()->GetServerPort();
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	dlg.m_pLogicManager =pView->m_PageDlg->m_paneArbiter.m_pLogicManager;
	
	dlg.DoModal();
#else
	CSearchLogicOutputDlg* dlg = new CSearchLogicOutputDlg;
	dlg->m_strServer = m_wndFileView.GetDataAccess()->GetServerAddress();
	dlg->m_nServerPort = m_wndFileView.GetDataAccess()->GetServerPort();
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	dlg->m_pLogicManager =pView->m_PageDlg->m_paneArbiter.m_pLogicManager;

	dlg->Create(IDD_DIALOG_SEARCH_LOGIC_OUTPUT);
	dlg->ShowWindow(SW_SHOW);
#endif
}


void CMainFrame::OnUpdateSearch(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}



void CMainFrame::OnCoreStatus()
{
	CCoreStatusDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnUpdateCoreStatus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


void CMainFrame::OnImportHistory()
{
//	// TODO: Add your command handler code here
	CImportHistoryDataDlg dlg;
	dlg.m_strServerIP = m_wndFileView.GetDataAccess()->GetServerAddress();
	dlg.m_nServerPort = m_wndFileView.GetDataAccess()->GetServerPort();
	dlg.DoModal();
}


void CMainFrame::OnUpdateImportHistory(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}

void CMainFrame::OnHistoryExport()
{
	// TODO: Add your command handler code here
	CHistoryDataDownloadDlg dlg;
	dlg.m_pFrame = this;
	COleDateTime tstart = COleDateTime::GetCurrentTime()-COleDateTimeSpan(10,0,0,0);
	dlg.m_DateFrom.SetDateTime(tstart.GetYear(), tstart.GetMonth(), tstart.GetDay(), 0,0,0);
	COleDateTime tnow = COleDateTime::GetCurrentTime();
	dlg.m_DateTo.SetDateTime(tnow.GetYear(), tnow.GetMonth(), tnow.GetDay(), 0,0,0);

	dlg.m_TimeFrom = dlg.m_DateFrom;
	dlg.m_TimeTo = dlg.m_DateTo;
	dlg.DoModal();
}


void CMainFrame::OnUpdateHistoryExport(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL && m_pPrjSqlite!=NULL);
}

void CMainFrame::OnChangePointNameForHistoricalData()
{
	// TODO: Add your command handler code here
	CChangePointNameForHistoricalDataDlg dlg;
	dlg.Set_m_pDataAccess(m_wndFileView.GetDataAccess());
	dlg.DoModal();
}

void CMainFrame::OnUpdateChangePointNameForHistoricalData(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL && m_pPrjSqlite!=NULL);
}

void CMainFrame::OnUpdateWarningConfigRestore(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL && m_pPrjSqlite!=NULL);
}

void CMainFrame::OnWarningConfigRestore()
{
	// TODO: Add your command handler code here

	CString strFileNameAbs;
	CFileDialog dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xlsx)|*.xlsx|"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		//pFrame->m_pPrjSqlite->DeleteAllPoint();
		CString strFilePath = dlg.GetPathName();
		strFileNameAbs= dlg.GetPathName();
	}

	CString strServerIP = m_wndFileView.GetDataAccess()->GetServerAddress();
	int nServerPort = m_wndFileView.GetDataAccess()->GetServerPort();
	CHttpOperation http(strServerIP, nServerPort, _T("warning/uploadConfigFile"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	CString strErrReturn = http.UploadPic(strFileNameAbs, strServerIP, nServerPort, _T("warning/uploadConfigFile")); 

	if(strErrReturn.GetLength()>0)
	{
		AfxMessageBox(_T("导入失败:") + strErrReturn);
	}
	else
	{

		AfxMessageBox(_T("导入成功!"));
	}
}


void CMainFrame::OnWarningConfigBackup()
{
	// TODO: Add your command handler code here
}

CString CMainFrame::GetMySQLDatabaseName()
{
	if(m_strMySQLDatabaseName.GetLength()<=0)
	{
		m_strMySQLDatabaseName = GetDatabaseNameFromPysite(m_strCalcIp).c_str();
	}

	return m_strMySQLDatabaseName;
}

wstring CMainFrame::GetDatabaseNameFromPysite(CString strIP)
{
	CHttpOperation http(strIP, 5000, _T("db/getDatabaseName"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();
	CString strResult;
	//请求接口
	bool bRet= http.HttpPostGetRequest(strResult);
	wstring wstrDefaultDBName = L"beopdata";

	//拿到json数据里的进度信息，字符串
	if(bRet)
	{
		Json::Reader jsReader;
		string strResult_utf8;
		Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
		Json::Value myReturnRoot;
		if (!jsReader.parse(strResult_utf8, myReturnRoot))
		{
			return wstrDefaultDBName;
		}
		else
		{
			if(myReturnRoot.isObject())
			{
				Json::Value myData =  myReturnRoot["data"];
				if(myData.isObject())
				{
					string strDBName = myData["dbname"].asString();
					wstring wstrDBName;
					Project::Tools::UTF8ToWideChar(strDBName, wstrDBName);

					return wstrDBName;
				}


			}
		}
	}

	return wstrDefaultDBName;
}


void CMainFrame::On4dbUploadToCloud()
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(L"是否确认将本机core目录下的项目4db文件上传至云端并更新云平台项目界面?",MB_YESNO) == IDYES)
	{
		CString strTemplatePath;
		wstring wstr = _T("");
		CString strDir;
		TCHAR szAppDir[MAX_PATH] = {0};
		GetModuleFileName(NULL, szAppDir, MAX_PATH);
		PathRemoveFileSpec(szAppDir);
		CString strFilePath;
		strFilePath.Format(_T("%s\\../core/domUpload/domUpload.exe"), szAppDir);


		int nRet = (int)::ShellExecute(NULL,L"open",strFilePath,NULL,NULL,SW_SHOWNORMAL);
		if(nRet <= 32)
		{
			AfxMessageBox(L"文件上传驱动核心程序不存在，请升级core软件！");
			return;
		}
	}

}


void CMainFrame::On4dbUpdateFromCloud()
{
	// TODO: Add your command handler code here

	if(AfxMessageBox(L"是否确认从云端更新最新项目文件版本并覆盖本机core目录下的项目4db文件?",MB_YESNO) == IDYES)
	{
		CString strTemplatePath;
		wstring wstr = _T("");
		CString strDir;
		TCHAR szAppDir[MAX_PATH] = {0};
		GetModuleFileName(NULL, szAppDir, MAX_PATH);
		PathRemoveFileSpec(szAppDir);
		CString strFilePath;
		strFilePath.Format(_T("%s\\../core/domUpload/domDownload.exe"), szAppDir);


		int nRet = (int)::ShellExecute(NULL,L"open",strFilePath,NULL,NULL,SW_SHOWNORMAL);
		if(nRet <= 32)
		{
			AfxMessageBox(L"文件下载驱动核心程序不存在，请升级core软件！");
			return;
		}
	}
}


void CMainFrame::OnUpdate4dbUploadToCloud(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ProjectOpen);
}


void CMainFrame::OnViewPointPane()
{
	// TODO: Add your command handler code here
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	pView->m_PageDlg->m_pPanePoint->ShowWindow(TRUE);
}


void CMainFrame::OnUpdateViewPointPane(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL && m_pPrjSqlite!=NULL);
}


void CMainFrame::OnGenStandardPoints()
{
	// TODO: Add your command handler code here
	CGenStandardPointNameDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnUpdateGenStandardPoints(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL && m_pPrjSqlite!=NULL);
}


void CMainFrame::OnHisdataManager()
{
	// TODO: Add your command handler code here
	CPointHisdataDlg dlg;
	dlg.m_strServer = m_strCalcIp;
	dlg.m_nServerPort = 5000;
	dlg.DoModal();
}


void CMainFrame::OnUpdateHisdataManager(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL && m_pPrjSqlite!=NULL);
}


void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO: Add your specialized code here and/or call the base class
	CString csAppName;

	csAppName.Format(AFX_IDS_APP_TITLE);

	SetWindowText(csAppName);
	//CFrameWndEx::OnUpdateFrameTitle(bAddToTitle);
}


void CMainFrame::OnBatchModifyPipeWidth()
{
	// TODO: Add your command handler code here

	CInputPipeWidthDlg dlg;
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}

	if(dlg.m_nPipeWidth<3 || dlg.m_nPipeWidth>15)
		return;


	list<CEqmDrawPipe*> listPipe(m_DrawContext.pPage->m_pipelist);
	list<CEqmDrawPipe*>::const_iterator itPipe;
	vector<Gdiplus::PointF> vecPoint;
	vector<Gdiplus::PointF>::const_iterator itPt;
	CPoint ptBegin;
	CPoint ptEnd;
	int nFirst = 0;
	int nLast = 0;
	for (itPipe=listPipe.begin(); itPipe!=listPipe.end(); itPipe++)
	{
		int nWidth = (*itPipe)->GetPipeWidth();
		if(nWidth)
		(*itPipe)->SetPipeWidth(dlg.m_nPipeWidth);
		(*itPipe)->SetPipeSpeed(dlg.m_nSpeed);
		(*itPipe)->SetPipeDenSity(dlg.m_nFlowDensityIndex);
		(*itPipe)->SetWaterFashion(dlg.m_nWaterFashion);
	}

	m_DrawContext.pPage->m_bNeedSave = true;
	
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(GetActiveView());
	if(pView && pView->m_PageDlg)
	{

		pView->m_PageDlg->m_dlgCanvas.Invalidate(TRUE);
	}
}


void CMainFrame::OnUpdateBatchModifyPipeWidth(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(m_pPrjSqlite!=NULL);
}

#include "CheckMissImageDlg.h"
void CMainFrame::OnCheckMissImage()
{
	CCheckMissImageDlg* dlg = new CCheckMissImageDlg;
	dlg->Create(IDD_DIALOG_CHECK_MISS_IMAGE, NULL);
	dlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateCheckMissImage(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pPrjSqlite!=NULL);
}

void CMainFrame::OnCloudUpdateSyslogic()
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(L"是否确认从云端更新所有策略库最新版本到本地（注意：不影响工程中的所有库文件版本）?",MB_YESNO) == IDYES)
	{
		CString strTemplatePath;
		wstring wstr = _T("");
		CString strDir;
		TCHAR szAppDir[MAX_PATH] = {0};
		GetModuleFileName(NULL, szAppDir, MAX_PATH);
		PathRemoveFileSpec(szAppDir);
		CString strFilePath;
		strFilePath.Format(_T("%s\\syslogic"), szAppDir);

		CString strServerIP = m_wndFileView.GetDataAccess()->GetServerAddress();
		int nServerPort = m_wndFileView.GetDataAccess()->GetServerPort();

		CString strLogResult;
		Json::Value myRoot;
		myRoot["syslogicPath"]=Project::Tools::WideCharToUtf8(strFilePath);
		string szBuf = myRoot.toStyledString();
		CHttpOperation http_log(strServerIP, nServerPort, _T("logic/update_syslogic"));
		http_log.SetInternetTimeOut(300000,300000);
		bool bRet_log = http_log.HttpPostGetRequestEx(0, szBuf.data(), strLogResult,  _T("Content-Type: application/json"));
		if ( bRet_log)
		{
			Json::Reader jsReader;
			Json::Value returnMsg;
			//转json格式数据只允许string
			wstring wstrLogResult = strLogResult.GetString();
			string sLogResult = Project::Tools::tstringToString(wstrLogResult);
			jsReader.parse(sLogResult,returnMsg);
			//返回信息进行转码
			
			CString strMsg;
			strMsg.Format(_T("策略文件库最新版本更新成功，最新策略文件位置:%s"), strFilePath);
			AfxMessageBox(strMsg);
		}
		else
		{
			AfxMessageBox(_T("策略文件库更新超时失败，后台应用dompysite连接超时失败或本机连接云超时失败。"));
		}
	}
}


void CMainFrame::OnUpdateCloudUpdateSyslogic(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


void CMainFrame::OnViewCoreVersions()
{
	// TODO: Add your command handler code here
	CCoreVersionDlg Dlg;
	wstring str1, str2, str3;
	m_wndFileView.GetDataAccess()->GetCoreVersion(str1, str2, str3);
	Dlg.m_strCoreVersion	= str1.c_str();
	Dlg.m_strLogicVersion	= str2.c_str();
	if(Dlg.m_strLogicVersion.Left(1)==_T("V"))
	{

		Dlg.m_strLogicVersion = Dlg.m_strLogicVersion.Mid(1);
	}
	Dlg.m_strPysiteVersion	= str3.c_str();
	Dlg.DoModal();

}


void CMainFrame::OnUpdateViewCoreVersions(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}


void CMainFrame::OnAddVpointBatch()
{
	// TODO: 在此添加命令处理程序代码
	CAddVPointBatchDialog dlg(this);
	if(dlg.DoModal()==IDOK)
	{
		CString str; //定义一个变量str
		str = dlg.m_strVpointBatchlist; //获取编辑框文本到str

		vector<CString> strVpointBatchlist;
		Project::Tools::SplitStringByChar(str,',', strVpointBatchlist);

		for(int i=0;i<strVpointBatchlist.size();i++)
		{
			int nID = m_pOnlinePrjSqlite->GetMaxIDInPointTable()+1;
			string strNameUtf8;
		
			Project::Tools::WideCharToUtf8(strVpointBatchlist[i].GetString(), strNameUtf8);
			m_pOnlinePrjSqlite->InsertRecordToOPCPoint(nID,0, strNameUtf8.data(),"vpoint",0, "","",0,0,0,0,"","","","","","","","","","","2","","","","",0,0,0);
		}
		
		CUPPCServerDataAccess *pdbAccess = m_wndFileView.GetDataAccess();

		if(pdbAccess)
		{
			if(pdbAccess->WriteCoreDebugItemValue(_T("point_vpoint_add"), str.GetString()))//向数据库unit01表写入数据
			{
			}
		}
		
		
	}


	/*
	unit01表， unitproperty01='point_vpoint_add', unitproperty02= 'aaa,bb,cc'
	*/
}

/*
 *函数名：OnCompletionData
 *函数描述：脚本型计算点历史数据补算，整理参数并使用这些参数去执行domRepairCalculationHistory.exe
*/
void CMainFrame::OnCompletionData()
{
	// TODO: 在此添加命令处理程序代码
	CCompletionData dlg(this);

	if(dlg.DoModal())
	{
		if( dlg.m_CompletionDataList.GetLength() > 0)
		{
			//补算点参数 分离和拼接
			std::vector<CString> str;
			Project::Tools::SplitStringByChar(dlg.m_CompletionDataList, ',',str);//字符串分离
			//删除不需要的字符
			std::vector<CString>::iterator iter= std::find(str.begin(),str.end(),_T(""));
			for(;iter!=str.end();iter = std::find(str.begin(),str.end(),_T("")))
			{
					str.erase(iter);
			}

			iter= std::find(str.begin(),str.end(),_T(" "));
			for(;iter!=str.end();iter = std::find(str.begin(),str.end(),_T(" ")))
			{
				str.erase(iter);
			}

			iter= std::find(str.begin(),str.end(),_T("\t"));
			for(;iter!=str.end();iter = std::find(str.begin(),str.end(),_T("\t")))
			{
				str.erase(iter);
			}
			iter= std::find(str.begin(),str.end(),_T("\n"));
			for(;iter!=str.end();iter = std::find(str.begin(),str.end(),_T("\n")))
			{
				str.erase(iter);
			}
			CString strCompletionDataList;
			for( int i=0;i<str.size()-1;i++ )//参数拼接
			{
				strCompletionDataList += str[i]+_T(" ");
			}
			strCompletionDataList += str[str.size()-1];//拼接最后一项，为不加空格

			/*========================检查补算点的存在====================================*/
			if (m_pPointMapDlg == NULL)
			{
				m_pPointMapDlg = new CDataPointConfigDlg(m_plcpointmap);//创建一个对象
				m_pPointMapDlg->Create(IDD_DATA_POINT_CONFIG, this);//加载出点表管理的对话框
			}
			m_pPrjSqlite->LoadProjectSqlite_opcid3(); //reload point table
			m_pPointMapDlg->SetPointMap(m_plcpointmap);
			m_pPointMapDlg->MakeAllList();
			vector <CString> findSuccess;
			vector <CString> findFail;
			bool finState = false;
			//从对话框渠道的点去点表中查找
			for (int list_count = 0;list_count < str.size();list_count++)
			{
				finState = false;
				for (int search_count=0;search_count < m_pPointMapDlg->m_allentrylist.size();search_count++)
				{
					wstring strPonitName = m_pPointMapDlg->m_allentrylist.at(search_count).GetShortName();
					if (str.at(list_count).GetString() == strPonitName)
					{
						finState = true;
						findSuccess.push_back(str.at(list_count));
						break;
					}
				}
				if (!finState)
				{
					findFail.push_back(str.at(list_count));
				}
			}
			if (findFail.size()>0)
			{
				CString FailMessage = _T("以下点名未在点表中找到：\n");
				FailMessage += findFail.at(0);
				for(int i=1;i<findFail.size();i++)
				{
					FailMessage = FailMessage+_T(", ")+findFail.at(i);
				}
				FailMessage+=_T(";");
				AfxMessageBox(FailMessage);
				return;
			}
			/*============================================================================*/

			//日期起始参数
			CString strDateStart = dlg.m_tFrom.Format(_T("%Y-%m-%d"));
			CString strDateEnd = dlg.m_tTo.Format(_T("%Y-%m-%d"));
			CString strDateInterval;

			//时间分割参数
			switch(dlg.m_nPeriodType)
			{
				case 0:strDateInterval = "m1";break;
				case 1:strDateInterval = "m5";break;
				case 2:strDateInterval = "h1";break;
				case 3:strDateInterval = "d1";break;
				default:break;
			}

			//配置执行文件需要的参数
			CString temp_File = _T("domRepairCalculationHistory.exe");//文件名
			CString lpDirectory;//文件地址
			Project::Tools::GetSysPath(lpDirectory);
			lpDirectory = lpDirectory.Left(lpDirectory.Find(_T("factory")));
			lpDirectory+=_T("core\\domRepairCalculationHistory\\");//文件执行位置
			CString lpFile = lpDirectory + temp_File;
			//文件执行参数
			CString lpParameters;
			lpParameters.Format(_T("%s %s %s %s"), strDateStart, strDateEnd,strDateInterval, strCompletionDataList);//参数
			/*执行文件*/
			//HINSTANCE m_exeReturn = ShellExecute(NULL,_T("open"),lpFile,lpParameters,lpDirectory,SW_SHOWNORMAL);//最后一个参数代表执行时是否显示
			HINSTANCE m_exeReturn = ShellExecute(NULL,_T("open"),lpFile,lpParameters,lpDirectory,SW_SHOW);//最后一个参数代表执行时是否显示

			//执行结果判断
			DWORD exeReturn = (DWORD)m_exeReturn;
			if (exeReturn >= 32)
			{
				CString successMseeage = _T("补算列表：\n");
				successMseeage += findSuccess.at(0);
				for(int i=1;i<findSuccess.size();i++)
				{
					successMseeage = successMseeage+_T(", ")+findSuccess.at(i);
				}
				successMseeage+=_T(";");
				AfxMessageBox(_T("执行成功"));
				AfxMessageBox(successMseeage);
			}else 
			{
				switch(exeReturn)
				{
					case 0:AfxMessageBox(_T("内存不足"));break;
					case 2:AfxMessageBox(_T("没有找到执行文件"));break;
					case 3:AfxMessageBox(_T("路径名错误"));break;
					case 11:AfxMessageBox(_T("EXE文件无效"));break;
					case 26:AfxMessageBox(_T("发生共享错误"));break;
					case 27:AfxMessageBox(_T("文件名不完全或无效"));break;
					case 28:AfxMessageBox(_T("超时"));break;
					case 29:AfxMessageBox(_T("DDE 事务失败"));break;
					case 30:AfxMessageBox(_T("正在处理其他 DDE 事务而不能完成该 DDE 事务"));break;
					case 31:AfxMessageBox(_T("没有相关联的应用程序"));break;
					default:break;
				}
			}
			/*========调试完删除==========*/
			//AfxMessageBox(lpFile);
			//AfxMessageBox(lpDirectory);
			//AfxMessageBox(lpParameters);
			//CString result;
			//result.Format(_T("执行结果：%d"),exeReturn);
			//AfxMessageBox(result);
			/*========================*/
		}
		else
		{
			AfxMessageBox(_T("编辑框为空!"));
		}
	}
}


void CMainFrame::OnUpdateCompletionData(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_ProjectOpen);
}

#include "ExportHistoricalDataAtPreciseTimeDLg.h"
void CMainFrame::OnExportHistoricalDataAtPreciseTime()
{
	CExportHistoricalDataAtPreciseTimeDLg dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateExportHistoricalDataAtPreciseTime(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_wndFileView.GetDataAccess()!=NULL);
}

void CMainFrame::OnDatabaseRestore()
{
	AfxBeginThread(Thread_DatabaseRestore,(LPVOID)this);
}

#include "LoadingProjectDlg.h"
UINT Thread_DatabaseRestore(LPVOID pParam)
{
	vector<CString> cstrPathList;
	vector<CString> cstrNameList;

	CFileDialog dlg(TRUE, _T(""), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_EXPLORER , _T("zip (*.zip)|*.zip"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		POSITION pItem = /*NULL*/dlg.GetStartPosition();
		while(true)
		{
			CString tempPath = dlg.GetNextPathName(pItem);
			if(false == tempPath.Find(L".zip"))
				continue;
			cstrPathList.push_back(tempPath);
			cstrNameList.push_back(tempPath.Right(tempPath.GetLength() - (tempPath.ReverseFind(L'\\')+1)/*不需要'/'*/));
			if(pItem == NULL){
				break;
			}
		}
		if(cstrPathList.size() == 0)
			return 0;
		
		//进度条设置
		CLoadingProjectDlg progressDlg;
		progressDlg.Create(IDD_DIALOG_LOADING);
		progressDlg.m_ProgressCtrl.SetRange(0,100);
		int nPos = 100/cstrPathList.size();
		progressDlg.ShowWindow(SW_SHOW);

		//执行文件的地址不包括执行文件
		CString cstr_domMySQLRestore_Path;
		Project::Tools::GetCorePath(cstr_domMySQLRestore_Path);
		cstr_domMySQLRestore_Path += L"\\domMySQLRestore\\";
		//目标文件夹
		CString cstrZipNewPath = cstr_domMySQLRestore_Path + L"zipfiles\\";

		for(int i=0; i<cstrPathList.size();i++)
		{
			CopyFile(cstrPathList[i], cstrZipNewPath+cstrNameList[i], FALSE);

			//查看文件是否复制完成
			if(false == DirectoryOperations::CheckFileExist(cstrZipNewPath, cstrNameList[i]))
			{
				for(int i=0;i<120;i++)
				{
					if(true == DirectoryOperations::CheckFileExist(cstrZipNewPath, cstrNameList[i]))
					{ 
						break;
					}
					Sleep(500);
				}
			}
			progressDlg.m_ProgressCtrl.SetPos(nPos);
		}
		progressDlg.ShowWindow(SW_HIDE);

		//启动文件
		CString cstr_domMySQLRestoreExe_Path = cstr_domMySQLRestore_Path + L"domMySQLRestore.exe";		
		HINSTANCE m_exeReturn = ShellExecute(NULL,_T("open"),cstr_domMySQLRestoreExe_Path,NULL,cstr_domMySQLRestore_Path,SW_SHOW);//最后一个参数代表执行时是否显示
	}
	return 0;
}
void CMainFrame::OnUpdateDatabaseRestore(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnDatabaseBuckupManager()
{
	CDatabaseBackupManagerDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateDatabaseBuckupManager(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(TRUE);
}
