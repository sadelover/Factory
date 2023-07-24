/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：ProjectSqlite.h
* 文件标识：
* 摘    要：Sqlite数据库操作
*
* 取代版本：N 
* 原作者  ：
* 完成日期：N年N月N日
*/
//projectxml.h bird. 3.1
#pragma once
#pragma warning(disable:4800)
#include <string>
//#include "../Tools/vld.h"             //该注释如果打开，会导致应用程序运行非常慢
#include "DataPoint/LogicDataPoint.h"
#include "DataPoint/AlarmDataPoint.h"
#include "DataPoint/LogicPointManager.h"
#include "DataPoint/AlarmPointManager.h"
#include "EqmDrawProject.h"
#include "DataPoint/DataPointEntry.h"
#include "DataPoint/DataPointManager.h"
#include "DB_BasicIO/Sqlite/SqliteAcess.h"
#include "ComponentDraw/PageTemplateInfo.h"

#include "EqmDrawDiagnoseGraphic.h"
#include "EqmDrawDiagnoseGraphicPlus.h"
#include "EqmDrawAreaChange.h"
#include "EqmDrawHistoryTrend.h"
#include "EqmDrawMapNavigate.h"
#include "EqmDrawNavigateWindow.h"
#include "EqmDrawLevelRuler.h"
#include "EqmDrawDataAnalyse.h"
#include "..\Tools\Log\UseLog.h"
#include <set>
#include "../DB_BasicIO/RealTimeDataAccess.h"
const bool C_IF_SAVE_OPCID = true;
const bool C_IF_SAVE_LogicPt = true;
const bool C_IF_SAVE_AlarmPt = true;


class DLLEXPORT CProjectSqlite
{
public:
    CProjectSqlite(CEqmDrawProject& project, CDataPointManager& pointmap, 
						CLogicPointManager&  mLogicPtManager, CAlarmPointManager&   mAlarmPtManager, int nFontResolution = 128, E_OWNER eOwner = eFactory);
    virtual ~CProjectSqlite();

    enum E_VEC_TYPE_{
        VEC_TYPE = 0,  //mVecStrType
        VEC_SUB_TYPE,  //mVecStrSubType

        VEC_MAX, 
    };

    enum E_LOAD_OPC_ERROR{
        LOAD_OPC_ERR_OK = 0, 
        LOAD_OPC_ERR_FILE_EMPTY,
        LOAD_OPC_ERR_XML_PARSE,
        LOAD_OPC_ERR_DUPLICATE_ITEM,

        LOAD_OPC_ERR_MAX,
    };
    typedef vector<DataPointEntry> VEC_ENTRY;

	bool				ClearProjectConfig();
	bool				LoadProjectConfig();
	bool                UpdateDBSettingPara(DataBaseSetting &dbset, CString &strErr);
    wstring             LoadProjectSqlite(CProgressCtrl *pProgressCtrl = NULL, CStatic *pStatic = NULL); 
	wstring				LoadProjectPages();
	wstring				LoadPageDetails(CEqmDrawPage*	pPage,CProgressCtrl *pProgressCtrl = NULL, CStatic *pStatic = NULL);
	void				LoadPictureLibSqlite(const bool bDbSelect = true);
	void                LoadAnimationSqlite(const bool bDbSelect = true);
	void                FreePictureLibMem();
	void				FreeAnimationMem();
	void                FreePictureLibImageMem(void);
	void				FreeAnimationImageMem(void);
    bool                SaveProjectSqlite(string strEditSoftVersion); 
    void                ContinueSetEntryToMap();
	int					DeleteAllUsefulTables();
	int					DeleteProjectConfig();
    const VEC_ENTRY&    GetVecEntry() const;
    const VEC_ENTRY&    GetVecEntryDuplicate() const;
	const VEC_ENTRY&    GetVecEntryError() const;
	void				SetFilePath(const wchar_t* lpszFilePath);
	wstring				GetFilePath();
	Image*				GetBitmapByIDFromPictureLib(const int nID, const bool bDbSelect = true);
	int					GetPictureIDByName(const wchar_t* lpszName);
	const wchar_t*		GetPictureNameByID(const int nID);
	const bool			SaveMemBitmapToFile(const char* pBlock, const int nSize, const wchar_t* lpszFileName);
	//从m_vPictureList中获取一个没有使用的ID
	int					GetUnusedPictureID(void);
	PictureInfo*		GetPictureInfoByID(const int nPicID, const bool bDbSelect = true);
	PictureInfo*		GetPictureInfoByIDOfAnimation(const int nPicID, const bool bDbSelect = true);
	PictureInfo*		CompareAndReturnPictureInfoByBlock(const char* pBlock, const int nSize, const wchar_t* pName, const wstring strType_p, const wstring strType_e, const bool bDbSelect = true);
	AnimationInfo*		CompareAndReturnAnimationInfoByName(const wstring strName, const vector<PictureInfo> vecPicIDList,const bool bDbSelect = true);
	Image*				GetBitmapByIdFromAnimationImageLib(const int nId, const bool bDbSelect = true);
	int					InsertPictureData(const int nPicID, const char* lpszName, const char* lpszType,char* pBlock, int size, const bool bDbSelect = true, const int nBtnId = 0, const int nBtnSubId = 0, const char* lpszImgType = "png");
	int					InsertPictureAnimationData(const int nPicID, const char* lpszName, const char* lpszType,char* pBlock, int size, const bool bDbSelect = true);
	int					GetMaxLibImageNum(const bool bDbSelect = true);
	int					GetMaxLibImageBtnId(const bool bDbSelect);
	int					GetMaxLibAnimationNum(const bool bDbSelect = true);
	int					GetMaxLibImageAnimationNum(const bool bDbSelect = true);
	void				InsertToLibImage(PictureInfo PicInfo);
	void				InsertToLibImageAnimation(PictureInfo PicInfo);
	bool				IsPicNameValid(CString strName);
	int					InsertRecordToAnimationList(const int nID, const char* lpszName, const char* lpszAnimationList, const bool bDbSelect = true);
	wstring				InsertAnimationVec(const int nID, const CString strName, const CString strSeries);
	wstring				InsertAnimationList(const int nID, const CString strName, const CString strSeries);
	std::vector<CEqmDrawPage*>    GetPageIDList(int groupid);
	CEqmDrawPage *      GetCurPage();
	void				SetOwner(const E_OWNER eOwner);
	E_OWNER				GetOwner();
	void				GetDetailChartByIDs(const int nPageID, const int nElementID, vector<_Graph_Item_Property>& vecItem);
	void				GetDetailAreaChangeByIDs(const int nPageID, const int nElementID, vector<_tagAreaInfo>& vecItem);
	bool				IsPageExist(const int nID);
	bool				IsPointExist(const wstring strPointName);
	bool				AddPointEntry(DataPointEntry entry);


	vector<_Import_Setting>		ReadImportSetting();
	bool				InsertImportSetting(_Import_Setting Setting);
	bool				UpdateImportSettingByID(const int nID,const int bLoadPoint, const int bLoadImage);
	bool				FindImportSettingByPrjName(const string lpszName,_Import_Setting& setting);
	int					GetMaxImportSettingID();
	bool				DeleteBaseImageByID(const int nStartID, const int nEndID);
	bool				DeleteBasePointByID(const int nStartID, const int nEndID);
	vector<DataPointEntry> GetPointList();
	CDataPointManager	GetPointMap();
	bool				FindAndInsertNotExist();

	vector<ProjectCheckInfo>	GetAllUseImageInfo();
	vector<ProjectCheckInfo>	CompareAndReturnNotExist();
	vector<ProjectCheckInfo>	CompareAndReturnNotExistPoint();
	vector<ProjectCheckInfo>	CompareAndReturnOverlapElement();

	PictureInfo			GetAnimationPictureInfoByID(const int nPicID, const bool bDbSelect = true);

	bool				OutputSaveErr(const wstring strOperation, const char* err);
public:
	wstring				LoadGroupPageSqlite( );
	wstring				LoadPageDetails(CProgressCtrl *pProgressCtrl, CStatic *pStatic);
	wstring				LoadProjectSqlite_pages(CProgressCtrl *pProgressCtrl = NULL, CStatic *pStatic = NULL);
	wstring				LoadProjectSqlite_devices( );
	wstring				LoadProjectSqlite_pipes( );
	wstring				LoadProjectSqlite_texts(  );
	wstring				LoadProjectSqlite_graphics(  );
	void				LoadProjectSqlite_opcid3();
	wstring				LoadProjectSqlite_LogicPoint();
	void				LoadProjectSqlite_AlarmPoint();
	wstring				LoadProjectSqlite_DashBoard();
	wstring				LoadProjectSqlite_LineChart();
	wstring				LoadProjectSqlite_BarChart();
	wstring				LoadProjectSqlite_PieChart();
	wstring				LoadProjectSqlite_Bend();
	wstring				LoadProjectSqlite_Button();
	wstring				LoadProjectSqlite_TimeSelect();
	wstring				LoadProjectSqlite_TimeBar();
	wstring				LoadProjectSqlite_ProgressBar();
	wstring				LoadProjectSqlite_DiagnoseChart();
	wstring				LoadProjectSqlite_Polygon();
	wstring				LoadProjectSqlite_AreaChange();
	wstring				LoadProjectSqlite_ShellExecute();
	wstring				LoadProjectSqlite_EmbededPage();
	wstring				LoadProjectSqlite_LiquidLevel();
	wstring				LoadProjectSqlite_CheckButtonPlus();
	wstring				LoadProjectSqlite_Scrawl();
	wstring				LoadProjectSqlite_HistoryTrend();
	wstring				LoadProjectSqlite_EnergySaveRoi();
	wstring				LoadProjectSqlite_MapNavigate();
	//wstring				LoadProjectSqlite_StandEquip();
	wstring				LoadProjectSqlite_NavigateWindow();
	wstring				LoadProjectSqlite_LevelRuler();
	wstring				LoadProjectSqlite_DataAnalyse();
	wstring				LoadProjectSqlite_TemperDistri();
	wstring				LoadProjectSqlite_DataReport();
	wstring				LoadProjectSqlite_StraightLine();
	wstring				LoadProjectSqlite_Line();
	wstring				LoadProjectSqlite_Rectangle();
	wstring				LoadProjectSqlite_PlaneTempeDistr();
	wstring				LoadProjectSqlite_DottedLineFrm();
	wstring				LoadProjectSqlite_GeneralComponent();
	wstring				LoadAllProjectImageInfo();			//加载项目中所有用到图片的信息
	wstring				LoadAllProjectPointInfo();			//加载项目中所有用到点位的信息

	bool				SaveGroupPageSqlite( );
    bool				SaveProjectSqlite_pages(  );
    bool				SaveProjectSqlite_devices( );
    bool				SaveProjectSqlite_pipes(  );
    bool				SaveProjectSqlite_texts(  );
    bool				SaveProjectSqlite_graphics( );
	bool				SaveProjectSqlite_opcid3();
	void				SaveProjectSqlite_LogicPoint();
	void				SaveProjectSqlite_AlarmPoint();
	bool				SaveProjectSqlite_dashboard();
	bool				SaveProjectSqlite_LineChart();
	bool				SaveProjectSqlite_BarChart();
	bool				SaveProjectSqlite_PieChart();
	bool				SaveProjectSqlite_Bend();
	bool				SaveProjectSqlite_Button();
	bool				SaveProjectSqlite_TimeSelect();
	bool				SaveProjectSqlite_TimeBar();
	bool				SaveProjectSqlite_ProgressBar();
	bool				SaveProjectSqlite_DiagnoseChart();
	bool				SaveProjectSqlite_Polygon();
	bool				SaveProjectSqlite_AreaChange();
	bool				SaveProjectSqlite_ShellExecute();
	bool				SaveProjectSqlite_EmbededPage();
	bool				SaveProjectSqlite_LiquidLevel();
	bool				SaveProjectSqlite_CheckButtonPlus();
	bool				SaveProjectSqlite_Scrawl();
	bool				SaveProjectSqlite_HistoryTrend();
	bool				SaveProjectSqlite_EnergySaveRoi();
	bool				SaveProjectSqlite_MapNavigate();
	//void				SaveProjectSqlite_StandEquip();
	bool				SaveProjectSqlite_NavigateWindow();
	bool				SaveProjectSqlite_LevelRuler();
	bool				SaveProjectSqlite_DataAnalyse();
	bool				SaveProjectSqlite_TemperDistri();
	bool				SaveProjectSqlite_StraightLine();
	void				SaveProjectSqlite_Line();
	bool				SaveProjectSqlite_Rectangle();
	bool				SaveProjectSqlite_DataReport();
	bool				SaveProjectSqlite_PlaneTempeDistr();
	bool				SaveProjectSqlite_DottedLineFrm();
	bool				SaveProjectSqlite_GeneralComponent();

    bool				SaveProjectCommon(string strEditSoftVersion);
    void				InitTypeString();
    void				InitSubTypeString();
	void				SetPage(CEqmDrawPage *pPage);
	void				SetIsNeedLoadAllPages(bool bLoadAll);
	bool				GetIsNeedLoadAllPages();
	void				SetIsEnglishLanguage(bool bEnglish);
	bool				GetIsEnglishLanguage();
	void				SetImageLibPath(wstring strImageLibPath);

	void				SetOutPutLogFlag(bool bOutPutErrLog);

	//device图片信息
	wstring				MakeDevicePlayInfo(CEqmDrawDevice *pDevice);
    void				UpdateDuplicateVecEntry();
    int                 FindIndexFromVecString(E_VEC_TYPE_ eType, const string& str ) const;
    const std::string&  GetVecStringFromIdx(E_VEC_TYPE_ eType, UINT idx) const;
	bool				IsFisrtDigital(wstring strID);
	bool				CheckEntryValid(const DataPointEntry& entry, CString& strErr);

	AnimationInfo * GetAnimationByID(const int nID, const bool bDbSelect = true);

	string				GetBindInfoSqlite(const int nPageID, const int nElementID);
	string				GetEventInfoSqlite(const int nPageID, const int nElementID);
	void				GetEventAndBindInfoSqlite(const int nPageID, const int nElementID, string& strEvent, string& strBind);
	void				DeleteOnePicture(const int nID, const bool bDbSelect = true);
	void				DeleteOnePictureAndInsert(const int nID, PictureInfo info, const bool bDBSelect);
	void				DeleteOneAnimation(const int nID);
	void				DeleteOnePictureForAnimation(const int nID);
	int					GetMaxPageID();
	int					GetMaxElementID();
	wstring				ReplacePic(const int nID, vector<ImgReplaceInfo>& vecReplaceInfo);

	void				SaveInfoIntoSelectDb(void);
	void				SaveIntoSelectDbImageAndAnimation(const list<CEqmDrawDevice*>& listDev);	// save picture and animation
	wstring				SaveIntoSelectDbImage(const int nPicId);
	void				SaveIntoSelectDbAnimation(const int nPicId);

	void				SaveIntoSelectDbPipe(const list<CEqmDrawPipe*>& lstPipe);
	void				SaveIntoSelectDbText(const list<CEqmDrawText*>& lstText);
	void				SaveIntoSelectDbGraphic(const list<CEqmDrawGraphic*>& lstGraph);
	void				SaveIntoSelectDbTimeSelect(const list<CEqmDrawTimeSelect*>& lstTmSel);
	void				SaveIntoSelectDbTimeBar(const list<CEqmDrawTimeBar*>& lstTmBar);
	void				SaveIntoSelectDbProgressBar(const list<CEqmDrawProgressBar*>& lstProgBar);
	void				SaveIntoSelectDbDiagnoseGraphic(const list<CEqmDrawDiagnoseGraphic*>& lstDiagGraph);
	void				SaveIntoSelectDbDiagnoseGraphicPlus(const list<CEqmDrawDiagnoseGraphicPlus*>& lstDiagGraphPlus);
	void				SaveIntoSelectDbAreaChange(const list<CEqmDrawAreaChange*>& lstAreaChange);
	void				SaveIntoSelectDbShellExecute(const list<CEqmDrawShellExecute*>& lstShellExecute);
	void				SaveIntoSelectDbEmbededPage(const list<CEqmDrawEmbededPage*>& lstEmbedPage);
	void				SaveIntoSelectDbLiquidLevel(const list<CEqmDrawLiquidLevel*>& lstLiquLev);
	void				SaveIntoSelectDbButtonPlus(const list<CEqmDrawButtonPlus*>& lstBtnPlus);
	void				SaveIntoSelectDbScrawl(const list<CEqmDrawScrawl*>& lstScrawl);
	void				SaveIntoSelectDbBend(const list<CEqmDrawBend*>& lstBend);	//
	void				SaveIntoSelectDbDashBoard(const list<CEqmDrawDashBoard*>& lstDashBoard);	//
	void				SaveIntoSelectDbLineGraphic(const list<CEqmDrawLineGraphic*>& lstLineGraph);	//
	void				SaveIntoSelectDbBarGraphic(const list<CEqmDrawBarGraphic*>& lstBarGraph);
	void				SaveIntoSelectDbPieGraphic(const list<CEqmDrawPieGraphic*>& lstPieGraph);
	void				SaveIntoSelectDbButton(const list<CEqmDrawButton*>& lstBtn);	//
	void				SaveIntoSelectDbHistoryTrend(const list<CEqmDrawHistoryTrend*>& lstHistoryTrend);
	void				SaveIntoSelectDbEnergySaveRoi(const list<CEqmDrawEnergySaveRoi*>& lstEnergySaveRoi);

	string				GetProjectLastEditSoftVersion();
	bool				SetShowErrMessage(bool bShow);

	void				LoadProjectSqlite_ListUnit20(void);
	bool				SaveProjectSqlite_ListUnit20(void);
	void				SaveProjectSqlite_CalcModuleInfo(void);
	void				SaveProjectSqlite_CalcModuleInfo_Dec(void);

	void				InsertOneIntoCalcMod(CString strName, const int nCycle, const bool bIsEnable, const bool bSyncDec);
	void				InsertOneIntoCalcModItem(CString strName, CString strParentName, CString strContent, const bool bSyncDec);
	void				DeleteOneFromCalcMod(CString strName, const bool bSyncDec);
	void				DeleteOneFromCalcModItem(CString strName, CString strParentName, const bool bSyncDec);
	void				UpdateCalcMod(CString strName, const int nCycle, const bool bSyncDec);
	void				UpdateCalcModItem(CString strName, CString strParentName, CString strContent, const bool bSyncDec);
	void				EnableCalcMod(CString strName, const bool bIsEnable, const bool bSyncDec);

	bool				SynchronousPlaneTempeDistr(void);

	bool				ExecuteCustomSqlForSqlite(const char* pSql);
	vector<VersionInfo>	LoadVersionInfo(void);
	bool				InsertIntoOneVersion(int nVersion, CString strUser, CString strTime, CString strLog);
	int					ReadLastVersionNumber(void);
	void				ParseFileType(const CString strPath);
	void				UpdateProjectConfigXupTime(void);

	wstring             GetModbusComConfig();
	bool				SaveModbusComConfig(wstring wstrConfig);


	wstring				GetSystemDefineConfig();

	bool				GetAllItemsInLocalConfig(vector<wstring> & wstrNameList, vector<wstring> & wstrValueList);
	bool				SaveSystemDefineConfig(wstring wstrConfig);

	int                GetMaxIDInReportTable();
	bool				GetReportInfo(vector<int>& nidList,vector<CString>& strReportNameList, vector<CString>& strFileDescriptionList, vector<int> &nReportTypeList);
	bool                AddReportInfo(int nid,CString cstrReportName,CString strReportTime,CString cstrReportPath,CString cstrReportDescr,
									int nFileType, int nTimeType, CString strTimeConfig);
	bool				ModifyReportInfo(int nId,CString strReportName);
	bool                DeleteReportInfo(int nid);
	int                 GetReportContentSize(int nid);
		 
	wstring             GetValueInLocalConfigTable(wstring strKey);
	wstring             GetGlobalConfig(CString strTemplateFileName,wstring strKey);
	bool                SetValueInLocalConfigTable(wstring strKey, wstring strValue);
	bool                SetValueInLocalConfigTable(wstring strKey, string strValue);
	bool                RemoveValueInLocalConfigTable(wstring strKey);
	bool                ModifyValueInLocalConfigTable(wstring strOldKey,wstring strKey, wstring strValue);


	bool				SaveBackupRealtimePointValuesStatus(vector<Beopdatalink::CRealTimeDataEntry> & tempEntryList);
	bool				SaveBackupLogicStatus(vector<CString> & strThreadNameList, vector<int> & nStatusList, vector<int> &nTimePeriodList);
	bool				GetBackupRealtimePointValuesStatus(vector<Beopdatalink::CRealTimeDataEntry> & tempEntryList);
	bool				GetBackupLogicStatus(vector<CString> & strThreadNameList, vector<int> & nStatusList, vector<int> &nTimePeriodList);
	bool				SaveBackupOneLogicStatus(CString cstrThreadName, int nStatus, int nTimePeriod);
	bool				GetBackupOneLogicStatus(CString strThreadName, int nStatus, int nTimePeriod);

	int					 InsertRecordToOPCPoint(const int nIndex, const int nType, const char* lpszName, const char* lpszSource, const int nProperty, const char* lpszDescription, const char* lpszUnit, const double dHigh, const double dHighHigh, const double dLow, const double dLowLow, const char* lpszParam1,const char* lpszParam2, const char* lpszParam3,const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8, const char* lpszParam9,const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,const char* lpszParam13,const char* lpszParam14,const char* lpszParam15,const int nParam16,const int nParam17,const int nParam18);
	bool	GetPointExist(const char* lpszName);
	int		GetMaxIDInPointTable();
	bool	InsertNewPointIntoPointTable(int id,CString pointName);

	int		ReadTemplatePageGroupList(CString strTemplateFileName, vector<GroupPage> & vecPageGroupNameList);
	int		ReadTemplatePageList(CString strTemplateFileName, int nGroupId, vector<CPageTemplateInfo> & vecPageTemplateInfoList);
	bool	ExtractTemplatePNGFile(CString cstrTemplateName, int templatePageId, CString strDestFilePath, int &nWidth, int &nHeight);
	void	LoadPictureLibFromTemplate(CString cstrTemplateName, const int nImageId,  CString strFilePath);
	bool	SelectPageID(const int nId);

	int					Get_devices_max_id();
private:
	int					Get_pipes_max_id();
	int					Get_texts_max_id();
	int					Get_graphics_max_id();
	int					Get_DashBoard_max_id();
	int					Get_Chart_max_id();
	int					Get_Bend_max_id();
	int					Get_Button_max_id();
	int					Get_TimeSelect_max_id();
	int					Get_TimeBar_max_id();
	int					Get_ProgressBar_max_id();
	int					Get_ShellExecute_max_id();
	int					Get_EmbededPage_max_id();
	int					Get_AreaChange_max_id();
	int					Get_LiquidLevel_max_id();
 	int					Get_CheckButtonPlus_max_id();
 	int					Get_Scrawl_max_id();
  	int					Get_HistoryTrend_max_id();
 	int					Get_EnergySaveRoi_max_id();
	int					Get_MapNavigate_max_id();
	int					Get_NavigateWindow_max_id();
	int					Get_LevelRuler_max_id();
	int					Get_DataAnalyse_max_id();
	int					Get_TemperDistri_max_id();
	int					Get_DataReport_max_id();
	int					Get_PlaneTempeDistr_max_id();
	int					Get_DottedLineFrm_max_id();
	int					Get_GeneralComponent_max_id();
	int					Get_Rectangle_max_id();
	int					Get_StraightLine_max_id();
	int					Get_Polygon_max_id();
  std::wstring					m_strFilePath;
  std::wstring					m_strFileDecPath;
   std::wstring					m_strImgDbPath;
   int								m_nFontResolution;
    std::wstring					wstr; //temp value
    std::string						str;  //temp value
    int								itemp;
    CEqmDrawProject&				m_project;
    CDataPointManager&				m_pointmap;
	CLogicPointManager&				m_LogicPtManager;
	CAlarmPointManager&				m_AlarmPtManager;
    CEqmDrawPage*					m_pPage;
    std::vector<std::string>		mVecStrType;
    std::vector<std::string>		mVecStrSubType;
    VEC_ENTRY						mVecEntry;				//bird,for save duplicate item
    VEC_ENTRY						mVecEntryDuplicate;		//bird,for save duplicate item
	VEC_ENTRY						mVecEntryNonDuplicate;  //non duplicate
	VEC_ENTRY						mVecEntryError;
	E_OWNER							m_eOwner;
	bool							m_bIsNullPageCreated;
	bool							m_bLoadAllPages;
	bool							m_bEnglishLanguage;
	bool							m_bShowErrorMessage;
	bool							m_bOutPutLog;
	CString							m_strSysPath;
public:
	std::vector<PictureInfo>		m_vPictureList;
	std::vector<AnimationInfo>		m_vAnimationList;
	std::vector<PictureInfo>		m_vPictureforAnimationList;
	std::vector<PictureInfo>		m_vPictureListImage;
	std::vector<AnimationInfo>		m_vAnimationListImage;
	std::vector<PictureInfo>		m_vPictureforAnimationListImage;
	std::vector<ProjectCheckInfo>	m_vecProjectCheckInfo;
	std::vector<ProjectCheckInfo>	m_vecProjectCheckNotFindInfo;
	std::vector<ProjectCheckInfo>	m_vecProjectPointCheckInfo;
	std::vector<ProjectCheckInfo>	m_vecProjectPointCheckNotFindInfo;
	std::vector<ProjectCheckInfo>	m_vecProjectOverLapElementInfo;
	std::vector<int>				m_vecPicID;
	vector<TextDeviceInfo>			m_vecTextDevInf;
	vector<TextAreaInfo>			m_vecTextAreaInf;
	vector<CalcModuleInfo>			m_vecCalcModuleInfo;
public:
	int  count_error;

	BOOL CreateFolder(CString strPath);
	BOOL FolderExist(CString strPath);
	BOOL FileExist(CString strFileName);
	bool DeleteDirectory(const wchar_t* sDirName);
	int DeleteAllTables();
	void DeleteElement(const EQMDRAWTYPE type,const int pageid,const int id);
	void DeletePage(CEqmDrawPage* pPage);
	void DeleteGroup(const int nID);
	void DeleteAllPoint();
	void DeleteOnePoint(const CString strPoint);
	wstring GetDetailChartByIDsDiagnose( const int nPageID, const int nElementID, vector<_tagDiagnoseItem>& vecItem );
	void GetDetailChartByIDsDiagnosePlus( const int nPageID, const int nElementID, vector<_tagBoolBindInfo>& vecItem );
	wstring GetDetailChartByIDsDiagnoseLineSubItem( const int nPageID, const int nElementID, const char* lpszItemName, const char* lpszPointName, vector<_tagDiagnoseItemInLine>& vecItem );
	void DeletePoints( const vector<wstring>& vec );
	void GetDiagnoseItemPointNameVec(const int nPageID, vector<wstring>& vecPointName);

	bool AddIntoPictureList(const int nPicId, const PictureInfo* pPicInfo, const bool bDbSelect = true);
	bool AddIntoAnimationList(const int nAnimId, const AnimationInfo* pAnimInfo, const bool bDbSelect);
	bool AddIntoAnimationImage(const int nPicId, const PictureInfo* pPicInfo, const bool bDbSelect);

	PictureInfo* GetAnimationPicInfoById(const int nId, const bool bIsSel, wstring & strErrorInfo);
	wstring SetAnimationData(const int nAnimId, const AnimationInfo* pAnimInf, const bool bDbSelect);
	bool SetAnimationDataEx(const int nAnimId, const AnimationInfo* pAnimInf, const bool bDbSelect);

	CEqmDrawProject& GetDrawProject(void);

	string GetSqliteSaveTime(void) const;

	bool InsertImageIntoOpenS3db(void);
	bool InsertAnimationIntoOpenS3db(const AnimationInfo* pSrcInfo, int& nUseId);
	bool InsertImageIntoOpenS3dbFromId(void);
	bool InsertAnimationIntoOpenS3dbFromId(const int nSrcId, int& nUseId);

	int GetMaxPictureNum(const bool bDbSelect);
	int GetMaxAnimationNum(const bool bDbSelect);
	int GetMaxAnimationPictureNum(const bool bDbSelect);

	void LoadAllPagesDetail(void);

	void ImageLeanFunc(void);
	void ImageLeanCoreFunc(void);
	bool VaccumDatabase(void);

	PictureInfo* GetPictureInfo(const wchar_t* pName, const int nSize, const wstring strRemark, PictureInfo* picInfo);
	bool GetFileEncryptType(void) const;
	void SetFileEncryptType(const bool bEncrypt);
	
	typedef struct _DeviceInfo
	{
		_DeviceInfo(int nID, int nPictureID, int nPageID, bool bPicOrAni)
			: m_nID(nID)
			, m_nPicInfo(nPictureID,bPicOrAni)
			, m_nPageId(nPageID)
		{

		}
		struct imageInfo
		{
			int bindID;
			bool bPicOrAnmt;//true picture,flase animation
			imageInfo(int id, bool b)
				: bindID(id)
				, bPicOrAnmt(b)
				{}
		};
		int m_nID;//device DI
		int m_nPageId;
		imageInfo m_nPicInfo;//直接现实的绑定的图片
		std::list<imageInfo> m_bindList;//其他绑定图元ID
		//std::list<int> m_bindList;//绑定图元ID
		//int m_nPictureID;
		//std::map<int,bool> m_bindList;//true picture,flase animation
	}CheckDlg_DeviceInfo;
	void GetDeviceInfo_Check(list<CheckDlg_DeviceInfo>* deviceInfoList);
	void GetPictureId_Check(list<int>* pictureList);
	void GetAnimationId_Check(list<int>* animationList);
	//true 检查现显示的图，flase检查绑定的图
	bool IsAnimationExist(int Id, bool bShowOrBind);
private:
	vector<CurveColumn>	GetScrawlCurveColumnInfo(const size_t nElementId) const;
	vector<HistoryTrendInfo> GetHistoryTrendVectorInfo(const size_t nElementId) const;
	vector<CMapPageInfo> GetMapNavigateVectorInfo(const size_t nElementId) const;
	vector<PageInfo> GetNavigateWindowVectorInfo(const size_t nElementId) const;
	vector<LabelInfo> GetLevelRulerVectorInfo(const size_t nElementId) const;
	vector<SectionInfo> GetLevelRulerSectionVectorInfo(const size_t nElementId) const;
	vector<DaPointInfo> GetDataAnalyseVectorInfo(const size_t nElementId) const;

	bool FindDestPictureId(const int nPicId);
	void AddIntoDestPictureId(const int nPicId);
	void ClearDestPictureId(void);
	vector<int>	m_vecDstPicId;

	bool FindDestAnimationId(const int nAnimId);
	void AddIntoDestAnimationId(const int nAnimId);
	void ClearDestAnimationId(void);
	vector<int>	m_vecDstAnimId;

	//CUseLog	m_log;


	// s3db减肥功能
	void LoadCustomImageInfo(void);
	void LoadLibImageInfo(void);
	void CheckExcessImage(void);
	bool DeleteExcessImage(void);
	bool DeleteAnimation();
	bool DeleteAnimationImg();
	bool DeleteImageAll(void);
	bool DeleteAnimationAll(void);
	bool DeleteImageAnimationAll(void);

	bool LoadImageCustomMeta(void);
	bool LoadImageEvent(void);
	bool LoadImageButton(void);
	bool LoadImageChartPlus(void);
	bool LoadImageAreaChange(void);
	bool LoadImageShellExecute(void);
	bool LoadImageEmbededPage(void);
	bool LoadImagePageBackground(void);

	vector<CustomImgInfo>	vecCustomImgInfo;
	set<int>				vecDeleteImgInfo;
	set<int>				vecDeleteAnimationId;
	set<int>				vecDeleteAnimationImgId;
	Image*	CompressPNG(Image* pBitmapSource, Gdiplus::REAL fWidth, Gdiplus::REAL fHeight);

	bool	GetEncryptTypeWithImgLib(const bool bDbSelect);
	bool	m_bIsEncrypt;
};