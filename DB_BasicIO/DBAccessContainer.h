
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
	Type_UNKNOWNTYPE = 0x00	//δ֪
	, Type_CHILLER			//���
	, Type_PUMP				//ˮ��
	, Type_COOLINGTOWER		//��ȴ��
	, Type_HX				//�彻
	, Type_PIPE				//�ܵ�
	, Type_CORNER			//��ͷ
	, Type_TERMINAL			//�ն�
	, Type_VALVE			//����
	, Type_WATERNET			//����
	, Type_Tank				//ˮ��
	, Type_WATERSOIL		//��ů
	, Type_POWMETER			//���
};

typedef struct tag_Project_DB
{
	wstring		strprojectname;		//	��Ŀ����,��Ҫ�ڽ�������ʾ
	string		strdatabasename;	//	���ݿ�����
}Project_DB;

//����������ݵ���Ϣ
typedef struct tag_StartUpInfo
{
	string		bpicture;			//	��ĿͼƬ
	Project_DB  projectdb;		//	���ݿ�������Ŀ��
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
	double dChillerGroupPower;        // ������ܹ���
	double dPriPumpGroupPower;        // һ�α��ܹ���
	double dSecPumpGroupPower;        // ���α��ܹ���
	double dCoolPumpGroupPower;       // ��ȴ���ܹ���
	double dColTowerGroupPower;       // ��ȴ���ܹ���
	double dAllPower;          //chiller plant room �ܹ��� //bird add,

	double dChillerGroupTotalPower;   // ������õ���
	double dPriPumpGroupTotalPower;   // һ�α��õ���
	double dSecPumpGroupTotalPower;   // ���α��õ���
	double dCoolPumpGroupTotalPower;  // ��ȴ���õ���
	double dColTowerGroupTotalPower;  // ��ȴ���õ���
	double dAllTotalPower;              //chiller plant room ���õ��� 

	double dChillerPlantRoomLoad;     // ��������
	double dOutDoorTDry;              // �������
	double dOutDoorTWet;              // ����ʪ��

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


//���ݿ������
class DBDLL DBAccessContainer : public CDatabaseSeesion
{
private:
	DBAccessContainer();
public:
	~DBAccessContainer();
	static DBAccessContainer *GetInstance();
	static void DestroyInstance();

public:
	// ============= ��Lib����صĽӿ� ==========================
	int			GenerateNewIDInLib(DEVICETYPE type);//����豸�����id
	MYSQL_RES*	ReadAllEquipmentInLib(DEVICETYPE type);//��ȡlib��ĳ���͵������豸��Ϣ
	MYSQL_RES*	ReadAllEquipmentIDInLib(DEVICETYPE type);//��ȡlib��ĳ���͵������豸id
	MYSQL_RES*	ReadOneEquipmentFromLib(DEVICETYPE type, int id);// ��ȡlib�е�ĳһ�豸
	bool		DeleteAllEquipmentsInLib(DEVICETYPE type);//ɾ��ָ���豸lib�е�������Ϣ.	
	bool		AddEquipmentToLib(DEVICETYPE type, void* record);//���豸lib�м��豸
	bool		DeleteEquipmentInLib(DEVICETYPE type, int id);//ɾ���豸�е�lib		

	//====room_contain_��ر�Ľӿ�========================
	int         ReadMaxIDInRoom(DEVICETYPE type);
	MYSQL_RES*	ReadAllEquipmentsInRoom( DEVICETYPE type, int roomid);
	bool		DeleteAllEquipmentsInRoom( DEVICETYPE type, int roomid );	//ɾ��
	bool		AddEquipmentToRoom(DEVICETYPE type, void* record);			//���
//	bool		DeleteEquipmentFromRoom(DEVICETYPE type, int roomid , int equipmentid);	//ɾ��ָ���������ƶ��豸
	bool		ClearAllRooms();
	int			GetMaxIDInTable(string tablename, string fieldname);	
	
	//�޸�a���úͲ�����
	bool		AlterEnableProperty(DEVICETYPE type, bool enable, int chillerid, int roomid);


	//==== ��Model��ر�Ľӿ�     ====
	MYSQL_RES*	ReadALLModel(DEVICETYPE type);					//��������Ϣ
	MYSQL_RES*	ReadModel(DEVICETYPE type, int modelid);			// ��ģ�ͱ�
	bool		InsertIntoModelTable(DEVICETYPE type, void* record);	//дģ�ͱ�	
	bool		DeleteFromModelTable(DEVICETYPE type, int modelid);	//ɾ��ģ�ͱ�	
	MYSQL_RES*  ReadModelIDs(DEVICETYPE type);
	gCH_Model * ReadChillerModel( int nChillerModelID );
	gPump_Model * ReadPumpModel( int nPumpModelID );
	gCT_Model * ReadCTModel( int nCTMODELID );
	gWaterSoil_Model * ReadWSModel( int nWSMODELID );

	bool        UpdateModel(DEVICETYPE type, int modelid,string strModel_Param); //����ģ��
	MYSQL_RES*  Read_Load_Define_Tempalte();                              //��ȡ���ɶ���ģ��
	// ==== Project�Լ�chillerplantroom��ر�Ľӿ� ====
	MYSQL_RES*  GetProjectInfo();								// ��һ��project����Ϣ	
	bool		ReadProject( gProject * pProject);
	int         GetProjectID();									// ��ȡproject���е�id	
	bool		AddNewProject(gProject* projectinfo);			// ���һ��project
	bool		DeleteProject(int projectid);					// ɾ��һ��project	
	MYSQL_RES*	GetChillerPlantRoomIDList(int projectid);		// ��ȡproject�Ļ���id
	MYSQL_RES*	GetAllChillerPlantRoomInfo(int projectid);		// �����л�����Ϣ
	MYSQL_RES*	GetChillerPlantRoomInfoByID(int roomid);		// ��һ����������Ϣ
	bool		AddNewToChillerPlantRoom(gCHPlantRoom* room);	// ��chillerplantroom�Ӽ�¼
	bool		DeleteFromChillerPlantRoom(int roomid);			// ɾ��chillerplantroom��һ����¼
	int			GetMaxIDInChillerPlantRoom();					// ȡ��chillerplantroom�����id
	bool		CreateNewProject(gProject* projectinfo, string& newdbname, string copyfrom = TEMPLATEDATABASE_CREATE);		// ����һ������Ŀ,������һ�����ݿ�
	bool		ReadAllProjectsFromServer(vector<StartUpInfo*>& projectlist, const wstring& createuser);				//�����������ݿ�,��projectlist
	bool		ReadAllProjectsFromServer(vector<StartUpInfo*>& projectlist);											//�����������ݿ�,��projectlist
	bool		ReadAllProjectsFromServer(vector<Project_DB>& projectlist, const wstring& creator);
	bool		ClearChillerPlantRoom();

	// ==== Eff Design Data��ر�Ľӿ� ====
	bool ReadChillerEffDesignData(vector<gChiller_Eff_Design_Data*>& resultlist , int id);  //���
	bool ReadCoolingTowerEffDesignData(vector<gCT_Eff_Design_Data*>& resultlist , int id);  //��ȴ��
	bool ReadPumpEffDesignData(vector<gPump_Eff_Design_Data*>& resultlist , int id);        //��
	bool DeleteEffDesignData(DEVICETYPE type, int deviceid);								//ɾ��
	bool InsertEffDesignData(DEVICETYPE type, void* data);									//����	
	bool InsertChillerEffDesignData(const vector<gChiller_Eff_Design_Data*>& record);	
	bool InsertPumpEffDesignData(const vector<gPump_Eff_Design_Data*>& record);
	bool InsertCoolingTowerDesignData(const vector<gCT_Eff_Design_Data*>& record);

    // ==== ChillerPlantRoom Load Define���д�ӿ�====
	MYSQL_RES*  ReadRoomLoadDefine(int roomid);   //read	
	bool InsertIntoRoomLoadDefine(const vector<gRoomLoad*>& record);  // write
	bool DeleteFromRoomLoadDefine(int roomid); // delete
	
	// ==== waternet param��д�ӿ�  ====
	MYSQL_RES*	ReadWaternetParam(int id);
	MYSQL_RES*  ReadWaternetParamIDs();
	bool		InsertIntoWaterParam(gWaternetParam* record);
	bool		DeleteWaternetParam(int id);
	bool		ClearWaternetParam();

	//Weather data �ӿ�
	bool		GetProvinceList(vector<wstring>& provincelist);
	bool		GetCityList(vector< pair<long, wstring> >& citylist, wstring provincename);
	gWeatherDesignData*		GetWeatherDesignData(long stationid);
	bool		GetWeatherHourData( vector<gWeatherHourData* >& resultlist, long stationid);
    
	// ������¼
	bool		InsertWarningRecord(int warningindex);	// ���뾯����¼
	bool		InsertWarningRecord(SYSTEMTIME st, const wstring& warninginfo);	

	// ����chiller�����м�¼
	bool		InsertChillerRunRecord(const gCHRunRecord& record);    
	
	 //��������߼������м�¼
	bool		InsertControlParamRecord(const gControlParamRecord& record); 
	
	//������������¼
	bool		InsertFlowRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	//����
	bool		InsertPowerRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	//�¶�
	bool		InsertTemperatureRecord(const vector<DataSensor_Record_Entry>& entrylist);

	//ѹ��
	bool		InsertPressureDifferenceRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	//�õ���
	bool		InsertPowerConsumptionRecord(const vector<DataSensor_Record_Entry>& entrylist);

	//����ʱ��
	bool		InsertRunningTimeRecord(const vector<DataSensor_Record_Entry>& entrylist );

	//�¶ȱ���
	bool		InsertMonthReportRecord(const vector<MonthReportParam>& vctParam);

	//����
	bool		InsertOnOffRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	//Ƶ��
	bool		InsertFreqRecord(const vector<DataSensor_Record_Entry>& entrylist);
	
	bool		InsertPointRecordHelper(string tablename,
									   const vector<DataSensor_Record_Entry>& entrylist);
	// �û�������¼
	bool		InsertOperationRecord(const SYSTEMTIME& st, const wstring& remark,
									const wstring& wstrUser);

	//ϵͳ״̬��Ϣ.
	bool		InsertSystemStatusRecord(const SYSTEMTIME& st, int status);
	
	//ɾ�����м�¼
	bool		DeleteRunRecord(SYSTEMTIME start, SYSTEMTIME end, int roomid, int deviceid, RECORDTYPE type);
	
	//��ָ���豸��ĳһʱ���������м�¼
	MYSQL_RES*	ReadChillerRunRecord(SYSTEMTIME start, SYSTEMTIME end, int roomid, int chillerid);
	MYSQL_RES*	ReadChillerRunRecord1(string from, string to);
//	MYSQL_RES*	ReadControlParamRunRecord(SYSTEMTIME start, SYSTEMTIME end);
	
	
	void ReadRecordDateRange(vector<string>& daterange, RECORDTYPE recordtype, 
							 int roomid = 0, int deviceid = 0);

	//UPPC_USER.users���д�ӿ�
	bool	AddNewUser(gUppcSimulateUser* record);	//���һ����user	
	bool	DeleteUser(wstring username);			//ɾ��һ��user
	bool	ReadAllUsers(vector<wstring>& userlist);	//�����е�user�б�
	bool	ReadUserInfo(wstring username, UppcUserInfo* puserinfo);	// ��user����Ϣ
	bool	ReadALLUserInfo(vector<UppcUserInfo*>& userlist);
	bool	ValidateUser(wstring username, wstring password); // �ж��û��Ƿ����
	
	
	//��λ�б�
	bool	AddRecordPointList(const vector<Record_Point_List_Entry>& pointlist, 
							   const wstring& tablename);
	bool	ReadRecordPointList(vector<Record_Point_List_Entry>& pointlist_out, 
								const wstring& tablename);
	bool	ClearRecordPointList(const wstring& tablename);

	bool    DeleteRecordPointList(const vector<int>& pointlist,const wstring& tablename);

	bool    UpdateRecordPointList(PointlistParam &pParam,const wstring& tablename);

	//�豸���ص�λ�б�
	bool	AddDeviceOnOffPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadDeviceOnOffPointList( vector<Record_Point_List_Entry>& pointlist);
	bool	ClearDeviceOnOffPointList();

	//�豸Ƶ�ʵ�λ�б�	
	bool	AddDeviceFreqPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadDeviceFreqPointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearDeviceFreqPointList();	

	//���ʵ�λ�б�
	bool	AddPowerPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadPowerPointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearPowerPointList();

	//������λ�б�
	bool	AddFlowPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadFlowPointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearFlowPointList();

	//�¶ȵ�λ�б�
	bool	AddTemperaturePointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadTemperaturePointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearTemperaturePointList();

	//ѹ���λ�б�
	bool	AddPressureDifferencePointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadPressureDifferencePointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearPressureDifferencePointList();
	
	//�õ�����λ�б�
	bool	AddPowerConsumptionPointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadPowerConsumptionPointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearPowerConsumptionPointList();

	//����ʱ���λ�б�
	bool	AddRunningTimePointList(const vector<Record_Point_List_Entry>& pointlist);
	bool	ReadRunningTimePointList(vector<Record_Point_List_Entry>& pointlist);
	bool	ClearRunningTimePointList();
	
    //������
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

	//ϵͳģʽ
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