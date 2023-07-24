/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：SqliteAcess.cpp
* 文件标识：
* 摘    要：Sqlite数据库操作
*
* 取代版本：N 
* 原作者  ：
* 完成日期：N年N月N日
*/
#include "StdAfx.h"
#include "SqliteAcess.h"

#include "../Tools/CustomTools/CustomTools.h"
#include <cassert>
#pragma warning(disable:4996) 

#define  MAX_PATH  260

static char szSQL[5000] = {0};

CSqliteAcess::CSqliteAcess(const wchar_t* lpszPath, bool bIsEncrypt)
{
	assert(lpszPath);
	m_strDBPath = UtilString::ConvertWideCharToMultiByte(lpszPath);
	wstring strPath = MbcsToUtf8(m_strDBPath.c_str());
	m_strDBPath = UtilString::ConvertWideCharToMultiByte(strPath);
	m_db = NULL;
	m_stmt = NULL;
	m_bOpenState = false;
	
	OpenDataBase(m_strDBPath.c_str(), bIsEncrypt);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
}

CSqliteAcess::~CSqliteAcess(void)
{
	CloseDataBase();
}

int  CSqliteAcess::CloseDataBase()
{
	if(m_db)
	{
		sqlite3_close(m_db); 
		m_db = NULL;
	}
	return 0;
}

bool CSqliteAcess::GetOpenState()
{
	return m_bOpenState;
}

int  CSqliteAcess::OpenDataBase(const char* pathName, bool bIsEncrypt)
{
	char * pErrMsg = 0;
	int ret = 0;
	if(m_db)
	{
		sqlite3_close(m_db); 
		m_db = NULL;
	}

	char  name[MAX_PATH] = "";
	strcpy(name, pathName);
	ret = sqlite3_open(name, &m_db);
	if (bIsEncrypt)
	{
		sqlite3_key(m_db,"RNB.beop-2013",strlen("RNB.beop-2013"));/*RNB.beop-2013*/
		//sqlite3_rekey(m_db,NULL,0);/*RNB.beop-2013*/
	}
	if ( ret != SQLITE_OK )
	{
		fprintf(stderr, "Could not open database: %s", sqlite3_errmsg(m_db));
		exit(1);
	}
	m_bOpenState = true;
	return ret;
}

int CSqliteAcess::CreateTable(const char* sqlr)
{
	char* error = NULL;
	int  result;

	//创建工程表
	char project[512] = "Create TABLE MAIN.[project] ([ProjectID] INTEGER PRIMARY KEY autoincrement,"\
		"[Name] char(30,0) unique,"\
		"[Location] char(30,0),"\
		"[ProjectType] INTEGER,"\
		"[WeatherStationID] INTEGER,"\
		"[FloorArea] DOUBLE,"\
		"[Picture] BLOB,"\
		"[Memo] char(30,0),"\
		"[Creator] char(30,0))";
	result = sqlite3_exec(m_db, project, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	char chillerRoom[1536] = "Create TABLE MAIN.[chillerplantroom] ([CPR_ID] INTEGER PRIMARY KEY autoincrement,"\
		"[CPR_NAME] char(30,0),"\
		"[OF_PROJECT_ID] INTEGER,"\
		"[Setting_Max_LEWT] DOUBLE,"\
		"[Setting_Min_LEWT] DOUBLE,"\
		"[Setting_Max_ECWT] DOUBLE,"\
		"[Setting_Min_ECWT] DOUBLE,"\
		"[Setting_Max_Cooling_DT] DOUBLE,"\
		"[Setting_Max_Chilled_DT] DOUBLE,"\
		"[Setting_Min_ChillerLoadRatio] DOUBLE,"\
		"[Setting_Min_PumpFlowRatio] DOUBLE,"\
		"[Setting_Min_TowerLoadRatio] DOUBLE,"\
		"[dsetting_sys_min_chillflow] DOUBLE,"\
		"[dsetting_sys_min_coolflow] DOUBLE,"\
		"[HvacType] char(25,0),"\
		"[SChillOther] DOUBLE,"\
		"[SCoolOther] DOUBLE,"\
		"[RoomType] INTEGER,"\
		"[PumpGroup_Param] char(25,0),"\
		"[TowerChillerRatio] DOUBLE,"\
		"[PumpChillerRatio] DOUBLE,"\
		"[RatioUseable] INTEGER,"\
		"[setting_common_lewt] DOUBLE,"\
		"[vsd_coolpump] INTEGER,"\
		"[vsd_pri_chillpump] INTEGER,"\
		"[vsd_sec_chillpump] INTEGER,"\
		"[vsd_value] DOUBLE,"\
		"[setting_coolpump_minpowerratio] DOUBLE,"\
		"[setting_prichillpump_minpowerratio] DOUBLE,"\
		"[setting_secchillpump_minpowerratio] DOUBLE,"\
		"[setting_tower_powerindex] DOUBLE,"\
		"[setting_h0] DOUBLE,"\
		"[setting_chiller_k] DOUBLE,"\
		"[setting_ct_minhz] DOUBLE,"\
		"[setting_ct_eff_correction] DOUBLE,"\
		"[setting_electricpercent_max] DOUBLE,"\
		"[setting_approacht_add] DOUBLE,"\
		"[setting_approacht_subtract] DOUBLE,"\
		"[setting_diffflow] DOUBLE,"\
		"[vsd_value_pri] DOUBLE,"\
		"[vsd_value_sec] DOUBLE,"\
		"[vsd_value_ct] DOUBLE,"\
		"[vsd_coolingtower] INTEGER,"\
		"[ChillpumpChillerratio] DOUBLE,"\
		"[ct_outt_control] INTEGER,"\
		"[deltat_control] INTEGER,"\
		"[deltat_chill] DOUBLE,"\
		"[deltat_cool] DOUBLE)";

	result = sqlite3_exec(m_db, chillerRoom, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}


	char room_chiller[512] = "Create TABLE MAIN.[room_contain_chiller] ([Chiller_ID_In_Room] INTEGER PRIMARY KEY,"\
		"[Chiller_Name_In_Room] char(30,0),"\
		"[ChillerID] INTEGER,"\
		"[OF_CPR_ID] INTEGER,"\
		"[Enabled] INTEGER,"\
		"[X] INTEGER,"\
		"[Y] INTEGER,"\
		"[ChillerModelID] INTEGER,"\
		"[COMCtrl_ID] INTEGER,"\
		"[PLCCtrl_ID] INTEGER,"\
		"[RotateAngel] DOUBLE,"\
		"[Scale] DOUBLE,"\
		"[SChill] DOUBLE,"\
		"[SCool] DOUBLE,"\
		"[ChillerRate] DOUBLE,"\
		"[HeatPump] INTEGER)";
	result = sqlite3_exec(m_db, room_chiller, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 


	char room_coolingtower[512] = "Create TABLE MAIN.[room_contain_coolingtower] ([Tower_ID_In_Room] INTEGER PRIMARY KEY,"\
		"[Tower_Name_In_Room] char(30,0),"\
		"[TowerID] INTEGER,"\
		"[OF_CPR_ID] INTEGER,"\
		"[Enabled] INTEGER,"\
		"[Max_Capacity] DOUBLE,"\
		"[X] INTEGER,"\
		"[Y] INTEGER,"\
		"[CTModelID] INTEGER,"\
		"[PLCCtrl_ID] INTEGER,"\
		"[WithVSD] INTEGER,"\
		"[RotateAngel] DOUBLE,"\
		"[Scale] DOUBLE,"\
		"[SCool] DOUBLE)";
	result = sqlite3_exec(m_db, room_coolingtower, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 


	char room_pump[512] = "Create TABLE MAIN.[room_contain_pump] ([Pump_ID_In_Room] INTEGER PRIMARY KEY,"\
		"[Pump_Name_In_Room] char(30,0),"\
		"[PumpID] INTEGER,"\
		"[OF_CPR_ID] INTEGER,"\
		"[Pump_Location] INTEGER,"\
		"[Enabled] INTEGER,"\
		"[Standby] INTEGER,"\
		"[X] INTEGER,"\
		"[Y] INTEGER,"\
		"[PumpModelID] INTEGER,"\
		"[PLCCtrl_ID] INTEGER,"\
		"[WithVSD] INTEGER,"\
		"[RotateAngel] DOUBLE,"\
		"[Scale] DOUBLE)";
	result = sqlite3_exec(m_db, room_pump, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 


	char room_terminal[512] = "Create TABLE MAIN.[room_contain_terminal] ([Terminal_ID_InRoom] INTEGER PRIMARY KEY,"\
		"[Terminal_ID] INTEGER,"\
		"[X] INTEGER,"\
		"[Y] INTEGER,"\
		"[OF_CPR_ID] INTEGER,"\
		"[RotateAngel] DOUBLE,"\
		"[Scale] DOUBLE,"\
		"[SChill] DOUBLE)";
	result = sqlite3_exec(m_db, room_terminal, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 


	char room_corner[256] = "Create TABLE MAIN.[room_contain_corner] ([Corner_ID_InRoom] INTEGER PRIMARY KEY,"\
		"[Corner_ID] INTEGER,"\
		"[X] INTEGER,"\
		"[Y] INTEGER,"\
		"[OF_CPR_ID] INTEGER)";
	result = sqlite3_exec(m_db, room_corner, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 

	char room_pipe[512] = "Create TABLE MAIN.[room_contain_pipe] ([Pipe_ID_In_Room] INTEGER PRIMARY KEY,"\
		"[Pipe_Name_In_Room] char(30,0),"\
		"[PipeID] INTEGER,"\
		"[OF_CPR_ID] INTEGER,"\
		"[Enabled] INTEGER,"\
		"[From_Service_ID] INTEGER,"\
		"[From_Service_Type] INTEGER,"\
		"[From_Service_Port] INTEGER,"\
		"[To_Service_ID] INTEGER,"\
		"[To_Service_Type] INTEGER,"\
		"[To_Service_Port] INTEGER)";
	result = sqlite3_exec(m_db, room_pipe, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 


	char room_value[512] = "Create TABLE MAIN.[room_contain_valve] ([Valve_ID_In_Room] INTEGER PRIMARY KEY,"\
		"[Valve_Name_In_Room] char(30,0),"\
		"[ValveID] INTEGER,"\
		"[OF_CPR_ID] INTEGER,"\
		"[Enabled] INTEGER,"\
		"[X] INTEGER,"\
		"[Y] INTEGER,"\
		"[PLCCtrl_ID] INTEGER,"\
		"[RotateAngel] DOUBLE,"\
		"[Scale] DOUBLE)";
	result = sqlite3_exec(m_db, room_value, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 

	//model四个表
	char chiller_model[512] = "Create TABLE MAIN.[chiller_model] ([ChillerModelID] INTEGER PRIMARY KEY,"\
		"[ChillerModelName] char(50,0),"\
		"[ChillerID] INTEGER,"\
		"[Type] INTEGER,"\
		"[Model_Gen_Time] char(50,0),"\
		"[Model_Param] char(4096,0))";
	result = sqlite3_exec(m_db, chiller_model, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 

	char pump_model[512] = "Create TABLE MAIN.[pump_model] ([PumpModelID] INTEGER PRIMARY KEY,"\
		"[PumpModelName] char(100,0),"\
		"[PumpID] INTEGER,"\
		"[Type] INTEGER,"\
		"[Model_Gen_Time] char(100,0),"\
		"[Model_Param] char(4096,0))";
	result = sqlite3_exec(m_db, pump_model, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 

	char ct_model[512] = "Create TABLE MAIN.[coolingtower_model] ([CoolingTowerModelID] INTEGER PRIMARY KEY,"\
		"[CoolingTowerModelName] char(100,0),"\
		"[CoolingTowerID] INTEGER,"\
		"[Type] INTEGER,"\
		"[Model_Gen_Time] char(100,0),"\
		"[Model_Param] char(4096,0))";
	result = sqlite3_exec(m_db, ct_model, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 

	char water_model[512] = "Create TABLE MAIN.[waternet_model] ([model_id] INTEGER PRIMARY KEY,"\
		"[model_name] char(100,0),"\
		"[Type] INTEGER,"\
		"[Model_Gen_Time] char(100,0),"\
		"[Model_Param] char(4096,0))";
	result = sqlite3_exec(m_db, water_model, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 

	//ws_model
	char ws_model[512] = "Create TABLE MAIN.[watersoil_model] ([WaterSoilModelID] INTEGER PRIMARY KEY,"\
		"[WaterSoilModelName] char(50,0),"\
		"[WaterSoilID] INTEGER,"\
		"[Type] INTEGER,"\
		"[Model_Gen_Time] char(50,0),"\
		"[Model_Param] char(4096,0))";
	result = sqlite3_exec(m_db, ws_model, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	} 

    //lib 9个表
	//chillerlib
	char chillerlib[512] = "Create TABLE MAIN.[chillerlib] ([ChillerID] INTEGER PRIMARY KEY,"\
		"[ModelName] char(50,0),"\
		"[Brand] char(30,0),"\
		"[Type] INTEGER,"\
		"[Refrig] INTEGER,"\
		"[Motor_Start_Type] INTEGER,"\
		"[Rating_CoolingCap] DOUBLE,"\
		"[Rating_HeatCap] DOUBLE,"\
		"[Rating_Flow_Cond] DOUBLE,"\
		"[Rating_Flow_Evap] DOUBLE,"\
		"[Rating_COP] DOUBLE,"\
		"[Rating_LEWT] DOUBLE,"\
		"[Rating_ECWT] DOUBLE,"\
		"[Surge_Param1] DOUBLE,"\
		"[Surge_Param2] DOUBLE,"\
		"[Surge_Param3] DOUBLE,"\
		"[Surge_Param4] DOUBLE,"\
		"[Rating_DP_Evap] DOUBLE,"\
		"[Rating_DP_Cond] DOUBLE)";
	result = sqlite3_exec(m_db, chillerlib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	//pumplib
	char pumplib[512] = "Create TABLE MAIN.[pumplib] ([PumpID] INTEGER PRIMARY KEY,"\
		"[ModelName] char(50,0),"\
		"[Brand] char(30,0),"\
		"[Type] INTEGER,"\
		"[Rating_RPM] INTEGER,"\
		"[Rating_Flow] DOUBLE,"\
		"[Rating_Head] DOUBLE,"\
		"[Rating_Power] DOUBLE)";
	result = sqlite3_exec(m_db, pumplib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	//coolingTowerlib
	char coolingTowerlib[512] = "Create TABLE MAIN.[coolingtowerlib] ([CoolingTowerID] INTEGER PRIMARY KEY,"\
		"[CoolingTowerModelName] char(50,0),"\
		"[Brand] char(30,0),"\
		"[Type] INTEGER,"\
		"[Rating_Air_Flow] INTEGER,"\
		"[Rating_Water_Flow] DOUBLE,"\
		"[Rating_TWB_Air] DOUBLE,"\
		"[Rating_T_water_in] DOUBLE,"\
		"[Rating_T_water_out] DOUBLE,"\
		"[Power] DOUBLE,"\
		"[FansCount] DOUBLE)";
	result = sqlite3_exec(m_db, coolingTowerlib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	//hxlib
	char hxlib[256] = "Create TABLE MAIN.[hxlib] ([HxID] INTEGER PRIMARY KEY,"\
		"[HxType] INTEGER,"\
		"[HxDP1] DOUBLE,"\
		"[HxDP2] DOUBLE,"\
		"[Tin1] DOUBLE,"\
		"[Tin2] DOUBLE,"\
		"[Tout1] DOUBLE,"\
		"[Tout2] DOUBLE,"\
		"[HxQ] DOUBLE)";
	result = sqlite3_exec(m_db, hxlib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	//pipelib
	char pipelib[256] = "Create TABLE MAIN.[pipelib] ([PipeID] INTEGER PRIMARY KEY,"\
		"[Diameter] DOUBLE,"\
		"[Material] INTEGER)";
	result = sqlite3_exec(m_db, pipelib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	//cornerlib
	char cornerlib[256] = "Create TABLE MAIN.[cornerlib] ([CornerID] INTEGER PRIMARY KEY,"\
		"[CornerType] INTEGER,"\
		"[CornerS] DOUBLE)";
	result = sqlite3_exec(m_db, cornerlib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}


	//Terminallib
	char terminallib[256] = "Create TABLE MAIN.[terminallib] ([Terminal_ID] INTEGER PRIMARY KEY,"\
		"[Terminal_Type] INTEGER)";
	result = sqlite3_exec(m_db, terminallib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	//valuelib
	char valvelib[256] = "Create TABLE MAIN.[valvelib] ([ValveID] INTEGER PRIMARY KEY,"\
		"[ValveType] INTEGER,"\
		"[Valves] DOUBLE)";
	result = sqlite3_exec(m_db, valvelib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	//tanklib
	char tanklib[256] = "Create TABLE MAIN.[tanklib] ([tankid] INTEGER PRIMARY KEY,"\
		"[type] INTEGER,"\
		"[brand] char(30,0),"\
		"[volumn] DOUBLE,"\
		"[capacity] DOUBLE,"\
		"[modelname] char(128,0))";
	result = sqlite3_exec(m_db, tanklib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	//wslib
	char watersoillib[1024] = "Create TABLE MAIN.[watersoillib] ([WaterSoilID] INTEGER PRIMARY KEY,"\
		"[Name] char(50,0),"\
		"[Brand] char(30,0),"\
		"[Type] INTEGER,"\
		"[GroutDensity] DOUBLE,"\
		"[GroutSpecificHeat] DOUBLE,"\
		"[GroutThermalConductivity] DOUBLE,"\
		"[GroundThermalConductivity] DOUBLE,"\
		"[GroundDensity] DOUBLE,"\
		"[GroundSpecificHeat] DOUBLE,"\
		"[BoreholeType] INTEGER,"\
		"[FluidSpecificHeat] DOUBLE,"\
		"[BoreholeDepth] DOUBLE,"\
		"[BoreholeRadius] DOUBLE,"\
		"[RowCount] INTEGER,"\
		"[ColumnCount] INTEGER,"\
		"[RowSpace] DOUBLE,"\
		"[ColumnSpace] DOUBLE,"\
		"[UTubeShanks] DOUBLE,"\
		"[UPipeOuterRadius] DOUBLE,"\
		"[UPipeInterRadius] DOUBLE,"\
		"[ConvectiveHeatTransferCoefficient] DOUBLE,"\
		"[PipeThermalConductivity] DOUBLE,"\
		"[FlowRate] DOUBLE)";
	result = sqlite3_exec(m_db, watersoillib, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}

	//8736个东西
	char chiller_plant[512] = "Create TABLE MAIN.[chiller_plant_room_load_define] ([Time_] char(50, 0) PRIMARY KEY,"\
		"[CoolingLoad] DOUBLE,"\
		"[HeatingLoad] DOUBLE,"\
		"[Total_Flow] DOUBLE,"\
		"[Total_T_Supply] DOUBLE,"\
		"[Total_T_Return] DOUBLE,"\
		"[OF_CPR_ID] INTEGER,"\
		"[Tdry] DOUBLE,"\
		"[Twet] DOUBLE,"\
		"[electricprice] DOUBLE,"\
		"[defaut1] DOUBLE,"\
		"[coolingload1] DOUBLE,"\
		"[coolingload2] DOUBLE,"\
		"[coolingload3] DOUBLE,"\
		"[coolingload4] DOUBLE);";

	result = sqlite3_exec(m_db, chiller_plant, NULL, NULL, &error);
	if(result!=SQLITE_OK)
	{
		printf("创建表失败，错误码%d,错误原因%s\n", result ,error);
	}
	return  result;
}


int CSqliteAcess::SqlExe(const char* sql)
{
	sqlite3_busy_timeout(m_db, 30*1000); //最长等待30m ,也相当于延时

	char* error;
	int  result;
	result = sqlite3_exec(m_db, sql, NULL, NULL, &error);

	if(result!=SQLITE_OK)
	{
		printf("失败，错误码%d,错误原因%s\n", result ,error);
	}
	return  result;
}

int	CSqliteAcess::BeginTransaction()
{
	char* error;
	int result = sqlite3_exec(m_db, "BEGIN;", NULL, NULL, &error);

	return result;
}

int CSqliteAcess::CommitTransaction()
{
	char* error;
	return sqlite3_exec(m_db, "COMMIT;", NULL, NULL, &error);
}

int CSqliteAcess::SqlQuery(const char* sqlr)
{
	return sqlite3_prepare(m_db, sqlr, strlen(sqlr), &m_stmt, 0);
}

int	CSqliteAcess::SqlFinalize()
{
	return sqlite3_finalize(m_stmt);
}

int CSqliteAcess::SqlNext()
{
	return sqlite3_step(m_stmt);
}

int CSqliteAcess::getColumn_Count()
{
	return sqlite3_column_count(m_stmt);
}

double CSqliteAcess::getColumn_Double(int i)
{
	return sqlite3_column_double(m_stmt, i);
}

int CSqliteAcess::getColumn_Int(int i)
{
	return sqlite3_column_int(m_stmt, i);
}

const char* CSqliteAcess::getColumn_Text(int i)
{
	return  reinterpret_cast<const char*>(sqlite3_column_text(m_stmt, i));
}

const void*	CSqliteAcess::getColumn_Blob (const int i)
{
	return sqlite3_column_blob(m_stmt, i);
}

int	CSqliteAcess::getColumn_Bytes(const int i)
{
	return sqlite3_column_bytes(m_stmt, i);
}

int	CSqliteAcess::SqlStep()
{
	return sqlite3_step(m_stmt);
}

sqlite3* CSqliteAcess::GetDB() const
{
	if (m_db)
	{
		return m_db;
	}
	return NULL;
}

sqlite3_stmt* CSqliteAcess::GetDataSet() const
{
	if (m_stmt)
	{
		return m_stmt;
	}
	return NULL;
}

int CSqliteAcess::DeleteAllPageData()
{
	int rc = 0;
	 
	rc = sqlite3_exec(m_db, "delete from list_page", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToPage( const char* lpszType, const int nStartColor, const int nEndColor, const int nImageID, const int nID, const char* lpszName, const int nWidth, const int nHeight, const int nGroupID, const char* lpszShowInObserver, const char* pWebAddr, const int nShowOrder, const int nXPos, const int nYPos,const int nMinRight)
{
	assert(lpszType);
	assert(lpszName);

	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_page(id, bgcolor1, bgcolor2, bgimg, 'pagetype', 'name', width, hight, groupid, showOrder, xposition, yposition, 'unitproperty01', 'unitproperty02', 'unitproperty03')\
		VALUES(%d, %d, %d, %d, '%s', '%s', %d, %d, %d, %d, %d, %d, '%s', '%s', '%d')", nID, nStartColor, nEndColor, nImageID, lpszType, lpszName, nWidth, nHeight, nGroupID, nShowOrder, nXPos, nYPos, lpszShowInObserver, pWebAddr, nMinRight);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllContainElements()
{
	int rc = 0;

	rc = sqlite3_exec(m_db, "delete from page_contain_elements", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteProjectConfigData()
{
	int rc = 0;

	rc = sqlite3_exec(m_db, "delete from project_config", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToProjectConfig( const int nID, const char* lpszName, const int nWidth, const int nHeight, const char* lpszVersion, const char* lpszXupTime, const char* lpszLastEditSoftVersion, const char* pSaveTime, const char* pDeviceType, bool bMenuGroupEnabled)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	string strMenuGroupEnabled = bMenuGroupEnabled?"1":"0";

	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO project_config(ProjectID, 'ProjectName', ScreenWidth, ScreenHeight, 'Version', 'XupTime','HostIP','RealTimeDBName','UserName',port, unitproperty01, unitproperty02, unitproperty03, unitproperty04)\
									VALUES(%d, '%s', %d, %d, '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s', '%s', '%s')", nID, lpszName, nWidth, nHeight, lpszVersion, lpszXupTime, "localhost", "beopdata", "root", 3306, lpszLastEditSoftVersion, pSaveTime, pDeviceType, strMenuGroupEnabled.data());
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllEventData()
{
	int rc = 0;

	rc = sqlite3_exec(m_db, "delete from event", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToContainElements( const int nID, const char* lpszName, const int nType, const int nXPos, const int nYPos, const int nWidth, const int nHeight, const char* lpszDataComID, const int nLayer, const int nPageID,const int nRotateAngle,const int nLinkPageID,const char* lpszShowString,const int nPicID,const char* lpszDiagnosePageString,const int nSubType, const bool bIsPic, int nBindType)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO page_contain_elements(elementid, 'elementName', elementtype, x, y, width, hight, DataComId, layer, pageid, rotateAngle,link,showString,elementSubType,pictureid,unitproperty01,unitproperty02,unitproperty03)\
									VALUES(%d, '%s', %d, %d, %d, %d, %d, '%s', %d, %d, %d, %d, '%s', %d, %d, '%s', %d, %d)", nID, lpszName, nType, nXPos, nYPos, nWidth, nHeight, lpszDataComID, nLayer, nPageID, nRotateAngle, nLinkPageID, lpszShowString, nSubType,nPicID,lpszDiagnosePageString,bIsPic, nBindType);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::UpdateRecordInContainElements(const int nOldPicId, const int nNewPicId)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	sprintf_s(szSQL, sizeof(szSQL), "UPDATE page_contain_elements set pictureid=%d WHERE pictureid=%d;", nNewPicId, nOldPicId);
	assert(strlen(szSQL) > 0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToEvent( const int nPageID, const int nID, const char* lpszEventContent, const char* lpszBindContent )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO event(pageid, id, event_content,bind_content)\
									VALUES(%d, %d, '%s', '%s')", nPageID, nID, lpszEventContent,lpszBindContent);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllLineData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_line", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToLine( const int nPageID, const int nID, const int nDirection, const int nInnerColor, const int nOutterColor, const char* lpszPointList, const int nWidth, const int nLayer, const char* lpszIDList, const int nLogic, const int nPipeType, const int nPipeBitMapID, const int nPipeDenSity, const int nPipeSpeed, const int WaterFashion)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_line(pageid, id, style, innerColor, outerColor, PointList, pipeWidth, layer, idlist, logic, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05)\
									VALUES(%d, %d, %d, %d, %d, '%s', %d, %d, '%s', %d, %d, %d, %d, %d, %d)", nPageID, nID, nDirection, nInnerColor, nOutterColor, lpszPointList, nWidth, nLayer, lpszIDList, nLogic, nPipeType, nPipeBitMapID, nPipeDenSity, nPipeSpeed, WaterFashion);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	return rc;
}

int CSqliteAcess::DeleteAllTextData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_text", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToText( const int nID, const int nPageID, const char* lpszInitialString, const int nXPos, const int nYPos, const int nWidth, const int nHeight, const int nFontSize, const int nFontColor, const char* lpszFontName, 
	const char* lpszDataComID, const int nShowMode, const double dTransparence, const int nBorderType, const int nBorderColor, const int nLayer, const int nAlign, const char* lpszTextModeShowString, const int nPointNum, const char* lpszDateTime, const char* lpszDateTimePoint, const int nTextBKType, const int nTextAttri, const char* lpszDevCfg, const char* lpszUnit, const char* lpszFaultDiag)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_text(id, pageid, initialvalue, xpos, ypos, width, height, size, color, font, dataComId, showMode, transparence, borderType, border_color, layer, align, bindstring, decimalplace, dataComTime, dataComTimeArrayPoint, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05)\
									VALUES(%d, %d, '%s', %d, %d, %d, %d, %d, %d, '%s', '%s', %d, %.2f, %d, %d, %d, %d, '%s', %d, '%s', '%s', %d, %d, '%s', '%s', '%s')", 
									nID, nPageID, lpszInitialString, nXPos, nYPos, nWidth, nHeight, nFontSize, nFontColor, lpszFontName, lpszDataComID, nShowMode, dTransparence, nBorderType, nBorderColor, nLayer, nAlign, lpszTextModeShowString, nPointNum, lpszDateTime, lpszDateTimePoint, nTextBKType, nTextAttri, lpszDevCfg,lpszUnit, lpszFaultDiag);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllGraphics()
{
	int rc = 0;

	rc = sqlite3_exec(m_db, "delete from list_graphic", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllPoint()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_point", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToOPCPoint(const int nIndex, const int nType, const char* lpszName, const char* lpszSource, const int nProperty, 
	const char* lpszDescription, const char* lpszUnit, const double dHigh, const double dHighHigh, const double dLow, const double dLowLow, 
	const char* lpszParam1,const char* lpszParam2, const char* lpszParam3,const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,
	const char* lpszParam7,const char* lpszParam8, const char* lpszParam9,const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,
	const char* lpszParam13,const char* lpszParam14,const char* lpszParam15,const char* lpszParam16,const char* lpszParam17,const char* lpszParam18)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	//int nMax = (int)atof(lpszParam12);
	//int nMin = (int)atof(lpszParam13);
	int nIsVisit = (int)atof(lpszParam14);
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_point(id, type, name, SourceType, R_W, ch_description, unit, high, highhigh, low, lowlow, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param15, Param16, Param17, Param18)\
									VALUES(%d, %d, '%s', '%s', %d, '%s', '%s', %d, '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s',  '%s', '%s', '%s')", nIndex,
									nType, lpszName, lpszSource, nProperty, lpszDescription, lpszUnit, (int)dHigh, lpszParam12, lpszParam13, nIsVisit, lpszParam1, lpszParam2, lpszParam3,lpszParam4,lpszParam5,lpszParam6,lpszParam7,lpszParam8, lpszParam9,lpszParam10,lpszParam11,lpszParam15,lpszParam16,lpszParam17,lpszParam18);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	if(rc!=0)
	{
		string strerr = GetSqliteErr();
		int aa=1;
	}
	//assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToLogicPoint(const int nType, const char* lpszName, const char* lpszLogicType, const char* lpszParam1, const char* lpszParam2, const char* lpszParam3, const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8,const char* lpszParam9, const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,const char* lpszParam13,const char* lpszParam14,const char* lpszParam15, const char* lpszParam16,const char* lpszParam17,const char* lpszParam18,const char* lpszParam19,const char* lpszParam20,const char* lpszParam21, const char* lpszParam22,const char* lpszParam23,const char* lpszParam24,const char* lpszParam25,const char* lpszParam26,const char* lpszParam27, const char* lpszParam28,const char* lpszParam29,const char* lpszParam30,const char* lpszDescription )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_point(type, name, SubType, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,\
									Param11,Param12,Param13,Param14,Param15,Param16,Param17,Param18,Param19,Param20,Param21,Param22,Param23,Param24,Param25,Param26,\
									Param27,Param28,Param29,Param30,ch_description)\
									VALUES(%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',\
									'%s','%s','%s','%s','%s','%s','%s','%s','%s')", 
									nType, lpszName, lpszLogicType, lpszParam1, lpszParam2, lpszParam3, lpszParam4,lpszParam5,lpszParam6,lpszParam7,lpszParam8,lpszParam9,
									lpszParam10,lpszParam11,lpszParam12,lpszParam13,lpszParam14,lpszParam15,lpszParam16,lpszParam17,lpszParam18,lpszParam19,lpszParam20,
									lpszParam21,lpszParam22,lpszParam23,lpszParam24,lpszParam25,lpszParam26,lpszParam27,lpszParam28,lpszParam29,lpszParam30,lpszDescription);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToAlarmPoint(const int nType, const char* lpszName, const char* lpszAlarmType, const char* lpszParam1, const char* lpszParam2, const char* lpszParam3, const char* lpszParam4,const char* lpszParam5,const char* lpszParam6,const char* lpszParam7,const char* lpszParam8,const char* lpszParam9, const char* lpszParam10,const char* lpszParam11,const char* lpszParam12,const char* lpszParam13,const char* lpszParam14,const char* lpszParam15, const char* lpszParam16,const char* lpszParam17,const char* lpszParam18,const char* lpszParam19,const char* lpszParam20,const char* lpszDescription )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_point(type, name, SubType, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10,\
									Param11,Param12,Param13,Param14,Param15,Param16,Param17,Param18,Param19,Param20,ch_description)\
									VALUES(%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')", 
									nType, lpszName, lpszAlarmType, lpszParam1, lpszParam2, lpszParam3, lpszParam4,lpszParam5,lpszParam6,lpszParam7,lpszParam8,lpszParam9,
									lpszParam10,lpszParam11,lpszParam12,lpszParam13,lpszParam14,lpszParam15,lpszParam16,lpszParam17,lpszParam18,lpszParam19,lpszParam20,lpszDescription);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int	CSqliteAcess::InsertRecordToGraphics(const int nID, const int nPageID, const int nType, const int nLineColor, const int nLineStyle, const int nLineWidth, const int nFillColor, const char* lpszPointList,
									const int nFrontPageID, const double dTransparence, const int nIsBorder3D, const int nLayer)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_graphic(pageid,id,type,pointlist,fillcolor,linecolor,linewidth,linestyle,forwardpageid,transparence,textbordet,layer)\
									VALUES(%d,%d,%d,'%s',%d,%d,%d,%d,%d,%.2f,%d,%d)", 
									nPageID,nID,nType,lpszPointList,nFillColor,nLineColor,nLineWidth,nLineStyle,nFrontPageID,dTransparence,nIsBorder3D,nLayer);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllUsefulTables()
{
	int rc = 0;
	rc = DeleteAllGroupData();
	assert( 0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllTables()
{
	int rc = 0;
	rc = DeleteAllGroupData();
	assert( 0 == rc);
	rc = DeleteAllPageData();
	assert( 0 == rc);
	rc = DeleteAllContainElements();
	assert( 0 == rc);
	rc = DeleteAllEventData();
	assert( 0 == rc);
	rc = DeleteAllLineData();
	assert( 0 == rc);
	rc = DeleteAllTextData();
	assert( 0 == rc);
	rc = DeleteAllGraphics();
	assert( 0 == rc);
	rc = DeleteAllPoint();
	assert( 0 == rc);
	rc = DeleteAllDashBoardData();
	assert( 0 == rc);
	rc = DeleteAllListChartData();
	assert( 0 == rc);
	rc = DeleteAllDetailChartData();
	assert( 0 == rc);
	rc = DeleteAllListBendData();
	assert(0 == rc);
	rc = DeleteAllButtonData();
	assert(0 == rc);
	rc = DeleteAllTimeSelectData();
	assert(0 == rc);
	rc = DeleteAllTimeBarData();
	assert(0 == rc);
	rc = DeleteAllTimeBarItemData();
	assert(0 == rc);
	rc = DeleteAllProgressBarData();
	assert(0 == rc);
	rc = DeleteAllDiagnoseChartItemData();
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllGroupData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_pagegroup", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToPageGroup( const int nGroupID, const char* lpszGroupName, const char* lpszGroupShowOrder )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_pagegroup(id,name,unitproperty01) VALUES(%d,'%s','%s')", nGroupID, lpszGroupName, lpszGroupShowOrder);
	size_t nLen = strlen(szSQL);
	assert(nLen > 0);
	if (nLen <= 0)
	{
		return -1;
	}

	try
	{
		rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	}
	catch (...)
	{
		return -2;
	}

	assert(0 == rc);
	return rc;
}

int CSqliteAcess::UpdatePageGroup(const int nGroupID, const char* lpszGroupName, const char* lpszGroupShowOrder)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "UPDATE list_pagegroup SET name='%s', unitproperty01='%s' WHERE id = %d", lpszGroupName, lpszGroupShowOrder, nGroupID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::UpdateProjectConfig( const int nID, const char* lpszName, const int nWidth, const int nHeight, const char* lpszVersion, const char* lpszXupTime, const char* lpszLastEditSoftVersion, const char* pSaveTime, const char* pDeviceType)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "UPDATE project_config set ProjectID=%d, ProjectName='%s', ScreenWidth=%d, ScreenHeight=%d, Version='%s', XupTime='%s', unitproperty01 = '%s', unitproperty02 = '%s', unitproperty03 = '%s'", nID, lpszName, nWidth, nHeight, lpszVersion, lpszXupTime, lpszLastEditSoftVersion, pSaveTime, pDeviceType);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::UpdateProjectConfigXupTime(const char* lpszXupTime)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "UPDATE project_config set XupTime='%s'", lpszXupTime);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordIntoLibImage( const int nID, const char* lpszName, const char* lpszSpecy, void* pBlock, const char* lpszFileType, const char* lpszRemark, const int bIsSyatemImage, const int nSize, const int nBtnId, const int nBtnSubId)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "insert into lib_image(ID,name,specy,imagebinary,filetype,remark,issystemimage,unitproperty01,unitproperty02) values(%d,'%s','%s',?,'%s','%s',%d,%d,%d)", nID, lpszName, lpszSpecy, lpszFileType, lpszRemark, bIsSyatemImage, nBtnId, nBtnSubId);
	assert(strlen(szSQL)>0);
	if (SQLITE_OK == sqlite3_prepare( m_db, szSQL, -1, &m_stmt, 0 ))
	{
		if (m_stmt)
		{
			rc = sqlite3_bind_blob(m_stmt,1,pBlock,nSize,NULL);
			rc = sqlite3_step(m_stmt);
			rc = sqlite3_finalize(m_stmt);
		}
	}
	return rc;
}

int CSqliteAcess::InsertRecordToLibAnimation( const int nID, const char* lpszName, const char* lpszAnimationList )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO lib_animation(id,name,animationlist) VALUES(%d,'%s','%s')", nID, lpszName, lpszAnimationList);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordIntoLibImageAnimation( const int nID, const char* lpszName, const char* lpszSpecy, void* pBlock, const char* lpszFileType, const char* lpszRemark, const int bIsSyatemImage, const int nSize )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "insert into lib_image_animation(ID,name,specy,imagebinary,filetype,remark,issystemimage) values(%d,'%s','%s',?,'%s','%s',%d)", nID, lpszName, lpszSpecy, lpszFileType, lpszRemark, bIsSyatemImage);
	assert(strlen(szSQL)>0);
	if (SQLITE_OK == sqlite3_prepare( m_db, szSQL, -1, &m_stmt, 0 ))
	{
		if (m_stmt)
		{
			rc = sqlite3_bind_blob(m_stmt,1,pBlock,nSize,NULL);
			rc = sqlite3_step(m_stmt);
			rc = sqlite3_finalize(m_stmt);
		}
	}
	return rc;
}

int CSqliteAcess::InsertRecordToListDashBoard(const int nPageID, const int nID, const char* lpszBindPointName, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nType, const int nStyle, const double dMax, const double dMin, const char* pMaxStr, const char* pMinStr, const bool bIsMaxBindPt, const bool bIsMinBindPt ,const COLORREF nFontColor, const int DecimalPlaces)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_dashboard(pageid,id,pointname,x,y,width,height,layer,elementtype,style,max,min,unitproperty01,unitproperty02,unitproperty03,unitproperty04,main_unit,second_unit) VALUES(%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%.1f,%.1f,'%s','%s',%d,%d,%d,%d)", nPageID, nID, lpszBindPointName, nX, nY, nWidth, nHeight, nLayer, nType, nStyle, dMax, dMin, pMaxStr, pMinStr, bIsMaxBindPt, bIsMinBindPt, nFontColor, DecimalPlaces);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllDashBoardData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_dashboard", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllListChartData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_chart", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllDetailChartData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from detail_chart", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToListChart( const int nPageID, const int nElementID, const char* lpszChartName, const int nUpdateInterval, const char* lpszXName, const char* lpszXUnit, const char* lpszYName, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const int nChartType, const double dMax, const char* lpszStartPoint, const int nQueryType, const char* lpszUnitProperty01,const char* lpszUnitProperty02,const char* lpszUnitProperty03,const char* lpszUnitProperty04,const char* lpszUnitProperty05)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_chart(pageid,id,name,update_interval,x_title,x_unit,y_title,x,y,width,height,layer,elementtype,type,max_value,default_x_start,querytype,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05) VALUES(%d,%d,'%s',%d,'%s','%s','%s',%d,%d,%d,%d,%d,%d,%d,%.2f,'%s',%d,'%s','%s','%s','%s','%s')"
		, nPageID, nElementID, lpszChartName, nUpdateInterval, lpszXName, lpszXUnit, lpszYName, nX, nY, nWidth, nHeight, nLayer, nElementType, nChartType, dMax, lpszStartPoint, nQueryType, lpszUnitProperty01, lpszUnitProperty02, lpszUnitProperty03, lpszUnitProperty04,lpszUnitProperty05);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToDetailChart( const int nPageID, const int nElementID, const char* lpszItemName, const int nItemColor, const char* lpszBindPointName )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO detail_chart(pageid,id,title,color1,pointname) VALUES(%d,%d,'%s',%d,'%s')", nPageID, nElementID, lpszItemName, nItemColor, lpszBindPointName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToDetailChart( const int nPageID, const int nElementID, const char* lpszItemName, const int nItemColor, const char* lpszBindPointName,const int nPointStoreType )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO detail_chart(pageid,id,title,color1,pointname,unitproperty01) VALUES(%d,%d,'%s',%d,'%s',%d)", nPageID, nElementID, lpszItemName, nItemColor, lpszBindPointName, nPointStoreType);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToListBend( const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const int nAngle )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_bend(pageid,id,x,y,width,height,layer,elementtype,rotate_angle) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,%d)", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, nElementType, nAngle);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllListBendData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_bend", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllButtonData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_button", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToListButton( const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const int nCommon, const int nDown, const int nOver, const int nDisable, const int nLinkPageID, const char* strBindSetPointName, const char* strSettingValue, const char* lpszTextContent, const COLORREF nColor, const int nSize, const char* lpszOperation,const char* lpszExpress,const char* lpszRelation,const char* lpszChecked , const char* lpszDlBtnEnable, const char* lpszDlBtnName)
{
	memset(szSQL, 0, sizeof(szSQL));

	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_button(pageid,id,x,y,width,height,layer,elementtype,comm,over,down,disable,link,settingvalue,settingpoint,textcontent,textcolor,textsize,description,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s',%d,%d,'%s','%s','%s','%s','%s','%s')", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, nElementType, nCommon, nOver, nDown, nDisable, nLinkPageID, strSettingValue, strBindSetPointName, lpszTextContent, nColor, nSize, lpszOperation,lpszExpress,lpszRelation,lpszChecked,lpszDlBtnEnable,lpszDlBtnName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeletePageByID( const int nPageID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_page where id=%d",nPageID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllTimeSelectData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_timeselect", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToListTimeSelect( const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const char* lpszBindPointName, const int nRWProperty, const int nSelectDateTime )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_timeselect(pageid,id,x,y,width,height,layer,elementtype,bindpoint,RW,datatime) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,'%s',%d,%d)", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, nElementType, lpszBindPointName, nRWProperty, nSelectDateTime);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllTimeBarData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_timebar", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToListTimeBar( const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const char* lpszBindPointName, const int nColor, const int nQueryType, const char* lpszBindPointNameStatus)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_timebar(pageid,id,x,y,width,height,layer,elementtype,bindpointstart,barcolor,querytype,bindpointstatus) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,'%s',%d,%d,'%s')", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, nElementType, lpszBindPointName, nColor, nQueryType, lpszBindPointNameStatus);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllTimeBarItemData()
{
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from list_timebaritem", NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToListTimeBarItem( const int nBarID, const int nIndex, const char* lpszName, const char* lpszBind )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_timebaritem(barid,`index`,name,bindpoint) VALUES(%d,%d,'%s','%s')", nBarID, nIndex, lpszName, lpszBind);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOnePage( const int nPageID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_page where id=%d",nPageID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneDevice( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from page_contain_elements where elementid=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneEvent( const int nPageID,const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from event where pageid=%d and id=%d",nPageID,nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOnePipe( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_line where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneText( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_text where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneGraphic( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_graphic where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneDashBoard( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_dashboard where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneChart( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_chart where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneBend( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_bend where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneButton( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_button where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneTimeSelect( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_timeselect where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneTimeBar( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_timebar where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneDetailChart( const int nPageID,const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from detail_chart where pageid=%d and id=%d",nPageID,nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteTimeBarItem( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_timebaritem where barid=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOnePoint( const char* lpszName )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_point where name='%s'",lpszName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllProgressBarData()
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_progressbar");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneProgressBarData(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_progressbar where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToListProgressBar( const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nElementType, const char* lpszBindPointName, const int nColor, const double dFactorValue, const char* lpszBindPointNameBase)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_progressbar(pageid,id,x,y,width,height,layer,elementtype,bindpoint,progresscolor,factorvalue,bindpointbase) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,'%s',%d,%.2f,'%s')", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, nElementType, lpszBindPointName, nColor, dFactorValue,lpszBindPointNameBase);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneGroup( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_pagegroup where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::Encrypt()
{
	char * pErrMsg = 0;
	int ret = 0;
	if(m_db)
	{
		sqlite3_close(m_db); 
		m_db = NULL;
	}

	char  name[MAX_PATH] = "";
	strcpy(name, m_strDBPath.c_str());
	ret = sqlite3_open(name, &m_db);
	//sqlite3_key(m_db,"RNB.beop-2013",strlen("RNB.beop-2013"));/*RNB.beop-2013*/
	sqlite3_rekey(m_db,"RNB.beop-2013",strlen("RNB.beop-2013"));/*RNB.beop-2013*/
	if ( ret == SQLITE_OK )
	{
		//AfxMessageBox(L"加密完成!");
	}
	return ret;
}

int CSqliteAcess::Decode()
{
	char * pErrMsg = 0;
	int ret = 0;
	if(m_db)
	{
		sqlite3_close(m_db); 
		m_db = NULL;
	}

	char  name[MAX_PATH] = "";
	strcpy(name, m_strDBPath.c_str());
	ret = sqlite3_open(name, &m_db);
	sqlite3_key(m_db,"RNB.beop-2013",strlen("RNB.beop-2013"));/*RNB.beop-2013*/
	sqlite3_rekey(m_db,NULL,0);/*RNB.beop-2013*/
	if ( ret == SQLITE_OK )
	{
		//AfxMessageBox(L"解密完成!");
	}
	return ret;
}

int CSqliteAcess::DeleteOnePicture( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from lib_image where ID=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneAnimation( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from lib_animation where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOnePictureForAnimation( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from lib_image_animation where ID=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::UpdatePage( const char* lpszType, const int nStartColor, const int nEndColor, const int nImageID, const int nID, const char* lpszName, const int nWidth, const int nHeight, const int nGroupID, const char* lpszShowInObserver, const char* pWebAddr, const int nXPos/*=0*/, const int nYPos/*=0*/, const int nMinRight)
{
	assert(lpszType);
	assert(lpszName);

	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	sprintf_s(szSQL, sizeof(szSQL), "update list_page set bgcolor1=%d , bgcolor2=%d , bgimg=%d , 'pagetype'='%s' , 'name'='%s' , width=%d , hight=%d , groupid=%d , xposition=%d , yposition=%d , 'unitproperty01'='%s', 'unitproperty02'='%s', 'unitproperty03'='%d' where id=%d", 
					nStartColor, nEndColor, nImageID, lpszType, lpszName, nWidth, nHeight, nGroupID, nXPos, nYPos, lpszShowInObserver, pWebAddr, nMinRight, nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::UpdatePageShowOrder(const int nPageID, const int nOrder )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;

	sprintf_s(szSQL, sizeof(szSQL), "update list_page set showOrder=%d where id=%d",nOrder, nPageID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertIntoDiagnoseLineDetail( const int nPageID, const int nElementID, const char* lpszPointName, const int nPicID, const char* lpszSubPointName, const char* lpszDescription ,const char* lpszItemName)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit01(pageid,id,unitproperty01,x,unitproperty02,unitproperty03,unitproperty04) VALUES(%d,%d,'%s',%d,'%s','%s','%s')", nPageID, nElementID, lpszPointName, nPicID,lpszSubPointName,lpszDescription,lpszItemName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteDiagnoseLineDetail( const int nPageID, const int nElementID, const char* lpszPointName,const char* lpszItemName )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit01 where pageid=%d and id=%d and unitproperty01='%s' and unitproperty04='%s'", nPageID, nElementID, lpszPointName,lpszItemName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllDiagnoseChartItemData()
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit01");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::SqlBind_Blob(char *pBlock,int iOrder ,int nSize)
{
	int rc = 0;
	rc = sqlite3_bind_blob(m_stmt,iOrder,pBlock,nSize,NULL);

	return rc;
}

int CSqliteAcess::InsertDiagnoseItemToDetail( const int nPageID, const int nElementID, const char* lpszBoolPointName, const char* lpszResultPointName, const char* lpszTreatmentPointName,const char* lpszResultPointName2,const char* lpszResultPointName3, const int nLevel )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO detail_chart(pageid,id,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,role) VALUES(%d,%d,'%s','%s','%s','%s','%s',%d)", nPageID, nElementID, lpszBoolPointName, lpszResultPointName, lpszTreatmentPointName,lpszResultPointName2, lpszResultPointName3,nLevel);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertDiagnoseSubItemToDetail( const int nPageID, const int nElementID, const char* lpszBoolPointName, const char* lpszResultPointName, const char* lpszTreatmentPointName, const char* lpszItemName, const char* lpszBindPointName, const int nColor, const int nLevel )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO detail_chart(pageid,id,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,color1,role) VALUES(%d,%d,'%s','%s','%s','%s','%s',%d,%d)", nPageID, nElementID, lpszBoolPointName, lpszResultPointName, lpszTreatmentPointName,lpszItemName,lpszBindPointName,nColor,nLevel);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

//dai修改
int CSqliteAcess::DeleteOneAreaChange( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit02 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertRecordToListUnit02( const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer,  const int nDefaultImageID, const char* strPointList,const int nAreaImageID,const int nLinkPageID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit02(x,y,width,height,layer,id,pageid,unitproperty01,unitproperty02,unitproperty03,unitproperty04) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,'%s',%d,%d)"
		, nX, nY, nWidth, nHeight, nLayer, nElementID,  nPageID, nDefaultImageID, strPointList, nAreaImageID,nLinkPageID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneImportSetting( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit03 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneImportSetting( const int nID, const char* lpszPrjName, const int bLoadPoint, const int nBasePointOffset, const int bLoadImage, const int nBaseImageOffset )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit03(id,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05) VALUES(%d,'%s',%d,%d,%d,%d)"
		, nID,  lpszPrjName, bLoadPoint, nBasePointOffset, bLoadImage,nBaseImageOffset);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteBaseImageByID( const int nStartID, const int nEndID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from lib_image where ID>=%d and ID<=%d",nStartID,nEndID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteBasePointByID( const int nStartID, const int nEndID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_point where id>=%d and id<=%d",nStartID,nEndID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::UpdateImportSettingByID( const int nID,const int bLoadPoint, const int bLoadImage )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "UPDATE list_unit03 set unitproperty02=%d, unitproperty04=%d where id = %d",bLoadPoint, bLoadImage,nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

wstring CSqliteAcess::MbcsToUtf8( const char *file )
{
	CString str;
	WCHAR   *pwchar=0;
	CHAR    *pchar=0;
	int len=0;
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	len=MultiByteToWideChar(codepage, 0, file, -1, NULL,0);
	pwchar=new WCHAR[len];
	if(pwchar!=0)
	{
		len = MultiByteToWideChar(codepage, 0, file, -1, pwchar, len);
		if( len!=0 ) 
		{
			len = WideCharToMultiByte(CP_UTF8, 0, pwchar, -1, 0, 0, 0, 0);
			pchar=new CHAR[len];
			if(pchar!=0)
			{
				len = WideCharToMultiByte(CP_UTF8, 0, pwchar, -1, pchar, len,0, 0);
				if(len!=0)                 
				{
					str=pchar;                    
				}
				delete pchar;
			}
			delete pwchar; 
		}
	}
	return str.GetString();
}

int CSqliteAcess::DeleteSqlitePoints( const vector<wstring>& vec )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	if (vec.size()>0)
	{
		string strSql="delete from list_point where name=";
		string str1="'";
		for (size_t i=0;i<vec.size();++i)
		{
			string str2=str1+UtilString::ConvertWideCharToMultiByte(vec[i].c_str())+str1;
			strSql+=str2;

			if(strSql.length()>=900)
			{
				assert(strSql.size()>0);
				rc = sqlite3_exec(m_db, strSql.c_str(), NULL, NULL, NULL);
				strSql="delete from list_point where name=";
 				str1="'";
			}
			else
			{
				if (i < vec.size()-1)
				{
					strSql+=" or name=";
				}
			}
		}
		assert(strSql.size()>0);
		rc = sqlite3_exec(m_db, strSql.c_str(), NULL, NULL, NULL);
	}
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneShellExecute( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit04 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllShellExecuteData()
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit04");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneShellExecute( const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* lpszServerIP, const char* lpszFileName, const char* lpszDownDirectory, const char* lpszPicID, const char* lpszAuto, const char* lpszFileID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit04(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06) VALUES(%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s')", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, lpszServerIP, lpszFileName, lpszDownDirectory, lpszPicID, lpszAuto, lpszFileID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneEmbededPage( const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* lpszPicID, const char* lpszPageNameList, const char* lpszInterval )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit05(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03) VALUES(%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s')", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, lpszPicID, lpszPageNameList, lpszInterval);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneEmbededPage( const int nID )
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit05 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllEmbededPage()
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit05");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoLiquidLevel(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* lpszPicID, const char* lpszBindPoint, const char* lpszMaxValue, const char* lpszMinValue, const char* lpszHighAlarmLineValue, const char* lpszLowAlarmLineValue)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit06(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06) VALUES(%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s')", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, lpszPicID, lpszBindPoint, lpszMaxValue, lpszMinValue,lpszHighAlarmLineValue,lpszLowAlarmLineValue);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromLiquidLevel(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit06 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromLiquidLevel()
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit06");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoCheckButtonPlus(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer
	, const char* lpszCtrlStyle, const char* lpszTextColor, const char* lpszTextSize, const char* lpszSelVal, const char* lpszUnSelVal, const char* lpszBindPointName
	, const char* lpszTextContent, const char* lpszGroudId, const char* lpszExpression, const char* lpszOptionsDec)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit07(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06,unitproperty07,unitproperty08,unitproperty09,unitproperty10) VALUES(%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, lpszCtrlStyle, lpszTextColor, lpszTextSize, lpszSelVal, lpszUnSelVal, lpszBindPointName, lpszTextContent, lpszGroudId, lpszExpression, lpszOptionsDec);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromCheckButtonPlus(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit07 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromCheckButtonPlus()
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit07");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int	CSqliteAcess::InsertOneIntoScrawl(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pShowWords)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit08(pageid,id,x,y,width,height,layer,unitproperty01) VALUES(%d,%d,%d,%d,%d,%d,%d,'%s')", nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, pShowWords);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int	CSqliteAcess::DeleteOneFromScrawl(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit08 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int	CSqliteAcess::DeleteAllFromScrawl()
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit08");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int	CSqliteAcess::InsertOneIntoCurveColumn(const int nElementID, const char* pPointName, const int color, const int nMaxVal, const int nMinVal, const char* pRemarks, const int nType)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit09(id,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06) VALUES(%d,'%s',%d,%d,%d,'%s',%d)", nElementID, pPointName, color, nMaxVal, nMinVal, pRemarks, nType);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int	CSqliteAcess::DeleteOneFromCurveColumn(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit09 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int	CSqliteAcess::DeleteAllFromCurveColumn()
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit09");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoHistoryTrend(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pImgType)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit10(pageid,id,x,y,width,height,layer,unitproperty01) VALUES(%d,%d,%d,%d,%d,%d,%d,'%s')",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, pImgType);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromHistoryTrend(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit10 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromHistoryTrend(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit10");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoHistoryTrendVector(const int nElementID, const int nCount, const char* pStartTime, const char* pEndTime, const int bIsAutoCalcRange, const char* pBindPtName,
												const char* pRemarks, const int nColor, const int nUpdateCycle, const int nLineWidth, const int dScale)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit11(id,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06,unitproperty07,unitproperty08,unitproperty09, unitproperty10) VALUES(%d,%d,'%s','%s',%d,'%s','%s',%d,%d,%d,%d)",
		nElementID, nCount, pStartTime, pEndTime, bIsAutoCalcRange, pBindPtName, pRemarks, nColor, nUpdateCycle, nLineWidth, dScale);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromHistoryTrendVector(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit11 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromHistoryTrendVector()
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit11");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoEnergySaveRoi(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer,
	const char* pBindPtName, const char* pCost, const char* pStartTime, const char* pEndTime, const char* pOccurTime)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit12(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05) VALUES(%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s')",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer,
		pBindPtName, pCost, pStartTime, pEndTime, pOccurTime);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromEnergySaveRoi(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit12 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromEnergySaveRoi(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit12");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoMapNavigate(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer,const int nRotateAngel,const int nEquipType,const int nGroupId,const int nId,const char* strEquipTemplatePageName,const char* paramList, const char* strTemplateName, const int nBanOnDisplay)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit13(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06,unitproperty07,unitproperty08) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,%d,'%d','%d','%s','%s','%s',%d)",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, nRotateAngel,nEquipType,nGroupId,nId,strEquipTemplatePageName,paramList,strTemplateName,nBanOnDisplay);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	if(rc!=0)
	{
		string strErr = GetSqliteErr();
	}
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromMapNavigate(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit13 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromMapNavigate(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit13");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoMapNavigateVector(const int nElementID, const char* pLongitudeName1, const char* pLongitudeName2, const char* pLatitudeName1, const char* pLatitudeName2, const int nPageId)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit14(id,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05) VALUES(%d,'%s','%s','%s','%s',%d)",
		nElementID, pLongitudeName1, pLongitudeName2, pLatitudeName1, pLatitudeName2, nPageId);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromMapNavigateVector(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit14 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromMapNavigateVector(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit14");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoNavigateWindow(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit15(pageid,id,x,y,width,height,layer) VALUES(%d,%d,%d,%d,%d,%d,%d)",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromNavigateWindow(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit15 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromNavigateWindow(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit15");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoNavigateWindowVector(const int nElementID, const char* pPageDesc, const char* pPageName, const int nPageId)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit16(id,unitproperty01,unitproperty02,unitproperty03) VALUES(%d,'%s','%s',%d)",
		nElementID, pPageDesc, pPageName, nPageId);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromNavigateWindowVector(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit16 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromNavigateWindowVector(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit16");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoLevelRuler(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer,
										const char* pName, const double dMax, const double dMin, const double dMainScale, const double dMinorScale, const int nDeci)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit17(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06) VALUES(%d,%d,%d,%d,%d,%d,%d,'%s',%f,%f,%f,%f,%d)",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer,
		pName, dMax, dMin, dMainScale, dMinorScale, nDeci);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromLevelRuler(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit17 where id=%d",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromLevelRuler(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit17");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoLevelRulerVector(const int nElementID, const char* pName, const char* pValue, const char* pBindPtName, const int nLinkPageId, const char* pLinkPageName, const int nIsUp)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit18(id,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06,unitproperty07) VALUES(%d,0,'%s','%s','%s',%d,'%s',%d)",
		nElementID, pName, pValue, pBindPtName, nLinkPageId, pLinkPageName, nIsUp);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromLevelRulerVector(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit18 where id=%d and unitproperty01=0",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromLevelRulerVector(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit18 where unitproperty01=0");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoLevelRulerSectionVector(const int nElementID, const char* pName, const double fMax, const double fMin)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit18(id,unitproperty01,unitproperty02,unitproperty03,unitproperty04) VALUES(%d,1,'%s',%f,%f)",
		nElementID, pName, fMax, fMin);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromLevelRulerSectionVector(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit18 where id=%d and unitproperty01=1",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromLevelRulerSectionVector(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit18 where unitproperty01=1");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

const char* CSqliteAcess::GetSqliteErr()
{
	return  reinterpret_cast<const char*>(sqlite3_errmsg(m_db));
}

int CSqliteAcess::InsertOneIntoDataAnalyse(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pPtTime, const char* pPtCount, const char* pPtStatus)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit19(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04) VALUES(%d,%d,%d,%d,%d,%d,%d,'73','%s','%s','%s')",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer,
		pPtTime, pPtCount, pPtStatus);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromDataAnalyse(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where id=%d and unitproperty01 = '73'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromDataAnalyse(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where unitproperty01 = '73'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoDataAnalyseVector(const int nElementID, const char* pPtName, const char* pRemarks)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit20(id,unitproperty01,unitproperty02,unitproperty03) VALUES(%d,'73','%s','%s')",
		nElementID, pPtName, pRemarks);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromDataAnalyseVector(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit20 where id=%d and unitproperty01='73'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromDataAnalyseVector(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit20 where unitproperty01='73'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoTemperDistri(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const int nPicId, const char* pPtList, const char* pBindPtName)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit19(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04) VALUES(%d,%d,%d,%d,%d,%d,%d,'tempdistribute',%d,'%s','%s')",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, nPicId, pPtList, pBindPtName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromTemperDistri(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where id=%d and unitproperty01 = 'tempdistribute'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromTemperDistri(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where unitproperty01 = 'tempdistribute'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoDataReport(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pPath)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit19(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty03) VALUES(%d,%d,%d,%d,%d,%d,%d,'report','%s')",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, pPath);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromDataReport(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where id=%d and unitproperty01 = 'report'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromDataReport(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where unitproperty01 = 'report'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoPlaneTempeDistr(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const char* pPointName, const int nCfgType)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit19(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty03,unitproperty04) VALUES(%d,%d,%d,%d,%d,%d,%d,'planeTempeDistr','%s',%d)",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, pPointName, nCfgType);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromPlaneTempeDistr(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where id=%d and unitproperty01 = 'planeTempeDistr'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromPlaneTempeDistr(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where unitproperty01 = 'planeTempeDistr'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoDottedLineFrm(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit19(pageid,id,x,y,width,height,layer,unitproperty01) VALUES(%d,%d,%d,%d,%d,%d,%d,'dottedLineFrame')",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromDottedLineFrm(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where id=%d and unitproperty01 = 'dottedLineFrame'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromDottedLineFrm(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where unitproperty01 = 'dottedLineFrame'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoGeneralComponent(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, wstring pCfgVal)
{
	CString cstrSQL;
	cstrSQL.Format(_T("INSERT INTO list_unit19(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02) \
					 VALUES(%d,%d,%d,%d,%d,%d,%d,'generalComponent','%s')") ,
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, pCfgVal.c_str());
	
	std::string strSQL;
	strSQL = Project::Tools::WideCharToAnsi(cstrSQL.GetString() );
	int rc = sqlite3_exec(m_db, strSQL.data(), NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromGeneralComponent(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where id=%d and unitproperty01 = 'generalComponent'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromGeneralComponent(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where unitproperty01 = 'generalComponent'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::ExecuteCustomSql(const char* pSql)
{
	assert(strlen(pSql)>0);
	char* pErr = NULL;
	int rc = sqlite3_exec(m_db, pSql, NULL, NULL, &pErr);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoListUnit20DeviceArea(const int nType, const char* pUnitProp02, const char* pUnitProp03, const char* pUnitProp04, const char* pUnitProp05)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit20(unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05) VALUES(%d,'%s','%s','%s','%s')", nType, pUnitProp02, pUnitProp03, pUnitProp04, pUnitProp05);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromListUnit20DeviceArea(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit20 where unitproperty01 = '1001' or unitproperty01 = '1002'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoListUnit20CalcModule(const int nUnitProp01, const char* pUnitProp02, const char* pUnitProp03, const char* pUnitProp04, const char* pUnitProp05, const char* pUnitProp06)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit20(unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06) VALUES('%d','%s',\"%s\",'%s',\"%s\",'%s')", nUnitProp01, pUnitProp02, pUnitProp03, pUnitProp04, pUnitProp05, pUnitProp06);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromListUnit20CalcModuleInfo(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit20 where unitproperty01 = '2001' or unitproperty01 = '2002'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoCalcModule(const char* pName, const int nCycle, const bool bIsEnable)
{
	int rc = 0;
	memset(szSQL, 0, sizeof(szSQL));
	int nEnable = bIsEnable ? 1 : 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit20(unitproperty01,unitproperty02,unitproperty03,unitproperty04) VALUES('2001','%s',%d,%d)", pName, nCycle, nEnable);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoCalcModuleItem(const char* pName, const char* pParentName, const char* pContent)
{
	int rc = 0;
	memset(szSQL, 0, sizeof(szSQL));
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit20(unitproperty01,unitproperty02,unitproperty03,unitproperty04) VALUES('2002','%s',\"%s\",'%s')", pName, pContent, pParentName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromCalcModule(const char* pName)
{
	int rc = 0;
	memset(szSQL, 0, sizeof(szSQL));
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit20 where unitproperty01 = '2001' and unitproperty02 = '%s'", pName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);

	memset(szSQL, 0, sizeof(szSQL));
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit20 where unitproperty01 = '2002' and unitproperty04 = '%s'", pName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromCalcModuleItem(const char* pName, const char* pParentName)
{
	int rc = 0;
	memset(szSQL, 0, sizeof(szSQL));
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit20 where unitproperty01 = '2002' and unitproperty02 = '%s' and unitproperty04 = '%s'", pName, pParentName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::UpdateCalcModule(const char* pName, const int nCycle)
{
	int rc = 0;
	memset(szSQL, 0, sizeof(szSQL));
	sprintf_s(szSQL, sizeof(szSQL), "update list_unit20 set unitproperty03 = %d where unitproperty01 = '2001' and unitproperty02 = '%s'", nCycle, pName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::UpdateCalcModuleItem(const char* pName, const char* pParentName, const char* pContent)
{
	int rc = 0;
	memset(szSQL, 0, sizeof(szSQL));
	sprintf_s(szSQL, sizeof(szSQL), "update list_unit20 set unitproperty03 = \"%s\" where unitproperty01 = '2002' and unitproperty02 = '%s' and unitproperty04 = '%s'",
		pContent, pName, pParentName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::EnableCalcModule(const char* pName, const bool bIsEnable)
{
	int rc = 0;
	memset(szSQL, 0, sizeof(szSQL));
	sprintf_s(szSQL, sizeof(szSQL), "update list_unit20 set unitproperty04 = %d where unitproperty01 = '2001' and unitproperty02 = '%s'", bIsEnable, pName);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}
int CSqliteAcess::InsertOneIntoRectangle(const int nPageID, const int nElementID, const int nX, const int nY, const int nWidth, const int nHeight, const int nLayer, const string strLineColor, const float fLineWidth, const bool bFill, const string strFillColor, const bool bRound, const UINT nRoundHeight, const UINT nRoundWidth
	, const int nRectangleStyle,  const string sTitleInfo, const string sBodyInfo, const string sParaphraseInfo
	, const bool bShowParaphrase, const string sPointInfoList, const string sTitle)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	const char *pLineColor = strLineColor.data();
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit19(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05,unitproperty06,unitproperty07,unitproperty08,unitproperty09,unitproperty10,unitproperty11,unitproperty12,unitproperty13,unitproperty14,unitproperty15) VALUES(%d,%d,%d,%d,%d,%d,%d,'rectangle','%s',%.2f,%d,'%s',%d,%d,%d,%d,'%s','%s','%s',%d,'%s','%s')",
		nPageID, nElementID, nX, nY, nWidth, nHeight, nLayer, pLineColor, fLineWidth, bFill, strFillColor.c_str(), bRound, nRoundHeight, nRoundWidth
		, nRectangleStyle,  sTitleInfo.data(),  sBodyInfo.data(),  sParaphraseInfo.data()
		,  bShowParaphrase,  sPointInfoList.data(),sTitle.data());
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}
int CSqliteAcess::DeleteOneFromRectangle(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where id=%d and unitproperty01 = 'rectangle'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromRectangle(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where unitproperty01 = 'rectangle'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoStraightLine(const int nPageID, const int nElementID, const int nX, const int nY, const int nEndX, const int nEndY, const int nLayer
	,const string strLineColor, const float fLineWidth, const int nLineStyle
	,const int nStartShape, const int nStartShapeSize
	,const int nEndShape, const int nEndShapeSize)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "INSERT INTO list_unit19(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty06,unitproperty07,unitproperty08,unitproperty09) VALUES(%d,%d,%d,%d,%d,%d,%d,'straightLine','%s',%.2f,%d,%d,%d,%d,%d)"
		,nPageID, nElementID, nX, nY, nEndX, nEndY, nLayer
		, strLineColor.c_str(), fLineWidth, nLineStyle
		, nStartShape, nStartShapeSize
		, nEndShape, nEndShapeSize);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromStraightLine(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where id=%d and unitproperty01 = 'straightLine'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromStraightLine(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where unitproperty01 = 'straightLine'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::InsertOneIntoPolygon(const int nPageID, const int nElementID, string strPointList, const int nLayer , int nTemplateGroupId, int nTemplatePageId, string sTemplatePageName, string strParam, string templateName, DWORD nColor, float nLineWidth)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	if(templateName.size() == 0)
		templateName = "template.4db";

	sprintf_s(szSQL, sizeof(szSQL)\
		,"INSERT INTO list_unit19(pageid,id,x,y,width,height,layer,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05) \
		 VALUES(%d,%d,'%s',%d,%d,'%s',%d,'polygon','%s','%s',%d,%.2f)"
		 //pageid  id           x                     y               width				height
		,nPageID, nElementID, strPointList.c_str(), nTemplateGroupId, nTemplatePageId, sTemplatePageName.c_str(), nLayer, strParam.c_str(), templateName.c_str(), nColor, nLineWidth);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteOneFromPolygon(const int nID)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where id=%d and unitproperty01 = 'polygon'",nID);
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

int CSqliteAcess::DeleteAllFromPolygon(void)
{
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	sprintf_s(szSQL, sizeof(szSQL), "delete from list_unit19 where unitproperty01 = 'polygon'");
	assert(strlen(szSQL)>0);
	rc = sqlite3_exec(m_db, szSQL, NULL, NULL, NULL);
	assert(0 == rc);
	return rc;
}

