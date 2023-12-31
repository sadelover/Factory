// -*- c++ -*-
// ***********************************************************
//
// Copyright (C) 2013 RNB
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Module:   
// Author:   david
// Date:     2010/7/13
// Revision: 1.0
//
// ***********************************************************


#include "stdafx.h"
#include "DBInterfaceExp.h"

#include "DBAccessContainer.h"
#include "comdef.h"
#include "assert.h"

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
#include <vector>

#include "../Tools/CustomTools/CustomTools.h"
#include "../Tools/Scope_Ptr.h"
#include "../Tools/Util/UtilString.h"

#include "MysqlDeleter.h"


using namespace Project::Tools;
using std::ostringstream;


//设备开关表名
const TCHAR* DEVICE_ONOFF_POINT_LIST_TABLE = _T("Device_OnOff_Point_List"); 

//设备频率表名
const TCHAR* DEVICE_FREQ_POINT_LIST_TABLE = _T("Device_Freq_Point_List");

//功率记录表
const TCHAR* POWER_POINT_LIST_TABLE = _T("Power_Point_List");

//流量记录表
const TCHAR* FLOW_POINT_LIST_TABLE = _T("Flow_Point_List");

//温度记录表。
const TCHAR* TEMPERATURE_POINT_LIST_TABLE = _T("Temperature_Point_List");

//压差记录表。
const TCHAR* PRESSUREDIFFERENCE_POINT_LIST_TABLE = _T("PressureDifference_Point_List");

// 用电量记录表。
const TCHAR* POWERCONSUMTION_POINT_LIST_TABLE = _T("PowerConsumtion_Point_List");

// 操作记录表
const char* OPERATION_RECORD_TABLE = ("operation_record");

// 运行时间表
const TCHAR* EQM_RUNNINGTIME_POINT_LIST_TABLE = _T("RunningTime_Point_List");

// 月度报表
const char* MONTHREPORT_TABLE = ("MonthReport_List");

const char* NAMEPREFIX = "uppc";
const char* DATABASE_TEMPLATE = "uppc_template";

//排热量
const char* HEATLOAD_YEAR_TABLE = "HeatLoad_Year";
const char* HEATLOAD_MONTH_TABLE = "HeatLoad_Month";

#define STATEMENTSIZE 14
const char* AddPrimaryKeySql[STATEMENTSIZE] = {\
				"alter table chiller_plant_room_load_define add primary key(Time,OF_CPR_ID);",
				"alter table chillerplantroom add primary key(CPR_ID);",
				"alter table project add primary key(projectid);",
				"alter table room_contain_chiller add primary key(Chiller_ID_In_Room, OF_CPR_ID) ;",
				"alter table room_contain_coolingtower add primary key(Tower_ID_In_Room, OF_CPR_ID);",
				"alter table room_contain_corner add primary key(Corner_ID_InRoom, OF_CPR_ID);",
				"alter table room_contain_hx add primary key(Hx_ID_In_Room, OF_CPR_ID);",
				"alter table room_contain_pipe add primary key(Pipe_ID_In_Room, OF_CPR_ID);",
				"alter table room_contain_pump add primary key(Pump_ID_In_Room, OF_CPR_ID);",
				"alter table room_contain_tank add primary key(id_in_room, OF_CPR_ID);",
				"alter table room_contain_terminal add primary key(Terminal_ID_InRoom, OF_CPR_ID);",
				"alter table room_contain_valve add primary key(Valve_ID_In_Room, OF_CPR_ID);",
				"alter table room_contain_waternet add primary key(waternetid);",
				"alter table room_contain_watersoil add primary key(id);"};



DBAccessContainer * DBAccessContainer::g_DBAccessContainer = NULL;
Mutex DBAccessContainer::m_mutex;

//////////////////////////////////////////////////////////////////////////

void MakeUpper(wstring &s)
{
	for (wstring::iterator it = s.begin(); it != s.end(); it++)
	{
		*it = toupper(*it);
	}
}

struct CompareProject
{
	bool operator()(StartUpInfo* s1, StartUpInfo* s2)
	{
		assert( (s1 != NULL) && (s2 != NULL));
		wstring s1_comp = s1->projectdb.strprojectname;
		wstring s2_comp = s2->projectdb.strprojectname;
		MakeUpper(s1_comp);
		MakeUpper(s2_comp);
		return s1_comp.compare(s2_comp) < 0;
	}
};

struct CompareprojectDB
{
	bool operator()(Project_DB s1, Project_DB s2)
	{
		
		wstring s1_comp = s1.strprojectname;
		wstring s2_comp = s2.strprojectname;

		MakeUpper(s1_comp);
		MakeUpper(s2_comp);
		return s1_comp.compare(s2_comp) < 0;
	}
};



// 构造函数
DBAccessContainer::DBAccessContainer()
:m_log(_T("DataBase.log"))
{
	m_log.Set_LogConfig(TRUE, LOG_MINLEVEL_ALL);
}

MYSQL_RES* DBAccessContainer::ReadAllEquipmentInLib( DEVICETYPE type )
{
	string sql;
	switch(type)
	{
	case Type_CHILLER:		
		sql = "select * from uppc_libmodel.chillerlib;";	
		break;
	case  Type_COOLINGTOWER:
		sql = "select * from uppc_libmodel.coolingtowerlib;";	
		break;
	case Type_PUMP:
		sql = "select * from uppc_libmodel.pumplib;";	
		break;
	case Type_PIPE:
		sql = "select * from uppc_libmodel.pipelib;";	
		break;
	case  Type_HX:
		sql = "select * from uppc_libmodel.hxlib;";	
		break;
	case  Type_VALVE:
		sql = "select * from uppc_libmodel.valvelib;";	
		break;			
	case Type_TERMINAL:
		sql = "select * from uppc_libmodel.terminallib;";	
		break;
	case Type_CORNER:
		sql = "select * from uppc_libmodel.cornerlib;";	
		break;
	case Type_Tank:
		sql = "select * from uppc_libmodel.tanklib";
		break;
	}

	if ( QueryAndStoreResult( sql.c_str()))  //查询数据库
		return m_pQuery;
	else
		LogDBError(sql);
	return NULL;
}

MYSQL_RES* DBAccessContainer::ReadAllEquipmentIDInLib( DEVICETYPE type )
{
	string sql;
	switch(type)
	{
	case Type_CHILLER:		
		sql = "select ChillerID from uppc_libmodel.chillerlib order by ChillerID;";	
		break;
	case  Type_COOLINGTOWER:
		sql = "select CoolingTowerID from uppc_libmodel.coolingtowerlib order by CoolingTowerID;";	
		break;
	case Type_PUMP:
		sql = "select pumpid from uppc_libmodel.pumplib order by pumpid;";	
		break;
	case Type_PIPE:
		sql = "select pipeid from uppc_libmodel.pipelib order by pipeid ;";	
		break;
	case  Type_HX:
		sql = "select hxid from uppc_libmodel.hxlib order by hxid;";	
		break;
	case  Type_VALVE:
		sql = "select valveid from uppc_libmodel.valvelib order by valveid;";	
		break;			
	case Type_TERMINAL:
		sql = "select terminal_id from uppc_libmodel.terminallib order by terminal_id;";	
		break;
	case Type_CORNER:
		sql = "select cornerid from uppc_libmodel.cornerlib order by cornerid;";	
		break;
	case Type_Tank:
		sql = "select tankid from uppc_libmodel.tanklib order by tankid";
		break;
	case Type_WATERSOIL:
		sql = "select watersoilid from uppc_libmodel.watersoillib order by watersoilid";
		break;
	}

	if ( QueryAndStoreResult(sql.c_str()))  //查询数据库
		return m_pQuery;
	else
		LogDBError(sql);

	return NULL;
}

MYSQL_RES* DBAccessContainer::ReadOneEquipmentFromLib( DEVICETYPE type, int id )
{ 
	char sql[128];
	memset(sql, 0x00, sizeof(sql));

	switch(type)
	{
	case Type_CHILLER:		
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.chillerlib where chillerid = %d;",  id );	
		break;
	case Type_PUMP:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.pumplib where pumpid = %d;", id);
		break;
	case Type_COOLINGTOWER:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.coolingtowerlib where CoolingTowerID = %d;", id);
		break;
	case Type_HX:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.Hxlib where Hxid = %d;", id);
		break;
	case Type_PIPE:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.Pipelib where Pipeid = %d;", id);
		break;
	case Type_CORNER:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.cornerlib where cornerid = %d;", id);
		break;
	case Type_TERMINAL:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.terminallib where terminal_id = %d;", id);
		break;
	case Type_VALVE:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.valvelib where valveid = %d;", id);
		break;
	case Type_Tank:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.tanklib where tankid = %d;", id);
		break;
	case Type_WATERSOIL:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.watersoillib where watersoilid = %d;", id);
		break;
	}

	if ( QueryAndStoreResult( sql ) )  //查询数据库
		return m_pQuery;
	else
		LogDBError(sql);
	return NULL;
}

bool DBAccessContainer::DeleteAllEquipmentsInLib( DEVICETYPE type )
{
	string sql;

	switch(type)
	{
	case Type_CHILLER:		
		sql = "delete from uppc_libmodel.chillerlib;";	
		break;
	case  Type_COOLINGTOWER:
		sql = "delete from uppc_libmodel.coolingtowerlib;";	
		break;
	case Type_PUMP:
		sql = "delete from uppc_libmodel.pumplib;";	
		break;
	case Type_PIPE:
		sql = "delete from uppc_libmodel.pipelib;";	
		break;
	case  Type_HX:
		sql = "delete from uppc_libmodel.hxlib;";	
		break;
	case  Type_VALVE:
		sql = "delete from uppc_libmodel.valvelib;";	
		break;			
	case Type_TERMINAL:
		sql = "delete from uppc_libmodel.terminallib;";	
		break;
	case Type_CORNER:
		sql = "delete from uppc_libmodel.cornerlib;";	
		break;
	case  Type_Tank:
		sql = "delete from uppc_libmodel.tanklib";
		break;
	}
	
	bool bresult = Execute(sql.c_str());

	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
	//return Execute(sql.c_str());
}


bool DBAccessContainer::AddEquipmentToLib( DEVICETYPE type, void* record )
{
	assert(record != NULL);
	char sql[512];
	memset(sql, 0x00, sizeof(sql));
	
	switch(type)
	{
	case Type_CHILLER:
		{
			gCHLib* data = (gCHLib*)record;
			//string modelname = tstringToString(data->strModelName);
			string modelname = WideCharToUtf8(data->strModelName.c_str());
			//string brand = tstringToString(data->strBrand);
			string brand = WideCharToUtf8(data->strBrand.c_str());
            
			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.chillerlib values(%d,'%s','%s',%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f);", \
				data->iChillerID, modelname.c_str(), brand.c_str(), data->iType, data->iRefrig, \
				data->iMotor_Start_Type, data->dRating_CoolingCap, data->dRating_HeatCap, data->dRating_Flow_Evap,\
				data->dRating_Flow_Cond, data->dRating_COP, data->dRating_LEWT, data->dRating_ECWT, \
				data->dSurge_Param1, data->dSurge_Param2, data->dSurge_Param3, data->dSurge_Param4, \
				data->dRating_DP_Evap, data->dRating_DP_Cond);
			break;
		}

	case Type_PUMP:
		 {	
			 gPumpLib* data = (gPumpLib*)record;
			 //string modelname = tstringToString(data->strModelName);
			 string modelname = WideCharToUtf8(data->strModelName.c_str());
			 //string brand = tstringToString(data->strBrand);
			 string brand = WideCharToUtf8(data->strBrand.c_str());
            
			 sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.PumpLib values(%d,'%s','%s',%d,%d,%f,%f,%f); ",\
					data->iPumpID, modelname.c_str(), brand.c_str(), data->iType,\
					data->iRating_RPM, data->dRating_Flow, data->dRating_Head, data->dRating_Power);
			break;
		 }
	case Type_COOLINGTOWER:
		{	
			gCTLib* data = (gCTLib*)record;

			//string modelname = tstringToString(data->strModelName);
			string modelname = WideCharToUtf8(data->strModelName.c_str());
			//string brand = tstringToString(data->strBrand);
			string brand =  WideCharToUtf8(data->strBrand.c_str());

			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.CoolingTowerLib values(%d,'%s','%s',%d,%f,%f,%f,%f,%f,%f,%d);", \
					data->iCTID, modelname.c_str(), brand.c_str(), data->iType, data->dRating_Air_Flow,\
					data->dRating_Water_Flow, data->dRating_TWB_Air, data->dRating_T_Water_In, \
					data->dRating_T_Water_Out, data->dPower, data->iFansCount);	
			break;
		}
	case Type_HX:
		{
			gHxLib* data = (gHxLib*)record;
			
			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.hxlib values (%d, %d, %f, %f, %f, %f, %f, %f, %f);" , \
					data->iHxID, data->iHxType, data->dHxDP1, data->dHxDP2, data->dTin1, data->dTin2, \
					data->dTout1, data->dTout2, data->dHxQ);	
			break;
		}
	case Type_PIPE:
		{
			gPipeLib* data = (gPipeLib*)record;
			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.pipelib values(%d,%f,%d);", \
						   data->iPipeID,data->dDiameter,data->iMaterial);
			break;
		}		
	case Type_CORNER:
		{
			gCornerLib* data = (gCornerLib*) record;
			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.Cornerlib values(%d,%d,%f);", \
				data->iCornerID, data->iCornerType, data->dCorners);
			break;
		}
		
	case Type_TERMINAL:
		{
			gTerminalLib* data = (gTerminalLib*)record;
			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.terminallib values(%d,%d);", \
							data->iTerminal_id, data->iTerminal_type);
			break;
		}
			
	case Type_VALVE:
		{
			gValveLib* data = (gValveLib*)record;
			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.ValveLib values(%d, %d, %f);", \
						   data->iValveID, data->iValveType, data->dValves);
			break;
		}		

	case  Type_Tank:
		{
			TankLibEntry* data = (TankLibEntry*)record;
			string strbrand;
			WideCharToUtf8(data->strBrand, strbrand);
			string strmodelname;
			WideCharToUtf8(data->strModelName, strmodelname);

			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.tanklib values(%d, %d, '%s', %f, %f, '%s')", data->iTankID, data->iType, strbrand.c_str(), data->dVolumn, data->dCapacity, strmodelname.c_str());	
			break;
		}
	}

	bool bresult = Execute(sql);

	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
}


bool DBAccessContainer::DeleteEquipmentInLib( DEVICETYPE type, int id )
{
	
	char  sql[128];
	memset(sql, 0x00, sizeof(sql));

	switch(type)
	{
	case Type_CHILLER:		
			sprintf_s(sql, sizeof(sql), "delete from uppc_libmodel.chillerlib where chillerid = %d;", id);
			break;		
	case Type_PUMP:
			sprintf_s(sql, sizeof(sql), "delete from uppc_libmodel.pumplib where pumpid = %d;", id);
			break;
	case Type_COOLINGTOWER:
			sprintf_s(sql, sizeof(sql), "delete from uppc_libmodel.coolingtowerlib where coolingtowerid = %d;", id);
			break;
	case Type_HX:
			sprintf_s(sql, sizeof(sql), "delete from uppc_libmodel.Hxlib where Hxid = %d;", id);
			break;
	case Type_PIPE:
			sprintf_s(sql, sizeof(sql), "delete from uppc_libmodel.Pipelib where Pipeid = %d;", id);
			break;
	case Type_CORNER:
			sprintf_s(sql, sizeof(sql), "delete from uppc_libmodel.cornerlib where cornerid = %d;", id);
			break;
	case Type_TERMINAL:
			sprintf_s(sql, sizeof(sql), "delete from uppc_libmodel.terminallib where terminal_id = %d;", id);
			break;
	case Type_VALVE:
			sprintf_s(sql, sizeof(sql), "delete from uppc_libmodel.valvelib where valveid = %d;", id);
			break;
	case Type_Tank:
			sprintf_s(sql, sizeof(sql), "delete from uppc_libmodel.tanklib where tankid = %d;", id);
			break;
	}

	bool bresult = Execute(sql);

	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
	//return Execute(sql);
}



MYSQL_RES* DBAccessContainer::ReadAllEquipmentsInRoom(DEVICETYPE type, int roomid)
{
	
	char  sql[256];
	memset(sql, 0x00, sizeof(sql));

	switch(type)
	{
	case Type_CHILLER:		
		sprintf_s(sql, sizeof(sql), "select c.*, r.*  from chillerlib c, Room_Contain_Chiller r \
				  where r.chillerid = c.chillerid and r.OF_CPR_ID = %d;", roomid);
		break;		
	case Type_PUMP:
		sprintf_s(sql, sizeof(sql), "select p.*, r.* from uppc_libmodel.pumplib p, room_contain_pump r where \
									 p.pumpid = r.pumpid and r.OF_CPR_ID = %d; ", roomid);
		break;
	case Type_COOLINGTOWER:
		sprintf_s(sql,sizeof(sql), "select c.*, r.* from uppc_libmodel.coolingtowerlib c, room_contain_coolingtower r \
								    where c.CoolingTowerID = r.TowerID and r.OF_CPR_ID = %d;", roomid);	
		break;
	case Type_HX:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.hxlib inner join room_contain_hx on uppc_libmodel.hxlib.HxID = room_contain_hx.HxID \
									  where room_contain_hx.OF_CPR_ID = %d;", roomid );
		break;
	case Type_PIPE:
		sprintf_s(sql, sizeof(sql), "select p.*, r.* from uppc_libmodel.pipelib p, room_contain_pipe r \
									 where p.pipeid = r.pipeid and r.of_cpr_id = %d;", roomid);
		break;
	case Type_CORNER:
		sprintf_s( sql, sizeof(sql), "select * from uppc_libmodel.cornerlib inner join room_contain_corner on cornerlib.CornerID = room_contain_corner.Corner_ID \
								      where room_contain_corner.OF_CPR_ID = %d;", roomid );
		break;
	case Type_TERMINAL:
		sprintf_s(sql, sizeof(sql), "select * from uppc_libmodel.terminallib inner join room_contain_terminal on terminallib.Terminal_ID = room_contain_terminal.Terminal_ID \
								     where room_contain_terminal.OF_CPR_ID = %d;", roomid);
		break;
	case Type_VALVE:
		sprintf_s(sql, sizeof(sql), "select v.*, r.* from uppc_libmodel.valvelib v, room_contain_valve r \
									 where v.valveid = r.valveid and r.of_cpr_id = %d;", roomid);		
		break;
	case Type_WATERNET:
		sprintf_s(sql, sizeof(sql), "select * from room_contain_waternet r, uppc_libmodel.waternet_model m where r.waternet_model_id = m.model_id and r.of_cpr_id = %d", roomid);
		break;
	case Type_Tank:
		sprintf_s(sql, sizeof(sql), "select * from room_contain_tank r,uppc_libmodel.tanklib l where r.tankid_in_lib = l.tankid and r.of_cpr_id = %d",roomid);
		break;
	case Type_WATERSOIL:
		//sprintf_s(sql, sizeof(sql), "select * from room_contain_watersoil");
		sprintf_s(sql, sizeof(sql), "select w.*, r.* from uppc_libmodel.watersoillib w, room_contain_watersoil r where \
									w.WaterSoilID = r.WaterSoilID and r.OF_CPR_ID = %d; ", roomid);
		break;
	case Type_POWMETER:
		sprintf_s(sql, sizeof(sql), "select * from room_contain_powermeter");
		break;
	}
	
	if (QueryAndStoreResult(sql))
		return m_pQuery;
	else
		LogDBError(sql);
	
	return NULL;
}



bool DBAccessContainer::AddEquipmentToRoom(DEVICETYPE type, void* record )
{
	assert(record != NULL);
	char sql[512];
	memset(sql, 0x00, sizeof(sql));

	switch(type)
	{
	case Type_CHILLER:
		{
			gRoom_Contain_Chiller* data = (gRoom_Contain_Chiller*)record;
			string name;
			Project::Tools::WideCharToUtf8(data->strCHillerName_InRoom, name);

			sprintf_s(sql, sizeof(sql), "insert into room_contain_chiller values(%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d, %f, %f, %f, %f, %f,%d); ", \
					data->iChillerID_In_Room, name.c_str(), data->iChillerID, data->iOF_CPR_ID, \
					data->bEnabled, data->iX, data->iY, data->iChillerModelID, data->iChillerComID, data->iPlcCtrl_id, data->fRotateAngel, data->fScale, data->fSChill, data->fSCool,data->fChillerRate,data->iHeatPump);
			break;
		}

	case Type_PUMP:
		{	
			gRoom_Contain_Pump* data = (gRoom_Contain_Pump*)record;
			string name;
			Project::Tools::WideCharToUtf8(data->strPumpName_InRoom, name);
			
			sprintf_s(sql, sizeof(sql), "insert into room_contain_pump values(%d, '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %f, %f);" ,\
					data->iPumpID_In_Room, name.c_str(), data->iPumpID, data->iOF_CPR_ID, \
					data->iPump_Location, data->bEnabled, data->bStandby, data->iX, data->iY, data->iPumpModelID, data->iPlcCtrl_id, data->bWithVSD, data->fRotateAngel, data->fScale);
			break;
		}
	case Type_COOLINGTOWER:
		{	
			gRoom_Contain_CoolingTower* data = (gRoom_Contain_CoolingTower*)record;
			string name;
			Project::Tools::WideCharToUtf8(data->strTowerName_InRoom, name);

			sprintf_s(sql, sizeof(sql), "insert into room_contain_coolingtower values (%d, '%s', %d, %d, %d, %f, %d, %d,%d, %d, %d, %f, %f, %f);", \
					data->iTowerID_In_Room, name.c_str(), data->iTowerID, data->iOF_CPR_ID, \
					data->bEnabled, data->dMax_Capacity, data->iX, data->iY, data->iCTModelID, data->iPlcCtrl_id,data->bWithVSD, data->fRotateAngel, data->fScale, data->fSCool);
					
			break;
		}
	case Type_HX:
		{
			gRoom_Contain_Hx* data = (gRoom_Contain_Hx*)record;
			string name;
			Project::Tools::WideCharToUtf8(data->strHxName_InRoom, name);

			sprintf_s(sql, sizeof(sql), "insert into room_contain_hx VALUES (%d,'%s',%d,%d,%d,%f,%d,%d, %d, %f, %f);", \
					data->iHxID_In_Room, name.c_str(), data->iHxID, data->iOF_CPR_ID, \
					data->bEnabled, data->dMax_Capacity, data->iX, data->iY, data->iPlcCtrl_id, data->fRotateAngel, data->fScale);
			break;
		}
	case Type_PIPE:
		{
			gRoom_Contain_Pipe* data = (gRoom_Contain_Pipe*)record;
			string name;
			Project::Tools::WideCharToUtf8(data->strPipeName_InRoom, name);
			

			sprintf_s(sql, sizeof(sql), "insert into room_contain_pipe values (%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d);" , \
					data->iPipeID_In_Room, name.c_str(), data->iPipeID, data->iOF_CPR_ID, \
					data->bEnabled, data->from_service_id, data->from_service_type, data->from_service_port,\
					data->to_service_id, data->to_service_type, data->to_service_port);
			break;
		}		
	case Type_CORNER:
		{
			gRoom_Contain_Corner* data = (gRoom_Contain_Corner*) record;
			sprintf_s(sql, sizeof(sql), "insert into room_contain_corner values (%d,%d,%d,%d,%d);", \
					data->iCorner_ID_InRoom, data->iCorner_ID, data->iX, data->iY, data->iOF_CPR_ID);
			break;
		}

	case Type_TERMINAL:
		{
			gRoom_Contain_Terminal* data = (gRoom_Contain_Terminal*)record;
			sprintf_s(sql, sizeof(sql), "insert into room_contain_terminal values (%d,%d,%d,%d,%d, %f, %f, %f);", \
					data->iTerminal_ID_InRoom, data->iTerminal_ID, data->iX, data->iY, data->iOF_CPR_ID, data->fRotateAngel, data->fScale, data->fSChill);
			break;
		}

	case Type_VALVE:
		{
			gRoom_Contain_Valve* data = (gRoom_Contain_Valve*)record;
			string name;
			Project::Tools::WideCharToUtf8(data->strValveName_InRoom, name);

			sprintf_s(sql, sizeof(sql), "insert into room_contain_valve values(%d,'%s',%d,%d,%d,%d,%d,%d, %f, %f);", \
					data->iValveID_In_Room, name.c_str(), data->iValveID, data->iOF_CPR_ID,\
					data->bEnabled, data->iX, data->iY, data->iPlcCtrl_id, data->fRotateAngel, data->fScale);
			break;
		}		
	case Type_WATERNET:
		{
			gRoomContainWaternet* data = (gRoomContainWaternet*)record;
			string pumpgroup = Project::Tools::tstringToString(data->strPumpGroup);
			string towergroup = Project::Tools::tstringToString(data->strTowerGroup);
			string chillergroup  = Project::Tools::tstringToString(data->strChillerGroup);
			
			sprintf_s(sql, sizeof(sql), "insert into room_contain_waternet values(%d,'%s','%s','%s',%d,%d,%d)", data->iWaternetId, \
						   pumpgroup.c_str(), towergroup.c_str(), chillergroup.c_str(), data->iOF_CPR_ID, data->iWaternetModelID, \
						   data->iChillOrCool);
			break;
		}

	case Type_Tank:
		{
			Tank_Entry_InRoom* data = (Tank_Entry_InRoom*)record;
			string tankname;
			Project::Tools::WideCharToUtf8(data->strName_In_Room, tankname);
			sprintf_s(sql, sizeof(sql), "insert into room_contain_tank values(%d, '%s', %d, %d, %d, %d)", \
					data->iId_In_Room, tankname.c_str(), data->iOf_CPR_ID, data->iX, data->iY,/*data->dVolumn,data->dCapacity,*/data->iTankID_In_Lib);
			break;
		}

	case Type_WATERSOIL:
		{
			WaterSoil_Entry_InRoom* data = (WaterSoil_Entry_InRoom*)record;
			sprintf_s(sql, sizeof(sql), "insert into room_contain_watersoil values(%d, %d, %f, %f, %f, %d, %d, %d, %d, %d )",\
				data->iId, data->iType, data->dTemp_Winter, data->dTemp_Summer, data->dTGrade, data->iOf_CPR_ID,\
				data->iX, data->iY,data->iWaterSoilID,data->iWSModelID);
			break;
		}
	}

	bool bresult = Execute(sql);

	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
	//return Execute(sql);

}


DBAccessContainer::~DBAccessContainer()
{	
}


bool DBAccessContainer::InsertChillerRunRecord(const gCHRunRecord& rRecord)
{
    Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);

	const gCHRunRecord* record = &rRecord;
	assert(record != NULL);

	char sql[512];
	memset(sql, 0x00, sizeof(sql));
	
	char buff[25];
	SYSTEMTIME st = record->stTime;
	sprintf_s(buff, sizeof(buff), "%d-%d-%d %d:%d:%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	sprintf_s(sql, sizeof(sql), "insert into %s.chiller_run_record values('%s',%d,%.2f,%.2f,%.2f,%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d) ;",\
				GetDBName(), buff, record->iChillerIDInRoom, record->dEvapFlow, record->dCondFLow, record->dPower, record->iMODE, record->iSTATUS,\
				record->iSYS_ALM, record->iCHIL_S_S, record->dLCW_STPT, record->dC_HRS,record->dAMPS_Ratio,	record->dCMPD, record->dECW,\
				record->dLCW, record->dERT, record->dERP, record->dECDW, record->dLCDW, record->dCRT, \
				record->dCRP, record->dAMPS_A, record->dVOLT_P, record->dVOLT_A, record->iOF_CPR_ID);

	bool bresult = Execute(sql);
	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
}


MYSQL_RES* DBAccessContainer::ReadChillerRunRecord( SYSTEMTIME start, SYSTEMTIME end,
												    int roomid, int chillerid )
{
	char sql[256];
	memset(sql, 0x00, sizeof(sql));

	string from = SystemTimeToString(start);
	string to = SystemTimeToString(end);

	sprintf_s(sql, sizeof(sql), "select * from chiller_run_record where time >= '%s' and time <= '%s' and ChillerIDInRoom = %d \
								 and OF_CPR_ID = %d; ", from.c_str(), to.c_str(), chillerid, roomid);

	if (QueryAndStoreResult(sql))
		return m_pQuery;
	else
		LogDBError(sql);
	return NULL;
}
//
//MYSQL_RES* DBAccessContainer::ReadChillerRunRecord( string from, string to,int roomid, int chillerid)
//{
//	char sql[256];
//	memset(sql, 0x00, sizeof(sql));
//	sprintf_s(sql, sizeof(sql), "select * from chiller_run_record where time >= '%s' and time <= '%s' and ChillerIDInRoom = %d \
//								and OF_CPR_ID = %d; ", from.c_str(), to.c_str(), chillerid, roomid);
//
//	if (QueryAndStoreResult(sql))
//		return m_pQuery;
//	else
//		LogDBError(sql);
//	return NULL;
//}

bool DBAccessContainer::InsertControlParamRecord( const gControlParamRecord& record)
{
    Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);

	ostringstream sqlstream;	
	string t = SystemTimeToString(record.tmTime);

	sqlstream << "insert into "
			  << GetDBName() << ".controlparam_record values('"
			  << t << "'," 
		      << record.dT << ","
			  << record.dFai << ","  
			  << record.dTwb << ",'"
			  << record.LEWT_Setpoint << "',"
			  << record.dLCWT_Setpoint << ",'"
			  << record.ChillerCombination << "','"
			  << record.CTCombination<< "','"
			  << record.ChilledPumpCombination << "','"
			  << record.CoolingPumpCombination << "',"
			  << record.dLoad_Actual << ","
			  << record.dLoad_UseCalc << "," 
			  << record.dChill_Flow << ","
              << record.dCool_Flow << ","
              << record.dDTChill << ","
              << record.dDTCool << ","
              << record.dChiller_K << ","
              << record.dRoomCOP << ","
              << record.dCHGroup_Power << ","
              << record.dCTGroup_Power << ","
              << record.dPriCHWPumpGroup_Power << ","
              << record.dSecCHWPumpGroup_Power << ","
              << record.dCWPumpGroup_Power <<  ")";
				
	string sql = sqlstream.str();
	bool bresult = Execute(sql);
	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
}

/*
MYSQL_RES* DBAccessContainer::ReadControlParamRunRecord( SYSTEMTIME start, SYSTEMTIME end )
{
	char sql[256];
	memset(sql, 0x00, sizeof(sql));

	string from = SystemTimeToString(start);
	string to = SystemTimeToString(end);

	sprintf_s(sql, sizeof(sql), "select * from controlparamrunrecord where time >= '%s' and time <= '%s' ;" ,\
								 from.c_str(), to.c_str());
	
	if (QueryAndStoreResult(sql))
		return m_pQuery;
	else
		LogDBError(sql);
	return NULL;
		
}*/

MYSQL_RES* DBAccessContainer::ReadModel( DEVICETYPE type, int modelid )
{
	char sql[256];
	memset(sql, 0x00, sizeof(sql));

	switch (type)
	{
	case Type_CHILLER:
		sprintf_s(sql,sizeof(sql), "select * from uppc_libmodel.chiller_model where ChillerModelID = %d;", modelid);
		break;
	case Type_PUMP:
		sprintf_s(sql,sizeof(sql), "select * from uppc_libmodel.pump_model where pumpModelID = %d;", modelid);
		break;
	case Type_COOLINGTOWER:
		sprintf_s(sql,sizeof(sql), "select * from uppc_libmodel.coolingtower_model where CoolingTowerModelID  = %d;", modelid);
		break;
	case Type_WATERNET:
		sprintf_s(sql,sizeof(sql), "select * from uppc_libmodel.waternet_model where model_id  = %d;", modelid);	
		break;
	case Type_WATERSOIL:
		sprintf_s(sql,sizeof(sql), "select * from uppc_libmodel.watersoil_model where watersoilmodelid  = %d;", modelid);	
		break;
	default:
		break;
	}
		
	return QueryAndReturnResult(sql);
}
MYSQL_RES*  DBAccessContainer::Read_Load_Define_Tempalte()
{
	char sql[256];
	memset(sql, 0x00, sizeof(sql));
	sprintf_s(sql,sizeof(sql), "select * from uppc_libmodel.load_define_template; ");

	return QueryAndReturnResult(sql);

}
bool DBAccessContainer::UpdateModel(DEVICETYPE type, int modelid,string strModel_Param)
{
	char sql[512];
	memset(sql, 0x00, sizeof(sql));
	switch (type)
	{
	case Type_CHILLER:
		sprintf_s(sql,sizeof(sql), "update uppc_libmodel.chiller_model  set Model_Param = '%s'  where ChillerModelID  = %d;",strModel_Param.c_str(), modelid);
		break;
	case Type_PUMP:
		sprintf_s(sql,sizeof(sql), "update uppc_libmodel.pump_model  set Model_Param = '%s'  where PumpModelID  = %d;",strModel_Param.c_str(), modelid);
		break;
	case Type_COOLINGTOWER:
		sprintf_s(sql,sizeof(sql), "update uppc_libmodel.coolingtower_model  set Model_Param = '%s'  where CoolingTowerModelID  = %d;",strModel_Param.c_str(), modelid);
		break;
	//case Type_WATERNET:
	//	sprintf_s(sql,sizeof(sql), "select * from uppc_libmodel.waternet_model where model_id  = %d;", modelid);	
	//	break;
	//case Type_WATERSOIL:
	//	sprintf_s(sql,sizeof(sql), "select * from uppc_libmodel.watersoil_model where watersoilmodelid  = %d;", modelid);	
	//	break;
	default:
		break;
	}

	bool bresult =  Execute(sql);	
	if (!bresult)
	{
		LogDBError(sql);
	}
	return bresult;
	//return QueryAndReturnResult(sql);
}
bool DBAccessContainer::InsertIntoModelTable( DEVICETYPE type, void* record )
{
	assert(record != NULL);

	char sql[5120];
	memset(sql, 0x00, sizeof(sql));

	switch (type)
	{
	case Type_CHILLER:
		{		
			gCH_Model* data = (gCH_Model*)record;
			string modelname;
			Project::Tools::WideCharToUtf8(data->strCHillerModelName, modelname);
			string gentime = Project::Tools::SystemTimeToString(data->tmModel_Gen_Time);
			string modelparam = Project::Tools::tstringToString(data->strModel_Param);
		
			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.chiller_model values(%d, '%s', %d, %d, '%s', '%s');" , \
					data->iChillerModelID, modelname.c_str(), data->iChillerID, data->iType, \
					gentime.c_str(), modelparam.c_str());

			break;
		}
	case Type_PUMP:
		{		
			gPump_Model* data = (gPump_Model*)record;
			string name;
			Project::Tools::WideCharToUtf8(data->strPumpModelName, name);
			string modelparam = Project::Tools::tstringToString(data->strModel_Param);
			string gentime = Project::Tools::SystemTimeToString(data->tmModel_Gen_Time);

			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.pump_model values(%d, '%s', %d, %d, '%s', '%s');", \
					data->iPumpModelID, name.c_str(), data->iPumpID, data->iType, \
					gentime.c_str(), modelparam.c_str());
			break;
		}
	case Type_WATERSOIL:
		{		
			gWaterSoil_Model* data = (gWaterSoil_Model*)record;
			string name;
			Project::Tools::WideCharToUtf8(data->strWaterSoilModelName, name);
			string modelparam = Project::Tools::tstringToString(data->strModel_Param);
			string gentime = Project::Tools::SystemTimeToString(data->tmModel_Gen_Time);

			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.watersoil_model values(%d, '%s', %d, %d, '%s', '%s');", \
				data->iWaterSoilModelID, name.c_str(), data->iWaterSoilID, data->iType, \
				gentime.c_str(), modelparam.c_str());
			break;
		}
	case Type_COOLINGTOWER:
		{		
			gCT_Model* data = (gCT_Model*)record;
			string name;
			Project::Tools:: WideCharToUtf8(data->strModelName, name);
			string modelparam = Project::Tools::tstringToString(data->strModel_Param);
			string gentime = Project::Tools::SystemTimeToString(data->tmModel_Gen_Time);
		
			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.coolingtower_model values(%d,'%s',%d,%d,'%s','%s');", \
				data->iModelID, name.c_str(), data->iCoolingTowerID, data->iType,\
				gentime.c_str(), modelparam.c_str());
			break;
		}
	case Type_WATERNET:
		{
			gWaterNet_Model* data = (gWaterNet_Model*)record;	
			string  modelname;
			Project::Tools::WideCharToUtf8(data->strModelName, modelname);
			string  modelparam =Project::Tools:: tstringToString(data->strModel_Param);
			string  gentime = Project::Tools::SystemTimeToString(data->tmModel_Gen_Time);
			sprintf_s(sql, sizeof(sql), "insert into uppc_libmodel.waternet_model values(%d,'%s',%d,'%s', '%s')", data->iModelID, \
							modelname.c_str(), data->iType, gentime.c_str(), modelparam.c_str());
				
			break;
		}
	default:
		break;
	}

	bool bresult =  Execute(sql);	
	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
}

bool DBAccessContainer::DeleteAllEquipmentsInRoom( DEVICETYPE type, int roomid )
{

	char sql[256];
	memset(sql, 0x00, sizeof(sql));
	switch(type)
	{
	case Type_CHILLER:		
		sprintf_s(sql, sizeof(sql), "delete from room_contain_chiller where OF_CPR_ID = %d;", roomid);
		break;		
	case Type_PUMP:
		sprintf_s(sql, sizeof(sql), "delete from room_contain_pump where OF_CPR_ID = %d; ", roomid);
		break;
	case Type_COOLINGTOWER:
		sprintf_s(sql,sizeof(sql), "delete from room_contain_coolingtower where OF_CPR_ID = %d;", roomid);	
		break;
	case Type_HX:
		sprintf_s(sql, sizeof(sql), "delete from room_contain_hx where OF_CPR_ID = %d;", roomid );
		break;
	case Type_PIPE:
		sprintf_s(sql, sizeof(sql), "delete from room_contain_pipe where OF_CPR_ID = %d;", roomid);
		break;
	case Type_CORNER:
		sprintf_s( sql, sizeof(sql), "delete from room_contain_corner where OF_CPR_ID = %d;", roomid );
		break;
	case Type_TERMINAL:
		sprintf_s(sql, sizeof(sql), "delete from room_contain_terminal where OF_CPR_ID = %d;", roomid);
		break;
	case Type_VALVE:
		sprintf_s(sql, sizeof(sql), "delete from room_contain_valve where OF_CPR_ID = %d;", roomid);		
		break;
	case Type_WATERNET:
		sprintf_s(sql, sizeof(sql), "delete from room_contain_waternet where OF_CPR_ID = %d;", roomid);		
		break;
	case Type_Tank:
		sprintf_s(sql, sizeof(sql), "delete from room_contain_tank where OF_CPR_ID = %d;", roomid);
		break;
	case Type_WATERSOIL:
		sprintf_s(sql, sizeof(sql), "delete from room_contain_watersoil where OF_CPR_ID = %d;", roomid);
		break;
	}


	bool bresult =  Execute(sql);	
	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
}

DBAccessContainer * DBAccessContainer::GetInstance()
{
	//Scoped_Lock<Mutex> mut(m_recordmutex);
	if ( NULL == g_DBAccessContainer )
	{
        Scoped_Lock<Mutex> mut(m_mutex); //bird 3.4
        if (!g_DBAccessContainer){
            g_DBAccessContainer = new DBAccessContainer();
            atexit(DestroyInstance);
        }
	}
	return g_DBAccessContainer;
}

// this function should be renamed to GetMaxIDInLib

int DBAccessContainer::GenerateNewIDInLib( DEVICETYPE type )
{		
	Scoped_Lock<Mutex> mut(m_recordmutex);

	string sql;
	int result = 0;	
	switch(type)
	{
	case Type_CHILLER:
		sql = "select max(chillerid) from uppc_libmodel.chillerlib";
		break;
	case Type_PUMP:
		sql = "select max(pumpid) from uppc_libmodel.pumplib";
		break;			
	case Type_COOLINGTOWER:
		sql = "select max(coolingtowerid) from uppc_libmodel.coolingtowerlib";
		break;
	case Type_HX:
		sql = "select max(hxid) from uppc_libmodel.hxlib";
		break;
	case Type_PIPE:
		sql = "select max(pipeid) from uppc_libmodel.pipelib";
		break;
	case Type_CORNER:
		sql = "select max(cornerid) from uppc_libmodel.cornerid";
		break;
	case Type_TERMINAL:
		sql = "select max(Terminal_ID) from uppc_libmodel.terminallib";
		break;
	case Type_VALVE:
		sql = "select max(valveid) from uppc_libmodel.valvelib";
		break;
	case Type_Tank:
		sql = "select max(tankid) from uppc_libmodel.tanklib";
		break;
	default:
		break;
	}

	if (QueryAndStoreResult(sql.c_str()))
	{
		if (GetRow())
		{
			result = ((m_row[0] != NULL) ? atoi(m_row[0]) : 0);
		}
		FreeResultSet();
	}
	else
	{
		LogDBError(sql);
	}

	return result;		
}


MYSQL_RES* DBAccessContainer::GetProjectInfo()
{
	string sql = "select * from project";
	if (QueryAndStoreResult(sql.c_str()))
	{
		return m_pQuery;
	}
	else
	{
		LogDBError(sql);
	}
	return NULL;	
}

bool DBAccessContainer::ReadProject( gProject * pProject)
{
	MYSQL_RES* pMySql_Res = GetProjectInfo();

	if ( NULL == pMySql_Res )
	{
		return false;
	}
	unsigned __int64 rownum = GetRowNum();
	if ( rownum <= 0 )
	{
		return false;
	}
	MYSQL_ROW row = GetRow();

	pProject->iProjectID      = ATOI(row[PROJ_PROJECTID]);

	// convert the utf8 to unicode 16
	if (row[PROJ_NAME] != NULL)
	{
		Project::Tools::UTF8ToWideChar(row[PROJ_NAME], pProject->strName);
	}

	// convert the utf8 to unicode 16
	if (row[PROJ_LOCATION] != NULL)
	{
		Project::Tools::UTF8ToWideChar(row[PROJ_LOCATION], pProject->strLocation);
	}
	unsigned long *length = mysql_fetch_lengths(pMySql_Res);

	pProject->iProjectType    = ATOI(row[PROJ_PROJECTTYPE]);
	pProject->iWeatherStation = ATOI(row[PROJ_WEATHERSTATION]);
	pProject->dFloorArea      = ATOF(row[PROJ_FLOORAREA]);
	
	pProject->strPicture.clear();
	pProject->strPicture.reserve(length[PROJ_PICTURE] + 1);
	pProject->strPicture.append(row[PROJ_PICTURE], length[PROJ_PICTURE]);

	Project::Tools::String_To_Tstring( row[PROJ_MEMO], pProject->strMemo );
	Project::Tools::String_To_Tstring( row[PROJ_CREATOR], pProject->strCreator);

	FreeResultSet();
	return true;
}

//
bool DBAccessContainer::AddNewProject( gProject* projectinfo )
{
	SetConnectionCharacterSet(MYSQL_UTF8);

	string name = Project::Tools::WideCharToUtf8(projectinfo->strName.c_str());
	
	string location = Project::Tools::WideCharToUtf8(projectinfo->strLocation.c_str());
	
	string memo = Project::Tools::WideCharToUtf8(projectinfo->strMemo.c_str());

	string creator = Project::Tools::tstringToString(projectinfo->strCreator);

	unsigned long len = (unsigned long)projectinfo->strPicture.length();
	char* buffer = new char[2 * len + 1];
	memset(buffer, 0x00, 2 * len + 1);
	try
	{	
		Execute("delete from project");
		long newlen = mysql_real_escape_string(&m_mysql, buffer, projectinfo->strPicture.data(), len);		
		ostringstream sqlstream;
		sqlstream << "insert into project values(" << projectinfo->iProjectID<< ",'" << name << "','" \
				  << location << "'," << projectinfo->iProjectType << "," << projectinfo->iWeatherStation \
				  << "," << projectinfo->dFloorArea << ",'" << buffer << "','" << memo  << "','" << creator << "')";
		
		delete[] buffer;
		string sql = sqlstream.str();

		if (Execute(sql))
			return TRUE;
		else
		{
			LogDBError(sql);
			return FALSE;
		}
	}
	catch (...)
	{
		delete []buffer;
		return FALSE;
	}
}

bool DBAccessContainer::DeleteProject( int projectid )
{
	// delete a project.
	string sql = "delete from project";
	
	
	bool bresult =  Execute(sql.c_str());
	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
}

MYSQL_RES* DBAccessContainer::GetChillerPlantRoomIDList( int projectid )
{
	//string sql = "select CPR_ID from chillerplantroom where OF_PROJECT_ID = %d ";
	char sql[256];
	memset(sql, 0x00, sizeof(sql));
	
	sprintf_s(sql, sizeof(sql), "select distinct CPR_ID from chillerplantroom where OF_PROJECT_ID = %d ", projectid);

	if (QueryAndStoreResult(sql))
	{
		return m_pQuery;
	}
	else
		LogDBError(sql);
	
	return NULL;
}

MYSQL_RES* DBAccessContainer::GetChillerPlantRoomInfoByID( int roomid )
{
	char sql[256];
	memset(sql, 0x00, sizeof(sql));

	sprintf_s(sql, sizeof(sql), "select * from chillerplantroom where cpr_id = %d", roomid);

	if (QueryAndStoreResult(sql))
	{
		return m_pQuery;
	}
	else
		LogDBError(sql);

	return NULL;
}


bool DBAccessContainer::AddNewToChillerPlantRoom( gCHPlantRoom* room )
{
	string name  = Project::Tools::tstringToString(room->strCPR_NAME);
	string controlparam = Project::Tools::tstringToString(room->strPumpGroup_Param);
	string roomtype  = Project::Tools::tstringToString(room->strHVACType);
	int    rario_useable		= room->bRatioUseable?1:0;
	int    vsd_coolpump			= room->bVSD_CoolPump?1:0;
	int    vsd_prichillpump		= room->bVSD_Pri_ChillPump?1:0;
	int    vsd_secchillpump		= room->bVSD_Sec_ChillPump?1:0;
	//2012/0618添加
	int    vsd_coolingtower		= room->dSetting_bVSD_CoolingTower?1:0;
	int    ct_outT_control      = room->dSetting_bCT_OutT_Control?1:0;
	int    deltaT_control       = room->dSetting_bDeltaT_Control?1:0;

	ostringstream sqlstream;
	sqlstream << "insert into chillerplantroom values(" << room->iCPR_ID <<",'" << name << "'," << room->iOF_PROJECT_ID \
				<< "," << room->dSetting_Max_LEWT << ","<<room->dSetting_Min_LEWT<<","<<room->dSetting_Max_ECWT<<"," << room->dSetting_Min_ECWT \
				<< "," << room->dSetting_Max_Cooling_DT << "," << room->dSetting_Max_Chilled_DT \
				<<","<< room->dSetting_Min_Chiller_LoadRatio<<"," << room->dSetting_Min_Pump_FlowRatio <<"," << room->dSetting_Min_Tower_LoadRatio\
				<<","<< room->dSetting_Sys_Min_ChillFlow<<"," << room->dSetting_Sys_Min_CoolFlow \
								
				<< ",'"<<roomtype<<"',"<< room->dSChillOther<<","<<room->dSCoolOther<<","<<room->iRoomType<< ",'" << controlparam << "',"<< room->dTowerChillerCountRatio\
				<< ","<<room->dPumpChillerCountRatio<<","<<rario_useable<<","<<room->dSetting_Common_LEWT<<","<<vsd_coolpump<<","<<vsd_prichillpump\
				<< ","<<vsd_secchillpump<<","<<room->dVSD_Value<<","<<room->dSetting_CoolPump_MinPowerRatio<<","<<room->dSetting_PriChillPump_MinPowerRatio\
				<< ","<<room->dSetting_SecChillPump_MinPowerRatio<<","<<room->dSetting_Tower_PowerIndex\

				<< ","<<room->dSetting_dH0<<","<<room->dSetting_dChiller_K<<","<<room->dSetting_dCT_MinHz<<","<<room->dSetting_dCT_eff_Correction\
				<< ","<<room->dSetting_dElectricPercent<<","<<room->dSetting_dApproachT_Add<<","<<room->dSetting_dApproachT_Subtract<<","<<room->dSetting_dDiffFlow\

				<< ","<<room->dSetting_dVSD_Value_PRI<<","<<room->dSetting_dVSD_Value_SEC<<","<<room->dSetting_dVSD_Value_CT<<","<<vsd_coolingtower\
				<< ","<<room->dSetting_fChillPumpChiller_CountRatio<<","<<ct_outT_control<<","<<deltaT_control<<","<<room->dSetting_dDeltaT_Chill<<","<<room->dSetting_dDeltaT_Cool\


				<< ")"; 

	bool bresult = Execute(sqlstream.str().c_str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

int DBAccessContainer::GetProjectID()
{
	Scoped_Lock<Mutex>  mut(m_recordmutex);
	int result = 0;
	string sql = "select projectid from project ";	
	if (QueryAndStoreResult(sql.c_str()))
	{
		if (GetRow())
		{
			if (m_row != NULL)
			{
				result = ATOI(m_row[0]);
			}
		}

		FreeResultSet();
	}
	else
		LogDBError(sql);
	return result;
}

int DBAccessContainer::GetMaxIDInChillerPlantRoom()
{
	Scoped_Lock<Mutex> mut(m_recordmutex);
	int result = 0;
	string str = "select max(CPR_ID) from chillerplantroom";
	if (QueryAndStoreResult(str.c_str()))
	{
		if (GetRow())
		{
			if ( m_row != NULL)
			{
				result = ATOI(m_row[0]);
			}
		}
		FreeResultSet();
	}
	else
		LogDBError(str);
	return result;
}

bool DBAccessContainer::DeleteFromChillerPlantRoom( int roomid )
{
	ostringstream sqlstream;
	sqlstream << "delete from chillerplantroom where CPR_ID = " << roomid;
	
	bool bresult =  Execute(sqlstream.str().c_str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

MYSQL_RES* DBAccessContainer::GetAllChillerPlantRoomInfo( int projectid )
{
	ostringstream sqlstream;

	sqlstream << "select * from chillerplantroom where OF_PROJECT_ID = " << projectid;

	if (QueryAndStoreResult(sqlstream.str().c_str()))
	{
		return m_pQuery;
	}
	else
		LogDBError(sqlstream.str());
	return NULL;
}

bool DBAccessContainer::CreateNewProject(gProject* projectinfo, string& newdbname, string copyfrom)
{
	//create a new database.	
	Scoped_Lock<Mutex> local_lock(m_recordmutex);	
	string dbname = "uppc_";

	SYSTEMTIME sttime;
	GetLocalTime(&sttime);
	char sql[50];
	
	sprintf_s(sql, sizeof(sql), "%d%d%d%d%d%d%d", sttime.wYear, sttime.wMonth, sttime.wDay, \
								sttime.wHour, sttime.wMinute, sttime.wSecond, sttime.wMilliseconds);

	dbname += sql;
	//	Project::Tools::String_To_Tstring(newdatabasename.c_str(), newdbname);
	newdbname = dbname;
	
	if (copyfrom == DATABASE_TEMPLATE)
	{
		bool bResult = CopyDatabaseWithNoData(dbname.c_str(), copyfrom.c_str());
		if (bResult)
		{
			return AddNewProject(projectinfo);			
		}
		else
		{
			LogDBError("copy database");
			return FALSE;
		}
	}
	else
	{
		bool bResult = CopyDataBase(dbname.c_str(), copyfrom.c_str());
		if (bResult)
		{
			AddNewProject(projectinfo);
		}else
		{
			LogDBError("copy database tables and data failed ");
			return FALSE;
		}
//////////////////////////////////////////////////////////////////////////
		////修改工程名称
		//string name = Project::Tools::WideCharToUtf8(projectinfo->strName.c_str());
		//ostringstream sqlstream;
		//	sqlstream << "update project set Name = '" << name <<" '";
		//string sql = sqlstream.str();
		//bool bresult = Execute(sql);
//////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < STATEMENTSIZE; i++)
		{
			Execute(AddPrimaryKeySql[i]);
		}
		return TRUE;
	}
	
}

// please establish a connection to the server first.
bool DBAccessContainer::ReadAllProjectsFromServer( vector<StartUpInfo*>& projectlist, const wstring& createuser )
{
	SetConnectionCharacterSet(MYSQL_UTF8);
	Scoped_Lock<Mutex> local_lock(m_recordmutex);

	// get the database names;
	vector<string> dbnames;
	if(!QueryAndStoreResult("show databases like 'uppc%%'"))
	{
		#if _DEBUG
		string error = GetMysqlError();
		#endif
		return FALSE;
	}

	while(GetRow())
	{
		string name  = m_row[0];
		dbnames.push_back(name);
	}
	FreeResultSet();

	for (vector<string>::iterator it = dbnames.begin(); it != dbnames.end(); it++)
	{
		string user = Project::Tools::tstringToString(createuser);
		string sql = "select name, picture from ";
		sql += *it;
		sql += ".project where creator = '";
		sql += user;
		sql += "'";

		// get the project name and picture.
		if (mysql_query(&m_mysql, sql.c_str()))
			continue;
		
		MYSQL_RES* result = mysql_use_result(&m_mysql);
		if (result != NULL)
		{			
			MYSQL_ROW row = mysql_fetch_row(result);
			if (row != NULL)
			{
				// if there is result, then create and save the info.
				StartUpInfo *temp = new StartUpInfo(); 
				unsigned long *length = mysql_fetch_lengths(result);	
				temp->bpicture.reserve(length[1] + 1);
				temp->bpicture.append(row[1], length[1]);
				temp->projectdb.strdatabasename = *it;
				
				wstring projectname ;
				if (row[0] != NULL)
				{
					Project::Tools::UTF8ToWideChar(row[0], projectname);
					temp->projectdb.strprojectname = projectname;
				}
				projectlist.push_back(temp);
			}
				
			mysql_free_result(result);
		}		
	}
	
	std::sort(projectlist.begin(), projectlist.end(), CompareProject());
	return TRUE; 
}

bool DBAccessContainer::ReadAllProjectsFromServer( vector<StartUpInfo*>& projectlist )
{
	SetConnectionCharacterSet(MYSQL_UTF8);
	Scoped_Lock<Mutex> local_lock(m_recordmutex);

	// get the database names;
	vector<string> dbnames;
	if(!QueryAndStoreResult("show databases like 'uppc%%'"))
	{
		string error = GetMysqlError();
		return FALSE;
	}
	while(GetRow())
	{
		string name  = m_row[0];
		dbnames.push_back(name);
	}
	FreeResultSet();

	for (vector<string>::iterator it = dbnames.begin(); it != dbnames.end(); it++)
	{
		string sql = "select name, picture from ";
		sql += *it;
		sql += ".project";
		
		// get the project name and picture.
		if (mysql_query(&m_mysql, sql.c_str()))
			continue;
		MYSQL_RES* result = mysql_use_result(&m_mysql);
		if (result != NULL)
		{
			MYSQL_ROW row = mysql_fetch_row(result);
			//if there is result, the project is not empty
			if (row != NULL)
			{
				unsigned long *length = mysql_fetch_lengths(result);
				StartUpInfo *temp = new StartUpInfo; 				
				if (row[1] != NULL)
				{
					temp->bpicture.reserve(length[1] + 1);
					temp->bpicture.append(row[1], length[1]);
				}				
				temp->projectdb.strdatabasename = *it;

				wstring projectname;
				if (row[0] != NULL)
				{
					Project::Tools::UTF8ToWideChar(row[0], projectname);
					temp->projectdb.strprojectname = projectname;									
				}				
				projectlist.push_back(temp);
			}
			mysql_free_result(result);
		}		
	}

	std::sort(projectlist.begin(), projectlist.end(), CompareProject());
	return TRUE; 
}

bool DBAccessContainer::ReadAllProjectsFromServer( vector<Project_DB> & projectlist, const wstring& creator )
{
	SetConnectionCharacterSet(MYSQL_UTF8);
	Scoped_Lock<Mutex> local_lock(m_recordmutex);
	// get the database names;
	vector<string> dbnames;
	if(!QueryAndStoreResult("show databases like 'uppc%%'"))
	{
		string error = GetMysqlError();
		return FALSE;
	}
	// get the database names;
	while(GetRow())
	{
		string name  = m_row[0];
		dbnames.push_back(name);
	}
	FreeResultSet();
	
	// get the project name for each database.
	for (vector<string>::iterator it = dbnames.begin(); it != dbnames.end(); it++)
	{
		string user =Project::Tools:: tstringToString(creator);
		string sql = "select name from ";
		sql += *it;
		sql += ".project where creator = '";
		sql += user;
		sql += "'";

		// if query failed, continue.
		if (mysql_query(&m_mysql, sql.c_str()))
			continue;
		
		// get the result. 
		// There is only one row in  the table 'Project'
		MYSQL_RES* result = mysql_use_result(&m_mysql);
		if (result != NULL)
		{
			MYSQL_ROW row = mysql_fetch_row(result);
			// read the project name
			if (row != NULL)
			{
				Project_DB projectdb;
				projectdb.strdatabasename = *it;

				wstring projectname ;
				if (row[0] != NULL)
				{
					// change the project name from utf-8 to utf-16.
					Project::Tools::UTF8ToWideChar(row[0], projectname);
					projectdb.strprojectname = projectname;
				}				
				projectlist.push_back(projectdb);
			}
			mysql_free_result(result);
		}		
	}
	
	std::sort(projectlist.begin(), projectlist.end(), CompareprojectDB());
	return TRUE;
}

void DBAccessContainer::DestroyInstance()
{
	if ( NULL != g_DBAccessContainer )
	{
		delete g_DBAccessContainer;
		g_DBAccessContainer = NULL;
	}
}

bool 
DBAccessContainer::ReadChillerEffDesignData( vector<gChiller_Eff_Design_Data*>& resultlist , int id )
{
	Scoped_Lock<Mutex>  scopelock(m_recordmutex);
	ostringstream sqlstream;
	sqlstream << "select * from uppc_libmodel.chiller_eff_design_data where chillerid = " << id;
	string sql = sqlstream.str();
	//chiller_eff_design_data(chillerid, lewt, ecwt, evap_flow, 
    //	                      cond_flow, cooling_cap, heating_cap, power)
	// query the sql statement. 
	if (Execute(sql.c_str()))
	{
		// the result record set my be huge, so mysql_store_result is not used because it
		// may be slow and need extra large memory.
		MYSQL_RES* result = mysql_use_result(&m_mysql);
		if (result != NULL)
		{
			MYSQL_ROW row;
			while(row = mysql_fetch_row(result))
			{
				gChiller_Eff_Design_Data* record = new gChiller_Eff_Design_Data;				
				if (record == NULL)				
					return FALSE;
				record->iChillerID = ATOI(row[CH_EFF_DD_CHILLERID]);
				record->dLEWT = ATOF(row[CH_EFF_DD_LEWT]);
				record->dECWT = ATOF(row[CH_EFF_DD_ECWT]);
				record->dEvap_Flow = ATOF(row[CH_EFF_DD_EVAP_FLOW]);
				record->dCond_Flow = ATOF(row[CH_EFF_DD_COND_FLOW]);
				record->dCooling_Cap = ATOF(row[CH_EFF_DD_COOLING_CAP]);
				record->dHeating_Cap = ATOF(row[CH_EFF_DD_HEATING_CAP]);
				record->dPower = ATOF(row[CH_EFF_DD_POWER]);
				
				resultlist.push_back(record);
			}
			mysql_free_result(result);
			return TRUE;
		}
	}
	else
		LogDBError(sql);
	return FALSE;
}

bool 
DBAccessContainer::ReadCoolingTowerEffDesignData( vector<gCT_Eff_Design_Data*>& resultlist , int id )
{
	Scoped_Lock<Mutex>  scopelock(m_recordmutex);
	ostringstream sqlstream;
	sqlstream << "select * from uppc_libmodel.coolingtower_eff_design_data where CoolingTowerID = " << id;
	string sql = sqlstream.str();
	//coolingtower_eff_design_data(CoolingTowerID, Twb_Air, T_water_in, T_water_out, 
	//	                           Freq, Flow_water, Flow_Air)
	// query the sql statement. 
	if (Execute(sql.c_str()))
	{
		// the result record set my be huge, so mysql_store_result is not used because it
		// may be slow and need extra large memory.
		MYSQL_RES* result = mysql_use_result(&m_mysql);
		if (result != NULL)
		{
			MYSQL_ROW row;
			while(row = mysql_fetch_row(result))
			{
				gCT_Eff_Design_Data* record = new gCT_Eff_Design_Data;				
				if (record == NULL)				
					return FALSE;

				record->iCTID = ATOI(row[CT_EFF_DD_CTID]);
				record->dTwb_Air = ATOF(row[CT_EFF_DD_TWB_AIR]);
				record->dT_Water_In = ATOF(row[CT_EFF_DD_T_WATER_IN]);
				record->dT_Water_Out = ATOF(row[CT_EFF_DD_T_WATER_OUT]);
				record->dFreq = ATOF(row[CT_EFF_DD_FREQ]);
				record->dFlow_Water = ATOF(row[CT_EFF_DD_FLOW_WATER]);
				record->dFlow_Air = ATOF(row[CT_EFF_DD_FLOW_AIR]);

				resultlist.push_back(record);
			}
			mysql_free_result(result);
			return TRUE;
		}
	}
	else
		LogDBError(sql);
	return FALSE;
}

bool DBAccessContainer::ReadPumpEffDesignData( vector<gPump_Eff_Design_Data*>& resultlist , int id )
{
	Scoped_Lock<Mutex>  scopelock(m_recordmutex);
	ostringstream sqlstream;
	sqlstream << "select * from uppc_libmodel.pump_eff_design_data where pumpid = " << id;
	string sql = sqlstream.str();

	//pump_eff_design_data(PumpID, Flow, Head, Freq, Power)
	// query the sql statement. 
	if (Execute(sql.c_str()))
	{
		// the result record set my be huge, so mysql_store_result is not used because it
		// may be slow and need extra large memory.
		MYSQL_RES* result = mysql_use_result(&m_mysql);
		if (result != NULL)
		{
			MYSQL_ROW row;
			while(row = mysql_fetch_row(result))
			{
				gPump_Eff_Design_Data* record = new gPump_Eff_Design_Data;				
				if (record == NULL)				
					return FALSE;
				record->iPumpID = ATOI(row[PM_EFF_DD_PMID]);
				record->dFlow = ATOF(row[PM_EFF_DD_FLOW]);
				record->dHead = ATOF(row[PM_EFF_DD_HEAD]);
				record->dFreq = ATOF(row[PM_EFF_DD_FREQ]);
				record->dPower = ATOF(row[PM_EFF_DD_POWER]);

				resultlist.push_back(record);
			}
			mysql_free_result(result);
			return TRUE;
		}
	}
	else
		LogDBError(sql);

	return FALSE;	
}

bool DBAccessContainer::DeleteEffDesignData( DEVICETYPE type, int deviceid )
{
	ostringstream sqlstream;

	switch(type)
	{
	case Type_CHILLER:
		sqlstream << "delete from uppc_libmodel.chiller_eff_design_data where chillerid = " << deviceid;
		break;
	case Type_PUMP:
		sqlstream << "delete from uppc_libmodel.pump_eff_design_data where pumpid = " << deviceid;
		break;
	case Type_COOLINGTOWER:
		sqlstream << "delete from uppc_libmodel.coolingtower_eff_design_data where CoolingTowerID = " << deviceid ;	
		break;
	}

	string sql = sqlstream.str();

	bool bresult = Execute(sql.c_str());
	if (!bresult)
	{
		LogDBError(sql);
	}

	return bresult;
}


MYSQL_RES*  DBAccessContainer::ReadModelIDs(DEVICETYPE type)
{
	ostringstream sqlstream;
	string sql;

	switch(type)
	{
	case Type_CHILLER:
		sqlstream << "select  chillermodelid from uppc_libmodel.chiller_model";
		break;
	case Type_PUMP:
		sqlstream << "select pumpmodelid from uppc_libmodel.pump_model";
		break;
	case Type_COOLINGTOWER:
		sqlstream << "select coolingtowermodelid from uppc_libmodel.coolingtower_model";
		break;
	case Type_WATERNET:
		sqlstream << "select model_id from uppc_libmodel.waternet_model";
		break;
	case Type_WATERSOIL:
		sqlstream << "select watersoilmodelid from uppc_libmodel.watersoil_model";
		break;
	}

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		LogDBError(sqlstream.str());
	}

	return result;
}



MYSQL_RES* DBAccessContainer::ReadALLModel( DEVICETYPE type )
{
	ostringstream sqlstream;

	switch(type)
	{
	case Type_CHILLER:
		sqlstream << "select * from uppc_libmodel.chiller_model";
		break;
	case Type_PUMP:
		sqlstream << "select * from uppc_libmodel.pump_model";
		break;
	case Type_COOLINGTOWER:
		sqlstream << "select * from uppc_libmodel.coolingtower_model";
		break;
	case Type_WATERNET:
		sqlstream << "select * from uppc_libmodel.waternet_model";
		break;
	}

	MYSQL_RES* result =  QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		LogDBError(sqlstream.str());
	}

	return result;
}

bool DBAccessContainer::DeleteFromModelTable( DEVICETYPE type, int modelid )
{
	ostringstream sqlstream;

	switch(type)
	{
	case Type_CHILLER:
		sqlstream << "delete from uppc_libmodel.chiller_model where ChillerModelID = " << modelid;
		break;
	case Type_PUMP:
		sqlstream << "delete from uppc_libmodel.pump_model where PumpModelID = " << modelid;
		break;
	case Type_COOLINGTOWER:
		sqlstream << "delete from uppc_libmodel.coolingtower_model where CoolingTowerModelID = " << modelid;
		break;
	case Type_WATERNET:
		sqlstream << "delete from uppc_libmodel.waternet_model where model_id = " << modelid;
		break;
	case Type_WATERSOIL:
		sqlstream << "delete from uppc_libmodel.watersoil_model where model_id = " << modelid;
		break;
	}

	bool bresult =  Execute(sqlstream.str().c_str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

MYSQL_RES* DBAccessContainer::ReadRoomLoadDefine( int roomid )
{
	ostringstream sqlstream;
	
	sqlstream << "select * from chiller_plant_room_load_define where OF_CPR_ID = " << roomid << " order by Time";
	
	MYSQL_RES* bresult = QueryAndReturnResult(sqlstream.str().c_str());
	if (bresult == NULL)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

bool DBAccessContainer::InsertIntoRoomLoadDefine(const vector<gRoomLoad*>& record)
{
	//assert(record != NULL);
	if (record.empty()) return true;

	Scoped_Lock<Mutex>  scopelock(m_recordmutex);

	if (StartTransaction())
	{		
		ostringstream sqlstream;
		sqlstream << "insert into chiller_plant_room_load_define values";
		for (vector<gRoomLoad*>::const_iterator it = record.begin(); it != record.end(); it++)
		{
			gRoomLoad* data = *it;
			string dt = SystemTimeToString(data->tmTime);
			sqlstream << "('" << dt << "'," << data->dCoolingLoad << "," << data->dHeatingLoad << "," \
				<< data->dTotal_Flow << "," << data->dTotal_T_Supply << "," << data->dTotal_T_Return << "," << data->iRoomID << "," << data->dTdry << "," \
				<< data->dTwet <<","<<data->fPowerBill <<","<<data->fPowerBill <<","<<data->dCoolingLoad1<<","<<data->dCoolingLoad2<<","<<data->dCoolingLoad3<<","<<data->dCoolingLoad4<< ")";
			if (it != record.end() - 1)			
				sqlstream << "," ;			
		}

		string str = sqlstream.str();
		bool result = Execute(str);
		if (!result)
		{
			LogDBError(str);
		}

		return Commit() && (!result);
	}
	else
		LogDBError("start transaction");

	return FALSE;
}

bool DBAccessContainer::DeleteFromRoomLoadDefine( int roomid )
{
	ostringstream sqlstream;

	sqlstream << "delete from chiller_plant_room_load_define where OF_CPR_ID = " << roomid;

	bool bresult = Execute(sqlstream.str().c_str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

int DBAccessContainer::GetMaxIDInTable( string tablename, string fieldname )
{
	Scoped_Lock<Mutex>  scopelock(m_recordmutex);
	ostringstream sqlstream;
	sqlstream << "select max(" << fieldname << ") from " << tablename;
	
	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	assert(result != NULL);
	MYSQL_ROW row = FetchRow(result);
	
	return ATOI(row[0]);
}

bool DBAccessContainer::ClearAllRooms()
{
	return  TruncateTable("room_contain_chiller") &&TruncateTable("room_contain_coolingtower")&&TruncateTable("room_contain_corner")\
		&&TruncateTable("room_contain_hx")&&TruncateTable("room_contain_pipe")&&TruncateTable("room_contain_pump")&&TruncateTable("room_contain_terminal")\
		&&TruncateTable("room_contain_valve")&&TruncateTable("room_contain_waternet")\
		&&TruncateTable("room_contain_tank")&&TruncateTable("chiller_plant_room_load_define")&&\
		TruncateTable("room_contain_watersoil");	
}

bool DBAccessContainer::ClearChillerPlantRoom()
{
	return TruncateTable("chillerplantroom");
}

bool DBAccessContainer::InsertIntoWaterParam( gWaternetParam* record )
{
	assert(record != NULL);
	ostringstream sqlstream;
	sqlstream << "insert into uppc_libmodel.waternet_param values(" << record->iWaternetId <<"," << record->dFrequency \
			  << "," << record->dHead << "," << record->dPower << "," << record->dWaterFlow <<")";
		
	bool bresult =  Execute(sqlstream.str().c_str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

MYSQL_RES* DBAccessContainer::ReadWaternetParam( int id )
{
	ostringstream sqlstream;
	sqlstream << "select * from uppc_libmodel.waternet_param where waternetid = " << id ;
	
	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		LogDBError(sqlstream.str());
	}
	return result;
}

MYSQL_RES* DBAccessContainer::ReadWaternetParamIDs()
{
	string sql;
	sql = "select distinct waternetid from uppc_libmodel.waternet_param";

	MYSQL_RES* result =  QueryAndReturnResult(sql.c_str());
	if (result == NULL)
	{
		LogDBError(sql);
	}

	return result;
}

bool DBAccessContainer::DeleteWaternetParam( int id )
{
	ostringstream sqlstream;
	sqlstream << "delete from uppc_libmodel.waternet_param where waternetid = " << id;

	bool bresult =  Execute(sqlstream.str().c_str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

bool DBAccessContainer::ClearWaternetParam()
{
	return TruncateTable("uppc_libmodel.waternet_param");
}

int DBAccessContainer::ReadMaxIDInRoom( DEVICETYPE type )
{
	char sql[128];
	memset(sql, 0x00, sizeof(sql));

	int result = 0;	

	switch(type)
	{
	case Type_CHILLER:
		sprintf_s(sql, sizeof(sql), "select max(Chiller_ID_In_Room) from room_contain_chiller;");		
		break;
	case Type_PUMP:
		sprintf_s(sql, sizeof(sql), "select max(Pump_ID_In_Room) from room_contain_pump;");		
		break;			
	case Type_COOLINGTOWER:
		sprintf_s(sql, sizeof(sql), "select max(Tower_ID_In_Room) from room_contain_coolingtower;");
		break;
	case Type_HX:
		sprintf_s(sql, sizeof(sql), "select max(Hx_ID_In_Room) from room_contain_hx;");
		break;
	case Type_PIPE:
		sprintf_s(sql, sizeof(sql), "select max(pipe_ID_In_Room) from room_contain_pipe;");
		break;
	case Type_CORNER:
		sprintf_s(sql, sizeof(sql), "select max(Corner_ID_InRoom) from room_contain_Corner;");
		break;
	case Type_TERMINAL:
		sprintf_s(sql, sizeof(sql), "select max(Terminal_ID_InRoom) from room_contain_terminal;");
		break;
	case Type_VALVE:
		sprintf_s(sql, sizeof(sql), "select max(Valve_ID_In_Room) from room_contain_Valve;");
		break;
	case Type_WATERNET:
		sprintf_s(sql, sizeof(sql), "select max(waternetid) from room_contain_waternet;");
		break;
	case Type_Tank:
		sprintf_s(sql, sizeof(sql), "select max(id_in_room) from room_contain_tank;");
		break;
	case Type_WATERSOIL:
		sprintf_s(sql, sizeof(sql), "select max(id) from room_contain_watersoil;");
		break;
	default:
		break;
	}

	Scoped_Lock<Mutex> mut(m_recordmutex);

	if (QueryAndStoreResult(sql))
	{
		if (GetRow())
		{
			result = ATOI(m_row[0]);
		}
		FreeResultSet();
	}
	else
		LogDBError(sql);

	return result;
}

bool DBAccessContainer::InsertChillerEffDesignData( const vector<gChiller_Eff_Design_Data*>& record )
{
	if (StartTransaction())
	{		
		ostringstream sqlstream;
		sqlstream << "insert into uppc_libmodel.chiller_eff_design_data values";
		for (vector<gChiller_Eff_Design_Data*>::const_iterator it = record.begin(); it != record.end(); it++)
		{
			gChiller_Eff_Design_Data* data = *it;
			
			sqlstream << "(" << data->iChillerID << "," \
					  << data->dLEWT << "," << data->dECWT << "," << data->dEvap_Flow << "," \
					  << data->dCond_Flow << "," << data->dCooling_Cap << "," << data->dHeating_Cap << ","\
					  << data->dPower << ")";
			if (it != record.end() - 1)
			{
				sqlstream << "," ;
			}
		}
		string str = sqlstream.str();
		bool result = !mysql_real_query(&m_mysql, str.data(), (unsigned long)str.size());
		if (!result)
		{
			LogDBError(str);
		}

		return Commit() && (!result);
	}
	else
		LogDBError("start transaction");
	
	return FALSE;
}

bool DBAccessContainer::InsertPumpEffDesignData( const vector<gPump_Eff_Design_Data*>& record )
{
	if (StartTransaction())
	{
		ostringstream sqlstream;
		sqlstream << "insert into uppc_libmodel.pump_eff_design_data values";
		for (vector<gPump_Eff_Design_Data*>::const_iterator it = record.begin(); it != record.end(); it++)
		{
			gPump_Eff_Design_Data* data = *it;
			
			sqlstream << "(" << data->iPumpID << "," << data->dFlow << "," \
				<< data->dHead << "," << data->dFreq << "," << data->dPower << ")";
			if (it != record.end() - 1)
			{
				sqlstream << "," ;
			}
		}
		string str = sqlstream.str();
		bool result = !mysql_real_query(&m_mysql, str.data(), (unsigned long)str.size());
		if (!result)
		{
			LogDBError(str);
		}

		return Commit() && (!result);
	}
	else
		LogDBError("start transaction");
	
	return FALSE;
}

bool DBAccessContainer::InsertCoolingTowerDesignData( const vector<gCT_Eff_Design_Data*>& record )
{
	if (StartTransaction())
	{
		ostringstream sqlstream;
		sqlstream << "insert into uppc_libmodel.coolingtower_eff_design_data values";
		for (vector<gCT_Eff_Design_Data*>::const_iterator it = record.begin(); it != record.end(); it++)
		{
			gCT_Eff_Design_Data* data = *it;

			sqlstream << "(" << data->iCTID << "," << data->dTwb_Air << "," \
				<< data->dT_Water_In << "," << data->dT_Water_Out << "," << data->dFreq << "," << data->dFlow_Water \
				<< "," << data->dFlow_Air << ")";
			if (it != record.end() - 1)
			{
				sqlstream << "," ;
			}
		}
		string str = sqlstream.str();
		bool result = !mysql_real_query(&m_mysql, str.data(), (unsigned long)str.size());
		if (!result)
		{
			LogDBError(str);
		}
		return Commit() && (!result);
	}
	else
		LogDBError("start transaction");

	return FALSE;
}

bool DBAccessContainer::GetProvinceList( vector<wstring>& provincelist )
{
	string sql = "select distinct province from weather.weather_station";
	MYSQL_RES* result = NULL;
	result = QueryAndReturnResult(sql.c_str());
	if (result != NULL)
	{
		MYSQL_ROW row;
		while (row = FetchRow(result))
		{
			if (row[0] != NULL)
			{
                wstring provincename;
				Project::Tools::UTF8ToWideChar(row[0], provincename);
				provincename.erase(provincename.begin());
				provincename.erase(provincename.end() - 1);
				provincelist.push_back(provincename);
			}
		}
		FreeReturnResult(result);
		return TRUE;
	}
	else
		LogDBError(sql);
	
	return FALSE;
}

bool DBAccessContainer::GetCityList( vector< pair<long, wstring> >& citylist, wstring provincename )
{
	ostringstream sqlstream;
	wstring newname = L"\"";
	newname += provincename;
	newname += L"\"";
	string name_utf8 = WideCharToUtf8(newname.c_str());
	sqlstream << "select station_id, name from weather.weather_station where province like '" << name_utf8 << "'";

	string sql = sqlstream.str();
	MYSQL_RES* result = NULL;
	result = QueryAndReturnResult(sql.c_str());
	if (result != NULL)
	{
		MYSQL_ROW row;
		while(row = FetchRow(result))
		{
			pair<int, wstring> cityidname;
			cityidname.first = ATOI(row[0]);
			if (row[1] != NULL)
			{				
				Project::Tools::UTF8ToWideChar(row[1],cityidname.second);
				cityidname.second.erase(cityidname.second.begin());
				cityidname.second.erase(cityidname.second.end() - 1);
			}
			citylist.push_back(cityidname);
		}

		FreeReturnResult(result);
		return TRUE;
	}
	else
		LogDBError(sql);
		
	return FALSE;
}

gWeatherDesignData* DBAccessContainer::GetWeatherDesignData( long stationid )
{
	ostringstream sqlstream;
	sqlstream << "select * from weather.weather_design_data where station_id = " << stationid;
	
	string sql = sqlstream.str();

	MYSQL_RES* result = NULL;
	result = QueryAndReturnResult(sql.c_str());
	if (result != NULL)
	{
		MYSQL_ROW row;
		gWeatherDesignData* data = NULL;
		while(row = FetchRow(result))
		{
			data = new gWeatherDesignData;
			data->lstation_id		=	ATOI(row[WD_STATION_ID]);
			data->dw_t_heating		=	ATOF(row[W_T_HEATING]);
			data->dw_t_vent			=	ATOF(row[W_T_VENT]);
			data->dw_t_ac			=	ATOF(row[W_T_AC]);
			data->dw_rh_ac			=	ATOF(row[W_RH_AC]);
			data->dw_ws_mean		=	ATOF(row[W_WS_MEAN]);
			data->dw_ws_meanmain	=	ATOF(row[W_WS_MEAN_MAIN]);
			
			Project::Tools::String_To_Tstring(row[W_WD_MAIN], data->strw_wd_main);
			data->strw_wd_main.erase(data->strw_wd_main.begin());
			data->strw_wd_main.erase(data->strw_wd_main.end() - 1);
			//data->strw_wd_main		=	row[W_WD_MAIN];
			
			data->dw_f_wd_main		=	ATOF(row[W_F_WD_MAIN]);
			data->dw_p_air			=	ATOF(row[W_P_AIR]);
			data->dw_s_ratio		=	ATOF(row[W_S_RATIO]);
			data->dw_date_heating	=	ATOF(row[W_DAYS_HEATING]);
			data->dw_date_heating_start =	ATOF(row[W_DATE_HEATING_START]);
			data->dw_date_heating_end	=	ATOF(row[W_DATE_HEATING_END]);
			data->ds_t_vent			=	ATOF(row[S_T_VENT]);
			data->ds_rh_vent		=	ATOF(row[S_RH_VENT]);
			data->ds_t_ac			=	ATOF(row[S_T_AC]);
			data->ds_ts_ac			=	ATOF(row[S_TS_AC]);
			data->ds_t_mean_ac		=	ATOF(row[S_T_MEAN_AC]);
			data->ds_ws_mean		=	ATOF(row[S_WS_MEAN]);

			Project::Tools::String_To_Tstring(row[S_WD_MAIN], data->strs_wd_main);
			data->strs_wd_main.erase(data->strs_wd_main.begin());
			data->strs_wd_main.erase(data->strs_wd_main.end() - 1);
			//data->strs_wd_main		=	row[S_WD_MAIN];

			data->ds_f_wd_main		=	ATOF(row[S_F_WD_MAIN]);
			data->ds_p_air			=	ATOF(row[S_P_AIR]);
			data->dt_min			=	ATOF(row[T_MIN]);
			data->dt_max			=	ATOF(row[T_MAX]);

			Project::Tools::String_To_Tstring(row[Y_WD_MAIN], data->stry_wd_main);
			data->stry_wd_main.erase(data->stry_wd_main.begin());
			data->stry_wd_main.erase(data->stry_wd_main.end() - 1);
			//data->stry_wd_main		=	row[Y_WD_MAIN];

			data->dy_f_wd_main		=	ATOF(row[Y_F_WD_MAIN]);			
		}
		FreeReturnResult(result);
		return data;
	}
	else
		LogDBError(sql);
	
	return NULL;
}

bool DBAccessContainer::GetWeatherHourData( vector<gWeatherHourData* >& resultlist, long stationid )
{
	ostringstream sqlstream;
	sqlstream << "select  * from weather.weather_hour_data where station_id = " << stationid;
	string sql = sqlstream.str();
	
	MYSQL_RES* result = NULL;
	if (Execute(sql.c_str()))
	{
		MYSQL_RES* result = NULL;
		result = mysql_use_result(&m_mysql);
		if (result != NULL)
		{
			MYSQL_ROW row;
			while(row = FetchRow(result))
			{
				gWeatherHourData* data = new gWeatherHourData;
				data->lstation_id	= ATOI(row[WH_STATION_ID]);
				data->dtype			= ATOF(row[WH_TYPE]);
				data->dhour			= ATOF(row[WH_HOUR]);
				data->dT			= ATOF(row[WH_T]);
				data->dD			= ATOF(row[WH_D]);
				data->dRT			= ATOF(row[WH_RT]);
				data->dRR			= ATOF(row[WH_RR]);
				data->dt_ground		= ATOF(row[WH_T_GROUND]);
				data->dt_sky		= ATOF(row[WH_T_SKY]);
				data->dWS			= ATOF(row[WH_WS]);
				data->dWD			= ATOF(row[WH_WD]);
				data->dB			= ATOF(row[WH_B]);
				resultlist.push_back(data);
			}
			FreeReturnResult(result);
			return TRUE;
		}
	}
	else
		LogDBError(sql);
		
	return FALSE;
}

bool DBAccessContainer::InsertWarningRecord(SYSTEMTIME st, const wstring& warninginfo)
{
    Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);
	SetConnectionCharacterSet();
	
	ostringstream sqlstream;
	string recordtime = SystemTimeToString(st);
	
	string warninginfo_utf8;
	Project::Tools::WideCharToUtf8(warninginfo, warninginfo_utf8);
	
	sqlstream << "insert into warning_record values('" << recordtime << "', '" << warninginfo_utf8 << "')";

	bool bresult =  Execute(sqlstream.str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}
	return bresult;
}


//bool DBAccessContainer::InsertOperationRecord( SYSTEMTIME st, wstring user, SYSTEMOPERATIONS optindex)
//{
//	Scoped_Lock<Mutex> scopelock(m_recordmutex);
//	
//	string recordtime = Project::Tools::SystemTimeToString(st);
//	ostringstream sqlstream;
//	string user_ansi = Project::Tools::tstringToString(user);
//	sqlstream << "insert into operation_record values('" << recordtime << "'," << user_ansi << "," << optindex << ")";
//
//	bool bresult =  Execute(sqlstream.str());
//	if (!bresult)
//	{
//		LogDBError(sqlstream.str());
//	}
//
//	return bresult;
//}

bool DBAccessContainer::InsertOperationRecord( const SYSTEMTIME& st, const wstring& remark, const wstring& wstrUser )
{
    Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);
    
    SetConnectionCharacterSet();
    
    const string strRemark = Project::Tools::WideCharToUtf8(remark.c_str());
    const string strUser = Project::Tools::WideCharToUtf8(wstrUser.c_str());

	ostringstream sqlstream;
	sqlstream	<< "insert into "
				<< GetDBName() << "."<<OPERATION_RECORD_TABLE <<" values('"				
				<< Project::Tools::SystemTimeToString(st) << "'"
                << "," <<"'"<< strUser << "'"
                << "," <<"'"<< strRemark << "'"
                << ")";

	string sql = sqlstream.str();
	bool bResult = Execute(sql);
	if (!bResult)
	{
		LogDBError(sql);
	}
	
	return bResult;
}

gPump_Model *DBAccessContainer::ReadPumpModel( int nPumpModelID )
{
	_bstr_t	bstr;
	MYSQL_RES* pMySql_Res = ReadModel( Type_PUMP, nPumpModelID );

	gPump_Model *m_pPumpModel = new gPump_Model();
	MYSQL_ROW row = FetchRow(pMySql_Res);
	if(row)
	{
		m_pPumpModel->iPumpModelID     = ATOI(row[PM_MODEL_PUMPMODELID]);

		if (row[PM_MODEL_PUMPMODELNAME] != NULL)
		{
			Project::Tools::UTF8ToWideChar(row[PM_MODEL_PUMPMODELNAME],m_pPumpModel->strPumpModelName);
		}

		m_pPumpModel->iPumpID          = ATOI(row[PM_MODEL_PUMPID]);
		m_pPumpModel->iType            = ATOI(row[PM_MODEL_TYPE]);
		//pPumpModel->tmModel_Gen_Time = row[PM_MODEL_GEN_TIME];
		tstring strGenTime = Project::Tools::AnsiToWideChar(row[CH_MODEL_GEN_TIME]);
		Project::Tools::String_To_SysTime( strGenTime, m_pPumpModel->tmModel_Gen_Time );
		bstr						   = row[PM_MODEL_PARAM];
		m_pPumpModel->strModel_Param   = bstr;
		FreeReturnResult(pMySql_Res);
		return m_pPumpModel;
	}

	return NULL;
}

gCT_Model *DBAccessContainer::ReadCTModel( int nCTMODELID )
{
	_bstr_t	bstr;
	MYSQL_RES* pMySql_Res = ReadModel( Type_COOLINGTOWER, nCTMODELID );

	
	MYSQL_ROW row = FetchRow(pMySql_Res);
	if(row)
	{
		gCT_Model *m_pCTModel = new gCT_Model();
		m_pCTModel->iModelID        = ATOI(row[CT_MODEL_ID]);

		if (row[CT_MODEL_NAME] != NULL)
		{
			Project::Tools::UTF8ToWideChar(row[CT_MODEL_NAME], m_pCTModel->strModelName);
		}

		m_pCTModel->iCoolingTowerID = ATOI(row[CT_MODEL_CTID]);
		m_pCTModel->iType           = ATOI(row[CT_MODEL_TYPE]);
		//pCTModel->tmModel_Gen_Time = row[CT_MODEL_GEN_TIME];
		tstring strGenTime = Project::Tools::AnsiToWideChar(row[CH_MODEL_GEN_TIME]);
		Project::Tools::String_To_SysTime( strGenTime , m_pCTModel->tmModel_Gen_Time );
		bstr					  = row[CT_MODEL_PARAM];
		m_pCTModel->strModel_Param  = bstr;
		FreeReturnResult(pMySql_Res);
		return m_pCTModel;
	}

	return NULL;
}

gWaterSoil_Model * DBAccessContainer::ReadWSModel( int nWSMODELID )
{
	_bstr_t	bstr;
	MYSQL_RES* pMySql_Res = ReadModel( Type_WATERSOIL, nWSMODELID );

	gWaterSoil_Model *m_pWSModel = new gWaterSoil_Model();
	MYSQL_ROW row = FetchRow(pMySql_Res);
	if(row)
	{
		m_pWSModel->iWaterSoilModelID        = ATOI(row[WS_MODEL_WATERSOILMODELID]);

		if (row[WS_MODEL_WATERSOILMODELNAME] != NULL)
		{
			Project::Tools::UTF8ToWideChar(row[WS_MODEL_WATERSOILMODELNAME], m_pWSModel->strWaterSoilModelName);
		}

		m_pWSModel->iWaterSoilID = ATOI(row[WS_MODEL_WATERSOILID]);
		m_pWSModel->iType           = ATOI(row[WS_MODEL_TYPE]);
		tstring strGenTime = Project::Tools::AnsiToWideChar(row[WS_MODEL_GEN_TIME]);
		Project::Tools::String_To_SysTime( strGenTime , m_pWSModel->tmModel_Gen_Time );
		bstr					  = row[WS_MODEL_PARAM];
		m_pWSModel->strModel_Param  = bstr;
		FreeReturnResult(pMySql_Res);
		return m_pWSModel;
	}
    return NULL;
}

gCH_Model *DBAccessContainer::ReadChillerModel( int nChillerModelID )
{
	MYSQL_RES* pMySql_Res = ReadModel( Type_CHILLER, nChillerModelID );

	if (NULL == pMySql_Res)
	{
		return NULL;
	}

	unsigned __int64 rownum = GetRowNum(pMySql_Res);

	MYSQL_ROW row = FetchRow(pMySql_Res);

	gCH_Model * m_pCHModel = NULL;

	if(row)
	{
		m_pCHModel = new gCH_Model();
		m_pCHModel->iChillerModelID     = ATOI(row[CH_MODEL_CHILLERMODELID]);
		if (row[CH_MODEL_CHILLERMODELNAME] != NULL)
		{
			Project::Tools::UTF8ToWideChar(row[CH_MODEL_CHILLERMODELNAME],m_pCHModel->strCHillerModelName);
		}

		m_pCHModel->iChillerID          = ATOI(row[CH_MODEL_CHILLERID]);
		m_pCHModel->iType               = ATOI(row[CH_MODEL_TYPE]);
		//pCHModel->tmModel_Gen_Time    = row[CH_MODEL_GEN_TIME];
		tstring strGenTime = Project::Tools::AnsiToWideChar(row[CH_MODEL_GEN_TIME]);
		Project::Tools::String_To_SysTime( strGenTime , m_pCHModel->tmModel_Gen_Time );		
		Project::Tools::String_To_Tstring(row[CH_MODEL_PARAM], m_pCHModel->strModel_Param);

		FreeReturnResult(pMySql_Res);
		return m_pCHModel;
	}
	return NULL;
}

bool DBAccessContainer::AddNewUser( gUppcSimulateUser* record )
{
	assert (record != NULL);
	ostringstream sqlstream;

	string username = Project::Tools::tstringToString(record->username);
	string pwd = Project::Tools::tstringToString(record->password);
	string lastlogintime = SystemTimeToString(record->lastlogintime);

	sqlstream << "insert into uppc_user.users values('" << username << "','" << pwd << "'," \
			<< record->readright << "," << record->writeright << "," << record->modelright << "," \
			<< record->simulateright << ",'" << lastlogintime << "')" ;

	bool bresult = Execute(sqlstream.str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}
	
	return bresult;
}

bool DBAccessContainer::DeleteUser( wstring username )
{
	string user = Project::Tools::tstringToString(username);
	ostringstream sqlstream;
	sqlstream << "delete from uppc_user.users where username = '" << user << "'";

	bool bresult = Execute(sqlstream.str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

bool DBAccessContainer::ReadAllUsers( vector<wstring>& userlist )
{
	string sql = "select username from uppc_user.users" ;
	
	if (!userlist.empty())
		userlist.clear();

	MYSQL_RES* result = QueryAndReturnResult(sql.c_str());
	if (result != NULL)
	{
		MYSQL_ROW row;
		while(row = FetchRow(result))
		{
			wstring	username;
			String_To_Tstring(row[0], username);
			userlist.push_back(username);
		}
		FreeReturnResult(result);

		return TRUE;
	}
	else
		LogDBError(sql);

	return FALSE;
}

bool DBAccessContainer::ReadUserInfo( wstring username, UppcUserInfo* puserinfo )
{
	assert (puserinfo != NULL);

	ostringstream sqlstream;
	string user = Project::Tools::tstringToString(username);
	sqlstream << "select readright, writeright, modelright, simulateright, lastlogintime from uppc_user.users where username = '" << user << "'" ;
	
	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result != NULL)
	{
		MYSQL_ROW row;
		while(row = FetchRow(result))
		{
			puserinfo->username  = username;
			puserinfo->readright = ATOI(row[0]);
			puserinfo->writeright = ATOI(row[1]);
			puserinfo->modelright = ATOI(row[2]);
			puserinfo->simulateright = ATOI(row[3]);			
			String_To_Tstring(row[4], puserinfo->lastlogintime);
		}
		FreeReturnResult(result);
		return TRUE;
	}		
	else
		LogDBError(sqlstream.str());

	return FALSE;
}

bool DBAccessContainer::ReadALLUserInfo( vector<UppcUserInfo*>& userlist )
{
	string sql = "select * from uppc_user.users";

	MYSQL_RES* result = QueryAndReturnResult(sql.c_str());
	if (result != NULL)
	{
		MYSQL_ROW row;
		while (row = FetchRow(result))
		{
			UppcUserInfo* pinfo = new UppcUserInfo;
			String_To_Tstring(row[0], pinfo->username);
			pinfo->readright = ATOI(row[2]);
			pinfo->writeright = ATOI(row[3]);
			pinfo->modelright = ATOI(row[4]);
			pinfo->simulateright = ATOI(row[5]);
			String_To_Tstring(row[6], pinfo->lastlogintime); 
			
			userlist.push_back(pinfo);
		}
		
		FreeReturnResult(result);
		return TRUE;
	}	
	else
		LogDBError(sql);

	return FALSE;
}

bool DBAccessContainer::ValidateUser( wstring username, wstring password )
{
	ostringstream	sqlstream;
	string user = Project::Tools:: tstringToString(username);
	string pwd  = Project::Tools::tstringToString(password);

	sqlstream << "select 1 from uppc_user.users where username = '" << user << "' and userpwd = '" << pwd << "'";
	MYSQL_RES *result = QueryAndReturnResult(sqlstream.str().c_str());
	
	
	if (result != NULL)
	{
		if (FetchRow(result) != NULL)
		{
			FreeReturnResult(result);
			return TRUE;
		}
	}
	else
		LogDBError(sqlstream.str());

	return FALSE;
}

void DBAccessContainer::LogDBError(const char* sql)
{
	wstring sqlstatement;
	Project::Tools::String_To_Tstring(sql, sqlstatement);

	wstring mysqlerror;

	Project::Tools::String_To_Tstring(GetMysqlError(), mysqlerror);

	mysqlerror += L" while ";

	mysqlerror += sqlstatement;
		
	m_log.WriteLog(mysqlerror);
}

void DBAccessContainer::LogDBError( const string& sql )
{
	LogDBError(sql.c_str());
}

bool DBAccessContainer::DeleteRunRecord( SYSTEMTIME start, SYSTEMTIME end, int roomid, int deviceid, RECORDTYPE type )
{
    Project::Tools::Scoped_Lock<Mutex> lock(m_recordmutex);

	ostringstream sqlstream;
	string from = Project::Tools::SystemTimeToString(start);
	string to = Project::Tools::SystemTimeToString(end);


	switch(type)
	{
	case RecordType_Chiller:
		sqlstream << "delete from chiller_run_record where ChillerIDInRoom = " << deviceid << " and OF_CPR_ID = " \
				  << roomid << " and Time >= '" << from << "' and Time <= '" << to << "'";
		break;
	case RecordType_Pump:
		sqlstream << "delete from pump_run_record where PumpIDInRoom = " << deviceid << " and OF_CPR_ID = " \
				  << roomid << " and Time >= '" << from << "' and Time <= '" << to << "'";
		break;
	case RecordType_CoolingTower:
		sqlstream << "delete from coolingtower_run_record where CoolingTowerIDInRoom = " << deviceid << " and OF_CPR_ID = " \
				  << roomid << " and Time >= '" << from << "' and Time <= '" << to << "'";	
		break;
	case RecordType_OpenChiller:
		sqlstream << "delete from chiller_open_record where ChillerID = " << deviceid << " and OF_CPR_ID = " \
			<< roomid << " and OpenDateTime >= '" << from << "' and OpenDateTime <= '" << to << "'";			
		break;
	case RecordType_Warning:
		sqlstream << "delete from warning_record where RecordTime >= '" << from << " and RecordTime <= '" << to << "'";
		break;
	case RecordType_ControlParam:
		sqlstream << "delete from controlparam_record where Time >= '" << from << " and Time <= '" << to << "'";
		break;
	case Recordtype_Operation:
			sqlstream << "delete from operation_record where RecordTime >= '" << from << " and RecordTime <= '" << to << "'";
			break;

	}

	string sql = sqlstream.str();
	bool bresult = Execute(sql);
	if (!bresult)
		LogDBError(sql);

	return TRUE;

}

void DBAccessContainer::ReadRecordDateRange( vector<string>& daterange, RECORDTYPE recordtype, 
											int roomid, int deviceid )
{
    Project::Tools::Scoped_Lock<Mutex> locallock(m_recordmutex);
	daterange.clear();
	
	ostringstream sqlstream;
	switch(recordtype)
	{
	case RecordType_Chiller:
		sqlstream << "select distinct Date(Time) from chiller_run_record \
					where chilleridinroom = " << deviceid << " and OF_CPR_ID = " << roomid;
		break;
	case RecordType_Pump:
		sqlstream << "select distinct Date(Time) from pump_run_record \
					 where pumpidinroom = " << deviceid << " and OF_CPR_ID = " << roomid;
		break;
	case RecordType_CoolingTower:
		sqlstream << "select distinct Date(Time) from coolingtower_run_record\
					 where coolingtoweridinroom = " << deviceid << " and OF_CPR_ID = " << roomid;
		break;
	case RecordType_ControlParam:
		sqlstream << "select distinct Date(Time) from controlparam_record";
		break;
	case RecordType_OpenChiller:
		sqlstream << "select distinct Date(opendatetime) from chiller_open_record \
					 where chilleridinroom = " << deviceid << " and OF_CPR_ID = " << roomid;
		break;
	case RecordType_Warning:
		sqlstream << "select distinct Date(RecordTime) from warning_record";
		break;
	case Recordtype_Operation:
        //operation_record
		sqlstream << "select distinct Date(RecordTime) from " << OPERATION_RECORD_TABLE;
		break;
	}
	
	string sql = sqlstream.str();

	// execute the query
	if (Execute(sql))
	{
		MYSQL_RES* result = mysql_use_result(&m_mysql);
		if (result != NULL)
		{	
			MYSQL_ROW row = NULL;
			while (row = mysql_fetch_row(result))
			{
				if (row[0] != NULL)
					daterange.push_back(row[0]);
			}
		}	
	}
	else
	{
		LogDBError(sql);
	}

}
//bird add, read all operation record
//直接查24h的数据, 在数据库查询排序
bool    DBAccessContainer::ReadAllOperationRecord(VEC_OP_RECORD&  vecOperation, const COleDateTime& timeStart, const COleDateTime& timeStop)
{
    Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

    SetConnectionCharacterSet();
    bool ifok = false;

    //const int hourMax = 24;
    wstring wstrStart, wstrStop;
    string strStart, strStop;
    Project::Tools::OleTime_To_String(timeStart, wstrStart);
    Project::Tools::OleTime_To_String(timeStop, wstrStop);
    strStart = UtilString::ConvertWideCharToMultiByte(wstrStart);
    strStop = UtilString::ConvertWideCharToMultiByte(wstrStop);

    ostringstream sqlstream;
    //SELECT * FROM operation_record where recordtime >= '2011-05-30 00:00:00' and recordtime <= '2011-05-30 14:00:00';
    sqlstream << "select * from " << OPERATION_RECORD_TABLE <<" where recordtime >= '"<< strStart.c_str() <<"' and recordtime <= '"<< strStop.c_str() << "'";

    string sql = sqlstream.str();
    COperationRecord  tempOpera;
    vecOperation.clear();

    COleDateTime old;

    // execute the query
    //if (Execute(sql))
    {
        //MYSQL_RES* result = mysql_use_result(&m_mysql);
        MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
        if (result)
        {	
            MYSQL_ROW row = NULL;
            while (row = mysql_fetch_row(result))
            {
                if (row[0] && row[1] && row[2]){
                    //daterange.push_back(row[0]);
                    tempOpera.m_strTime = UtilString::ConvertMultiByteToWideChar(row[0] );
                    Project::Tools::UTF8ToWideChar(row[1],tempOpera.m_strUser); //bird add, 0630
                    Project::Tools::UTF8ToWideChar(row[2],tempOpera.m_strOperation);

                    vecOperation.push_back(tempOpera);
                }
            }
            FreeReturnResult(result);
            ifok = true;
        }	
        else{
            ifok = false;
            LogDBError(sql);
        }
    }
    //else
    //{
    //    ifok = false;
    //    LogDBError(sql);
    //}

    return ifok;
}
//////////////////////////////////////////////////////////////////////////

bool DBAccessContainer::InsertFlowRecord( const vector<DataSensor_Record_Entry>& entrylist)
{
	return InsertPointRecordHelper("Flow_record", entrylist);
}

bool DBAccessContainer::InsertPowerRecord( const vector<DataSensor_Record_Entry>& entrylist)
{
	return InsertPointRecordHelper("Power_record", entrylist);
}


bool DBAccessContainer::InsertTemperatureRecord( const vector<DataSensor_Record_Entry>& entrylist)
{
	return InsertPointRecordHelper("Temperature_record", entrylist);
}

bool DBAccessContainer::InsertPressureDifferenceRecord( const vector<DataSensor_Record_Entry>& entrylist )
{
	return InsertPointRecordHelper("PressureDifference_record", entrylist);
}

bool DBAccessContainer::InsertPowerConsumptionRecord( const vector<DataSensor_Record_Entry>& entrylist )
{
	return InsertPointRecordHelper("PowerConsumption_record", entrylist);
}

//运行时间
bool DBAccessContainer::InsertRunningTimeRecord(const vector<DataSensor_Record_Entry>& entrylist )
{
	return InsertPointRecordHelper("RunningTime_record", entrylist);
}

//月度报告
bool DBAccessContainer::InsertMonthReportRecord(const vector<MonthReportParam>& vctParam)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);
	if (vctParam.empty()){
		return false;
	}

	ostringstream sqlstream;
	sqlstream << "insert into "
		<< GetDBName()
		<<".MonthReport_List"
		<< " values";

	vector<MonthReportParam>::const_iterator it = vctParam.begin();
	for (;it != vctParam.end(); ++it){
		sqlstream << "('" << Project::Tools::WideCharToAnsi((*it).strReportYM) << "'," 
			<< Project::Tools::WideCharToAnsi((*it).strEqmName) << "," 
			<< (*it).dPowerTotal<<","
			<< (*it).dTimeTotal << "),";
	}

	string sql_temp = sqlstream.str();
	if (sql_temp.empty()){
		return false;
	}

	//remove the last ','
	sql_temp.erase(--sql_temp.end());

	return Execute(sql_temp);
}


bool DBAccessContainer::InsertOnOffRecord( const vector<DataSensor_Record_Entry>& entrylist )
{
	return InsertPointRecordHelper("Device_OnOff_record", entrylist);
}

bool DBAccessContainer::InsertFreqRecord( const vector<DataSensor_Record_Entry>& entrylist)
{
	return InsertPointRecordHelper("Device_Freq_Record", entrylist);
}


bool DBAccessContainer::InsertSystemStatusRecord( const SYSTEMTIME& st, int status)
{
	char buff[25];
	sprintf_s(buff, sizeof(buff), "%d-%d-%d %d:%d:%d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	ostringstream sqlstream;
	sqlstream << "insert into "
			  << GetDBName() << ".SystemStatus_Record values('"
			  << buff << "',"
			  << status << ")";
	
	string sql = sqlstream.str();

	bool bResult = Execute(sql);
	if(!bResult)
	{
		LogDBError(sql);
	}
	
	return bResult;
}


bool DBAccessContainer::AlterEnableProperty( DEVICETYPE type, bool enable, int id, int roomid )
{
	char sql[128];
	memset(sql, 0x00, sizeof(sql));

	switch(type)
	{
	case Type_CHILLER:
		sprintf_s(sql, sizeof(sql),
		"update room_contain_chiller set enabled = %d where chiller_id_in_room = %d and of_cpr_id = %d", \
		enable ? 1 : 0, id, roomid);
		
		break;
	case Type_PUMP:
		sprintf_s(sql, sizeof(sql),
		"update room_contain_pump set enabled = %d where pump_id_in_room = %d and of_cpr_id = %d", \
		enable ? 1 : 0, id, roomid);
		
		break;
	case Type_COOLINGTOWER:
		sprintf_s(sql, sizeof(sql),
		"update room_contain_coolingtower set enabled = %d where tower_id_in_room = %d and of_cpr_id = %d", \
		enable ? 1 : 0, id, roomid);
		
		break;
	case Type_VALVE:
		sprintf_s(sql, sizeof(sql),
		"update room_contain_valve set enabled = %d where valve_id_in_room = %d and of_cpr_id = %d", \
		enable ? 1 : 0, id, roomid);
		
		break;
	}

	bool bResult = Execute(sql);
	if(!bResult)
		LogDBError(sql);
	
	return bResult;
}

bool DBAccessContainer::InsertPointRecordHelper( string tablename, const vector<DataSensor_Record_Entry>& entrylist )
{
    Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);
	if (entrylist.empty()){
		return false;
	}

	ostringstream sqlstream;
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename 
		<< " values";

	string str_temp = Project::Tools::SystemTimeToString(entrylist[0].current_time);
	vector<DataSensor_Record_Entry>::const_iterator it = entrylist.begin();
	for (;it != entrylist.end(); ++it){
		sqlstream << "('" << str_temp << "'," 
			<< (*it).pointindex << "," 
			<< (*it).sensor_value << "),";
	}

	string sql_temp = sqlstream.str();
	if (sql_temp.empty()){
		return false;
	}

	//remove the last ','
	sql_temp.erase(--sql_temp.end());

	return Execute(sql_temp);
}

bool DBAccessContainer::AddRecordPointList( const vector<Record_Point_List_Entry>& pointlist,
										    const wstring& tablename )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);
	if (pointlist.empty()){
		return false;
	}
	SetConnectionCharacterSet();
	
	ostringstream sqlstream;
	string tablename_ansi = Project::Tools::WideCharToAnsi(tablename.c_str());
	sqlstream << "insert into " <<  GetDBName()  << "."<< tablename_ansi << " values ";
	for (unsigned int i = 0; i < pointlist.size(); i++)
	{
		const Record_Point_List_Entry& entry = pointlist.at(i);
		string shortname;
		Project::Tools::WideCharToUtf8(entry.shortname, shortname);
		string longname;
		Project::Tools::WideCharToUtf8(entry.longname, longname);
		string remark;
		Project::Tools::WideCharToUtf8(entry.remark, remark);
		string unit;
		Project::Tools::WideCharToUtf8(entry.unit, unit);
		
		sqlstream << "(" << entry.index << ",'"
				  << shortname << "','"
				  << longname << "','"
				  << remark << "','"
				  << unit << "'),";
	}
	string sql = sqlstream.str();
	// remove the trailing ','
	sql.erase(--sql.end());

	return Execute(sql);
}

bool DBAccessContainer::ReadRecordPointList( vector<Record_Point_List_Entry>& pointlist_out,
											 const wstring& tablename )
{
    Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

	pointlist_out.clear();

	string tablename_ansi = Project::Tools::WideCharToAnsi(tablename.c_str());
	ostringstream sqlstream;
	sqlstream << "select * from " <<  GetDBName() << "." << tablename_ansi << " l order by l.index";
	
	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);
		
	int length = (int)GetRowNum(result);
	if (0 == length){
		return true;
	}
	pointlist_out.reserve(length);
	
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result))
	{
		Record_Point_List_Entry entry;
		memset(&entry, 0x00, sizeof(entry));
		entry.index = ATOI(row[RECORD_POINT_LIST_INDEX]);
		wstring shortname;
		wstring longname;
		wstring remark;
		wstring unit;
		Project::Tools::UTF8ToWideChar(row[RECORD_POINT_LIST_SHORTNAME], shortname);
		//Project::Tools::UTF8ToWideChar(row[RECORD_POINT_LIST_LONGNAME], longname);
		Project::Tools::UTF8ToWideChar(row[RECORD_POINT_LIST_REMARK], remark);
		Project::Tools::UTF8ToWideChar(row[RECORD_POINT_LIST_UNIT], unit);
		
		_tcscpy_s(entry.shortname, POINT_LIST_BUFFERSIZE,  shortname.c_str());
		_tcscpy_s(entry.longname, POINT_LIST_BUFFERSIZE, longname.c_str());
		_tcscpy_s(entry.remark, POINT_LIST_BUFFERSIZE, remark.c_str());
		_tcscpy_s(entry.unit, POINT_LIST_BUFFERSIZE, unit.c_str());
		
		pointlist_out.push_back(entry);
	}
	
	return true;
}

bool DBAccessContainer::ReadPowerTotal( vector<RecordPointValue>& pointlist_out,
                                            const wstring& tablename, 
                                            UINT pointIndex,
                                            const COleDateTime& oleTimeStart, COleDateTime& oleTimeEnd,
                                            bool ifOrder)
{
    Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);
    pointlist_out.clear();

    //const UINT idxNum = (UINT)vecPointIndex.size();
    char charIdx[11];
    _itoa_s(pointIndex, charIdx, 10); //radix

    wstring wstrTime; //2011-05-25 16:20:32
    Project::Tools::OleTime_To_String(oleTimeStart, wstrTime);
    const string strTimeStart = Project::Tools::WideCharToAnsi(wstrTime.c_str() );
    Project::Tools::OleTime_To_String(oleTimeEnd, wstrTime);
    const string strTimeEnd = Project::Tools::WideCharToAnsi(wstrTime.c_str() );

    const string tablename_ansi = Project::Tools::WideCharToAnsi(tablename.c_str());

    const string strDesc = ifOrder?"":"DESC ";

    ostringstream sqlstream;
    sqlstream << "select * from " << tablename_ansi << " "
        //<< "where PointIndex = 1 or pointindex = 6 or  pointindex = 13 or pointindex = 20 ) "
        << "where ( PointIndex = "<< charIdx << " ) "
        << "and recordtime >= '" << strTimeStart <<"' "
        << "and recordtime <= '" << strTimeEnd <<"' " 
        <<"order by recordtime " << strDesc
        //<<"limit "<< 1<<" "
        ;

    MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
    if (result == NULL){
        return false;
    }
    Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

    int length = (int)GetRowNum(result);
    if (0 == length){
        return true;
    }
    pointlist_out.reserve(length);

    MYSQL_ROW  row = NULL;
    RecordPointValue entry;

    while(row = FetchRow(result) )
    {              
        entry.mPointIndex = ATOI(row[RecordPointValue::POINT_LIST_INDEX] );

        Project::Tools::UTF8ToWideChar(row[RecordPointValue::POINT_LIST_TIME], entry.mStrTime);
        Project::Tools::UTF8ToWideChar(row[RecordPointValue::POINT_LIST_VALUE], entry.mStrValue);

        pointlist_out.push_back(entry);
    }

    return true;
}

bool DBAccessContainer::ReadRunningTimeTotal( vector<RecordPointValue>& pointlist_out,
	const wstring& tablename, 
	UINT pointIndex,
	const COleDateTime& oleTimeStart, COleDateTime& oleTimeEnd,
	bool ifOrder)
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);
	pointlist_out.clear();

	char charIdx[11];
	_itoa_s(pointIndex, charIdx, 10); //radix

	wstring wstrTime;
	Project::Tools::OleTime_To_String(oleTimeStart, wstrTime);
	const string strTimeStart = Project::Tools::WideCharToAnsi(wstrTime.c_str() );
	Project::Tools::OleTime_To_String(oleTimeEnd, wstrTime);
	const string strTimeEnd = Project::Tools::WideCharToAnsi(wstrTime.c_str() );

	const string tablename_ansi = Project::Tools::WideCharToAnsi(tablename.c_str());

	const string strDesc = ifOrder?"":"DESC ";

	ostringstream sqlstream;
	sqlstream << "select * from " << tablename_ansi << " "
		<< "where ( PointIndex = "<< charIdx << " ) "
		<< "and recordtime >= '" << strTimeStart <<"' "
		<< "and recordtime <= '" << strTimeEnd <<"' " 
		<<"order by recordtime " << strDesc
		<<"limit "<< 1<<" "
		;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = (int)GetRowNum(result);
	if (0 == length){
		return true;
	}
	pointlist_out.reserve(length);

	MYSQL_ROW  row = NULL;
	RecordPointValue entry;

	while(row = FetchRow(result) )
	{              
		entry.mPointIndex = ATOI(row[RecordPointValue::POINT_LIST_INDEX] );

		Project::Tools::UTF8ToWideChar(row[RecordPointValue::POINT_LIST_TIME], entry.mStrTime);
		Project::Tools::UTF8ToWideChar(row[RecordPointValue::POINT_LIST_VALUE], entry.mStrValue);

		pointlist_out.push_back(entry);
	}

	return true;
}

bool DBAccessContainer::ReadMonthReportRecord(vector<MonthReportParam> &reportParam,int nYear, int nMonth)
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

	if (nYear < 0|| nMonth < 0)
	{
		return false;
	}
	ostringstream sqlstream;

	CString strTmp;
	strTmp.Format(_T("%04d%02d"),nYear,nMonth);

	sqlstream << "select * from " << MONTHREPORT_TABLE
		<<"where RecordYearMonth = "
		<< Project::Tools::WideCharToAnsi(strTmp);
	

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}

	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);
	int length = (int)GetRowNum(result);
	if (0 == length){
		return true;
	}

	MYSQL_ROW  row = NULL;
	reportParam.clear();
	MonthReportParam gRecord;

	while(row = FetchRow(result) )
	{
		gRecord.strReportYM = row[0];
		gRecord.strEqmName  = row[1];
		gRecord.dPowerTotal =  ATOI(row[2]);
		gRecord.dTimeTotal  =  ATOI(row[3]);

		reportParam.push_back(gRecord);
	}

	return true;
}

bool DBAccessContainer::ClearRecordPointList( const wstring& tablename )
{
	ostringstream sqlstream;
	string tablename_ansi = Project::Tools::WideCharToAnsi(tablename.c_str());

	sqlstream << "delete from " << tablename_ansi;
	string sql = sqlstream.str();
	return Execute(sql);
}

bool DBAccessContainer::DeleteRecordPointList(const vector<int> &pointlist,const wstring& tablename)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);
	bool bFlg = true;
	ostringstream sqlstream;
	string tablename_ansi = Project::Tools::WideCharToAnsi(tablename.c_str());

	string sql;
	for (int i=0;i<pointlist.size();i++)
	{
		sqlstream << "delete from " << GetDBName()<< "." << tablename_ansi << " where "<<tablename_ansi<<".Index = '"<<pointlist[i]<<"'";
		sql = sqlstream.str();
		bFlg = bFlg&&Execute(sql);
		sqlstream.str("");
	}

	return bFlg;
}

bool DBAccessContainer::UpdateRecordPointList(PointlistParam &pParam,const wstring& tablename)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);
	bool bFlg = false;
	SetConnectionCharacterSet();

	ostringstream sqlstream;
	string tablename_ansi = Project::Tools::WideCharToUtf8(tablename.c_str());

	string longname;
	Project::Tools::WideCharToUtf8(pParam.strName,longname);
	string remark;
	Project::Tools::WideCharToUtf8(pParam.strVar,remark);
	string index;
	Project::Tools::WideCharToUtf8(pParam.strPoint,index);

	string sql;
	sqlstream << "update " << GetDBName()<< "." << tablename_ansi 
		<< " set PLCLongName = '"<<longname <<"' , Remark = '"<<remark<<"'"
		<<" where "<<tablename_ansi<<".Index ="<<index;
	sql = sqlstream.str();

	bFlg = Execute(sql);
	return bFlg;
}

bool DBAccessContainer::AddDeviceOnOffPointList( const vector<Record_Point_List_Entry>& pointlist )
{
	return AddRecordPointList(pointlist, DEVICE_ONOFF_POINT_LIST_TABLE);
}

bool DBAccessContainer::ReadDeviceOnOffPointList(  vector<Record_Point_List_Entry>& pointlist )
{
	return ReadRecordPointList(pointlist, DEVICE_ONOFF_POINT_LIST_TABLE);
}

bool DBAccessContainer::ClearDeviceOnOffPointList()
{
	return ClearRecordPointList(DEVICE_ONOFF_POINT_LIST_TABLE);
}

bool DBAccessContainer::AddDeviceFreqPointList( const vector<Record_Point_List_Entry>& pointlist)
{
	return AddRecordPointList(pointlist, DEVICE_FREQ_POINT_LIST_TABLE);
}

bool DBAccessContainer::ClearDeviceFreqPointList()
{
	return ClearRecordPointList(DEVICE_FREQ_POINT_LIST_TABLE);
}


bool DBAccessContainer::ReadDeviceFreqPointList(  vector<Record_Point_List_Entry>& pointlist )
{
	return ReadRecordPointList(pointlist, DEVICE_FREQ_POINT_LIST_TABLE);
}

bool DBAccessContainer::AddPowerPointList( const vector<Record_Point_List_Entry>& pointlist )
{
	return AddRecordPointList(pointlist, POWER_POINT_LIST_TABLE);
}

bool DBAccessContainer::ReadPowerPointList( vector<Record_Point_List_Entry>& pointlist)
{
	return ReadRecordPointList(pointlist,POWER_POINT_LIST_TABLE);
}

bool DBAccessContainer::ClearPowerPointList()
{
	return ClearRecordPointList(POWER_POINT_LIST_TABLE);
}

bool DBAccessContainer::AddFlowPointList( const vector<Record_Point_List_Entry>& pointlist)
{
	return AddRecordPointList(pointlist, FLOW_POINT_LIST_TABLE);
}

bool DBAccessContainer::ReadFlowPointList( vector<Record_Point_List_Entry>& pointlist)
{
	return ReadRecordPointList(pointlist,FLOW_POINT_LIST_TABLE);
}

bool DBAccessContainer::ClearFlowPointList()
{
	return	ClearRecordPointList(FLOW_POINT_LIST_TABLE);
}

bool DBAccessContainer::AddTemperaturePointList( const vector<Record_Point_List_Entry>& pointlist)
{
	return AddRecordPointList(pointlist, TEMPERATURE_POINT_LIST_TABLE);
}

bool DBAccessContainer::ReadTemperaturePointList( vector<Record_Point_List_Entry>& pointlist)
{
	return ReadRecordPointList(pointlist,TEMPERATURE_POINT_LIST_TABLE);
}

bool DBAccessContainer::ClearTemperaturePointList()
{
	return ClearRecordPointList(TEMPERATURE_POINT_LIST_TABLE);
}

bool DBAccessContainer::AddPressureDifferencePointList( const vector<Record_Point_List_Entry>& pointlist)
{
	return AddRecordPointList(pointlist, PRESSUREDIFFERENCE_POINT_LIST_TABLE);
}

bool DBAccessContainer::ReadPressureDifferencePointList( vector<Record_Point_List_Entry>& pointlist)
{
	return ReadRecordPointList(pointlist, PRESSUREDIFFERENCE_POINT_LIST_TABLE);
}

bool DBAccessContainer::ClearPressureDifferencePointList()
{
	return ClearRecordPointList(PRESSUREDIFFERENCE_POINT_LIST_TABLE);
}

bool DBAccessContainer::AddPowerConsumptionPointList(const vector<Record_Point_List_Entry>& pointlist)
{
	return AddRecordPointList(pointlist, POWERCONSUMTION_POINT_LIST_TABLE);
}

bool DBAccessContainer::ReadPowerConsumptionPointList( vector<Record_Point_List_Entry>& pointlist)
{
	return ReadRecordPointList(pointlist, POWERCONSUMTION_POINT_LIST_TABLE);
}

bool DBAccessContainer::ClearPowerConsumptionPointList()
{
	return ClearRecordPointList(POWERCONSUMTION_POINT_LIST_TABLE);
}

bool DBAccessContainer::AddRunningTimePointList(const vector<Record_Point_List_Entry>& pointlist)
{
	return AddRecordPointList(pointlist, EQM_RUNNINGTIME_POINT_LIST_TABLE);
}

bool DBAccessContainer::ReadRunningTimePointList( vector<Record_Point_List_Entry>& pointlist)
{
	return ReadRecordPointList(pointlist, EQM_RUNNINGTIME_POINT_LIST_TABLE);
}

bool DBAccessContainer::ClearRunningTimePointList()
{
	return ClearRecordPointList(EQM_RUNNINGTIME_POINT_LIST_TABLE);
}



int DBAccessContainer::ReadSystemModeFromDB()
{
	Scoped_Lock<Mutex>  dblock(m_recordmutex);
	//vector<wstring>	vec_result;
	int nSystemmode = 0;
	string sql = "select * from systemmode";

	//string sql;
	//ostringstream sqlstream;

	//string sysmode = "systemmode";
	//sqlstream << "select * from " << GetDBName()<< "." << sysmode;
	//sql = sqlstream.str();

	MYSQL_RES* result = QueryAndReturnResult(sql.c_str());
	if (result == NULL)
	{
		LogDBError(sql);
	}
	else
	{
		MYSQL_ROW row = NULL;
		if(row = FetchRow(result))
		{
			//wstring wstrSystemmode = Project::Tools::AnsiToWideChar(row[1]);
			//if (!wstrSystemmode.empty())
			//{
				//vec_result.push_back(shortname);
				nSystemmode = atoi(row[0]);
			//}
		}

		FreeReturnResult(result);
	}

	return  nSystemmode;
}

bool DBAccessContainer::SetSystemModeToDB(int nSysmode)
{
	Scoped_Lock<Mutex>  dblock(m_recordmutex);
	//vector<wstring>	vec_result;
	//int nSystemmode;
	//string sql = "select * from systemmode";

	char szSqlStatement[256] = {0};
	//char szDbTable[256] = {0};
	//char szUsername[256] = {0};
	//wsprintfA(szUsername, "%S", _tstrUserName.c_str());
	sprintf(szSqlStatement, 
		"update systemmode set mode = %d", nSysmode);

	//return m_dbSession.Execute(szSqlStatement);
	const bool ifok =  Execute(szSqlStatement);
	return ifok;
}

bool DBAccessContainer::ReadWarningRecord( WARNING_LIST& warninglist, const COleDateTime& timebegin, const COleDateTime& timeend )
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);
        
    SetConnectionCharacterSet();
	ostringstream sqlstream;
	wstring wstr_begin, wstr_end;
	Project::Tools::OleTime_To_String(timebegin, wstr_begin);
	Project::Tools::OleTime_To_String(timeend, wstr_end);

	string str_begin = Project::Tools::WideCharToAnsi(wstr_begin.c_str());
	string str_end = Project::Tools::WideCharToAnsi(wstr_end.c_str());

	sqlstream << "select recordtime, warninginfo from warning_record "
		<< " where recordtime > '"
		<< str_begin 
		<< "' and recordtime < '"
		<< str_end 
		<< "'";

	string sql = sqlstream.str();

	MYSQL_RES* result = QueryAndReturnResult(sql.c_str());
	if (NULL == result)
	{
		return false;
	}

	CWarningRecord record;
	MYSQL_ROW row = NULL;
	while(row = FetchRow(result))
	{
		if (row[0] != NULL && row[1] != NULL ) {
			record.m_strTime = Project::Tools::AnsiToWideChar(row[0]);
			record.m_warninginfo = Project::Tools::UTF8ToWideChar(row[1]);
			warninglist.push_back(record);
		}
	}

	FreeReturnResult(result);

	return true;
}               

bool DBAccessContainer::InsertWarningRecord( int warningindex)
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

    SetConnectionCharacterSet();

	ostringstream sqlstream;
	SYSTEMTIME st;
	GetLocalTime(&st);
	string recordtime = SystemTimeToString(st);
	sqlstream << "insert into warning_record values('" << recordtime << "'," << warningindex << ")";

	bool bresult =  Execute(sqlstream.str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

//////////////////////////////////////////////////////////////////////////
//bird add
//排热量
bool	DBAccessContainer::AddHeatLoad(const CHeatLoadData& entry, eHeatLoadType etype)
{
    Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);
    
    string strTime;
    wstring wstr;
    Project::Tools::OleTime_To_String(entry.mRecordTime, wstr);
    strTime = UtilString::ConvertWideCharToMultiByte(wstr);

    string strTable = (etype == HEAT_LOAD_YEAR)?HEATLOAD_YEAR_TABLE:HEATLOAD_MONTH_TABLE ;

    ostringstream sqlstream;
    sqlstream << "insert into "
        << GetDBName()
        <<"."  << strTable 
        << " values("
        << "'" << strTime << "'"  //time
        << ", " << entry.mHeatingLoad
        << ");";    

    string sql_temp = sqlstream.str();
    if (sql_temp.empty()){
        return false;
    }

    const bool ifok =  Execute(sql_temp);
    return ifok;
}

bool	DBAccessContainer::ReadHeatLoad(vector<CHeatLoadData>& pointlist, eHeatLoadType etype)
{
    Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);

    string strTime;
    wstring wstr;

    string strTable = (etype == HEAT_LOAD_YEAR)?HEATLOAD_YEAR_TABLE:HEATLOAD_MONTH_TABLE ;

    ostringstream sqlstream;
    sqlstream << "select * from "
        << GetDBName()
        <<"." << strTable 
        <<";";    

    string sql_temp = sqlstream.str();
    if (sql_temp.empty()){
        return false;
    }

    MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
    if (result == NULL){
        return false;
    }
    Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

    const int rowNum = (int)GetRowNum(result);
    if (rowNum < 1){
        return true;
    }

    MYSQL_ROW  row = NULL;
    pointlist.clear();
    pointlist.reserve(rowNum);
    CHeatLoadData entry;
    string str;
    //wstring wstr;
    bool ifParse = false;
    while(row = FetchRow(result) )
    {
        str = row[HLY_TIME];
        wstr = UtilString::ConvertMultiByteToWideChar(str);
        ifParse = entry.mRecordTime.ParseDateTime(wstr.c_str() ) ;
        entry.mHeatingLoad = ATOF(row[HLY_HEATING_LOAD] );      

        pointlist.push_back(entry);
    }
    return true;
}

bool    DBAccessContainer::GetHeatLoadSum(double& dSum, eHeatLoadType etype)
{
    Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);

    string strTime;
    wstring wstr;

    string strTable = (etype == HEAT_LOAD_YEAR)?HEATLOAD_YEAR_TABLE:HEATLOAD_MONTH_TABLE ;

    ostringstream sqlstream;
    sqlstream << "select sum(heatingload) from "
        << GetDBName()
        <<"." << strTable 
        <<";";    

    string sql_temp = sqlstream.str();
    if (sql_temp.empty()){
        return true;
    }

    MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
    if (result == NULL){
        return false;
    }
    Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

    const int rowNum = (int)GetRowNum(result);
    if (rowNum < 1){
        return false;
    }

    MYSQL_ROW  row = NULL;
    if(row = FetchRow(result) )
    {
        dSum = ATOF(row[0] );       
    }
    else{
        return false;
    }
    return true;
}

bool	DBAccessContainer::ClearHeatLoad(eHeatLoadType etype)
{
    string str = (etype == HEAT_LOAD_YEAR)?HEATLOAD_YEAR_TABLE:HEATLOAD_MONTH_TABLE ;
    const wstring wstr = UtilString::ConvertMultiByteToWideChar(str);
    const bool ifok = ClearRecordPointList(wstr);
    return ifok;
}

bool    DBAccessContainer::DeleteHeatLoadMonthBeforeTime(const COleDateTime& timeStart )
{    
    Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);

    string strTime;
    wstring wstr;

    Project::Tools::OleTime_To_String(timeStart, wstr);
    const string strTimeStart = UtilString::ConvertWideCharToMultiByte(wstr);

    const string strTable = HEATLOAD_MONTH_TABLE;

    ostringstream sqlstream;
    sqlstream << "delete from "
        << GetDBName()
        <<"."  << strTable 
        << " where recordTime < "
        << "'" << strTime << "'"  //time        
        << ";";    

    string sql_temp = sqlstream.str();
    if (sql_temp.empty()){
        return false;
    }

    const bool ifok =  Execute(sql_temp);
    return ifok;
}

bool  DBAccessContainer::GetHeatLoadAverage(const COleDateTime& timeStart, const COleDateTime& timeEnd, double& davg)
{
    Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);

    string strTime;
    wstring wstr;

    Project::Tools::OleTime_To_String(timeStart, wstr);
    const string strTimeStart = UtilString::ConvertWideCharToMultiByte(wstr);
    Project::Tools::OleTime_To_String(timeEnd, wstr);
    const string strTimeEnd = UtilString::ConvertWideCharToMultiByte(wstr);

    const string strTable = HEATLOAD_MONTH_TABLE;

    ostringstream sqlstream;
    sqlstream << "select avg(h.heatingload) from "
        << GetDBName() <<"."  << strTable << " h"
        << " where h.recordtime >="   //>=
        << "'" << strTimeStart << "'"  //time     
        << " and h.recordtime<"   //< 
        << "'" << strTimeEnd << "'"
        << ";";    

    string sql_temp = sqlstream.str();
    if (sql_temp.empty()){
        return false;
    }

    MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
    if (result == NULL){
        return true;
    }
    Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

    const int rowNum = (int)GetRowNum(result);
    if (rowNum < 1){
        return true;
    }

    MYSQL_ROW  row = NULL;
    if(row = FetchRow(result) )
    {
        davg = ATOF(row[0] );       
    }
    else{
        return false;
    }
    return true;
}

bool   DBAccessContainer::AddHeadLoadAverage(const COleDateTime& timeMonth, double davg)
{
    Project::Tools::Scoped_Lock<Mutex>  scopelock(m_recordmutex);

    wstring wstr;

    Project::Tools::OleTime_To_String(timeMonth, wstr);
    const string strTime = UtilString::ConvertWideCharToMultiByte(wstr);

    const string strTable = HEATLOAD_MONTH_TABLE;

    ostringstream sqlstream;
    sqlstream << "INSERT INTO "
        << GetDBName()
        <<"."  << strTable 
        << " VALUES ("
        << "'" << strTime << "'"  //time  
        << "," << davg  //double
        << ")"
        << ";";    

    string sql_temp = sqlstream.str();
    if (sql_temp.empty()){
        return false;
    }

    const bool ifok =  Execute(sql_temp);
    return ifok;
}

bool DBAccessContainer::InsertCopRecord( const SYSTEMTIME& time, double value )
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

	ostringstream sqlstream;
	string recordtime = SystemTimeToString(time);

	sqlstream << "insert into cop_record values('" << recordtime << "', '" 

		<< value << "')";

	bool bresult =  Execute(sqlstream.str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

bool DBAccessContainer::InsertLoadRecord( const SYSTEMTIME& time, double value )
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

	ostringstream sqlstream;
	string recordtime = SystemTimeToString(time);

	sqlstream << "insert into load_record values('" << recordtime << "', '" 

		<< value << "')";

	bool bresult =  Execute(sqlstream.str());
	if (!bresult)
	{
		LogDBError(sqlstream.str());
	}

	return bresult;
}

void DBAccessContainer::ReadCopRecord( vector<CopRecord>& result, const SYSTEMTIME& start, const SYSTEMTIME& end )
{
	Scoped_Lock<Mutex>  dblock(m_recordmutex);
	char sql[256];
	memset(sql, 0x00, sizeof(sql));

	string from = SystemTimeToString(start);
	string to = SystemTimeToString(end);

	sprintf_s(sql, sizeof(sql), "select * from cop_record where RecordTime >= '%s' and RecordTime <= '%s' ", from.c_str(), to.c_str());

	MYSQL_RES* result_sql = QueryAndReturnResult(sql);
	if (result_sql == NULL)
	{
		LogDBError(sql);
	}
	else
	{
		MYSQL_ROW row = NULL;
		while(row = FetchRow(result_sql))
		{
			if (row[0] != NULL && row[1] != NULL ) {
					CopRecord record;
					record.copvalue =(double) ATOI(row[1]);
					wstring str_time = Project::Tools::UTF8ToWideChar(row[0]);
					String_To_SysTime(str_time,record.recordtime);
					result.push_back(record);

			}
		}

		FreeReturnResult(result_sql);
	}

}

void DBAccessContainer::ReadLoadRecord( vector<LoadRecord>& result, const SYSTEMTIME& start, const SYSTEMTIME& end )
{
	Scoped_Lock<Mutex>  dblock(m_recordmutex);
	char sql[256];
	memset(sql, 0x00, sizeof(sql));

	string from = SystemTimeToString(start);
	string to = SystemTimeToString(end);

	sprintf_s(sql, sizeof(sql), "select * from load_record where RecordTime >= '%s' and RecordTime <= '%s' ", from.c_str(), to.c_str());

	MYSQL_RES* result_sql = QueryAndReturnResult(sql);
	if (result_sql == NULL)
	{
		LogDBError(sql);
	}
	else
	{
		MYSQL_ROW row = NULL;
		while(row = FetchRow(result_sql))
		{
			if (row[0] != NULL && row[1] != NULL ) {
				LoadRecord record;
				record.loadvalue =(double) ATOI(row[1]);
				wstring str_time = Project::Tools::UTF8ToWideChar(row[0]);
				String_To_SysTime(str_time,record.recordtime);
				result.push_back(record);
			}
		}

		FreeReturnResult(result_sql);
	}
}

bool DBAccessContainer::InsertChillerPlantRoomInfo( const RoomEnergy& energyinfo, const SYSTEMTIME& currenttime )
{

	Scoped_Lock<Mutex>  dblock(m_recordmutex);
	ostringstream sqlstream;
	bool bResult = false;

	// Get the room energy values.
	// Get the current date time.
	string recordtime = SystemTimeToString(currenttime);

	// Write the sql statements.
	sqlstream << "insert into "
		<< GetDBName()
		<< ".chillerplantroominfo values";

	sqlstream << "('" << recordtime << "',"
		<< energyinfo.dChillerGroupPower << ","
		<< energyinfo.dPriPumpGroupPower << ","
		<< energyinfo.dSecPumpGroupPower << ","
		<< energyinfo.dCoolPumpGroupPower << ","
		<< energyinfo.dColTowerGroupPower << ","
		<< energyinfo.dChillerGroupTotalPower << ","
		<< energyinfo.dPriPumpGroupTotalPower << ","
		<< energyinfo.dSecPumpGroupTotalPower << ","
		<< energyinfo.dCoolPumpGroupTotalPower << ","
		<< energyinfo.dColTowerGroupTotalPower << ","
		<< energyinfo.dChillerPlantRoomLoad << ","
		<< energyinfo.dOutDoorTDry
		<< "," << energyinfo.dOutDoorTWet 
		<< "," << energyinfo.dAllPower  //bird add all power
		<< "," << energyinfo.dAllTotalPower //all total power
		<< "," << energyinfo.dCop //cop
		<< ")";

	string sql = sqlstream.str();
	bResult = Execute(sql);
	if (!bResult)
	{
		LogDBError(sql);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
bool DBAccessContainer::ReadRecordList(vector<RecordPointValue>& vecRecord, const string& tableName, int  pointIdx,  const COleDateTime& oleTimeStart, const COleDateTime& oleTimeEnd )
{
	vecRecord.clear();

	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

	//const UINT idxNum = (UINT)vecPointIndex.size();
	char charIdx[11];
	_itoa_s(pointIdx, charIdx, 10); //radix

	wstring wstrTime; //2011-05-25 16:20:32
	Project::Tools::OleTime_To_String(oleTimeStart, wstrTime);
	const string strTimeStart = Project::Tools::WideCharToAnsi(wstrTime.c_str() );
	Project::Tools::OleTime_To_String(oleTimeEnd, wstrTime);
	const string strTimeEnd = Project::Tools::WideCharToAnsi(wstrTime.c_str() );

	const string tablename_ansi = tableName; //Project::Tools::WideCharToAnsi(tableName.c_str() );

	ostringstream sqlstream;
	sqlstream << "select * from " << tablename_ansi << " "
		//<< "where PointIndex = 1 or pointindex = 6 or  pointindex = 13 or pointindex = 20 ) "
		<< "where ( PointIndex = "<< charIdx << " ) "
		<< "and recordtime >= '" << strTimeStart <<"' "
		<< "and recordtime <= '" << strTimeEnd <<"' " 
		//<<"order by recordtime " << strDesc
		//<<"limit "<< 1<<" "
		;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = (int)GetRowNum(result);
	if (0 == length){
		return true;
	}
	vecRecord.reserve(length);

	MYSQL_ROW  row = NULL;
	RecordPointValue entry;
	bool ifParseOk = false;

	while(row = FetchRow(result) )
	{              
		entry.mPointIndex = ATOI(row[RecordPointValue::POINT_LIST_INDEX] );

		Project::Tools::UTF8ToWideChar(row[RecordPointValue::POINT_LIST_TIME], entry.mStrTime);
		Project::Tools::UTF8ToWideChar(row[RecordPointValue::POINT_LIST_VALUE], entry.mStrValue);

		entry.mValueDouble = atof(row[RecordPointValue::POINT_LIST_VALUE] );
		ifParseOk = entry.mOleTime.ParseDateTime(entry.mStrTime.c_str(), 0, LANG_USER_DEFAULT);
		
		vecRecord.push_back(entry);
	}

	return true;
}
bool	DBAccessContainer::ReadRecordMaxMIn(double& dMax, double& dMin, const string& tableName, int  pointIdx,  const COleDateTime& oleTimeStart, const COleDateTime& oleTimeEnd )
{
	dMax = 0, dMin = 0;

	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

	//const UINT idxNum = (UINT)vecPointIndex.size();
	char charIdx[11];
	_itoa_s(pointIdx, charIdx, 10); //radix

	wstring wstrTime; //2011-05-25 16:20:32
	Project::Tools::OleTime_To_String(oleTimeStart, wstrTime);
	const string strTimeStart = Project::Tools::WideCharToAnsi(wstrTime.c_str() );
	Project::Tools::OleTime_To_String(oleTimeEnd, wstrTime);
	const string strTimeEnd = Project::Tools::WideCharToAnsi(wstrTime.c_str() );

	const string tablename_ansi = tableName; //Project::Tools::WideCharToAnsi(tableName.c_str() );

	ostringstream sqlstream;
	sqlstream << "select max(t.value),min(t.value) from " << tablename_ansi << " t "
		<< "where ( PointIndex = "<< charIdx << " ) "
		<< "and recordtime >= '" << strTimeStart <<"' "
		<< "and recordtime <= '" << strTimeEnd <<"' " 
		//<<"order by recordtime " << strDesc
		//<<"limit "<< 1<<" "
		;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = (int)GetRowNum(result);
	if (0 == length){
		return true;
	}

	MYSQL_ROW  row = NULL;
	//RecordPointValue entry;
	bool ifParseOk = false;

	if(row = FetchRow(result) )
	{              
		dMax = ATOF(row[0] );
		dMin = ATOF(row[1] );
	}

	return true;
}

//取十分钟内的最近一条记录。
bool DBAccessContainer::ReadControlParamRecord( gControlParamRecord& record, 
												const COleDateTime& timeStart,
												const COleDateTime& timeStop )
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

	CString time_begin = timeStart.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString time_end = timeStop.Format(_T("%Y-%m-%d %H:%M:%S"));

	string timebegin_ansi = Project::Tools::WideCharToAnsi(time_begin.GetString());
	string timeend_ansi = Project::Tools::WideCharToAnsi(time_end.GetString());

	ostringstream sqlstream;
	sqlstream << "select * from " << "controlparam_record " 
		<< "where recordtime >= '" << timebegin_ansi <<"' "
		<< "and recordtime <= '" << timeend_ansi <<"' "
		<< "order by recordtime desc";

	string sql = sqlstream.str();
	MYSQL_RES* queryresult = QueryAndReturnResult(sql.c_str());
	if (queryresult == NULL){
		LogDBError(sql);
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(queryresult);
	MYSQL_ROW row = NULL;
	my_ulonglong rownum = GetRowNum(queryresult);
	if (rownum == 0){
		return false;
	}else if (rownum >= 1)
	{
		row = FetchRow(queryresult);
		if (row == NULL){
			return false;
		}else
		{
			Project::Tools::String_To_SysTime(row[CTRLPM_TIME], record.tmTime);
			record.dT = ATOF(row[CTRLPM_T]);
			record.dFai = ATOF(row[CTRLPM_FAI]);
			record.dTwb = ATOF(row[CTRLPM_TWB]);
			strcpy_s(record.LEWT_Setpoint, row[CTRLPM_LEWT_Setpoint]);
			record.dLCWT_Setpoint = ATOF(row[CTRLPM_LCWT_Setpoint]);
			strcpy_s(record.ChillerCombination, row[CTRLPM_ChillerCombination]);
			strcpy_s(record.CTCombination, row[CTRLPM_CoolingTowerCombination]);
			strcpy_s(record.ChilledPumpCombination, row[CTRLPM_ChilledPumpCombination]);
			strcpy_s(record.CoolingPumpCombination, row[CTRLPM_CoolingPumpCombination]);
			record.dLoad_Actual = ATOF(row[CTRLPM_Load_Actual]);
			record.dLoad_UseCalc = ATOF(row[CTRLPM_Load_Calc_Aver]);
			record.dChill_Flow = ATOF(row[CTRLPM_Chill_Flow]);
			record.dCool_Flow = ATOF(row[CTRLPM_Cool_Flow]);
			record.dDTChill = ATOF(row[CTRLPM_DTChill]);
			record.dDTCool = ATOF(row[CTRLPM_DTChill]);
			record.dChiller_K = ATOF(row[CTRLPM_Chiller_K]);
			record.dRoomCOP = ATOF(row[CTRLPM_RoomCOP]);
			record.dCHGroup_Power = ATOF(row[CTRLPM_CHGroup_Power]);
			record.dCTGroup_Power = ATOF(row[CTRLPM_CTGroup_Power]);
			record.dPriCHWPumpGroup_Power = ATOF(row[CTRLPM_PriCHWPumpGroup_Power]);
			record.dSecCHWPumpGroup_Power = ATOF(row[CTRLPM_SecCHWPumpGroup_Power]);
			record.dCWPumpGroup_Power = ATOF(row[CTRLPM_CWPumpGroup_Power]);
		}
	}

	return true;
}

bool DBAccessContainer::ReadControlParamRecord( string& strrecord, const COleDateTime& timeStart,const COleDateTime& timeStop )
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

	CString time_begin = timeStart.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString time_end = timeStop.Format(_T("%Y-%m-%d %H:%M:%S"));

	string timebegin_ansi = Project::Tools::WideCharToAnsi(time_begin.GetString());
	string timeend_ansi = Project::Tools::WideCharToAnsi(time_end.GetString());

	ostringstream sqlstream;
	sqlstream << "select * from " << "controlparam_record " 
		<< "where recordtime >= '" << timebegin_ansi <<"' "
		<< "and recordtime <= '" << timeend_ansi <<"' "
		<< "order by recordtime desc";

	string sql = sqlstream.str();
	MYSQL_RES* queryresult = QueryAndReturnResult(sql.c_str());
	if (queryresult == NULL){
		LogDBError(sql);
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(queryresult);
	MYSQL_ROW row = NULL;
	my_ulonglong rownum = GetRowNum(queryresult);
	if (rownum == 0){
		return false;
	}else if (rownum >= 1)
	{
		row = FetchRow(queryresult);
		if (row == NULL){
			return false;
		}else
		{
			strrecord += "('";
			strrecord += row[CTRLPM_TIME];
			strrecord += "',";
			strrecord += row[CTRLPM_T];
			strrecord += ",";
			strrecord += row[CTRLPM_FAI];
			strrecord += ",";
			strrecord += row[CTRLPM_TWB];
			strrecord += ",'";
			strrecord += row[CTRLPM_LEWT_Setpoint];
			strrecord += "',";
			strrecord += row[CTRLPM_LCWT_Setpoint];
			strrecord += ",'";
			strrecord += row[CTRLPM_ChillerCombination];
			strrecord += "','";
			strrecord += row[CTRLPM_CoolingTowerCombination];
			strrecord += "','";
			strrecord += row[CTRLPM_ChilledPumpCombination];
			strrecord += "','";
			strrecord += row[CTRLPM_CoolingPumpCombination];
			strrecord += "',";
			strrecord += row[CTRLPM_Load_Actual];
			strrecord += ",";
			strrecord += row[CTRLPM_Load_Calc_Aver];
			strrecord += ",";
			strrecord += row[CTRLPM_Chill_Flow];
			strrecord += ",";
			strrecord += row[CTRLPM_Cool_Flow];
			strrecord += ",";
			strrecord += row[CTRLPM_DTChill];
			strrecord += ",";
			strrecord += row[CTRLPM_DTChill];
			strrecord += ",";
			strrecord += row[CTRLPM_Chiller_K];
			strrecord += ",";
			strrecord += row[CTRLPM_RoomCOP];
			strrecord += ",";
			strrecord += row[CTRLPM_CHGroup_Power];
			strrecord += ",";
			strrecord += row[CTRLPM_CTGroup_Power];
			strrecord += ",";
			strrecord += row[CTRLPM_PriCHWPumpGroup_Power];
			strrecord += ",";
			strrecord += row[CTRLPM_SecCHWPumpGroup_Power];
			strrecord += ",";
			strrecord += row[CTRLPM_CWPumpGroup_Power];
			strrecord += ");";
		}
	}

	return true;
}
