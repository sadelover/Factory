/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：ProjectSqlite.cpp
* 文件标识：
* 摘    要：Sqlite数据库操作
*
* 取代版本：N 
* 原作者  ：
* 完成日期：N年N月N日
*/
//projectxml.cpp bird. 3.1

#include "stdafx.h"
#include "projectSqlite.h"
#include "Tools/Util/UtilsFile.h"
#include "Tools/Util/UtilString.h"
#include "EqmDrawProject.h"
#include "EqmDrawDevice.h"
#include "EqmDrawPipe.h"
#include "EqmDrawGraphic.h"
#include "EqmDrawText.h"
#include "EqmDrawGraphicRect.h"
#include "EqmDrawGraphicLine.h"
#include "EqmDrawGraphicEllipse.h"
#include "EqmDrawGraphicPolygon.h"
#include "DataPoint/DataPointEntry.h"
#include "ParseStringParam3.h"
#include <list>
#include "../ControlEx/WarningDlg.h"

#include "DB_BasicIO/Sqlite/SqliteAcess.h"
#include "EqmDrawDashBoard.h"
#include "EqmDrawLineGraphic.h"
#include "EqmDrawBarGraphic.h"
#include "EqmDrawPieGraphic.h"
#include "EqmDrawBend.h"
#include "EqmDrawButton.h"
#include "EqmDrawTimeSelect.h"
#include "EqmDrawTimeBar.h"
#include "EqmDrawProgressBar.h"
#include "EqmDrawDiagnoseGraphic.h"
#include "EqmDrawShellExecute.h"
#include "EqmDrawEmbededPage.h"
#include "EqmDrawLiquidLevel.h"
#include "EqmDrawButtonPlus.h"
#include "EqmDrawScrawl.h"
#include "EqmDrawEnergySaveRoi.h"
#include "EqmDrawTemperDistri.h"
#include "EqmDrawDataReport.h"
#include "EqmDrawPlaneTempeDistr.h"
#include "EqmDrawDottedLineFrm.h"
#include "EqmDrawGeneralComponent.h"
#include "EqmDrawRectangle.h"
#include "EqmDrawStraightLine.h"
#include "EqmDrawPolygon.h"
#include "../DB_BasicIO/RealTimeDataAccess.h"
#include "ComponentDraw/PageTemplateInfo.h"
#include "Tools/DirectoryOperations.h"
#include <sstream>
#include <iostream> 
#include <fstream>
#include <cassert>
#include <string>

using namespace std;
using namespace Beopdatalink;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_PLC_READ        PLC_READ
#define DEFAULT_PLC_MAX			0
#define DEFAULT_PLC_MIN			0
#define DEFAULT_PLC_TYPE        "VT_BOOL"
#define DEFAULT_PLC_UNIT        ""
#define DEFAULT_TXT_SHOWMODE    1
#define DEFAULT_TXT_TRANS       "0.00"
#define DEFAULT_LAYER           1
#define DEFAULT_LIMIT           "0.00"

const size_t nCalcModContentLen = 500000;


//////////////////////////////////////////////////////////////////////////
CProjectSqlite::CProjectSqlite(CEqmDrawProject&    project, CDataPointManager& pointmap,
							CLogicPointManager&  mLogicPtManager, CAlarmPointManager&   mAlarmPtManager, int nFontResolution, E_OWNER eOwner)
:m_project(project)
,m_pointmap(pointmap)
,m_LogicPtManager(mLogicPtManager)
,m_AlarmPtManager(mAlarmPtManager)
,m_pPage(NULL)
,m_eOwner(eOwner)
,m_bIsNullPageCreated(false)
,m_bLoadAllPages(false)
,m_bEnglishLanguage(false)
,m_nFontResolution(nFontResolution)
,m_bShowErrorMessage(true)
,m_bOutPutLog(true)
,m_bIsEncrypt(true)
{
    InitTypeString();
    InitSubTypeString();

	Project::Tools::GetSysPath(m_strSysPath);

	CString cstrFile;
	Project::Tools::GetSysPath(cstrFile);
	CString exepath = cstrFile +  L"\\factory.ini";
	wchar_t strOption[1024];
	GetPrivateProfileString(L"BeopFactory", L"ImageLibPath", L"", strOption, 1024, exepath.GetString());
	wstring strImageLibPath = strOption;
	if(strImageLibPath.length()<=0)
	{
		strImageLibPath = cstrFile + L"\\BEOPImageLib.s3db";
	}
	SetImageLibPath(strImageLibPath);

	CString	strCfgPath;
	strCfgPath.Format(_T("%s\\factory.ini"), m_strSysPath);
	int nWriteLog = GetPrivateProfileInt(_T("BeopFactory"), _T("WriteLog"), 0, strCfgPath);
	bool bWLog = (0 == nWriteLog) ? false : true;
	//m_log.Set_LogConfig(bWLog, LOG_MINLEVEL_ALL);
	//m_log.ReLoadFile(_T("Log.txt"));
}

CProjectSqlite::~CProjectSqlite()
{
	mVecStrType.clear();
	mVecStrSubType.clear();
	mVecEntry.clear(); 
	mVecEntryDuplicate.clear();
	mVecEntryNonDuplicate.clear();
	mVecEntryError.clear();
	FreePictureLibMem();
	FreeAnimationMem();
	FreePictureLibImageMem();
	FreeAnimationImageMem();
}


//////////////////////////////////////////////////////////////////////////

const string c_str_xml_project = "project";
const string c_str_xml_id = "id";
const char* c_str_xml_type = "type";
const char* c_str_xml_sub_type = "subType";
const string c_str_xml_name = "name";
const string c_str_xml_xuptime = "xuptime";

const string c_str_xml_file_path = "filePath";
const string c_str_xml_state_num = "stateNumber";
const string c_str_xml_pics = "pictures";
const string c_str_xml_pic = "picture";
const string c_str_xml_pages = "pages";
const string c_str_xml_page = "page";
const string c_str_xml_show_order = "showOrder";

const string c_str_xml_devices = "devices";
const string c_str_xml_device = "device";
const string c_str_xml_xpos = "xpos";
const string c_str_xml_ypos = "ypos";
const string c_str_xml_dataComId = "dataComId";
const string c_str_xml_rotate_angle = "rotateAngle";
const string c_str_xml_event_info = "eventInfo";
const string c_str_xml_bind_info = "bindInfo";
const string c_str_xml_width = "width";
const string c_str_xml_height = "height";
const string c_str_xml_point_list = "pointList";
const string c_str_xml_step_interval = "stepInterval";
const string c_str_xml_limit_up = "limitUp";
const string c_str_xml_limit_down = "limitDown";
const string c_str_xml_pipes = "pipes";
const string c_str_xml_pipe = "pipe";
const string c_str_xml_inner_color = "innerColor";
const string c_str_xml_outer_color = "outerColor";
const string c_str_xml_pipeWidth = "pipeWidth";
const string c_str_xml_texts = "texts";
const string c_str_xml_text = "text";
const string c_str_xml_fontSize = "fontSize";
const string c_str_xml_fontColor = "fontColor";
const string c_str_xml_fontStyle = "fontStyle";
const string c_str_xml_showMode = "showMode";
const string c_str_xml_transparence = "transparence";
const string c_str_xml_initialValue = "initialValue";
const string c_str_xml_graphics = "graphics";
const string c_str_xml_graphic = "graphic";
const string c_str_xml_lineColor = "lineColor";
const string c_str_xml_lineStyle = "lineStyle";
const string c_str_xml_lineWidth = "lineWidth";
const string c_str_xml_fillColor = "fillColor";
const string c_str_xml_forwardPageId = "forwardPageId";
const string c_str_xml_point = "point";
const string c_str_xml_logic = "logic";
const string c_str_xml_alarm = "alarm";
const string c_str_xml_data = "Data";
const string c_str_xml_index = "index";
const string c_str_xml_value = "value";
const string c_str_xml_RWProperty = "RWProperty";
const string c_str_xml_remark = "remark";
const string c_str_xml_unit = "Unit";
const string c_str_xml_plcPointMaxVal = "plcPointMaxVal";
const string c_str_xml_plcPointMinVal = "plcPointMinVal";
const string c_str_xml_plcPointMaxMaxVal = "plcPointMaxMaxVal";
const string c_str_xml_plcPointMinMinVal = "plcPointMinMinVal";
const string c_str_xml_bg_color = "backgroundColor";
const string c_str_xml_screenwidth = "width";
const string c_str_xml_screenheight = "height";
const string c_str_xml_layer = "layer";
const string c_str_xml_textborder = "textborder";
const string c_str_xml_textbordercolor = "textbordercolor";
const string c_str_xml_rectborder = "rectborder";
const string c_str_xml_rectbordercolor = "rectbordercolor";
const string c_str_version = "version";
const string c_str_align = "align";
const string c_str_null = "";
const char* c_str_pipe_split = ",";

bool CProjectSqlite::SaveProjectSqlite(string strEditSoftVersion)
{
	bool bOK = true;
	CString sItem;
	PROCESSENTRY32 pe;
	pe.dwSize=sizeof(pe);
	HANDLE hSnapshot= CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	Process32First(hSnapshot,&pe);
	do
	{
		sItem.Format(L"%s",pe.szExeFile);
		if (sItem.CompareNoCase(L"navicat.exe") == 0 || sItem.CompareNoCase(L"sqliteadmin.exe") == 0)
		{
			bOK = false;
		}
	}while(Process32Next(hSnapshot,&pe));
	CloseHandle(hSnapshot);
	if (!bOK)
	{
		AfxMessageBox(L"数据源共享冲突，请关闭其他数据库客户端软件.");
		return false;
	}
	DeleteAllUsefulTables();
    if(SaveProjectCommon(strEditSoftVersion) == false)
		return false;
	return true;
}

bool CProjectSqlite::SaveProjectCommon(string strEditSoftVersion)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	string strProjectName;
	string strXupTime;

    wstr = m_project.GetProjectName();
    strProjectName = UtilString::ConvertWideCharToMultiByte(wstr);

	wstr = m_project.GetxupTime();
	strXupTime = UtilString::ConvertWideCharToMultiByte(wstr);

	ostringstream sqlstream;
	sqlstream << "select * from project_config;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	bool bSave = true;
	bool bHasRecord = false;
	while(true)
	{
		if(SQLITE_ROW == access.SqlNext())
		{
			bHasRecord = true;
		}
		break;
	}
	string strVersion = UtilString::ConvertWideCharToMultiByte(m_project.GetVersion());

	SYSTEMTIME	sysTime;
	char		szTmNow[MAX_PATH] = {0};
	GetLocalTime(&sysTime);
	sprintf_s(szTmNow, MAX_PATH, "%d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	char	szDevType[MAX_PATH] = {0};
	sprintf_s(szDevType, MAX_PATH, "%d", m_project.GetDeviceType());

	if (bHasRecord)
	{
		if(access.UpdateProjectConfig(m_project.GetProjectID(), strProjectName.c_str(), m_project.GetScreenWidth(), m_project.GetScreenHeight(), strVersion.c_str(), strXupTime.c_str(), strEditSoftVersion.c_str(), szTmNow, szDevType)!=0)
		{
			OutputSaveErr(L"UpdateProjectConfig",access.GetSqliteErr());
			bSave = false;
		}
	}
	else
	{
		if(access.InsertRecordToProjectConfig(m_project.GetProjectID(), strProjectName.c_str(), m_project.GetScreenWidth(), m_project.GetScreenHeight(), strVersion.c_str(), strXupTime.c_str(), strEditSoftVersion.c_str(), szTmNow, szDevType)!=0)
		{
			OutputSaveErr(L"InsertRecordToProjectConfig",access.GetSqliteErr());
			bSave = false;
		}
	}
	access.SqlFinalize();
	if(SaveGroupPageSqlite() == false)
		bSave = false;
	if(SaveProjectSqlite_pages() == false)
		bSave = false;
	if(SaveProjectSqlite_ListUnit20() == false)
		bSave = false;
	//SaveProjectSqlite_CalcModuleInfo();
	//SaveInfoIntoSelectDb();
	//SaveProjectSqlite_opcid3();
	//SaveProjectSqlite_LogicPoint();
	//SaveProjectSqlite_AlarmPoint();
	return bSave;
}

bool CProjectSqlite::UpdateDBSettingPara(DataBaseSetting &dbset, CString &strErr)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from project_config;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	wstring wstrErrInfo;

	string strMenuGroupEnabled = (dbset.bEnableMenuGroup)?"1":"0";

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
	if (iCount==0)
	{
		string strProjectName;
		string strXupTime;
		const int rc = access.DeleteProjectConfigData();
		assert(0 == rc);

		wstr = m_project.GetProjectName();
		strProjectName = UtilString::ConvertWideCharToMultiByte(wstr);

		wstr = m_project.GetxupTime();
		strXupTime = UtilString::ConvertWideCharToMultiByte(wstr);
		string strVersion = UtilString::ConvertWideCharToMultiByte(m_project.GetVersion());

		SYSTEMTIME	sysTime;
		char		szTmNow[MAX_PATH] = {0};
		GetLocalTime(&sysTime);
		sprintf_s(szTmNow, MAX_PATH, "%d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		char	szDevType[MAX_PATH] = {0};
		sprintf_s(szDevType, MAX_PATH, "%d", m_project.GetDeviceType());

		if(access.InsertRecordToProjectConfig(m_project.GetProjectID(), strProjectName.c_str(), 
			m_project.GetScreenWidth(), m_project.GetScreenHeight(), strVersion.c_str(), strXupTime.c_str(), strVersion.data(), szTmNow, szDevType, dbset.bEnableMenuGroup)!=0)
		{
			OutputSaveErr(L"InsertRecordToProjectConfig",access.GetSqliteErr());

			Project::Tools::UTF8ToWideChar(access.GetSqliteErr(), wstrErrInfo);
			strErr+= wstrErrInfo.c_str();
			strErr+=_T("\r\n");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//更新sql

	ostringstream sqlstream_set;
	sqlstream_set << "update project_config set HostIP = '"<< dbset.strDBIP <<"', DataBaseName =  '"<< dbset.strDBName <<"',RealTimeDBName = '"<< dbset.strRealTimeDBName <<"',UserName = '"\
		<< dbset.strUserName <<"', AutoLogin = "<< dbset.nAutoLogin << ", DTUPortNumber = "<< dbset.DTUPortNumber \
		<< ", DTUBaudrate = "<< dbset.DTUBaudrate <<", HostIP2 = '"<< dbset.strDBIP2 << "',port = "<< dbset.nPort \
		<< ", unitproperty04 = '"<< strMenuGroupEnabled << "';";
	sql_ = sqlstream_set.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	if (re == SQLITE_OK)
	{
		return true;
	}
	else
	{
		OutputSaveErr(L"update project_config",access.GetSqliteErr());
		Project::Tools::UTF8ToWideChar(access.GetSqliteErr(), wstrErrInfo);
		strErr+= wstrErrInfo.c_str();
		strErr+=_T("\r\n");
		return false;
	}
	
}
bool CProjectSqlite::SaveProjectSqlite_pages()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	PAGETYPE pagetype = FULLSCREEN;
	const vector<CEqmDrawPage*>& pagelist = m_project.GetPageList();        

	//// 判断页面是否改变过序列
	//if (m_project.m_bIsPageSort)
	//{	// 保存新序列至sqlite
	//	vector<CEqmDrawPage*>::const_iterator	iter;
	//	int i = 0;
	//	for (iter=pagelist.begin(); iter!=pagelist.end(); ++iter,++i)
	//	{
	//		access.UpdatePageShowOrder((*iter)->GetID(), i+1);
	//	}
	//	m_project.m_bIsPageSort = false;
	//}
	bool bSave = true;

	USES_CONVERSION;
	char szWebAddr[MAX_PATH] = {0};
	size_t nPageSize = pagelist.size();
	for (UINT pageIdx = 0; pageIdx < nPageSize; pageIdx++)
	{
		m_pPage = pagelist[pageIdx];
		if (m_pPage->m_bNeedSave)
		{
			string strType;
			string strName;
			if(!m_pPage)
			{
				continue;
			}
			pagetype = m_pPage->GetPageType();
			if(pagetype == FULLSCREEN)
			{
				wstr = _T("fullscreen");
			}
			else if (pagetype == FLOATING)
			{
				wstr = _T("floating");
			}
			else if (pagetype == WEBTYPE)
			{
				wstr = _T("webtype");
			}
			strType = UtilString::ConvertWideCharToMultiByte(wstr);
			wstr = m_pPage->GetPageName();
			strName = UtilString::ConvertWideCharToMultiByte(wstr);
			const int nShow = m_pPage->GetPageShowTopInObserver();
			char szShow[10] = {0};
			_itoa(nShow, szShow, 10);
			sprintf_s(szWebAddr, MAX_PATH, "%s", T2A(m_pPage->GetWebAddress()));
			if (IsPageExist(m_pPage->GetID()))
			{
				if(access.UpdatePage(strType.c_str(), m_pPage->GetStartColor(), m_pPage->GetEndColor(), m_pPage->GetBKImageID(), m_pPage->GetID(), strName.c_str(),   
					m_pPage->GetPageWidth(), m_pPage->GetPageHeight(), m_pPage->GetGroupID(), szShow, szWebAddr, m_pPage->GetPagexPosition(), m_pPage->GetPageyPosition(), m_pPage->GetPageMinRight())!=0)
				{
					OutputSaveErr(L"UpdatePage",access.GetSqliteErr());
					bSave = false;
				}	
			}
			else
			{
				access.DeleteOnePage(m_pPage->GetID());
				if(access.InsertRecordToPage(strType.c_str(), m_pPage->GetStartColor(), m_pPage->GetEndColor(), m_pPage->GetBKImageID(), m_pPage->GetID(), strName.c_str(),   
					m_pPage->GetPageWidth(), m_pPage->GetPageHeight(), m_pPage->GetGroupID(), szShow, szWebAddr, m_pPage->GetPageShowOrder(), m_pPage->GetPagexPosition(), m_pPage->GetPageyPosition(), m_pPage->GetPageMinRight())!=0)
				{
					OutputSaveErr(L"InsertRecordToPage",access.GetSqliteErr());
					bSave = false;
				}
			}
		}
        if(SaveProjectSqlite_devices() == false)
			bSave = false;
        if(SaveProjectSqlite_pipes() == false)
			bSave = false;
        if(SaveProjectSqlite_texts() == false)
			bSave = false;
        if(SaveProjectSqlite_graphics() == false)
			bSave = false;
		if(SaveProjectSqlite_dashboard() == false)
			bSave = false;
		if(SaveProjectSqlite_LineChart() == false)
			bSave = false;
		if(SaveProjectSqlite_BarChart() == false)
			bSave = false;
		if(SaveProjectSqlite_PieChart() == false)
			bSave = false;
		if(SaveProjectSqlite_Bend() == false)
			bSave = false;
		if(SaveProjectSqlite_Button() == false)
			bSave = false;
		if(SaveProjectSqlite_TimeSelect() == false)
			bSave = false;
		if(SaveProjectSqlite_TimeBar() == false)
			bSave = false;
		if(SaveProjectSqlite_ProgressBar() == false)
			bSave = false;
		if(SaveProjectSqlite_DiagnoseChart() == false)
			bSave = false;
		if(SaveProjectSqlite_Polygon() == false)
			bSave = false;
		if(SaveProjectSqlite_AreaChange() == false)
			bSave = false;
		if(SaveProjectSqlite_ShellExecute() == false)
			bSave = false;
		if(SaveProjectSqlite_EmbededPage() == false)
			bSave = false;
		if(SaveProjectSqlite_LiquidLevel() == false)
			bSave = false;
		if(SaveProjectSqlite_CheckButtonPlus() == false)
			bSave = false;
		if(SaveProjectSqlite_Scrawl() == false)
			bSave = false;
		if(SaveProjectSqlite_HistoryTrend() == false)
			bSave = false;
		if(SaveProjectSqlite_EnergySaveRoi() == false)
			bSave = false;
		if(SaveProjectSqlite_MapNavigate() == false)
			bSave = false;
		if(SaveProjectSqlite_NavigateWindow() == false)
			bSave = false;
		if(SaveProjectSqlite_LevelRuler() == false)
			bSave = false;
		if(SaveProjectSqlite_DataAnalyse() == false)
			bSave = false;
		if(SaveProjectSqlite_TemperDistri() == false)
			bSave = false;
		if(SaveProjectSqlite_DataReport() == false)
			bSave = false;
		if(SaveProjectSqlite_PlaneTempeDistr() == false)
			bSave = false;
		if(SaveProjectSqlite_DottedLineFrm() == false)
			bSave = false;
		if(SaveProjectSqlite_GeneralComponent() == false)
			bSave = false;
		if(SaveProjectSqlite_Rectangle() == false)
			bSave = false;
		if(SynchronousPlaneTempeDistr() == false)
			bSave = false;
		if(SaveProjectSqlite_StraightLine() == false)
			bSave = false;
    }

	/*
		判断页面是否改变过序列
		修改：
			2020-08-12：从上边挪下来，因为如果在页面导入表前做此操作是无效的。
	*/
	if (m_project.m_bIsPageSort)
	{	// 保存新序列至sqlite
		vector<CEqmDrawPage*>::const_iterator	iter;
		int i = 0;
		for (iter=pagelist.begin(); iter!=pagelist.end(); ++iter,++i)
		{
			access.UpdatePageShowOrder((*iter)->GetID(), i+1);
		}
		m_project.m_bIsPageSort = false;
	}
	return bSave;
}

bool CProjectSqlite::SaveProjectSqlite_devices()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

    const list<CEqmDrawDevice*>& deviceList = m_pPage->GetDrawDeviceList();
    if (deviceList.empty())
	{
        return true;
	}
    CEqmDrawDevice* pDevice  = NULL;
    list<CEqmDrawDevice*>::const_iterator it = deviceList.begin();

	access.BeginTransaction();

    for (; it != deviceList.end(); it++)
    {
		if ((*it)->GetNeedSave())
		{
			string strDataComID;
			string strEventInfo;
			string strBindInfo;
			string strBtnText;

			pDevice = *it;
			if(!pDevice)
			{
				continue;
			}
			//datacomid
			wstring strID = pDevice->GetPhysicalID();
			strDataComID = UtilString::ConvertWideCharToMultiByte(strID);
			//eventInfo
			const EventInfo& info = pDevice->GetEventInfo();
			string strName = UtilString::ConvertWideCharToMultiByte(info.strSettingPointName.GetString());
			int nDeviceType = (int)(info.deviceType);
			int nMouseEventType = (int)(info.mouseEventType);
			int nDeviceID = info.nDeviceID;
			int nPageID = info.nPageID;
			int nValue = info.nValue;
			string strShowName = UtilString::ConvertWideCharToMultiByte(info.strShowName.GetString());
			string strOutlink = UtilString::ConvertWideCharToMultiByte(info.strOutLink.GetString());
			char szTemp[100] = {0};
			sprintf_s(szTemp, sizeof(szTemp), "%d", nDeviceType);
			strEventInfo = strName + "," + szTemp;
			memset(szTemp, 0, sizeof(szTemp));
			sprintf_s(szTemp, sizeof(szTemp), "%d", nMouseEventType);
			strEventInfo = strEventInfo + "," + szTemp;
			memset(szTemp, 0, sizeof(szTemp));
			sprintf_s(szTemp, sizeof(szTemp), "%d", nDeviceID);
			strEventInfo = strEventInfo + "," + szTemp;
			memset(szTemp, 0, sizeof(szTemp));
			sprintf_s(szTemp, sizeof(szTemp), "%d", nPageID);
			strEventInfo = strEventInfo + "," + szTemp;
			memset(szTemp, 0, sizeof(szTemp));
			sprintf_s(szTemp, sizeof(szTemp), "%d", nValue);
			strEventInfo = strEventInfo + "," + szTemp;
			strEventInfo = strEventInfo + "," + strShowName;
			strEventInfo = strEventInfo + "," + strOutlink;
			//bindInfo
			CString strParam10;
			const size_t size = pDevice->m_vecBindInfo.size();
			for (size_t i=0;i<size;++i)
			{
				if (pDevice->m_vecBindInfo[i].nPicID != 0)
				{
					const int nValue = pDevice->m_vecBindInfo[i].nValue;
					CString strValue;
					strValue.Format(L"%d", nValue);
					const int nPicID = pDevice->m_vecBindInfo[i].nPicID;
					CString strPicID;
					strPicID.Format(L"%d",nPicID);
					const int nFrame = pDevice->m_vecBindInfo[i].nFrame;
					CString strFrame;
					strFrame.Format(L"%d", nFrame);
					const int nInterval = pDevice->m_vecBindInfo[i].nInterval;
					CString strInterval;
					strInterval.Format(L"%d", nInterval);
					strParam10 += strValue+L","+strPicID+L","+strFrame+L","+strInterval;
					if (i != size-1)
					{
						strParam10 += L"|";
					}
				}
			}
			wstring strBind = strParam10.GetString();
			strBindInfo = UtilString::ConvertWideCharToMultiByte(strBind);
			CString strDiagnosePage = L"";
			{
				const vector<PageIDName>& vec = pDevice->GetDiagnosePageVec();
				for (size_t i=0;i<vec.size();++i)
				{
					const int nID = vec[i].nPageID;
					const CString strName = vec[i].strPageName;
					CString strID;
					strID.Format(L"%d",nID);
					strDiagnosePage += strID+L","+strName;
					if (i != vec.size()-1)
					{
						strDiagnosePage += L"|";
					}
				}
			}
			access.DeleteOneDevice(pDevice->GetID());
			access.DeleteOneEvent(m_pPage->GetID(), pDevice->GetID());
			if(pDevice->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToContainElements(pDevice->GetID(), UtilString::ConvertWideCharToMultiByte(pDevice->GetEqmName()).c_str(), 
					(int)pDevice->GetEqmType(), pDevice->GetPos().X, pDevice->GetPos().Y, pDevice->GetWidth(), pDevice->GetHeight(), 
					strDataComID.c_str(), pDevice->GetLayer(), m_pPage->GetID(), pDevice->GetRotateAngle(), pDevice->GetLinkPageID(), 
					strBtnText.c_str(), pDevice->GetPicID(), UtilString::ConvertWideCharToMultiByte(strDiagnosePage.GetString()).c_str(),
					(int)pDevice->GetEquipmentSubType(), pDevice->GetImageMetaType(),pDevice->GetBindType())!=0)
				{
					OutputSaveErr(L"InsertRecordToContainElements",access.GetSqliteErr());
				}
				if(access.InsertRecordToEvent(m_pPage->GetID(), pDevice->GetID(), strEventInfo.c_str(),strBindInfo.c_str())!=0)
				{
					OutputSaveErr(L"InsertRecordToEvent",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

bool CProjectSqlite::SaveProjectSqlite_pipes( )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
    const list<CEqmDrawPipe*>& pipeList = m_pPage->GetPagePipeList();
    if (pipeList.empty())
	{
        return true;
	}
    string strTemp;

    CEqmDrawPipe* pPipe  = NULL;
    list<CEqmDrawPipe*>::const_iterator it = pipeList.begin();

	access.BeginTransaction();
    for (; it != pipeList.end(); it++)
    {
		if ((*it)->GetNeedSave())
		{
			pPipe= *it;
			if(!pPipe)
			{
				continue;
			}
			CString str;
			CString strID = _T("");
			for(UINT idx=0; idx< pPipe->GetDataComIDVector().size(); idx++)
			{
				str = pPipe->GetDataComIDVector()[idx];
				if (idx != pPipe->GetDataComIDVector().size()-1)
				{
					str += _T(",");
				}
				strID += str;
			}
			string strTemp;
			if (strID.GetLength() > 0)
			{
				strTemp = UtilString::ConvertWideCharToMultiByte(strID.GetString());
			}
			access.DeleteOnePipe(pPipe->GetID());
			if(pPipe->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToLine(m_pPage->GetID(), pPipe->GetID(), (int)pPipe->GetDirection(), pPipe->GetPipeColor().innercolor, pPipe->GetPipeColor().outercolor, 
					pPipe->GetPointStringList().c_str(), pPipe->GetPipeWidth(), pPipe->GetLayer(), strTemp.c_str(), pPipe->m_nLogic, pPipe->GetPipeType(), pPipe->GetPipeBitMapID(), pPipe->GetPipeDenSity(), pPipe->GetPipeSpeed(), pPipe->GetWaterFashion())!=0)
				{
					OutputSaveErr(L"InsertRecordToLine",access.GetSqliteErr());
				}
			}
		}
    }

	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

bool CProjectSqlite::SaveProjectSqlite_texts()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

    const list<CEqmDrawText*>& textlist = m_pPage->GetPageTextList();
    if (textlist.empty())
	{
        return true;
	}


    CEqmDrawText* pText  = NULL;
    list<CEqmDrawText*>::const_iterator it = textlist.begin();

	access.BeginTransaction();

	USES_CONVERSION;
    for (; it != textlist.end(); it++)
    {
		if ((*it)->GetNeedSave())
		{
			string strinitialValue;
			string strFontName;
			string strDataComID;
			int align = 0;
			int temp = 0;
			wstring wstr;
			char szDevCfg[MAX_PATH] = {0};
			char szTemperfg[MAX_PATH] = {0};
			char szFaultDiag[MAX_PATH] = {0};

			pText= *it;
			if(!pText)
			{
				continue;
			}
			//initialValue
			wstr = pText->GetInitialstring();
			strinitialValue = UtilString::ConvertWideCharToMultiByte(wstr);
			//font style
			wstr = pText->GetFontName();
			strFontName = UtilString::ConvertWideCharToMultiByte(wstr);
			//datacomid, write phisical id
			wstr = pText->GetPhysicalID();
			strDataComID = UtilString::ConvertWideCharToMultiByte(wstr);
			
			CString strText				= pText->GetTextModeShowString();
			CString strDateTime			= pText->GetDateTime();
			CString strDateTimePoint	= pText->GetDateTimePoint();
			CString cstrUnit			= pText->GetUnit();
			string strTextModeShowString	= UtilString::ConvertWideCharToMultiByte(strText.GetString());
			string strDateTimeString		= UtilString::ConvertWideCharToMultiByte(strDateTime.GetString());
			string strDateTimePointString	= UtilString::ConvertWideCharToMultiByte(strDateTimePoint.GetString());
			string strUnit					= UtilString::ConvertWideCharToMultiByte(cstrUnit.GetString());
			sprintf_s(szDevCfg, MAX_PATH, "%s", T2A(pText->GetDeviceCfg()));
			//sprintf_s(szTemperfg, MAX_PATH, "%d", T2A(pText->GetTemperCfg()));
			sprintf_s(szFaultDiag, MAX_PATH, "%s", T2A(pText->GetBindScript()));
			
			access.DeleteOneText(pText->GetID());
			if(pText->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToText(pText->GetID(),m_pPage->GetID(), strinitialValue.c_str(),(int)pText->GetPos().X,(int)pText->GetPos().Y,
					pText->GetWidth(),pText->GetHeight(),pText->GetFontSize(),pText->GetFontColor(),strFontName.c_str(),strDataComID.c_str(),
					(int)pText->GetTextShowMode(),pText->GetTransparence(),pText->GetBorderType(),pText->GetBorderColor(),pText->GetLayer(),
					pText->GetAlign(),strTextModeShowString.c_str(),pText->GetPointNum(),strDateTimeString.c_str(),strDateTimePointString.c_str(), pText->GetTextBKType(), pText->GetTextAttribute(),
					szDevCfg,strUnit.c_str(), szFaultDiag)!=0)
				{
					OutputSaveErr(L"InsertRecordToText",access.GetSqliteErr());
				}
			}
		}
    }

	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

bool CProjectSqlite::SaveProjectSqlite_graphics( )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
    const list<CEqmDrawGraphic*>& graphiclist = m_pPage->Get2DGraphicList();
    if (graphiclist.empty() )
	{
        return true;
	}
    CEqmDrawGraphic* pGraphic  = NULL;
    list<CEqmDrawGraphic*>::const_iterator it = graphiclist.begin();

	access.BeginTransaction();

    for (; it != graphiclist.end(); it++)
    {
		if ((*it)->GetNeedSave())
		{
			pGraphic= *it;
			if(!pGraphic)
			{
				continue;
			}
			access.DeleteOneGraphic(pGraphic->GetID());
			if(pGraphic->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToGraphics(pGraphic->GetID(),m_pPage->GetID(),pGraphic->GetEqmType(),pGraphic->GetLineColor(),
					pGraphic->GetLineStyle(),pGraphic->GetLineWidth(),pGraphic->GetFilledColor(),pGraphic->GetPointStringList().c_str(),pGraphic->GetForwardPageID(),
					pGraphic->GetTransparent(),(int)pGraphic->GetBorderIs3D(),pGraphic->GetLayer())!=0)
				{
					OutputSaveErr(L"InsertRecordToGraphics",access.GetSqliteErr());
				}
			}
		}
    }

	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////


bool CProjectSqlite::LoadProjectConfig()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	m_project.ClearProject();
	ostringstream sqlstream;
	sqlstream << "select * from project_config;";
	string sql_ = sqlstream.str();

	if (SQLITE_OK != access.SqlQuery(sql_.c_str()))
	{
		return false;
	}

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		int iProjectID = access.getColumn_Int(0);
		m_project.SetProjectID(iProjectID);

		if(access.getColumn_Text(1) != NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			wstring   name = UtilString::ConvertMultiByteToWideChar(name_temp);
			m_project.SetProjectName(name);
		}

		int ScreenWidth = access.getColumn_Int(2);
		m_project.SetScreenWidth(ScreenWidth);

		int ScreenHeight = access.getColumn_Int(3);
		m_project.SetScreenHeight(ScreenHeight);
		string version;
		if (access.getColumn_Text(4))
		{
			version = access.getColumn_Text(4);
		}
		m_project.SetVersion(UtilString::ConvertMultiByteToWideChar(version));

		if(access.getColumn_Text(5) != NULL)
		{
			string   xupTime_temp(const_cast<char*>(access.getColumn_Text(5)));
			wstring   xupTime = UtilString::ConvertMultiByteToWideChar(xupTime_temp);
			m_project.SetxupTime(xupTime);
		}

		if(access.getColumn_Text(6) != NULL)
		{
			string  dbip(const_cast<char*>(access.getColumn_Text(6)));
			m_project.m_DBSetting.strDBIP = dbip;
		}

		if(access.getColumn_Text(7) != NULL)
		{
			string  dbname(const_cast<char*>(access.getColumn_Text(7)));
			m_project.m_DBSetting.strDBName = dbname;
		}

		if(access.getColumn_Text(8) != NULL)
		{
			string  rtdbname(const_cast<char*>(access.getColumn_Text(8)));
			m_project.m_DBSetting.strRealTimeDBName = rtdbname;
		}

		if(access.getColumn_Text(9) != NULL)
		{
			string  uname(const_cast<char*>(access.getColumn_Text(9)));
			m_project.m_DBSetting.strUserName = uname;
		}


		m_project.m_DBSetting.nAutoLogin = access.getColumn_Int(10);

		m_project.m_DBSetting.DTUPortNumber = access.getColumn_Int(11);

		m_project.m_DBSetting.DTUBaudrate = access.getColumn_Int(12);

		if(access.getColumn_Text(13)!=NULL)
		{
			string  dbip2(const_cast<char*>(access.getColumn_Text(13)));
			m_project.m_DBSetting.strDBIP2 = dbip2;
		}
		m_project.m_DBSetting.nPort = access.getColumn_Int(14);

		if(access.getColumn_Text(15)!=NULL)
		{
			string  dbsoftversion(const_cast<char*>(access.getColumn_Text(15)));
			m_project.m_DBSetting.strLastEditSoftVersion = dbsoftversion;
		}
		else
		{
			m_project.m_DBSetting.strLastEditSoftVersion = "";
		}

		if (access.getColumn_Text(17) != NULL)
		{
			string  strDevType(const_cast<char*>(access.getColumn_Text(17)));
			int		nDevType = atoi(strDevType.c_str());
			m_project.SetDeviceType(nDevType);
		}

		if (access.getColumn_Text(18) != NULL)
		{
			string  strMenu(const_cast<char*>(access.getColumn_Text(18)));
			if(strMenu=="1")
				m_project.m_DBSetting.bEnableMenuGroup = true;
			else
				m_project.m_DBSetting.bEnableMenuGroup = false;
		}
	}
	access.SqlFinalize();

	return true;
}

wstring CProjectSqlite::LoadProjectSqlite(CProgressCtrl *pProgressCtrl, CStatic *pStatic)
{
	wstring strErrorAll;
	if(pProgressCtrl && pStatic)
	{
		pProgressCtrl->StepIt();
		if(m_bEnglishLanguage)
		{
			pStatic->SetWindowTextW(L"loading config.");
		}
		else
		{
			pStatic->SetWindowTextW(L"加载配置...");
		}
	}
	LoadProjectConfig();

	LoadProjectSqlite_opcid3();
	
	// switch into double click pages when in Factory

	if(pProgressCtrl && pStatic)
	{
		pProgressCtrl->StepIt();
		if(m_bEnglishLanguage)
		{
			pStatic->SetWindowTextW(L"loading page info.");
		}
		else
		{
			pStatic->SetWindowTextW(L"加载页面信息...");
		}
	}

	strErrorAll += LoadGroupPageSqlite();
	
	strErrorAll += LoadProjectSqlite_pages(pProgressCtrl, pStatic);

	if(pProgressCtrl && pStatic)
	{
		pProgressCtrl->StepIt();
		if(m_bEnglishLanguage)
		{
			pStatic->SetWindowTextW(L"loading points.");
		}
		else
		{
			pStatic->SetWindowTextW(L"加载点表...");
		}
	}

	LoadProjectSqlite_LogicPoint();
	LoadProjectSqlite_AlarmPoint();
	LoadProjectSqlite_ListUnit20();

	return strErrorAll;
}

wstring CProjectSqlite::LoadGroupPageSqlite( )
{
	CString strERR;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_pagegroup;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	GroupPage  gp;  
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		gp.GroupID = access.getColumn_Int(0);

		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			gp.GroupName =UtilString::ConvertMultiByteToWideChar(name_temp);
		}

		if (const_cast<char*>(access.getColumn_Text(3)) !=NULL)
		{
			string   strShowOrder(const_cast<char*>(access.getColumn_Text(3)));
			gp.nGroupShowOrder = atoi(strShowOrder.c_str());
		}

		m_project.m_groupgage.push_back(gp);
	}

	access.SqlFinalize();

	return strERR.GetString();
}


wstring CProjectSqlite::LoadProjectSqlite_pages(CProgressCtrl *pProgressCtrl, CStatic *pStatic)
{
	CString strERR;
	int id =0;
	int showOrder = 0;

	PAGETYPE pagetype = FULLSCREEN;
	int width = 0;
	int height = 0;
	int xpt = 0;
	int ypt = 0;
	int pagewidth = 0;
	int pageheight = 0;
	int backcolor = DEFAULTBACKGROUNDCOLOR;

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_page order by showOrder;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageIndex = 0;
	int nPageCount = 0;
	CString strWebAddr;

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		wstring   type;
		if(access.getColumn_Text(10)!=NULL)
		{
			string   type_temp(const_cast<char*>(access.getColumn_Text(10)));
			type = UtilString::ConvertMultiByteToWideChar(type_temp);
		}

		if(type ==_T("floating"))
		{
			pagetype = FLOATING;
		}
		else if (type == L"fullscreen")
		{
			pagetype = FULLSCREEN;
		}
		else if (type == _T("webtype"))
		{
			pagetype = WEBTYPE;
		}
		//id
		id = access.getColumn_Int(0);
		//name
		wstring   wstr;
		if(access.getColumn_Text(1)!=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			wstr = UtilString::ConvertMultiByteToWideChar(name_temp);
		}

		//show order
		showOrder = access.getColumn_Int(11);

		int groupID = access.getColumn_Int(5);
		
		//bg color
		backcolor = access.getColumn_Int(4);
		//width
		pagewidth = access.getColumn_Int(7);
		//height
		pageheight = access.getColumn_Int(8);
		//xposition
		xpt = access.getColumn_Int(12);
		//yposition
		ypt = access.getColumn_Int(13);
		string strShow;
		if (access.getColumn_Text(14))
		{
			strShow = access.getColumn_Text(14);
		}
		if (NULL != access.getColumn_Text(15))
		{
			string strAddrTmp(const_cast<char*>(access.getColumn_Text(15)));
			strWebAddr.Format(_T("%s"), UtilString::ConvertMultiByteToWideChar(strAddrTmp).c_str());
		}


		int nPageMinRight = access.getColumn_Int(16);

		int nStartColor = access.getColumn_Int(2);
		int nEndColor = access.getColumn_Int(3);
		int nBKImageID = access.getColumn_Int(4);

		if(pProgressCtrl && pStatic)
		{
			CString strTemp;
			if(m_bEnglishLanguage)
			{
				strTemp.Format(_T("loading page(%s) config."), wstr.c_str());
				pStatic->SetWindowTextW(strTemp);
			}
			else
			{
				strTemp.Format(_T("正在加载页面(%s) 配置..."), wstr.c_str());
				pStatic->SetWindowTextW(strTemp);
			}
		}

		m_pPage = new CEqmDrawPage(id, wstr, m_project.GetProjectID(), showOrder,pagetype,pagewidth,pageheight,xpt,ypt, backcolor);

		m_pPage->m_GroupID = groupID;
		m_pPage->SetStartColor(nStartColor);
		m_pPage->SetEndColor(nEndColor);
		m_pPage->SetBKImageID(nBKImageID);
		m_pPage->SetPageMinRight(nPageMinRight);
		if (strShow=="0")
		{
			m_pPage->SetPageShowTopInObserver(false);
		}
		else
		{
			m_pPage->SetPageShowTopInObserver(true);
		}
		m_pPage->SetWebAddress(strWebAddr);
		
		ASSERT(m_pPage);
		
		if(m_pPage)
		{
			if (nPageIndex == 0)
			{
				m_pPage->m_bIsMainPage = true;
			}
			nPageIndex++;
			m_pPage->m_bNeedSave = false;
			m_project.AddPage(m_pPage);
		}
	}

	
	access.SqlFinalize();

	return strERR.GetString();
}

wstring CProjectSqlite::LoadPageDetails(CProgressCtrl *pProgressCtrl, CStatic *pStatic)
{
	wstring wstrErrorAll;
	if(m_pPage==NULL)
		return wstrErrorAll;

	if(pProgressCtrl && pStatic)
	{
		CString strTemp;
		if(m_bEnglishLanguage)
		{
			strTemp.Format(_T("loading page(%s) basic info."),m_pPage->GetPageName().c_str());
			pStatic->SetWindowTextW(strTemp);
		}
		else
		{
			strTemp.Format(_T("正在加载页面(%s) 基本信息..."), m_pPage->GetPageName().c_str());
			pStatic->SetWindowTextW(strTemp);
		}

		pProgressCtrl->StepIt();
	}

	wstrErrorAll += LoadProjectSqlite_devices();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_pipes();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_texts();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	////////////////////////////////////////////////////////////////
	if(pProgressCtrl && pStatic)
	{
		CString strTemp;
		if(m_bEnglishLanguage)
		{
			strTemp.Format(_T("loading page(%s) advanced info."),m_pPage->GetPageName().c_str());
			pStatic->SetWindowTextW(strTemp);
		}
		else
		{
			strTemp.Format(_T("正在加载页面(%s) 基本信息..."), m_pPage->GetPageName().c_str());
			pStatic->SetWindowTextW(strTemp);
		}
	}

	wstrErrorAll += LoadProjectSqlite_graphics();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_DashBoard();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_LineChart();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_BarChart();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_PieChart();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_Bend();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_Button();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_TimeSelect();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_TimeBar();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_ProgressBar();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_DiagnoseChart();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_Polygon();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_AreaChange();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_ShellExecute();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_EmbededPage();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_LiquidLevel();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_CheckButtonPlus();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_Scrawl();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_HistoryTrend();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_EnergySaveRoi();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	
	wstrErrorAll += LoadProjectSqlite_MapNavigate();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_NavigateWindow();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_LevelRuler();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_DataAnalyse();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_TemperDistri();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_DataReport();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_PlaneTempeDistr();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_DottedLineFrm();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_GeneralComponent();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_Rectangle();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();

	wstrErrorAll += LoadProjectSqlite_StraightLine();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	//if (m_pPage->GetPageName() == L"test")
	//{
	//	osg::ref_ptr<COSGPlusCurve> t = new COSGPlusCurve(100,100,1234,856,0.5,m_OSGFontMap[L"msyh.ttf"],NULL);
	//	m_pPage->m_pOsgNode->addChild(t);
	//}
	m_pPage->m_bDetailLoaded = true;

	return wstrErrorAll;
}

wstring CProjectSqlite::LoadPageDetails(CEqmDrawPage*	pPage,CProgressCtrl *pProgressCtrl, CStatic *pStatic)
{
	wstring wstrErrorAll;
	m_pPage = pPage;

	if(m_pPage==NULL)
		return wstrErrorAll;

	if(pProgressCtrl && pStatic)
	{
		CString strTemp;
		if(m_bEnglishLanguage)
		{
			strTemp.Format(_T("loading page(%s) advanced info."),m_pPage->GetPageName().c_str());
			pStatic->SetWindowTextW(strTemp);
		}
		else
		{
			strTemp.Format(_T("正在加载页面(%s) 基本信息..."), m_pPage->GetPageName().c_str());
			pStatic->SetWindowTextW(strTemp);
		}
		pProgressCtrl->StepIt();
	}

	wstrErrorAll += LoadProjectSqlite_devices();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_pipes();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_texts();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	////////////////////////////////////////////////////////////////
	if(pProgressCtrl && pStatic)
	{
		CString strTemp;
		if(m_bEnglishLanguage)
		{
			strTemp.Format(_T("loading page(%s) advanced info."),m_pPage->GetPageName().c_str());
			pStatic->SetWindowTextW(strTemp);
		}
		else
		{
			strTemp.Format(_T("正在加载页面(%s) 基本信息..."), m_pPage->GetPageName().c_str());
			pStatic->SetWindowTextW(strTemp);
		}
	}

	wstrErrorAll += LoadProjectSqlite_graphics();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_DashBoard();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_LineChart();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_BarChart();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_PieChart();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_Bend();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_Button();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_TimeSelect();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_TimeBar();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_ProgressBar();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_DiagnoseChart();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_Polygon();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_AreaChange();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_ShellExecute();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_EmbededPage();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_LiquidLevel();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_CheckButtonPlus();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_Scrawl();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_HistoryTrend();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_EnergySaveRoi();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_MapNavigate();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_NavigateWindow();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_LevelRuler();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_DataAnalyse();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_TemperDistri();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_DataReport();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_PlaneTempeDistr();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_DottedLineFrm();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_GeneralComponent();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_Rectangle();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	wstrErrorAll += LoadProjectSqlite_StraightLine();
	if(pProgressCtrl)
		pProgressCtrl->StepIt();
	//if (m_pPage->GetPageName() == L"test")
	//{
	//	osg::ref_ptr<COSGPlusCurve> t = new COSGPlusCurve(100,100,1234,856,0.5,m_OSGFontMap[L"msyh.ttf"],NULL);
	//	m_pPage->m_pOsgNode->addChild(t);
	//}
	m_pPage->m_bDetailLoaded = true;

	return wstrErrorAll;
}

wstring CProjectSqlite::LoadProjectSqlite_devices()
{
	CString strError;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	string strPointList;
	wstring wstrName;
	const int pageId = m_pPage->GetID();
	int id = 0;
	EQMDRAWTYPE etype = EQMDRAW_METAFILE;
	EQUIPMENTSUBTYPE eSubType = SUBTYPE_UNKNOWN;
	int xpos=0, ypos=0;
	int width = 0, height = 0;
	int dataComId = 0;
	int rotateAngle = 0;
	double dStepInterval = 0;
	double dUpLimit = 0, dDownLimit = 0;
	int layer = E_LAYER_10;
	const char* pchar = NULL;
	string strDiagnosePage;
	bool bIsPic = true;
	ostringstream sqlstream;
	sqlstream << "select * from page_contain_elements where pageid = " <<pageId<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		string strEvent;
		string strBindString;
		std::wstring strComID;

		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		
		//reset parameter
		layer = E_LAYER_10;
		//id
		id = access.getColumn_Int(2);
		//name
		wstring   wstrName;
		if (access.getColumn_Text(15)!= NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(15)));
			wstrName = UtilString::ConvertMultiByteToWideChar(name_temp);
		}

		//elements type
		if (access.getColumn_Text(1)!= NULL)
		{
			string   type_temp(const_cast<char*>(access.getColumn_Text(1)));
			itemp = FindIndexFromVecString(VEC_TYPE, type_temp);
			etype = (EQMDRAWTYPE)itemp;
		}


		//子类型
		if (access.getColumn_Text(14)!= NULL)
		{
			string   sub_type_temp(const_cast<char*>(access.getColumn_Text(14)));
			itemp = FindIndexFromVecString(VEC_SUB_TYPE, sub_type_temp);
			eSubType = (EQUIPMENTSUBTYPE)itemp;
		}

		//x, y, w, h
		xpos = access.getColumn_Int(3);
		ypos = access.getColumn_Int(4);
		width = access.getColumn_Int(5);
		if(width<=0)
		{
			width = 32;
		}
		height = access.getColumn_Int(6);
		if(height<=0)
		{
			height = 32;
		}

		//datacomid
		dataComId = -1;
		{
			if (access.getColumn_Text(12) != NULL)
			{
				string dataComId_temp(const_cast<char*>(access.getColumn_Text(12)));
				str = dataComId_temp;
				strComID = UtilString::ConvertMultiByteToWideChar(str);
			}
		}

		//rotateAngle/ 
		rotateAngle = 0;  
		rotateAngle = access.getColumn_Double(13);

		//eventInfo_bindInfo
		GetEventAndBindInfoSqlite(pageId, id, strEvent, strBindString);

		//layer 
		layer = DEFAULT_LAYER;
		layer = access.getColumn_Int(7);

		int link = access.getColumn_Int(9);

		wstring  showstring;
		if (access.getColumn_Text(16) != NULL)
		{
			string showstring_temp(const_cast<char*>(access.getColumn_Text(16)));
			showstring = UtilString::ConvertMultiByteToWideChar(showstring_temp);
		}
		int nPicID = access.getColumn_Int(17);
		wstring wcsDiagnosePage;
		if (access.getColumn_Text(18) != NULL)
		{
			strDiagnosePage = access.getColumn_Text(18);
			wcsDiagnosePage = UtilString::ConvertMultiByteToWideChar(strDiagnosePage);
		}

		if (NULL == access.getColumn_Text(19))
		{
			bIsPic = true;
		}
		else
		{
			string strIsPic = access.getColumn_Text(19);
			bIsPic = (strIsPic == "0") ? false : true;
		}

		double dX = 0.0f;
		double dY = 0.0f;
		double dWidth = 0.0f;
		double dHeight = 0.0f;
		
			dX = xpos;
			dY = ypos;
			dWidth = width;
			dHeight = height;

		int nBindType = access.getColumn_Int(20);
		
		CEqmDrawDevice* pDevice = new CEqmDrawDevice(etype, Gdiplus::PointF((float)dX, (float)dY), pageId, id, dataComId, rotateAngle, wstrName, eSubType, dWidth, strPointList, dHeight, dStepInterval, dUpLimit, dDownLimit, nBindType);
		ASSERT(pDevice);
		pDevice->ClearDiagnosePageItemVec();
		CString strName;
		strName.Format(_T("%d"), pDevice->GetID());

		if(!pDevice ) 
		{
			return strError.GetString();
		}
		pDevice->SetLayer(layer);
		pDevice->SetPicID(nPicID);
		pDevice->SetPhysicalID(strComID);
		pDevice->SetBindString(UtilString::ConvertMultiByteToWideChar(strBindString).c_str());

		if (strBindString.size() > 0)
		{
			pDevice->ParseParam10(UtilString::ConvertMultiByteToWideChar(strBindString).c_str());
		}
		if (strEvent.size() > 0)
		{
			pDevice->ParseEvent(UtilString::ConvertMultiByteToWideChar(strEvent).c_str());
		}
		m_pPage->AddDevice(pDevice);

		pDevice->SetLinkPageID(link);

		pDevice->SetImageMetaType(bIsPic);

		PictureInfo* pInfo = NULL;
		wstring strInfoTemp;
		if (bIsPic)
		{
			pInfo = GetPictureInfoByID(pDevice->GetPicID(), true);
		}
		else
		{
			
			pInfo = GetAnimationPicInfoById(pDevice->GetPicID(), true, strInfoTemp);
		}
		strError += strInfoTemp.c_str();
		if (NULL == pInfo)
		{
			CString strMsg;
			strMsg.Format(L"自定义图片图元图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDevice->GetID(), pDevice->GetEqmName().c_str(), pDevice->GetPhysicalID().c_str());
			strError +=strMsg;
			strError +=L"\r\n";
			continue;
		}
		strName.Format(L"%d",pInfo->ID);
		if(!SaveMemBitmapToFile(pInfo->pBlock, pInfo->nSize, strName.GetString()))
		{
			CString strMsg;
			strMsg.Format(L"自定义图片图元图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDevice->GetID(), pDevice->GetEqmName().c_str(), pDevice->GetPhysicalID().c_str());
			strError +=strMsg;
			strError +=L"\r\n";
		}
		{
			if (wcsDiagnosePage.size()>0)
			{
				vector<wstring> vecContent;
				SplitStringByChar(wcsDiagnosePage.c_str(), L'|', vecContent);
				for (size_t i=0;i<vecContent.size();++i)
				{
					if (vecContent[i].size() > 0)
					{
						vector<wstring> vecContentSub;
						SplitStringByChar(vecContent[i].c_str(), L',', vecContentSub);
						const int nID = _wtoi(vecContentSub[0].c_str());
						const CString strPageName = vecContentSub[1].c_str();
						pDevice->InsertDiagnoseVec(nID, strPageName);
					}
				}
			}
			const vector<PageIDName>& vec = pDevice->GetDiagnosePageVec();
			pDevice->InitDiagnoseVec(vec.size());
			for (size_t k=0;k<vec.size();++k)
			{
				vector<wstring> vecPointName;
				GetDiagnoseItemPointNameVec(vec[k].nPageID,vecPointName);
				for (size_t n=0;n<vecPointName.size();++n)
				{
					NameValue st;
					st.strPointName = vecPointName[n];
					pDevice->InsertDiagnosePageItemName(k,st);
				}
			}

		}
		if(m_eOwner == eFactory)
		{
			Image* pBitmap = NULL;
			if (bIsPic)
			{
				pBitmap = GetBitmapByIDFromPictureLib(pDevice->GetPicID(), true);
			}
			else
			{
				wstring wstrErrorInfoTemp;
				PictureInfo* pPicInf = GetAnimationPicInfoById(pDevice->GetPicID(), true, wstrErrorInfoTemp);
				if (pPicInf != NULL)
				{
					pBitmap = pPicInf->pBitmap;
				}
				strError +=wstrErrorInfoTemp.c_str();
			}
			if (pBitmap != NULL)
			{
				pDevice->SetBitmap(pBitmap);
			}
			//else
			//{
			//	CString strTemp;
			//	strTemp.Format(L"Pic ID:%d reading wrong!", pDevice->GetPicID());
			//	CWarningDlg warnDlg;
			//	warnDlg.InitWarn(L"错误", strTemp, TRUE);
			//	warnDlg.DoModal();
			//}
		}

		
		pDevice->SetNeedSave(false);

		//生成动画，原AddPlayInfo()

		const size_t size = pDevice->m_vecBindInfo.size();
		if (size>0)
		{
			MakeDevicePlayInfo(pDevice);
		}

		
	}

	access.SqlFinalize();

	return strError.GetString();
}



wstring CProjectSqlite::LoadProjectSqlite_pipes( )
{
	CString strError;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
    string strPointList;
    wstring wstrName;
    const int pageId = m_pPage->GetID();
    int id=0;
    PipeColor pipecolor; 
    int pipewidth =0;
    int layer = E_LAYER_10;
	int nDirection = 0;
    vector<int> vecDataid;
	ostringstream sqlstream;
	sqlstream << "select * from list_line where list_line.pageid = " <<pageId<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	int nLogic = 0;
	int nPipeType = 0;
	int	nPipeBitMapID = 0;
	int	nPipeDenSity = 0;
	int nPipeSpeed = 0; 
	CString strIDList;
	int nWaterFashion = 0;

    while(true)
    {
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

        //init parameter
        layer = E_LAYER_10;
        vecDataid.clear();
        //id
		id = access.getColumn_Int(1);
		nDirection = access.getColumn_Int(2);
        //color
		pipecolor.innercolor = access.getColumn_Int(10);
		pipecolor.outercolor = access.getColumn_Int(11);
        //point list
        strPointList = access.getColumn_Text(12);

        //pipe width
		pipewidth = access.getColumn_Int(13);

		//pipe type
		nPipeType = access.getColumn_Int(17);

		//pipe BitMapID
		nPipeBitMapID = access.getColumn_Int(18);

		//pipe DenSity
		nPipeDenSity = access.getColumn_Int(19);

		//pipe Speed
		nPipeSpeed = access.getColumn_Int(20);

        //layer        
		layer = access.getColumn_Int(14);
		nLogic = access.getColumn_Int(15);
		if (access.getColumn_Text(16))
		{
			string strTemp = access.getColumn_Text(16);
			strIDList = UtilString::ConvertMultiByteToWideChar(strTemp.c_str()).c_str();
		}
        CEqmDrawPipe* pPipe = new CEqmDrawPipe( pageId, id, pipecolor, pipewidth, strPointList ); //8.9
		//水流形状
		nWaterFashion = access.getColumn_Int(21);
        ASSERT(pPipe);
        if(!pPipe)
		{
            return strError.GetString();
		}
		if (strIDList.GetLength() > 0)
		{
			vector<wstring> vecContent;
			SplitStringByChar(strIDList, L',', vecContent);
			const size_t size = vecContent.size();
			for (size_t i=0;i<size;++i)
			{
				pPipe->SetDataComID(vecContent[i].c_str());
			}
		}
        pPipe->SetLayer(layer);
		pPipe->SetPipeType(nPipeType);
		pPipe->SetPipeSpeed(nPipeSpeed);
		pPipe->SetPipeDenSity(nPipeDenSity);
		pPipe->SetPipeBitMapID(nPipeBitMapID);
		pPipe->SetDirection((_tagPipeDirection)nDirection);
		pPipe->m_nLogic = nLogic;
		pPipe->SetWaterFashion(nWaterFashion);
        m_pPage->AddPipe(pPipe);
		
		pPipe->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strError.GetString();
}

wstring CProjectSqlite::LoadProjectSqlite_texts()
{
	CString strERR;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
    string strPointList;
    wstring wstrName,wstr_fontstyle;
    const int pageId = m_pPage->GetID();
    int id=0;
    int xpos=0, ypos=0, width = 0, height = 0;
    int fontSize = 0, fontColor =0;
    int layer = E_LAYER_10;
    int align = 0; //left = 0
    TEXTSHOWMODE showMode = MODE_VALUE;
    double transparence = 0;
    int textbordetis3d = 0;
    int bordercolor = 0;
    int m_num = 0;
    int m_text = 0;
	int nTextBKType = 0;
	TextAttri textAttri = TEXT_ATTR_NO_USE;
    CString m_strfont;
    const char* pchar = NULL;
	int nPointNum = 0;
	string strTextModeShowString;
	wstring wstrTextModeShowString;
	string strDateTimeString;
	wstring wstrDateTimeString;
	string strDateTimePointString;
	wstring wstrDateTimePointString;
	CString	strDevCfg;
	CString	cstrUnit;
	CString strBindScript;
	

	ostringstream sqlstream;
	sqlstream << "select * from list_text where list_text.pageid = " <<pageId<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

    while(true)
    {
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		wstring wcsPhysicalID;
        //id
		id = access.getColumn_Int(1);
        //x, y ,width ,height
		xpos = access.getColumn_Int(14);
		ypos = access.getColumn_Int(15);
		height = access.getColumn_Int(16);
		width = access.getColumn_Int(17);
		nPointNum = access.getColumn_Int(12);
        //font size, color, style
        fontColor = 0;
		fontSize = access.getColumn_Int(4);
		fontColor = access.getColumn_Int(5);
		
		if (access.getColumn_Text(2) != NULL)
		{
			string   fontstyle_temp(const_cast<char*>(access.getColumn_Text(2)));
			wstr_fontstyle = UtilString::ConvertMultiByteToWideChar(fontstyle_temp);
		}

        CString str_tran;
        str_tran = wstr_fontstyle.c_str();
		if(str_tran.GetAt(0)>='0'
			&& str_tran.GetAt(0)<='9' )
        {
            m_strfont = _T("微软雅黑");
        }
        else
        {
            m_strfont = wstr_fontstyle.c_str();
        }
		{
			if (access.getColumn_Text(18) != NULL)
			{
				string datacomid_str(const_cast<char*>(access.getColumn_Text(18)));
				str = datacomid_str;
				wcsPhysicalID = UtilString::ConvertMultiByteToWideChar(str);
			}
        }

        //initialvalue
		if (access.getColumn_Text(10) != NULL)
		{
			string inivalue_str(const_cast<char*>(access.getColumn_Text(10)));
			str = inivalue_str;
			wstrName = UtilString::ConvertMultiByteToWideChar(str);
		}


        //show mode
        showMode = (TEXTSHOWMODE)DEFAULT_TXT_SHOWMODE;
		showMode = (TEXTSHOWMODE)access.getColumn_Int(19);
		if (showMode == -1)
		{
			showMode = (TEXTSHOWMODE)1;
		}
        //transparence
        transparence = 0;
		transparence = access.getColumn_Double(20);
        //textbordetis3d
        
		textbordetis3d = 0;
		textbordetis3d = access.getColumn_Int(21);
        //bordercolor
        bordercolor = 0;
		bordercolor = access.getColumn_Int(7);

        //layer 
        layer = DEFAULT_LAYER;
		layer = access.getColumn_Int(22);
        
		//alignment
		align = access.getColumn_Int(23);
		if (access.getColumn_Text(24))
		{
			strTextModeShowString = access.getColumn_Text(24);
		}
		if (strTextModeShowString.size() > 0)
		{
			wstrTextModeShowString = UtilString::ConvertMultiByteToWideChar(strTextModeShowString);
			CParseSplitString::Instance().ParseParam3(id, wstrTextModeShowString.c_str());
		}
		if (access.getColumn_Text(25))
		{
			strDateTimeString = access.getColumn_Text(25);
		}
		wstrDateTimeString = UtilString::ConvertMultiByteToWideChar(strDateTimeString);
		if (access.getColumn_Text(26))
		{
			strDateTimePointString = access.getColumn_Text(26);
		}
		wstrDateTimePointString = UtilString::ConvertMultiByteToWideChar(strDateTimePointString);

		nTextBKType = access.getColumn_Int(27);

		textAttri = static_cast<TextAttri>(access.getColumn_Int(28));
		
		const char* pDevCfg = access.getColumn_Text(29);
		if (NULL != pDevCfg)
		{
			wstring wstrDevCfg = UtilString::ConvertMultiByteToWideChar(pDevCfg);
			strDevCfg.Format(L"%s", wstrDevCfg.c_str());
		}

		const char* pUnit = access.getColumn_Text(30);
		if (NULL !=pUnit)
		{
			wstring wstrUnit = UtilString::ConvertMultiByteToWideChar(pUnit);
			cstrUnit.Format(L"%s", wstrUnit.c_str());
		}
	   

		const char* pBindScript = access.getColumn_Text(31);
		bool bFaultDiag = false;
		if (NULL != pBindScript)
		{
			wstring wstrBindScript= UtilString::ConvertMultiByteToWideChar(pBindScript);
			strBindScript.Format(L"%s", wstrBindScript.c_str());
		}

		double dX = 0.0f;
		double dY = 0.0f;
		double dWidth = 0.0f;
		double dHeight = 0.0f;
		
		dX = xpos;
		dY = ypos;
		dWidth = width;
		dHeight = height;
		
        CEqmDrawText* pText = new CEqmDrawText(Gdiplus::PointF((float)dX, (float)dY), pageId, id, dWidth, 
            dHeight, fontSize, m_strfont, wcsPhysicalID,wstrName, fontColor, showMode,transparence,textbordetis3d,bordercolor,
			strDevCfg, bFaultDiag, strBindScript);
        ASSERT(pText);
        if(!pText)
		{
            return strERR.GetString();
		}
        pText->SetLayer(layer);
        pText->SetAlign(align);
		pText->SetPointNum(nPointNum);
		pText->SetTextModeShowString(wstrTextModeShowString.c_str());
		pText->SetDateTime(wstrDateTimeString.c_str());
		pText->SetDateTimePoint(wstrDateTimePointString.c_str());
		pText->SetTextBKType(nTextBKType);
		pText->SetTextAttribute(textAttri);
		pText->SetBindScript(strBindScript);
		pText->SetUnit(cstrUnit);
        m_pPage->AddText(pText);        

		//osg generate

		pText->SetNeedSave(false);
    }
	access.SqlFinalize();

	return strERR.GetString();
}


wstring CProjectSqlite::LoadProjectSqlite_graphics( )
{
	CString strError;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
    string str;
    string strPointList;
    wstring wstrName;
    const int pageId = m_pPage->GetID();
    int id=0, type=0, forwardpageid = 0;
    int filledcolor = 0, linecolor= 0, linewidth= 0, linestyle =0;
    double transparence = 0;
    const int width = 0, height = 0;
    int layer = E_LAYER_10;
    int rectbordetis3d = 0;

    CEqmDrawGraphic* pgrapic = NULL;

	ostringstream sqlstream;
	sqlstream << "select * from list_graphic where list_graphic.pageid = " <<pageId<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

    while(true)
    {
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
        //id
		id = access.getColumn_Int(1);
        //type
		int type = access.getColumn_Int(2);        
        //strpointlist
		if (access.getColumn_Text(3) != NULL)
		{
			string  PL(const_cast<char*>(access.getColumn_Text(3)));
			strPointList = PL;
		}

        //fillcolor
		filledcolor = access.getColumn_Int(3);
        //line color, width , style
		linecolor = access.getColumn_Int(4);
		linewidth = access.getColumn_Int(5);
		linestyle = access.getColumn_Int(6);

        //forwardpageid
		forwardpageid = access.getColumn_Int(7);
        //transparence
		transparence = access.getColumn_Double(8);
        //textbordetis3d
		rectbordetis3d = access.getColumn_Int(9);
        //layer 
		layer = access.getColumn_Int(10);
        //CEqmDrawGraphic
        switch(type)
		{
            case GraphicType_LINE:		//直线
                pgrapic = new CEqmDrawGraphicLine(
                    (EQMDRAWTYPE)type, Gdiplus::PointF(0,0), pageId,  id, linecolor, filledcolor, linewidth, linestyle, 
                    strPointList, forwardpageid, width, height);
                break;
            case GraphicType_RECTANGLE:	//矩形
                pgrapic = new CEqmDrawGraphicRect(
                    (EQMDRAWTYPE)type, Gdiplus::PointF(0,0), pageId,  id, linecolor, filledcolor, linewidth, linestyle, 
                    strPointList, forwardpageid,transparence, width, height,rectbordetis3d);
                break;
            case GraphicType_ELLIPSE:	//椭圆
                pgrapic = new CEqmDrawGraphicEllipse(
                    (EQMDRAWTYPE)type, Gdiplus::PointF(0,0), pageId,  id, linecolor, filledcolor, linewidth, linestyle, 
                    strPointList, forwardpageid, width, height);
                break;
            case GraphicType_POLYGON:	//多边形
                pgrapic = new CEqmDrawGraphicPolygon(
                    (EQMDRAWTYPE)type, Gdiplus::PointF(0,0), pageId,  id, linecolor, filledcolor, linewidth, linestyle, 
                    strPointList, forwardpageid, width, height);
                break;
            case GraphicType_TRIANGLE:
					break;
            default:
                ASSERT(false);
                break;
        }

        ASSERT(pgrapic);
        if(!pgrapic)
		{
            return strError.GetString();
		}
        pgrapic->SetLayer(layer);
        m_pPage->Add2DGraphic(pgrapic);

		pgrapic->SetNeedSave(false);
    }
	access.SqlFinalize();

	return strError.GetString();
}

void CProjectSqlite::ContinueSetEntryToMap()
{
    const UINT vecNum = (UINT)mVecEntryNonDuplicate.size();   //非重复
    for(UINT idx=0; idx<vecNum; idx++){
        const DataPointEntry& entry = mVecEntryNonDuplicate[idx];
        m_pointmap.AddEntry(entry);
    }

}

void CProjectSqlite::UpdateDuplicateVecEntry()
{
    mVecEntryDuplicate.clear();
    mVecEntryNonDuplicate.clear();

    bool ifDup = false;
    const UINT vecNum = (UINT)mVecEntry.size();
    for(UINT idx0 = 0; idx0 < vecNum; idx0++){
        const DataPointEntry& entry0 = mVecEntry[idx0];

        //idx1 init must start from 0
        for(UINT idx1=0; idx1<vecNum; idx1++){  
            if(idx0 == idx1)
                continue;
            const DataPointEntry& entry1 = mVecEntry[idx1];
            ifDup = entry0.CheckIfDuplicate(entry1);
            if(ifDup){
                mVecEntryDuplicate.push_back(entry0);
                break;
            }	
        }
        if(!ifDup)                                   // add 8.15
            mVecEntryNonDuplicate.push_back(entry0);
    }
}


//////////////////////////////////////////////////////////////////////////
void CProjectSqlite::InitTypeString()
{
    mVecStrType.resize(EQMDRAWTYPE_MAX);     

    mVecStrType[EQMDRAW_UNKNOWN]			= "unknown";		//unknown

    mVecStrType[EQMDRAW_CHILLER]			= "chiller_default";		//冷机
    mVecStrType[EQMDRAW_PUMP]				= "pump_default";			//水泵
    mVecStrType[EQMDRAW_COOLINGTOWER]		= "coolingtower_default";	//冷却塔
    mVecStrType[EQMDRAW_VALVE]				= "valve_default";			//阀门
    mVecStrType[EQMDRAW_TEMPUP]				= "temp-up";         //温度上
    mVecStrType[EQMDRAW_TEMPDOWN]			= "temp_down";       //温度下
    mVecStrType[EQMDRAW_FLOW]				= "flow_default";           //流量
    mVecStrType[EQMDRAW_PRESS]				= "press_default";         //压差	
    mVecStrType[EQMDRAW_REDARROW]			= "arrow_red";      //红箭头
    mVecStrType[EQMDRAW_BLUEARROW]			= "arrow_blue";      //蓝箭头
    mVecStrType[EQMDRAW_REDLIGHT]			= "light_red";       //红色灯
    mVecStrType[EQMDRAW_GREENLIGHT]			= "light_green";     //绿色灯
    mVecStrType[EQMDRAW_YELLOWLIGHT]		= "light_yellow";    //黄色灯
    mVecStrType[EQMDRAW_BOOLBUTTON]			= "button_bool";     //bool button
    mVecStrType[EQMDRAW_INCREBUTTON]		= "button_incre";    //increment button
    mVecStrType[EQMDRAW_DECBUTTON]			= "button-dec";      //dec button

    mVecStrType[EQMDRAW_UNSERIAL]			= "device_unserial"; //未知串口设备
    mVecStrType[EQMDRAW_UNPLC]				= "device_unplc";	//未知PLC设备

    mVecStrType[EQMDRAW_POWERMETER]			= "powermeter_default";	//电表。
    mVecStrType[EQMDRAW_AIRBOX]				= "airbox_default";		//空调箱

    mVecStrType[EQMDRAW_TEXT]				= "text";    		//文字
    mVecStrType[EQMDRAW_STRAIGHTPIPE]		= "pipe-straight";	//直管道
    mVecStrType[EQMDRAW_FOLDPIPE]			= "pipe-fold";		//折管道

    mVecStrType[EQMDRAW_MOVE]				= "move";

    mVecStrType[GraphicType_LINE]			= "line";			//直线
    mVecStrType[GraphicType_RECTANGLE]		= "rect";			//矩形
    mVecStrType[GraphicType_TRIANGLE]		= "triangle";		//三角形
    mVecStrType[GraphicType_ELLIPSE]		= "ellipse";		//椭圆
    mVecStrType[GraphicType_POLYGON]		= "polygon";		//多边形
    mVecStrType[GraphicType_ARROW]			= "arrow";			//箭头

    mVecStrType[EQMDRAW_PRESSURE]			= "pressure";		//压力传感器
    mVecStrType[EQMDRAW_MV_DUCT]			= "mv_duct";		//电动风阀执行器
    mVecStrType[EQMDRAW_TH]					= "th";				//温湿度传感器
    mVecStrType[EQMDRAW_RETURNFAN]			= "returnfan";		//回风机
    mVecStrType[EQMDRAW_SUPPLYFAN]			= "supplyfan";		//送风机
    mVecStrType[EQMDRAW_AHU]				= "ahu";			//组合式空气处理机组

	//add by hun 9.5
	mVecStrType[EQMDRAW_YORKCENTRIFUGAL]		= "chilller_yorkcentrifugal";     //约克离心
	mVecStrType[EQMDRAW_YORKSCREW]				= "chilller_yorkscrew";           //约克螺杆
	mVecStrType[EQMDRAW_TRANCECENTRIFUGAL]		= "chilller_trancecentrifugal";   //特灵离心
	mVecStrType[EQMDRAW_TRANCESCREW]			= "chilller_trancescrew";         //特灵螺杆
	mVecStrType[EQMDRAW_CARRIERCENTRIFUGAL]		= "chilller_carriercentrifugal";  //开利离心
	mVecStrType[EQMDRAW_CARRIERSCREW]			= "chilller_carrierscrew";        //开利螺杆
	mVecStrType[EQMDRAW_COOLINGPUMP]			= "pump_cw";						//冷却泵
	mVecStrType[EQMDRAW_PRICHWPUMP]				= "pump_pri";						//一次冷冻泵
	mVecStrType[EQMDRAW_SECCHWPUMP]				= "pump_sec";						//二次冷冻泵
	mVecStrType[EQMDRAW_COOLINGTOWER1]			= "coolingtower1";					//冷却塔一
	mVecStrType[EQMDRAW_COOLINGTOWER2]			= "coolingtower2";					//冷却塔二
	//added by mango 2013/2/20
	mVecStrType[EQMDRAW_METAFILE]				= "metafile";

    //EQMDRAWTYPE_MAX,
}

void CProjectSqlite::SetPage(CEqmDrawPage *pPage)
{
	m_pPage = pPage;
}

void CProjectSqlite::InitSubTypeString()
{
    mVecStrSubType.resize(EQUIPMENTSUBTYPE_MAX);

    mVecStrSubType[SUBTYPE_UNKNOWN] = "sub-unknown";
    mVecStrSubType[EQUIPMENTSUBTYPE_CHILLER_VALVE] = "sub-ch-valve";			// chiller valve
    mVecStrSubType[EQUIPMENTSUBTYPE_COOLINGTOWER_VALVE] = "sub-ct-valve";		// cooling tower valve

    mVecStrSubType[EQUIPMENTSUBTYPE_COOLING_PUMP] = "sub-cool-pump";			// cooling water pump
    mVecStrSubType[EQUIPMENTSUBTYPE_PRICHILLING_PUMP] = "sub-pri-ch-pump";		// primary chilled pump
    mVecStrSubType[EQUIPMENTSUBTYPE_SECCHILLING_PUMP] = "sub-sec-ch-pump";		// secondary chilled pump

    mVecStrSubType[EQUIPMENTSUBTYPE_MV_VALVE] = "sub_mv_valve";
    //mVecStrSubType[EQUIPMENTSUBTYPE_HOT_PUMP] = "sub-hot-pump";				// hot water pump
	//sany add 添加新类型：一次热水泵，二次热水泵
	mVecStrSubType[EQUIPMENTSUBTYPE_PRIHOT_PUMP] = "sub-pri-hot-pump";			// hot water pump
	mVecStrSubType[EQUIPMENTSUBTYPE_SECHOT_PUMP] = "sub-sec-hot-pump";			// hot water pump

    //EQUIPMENTSUBTYPE_MAX, 
}

//if strSearch is number, just use the number.
int  CProjectSqlite::FindIndexFromVecString(E_VEC_TYPE_ eType, const string& strSearch ) const
{
    const vector<std::string>&  vecString = (eType == VEC_TYPE)? mVecStrType : mVecStrSubType;

    const UINT vecNum = (UINT)vecString.size();

    int numberSearch =0 ;
    const wstring wstrSearch = UtilString::ConvertMultiByteToWideChar(strSearch);
    const bool ifNumber = UtilString::CheckStringIsNumber(wstrSearch, numberSearch);
    if(ifNumber){
        return numberSearch;
    }

    int idxret = 0;
    for(UINT idx = 0; idx< vecNum; idx++){
        const string& str0 = vecString[idx];
        if(str0 == strSearch){
            idxret = idx;
            break;
        }
    }
    return idxret;
}

const std::string&    CProjectSqlite::GetVecStringFromIdx(E_VEC_TYPE_ eType, UINT idx) const
{
    const vector<string>& vecString = (VEC_TYPE == eType)? mVecStrType : mVecStrSubType ;
    const UINT vecNum = (UINT)vecString.size();
    if(idx>= vecNum){
        ASSERT(false && "GetVecStringFromIdx" );
        return c_str_null;
    }

    return vecString[idx];    
}
//////////////////////////////////////////////////////////////////////////

const CProjectSqlite::VEC_ENTRY&  CProjectSqlite::GetVecEntry() const
{
    return mVecEntry;
}
const CProjectSqlite::VEC_ENTRY&  CProjectSqlite::GetVecEntryDuplicate() const
{
    return mVecEntryDuplicate;
}

const CProjectSqlite::VEC_ENTRY&  CProjectSqlite::GetVecEntryError() const
{
    return mVecEntryError;
}

void CProjectSqlite::LoadProjectSqlite_opcid3()
{
	if(!C_IF_SAVE_OPCID)
	{
		return ;
	}
	m_pointmap.Clear();
	int tempInt = 0;

	DataPointEntry entry;
	CString cstr;
	const char* pchar = NULL;
	CString strPhysicalID;
	CString strParam3;
	CString strSourceType;
	int i = 1;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_point where list_point.type = '0';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	int nReadWrite = 0;
	const char szFlag[MAX_PATH] = {'\'', '"', ' ', '"', '\''};
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())//100
		{
			break;
		}

		entry.Clear();  //reset entry

		//OPC id
		int pointindex = access.getColumn_Int(0);
		if (pointindex == 0)
		{
			entry.SetPointIndex(i++);
		}
		else
		{
			entry.SetPointIndex(pointindex);
		}
		// get shortname
		//str =pElem->Attribute("physicalid");
		if (access.getColumn_Text(1) !=NULL)
		{
			string   Physicalid_temp(const_cast<char*>(access.getColumn_Text(1)));
			strPhysicalID = Physicalid_temp.c_str();
			wstring strBuf = UtilString::ConvertMultiByteToWideChar(Physicalid_temp);
			entry.SetShortName(strBuf);
			entry.SetShortNameOrigin(strBuf);
		}



		//get point source type
		if (access.getColumn_Text(24) !=NULL)
		{
			string   st_temp(const_cast<char*>(access.getColumn_Text(24)));
			str =st_temp;
			entry.SetSourceType(UtilString::ConvertMultiByteToWideChar(str));
			strSourceType = UtilString::ConvertMultiByteToWideChar(str).c_str();
		}


		//plcid name
		if (access.getColumn_Text(14) !=NULL)
		{
			string   st_temp1(const_cast<char*>(access.getColumn_Text(14)));
			str =st_temp1;
			entry.SetParam(1, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param2
		if (access.getColumn_Text(15) !=NULL)
		{
			string   st_temp2(const_cast<char*>(access.getColumn_Text(15)));
			str =st_temp2;
			entry.SetParam(2, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param3
		if (access.getColumn_Text(16) != NULL)
		{
			string   st_temp3(const_cast<char*>(access.getColumn_Text(16)));
			str =st_temp3;
			strParam3 = UtilString::ConvertMultiByteToWideChar(str).c_str();
			entry.SetParam(3, strParam3.GetString()) ;
		}

		//param4
		if (access.getColumn_Text(17) != NULL)
		{
			string   st_temp4(const_cast<char*>(access.getColumn_Text(17)));
			str =st_temp4;
			entry.SetParam(4, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param5
		if (access.getColumn_Text(18) !=NULL)
		{
			string   st_temp5(const_cast<char*>(access.getColumn_Text(18)));
			str =st_temp5;
			entry.SetParam(5, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param6
		if (access.getColumn_Text(19) != NULL)
		{
			string   st_temp6(const_cast<char*>(access.getColumn_Text(19)));
			str =st_temp6;
			entry.SetParam(6, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param7
		if (access.getColumn_Text(20) != NULL)
		{
			string   st_temp7(const_cast<char*>(access.getColumn_Text(20)));
			str =st_temp7;
			entry.SetParam(7, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param8
		if (access.getColumn_Text(21) != NULL)
		{
			string   st_temp8(const_cast<char*>(access.getColumn_Text(21)));
			str =st_temp8;
			entry.SetParam(8, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param9
		if (access.getColumn_Text(22) !=NULL)
		{
			string   st_temp9(const_cast<char*>(access.getColumn_Text(22)));
			str =st_temp9;
			entry.SetParam(9, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param10
		if (access.getColumn_Text(23) != NULL)
		{
			string   st_temp10(const_cast<char*>(access.getColumn_Text(23)));
			str =st_temp10;
			entry.SetParam(10, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//read write
		nReadWrite = access.getColumn_Int(10);
		entry.SetProperty((PLCVALUEPROPERTY)nReadWrite);
		//remark
		if (access.getColumn_Text(8) != NULL)
		{
			string   st_remarkp(const_cast<char*>(access.getColumn_Text(8)));
			str = st_remarkp;
			entry.SetDescription(UtilString::ConvertMultiByteToWideChar(str));
		}

		//unit
		pchar = access.getColumn_Text(7);
		if(pchar != NULL)
		{
			str = pchar;  
		}
		else
		{
			str = DEFAULT_PLC_UNIT;            
		}
		entry.SetUnit(UtilString::ConvertMultiByteToWideChar(str));
		//high
		if (access.getColumn_Text(25) != NULL)
		{
			string   st_high(const_cast<char*>(access.getColumn_Text(25)));
			str = st_high;
			if (str.size()>0)
			{
				entry.SetHighAlarm(atof(str.c_str()));
			}
		}
		//highhigh -- maxValue
		if (access.getColumn_Text(26) != NULL)
		{
			string   strMaxValue(const_cast<char*>(access.getColumn_Text(26)));
			str = strMaxValue;
			//int nMaxVal = atoi(strMaxValue.c_str());
			//stringstream ss;
			//ss << nMaxVal;
			//ss >> str;
			entry.SetParam(12, UtilString::ConvertMultiByteToWideChar(str));
		}
		//low -- minValue
		if (access.getColumn_Text(27) != NULL)
		{
			string   strMinValue(const_cast<char*>(access.getColumn_Text(27)));
			str = strMinValue;
			//int nMinVal = atoi(strMinValue.c_str());
			//stringstream ss;
			//ss << nMinVal;
			//ss >> str;
			entry.SetParam(13, UtilString::ConvertMultiByteToWideChar(str));
		}
		//lowlow -- isVisited
		if (access.getColumn_Text(28) != NULL)
		{
			string   strIsVisited(const_cast<char*>(access.getColumn_Text(28)));
			int nIsVisited = atoi(strIsVisited.c_str());
			stringstream ss;
			ss << nIsVisited;
			ss >> str;
			entry.SetParam(14, UtilString::ConvertMultiByteToWideChar(str));
		}
		if (access.getColumn_Text(29))
		{
			POINT_STORE_CYCLE cycle = (POINT_STORE_CYCLE)atoi(access.getColumn_Text(29));
			entry.SetStoreCycle(cycle);
		}
		//
		const char* pParam15 = access.getColumn_Text(33);
		if (pParam15 != NULL)
		{
			string strParam15;
			if (0 == strcmp(szFlag, pParam15))
			{
				strParam15 = "";
			}
			else
			{
				strParam15 = const_cast<char*>(pParam15);
			}
			entry.SetParam(15, UtilString::ConvertMultiByteToWideChar(strParam15));
		}
		const char* pParam16 = access.getColumn_Text(34);
		if (pParam16 != NULL)
		{
			string strParam16;
			if (0 == strcmp(szFlag, pParam16))
			{
				strParam16 = "";
			}
			else
			{
				strParam16 = const_cast<char*>(pParam16);
			}
			entry.SetParam(16, UtilString::ConvertMultiByteToWideChar(strParam16));
		}
		const char* pParam17 = access.getColumn_Text(35);
		if (pParam17 != NULL)
		{
			string strParam17;
			if (0 == strcmp(szFlag, pParam17))
			{
				strParam17 = "";
			}
			else
			{
				strParam17 = const_cast<char*>(pParam17);
			}
			entry.SetParam(17, UtilString::ConvertMultiByteToWideChar(strParam17));
		}
		const char* pParam18 = access.getColumn_Text(36);
		if (pParam18 != NULL)
		{
			string strParam18;
			if (0 == strcmp(szFlag, pParam18))
			{
				strParam18 = "";
			}
			else
			{
				strParam18 = const_cast<char*>(pParam18);
			}
			entry.SetParam(18, UtilString::ConvertMultiByteToWideChar(strParam18));
		}

		wstr = UtilString::ConvertMultiByteToWideChar(str);
		entry.SetUnit(wstr);
		entry.m_bNeedSave = false;
		m_pointmap.AddEntry(entry);
	}

	access.SqlFinalize();
}

bool CProjectSqlite::SaveProjectSqlite_opcid3()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	map<int, DataPointEntry>& mapData = m_pointmap.GetPointMap();
	map<int, DataPointEntry>::iterator it = mapData.begin();
	map<int, DataPointEntry>::const_iterator end = mapData.end();
	int i = 1;
	bool bOK = false;
	int nNeedSave = 0;
	int nSaved = 0;
	access.BeginTransaction();
	int nCount = 0;

	vector<CString>	vecErr;
	for ( ; it != end; it++ )
	{	
		if ((*it).second.m_bNeedSave || 0 == (*it).second.GetPointIndex())
		{
			nNeedSave++;
			string strName;
			string strSourceType;
			string strParam1;
			string strParam2;
			string strParam3;
			string strParam4;
			string strParam5;
			string strParam6;
			string strParam7;
			string strParam8;
			string strParam9;
			string strParam10;
			string strParam11;
			string strParam12;
			string strParam13;
			string strParam14;
			string strParam15;
			string strParam16;
			string strParam17;
			string strParam18;
			string strDescription;
			string strUnit;
			string strNameOrig;
			DataPointEntry& info = (*it).second;
			CString	strErr;
			//if (!CheckEntryValid(info, strErr))
			//{
			//	vecErr.push_back(strErr);
			//}
			//name
			strName = UtilString::ConvertWideCharToMultiByte(info.GetShortName());
			//SourceType
			strSourceType = UtilString::ConvertWideCharToMultiByte(info.GetSourceType());
			//param1 点位值
			strParam1 = UtilString::ConvertWideCharToMultiByte(info.GetParam(1));
			//param2 点位值
			strParam2 = UtilString::ConvertWideCharToMultiByte(info.GetParam(2));
			//param3 点位值
			strParam3 = UtilString::ConvertWideCharToMultiByte(info.GetParam(3));
			//param4 点位值
			strParam4 = UtilString::ConvertWideCharToMultiByte(info.GetParam(4));
			//param5 点位值
			strParam5 = UtilString::ConvertWideCharToMultiByte(info.GetParam(5));
			//param6 点位值
			strParam6 = UtilString::ConvertWideCharToMultiByte(info.GetParam(6));
			//param7 点位值
			strParam7 = UtilString::ConvertWideCharToMultiByte(info.GetParam(7));
			//param8 点位值
			strParam8 = UtilString::ConvertWideCharToMultiByte(info.GetParam(8));
			//param9 点位值
			strParam9 = UtilString::ConvertWideCharToMultiByte(info.GetParam(9));
			//param10 点位值
			strParam10 = UtilString::ConvertWideCharToMultiByte(info.GetParam(10));
			//param11 点位值
			strParam11 = UtilString::ConvertWideCharToMultiByte(info.GetParam(11));
			//param12 点位值
			strParam12 = UtilString::ConvertWideCharToMultiByte(info.GetParam(12));
			//param13 点位值
			strParam13 = UtilString::ConvertWideCharToMultiByte(info.GetParam(13));
			//param14 点位值
			strParam14 = UtilString::ConvertWideCharToMultiByte(info.GetParam(14));
			//param15 点位值
			strParam15 = UtilString::ConvertWideCharToMultiByte(info.GetParam(15));
			//param16 点位值
			strParam16 = UtilString::ConvertWideCharToMultiByte(info.GetParam(16));
			
			//param17 点位值
			strParam17 = UtilString::ConvertWideCharToMultiByte(info.GetParam(17));
			
			//param18 点位值
			strParam18 = UtilString::ConvertWideCharToMultiByte(info.GetParam(18));
			
			//remark
			strDescription = UtilString::ConvertWideCharToMultiByte(info.GetDescription());
			//单位 unit
			strUnit = UtilString::ConvertWideCharToMultiByte(info.GetUnit());
			strNameOrig = UtilString::ConvertWideCharToMultiByte(info.GetShortNameOrigin());
			char szCycle[10] = {0};
			sprintf_s(szCycle,sizeof(szCycle),"%d",(int)info.GetStoreCycle());
			assert(strlen(szCycle)>0);
			access.DeleteOnePoint(strName.c_str());
			int nIndex = 0;
			if (0 == (*it).second.GetPointIndex())
			{
				nIndex = i++;
			}
			else
			{
				nIndex = (*it).second.GetPointIndex();
			}

			if ((*it).second.GetShortName() != (*it).second.GetShortNameOrigin())
			{	// have been changed name
				string strOriName = UtilString::ConvertWideCharToMultiByte((*it).second.GetShortNameOrigin());
				access.DeleteOnePoint(strOriName.c_str());
			}
			if(access.InsertRecordToOPCPoint(nIndex,0,strName.c_str(),strSourceType.c_str(),info.GetProperty(),strDescription.c_str(),strUnit.c_str(),
				info.GetHighAlarm(),info.GetHighHighAlarm(),info.GetLowAlarm(),info.GetLowLowAlarm(),strParam1.c_str(),
				strParam2.c_str(),strParam3.c_str(),strParam4.c_str(),strParam5.c_str(),strParam6.c_str(),strParam7.c_str(),
				strParam8.c_str(),strParam9.c_str(),strParam10.c_str(),szCycle,strParam12.c_str(),strParam13.c_str(),strParam14.c_str(),
				strParam15.c_str(),strParam16.c_str(), strParam17.c_str(), strParam18.c_str()) == 0)
			{
				nSaved++;
				m_pointmap.SetPointNeedSave((*it).second.GetShortName(), false);//add by golding 20180609
				m_pointmap.SetPointShortNameOrigin((*it).second.GetShortName(), (*it).second.GetShortName());
			}
			else
			{
				OutputSaveErr(L"InsertRecordToOPCPoint",access.GetSqliteErr());
			}
		}
	}

	if (vecErr.size() > 0)
	{
		CString strErrShow;
		vector<CString>::const_iterator	iter;
		for (iter=vecErr.begin(); iter!=vecErr.end(); iter++)
		{
			strErrShow += *iter + _T("\r\n");
		}
		MessageBox(NULL, strErrShow, _T("错误"), MB_OK|MB_ICONERROR);
	}

	if (nNeedSave == nSaved)
	{
		bOK = true;
	}
	access.CommitTransaction();
	return bOK;
}

wstring CProjectSqlite::MakeDevicePlayInfo(CEqmDrawDevice *pDevice)
{
	CString strError;
	

	return strError.GetString();
}


wstring CProjectSqlite::LoadProjectSqlite_LogicPoint()
{
 	CString strERR;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	int tempInt = 0;
	CLogicDataPointEntry entry;
	CString cstr;
	const char* pchar = NULL;
	int i = 1;
	ostringstream sqlstream;
	sqlstream << "select * from list_point where list_point.type = '1';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		entry.Clear();  //reset entry
		if (access.getColumn_Text(1) != NULL)
		{
			string idname(access.getColumn_Text(1));
			str = idname;
			entry.SetNameID(UtilString::ConvertMultiByteToWideChar(str));
		}
		// logic type
		if (access.getColumn_Text(49) != NULL)
		{
			string logic_type(access.getColumn_Text(49));
			str = logic_type;
			entry.SetLogicType(UtilString::ConvertMultiByteToWideChar(str));
		}

		//param1
		if (access.getColumn_Text(14) != NULL)
		{
			string   st_temp1(const_cast<char*>(access.getColumn_Text(14)));
			str =st_temp1;
			entry.SetParam(1, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param2
		if (access.getColumn_Text(15) != NULL)
		{
			string   st_temp2(const_cast<char*>(access.getColumn_Text(15)));
			str =st_temp2;
			entry.SetParam(2, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param3
		if (access.getColumn_Text(16) != NULL)
		{
			string   st_temp3(const_cast<char*>(access.getColumn_Text(16)));
			str =st_temp3;
			entry.SetParam(3, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param4
		if (access.getColumn_Text(17) != NULL)
		{
			string   st_temp4(const_cast<char*>(access.getColumn_Text(17)));
			str =st_temp4;
			entry.SetParam(4, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param5
		if (access.getColumn_Text(18) != NULL)
		{
			string   st_temp5(const_cast<char*>(access.getColumn_Text(18)));
			str =st_temp5;
			entry.SetParam(5, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param6
		if (access.getColumn_Text(19) != NULL)
		{
			string   st_temp6(const_cast<char*>(access.getColumn_Text(19)));
			str =st_temp6;
			entry.SetParam(6, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param7
		if (access.getColumn_Text(20) != NULL)
		{
			string   st_temp7(const_cast<char*>(access.getColumn_Text(20)));
			str =st_temp7;
			entry.SetParam(7, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param8
		if (access.getColumn_Text(21) != NULL)
		{
			string   st_temp8(const_cast<char*>(access.getColumn_Text(21)));
			str =st_temp8;
			entry.SetParam(8, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param9
		if (access.getColumn_Text(22) != NULL)
		{
			string   st_temp9(const_cast<char*>(access.getColumn_Text(22)));
			str =st_temp9;
			entry.SetParam(9, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param10
		if (access.getColumn_Text(23) != NULL)
		{
			string   st_temp10(const_cast<char*>(access.getColumn_Text(23)));
			str =st_temp10;
			entry.SetParam(10, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param11
		if (access.getColumn_Text(29) != NULL)
		{
			string   st_temp11(const_cast<char*>(access.getColumn_Text(29)));
			str =st_temp11;
			entry.SetParam(11, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param12
		if (access.getColumn_Text(30) != NULL)
		{
			string   st_temp12(const_cast<char*>(access.getColumn_Text(30)));
			str =st_temp12;
			entry.SetParam(12, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param13
		if (access.getColumn_Text(31) != NULL)
		{
			string   st_temp13(const_cast<char*>(access.getColumn_Text(31)));
			str =st_temp13;
			entry.SetParam(13, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param14
		if (access.getColumn_Text(32) != NULL)
		{
			string   st_temp14(const_cast<char*>(access.getColumn_Text(32)));
			str =st_temp14;
			entry.SetParam(14, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param15
		if (access.getColumn_Text(33) != NULL)
		{
			string   st_temp15(const_cast<char*>(access.getColumn_Text(33)));
			str =st_temp15;
			entry.SetParam(15, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param16
		if (access.getColumn_Text(34) != NULL)
		{
			string   st_temp16(const_cast<char*>(access.getColumn_Text(34)));
			str =st_temp16;
			entry.SetParam(16, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param17
		if (access.getColumn_Text(35) != NULL)
		{
			string   st_temp17(const_cast<char*>(access.getColumn_Text(35)));
			str =st_temp17;
			entry.SetParam(17, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param18
		if (access.getColumn_Text(36) != NULL)
		{
			string   st_temp18(const_cast<char*>(access.getColumn_Text(36)));
			str =st_temp18;
			entry.SetParam(18, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param19
		if (access.getColumn_Text(37) != NULL)
		{
			string   st_temp19(const_cast<char*>(access.getColumn_Text(37)));
			str =st_temp19;
			entry.SetParam(19, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param20
		if (access.getColumn_Text(38) != NULL)
		{
			string   st_temp20(const_cast<char*>(access.getColumn_Text(38)));
			str =st_temp20;
			entry.SetParam(20, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param21
		if (access.getColumn_Text(39) != NULL)
		{
			string   st_temp21(const_cast<char*>(access.getColumn_Text(39)));
			str =st_temp21;
			entry.SetParam(21, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param22
		if (access.getColumn_Text(40) != NULL)
		{
			string   st_temp22(const_cast<char*>(access.getColumn_Text(40)));
			str =st_temp22;
			entry.SetParam(22, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param23
		if (access.getColumn_Text(41) != NULL)
		{
			string   st_temp23(const_cast<char*>(access.getColumn_Text(41)));
			str = st_temp23;
			entry.SetParam(23, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param24
		if (access.getColumn_Text(42) != NULL)
		{
			string   st_temp24(const_cast<char*>(access.getColumn_Text(42)));
			str = st_temp24;
			entry.SetParam(24, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param25
		if (access.getColumn_Text(43) != NULL)
		{
			string   st_temp25(const_cast<char*>(access.getColumn_Text(43)));
			str = st_temp25;
			entry.SetParam(25, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param26
		if (access.getColumn_Text(44) != NULL)
		{
			string   st_temp26(const_cast<char*>(access.getColumn_Text(44)));
			str = st_temp26;
			entry.SetParam(26, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param27
		if (access.getColumn_Text(45) != NULL)
		{
			string   st_temp27(const_cast<char*>(access.getColumn_Text(45)));
			str = st_temp27;
			entry.SetParam(27, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param28
		if (access.getColumn_Text(46) != NULL)
		{
			string   st_temp28(const_cast<char*>(access.getColumn_Text(46)));
			str = st_temp28;;
			entry.SetParam(28, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param29
		if (access.getColumn_Text(47) != NULL)
		{
			string   st_temp29(const_cast<char*>(access.getColumn_Text(47)));
			str = st_temp29;
			entry.SetParam(29, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param30
		if (access.getColumn_Text(48) != NULL)
		{
			string   st_temp30(const_cast<char*>(access.getColumn_Text(48)));
			str = st_temp30;
			entry.SetParam(30, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//remark
		if (access.getColumn_Text(8) != NULL)
		{
			string   st_remarkp(const_cast<char*>(access.getColumn_Text(8)));
			str = st_remarkp;
			entry.SetDescription(UtilString::ConvertMultiByteToWideChar(str));
		}
		entry.m_bNeedSave = false;
		m_LogicPtManager.AddEntry(entry);
	}

	access.SqlFinalize();

	return strERR.GetString();
}

void CProjectSqlite::SaveProjectSqlite_LogicPoint()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const vector< CLogicDataPointEntry>& vecData = m_LogicPtManager.GetPointList();
	size_t tsize = vecData.size();

	access.BeginTransaction();

	for (size_t i =0; i<tsize; i++ )
	{		
		if (vecData[i].m_bNeedSave)
		{
			string strName;
			string strLogicType;
			string strParam1;
			string strParam2;
			string strParam3;
			string strParam4;
			string strParam5;
			string strParam6;
			string strParam7;
			string strParam8;
			string strParam9;
			string strParam10;
			string strParam11;
			string strParam12;
			string strParam13;
			string strParam14;
			string strParam15;
			string strParam16;
			string strParam17;
			string strParam18;
			string strParam19;
			string strParam20;
			string strParam21;
			string strParam22;
			string strParam23;
			string strParam24;
			string strParam25;
			string strParam26;
			string strParam27;
			string strParam28;
			string strParam29;
			string strParam30;
			string strDescription;
			const CLogicDataPointEntry& info = vecData[i];
			//name
			strName = UtilString::ConvertWideCharToMultiByte(info.GetNameID());
			// logic type
			strLogicType = UtilString::ConvertWideCharToMultiByte(info.GetLogicType());
			//param1 点位值
			strParam1 = UtilString::ConvertWideCharToMultiByte(info.GetParam(1));
			//param2 点位值
			strParam2 = UtilString::ConvertWideCharToMultiByte(info.GetParam(2));
			//param3 点位值
			strParam3 = UtilString::ConvertWideCharToMultiByte(info.GetParam(3));
			//param4 点位值
			strParam4 = UtilString::ConvertWideCharToMultiByte(info.GetParam(4));
			//param5 点位值
			strParam5 = UtilString::ConvertWideCharToMultiByte(info.GetParam(5));
			//param6 点位值
			strParam6 = UtilString::ConvertWideCharToMultiByte(info.GetParam(6));
			//param7 点位值
			strParam7 = UtilString::ConvertWideCharToMultiByte(info.GetParam(7));
			//param8 点位值
			strParam8 = UtilString::ConvertWideCharToMultiByte(info.GetParam(8));
			//param9 点位值
			strParam9 = UtilString::ConvertWideCharToMultiByte(info.GetParam(9));
			//param10 点位值
			strParam10 = UtilString::ConvertWideCharToMultiByte(info.GetParam(10));
			//param11 点位值
			strParam11 = UtilString::ConvertWideCharToMultiByte(info.GetParam(11));
			//param12 点位值
			strParam12 = UtilString::ConvertWideCharToMultiByte(info.GetParam(12));
			//param13 点位值
			strParam13 = UtilString::ConvertWideCharToMultiByte(info.GetParam(13));
			//param14 点位值
			strParam14 = UtilString::ConvertWideCharToMultiByte(info.GetParam(14));
			//param15 点位值
			strParam15 = UtilString::ConvertWideCharToMultiByte(info.GetParam(15));
			//param16 点位值
			strParam16 = UtilString::ConvertWideCharToMultiByte(info.GetParam(16));
			//param17 点位值
			strParam17 = UtilString::ConvertWideCharToMultiByte(info.GetParam(17));
			//param18 点位值
			strParam18 = UtilString::ConvertWideCharToMultiByte(info.GetParam(18));
			//param19 点位值
			strParam19 = UtilString::ConvertWideCharToMultiByte(info.GetParam(19));
			//param20 点位值
			strParam20 = UtilString::ConvertWideCharToMultiByte(info.GetParam(20));
			//param21 点位值
			strParam21 = UtilString::ConvertWideCharToMultiByte(info.GetParam(21));
			//param22 点位值
			strParam22 = UtilString::ConvertWideCharToMultiByte(info.GetParam(22));
			//param23 点位值
			strParam23 = UtilString::ConvertWideCharToMultiByte(info.GetParam(23));
			//param24 点位值
			strParam24 = UtilString::ConvertWideCharToMultiByte(info.GetParam(24));
			//param25 点位值
			strParam25 = UtilString::ConvertWideCharToMultiByte(info.GetParam(25));
			//param26 点位值
			strParam26 = UtilString::ConvertWideCharToMultiByte(info.GetParam(26));
			//param27 点位值
			strParam27 = UtilString::ConvertWideCharToMultiByte(info.GetParam(27));
			//param28 点位值
			strParam28 = UtilString::ConvertWideCharToMultiByte(info.GetParam(28));
			//param29 点位值
			strParam29 = UtilString::ConvertWideCharToMultiByte(info.GetParam(29));
			//param30 点位值
			strParam30 = UtilString::ConvertWideCharToMultiByte(info.GetParam(30));
			//remark
			strDescription = UtilString::ConvertWideCharToMultiByte(info.GetDescription());
			access.DeleteOnePoint(strName.c_str());
			if(access.InsertRecordToLogicPoint(1,strName.c_str(),strLogicType.c_str(),strParam1.c_str(),strParam2.c_str(),
				strParam3.c_str(),strParam4.c_str(),strParam5.c_str(),strParam6.c_str(),strParam7.c_str(),strParam8.c_str(),strParam9.c_str(),
				strParam10.c_str(),strParam11.c_str(),strParam12.c_str(),strParam13.c_str(),strParam14.c_str(),strParam15.c_str(),strParam16.c_str(),
				strParam17.c_str(),strParam18.c_str(),strParam19.c_str(),strParam20.c_str(),strParam21.c_str(),strParam22.c_str(),strParam23.c_str(),
				strParam24.c_str(),strParam25.c_str(),strParam26.c_str(),strParam27.c_str(),strParam28.c_str(),strParam29.c_str(),strParam30.c_str(),
				strDescription.c_str())!=0)
			{
				OutputSaveErr(L"InsertRecordToLogicPoint",access.GetSqliteErr());
			}
		}
	}

	access.CommitTransaction();
}


void CProjectSqlite::LoadProjectSqlite_AlarmPoint()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	int tempInt = 0;
	CAlarmDataPointEntry entry;
	CString cstr;
	const char* pchar = NULL;
	wstring wstr = _T("");
	ALARMTYPE alarmtype = ALARMTYPE_None;
	int i = 1;

	ostringstream sqlstream;
	sqlstream << "select * from list_point where list_point.type = '2';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		entry.Clear();  //reset entry

		if (access.getColumn_Text(1) != NULL)
		{
			string idname(access.getColumn_Text(1));
			str = idname;
			entry.SetNameID(UtilString::ConvertMultiByteToWideChar(str));
		}
		// alarm type
		if (access.getColumn_Text(49) != NULL)
		{
			string logic_type(access.getColumn_Text(49));
			str = logic_type;
			wstr = UtilString::ConvertMultiByteToWideChar(str);
		}
		
		if(wstr == _T("Higher"))
		{
			alarmtype = Higher;
		}
		else if(wstr == _T("Express"))
		{
			alarmtype = Express;
		}
		entry.SetAlarmType(alarmtype);
		
		//param1
		if (access.getColumn_Text(14) != NULL)
		{
			string   st_temp1(const_cast<char*>(access.getColumn_Text(14)));
			str =st_temp1;
			entry.SetParam(1, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param2
		if (access.getColumn_Text(15) != NULL)
		{
			string   st_temp2(const_cast<char*>(access.getColumn_Text(15)));
			str =st_temp2;
			entry.SetParam(2, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param3
		if (access.getColumn_Text(16) != NULL)
		{
			string   st_temp3(const_cast<char*>(access.getColumn_Text(16)));
			str =st_temp3;
			entry.SetParam(3, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param4
		if (access.getColumn_Text(17) != NULL)
		{
			string   st_temp4(const_cast<char*>(access.getColumn_Text(17)));
			str =st_temp4;
			entry.SetParam(4, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param5
		if (access.getColumn_Text(18) != NULL)
		{
			string   st_temp5(const_cast<char*>(access.getColumn_Text(18)));
			str =st_temp5;
			entry.SetParam(5, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param6
		if (access.getColumn_Text(19) != NULL)
		{
			string   st_temp6(const_cast<char*>(access.getColumn_Text(19)));
			str =st_temp6;
			entry.SetParam(6, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param7
		if (access.getColumn_Text(20) != NULL)
		{
			string   st_temp7(const_cast<char*>(access.getColumn_Text(20)));
			str =st_temp7;
			entry.SetParam(7, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param8
		if (access.getColumn_Text(21) != NULL)
		{
			string   st_temp8(const_cast<char*>(access.getColumn_Text(21)));
			str =st_temp8;
			entry.SetParam(8, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param9
		if (access.getColumn_Text(22) != NULL)
		{
			string   st_temp9(const_cast<char*>(access.getColumn_Text(22)));
			str =st_temp9;
			entry.SetParam(9, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param10
		if (access.getColumn_Text(23) != NULL)
		{
			string   st_temp10(const_cast<char*>(access.getColumn_Text(23)));
			str =st_temp10;
			entry.SetParam(10, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param11
		if (access.getColumn_Text(29) != NULL)
		{
			string   st_temp11(const_cast<char*>(access.getColumn_Text(29)));
			str =st_temp11;
			entry.SetParam(11, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param12
		if (access.getColumn_Text(30) != NULL)
		{
			string   st_temp12(const_cast<char*>(access.getColumn_Text(30)));
			str =st_temp12;
			entry.SetParam(12, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param13
		if (access.getColumn_Text(31) != NULL)
		{
			string   st_temp13(const_cast<char*>(access.getColumn_Text(31)));
			str =st_temp13;
			entry.SetParam(13, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//param14
		if (access.getColumn_Text(32) != NULL)
		{
			string   st_temp14(const_cast<char*>(access.getColumn_Text(32)));
			str =st_temp14;
			entry.SetParam(14, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param15
		if (access.getColumn_Text(33) != NULL)
		{
			string   st_temp15(const_cast<char*>(access.getColumn_Text(33)));
			str =st_temp15;
			entry.SetParam(15, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param16
		if (access.getColumn_Text(34) != NULL)
		{
			string   st_temp16(const_cast<char*>(access.getColumn_Text(34)));
			str =st_temp16;
			entry.SetParam(16, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param17
		if (access.getColumn_Text(35) != NULL)
		{
			string   st_temp17(const_cast<char*>(access.getColumn_Text(35)));
			str =st_temp17;
			entry.SetParam(17, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param18
		if (access.getColumn_Text(36) != NULL)
		{
			string   st_temp18(const_cast<char*>(access.getColumn_Text(36)));
			str =st_temp18;
			entry.SetParam(18, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param19
		if (access.getColumn_Text(37) != NULL)
		{
			string   st_temp19(const_cast<char*>(access.getColumn_Text(37)));
			str =st_temp19;
			entry.SetParam(19, UtilString::ConvertMultiByteToWideChar(str)) ;
		}


		//param20
		if (access.getColumn_Text(38) != NULL)
		{
			string   st_temp20(const_cast<char*>(access.getColumn_Text(38)));
			str =st_temp20;
			entry.SetParam(20, UtilString::ConvertMultiByteToWideChar(str)) ;
		}

		//remark
		//str = pElem->Attribute(c_str_xml_remark.c_str());
		if (access.getColumn_Text(8) != NULL)
		{
			string   st_remarkp(const_cast<char*>(access.getColumn_Text(8)));
			str = st_remarkp;
			entry.SetDescription(UtilString::ConvertMultiByteToWideChar(str));
		}
		entry.m_bNeedSave = false;
		m_AlarmPtManager.AddEntry(entry);
	}

	access.SqlFinalize();

}

void CProjectSqlite::SaveProjectSqlite_AlarmPoint()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const vector< CAlarmDataPointEntry>& vecData = m_AlarmPtManager.GetPointList();
	size_t tsize = vecData.size();
	CString cstr;
	ALARMTYPE alarmtype = ALARMTYPE_None;
	access.BeginTransaction();
	for (size_t i =0; i<tsize; i++ )
	{		
		if (vecData[i].m_bNeedSave)
		{
			string strName;
			string strAlarmType;
			string strParam1;
			string strParam2;
			string strParam3;
			string strParam4;
			string strParam5;
			string strParam6;
			string strParam7;
			string strParam8;
			string strParam9;
			string strParam10;
			string strParam11;
			string strParam12;
			string strParam13;
			string strParam14;
			string strParam15;
			string strParam16;
			string strParam17;
			string strParam18;
			string strParam19;
			string strParam20;
			string strDescription;
			const CAlarmDataPointEntry& info = vecData[i];
			//name
			strName = UtilString::ConvertWideCharToMultiByte(info.GetNameID());
			//alarm type
			alarmtype = info.GetAlarmType();
			switch(alarmtype)
			{
			case ALARMTYPE_None:
				wstr = _T("None");
				break;
			case Higher:
				wstr = _T("Higher");
				break;
			case Express:
				wstr = _T("Express");
				break;
			default:
				break;
			}
			strAlarmType = UtilString::ConvertWideCharToMultiByte(wstr);
			//param1 点位值
			strParam1 = UtilString::ConvertWideCharToMultiByte(info.GetParam(1));
			//param2 点位值
			strParam2 = UtilString::ConvertWideCharToMultiByte(info.GetParam(2));
			//param3 点位值
			strParam3 = UtilString::ConvertWideCharToMultiByte(info.GetParam(3));
			//param4 点位值
			strParam4 = UtilString::ConvertWideCharToMultiByte(info.GetParam(4));
			//param5 点位值
			strParam5 = UtilString::ConvertWideCharToMultiByte(info.GetParam(5));
			//param6 点位值
			strParam6 = UtilString::ConvertWideCharToMultiByte(info.GetParam(6));
			//param7 点位值
			strParam7 = UtilString::ConvertWideCharToMultiByte(info.GetParam(7));
			//param8 点位值
			strParam8 = UtilString::ConvertWideCharToMultiByte(info.GetParam(8));
			//param9 点位值
			strParam9 = UtilString::ConvertWideCharToMultiByte(info.GetParam(9));
			//param10 点位值
			strParam10 = UtilString::ConvertWideCharToMultiByte(info.GetParam(10));
			//param11 点位值
			strParam11 = UtilString::ConvertWideCharToMultiByte(info.GetParam(11));
			//param12 点位值
			strParam12 = UtilString::ConvertWideCharToMultiByte(info.GetParam(12));
			//param13 点位值
			strParam13 = UtilString::ConvertWideCharToMultiByte(info.GetParam(13));
			//param14 点位值
			strParam14 = UtilString::ConvertWideCharToMultiByte(info.GetParam(14));
			//param15 点位值
			strParam15 = UtilString::ConvertWideCharToMultiByte(info.GetParam(15));
			//param16 点位值
			strParam16 = UtilString::ConvertWideCharToMultiByte(info.GetParam(16));
			//param17 点位值
			strParam17 = UtilString::ConvertWideCharToMultiByte(info.GetParam(17));
			//param18 点位值
			strParam18 = UtilString::ConvertWideCharToMultiByte(info.GetParam(18));
			//param19 点位值
			strParam19 = UtilString::ConvertWideCharToMultiByte(info.GetParam(19));
			//param20 点位值
			strParam20 = UtilString::ConvertWideCharToMultiByte(info.GetParam(20));
			//remark
			strDescription = UtilString::ConvertWideCharToMultiByte(info.GetDescription());
			access.DeleteOnePoint(strName.c_str());
			if(access.InsertRecordToAlarmPoint(2,strName.c_str(),strAlarmType.c_str(),strParam1.c_str(),strParam2.c_str(),
				strParam3.c_str(),strParam4.c_str(),strParam5.c_str(),strParam6.c_str(),strParam7.c_str(),strParam8.c_str(),strParam9.c_str(),
				strParam10.c_str(),strParam11.c_str(),strParam12.c_str(),strParam13.c_str(),strParam14.c_str(),strParam15.c_str(),strParam16.c_str(),
				strParam17.c_str(),strParam18.c_str(),strParam19.c_str(),strParam20.c_str(),strDescription.c_str())!=0)
			{
				OutputSaveErr(L"InsertRecordToAlarmPoint",access.GetSqliteErr());
			}
		}
	}
	access.CommitTransaction();
}

bool CProjectSqlite::IsFisrtDigital( wstring strID )
{
	if (strID.size()>0)
	{
		return iswdigit(strID.at(0));
	}
	return false;
}

bool CProjectSqlite::CheckEntryValid(const DataPointEntry& entry, CString& strErr)
{
	CString strType = entry.GetSourceType().c_str();
	CString strName = entry.GetShortName().c_str();
	CString strInfo;
	if (strType.GetLength() == 0)
	{
		strInfo.Format(L"%s的类型为空",strName);
		strErr = strInfo;
		return false;
	}
	if (strType.CompareNoCase(L"bacnet") == 0)
	{
		CString strParam1 = entry.GetParam(1).c_str();
		if (strParam1.GetLength() > 0)
		{
			if (strParam1.FindOneOf(L"abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ ,;")>=0)
			{
				strInfo.Format(L"%s的参数1非法",strName);
				strErr = strInfo;
				return false;
			}
		}
		CString strParam2 = entry.GetParam(2).c_str();
		if (strParam2.GetLength() > 0)
		{
			if (!(strParam2 == L"AI" || strParam2 == L"AO" || strParam2 == L"AV"
				|| strParam2 == L"BI" || strParam2 == L"BO" || strParam2 == L"BV"
				|| strParam2 == L"MI" || strParam2 == L"MO" || strParam2 == L"MV"))
			{
				strInfo.Format(L"%s的参数2非法",strName);
				strErr = strInfo;
				return false;
			}
		}
		CString strParam3 = entry.GetParam(3).c_str();
		if (strParam3.GetLength() > 0)
		{
			if (strParam3.FindOneOf(L"abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ ,;")>=0)
			{
				strInfo.Format(L"%s的参数3非法",strName);
				strErr = strInfo;
				return false;
			}
		}
	}
	else if (strType.CompareNoCase(L"modbus") == 0)
	{
		CString strParam1 = entry.GetParam(1).c_str();
		if (strParam1.GetLength() > 0)
		{
			if (strParam1.FindOneOf(L"abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ ,;")>=0)
			{
				strInfo.Format(L"%s的参数1非法",strName);
				strErr = strInfo;
				return false;
			}
		}
		CString strParam2 = entry.GetParam(2).c_str();
		if (strParam2.GetLength() > 0)
		{
			if (strParam2.FindOneOf(L"abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ ,;")>=0)
			{
				strInfo.Format(L"%s的参数2非法",strName);
				strErr = strInfo;
				return false;
			}
		}
		CString strParam3 = entry.GetParam(3).c_str();
		if (strParam3.GetLength() > 0)
		{
			if (strParam3.FindOneOf(L"abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ ,;")>=0)
			{
				strInfo.Format(L"%s的参数3非法",strName);
				strErr = strInfo;
				return false;
			}
		}
		CString strParam4 = entry.GetParam(4).c_str();
		if (strParam4.GetLength() > 0)
		{
			if (strParam4.FindOneOf(L"abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ ,;")>=0)
			{
				strInfo.Format(L"%s的参数4非法",strName);
				strErr = strInfo;
				return false;
			}
		}
		CString strParam5 = entry.GetParam(5).c_str();
		if (strParam5.GetLength() > 0)
		{
			if (strParam5.FindOneOf(L"abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ ,;")>=0)
			{
				strInfo.Format(L"%s的参数5非法",strName);
				strErr = strInfo;
				return false;
			}
		}
	}
	else if (strType.CompareNoCase(L"vpoint") == 0)
	{
		//CString strParam1 = entry.GetParam(1).c_str();
		//if (strParam1.GetLength() > 0)
		//{
		//	if (!(strParam1.CompareNoCase(L"RelationOP")==0||strParam1.CompareNoCase(L"LogicAnd")==0||strParam1.CompareNoCase(L"LogicOr")==0||strParam1.CompareNoCase(L"LogicXor")==0))
		//	{
		//		strInfo.Format(L"%s的参数1非法",strName);
		//		AfxMessageBox(strInfo);
		//		return;
		//	}
		//}
	}
	else
	{
		//CString strParam1 = entry.GetParam(1).c_str();
		//if (strParam1.GetLength() > 0)
		//{
		//	int pos = 0;
		//	pos = strType.Find(L"simense");
		//	if (pos != -1)
		//	{
		//		pos = strParam1.Find(L":[main");
		//		if (pos == -1)
		//		{
		//			strInfo.Format(L"%s的参数1非法",strName);
		//			AfxMessageBox(strInfo);
		//			return;
		//		}
		//	}
		//}
	}
	return true;
}

void CProjectSqlite::LoadPictureLibSqlite(const bool bDbSelect)
{
	wstring strFilePathName;
	if (bDbSelect)
	{
		strFilePathName = m_strFilePath;
		FreePictureLibMem();
	}
	else
	{
		strFilePathName = m_strImgDbPath;
		FreePictureLibImageMem();
	}

	bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
	CSqliteAcess access(strFilePathName.c_str(), bEncry);
	ostringstream sqlstream;
	sqlstream << "select * from lib_image;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	
	if (bDbSelect)
	{
		ClearDestPictureId();
	}
	
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		PictureInfo info;
		//ID
		info.ID = access.getColumn_Int(0);
		//name
		const char* lpszName = access.getColumn_Text(1);
		assert(lpszName);
		wstring wstrName = UtilString::ConvertMultiByteToWideChar(lpszName);

		wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t),wstrName.c_str(),sizeof(info.szName)/sizeof(wchar_t)-1);

		///
		if (access.getColumn_Text(2) != NULL)
		{
			string   equipType(const_cast<char*>(access.getColumn_Text(2)));
			info.etype_equip = UtilString::ConvertMultiByteToWideChar(equipType);
		}

		///
		const void* pwchar = access.getColumn_Blob(3);
		if (access.getColumn_Blob(3) != NULL)
		{
			int  size_p  = access.getColumn_Bytes(3);
			info.nSize = size_p;
			info.pBlock = new char[size_p];
			if (info.pBlock !=NULL)
			{
				memcpy_s(info.pBlock,size_p,pwchar,size_p);
			}
		}

		///
		if (access.getColumn_Text(4) !=NULL)
		{
			string filetype(const_cast<char*>(access.getColumn_Text(4)));
			info.etype_p = UtilString::ConvertMultiByteToWideChar(filetype);
		}

		/// set name with name + ex
		//wstring nameEx(wstrName + L"." + info.etype_p);
		//wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t), nameEx.c_str(), sizeof(info.szName)/sizeof(wchar_t)-1);

		////
		if (access.getColumn_Text(5) !=NULL)
		{
			string remark(const_cast<char*>(access.getColumn_Text(5)));
			info.remark = UtilString::ConvertMultiByteToWideChar(remark);
		}

		///
		int issystemimage = access.getColumn_Int(6);
		info.issystemimage = issystemimage;

		/// button id
		const char* pBtnId = access.getColumn_Text(7);
		if (NULL != pBtnId)
		{
			info.nBtnId = atoi(pBtnId);
		}

		/// button sub id
		const char* pBtnSubId = access.getColumn_Text(8);
		if (NULL != pBtnSubId)
		{
			info.nBtnSubId = atoi(pBtnSubId);
		}

		if(info.pBlock && info.nSize>0)
		{
			CMemFile   memfile;  //把文件放到RAM中
			memfile.Attach((BYTE*)(info.pBlock),info.nSize,1024);  
			memfile.SetLength(info.nSize); 
			HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, info.nSize);
			if (hMemBmp)
			{
				IStream* pStmBmp = NULL;
				if(CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
				{
					BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
					memfile.SeekToBegin();
					memfile.Read(pbyBmp, info.nSize);
					Image* pBitmap = Image::FromStream(pStmBmp);
					info.pBitmap = pBitmap;
					GlobalUnlock(hMemBmp);
				}
				//GlobalFree(hMemBmp);	// Marked for jpg show
				hMemBmp = NULL;
			}
		}
		if (bDbSelect)
		{
			m_vPictureList.push_back(info);

			AddIntoDestPictureId(info.ID);
		}
		else
		{
			m_vPictureListImage.push_back(info);
		}
	}
	access.SqlFinalize();
}

void CProjectSqlite::LoadAnimationSqlite(const bool bDbSelect)
{
	wstring strFilePathName;
	if (bDbSelect)
	{
		strFilePathName = m_strFilePath;
		FreeAnimationMem();
	}
	else
	{
		strFilePathName = m_strImgDbPath;
		FreeAnimationImageMem();
	}

	bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
	CSqliteAcess access(strFilePathName.c_str(), bEncry);
	ostringstream sqlstream;
	sqlstream << "select * from lib_animation;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	if (bDbSelect)
	{
		ClearDestAnimationId();
	}

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		AnimationInfo info;
		//ID
		info.ID = access.getColumn_Int(0);
		
		//name
		const char* lpszName = access.getColumn_Text(1);
		if (lpszName !=NULL)
		{
			info.szName = UtilString::ConvertMultiByteToWideChar(lpszName);
		}


		///
		if (access.getColumn_Text(3) !=NULL)
		{
			string   equipType(const_cast<char*>(access.getColumn_Text(3)));
			char *sep =","; 
			UtilString::SplitString(equipType,sep,info.vecPicIDList);
			if (bDbSelect)
			{
				m_vAnimationList.push_back(info);

				AddIntoDestAnimationId(info.ID);
			}
			else
			{
				m_vAnimationListImage.push_back(info);
			}			
		}
	}
	access.SqlFinalize();
//////////////////////////////////////////////////////////////////////////
	//获取每个动画的图片集
	std::vector<int> vAllIDList,vAllIDList_temp;
	if (bDbSelect)
	{
		for (int i=0;i<m_vAnimationList.size();++i)
		{
			for (int j = 0;j<m_vAnimationList[i].vecPicIDList.size();++j)
			{
				vAllIDList_temp.push_back(m_vAnimationList[i].vecPicIDList[j]);
			}
		}
	}
	else
	{
		for (int i=0;i<m_vAnimationListImage.size();++i)
		{
			for (int j = 0;j<m_vAnimationListImage[i].vecPicIDList.size();++j)
			{
				vAllIDList_temp.push_back(m_vAnimationListImage[i].vecPicIDList[j]);
			}
		}
	}
	int n = vAllIDList_temp.size();
	for (int i=1;i<n;i++)
	{
		int j=i-1;
		int temp=vAllIDList_temp[i];
		while (j>=0 && vAllIDList_temp[j]>temp)
		{
			vAllIDList_temp[j+1]=vAllIDList_temp[j];
			j--;
		}
		vAllIDList_temp[j+1]=temp;
	}
	if (vAllIDList_temp.empty())
	{
		return;
	}
	vAllIDList.clear();
	vAllIDList.push_back(vAllIDList_temp[0]);
	for (int i=1;i<vAllIDList_temp.size();++i)
	{
		if (vAllIDList_temp[i-1] != vAllIDList_temp[i])
		{
			vAllIDList.push_back(vAllIDList_temp[i]);
		}
	}
//////////////////////////////////////////////////////////////////////////
	//
	ostringstream sqlstream_ani;
	sqlstream_ani << "select * from lib_image_animation where id in(";
	for (int i=0;i<vAllIDList.size()-1;++i)
	{
		sqlstream_ani << vAllIDList[i] << ",";
	}
	sqlstream_ani << vAllIDList.back() << ")";
	//sqlstream_ani << " order by ID";
	sql_ = sqlstream_ani.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	int re_ = access.SqlQuery(ex_sql);
	if (re_ != 0)
	{
		access.SqlFinalize();
		return;
	}

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		PictureInfo info;
		//ID
		info.ID = access.getColumn_Int(0);
		//name
		const char* lpszName = access.getColumn_Text(1);
		assert(lpszName);
		wstring wstrName = UtilString::ConvertMultiByteToWideChar(lpszName);
		wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t),wstrName.c_str(),sizeof(info.szName)/sizeof(wchar_t)-1);
		///
		if (access.getColumn_Text(2) !=NULL)
		{
			string   equipType(const_cast<char*>(access.getColumn_Text(2)));
			info.etype_equip = UtilString::ConvertMultiByteToWideChar(equipType);
		}


		///
		const void* pwchar = access.getColumn_Blob(3);
		if (pwchar !=NULL)
		{
			int  size_p  = access.getColumn_Bytes(3);
			info.nSize = size_p;
			info.pBlock = new char[size_p];
			if (info.pBlock !=NULL)
			{
				memcpy_s(info.pBlock,size_p,pwchar,size_p);
			}
		}

		///
		if (access.getColumn_Text(4) !=NULL)
		{
			string filetype(const_cast<char*>(access.getColumn_Text(4)));
			info.etype_p = UtilString::ConvertMultiByteToWideChar(filetype);
		}

		////
		if (access.getColumn_Text(5) !=NULL)
		{
			string remark(const_cast<char*>(access.getColumn_Text(5)));
			info.remark = UtilString::ConvertMultiByteToWideChar(remark);
		}

		///
		int issystemimage = access.getColumn_Int(6);
		info.issystemimage = issystemimage;

		if(info.pBlock && info.nSize>0)
		{
			CMemFile   memfile;  
			memfile.Attach((BYTE*)(info.pBlock),info.nSize,1024);  
			memfile.SetLength(info.nSize); 
			HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, info.nSize);
			if (hMemBmp)
			{
				IStream* pStmBmp = NULL;
				if(CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
				{
					BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
					memfile.SeekToBegin();
					memfile.Read(pbyBmp, info.nSize);
					Image* pBitmap = Image::FromStream(pStmBmp);
					info.pBitmap = pBitmap;
					GlobalUnlock(hMemBmp);
				}
				GlobalFree(hMemBmp);
				hMemBmp = NULL;
			}
		}

		if (bDbSelect)
		{
			m_vPictureforAnimationList.push_back(info);
		}
		else
		{
			m_vPictureforAnimationListImage.push_back(info);
		}
	}

	access.SqlFinalize();
//////////////////////////////////////////////////////////////////////////
	if (bDbSelect)
	{
		for (int i=0;i<m_vAnimationList.size();++i)
		{
			m_vAnimationList[i].vecPicture.clear();
			for (int j=0;j<m_vAnimationList[i].vecPicIDList.size();++j)
			{
			
				for (int k =0;k<m_vPictureforAnimationList.size();++k)
				{
					if (m_vAnimationList[i].vecPicIDList[j] == m_vPictureforAnimationList[k].ID)
					{
						m_vAnimationList[i].vecPicture.push_back(m_vPictureforAnimationList[k]);
					}
				}
			}
		}
	}
	else
	{
		for (int i=0;i<m_vAnimationListImage.size();++i)
		{
			m_vAnimationListImage[i].vecPicture.clear();
			for (int j=0;j<m_vAnimationListImage[i].vecPicIDList.size();++j)
			{
			
				for (int k =0;k<m_vPictureforAnimationListImage.size();++k)
				{
					if (m_vAnimationListImage[i].vecPicIDList[j] == m_vPictureforAnimationListImage[k].ID)
					{
						m_vAnimationListImage[i].vecPicture.push_back(m_vPictureforAnimationListImage[k]);
					}
				}
			}
		}
	}

	return;
}
void CProjectSqlite::FreePictureLibMem()
{
	if (m_vPictureList.empty())
	{
		return;
	}

	for (int i=0;i<m_vPictureList.size();++i)
	{
		if (m_vPictureList[i].pBlock !=NULL)
		{
			delete m_vPictureList[i].pBlock;
			m_vPictureList[i].pBlock =NULL;
		}
		if (m_vPictureList[i].pBitmap !=NULL)
		{
			delete m_vPictureList[i].pBitmap;
			m_vPictureList[i].pBitmap = NULL;
		}
	}

	m_vPictureList.clear();
	return;
}

void CProjectSqlite::FreeAnimationMem()
{
	if (m_vPictureforAnimationList.empty())
	{
		return;
	}

	for (int i=0;i<m_vPictureforAnimationList.size();++i)
	{
		if (m_vPictureforAnimationList[i].pBlock !=NULL)
		{
			delete m_vPictureforAnimationList[i].pBlock;
			m_vPictureforAnimationList[i].pBlock =NULL;
		}
		//if (m_vPictureforAnimationList[i].pBitmap !=NULL)
		//{
		//	delete m_vPictureforAnimationList[i].pBitmap;
		//	m_vPictureforAnimationList[i].pBitmap =NULL;
		//}
	}
	m_vPictureforAnimationList.clear();

	if (m_vAnimationList.empty())
	{
		return;
	}
	for (int i=0;i<m_vAnimationList.size();++i)
	{
		m_vAnimationList[i].vecPicture.clear();
	}
	m_vAnimationList.clear();

	return;
}

void CProjectSqlite::FreePictureLibImageMem(void)
{
	if (m_vPictureListImage.empty())
	{
		return;
	}

	for (int i=0;i<m_vPictureListImage.size();++i)
	{
		if (m_vPictureListImage[i].pBlock !=NULL)
		{
			delete m_vPictureListImage[i].pBlock;
			m_vPictureListImage[i].pBlock =NULL;
		}
	}

	m_vPictureListImage.clear();
}

void CProjectSqlite::FreeAnimationImageMem(void)
{
	if (m_vPictureforAnimationListImage.empty())
	{
		return;
	}

	for (int i=0;i<m_vPictureforAnimationListImage.size();++i)
	{
		if (m_vPictureforAnimationListImage[i].pBlock !=NULL)
		{
			delete m_vPictureforAnimationListImage[i].pBlock;
			m_vPictureforAnimationListImage[i].pBlock =NULL;
		}
	}
	m_vPictureforAnimationListImage.clear();


	if (m_vAnimationListImage.empty())
	{
		return;
	}
	for (int i=0;i<m_vAnimationListImage.size();++i)
	{
		m_vAnimationListImage[i].vecPicture.clear();
	}
	m_vAnimationListImage.clear();
}

int CProjectSqlite::DeleteAllUsefulTables()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	return access.DeleteAllUsefulTables();
}

int CProjectSqlite::DeleteAllTables()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	return access.DeleteAllTables();
}

void CProjectSqlite::SetFilePath( const wchar_t* lpszFilePath )
{
	assert(lpszFilePath);

	m_strFilePath = lpszFilePath;
	m_strFileDecPath = m_strFilePath + L".dec";
}

Image* CProjectSqlite::GetBitmapByIDFromPictureLib(const int nID, const bool bDbSelect)
{
	if (bDbSelect)
	{
		const size_t size = m_vPictureList.size();
		for (size_t i=0;i<size;++i)
		{
			if (nID == m_vPictureList[i].ID)
			{
				return m_vPictureList[i].pBitmap;
			}
		}
	}
	else
	{
		const size_t size = m_vPictureListImage.size();
		for (size_t i=0;i<size;++i)
		{
			if (nID == m_vPictureListImage[i].ID)
			{
				return m_vPictureListImage[i].pBitmap;
			}
		}
	}
	return NULL;
}

int CProjectSqlite::GetPictureIDByName( const wchar_t* lpszName )
{
	assert(lpszName);

	const size_t size = m_vPictureList.size();
	for (size_t i=0;i<size;++i)
	{
		if (wcsicmp(lpszName, m_vPictureList[i].szName) == 0)
		{
			return m_vPictureList[i].ID;
		}
	}
	return -1;
}

BOOL CProjectSqlite::FolderExist(CString strPath)
{
	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind!=INVALID_HANDLE_VALUE) &&
		(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}


BOOL CProjectSqlite::CreateFolder(CString strPath)
{
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	return ::CreateDirectory(strPath, &attrib);
}

BOOL CProjectSqlite::FileExist(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName);
}

const bool  CProjectSqlite::SaveMemBitmapToFile( const char* pBlock, const int nSize, const wchar_t* lpszFileName)
{
	if (pBlock == NULL)
	{
		return false;
	}

	bool bIsAbsPath = false;
	CString strFF = lpszFileName;
	if(strFF.Find(_T("\\"))>=0)
		bIsAbsPath = true;

	CString strFileName;
	if(!bIsAbsPath)
	{
		CString  tempPath;
		Project::Tools::GetSysPath(tempPath);
		tempPath += _T("\\temp");

		const wchar_t* lpszFolderPath =  tempPath.GetString();
		if (!FolderExist(lpszFolderPath))
		{
			CreateFolder(lpszFolderPath);
		}
		
		strFileName.Format(L"%s\\%s.png",lpszFolderPath,lpszFileName);
	}
	else
	{
		strFileName = strFF;
	}

	if(FolderExist(strFileName))
	{
		DeleteFile(strFileName.GetString());
	}

	
	
	FILE* pFile = NULL;
	const errno_t rs = _wfopen_s(&pFile, strFileName.GetString(), L"wb");
	if(pFile)
	{
		fseek(pFile, 0, SEEK_SET);
		fwrite(pBlock, nSize, 1, pFile);
		fclose(pFile);
	}
		 
	
	
	return true;
}

const wchar_t* CProjectSqlite::GetPictureNameByID( const int nID )
{
	assert(nID != -1);

	const size_t size = m_vPictureList.size();
	for (size_t i=0;i<size;++i)
	{
		if (nID == m_vPictureList[i].ID)
		{
			return m_vPictureList[i].szName;
		}
	}
	return NULL;
}

std::vector<CEqmDrawPage*> CProjectSqlite::GetPageIDList(int groupid)
{
	std::vector<CEqmDrawPage*> pagelist;
	for (int i=0;i<m_project.GetPageList().size();++i)
	{
		if (groupid == m_project.GetPageList()[i]->m_GroupID)
		{
			pagelist.push_back(m_project.GetPageList()[i]);
		}
	}

	return pagelist;
}

CEqmDrawPage *      CProjectSqlite::GetCurPage()
{
	return m_pPage;
}

bool CProjectSqlite::SaveGroupPageSqlite()
{
	string	strName;
	char	szShowOrder[MAX_PATH]	= {0};
	int		nRet					= 0;
	int		nCount					= 0;

	bool bSave = true;

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	const size_t size = m_project.m_groupgage.size();
	for (size_t i=0; i<size; ++i)
	{
		strName = UtilString::ConvertWideCharToMultiByte(m_project.m_groupgage[i].GroupName);
		sprintf_s(szShowOrder, MAX_PATH, "%d", m_project.m_groupgage[i].nGroupShowOrder);

		nCount = 0;
		while (true)
		{
			nRet = access.InsertRecordToPageGroup(m_project.m_groupgage[i].GroupID, strName.c_str(), szShowOrder);
			if (SQLITE_OK == nRet)
			{
				break;
			}
			else
			{
				++nCount;
				//m_log.WriteLog(_T("Fail insert into list_pagegroup, return value = %d, i = %d, times = %d"), nRet, i, nCount);

				OutputSaveErr(L"InsertRecordToPageGroup",access.GetSqliteErr());

				if (nCount >= 3)
				{
					break;
				}
				bSave = false;
				Sleep(5000);
			}
		}


		//ostringstream sqlstream;
		//sqlstream << "select * from list_pagegroup where id = " << nGroupId << ";";
		//string sql_ = sqlstream.str();
		//char *ex_sql = const_cast<char*>(sql_.c_str());
		//access.SqlQuery(ex_sql);

		//if (SQLITE_ROW != access.SqlNext())
		//{	// not found
		//	access.InsertRecordToPageGroup(nGroupId, strName.c_str(), szShowOrder);
		//}
		//else
		//{	// found
		//	access.UpdatePageGroup(nGroupId, strName.c_str(), szShowOrder);
		//}
		//access.SqlFinalize();
	}
	return true;
}

int CProjectSqlite::DeleteProjectConfig()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	return access.DeleteProjectConfigData();
}

AnimationInfo * CProjectSqlite::GetAnimationByID(const int nID, const bool bDbSelect)
{
	if (bDbSelect)
	{
		const size_t size = m_vAnimationList.size();
		for (size_t i=0;i<size;++i)
		{
			if (nID == m_vAnimationList[i].ID)
			{
				return &(m_vAnimationList[i]);
			}
		}
	} 
	else
	{
		const size_t size = m_vAnimationListImage.size();
		for (size_t i=0;i<size;++i)
		{
			if (nID == m_vAnimationListImage[i].ID)
			{
				return &(m_vAnimationListImage[i]);
			}
		}
	}

	return NULL;
}

wstring CProjectSqlite::GetFilePath()
{
	return m_strFilePath;
}

int CProjectSqlite::InsertPictureData(const int nPicID, const char* lpszName, const char* lpszType,char* pBlock, int size, const bool bDbSelect, const int nBtnId, const int nBtnSubId, const char* lpszImgType)
{
	wstring strFilePathName = bDbSelect ? m_strFilePath : m_strImgDbPath;
	bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
	CSqliteAcess access(strFilePathName.c_str(), bEncry);
	int nResult = access.InsertRecordIntoLibImage(nPicID, lpszName, lpszType, pBlock, lpszImgType, "自定义的图元", 1, size, nBtnId, nBtnSubId);
	if(nResult!=0)
	{
		OutputSaveErr(L"InsertRecordIntoLibImage",access.GetSqliteErr());
	}
	return nResult;
}

int CProjectSqlite::GetMaxLibImageNum(const bool bDbSelect)
{
	int nMax = 0;
	wstring strFilePathName = bDbSelect ? m_strFilePath : m_strImgDbPath;
	bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
	CSqliteAcess access(strFilePathName.c_str(), bEncry);
	ostringstream sqlstream;
	sqlstream << "select max(ID) from lib_image;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::GetMaxLibImageBtnId(const bool bDbSelect)
{
	int	nMaxBtnNum = 0;
	vector<PictureInfo>::const_iterator	iter;


	if (bDbSelect)
	{
		for (iter=m_vPictureList.begin(); iter!=m_vPictureList.end(); ++iter)
		{
			if (L"7" == iter->etype_equip)
			{
				if (iter->nBtnId > nMaxBtnNum)
				{
					nMaxBtnNum = iter->nBtnId;
				}
			}
		}
	} 
	else
	{
		for (iter=m_vPictureListImage.begin(); iter!=m_vPictureListImage.end(); ++iter)
		{
			if (L"7" == iter->etype_equip)
			{
				if (iter->nBtnId > nMaxBtnNum)
				{
					nMaxBtnNum = iter->nBtnId;
				}
			}
		}
	}

	return nMaxBtnNum;
}

void CProjectSqlite::InsertToLibImage(PictureInfo PicInfo)
{
	m_vPictureListImage.push_back(PicInfo);
}

bool CProjectSqlite::IsPicNameValid( CString strName )
{
	bool bValid = true;
	const size_t size = m_vPictureList.size();
	for (size_t i=0;i<size;++i)
	{
		if (wcsicmp(m_vPictureList[i].szName, strName.GetString()) == 0)
		{
			bValid = false;
			break;
		}
	}
	return bValid;
}

int CProjectSqlite::GetMaxLibAnimationNum(const bool bDbSelect)
{
	int nMax = 0;
	wstring strFilePathName = bDbSelect ? m_strFilePath : m_strImgDbPath;
	bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
	CSqliteAcess access(strFilePathName.c_str(), bEncry);
	ostringstream sqlstream;
	sqlstream << "select max(id) from lib_animation;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::InsertRecordToAnimationList(const int nID, const char* lpszName, const char* lpszAnimationList, const bool bDbSelect)
{
	wstring strFilePathName = bDbSelect ? m_strFilePath : m_strImgDbPath;
	bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
	CSqliteAcess access(strFilePathName.c_str(), bEncry);
	int nResult = access.InsertRecordToLibAnimation(nID,lpszName,lpszAnimationList);
	if(nResult != 0)
	{
		OutputSaveErr(L"InsertRecordToLibAnimation",access.GetSqliteErr());
	}
	return nResult;
}

wstring CProjectSqlite::InsertAnimationVec( const int nID, const CString strName, const CString strSeries )
{
	CString strError;
	AnimationInfo aniInfo;
	aniInfo.ID = nID;
	aniInfo.szName = strName.GetString();
	vector<wstring> vecSeries;
	SplitStringByChar(strSeries, L',', vecSeries);
	size_t size = vecSeries.size();
	for (size_t i=0;i<size;++i)
	{
		const int nPicID = _wtoi(vecSeries[i].c_str());
		PictureInfo* info = GetPictureInfoByIDOfAnimation(nPicID, false);
		if (info)
		{
			aniInfo.vecpBitmap.push_back(info->pBitmap);
			aniInfo.vecPicIDList.push_back(info->ID);
			aniInfo.vecPicture.push_back(*info);
		}
		else
		{
			CString strMsg;
			strMsg.Format(L"动画图片缺失!!!\n页面名称:%s\n图片ID:%d",m_pPage->GetPageName().c_str(), nPicID);
			strError +=strMsg;
			strError +=L"\r\n";
		}
	}
	m_vAnimationListImage.push_back(aniInfo);

	return strError.GetString();
}
int	CProjectSqlite::GetUnusedPictureID(void)
{
	int minID = 0;
	const size_t size = m_vPictureList.size();
	while(minID<2147483647)
	{
		bool bFindId = false;
		for (size_t i=0;i<size;++i)
		{
			if(m_vPictureList[i].ID == minID)
			{
				minID++;
				bFindId = true;
				break;
			}
		}
		if(bFindId == false)
			return minID;
	}

	//负值情况下处理
	minID = 0;
	while(minID > -2147483648)
	{
		bool bFindId = false;
		for (size_t i=0;i<size;++i)
		{
			if(m_vPictureList[i].ID == minID)
			{
				minID--;
				bFindId = true;
				break;
			}
		}
		if(bFindId == false)
			return minID;
	}
}
PictureInfo* CProjectSqlite::GetPictureInfoByID(const int nPicID, const bool bDbSelect)
{
	if (bDbSelect)
	{
		const size_t size = m_vPictureList.size();
		for (size_t i=0;i<size;++i)
		{
			if (m_vPictureList[i].ID == nPicID)
			{
				return &m_vPictureList[i];
			}
		}
	} 
	else
	{
		const size_t size = m_vPictureListImage.size();
		for (size_t i=0;i<size;++i)
		{
			if (m_vPictureListImage[i].ID == nPicID)
			{
				return &m_vPictureListImage[i];
			}
		}
	}
	return NULL;
}

int CProjectSqlite::GetMaxLibImageAnimationNum(const bool bDbSelect)
{
	int nMax = 0;
	wstring strFilePathName = bDbSelect ? m_strFilePath : m_strImgDbPath;
	bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
	CSqliteAcess access(strFilePathName.c_str(), bEncry);
	ostringstream sqlstream;
	sqlstream << "select max(ID) from lib_image_animation;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::InsertPictureAnimationData(const int nPicID, const char* lpszName, const char* lpszType,char* pBlock, int size, const bool bDbSelect)
{
	wstring strFilePathName = bDbSelect ? m_strFilePath : m_strImgDbPath;
	bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
	CSqliteAcess access(strFilePathName.c_str(), bEncry);
	int nResult = access.InsertRecordIntoLibImageAnimation(nPicID, lpszName, lpszType, pBlock, "png", "动画的图元", 1, size);
	if(nResult != 0)
	{
		OutputSaveErr(L"InsertRecordIntoLibImageAnimation",access.GetSqliteErr());
	}
	return nResult;
}

void CProjectSqlite::InsertToLibImageAnimation( PictureInfo PicInfo )
{
	m_vPictureforAnimationListImage.push_back(PicInfo);
}

PictureInfo* CProjectSqlite::GetPictureInfoByIDOfAnimation(const int nPicID, const bool bDbSelect)
{
	if (bDbSelect)
	{
		const size_t size = m_vPictureforAnimationList.size();
		for (size_t i=0;i<size;++i)
		{
			if (m_vPictureforAnimationList[i].ID == nPicID)
			{
				return &m_vPictureforAnimationList[i];
			}
		}
	} 
	else
	{
		const size_t size = m_vPictureforAnimationListImage.size();
		for (size_t i=0;i<size;++i)
		{
			if (m_vPictureforAnimationListImage[i].ID == nPicID)
			{
				return &m_vPictureforAnimationListImage[i];
			}
		}
	}
	return NULL;
}

Image* CProjectSqlite::GetBitmapByIdFromAnimationImageLib(const int nId, const bool bDbSelect)
{
	Image* pRet	= NULL;
	int	nShowAnimaId		= 0;
	vector<AnimationInfo>::const_iterator	iterAni;
	vector<PictureInfo>::const_iterator		iterPic;


	if (bDbSelect)
	{
		for (iterAni=m_vAnimationList.begin(); iterAni!=m_vAnimationList.end(); ++iterAni)
		{
			if (iterAni->ID == nId)
			{
				nShowAnimaId = iterAni->vecPicIDList.at(0);
				break;
			}
		}

		for (iterPic=m_vPictureforAnimationList.begin(); iterPic!=m_vPictureforAnimationList.end(); ++iterPic)
		{
			if (iterPic->ID == nShowAnimaId)
			{
				pRet = iterPic->pBitmap;
				break;
			}
		}
	}
	else
	{
		for (iterAni=m_vAnimationListImage.begin(); iterAni!=m_vAnimationListImage.end(); ++iterAni)
		{
			if (iterAni->ID == nId)
			{
				nShowAnimaId = iterAni->vecPicIDList.at(0);
				break;
			}
		}

		for (iterPic=m_vPictureforAnimationListImage.begin(); iterPic!=m_vPictureforAnimationListImage.end(); ++iterPic)
		{
			if (iterPic->ID == nShowAnimaId)
			{
				pRet = iterPic->pBitmap;
				break;
			}
		}
	}

	return pRet;
}

bool CProjectSqlite::SaveProjectSqlite_dashboard()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawDashBoard*>& dashboard_list = m_pPage->GetDashboardList();
	list<CEqmDrawDashBoard*>::const_iterator it = dashboard_list.begin();
	
	string	strBindPointName;
	string	strMaxStr;
	string	strMinStr;

	access.BeginTransaction();
	for (;it!=dashboard_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			strBindPointName = UtilString::ConvertWideCharToMultiByte((*it)->GetBindPointName().GetString());
			strMaxStr = UtilString::ConvertWideCharToMultiByte((*it)->GetMaxString().GetString());
			strMinStr = UtilString::ConvertWideCharToMultiByte((*it)->GetMinString().GetString());

			access.DeleteOneDashBoard((*it)->GetID());
			COLORREF color = (*it)->GetFontColor();
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToListDashBoard(m_pPage->GetID(), (*it)->GetID(), strBindPointName.c_str(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (*it)->GetEqmType(), (*it)->GetStyle(), (*it)->GetMaxValue(), (*it)->GetMinValue(),
					strMaxStr.c_str(), strMinStr.c_str(), (*it)->GetMaxBindFlag(), (*it)->GetMinBindFlag(), (*it)->GetFontColor(), (*it)->GetDecimalPlaces())!=0 ,(*it)->GetFontColor(), (*it)->GetDecimalPlaces())
				{
					OutputSaveErr(L"InsertRecordToListDashBoard",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}
//严观微增加
wstring CProjectSqlite::LoadProjectSqlite_DashBoard()
{
	CString strError;//golding
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_dashboard where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_DASHBOARD<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	CString strBindPointName;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	int nStyle = 0;
	double dMax = 0.0f;
	double dMin = 0.0f;
	EQMDRAWTYPE eType = EQMDRAW_DASHBOARD;
	CString	strMaxVal;
	CString	strMinVal;
	bool	bIsMaxBindPt;
	bool	bIsMinBindPt;
	//20191208
	COLORREF nFontColor;// 仪表盘字体颜色
	int nDecimalPlaces;// 小数位数

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		nStyle = access.getColumn_Int(2);
		dMax = access.getColumn_Double(3);
		dMin = access.getColumn_Double(4);

		//20191208
		nFontColor = access.getColumn_Int(5);
		nDecimalPlaces = access.getColumn_Int(6);

		const char* lpszBindPointName = access.getColumn_Text(9);
		nX = access.getColumn_Int(10);
		nY = access.getColumn_Int(11);
		nWidth = access.getColumn_Int(12);
		nHeight = access.getColumn_Int(13);
		nLayer = access.getColumn_Int(14);
		eType = (EQMDRAWTYPE)access.getColumn_Int(15);
		if (lpszBindPointName)
		{
			strBindPointName = UtilString::ConvertMultiByteToWideChar(lpszBindPointName).c_str();
		}
		strMaxVal = access.getColumn_Text(16);
		strMinVal = access.getColumn_Text(17);
		bIsMaxBindPt = access.getColumn_Int(18);
		bIsMinBindPt = access.getColumn_Int(19);
		

		CEqmDrawDashBoard* pDashBoard = new CEqmDrawDashBoard();
		if(pDashBoard)
		{
			pDashBoard->SetPos(Gdiplus::PointF(nX, nY));
			pDashBoard->SetEqmType(eType);
			pDashBoard->SetPageID(nPageID);
			pDashBoard->SetID(nElementID);
			pDashBoard->SetWidth(nWidth);
			pDashBoard->SetHeight(nHeight);
			pDashBoard->SetStyle(nStyle);
			pDashBoard->SetLayer(nLayer);
			pDashBoard->SetMaxValue(dMax);
			pDashBoard->SetMinValue(dMin);
			pDashBoard->SetBindPointName(strBindPointName);
			pDashBoard->SetMaxString(strMaxVal);
			pDashBoard->SetMinString(strMinVal);
			pDashBoard->SetMaxBindFlag(bIsMaxBindPt);
			pDashBoard->SetMinBindFlag(bIsMinBindPt);
			//20191208
			pDashBoard->SetFontColor(nFontColor);
			pDashBoard->SetDecimalPlaces(nDecimalPlaces);

			int nPicID = 0;
			Image* pBitmap = NULL;
			wchar_t szIdx[MAX_PATH] = {0};
			if(nStyle == 0)
			{
				nPicID = g_nPicIndexDashBoard1;
				PictureInfo* pInfoPointer = GetPictureInfoByID(g_nPicIndexPointer1);
				if (pInfoPointer)
				{
					_itow_s(g_nPicIndexPointer1, szIdx, 10);
					if(!SaveMemBitmapToFile(pInfoPointer->pBlock, pInfoPointer->nSize, szIdx))
					{
						CString strMsg;
						strMsg.Format(L"仪表盘图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDashBoard->GetID(), L"", pDashBoard->GetPhysicalID().c_str());
						strError +=strMsg;
						strError +=L"\r\n";
					}
				}
				else
				{
					CString strMsg;
					strMsg.Format(L"仪表盘图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDashBoard->GetID(), L"", pDashBoard->GetPhysicalID().c_str());
					strError +=strMsg;
					strError +=L"\r\n";
				}

				PictureInfo* pInfoDial = GetPictureInfoByID(nPicID);
				if (pInfoDial)
				{
					CString strName;
					strName.Format(L"%d",nPicID);
					if(!SaveMemBitmapToFile(pInfoPointer->pBlock, pInfoPointer->nSize, strName))
					{
						CString strMsg;
						strMsg.Format(L"仪表盘图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDashBoard->GetID(), L"", pDashBoard->GetPhysicalID().c_str());
						strError +=strMsg;
						strError +=L"\r\n";
					}
				}
				else
				{
					CString strMsg;
					strMsg.Format(L"仪表盘图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDashBoard->GetID(), L"", pDashBoard->GetPhysicalID().c_str());
					strError +=strMsg;
					strError +=L"\r\n";
				}
				pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\meterpan_0.png");
			}
			else if(nStyle == 1)
			{
				nPicID = g_nPicIndexDashBoard2;
				PictureInfo* pInfoPointer = GetPictureInfoByID(g_nPicIndexPointer2);
				if (pInfoPointer)
				{
					_itow_s(g_nPicIndexPointer2, szIdx, 10);
					if(!SaveMemBitmapToFile(pInfoPointer->pBlock, pInfoPointer->nSize, szIdx))
					{
						CString strMsg;
						strMsg.Format(L"仪表盘图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDashBoard->GetID(), L"", pDashBoard->GetPhysicalID().c_str());
						strError +=strMsg;
						strError +=L"\r\n";
					}
				}
				else
				{
					CString strMsg;
					strMsg.Format(L"仪表盘图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDashBoard->GetID(), L"", pDashBoard->GetPhysicalID().c_str());
					strError +=strMsg;
					strError +=L"\r\n";
				}

				PictureInfo* pInfoDial = GetPictureInfoByID(nPicID);
				if (pInfoDial)
				{
					CString strName;
					strName.Format(L"%d",nPicID);
					if(!SaveMemBitmapToFile(pInfoPointer->pBlock, pInfoPointer->nSize, strName))
					{
						CString strMsg;
						strMsg.Format(L"仪表盘图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDashBoard->GetID(), L"", pDashBoard->GetPhysicalID().c_str());
						strError +=strMsg;
						strError +=L"\r\n";
					}
				}
				else
				{
					CString strMsg;
					strMsg.Format(L"仪表盘图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pDashBoard->GetID(), L"", pDashBoard->GetPhysicalID().c_str());
					strError +=strMsg;
					strError +=L"\r\n";
				}
				pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\meterpan_1.png");
			}
			else if(nStyle == 2)
			{
				//仪表盘3图片地址
				pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\meterpan_2.png");
			}
			else if(nStyle == 3)
			{
				//仪表盘4图片地址
				pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\meterpan_3.png");
			}
			else
			{	// error, unknown type, set as type = 0
				pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\meterpan_0.png");
			}

			if(pBitmap)
			{
				pDashBoard->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddDashboard(pDashBoard);
			
		}
		pDashBoard->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strError.GetString();
}
//严观微增加
string CProjectSqlite::GetBindInfoSqlite( const int nPageID, const int nElementID )
{
	string strBind;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from event where pageid="<<nPageID<<" and id="<<nElementID<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		const char* lpszBindString = access.getColumn_Text(24);
		if (lpszBindString)
		{
			strBind = lpszBindString;
		}
	}
	access.SqlFinalize();
	return strBind;
}
//严观微增加
string CProjectSqlite::GetEventInfoSqlite( const int nPageID, const int nElementID )
{
	string strEvent;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from event where pageid="<<nPageID<<" and id="<<nElementID<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		const char* lpszEventString = access.getColumn_Text(6);
		if (lpszEventString)
		{
			strEvent = lpszEventString;
		}
	}
	access.SqlFinalize();
	return strEvent;
}

void CProjectSqlite::GetEventAndBindInfoSqlite(const int nPageID, const int nElementID, string& strEvent, string& strBind)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from event where pageid=" << nPageID << " and id=" <<nElementID << ";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	const char* lpszEvent = NULL;
	const char* lpszBind = NULL;
	bool bIsEventFound = false;
	bool bIsBindFound = false;
	while (true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		bIsEventFound = false;
		bIsBindFound = false;

		lpszEvent = access.getColumn_Text(6);
		if (lpszEvent != NULL)
		{
			strEvent = lpszEvent;
			bIsEventFound = true;
		}

		lpszBind = access.getColumn_Text(24);
		if (lpszBind != NULL)
		{
			strBind = lpszBind;
			bIsBindFound = true;
		}

		if (bIsEventFound && bIsBindFound)
		{
			break;
		}
	}
	access.SqlFinalize();
}

//严观微增加
wstring CProjectSqlite::LoadProjectSqlite_LineChart()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_chart where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_LINE_CHART<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	string strChartName;
	int nUpdateInterval = 0;
	string strXName;
	string strXUnit; 
	string strYName;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	double dMax = 0.f;
	EQMDRAWTYPE eType = EQMDRAW_LINE_CHART;
	E_BAR_TYPE eBarType = eBarNormal;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		eBarType = (E_BAR_TYPE)access.getColumn_Int(2);
		dMax = access.getColumn_Double(19);

		if (access.getColumn_Text(5))
		{
			strChartName = access.getColumn_Text(5);
		}
		nUpdateInterval = access.getColumn_Int(9);
		if (access.getColumn_Text(6))
		{
			strXName = access.getColumn_Text(6);
		}
		if (access.getColumn_Text(8))
		{
			strXUnit = access.getColumn_Text(8);
		}
		if (access.getColumn_Text(7))
		{
			strYName = access.getColumn_Text(7);
		}
		nX = access.getColumn_Int(13);
		nY = access.getColumn_Int(14);
		nWidth = access.getColumn_Int(15);
		nHeight = access.getColumn_Int(16);
		nLayer = access.getColumn_Int(17);
		eType = (EQMDRAWTYPE)access.getColumn_Int(18);
		string strColor;
		if (access.getColumn_Text(21))
		{
			strColor = access.getColumn_Text(21);
		}
		double dMinValue = 0.0f;
		int nMaxCheck = 0;
		int nMinCheck = 0;
		string strMinValue;
		if (access.getColumn_Text(22))
		{
			strMinValue = access.getColumn_Text(22);
			dMinValue = atof(strMinValue.c_str());
		}
		string strMaxCheck;
		if (access.getColumn_Text(23))
		{
			strMaxCheck = access.getColumn_Text(23);
			nMaxCheck = atoi(strMaxCheck.c_str());
		}
		string strMinCheck;
		if (access.getColumn_Text(24))
		{
			strMinCheck = access.getColumn_Text(24);
			nMinCheck = atoi(strMinCheck.c_str());
		}
		
		COLORREF nColor = RGB(0,0,0);
		if (strColor.size() > 0)
		{
			nColor = atoi(strColor.c_str());
		}
		CEqmDrawLineGraphic* pLineGraphic = new CEqmDrawLineGraphic();
		if(pLineGraphic)
		{
			pLineGraphic->SetTextColor(nColor);
			pLineGraphic->SetPos(Gdiplus::PointF(nX, nY));
			pLineGraphic->SetEqmType(eType);
			pLineGraphic->SetPageID(nPageID);
			pLineGraphic->SetID(nElementID);
			pLineGraphic->SetLayer(nLayer);
			pLineGraphic->SetTitleName(UtilString::ConvertMultiByteToWideChar(strChartName).c_str());
			pLineGraphic->SetUpdateInterval(nUpdateInterval);
			pLineGraphic->SetXName(UtilString::ConvertMultiByteToWideChar(strXName).c_str());
			pLineGraphic->SetXUnit(UtilString::ConvertMultiByteToWideChar(strXUnit).c_str());
			pLineGraphic->SetYName(UtilString::ConvertMultiByteToWideChar(strYName).c_str());
			pLineGraphic->SetWidth(nWidth);
			pLineGraphic->SetHeight(nHeight);
			pLineGraphic->SetType(eBarType);
			pLineGraphic->SetMaxValue(dMax);
			pLineGraphic->SetMinValue(dMinValue);
			pLineGraphic->SetMaxCheck(nMaxCheck);
			pLineGraphic->SetMinCheck(nMinCheck);
			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartLine.png");
			if(pBitmap)
			{
				pLineGraphic->SetDefaultBitmap(pBitmap);
			}
			GetDetailChartByIDs(nPageID, nElementID, pLineGraphic->GetItemVector());
		}
		m_pPage->AddLineGraphic(pLineGraphic);
		
		pLineGraphic->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_LineChart()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawLineGraphic*>& linegraphic_list = m_pPage->GetLineGraphList();
	list<CEqmDrawLineGraphic*>::const_iterator it = linegraphic_list.begin();

	access.BeginTransaction();
	for (;it!=linegraphic_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			string strTitleName = UtilString::ConvertWideCharToMultiByte((*it)->GetTitleName().GetString());
			string strXName = UtilString::ConvertWideCharToMultiByte((*it)->GetXName().GetString());
			string strXUnit = UtilString::ConvertWideCharToMultiByte((*it)->GetXUnit().GetString());
			string strYName = UtilString::ConvertWideCharToMultiByte((*it)->GetYName().GetString());
			
			const double dMinValue = (*it)->GetMinValue();
			const int nCheckMax = (*it)->GetMaxCheck();
			const int nCheckMin = (*it)->GetMinCheck();
			char szMinValue[MAX_PATH] = {0};
			char szCheckMax[MAX_PATH] = {0};
			char szCheckMin[MAX_PATH] = {0};
			
			sprintf_s(szMinValue, sizeof(szMinValue)/sizeof(char), "%.2f", dMinValue);
			sprintf_s(szCheckMax, sizeof(szCheckMax)/sizeof(char), "%d", nCheckMax);
			sprintf_s(szCheckMin, sizeof(szCheckMin)/sizeof(char), "%d", nCheckMin);

			char szColor[MAX_PATH] = {0};
			sprintf_s(szColor, sizeof(szColor)/sizeof(char), "%d", (*it)->GetTextColor());
			ASSERT(strlen(szColor) > 0);
			access.DeleteOneChart((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToListChart((*it)->GetPageID(), (*it)->GetID(), strTitleName.c_str(), (*it)->GetUpdateInterval(), strXName.c_str(), strXUnit.c_str(),
					strYName.c_str(),(*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (int)(*it)->GetEqmType(), (*it)->GetType(), (*it)->GetMaxValue(), "", -1,
					szColor,szMinValue,szCheckMax,szCheckMin,"")!=0)
				{
					OutputSaveErr(L"InsertRecordToListChart",access.GetSqliteErr());
				}
				vector<_Graph_Item_Property>& vec = (*it)->GetItemVector();
				const size_t size = vec.size();
				access.DeleteOneDetailChart((*it)->GetPageID(), (*it)->GetID());
				for (size_t i=0;i<size;++i)
				{
					string strItemName = UtilString::ConvertWideCharToMultiByte(vec[i].strItemName.GetString());
					string strBindPointName = UtilString::ConvertWideCharToMultiByte(vec[i].strPointName.GetString());
					int nColor = vec[i].color;
					//access.InsertRecordToDetailChart((*it)->GetPageID(), (*it)->GetID(), strItemName.c_str(), nColor, strBindPointName.c_str());
					if(access.InsertRecordToDetailChart((*it)->GetPageID(), (*it)->GetID(), strItemName.c_str(), nColor, strBindPointName.c_str(),vec[i].nStoreType)!=0)
					{
						OutputSaveErr(L"InsertRecordToDetailChart",access.GetSqliteErr());
					}
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

void CProjectSqlite::GetDetailChartByIDs( const int nPageID, const int nElementID, vector<_Graph_Item_Property>& vecItem )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from detail_chart where pageid="<<nPageID<<" and id="<<nElementID<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		string strItemName;
		int nColor = 0;
		string strBindPointName;
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(13))
		{
			strItemName = access.getColumn_Text(13);
		}
		nColor = access.getColumn_Int(2);
		strBindPointName = access.getColumn_Text(16);
		_Graph_Item_Property item;
		item.strItemName = UtilString::ConvertMultiByteToWideChar(strItemName).c_str();
		item.color = nColor;
		item.strPointName = UtilString::ConvertMultiByteToWideChar(strBindPointName).c_str();
		item.nStoreType = access.getColumn_Int(17);
		vecItem.push_back(item);
	}
	access.SqlFinalize();
}

wstring CProjectSqlite::LoadProjectSqlite_BarChart()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_chart where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_BAR_CHART<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	string strChartName;
	int nUpdateInterval = 0;
	string strXName;
	string strXUnit; 
	string strYName;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	int nQueryType = 0;
	EQMDRAWTYPE eType = EQMDRAW_BAR_CHART;
	E_BAR_TYPE eBarType = eBarNormal;
	double dMax = 0.f;
	string strStartPoint;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		eBarType = (E_BAR_TYPE)access.getColumn_Int(2);
		dMax = access.getColumn_Double(19);

		if (access.getColumn_Text(5))
		{
			strChartName = access.getColumn_Text(5);
		}
		nUpdateInterval = access.getColumn_Int(9);
		if (access.getColumn_Text(6))
		{
			strXName = access.getColumn_Text(6);
		}
		if (access.getColumn_Text(8))
		{
			strXUnit = access.getColumn_Text(8);
		}
		if (access.getColumn_Text(7))
		{
			strYName = access.getColumn_Text(7);
		}
		if (access.getColumn_Text(11))
		{
			strStartPoint = access.getColumn_Text(11);
		}
		nQueryType = access.getColumn_Int(20);
		
		nX = access.getColumn_Int(13);
		nY = access.getColumn_Int(14);
		nWidth = access.getColumn_Int(15);
		nHeight = access.getColumn_Int(16);
		nLayer = access.getColumn_Int(17);
		eType = (EQMDRAWTYPE)access.getColumn_Int(18);
		string strColor;
		if (access.getColumn_Text(21))
		{
			strColor = access.getColumn_Text(21);
		}
		COLORREF nColor = RGB(0,0,0);
		if (strColor.size() > 0)
		{
			nColor = atoi(strColor.c_str());
		}
		CEqmDrawBarGraphic* pBarGraphic = new CEqmDrawBarGraphic();
		if(pBarGraphic)
		{
			pBarGraphic->SetTextColor(nColor);
			pBarGraphic->SetPos(Gdiplus::PointF(nX, nY));
			pBarGraphic->SetEqmType(eType);
			pBarGraphic->SetPageID(nPageID);
			pBarGraphic->SetID(nElementID);
			pBarGraphic->SetLayer(nLayer);
			pBarGraphic->SetTitleName(UtilString::ConvertMultiByteToWideChar(strChartName).c_str());
			pBarGraphic->SetUpdateInterval(nUpdateInterval);
			pBarGraphic->SetXName(UtilString::ConvertMultiByteToWideChar(strXName).c_str());
			pBarGraphic->SetXUnit(UtilString::ConvertMultiByteToWideChar(strXUnit).c_str());
			pBarGraphic->SetYName(UtilString::ConvertMultiByteToWideChar(strYName).c_str());
			pBarGraphic->SetWidth(nWidth);
			pBarGraphic->SetHeight(nHeight);
			pBarGraphic->SetType(eBarType);
			pBarGraphic->SetMaxValue(dMax);
			pBarGraphic->SetStartPoint(UtilString::ConvertMultiByteToWideChar(strStartPoint).c_str());
			pBarGraphic->SetQueryType(nQueryType);
			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartBar.png");
			if(pBitmap)
			{
				pBarGraphic->SetDefaultBitmap(pBitmap);
			}
			GetDetailChartByIDs(nPageID, nElementID, pBarGraphic->GetItemVector());
		}
		m_pPage->AddBarGraphic(pBarGraphic);
	
		pBarGraphic->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

wstring CProjectSqlite::LoadProjectSqlite_PieChart()
{
	wstring strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_chart where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_PIE_CHART<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	string strChartName;
	int nUpdateInterval = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	EQMDRAWTYPE eType = EQMDRAW_PIE_CHART;
	E_BAR_TYPE eBarType = eBarNormal;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		eBarType = (E_BAR_TYPE)access.getColumn_Int(2);
		if (access.getColumn_Text(5))
		{
			strChartName = access.getColumn_Text(5);
		}
		nUpdateInterval = access.getColumn_Int(9);
		nX = access.getColumn_Int(13);
		nY = access.getColumn_Int(14);
		nWidth = access.getColumn_Int(15);
		nHeight = access.getColumn_Int(16);
		nLayer = access.getColumn_Int(17);
		eType = (EQMDRAWTYPE)access.getColumn_Int(18);
		string strColor;
		if (access.getColumn_Text(21))
		{
			strColor = access.getColumn_Text(21);
		}
		COLORREF nColor = RGB(0,0,0);
		if (strColor.size() > 0)
		{
			nColor = atoi(strColor.c_str());
		}
		CEqmDrawPieGraphic* pPieGraphic = new CEqmDrawPieGraphic();
		if(pPieGraphic)
		{
			pPieGraphic->SetTextColor(nColor);
			pPieGraphic->SetPos(Gdiplus::PointF(nX, nY));
			pPieGraphic->SetEqmType(eType);
			pPieGraphic->SetPageID(nPageID);
			pPieGraphic->SetID(nElementID);
			pPieGraphic->SetLayer(nLayer);
			pPieGraphic->SetTitleName(UtilString::ConvertMultiByteToWideChar(strChartName).c_str());
			pPieGraphic->SetUpdateInterval(nUpdateInterval);
			pPieGraphic->SetWidth(nWidth);
			pPieGraphic->SetHeight(nHeight);
			pPieGraphic->SetType(eBarType);
			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartPie.png");
			if(pBitmap)
			{
				pPieGraphic->SetDefaultBitmap(pBitmap);
			}
			GetDetailChartByIDs(nPageID, nElementID, pPieGraphic->GetItemVector());
		}
		m_pPage->AddPieGraphic(pPieGraphic);
		
		pPieGraphic->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll;
}

bool CProjectSqlite::SaveProjectSqlite_BarChart()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawBarGraphic*>& bargraphic_list = m_pPage->GetBarGraphList();
	list<CEqmDrawBarGraphic*>::const_iterator it = bargraphic_list.begin();

	access.BeginTransaction();
	for (;it!=bargraphic_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			string strTitleName = UtilString::ConvertWideCharToMultiByte((*it)->GetTitleName().GetString());
			string strXName = UtilString::ConvertWideCharToMultiByte((*it)->GetXName().GetString());
			string strXUnit = UtilString::ConvertWideCharToMultiByte((*it)->GetXUnit().GetString());
			string strYName = UtilString::ConvertWideCharToMultiByte((*it)->GetYName().GetString());
			string strStartPoint = UtilString::ConvertWideCharToMultiByte((*it)->GetStartPoint().GetString());
			char szColor[MAX_PATH] = {0};
			sprintf_s(szColor, sizeof(szColor)/sizeof(char), "%d", (*it)->GetTextColor());
			ASSERT(strlen(szColor) > 0);
			access.DeleteOneChart((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToListChart((*it)->GetPageID(), (*it)->GetID(), strTitleName.c_str(), (*it)->GetUpdateInterval(), strXName.c_str(), strXUnit.c_str(),
					strYName.c_str(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (int)(*it)->GetEqmType(), (*it)->GetType(), (*it)->GetMaxValue(), strStartPoint.c_str(), (*it)->GetQueryType(),
					szColor,"","","","")!=0)
				{
					OutputSaveErr(L"InsertRecordToListChart",access.GetSqliteErr());
				}
				vector<_Graph_Item_Property>& vec = (*it)->GetItemVector();
				const size_t size = vec.size();
				access.DeleteOneDetailChart((*it)->GetPageID(), (*it)->GetID());
				for (size_t i=0;i<size;++i)
				{
					string strItemName = UtilString::ConvertWideCharToMultiByte(vec[i].strItemName.GetString());
					string strBindPointName = UtilString::ConvertWideCharToMultiByte(vec[i].strPointName.GetString());
					int nColor = vec[i].color;				
					//access.InsertRecordToDetailChart((*it)->GetPageID(), (*it)->GetID(), strItemName.c_str(), nColor, strBindPointName.c_str());
					if(access.InsertRecordToDetailChart((*it)->GetPageID(), (*it)->GetID(), strItemName.c_str(), nColor, strBindPointName.c_str(),vec[i].nStoreType)!=0)
					{
						OutputSaveErr(L"InsertRecordToDetailChart",access.GetSqliteErr());
					}
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

bool CProjectSqlite::SaveProjectSqlite_PieChart()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawPieGraphic*>& piegraphic_list = m_pPage->GetPieGraphList();
	list<CEqmDrawPieGraphic*>::const_iterator it = piegraphic_list.begin();

	access.BeginTransaction();
	for (;it!=piegraphic_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			string strTitleName = UtilString::ConvertWideCharToMultiByte((*it)->GetTitleName().GetString());
			char szColor[MAX_PATH] = {0};
			sprintf_s(szColor, sizeof(szColor)/sizeof(char), "%d", (*it)->GetTextColor());
			ASSERT(strlen(szColor) > 0);
			access.DeleteOneChart((*it)->GetID());
			if(access.InsertRecordToListChart((*it)->GetPageID(), (*it)->GetID(), strTitleName.c_str(), (*it)->GetUpdateInterval(), "", "",
				"",(*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (int)(*it)->GetEqmType(), (*it)->GetType(), 0, "", -1,
				szColor,"","","","")!=0)
			{
				OutputSaveErr(L"InsertRecordToListChart",access.GetSqliteErr());
			}
			vector<_Graph_Item_Property>& vec = (*it)->GetItemVector();
			const size_t size = vec.size();
			access.DeleteOneDetailChart((*it)->GetPageID(), (*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				for (size_t i=0;i<size;++i)
				{
					string strItemName = UtilString::ConvertWideCharToMultiByte(vec[i].strItemName.GetString());
					string strBindPointName = UtilString::ConvertWideCharToMultiByte(vec[i].strPointName.GetString());
					int nColor = vec[i].color;
					//access.InsertRecordToDetailChart((*it)->GetPageID(), (*it)->GetID(), strItemName.c_str(), nColor, strBindPointName.c_str());
					if(access.InsertRecordToDetailChart((*it)->GetPageID(), (*it)->GetID(), strItemName.c_str(), nColor, strBindPointName.c_str(),vec[i].nStoreType)!=0)
					{
						OutputSaveErr(L"InsertRecordToDetailChart",access.GetSqliteErr());
					}
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

wstring CProjectSqlite::LoadProjectSqlite_Bend()
{
	CString strError;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_bend where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_BEND<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	EQMDRAWTYPE eType = EQMDRAW_BEND;
	int nAngle = 0;

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		nX = access.getColumn_Int(2);
		nY = access.getColumn_Int(3);
		nWidth = access.getColumn_Int(4);
		nHeight = access.getColumn_Int(5);
		nLayer = access.getColumn_Int(6);
		eType = (EQMDRAWTYPE)access.getColumn_Int(7);
		nAngle = access.getColumn_Int(8);

		CEqmDrawBend* pBend = new CEqmDrawBend();
		if(pBend)
		{
			pBend->SetPos(Gdiplus::PointF(nX, nY));
			pBend->SetEqmType(eType);
			pBend->SetPageID(nPageID);
			pBend->SetID(nElementID);
			pBend->SetWidth(nWidth);
			pBend->SetHeight(nHeight);
			pBend->SetRotateAngle(nAngle);
			pBend->SetLayer(nLayer);
			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\bend.png");
			if(pBitmap)
			{
				pBend->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddBend(pBend);
			
			wchar_t	szIdx[MAX_PATH] = {0};
			_itow_s(g_nPicIndexBend, szIdx, 10);

		}
		pBend->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strError.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_Bend()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawBend*>& bend_list = m_pPage->GetBendList();
	list<CEqmDrawBend*>::const_iterator it = bend_list.begin();

	access.BeginTransaction();
	for (;it!=bend_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneBend((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToListBend((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (int)(*it)->GetEqmType(),(*it)->GetRotateAngle())!=0)
				{
					OutputSaveErr(L"InsertRecordToListBend",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

void CProjectSqlite::SetOwner( const E_OWNER eOwner )
{
	m_eOwner = eOwner;
}

E_OWNER CProjectSqlite::GetOwner()
{
	return m_eOwner;
}

bool CProjectSqlite::DeleteDirectory(const wchar_t* sDirName)
{ 
	CFileFind tempFind; 
	wchar_t sTempFileFind[200];

	wsprintf(sTempFileFind,L"%s\\*.*",sDirName); 
	BOOL IsFinded = tempFind.FindFile(sTempFileFind); 
	while (IsFinded) 
	{ 
		IsFinded = tempFind.FindNextFile(); 

		if (!tempFind.IsDots()) 
		{ 
			wchar_t sFoundFileName[200]; 
			wcscpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 

			if (tempFind.IsDirectory()) 
			{ 
				wchar_t sTempDir[200]; 
				wsprintf(sTempDir,L"%s\\%s",sDirName,sFoundFileName); 
				DeleteDirectory(sTempDir); 
			} 
			else 
			{ 
				wchar_t sTempFileName[200]; 
				wsprintf(sTempFileName,L"%s\\%s",sDirName,sFoundFileName); 
				DeleteFile(sTempFileName); 
			} 
		} 
	} 
	tempFind.Close(); 
	if(!RemoveDirectory(sDirName)) 
	{ 
		return FALSE; 
	} 
	return TRUE; 
}

bool CProjectSqlite::SaveProjectSqlite_Button()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawButton*>& button_list = m_pPage->GetButtonList();
	list<CEqmDrawButton*>::const_iterator it = button_list.begin();

	ostringstream sqlstream;
	char szDlEnable[MAX_PATH] = {0};
	char szDlName[MAX_PATH] = {0};
	USES_CONVERSION;

	access.BeginTransaction();
	for (;it!=button_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			int nComm = -1;
			int nDown = -1;
			int nOver = -1;
			int nDisable = -1;
			int nChecked = -1;
			(*it)->Get5PicID(nComm, nDisable, nDown, nOver, nChecked);
			assert(nComm != -1 && nDown != -1 && nOver != -1 && nDisable != -1 && nChecked != -1);
			string strBindSetPointName = UtilString::ConvertWideCharToMultiByte((*it)->GetBindSetPointName().GetString());
			string strTextContent = UtilString::ConvertWideCharToMultiByte((*it)->GetTextContent().GetString());
			string strOperation = UtilString::ConvertWideCharToMultiByte((*it)->GetOperation().GetString());
			int		nRelation = (*it)->GetRelation();
			vector<_tagRelationExpress> vecExpress = (*it)->GetRelationVec();
			sqlstream.str("");
			for(int i=0; i<vecExpress.size(); i++)
			{
				_tagRelationExpress sExpress = vecExpress[i];
				string strName = UtilString::ConvertWideCharToMultiByte(sExpress.strPointName.GetString());
				sqlstream << strName << "|" << sExpress.nRelationExpress << "|" << sExpress.fResult << ",";
			}
			string strExpress = sqlstream.str();
			if(strExpress != "")
			{
				strExpress.erase(--strExpress.end());
			}

			sqlstream.str("");
			sqlstream << nRelation;
			string strRelation = sqlstream.str();

			access.DeleteOneButton((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				char szTemp[256]={0};
				_itoa_s(nChecked, szTemp, 256, 10);
				ASSERT(strlen(szTemp)>0);

				sprintf_s(szDlEnable, MAX_PATH, "%s", T2A((*it)->GetDownloadEnable()));
				sprintf_s(szDlName, MAX_PATH, "%s", T2A((*it)->GetDownloadName()));

				string strSettingValue;
				Project::Tools::WideCharToUtf8((*it)->GetSettingValue().GetString(),strSettingValue);

				if(access.InsertRecordToListButton((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (int)(*it)->GetEqmType(),nComm,nDown,nOver,nDisable,
					(*it)->GetLinkPageID(),strBindSetPointName.c_str(),strSettingValue.c_str(),strTextContent.c_str(),(*it)->GetTextColor(),(*it)->GetTextSize(),strOperation.c_str(),strExpress.c_str(),strRelation.c_str(),szTemp,
					szDlEnable, szDlName)!=0)
				{
					OutputSaveErr(L"InsertRecordToListButton",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

wstring CProjectSqlite::LoadProjectSqlite_Button()
{
	CString strError;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_button where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_METABUTTON<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	int nComm = 0;
	int nDown = 0;
	int nOver = 0;
	int nDisable = 0;
	int nChecked = 0;
	int nLinkPageID = 0;
	string strTextContent;
	string strBindSetPointName;
	//int nSettingValue = 0;
	CString strSettingValue = _T("0");
	COLORREF nColor = 0;
	int nSize = 0;
	EQMDRAWTYPE eType = EQMDRAW_METABUTTON;
	string strOperation;
	string strRelation;
	int nRelation = 0;
	string strDownloadEnable;
	string strDownloadName;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		nX = access.getColumn_Int(9);
		nY = access.getColumn_Int(10);
		nComm = access.getColumn_Int(4);
		nOver = access.getColumn_Int(5);
		nDown = access.getColumn_Int(6);
		nDisable = access.getColumn_Int(7);
		eType = (EQMDRAWTYPE)access.getColumn_Int(14);
		nLinkPageID = access.getColumn_Int(8);
		nWidth = access.getColumn_Int(11);
		nHeight = access.getColumn_Int(12);
		nLayer = access.getColumn_Int(13);
		if(access.getColumn_Text(15))
		{
			strSettingValue = access.getColumn_Text(15);
		}

		if(access.getColumn_Text(16))
		{
			strBindSetPointName = access.getColumn_Text(16);
		}
		if(access.getColumn_Text(17))
		{
			strTextContent = access.getColumn_Text(17);
		}
		nColor = access.getColumn_Int(18);
		nSize = access.getColumn_Int(19);
		if (access.getColumn_Text(20))
		{
			strOperation = access.getColumn_Text(20);
		}
		if (access.getColumn_Text(21))
		{
			strRelation = access.getColumn_Text(21);
		}
		if (access.getColumn_Text(22))
		{
			nRelation = atoi(access.getColumn_Text(22));
		}
		if (access.getColumn_Text(23))
		{
			nChecked = atoi(access.getColumn_Text(23));
		}
		if (NULL != access.getColumn_Text(24))
		{
			strDownloadEnable = access.getColumn_Text(24);
		}
		if (NULL != access.getColumn_Text(25))
		{
			strDownloadName = access.getColumn_Text(25);
		}
		CEqmDrawButton* pButton = new CEqmDrawButton();
		if(pButton)
		{
			pButton->SetPos(Gdiplus::PointF(nX, nY));
			pButton->SetEqmType(eType);
			pButton->SetPageID(nPageID);
			pButton->SetID(nElementID);
			pButton->SetWidth(nWidth);
			pButton->SetHeight(nHeight);
			pButton->SetLayer(nLayer);
			pButton->Set5PicID(nComm,nDisable,nDown,nOver,nChecked);
			pButton->SetLinkPageID(nLinkPageID);
			pButton->SetSettingValue(strSettingValue);
			pButton->SetTextColor(nColor);
			pButton->SetTextSize(nSize);
			pButton->SetTextContent(UtilString::ConvertMultiByteToWideChar(strTextContent.c_str()).c_str());
			pButton->SetBindSetPointName(UtilString::ConvertMultiByteToWideChar(strBindSetPointName.c_str()).c_str());
			pButton->SetOperation(UtilString::ConvertMultiByteToWideChar(strOperation.c_str()).c_str());
			Image* pBitmap = GetBitmapByIDFromPictureLib(nComm);
			if(pBitmap)
			{
				pButton->SetDefaultBitmap(pBitmap);
			}
			if (strRelation.size() > 0)
			{
				CString strRelationString = UtilString::ConvertMultiByteToWideChar(strRelation.c_str()).c_str();
				vector<wstring> vecContent;
				SplitStringByChar(strRelationString, L',', vecContent);
				const size_t sizeVec = vecContent.size();
				for (size_t i=0;i<sizeVec;++i)
				{
					vector<wstring> vecContent1;
					SplitStringByChar(vecContent[i].c_str(), L'|', vecContent1);
					if (vecContent1.size() == 3)
					{
						_tagRelationExpress express;
						express.strPointName = vecContent1[0].c_str();
						express.nRelationExpress = _wtoi(vecContent1[1].c_str());
						express.fResult = _wtoi(vecContent1[2].c_str());
						pButton->InsertExpress(express);
					}
				}
			}
			pButton->SetRelation(nRelation);

			CString strDlEnable = UtilString::ConvertMultiByteToWideChar(strDownloadEnable.c_str()).c_str();
			CString strDlName = UtilString::ConvertMultiByteToWideChar(strDownloadName.c_str()).c_str();
			pButton->SetDownloadEnable(strDlEnable);
			pButton->SetDownloadName(strDlName);

			m_pPage->AddButton(pButton);

		}
		pButton->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strError.GetString();
}

wstring CProjectSqlite::LoadProjectSqlite_TimeSelect()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_timeselect where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_TIME_SELECT<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	EQMDRAWTYPE eType = EQMDRAW_TIME_SELECT;
	string strBindPointName;
	string strDateTime;
	int nRWProperty = 0;
	int nDateTimeSelect;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		nX = access.getColumn_Int(2);
		nY = access.getColumn_Int(3);
		nWidth = access.getColumn_Int(4);
		nHeight = access.getColumn_Int(5);
		nLayer = access.getColumn_Int(6);
		eType = (EQMDRAWTYPE)access.getColumn_Int(7);
		if (access.getColumn_Text(8))
		{
			strBindPointName = access.getColumn_Text(8);
		}
		nRWProperty = access.getColumn_Int(9);
		nDateTimeSelect = access.getColumn_Int(10);
		CEqmDrawTimeSelect* pTimeSelect = new CEqmDrawTimeSelect();
		if(pTimeSelect)
		{
			pTimeSelect->SetPos(Gdiplus::PointF(nX, nY));
			pTimeSelect->SetEqmType(eType);
			pTimeSelect->SetPageID(nPageID);
			pTimeSelect->SetID(nElementID);
			pTimeSelect->SetWidth(nWidth);
			pTimeSelect->SetHeight(nHeight);
			pTimeSelect->SetLayer(nLayer);
			pTimeSelect->SetPointName(UtilString::ConvertMultiByteToWideChar(strBindPointName).c_str());
			DataPointEntry entry = m_pointmap.GetEntry(UtilString::ConvertMultiByteToWideChar(strBindPointName).c_str());
			CString strFormat = entry.GetParam(1).c_str();
			pTimeSelect->SetRWProperty(nRWProperty);
			pTimeSelect->SetTimeDataFormat(nDateTimeSelect);
			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\timeSelect.png");
			if(pBitmap)
			{
				pTimeSelect->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddTimeSelect(pTimeSelect);

			
		}
		pTimeSelect->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_TimeSelect()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawTimeSelect*>& timeselect_list = m_pPage->GetTimeSelectList();
	list<CEqmDrawTimeSelect*>::const_iterator it = timeselect_list.begin();

	access.BeginTransaction();
	for (;it!=timeselect_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			const CString strPointName = (*it)->GetPointName();
			string strBindPoint = UtilString::ConvertWideCharToMultiByte(strPointName.GetString());
			access.DeleteOneTimeSelect((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToListTimeSelect((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (int)(*it)->GetEqmType(),strBindPoint.c_str(),(*it)->GetRWProperty(), (*it)->GetTimeDataFormat()))
				{
					OutputSaveErr(L"InsertRecordToListTimeSelect",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

wstring CProjectSqlite::LoadProjectSqlite_TimeBar()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_timebar where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_TIME_BAR<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	EQMDRAWTYPE eType = EQMDRAW_TIME_BAR;
	string strBindPointStart;
	int nQueryType = 0;
	string strBindPointStatus;
	int nColor = 0;

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		nX = access.getColumn_Int(2);
		nY = access.getColumn_Int(3);
		nWidth = access.getColumn_Int(4);
		nHeight = access.getColumn_Int(5);
		nLayer = access.getColumn_Int(6);
		eType = (EQMDRAWTYPE)access.getColumn_Int(7);
		nColor = access.getColumn_Int(9);
		if (access.getColumn_Text(10))
		{
			strBindPointStart = access.getColumn_Text(10);
		}
		nQueryType = access.getColumn_Int(11);
		if (access.getColumn_Text(12))
		{
			strBindPointStatus = access.getColumn_Text(12);
		}
		CEqmDrawTimeBar* pTimeBar = new CEqmDrawTimeBar();
		if(pTimeBar)
		{
			pTimeBar->SetPos(Gdiplus::PointF(nX, nY));
			pTimeBar->SetEqmType(eType);
			pTimeBar->SetPageID(nPageID);
			pTimeBar->SetID(nElementID);
			pTimeBar->SetWidth(nWidth);
			pTimeBar->SetHeight(nHeight);
			pTimeBar->SetLayer(nLayer);
			pTimeBar->SetQueryType(nQueryType);
			pTimeBar->SetBindPointStart(UtilString::ConvertMultiByteToWideChar(strBindPointStart).c_str());
			pTimeBar->SetBindPointStatus(UtilString::ConvertMultiByteToWideChar(strBindPointStatus).c_str());
			pTimeBar->SetBarColor(nColor);
			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartBar.png");
			if(pBitmap)
			{
				pTimeBar->SetDefaultBitmap(pBitmap);
			}
			{
				CSqliteAcess accessItem(m_strFilePath.c_str(), m_bIsEncrypt);
				ostringstream sqlstreamItem;
				sqlstreamItem << "select * from list_timebaritem where barid="<<nElementID<<";";
				string sql_Item = sqlstreamItem.str();
				char *ex_sqlItem = const_cast<char*>(sql_Item.c_str());
				accessItem.SqlQuery(ex_sqlItem);

				int nIndex=0;
				string strName = "";
				string strBindName = "";

				while(true)
				{
					if(SQLITE_ROW != accessItem.SqlNext())
					{
						break;
					}
					nIndex = accessItem.getColumn_Int(1);
					if (accessItem.getColumn_Text(2))
					{
						strName = accessItem.getColumn_Text(2);
					}
					if (accessItem.getColumn_Text(3))
					{
						strBindName = accessItem.getColumn_Text(3);
					}
					_tag_bar_item item;
					item.nIndex = nIndex;
					item.strName = UtilString::ConvertMultiByteToWideChar(strName).c_str();
					pTimeBar->InsertItem(item);
				}
				accessItem.SqlFinalize();
			}
			m_pPage->AddTimeBar(pTimeBar);

			
		}
		pTimeBar->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_TimeBar()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawTimeBar*>& timebar_list = m_pPage->GetTimeBarList();
	list<CEqmDrawTimeBar*>::const_iterator it = timebar_list.begin();

	access.BeginTransaction();
	for (;it!=timebar_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			string strBindPointStart = UtilString::ConvertWideCharToMultiByte((*it)->GetBindPointStart().GetString());
			string strBindPointStatus = UtilString::ConvertWideCharToMultiByte((*it)->GetBindPointStatus().GetString());

			access.DeleteOneTimeBar((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToListTimeBar((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (int)(*it)->GetEqmType(),strBindPointStart.c_str(),(int)(*it)->GetBarColor(),
					(*it)->GetQueryType(),strBindPointStatus.c_str())!=0)
				{
					OutputSaveErr(L"InsertRecordToListTimeBar",access.GetSqliteErr());
				}
			}
			const vector<_tag_bar_item>& vec = (*it)->GetItemVec();
			const size_t size = vec.size();
			access.DeleteTimeBarItem((*it)->GetID());
			for (size_t i=0;i<size;++i)
			{
				string strName = "";
				string strBindName = "";
				if (vec[i].strName.GetLength() > 0)
				{
					strName = UtilString::ConvertWideCharToMultiByte(vec[i].strName.GetString());
				}			
				if(access.InsertRecordToListTimeBarItem((*it)->GetID(),vec[i].nIndex,strName.c_str(),"")!=0)
				{
					OutputSaveErr(L"InsertRecordToListTimeBarItem",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

wstring CProjectSqlite::LoadProjectSqlite_ProgressBar()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_progressbar where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_PROGRESS_BAR<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	EQMDRAWTYPE eType = EQMDRAW_PROGRESS_BAR;
	string strBindPointName;
	string strBindPointNameBase;
	int nColor = 0;
	double dMaxValue = 0.0f; 

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		nX = access.getColumn_Int(2);
		nY = access.getColumn_Int(3);
		nWidth = access.getColumn_Int(4);
		nHeight = access.getColumn_Int(5);
		nLayer = access.getColumn_Int(6);
		eType = (EQMDRAWTYPE)access.getColumn_Int(7);
		dMaxValue = access.getColumn_Double(8);
		nColor = access.getColumn_Int(9);
		if (access.getColumn_Text(10))
		{
			strBindPointName = access.getColumn_Text(10);
		}
		if (access.getColumn_Text(11))
		{
			strBindPointNameBase = access.getColumn_Text(11);
		}
		CEqmDrawProgressBar* pProgressBar = new CEqmDrawProgressBar();
		if(pProgressBar)
		{
			pProgressBar->SetPos(Gdiplus::PointF(nX, nY));
			pProgressBar->SetEqmType(eType);
			pProgressBar->SetPageID(nPageID);
			pProgressBar->SetID(nElementID);
			pProgressBar->SetWidth(nWidth);
			pProgressBar->SetHeight(nHeight);
			pProgressBar->SetLayer(nLayer);
			pProgressBar->SetBindPoint(UtilString::ConvertMultiByteToWideChar(strBindPointName).c_str());
			pProgressBar->SetBindPointBase(UtilString::ConvertMultiByteToWideChar(strBindPointNameBase).c_str());
			pProgressBar->SetFactorValue(dMaxValue);
			pProgressBar->SetBarColor(nColor);
			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\progressBar.png");
			if(pBitmap)
			{
				pProgressBar->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddProgressBar(pProgressBar);

		
		}
		pProgressBar->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_ProgressBar()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawProgressBar*>& progressbar_list = m_pPage->GetProgressBarList();
	list<CEqmDrawProgressBar*>::const_iterator it = progressbar_list.begin();

	access.BeginTransaction();
	for (;it!=progressbar_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			const CString strPointName = (*it)->GetBindPoint();
			string strBindPoint = UtilString::ConvertWideCharToMultiByte(strPointName.GetString());
			const CString strPointNameBase = (*it)->GetBindPointBase();
			string strBindPointBase = UtilString::ConvertWideCharToMultiByte(strPointNameBase.GetString());
			access.DeleteOneProgressBarData((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToListProgressBar((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (int)(*it)->GetEqmType(),strBindPoint.c_str(),(int)(*it)->GetBarColor(),(*it)->GetFactorValue(),strBindPointBase.c_str())!=0)
				{
					OutputSaveErr(L"InsertRecordToListProgressBar",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

void CProjectSqlite::DeleteElement(const EQMDRAWTYPE type,const int pageid,const int id)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	switch(type)
	{
	case EQMDRAW_METAFILE:
	case EQMDRAW_CHILLER:
	case EQMDRAW_PUMP:
	case EQMDRAW_COOLINGTOWER:
	case EQMDRAW_VALVE:
		access.DeleteOneDevice(id);
		break;
	case EQMDRAW_TEXT:
		access.DeleteOneText(id);
		break;
	case EQMDRAW_STRAIGHTPIPE:
	case EQMDRAW_FOLDPIPE:
		access.DeleteOnePipe(id);
		break;
	case GraphicType_LINE:
	case GraphicType_RECTANGLE:
	case GraphicType_TRIANGLE:
	case GraphicType_ELLIPSE:
		access.DeleteOneGraphic(id);
		break;
	case EQMDRAW_BEND:
		access.DeleteOneBend(id);
		break;
	case EQMDRAW_DASHBOARD:
		access.DeleteOneDashBoard(id);
		break;
	case EQMDRAW_LINE_CHART:
	case EQMDRAW_BAR_CHART:
	case EQMDRAW_PIE_CHART:
	case EQMDRAW_DIAGNOSE_CHART:
	case EQMDRAW_POLYGON:
		access.DeleteOneChart(id);
		access.DeleteOneDetailChart(pageid,id);
		break;
	case EQMDRAW_METABUTTON:
		access.DeleteOneButton(id);
		break;
	case EQMDRAW_TIME_SELECT:
		access.DeleteOneTimeSelect(id);
		break;
	case EQMDRAW_TIME_BAR:
		access.DeleteOneTimeBar(id);
		access.DeleteTimeBarItem(id);
		break;
	case EQMDRAW_PROGRESS_BAR:
		access.DeleteOneProgressBarData(id);
		break;
	case EQMDRAW_AREA_CHANGE:
		access.DeleteOneAreaChange(id);
		break;
	case EQMDRAW_SHELL_EXECUTE:
		access.DeleteOneShellExecute(id);
		break;
	case EQMDRAW_EMBEDED_PAGE:
		access.DeleteOneEmbededPage(id);
		break;
	case EQMDRAW_LIQUID_LEVEL:
		access.DeleteOneFromLiquidLevel(id);
		break;
	case EQMDRAW_CHKBTN_PLUS:
		access.DeleteOneFromCheckButtonPlus(id);
		break;
	case EQMDRAW_LINE:
		access.DeleteOneFromStraightLine(id);
		break;
	case EQMDRAW_HISTORY_TREND:
		access.DeleteOneFromHistoryTrend(id);
		break;
	case EQMDRAW_RECTANGLE:
		access.DeleteOneFromRectangle(id);
		break;
	case EQMDRAW_MAP_NAVIGATE:
		access.DeleteOneFromMapNavigate(id);
		break;
	case EQMDRAW_NAVIGATE_WINDOW:
		access.DeleteOneFromNavigateWindow(id);
		break;
	case EQMDRAW_LEVEL_RULER:
		access.DeleteOneFromLevelRuler(id);
		break;
	case EQMDRAW_DATA_ANALYSE:
		access.DeleteOneFromDataAnalyse(id);
		break;
	case EQMDRAW_TEMPER_DISTRI:
		access.DeleteOneFromTemperDistri(id);
		break;
	case EQMDRAW_DATA_REPORT:
		access.DeleteOneFromDataReport(id);
		break;
	case EQMDRAW_PLANE_TEMPE_DISTR:
		access.DeleteOneFromPlaneTempeDistr(id);
		break;
	case EQMDRAW_DOTLINE_FRAME:
		access.DeleteOneFromDottedLineFrm(id);
		break;
	case EQMDRAW_GENERAL_COMPONENT:
		access.DeleteOneFromGeneralComponent(id);
		break;
	default:
		break;
	}
	access.CommitTransaction();
}

void CProjectSqlite::DeletePage( CEqmDrawPage* pPage )
{
	ASSERT(pPage);

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.DeleteOnePage(pPage->GetID());
	
	for (list<CEqmDrawDevice*>::const_iterator it = pPage->GetDrawDeviceList().begin();
		it != pPage->GetDrawDeviceList().end(); it++)
	{
		access.DeleteOneDevice((*it)->GetID());
	}
	for (list<CEqmDrawPipe*>::const_iterator it = pPage->GetPagePipeList().begin();
		it != pPage->GetPagePipeList().end(); it++)
	{
		access.DeleteOnePipe((*it)->GetID());
	}

	for (list<CEqmDrawText*>::const_iterator it = pPage->GetPageTextList().begin();
		it != pPage->GetPageTextList().end(); it++)
	{
		access.DeleteOneText((*it)->GetID());
	}

	for (list<CEqmDrawGraphic*>::const_iterator it = pPage->Get2DGraphicList().begin();
		it != pPage->Get2DGraphicList().end(); it++)
	{
		access.DeleteOneGraphic((*it)->GetID());
	}

	for (list<CEqmDrawBend*>::const_iterator it = pPage->GetBendList().begin();
		it != pPage->GetBendList().end(); it++)
	{
		access.DeleteOneBend((*it)->GetID());
	}

	for (list<CEqmDrawDashBoard*>::const_iterator it = pPage->GetDashboardList().begin();
		it != pPage->GetDashboardList().end(); it++)
	{
		access.DeleteOneDashBoard((*it)->GetID());
	}

	for (list<CEqmDrawLineGraphic*>::const_iterator it = pPage->GetLineGraphList().begin();
		it != pPage->GetLineGraphList().end(); it++)
	{
		access.DeleteOneChart((*it)->GetID());
		access.DeleteOneDetailChart((*it)->GetPageID(),(*it)->GetID());
	}

	for (list<CEqmDrawBarGraphic*>::const_iterator it = pPage->GetBarGraphList().begin();
		it != pPage->GetBarGraphList().end(); it++)
	{
		access.DeleteOneChart((*it)->GetID());
		access.DeleteOneDetailChart((*it)->GetPageID(),(*it)->GetID());
	}

	for (list<CEqmDrawPieGraphic*>::const_iterator it = pPage->GetPieGraphList().begin();
		it != pPage->GetPieGraphList().end(); it++)
	{
		access.DeleteOneChart((*it)->GetID());
		access.DeleteOneDetailChart((*it)->GetPageID(),(*it)->GetID());
	}

	for (list<CEqmDrawButton*>::const_iterator it = pPage->GetButtonList().begin();
		it != pPage->GetButtonList().end(); it++)
	{
		access.DeleteOneButton((*it)->GetID());
	}

	for (list<CEqmDrawTimeSelect*>::const_iterator it = pPage->GetTimeSelectList().begin();
		it != pPage->GetTimeSelectList().end(); it++)
	{
		access.DeleteOneTimeSelect((*it)->GetID());
	}

	for (list<CEqmDrawTimeBar*>::const_iterator it = pPage->GetTimeBarList().begin();
		it != pPage->GetTimeBarList().end(); it++)
	{
		access.DeleteOneTimeBar((*it)->GetID());
		access.DeleteTimeBarItem((*it)->GetID());
	}

	for (list<CEqmDrawProgressBar*>::const_iterator it = pPage->GetProgressBarList().begin();
		it != pPage->GetProgressBarList().end(); it++)
	{
		access.DeleteOneProgressBarData((*it)->GetID());
	}
	//矩形
	for (list<CEqmDrawRectangle*>::const_iterator it = pPage->GetRectangleList().begin();
		it != pPage->GetRectangleList().end(); it++)
	{
		access.DeleteOneFromRectangle((*it)->GetID());
	}
	//通用组件
	for (list<CEqmDrawGeneralComponent*>::const_iterator it = pPage->GetGeneralComponentList().begin();
		it != pPage->GetGeneralComponentList().end(); it++)
	{
		access.DeleteOneFromGeneralComponent((*it)->GetID());
	}
	//标准设备
	for (list<CEqmDrawMapNavigate*>::const_iterator it = pPage->GetMapNavigateList().begin();
		it != pPage->GetMapNavigateList().end(); it++)
	{
		access.DeleteOneFromMapNavigate((*it)->GetID());
	}
	//直线
	for (list<CEqmDrawStraightLine*>::const_iterator it = pPage->GetStraightLineList().begin();
		it != pPage->GetStraightLineList().end(); it++)
	{
		access.DeleteOneFromStraightLine((*it)->GetID());
	}
	access.CommitTransaction();
}

void CProjectSqlite::DeleteGroup( const int nID )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.DeleteOneGroup(nID);
	access.CommitTransaction();
}

void CProjectSqlite::DeleteAllPoint()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.DeleteAllPoint();
	access.CommitTransaction();
}

void CProjectSqlite::DeleteOnePoint( const CString strPoint )
{
	ASSERT(strPoint.GetLength() > 0);
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	string strPointName = UtilString::ConvertWideCharToMultiByte(strPoint.GetString());
	access.DeleteOnePoint(strPointName.c_str());
	access.CommitTransaction();
}

void CProjectSqlite::DeletePoints( const vector<wstring>& vec )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	if(access.DeleteSqlitePoints(vec) != 0)
	{
		OutputSaveErr(L"DeleteSqlitePoints",access.GetSqliteErr());
	}
	access.CommitTransaction();
}

void CProjectSqlite::DeleteOnePicture(const int nID, const bool bDbSelect)
{
	if (bDbSelect)
	{
		vector<PictureInfo>::iterator iter = m_vPictureList.begin();
		while (iter != m_vPictureList.end())
		{
			if ((*iter).ID == nID)
			{
				if ((*iter).pBitmap)
				{
					delete (*iter).pBitmap;
					(*iter).pBitmap = NULL;
				}
				if ((*iter).pBlock)
				{
					delete (*iter).pBlock;
					(*iter).pBlock = NULL;
				}
				CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
				access.BeginTransaction();
				access.DeleteOnePicture((*iter).ID);
				access.CommitTransaction();
				iter = m_vPictureList.erase(iter);
				break;
			}
			else
			{
				++iter;
			}
		}
	}
	else
	{
		vector<PictureInfo>::iterator iter = m_vPictureListImage.begin();
		while (iter != m_vPictureListImage.end())
		{
			if ((*iter).ID == nID)
			{
				if ((*iter).pBitmap)
				{
					delete (*iter).pBitmap;
					(*iter).pBitmap = NULL;
				}
				if ((*iter).pBlock)
				{
					delete (*iter).pBlock;
					(*iter).pBlock = NULL;
				}
				CSqliteAcess access(m_strImgDbPath.c_str(), m_bIsEncrypt);
				access.BeginTransaction();
				access.DeleteOnePicture((*iter).ID);
				access.CommitTransaction();
				iter = m_vPictureListImage.erase(iter);
				break;
			}
			else
			{
				++iter;
			}
		}
	}
}

void CProjectSqlite::DeleteOneAnimation( const int nID )
{
	vector<AnimationInfo>::iterator iter = m_vAnimationListImage.begin();
	while (iter != m_vAnimationListImage.end())
	{
		if ((*iter).ID == nID)
		{
			{
				vector<PictureInfo>::iterator iter_pic = (*iter).vecPicture.begin();
				while (iter_pic != (*iter).vecPicture.end())
				{
					if ((*iter_pic).pBitmap)
					{
						delete (*iter_pic).pBitmap;
						(*iter_pic).pBitmap = NULL;
					}
					if ((*iter_pic).pBlock)
					{
						delete (*iter_pic).pBlock;
						(*iter_pic).pBlock = NULL;
					}
					iter_pic = (*iter).vecPicture.erase(iter_pic);
				}
				(*iter).vecPicture.clear();
			}
			{
				vector<Image*>::iterator iter_bmp = (*iter).vecpBitmap.begin();
				while (iter_bmp != (*iter).vecpBitmap.end())
				{
					if ((*iter_bmp))
					{
						//delete *iter_bmp;
						*iter_bmp = NULL;
					}
					++iter_bmp;
				}
				(*iter).vecpBitmap.clear();
			}
			{
				vector<int>::iterator iter_id = (*iter).vecPicIDList.begin();
				while (iter_id != (*iter).vecPicIDList.end())
				{
					CSqliteAcess access(m_strImgDbPath.c_str(), m_bIsEncrypt);
					access.BeginTransaction();
					access.DeleteOnePictureForAnimation((*iter_id));
					access.CommitTransaction();
					DeleteOnePictureForAnimation((*iter_id));
					++iter_id;
				}
				(*iter).vecPicIDList.clear();
			}
			CSqliteAcess access(m_strImgDbPath.c_str(), m_bIsEncrypt);
			access.BeginTransaction();
			access.DeleteOneAnimation((*iter).ID);
			access.CommitTransaction();
			iter = m_vAnimationListImage.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
	}
}

void CProjectSqlite::DeleteOnePictureForAnimation( const int nID )
{
	vector<PictureInfo>::iterator iter = m_vPictureforAnimationListImage.begin();
	while (iter != m_vPictureforAnimationListImage.end())
	{
		if ((*iter).ID == nID)
		{
			iter = m_vPictureforAnimationListImage.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
	}
}

bool CProjectSqlite::IsPageExist( const int nID )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_page where id="<<nID;
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	int nCount = 0;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		++nCount;
	}
	access.SqlFinalize();
	return nCount != 0;
}

wstring CProjectSqlite::LoadProjectSqlite_DiagnoseChart()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_chart where pageid="<<m_pPage->GetID()<<" and elementtype="<<(int)EQMDRAW_DIAGNOSE_CHART<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	string strChartName;
	int nUpdateInterval = 0;
	string strXName;
	string strYName;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	EQMDRAWTYPE eType = EQMDRAW_DIAGNOSE_CHART;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(0);
		nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(5))
		{
			strChartName = access.getColumn_Text(5);
		}
		nUpdateInterval = access.getColumn_Int(9);
		if (access.getColumn_Text(6))
		{
			strXName = access.getColumn_Text(6);
		}
		if (access.getColumn_Text(7))
		{
			strYName = access.getColumn_Text(7);
		}
		nX = access.getColumn_Int(13);
		nY = access.getColumn_Int(14);
		nWidth = access.getColumn_Int(15);
		nHeight = access.getColumn_Int(16);
		nLayer = access.getColumn_Int(17);
		eType = (EQMDRAWTYPE)access.getColumn_Int(18);

		CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = new CEqmDrawDiagnoseGraphic();
		if(pDiagnoseGraphic)
		{
			pDiagnoseGraphic->SetPos(Gdiplus::PointF(nX, nY));
			pDiagnoseGraphic->SetEqmType(eType);
			pDiagnoseGraphic->SetPageID(nPageID);
			pDiagnoseGraphic->SetID(nElementID);
			pDiagnoseGraphic->SetUpdateInterval(nUpdateInterval);
			pDiagnoseGraphic->SetWidth(nWidth);
			pDiagnoseGraphic->SetHeight(nHeight);
			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartDiagnose.png");
			if(pBitmap)
			{
				pDiagnoseGraphic->SetDefaultBitmap(pBitmap);
			}
			wstring wstrErrorLoadDetail =GetDetailChartByIDsDiagnose(nPageID, nElementID, pDiagnoseGraphic->GetItemVec());
			strErrorAll += wstrErrorLoadDetail.c_str();
			m_pPage->AddDiagnoseGraphic(pDiagnoseGraphic);
		}
	
		pDiagnoseGraphic->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_DiagnoseChart()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawDiagnoseGraphic*>& diagnosegraphic_list = m_pPage->GetDiagnoseList();
	list<CEqmDrawDiagnoseGraphic*>::const_iterator it = diagnosegraphic_list.begin();

	access.BeginTransaction();
	for (;it!=diagnosegraphic_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneChart((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertRecordToListChart((*it)->GetPageID(), (*it)->GetID(), "", (*it)->GetUpdateInterval(), "", "",
					"",(*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (int)(*it)->GetEqmType(), 0, 0.0, "", -1,
					"","","","","")!=0)
				{
					OutputSaveErr(L"InsertRecordToListChart",access.GetSqliteErr());
				}
				vector<_tagDiagnoseItem>& vec = (*it)->GetItemVec();
				const size_t size = vec.size();
				access.DeleteOneDetailChart((*it)->GetPageID(), (*it)->GetID());
				for (size_t i=0;i<size;++i)
				{
					string strItemName = UtilString::ConvertWideCharToMultiByte(vec[i].strItemName.GetString());
					string strBindPointName = UtilString::ConvertWideCharToMultiByte(vec[i].strPointName.GetString());
					int nColor = vec[i].color;
					if(access.InsertRecordToDetailChart((*it)->GetPageID(), (*it)->GetID(), strItemName.c_str(), nColor, strBindPointName.c_str())!=0)
					{
						OutputSaveErr(L"InsertRecordToDetailChart",access.GetSqliteErr());
					}
					access.DeleteDiagnoseLineDetail((*it)->GetPageID(), (*it)->GetID(),strBindPointName.c_str(),strItemName.c_str());
					for (size_t j=0;j<vec[i].vecItemInLine.size();++j)
					{
						string strSubBindPointName = UtilString::ConvertWideCharToMultiByte(vec[i].vecItemInLine[j].strPointName.GetString());
						string strSubDescription = UtilString::ConvertWideCharToMultiByte(vec[i].vecItemInLine[j].strDescription.GetString());
						if(access.InsertIntoDiagnoseLineDetail((*it)->GetPageID(), (*it)->GetID(),strBindPointName.c_str(),vec[i].vecItemInLine[j].nID,strSubBindPointName.c_str(),strSubDescription.c_str(),strItemName.c_str())!=0)
						{
							OutputSaveErr(L"InsertIntoDiagnoseLineDetail",access.GetSqliteErr());
						}
					}
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

wstring CProjectSqlite::GetDetailChartByIDsDiagnose( const int nPageID, const int nElementID, vector<_tagDiagnoseItem>& vecItem )
{
	wstring wstrErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from detail_chart where pageid="<<nPageID<<" and id="<<nElementID<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		string strItemName;
		int nColor = 0;
		string strBindPointName;
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(13))
		{
			strItemName = access.getColumn_Text(13);
		}
		nColor = access.getColumn_Int(2);
		strBindPointName = access.getColumn_Text(16);
		_tagDiagnoseItem item;
		item.strItemName = UtilString::ConvertMultiByteToWideChar(strItemName).c_str();
		item.color = nColor;
		item.strPointName = UtilString::ConvertMultiByteToWideChar(strBindPointName).c_str();
		vecItem.push_back(item);
		const size_t size = vecItem.size();
		wstrErrorAll +=GetDetailChartByIDsDiagnoseLineSubItem(nPageID, nElementID,strItemName.c_str(),strBindPointName.c_str(),vecItem[size-1].vecItemInLine);
	}
	access.SqlFinalize();

	return wstrErrorAll;
}

wstring CProjectSqlite::GetDetailChartByIDsDiagnoseLineSubItem( const int nPageID, const int nElementID, const char* lpszItemName, const char* lpszPointName, vector<_tagDiagnoseItemInLine>& vecItem )
{
	CString strError;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit01 where pageid="<<nPageID<<" and id="<<nElementID<<" and unitproperty01='"<<lpszPointName<<"' and unitproperty04='"<<lpszItemName<<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		string strDescription;
		string strBindPointName;
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		int nID = access.getColumn_Int(0);
		if (access.getColumn_Text(8))
		{
			strBindPointName = access.getColumn_Text(8);
		}
		if (access.getColumn_Text(9))
		{
			strDescription = access.getColumn_Text(9);
		}
		_tagDiagnoseItemInLine item;
		item.strPointName = UtilString::ConvertMultiByteToWideChar(strBindPointName).c_str();
		item.nID = nID;
	
		item.strDescription = UtilString::ConvertMultiByteToWideChar(strDescription).c_str();
		vecItem.push_back(item);
	}
	access.SqlFinalize();

	return strError.GetString();
}

wstring CProjectSqlite::LoadProjectSqlite_Polygon()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='polygon';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nLayer			= 0;

	string strPointList;
	int nTemplateGroupId = 0;
	int nTemplatePageId;;
	string strTemplatePageName = "";
	string strParam = "";
	string strTemplateName = "";	
	DWORD nColor = 0xFFFFFF;
	float nLineWidth = 4.0;
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID				= access.getColumn_Int(6);//pageid
		nElementID			= access.getColumn_Int(5);//id
		strPointList		= access.getColumn_Text(0);//x
		nTemplateGroupId	= access.getColumn_Int(1);//y
		nTemplatePageId		= access.getColumn_Int(2);//width
		strTemplatePageName	= access.getColumn_Text(3);//hight
		nLayer				= access.getColumn_Int(4);//layer

		strParam			= access.getColumn_Text(8);//unitproperty02
		strTemplateName		= access.getColumn_Text(9);//unitproperty03
		nColor				= access.getColumn_Int(10);//unitproperty04
		nLineWidth			= access.getColumn_Double(11);//unitproperty05
		CEqmDrawPolygon* pPolygon = new CEqmDrawPolygon();
		if (pPolygon != NULL)
		{
			pPolygon->SetPageID(nPageID);
			pPolygon->SetID(nElementID);
			pPolygon->SetPointListInString(strPointList);
			pPolygon->SetTemplateGroupId(nTemplateGroupId);
			pPolygon->SetTemplatePageId(nTemplatePageId);
			pPolygon->SetTemplatePageName(strTemplatePageName);
			pPolygon->SetLayer(nLayer);
			pPolygon->SetParam(strParam);
			pPolygon->SetTemplateName(strTemplateName);
			pPolygon->SetEqmType(EQMDRAW_POLYGON);
			pPolygon->SetLineColor_PptsWnd(nColor);
			pPolygon->SetLineWidth_PptsWnd(nLineWidth);
			m_pPage->AddPolygon(pPolygon);
		}
		pPolygon->SetNeedSave(true);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_Polygon()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawPolygon*>& pPolygonList = m_pPage->GetPolygon();
	list<CEqmDrawPolygon*>::const_iterator it = pPolygonList.begin();

	access.BeginTransaction();
	for (;it!=pPolygonList.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneFromPolygon((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				access.InsertOneIntoPolygon(
					(*it)->GetPageID(),(*it)->GetID(),(*it)->GetPointListInString(),(*it)->GetLayer()
					,(*it)->GetTemplateGroupId(), (*it)->GetTemplatePageId(),(*it)->GetTemplatePageNameInString()
					,(*it)->GetParamInString(), (*it)->GetTemplateNameInString()
					,(*it)->GetLineColor_PptsWnd(), (*it)->GetLineWidth_PptsWnd()
					);
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

void CProjectSqlite::GetDetailChartByIDsDiagnosePlus( const int nPageID, const int nElementID, vector<_tagBoolBindInfo>& vecItem )
{
	{
		CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
		ostringstream sqlstream;
		sqlstream << "select * from detail_chart where pageid="<<nPageID<<" and id="<<nElementID<<" and role=0"<<";";
		string sql_ = sqlstream.str();
		char *ex_sql = const_cast<char*>(sql_.c_str());
		int re = access.SqlQuery(ex_sql);
		while(true)
		{
			wstring strBoolPointName;
			wstring strResultPointName;
			wstring strResultPointName2;
			wstring strResultPointName3;
			wstring strTreatmentPointName;
			if(SQLITE_ROW != access.SqlNext())
			{
				break;
			}
			if (access.getColumn_Text(17))
			{
				strBoolPointName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(17));
			}
			if (access.getColumn_Text(18))
			{
				strResultPointName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(18));
			}
			if (access.getColumn_Text(19))
			{
				strTreatmentPointName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(19));
			}
			if (access.getColumn_Text(20))
			{
				strResultPointName2 = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(20));
			}
			if (access.getColumn_Text(21))
			{
				strResultPointName3 = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(21));
			}
			DataPointEntry entry;
			entry = m_pointmap.GetEntry(strBoolPointName);
			_tagBoolBindInfo info;
			info.strBoolPointName = strBoolPointName.c_str();
			info.strResultPointName = strResultPointName.c_str();
			info.strResultPointName2 = strResultPointName2.c_str();
			info.strResultPointName3 = strResultPointName3.c_str();
			info.strTreatmentStatePointName = strTreatmentPointName.c_str();
			info.strBoolPointDescription = entry.GetDescription().c_str();
			vecItem.push_back(info);
		}
		access.SqlFinalize();
	}
	{
		CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
		ostringstream sqlstream;
		sqlstream << "select * from detail_chart where pageid="<<nPageID<<" and id="<<nElementID<<" and role=1"<<";";
		string sql_ = sqlstream.str();
		char *ex_sql = const_cast<char*>(sql_.c_str());
		int re = access.SqlQuery(ex_sql);
		while(true)
		{
			wstring strBoolPointName;
			wstring strResultPointName;
			wstring strTreatmentPointName;
			wstring strItemName;
			wstring strBindPointName;
			int nColor = 0;
			if(SQLITE_ROW != access.SqlNext())
			{
				break;
			}
			if (access.getColumn_Text(17))
			{
				strBoolPointName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(17));
			}
			if (access.getColumn_Text(18))
			{
				strResultPointName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(18));
			}
			if (access.getColumn_Text(19))
			{
				strTreatmentPointName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(19));
			}
			if (access.getColumn_Text(20))
			{
				strItemName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(20));
			}
			if (access.getColumn_Text(21))
			{
				strBindPointName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(21));
			}
			nColor = access.getColumn_Int(2);
			for(size_t i=0;i<vecItem.size();++i)
			{
				if (vecItem[i].strBoolPointName == strBoolPointName.c_str() && vecItem[i].strResultPointName == strResultPointName.c_str() && vecItem[i].strTreatmentStatePointName == strTreatmentPointName.c_str())
				{
					_tagDiagnoseItemPlus item;
					item.strItemName = strItemName.c_str();
					item.strPointName = strBindPointName.c_str();
					item.color = nColor;
					vecItem[i].vecDiagnoseItemPlus.push_back(item);
				}
			}
		}
		access.SqlFinalize();
	}
}

int CProjectSqlite::GetMaxPageID()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_page;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::GetMaxElementID()
{
	int nMax = 0;
	int	ndevices = Get_devices_max_id();
	if (nMax <= ndevices)
	{
		nMax = ndevices;
	}
	int	npipes = Get_pipes_max_id();
	if (nMax <= npipes)
	{
		nMax = npipes;
	}
	int	ntexts = Get_texts_max_id();
	if (nMax <= ntexts)
	{
		nMax = ntexts;
	}
	int	ngraphics = Get_graphics_max_id();
	if (nMax <= ngraphics)
	{
		nMax = ngraphics;
	}
	int	nDashBoard = Get_DashBoard_max_id();
	if (nMax <= nDashBoard)
	{
		nMax = nDashBoard;
	}
	int	nLineChart = Get_Chart_max_id();
	if (nMax <= nLineChart)
	{
		nMax = nLineChart;
	}
	int	nBend = Get_Bend_max_id();
	if (nMax <= nBend)
	{
		nMax = nBend;
	}
	int	nButton = Get_Button_max_id();
	if(nMax <= nButton)
	{
		nMax = nButton;
	}
	int	nTimeSelect = Get_TimeSelect_max_id();
	if (nMax <= nTimeSelect)
	{
		nMax = nTimeSelect;
	}
	int	nTimeBar = Get_TimeBar_max_id();
	if (nMax <= nTimeBar)
	{
		nMax = nTimeBar;
	}
	int	nProgressBar = Get_ProgressBar_max_id();
	if (nMax <= nProgressBar)
	{
		nMax = nProgressBar;
	}
	int nAraeChange = Get_AreaChange_max_id();
	if (nMax <= nAraeChange)
	{
		nMax = nAraeChange;
	}
	int nShellExecute = Get_ShellExecute_max_id();
	if(nMax <= nShellExecute)
	{
		nMax = nShellExecute;
	}
	int nEmbededPage = Get_EmbededPage_max_id();
	if (nMax <= nEmbededPage)
	{
		nMax = nEmbededPage;
	}
	int nAreaChange = Get_AreaChange_max_id();
	if (nMax <= nAreaChange)
	{
		nMax = nAreaChange;
	}
	int nLiquidLevel = Get_LiquidLevel_max_id();
	if (nMax <= nLiquidLevel)
	{
		nMax = nLiquidLevel;
	}
	int nChkBtnPlus = Get_CheckButtonPlus_max_id();
	if (nMax <= nChkBtnPlus)
	{
		nMax = nChkBtnPlus;
	}
	int nScrawl = Get_Scrawl_max_id();
	if (nMax <= nScrawl)
	{
		nMax = nScrawl;
	}
	int nHisTrend = Get_HistoryTrend_max_id();
	if (nMax <= nHisTrend)
	{
		nMax = nHisTrend;
	}
	int nEnergySaveRoi = Get_EnergySaveRoi_max_id();
	if (nMax <= nEnergySaveRoi)
	{
		nMax = nEnergySaveRoi;
	}
	int nMapNavigate = Get_MapNavigate_max_id();
	if (nMax <= nMapNavigate)
	{
		nMax = nMapNavigate;
	}
	int nNaviWind = Get_NavigateWindow_max_id();
	if (nMax <= nNaviWind)
	{
		nMax = nNaviWind;
	}
	int nLevelRuler = Get_LevelRuler_max_id();
	if (nMax <= nLevelRuler)
	{
		nMax = nLevelRuler;
	}
	int nDataAnalyse = Get_DataAnalyse_max_id();
	if (nMax <= nDataAnalyse)
	{
		nMax = nDataAnalyse;
	}
	int nTemperDistri = Get_TemperDistri_max_id();
	if (nMax <= nTemperDistri)
	{
		nMax = nTemperDistri;
	}
	int nDataReport = Get_DataReport_max_id();
	if (nMax <= nDataReport)
	{
		nMax = nDataReport;
	}
	int nPlaneTempeDistr = Get_PlaneTempeDistr_max_id();
	if (nMax <= nPlaneTempeDistr)
	{
		nMax = nPlaneTempeDistr;
	}
	int nDottedLineFrm = Get_DottedLineFrm_max_id();
	if (nMax <= nDottedLineFrm)
	{
		nMax = nDottedLineFrm;
	}
	int nGeneralComponent = Get_GeneralComponent_max_id();
	if (nMax <= nGeneralComponent)
	{
		nMax = nGeneralComponent;
	}
	int nRectangle = Get_Rectangle_max_id();
	if (nMax <= nRectangle)
	{
		nMax = nRectangle;
	}
	int nStraightLine = Get_StraightLine_max_id();
	if (nMax <= nStraightLine)
	{
		nMax = nStraightLine;
	}
	int nPolygon = Get_Polygon_max_id();
	if (nMax <= nPolygon)
	{
		nMax = nPolygon;
	}
	return nMax;
}

int CProjectSqlite::Get_devices_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(elementid) from page_contain_elements";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_pipes_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_line";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_texts_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_text";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_graphics_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_graphic";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_DashBoard_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_dashboard";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_Chart_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_chart";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_Bend_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_bend";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_Button_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_button";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_TimeSelect_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_timeselect";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_TimeBar_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_timebar";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_ProgressBar_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_progressbar";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

void CProjectSqlite::DeleteOnePictureAndInsert(const int nID, PictureInfo info, const bool bDBSelect)
{
	if (bDBSelect)
	{	// 替换打开s3db中的图元
		vector<PictureInfo>::iterator iter = m_vPictureList.begin();
		while (iter != m_vPictureList.end())
		{
			if ((*iter).ID == nID)
			{
				if ((*iter).pBitmap)
				{
					delete (*iter).pBitmap;
					(*iter).pBitmap = NULL;
				}
				if ((*iter).pBlock)
				{
					delete (*iter).pBlock;
					(*iter).pBlock = NULL;
				}
				CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
				access.BeginTransaction();
				access.DeleteOnePicture((*iter).ID);
				access.CommitTransaction();
				iter = m_vPictureList.erase(iter);
				m_vPictureList.insert(iter,info);
				break;
			}
			else
			{
				++iter;
			}
		}
	}
	else
	{	// 替换图源库中的图元
		vector<PictureInfo>::iterator iter = m_vPictureListImage.begin();
		while (iter != m_vPictureListImage.end())
		{
			if ((*iter).ID == nID)
			{
				if ((*iter).pBitmap)
				{
					delete (*iter).pBitmap;
					(*iter).pBitmap = NULL;
				}
				if ((*iter).pBlock)
				{
					delete (*iter).pBlock;
					(*iter).pBlock = NULL;
				}
				CSqliteAcess access(m_strImgDbPath.c_str(), m_bIsEncrypt);
				access.BeginTransaction();
				access.DeleteOnePicture((*iter).ID);
				access.CommitTransaction();
				iter = m_vPictureListImage.erase(iter);
				m_vPictureListImage.insert(iter,info);
				break;
			}
			else
			{
				++iter;
			}
		}
	}
}

wstring CProjectSqlite::ReplacePic(const int nID, vector<ImgReplaceInfo>& vecReplaceInfo)
{
	CString strError;
	ImgReplaceInfo	replaceInfo;
	vecReplaceInfo.clear();
	 const vector<CEqmDrawPage*>& pagelist = m_project.GetPageList();
	 const size_t size = pagelist.size();
	 for (size_t i=0;i<size;++i)
	 {
		 {
			 list<CEqmDrawDevice*>& vecDevice = pagelist[i]->GetDrawDeviceList();
			 list<CEqmDrawDevice*>::iterator iter = vecDevice.begin();
			 while (iter!=vecDevice.end())
			 {
				 if ((*iter)->GetPicID() == nID)
				 {
					 PictureInfo* pInfo = GetPictureInfoByID(nID);
					 if (pInfo)
					 {
						 if (pInfo->pBitmap->GetLastStatus() == Ok)
						 {
							 const int nWidth = pInfo->pBitmap->GetWidth();
							 const int nHeight = pInfo->pBitmap->GetHeight();
							 (*iter)->SetWidth(nWidth);
							 (*iter)->SetHeight(nHeight);
							 (*iter)->SetBitmap(pInfo->pBitmap);

							 replaceInfo.Reset();
							 replaceInfo.nPageId = pagelist[i]->GetID();
							 replaceInfo.strPageName = pagelist[i]->GetPageName();
							 replaceInfo.nElementId = (*iter)->GetID();
							 replaceInfo.nPicId = (*iter)->GetPicID();
							 vecReplaceInfo.push_back(replaceInfo);
						 }
					 }
					 else
					 {
						 CString strMsg;
						 strMsg.Format(L"ReplacePic图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s\n",m_pPage->GetPageName().c_str(), (*iter)->GetID(), (*iter)->GetEqmName().c_str(), (*iter)->GetPhysicalID().c_str());
						 strError +=strMsg;
					 }
				 }
				 ++iter;
			 }
		 }
		 {
			 list<CEqmDrawButton*>& vecBtn = pagelist[i]->GetButtonList();
			 list<CEqmDrawButton*>::iterator iter_btn = vecBtn.begin();
			 while (iter_btn!=vecBtn.end())
			 {
				 int nIDPicComm = 0;
				 int nIDPicDisable = 0;
				 int nIDPicDown = 0;
				 int nIDPicOver = 0;
				 int nChecked = 0;
				 (*iter_btn)->Get5PicID(nIDPicComm, nIDPicDisable, nIDPicDown, nIDPicOver, nChecked);
				 if (nIDPicComm == nID)
				 {
					 PictureInfo* pInfo = GetPictureInfoByID(nID);
					 if (pInfo)
					 {
						 if (pInfo->pBitmap->GetLastStatus() == Ok)
						 {
							 const int nWidth = pInfo->pBitmap->GetWidth();
							 const int nHeight = pInfo->pBitmap->GetHeight();
							 (*iter_btn)->SetWidth(nWidth);
							 (*iter_btn)->SetHeight(nHeight);
							 (*iter_btn)->SetDefaultBitmap(pInfo->pBitmap);

							replaceInfo.Reset();
							replaceInfo.nPageId = pagelist[i]->GetID();
							replaceInfo.strPageName = pagelist[i]->GetPageName();
							replaceInfo.nElementId = (*iter_btn)->GetID();
							int nIDPicComm = 0;
							int	nIDPicDisable = 0;
							int	nIDPicDown = 0;
							int	nIDPicOver = 0;
							int	nIDPicChecked = 0;
							(*iter_btn)->Get5PicID(nIDPicComm, nIDPicDisable, nIDPicDown, nIDPicOver, nIDPicChecked);
							replaceInfo.nPicId = nIDPicComm;
							vecReplaceInfo.push_back(replaceInfo);
						 }
					 }
					 else
					 {
						 CString strMsg;
						 strMsg.Format(L"Replace图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s\n",m_pPage->GetPageName().c_str(), (*iter_btn)->GetID(), L"", (*iter_btn)->GetBindSetPointName());
						  strError +=strMsg;
						  strError +=L"\r\n";
					 }
				 }
				 ++iter_btn;
			 }
		 }
		 //{
			// list<CEqmDrawDiagnoseGraphicPlus*>& vecPlus = pagelist[i]->GetDiagnoseListPlus();
			// list<CEqmDrawDiagnoseGraphicPlus*>::iterator iter_plus = vecPlus.begin();
			// while (iter_plus != vecPlus.end())
			// {
			//	 const int nNormalID = (*iter_plus)->GetNormalPicID();
			//	 if (nID == nNormalID)
			//	 {
			//		 PictureInfo* pInfo = GetPictureInfoByID(nID);
			//		 if (pInfo)
			//		 {
			//			 if (pInfo->pBitmap->GetLastStatus() == Ok)
			//			 {
			//				 const int nWidth = pInfo->pBitmap->GetWidth();
			//				 const int nHeight = pInfo->pBitmap->GetHeight();
			//				 (*iter_plus)->SetWidth(nWidth);
			//				 (*iter_plus)->SetHeight(nHeight);
			//				 (*iter_plus)->SetDefaultBitmap(pInfo->pBitmap);

			//				 replaceInfo.Reset();
			//				 replaceInfo.nPageId = pagelist[i]->GetID();
			//				 replaceInfo.strPageName = pagelist[i]->GetPageName();
			//				 replaceInfo.nElementId = (*iter_plus)->GetID();
			//				 replaceInfo.nPicId = (*iter_plus)->GetNormalPicID();
			//				 vecReplaceInfo.push_back(replaceInfo);
			//			 }
			//		 }
			//		 else
			//		 {
			//			 CString strMsg;
			//			 strMsg.Format(L"Replace图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s\n",m_pPage->GetPageName().c_str(), (*iter_plus)->GetID(), L"", L"");
			//			 strError +=strMsg;
			//			 strError +=L"\r\n";
			//		 }
			//	 }
			//	 ++iter_plus;
			// }
		 //}
	 }

	 return strError.GetString();
}

wstring CProjectSqlite::LoadProjectPages()
{
	CString strErrorAll;
	//LoadPictureLibSqlite(); //load pic lib
	wstring  wstrLoadGroup= LoadGroupPageSqlite();
	strErrorAll += wstrLoadGroup.c_str();
	int id =0;
	int showOrder = 0;

	PAGETYPE pagetype = FULLSCREEN;
	int width = 0;
	int height = 0;
	int xpt = 0;
	int ypt = 0;
	int pagewidth = 0;
	int pageheight = 0;
	int backcolor = DEFAULTBACKGROUNDCOLOR;

	CString strWebAddr;

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_page order by showOrder;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageIndex = 0;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}


		wstring   type;
		if(access.getColumn_Text(10)!=NULL)
		{
			string   type_temp(const_cast<char*>(access.getColumn_Text(10)));
			type = UtilString::ConvertMultiByteToWideChar(type_temp);
		}

		if (type == L"fullscreen")
		{
			pagetype = FULLSCREEN;
		}
		else if (type == _T("floating"))
		{
			pagetype = FLOATING;
		}
		else if (type == _T("webtype"))
		{
			pagetype = WEBTYPE;
		}
		//id
		id = access.getColumn_Int(0);
		//name
		wstring   wstr;
		if(access.getColumn_Text(1)!=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			wstr = UtilString::ConvertMultiByteToWideChar(name_temp);
		}

		//show order
		showOrder = access.getColumn_Int(11);

		int groupID = access.getColumn_Int(5);

		//bg color
		backcolor = access.getColumn_Int(4);
		//width
		pagewidth = access.getColumn_Int(7);
		//height
		pageheight = access.getColumn_Int(8);
		//xposition
		xpt = access.getColumn_Int(12);
		//yposition
		ypt = access.getColumn_Int(13);
		string strShow;
		if (access.getColumn_Text(14))
		{
			strShow = access.getColumn_Text(14);
		}
		if (NULL != access.getColumn_Text(15))
		{
			string	strAddrTmp(const_cast<char*>(access.getColumn_Text(15)));
			strWebAddr.Format(_T("%s"), UtilString::ConvertMultiByteToWideChar(strAddrTmp).c_str());
		}
		int nPageMinRight = access.getColumn_Int(16);
		
		

		int nStartColor = access.getColumn_Int(2);
		int nEndColor = access.getColumn_Int(3);
		int nBKImageID = access.getColumn_Int(4);

		m_pPage = new CEqmDrawPage(id, wstr, m_project.GetProjectID(), showOrder,pagetype,pagewidth,pageheight,xpt,ypt, backcolor);

		m_pPage->m_GroupID = groupID;
		m_pPage->SetStartColor(nStartColor);
		m_pPage->SetEndColor(nEndColor);
		m_pPage->SetBKImageID(nBKImageID);
		m_pPage->SetPageMinRight(nPageMinRight);

		if (strShow=="0")
		{
			m_pPage->SetPageShowTopInObserver(false);
		}
		else
		{	
			m_pPage->SetPageShowTopInObserver(true);
		}
		m_pPage->SetWebAddress(strWebAddr);

		ASSERT(m_pPage);

		if(m_pPage)
		{
			if (nPageIndex == 0)
			{
				m_pPage->m_bIsMainPage = true;
			}
			nPageIndex++;
			m_pPage->m_bNeedSave = false;
			m_project.AddPage(m_pPage);
		}
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

wstring CProjectSqlite::LoadProjectSqlite_AreaChange()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit02 where pageid="<<m_pPage->GetID()<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	int nImageID = 0;
	CString strPointList;
	int nAreaImageID = 0;
	int	nLinkPageID = 0;
	EQMDRAWTYPE eType = EQMDRAW_AREA_CHANGE;

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(6);
		nElementID = access.getColumn_Int(5);
		nX = access.getColumn_Int(0);
		nY = access.getColumn_Int(1);
		nWidth = access.getColumn_Int(2);
		nHeight = access.getColumn_Int(3);
		nLayer = access.getColumn_Int(4);
		nImageID = access.getColumn_Int(7);

		if(access.getColumn_Text(8)!=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(8)));
			strPointList = UtilString::ConvertMultiByteToWideChar(name_temp).c_str();
		}

		nAreaImageID = access.getColumn_Int(9);
		nLinkPageID = access.getColumn_Int(10);
		CEqmDrawAreaChange* pAreaChange = m_pPage->GetAreaChange(nElementID);
		if(pAreaChange == NULL)		//创建
		{
			pAreaChange = new CEqmDrawAreaChange();

			pAreaChange->SetPos(Gdiplus::PointF(nX, nY));
			pAreaChange->SetEqmType(eType);
			pAreaChange->SetPageID(nPageID);
			pAreaChange->SetID(nElementID);
			pAreaChange->SetWidth(nWidth);
			pAreaChange->SetHeight(nHeight);
			pAreaChange->SetLayer(nLayer);
			Image* pBitmap = GetBitmapByIDFromPictureLib(nImageID);
			pAreaChange->SetPicID(nImageID);
			if(pBitmap)
			{
				pAreaChange->SetBitmap(pBitmap);
			}
			pAreaChange->AddAreaInfo(strPointList,nAreaImageID,nLinkPageID);
			m_pPage->AddAreaChange(pAreaChange);
		}
		else			//增加项
		{
			pAreaChange->AddAreaInfo(strPointList,nAreaImageID,nLinkPageID);
		}

		pAreaChange->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_AreaChange()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawAreaChange*>& areachange_list = m_pPage->GetAreaChangeList();
	list<CEqmDrawAreaChange*>::const_iterator it = areachange_list.begin();

	access.BeginTransaction();
	for (;it!=areachange_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneAreaChange((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				const int nImageID = (*it)->GetPicID();

				const int nPageID = (*it)->GetPageID();
				const int nElementID = (*it)->GetID();
				const int nX = (*it)->GetPos().X;
				const int nY = (*it)->GetPos().Y;
				const int nWidth = (*it)->GetWidth();
				const int nHeight = (*it)->GetHeight();
				const int nLayer =  (*it)->GetLayer();

				string  strPointList;
				int		nAreaImageID = 0;	
				int		nLinkPageID = 0;
				vector<_tagAreaInfo>& vec = (*it)->GetAreaInfo();
				size_t nSize = vec.size();
				if (nSize > 0)
				{
					for (size_t i=0;i<nSize;++i)
					{
						strPointList = UtilString::ConvertWideCharToMultiByte(vec[i].strPointList.GetString());
						nAreaImageID = vec[i].BnmID;
						nLinkPageID = vec[i].nPageID;
						if(access.InsertRecordToListUnit02(nPageID,nElementID,nX,nY,nWidth,nHeight,nLayer,nImageID,strPointList.c_str(),nAreaImageID,nLinkPageID)!=0)
						{
							OutputSaveErr(L"InsertRecordToListUnit02",access.GetSqliteErr());
						}
					}
				}
				else
				{
					if(access.InsertRecordToListUnit02(nPageID,nElementID,nX,nY,nWidth,nHeight,nLayer,nImageID,strPointList.c_str(),nAreaImageID,nLinkPageID)!=0)
					{
						OutputSaveErr(L"InsertRecordToListUnit02",access.GetSqliteErr());
					}
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

void CProjectSqlite::GetDetailAreaChangeByIDs( const int nPageID, const int nElementID, vector<_tagAreaInfo>& vecItem )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit03 where pageid="<<nPageID<<" and id="<<nElementID<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		
		string strPointList;
		int nImageID = 0;
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(7))
		{
			strPointList = access.getColumn_Text(7);
		}
		nImageID = access.getColumn_Int(8);

		_tagAreaInfo item;
		item.strPointList = UtilString::ConvertMultiByteToWideChar(strPointList).c_str();
		item.BnmID = nImageID;
		vecItem.push_back(item);
	}
	access.SqlFinalize();
}

void CProjectSqlite::SetIsNeedLoadAllPages( bool bLoadAll )
{
	m_bLoadAllPages = bLoadAll;
}

bool CProjectSqlite::GetIsNeedLoadAllPages()
{
	return m_bLoadAllPages;
}

void CProjectSqlite::SetIsEnglishLanguage( bool bEnglish )
{
	m_bEnglishLanguage = bEnglish;
}

bool CProjectSqlite::GetIsEnglishLanguage()
{
	return m_bEnglishLanguage;
}

bool CProjectSqlite::IsPointExist( const wstring strPointName )
{
	DataPointEntry entry;
	entry = m_pointmap.GetEntry(strPointName);
	if(entry.GetShortName() != strPointName)
		return false;
	return true;
}

bool CProjectSqlite::AddPointEntry( DataPointEntry entry )
{
	m_pointmap.AddEntry(entry);
	return true;
}

vector<_Import_Setting> CProjectSqlite::ReadImportSetting()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit03;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	vector<_Import_Setting> vecSetting;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		_Import_Setting setting;
		if (access.getColumn_Text(7))
		{
			setting.strPrjName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(7)).c_str();
		}
		setting.nID = access.getColumn_Int(5);
		setting.bLoadPoint = access.getColumn_Int(8);
		setting.nPointBaseOffset = access.getColumn_Int(9);
		setting.bLoadImage = access.getColumn_Int(10);
		setting.nImageBaseOffset = access.getColumn_Int(11);
		vecSetting.push_back(setting);
	}
	access.SqlFinalize();
	return vecSetting;
}

bool CProjectSqlite::InsertImportSetting(_Import_Setting Setting )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.DeleteOneImportSetting(Setting.nID);
	string strPrjName = UtilString::ConvertWideCharToMultiByte(Setting.strPrjName.GetString());
	if(access.InsertOneImportSetting(Setting.nID,strPrjName.c_str(),Setting.bLoadPoint,Setting.nPointBaseOffset,Setting.bLoadImage,Setting.nImageBaseOffset)!=0)
	{
		OutputSaveErr(L"InsertOneImportSetting",access.GetSqliteErr());
	}
	access.SqlFinalize();
	return true;
}

bool CProjectSqlite::FindImportSettingByPrjName( const string lpszName,_Import_Setting& setting )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	string strSql = "select * from list_unit03 where unitproperty01='";
	strSql += lpszName;
	strSql += "';";

	char* ex_sql = const_cast<char*>(strSql.c_str());
	int re = access.SqlQuery(ex_sql);
	bool bFind = false;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(7))
		{
			setting.strPrjName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(7)).c_str();
		}
		setting.nID = access.getColumn_Int(5);
		setting.bLoadPoint = access.getColumn_Int(8);
		setting.nPointBaseOffset = access.getColumn_Int(9);
		setting.bLoadImage = access.getColumn_Int(10);
		setting.nImageBaseOffset = access.getColumn_Int(11);
		bFind = true;
	}
	access.SqlFinalize();
	return bFind;
}

int CProjectSqlite::GetMaxImportSettingID()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit03;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nMaxID = 0;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		
		if(access.getColumn_Int(5)>nMaxID)
			nMaxID = access.getColumn_Int(5);		
	}
	access.SqlFinalize();
	return nMaxID+1;
}

bool CProjectSqlite::DeleteBaseImageByID( const int nStartID, const int nEndID)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.DeleteBaseImageByID(nStartID,nEndID);
	access.SqlFinalize();
	return true;
}

bool CProjectSqlite::DeleteBasePointByID( const int nStartID, const int nEndID)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.DeleteBasePointByID(nStartID,nEndID);
	access.SqlFinalize();
	return true;
}

vector<DataPointEntry> CProjectSqlite::GetPointList()
{
	return m_pointmap.GetPointList();
}

CDataPointManager CProjectSqlite::GetPointMap()
{
	return m_pointmap;
}

bool CProjectSqlite::UpdateImportSettingByID( const int nID,const int bLoadPoint, const int bLoadImage )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	if(access.UpdateImportSettingByID(nID,bLoadPoint,bLoadImage)!=0)
	{
		OutputSaveErr(L"UpdateImportSettingByID",access.GetSqliteErr());
	}
	access.SqlFinalize();
	return true;
}

int CProjectSqlite::Get_ShellExecute_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit04";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_AreaChange_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit02";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

wstring CProjectSqlite::LoadProjectSqlite_ShellExecute()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit04 where pageid="<<m_pPage->GetID()<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	string strServerIP;
	string strFileName;
	string strDownloadDirectory;
	string strPicID;
	string strAuto;
	string strFileID;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(6);
		nElementID = access.getColumn_Int(5);
		nX = access.getColumn_Int(0);
		nY = access.getColumn_Int(1);
		nWidth = access.getColumn_Int(2);
		nHeight = access.getColumn_Int(3);
		nLayer = access.getColumn_Int(4);
		if (access.getColumn_Text(7))
		{
			strServerIP = access.getColumn_Text(7);
		}
		if (access.getColumn_Text(8))
		{
			strFileName = access.getColumn_Text(8);
		}
		if (access.getColumn_Text(9))
		{
			strDownloadDirectory = access.getColumn_Text(9);
		}
		if (access.getColumn_Text(10))
		{
			strPicID = access.getColumn_Text(10);
		}
		if (access.getColumn_Text(11))
		{
			strAuto = access.getColumn_Text(11);
		}
		if (access.getColumn_Text(12))
		{
			strFileID = access.getColumn_Text(12);
		}
		if (strPicID.size()==0)
		{
			return strErrorAll.GetString();
		}
		CEqmDrawShellExecute* pShellExecute = new CEqmDrawShellExecute();
		if(pShellExecute)
		{
			pShellExecute->SetPos(Gdiplus::PointF(nX, nY));
			pShellExecute->SetEqmType(EQMDRAW_SHELL_EXECUTE);
			pShellExecute->SetPageID(nPageID);
			pShellExecute->SetID(nElementID);
			pShellExecute->SetWidth(nWidth);
			pShellExecute->SetHeight(nHeight);
			pShellExecute->SetLayer(nLayer);

			pShellExecute->SetServerIP(UtilString::ConvertMultiByteToWideChar(strServerIP).c_str());
			pShellExecute->SetFileName(UtilString::ConvertMultiByteToWideChar(strFileName).c_str());
			pShellExecute->SetDownloadDirectory(UtilString::ConvertMultiByteToWideChar(strDownloadDirectory).c_str());
			pShellExecute->SetFileID(UtilString::ConvertMultiByteToWideChar(strFileID).c_str());
			pShellExecute->SetBKPicID(atoi(strPicID.c_str()));
			pShellExecute->SetAutoDownload(atoi(strAuto.c_str()));

			Image* pBitmap = GetBitmapByIDFromPictureLib(atoi(strPicID.c_str()));
			if(pBitmap)
			{
				pShellExecute->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddShellExecute(pShellExecute);

			PictureInfo* pInfo = GetPictureInfoByID(pShellExecute->GetBKPicID());
			if (!pInfo)
			{
				CString strMsg;
				strMsg.Format(L"ShellExcute图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pShellExecute->GetID(), L"", pShellExecute->GetPhysicalID().c_str());
				strErrorAll+=strMsg;
				strErrorAll+=L"\r\n";

				continue;
			}
			CString strName;
			strName.Format(L"%d",pInfo->ID);
			if(!SaveMemBitmapToFile(pInfo->pBlock, pInfo->nSize, strName.GetString()))
			{
				CString strMsg;
				strMsg.Format(L"ShellExcute图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pShellExecute->GetID(), L"", pShellExecute->GetPhysicalID().c_str());
				strErrorAll+=strMsg;
				strErrorAll+=L"\r\n";

			}

		}
		pShellExecute->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_ShellExecute()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawShellExecute*>& shellexecute_list = m_pPage->GetShellExecuteList();
	list<CEqmDrawShellExecute*>::const_iterator it = shellexecute_list.begin();

	access.BeginTransaction();
	for (;it!=shellexecute_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			string strServerIP = UtilString::ConvertWideCharToMultiByte((*it)->GetServerIP().GetString());
			string strFileName = UtilString::ConvertWideCharToMultiByte((*it)->GetFileName().GetString());
			string strDownloadDirectory = UtilString::ConvertWideCharToMultiByte((*it)->GetDownloadDirectory().GetString());
			string strFileID = UtilString::ConvertWideCharToMultiByte((*it)->GetFileID().GetString());
			char szTemp[10]={0};
			string strPicID = itoa((*it)->GetBKPicID(),szTemp,10);
			memset(szTemp,0,sizeof(szTemp));
			string strAuto = itoa((*it)->GetAutoDownload(),szTemp,10);
			access.DeleteOneShellExecute((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertOneShellExecute((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), strServerIP.c_str(),strFileName.c_str(),strDownloadDirectory.c_str(),strPicID.c_str(),strAuto.c_str(),strFileID.c_str())!=0)
				{
					OutputSaveErr(L"InsertOneShellExecute",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

void CProjectSqlite::GetDiagnoseItemPointNameVec( const int nPageID, vector<wstring>& vecPointName )
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from detail_chart where pageid="<<nPageID<<" and role=0"<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	while(true)
	{
		wstring strBoolPointName;
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(17))
		{
			strBoolPointName = UtilString::ConvertMultiByteToWideChar(access.getColumn_Text(17));
		}
		vecPointName.push_back(strBoolPointName);
	}
	access.SqlFinalize();
}

wstring CProjectSqlite::LoadProjectSqlite_EmbededPage()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit05 where pageid="<<m_pPage->GetID()<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	string strPicID;
	string strPageList;
	string strInterval;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(6);
		nElementID = access.getColumn_Int(5);
		nX = access.getColumn_Int(0);
		nY = access.getColumn_Int(1);
		nWidth = access.getColumn_Int(2);
		nHeight = access.getColumn_Int(3);
		nLayer = access.getColumn_Int(4);
		if (access.getColumn_Text(7))
		{
			strPicID = access.getColumn_Text(7);
		}
		if (access.getColumn_Text(8))
		{
			strPageList = access.getColumn_Text(8);
		}
		if (access.getColumn_Text(9))
		{
			strInterval = access.getColumn_Text(9);
		}
		if (strPicID.size()==0)
		{
			return strErrorAll.GetString();
		}
		CEqmDrawEmbededPage* pEmbededPage = new CEqmDrawEmbededPage();
		if(pEmbededPage)
		{
			pEmbededPage->SetPos(Gdiplus::PointF(nX, nY));
			pEmbededPage->SetEqmType(EQMDRAW_EMBEDED_PAGE);
			pEmbededPage->SetPageID(nPageID);
			pEmbededPage->SetID(nElementID);
			pEmbededPage->SetWidth(nWidth);
			pEmbededPage->SetHeight(nHeight);
			pEmbededPage->SetLayer(nLayer);

			pEmbededPage->SetPicID(atoi(strPicID.c_str()));
			pEmbededPage->SetChangeInterval(atoi(strInterval.c_str()));

			wstring wcsDiagnosePage = UtilString::ConvertMultiByteToWideChar(strPageList);
			if (wcsDiagnosePage.size()>0)
			{
				vector<wstring> vecContent;
				SplitStringByChar(wcsDiagnosePage.c_str(), L'|', vecContent);
				for (size_t i=0;i<vecContent.size();++i)
				{
					if (vecContent[i].size() > 0)
					{
						vector<wstring> vecContentSub;
						SplitStringByChar(vecContent[i].c_str(), L',', vecContentSub);
						const int nID = _wtoi(vecContentSub[0].c_str());
						const CString strPageName = vecContentSub[1].c_str();
						pEmbededPage->InsertPageList(nID,strPageName);
						
					}
				}
			}
			Image* pBitmap = GetBitmapByIDFromPictureLib(atoi(strPicID.c_str()));
			if(pBitmap)
			{
				pEmbededPage->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddEmbededPage(pEmbededPage);

			PictureInfo* pInfo = GetPictureInfoByID(pEmbededPage->GetPicID());
			if (!pInfo)
			{
				CString strMsg;
				strMsg.Format(L"嵌入式页面图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pEmbededPage->GetID(), L"", pEmbededPage->GetPhysicalID().c_str());
				strErrorAll+=strMsg;
				strErrorAll+=L"\r\n";

				continue;
			}
			CString strName;
			strName.Format(L"%d",pInfo->ID);
			if(!SaveMemBitmapToFile(pInfo->pBlock, pInfo->nSize, strName.GetString()))
			{
				CString strMsg;
				strMsg.Format(L"嵌入式页面图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pEmbededPage->GetID(), L"", pEmbededPage->GetPhysicalID().c_str());
				strErrorAll+=strMsg;
				strErrorAll+=L"\r\n";

			}
			
		}
		pEmbededPage->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_EmbededPage()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawEmbededPage*>& embededpage_list = m_pPage->GetEmbededPageList();
	list<CEqmDrawEmbededPage*>::const_iterator it = embededpage_list.begin();

	access.BeginTransaction();
	for (;it!=embededpage_list.end();++it)
	{
		if ((*it)->GetNeedSave())
		{
			char szPicID[10] = {0};
			sprintf_s(szPicID,sizeof(szPicID)/sizeof(char),"%d",(*it)->GetPicID());
			char szInterval[10] = {0};
			sprintf_s(szInterval,sizeof(szInterval)/sizeof(char),"%d",(*it)->GetChangeInterval());
			vector<PageIDName>&	listPage = (*it)->GetPageList();
			string strPageList;
			for (size_t i=0;i<listPage.size();++i)
			{
				char szPageID[10] = {0};
				sprintf_s(szPageID,sizeof(szPageID)/sizeof(char),"%d",listPage[i].nPageID);
				strPageList += szPageID;
				strPageList += ",";
				strPageList += UtilString::ConvertWideCharToMultiByte(listPage[i].strPageName.GetString());
				if (i != listPage.size()-1)
				{
					strPageList += "|";
				}
			}
			access.DeleteOneEmbededPage((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertOneEmbededPage((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), szPicID, strPageList.c_str(), szInterval)!=0)
				{
					OutputSaveErr(L"InsertOneEmbededPage",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_EmbededPage_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit05";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

wstring CProjectSqlite::LoadProjectSqlite_LiquidLevel()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit06 where pageid="<<m_pPage->GetID()<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;

	//string strPicID;
	string strBindPoint;
	string strMaxVal;
	string strMinVal;
	string strHighAlarmLine;
	string strLowAlarmLine;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(6);
		nElementID = access.getColumn_Int(5);
		nX = access.getColumn_Int(0);
		nY = access.getColumn_Int(1);
		nWidth = access.getColumn_Int(2);
		nHeight = access.getColumn_Int(3);
		nLayer = access.getColumn_Int(4);
		//if (access.getColumn_Text(7))
		//{
		//	strPicID = access.getColumn_Text(7);
		//}
		if (access.getColumn_Text(8))
		{
			strBindPoint = access.getColumn_Text(8);
		}
		if (access.getColumn_Text(9))
		{
			strMaxVal = access.getColumn_Text(9);
		}
		if (access.getColumn_Text(10))
		{
			strMinVal = access.getColumn_Text(10);
		}
		if (access.getColumn_Text(11))
		{
			strHighAlarmLine = access.getColumn_Text(11);
		}
		if (access.getColumn_Text(12))
		{
			strLowAlarmLine = access.getColumn_Text(12);
		}
		//if (strPicID.size()==0)
		//{
		//	return;
		//}
		CEqmDrawLiquidLevel* pLiquidLevel = new CEqmDrawLiquidLevel();
		if (pLiquidLevel != NULL)
		{
			pLiquidLevel->SetPos(Gdiplus::PointF(nX, nY));
			pLiquidLevel->SetEqmType(EQMDRAW_LIQUID_LEVEL);
			pLiquidLevel->SetPageID(nPageID);
			pLiquidLevel->SetID(nElementID);
			pLiquidLevel->SetWidth(nWidth);
			pLiquidLevel->SetHeight(nHeight);
			pLiquidLevel->SetLayer(nLayer);

			//pLiquidLevel->SetPicID(atoi(strPicID.c_str()));
			pLiquidLevel->SetBindPoint(strBindPoint.c_str());
			pLiquidLevel->SetMaxValue(atoi(strMaxVal.c_str()));
			pLiquidLevel->SetMinValue(atoi(strMinVal.c_str()));
			pLiquidLevel->SetHighAlarmLine(atof(strHighAlarmLine.c_str()));
			pLiquidLevel->SetLowAlarmLine(atof(strLowAlarmLine.c_str()));
			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\liquidLevel.png");
			if (pBitmap != NULL)
			{
				pLiquidLevel->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddLiquidLevel(pLiquidLevel);

			//PictureInfo* pInfo = GetPictureInfoByID(pLiquidLevel->GetPicID());
			//if (NULL == pInfo)
			//{
			//	CString strMsg;
			//	strMsg.Format(L"液位图元图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pLiquidLevel->GetID(), L"", pLiquidLevel->GetPhysicalID().c_str());
			//	if(m_bShowErrorMessage)
			//		AfxMessageBox(strMsg);
			//}
			//CString strName;
			//strName.Format(L"%d",pInfo->ID);
			//if(!SaveMemBitmapToFile(pInfo->pBlock, pInfo->nSize, strName.GetString()))
			//{
			//	CString strMsg;
			//	strMsg.Format(L"液位图元图片缺失!!!\n页面名称:%s\n图元ID:%d\n图元名称:%s\n绑定点位:%s",m_pPage->GetPageName().c_str(), pLiquidLevel->GetID(), L"", pLiquidLevel->GetPhysicalID().c_str());
			//	if(m_bShowErrorMessage)
			//		AfxMessageBox(strMsg);
			//}
			
		}
		pLiquidLevel->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_LiquidLevel()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawLiquidLevel*>& liquidLevel_list = m_pPage->GetLiquidLevelList();
	list<CEqmDrawLiquidLevel*>::const_iterator it = liquidLevel_list.begin();

	access.BeginTransaction();
	char szPicID[30] = {0};
	string szBindPoint;
	char szMaxValue[30] = {0};
	char szMinValue[30] = {0};
	char szHighAlarmLineValue[30] = {0};
	char szLowAlarmLineValue[30] = {0};
	USES_CONVERSION;
	for (; it!=liquidLevel_list.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			//sprintf_s(szPicID, sizeof(szPicID)/sizeof(char), "%d", (*it)->GetPicID());
			Project::Tools::WideCharToUtf8((*it)->GetBindPoint().GetString(), szBindPoint);
			//sprintf_s(szBindPoint, sizeof(szBindPoint)/sizeof(char), "%s", T2A((*it)->GetBindPoint()));
			sprintf_s(szMaxValue, sizeof(szMaxValue)/sizeof(char), "%d", (*it)->GetMaxValue());
			sprintf_s(szMinValue, sizeof(szMinValue)/sizeof(char), "%d", (*it)->GetMinValue());
			sprintf_s(szHighAlarmLineValue, sizeof(szHighAlarmLineValue)/sizeof(char), "%.2f", (*it)->GetHighAlarmLine());
			sprintf_s(szLowAlarmLineValue, sizeof(szLowAlarmLineValue)/sizeof(char), "%.2f", (*it)->GetLowAlarmLine());

			access.DeleteOneFromLiquidLevel((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertOneIntoLiquidLevel((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), szPicID, szBindPoint.c_str(), szMaxValue, szMinValue, szHighAlarmLineValue, szLowAlarmLineValue)!=0)
				{
					OutputSaveErr(L"InsertOneIntoLiquidLevel",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_LiquidLevel_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit06";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

wstring CProjectSqlite::LoadProjectSqlite_CheckButtonPlus()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit07 where pageid="<<m_pPage->GetID()<<";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	string strCtrlStyle;
	string strTextColor;
	string strTextSize;
	string strSelVal;
	string strUnSelVal;
	string strBindPointName;
	string strTextContent;
	string strGroudId;
	string strExpression;
	string strOptionsDec;
	eButtonStyle eBtnStyle;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(6);
		nElementID = access.getColumn_Int(5);
		nX = access.getColumn_Int(0);
		nY = access.getColumn_Int(1);
		nWidth = access.getColumn_Int(2);
		nHeight = access.getColumn_Int(3);
		nLayer = access.getColumn_Int(4);
		if (access.getColumn_Text(7))
		{
			strCtrlStyle = access.getColumn_Text(7);
		}
		if (access.getColumn_Text(8))
		{
			strTextColor = access.getColumn_Text(8);
		}
		if (access.getColumn_Text(9))
		{
			strTextSize = access.getColumn_Text(9);
		}
		if (access.getColumn_Text(10))
		{
			strSelVal = access.getColumn_Text(10);
		}
		if (access.getColumn_Text(11))
		{
			strUnSelVal = access.getColumn_Text(11);
		}
		if (access.getColumn_Text(12))
		{
			strBindPointName = access.getColumn_Text(12);
		}
		if (access.getColumn_Text(13))
		{
			strTextContent = access.getColumn_Text(13);
		}
		if (access.getColumn_Text(14))
		{
			strGroudId = access.getColumn_Text(14);
		}
		if (access.getColumn_Text(15))
		{
			strExpression = access.getColumn_Text(15);
		}
		if (access.getColumn_Text(16))
		{
			strOptionsDec = access.getColumn_Text(16);
		}

		CEqmDrawButtonPlus* pChkBtnPlus = new CEqmDrawButtonPlus();
		if (pChkBtnPlus != NULL)
		{
			pChkBtnPlus->SetPos(Gdiplus::PointF(nX, nY));
			pChkBtnPlus->SetEqmType(EQMDRAW_CHKBTN_PLUS);
			pChkBtnPlus->SetPageID(nPageID);
			pChkBtnPlus->SetID(nElementID);
			pChkBtnPlus->SetWidth(nWidth);
			pChkBtnPlus->SetHeight(nHeight);
			pChkBtnPlus->SetLayer(nLayer);

			eBtnStyle = (eButtonStyle)atoi(strCtrlStyle.c_str());
			pChkBtnPlus->SetButtonStyle(eBtnStyle);
			pChkBtnPlus->SetTextColor(atoi(strTextColor.c_str()));
			pChkBtnPlus->SetTextSize(atoi(strTextSize.c_str()));
			pChkBtnPlus->SetTextContent(strTextContent.c_str());
			pChkBtnPlus->SetValueForUnCheck(strUnSelVal.c_str());
			pChkBtnPlus->SetValueForCheck(strSelVal.c_str());
			pChkBtnPlus->SetBindPointName(strBindPointName.c_str());
			pChkBtnPlus->SetGroupID(atoi(strGroudId.c_str()));
			pChkBtnPlus->SetExpression(strExpression.c_str());
			pChkBtnPlus->SetOptionsDec(strOptionsDec.c_str());
			CString strImgPath;
			GetSysPath(strImgPath);
			strImgPath += _T("\\Images\\");
			Image* pBitmap = NULL;
			if (E_RADIO_BUTTON == eBtnStyle)
			{
				pChkBtnPlus->SetPathCheckCom(strImgPath + _T("radio_btn_sel_com.png"));
				pChkBtnPlus->SetPathCheckOver(strImgPath + _T("radio_btn_sel_over.png"));
				pChkBtnPlus->SetPathUncheckCom(strImgPath + _T("radio_btn_unsel_com.png"));
				pChkBtnPlus->SetPathUncheckOver(strImgPath + _T("radio_btn_unsel_over.png"));
				pBitmap = Image::FromFile(strImgPath + _T("radio_btn_sel_com.png"));
			}
			else if (E_CHECK_BUTTON == eBtnStyle)
			{
				pChkBtnPlus->SetPathCheckCom(strImgPath + _T("check_btn_sel_com.png"));
				pChkBtnPlus->SetPathCheckOver(strImgPath + _T("check_btn_sel_over.png"));
				pChkBtnPlus->SetPathUncheckCom(strImgPath + _T("check_btn_unsel_com.png"));
				pChkBtnPlus->SetPathUncheckOver(strImgPath + _T("check_btn_unsel_over.png"));
				pBitmap = Image::FromFile(strImgPath + _T("check_btn_sel_com.png"));
			}

			if (pBitmap != NULL)
			{
				pChkBtnPlus->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddCheckButtonPlus(pChkBtnPlus);

			//PictureInfo* pInfo = GetPictureInfoByID(pChkBtnPlus->GetPicID());
			//if (NULL == pInfo)
			//{
			//	continue;
			//}
			//CString strName;
			//strName.Format(L"%d",pInfo->ID);
			//SaveMemBitmapToFile(pInfo->pBlock, pInfo->nSize, strName.GetString());
		
		}
		pChkBtnPlus->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_CheckButtonPlus()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawButtonPlus*>& chkBtnPlus_list = m_pPage->GetCheckButtonPlusList();
	list<CEqmDrawButtonPlus*>::const_iterator it = chkBtnPlus_list.begin();

	access.BeginTransaction();
	char szCtrlStyle[30] = {0};
	char szTextColor[30] = {0};
	char szTextSize[30] = {0};
	char szSelVal[30] = {0};
	char szUnSelVal[30] = {0};
	char szBindPointName[255] = {0};
	char szTextContent[255] = {0};
	char szGroudId[30] = {0};
	char szExpression[255] = {0};
	string strOptionsDec;
	USES_CONVERSION;
	for (; it!=chkBtnPlus_list.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			sprintf_s(szCtrlStyle, sizeof(szCtrlStyle)/sizeof(char), "%d", (*it)->GetButtonStyle());
			sprintf_s(szTextColor, sizeof(szTextColor)/sizeof(char), "%d", (*it)->GetTextColor());
			sprintf_s(szTextSize, sizeof(szTextSize)/sizeof(char), "%d", (*it)->GetTextSize());
			sprintf_s(szSelVal, sizeof(szSelVal)/sizeof(char), "%s", T2A((*it)->GetValueForCheck()));
			sprintf_s(szUnSelVal, sizeof(szUnSelVal)/sizeof(char), "%s", T2A((*it)->GetValueForUnCheck()));
			sprintf_s(szBindPointName, sizeof(szBindPointName)/sizeof(char), "%s", T2A((*it)->GetBindPointName()));
			sprintf_s(szTextContent, sizeof(szTextContent)/sizeof(char), "%s", T2A((*it)->GetTextContent()));
			sprintf_s(szGroudId, sizeof(szGroudId)/sizeof(char), "%d", (*it)->GetGroupID());
			sprintf_s(szExpression, sizeof(szExpression)/sizeof(char), "%s", T2A((*it)->GetExpression()));
			strOptionsDec	= UtilString::ConvertWideCharToMultiByte((*it)->GetOptionsDec().GetString());
			access.DeleteOneFromCheckButtonPlus((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertOneIntoCheckButtonPlus((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(),
					szCtrlStyle, szTextColor, szTextSize, szSelVal, szUnSelVal, szBindPointName, szTextContent, szGroudId, szExpression
					,strOptionsDec.c_str())!=0)
				{
					OutputSaveErr(L"InsertOneIntoCheckButtonPlus",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_CheckButtonPlus_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit07";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

bool CProjectSqlite::ClearProjectConfig()
{
	m_project.ClearPageList();
	m_project.m_groupgage.clear();
	return true; 
}

void CProjectSqlite::SaveInfoIntoSelectDb(void)
{
	const vector<CEqmDrawPage*>& vecPage = m_project.GetPageList();
	vector<CEqmDrawPage*>::const_iterator	iter;

	for (iter=vecPage.begin(); iter!=vecPage.end(); ++iter)	// page循环
	{
		SaveIntoSelectDbImageAndAnimation((*iter)->GetDrawDeviceList());
		//SaveIntoSelectDbPipe((*iter)->GetPagePipeList());
		//SaveIntoSelectDbText((*iter)->GetPageTextList());
		//SaveIntoSelectDbGraphic((*iter)->Get2DGraphicList());
		SaveIntoSelectDbTimeSelect((*iter)->GetTimeSelectList());
		SaveIntoSelectDbTimeBar((*iter)->GetTimeBarList());
		SaveIntoSelectDbProgressBar((*iter)->GetProgressBarList());
		SaveIntoSelectDbDiagnoseGraphic((*iter)->GetDiagnoseList());
		/*SaveIntoSelectDbDiagnoseGraphicPlus((*iter)->GetDiagnoseListPlus());*/
		SaveIntoSelectDbAreaChange((*iter)->GetAreaChangeList());
		SaveIntoSelectDbShellExecute((*iter)->GetShellExecuteList());
		SaveIntoSelectDbEmbededPage((*iter)->GetEmbededPageList());
		SaveIntoSelectDbLiquidLevel((*iter)->GetLiquidLevelList());
		SaveIntoSelectDbButtonPlus((*iter)->GetCheckButtonPlusList());
		SaveIntoSelectDbScrawl((*iter)->GetScrawlList());
		SaveIntoSelectDbBend((*iter)->GetBendList());	//
		SaveIntoSelectDbDashBoard((*iter)->GetDashboardList());	//
		SaveIntoSelectDbLineGraphic((*iter)->GetLineGraphList());	//
		SaveIntoSelectDbBarGraphic((*iter)->GetBarGraphList());
		SaveIntoSelectDbPieGraphic((*iter)->GetPieGraphList());
		SaveIntoSelectDbButton((*iter)->GetButtonList());	//
		SaveIntoSelectDbHistoryTrend((*iter)->GetHistoryTrendList());
		SaveIntoSelectDbEnergySaveRoi((*iter)->GetEnergySaveRoiList());
	}
}

void CProjectSqlite::SaveIntoSelectDbImageAndAnimation(const list<CEqmDrawDevice*>& listDev)
{
	if (listDev.empty())
	{
		return;
	}

	list<CEqmDrawDevice*>::const_iterator iter;
	int	nPicId = 0;

	for (iter=listDev.begin(); iter!=listDev.end(); ++iter)	// page内图片/动画循环
	{
		if ((*iter)->GetNeedSave())
		{
			nPicId = (*iter)->GetPicID();

			if ((*iter)->GetImageMetaType())
			{	// 图片
				SaveIntoSelectDbImage(nPicId);
			}
			else
			{	// 动画
				SaveIntoSelectDbAnimation(nPicId);
			}
		}
	}
}

wstring CProjectSqlite::SaveIntoSelectDbImage(const int nPicId)
{
	CString strErrorAll;
	//PictureInfo* pPicInfDst = GetPictureInfoByID(nPicId, true);
	bool bDstFind = FindDestPictureId(nPicId);
	PictureInfo* pPicInfLib = GetPictureInfoByID(nPicId, false);
	if (pPicInfLib == NULL)
	{
		CString strMsg;
		strMsg.Format(L"SaveIntoSelectDbImage图片缺失!!!\n页面名称:%s\n图片ID:%d",m_pPage->GetPageName().c_str(), nPicId);
		strErrorAll+=strMsg;
		strErrorAll+=L"\r\n";

	}
	USES_CONVERSION;
	if (!bDstFind && NULL != pPicInfLib)
	{	// 打开s3db的表lib_image中没有，而图元s3db中有，则需要插入到打开的s3db中
		//PictureInfo* pPicInf = new PictureInfo();
		//ASSERT(pPicInf != NULL);
		//if (NULL == pPicInf)
		//{
		//	return;
		//}
		//pPicInf->ID = pPicInfLib->ID;

		//wcscpy_s(pPicInf->szName, MAX_PATH, pPicInfLib->szName);
		//pPicInf->nSize = pPicInfLib->nSize;
		//pPicInf->etype_p = pPicInfLib->etype_p;
		//pPicInf->etype_equip = pPicInfLib->etype_equip;
		//pPicInf->ID = pPicInfLib->ID;
		//pPicInf->remark = pPicInfLib->remark;
		//pPicInf->issystemimage = pPicInfLib->issystemimage;

		//int nLen = pPicInfLib->nSize;
		//pPicInf->pBlock = new char[nLen];
		//memset(pPicInf->pBlock, 0, nLen);
		//strcpy_s(pPicInf->pBlock, nLen, pPicInfLib->pBlock);

		//HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, nLen);
		//IStream* pStmBmp = NULL;
		//CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp);
		//if (pStmBmp == NULL) 
		//{
		//	GlobalFree(hMemBmp);
		//}
		//BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
		//Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromStream(pStmBmp);
		//GlobalFree(hMemBmp);
		//GlobalUnlock(hMemBmp);

		//pPicInf->pBitmap = pBitmap;
		//memcpy_s(pPicInf->pBitmap, nLen, pPicInfLib->pBitmap, nLen);
		// end --
		//m_vPictureList.push_back(*pPicInf);

		//add by golding
		//ASSERT(pPicInfLib->nSize>0);
		//if(pPicInfLib->nSize<=0)
		//{
		//	CString strTemp;
		//	strTemp.Format(L"Picture(%d) lib wrong for size = 0", pPicInfLib->ID);
		//	CWarningDlg warnDlg;
		//	warnDlg.InitWarn(L"错误", strTemp, TRUE);
		//	warnDlg.DoModal();
		//}


		AddIntoDestPictureId(nPicId);

		int nResult = InsertPictureData(nPicId, T2A(pPicInfLib->szName), Project::Tools::WideCharToAnsi(pPicInfLib->etype_equip.c_str()).c_str(),pPicInfLib->pBlock, pPicInfLib->nSize, true);
		//if(nResult!=0)
		//{
		//	CString strTemp;
		//	strTemp.Format(L"Insert Picture(%d) Data into DBFile failed", pPicInfLib->ID);
		//	CWarningDlg warnDlg;
		//	warnDlg.InitWarn(L"错误", strTemp, TRUE);
		//	warnDlg.DoModal();
		//}
	}
	else if (bDstFind && NULL != pPicInfLib)
	{	// 打开s3db的表lib_image中有，而图元s3db中也有
		PictureInfo* pPicInfDst = GetPictureInfoByID(nPicId, true);
		if (NULL == pPicInfDst)
		{
			CString strMsg;
			strMsg.Format(L"SaveIntoSelectDbImage图片缺失!!!\n页面名称:%s\n图片ID:%d",m_pPage->GetPageName().c_str(), nPicId);
			strErrorAll +=strMsg;
			strErrorAll +=L"\r\n";
		}
		int nCmpRet = wcscmp(pPicInfDst->szName, pPicInfLib->szName);
		if (nCmpRet != 0 && pPicInfDst->nSize != pPicInfLib->nSize)// 但两张图片是不同的
		{
			int nNewId = GetMaxLibImageNum(true) + 1;

			CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
			access.BeginTransaction();
			access.DeleteOnePicture(nPicId);	// delete表lib_image中对应数据
			if(access.UpdateRecordInContainElements(nPicId, nNewId)!=0)// update表page_contain_elements中的pictureid
			{
				OutputSaveErr(L"UpdateRecordInContainElements",access.GetSqliteErr());
			}
			access.CommitTransaction();

			InsertPictureData(nNewId, T2A(pPicInfDst->szName), Project::Tools::WideCharToAnsi(pPicInfDst->etype_equip.c_str()).c_str(),pPicInfDst->pBlock, pPicInfDst->nSize, true);

			vector<PictureInfo>::iterator	iter;
			for (iter=m_vPictureList.begin(); iter!=m_vPictureList.end(); ++iter)
			{
				if (iter->ID == nPicId)
				{
					iter->ID = nNewId;
				}
			}
		}
	}

	return strErrorAll.GetString();
}

void CProjectSqlite::SaveIntoSelectDbAnimation(const int nPicId)
{
	bool bAnimFind = FindDestAnimationId(nPicId);
	AnimationInfo* pAnimInfLib = (AnimationInfo*)GetAnimationByID(nPicId, false);
	if (!bAnimFind && NULL != pAnimInfLib)
	{	// 打开s3db的表lib_animation中没有，而图元s3db中有，则需要插入到打开的s3db中
		//AnimationInfo* pAni = new AnimationInfo();
		//ASSERT(pAni != NULL);
		//if (NULL == pAni)
		//{
		//	return;
		//}
		//pAni->ID = pAnimInfLib->ID;
		//m_vAnimationList.push_back(*pAni);

		AddIntoDestAnimationId(nPicId);

		SetAnimationData(pAnimInfLib->ID, pAnimInfLib, true);
	}
	else if (bAnimFind && NULL != pAnimInfLib)
	{	// 打开s3db的表lib_animation中有，而图元s3db中也有
		AnimationInfo* pAnimInfDst = (AnimationInfo*)GetAnimationByID(nPicId, true);
		if (pAnimInfDst->szName != pAnimInfLib->szName)// 但两者图片的名称不同
		{
			//int nNewId = GetMaxLibAnimationNum(true) + 1;

			//CSqliteAcess access(m_strFilePath.c_str());
			//access.BeginTransaction();
			//access.DeleteOneAnimation(nPicId);	// delete表lib_animation中对应数据
			//access.UpdateRecordInContainElements(nPicId, nNewId);// update表page_contain_elements中的pictureid
			//access.CommitTransaction();

			//pAnimInfDst->ID = nNewId;
			//SetAnimationData(pAnimInfDst);
		}
	}
}

wstring CProjectSqlite::LoadProjectSqlite_Scrawl()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit08 where pageid=" << m_pPage->GetID() << ";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	size_t nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	string	strShowWords;
	vector<CurveColumn>	vecCurveCol;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(6);
		nElementID = access.getColumn_Int(5);
		nX = access.getColumn_Int(0);
		nY = access.getColumn_Int(1);
		nWidth = access.getColumn_Int(2);
		nHeight = access.getColumn_Int(3);
		nLayer = access.getColumn_Int(4);
		if (access.getColumn_Text(7) != NULL)
		{
			strShowWords = access.getColumn_Text(7);
		}
		vecCurveCol = GetScrawlCurveColumnInfo(nElementID);

		CEqmDrawScrawl* pScrawl = new CEqmDrawScrawl();
		if (pScrawl != NULL)
		{
			pScrawl->SetPos(Gdiplus::PointF(nX, nY));
			pScrawl->SetEqmType(EQMDRAW_NOTICE_BOARD);
			pScrawl->SetPageID(nPageID);
			pScrawl->SetID(nElementID);
			pScrawl->SetWidth(nWidth);
			pScrawl->SetHeight(nHeight);
			pScrawl->SetLayer(nLayer);
			pScrawl->SetShowWords(strShowWords.c_str());
			pScrawl->SetCurveColumn(vecCurveCol);

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\scrawl.png");
			if (pBitmap != NULL)
			{
				pScrawl->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddScrawlBoard(pScrawl);

		

		}
		pScrawl->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_Scrawl()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawScrawl*>& scrawl_list = m_pPage->GetScrawlList();
	list<CEqmDrawScrawl*>::const_iterator it = scrawl_list.begin();
	vector<CurveColumn> vecCurCol;
	vector<CurveColumn>::const_iterator	itCurCol;
	int	nId = 0;
	char szShowWords[MAX_PATH] = {0};
	char szPointName[MAX_PATH] = {0};
	char szRemarks[MAX_PATH] = {0};

	USES_CONVERSION;

	access.BeginTransaction();
	for (; it!=scrawl_list.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			nId = (*it)->GetID();
			access.DeleteOneFromScrawl(nId);

			sprintf_s(szShowWords, sizeof(szShowWords)/sizeof(char), "%s", T2A((*it)->GetShowWords()));
			if(access.InsertOneIntoScrawl((*it)->GetPageID(), nId, (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(),
				szShowWords)!=0)
			{
				OutputSaveErr(L"InsertOneIntoScrawl",access.GetSqliteErr());
			}

			vecCurCol = (*it)->GetCurveColumn();
			access.DeleteOneFromCurveColumn(nId);
			if((*it)->GetDeleteFlag() == false)
			{
				for (itCurCol=vecCurCol.begin(); itCurCol!=vecCurCol.end(); ++itCurCol)
				{
					sprintf_s(szPointName, sizeof(szPointName)/sizeof(char), "%s", T2A(itCurCol->strPointName));
					sprintf_s(szRemarks, sizeof(szRemarks)/sizeof(char), "%s", T2A(itCurCol->strRemarks));
					if(access.InsertOneIntoCurveColumn(nId, szPointName, itCurCol->color, itCurCol->nMax, itCurCol->nMin, szRemarks, itCurCol->scrawlType)!=0)
					{
						OutputSaveErr(L"InsertOneIntoCurveColumn",access.GetSqliteErr());
					}
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_Scrawl_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit08";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

vector<CurveColumn> CProjectSqlite::GetScrawlCurveColumnInfo(const size_t nElementId) const
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit09";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	vector<CurveColumn> vecCurveCol;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (nElementId == access.getColumn_Int(5))
		{
			CurveColumn curCol;
			if (access.getColumn_Text(7) != NULL)
			{
				curCol.strPointName = access.getColumn_Text(7);
			}
			curCol.color = access.getColumn_Int(8);
			curCol.nMax = access.getColumn_Int(9);
			curCol.nMin = access.getColumn_Int(10);
			if (access.getColumn_Text(11) != NULL)
			{
				curCol.strRemarks = access.getColumn_Text(11);
			}
			curCol.scrawlType = (ScrawlListType)access.getColumn_Int(12);
			
			vecCurveCol.push_back(curCol);
		}
	}
	access.SqlFinalize();

	return vecCurveCol;
}

vector<HistoryTrendInfo> CProjectSqlite::GetHistoryTrendVectorInfo(const size_t nElementId) const
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit11";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	vector<HistoryTrendInfo>	vecHisTrendInf;
	CString	strStartTime;
	CString	strEndTime;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (nElementId == access.getColumn_Int(5))
		{
			HistoryTrendInfo hisTrendInf;

			hisTrendInf.nCount = access.getColumn_Int(7);

			if (access.getColumn_Text(8) != NULL)
			{
				strStartTime = access.getColumn_Text(8);
				(hisTrendInf.dateTimeStart).ParseDateTime(strStartTime);
			}

			if (access.getColumn_Text(9) != NULL)
			{
				strEndTime = access.getColumn_Text(9);
				(hisTrendInf.dateTimeEnd).ParseDateTime(strEndTime);
			}

			hisTrendInf.bIsCalcRange = access.getColumn_Int(10);

			if (access.getColumn_Text(11) != NULL)
			{
				hisTrendInf.strBindPointName = access.getColumn_Text(11);
			}

			if (access.getColumn_Text(12) != NULL)
			{
				hisTrendInf.strRemarks = access.getColumn_Text(12);
			}

			hisTrendInf.colorImage = access.getColumn_Int(13);
			hisTrendInf.nUpdateCycle = access.getColumn_Int(14);
			hisTrendInf.nLineWidth = access.getColumn_Int(15);
			hisTrendInf.fScale = access.getColumn_Double(16);

			vecHisTrendInf.push_back(hisTrendInf);
		}
	}
	access.SqlFinalize();

	return vecHisTrendInf;
}

vector<CMapPageInfo> CProjectSqlite::GetMapNavigateVectorInfo(const size_t nElementId) const
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit14";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	vector<CMapPageInfo>	vecMapPageInf;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (nElementId == access.getColumn_Int(5))
		{
			CMapPageInfo	mapPageInf;

			mapPageInf.SetLongitudeValue(1, access.getColumn_Double(7));
			mapPageInf.SetLongitudeValue(2, access.getColumn_Double(8));
			mapPageInf.SetLatitudeValue(1, access.getColumn_Double(9));
			mapPageInf.SetLatitudeValue(2, access.getColumn_Double(10));
			mapPageInf.SetPageId(access.getColumn_Int(11));

			vecMapPageInf.push_back(mapPageInf);
		}
	}
	access.SqlFinalize();

	return vecMapPageInf;
}

vector<PageInfo> CProjectSqlite::GetNavigateWindowVectorInfo(const size_t nElementId) const
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit16";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	vector<PageInfo>	vecPageInfo;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (nElementId == access.getColumn_Int(5))
		{
			PageInfo	pageInfo;
			if (access.getColumn_Text(7) != NULL)
			{
				pageInfo.strPageDesc = access.getColumn_Text(7);
			}
			if (access.getColumn_Text(8) != NULL)
			{
				pageInfo.strPageName = access.getColumn_Text(8);
			}
			pageInfo.nPageId = access.getColumn_Int(9);

			vecPageInfo.push_back(pageInfo);
		}
	}
	access.SqlFinalize();

	return vecPageInfo;
}

vector<LabelInfo> CProjectSqlite::GetLevelRulerVectorInfo(const size_t nElementId) const
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit18 where unitproperty01 = 0";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	vector<LabelInfo>	vecLabelInfo;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (nElementId == access.getColumn_Int(5) && 0 == access.getColumn_Int(7))
		{
			LabelInfo	labelInf;
			if (access.getColumn_Text(8) != NULL)
			{
				labelInf.strName = access.getColumn_Text(8);
			}
			labelInf.dValue = access.getColumn_Double(9);
			if (access.getColumn_Text(10) != NULL)
			{
				labelInf.strBindPtName = access.getColumn_Text(10);
			}
			labelInf.nLinkPageId = access.getColumn_Int(11);
			if (access.getColumn_Text(12) != NULL)
			{
				labelInf.strLinkPageName = access.getColumn_Text(12);
			}
			if (0 == access.getColumn_Int(13))
			{
				labelInf.bIsUp = false;
			}
			else
			{
				labelInf.bIsUp = true;
			}

			vecLabelInfo.push_back(labelInf);
		}
	}
	access.SqlFinalize();

	return vecLabelInfo;
}

vector<SectionInfo> CProjectSqlite::GetLevelRulerSectionVectorInfo(const size_t nElementId) const
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit18 where unitproperty01 = 1";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	vector<SectionInfo>	vecSectionInfo;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (nElementId == access.getColumn_Int(5) && 1 == access.getColumn_Int(7))
		{
			SectionInfo	secInf;
			if (access.getColumn_Text(8) != NULL)
			{
				secInf.strName = access.getColumn_Text(8);
			}
			secInf.fMax = access.getColumn_Double(9);
			secInf.fMin = access.getColumn_Double(10);

			vecSectionInfo.push_back(secInf);
		}
	}
	access.SqlFinalize();

	return vecSectionInfo;
}

vector<DaPointInfo> CProjectSqlite::GetDataAnalyseVectorInfo(const size_t nElementId) const
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit20";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	vector<DaPointInfo>	vecPtInfo;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (nElementId == access.getColumn_Int(5) && EQMDRAW_DATA_ANALYSE == access.getColumn_Int(7))
		{
			DaPointInfo	ptInf;
			if (access.getColumn_Text(8) != NULL)
			{
				ptInf.strPointName = access.getColumn_Text(8);
			}
			if (access.getColumn_Text(9) != NULL)
			{
				ptInf.strPointRemarks = access.getColumn_Text(9);
			}

			vecPtInfo.push_back(ptInf);
		}
	}
	access.SqlFinalize();

	return vecPtInfo;
}

PictureInfo* CProjectSqlite::GetAnimationPicInfoById(const int nId, const bool bIsSel, wstring & strErrorInfo)
{
	AnimationInfo* pAnim = NULL;
	PictureInfo* pPicInf = NULL;
	
	CString strErrorAll;
	pAnim = (AnimationInfo*)GetAnimationByID(nId, bIsSel);
	if (pAnim != NULL)
	{
		vector<int> vecId = pAnim->vecPicIDList;
		pPicInf = GetPictureInfoByIDOfAnimation(vecId.at(0), bIsSel);
		if (pPicInf == NULL)
		{
			CString strMsg;
			strMsg.Format(L"动画图片缺失!!!\n页面名称:%s\n图片ID:%d",m_pPage->GetPageName().c_str(), vecId.at(0));
			strErrorAll+=strMsg;
			strErrorAll+=L"\r\n";

		}
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"动画图片缺失!!!\n页面名称:%s\n图片ID:%d",m_pPage->GetPageName().c_str(), nId);
		strErrorAll+=strMsg;
		strErrorAll+=L"\r\n";

	}

	strErrorInfo = strErrorAll.GetString();
	return pPicInf;
}

void CProjectSqlite::SaveIntoSelectDbPipe(const list<CEqmDrawPipe*>& lstPipe)
{
	if (lstPipe.empty())
	{
		return;
	}

	list<CEqmDrawPipe*>::const_iterator	iter;
	for (iter=lstPipe.begin(); iter!=lstPipe.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbText(const list<CEqmDrawText*>& lstText)
{
	if (lstText.empty())
	{
		return;
	}

	list<CEqmDrawText*>::const_iterator	iter;
	for (iter=lstText.begin(); iter!=lstText.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbGraphic(const list<CEqmDrawGraphic*>& lstGraph)
{
	if (lstGraph.empty())
	{
		return;
	}

	list<CEqmDrawGraphic*>::const_iterator	iter;
	for (iter=lstGraph.begin(); iter!=lstGraph.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbTimeSelect(const list<CEqmDrawTimeSelect*>& lstTmSel)
{
	if (lstTmSel.empty())
	{
		return;
	}

	list<CEqmDrawTimeSelect*>::const_iterator	iter;
	for (iter=lstTmSel.begin(); iter!=lstTmSel.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexTimeSelect);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbTimeBar(const list<CEqmDrawTimeBar*>& lstTmBar)
{
	if (lstTmBar.empty())
	{
		return;
	}

	list<CEqmDrawTimeBar*>::const_iterator	iter;
	for (iter=lstTmBar.begin(); iter!=lstTmBar.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexTimeBar);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbProgressBar(const list<CEqmDrawProgressBar*>& lstProgBar)
{
	if (lstProgBar.empty())
	{
		return;
	}

	list<CEqmDrawProgressBar*>::const_iterator	iter;
	for (iter=lstProgBar.begin(); iter!=lstProgBar.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexProgressBar);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbDiagnoseGraphic(const list<CEqmDrawDiagnoseGraphic*>& lstDiagGraph)
{
	if (lstDiagGraph.empty())
	{
		return;
	}

	list<CEqmDrawDiagnoseGraphic*>::const_iterator	iter;
	for (iter=lstDiagGraph.begin(); iter!=lstDiagGraph.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexDiagnoseChart);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbDiagnoseGraphicPlus(const list<CEqmDrawDiagnoseGraphicPlus*>& lstDiagGraphPlus)
{
	if (lstDiagGraphPlus.empty())
	{
		return;
	}

	list<CEqmDrawDiagnoseGraphicPlus*>::const_iterator	iter;
	for (iter=lstDiagGraphPlus.begin(); iter!=lstDiagGraphPlus.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			int nPicId = (*iter)->GetNormalPicID();
			if (nPicId != -1)
			{
				SaveIntoSelectDbImage(nPicId);
			}

			int nAniOrPicId = (*iter)->GetAnimationOrPicID();
			if (nAniOrPicId != -1)
			{
				SaveIntoSelectDbImage(nAniOrPicId);
			}
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbAreaChange(const list<CEqmDrawAreaChange*>& lstAreaChange)
{
	if (lstAreaChange.empty())
	{
		return;
	}

	list<CEqmDrawAreaChange*>::const_iterator	iter;
	for (iter=lstAreaChange.begin(); iter!=lstAreaChange.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage((*iter)->GetPicID());
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbShellExecute(const list<CEqmDrawShellExecute*>& lstShellExecute)
{
	if (lstShellExecute.empty())
	{
		return;
	}

	list<CEqmDrawShellExecute*>::const_iterator	iter;
	for (iter=lstShellExecute.begin(); iter!=lstShellExecute.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			int nBackPicId = (*iter)->GetBKPicID();
			if (nBackPicId != -1)
			{
				SaveIntoSelectDbImage(nBackPicId);
			}
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbEmbededPage(const list<CEqmDrawEmbededPage*>& lstEmbedPage)
{
	if (lstEmbedPage.empty())
	{
		return;
	}

	list<CEqmDrawEmbededPage*>::const_iterator	iter;
	for (iter=lstEmbedPage.begin(); iter!=lstEmbedPage.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			int nPicId = (*iter)->GetPicID();
			if (nPicId != -1)
			{
				SaveIntoSelectDbImage(nPicId);
			}
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbLiquidLevel(const list<CEqmDrawLiquidLevel*>& lstLiquLev)
{
	if (lstLiquLev.empty())
	{
		return;
	}

	list<CEqmDrawLiquidLevel*>::const_iterator	iter;
	for (iter=lstLiquLev.begin(); iter!=lstLiquLev.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexLiquidLevel);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbButtonPlus(const list<CEqmDrawButtonPlus*>& lstBtnPlus)
{
	if (lstBtnPlus.empty())
	{
		return;
	}

	list<CEqmDrawButtonPlus*>::const_iterator	iter;
	for (iter=lstBtnPlus.begin(); iter!=lstBtnPlus.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexCheckButton1);
			SaveIntoSelectDbImage(g_nPicIndexCheckButton2);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbScrawl(const list<CEqmDrawScrawl*>& lstScrawl)
{
	if (lstScrawl.empty())
	{
		return;
	}

	list<CEqmDrawScrawl*>::const_iterator	iter;
	for (iter=lstScrawl.begin(); iter!=lstScrawl.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexScrawl);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbBend(const list<CEqmDrawBend*>& lstBend)	//
{
	if (lstBend.empty())
	{
		return;
	}

	list<CEqmDrawBend*>::const_iterator	iter;
	for (iter=lstBend.begin(); iter!=lstBend.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexBend);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbDashBoard(const list<CEqmDrawDashBoard*>& lstDashBoard)	//
{
	if (lstDashBoard.empty())
	{
		return;
	}

	list<CEqmDrawDashBoard*>::const_iterator	iter;
	for (iter=lstDashBoard.begin(); iter!=lstDashBoard.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexDashBoard1);
			SaveIntoSelectDbImage(g_nPicIndexDashBoard2);
			SaveIntoSelectDbImage(g_nPicIndexPointer1);
			SaveIntoSelectDbImage(g_nPicIndexPointer2);
			SaveIntoSelectDbImage(g_nPicIndexPointer3);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbLineGraphic(const list<CEqmDrawLineGraphic*>& lstLineGraph)	//
{
	if (lstLineGraph.empty())
	{
		return;
	}

	list<CEqmDrawLineGraphic*>::const_iterator	iter;
	for (iter=lstLineGraph.begin(); iter!=lstLineGraph.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexChartLine);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbBarGraphic(const list<CEqmDrawBarGraphic*>& lstBarGraph)
{
	if (lstBarGraph.empty())
	{
		return;
	}

	list<CEqmDrawBarGraphic*>::const_iterator	iter;
	for (iter=lstBarGraph.begin(); iter!=lstBarGraph.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexChartBar);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbPieGraphic(const list<CEqmDrawPieGraphic*>& lstPieGraph)
{
	if (lstPieGraph.empty())
	{
		return;
	}

	list<CEqmDrawPieGraphic*>::const_iterator	iter;
	for (iter=lstPieGraph.begin(); iter!=lstPieGraph.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexChartPie);
			break;
		}
	}
}

void CProjectSqlite::SaveIntoSelectDbButton(const list<CEqmDrawButton*>& lstBtn)	//
{
	if (lstBtn.empty())
	{
		return;
	}

	list<CEqmDrawButton*>::const_iterator	iterBtn;
	int	m_nIDPicComm = -1;
	int	m_nIDPicOver = -1;
	int	m_nIDPicDown = -1;
	int	m_nIDPicDisable = -1;
	int	m_nIDPicChecked = -1;

	for (iterBtn=lstBtn.begin(); iterBtn!=lstBtn.end(); ++iterBtn)	// 按钮循环
	{
		if ((*iterBtn)->GetNeedSave())
		{
			(*iterBtn)->Get5PicID(m_nIDPicComm, m_nIDPicDisable, m_nIDPicDown, m_nIDPicOver, m_nIDPicChecked);
			if (m_nIDPicComm != -1)
			{
				SaveIntoSelectDbImage(m_nIDPicComm);
			}
			if (m_nIDPicOver != -1)
			{
				SaveIntoSelectDbImage(m_nIDPicOver);
			}
			if (m_nIDPicDown != -1)
			{
				SaveIntoSelectDbImage(m_nIDPicDown);
			}
			if (m_nIDPicDisable != -1)
			{
				SaveIntoSelectDbImage(m_nIDPicDisable);
			}
			if (m_nIDPicChecked != -1)
			{
				SaveIntoSelectDbImage(m_nIDPicChecked);
			}
		}
	}
}

wstring CProjectSqlite::SetAnimationData(const int nAnimId, const AnimationInfo* pAnimInf, const bool bDbSelect)
{
	USES_CONVERSION;

	CString strErrorAll;


	// 重编该动画中各图编号
	vector<PictureInfo>::iterator	iterAniPic;
	int nMaxAniPicId = GetMaxAnimationPictureNum(true) + 1;


	AnimationInfo anim;
	anim.ID = pAnimInf->ID;
	anim.szName = pAnimInf->szName;
	anim.vecPicture.clear();
	anim.vecpBitmap.clear();
	anim.vecPicIDList.clear();

	// insert into lib_image_animation
	vector<PictureInfo>	vecPic = pAnimInf->vecPicture;
	vector<PictureInfo>::const_iterator	iterPic;
	for (iterPic=vecPic.begin(); iterPic!=vecPic.end(); ++iterPic)
	{
		PictureInfo picInf = *iterPic;
		picInf.ID = nMaxAniPicId;

		anim.vecPicIDList.push_back(nMaxAniPicId);
		anim.vecPicture.push_back(picInf);
		++nMaxAniPicId;

		wstring	strPath;
		if (bDbSelect)
		{
			strPath = m_strFilePath;
		}
		else
		{
			strPath = m_strImgDbPath;
		}

		bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
		CSqliteAcess access(strPath.c_str(), bEncry);
		access.BeginTransaction();
		access.DeleteOnePictureForAnimation(picInf.ID);
		access.CommitTransaction();

		InsertPictureAnimationData(picInf.ID, T2A(picInf.szName), Project::Tools::WideCharToAnsi(picInf.etype_equip.c_str()).c_str(),picInf.pBlock, picInf.nSize, bDbSelect);

		// 插入到内存结构m_vPictureforAnimationList
		AddIntoAnimationImage(picInf.ID, &picInf, bDbSelect);
	}


	// insert into lib_animation

	// 将vecId组合成字符串
	CString	strList;
	CString strTemp;
	vector<int>::const_iterator iterId;
	for (iterId=(anim.vecPicIDList).begin(); iterId!=(anim.vecPicIDList).end(); ++iterId)
	{
		strTemp.Format(_T("%d,"), *iterId);
		strList += strTemp;
	}
	strList.TrimRight(_T(","));

	int nLen = strList.GetLength() + 1;
	char* pList = new char[nLen];
	memset(pList, 0, nLen);
	strcpy_s(pList, nLen, T2A(strList.GetBuffer()));
	strList.ReleaseBuffer();

	InsertRecordToAnimationList(nAnimId, T2A((anim.szName).c_str()), pList, bDbSelect);
	SAFE_DELETE_ARRAY(pList);

	// 插入到内存结构m_vAnimationList
	AddIntoAnimationList(nAnimId, &anim, bDbSelect);


	return strErrorAll.GetString();
}

bool CProjectSqlite::SetAnimationDataEx(const int nAnimId, const AnimationInfo* pAnimInf, const bool bDbSelect)
{
	bool bRet = true;

	USES_CONVERSION;


	// 重编该动画中各图编号
	vector<PictureInfo>::iterator	iterAniPic;
	int nMaxAniId = GetMaxAnimationNum(true) + 1;
	int nMaxAniPicId = GetMaxAnimationPictureNum(true) + 1;


	AnimationInfo anim;
	anim.ID = nMaxAniId;
	anim.szName = pAnimInf->szName;
	anim.vecPicture.clear();
	anim.vecpBitmap.clear();	// 不用
	anim.vecPicIDList.clear();

	// insert into lib_image_animation
	vector<int> vecId = pAnimInf->vecPicIDList;
	vector<int>::const_iterator	iterId;

	vector<PictureInfo> vecPic = pAnimInf->vecPicture;
	vector<PictureInfo>::const_iterator	iterPic;
	for (iterPic=vecPic.begin(); iterPic!=vecPic.end(); ++iterPic)
	{
		PictureInfo picInf;
		picInf = *iterPic;

		picInf.ID = nMaxAniPicId;
		anim.vecPicture.push_back(picInf);
		anim.vecPicIDList.push_back(nMaxAniPicId);
		++nMaxAniPicId;

		wstring	strPath;
		if (bDbSelect)
		{
			strPath = m_strFilePath;
		}
		else
		{
			strPath = m_strImgDbPath;
		}

		bool bEncry = GetEncryptTypeWithImgLib(bDbSelect);
		CSqliteAcess access(strPath.c_str(), bEncry);
		access.BeginTransaction();
		access.DeleteOnePictureForAnimation(picInf.ID);
		access.CommitTransaction();

		if (0 != InsertPictureAnimationData(picInf.ID, T2A(picInf.szName), Project::Tools::WideCharToAnsi(picInf.etype_equip.c_str()).c_str(),picInf.pBlock, picInf.nSize, bDbSelect))
		{
			bRet = false;
		}

		// 插入到内存结构m_vPictureforAnimationList
		if (!AddIntoAnimationImage(picInf.ID, &picInf, bDbSelect))
		{
			bRet = false;
		}
	}


	// insert into lib_animation

	// 将vecId组合成字符串
	CString	strList;
	CString strTemp;
	for (iterId=(anim.vecPicIDList).begin(); iterId!=(anim.vecPicIDList).end(); ++iterId)
	{
		strTemp.Format(_T("%d,"), *iterId);
		strList += strTemp;
	}
	strList.TrimRight(_T(","));

	int nLen = strList.GetLength() + 1;
	char* pList = new char[nLen];
	memset(pList, 0, nLen);
	strcpy_s(pList, nLen, T2A(strList.GetBuffer()));
	strList.ReleaseBuffer();

	if (0 != InsertRecordToAnimationList(nAnimId, T2A((anim.szName).c_str()), pList, bDbSelect))
	{
		bRet = false;
	}
	SAFE_DELETE_ARRAY(pList);

	// 插入到内存结构m_vAnimationList
	if (!AddIntoAnimationList(nAnimId, &anim, bDbSelect))
	{
		bRet = false;
	}


	return bRet;
}

wstring CProjectSqlite::LoadProjectSqlite_HistoryTrend()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit10 where pageid=" << m_pPage->GetID() << ";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int	nLayer = 0;
	string	strImgType;
	vector<HistoryTrendInfo>	vecHisTrendInfo;

	USES_CONVERSION;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(6);
		nElementID = access.getColumn_Int(5);
		nX = access.getColumn_Int(0);
		nY = access.getColumn_Int(1);
		nWidth = access.getColumn_Int(2);
		nHeight = access.getColumn_Int(3);
		nLayer = access.getColumn_Int(4);

		if (access.getColumn_Text(7))
		{
			strImgType = access.getColumn_Text(7);
		}

		vecHisTrendInfo = GetHistoryTrendVectorInfo(nElementID);

		CEqmDrawHistoryTrend* pHistoryTrend = new CEqmDrawHistoryTrend();
		if (pHistoryTrend != NULL)
		{
			pHistoryTrend->SetPos(Gdiplus::PointF(nX, nY));
			pHistoryTrend->SetEqmType(EQMDRAW_HISTORY_TREND);
			pHistoryTrend->SetPageID(nPageID);
			pHistoryTrend->SetID(nElementID);
			pHistoryTrend->SetWidth(nWidth);
			pHistoryTrend->SetHeight(nHeight);
			pHistoryTrend->SetLayer(nLayer);

			HistoryTrendType hisTrendType;
			hisTrendType = (strImgType == "0" || strImgType == "") ? enumCurve : enumColumn;
			pHistoryTrend->SetHistoryTrendType(hisTrendType);
			pHistoryTrend->SetHistoryTrendVector(vecHisTrendInfo);

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\historyTrend.png");
			if (pBitmap != NULL)
			{
				pHistoryTrend->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddHistoryTrend(pHistoryTrend);


		
		}
		pHistoryTrend->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_HistoryTrend()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawHistoryTrend*>& listHisTrend = m_pPage->GetHistoryTrendList();
	list<CEqmDrawHistoryTrend*>::const_iterator it;
	int	nId = 0;
	char szImgType[MAX_PATH] = {0};

	vector<HistoryTrendInfo> vecHisTrend;
	vector<HistoryTrendInfo>::const_iterator	iter;
	CString		strTmStart;
	CString		strTmEnd;
	char		szTmStart[MAX_PATH] = {0};
	char		szTmEnd[MAX_PATH] = {0};
	char		szBindPtName[MAX_PATH] = {0};
	char		szRemarks[MAX_PATH] = {0};

	USES_CONVERSION;

	access.BeginTransaction();
	for (it=listHisTrend.begin(); it!=listHisTrend.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			sprintf_s(szImgType, sizeof(szImgType)/sizeof(char), "%d", (*it)->GetHistoryTrendType());

			access.DeleteOneFromHistoryTrend((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertOneIntoHistoryTrend((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), szImgType)!=0)
				{
					OutputSaveErr(L"InsertOneIntoHistoryTrend",access.GetSqliteErr());
				}

				nId = (*it)->GetID();
				vecHisTrend = (*it)->GetHistoryTrendVector();
				access.DeleteOneFromHistoryTrendVector(nId);
				for (iter=vecHisTrend.begin(); iter!=vecHisTrend.end(); ++iter)
				{
					strTmStart = (iter->dateTimeStart).Format(_T("%Y-%m-%d %H:%M:%S"));
					strTmEnd = (iter->dateTimeEnd).Format(_T("%Y-%m-%d %H:%M:%S"));
					sprintf_s(szTmStart, sizeof(szTmStart)/sizeof(char), "%s", T2A(strTmStart));
					sprintf_s(szTmEnd, sizeof(szTmEnd)/sizeof(char), "%s", T2A(strTmEnd));
					sprintf_s(szBindPtName, sizeof(szBindPtName)/sizeof(char), "%s", T2A(iter->strBindPointName));
					sprintf_s(szRemarks, sizeof(szRemarks)/sizeof(char), "%s", T2A(iter->strRemarks));
					if(access.InsertOneIntoHistoryTrendVector(nId, iter->nCount, szTmStart, szTmEnd, iter->bIsCalcRange, szBindPtName, szRemarks, iter->colorImage, iter->nUpdateCycle, iter->nLineWidth, iter->fScale)!=0)
					{
						OutputSaveErr(L"InsertOneIntoHistoryTrendVector",access.GetSqliteErr());
					}
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

void CProjectSqlite::SaveIntoSelectDbHistoryTrend(const list<CEqmDrawHistoryTrend*>& lstHistoryTrend)
{
	if (lstHistoryTrend.empty())
	{
		return;
	}

	list<CEqmDrawHistoryTrend*>::const_iterator	iter;
	for (iter=lstHistoryTrend.begin(); iter!=lstHistoryTrend.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexHistoryTrend);
			break;
		}
	}
}

int CProjectSqlite::Get_HistoryTrend_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit10";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

wstring CProjectSqlite::LoadProjectSqlite_EnergySaveRoi()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit12 where pageid=" << m_pPage->GetID() << ";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nPageID = 0;
	int nElementID = 0;
	int nX = 0;
	int nY = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nLayer = 0;
	string	strBindPtName;
	double	dCost = 0.0;
	string	strStartTime;
	string	strEndTime;
	string	strOccurTime;
	COleDateTime	tmStart;
	COleDateTime	tmEnd;
	COleDateTime	tmOccur;

	USES_CONVERSION;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID = access.getColumn_Int(6);
		nElementID = access.getColumn_Int(5);
		nX = access.getColumn_Int(0);
		nY = access.getColumn_Int(1);
		nWidth = access.getColumn_Int(2);
		nHeight = access.getColumn_Int(3);
		nLayer = access.getColumn_Int(4);

		if (access.getColumn_Text(7))
		{
			strBindPtName = access.getColumn_Text(7);
		}
		dCost = access.getColumn_Double(8);
		if (access.getColumn_Text(9))
		{
			strStartTime = access.getColumn_Text(9);
		}
		if (access.getColumn_Text(10))
		{
			strEndTime = access.getColumn_Text(10);
		}
		if (access.getColumn_Text(11))
		{
			strOccurTime = access.getColumn_Text(11);
		}

		CEqmDrawEnergySaveRoi* pEnergySaveRoi = new CEqmDrawEnergySaveRoi();
		if (pEnergySaveRoi != NULL)
		{
			pEnergySaveRoi->SetPos(Gdiplus::PointF(nX, nY));
			pEnergySaveRoi->SetEqmType(EQMDRAW_ENERGY_SAVE_ROI);
			pEnergySaveRoi->SetPageID(nPageID);
			pEnergySaveRoi->SetID(nElementID);
			pEnergySaveRoi->SetWidth(nWidth);
			pEnergySaveRoi->SetHeight(nHeight);
			pEnergySaveRoi->SetLayer(nLayer);

			pEnergySaveRoi->SetBindPointName(strBindPtName.c_str());
			pEnergySaveRoi->SetCost(dCost);

			tmStart.ParseDateTime(A2T(strStartTime.c_str()));
			tmEnd.ParseDateTime(A2T(strEndTime.c_str()));
			tmOccur.ParseDateTime(A2T(strOccurTime.c_str()));
			pEnergySaveRoi->SetStartTime(tmStart);
			pEnergySaveRoi->SetEndTime(tmEnd);
			pEnergySaveRoi->SetOccurTime(tmOccur);

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\roi.png");
			if (pBitmap != NULL)
			{
				pEnergySaveRoi->SetDefaultBitmap(pBitmap);
			}
			m_pPage->AddEnergySaveRoi(pEnergySaveRoi);

		}
		pEnergySaveRoi->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_EnergySaveRoi()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawEnergySaveRoi*>& lstEnergySaveRoi = m_pPage->GetEnergySaveRoiList();
	list<CEqmDrawEnergySaveRoi*>::const_iterator it;
	int	nId = 0;
	char szBindPtName[MAX_PATH] = {0};
	char szCost[MAX_PATH] = {0};
	char szStartTime[MAX_PATH] = {0};
	char szEndTime[MAX_PATH] = {0};
	char szOccurTime[MAX_PATH] = {0};
	CString	strStartTm;
	CString	strEndTm;
	CString	strOccurTm;

	USES_CONVERSION;

	access.BeginTransaction();
	for (it=lstEnergySaveRoi.begin(); it!=lstEnergySaveRoi.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			sprintf_s(szBindPtName, sizeof(szBindPtName)/sizeof(char), "%s", T2A((*it)->GetBindPointName()));
			sprintf_s(szCost, sizeof(szCost)/sizeof(char), "%f", (*it)->GetCost());

			strStartTm = ((*it)->GetStartTime()).Format(_T("%Y-%m-%d %H:%M:%S"));
			strEndTm = ((*it)->GetEndTime()).Format(_T("%Y-%m-%d %H:%M:%S"));
			strOccurTm = ((*it)->GetOccurTime()).Format(_T("%Y-%m-%d %H:%M:%S"));
			sprintf_s(szStartTime, sizeof(szStartTime)/sizeof(char), "%s", T2A(strStartTm));
			sprintf_s(szEndTime, sizeof(szEndTime)/sizeof(char), "%s", T2A(strEndTm));
			sprintf_s(szOccurTime, sizeof(szOccurTime)/sizeof(char), "%s", T2A(strOccurTm));

			access.DeleteOneFromEnergySaveRoi((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertOneIntoEnergySaveRoi((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(),
					szBindPtName, szCost, szStartTime, szEndTime, szOccurTime)!=0)
				{
					OutputSaveErr(L"InsertOneIntoEnergySaveRoi",access.GetSqliteErr());
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

void CProjectSqlite::SaveIntoSelectDbEnergySaveRoi(const list<CEqmDrawEnergySaveRoi*>& lstEnergySaveRoi)
{
	if (lstEnergySaveRoi.empty())
	{
		return;
	}

	list<CEqmDrawEnergySaveRoi*>::const_iterator	iter;
	for (iter=lstEnergySaveRoi.begin(); iter!=lstEnergySaveRoi.end(); ++iter)
	{
		if ((*iter)->GetNeedSave())
		{
			SaveIntoSelectDbImage(g_nPicIndexEnergySaveRoi);
			break;
		}
	}
}

int CProjectSqlite::Get_EnergySaveRoi_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit12";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

bool CProjectSqlite::FindDestPictureId(const int nPicId)
{
	bool	bIsFind = false;
	vector<int>::const_iterator	iter;

	for (iter=m_vecDstPicId.begin(); iter!=m_vecDstPicId.end(); ++iter)
	{
		if (*iter == nPicId)
		{
			bIsFind = true;
			break;
		}
	}

	return bIsFind;
}

void CProjectSqlite::AddIntoDestPictureId(const int nPicId)
{
	m_vecDstPicId.push_back(nPicId);
}

void CProjectSqlite::ClearDestPictureId(void)
{
	m_vecDstPicId.clear();
}

bool CProjectSqlite::FindDestAnimationId(const int nAnimId)
{
	bool	bIsFind = false;
	vector<int>::const_iterator	iter;

	for (iter=m_vecDstAnimId.begin(); iter!=m_vecDstAnimId.end(); ++iter)
	{
		if (*iter == nAnimId)
		{
			bIsFind = true;
			break;
		}
	}

	return bIsFind;
}

void CProjectSqlite::AddIntoDestAnimationId(const int nAnimId)
{
	m_vecDstAnimId.push_back(nAnimId);
}

void CProjectSqlite::ClearDestAnimationId(void)
{
	m_vecDstAnimId.clear();
}

bool CProjectSqlite::AddIntoPictureList(const int nPicId, const PictureInfo* pPicInfo, const bool bDbSelect)
{
	ASSERT(pPicInfo != NULL);
	if (NULL == pPicInfo)
	{
		return false;
	}

	if (bDbSelect)
	{
		bool bIsFind = false;
		vector<PictureInfo>::const_iterator	iter;
		for (iter=m_vPictureList.begin(); iter!=m_vPictureList.end(); ++iter)
		{
			if (iter->ID == nPicId)
			{
				bIsFind = true;
				break;
			}
		}
		if (bIsFind)
		{	// 已有相同ID成员，不插入，返回
			return false;
		}


		PictureInfo info;
		wcscpy_s(info.szName, MAX_PATH, pPicInfo->szName);
		info.nSize			= pPicInfo->nSize;
		info.etype_p		= pPicInfo->etype_p;
		info.etype_equip	= pPicInfo->etype_equip;
		info.ID				= nPicId;
		info.remark			= pPicInfo->remark;
		info.issystemimage	= pPicInfo->issystemimage;

		info.pBlock = new char[info.nSize];
		ASSERT(info.pBlock != NULL);
		if (NULL == info.pBlock)
		{
			return false;
		}
		memset(info.pBlock, 0, info.nSize);
		memcpy_s(info.pBlock, info.nSize, pPicInfo->pBlock, info.nSize);

		if (info.pBlock != NULL && info.nSize > 0)
		{
			CMemFile	memfile;
			memfile.Attach((BYTE*)(info.pBlock), info.nSize, 1024);
			memfile.SetLength(info.nSize);
			HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, info.nSize);
			if (hMemBmp != NULL)
			{
				IStream* pStmBmp = NULL;
				if (CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
				{
					BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
					memfile.SeekToBegin();
					memfile.Read(pbyBmp, info.nSize);
					Image* pBitmap = Image::FromStream(pStmBmp);
					info.pBitmap = pBitmap;
					GlobalUnlock(hMemBmp);
				}
				//GlobalFree(hMemBmp);	// Marked for jpg show
				hMemBmp = NULL;
			}
		}

		m_vPictureList.push_back(info);
	}
	else
	{
		bool bIsFind = false;
		vector<PictureInfo>::const_iterator	iter;
		for (iter=m_vPictureListImage.begin(); iter!=m_vPictureListImage.end(); ++iter)
		{
			if (iter->ID == nPicId)
			{
				bIsFind = true;
				break;
			}
		}
		if (bIsFind)
		{	// 已有相同ID成员，不插入，返回
			return false;
		}


		PictureInfo info;
		wcscpy_s(info.szName, MAX_PATH, pPicInfo->szName);
		info.nSize			= pPicInfo->nSize;
		info.etype_p		= pPicInfo->etype_p;
		info.etype_equip	= pPicInfo->etype_equip;
		info.ID				= nPicId;
		info.remark			= pPicInfo->remark;
		info.issystemimage	= pPicInfo->issystemimage;

		info.pBlock = new char[info.nSize];
		ASSERT(info.pBlock != NULL);
		if (NULL == info.pBlock)
		{
			return false;
		}
		memset(info.pBlock, 0, info.nSize);
		memcpy_s(info.pBlock, info.nSize, pPicInfo->pBlock, info.nSize);

		if (info.pBlock != NULL && info.nSize > 0)
		{
			CMemFile	memfile;
			memfile.Attach((BYTE*)(info.pBlock), info.nSize, 1024);
			memfile.SetLength(info.nSize);
			HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, info.nSize);
			if (hMemBmp != NULL)
			{
				IStream* pStmBmp = NULL;
				if (CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
				{
					BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
					memfile.SeekToBegin();
					memfile.Read(pbyBmp, info.nSize);
					Image* pBitmap = Image::FromStream(pStmBmp);
					info.pBitmap = pBitmap;
					GlobalUnlock(hMemBmp);
				}
				//GlobalFree(hMemBmp);	// Marked for jpg show
				hMemBmp = NULL;
			}
		}

		m_vPictureListImage.push_back(info);
	}

	return true;
}

bool CProjectSqlite::AddIntoAnimationList(const int nAnimId, const AnimationInfo* pAnimInfo, const bool bDbSelect)
{
	ASSERT(pAnimInfo != NULL);
	if (NULL == pAnimInfo)
	{
		return false;
	}


	bool bIsFind = false;
	vector<AnimationInfo>::const_iterator	iter;

	if (bDbSelect)
	{
		for (iter=m_vAnimationList.begin(); iter!=m_vAnimationList.end(); ++iter)
		{
			if (iter->ID == nAnimId)
			{
				bIsFind = true;
				break;
			}
		}
	}
	else
	{
		for (iter=m_vAnimationListImage.begin(); iter!=m_vAnimationListImage.end(); ++iter)
		{
			if (iter->ID == nAnimId)
			{
				bIsFind = true;
				break;
			}
		}
	}

	if (bIsFind)
	{	// 已有相同ID成员，不插入，返回
		return false;
	}


	AnimationInfo info;
	info.ID = nAnimId;
	info.szName = pAnimInfo->szName;
	info.vecPicture = pAnimInfo->vecPicture;
	info.vecpBitmap = pAnimInfo->vecpBitmap;
	info.vecPicIDList = pAnimInfo->vecPicIDList;

	if (bDbSelect)
	{
		m_vAnimationList.push_back(info);
	}
	else
	{
		m_vAnimationListImage.push_back(info);
	}


	return true;
}

bool CProjectSqlite::AddIntoAnimationImage(const int nPicId, const PictureInfo* pPicInfo, const bool bDbSelect)
{
	ASSERT(pPicInfo != NULL);
	if (NULL == pPicInfo)
	{
		return false;
	}


	bool bIsFind = false;
	vector<PictureInfo>::const_iterator	iter;

	if (bDbSelect)
	{
		for (iter=m_vPictureforAnimationList.begin(); iter!=m_vPictureforAnimationList.end(); ++iter)
		{
			if (iter->ID == nPicId)
			{
				bIsFind = true;
				break;
			}
		}
	} 
	else
	{
		for (iter=m_vPictureforAnimationListImage.begin(); iter!=m_vPictureforAnimationListImage.end(); ++iter)
		{
			if (iter->ID == nPicId)
			{
				bIsFind = true;
				break;
			}
		}
	}

	if (bIsFind)
	{	// 已有相同ID成员，不插入，返回
		return false;
	}


	PictureInfo info;
	wcscpy_s(info.szName, MAX_PATH, pPicInfo->szName);
	info.nSize			= pPicInfo->nSize;
	info.etype_p		= pPicInfo->etype_p;
	info.etype_equip	= pPicInfo->etype_equip;
	info.ID				= nPicId;
	info.remark			= pPicInfo->remark;
	info.issystemimage	= pPicInfo->issystemimage;

	info.pBlock = new char[info.nSize];
	ASSERT(info.pBlock != NULL);
	if (NULL == info.pBlock)
	{
		return false;
	}
	memset(info.pBlock, 0, info.nSize);
	memcpy_s(info.pBlock, info.nSize, pPicInfo->pBlock, info.nSize);

	if (info.pBlock != NULL && info.nSize > 0)
	{
		CMemFile	memfile;
		memfile.Attach((BYTE*)(info.pBlock), info.nSize, 1024);
		memfile.SetLength(info.nSize);
		HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, info.nSize);
		if (hMemBmp != NULL)
		{
			IStream* pStmBmp = NULL;
			if (CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
			{
				BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
				memfile.SeekToBegin();
				memfile.Read(pbyBmp, info.nSize);
				Image* pBitmap = Image::FromStream(pStmBmp);
				info.pBitmap = pBitmap;
				GlobalUnlock(hMemBmp);
			}
			GlobalFree(hMemBmp);
			hMemBmp = NULL;
		}
	}

	if (bDbSelect)
	{
		m_vPictureforAnimationList.push_back(info);
	}
	else
	{
		m_vPictureforAnimationListImage.push_back(info);
	}


	return true;
}

string	CProjectSqlite::GetProjectLastEditSoftVersion()
{
	return m_project.m_DBSetting.strLastEditSoftVersion;
}


// map navigate
wstring CProjectSqlite::LoadProjectSqlite_MapNavigate()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit13 where pageid=" << m_pPage->GetID() << ";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int		nPageID		= 0;
	int		nElementID	= 0;
	int		nX			= 0;
	int		nY			= 0;
	int		nWidth		= 0;
	int		nHeight		= 0;
	int		nLayer		= 0;
	int		nRotateAngel	= 0;
	int     nTemplateGroupId = 0;
	int     nTemplatePageId = 0;
	CString strTemplateName;
	CString paramList;
	int     nEquipNo =0;
	int     nEquipType = 0;
	CString strEquipType;
	CString strRoomName;
	CString strEquipName;
	CString cstrTemplateName;
	vector<CMapPageInfo>	vecMapPageInf;
	int		nBanOnDisplay = 0;

	while (true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nPageID				= access.getColumn_Int(6);
		nElementID			= access.getColumn_Int(5);
		nX					= access.getColumn_Int(0);
		nY					= access.getColumn_Int(1);
		nWidth				= access.getColumn_Int(2);
		nHeight				= access.getColumn_Int(3);
		nLayer				= access.getColumn_Int(4);
		nRotateAngel		= access.getColumn_Int(7);
		nEquipType			=access.getColumn_Int(8);
		strRoomName			=access.getColumn_Text(9);
		strEquipName		=access.getColumn_Text(10);
		nEquipNo			= access.getColumn_Int(11);
		nTemplateGroupId	= access.getColumn_Int(9);
	    nTemplatePageId		= access.getColumn_Int(10);
		strTemplateName		= access.getColumn_Text(11);
		paramList			= access.getColumn_Text(12);
		//新加字段用于识别模板，为空便为老版本赋值系统模板template.4db
		cstrTemplateName	= access.getColumn_Text(13);
		if(cstrTemplateName == "")
			cstrTemplateName = L"template.4db";

		nBanOnDisplay		= access.getColumn_Int(14);

		vecMapPageInf = GetMapNavigateVectorInfo(nElementID);

		CEqmDrawMapNavigate* pMapNavigate = new CEqmDrawMapNavigate();
		if (pMapNavigate != NULL)
		{
			pMapNavigate->SetPos(Gdiplus::PointF(nX, nY));
			pMapNavigate->SetEqmType(EQMDRAW_MAP_NAVIGATE);
			pMapNavigate->SetPageID(nPageID);
			pMapNavigate->SetID(nElementID);
			pMapNavigate->SetWidth(nWidth);
			pMapNavigate->SetHeight(nHeight);
			pMapNavigate->SetLayer(nLayer);

			pMapNavigate->SetRotateAngle(nRotateAngel);
			pMapNavigate->SetMapPageInfo(vecMapPageInf);
			pMapNavigate->SetEquipTemplateGroupId(nTemplateGroupId);
			pMapNavigate->SetEquipTemplatePageId(nTemplatePageId);
			pMapNavigate->SetEquipTemplatePageName(strTemplateName);
			pMapNavigate->SetParamListAllString(paramList);
			pMapNavigate->SetTemplateName(cstrTemplateName);
			if(nBanOnDisplay == 0){
				pMapNavigate->SetBanOnDisplay(false);
			}
			else{
				pMapNavigate->SetBanOnDisplay(true);
			}
			//查找4db是否存在，防止被直接创建出来
			CString cstrRunningPath;
			Project::Tools::GetSysPath(cstrRunningPath);
			if(DirectoryOperations::FileExists(cstrRunningPath+L"\\template\\", cstrTemplateName)  == false)
			{
				CString cstrAfxMes;
				cstrAfxMes.Format(L"%s模板不存在，请检查Factory安装目录下的template目录。",cstrTemplateName);
				AfxMessageBox(cstrAfxMes);
				break;
			}

			//加载图片并显示的部分，如果4DB不存在会直接创建一个出来。
			int nWidth, nHeight;
			CString strTemFile;
			CString cstrNo4db = cstrTemplateName.Left(cstrTemplateName.Find(L".4db"));
			DirectoryOperations::MakeDirectory(m_strSysPath+L"\\temp",cstrNo4db);
			strTemFile.Format(_T("%s\\temp\\%s\\%d.png"), m_strSysPath, cstrNo4db, nTemplatePageId);
			ExtractTemplatePNGFile(cstrTemplateName, nTemplatePageId, strTemFile, nWidth, nHeight);
			Image* pBitmap = Image::FromFile(strTemFile);

			if(pBitmap != NULL)
			{
				pMapNavigate->SetDefaultBitmap(pBitmap);
			}


			m_pPage->AddMapNavigate(pMapNavigate);
		}
		pMapNavigate->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_MapNavigate()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	const list<CEqmDrawMapNavigate*>& lstMapNavigate = m_pPage->GetMapNavigateList();
	list<CEqmDrawMapNavigate*>::const_iterator it;
	int	nId = 0;
	USES_CONVERSION;
	vector<CMapPageInfo> vecMapPageInf;
	vector<CMapPageInfo>::const_iterator	iter;
    
	char	szLongitude1[MAX_PATH]	= {0};
	char	szLongitude2[MAX_PATH]	= {0};
	char	szLatitude1[MAX_PATH]	= {0};
	char	szLatitude2[MAX_PATH]	= {0};

	//char	szEquipType[MAX_PATH]	= {0};
	char	szRoomName[MAX_PATH]	= {0};
	char	szEquipName[MAX_PATH]	= {0};
	
	
	//sprintf_s(szEquipType, MAX_PATH, "%s", T2A(pMapNavigate->GetEquipType()));
	access.BeginTransaction();
	for (it=lstMapNavigate.begin(); it!=lstMapNavigate.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			CEqmDrawMapNavigate* pMapNavigate = (*it);
			sprintf_s(szEquipName, MAX_PATH, "%s", T2A(pMapNavigate->GetEquipName()));
			sprintf_s(szRoomName, MAX_PATH, "%s", T2A(pMapNavigate->GetRoomName()));


			access.DeleteOneFromMapNavigate((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				string str_paramlist_utf8;
				str_paramlist_utf8 = Project::Tools::WideCharToAnsi((*it)->GetParamListAllString().GetString());

				string str_TemplateName_utf8;
				str_TemplateName_utf8 = Project::Tools::WideCharToAnsi((*it)->GetEquipTemplatePageName().GetString());

				string str_Template4dbName_utf8;
				str_Template4dbName_utf8 = Project::Tools::WideCharToAnsi((*it)->GetTemplateName().GetString());

				if(access.InsertOneIntoMapNavigate(
					(*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), (*it)->GetRotateAngle(),(*it)->GetEquipType(),
					(*it)->GetEquipTemplateGroupId(), (*it)->GetEquipTemplatePageId(), str_TemplateName_utf8.data(), 
					str_paramlist_utf8.data(), str_Template4dbName_utf8.data(), (*it)->GetBanOnDisplay())!=0)
				{
					OutputSaveErr(L"InsertOneIntoMapNavigate",access.GetSqliteErr());
				}

				nId = (*it)->GetID();
				vecMapPageInf = (*it)->GetMapPageInfo();
				access.DeleteOneFromMapNavigateVector(nId);
				for (iter=vecMapPageInf.begin(); iter!=vecMapPageInf.end(); ++iter)
				{
					sprintf_s(szLongitude1, sizeof(szLongitude1)/sizeof(char), "%0.6f", iter->GetLongitudeValue(1));
					sprintf_s(szLongitude2, sizeof(szLongitude2)/sizeof(char), "%0.6f", iter->GetLongitudeValue(2));
					sprintf_s(szLatitude1, sizeof(szLatitude1)/sizeof(char), "%0.6f", iter->GetLatitudeValue(1));
					sprintf_s(szLatitude2, sizeof(szLatitude2)/sizeof(char), "%0.6f", iter->GetLatitudeValue(2));
					if(access.InsertOneIntoMapNavigateVector(nId, szLongitude1, szLongitude2, szLatitude1, szLatitude2, iter->GetPageId())!=0)
					{
						OutputSaveErr(L"InsertOneIntoMapNavigateVector",access.GetSqliteErr());
					}
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_MapNavigate_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit13";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}


// navigate window
wstring CProjectSqlite::LoadProjectSqlite_NavigateWindow()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit15 where pageid=" << m_pPage->GetID() << ";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID		= 0;
	int		nElementID	= 0;
	int		nX			= 0;
	int		nY			= 0;
	int		nWidth		= 0;
	int		nHeight		= 0;
	int		nLayer		= 0;
	vector<PageInfo>	vecPageInfo;


	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID		= access.getColumn_Int(6);
		nElementID	= access.getColumn_Int(5);
		nX			= access.getColumn_Int(0);
		nY			= access.getColumn_Int(1);
		nWidth		= access.getColumn_Int(2);
		nHeight		= access.getColumn_Int(3);
		nLayer		= access.getColumn_Int(4);

		vecPageInfo = GetNavigateWindowVectorInfo(nElementID);

		CEqmDrawNavigateWindow* pNaviWind = new CEqmDrawNavigateWindow();
		if (pNaviWind != NULL)
		{
			pNaviWind->SetPos(Gdiplus::PointF(nX, nY));
			pNaviWind->SetEqmType(EQMDRAW_NAVIGATE_WINDOW);
			pNaviWind->SetPageID(nPageID);
			pNaviWind->SetID(nElementID);
			pNaviWind->SetWidth(nWidth);
			pNaviWind->SetHeight(nHeight);
			pNaviWind->SetLayer(nLayer);

			pNaviWind->SetPageInfo(vecPageInfo);

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\NavigateWindow.png");
			if (pBitmap != NULL)
			{
				pNaviWind->SetDefaultBitmap(pBitmap);
			}

			m_pPage->AddNavigateWindow(pNaviWind);
		}
		pNaviWind->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_NavigateWindow()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawNavigateWindow*>& lstNaviWind = m_pPage->GetNavigateWindowList();
	list<CEqmDrawNavigateWindow*>::const_iterator it;
	int	nId = 0;

	vector<PageInfo> vecPageInfo;
	vector<PageInfo>::const_iterator	iter;
	char	szPageDesc[MAX_PATH]	= {0};
	char	szPageName[MAX_PATH]	= {0};

	USES_CONVERSION;

	access.BeginTransaction();
	for (it=lstNaviWind.begin(); it!=lstNaviWind.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneFromNavigateWindow((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				if(access.InsertOneIntoNavigateWindow((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer())!=0)
				{
					OutputSaveErr(L"InsertOneIntoNavigateWindow",access.GetSqliteErr());
				}

				nId = (*it)->GetID();
				vecPageInfo = (*it)->GetPageInfo();
				access.DeleteOneFromNavigateWindowVector(nId);
				for (iter=vecPageInfo.begin(); iter!=vecPageInfo.end(); ++iter)
				{
					sprintf_s(szPageDesc, sizeof(szPageDesc)/sizeof(char), "%s", T2A(iter->strPageDesc));
					sprintf_s(szPageName, sizeof(szPageName)/sizeof(char), "%s", T2A(iter->strPageName));
					if(access.InsertOneIntoNavigateWindowVector(nId, szPageDesc, szPageName, iter->nPageId)!=0)
					{
						OutputSaveErr(L"InsertOneIntoNavigateWindowVector",access.GetSqliteErr());
					}
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_NavigateWindow_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit15";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

// level ruler
wstring CProjectSqlite::LoadProjectSqlite_LevelRuler()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit17 where pageid=" << m_pPage->GetID() << ";";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nX				= 0;
	int		nY				= 0;
	int		nWidth			= 0;
	int		nHeight			= 0;
	int		nLayer			= 0;

	string	strName;
	double	dMax			= 0;
	double	dMin			= 0;
	double	dMainInterval	= 0;
	double	dMinorInterval	= 0;
	int		nDecimalPlace	= 0;
	vector<LabelInfo>	vecLabelInfo;
	vector<SectionInfo>	vecSectionInfo;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID			= access.getColumn_Int(6);
		nElementID		= access.getColumn_Int(5);
		nX				= access.getColumn_Int(0);
		nY				= access.getColumn_Int(1);
		nWidth			= access.getColumn_Int(2);
		nHeight			= access.getColumn_Int(3);
		nLayer			= access.getColumn_Int(4);

		if (access.getColumn_Text(7) != NULL)
		{
			strName	= access.getColumn_Text(7);
		}
		dMax			= access.getColumn_Double(8);
		dMin			= access.getColumn_Double(9);
		dMainInterval	= access.getColumn_Double(10);
		dMinorInterval	= access.getColumn_Double(11);
		nDecimalPlace	= access.getColumn_Int(12);

		vecLabelInfo = GetLevelRulerVectorInfo(nElementID);
		vecSectionInfo = GetLevelRulerSectionVectorInfo(nElementID);

		CEqmDrawLevelRuler* pLevelRuler = new CEqmDrawLevelRuler();
		if (pLevelRuler != NULL)
		{
			pLevelRuler->SetPos(Gdiplus::PointF(nX, nY));
			pLevelRuler->SetEqmType(EQMDRAW_LEVEL_RULER);
			pLevelRuler->SetPageID(nPageID);
			pLevelRuler->SetID(nElementID);
			pLevelRuler->SetWidth(nWidth);
			pLevelRuler->SetHeight(nHeight);
			pLevelRuler->SetLayer(nLayer);

			pLevelRuler->SetName(strName.c_str());
			pLevelRuler->SetMax(dMax);
			pLevelRuler->SetMin(dMin);
			pLevelRuler->SetMainScaleInterval(dMainInterval);
			pLevelRuler->SetMinorScaleInterval(dMinorInterval);
			pLevelRuler->SetDecimalPlace(nDecimalPlace);
			pLevelRuler->SetLabelInfo(vecLabelInfo);
			pLevelRuler->SetSectionInfo(vecSectionInfo);

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\LevelRuler.png");
			if (pBitmap != NULL)
			{
				pLevelRuler->SetDefaultBitmap(pBitmap);
			}

			m_pPage->AddLevelRuler(pLevelRuler);
		}
		pLevelRuler->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_LevelRuler()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawLevelRuler*>& lstLevelRuler = m_pPage->GetLevelRulerList();
	list<CEqmDrawLevelRuler*>::const_iterator it;
	int	nId = 0;

	vector<LabelInfo> vecLabelInfo;
	vector<LabelInfo>::const_iterator	iter;
	char	szName[MAX_PATH]			= {0};
	char	szValue[MAX_PATH]			= {0};
	char	szBindPtName[MAX_PATH]		= {0};
	char	szLinkPageName[MAX_PATH]	= {0};

	vector<SectionInfo>	vecSectionInfo;
	vector<SectionInfo>::const_iterator	itSect;
	char	szMax[MAX_PATH]	= {0};
	char	szMin[MAX_PATH]	= {0};

	USES_CONVERSION;

	access.BeginTransaction();
	for (it=lstLevelRuler.begin(); it!=lstLevelRuler.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneFromLevelRuler((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				sprintf_s(szName, sizeof(szName)/sizeof(char), "%s", T2A((*it)->GetName()));
				access.InsertOneIntoLevelRuler((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y,
					(*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(),
					szName, (*it)->GetMax(), (*it)->GetMin(), (*it)->GetMainScaleInterval(), (*it)->GetMinorScaleInterval(), (*it)->GetDecimalPlace());


				nId = (*it)->GetID();
				access.DeleteOneFromLevelRulerVector(nId);

				vecLabelInfo = (*it)->GetLabelInfo();
				for (iter=vecLabelInfo.begin(); iter!=vecLabelInfo.end(); ++iter)
				{
					sprintf_s(szName, sizeof(szName)/sizeof(char), "%s", T2A(iter->strName));
					sprintf_s(szValue, sizeof(szValue)/sizeof(char), "%f", iter->dValue);
					sprintf_s(szBindPtName, sizeof(szBindPtName)/sizeof(char), "%s", T2A(iter->strBindPtName));
					sprintf_s(szLinkPageName, sizeof(szLinkPageName)/sizeof(char), "%s", T2A(iter->strLinkPageName));
					access.InsertOneIntoLevelRulerVector(nId, szName, szValue, szBindPtName, iter->nLinkPageId, szLinkPageName, iter->bIsUp);
				}


				access.DeleteOneFromLevelRulerSectionVector(nId);

				vecSectionInfo = (*it)->GetSectionInfo();
				for (itSect=vecSectionInfo.begin(); itSect!=vecSectionInfo.end(); ++itSect)
				{
					sprintf_s(szName, sizeof(szName)/sizeof(char), "%s", T2A(itSect->strName));
					access.InsertOneIntoLevelRulerSectionVector(nId, szName, itSect->fMax, itSect->fMin);
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_LevelRuler_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit17";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

// data analyse
wstring CProjectSqlite::LoadProjectSqlite_DataAnalyse()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='" << EQMDRAW_DATA_ANALYSE << "';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nX				= 0;
	int		nY				= 0;
	int		nWidth			= 0;
	int		nHeight			= 0;
	int		nLayer			= 0;

	vector<DaPointInfo>	vecPtInfo;
	string	strPtTime;
	string	strPtCount;
	string	strPtStatus;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID			= access.getColumn_Int(6);
		nElementID		= access.getColumn_Int(5);
		nX				= access.getColumn_Int(0);
		nY				= access.getColumn_Int(1);
		nWidth			= access.getColumn_Int(2);
		nHeight			= access.getColumn_Int(3);
		nLayer			= access.getColumn_Int(4);

		vecPtInfo = GetDataAnalyseVectorInfo(nElementID);
		if (access.getColumn_Text(8) != NULL)
		{
			strPtTime = access.getColumn_Text(8);
		}
		if (access.getColumn_Text(9) != NULL)
		{
			strPtCount = access.getColumn_Text(9);
		}
		if (access.getColumn_Text(10) != NULL)
		{
			strPtStatus = access.getColumn_Text(10);
		}

		CEqmDrawDataAnalyse* pDataAnalyse = new CEqmDrawDataAnalyse();
		if (pDataAnalyse != NULL)
		{
			pDataAnalyse->SetPos(Gdiplus::PointF(nX, nY));
			pDataAnalyse->SetEqmType(EQMDRAW_DATA_ANALYSE);
			pDataAnalyse->SetPageID(nPageID);
			pDataAnalyse->SetID(nElementID);
			pDataAnalyse->SetWidth(nWidth);
			pDataAnalyse->SetHeight(nHeight);
			pDataAnalyse->SetLayer(nLayer);

			pDataAnalyse->SetDaPointInfo(vecPtInfo);
			pDataAnalyse->SetPointTime(strPtTime.c_str());
			pDataAnalyse->SetPointCount(strPtCount.c_str());
			pDataAnalyse->SetPointStatus(strPtStatus.c_str());

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataAnalyse.png");
			if (pBitmap != NULL)
			{
				pDataAnalyse->SetDefaultBitmap(pBitmap);
			}

			m_pPage->AddDataAnalyse(pDataAnalyse);
		}
		pDataAnalyse->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_DataAnalyse()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawDataAnalyse*>& lstDataAnalyse = m_pPage->GetDataAnalyseList();
	list<CEqmDrawDataAnalyse*>::const_iterator it;

	vector<DaPointInfo>	vecDaPtInfo;
	vector<DaPointInfo>::const_iterator	iter;
	char	szPtTime[MAX_PATH]		= {0};
	char	szPtCount[MAX_PATH]		= {0};
	char	szPtStatus[MAX_PATH]	= {0};
	char	szPtName[MAX_PATH]		= {0};
	char	szRemarks[MAX_PATH]		= {0};
	int		nId = 0;


	USES_CONVERSION;

	access.BeginTransaction();
	for (it=lstDataAnalyse.begin(); it!=lstDataAnalyse.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			nId = (*it)->GetID();
			sprintf_s(szPtTime, sizeof(szPtTime)/sizeof(char), "%s", T2A((*it)->GetPointTime()));
			sprintf_s(szPtCount, sizeof(szPtCount)/sizeof(char), "%s", T2A((*it)->GetPointCount()));
			sprintf_s(szPtStatus, sizeof(szPtStatus)/sizeof(char), "%s", T2A((*it)->GetPointStatus()));

			access.DeleteOneFromDataAnalyse(nId);
			if((*it)->GetDeleteFlag() == false)
			{
				access.InsertOneIntoDataAnalyse((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), szPtTime, szPtCount, szPtStatus);


				access.DeleteOneFromDataAnalyseVector(nId);

				vecDaPtInfo = (*it)->GetDaPointInfo();
				for (iter=vecDaPtInfo.begin(); iter!=vecDaPtInfo.end(); ++iter)
				{
					sprintf_s(szPtName, sizeof(szPtName)/sizeof(char), "%s", T2A(iter->strPointName));
					sprintf_s(szRemarks, sizeof(szRemarks)/sizeof(char), "%s", T2A(iter->strPointRemarks));
					access.InsertOneIntoDataAnalyseVector(nId, szPtName, szRemarks);
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_DataAnalyse_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit19 where unitproperty01 = '" << EQMDRAW_DATA_ANALYSE << "';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

wstring CProjectSqlite::LoadAllProjectImageInfo()
{
	CString strErrorAll;
	m_vecProjectCheckInfo.clear();
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ProjectCheckInfo info;
	ostringstream sqlstream;
	//event:newpicture

	sqlstream << "select e.pageid,e.id,e.new_picture,l.name from event e ,list_page l,page_contain_elements p where e.new_picture != '' and e.pageid = l.id and e.id=p.elementid;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	USES_CONVERSION;
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.nImageID = access.getColumn_Int(2);
		info.nErrType = 0;
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//event:bind
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.bind_content,l.name from event e ,list_page l,page_contain_elements p where e.bind_content != '' and e.pageid = l.id and e.id=p.elementid;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.nErrType = 1;
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}

		wstring   wstrBind;
		if(access.getColumn_Text(1)!=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(2)));
			wstrBind = UtilString::ConvertMultiByteToWideChar(name_temp);
		}
		vector<wstring> vecInfo;
		Project::Tools::SplitStringByChar(wstrBind.c_str(),L'|',vecInfo);
		for(int i=0; i<vecInfo.size(); ++i)
		{
			vector<wstring> vecImageInfo;
			Project::Tools::SplitStringByChar(vecInfo[i].c_str(),L',',vecImageInfo);
			if(vecImageInfo.size() == 4 && vecImageInfo[2] == L"1")
			{
				info.nImageID = _wtoi(vecImageInfo[1].c_str());
				m_vecProjectCheckInfo.push_back(info);
			}
		}
	}
	access.SqlFinalize();

	//list_button:comm over down disable
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.comm,e.over,e.down,e.disable,l.name from list_button e,list_page l where e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.nErrType = 2;
		if (access.getColumn_Text(6) != NULL)
		{
			string idname(access.getColumn_Text(6));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}

		info.nImageID = access.getColumn_Int(2);
		m_vecProjectCheckInfo.push_back(info);

		info.nImageID = access.getColumn_Int(3);
		m_vecProjectCheckInfo.push_back(info);

		info.nImageID = access.getColumn_Int(4);
		m_vecProjectCheckInfo.push_back(info);

		info.nImageID = access.getColumn_Int(5);
		m_vecProjectCheckInfo.push_back(info);		
	}
	access.SqlFinalize();

	//page_contain_elements:pictureid
	sqlstream.str("");
	sqlstream << "select e.pageid,e.elementid,e.pictureid,l.name from page_contain_elements e,list_page l where e.pageid = l.id";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.nImageID = access.getColumn_Int(2);
		info.nErrType = 3;
		m_vecProjectCheckInfo.push_back(info);	
	}
	access.SqlFinalize();

	//list_bend:pictureid
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id ,l.name from list_bend e,list_page l where e.pageid = l.id";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.nImageID = g_nPicIndexBend;
		info.nErrType = 4;
		m_vecProjectCheckInfo.push_back(info);	
	}
	access.SqlFinalize();

	//list_dashboard:pictureid
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.style,l.name from list_dashboard e,list_page l where e.pageid = l.id";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		int nType = access.getColumn_Int(2);
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.nErrType = 5;
		if(nType == 0)
		{
			info.nImageID = g_nPicIndexDashBoard1;
			m_vecProjectCheckInfo.push_back(info);	
			info.nImageID = g_nPicIndexPointer1;
			m_vecProjectCheckInfo.push_back(info);	
		}
		else if(nType == 1)
		{
			info.nImageID = g_nPicIndexDashBoard2;
			m_vecProjectCheckInfo.push_back(info);	
			info.nImageID = g_nPicIndexPointer2;
			m_vecProjectCheckInfo.push_back(info);	
		}
	}
	access.SqlFinalize();

	//list_chart:elementtype
	/*sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.elementtype,l.name from list_chart e,list_page l where e.pageid = l.id";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		int nType = access.getColumn_Int(2);
		info.nErrType = 6;
		if(nType == EQMDRAW_LINE_CHART)
		{
			info.nImageID = g_nPicIndexChartLine;
			m_vecProjectCheckInfo.push_back(info);	
		}
		else if(nType == EQMDRAW_BAR_CHART)
		{
			info.nImageID = g_nPicIndexChartBar;
			m_vecProjectCheckInfo.push_back(info);	
		}
		else if(nType == EQMDRAW_PIE_CHART)
		{
			info.nImageID = g_nPicIndexChartPie;
			m_vecProjectCheckInfo.push_back(info);	
		}
		else if(nType == EQMDRAW_DIAGNOSE_CHART)
		{
			info.nImageID = g_nPicIndexDiagnoseChart;
			m_vecProjectCheckInfo.push_back(info);	
		}
	}
	access.SqlFinalize();*/

	//list_unit10:pictureid
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,l.name from list_unit10 e,list_page l where e.pageid = l.id";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.nImageID = g_nPicIndexHistoryTrend;
		info.nErrType = 7;
		m_vecProjectCheckInfo.push_back(info);	
	}
	access.SqlFinalize();

	//list_unit12:pictureid
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,l.name from list_unit12 e,list_page l where e.pageid = l.id";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.nImageID = g_nPicIndexEnergySaveRoi;
		info.nErrType = 8;
		m_vecProjectCheckInfo.push_back(info);	
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::FindAndInsertNotExist()
{
	m_vecPicID.clear();
	for(int i=0; i<m_vecProjectCheckInfo.size(); ++i)
	{
		bool bFind = false;
		int nPicID = m_vecProjectCheckInfo[i].nImageID;
		for(int j=0; j<m_vecPicID.size(); ++j)
		{
			if(m_vecPicID[j] == nPicID)
			{
				bFind = true;
				break;
			}
		}
		if(!bFind)
		{
			m_vecPicID.push_back(nPicID);
		}
	}
	return true;
}

vector<ProjectCheckInfo> CProjectSqlite::GetAllUseImageInfo()
{
	return m_vecProjectCheckInfo;
}

vector<ProjectCheckInfo> CProjectSqlite::CompareAndReturnNotExist()
{
	vector<AnimationInfo>::const_iterator	iterAni;

	m_vecProjectCheckNotFindInfo.clear();
	for(int i=0; i<m_vecProjectCheckInfo.size(); ++i)
	{
		bool bFind = false;
		int nPicID = m_vecProjectCheckInfo[i].nImageID;
		int nPicSize = m_vPictureList.size();
		for(int j=0; j<nPicSize; ++j)
		{
			if(m_vPictureList[j].ID == nPicID)
			{
				bFind = true;
				break;
			}
		}

		for (iterAni=m_vAnimationList.begin(); iterAni!=m_vAnimationList.end(); ++iterAni)
		{
			if (iterAni->ID == nPicID)
			{
				bFind = true;
				break;
			}
		}

		if(!bFind)
		{
			m_vecProjectCheckNotFindInfo.push_back(m_vecProjectCheckInfo[i]);
		}
	}
	return m_vecProjectCheckNotFindInfo;
}

bool CProjectSqlite::SetShowErrMessage( bool bShow )
{
	m_bShowErrorMessage = bShow;
	return true;
}

wstring CProjectSqlite::LoadAllProjectPointInfo()
{
	CString strErrorAll;
	m_vecProjectPointCheckInfo.clear();
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ProjectCheckInfo info;
	ostringstream sqlstream;
	//detail_chart:pointname
	sqlstream << "select e.pageid,e.id,e.pointname,l.name from detail_chart e ,list_page l   where e.pointname != '' and e.pageid = l.id;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	USES_CONVERSION;
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//detail_form:pointid
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.pointid,l.name from detail_form e ,list_page l where e.pointid != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//event:event_point_valuechange_id
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.event_point_valuechange_id,l.name from event e ,list_page l where e.event_point_valuechange_id != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//list_button:settingpoint
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.settingpoint,l.name from list_button e ,list_page l where e.settingpoint != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//list_dashboard:pointname
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.pointname,l.name from list_dashboard e ,list_page l where e.pointname != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//list_progressbar:pointid
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.bindpoint,l.name from list_progressbar e ,list_page l where e.bindpoint != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//list_text:dataComId
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.dataComId,l.name from list_text e ,list_page l where e.dataComId != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//detail_form:pointid
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.pointid,l.name from detail_form e ,list_page l where e.pointid != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();
	//list_timebar:pointid
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.bindpointstart,l.name from list_timebar e ,list_page l where e.bindpointstart != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//list_timeselect:bindpoint
	sqlstream.str("");
	sqlstream << "select e.pageid,e.id,e.bindpoint,l.name from list_timeselect e ,list_page l where e.bindpoint != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	//page_contain_elements:DataComId
	sqlstream.str("");
	sqlstream << "select e.pageid,e.elementid,e.DataComId,l.name from page_contain_elements e ,list_page l where e.DataComId != '' and e.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPointName = UtilString::ConvertMultiByteToWideChar(str);
		}
		if (access.getColumn_Text(3) != NULL)
		{
			string idname(access.getColumn_Text(3));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectPointCheckInfo.push_back(info);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

vector<ProjectCheckInfo> CProjectSqlite::CompareAndReturnNotExistPoint()
{
	m_vecProjectPointCheckNotFindInfo.clear();
	vector<DataPointEntry> vecPoint =  m_pointmap.GetPointList();
	for(int i=0; i<m_vecProjectPointCheckInfo.size(); ++i)
	{
		bool bFind = false;
		wstring strPointName = m_vecProjectPointCheckInfo[i].strPointName;
		for(int j=0; j<vecPoint.size(); ++j)
		{
			if(vecPoint[j].GetShortName() == strPointName)
			{
				bFind = true;
				break;
			}
		}
		if(!bFind)
		{
			m_vecProjectPointCheckNotFindInfo.push_back(m_vecProjectPointCheckInfo[i]);
		}
	}
	return m_vecProjectPointCheckNotFindInfo;
}

CEqmDrawProject& CProjectSqlite::GetDrawProject(void)
{
	return m_project;
}

string CProjectSqlite::GetSqliteSaveTime(void) const
{
	string	strSaveTime;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from project_config;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (NULL != access.getColumn_Text(16))
		{
			strSaveTime = (const_cast<char*>(access.getColumn_Text(16)));
		}
	}

	return strSaveTime;
}

PictureInfo* CProjectSqlite::CompareAndReturnPictureInfoByBlock( const char* pBlock, const int nSize, const wchar_t* pName, const wstring strType_p, const wstring strType_e, const bool bDbSelect /*= true*/ )
{
	if (bDbSelect)
	{
		const size_t size = m_vPictureList.size();
		for (size_t i=0;i<size;++i)
		{
			if (m_vPictureList[i].nSize == nSize
				&& strcmp(m_vPictureList[i].pBlock,pBlock)==0
				&& 0 == wcscmp(m_vPictureList[i].szName, pName)
				&& strType_p == m_vPictureList[i].etype_p
				&& strType_e == m_vPictureList[i].etype_equip)
			{
				return &m_vPictureList[i];
			}
		}
	} 
	else
	{
		const size_t size = m_vPictureListImage.size();
		for (size_t i=0;i<size;++i)
		{
			if (m_vPictureListImage[i].nSize == nSize
				&& strcmp(m_vPictureListImage[i].pBlock,pBlock)==0
				&& 0 == wcscmp(m_vPictureList[i].szName, pName)
				&& strType_p == m_vPictureList[i].etype_p
				&& strType_e == m_vPictureList[i].etype_equip)
			{
				return &m_vPictureListImage[i];
			}
		}
	}
	return NULL;
}

AnimationInfo* CProjectSqlite::CompareAndReturnAnimationInfoByName( const wstring strName, const vector<PictureInfo> vecPicIDList,const bool bDbSelect /*= true*/ )
{
	if (bDbSelect)
	{
		const size_t size = m_vAnimationList.size();
		for (size_t i=0;i<size;++i)
		{
			if (m_vAnimationList[i].szName == strName && m_vAnimationList[i].vecPicture.size() == vecPicIDList.size())
			{
				if(m_vAnimationList[i].vecPicture.size()>0 && vecPicIDList.size()>0)
				{
					if(m_vAnimationList[i].vecPicture[0].nSize == vecPicIDList[0].nSize &&   strcmp(m_vAnimationList[i].vecPicture[0].pBlock,vecPicIDList[0].pBlock)==0)
							return &m_vAnimationList[i];
				}
			}
		}
	} 
	else
	{
		const size_t size = m_vAnimationListImage.size();
		for (size_t i=0;i<size;++i)
		{
			if (m_vAnimationListImage[i].szName == strName && m_vAnimationListImage[i].vecPicture.size() == vecPicIDList.size())
			{
				if(m_vAnimationListImage[i].vecPicture.size()>0 && vecPicIDList.size()>0)
				{
					if(m_vAnimationListImage[i].vecPicture[0].nSize == vecPicIDList[0].nSize &&   strcmp(m_vAnimationListImage[i].vecPicture[0].pBlock,vecPicIDList[0].pBlock)==0)
						return &m_vAnimationListImage[i];
				}
			}
		}
	}
	return NULL;
}

wstring CProjectSqlite::InsertAnimationList( const int nID, const CString strName, const CString strSeries )
{
	CString strError;
	AnimationInfo aniInfo;
	aniInfo.ID = nID;
	aniInfo.szName = strName.GetString();
	vector<wstring> vecSeries;
	SplitStringByChar(strSeries, L',', vecSeries);
	size_t size = vecSeries.size();
	for (size_t i=0;i<size;++i)
	{
		const int nPicID = _wtoi(vecSeries[i].c_str());
		PictureInfo* info = GetPictureInfoByIDOfAnimation(nPicID, true);
		if (info)
		{
			aniInfo.vecpBitmap.push_back(info->pBitmap);
			aniInfo.vecPicIDList.push_back(info->ID);
			aniInfo.vecPicture.push_back(*info);
		}
		else
		{
			CString strMsg;
			strMsg.Format(L"动画图片缺失!!!\n页面名称:%s\n图片ID:%d",m_pPage->GetPageName().c_str(), nPicID);
			strError +=strMsg;
			strError +=L"\r\n";
		}
	}
	m_vAnimationList.push_back(aniInfo);

	return strError.GetString();
}


void	CProjectSqlite::SetImageLibPath(wstring strImageLibPath)
{
	m_strImgDbPath = strImageLibPath;
}

bool CProjectSqlite::OutputSaveErr( const wstring strOperation, const char* err )
{
	if(m_bOutPutLog)
	{
		wstring strFilePath;
		Project::Tools::GetSysPath(strFilePath);
		SYSTEMTIME stNow;
		GetLocalTime(&stNow);
		CString strLogPath;
		strLogPath.Format(_T("%s\\SqliteErr.log"),strFilePath.c_str());
		CStdioFile	ff;
		if(ff.Open(strLogPath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			wstring strTime,strEi;
			Project::Tools::SysTime_To_String(stNow,strTime);
			strEi = Project::Tools::AnsiToWideChar(err);
			CString strLog;
			strLog.Format(_T("%s::%s(%s)...\r"),strTime.c_str(),strOperation.c_str(),strEi.c_str());
			ff.Seek(0,CFile::end);
			ff.WriteString(strLog);
			ff.Close();
			return true;
		}
	}
	return false;
}

void CProjectSqlite::SetOutPutLogFlag( bool bOutPutErrLog )
{
	m_bOutPutLog = bOutPutErrLog;
}

vector<ProjectCheckInfo> CProjectSqlite::CompareAndReturnOverlapElement()
{
	CString strErrorAll;
	m_vecProjectOverLapElementInfo.clear();
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ProjectCheckInfo info;
	ostringstream sqlstream;

	//list_button:按钮重叠
	sqlstream.str("");
	sqlstream << "select p.pageid,p.id,l.name from list_button p, list_button s, list_page l where p.pageid = s.pageid and p.layer = s.layer and p.x=s.x and p.y=s.y and p.width=s.width and p.height =s.height and p.id != s.id and p.pageid = l.id;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	USES_CONVERSION;
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.strErrInfo = L"按钮边界及图层重叠";
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectOverLapElementInfo.push_back(info);		
	}
	access.SqlFinalize();

	//page_contain_elements:图片
	sqlstream.str("");
	sqlstream << "select p.pageid,p.elementid,l.name from page_contain_elements p, page_contain_elements s, list_page l where p.pageid = s.pageid and p.layer = s.layer and p.x=s.x and p.y=s.y and p.width=s.width and p.hight =s.hight and p.elementid != s.elementid and p.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.strErrInfo = L"自定义图片边界及图层重叠";
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectOverLapElementInfo.push_back(info);	
	}
	access.SqlFinalize();

	//list_chart:elementtype
	sqlstream.str("");
	sqlstream << "select p.pageid,p.id,l.name from list_chart p, list_chart s, list_page l where p.pageid = s.pageid and p.layer = s.layer and p.x=s.x and p.y=s.y and p.width=s.width and p.height =s.height and p.id != s.id and p.pageid = l.id;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		info.nPageID = access.getColumn_Int(0);
		info.nElementID = access.getColumn_Int(1);
		info.strErrInfo = L"图表边界及图层重叠";
		if (access.getColumn_Text(2) != NULL)
		{
			string idname(access.getColumn_Text(2));
			str = idname;
			info.strPageName = UtilString::ConvertMultiByteToWideChar(str);
		}
		m_vecProjectOverLapElementInfo.push_back(info);
	}
	access.SqlFinalize();

	return m_vecProjectOverLapElementInfo;
}

// temperature distribute
wstring CProjectSqlite::LoadProjectSqlite_TemperDistri()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='tempdistribute';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nX				= 0;
	int		nY				= 0;
	int		nWidth			= 0;
	int		nHeight			= 0;
	int		nLayer			= 0;

	TemperDistriInfo	info;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID			= access.getColumn_Int(6);
		nElementID		= access.getColumn_Int(5);
		nX				= access.getColumn_Int(0);
		nY				= access.getColumn_Int(1);
		nWidth			= access.getColumn_Int(2);
		nHeight			= access.getColumn_Int(3);
		nLayer			= access.getColumn_Int(4);

		info.nPicId		= access.getColumn_Int(8);
		if (access.getColumn_Text(9) != NULL)
		{
			info.strPointList = access.getColumn_Text(9);
		}
		if (access.getColumn_Text(10) != NULL)
		{
			info.strBindPtName = access.getColumn_Text(10);
		}

		CEqmDrawTemperDistri* pTemperDistri = m_pPage->GetTemperDistri(nElementID);
		if (NULL == pTemperDistri)
		{
			pTemperDistri = new CEqmDrawTemperDistri();
			pTemperDistri->SetPos(Gdiplus::PointF(nX, nY));
			pTemperDistri->SetEqmType(EQMDRAW_TEMPER_DISTRI);
			pTemperDistri->SetPageID(nPageID);
			pTemperDistri->SetID(nElementID);
			pTemperDistri->SetWidth(nWidth);
			pTemperDistri->SetHeight(nHeight);
			pTemperDistri->SetLayer(nLayer);

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\TemperDistri.png");
			if (pBitmap != NULL)
			{
				pTemperDistri->SetDefaultBitmap(pBitmap);
			}

			m_pPage->AddTemperDistri(pTemperDistri);
		}

		if (-1 == info.nPicId && _T("") == info.strPointList && _T("") == info.strBindPtName)
		{
			continue;
		}
		else
		{
			pTemperDistri->AddTemperDistri(info);
			pTemperDistri->SetNeedSave(false);
		}
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_TemperDistri()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawTemperDistri*>& listContain = m_pPage->GetTemperDistriList();
	list<CEqmDrawTemperDistri*>::const_iterator it;

	vector<TemperDistriInfo>					vecInf;
	vector<TemperDistriInfo>::const_iterator	iterInf;
	char	szPtList[MAX_PATH] = {0};
	char	szBindPtName[MAX_PATH] = {0};

	USES_CONVERSION;

	access.BeginTransaction();
	for (it=listContain.begin(); it!=listContain.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneFromTemperDistri((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				access.InsertOneIntoTemperDistri((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), -1, "", "");

				vecInf = (*it)->GetTemperDistri();
				for (iterInf=vecInf.begin(); iterInf!=vecInf.end(); ++iterInf)
				{
					sprintf_s(szPtList, MAX_PATH, "%s", T2A(iterInf->strPointList));
					sprintf_s(szBindPtName, MAX_PATH, "%s", T2A(iterInf->strBindPtName));
					access.InsertOneIntoTemperDistri((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), iterInf->nPicId, szPtList, szBindPtName);
				}
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

bool CProjectSqlite::SaveProjectSqlite_StraightLine()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawStraightLine*>& listContain = m_pPage->GetStraightLineList();
	list<CEqmDrawStraightLine*>::const_iterator it;

	USES_CONVERSION;

	access.BeginTransaction();
	for (it=listContain.begin(); it!=listContain.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneFromStraightLine((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				access.InsertOneIntoStraightLine((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetTerminalPointX()
					,(*it)->GetTerminalPointY(), (*it)->GetLayer(), (*it)->GetLineColorStrRGB(), (*it)->GetLineWidth(), (*it)->GetLineStyle()
					, (*it)->GetStartShape(), (*it)->GetStartShapeSize(), (*it)->GetEndShape(), (*it)->GetEndShapeSize());
			}
		}
	}
	access.CommitTransaction();
	return true;
}

int CProjectSqlite::Get_TemperDistri_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit19 where unitproperty01='tempdistribute'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}


//wstring	CProjectSqlite::LoadProjectSqlite_Line()
//{
//	CString strErrorAll;
//	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
//	ostringstream sqlstream;
//	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='line';";
//	string sql_ = sqlstream.str();
//	char *ex_sql = const_cast<char*>(sql_.c_str());
//	int re = access.SqlQuery(ex_sql);
//
//
//	int		nPageID			= 0;
//	int		nElementID		= 0;
//	int		nX				= 0;
//	int		nY				= 0;
//	int		nWidth			= 0;
//	int		nHeight			= 0;
//	int		nLayer			= 0;
//
//	string	strPath;
//
//	while (true)
//	{
//		if (SQLITE_ROW != access.SqlNext())
//		{
//			break;
//		}
//
//		nPageID			= access.getColumn_Int(6);
//		nElementID		= access.getColumn_Int(5);
//		nX				= access.getColumn_Int(0);
//		nY				= access.getColumn_Int(1);
//		nWidth			= access.getColumn_Int(2);
//		nHeight			= access.getColumn_Int(3);
//		nLayer			= access.getColumn_Int(4);
//
//		if (access.getColumn_Text(9) != NULL)
//		{
//			strPath = access.getColumn_Text(9);
//		}
//
//		CEqmDrawLine* pDataReport = new CEqmDrawLine();
//		if (pDataReport != NULL)
//		{
//			pDataReport->SetPos(Gdiplus::PointF(nX, nY));
//			pDataReport->SetEqmType(EQMDRAW_LINE);
//			pDataReport->SetPageID(nPageID);
//			pDataReport->SetID(nElementID);
//			pDataReport->SetWidth(nWidth);
//			pDataReport->SetHeight(nHeight);
//			pDataReport->SetLayer(nLayer);
//
//			pDataReport->SetPath(strPath.c_str());
//
//			//Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataReport.png");
//			//if (pBitmap != NULL)
//			/*{
//			pDataReport->SetDefaultBitmap(pBitmap);
//			}
//			*/
//			m_pPage->AddLine(pDataReport);
//		}
//		pDataReport->SetNeedSave(false);
//	}
//	access.SqlFinalize();
//
//	return strErrorAll.GetString();
//}



wstring	CProjectSqlite::LoadProjectSqlite_Rectangle()
{

	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='rectangle';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nX				= 0;
	int		nY				= 0;
	int		nWidth			= 0;
	int		nHeight			= 0;
	int		nLayer			= 0;

	string strColor = "#25497C#0.50";
	REAL fWidth = 3.0;
	bool bFill = 0;
	string strFillColor = "";
	bool bRound = 0;
	float bRoundHeight = 10;
	float bRoundWidth = 10;

	int nRectangleStyle = 0;
	string sTitleInfo = "";
	string sBodyInfo = "";
	string sParaphraseInfo = "";
	bool bShowParaphrase = false;
	string sPointInfoList = "";
	string sTitile = "";
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID			= access.getColumn_Int(6);
		nElementID		= access.getColumn_Int(5);
		nX				= access.getColumn_Int(0);
		nY				= access.getColumn_Int(1);
		nWidth			= access.getColumn_Int(2);
		nHeight			= access.getColumn_Int(3);
		nLayer			= access.getColumn_Int(4);
		if (access.getColumn_Text(8) != NULL)	//unitproperty02
		{
			strColor = access.getColumn_Text(8);
		}
		fWidth			= access.getColumn_Double(9);	// unitproperty03
		bFill			= access.getColumn_Int(10);		//unitproperty04
		if (access.getColumn_Text(11) != NULL)	
		{
			strFillColor = access.getColumn_Text(11);	//unitproperty05
		}
		bRound = access.getColumn_Int(12);			//unitproperty06
		bRoundHeight = access.getColumn_Int(13);	//unitproperty07
		bRoundWidth = access.getColumn_Int(14);		//unitproperty08

		nRectangleStyle = access.getColumn_Int(15);	//unitproperty09

		if(NULL != access.getColumn_Text(16))	
			sTitleInfo = access.getColumn_Text(16);		//unitproperty10

		if(NULL != access.getColumn_Text(17))
			sBodyInfo = access.getColumn_Text(17);		//unitproperty11

		if(NULL != access.getColumn_Text(18))
			sParaphraseInfo = access.getColumn_Text(18);	//unitproperty12

		bShowParaphrase = access.getColumn_Int(19);	//unitproperty13

		if(NULL != access.getColumn_Text(20))
			sPointInfoList = access.getColumn_Text(20);	//unitproperty14

		if(NULL != access.getColumn_Text(21))
			sTitile = access.getColumn_Text(21);	//unitproperty14

		if(bRoundHeight == 0)
			bRoundHeight = 10;
		if(bRoundWidth == 0)
			bRoundWidth = 10;
		if(strFillColor.size() == 0)
			strFillColor = strColor;

		CEqmDrawRectangle* pRectangle = new CEqmDrawRectangle();
		if (pRectangle != NULL)
		{
			pRectangle->SetPos(Gdiplus::PointF(nX, nY));
			pRectangle->SetEqmType(EQMDRAW_RECTANGLE);
			pRectangle->SetPageID(nPageID);
			pRectangle->SetID(nElementID);
			pRectangle->SetWidth(nWidth);
			pRectangle->SetHeight(nHeight);
			pRectangle->SetLayer(nLayer);

			pRectangle->SetLineColorStrRGB(strColor);
			pRectangle->SetLineWidth(fWidth);
			pRectangle->SetFillStyl(bFill);
			pRectangle->SetFillColorStrRGB(strFillColor);			
			pRectangle->SetToRoundRect(bRound);
			pRectangle->SetRoundHeight(bRoundHeight);
			pRectangle->SetRoundWidth(bRoundWidth);
			
			pRectangle->SetRectangleStyle(nRectangleStyle);
			pRectangle->SetTitlePropertiesInStr(sTitleInfo);
			pRectangle->SetBodyPropertiesInStr(sBodyInfo);
			pRectangle->SetParaphrasePropertiesInStr(sParaphraseInfo);
			pRectangle->SetShowParaphrase(bShowParaphrase);
			pRectangle->SetShowPointListInJsonStr(sPointInfoList);
			pRectangle->SetTitleInStr(sTitile);

			m_pPage->AddRectangle(pRectangle);
		}
		pRectangle->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}
bool CProjectSqlite::SaveProjectSqlite_Rectangle()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawRectangle*>& listContain = m_pPage->GetRectangleList();
	list<CEqmDrawRectangle*>::const_iterator it;

	USES_CONVERSION;

	access.BeginTransaction();
	for (it=listContain.begin(); it!=listContain.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneFromRectangle((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
				access.InsertOneIntoRectangle((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth()
				, (*it)->GetHeight(), (*it)->GetLayer(), (*it)->GetLineColorStrRGB(), (*it)->GetLineWidth(), (*it)->GetFillStyl()
				, (*it)->GetFillColorStrRGB(), (*it)->GetToRoundRect(), (*it)->GetRoundHeight(), (*it)->GetRoundWidth()
				, (*it)->GetRectangleStyle(), (*it)->GetTitlePropertiesInStr(), (*it)->GetBodyPropertiesInStr(), (*it)->GetParaphrasePropertiesInStr()
				, (*it)->GetShowParaphrase(), (*it)->GetShowPointListInJsonStr(), (*it)->GetTitleInStr());
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}
// data report
wstring CProjectSqlite::LoadProjectSqlite_DataReport()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='report';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nX				= 0;
	int		nY				= 0;
	int		nWidth			= 0;
	int		nHeight			= 0;
	int		nLayer			= 0;

	string	strPath;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID			= access.getColumn_Int(6);
		nElementID		= access.getColumn_Int(5);
		nX				= access.getColumn_Int(0);
		nY				= access.getColumn_Int(1);
		nWidth			= access.getColumn_Int(2);
		nHeight			= access.getColumn_Int(3);
		nLayer			= access.getColumn_Int(4);

		if (access.getColumn_Text(9) != NULL)
		{
			strPath = access.getColumn_Text(9);
		}

		CEqmDrawDataReport* pDataReport = new CEqmDrawDataReport();
		if (pDataReport != NULL)
		{
			pDataReport->SetPos(Gdiplus::PointF(nX, nY));
			pDataReport->SetEqmType(EQMDRAW_DATA_REPORT);
			pDataReport->SetPageID(nPageID);
			pDataReport->SetID(nElementID);
			pDataReport->SetWidth(nWidth);
			pDataReport->SetHeight(nHeight);
			pDataReport->SetLayer(nLayer);

			pDataReport->SetPath(strPath.c_str());

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataReport.png");
			if (pBitmap != NULL)
			{
				pDataReport->SetDefaultBitmap(pBitmap);
			}

			m_pPage->AddDataReport(pDataReport);
		}
		pDataReport->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

wstring CProjectSqlite::LoadProjectSqlite_StraightLine()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='straightLine';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nX				= 0;
	int		nY				= 0;
	int		nEndx			= 0;
	int		nEndY			= 0;
	int		nLayer			= 0;

	string strColor;
	REAL fWidth;
	int nLineStyle;

	int nStartShape = 0;
	int nStartShapeSize = 0;
	int nEndShape = 0;
	int nEndShapeSize = 0;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID			= access.getColumn_Int(6);//pageid
		nElementID		= access.getColumn_Int(5);//id
		nX				= access.getColumn_Int(0);//x
		nY				= access.getColumn_Int(1);//y
		nEndx			= access.getColumn_Int(2);//width
		nEndY			= access.getColumn_Int(3);//hight
		nLayer			= access.getColumn_Int(4);//layer

		strColor			= access.getColumn_Text(8);//unitproperty02
		fWidth				= access.getColumn_Double(9);//unitproperty03
		nLineStyle			= access.getColumn_Int(10);//unitproperty04

		nStartShape		= access.getColumn_Int(12);//unitproperty06
		nStartShapeSize		= access.getColumn_Int(13);//unitproperty07
		nEndShape		= access.getColumn_Int(14);//unitproperty08
		nEndShapeSize		= access.getColumn_Int(15);//unitproperty09

		CEqmDrawStraightLine* pStraightLine = new CEqmDrawStraightLine();
		if (pStraightLine != NULL)
		{
			pStraightLine->SetPos(Gdiplus::PointF(nX, nY));
			pStraightLine->SetEqmType(EQMDRAW_LINE);
			pStraightLine->SetPageID(nPageID);
			pStraightLine->SetID(nElementID);
			pStraightLine->SetTerminalPointX(nEndx);
			pStraightLine->SetTerminalPointY(nEndY);
			pStraightLine->SetLayer(nLayer);

			pStraightLine->SetLineColorStrRGB(strColor);
			pStraightLine->SetLineWidth(fWidth);
			pStraightLine->SetLineStyle(nLineStyle);

			pStraightLine->SetStartShape((_LineEndShape)nStartShape);
			pStraightLine->SetStartShapeSize(nStartShapeSize);
			pStraightLine->SetEndShape((_LineEndShape)nEndShape);
			pStraightLine->SetEndShapeSize(nEndShapeSize);

			m_pPage->AddStraightLine(pStraightLine);
		}
		pStraightLine->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_DataReport()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawDataReport*>& listContain = m_pPage->GetDataReportList();
	list<CEqmDrawDataReport*>::const_iterator it;
	char	szPath[MAX_PATH] = {0};

	USES_CONVERSION;

	access.BeginTransaction();
	for (it=listContain.begin(); it!=listContain.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneFromDataReport((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				sprintf_s(szPath, MAX_PATH, "%s", T2A((*it)->GetPath()));
				access.InsertOneIntoDataReport((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), szPath);
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_DataReport_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit19 where unitproperty01='report'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

// Plane Temperature Distribution
wstring CProjectSqlite::LoadProjectSqlite_PlaneTempeDistr()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='planeTempeDistr';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nX				= 0;
	int		nY				= 0;
	int		nWidth			= 0;
	int		nHeight			= 0;
	int		nLayer			= 0;

	string	strPointName;
	int		nCfgType		= 0;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID			= access.getColumn_Int(6);
		nElementID		= access.getColumn_Int(5);
		nX				= access.getColumn_Int(0);
		nY				= access.getColumn_Int(1);
		nWidth			= access.getColumn_Int(2);
		nHeight			= access.getColumn_Int(3);
		nLayer			= access.getColumn_Int(4);

		if (access.getColumn_Text(9) != NULL)
		{
			strPointName = access.getColumn_Text(9);
		}
		
		if (access.getColumn_Text(10) != NULL)
		{
			string strCfgType(access.getColumn_Text(10));
			nCfgType = atoi(strCfgType.c_str());
		}

		CEqmDrawPlaneTempeDistr* pPlaneTempeDistr = new CEqmDrawPlaneTempeDistr();
		if (pPlaneTempeDistr != NULL)
		{
			pPlaneTempeDistr->SetPos(Gdiplus::PointF(nX, nY));
			pPlaneTempeDistr->SetEqmType(EQMDRAW_PLANE_TEMPE_DISTR);
			pPlaneTempeDistr->SetPageID(nPageID);
			pPlaneTempeDistr->SetID(nElementID);
			pPlaneTempeDistr->SetWidth(nWidth);
			pPlaneTempeDistr->SetHeight(nHeight);
			pPlaneTempeDistr->SetLayer(nLayer);

			pPlaneTempeDistr->SetPointName(strPointName.c_str());
			pPlaneTempeDistr->SetConfigType(nCfgType);

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\PlaneTempeDistr.png");
			if (pBitmap != NULL)
			{
				pPlaneTempeDistr->SetDefaultBitmap(pBitmap);
			}

			m_pPage->AddPlaneTempeDistr(pPlaneTempeDistr);
		}
		pPlaneTempeDistr->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_PlaneTempeDistr()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	const list<CEqmDrawPlaneTempeDistr*>& listContain = m_pPage->GetPlaneTempeDistrList();
	list<CEqmDrawPlaneTempeDistr*>::const_iterator it;
	const int c_nLen = 100000;
	char* pPtName = new char[c_nLen];
	memset(pPtName, 0, c_nLen);

	USES_CONVERSION;

	access.BeginTransaction();
	for (it=listContain.begin(); it!=listContain.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneFromPlaneTempeDistr((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				sprintf_s(pPtName, c_nLen, "%s", T2A((*it)->GetPointName()));
				access.InsertOneIntoPlaneTempeDistr((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer(), pPtName, (*it)->GetConfigType());
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	SAFE_DELETE_ARRAY(pPtName);
	return true;
}

int CProjectSqlite::Get_PlaneTempeDistr_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit19 where unitproperty01='planeTempeDistr'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

// Dotted Line Frame
wstring CProjectSqlite::LoadProjectSqlite_DottedLineFrm()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='dottedLineFrame';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nX				= 0;
	int		nY				= 0;
	int		nWidth			= 0;
	int		nHeight			= 0;
	int		nLayer			= 0;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID			= access.getColumn_Int(6);
		nElementID		= access.getColumn_Int(5);
		nX				= access.getColumn_Int(0);
		nY				= access.getColumn_Int(1);
		nWidth			= access.getColumn_Int(2);
		nHeight			= access.getColumn_Int(3);
		nLayer			= access.getColumn_Int(4);

		CEqmDrawDottedLineFrm* pDottedLineFrm = new CEqmDrawDottedLineFrm();
		if (pDottedLineFrm != NULL)
		{
			pDottedLineFrm->SetPos(Gdiplus::PointF(nX, nY));
			pDottedLineFrm->SetEqmType(EQMDRAW_DOTLINE_FRAME);
			pDottedLineFrm->SetPageID(nPageID);
			pDottedLineFrm->SetID(nElementID);
			pDottedLineFrm->SetWidth(nWidth);
			pDottedLineFrm->SetHeight(nHeight);
			pDottedLineFrm->SetLayer(nLayer);

			m_pPage->AddDottedLineFrm(pDottedLineFrm);
		}
		pDottedLineFrm->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_DottedLineFrm()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	const list<CEqmDrawDottedLineFrm*>& listContain = m_pPage->GetDottedLineFrmList();
	list<CEqmDrawDottedLineFrm*>::const_iterator it;

	access.BeginTransaction();
	for (it=listContain.begin(); it!=listContain.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			access.DeleteOneFromDottedLineFrm((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				access.InsertOneIntoDottedLineFrm((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y, (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetLayer());
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_DottedLineFrm_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit19 where unitproperty01='dottedLineFrame'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

// General Component
wstring CProjectSqlite::LoadProjectSqlite_GeneralComponent()
{
	CString strErrorAll;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit19 where pageid=" << m_pPage->GetID() << " and unitproperty01='generalComponent';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int		nPageID			= 0;
	int		nElementID		= 0;
	int		nX				= 0;
	int		nY				= 0;
	int		nWidth			= 0;
	int		nHeight			= 0;
	int		nLayer			= 0;

	string	strCfgValue;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nPageID			= access.getColumn_Int(6);
		nElementID		= access.getColumn_Int(5);
		nX				= access.getColumn_Int(0);
		nY				= access.getColumn_Int(1);
		nWidth			= access.getColumn_Int(2);
		nHeight			= access.getColumn_Int(3);
		nLayer			= access.getColumn_Int(4);

		if (access.getColumn_Text(8) != NULL)
		{
			if (NULL != access.getColumn_Text(8))
			{
				strCfgValue = access.getColumn_Text(8);
			}
		}

		CEqmDrawGeneralComponent* pGeneralComponent = new CEqmDrawGeneralComponent();
		if (pGeneralComponent != NULL)
		{
			pGeneralComponent->SetPos(Gdiplus::PointF(nX, nY));
			pGeneralComponent->SetEqmType(EQMDRAW_GENERAL_COMPONENT);
			pGeneralComponent->SetPageID(nPageID);
			pGeneralComponent->SetID(nElementID);
			pGeneralComponent->SetWidth(nWidth);
			pGeneralComponent->SetHeight(nHeight);
			pGeneralComponent->SetLayer(nLayer);


			wstring wstrValue = Project::Tools::AnsiToWideChar(strCfgValue.c_str());
			pGeneralComponent->SetConfigValue(wstrValue.c_str());

			Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\GeneralComponent.png");
			if (pBitmap != NULL)
			{
				pGeneralComponent->SetDefaultBitmap(pBitmap);
			}

			m_pPage->AddGeneralComponent(pGeneralComponent);
		}
		pGeneralComponent->SetNeedSave(false);
	}
	access.SqlFinalize();

	return strErrorAll.GetString();
}

bool CProjectSqlite::SaveProjectSqlite_GeneralComponent()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	const list<CEqmDrawGeneralComponent*>& listContain = m_pPage->GetGeneralComponentList();
	list<CEqmDrawGeneralComponent*>::const_iterator it;


	USES_CONVERSION;
	access.BeginTransaction();
	for (it=listContain.begin(); it!=listContain.end(); ++it)
	{
		if ((*it)->GetNeedSave())
		{
			CString cstrContent = (*it)->GetConfigValue();
			cstrContent.Replace(_T("'"), _T("\""));

			access.DeleteOneFromGeneralComponent((*it)->GetID());
			if((*it)->GetDeleteFlag() == false)
			{
				int nWidth = (*it)->GetWidth();
				int nHeight = (*it)->GetHeight();
				if(nWidth<=0)
					nWidth  =200;
				if(nHeight<=0)
					nHeight = 200;
				access.InsertOneIntoGeneralComponent((*it)->GetPageID(), (*it)->GetID(), (*it)->GetPos().X, (*it)->GetPos().Y, nWidth, nHeight, (*it)->GetLayer(), cstrContent.GetString());
			}
		}
	}
	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

int CProjectSqlite::Get_GeneralComponent_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit19 where unitproperty01='generalComponent'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}


int CProjectSqlite::Get_Rectangle_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit19 where unitproperty01='rectangle'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_StraightLine_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit19 where unitproperty01='straightLine'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

int CProjectSqlite::Get_Polygon_max_id()
{
	int nMax = 0;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select max(id) from list_unit19 where unitproperty01='polygon'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		nMax = access.getColumn_Int(0);
	}
	access.SqlFinalize();
	return nMax;
}

PictureInfo CProjectSqlite::GetAnimationPictureInfoByID(const int nPicID, const bool bDbSelect)
{
	vector<PictureInfo>::const_iterator	iter;
	if (bDbSelect)
	{
		for (iter=m_vPictureforAnimationList.begin(); iter!=m_vPictureforAnimationList.end(); ++iter)
		{
			if (iter->ID == nPicID)
			{
				return *iter;
			}
		}
	} 
	else
	{
		for (iter=m_vPictureforAnimationListImage.begin(); iter!=m_vPictureforAnimationListImage.end(); ++iter)
		{
			if (iter->ID == nPicID)
			{
				return *iter;
			}
		}
	}

	PictureInfo info;
	return info;
}

bool CProjectSqlite::InsertImageIntoOpenS3db(void)
{
	return false;
}

bool CProjectSqlite::InsertAnimationIntoOpenS3db(const AnimationInfo* pSrcInfo, int& nUseId)
{
	bool bRet = false;

	try
	{
		if (NULL == pSrcInfo)
		{
			return false;
		}

		int nSrcId = pSrcInfo->ID;
		const AnimationInfo* pDstInfo = GetAnimationByID(nSrcId, true); 
		if (NULL == pDstInfo)
		{	// 打开的s3db没有该编号的图，直接插入数据库
			nUseId = nSrcId;
			bRet = SetAnimationDataEx(nUseId, pSrcInfo, true);
		}
		else
		{	// 打开的s3db有该编号的图
			if (0 == wcscmp(pSrcInfo->szName.c_str(), pDstInfo->szName.c_str())
				&& pSrcInfo->vecPicture.size() == pDstInfo->vecPicture.size()
				&& pSrcInfo->vecPicIDList.size() == pDstInfo->vecPicIDList.size()
				&& pSrcInfo->vecPicIDList.at(0) == pDstInfo->vecPicIDList.at(0)
				)
			{
				// 文件名相同，则认为是同一个文件，跳过
				nUseId = nSrcId;
			}
			else
			{
				// 有该编号的图，但文件名不同，则取目标s3db最大编号+1，插入
				nUseId = GetMaxAnimationNum(true) + 1;
				bRet = SetAnimationDataEx(nUseId, pSrcInfo, true);
			}
		}
	}
	catch (...)
	{
		return false;
	}

	return bRet;
}

bool CProjectSqlite::InsertImageIntoOpenS3dbFromId(void)
{
	return false;
}

bool CProjectSqlite::InsertAnimationIntoOpenS3dbFromId(const int nSrcId, int& nUseId)
{
	bool bRet = false;
	const AnimationInfo* pAniInf = GetAnimationByID(nSrcId, false);
	if (pAniInf != NULL)
	{
		bRet = InsertAnimationIntoOpenS3db(pAniInf, nUseId);
	}

	return bRet;
}

int CProjectSqlite::GetMaxPictureNum(const bool bDbSelect)
{
	vector<PictureInfo>::const_iterator	iter;
	int nMaxNum = 0;

	if (bDbSelect)
	{
		for (iter=m_vPictureList.begin(); iter!=m_vPictureList.end(); ++iter)
		{
			if (nMaxNum < iter->ID)
			{
				nMaxNum = iter->ID;
			}
		}
	}
	else
	{
		for (iter=m_vPictureListImage.begin(); iter!=m_vPictureListImage.end(); ++iter)
		{
			if (nMaxNum < iter->ID)
			{
				nMaxNum = iter->ID;
			}
		}
	}

	return nMaxNum;
}

int CProjectSqlite::GetMaxAnimationNum(const bool bDbSelect)
{
	vector<AnimationInfo>::const_iterator	iter;
	int nMaxNum = 0;

	if (bDbSelect)
	{
		for (iter=m_vAnimationList.begin(); iter!=m_vAnimationList.end(); ++iter)
		{
			if (nMaxNum < iter->ID)
			{
				nMaxNum = iter->ID;
			}
		}
	}
	else
	{
		for (iter=m_vAnimationListImage.begin(); iter!=m_vAnimationListImage.end(); ++iter)
		{
			if (nMaxNum < iter->ID)
			{
				nMaxNum = iter->ID;
			}
		}
	}

	return nMaxNum;
}

int CProjectSqlite::GetMaxAnimationPictureNum(const bool bDbSelect)
{
	vector<PictureInfo>::const_iterator	iter;
	int nMaxNum = 0;

	if (bDbSelect)
	{
		for (iter=m_vPictureforAnimationList.begin(); iter!=m_vPictureforAnimationList.end(); ++iter)
		{
			if (nMaxNum < iter->ID)
			{
				nMaxNum = iter->ID;
			}
		}
	}
	else
	{
		for (iter=m_vPictureforAnimationListImage.begin(); iter!=m_vPictureforAnimationListImage.end(); ++iter)
		{
			if (nMaxNum < iter->ID)
			{
				nMaxNum = iter->ID;
			}
		}
	}

	return nMaxNum;
}

void CProjectSqlite::LoadAllPagesDetail(void)
{
	int nPageCount = m_project.GetPageCount();
	for(int i=0; i<nPageCount; i++)
	{
		if (!m_project.GetPageList()[i]->m_bDetailLoaded)
		{
			LoadPageDetails(m_project.GetPageList()[i], NULL, NULL);
		}
	}
}

void CProjectSqlite::ImageLeanFunc(void)
{
	// 扫描6种图元对应的7张表
	LoadCustomImageInfo();

	// 扫描lib_image
	LoadLibImageInfo();

	// 比较
	CheckExcessImage();

	// 删除
	DeleteExcessImage();

	//删除动画用到的图片 
	DeleteAnimation();
	DeleteAnimationImg();
}

void CProjectSqlite::ImageLeanCoreFunc(void)
{
	DeleteImageAll();
	DeleteAnimationAll();
	DeleteImageAnimationAll();
}

void CProjectSqlite::LoadCustomImageInfo(void)
{
	vecCustomImgInfo.clear();

	LoadImageCustomMeta();
	LoadImageEvent();
	LoadImageButton();
	//LoadImageChartPlus();
	LoadImageAreaChange();
	LoadImageShellExecute();
	LoadImageEmbededPage();
	LoadImagePageBackground();
}

bool CProjectSqlite::LoadImageCustomMeta(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select pageid, pictureid, width, hight, elementid from page_contain_elements where elementtype=" << EQMDRAW_METAFILE << ";";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());

	try
	{
		access.SqlQuery(ex_sql);
		while (true)
		{
			if (SQLITE_ROW != access.SqlNext())
			{
				break;
			}

			CustomImgInfo	info;
			info.nPageId	= access.getColumn_Int(0);
			info.nImgType	= EQMDRAW_METAFILE;
			info.nImgId		= access.getColumn_Int(1);
			info.nWidth     = access.getColumn_Int(2);
			info.nHeight    = access.getColumn_Int(3);
			info.nElementId = access.getColumn_Int(4);
			//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
			//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
			{
				vecCustomImgInfo.push_back(info);
			}
		}
		access.SqlFinalize();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//yan modified
bool CProjectSqlite::LoadImageEvent(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select pageid, bind_content, id from event;";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());

	string strBindContent;
	const TCHAR cFlag(_T('|'));
	const TCHAR cDiv(_T(','));

	try
	{
		access.SqlQuery(ex_sql);
		while (true)
		{
			if (SQLITE_ROW != access.SqlNext())
			{
				break;
			}

			CustomImgInfo	info;
			info.nPageId	= access.getColumn_Int(0);
			info.nImgType	= EQMDRAW_METAFILE;

			const char* pBindCont = access.getColumn_Text(1);
			const int nElementId = access.getColumn_Int(2);
			info.nElementId = nElementId;
			if (pBindCont && strlen(pBindCont) > 0)
			{
				wstring wstrTemp = UtilString::ConvertMultiByteToWideChar(pBindCont);
				vector<wstring> vec1;
				Project::Tools::SplitStringByChar(wstrTemp.c_str(), L'|', vec1);
				for (int i=0;i<(int)vec1.size();++i)
				{
					vector<wstring> vec2;
					Project::Tools::SplitStringByChar(vec1[i].c_str(), L',', vec2);
					assert(vec2.size() == 4);
					info.nImgId = _ttoi(vec2[1].c_str());
					info.bAnimation = wcscmp(vec2[2].c_str(), L"1") > 0?true:false;
					for(int j=0;j<(int)vecCustomImgInfo.size();++j)
					{
						if (vecCustomImgInfo[j].nElementId == nElementId)
						{
							info.nWidth = vecCustomImgInfo[j].nWidth;
							info.nHeight = vecCustomImgInfo[j].nHeight;
							//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
							//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
							{
								vecCustomImgInfo.push_back(info);
							}
							break;
						}
					}
				}
			}
		}
		access.SqlFinalize();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CProjectSqlite::LoadImageButton(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select pageid, comm, over, down, disable, unitproperty03, id, width, height from list_button where elementtype=" << EQMDRAW_METABUTTON << ";";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());
	
	try
	{
		access.SqlQuery(ex_sql);
		while (true)
		{
			if (SQLITE_ROW != access.SqlNext())
			{
				break;
			}

			CustomImgInfo	info;
			info.nImgType	= EQMDRAW_METABUTTON;
			info.nPageId	= access.getColumn_Int(0);

			info.nElementId = access.getColumn_Int(6);
			info.nWidth = access.getColumn_Int(7);
			info.nHeight = access.getColumn_Int(8);

			info.nImgId = access.getColumn_Int(1);
			{
				//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
				//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
				{
					vecCustomImgInfo.push_back(info);
				}
			}


			info.nImgId = access.getColumn_Int(2);
			{
				//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
				//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
				{
					vecCustomImgInfo.push_back(info);
				}
			}


			info.nImgId = access.getColumn_Int(3);
			{
				//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
				//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
				{
					vecCustomImgInfo.push_back(info);
				}
			}


			info.nImgId = access.getColumn_Int(4);
			{
				//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
				//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
				{
					vecCustomImgInfo.push_back(info);
				}
			}



			const char* pU3 = access.getColumn_Text(5);
			if (NULL != pU3)
			{
				info.nImgId = atoi(pU3);
				{
					//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
					//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
					{
						vecCustomImgInfo.push_back(info);
					}
				}
			}
		}
		access.SqlFinalize();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CProjectSqlite::LoadImageChartPlus(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select pageid, unitproperty01, id, width, height from list_chart where elementtype=" << EQMDRAW_POLYGON << ";";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());

	try
	{
		access.SqlQuery(ex_sql);
		while (true)
		{
			if (SQLITE_ROW != access.SqlNext())
			{
				break;
			}

			CustomImgInfo	info;
			info.nPageId	= access.getColumn_Int(0);
			info.nImgType	= EQMDRAW_POLYGON;
			info.nElementId = access.getColumn_Int(2);
			info.nWidth = access.getColumn_Int(3);
			info.nHeight = access.getColumn_Int(4);

			const char* pU1 = access.getColumn_Text(1);
			if (NULL != pU1)
			{
				info.nImgId = atoi(pU1);
				//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
				//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
				{
					vecCustomImgInfo.push_back(info);
				}
			}
		}
		access.SqlFinalize();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CProjectSqlite::LoadImageAreaChange(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select pageid, unitproperty01, unitproperty03, id, width, height from list_unit02;";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());

	try
	{
		access.SqlQuery(ex_sql);
		while (true)
		{
			if (SQLITE_ROW != access.SqlNext())
			{
				break;
			}

			CustomImgInfo	info;
			info.nPageId	= access.getColumn_Int(0);
			info.nImgType	= EQMDRAW_AREA_CHANGE;
			info.nElementId = access.getColumn_Int(3);
			info.nWidth = access.getColumn_Int(4);
			info.nHeight = access.getColumn_Int(5);

			const char* pU1 = access.getColumn_Text(1);
			if (NULL != pU1)
			{
				info.nImgId = atoi(pU1);
				//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
				//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
				{
					vecCustomImgInfo.push_back(info);
				}
			}

			const char* pU3 = access.getColumn_Text(2);
			if (NULL != pU3)
			{
				info.nImgId = atoi(pU3);
				//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
				//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
				{
					vecCustomImgInfo.push_back(info);
				}
			}
		}
		access.SqlFinalize();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CProjectSqlite::LoadImageShellExecute(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select pageid, unitproperty04, id, width, height from list_unit04;";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());

	try
	{
		access.SqlQuery(ex_sql);
		while (true)
		{
			if (SQLITE_ROW != access.SqlNext())
			{
				break;
			}

			CustomImgInfo	info;
			info.nPageId	= access.getColumn_Int(0);
			info.nImgType	= EQMDRAW_SHELL_EXECUTE;
			info.nElementId = access.getColumn_Int(2);
			info.nWidth = access.getColumn_Int(3);
			info.nHeight = access.getColumn_Int(4);

			const char* pU4 = access.getColumn_Text(1);
			if (NULL != pU4)
			{
				info.nImgId = atoi(pU4);
				//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
				//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
				{
					vecCustomImgInfo.push_back(info);
				}
			}
		}
		access.SqlFinalize();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CProjectSqlite::LoadImageEmbededPage(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select pageid, unitproperty01, id, width, height from list_unit05;";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());

	try
	{
		access.SqlQuery(ex_sql);
		while (true)
		{
			if (SQLITE_ROW != access.SqlNext())
			{
				break;
			}

			CustomImgInfo	info;
			info.nPageId	= access.getColumn_Int(0);
			info.nImgType	= EQMDRAW_EMBEDED_PAGE;
			info.nElementId = access.getColumn_Int(2);
			info.nWidth = access.getColumn_Int(3);
			info.nHeight = access.getColumn_Int(4);

			const char* pU1 = access.getColumn_Text(1);
			if (NULL != pU1)
			{
				info.nImgId = atoi(pU1);
				//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
				//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
				{
					vecCustomImgInfo.push_back(info);
				}
			}
		}
		access.SqlFinalize();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CProjectSqlite::LoadImagePageBackground(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select id, bgimg, width, hight from list_page;";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());

	try
	{
		access.SqlQuery(ex_sql);
		while (true)
		{
			if (SQLITE_ROW != access.SqlNext())
			{
				break;
			}

			CustomImgInfo	info;
			info.nPageId	= access.getColumn_Int(0);
			info.nImgType	= EQMDRAW_UNKNOWN;	// 背景图片，不是图元
			info.nImgId		= access.getColumn_Int(1);
			info.nElementId = access.getColumn_Int(0);
			info.nWidth = access.getColumn_Int(2);
			info.nHeight = access.getColumn_Int(3);
			//Gdiplus::Bitmap* pBitmap = GetBitmapByIDFromPictureLib(info.nImgId);
			//if (pBitmap && ((pBitmap->GetWidth()>info.nWidth)|(pBitmap->GetHeight()>info.nHeight)))
			{
				vecCustomImgInfo.push_back(info);
			}
		}
		access.SqlFinalize();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

void CProjectSqlite::LoadLibImageInfo(void)
{
	LoadPictureLibSqlite(true);
	LoadAnimationSqlite(true);
}

void CProjectSqlite::CheckExcessImage(void)
{
	bool bFind = false;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	UINT num, size;
	GetImageEncodersSize(&num, &size);
	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	GetImageEncoders(num, size, pImageCodecInfo);
	CLSID encoder;
	for (int i = 0;i < num; ++i) 
	{
		if (_wcsicmp(pImageCodecInfo[i].MimeType, L"image/png") == 0) 
		{
			encoder = pImageCodecInfo[i].Clsid;
			break;
		}
	}
	free(pImageCodecInfo);
	for (int i=0;i<(int)m_vPictureList.size();++i)
	{
		bFind = false;
		for (int j=0;j<(int)vecCustomImgInfo.size();++j)
		{
			if (vecCustomImgInfo[j].nImgId == m_vPictureList[i].ID && !vecCustomImgInfo[j].bAnimation)
			{
				bFind = true;
				{
					CMemFile   memfile;  
					memfile.Attach((BYTE*)(m_vPictureList[i].pBlock),m_vPictureList[i].nSize,1024);
					memfile.SetLength(m_vPictureList[i].nSize); 
					HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, m_vPictureList[i].nSize);
					if (hMemBmp)
					{
						IStream* pStmBmp = NULL;
						if(CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
						{
							BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
							memfile.SeekToBegin();
							memfile.Read(pbyBmp, m_vPictureList[i].nSize);
							Image* pBitmap = Image::FromStream(pStmBmp);
							Image* p = CompressPNG( pBitmap, (Gdiplus::REAL)vecCustomImgInfo[j].nWidth, (Gdiplus::REAL)vecCustomImgInfo[j].nHeight);
							assert(p);	
							if (p)
							{
								Status status = p->Save(L"tempCompressed.png", &encoder, NULL);
								if (status == Status::Ok)
								{
									FILE* pFile = NULL;
									const errno_t rs = _wfopen_s(&pFile, L"tempCompressed.png", L"rb");
									assert(0 == rs);
									assert(pFile);
									fseek(pFile, 0, SEEK_END);
									const int file_size = ftell(pFile);
									assert(file_size > 0);
									char* pBuf = new char[file_size];
									memset(pBuf, 0, file_size);
									fseek(pFile, 0, SEEK_SET);
									fread(pBuf, sizeof(char), file_size, pFile);
									access.DeleteOnePicture(m_vPictureList[i].ID);
									access.InsertRecordIntoLibImage(m_vPictureList[i].ID, 
									Project::Tools::WideCharToAnsi(m_vPictureList[i].szName).c_str(),
									Project::Tools::WideCharToAnsi(m_vPictureList[i].etype_equip.c_str()).c_str(),
									pBuf,
									Project::Tools::WideCharToAnsi(m_vPictureList[i].etype_p.c_str()).c_str(),
									Project::Tools::WideCharToAnsi(m_vPictureList[i].remark.c_str()).c_str(),
									m_vPictureList[i].issystemimage,
									file_size
									);
									fclose(pFile);
									delete pBuf;
								}
							}
							GlobalUnlock(hMemBmp);
						}
						GlobalFree(hMemBmp);
						hMemBmp = NULL;
					}
				}
				break;
			}
		}
		if (!bFind && m_vPictureList[i].ID!=0)
		{
			vecDeleteImgInfo.insert(m_vPictureList[i].ID);
		}
	}
	for (int i=0;i<(int)m_vAnimationList.size();++i)
	{
		bFind = false;
		for (int j=0;j<(int)vecCustomImgInfo.size();++j)
		{
			if (vecCustomImgInfo[j].nImgId == m_vAnimationList[i].ID && vecCustomImgInfo[j].bAnimation)
			{
				bFind = true;
				break;
			}
		}
		if(!bFind && m_vAnimationList[i].ID!=0)
		{
			vecDeleteAnimationId.insert(m_vAnimationList[i].ID);
			for (int n=0;n<(int)m_vAnimationList[i].vecPicIDList.size();++n)
			{
				vecDeleteAnimationImgId.insert(m_vAnimationList[i].vecPicIDList[n]);
			}
		}
	}
}

bool CProjectSqlite::DeleteExcessImage(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	int nSize = (int)vecDeleteImgInfo.size();
	if (nSize > 0)
	{
		sqlstream << "delete from lib_image where ID in(";
		try
		{
			for (set<int>::iterator iter = vecDeleteImgInfo.begin();iter!=vecDeleteImgInfo.end();++iter)
			{
				sqlstream << *iter << ",";
			}
			string sql_ = sqlstream.str();
			sql_ = sql_.substr(0,sql_.length()-1);
			sql_ += ")";
			char* ex_sql = const_cast<char*>(sql_.c_str());

			access.BeginTransaction();
			access.ExecuteCustomSql(ex_sql);
			access.CommitTransaction();
		}
		catch (...)
		{
			return false;
		}
	}
	return true;
}

bool CProjectSqlite::VaccumDatabase(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "vacuum;";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());
	bool bRet = false;

	try
	{
		if (SQLITE_OK == access.ExecuteCustomSql(ex_sql))
		{
			bRet = true;
		}
	}
	catch (...)
	{
		return bRet;
	}

	return bRet;
}

Image* CProjectSqlite::CompressPNG( Image* pBitmapSource, Gdiplus::REAL fWidth, Gdiplus::REAL fHeight )
{
	if (pBitmapSource)
	{
		return static_cast<Image*>(pBitmapSource->GetThumbnailImage(fWidth, fHeight));
	}
	return NULL;
}

bool CProjectSqlite::DeleteAnimation()
{
	bool bRet = false;
	if ((int)vecDeleteAnimationId.size() > 0)
	{
		CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
		char szSQL[2000] = {0};
		strncpy_s(szSQL, sizeof(szSQL), "delete from lib_animation where ID in(", _TRUNCATE);
		for (set<int>::iterator iter = vecDeleteAnimationId.begin();iter!=vecDeleteAnimationId.end();++iter)
		{
			char szTemp[10] = {0};
			_itoa_s(*iter, szTemp, sizeof(szTemp), 10);
			assert(strlen(szTemp) > 0);
			strncat_s(szSQL, sizeof(szSQL), szTemp, _TRUNCATE);
			strncat_s(szSQL, sizeof(szSQL), ",", _TRUNCATE);
		}
		char szSQLFinal[2000] = {0};
		strncpy_s(szSQLFinal, sizeof(szSQLFinal), szSQL, strlen(szSQL)-1);
		strncat_s(szSQLFinal, sizeof(szSQLFinal), ")", _TRUNCATE);
		try
		{
			if(access.ExecuteCustomSql(szSQLFinal) == SQLITE_OK)
			{
				bRet = true;
			}
		}
		catch (...)
		{
			bRet = false;
		}
	}
	return bRet;
}

bool CProjectSqlite::DeleteAnimationImg()
{
	bool bRet = false;
	if ((int)vecDeleteAnimationImgId.size() > 0)
	{
		CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
		char szSQL[2000] = {0};
		strncpy_s(szSQL, sizeof(szSQL), "delete from lib_image_animation where ID in(", _TRUNCATE);
		for (set<int>::iterator iter=vecDeleteAnimationImgId.begin();iter!=vecDeleteAnimationImgId.end();++iter)
		{
			char szTemp[10] = {0};
			_itoa_s(*iter, szTemp, sizeof(szTemp), 10);
			assert(strlen(szTemp) > 0);
			strncat_s(szSQL, sizeof(szSQL), szTemp, _TRUNCATE);
			strncat_s(szSQL, sizeof(szSQL), ",", _TRUNCATE);
		}
		char szSQLFinal[2000] = {0};
		strncpy_s(szSQLFinal, sizeof(szSQLFinal), szSQL, strlen(szSQL)-1);
		strncat_s(szSQLFinal, sizeof(szSQLFinal), ")", _TRUNCATE);
		try
		{
			if(access.ExecuteCustomSql(szSQLFinal) == SQLITE_OK)
			{
				bRet = true;
			}
		}
		catch (...)
		{
			bRet = false;
		}
	}
	return bRet;
}

void CProjectSqlite::LoadProjectSqlite_ListUnit20(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_unit20";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	m_vecTextDevInf.clear();
	m_vecTextAreaInf.clear();
	m_vecCalcModuleInfo.clear();

	vector<CalcModuleItem>					vecCalcItem;
	vector<CalcModuleItem>::const_iterator	iterItem;
	vector<CalcModuleInfo>::iterator		iterModule;

	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		int nUnitProperty1 = access.getColumn_Int(7);
		if (1001 == nUnitProperty1)
		{
			TextDeviceInfo devInf;
			const char* pCo8 = access.getColumn_Text(8);
			if (pCo8 != NULL)
			{
				devInf.strRegion = pCo8;
			}
			devInf.nBindPageId = access.getColumn_Int(9);
			const char* pCo10 = access.getColumn_Text(10);
			if (pCo10 != NULL)
			{
				devInf.strBindPageName = pCo10;
			}
			devInf.nBindTextId = access.getColumn_Int(11);
			m_vecTextDevInf.push_back(devInf);
		}
		else if (1002 == nUnitProperty1)
		{
			TextAreaInfo areaInf;
			areaInf.nPageId = access.getColumn_Int(8);
			const char* pCo9 = access.getColumn_Text(9);
			if (pCo9 != NULL)
			{
				areaInf.strPageName = pCo9;
			}
			const char* pCo10 = access.getColumn_Text(10);
			if (pCo10 != NULL)
			{
				areaInf.strRegion = pCo10;
			}
			m_vecTextAreaInf.push_back(areaInf);
		}
		else if (2001 == nUnitProperty1)
		{
			CalcModuleInfo	calcModuleInfo;
			const char* pCo8 = access.getColumn_Text(8);
			if (pCo8 != NULL)
			{
				calcModuleInfo.strName = pCo8;
			}
			calcModuleInfo.nPeriod = access.getColumn_Int(9);
			calcModuleInfo.nRunState = access.getColumn_Int(10);
			m_vecCalcModuleInfo.push_back(calcModuleInfo);
		}
		else if (2002 == nUnitProperty1)
		{
			CalcModuleItem	calcItem;
			const char* pCo8 = access.getColumn_Text(8);
			if (pCo8 != NULL)
			{
				calcItem.strName = pCo8;
			}
			const char* pCo9 = access.getColumn_Text(9);
			if (pCo9 != NULL)
			{
				calcItem.strContent = pCo9;
			}
			const char* pCo10 = access.getColumn_Text(10);
			if (pCo10 != NULL)
			{
				calcItem.strParentName = pCo10;
			}
			vecCalcItem.push_back(calcItem);
		}
	}
	access.SqlFinalize();

	for (iterModule=m_vecCalcModuleInfo.begin(); iterModule!=m_vecCalcModuleInfo.end(); iterModule++)
	{
		const CString strModuleName(iterModule->strName);
		for (iterItem=vecCalcItem.begin(); iterItem!=vecCalcItem.end(); iterItem++)
		{
			if (strModuleName == iterItem->strParentName)
			{
				(iterModule->vecItem).push_back(*iterItem);
			}
		}
	}
}

bool CProjectSqlite::SaveProjectSqlite_ListUnit20(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	vector<TextDeviceInfo>::const_iterator	iterDev;
	vector<TextAreaInfo>::const_iterator	iterArea;
	char szUnit02[MAX_PATH]	= {0};
	char szUnit03[MAX_PATH]	= {0};
	char szUnit04[MAX_PATH]	= {0};
	char szUnit05[MAX_PATH]	= {0};

	access.BeginTransaction();
	access.DeleteAllFromListUnit20DeviceArea();

	USES_CONVERSION;
	for (iterDev=m_vecTextDevInf.begin(); iterDev!=m_vecTextDevInf.end(); iterDev++)
	{
		sprintf_s(szUnit02, MAX_PATH, "%s", T2A(iterDev->strRegion));
		sprintf_s(szUnit03, MAX_PATH, "%d", iterDev->nBindPageId);
		sprintf_s(szUnit04, MAX_PATH, "%s", T2A(iterDev->strBindPageName));
		sprintf_s(szUnit05, MAX_PATH, "%d", iterDev->nBindTextId);
		access.InsertOneIntoListUnit20DeviceArea(1001, szUnit02, szUnit03, szUnit04, szUnit05);
	}
	for (iterArea=m_vecTextAreaInf.begin(); iterArea!=m_vecTextAreaInf.end(); iterArea++)
	{
		sprintf_s(szUnit02, MAX_PATH, "%d", iterArea->nPageId);
		sprintf_s(szUnit03, MAX_PATH, "%s", T2A(iterArea->strPageName));
		sprintf_s(szUnit04, MAX_PATH, "%s", T2A(iterArea->strRegion));
		access.InsertOneIntoListUnit20DeviceArea(1002, szUnit02, szUnit03, szUnit04, "");
	}

	if(access.CommitTransaction() != 0)
		return false;
	else
		return true;
}

void CProjectSqlite::SaveProjectSqlite_CalcModuleInfo(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	vector<CalcModuleInfo>::iterator	iter;
	char szUnit02[MAX_PATH]	= {0};
	char szUnit03[MAX_PATH]	= {0};
	char szUnit04[MAX_PATH]	= {0};
	char szUnit05[MAX_PATH]	= {0};
	char szUnit06[MAX_PATH]	= {0};
	char szContent[nCalcModContentLen] = {0};

	access.BeginTransaction();
	access.DeleteAllFromListUnit20CalcModuleInfo();

	vector<CalcModuleItem>				vecCalcItem;
	vector<CalcModuleItem>::iterator	iterItem;
	for (iter=m_vecCalcModuleInfo.begin(); iter!=m_vecCalcModuleInfo.end(); iter++)
	{
		for (iterItem=iter->vecItem.begin(); iterItem!=iter->vecItem.end(); iterItem++)
		{
			vecCalcItem.push_back(*iterItem);
		}
	}

	USES_CONVERSION;
	for (iter=m_vecCalcModuleInfo.begin(); iter!=m_vecCalcModuleInfo.end(); iter++)
	{
		sprintf_s(szUnit02, MAX_PATH, "%s", T2A(iter->strName));
		sprintf_s(szUnit03, MAX_PATH, "%d", iter->nPeriod);
		sprintf_s(szUnit04, MAX_PATH, "%d", iter->nRunState);
		memset(szUnit05, 0, MAX_PATH);
		memset(szUnit06, 0, MAX_PATH);
		access.InsertOneIntoListUnit20CalcModule(2001, szUnit02, szUnit03, szUnit04, szUnit05, szUnit06);
	}

	for (iterItem=vecCalcItem.begin(); iterItem!=vecCalcItem.end(); iterItem++)
	{
		sprintf_s(szUnit02, MAX_PATH, "%s", T2A(iterItem->strName));
		sprintf_s(szContent, nCalcModContentLen, "%s", T2A(iterItem->strContent));
		sprintf_s(szUnit04, MAX_PATH, "%s", T2A(iterItem->strParentName));
		memset(szUnit05, 0, MAX_PATH);
		memset(szUnit06, 0, MAX_PATH);
		access.InsertOneIntoListUnit20CalcModule(2002, szUnit02, szContent, szUnit04, szUnit05, szUnit06);
	}

	access.CommitTransaction();
}

void CProjectSqlite::SaveProjectSqlite_CalcModuleInfo_Dec(void)
{
	wstring wstrDecPath(m_strFilePath + L".dec");
	CSqliteAcess access(wstrDecPath.c_str(), false);
	vector<CalcModuleInfo>::iterator	iter;
	char szUnit02[MAX_PATH]	= {0};
	char szUnit03[MAX_PATH]	= {0};
	char szUnit04[MAX_PATH]	= {0};
	char szUnit05[MAX_PATH]	= {0};
	char szUnit06[MAX_PATH]	= {0};
	char szContent[nCalcModContentLen] = {0};

	access.BeginTransaction();
	access.DeleteAllFromListUnit20CalcModuleInfo();

	vector<CalcModuleItem>				vecCalcItem;
	vector<CalcModuleItem>::iterator	iterItem;
	for (iter=m_vecCalcModuleInfo.begin(); iter!=m_vecCalcModuleInfo.end(); iter++)
	{
		for (iterItem=iter->vecItem.begin(); iterItem!=iter->vecItem.end(); iterItem++)
		{
			vecCalcItem.push_back(*iterItem);
		}
	}

	USES_CONVERSION;
	for (iter=m_vecCalcModuleInfo.begin(); iter!=m_vecCalcModuleInfo.end(); iter++)
	{
		sprintf_s(szUnit02, MAX_PATH, "%s", T2A(iter->strName));
		sprintf_s(szUnit03, MAX_PATH, "%d", iter->nPeriod);
		sprintf_s(szUnit04, MAX_PATH, "%d", iter->nRunState);
		memset(szUnit05, 0, MAX_PATH);
		memset(szUnit06, 0, MAX_PATH);
		access.InsertOneIntoListUnit20CalcModule(2001, szUnit02, szUnit03, szUnit04, szUnit05, szUnit06);
	}

	for (iterItem=vecCalcItem.begin(); iterItem!=vecCalcItem.end(); iterItem++)
	{
		sprintf_s(szUnit02, MAX_PATH, "%s", T2A(iterItem->strName));
		sprintf_s(szContent, nCalcModContentLen, "%s", T2A(iterItem->strContent));
		sprintf_s(szUnit04, MAX_PATH, "%s", T2A(iterItem->strParentName));
		memset(szUnit05, 0, MAX_PATH);
		memset(szUnit06, 0, MAX_PATH);
		access.InsertOneIntoListUnit20CalcModule(2002, szUnit02, szContent, szUnit04, szUnit05, szUnit06);
	}

	access.CommitTransaction();
}

void CProjectSqlite::InsertOneIntoCalcMod(CString strName, const int nCycle, const bool bIsEnable, const bool bSyncDec)
{
	char szName[MAX_PATH] = {0};

	USES_CONVERSION;
	sprintf_s(szName, MAX_PATH, "%s", T2A(strName));

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.InsertOneIntoCalcModule(szName, nCycle, bIsEnable);
	access.CommitTransaction();

	if (bSyncDec)
	{
		CSqliteAcess access(m_strFileDecPath.c_str(), false);
		access.BeginTransaction();
		access.InsertOneIntoCalcModule(szName, nCycle, bIsEnable);
		access.CommitTransaction();
	}
}

void CProjectSqlite::InsertOneIntoCalcModItem(CString strName, CString strParentName, CString strContent, const bool bSyncDec)
{
	char szName[MAX_PATH] = {0};
	char szPaName[MAX_PATH] = {0};
	char szContent[nCalcModContentLen] = {0};

	strContent.Replace(_T("\""), _T("\'"));

	USES_CONVERSION;
	sprintf_s(szName, MAX_PATH, "%s", T2A(strName));
	sprintf_s(szPaName, MAX_PATH, "%s", T2A(strParentName));
	sprintf_s(szContent, nCalcModContentLen, "%s", T2A(strContent));

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.InsertOneIntoCalcModuleItem(szName, szPaName, szContent);
	access.CommitTransaction();

	if (bSyncDec)
	{
		CSqliteAcess access(m_strFileDecPath.c_str(), false);
		access.BeginTransaction();
		access.InsertOneIntoCalcModuleItem(szName, szPaName, szContent);
		access.CommitTransaction();
	}
}

void CProjectSqlite::DeleteOneFromCalcMod(CString strName, const bool bSyncDec)
{
	char szName[MAX_PATH] = {0};

	USES_CONVERSION;
	sprintf_s(szName, MAX_PATH, "%s", T2A(strName));

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.DeleteOneFromCalcModule(szName);
	access.CommitTransaction();

	if (bSyncDec)
	{
		CSqliteAcess access(m_strFileDecPath.c_str(), false);
		access.BeginTransaction();
		access.DeleteOneFromCalcModule(szName);
		access.CommitTransaction();
	}
}

void CProjectSqlite::DeleteOneFromCalcModItem(CString strName, CString strParentName, const bool bSyncDec)
{
	char szName[MAX_PATH] = {0};
	char szPaName[MAX_PATH] = {0};

	USES_CONVERSION;
	sprintf_s(szName, MAX_PATH, "%s", T2A(strName));
	sprintf_s(szPaName, MAX_PATH, "%s", T2A(strParentName));

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.DeleteOneFromCalcModuleItem(szName, szPaName);
	access.CommitTransaction();

	if (bSyncDec)
	{
		CSqliteAcess access(m_strFileDecPath.c_str(), false);
		access.BeginTransaction();
		access.DeleteOneFromCalcModuleItem(szName, szPaName);
		access.CommitTransaction();
	}
}

void CProjectSqlite::UpdateCalcMod(CString strName, const int nCycle, const bool bSyncDec)
{
	char szName[MAX_PATH] = {0};

	USES_CONVERSION;
	sprintf_s(szName, MAX_PATH, "%s", T2A(strName));

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.UpdateCalcModule(szName, nCycle);
	access.CommitTransaction();

	if (bSyncDec)
	{
		CSqliteAcess access(m_strFileDecPath.c_str(), false);
		access.BeginTransaction();
		access.UpdateCalcModule(szName, nCycle);
		access.CommitTransaction();
	}
}

void CProjectSqlite::UpdateCalcModItem(CString strName, CString strParentName, CString strContent, const bool bSyncDec)
{
	char szName[MAX_PATH] = {0};
	char szPaName[MAX_PATH] = {0};
	char szContent[nCalcModContentLen] = {0};

	strContent.Replace(_T("\""), _T("\'"));

	USES_CONVERSION;
	sprintf_s(szName, MAX_PATH, "%s", T2A(strName));
	sprintf_s(szPaName, MAX_PATH, "%s", T2A(strParentName));
	sprintf_s(szContent, nCalcModContentLen, "%s", T2A(strContent));

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.UpdateCalcModuleItem(szName, szPaName, szContent);
	access.CommitTransaction();

	if (bSyncDec)
	{
		CSqliteAcess access(m_strFileDecPath.c_str(), false);
		access.BeginTransaction();
		access.UpdateCalcModuleItem(szName, szPaName, szContent);
		access.CommitTransaction();
	}
}

void CProjectSqlite::EnableCalcMod(CString strName, const bool bIsEnable, const bool bSyncDec)
{
	char szName[MAX_PATH] = {0};

	USES_CONVERSION;
	sprintf_s(szName, MAX_PATH, "%s", T2A(strName));

	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	access.BeginTransaction();
	access.EnableCalcModule(szName, bIsEnable);
	access.CommitTransaction();

	if (bSyncDec)
	{
		CSqliteAcess access(m_strFileDecPath.c_str(), false);
		access.BeginTransaction();
		access.EnableCalcModule(szName, bIsEnable);
		access.CommitTransaction();
	}
}

bool CProjectSqlite::SynchronousPlaneTempeDistr(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

    const list<CEqmDrawText*>& textlist = m_pPage->GetPageTextList();
    if (textlist.empty())
	{
        return true;
	}

	access.BeginTransaction();

	USES_CONVERSION;
	list<CEqmDrawText*>::const_iterator it = textlist.begin();
    for (; it != textlist.end(); it++)
    {
		if ((*it)->GetNeedSave())
		{
			CString	strTemperCfg((*it)->GetTemperCfg());
			if (strTemperCfg != _T(""))
			{	// synchronous
				Gdiplus::PointF pos = (*it)->GetPos();
				int nX = pos.X;
				int nY = pos.Y;
				int nWidth = (*it)->GetWidth();
				int nHeight = (*it)->GetHeight();
				int nLayer = (*it)->GetLayer();
				int nId = (*it)->GetID();
				int nPageId = (*it)->GetPageID();
				CString strUnit01 = _T("planeTempeDistr");
				bool bFind = false;

				// find if exist EQMDRAW_PLANE_TEMPE_DISTR
				vector<CEqmDrawPage*> vecPage(m_project.GetPageList());
				vector<CEqmDrawPage*>::const_iterator			iterPage;
				list<CEqmDrawPlaneTempeDistr*>::const_iterator	iterPtd;
				for (iterPage=vecPage.begin(); iterPage!=vecPage.end(); iterPage++)
				{
					if (bFind)
					{
						break;
					}

					list<CEqmDrawPlaneTempeDistr*> listPtd((*iterPage)->m_planeTempeDistrlist);
					for (iterPtd=listPtd.begin(); iterPtd!=listPtd.end(); iterPtd++)
					{
						if ((*iterPtd)->GetPageID() == nPageId)
						{
							bFind = true;
							break;
						}
					}
				}

				if (!bFind)
				{
					// add into memory
					CEqmDrawPlaneTempeDistr* pPlaneTempeDistr = new CEqmDrawPlaneTempeDistr();
					if (pPlaneTempeDistr != NULL)
					{
						pPlaneTempeDistr->SetPos(pos);
						pPlaneTempeDistr->SetEqmType(EQMDRAW_PLANE_TEMPE_DISTR);
						pPlaneTempeDistr->SetPageID(nPageId);
						pPlaneTempeDistr->SetID(nId);
						pPlaneTempeDistr->SetWidth(nWidth);
						pPlaneTempeDistr->SetHeight(nHeight);
						pPlaneTempeDistr->SetLayer(nLayer);
						pPlaneTempeDistr->SetPointName(strTemperCfg);
						pPlaneTempeDistr->SetNeedSave(false);
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\PlaneTempeDistr.png");
						if (pBitmap != NULL)
						{
							pPlaneTempeDistr->SetDefaultBitmap(pBitmap);
						}
						m_pPage->AddPlaneTempeDistr(pPlaneTempeDistr);
					}

					// add into database
					const int c_nLen = 100000;
					char* pPtName = new char[c_nLen];
					memset(pPtName, 0, c_nLen);
					sprintf_s(pPtName, c_nLen, "%s", T2A(strTemperCfg));
					access.InsertOneIntoPlaneTempeDistr(nPageId, nId, nX, nY, nWidth, nHeight, nLayer, pPtName, 0);
					SAFE_DELETE_ARRAY(pPtName);
				}
			}
		}
    }

	if(access.CommitTransaction() != 0)
	{
		return false;
	}
	return true;
}

PictureInfo* CProjectSqlite::GetPictureInfo(const wchar_t* pName, const int nSize, const wstring strRemark, PictureInfo* picInfo)
{
	PictureInfo* pPicInf = NULL;
	vector<PictureInfo *> pPicInfSameList;
	std::vector<PictureInfo>::iterator iter;
	for (iter=m_vPictureList.begin(); iter!=m_vPictureList.end(); iter++)
	{
		int nWidth1 = iter->pBitmap->GetWidth();
		int nHeight1 = iter->pBitmap->GetHeight();
		int nWidth2 = picInfo->pBitmap->GetWidth();
		int nHeight2 = picInfo->pBitmap->GetHeight();

		PixelFormat pf2 = picInfo->pBitmap->GetPixelFormat();
		PixelFormat pf1 = iter->pBitmap->GetPixelFormat();

		int pp2 = picInfo->pBitmap->GetPaletteSize();
		int pp1 = iter->pBitmap->GetPaletteSize();


		if (false)//0 == wcscmp(iter->szName, pName) && iter->nSize == nSize /*&& iter->remark == strRemark*/)
		{
			pPicInf = &(*iter);
			pPicInfSameList.push_back(&(*iter));
			//break;
		}
	}
	return pPicInf;
}

bool CProjectSqlite::DeleteImageAll(void)
{
	bool bRet = false;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	char szSQL[MAX_PATH] = {0};
	strncpy_s(szSQL, sizeof(szSQL), "delete from lib_image;", _TRUNCATE);
	try
	{
		if (SQLITE_OK == access.ExecuteCustomSql(szSQL))
		{
			bRet = true;
		}
	}
	catch (...)
	{
		bRet = false;
	}

	return bRet;
}

bool CProjectSqlite::DeleteAnimationAll(void)
{
	bool bRet = false;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	char szSQL[MAX_PATH] = {0};
	strncpy_s(szSQL, sizeof(szSQL), "delete from lib_animation;", _TRUNCATE);
	try
	{
		if (SQLITE_OK == access.ExecuteCustomSql(szSQL))
		{
			bRet = true;
		}
	}
	catch (...)
	{
		bRet = false;
	}

	return bRet;
}

bool CProjectSqlite::DeleteImageAnimationAll(void)
{
	bool bRet = false;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	char szSQL[MAX_PATH] = {0};
	strncpy_s(szSQL, sizeof(szSQL), "delete from lib_image_animation;", _TRUNCATE);
	try
	{
		if (SQLITE_OK == access.ExecuteCustomSql(szSQL))
		{
			bRet = true;
		}
	}
	catch (...)
	{
		bRet = false;
	}

	return bRet;
}

bool CProjectSqlite::ExecuteCustomSqlForSqlite(const char* pSql)
{
	bool bRet = false;
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	try
	{
		if (SQLITE_OK == access.ExecuteCustomSql(pSql))
		{
			bRet = true;
		}
	}
	catch (...)
	{
		bRet = false;
	}

	return bRet;
}

vector<VersionInfo> CProjectSqlite::LoadVersionInfo(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "SELECT unitproperty01, unitproperty02, unitproperty03, content FROM log WHERE type = 1 ORDER BY (CAST (unitproperty01 AS int)) ASC;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	USES_CONVERSION;
	vector<VersionInfo> vecVi;
	while (true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		VersionInfo vi;
		vi.nVersion = access.getColumn_Int(0);

		char* pUser = const_cast<char*>(access.getColumn_Text(1));
		if (pUser != NULL)
		{
			(vi.strUser).Format(_T("%s"), A2T(pUser));
		}

		char* pTime = const_cast<char*>(access.getColumn_Text(2));
		if (pTime != NULL)
		{
			(vi.strTime).Format(_T("%s"), A2T(pTime));
		}

		char* pLog = const_cast<char*>(access.getColumn_Text(3));
		if (pLog != NULL)
		{
			(vi.strLog).Format(_T("%s"), A2T(pLog));
		}

		vecVi.push_back(vi);
	}
	access.SqlFinalize();
	return vecVi;
}

bool CProjectSqlite::InsertIntoOneVersion(int nVersion, CString strUser, CString strTime, CString strLog)
{
	USES_CONVERSION;
	char szSql[5000] = {0};
	sprintf_s(szSql, 5000, "INSERT INTO log(unitproperty01, unitproperty02, unitproperty03, content, type) VALUES(%d, '%s', '%s', '%s', 1);",
		nVersion, T2A(strUser), T2A(strTime), T2A(strLog));
	return ExecuteCustomSqlForSqlite(szSql);
}

int CProjectSqlite::ReadLastVersionNumber(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "SELECT max(CAST (unitproperty01 AS int)) FROM log;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	int nLastNum = 0;

	while (true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		nLastNum = access.getColumn_Int(0);
		break;
	}
	access.SqlFinalize();
	return nLastNum;
}


bool	CProjectSqlite::SaveSystemDefineConfig(wstring wstrConfig)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	sqlstream << "CREATE TABLE IF NOT EXISTS local_config(name varchar, content varchar)";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "delete from local_config where name = 'globalconfig';";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	//////////////////////////////////////////////////////////////////////////
	//更新sql
	string strConfig =  UtilString::ConvertWideCharToMultiByte(wstrConfig);
	ostringstream sqlstream_set;
	sqlstream_set << "insert into local_config(name, content) values( 'globalconfig', '"<< strConfig<<"');";
	sql_ = sqlstream_set.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	if (re == SQLITE_OK)
	{
		return true;
	}
	else
	{
		OutputSaveErr(L"update globalconfig",access.GetSqliteErr());
		return false;
	}

}








wstring CProjectSqlite::GetValueInLocalConfigTable(wstring strKey)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	int tempInt = 0;
	CAlarmDataPointEntry entry;
	CString cstr;
	const char* pchar = NULL;
	wstring wstr = _T("");
	ALARMTYPE alarmtype = ALARMTYPE_None;
	int i = 1;

	string strKey_utf8;
	Project::Tools::WideCharToUtf8(strKey, strKey_utf8);

	ostringstream sqlstream;
	sqlstream << "select content from local_config where name = '"<< strKey_utf8  <<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		entry.Clear();  //reset entry

		if (access.getColumn_Text(0) != NULL)
		{
			string strContetn(access.getColumn_Text(0));
			wstr = Project::Tools::AnsiToWideChar(strContetn.c_str());

		}

	}

	access.SqlFinalize();

	return wstr;
}

wstring CProjectSqlite::GetGlobalConfig(CString strTemplateFileName,wstring strKey)
{

	/*    */
	CString strTemplatePath;
	wstring wstr = _T("");
	CString strDir;
	TCHAR szAppDir[MAX_PATH] = {0};
	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	PathRemoveFileSpec(szAppDir);
	CString strFilePath;
	strFilePath.Format(_T("%s\\template\\%s"), szAppDir,strTemplateFileName);
	string strKey_utf8;
	Project::Tools::WideCharToUtf8(strKey, strKey_utf8);
	CSqliteAcess access(strFilePath, m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select content from local_config where name = '"<< strKey_utf8  <<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	//CPageTemplateInfo  pt;  
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (access.getColumn_Text(0) != NULL)
		{
			string strContetn(access.getColumn_Text(0));
			wstr = Project::Tools::AnsiToWideChar(strContetn.c_str());

		}

	}
	access.SqlFinalize();
	return wstr;
}

bool  CProjectSqlite::SetValueInLocalConfigTable(wstring strKey, wstring strValue)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	sqlstream << "CREATE TABLE IF NOT EXISTS local_config(name varchar, content varchar)";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	string strKey_utf8=Project::Tools::WideCharToAnsi(strKey.c_str());

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "delete from local_config where name = '" << strKey_utf8 <<"';";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	//////////////////////////////////////////////////////////////////////////
	//更新sql
	string strConfig =  UtilString::ConvertWideCharToMultiByte(strValue);
	ostringstream sqlstream_set;
	sqlstream_set << "insert into local_config(name, content) values( '"<<   strKey_utf8 <<"', '"<< strConfig<<"');";
	sql_ = sqlstream_set.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	if (re == SQLITE_OK)
	{
		return true;
	}
	else
	{
		OutputSaveErr(L"update SetValueInLocalConfigTable",access.GetSqliteErr());
		return false;
	}

}

bool CProjectSqlite::SetValueInLocalConfigTable(wstring strKey, string strValue)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	sqlstream << "CREATE TABLE IF NOT EXISTS local_config(name varchar, content varchar)";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	string strKey_utf8=Project::Tools::WideCharToAnsi(strKey.c_str());

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "delete from local_config where name = '" << strKey_utf8 <<"';";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	//////////////////////////////////////////////////////////////////////////
	//更新sql
	string strConfig = strValue;
	ostringstream sqlstream_set;
	sqlstream_set << "insert into local_config(name, content) values( '"<<   strKey_utf8 <<"', '"<< strConfig<<"');";
	sql_ = sqlstream_set.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	if (re == SQLITE_OK)
	{
		return true;
	}
	else
	{
		OutputSaveErr(L"update SetValueInLocalConfigTable",access.GetSqliteErr());
		return false;
	}
}

bool  CProjectSqlite::RemoveValueInLocalConfigTable(wstring strKey)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	string strKey_utf8 = Project::Tools::WideCharToAnsi(strKey.c_str());

	


	sqlstream << "delete from local_config where name = '" << strKey_utf8 <<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	if (re == SQLITE_OK)
	{
		return true;
	}
	else
	{
		OutputSaveErr(L"update SetValueInLocalConfigTable",access.GetSqliteErr());
		return false;
	}

}

bool  CProjectSqlite::ModifyValueInLocalConfigTable(wstring strOldKey,wstring strKey, wstring strValue)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	string strKey_utf8=Project::Tools::WideCharToAnsi(strKey.c_str());

	string strValue_utf8= Project::Tools::WideCharToAnsi(strValue.c_str());

	string strOldValue_utf8=Project::Tools::WideCharToAnsi(strOldKey.c_str());

	sqlstream << "update local_config set name = '"<< strKey_utf8 <<"' ,content = '"<< strValue_utf8 <<"'where name ='"<< strOldValue_utf8 <<"'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	if (re == SQLITE_OK)
	{
		return true;
	}
	else
	{
		OutputSaveErr(L"update ModifyValueInLocalConfigTable",access.GetSqliteErr());
		return false;
	}

}


bool   CProjectSqlite::GetAllItemsInLocalConfig(vector<wstring> &wstrNameList,vector<wstring> &wstrValueList)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	int tempInt = 0;
	CAlarmDataPointEntry entry;
	CString cstr;
	const char* pchar = NULL;
	wstring wstrName = _T("");
	wstring wstrContent = _T("");
	ALARMTYPE alarmtype = ALARMTYPE_None;
	int i = 1;


	ostringstream sqlstream;
	sqlstream << "select name,content from local_config ";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		entry.Clear();  //reset entry

		if (access.getColumn_Text(0) != NULL)
		{
			string strName(access.getColumn_Text(0));
			string strContent(access.getColumn_Text(1));
			wstrName = Project::Tools::AnsiToWideChar(strName.c_str());
			wstrContent = Project::Tools::AnsiToWideChar(strContent.c_str());
			wstrNameList.push_back(wstrName);
			wstrValueList.push_back(wstrContent);

		}

	}

	access.SqlFinalize();
	return true;
	
}






wstring CProjectSqlite::GetSystemDefineConfig()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	int tempInt = 0;
	CAlarmDataPointEntry entry;
	CString cstr;
	const char* pchar = NULL;
	wstring wstr = _T("");
	ALARMTYPE alarmtype = ALARMTYPE_None;
	int i = 1;

	ostringstream sqlstream;
	sqlstream << "select content from local_config where name = 'globalconfig';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		entry.Clear();  //reset entry

		if (access.getColumn_Text(0) != NULL)
		{
			string strContetn(access.getColumn_Text(0));
			wstr = UtilString::ConvertMultiByteToWideChar(strContetn);

		}

	}

	access.SqlFinalize();

	return wstr;
}


bool	CProjectSqlite::SaveModbusComConfig(wstring wstrConfig)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	sqlstream << "CREATE TABLE IF NOT EXISTS local_config(name varchar, content varchar)";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "delete from local_config where name = 'modbusclientconfig';";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);
	
	//////////////////////////////////////////////////////////////////////////
	//更新sql
	string strConfig =  UtilString::ConvertWideCharToMultiByte(wstrConfig);
	ostringstream sqlstream_set;
	sqlstream_set << "insert into local_config(name, content) values( 'modbusclientconfig', '"<< strConfig<<"');";
	sql_ = sqlstream_set.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	if (re == SQLITE_OK)
	{
		return true;
	}
	else
	{
		OutputSaveErr(L"update SaveModbusComConfig",access.GetSqliteErr());
		return false;
	}

}

wstring CProjectSqlite::GetModbusComConfig()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	int tempInt = 0;
	CAlarmDataPointEntry entry;
	CString cstr;
	const char* pchar = NULL;
	wstring wstr = _T("");
	ALARMTYPE alarmtype = ALARMTYPE_None;
	int i = 1;

	ostringstream sqlstream;
	sqlstream << "select content from local_config where name = 'modbusclientconfig';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		entry.Clear();  //reset entry

		if (access.getColumn_Text(0) != NULL)
		{
			string strContetn(access.getColumn_Text(0));
			wstr = UtilString::ConvertMultiByteToWideChar(strContetn);
			
		}
		
	}

	access.SqlFinalize();

	return wstr;
}

bool	CProjectSqlite::SaveBackupRealtimePointValuesStatus(vector<Beopdatalink::CRealTimeDataEntry> & tempEntryList)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	sqlstream << "CREATE TABLE IF NOT EXISTS backup_realtimedata_input(pointtime TIMESTAMP NOT NULL DEFAULT current_timestamp,pointname varchar(255), pointvalue varchar(2560))";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "delete from backup_realtimedata_input;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	access.BeginTransaction();
	for(int i=0; i<tempEntryList.size(); i++)
	{
		SYSTEMTIME mytime = tempEntryList[i].GetTimestamp();
		string strPointTime = Project::Tools::SystemTimeToString(mytime);
		string strPointName = tempEntryList[i].GetName();
		string strPointValue = UtilString::ConvertWideCharToMultiByte(tempEntryList[i].GetValue()) ;
		sqlstream.str("");
		sqlstream << "insert into backup_realtimedata_input(pointtime,pointname,pointvalue)  values('" <<strPointTime << "','" << strPointName << "','"<< strPointValue <<"')";
		string sql_in = sqlstream.str();
		access.SqlExe(sql_in.c_str());
	}
	access.CommitTransaction();
	access.SqlFinalize();
	return true;
}


bool	CProjectSqlite::SaveBackupLogicStatus(vector<CString> & strThreadNameList, vector<int> & nStatusList, vector<int> &nTimePeriodList)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	sqlstream << "CREATE TABLE IF NOT EXISTS backup_logic_status(threadname varchar(255),threadstatus int, threadtimespan int)";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);
	
	//插入表数据
	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "delete from backup_logic_status;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);

	access.BeginTransaction();
	for(int i=0; i<strThreadNameList.size(); i++)
	{
		string strThreadName = UtilString::ConvertWideCharToMultiByte(strThreadNameList[i].GetString());
		sqlstream.str("");
		sqlstream << "insert into backup_logic_status(threadname,threadstatus,threadtimespan)  values('" <<strThreadName << "'," << nStatusList[i] << ","<< nTimePeriodList[i] <<")";
		string sql_in = sqlstream.str();
		access.SqlExe(sql_in.c_str());
	}
	access.CommitTransaction();
	access.SqlFinalize();
	return true;
}


bool	CProjectSqlite::GetBackupRealtimePointValuesStatus(vector<Beopdatalink::CRealTimeDataEntry> & tempEntryList)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	sqlstream << "CREATE TABLE IF NOT EXISTS backup_realtimedata_input(pointtime TIMESTAMP NOT NULL DEFAULT current_timestamp,pointname varchar(255), pointvalue varchar(2560))";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "select pointtime,pointname,pointvalue from backup_realtimedata_input;";
	 sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	 re = access.SqlQuery(ex_sql);


	tempEntryList.clear();

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (access.getColumn_Text(0) != NULL)
		{
			CRealTimeDataEntry dataEntry;


			string strContetn(access.getColumn_Text(0));
			SYSTEMTIME st;
			Project::Tools::String_To_SysTime(strContetn, st);
			dataEntry.SetTimestamp(st);

			 strContetn = (access.getColumn_Text(1));
			dataEntry.SetName(strContetn);

			 strContetn = (access.getColumn_Text(2));
			wstr = UtilString::ConvertMultiByteToWideChar(strContetn);
			dataEntry.SetValue(wstr);

			tempEntryList.push_back(dataEntry);
		}

	}

	access.SqlFinalize();

	return true;
}

int    CProjectSqlite::GetMaxIDInReportTable()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	ostringstream sqlstream;

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "select max(id)  from template_files";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());

	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int nMaxID = 0;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (access.getColumn_Text(0) != NULL)
		{

			nMaxID = access.getColumn_Int(0);
		}

	}

	access.SqlFinalize();

	return nMaxID;
}
bool	CProjectSqlite::GetReportInfo(vector<int>& nidList,vector<CString>& strReportNameList, vector<CString>& strFileDescriptionList,vector<int> &nReportTypeList)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "CREATE TABLE IF NOT EXISTS template_files(id int,file_name TEXT, file_binary TEXT,file_time TEXT,file_description TEXT,reserve01 TEXT,reserve02 TEXT,reserve03 TEXT,reserve04 TEXT,reserve05 TEXT)";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "select id,file_name ,file_binary ,file_description,reserve01 from template_files;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);


	nidList.clear();
	strReportNameList.clear();
	strFileDescriptionList.clear();
	nReportTypeList.clear();

	while(true)
	{
		int nID = -1;
		int nFileType = -1;
		CString strFileName;
		CString strFileDescription;
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(0) != NULL)
		{
			nID = access.getColumn_Int(0);
		}
		else
		{
			continue;
		}


		if (access.getColumn_Text(1) != NULL)
		{
			wstring wstr =UtilString::ConvertMultiByteToWideCharUTF8(access.getColumn_Text(1));
			CString str( wstr.c_str() );
			strFileName = str;
		}
		else
		{
			continue;
		}

		if (access.getColumn_Text(3) != NULL)
		{
			wstring wstr =UtilString::ConvertMultiByteToWideCharUTF8(access.getColumn_Text(3));
			CString str( wstr.c_str() );
			strFileDescription = str;
		}
		else
		{
			strFileDescription = _T("");
		}



		if(access.getColumn_Text(4) != NULL)
		{
			nFileType = access.getColumn_Int(4);
		}
		else
		{
			nFileType = 0;
		}



		nidList.push_back(nID);
		nReportTypeList.push_back(nFileType);
		strFileDescriptionList.push_back(strFileDescription);
		strReportNameList.push_back(strFileName);


	}

	access.SqlFinalize();

	return true;

}
bool	CProjectSqlite::AddReportInfo(int nid,CString cstrReportName,CString cstrReportTime,CString cstrReportPath,CString cstrReportDescr,int nFileType, int nTimeType, CString strTimeConfig)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	int rc=0;
	FILE *fp;
	long filesize = 0;
	char * ffile;
	_bstr_t tem_des = cstrReportPath.GetString();
	char* des = (char*)tem_des;
	fopen_s(&fp,des, "rb");
	if(fp==NULL)
	{
		return false;
	}
	
	if(fp != NULL)
	{
		//计算文件的大小
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		//读取文件
		ffile = new char[filesize+1];
		size_t sz = fread(ffile, sizeof(char), filesize, fp);
		fclose(fp);
	}
	sqlstream << "CREATE TABLE IF NOT EXISTS template_files(id int,file_name TEXT, file_binary TEXT,file_time TEXT,file_description TEXT,reserve01 TEXT,reserve02 TEXT,reserve03 TEXT,reserve04 TEXT,reserve05 TEXT)";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);
	
    string strReportName = UtilString::ConvertWideCharToMultiByteUTF8(cstrReportName.GetString());
	string strReportTime = UtilString::ConvertWideCharToMultiByte(cstrReportTime.GetString());
	string strDes =  UtilString::ConvertWideCharToMultiByte(cstrReportDescr.GetString());
	string strConfig =  UtilString::ConvertWideCharToMultiByte(strTimeConfig.GetString());
	sqlstream.str("");
	sqlstream << "insert into template_files(id,file_name,file_time,file_description,reserve01,reserve02,reserve03,file_binary) values('" 
		<<nid << "','" << strReportName << "','"<< strReportTime << "','"<< strDes <<" ','"<< nFileType <<"','"<< nTimeType <<"','"<< strConfig <<"',?)"; //@tobefix

	

	string sql_in = sqlstream.str();
	
	if (SQLITE_OK == access.SqlQuery(sql_in.c_str()))
	{
		if (access.m_stmt)
		{
			rc = access.SqlBind_Blob(ffile,1,filesize);
			assert(0 == rc);
			rc = access.SqlNext();
			//assert(0 == rc);
			rc = access.SqlFinalize();
			assert(0 == rc);
		}
	}
	else
	{
		return false;
	}


	int nCommitResult = access.CommitTransaction();

	return true;
}



bool	CProjectSqlite::ModifyReportInfo(int nId,CString strReportName)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	
	USES_CONVERSION;
	string str(W2A(strReportName));
	
	sqlstream <<"update template_files set file_name = '"<< str <<"' where id ='"<< nId <<"'";

	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	if (re == SQLITE_OK)
	{
		return true;
	}
	else
	{
		OutputSaveErr(L"update template_files",access.GetSqliteErr());
		return false;
	}

	return true;

}


bool   CProjectSqlite::DeleteReportInfo(int nid)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	sqlstream << "CREATE TABLE IF NOT EXISTS template_files(id int,file_name TEXT, file_binary TEXT,file_time TEXT)";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "delete from template_files where id = '" << nid <<"';";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlExe(ex_sql);
	access.SqlFinalize();
	return true;

}
int       CProjectSqlite::GetReportContentSize(int nid)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select file_binary from template_files where id = '" << nid <<"';";

	
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);
	int nSize;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(2) != NULL)
		{
			nSize = access.getColumn_Bytes(2);	
		}
		
	}
	access.SqlFinalize();

	
	return nSize;
}






bool CProjectSqlite::GetBackupLogicStatus(vector<CString> & strThreadNameList, vector<int> & nStatusList, vector<int> &nTimePeriodList)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;

	sqlstream << "CREATE TABLE IF NOT EXISTS backup_logic_status(threadname varchar(255),threadstatus int, threadtimespan int)";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "select * from backup_logic_status;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);

	strThreadNameList.clear();
	nStatusList.clear();
	nTimePeriodList.clear();

	while(true)
	{
		CString strThreadName;
		int nStatus;
		int nTimePeriod;

		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		if (access.getColumn_Text(0) != NULL)
		{
			wstring wstr =UtilString::ConvertMultiByteToWideCharUTF8(access.getColumn_Text(0));
			CString str( wstr.c_str() );
			strThreadName = str;
		}
		else
		{
			continue;
		}


		if (access.getColumn_Int(1) != NULL)
		{
			nStatus =access.getColumn_Int(1);
		}
		else if(access.getColumn_Text(1) != NULL)
		{
			nStatus = stoi(access.getColumn_Text(1));
		}
		else
		{
			continue;
		}

		if (access.getColumn_Int(2) != NULL)
		{
			nTimePeriod =access.getColumn_Int(2);
		}
		else if(access.getColumn_Text(2) != NULL)
		{
			nTimePeriod = stoi(access.getColumn_Text(1));
		}
		else
		{
			continue;
		}

		strThreadNameList.push_back(strThreadName);
		nStatusList.push_back(nStatus);
		nTimePeriodList.push_back(nTimePeriod);
	}

	access.SqlFinalize();

	return true;
}


bool CProjectSqlite::SaveBackupOneLogicStatus(CString cstrThreadName, int nStatus, int nTimePeriod)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	string strThreadName = UtilString::ConvertWideCharToMultiByte(cstrThreadName.GetString());

	//查询表存在或创建表
	sqlstream << "CREATE TABLE IF NOT EXISTS backup_logic_status(threadname varchar(255),threadstatus int, threadtimespan int);";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);

	//查询字段存在 
	sqlstream.clear();
	sqlstream.str("");
	sqlstream<<"select count(*) FROM backup_logic_status where threadname = '"<<strThreadName<<"' ;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);

	if(SQLITE_ROW == access.SqlNext())
	{
		int nFind = access.getColumn_Int(0);
		if(nFind == 1)//更新
		{
			sqlstream.clear();
			sqlstream.str("");
			sqlstream<<"UPDATE backup_logic_status SET threadstatus = "<<nStatus<<" , threadtimespan ="<<nTimePeriod<<" WHERE threadname = '"<<strThreadName<<"';";
			sql_ = sqlstream.str();
			ex_sql = const_cast<char*>(sql_.c_str());
			re = access.SqlExe(ex_sql);
		}
		else if(nFind == 0)//插入
		{
			sqlstream.clear();
			sqlstream.str("");
			sqlstream<<"INSERT into backup_logic_status (threadname, threadstatus, threadtimespan)  VALUES ('"<<strThreadName<<"',"<<nStatus<<","<<nTimePeriod<<");";
			sql_ = sqlstream.str();
			ex_sql = const_cast<char*>(sql_.c_str());
			re = access.SqlExe(ex_sql);
		}
	}

	access.SqlFinalize();
	return true;
}

void CProjectSqlite::ParseFileType(const CString strPath)
{
	// if filename is s3db or 4db
	CString strSuffix;
	strSuffix = strPath.Right(strPath.GetLength() - strPath.ReverseFind(_T('.')) - 1);
	if (_T("s3db") == strSuffix)
	{
		m_bIsEncrypt = true;
	}
	else if (_T("4db") == strSuffix)
	{
		m_bIsEncrypt = false;
	}
}

bool CProjectSqlite::GetFileEncryptType(void) const
{
	return m_bIsEncrypt;
}

void CProjectSqlite::SetFileEncryptType(const bool bEncrypt)
{
	m_bIsEncrypt = bEncrypt;
}

void CProjectSqlite::GetDeviceInfo_Check(list<CheckDlg_DeviceInfo>* deviceInfoList)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select pageid,elementid,pictureid,unitproperty02 from page_contain_elements;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		int nId = 0;
		int nPageId = 0;
		int nPictureId = 0;
		bool bPicOrAni = true;
		nPageId = access.getColumn_Int(0);
		nId = access.getColumn_Int(1);
		nPictureId = access.getColumn_Int(2);
		bPicOrAni = access.getColumn_Int(3);
		deviceInfoList->push_back(CheckDlg_DeviceInfo(nId, nPictureId, nPageId, bPicOrAni));
	}

	for(auto it = deviceInfoList->begin()
		; it != deviceInfoList->end()
		; it++)
	{
		ostringstream sqltemp;
		sqltemp << "select bind_content from event where id="<< it->m_nID<<" and pageid="<<it->m_nPageId<<";";
		sql_ = sqltemp.str();
		ex_sql = const_cast<char*>(sql_.c_str());
		re = access.SqlQuery(ex_sql);
		while(true)
		{
			if(SQLITE_ROW != access.SqlNext())
			{
				break;
			}
			if(access.getColumn_Text(0) != NULL)
			{
				string sBind = access.getColumn_Text(0);
				if(sBind.size() == 0)
					continue;
				vector<CString>AllList;
				CString csTemp = Project::Tools::AnsiToWideChar(sBind.c_str()).c_str();
				Project::Tools::SplitStringByChar(csTemp, L'|', AllList);
				
				for(auto it_string = AllList.begin(); it_string!= AllList.end();it_string++)
				{
					vector<CString> csSplitList;
					Project::Tools::SplitStringByChar((*it_string), L',', csSplitList);
					if(csSplitList.size()>3)
					{
						if(csSplitList.at(2) == L"1")
							it->m_bindList.push_back(CheckDlg_DeviceInfo::imageInfo(_wtoi(csSplitList.at(1)), true));
						else
							it->m_bindList.push_back(CheckDlg_DeviceInfo::imageInfo(_wtoi(csSplitList.at(1)), false));
					}
				}
			}
		}
	}
	access.SqlFinalize();
}

void CProjectSqlite::GetPictureId_Check(list<int>* pictureList)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select ID from lib_image;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		int nId = access.getColumn_Int(0);
		pictureList->push_back(nId);
	}
	access.SqlFinalize();
}

bool CProjectSqlite::IsAnimationExist(int Id, bool bShowOrBind)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	//检索动画信息
	ostringstream sqlstream;
	sqlstream << "select animationlist from lib_animation where id = "<<Id<<";";
	string sql_ = sqlstream.str();
	char* ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	string sPicList;
	if(SQLITE_ROW != access.SqlNext())
	{
		access.SqlFinalize();
		return false;
	}
	if(access.getColumn_Text(0) == NULL)
	{
		access.SqlFinalize();
		return false;	
	}
	sPicList = access.getColumn_Text(0);
	if(sPicList.size() == 0)
	{
		access.SqlFinalize();
		return false;
	}

	//如果显示图绑定的是一个动图那么动图列表的第一个就是他显示的图
	//对动画中存放的图片列表进行检查
	vector<int>picList;
	Project::Tools::SplitString(sPicList.c_str(), ",", picList);
	if(bShowOrBind == true)
	{
		ostringstream sqlostringstream;
		sqlostringstream << "select ID from lib_image_animation where ID = "<<picList[0]<<";";
		string sql_ = sqlostringstream.str();
		char* ex_sql = const_cast<char*>(sql_.c_str());
		int re = access.SqlQuery(ex_sql);

		if(SQLITE_ROW != access.SqlNext())
		{
			access.SqlFinalize();
			return false;
		}
		return true;
	}
	else
	{
		for(auto it = picList.begin(); it != picList.end(); it++)
		{
			ostringstream sqlostringstream;
			sqlostringstream << "select ID from lib_image_animation where ID = "<<(*it)<<";";
			string sql_ = sqlostringstream.str();
			char* ex_sql = const_cast<char*>(sql_.c_str());
			int re = access.SqlQuery(ex_sql);

			if(SQLITE_ROW != access.SqlNext())
			{
				access.SqlFinalize();
				return false;
			}
		}
	}
	access.SqlFinalize();
	return true;
}

void CProjectSqlite::UpdateProjectConfigXupTime(void)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	ostringstream sqlstream;
	sqlstream << "select * from project_config;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	bool bHasRecord = false;
	while (true)
	{
		if (SQLITE_ROW == access.SqlNext())
		{
			bHasRecord = true;
		}
		break;
	}

	SYSTEMTIME	sysTime;
	char		szTmNow[MAX_PATH] = {0};
	GetLocalTime(&sysTime);
	sprintf_s(szTmNow, MAX_PATH, "%d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	if (bHasRecord)
	{
		if (access.UpdateProjectConfigXupTime(szTmNow) != 0)
		{
			OutputSaveErr(L"UpdateProjectConfigXupTime", access.GetSqliteErr());
		}
	}
	access.SqlFinalize();
}

bool CProjectSqlite::GetEncryptTypeWithImgLib(const bool bDbSelect)
{
	bool bEncrypt = true;
	if (bDbSelect)
	{	// project
		bEncrypt = m_bIsEncrypt;
	}
	else
	{	// image lib
		bEncrypt = true;
	}
	return bEncrypt;
}

int CProjectSqlite::GetMaxIDInPointTable()
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	ostringstream sqlstream;

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "select max(id)  from list_point";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());

	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	int nMaxID = 0;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (access.getColumn_Text(0) != NULL)
		{
			
			nMaxID = access.getColumn_Int(0);
		}

	}

	access.SqlFinalize();

	return nMaxID;
}
bool CProjectSqlite::InsertNewPointIntoPointTable(int id,CString pointName)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream.clear();
	sqlstream.str("");
	string strPointName = UtilString::ConvertWideCharToMultiByte(pointName.GetString());
	sqlstream << "insert into list_point(id,name,type,unit,SourceType,R_W) values('" <<id << "','" << strPointName << "','"<< 0<<"','"<< 0<<"','vpoint','0')";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	sql_ = sqlstream.str();

	ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	bool binsert = false;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		binsert = true;

	}

	access.SqlFinalize();

	return binsert;
}
bool CProjectSqlite::GetPointExist(const char* lpszName)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	ostringstream sqlstream;

	sqlstream.clear();
	sqlstream.str("");
	sqlstream << "select  from list_point where name ='"<< lpszName <<"'";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());

	sql_ = sqlstream.str();

	ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);


	bool bExist = false;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

	

	}

	access.SqlFinalize();

	return bExist;
}

int CProjectSqlite::InsertRecordToOPCPoint(const int nIndex, const int nType, const char* lpszName, const char* lpszSource, const int nProperty, const char* lpszDescription, const char* lpszUnit, const double dHigh, const double dHighHigh, const double dLow, const double dLowLow, 
					const char* lpszParam1,const char* lpszParam2, const char* lpszParam3,const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8, const char* lpszParam9,const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,const char* lpszParam13,const char* lpszParam14,const char* lpszParam15,const int nParam16,const int nParam17,const int nParam18)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);

	char szSQL[5000] = {0};
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	//int nMax = (int)atof(lpszParam12);
	//int nMin = (int)atof(lpszParam13);
	int nIsVisit = (int)atof(lpszParam14);
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_point(id, type, name, SourceType, R_W, ch_description, unit, high, highhigh, low, lowlow, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param15, Param16, Param17, Param18)\
									VALUES(%d, %d, '%s', '%s', %d, '%s', '%s', %d, '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, %d, %d)", nIndex,
									nType, lpszName, lpszSource, nProperty, lpszDescription, lpszUnit, (int)dHigh, lpszParam12, lpszParam13, nIsVisit, lpszParam1, lpszParam2, lpszParam3,lpszParam4,lpszParam5,lpszParam6,lpszParam7,lpszParam8, lpszParam9,lpszParam10,lpszParam11,lpszParam15,nParam16,nParam17,nParam18);

	rc = access.SqlExe(szSQL);
	assert(0 == rc);
	return rc;
}


int CProjectSqlite::ReadTemplatePageGroupList(CString strTemplateFileName, vector<GroupPage> & vecPageGroupNameList)
{
	


	int rc;
	CString strERR;

	CString strTemplatePath;

	CString strDir;
	TCHAR szAppDir[MAX_PATH] = {0};
	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	PathRemoveFileSpec(szAppDir);
	CString strFilePath;
	strFilePath.Format(_T("%s\\template\\%s"), szAppDir,strTemplateFileName);

	CSqliteAcess access(strFilePath, m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_pagegroup;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	GroupPage  gp;  
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		gp.GroupID = access.getColumn_Int(0);

		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			gp.GroupName =UtilString::ConvertMultiByteToWideChar(name_temp);
		}

		if (const_cast<char*>(access.getColumn_Text(3)) !=NULL)
		{
			string   strShowOrder(const_cast<char*>(access.getColumn_Text(3)));
			gp.nGroupShowOrder = atoi(strShowOrder.c_str());
		}

		vecPageGroupNameList.push_back(gp);
	}

	rc = access.SqlFinalize();

	return rc;
}

int CProjectSqlite::ReadTemplatePageList(CString strTemplateFileName, int m_nGroupId, vector<CPageTemplateInfo> & vecPageTemplateInfoList)
{
	
	vecPageTemplateInfoList.clear();
	int rc=1;
	CString strERR;

	CString strTemplatePath;

	CString strDir;
	TCHAR szAppDir[MAX_PATH] = {0};
	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	PathRemoveFileSpec(szAppDir);
	CString strFilePath;
	strFilePath.Format(_T("%s\\template\\%s"), szAppDir,strTemplateFileName);

	CSqliteAcess access(strFilePath, m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select * from list_page where groupid = "<<m_nGroupId;
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	CPageTemplateInfo  pt;  
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		pt.m_nID = access.getColumn_Int(0);

		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			pt.m_strName =name_temp.c_str();//UtilString::ConvertMultiByteToWideChar(name_temp);
		}

		/*if (const_cast<char*>(access.getColumn_Text(3)) !=NULL)
		{
			string   strShowOrder(const_cast<char*>(access.getColumn_Text(3)));
			pt.nGroupShowOrder = atoi(strShowOrder.c_str());
		}*/

		vecPageTemplateInfoList.push_back(pt);
	}

	rc = access.SqlFinalize();

	return rc;
}


bool CProjectSqlite::ExtractTemplatePNGFile(CString cstrTemplateName, int templatePageId, CString strDestFilePath, int &nWidth, int &nHeight)
{

	int rc=1;
	CString strERR;
	CString strTemplatePath;
	CString strDir;
	CString strFilePath;
	TCHAR szAppDir[MAX_PATH] = {0};
	
	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	PathRemoveFileSpec(szAppDir);

	if(cstrTemplateName == L"")cstrTemplateName = L"template.4db";

	strFilePath.Format(_T("%s\\template\\%s"), szAppDir, cstrTemplateName);

	CSqliteAcess access(strFilePath, m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream << "select elementtype,width,hight,pictureid from page_contain_elements where pageid = "<<templatePageId  <<" order by hight, width DESC";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	CPageTemplateInfo  pt;  
	int nPictureId = -1;
	int nMaxSize = -1e9;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		int nType= access.getColumn_Int(0);

		nWidth= access.getColumn_Int(1);

		nHeight= access.getColumn_Int(2);

		

		if(nWidth*nHeight>nMaxSize)
		{
			nPictureId = access.getColumn_Int(3);
			nMaxSize = nWidth*nHeight;
		}

	}

	rc = access.SqlFinalize();

	LoadPictureLibFromTemplate(cstrTemplateName, nPictureId, strDestFilePath);

	return rc;
}



void CProjectSqlite::LoadPictureLibFromTemplate(CString cstrTemplateName, const int nImageId,  CString strDestFilePath)
{
	wstring strFilePathName;

	CString strDir;
	TCHAR szAppDir[MAX_PATH] = {0};
	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	PathRemoveFileSpec(szAppDir);
	CString strFilePath;
	//strFilePath.Format(_T("%s\\template\\template.4db"), szAppDir);
	strFilePath.Format(_T("%s\\template\\%s"), szAppDir, cstrTemplateName);

	CSqliteAcess access(strFilePath, m_bIsEncrypt);

	ostringstream sqlstream;
	sqlstream << "select * from lib_image where ID = "<< nImageId;
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	if(SQLITE_ROW != access.SqlNext())
	{
		return;
	}

	PictureInfo info;
	//ID
	info.ID = access.getColumn_Int(0);
	//name
	const char* lpszName = access.getColumn_Text(1);
	assert(lpszName);
	wstring wstrName = UtilString::ConvertMultiByteToWideChar(lpszName);


	///
	const void* pwchar = access.getColumn_Blob(3);
	if (access.getColumn_Blob(3) != NULL)
	{
		int  size_p  = access.getColumn_Bytes(3);
		info.nSize = size_p;
		info.pBlock = new char[size_p];
		if (info.pBlock !=NULL)
		{
			memcpy_s(info.pBlock,size_p,pwchar,size_p);
		}
	}


	if(info.pBlock && info.nSize>0)
	{
		

		SaveMemBitmapToFile(info.pBlock, info.nSize, strDestFilePath.GetString());
			
	}

	
	access.SqlFinalize();
}


/* 
* 函数介绍：搜索数据库中重复ID 
* 输入参数：
		nId:(int) ;
* 输出参数：
* 返回值  ：
		(bool)找到为True,找不到为False
*/
bool CProjectSqlite::SelectPageID(const int nId)
{
	CSqliteAcess access(m_strFilePath.c_str(), m_bIsEncrypt);
	ostringstream sqlstream;
	sqlstream.clear();
	sqlstream.str("");
	/*sqlstream << "select max(id)  from list_page";*/
	sqlstream << "select id from list_page where id="<< nId <<" ;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());

	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	int nMaxID = 0;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (access.getColumn_Text(0) != NULL)
		{

			nMaxID = access.getColumn_Int(0);
		}
		if (nMaxID == nId)
			return true;
		else
			return false;
	}
	access.SqlFinalize();
	return false;
}