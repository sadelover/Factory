/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：SqliteAcess.h
* 文件标识：
* 摘    要：数据库操作
*
* 取代版本：N 
* 原作者  ：
* 完成日期：N年N月N日
*/
#ifndef _SQLITE_ACESS_H_
#define _SQLITE_ACESS_H_
extern "C"
{
#include "sqlite3.h"
};

#include <ATLComTime.h>
#include "vector"
#include "../Tools/Util/UtilString.h"
using namespace std;
using std::wstring;
/*******************************************************
sqlite文件标题的操作，写入时要GBToUTF8，读取时UTF8ToGB
不过写入表中的汉字不需要转
********************************************************/

class _declspec(dllexport) CSqliteAcess
{
public:
	CSqliteAcess(const wchar_t* lpszPath, bool bIsEncrypt = true);
	~CSqliteAcess(void);

public:
	sqlite3*						GetDB() const;
	sqlite3_stmt*                   GetDataSet() const;
	int								SqlExe(const char* sqlr);
	bool							GetOpenState();
	int								OpenDataBase(const char* pathName, bool bIsEncrypt = true);
	int								CloseDataBase();
	int								CreateTable(const char* sqlr);
	int								InquiryPumpModel(const int pumpModelID);
	int								BeginTransaction();	
	int								CommitTransaction();
	int								SqlQuery(const char* sqlr);
	int								SqlNext();
	int								SqlFinalize();
	int								Encrypt();
	int								Decode();
	const char*						GetSqliteErr();
	int								getColumn_Count();
	double							getColumn_Double(const int i);
	int								getColumn_Int(const int i);
	const char*						getColumn_Text(const int i);
	const void*						getColumn_Blob(const int i);
	int								getColumn_Bytes(const int i);
	int								InsertRecordIntoLibImage(const int nID, const char* lpszName, const char* lpszSpecy, void* pBlock, const char* lpszFileType, const char* lpszRemark, const int bIsSyatemImage, const int nSize, const int nBtnId = 0, const int nBtnSubId = 0);
	int								InsertRecordIntoLibImageAnimation(const int nID, const char* lpszName, const char* lpszSpecy, void* pBlock, const char* lpszFileType, const char* lpszRemark, const int bIsSyatemImage, const int nSize);
	int								SqlStep();
	//清空project_config表
	int								DeleteProjectConfigData();
	//插入数据到project_config表
	int								InsertRecordToProjectConfig(const int nID, const char* lpszName, const int nWidth, const int nHeight, const char* lpszVersion, const char* lpszXupTime = "", const char* lpszLastEditSoftVersion = "", const char* pSaveTime = "", const char* pDeviceType = "0", bool bMenuGroupEnabled = false);
	//清空page表
	int								DeleteAllPageData();
	//插入数据到page表
	int								InsertRecordToPage(const char* lpszType, const int nStartColor, const int nEndColor, const int nImageID, const int nID, const char* lpszName,   
														const int nWidth, const int nHeight, const int nGroupID, const char* lpszShowInObserver, const char* pWebAddr, const int nShowOrder, const int nXPos=0, const int nYPos=0, const int nMinRight=0);
	//清空page_contain_elements表
	int								DeleteAllContainElements();
	//插入数据到page_contain_elements表
	int								InsertRecordToContainElements(const int nID, const char* lpszName, const int nType, const int nXPos, const int nYPos, const int nWidth, const int nHeight, const char* lpszDataComID, 
									const int nLayer, const int nPageID, const int nRotateAngle, const int nLinkPageID,const char* lpszShowString,const int nPicID, const char* lpszDiagnosePageString, const int nSubType, const bool bIsPic, int nBindType);
	//更新page_contain_elements表中的字段pictureid
	int								UpdateRecordInContainElements(const int nOldPicId, const int nNewPicId);
	//清空event表
	int								DeleteAllEventData();
	//插入数据到event表
	int								InsertRecordToEvent(const int nPageID, const int nID, const char* lpszEventContent, const char* lpszBindContent);
	//清空list_line表
	int								DeleteAllLineData();
	//插入数据到list_line表
	int								InsertRecordToLine(const int nPageID, const int nID, const int nDirection, const int nInnerColor, const int nOutterColor, const char* lpszPointList, const int nWidth, const int nLayer, const char* lpszIDList, const int nLogic, const int nPipeType, const int nPipeBitMapID, const int nPipeDenSity, const int nPipeSpeed, const int WaterFashion);
	//清空list_text表
	int								DeleteAllTextData();
	//插入数据到list_text表
	int								InsertRecordToText(const int nID, const int nPageID, const char* lpszInitialString, const int nXPos, const int nYPos,
														const int nWidth, const int nHeight, const int nFontSize, const int nFontColor, const char* lpszFontName,
														const char* lpszDataComID, const int nShowMode, const double dTransparence, const int nBorderType,
														const int nBorderColor, const int nLayer, const int nAlign, const char* lpszTextModeShowString, const int nPointNum, const char* lpszDateTime, const char* lpszDateTimePoint, const int nTextBKType, const int nTextAttri, const char* lpszDevCfg, const char* lpszUnit, const char* lpszFaultDiag);
	//清空list_graphics
	int								DeleteAllGraphics();
	//插入数据到list_graphics
	int								InsertRecordToGraphics(const int nID, const int nPageID, const int nType, const int nLineColor, const int nLineStyle, const int nLineWidth, const int nFillColor, const char* lpszPointList,
														const int nFrontPageID, const double dTransparence, const int nIsBorder3D, const int nLayer);
	//清空list_point
	int								DeleteAllPoint();

	//插入opc数据到list_point
	int								InsertRecordToOPCPoint(const int nIndex, const int nType, const char* lpszName, const char* lpszSource, const int nProperty, const char* lpszDescription, const char* lpszUnit,
														const double dHigh, const double dHighHigh, const double dLow, const double dLowLow, const char* lpszParam1,const char* lpszParam2,
														const char* lpszParam3,const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8,
														const char* lpszParam9,const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,const char* lpszParam13,const char* lpszParam14,
														const char* lpszParam15,const char* lpszParam16,const char* lpszParam17,const char* lpszParam18);	
	//插入Logic数据到list_point
	int								InsertRecordToLogicPoint(const int nType, const char* lpszName, const char* lpszLogicType, const char* lpszParam1, const char* lpszParam2, const char* lpszParam3,
															const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8,const char* lpszParam9,
															const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,const char* lpszParam13,const char* lpszParam14,const char* lpszParam15,
															const char* lpszParam16,const char* lpszParam17,const char* lpszParam18,const char* lpszParam19,const char* lpszParam20,const char* lpszParam21,
															const char* lpszParam22,const char* lpszParam23,const char* lpszParam24,const char* lpszParam25,const char* lpszParam26,const char* lpszParam27,
															const char* lpszParam28,const char* lpszParam29,const char* lpszParam30,const char* lpszDescription);
	//插入Alarm数据到list_point
	int								InsertRecordToAlarmPoint(const int nType, const char* lpszName, const char* lpszAlarmType, const char* lpszParam1, const char* lpszParam2, const char* lpszParam3,
															const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8,const char* lpszParam9,
															const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,const char* lpszParam13,const char* lpszParam14,const char* lpszParam15,
															const char* lpszParam16,const char* lpszParam17,const char* lpszParam18,const char* lpszParam19,const char* lpszParam20,const char* lpszDescription);
	//清空所有有用的表
	int								DeleteAllUsefulTables();
	//清空组的表
	int								DeleteAllGroupData();
	//插入数据到组
	int								InsertRecordToPageGroup(const int nGroupID, const char* lpszGroupName, const char* lpszGroupShowOrder);
	//更新组表
	int								UpdatePageGroup(const int nGroupID, const char* lpszGroupName, const char* lpszGroupShowOrder);
	//更新工程配置表
	int								UpdateProjectConfig(const int nID, const char* lpszName, const int nWidth, const int nHeight, const char* lpszVersion, const char* lpszXupTime = "", const char* lpszLastEditSoftVersion = "", const char* pSaveTime = "", const char* pDeviceType = "0");
	//更新初始时间
	int								UpdateProjectConfigXupTime(const char* lpszXupTime);
	//插入数据到lib_animation
	int								InsertRecordToLibAnimation(const int nID, const char* lpszName, const char* lpszAnimationList);
	//插入数据到list_dashboard
	int								InsertRecordToListDashBoard(const int nPageID, const int nID, const char* lpszBindPointName, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nType, const int nStyle, const double dMax, const double dMin, const char* pMaxStr, const char* pMinStr, const bool bIsMaxBindPt, const bool bIsMinBindPt ,const COLORREF nFontColor, const int DecimalPlaces);
	//清空list_dashboard表的数据
	int								DeleteAllDashBoardData();
	//清空list_chart
	int								DeleteAllListChartData();
	//清空detail_chart
	int								DeleteAllDetailChartData();
	//插入数据到list_chart
	int								InsertRecordToListChart(const int nPageID, const int nElementID, const char* lpszChartName, const int nUpdateInterval, const char* lpszXName, const char* lpszXUnit, 
															const char* lpszYName, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const int nChartType, 
															const double dMax, const char* lpszStartPoint, const int nQueryType, const char* lpszUnitProperty01,const char* lpszUnitProperty02,const char* lpszUnitProperty03,const char* lpszUnitProperty04, const char* lpszUnitProperty05);
	//插入数据到list_unit02
	int								InsertRecordToListUnit02(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nDefaultImageID, const char* strPointList,const int nAreaImageID,const int nLinkPageID);
	
	//插入数据到detail_chart
	int								InsertRecordToDetailChart(const int nPageID, const int nElementID, const char* lpszItemName, const int nItemColor, const char* lpszBindPointName);
	int								InsertRecordToDetailChart(const int nPageID, const int nElementID, const char* lpszItemName, const int nItemColor, const char* lpszBindPointName,const int nPointStoreType);
	//插入数据到list_bend
	int								InsertRecordToListBend(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const int nAngle);
	//清空list_bend
	int								DeleteAllListBendData();
	int								DeleteAllButtonData();
	int								InsertRecordToListButton(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const int nCommon, const int nDown, const int nOver, const int nDisable, const int nLinkPageID, const char* strBindSetPointName, const char * strSettingValue, const char* lpszTextContent, const COLORREF nColor, const int nSize, const char* lpszOperation,const char* lpszExpress,const char* lpszRelation,const char* lpszChecked, const char* lpszDlBtnEnable, const char* lpszDlBtnName);
	int								DeletePageByID(const int nPageID);

	int								DeleteAllTimeSelectData();
	int								InsertRecordToListTimeSelect(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const char* lpszBindPointName, const int nRWProperty, const int nSelectDateTime);

	int								DeleteAllTimeBarData();
	int								InsertRecordToListTimeBar(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const char* lpszBindPointName, const int nColor, const int nQueryType, const char* lpszBindPointNameStatus);

	int								DeleteAllTimeBarItemData();
	int								InsertRecordToListTimeBarItem(const int nBarID, const int nIndex, const char* lpszName, const char* lpszBind);
	int								DeleteOneGroup(const int nID);
	int								DeleteOnePage(const int nPageID);
	int								DeleteOneDevice(const int nID);
	int								DeleteOneEvent(const int nPageID,const int nID);
	int								DeleteOneDetailChart(const int nPageID,const int nID);
	int								DeleteOnePipe(const int nID);
	int								DeleteOneText(const int nID);
	int								DeleteOneGraphic(const int nID);
	int								DeleteOneDashBoard(const int nID);
	int								DeleteOneChart(const int nID);
	int								DeleteOneBend(const int nID);
	int								DeleteOneButton(const int nID);
	int								DeleteOneTimeSelect(const int nID);
	int								DeleteOneTimeBar(const int nID);
	int								DeleteTimeBarItem(const int nID);
	int								DeleteOnePoint(const char* lpszName);
	int								DeleteAllTables();
	int								DeleteAllProgressBarData();
	int								DeleteAllDiagnoseChartItemData();
	int								DeleteOneProgressBarData(const int nID);
	int								InsertRecordToListProgressBar(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const char* lpszBindPointName, const int nColor, const double dFactorValue, const char* lpszBindPointNameBase);
	int								DeleteOnePicture(const int nID);
	int								DeleteOneAnimation(const int nID);
	int								DeleteOnePictureForAnimation(const int nID);
	int								DeleteOneAreaChange(const int nID);

	int								DeleteOneImportSetting(const int nID);
	int								InsertOneImportSetting(const int nID, const char* lpszPrjName, const int bLoadPoint, const int nBasePointOffset, const int bLoadImage, const int nBaseImageOffset);
	int								UpdateImportSettingByID(const int nID,const int bLoadPoint, const int bLoadImage);

	int								DeleteBaseImageByID(const int nStartID, const int nEndID);
	int								DeleteBasePointByID(const int nStartID, const int nEndID);

	int								UpdatePage(const char* lpszType, const int nStartColor, const int nEndColor, const int nImageID, const int nID, const char* lpszName,   
												const int nWidth, const int nHeight, const int nGroupID, const char* lpszShowInObserver, const char* pWebAddr, const int nXPos=0, const int nYPos=0, const int nMinRight = 0);
	int								UpdatePageShowOrder(const int nPageID, const int nOrder);
	int								InsertIntoDiagnoseLineDetail(const int nPageID, const int nElementID, const char* lpszPointName, const int nPicID, const char* lpszSubPointName, const char* lpszDescription,const char* lpszItemName);
	int								DeleteDiagnoseLineDetail(const int nPageID, const int nElementID, const char* lpszPointName,const char* lpszItemName);
	int								InsertDiagnoseItemToDetail(const int nPageID, const int nElementID, const char* lpszBoolPointName, const char* lpszResultPointName,const char* lpszTreatmentPointName,  const char* lpszResultPointName2,const char* lpszResultPointName3, const int nLevel);  
	int								InsertDiagnoseSubItemToDetail(const int nPageID, const int nElementID, const char* lpszBoolPointName, const char* lpszResultPointName, const char* lpszTreatmentPointName, const char* lpszItemName, const char* lpszBindPointName, const int nColor, const int nLevel);
	int								SqlBind_Blob(char *pBlock,int iOrder ,int nSize);

	wstring							MbcsToUtf8(const char *file);
	int								DeleteSqlitePoints(const vector<wstring>& vec);

	int								ExecuteCustomSql(const char* pSql);

	int								DeleteOneShellExecute(const int nID);
	int								DeleteAllShellExecuteData();
	int								InsertOneShellExecute(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* lpszServerIP, const char* lpszFileName, const char* lpszDownDirectory, const char* lpszPicID, const char* lpszAuto, const char* lpszFileID);
	int								InsertOneEmbededPage(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* lpszPicID, const char* lpszPageNameList, const char* lpszInterval);
	int								DeleteOneEmbededPage(const int nID);
	int								DeleteAllEmbededPage();
	int								InsertOneIntoLiquidLevel(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* lpszPicID, const char* lpszBindPoint, const char* lpszMaxValue, const char* lpszMinValue, const char* lpszHighAlarmLineValue, const char* lpszLowAlarmLineValue);
	int								DeleteOneFromLiquidLevel(const int nID);
	int								DeleteAllFromLiquidLevel();
	int								InsertOneIntoCheckButtonPlus(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer
										, const char* lpszCtrlStyle, const char* lpszTextColor, const char* lpszTextSize, const char* lpszSelVal, const char* lpszUnSelVal
										, const char* lpszBindPointName, const char* lpszTextContent, const char* lpszGroudId, const char* lpszExpression, const char* lpszOptionsDec);
	int								DeleteOneFromCheckButtonPlus(const int nID);
	int								DeleteAllFromCheckButtonPlus();
	int								InsertOneIntoScrawl(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pShowWords);
	int								DeleteOneFromScrawl(const int nID);
	int								DeleteAllFromScrawl();
	int								InsertOneIntoCurveColumn(const int nElementID, const char* pPointName, const int color, const int nMaxVal, const int nMinVal, const char* pRemarks, const int nType);
	int								DeleteOneFromCurveColumn(const int nID);
	int								DeleteAllFromCurveColumn();

	int								InsertOneIntoHistoryTrend(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pImgType);
	int								DeleteOneFromHistoryTrend(const int nID);
	int								DeleteAllFromHistoryTrend(void);
	int								InsertOneIntoHistoryTrendVector(const int nElementID, const int nCount, const char* pStartTime, const char* pEndTime, const int bIsAutoCalcRange, const char* pBindPtName,
																const char* pRemarks, const int nColor, const int nUpdateCycle, const int nLineWidth, const int dScale);
	int								DeleteOneFromHistoryTrendVector(const int nID);
	int								DeleteAllFromHistoryTrendVector();

	int								InsertOneIntoEnergySaveRoi(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer,
															const char* pBindPtName, const char* pCost, const char* pStartTime, const char* pEndTime, const char* pOccurTime);
	int								DeleteOneFromEnergySaveRoi(const int nID);
	int								DeleteAllFromEnergySaveRoi(void);

	int								InsertOneIntoMapNavigate(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer,const int nRotateAngel,const int nEquipType,const int nGroupId,const int nId,const char* strEquipTemplatePageName,const char* paramList, const char* strTemplateName, const int nBanOnDisplay);
	int								DeleteOneFromMapNavigate(const int nID);
	int								DeleteAllFromMapNavigate(void);
	int								InsertOneIntoMapNavigateVector(const int nElementID, const char* pLongitudeName1, const char* pLongitudeName2, const char* pLatitudeName1, const char* pLatitudeName2, const int nPageId);
	int								DeleteOneFromMapNavigateVector(const int nID);
	int								DeleteAllFromMapNavigateVector(void);

	int								InsertOneIntoNavigateWindow(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer);
	int								DeleteOneFromNavigateWindow(const int nID);
	int								DeleteAllFromNavigateWindow(void);
	int								InsertOneIntoNavigateWindowVector(const int nElementID, const char* pPageDesc, const char* pPageName, const int nPageId);
	int								DeleteOneFromNavigateWindowVector(const int nID);
	int								DeleteAllFromNavigateWindowVector(void);

	int								InsertOneIntoLevelRuler(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer,
															const char* pName, const double dMax, const double dMin, const double dMainScale, const double dMinorScale, const int nDeci);
	int								DeleteOneFromLevelRuler(const int nID);
	int								DeleteAllFromLevelRuler(void);
	int								InsertOneIntoLevelRulerVector(const int nElementID, const char* pName, const char* pValue, const char* pBindPtName, const int nLinkPageId, const char* pLinkPageName, const int nIsUp);
	int								DeleteOneFromLevelRulerVector(const int nID);
	int								DeleteAllFromLevelRulerVector(void);
	int								InsertOneIntoLevelRulerSectionVector(const int nElementID, const char* pName, const double fMax, const double fMin);
	int								DeleteOneFromLevelRulerSectionVector(const int nID);
	int								DeleteAllFromLevelRulerSectionVector(void);

	int								InsertOneIntoDataAnalyse(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pPtTime, const char* pPtCount, const char* pPtStatus);
	int								DeleteOneFromDataAnalyse(const int nID);
	int								DeleteAllFromDataAnalyse(void);
	int								InsertOneIntoDataAnalyseVector(const int nElementID, const char* pPtName, const char* pRemarks);
	int								DeleteOneFromDataAnalyseVector(const int nID);
	int								DeleteAllFromDataAnalyseVector(void);

	int								InsertOneIntoTemperDistri(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nPicId, const char* pPtList, const char* pBindPtName);
	int								DeleteOneFromTemperDistri(const int nID);
	int								DeleteAllFromTemperDistri(void);

	int								InsertOneIntoDataReport(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pPath);
	int								DeleteOneFromDataReport(const int nID);
	int								DeleteAllFromDataReport(void);

	int								InsertOneIntoPlaneTempeDistr(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pPointName, const int nCfgType);
	int								DeleteOneFromPlaneTempeDistr(const int nID);
	int								DeleteAllFromPlaneTempeDistr(void);

	int								InsertOneIntoDottedLineFrm(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer);
	int								DeleteOneFromDottedLineFrm(const int nID);
	int								DeleteAllFromDottedLineFrm(void);

	int								InsertOneIntoGeneralComponent(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, wstring pCfgVal);
	int								DeleteOneFromGeneralComponent(const int nID);
	int								DeleteAllFromGeneralComponent(void);

	int								InsertOneIntoListUnit20DeviceArea(const int nType, const char* pUnitProp02, const char* pUnitProp03, const char* pUnitProp04, const char* pUnitProp05);
	int								DeleteAllFromListUnit20DeviceArea(void);

	int								InsertOneIntoListUnit20CalcModule(const int nUnitProp01, const char* pUnitProp02, const char* pUnitProp03, const char* pUnitProp04, const char* pUnitProp05, const char* pUnitProp06);
	int								DeleteAllFromListUnit20CalcModuleInfo(void);

	int								InsertOneIntoCalcModule(const char* pName, const int nCycle, const bool bIsEnable);
	int								InsertOneIntoCalcModuleItem(const char* pName, const char* pParentName, const char* pContent);
	int								DeleteOneFromCalcModule(const char* pName);
	int								DeleteOneFromCalcModuleItem(const char* pName, const char* pParentName);
	int								UpdateCalcModule(const char* pName, const int nCycle);
	int								UpdateCalcModuleItem(const char* pName, const char* pParentName, const char* pContent);
	int								EnableCalcModule(const char* pName, const bool bIsEnable);

	int								InsertOneIntoRectangle(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer
														, const string strLineColor, const float fLineWidth, const bool bFill
														, const string strFillColor, const bool bRound, const UINT nRoundHeight, const UINT nRoundWidth
														, const int nRectangleStyle,  const string sTitleInfo, const string sBodyInfo, const string sParaphraseInfo
														, const bool bShowParaphrase, const string sPointInfoList, const string sTitle);
	int								DeleteOneFromRectangle(const int nID);
	int								DeleteAllFromRectangle(void);

	int								InsertOneIntoStraightLine(const int nPageID, const int nElementID, const int nX, const int nY, const int nEndX, const int nEndY, const int nLayer
										,const string strLineColor, const float fLineWidth, const int nLineStyle
										,const int nStartShape, const int nStartShapeSize
										,const int nEndShape, const int nEndShapeSize );
	int								DeleteOneFromStraightLine(const int nID);
	int								DeleteAllFromStraightLine(void);

	int								InsertOneIntoPolygon(const int nPageID, const int nElementID, string strPointList, const int nLayer
										, int nTemplateGroupId, int nTemplatePageId, string sTemplatePageName, string strParam, string templateName
										, DWORD nColor, float nLineWidth);
	int								DeleteOneFromPolygon(const int nID);
	int								DeleteAllFromPolygon(void);

	sqlite3*						m_db;
	sqlite3_stmt*					m_stmt;
	bool							m_bOpenState;
	string							m_strDBPath;
};
#endif