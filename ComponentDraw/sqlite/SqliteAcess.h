
#ifndef _SQLITE_ACESS_H_
#define _SQLITE_ACESS_H_
extern "C"
{
#include "sqlite3.h"
};

#include "vector"
#include "string"
using namespace std;
/*******************************************************
sqlite�ļ�����Ĳ�����д��ʱҪGBToUTF8����ȡʱUTF8ToGB
����д����еĺ��ֲ���Ҫת
********************************************************/

class _declspec(dllexport) CSqliteAcess
{
public:
	CSqliteAcess(const wchar_t* lpszPath);
	~CSqliteAcess(void);

public:
	sqlite3*						GetDB() const;
	sqlite3_stmt*                   GetDataSet() const;
	int								SqlExe(const char* sqlr);
	bool							GetOpenState();
	int								OpenDataBase(const char* pathName);
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
	int								getColumn_Count();
	double							getColumn_Double(const int i);
	int								getColumn_Int(const int i);
	const char*						getColumn_Text(const int i);
	const void*						getColumn_Blob(const int i);
	int								getColumn_Bytes(const int i);
	int								InsertRecordIntoLibImage(const int nID, const char* lpszName, const char* lpszSpecy, void* pBlock, const char* lpszFileType, const char* lpszRemark, const int bIsSyatemImage, const int nSize);
	int								InsertRecordIntoLibImageAnimation(const int nID, const char* lpszName, const char* lpszSpecy, void* pBlock, const char* lpszFileType, const char* lpszRemark, const int bIsSyatemImage, const int nSize);
	int								SqlStep();
	//���project_config��
	int								DeleteProjectConfigData();
	//�������ݵ�project_config��
	int								InsertRecordToProjectConfig(const int nID, const char* lpszName, const int nWidth, const int nHeight, const char* lpszVersion, const char* lpszXupTime = "");
	//���page��
	int								DeleteAllPageData();
	//�������ݵ�page��
	int								InsertRecordToPage(const char* lpszType, const int nStartColor, const int nEndColor, const int nImageID, const int nID, const char* lpszName,   
														const int nWidth, const int nHeight, const int nGroupID, const char* lpszShowInObserver, const int nXPos=0, const int nYPos=0);
	//���page_contain_elements��
	int								DeleteAllContainElements();
	//�������ݵ�page_contain_elements��
	int								InsertRecordToContainElements(const int nID, const char* lpszName, const int nType, const int nXPos, const int nYPos, const int nWidth, const int nHeight, const char* lpszDataComID, 
									const int nLayer, const int nPageID, const int nRotateAngle, const int nLinkPageID,const char* lpszShowString,const int nPicID, const int nSubType = 0);
	//���event��
	int								DeleteAllEventData();
	//�������ݵ�event��
	int								InsertRecordToEvent(const int nPageID, const int nID, const char* lpszEventContent, const char* lpszBindContent);
	//���list_line��
	int								DeleteAllLineData();
	//�������ݵ�list_line��
	int								InsertRecordToLine(const int nPageID, const int nID, const int nDirection, const int nInnerColor, const int nOutterColor, const char* lpszPointList, const int nWidth, const int nLayer, const char* lpszIDList, const int nLogic);
	//���list_text��
	int								DeleteAllTextData();
	//�������ݵ�list_text��
	int								InsertRecordToText(const int nID, const int nPageID, const char* lpszInitialString, const int nXPos, const int nYPos,
														const int nWidth, const int nHeight, const int nFontSize, const int nFontColor, const char* lpszFontName,
														const char* lpszDataComID, const int nShowMode, const double dTransparence, const int nBorderType,
														const int nBorderColor, const int nLayer, const int nAlign, const char* lpszTextModeShowString, const int nPointNum, const char* lpszDateTime, const char* lpszDateTimePoint);
	//���list_graphics
	int								DeleteAllGraphics();
	//�������ݵ�list_graphics
	int								InsertRecordToGraphics(const int nID, const int nPageID, const int nType, const int nLineColor, const int nLineStyle, const int nLineWidth, const int nFillColor, const char* lpszPointList,
														const int nFrontPageID, const double dTransparence, const int nIsBorder3D, const int nLayer);
	//���list_point
	int								DeleteAllPoint();

	//����opc���ݵ�list_point
	int								InsertRecordToOPCPoint(const int nIndex, const int nType, const char* lpszName, const char* lpszSource, const int nProperty, const char* lpszDescription, const char* lpszUnit,
														const double dHigh, const double dHighHigh, const double dLow, const double dLowLow, const char* lpszParam1,const char* lpszParam2,
														const char* lpszParam3,const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8,
														const char* lpszParam9,const char* lpszParam10,const char* lpszParam11);	
	//����Logic���ݵ�list_point
	int								InsertRecordToLogicPoint(const int nType, const char* lpszName, const char* lpszLogicType, const char* lpszParam1, const char* lpszParam2, const char* lpszParam3,
															const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8,const char* lpszParam9,
															const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,const char* lpszParam13,const char* lpszParam14,const char* lpszParam15,
															const char* lpszParam16,const char* lpszParam17,const char* lpszParam18,const char* lpszParam19,const char* lpszParam20,const char* lpszParam21,
															const char* lpszParam22,const char* lpszParam23,const char* lpszParam24,const char* lpszParam25,const char* lpszParam26,const char* lpszParam27,
															const char* lpszParam28,const char* lpszParam29,const char* lpszParam30,const char* lpszDescription);
	//����Alarm���ݵ�list_point
	int								InsertRecordToAlarmPoint(const int nType, const char* lpszName, const char* lpszAlarmType, const char* lpszParam1, const char* lpszParam2, const char* lpszParam3,
															const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8,const char* lpszParam9,
															const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,const char* lpszParam13,const char* lpszParam14,const char* lpszParam15,
															const char* lpszParam16,const char* lpszParam17,const char* lpszParam18,const char* lpszParam19,const char* lpszParam20,const char* lpszDescription);
	//����������õı�
	int								DeleteAllUsefulTables();
	//�����ı�
	int								DeleteAllGroupData();
	//�������ݵ���
	int								InsertRecordToPageGroup(const int nGroupID, const char* lpszGroupName);
	//���¹������ñ�
	int								UpdateProjectConfig(const int nID, const char* lpszName, const int nWidth, const int nHeight, const char* lpszVersion, const char* lpszXupTime = "");
	//�������ݵ�lib_animation
	int								InsertRecordToLibAnimation(const int nID, const char* lpszName, const char* lpszAnimationList);
	//�������ݵ�list_dashboard
	int								InsertRecordToListDashBoard(const int nPageID, const int nID, const char* lpszBindPointName, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nType, const int nStyle, const double dMax, const double dMin );
	//���list_dashboard�������
	int								DeleteAllDashBoardData();
	//���list_chart
	int								DeleteAllListChartData();
	//���detail_chart
	int								DeleteAllDetailChartData();
	//�������ݵ�list_chart
	int								InsertRecordToListChart(const int nPageID, const int nElementID, const char* lpszChartName, const int nUpdateInterval, const char* lpszXName, const char* lpszXUnit, 
															const char* lpszYName, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const int nChartType, 
															const double dMax, const char* lpszStartPoint, const int nQueryType, const char* lpszUnitProperty01,const char* lpszUnitProperty02,const char* lpszUnitProperty03,const char* lpszUnitProperty04, const char* lpszUnitProperty05);
	//�������ݵ�list_unit02
	int								InsertRecordToListUnit02(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nDefaultImageID, const char* strPointList,const int nAreaImageID,const int nLinkPageID);
	
	//�������ݵ�detail_chart
	int								InsertRecordToDetailChart(const int nPageID, const int nElementID, const char* lpszItemName, const int nItemColor, const char* lpszBindPointName);
	int								InsertRecordToDetailChart(const int nPageID, const int nElementID, const char* lpszItemName, const int nItemColor, const char* lpszBindPointName,const int nPointStoreType);
	//�������ݵ�list_bend
	int								InsertRecordToListBend(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const int nAngle);
	//���list_bend
	int								DeleteAllListBendData();
	int								DeleteAllButtonData();
	int								InsertRecordToListButton(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const int nCommon, const int nDown, const int nOver, const int nDisable, const int nLinkPageID, const char* strBindSetPointName, const int nSettingValue, const char* lpszTextContent, const COLORREF nColor, const int nSize, const char* lpszOperation,const char* lpszExpress,const char* lpszRelation);
	int								DeletePageByID(const int nPageID);

	int								DeleteAllTimeSelectData();
	int								InsertRecordToListTimeSelect(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const char* lpszBindPointName, const int nRWProperty, const int nTimePoint);

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
												const int nWidth, const int nHeight, const int nGroupID, const char* lpszShowInObserver, const int nXPos=0, const int nYPos=0);
	int								UpdatePageShowOrder(const int nPageID, const int nOrder);
	int								InsertIntoDiagnoseLineDetail(const int nPageID, const int nElementID, const char* lpszPointName, const int nPicID, const char* lpszSubPointName, const char* lpszDescription,const char* lpszItemName);
	int								DeleteDiagnoseLineDetail(const int nPageID, const int nElementID, const char* lpszPointName,const char* lpszItemName);
	int								InsertDiagnoseItemToDetail(const int nPageID, const int nElementID, const char* lpszBoolPointName, const char* lpszResultPointName,const char* lpszTreatmentPointName,  const char* lpszResultPointName2,const char* lpszResultPointName3, const int nLevel);  
	int								InsertDiagnoseSubItemToDetail(const int nPageID, const int nElementID, const char* lpszBoolPointName, const char* lpszResultPointName, const char* lpszTreatmentPointName, const char* lpszItemName, const char* lpszBindPointName, const int nColor, const int nLevel);
	int								SqlBind_Blob(char *pBlock,int iOrder ,int nSize);
	sqlite3*						m_db;
	sqlite3_stmt*					m_stmt;
	bool							m_bOpenState;
	string							m_strDBPath;
};
#endif