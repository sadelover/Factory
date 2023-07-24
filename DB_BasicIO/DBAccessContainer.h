
// -*- c++ -*-
// ***********************************************************
// Copyright (C) 2013 RNB
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Module:   
// Author:   david
// Date:     2010/7/13
// Revision: 1.0
//
// ***********************************************************
#ifndef DBACCESSCONTAINER_H
#define DBACCESSCONTAINER_H

#pragma once
#include <map>
#include <vector>
#include <utility>
#include <string>

#include "DBInterfaceImp.h"
#include "DatabaseSession.h"
#include "TableDef.h"
#include "../Tools/Log/UseLog.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "../Tools/NormalDefines.h"
#include "../Tools/Log/UseLog.h"

#include "./StructDefine/Define_Record_Point_List.h"
#include "./StructDefine/Define_HeatLoadYear.h"

#include "RunningRecordOperation.h" //bird add

using std::string;
using std::wstring;
using std::map;
using std::multimap;
using std::vector;
using std::pair;
using Project::Tools::Mutex;
using Project::Tools::Scoped_Lock;
;
#define TEMPLATEDATABASE_CREATE "uppc_template";

enum DEVICETYPE
{
	Type_UNKNOWNTYPE = 0x00	//未知
	, Type_CHILLER			//冷机
	, Type_PUMP				//水泵
	, Type_COOLINGTOWER		//冷却塔
	, Type_HX				//板交
	, Type_PIPE				//管道
	, Type_CORNER			//弯头
	, Type_TERMINAL			//终端
	, Type_VALVE			//阀门
	, Type_WATERNET			//管网
	, Type_Tank				//水箱
	, Type_WATERSOIL		//地暖
	, Type_POWMETER			//电表
};

typedef struct tag_Project_DB
{
	wstring		strprojectname;		//	项目名字,需要在界面上显示
	string		strdatabasename;	//	数据库名字
}Project_DB;

//工程相关数据的信息
typedef struct tag_StartUpInfo
{
	string		bpicture;			//	项目图片
	Project_DB  projectdb;		//	数据库名和项目名
}StartUpInfo;

typedef struct tag_UppcUserInfo
{
	wstring		username;
	int			readright;
	int			writeright;
	int			modelright;
	int			simulateright;
	wstring		lastlogintime;
}UppcUserInfo;

enum Room_Control_Type
{
	ROOM_NORMALCONTROL,
	ROOM_UPPCCONTROL,
};

enum RECORDTYPE
{
	RecordType_Chiller,
	RecordType_Pump,
	RecordType_CoolingTower,
	RecordType_ControlParam,
	RecordType_OpenChiller,
	RecordType_Warning,
	Recordtype_Operation,	
};


struct tagRoomEnergy
{
	double dChillerGroupPower;        // 冷机组总功率
	double dPriPumpGroupPower;        // 一次泵总功率
	double dSecPumpGroupPower;        // 二次泵总功率
	double dCoolPumpGroupPower;       // 冷却泵总功率
	double dColTowerGroupPower;       // 冷却塔总功率
	double dAllPower;          //chiller plant room 总功率 //bird add,

	double dChillerGroupTotalPower;   // 冷机组用电量
	double dPriPumpGroupTotalPower;   // 一次泵用电量
	double dSecPumpGroupTotalPower;   // 二次泵用电量
	double dCoolPumpGroupTotalPower;  // 冷却泵用电量
	double dColTowerGroupTotalPower;  // 冷却塔用电量
	double dAllTotalPower;              //chiller plant room 总用电量 

	double dChillerPlantRoomLoad;     // 机房负荷
	double dOutDoorTDry;              // 室外干球
	double dOutDoorTWet;              // 室外湿球

	double dCop;             
	tagRoomEnergy()
	{
		memset(this, 0, sizeof(*this));
	}
};
typedef struct tagRoomEnergy RoomEnergy;
typedef struct tagRoomEnergy *pRoomEnergy;


class DBDLL CopRecord
{
public:
	SYSTEMTIME recordtime;
	double	   copvalue;
};

class DBDLL LoadRecord
{
public:
	SYSTEMTIME recordtime;
	double	   loadvalue;
};


//数据库访问类
class DBDLL DBAccessContainer : public CDatabaseSeesion
{
private:
	DBAccessContainer();
public:
	~DBAccessContainer();
	static DBAccessContainer *GetInstance();
	static void DestroyInstance();

public:
	// ============= 读Lib表相关的接口 ==========================
	int			GenerateNewIDInLib(DEVICETYPE type);//获得设备的最大id
	MYSQL_RES*	ReadAllEquipmentInLib(DEVICETYPE type);//读取lib中某类型的所有设备信息
	MYSQL_RES*	ReadAllEquipmentIDInLib(DEVICETYPE type);//读取lib中某类型的所有设备id
	MYSQL_RES*	ReadOneEquipmentFromLib(DEVICETYPE type, int id);// 读取lib中的某一设备
	bool		DeleteAllEquipmentsInLib(DEVICETYPE type);//删除指定设备lib中的所有信息.	
	bool		AddEquipmentToLib(DEVICETYPE type, void* record);//向设备lib中加设备
	bool		DeleteEquipmentInLib(DEVICETYPE type, int id);//删除设备中的lib		

	//====room_contain_相关表的接口========================
	int         ReadMaxIDInRoom(DEVICETYPE type);
	MYSQL_RES*	ReadAllEquipmentsInRoom( DEVICETYPE type, int roomid);
	bool		DeleteAllEquipmentsInRoom( DEVICETYPE type, int roomid );	//删除
	bool		AddEquipmentToRoom(DEVICETYPE type, void* record);			//添加
//	bool		DeleteEquipmentFromRoom(DEVICETYPE type, int roomid , int equipmentid);	//删除指定机房的制定设备
	bool		ClearAllRooms();
	int			GetMaxIDInTable(string tablename, string fieldname);	
	
	//修改a可用和不可用
	bool		AlterEnableProperty(DEVICETYPE type, bool enable, int chillerid, int roomid);


	//==== 读Model相关表的接口     ====
	MYSQL_RES*	ReadALLModel(DEVICETYPE type);					//读所有信息
	MYSQL_RES*	ReadModel(DEVICETYPE type, int modelid);			// 读模型表
	bool		InsertIntoModelTable(DEVICETYPE type, void* record);	//写模型表	
	bool		DeleteFromModelTable(DEVICETYPE type, int modelid);	//删除模型表	
	MYSQL_RES*  ReadModelIDs(DEVICETYPE type);
	gCH_Model * ReadChillerModel( int nChillerModelID );
	gPump_Model * ReadPumpModel( int nPumpModelID );
	gCT_Model * ReadCTModel( int nCTMODELID );
	gWaterSoil_Model * ReadWSModel( int nWSMODELID );

	bool        UpdateModel(DEVICETYPE type, int modelid,string strModel_Param); //更新模型
	MYSQL_RES*  Read_Load_Define_Tempalte();                              //读取负荷定义模板
	// ==== Project以及chillerplantroom相关表的接口 ====
	MYSQL_RES*  GetProjectInfo();								// 读一个project的信息	
	bool		ReadProject( gProject * pProject);
	int         GetProjectID();									// 读取project表中的id	
	bool		AddNewProject(gProject* projectinfo);			// 添加一个project
	bool		DeleteProject(int projectid);					// 删除一个project	
	MYSQL_RES*	GetChillerPlantRoomIDList(int projectid);		// 读取project的机房id
	MYSQL_RES*	GetAllChillerPlantRoomInfo(int projectid);		// 读所有机房信息
	MYSQL_RES*	GetChillerPlantRoomInfoByID(int roomid);		// 读一个机房的信息
	bool		AddNewToChillerPlantRoom(gCHPlantRoom* room);	// 向chillerplantroom加记录
	bool		DeleteFromChillerPlantRoom(int roomid);			// 删除chillerplantroom的一个记录
	int			GetMaxIDInChillerPlantRoom();					// 取得chillerplantroom的最大id
	bool		CreateNewProject(gProject* projectinfo, string& newdbname, string copyfrom = TEMPLATEDATABASE_CREATE);		// 创建一个新项目,并创建一个数据库
	bool		ReadAllProjectsFromServer(vector<StartUpInfo*>& projectlist, const wstring& createuser);				//遍历所有数据库,读projectlist
	bool		ReadAllProjectsFromServer(vector<StartUpInfo*>& projectlist);											//遍历所有数据库,读projectlist
	bool		ReadAllProjectsFromServer(vector<Project_DB>& projectlist, const wstring& creator);
	bool		ClearChillerPlantRoom();

	// ==== Eff Design Data相关表的接口 ====
	bool ReadChillerEffDesignData(vector<gChiller_Eff_Design_Data*>& resultlist , int id);  //冷机
	bool ReadCoolingTowerEffDesignData(vector<gCT_Eff_Design_Data*>& resultlist , int id);  //冷却塔
	bool ReadPumpEffDesignData(vector<gPump_Eff_Design_Data*>& resultlist , int id);        //泵
	bool DeleteEffDesignData(DEVICETYPE type, int deviceid);								//删除
	bool InsertEffDesignData(DEVICETYPE type, void* data);									//插入	
	bool InsertChillerEffDesignData(const vector<gChiller_Eff_Design_Data*>& record);	
	bool InsertPumpEffDesignData(const vector<gPump_Eff_Design_Data*>& record);
	bool InsertCoolingTowerDesignData(const vector<gCT_Eff_Design_Data*>& record);

    // ==== ChillerPlantRoom Load Define表读写接口====
	MYSQL_RES*  ReadRoomLoadDefine(int roomid);   //read	
	bool InsertIntoRoomLoadDefine(const vector<gRoomLoad*>& record);  // write
	bool DeleteFromRoomLoadDefine(int roomid); // delete
	
	// ==== waternet param读写接口  ====
	MYSQL_RES*	ReadWaternetParam(int id);
	MYSQL_RES*  ReadWaternetParamIDs();
	bool		InsertIntoWaterParam(gWaternetParam* record);
	bool		DeleteWaternetParam(int id);
	bool		ClearWaternetParam();

	//Weather data 接口
	bool		GetProvinceList(vector<wstring>& provincelist);
	bool		GetCityList(vector< pair<long, wstring> >& citylist, wstring provincename);
	gWeatherDesignData*		GetWeatherDesignData(long stationid);
	bool		GetWeatherHourData( vector<gWeatherHourData* >& resultlist, long stationid);
    
	// 报警记录
	bool		InsertWarningRecord(int warningindex);	// 插入警报记录
	bool		InsertWarningRecord(SYSTEMTIME st, const wstring& warninginfo);	

	// 插入chiller的运行记录
	bool		InsertChillerRunRecord(const gCHRunRecord& record);    
	
	 //插入控制逻辑的运行记录
	bool		InsertControlParamRecord(const gControlParamRecord& record); 
	
	//流量，，，记录
	bool		InsertFlowRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	//功率
	bool		InsertPowerRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	//温度
	bool		InsertTemperatureRecord(const vector<DataSensor_Record_Entry>& entrylist);

	//压差
	bool		InsertPressureDifferenceRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	//用电量
	bool		InsertPowerConsumptionRecord(const vector<DataSensor_Record_Entry>& entrylist);

	//运行时间
	bool		InsertRunningTimeRecord(const vector<DataSensor_Record_Entry>& entrylist );

	//月度报告
	bool		InsertMonthReportRecord(const vector<MonthReportParam>& vctParam);

	//开关
	bool		InsertOnOffRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	//频率
	bool		InsertFreqRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	bool		InsertPointRecordHelper(string tablename,
									   const vector<DataSensor_Record_Entry>& entrylist);
	// 用户操作记录
	bool		InsertOperationRecord(const SYSTEMTIME& st, const wstring& remark,
									const wstring& wstrUser);

	//系统状态信息.
	bool		InsertSystemStatusRecord(const SYSTEMTIME& st, int status);
	
	//删除运行记录
	bool		DeleteRunRecord(SYSTEMTIME start, SYSTEMTIME end, int roomid, int deviceid, RECORDTYPE type);
	
	//读指定设备的某一时间段里的运行记录
	MYSQL_RES*	ReadChillerRunRecord(SYSTEMTIME start, SYSTEMTIME end, int roomid, int chillerid);
	MYSQL_RES*	ReadChillerRunRecord1(string from, string to);
//	MYSQL_RES*	ReadControlParamRunRecord(SYSTEMTIME start, SYSTEMTIME end);
	
	
	void ReadRecordDateRange(vector<string>& daterange, RECORDTYPE recordtype, 
							 int roomid = 0, int deviceid = 0);

	//UPPC_USER.users表读写接口
	bool	AddNewUser(gUppcSimulateUser* record);	//添加一个新user	
	bool	DeleteUser(wstring username);			//删除一个user
	bool	ReadAllUsers(vector<wstring>& userlist);	//读所有的user列表
	bool	ReadUserInfo(wstring username, UppcUserInfo* puserinfo);	// 读user的信息
	bool	ReadALLUserInfo(vector<UppcUserInfo*>& userlist);
	bool	ValidateUser(wstring username, wstring password); // 判断用户是否存在
	
	
	//点位列表
	bool	AddRecordPointList(const vector<Record_Point_List_Entry>& pointlist, 
							   const wstring& tablename);
	bool	ReadRecordPointList(vector<Record_Point_List_Entry>& pointlist_out, 
								const wstring& tablename);
	bool	ClearRecordPointList(const wstring& tablename);

	bool    DeleteRecordPointList(const vector<int>& pointlist,const wstring& tablename);

	bool    UpdateRecordPointList(PointlistParam &pParam,const wstring& tablename);

	//设备开关点位列表。
	bool	AddDeviceOnOffPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadDeviceOnOffPointList( vector<Record_Point_List_Entry>& pointlist);
	bool	ClearDeviceOnOffPointList();

	//设备频率点位列表	
	bool	AddDeviceFreqPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadDeviceFreqPointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearDeviceFreqPointList();	

	//功率点位列表
	bool	AddPowerPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadPowerPointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearPowerPointList();

	//流量点位列表
	bool	AddFlowPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadFlowPointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearFlowPointList();

	//温度点位列表
	bool	AddTemperaturePointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadTemperaturePointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearTemperaturePointList();

	//压差点位列表
	bool	AddPressureDifferencePointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadPressureDifferencePointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearPressureDifferencePointList();
	
	//用电量点位列表
	bool	AddPowerConsumptionPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadPowerConsumptionPointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearPowerConsumptionPointList();

	//运行时间点位列表
	bool	AddRunningTimePointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadRunningTimePointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearRunningTimePointList();
	
    //排热量
    bool	AddHeatLoad(const CHeatLoadData& point, eHeatLoadType etype);
    bool	ReadHeatLoad(vector<CHeatLoadData>& pointlist, eHeatLoadType etype);
    bool    GetHeatLoadSum(double& dSum, eHeatLoadType etype);
    bool	ClearHeatLoad(eHeatLoadType etype); 
    bool    DeleteHeatLoadMonthBeforeTime(const COleDateTime& timeStart);
    bool    GetHeatLoadAverage(const COleDateTime& timeStart, const COleDateTime& timeEnd, double& davg);
    bool    AddHeadLoadAverage(const COleDateTime& timeMonth, double davg);
	
    bool    ReadAllOperationRecord(VEC_OP_RECORD&  vecVecOperation, const COleDateTime& timeStart, const COleDateTime& timeStop);
	
	bool    ReadWarningRecord(WARNING_LIST& warninglist, const COleDateTime& timebegin, const COleDateTime& timeend);
    //bird add, read power consumption total
    bool    ReadPowerTotal(vector<RecordPointValue>& pointlist, const wstring& tablename, UINT pointIdx,
							const COleDateTime& oleTimeStart, COleDateTime& oleTimeEnd, bool ifOrder);
	bool    ReadRunningTimeTotal(vector<RecordPointValue>& pointlist, const wstring& tablename, UINT pointIdx,
		const COleDateTime& oleTimeStart, COleDateTime& oleTimeEnd, bool ifOrder);

	bool	ReadMonthReportRecord(vector<MonthReportParam> &reportParam,int nYear, int nMonth);

	//系统模式
	int ReadSystemModeFromDB();
	bool SetSystemModeToDB(int nSysmode);

	bool	ReadRecordList(vector<RecordPointValue>& vecRecord, const string& tableName, int  pointIdx,  const COleDateTime& oleTimeStart, const COleDateTime& oleTimeEnd );
	//bool	ReadRecordList(CArray<double>&  value_param5, const string& tableName, int  pointIdx,  const COleDateTime& oleTimeStart, const COleDateTime& oleTimeEnd );
	bool	ReadRecordMaxMIn(double& dMax, double& dMin, const string& tableName, int  pointIdx,  const COleDateTime& oleTimeStart, const COleDateTime& oleTimeEnd );
	
	void LogDBError(const char* sql);
	void LogDBError(const string& sql);
	
	bool	InsertCopRecord(const SYSTEMTIME& time, double value);
	bool	InsertLoadRecord(const SYSTEMTIME& time, double value);

	void	ReadCopRecord(vector<CopRecord>& result, const SYSTEMTIME& start, const SYSTEMTIME& end);
	void	ReadLoadRecord(vector<LoadRecord>& result, const SYSTEMTIME& start, const SYSTEMTIME& end);
	
	bool	InsertChillerPlantRoomInfo(const RoomEnergy& energyinfo, const SYSTEMTIME& currenttime);

	bool    ReadControlParamRecord(gControlParamRecord& record, const COleDateTime& timeStart,const COleDateTime& timeStop);
	bool    ReadControlParamRecord(string& strrecord, const COleDateTime& timeStart,const COleDateTime& timeStop);
private:	
	static Mutex               m_mutex;

    Mutex       m_recordmutex;
	static DBAccessContainer * g_DBAccessContainer;
	CUseLog					   m_log;   // log object.
};
#endif