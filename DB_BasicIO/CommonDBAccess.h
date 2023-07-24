#pragma once


#include "DatabaseSession.h"

#include "../Tools/CustomTools/CustomTools.h"
#include "StructDefine/Define_ControlParamRecord.h"
#include "StructDefine/ActionInfo_Define.h"
#include "WarningConfigItem.h"
#include "../DataPoint/DataPointEntry.h"

#include "../Tools/USingletonTemplate.h"
#include "RealTimeDataAccess.h"
#include "WarningEntry.h"
#include "wtypes.h"
#include <vector>
#include <string>
#include <utility>


typedef struct _declspec(dllexport) tag_WarningHistoryStruct 
{
	SYSTEMTIME   timeFrom;
	SYSTEMTIME   timeTo;
	//wstring   physicalid;
	wstring strWarningLabel;
	wstring strWarningInfo;
	vector<wstring> vecAction;
	wstring strPointName;
	wstring strConfirmed;

}tag_WarningHistoryStruct;

typedef struct _declspec(dllexport) tag_UserRoleInfo
{
	int		nRoleID;
	wstring strRoleName;

}UserRoleInfo;

typedef struct _declspec(dllexport) tag_UserInfo
{
	int		nUserID;
	int		nRoleID;
	wstring strRoleName;
	wstring strUserName;
	wstring strUserPsw;
	wstring strExpireDate;

}UserInfo;

enum _declspec(dllexport) E_ACTIONTYPE
{
	//0 ����  1 ����   2:ֱ��ȷ�� 3 ���ڽ��  4 �����  5 ��
	ACTION_UNSOLVE =0,   //0 ����δ����
	ACTION_IGNORE,		//1 ���� 
	ACTION_CONFIRM,		//2:ֱ��ȷ�� 
	ACTION_SOLVING,		//3 ���ڽ��
	ACTION_RESOLVED,	// 4 �����
	ACTION_BADWARNING,	//5 ��
};

typedef struct _declspec(dllexport) _tagActionInfo
{
	wstring strActionUser;
	wstring strActionTime;
	wstring strAction;
}ActionInfo;

enum _declspec(dllexport) WARNING_CONFIG_TYPE
{
	WARNING_CONFIG_UNITPROPERTY01 =0,
	WARNING_CONFIG_UNITPROPERTY02,
	WARNING_CONFIG_UNITPROPERTY03,
	WARNING_CONFIG_UNITPROPERTY04,
	WARNING_CONFIG_UNITPROPERTY05,
};

// Work Order parts
typedef struct tag_wo_item
{
	CString strItemName;
	int		nDepartmentNum;
	int		nPeopleNum;
	COleDateTime tmStart;
	int		nDeviceNum;
	CString	strDesc;
	double	dCost;
	int		nResultNum;
	CString	strRemark;

	tag_wo_item()
	{
		nDepartmentNum = 0;
		nPeopleNum = 0;
		nDeviceNum = 0;
		dCost = 0.00;
		nResultNum = 0;
	}
}WorkOrderItem;

typedef struct tag_wo_progress
{
	int		nItemNum;
	int		nProgressNum;
	int		nPeopleNum;

	tag_wo_progress()
	{
		nItemNum = 0;
		nProgressNum = 0;
		nPeopleNum = 0;
	}
}WorkOrderProgress;

typedef struct tag_wo_department
{
	int		nDepartmentNum;
	CString	strDepartmentName;

	tag_wo_department()
	{
		nDepartmentNum = 0;
	}
}WorkOrderDepartment;

typedef struct tag_wo_people
{
	int		nPeopleNum;
	CString	strPeopleName;

	tag_wo_people()
	{
		nPeopleNum = 0;
	}
}WorkOrderPeople;

typedef struct tag_wo_device
{
	int		nDeviceNum;
	CString	strDeviceName;

	tag_wo_device()
	{
		nDeviceNum = 0;
	}
}WorkOrderDevice;

typedef struct tag_wo_result
{
	int		nResultNum;
	CString	strResultName;

	tag_wo_result()
	{
		nResultNum = 0;
	}
}WorkOrderResult;

typedef struct tag_wo_warn_relate
{
	CString	strItemName;
	COleDateTime tmStart;
	CString	strWarnInfo;
}WorkOrderWarnRelate;

typedef struct tag_board_note_info
{
	tag_board_note_info()
	{
		bHasArrow = true;
		nStyle = 0;
		dwTextColor = RGB(0,0,0);
	};
	int nID;//note ��Ψһ����ID
	int nOfBoardID;//����board��ID
	wstring strUserName;//�ĸ��û������note
	int nStyle; //note��ʽ
	double fPosX;//note���½�X����
	double fPosY;//note���½�Y����
	double fWidth;//note���
	double fHeight;//note�߶�
	wstring strContent; //note����
	//wstring strFontName; //note������
	int    nFontSize;  //note �����С
	bool   bHasArrow;  //�Ƿ��м�ͷ
	double fArrowEndX;//��ͷ��ָ����յ�X����
	double fArrowEndY;//��ͷ��ָ����յ�Y����
	int nArrowStyle;//��ͷ�ߵ�ʽ��
	int nArrowWidth;//��ͷ�ߴ�ϸ
	DWORD dwTextColor;//
	wstring strNoteTitle;
}BoardNoteInfo;

typedef struct tag_board_benchmarkline_info
{
	tag_board_benchmarkline_info()
	{
		 bConst = false;
	};
	int nID; //��ID
	int nOfBoardID; //����board��ID
	bool bConst; //�Ƿ��ǳ���
	wstring strPointName; //�߶�Ӧ�ĵ��� 
	double fYMin; //������Сֵ
	double fYMax;//�������ֵ
	int nStyle;//����
	int nWidth; //�ߴ�ϸ
	DWORD dwColor;//����ɫ
	wstring strPointDescription; //jl  not used

}BoardBenchmarkLineInfo;


typedef struct tag_board_trendline_info
{
	tag_board_trendline_info()
	{
		dwColor = RGB(0,0,0);
	}
	wstring strPointName;
	int nOfBoardID;
	DWORD  dwColor;


} TrendLineInfo;

typedef struct tag_board_barline_info
{
	int nID;
	int nOfBoardID;
	DWORD	dwColor;
	vector<double> valuelist;
	wstring strPointName;
	bool	bShow;

} BarLineInfo;

namespace Beopdatalink
{

	class _declspec(dllexport) CCommonDBAccess  : public CDatabaseSeesion
	{
	public:
		CCommonDBAccess(void);
		~CCommonDBAccess(void);

	public:
		bool	ReadWarningConfig(vector<CWarningConfigItem>& configList);
		bool	AddWarningConfig(const CWarningConfigItem & configItem);
		bool    ModifyWarningConfig(const CWarningConfigItem & configItem);
		bool	DeleteWarningConfig(wstring strConfigPointName); 

		bool	ReadWarningConfigGroupType(const WARNING_CONFIG_TYPE type, wstring& GroupName, vector<wstring>& vecSeries);

		bool	ConfirmWarning(const CWarningEntry& entry, wstring strUser);
		bool	ConfirmWarning(const CWarningEntry& entry, wstring strUser, int nResolveType);
		bool	ConfirmWarning(const CWarningEntry& entry, wstring strUser, int nResolveType,bool bIsEnglishMode);
		bool	ConfirmWarningAll(const wstring &strUser);

		string	GetWarningAction(int nType, bool bIsEnglishMode);

		bool	ReadWarning(vector<CWarningEntry>& resultlist, const SYSTEMTIME& start, const SYSTEMTIME& end, bool bUnconfirmed = false);
		bool	ReadWarningOperationByInfo(vector<ActionInfo>& resultlist, const wstring strInfo, int nRecentCount = 10);
		bool	ReadWarningOperationByPointName(vector<wstring>& resultlist, const wstring strPointName, const SYSTEMTIME& start, const SYSTEMTIME& end);

		bool	DeleteWarning();

		bool	InsertAction(const wstring& systemaction, const wstring& chillerlist);

		bool	ReadAction(wstring& systemaction, wstring& chillerlist);

		bool	SetForceClose();	//ǿ����ֹ��־λ

		bool	ReadForceClose();	//��ȡǿ����ֹ��־λ

		bool	DeleteAction();

		//��ȡ�������ʷ��¼����ֹ���ݿ�����ʱmemory�㶪ʧ��
		bool	ReadLatestHistoryData(vector<CRealTimeDataEntry>& resultlist);
		bool	ReadHistoryDataInTable(wstring & strValue, const COleDateTime &sTime,const wstring &strPointName, const wstring &strTableName);
		bool	ReadHistoryDataInTable(vector<CRealTimeDataEntry>& resultlist, const COleDateTime& start, const COleDateTime& end, const wstring &strTableName);
		bool	ReadHistoryDataInTable(vector<CRealTimeDataEntry>& resultlist, const COleDateTime& start, const COleDateTime& end, const wstring &strPointName, const wstring &strTableName);
		bool	ReadLastHistoryDataInTable(wstring & strValue,const wstring &strPointName, const wstring &strTableName);

		bool	InsertControlParamRecord( const gControlParamRecord& record);

		bool    ReadServerPointsList(vector<DataPointEntry> &vecPtList);
		bool	ReadWatchingPoinitList(std::vector<wstring> &strWatchPointNameList);
		bool	SetPointWatching(wstring wstrPointName, bool bWatching);
		bool    ClearLogicBadRecord();

		//MYSQL_RES*	ReadControlParamRunRecord( /*SYSTEMTIME start, SYSTEMTIME end */);
		bool	ReadControlParamRunRecord( gControlParamRecord & mControlParamRecord);


		bool	InsertOperationRecord(const SYSTEMTIME& st, const wstring& remark, const wstring& wstrUser);
		bool	ReadAllOperationRecord(VEC_OPT_RECORD&  vecOperation, const COleDateTime& timeStart, const COleDateTime& timeStop);
		bool	DeleteOperationRecord();

		bool	InsertActionInfo(const gActinInfo_Record& record);
		bool	ReadAllActionInfo(vector< gActinInfo_Record >& actioninfolist);		
		bool	DeleteAllActionInfo();
		bool	ModifyActionStatus(const wstring& info, int nStatus); //0:ready, 1:running, 2: success, 3:failed.


		bool	RegisterUserRecord(const CUserOnlineEntry& entry);

		bool	UpdateUserOnlineTime(wstring username, wstring usertype, SYSTEMTIME time);

		bool	ReadUserRecord(vector<CUserOnlineEntry>& resultlist/*, const SYSTEMTIME& start, const SYSTEMTIME& end*/);

		bool	DeleteUserRecord(wstring username, wstring usertype);

		bool	IsUserOnline(wstring username, wstring usertype);

		bool	GetServerTime(SYSTEMTIME &tServerTime);

		bool	CheckDBVersion(wstring &strVersion);

		bool	FindIfExistInVec(wstring str, vector<wstring> vec);

		// ========================== user manager =====================================

		//========================== user manager ================================
		/** �޸��û���Ϣ */
		bool ModifyServerUserInfo(wstring strUser, wstring strPwd, wstring strRightLevel);

		/** ����ϵͳ�û� */
		bool AddServerUserInfo(int nUserID, wstring strUser, wstring strPwd, wstring strRightLevel);

		/** ɾ��ϵͳ�û� */
		bool DelServerUserInfo(int nIndex);

		/** ��ȡϵͳ�û���Ϣ **/
		bool GetServerUserInfo(vector<UserInfo>& vecUsers);

		bool CheckAndInsertDefaultUserInfo();

		bool ImportCSVToTable(const wstring strTableName, const wstring strCSVPath);
		bool ExportTableToCSV(const wstring strTableName, const wstring strCSVPath);

		bool GetLogDatabaseExist();
		
		// ==========================connnection status =====================================
	public:
		bool	SetStatus(const wstring& statustype, int value);
		bool	GetStatus(vector< pair<wstring, int> >& statuslist);

		bool	SetPLCConnectionStatus(bool bconnected);
		bool	GetPLCConnectionStatus();
		bool	GetModbusConnectionStatus();
		int		GetSystemActionStatus();


		bool	InsertLog(const wstring& loginfo);
		bool    InsertLog(SYSTEMTIME st, const wstring & loginfo);
		bool	ReadLog(vector< pair<wstring,wstring> >& loglist, const SYSTEMTIME& start);
		bool	ReadLogByFilter(vector< pair<wstring,wstring> >& loglist, const SYSTEMTIME& start, const SYSTEMTIME& end, const wstring& strLogFilter);

		bool	ExportLog(const SYSTEMTIME& start, const SYSTEMTIME& end, const wstring& filepath);

		bool    DeleteLog(const SYSTEMTIME tEnd);

		wstring ReadOrCreateCoreDebugItemValue(wstring strItemName);
		wstring ReadOrCreateCoreDebugItemValue(wstring strItemName, wstring strItemName2, wstring strItemName3);
		wstring ReadOrCreateCoreDebugItemValue_Defalut(wstring strItemName,wstring strValue = L"0");
		bool    WriteCoreDebugItemValue(const wstring &strItemName, const wstring &strItemValue);
		bool    WriteCoreDebugItemValue(const wstring &strItemName, const wstring &strItemName2,const wstring &strItemValue);
		bool    RemoveCoreDebugItem(const wstring &strItemName, const wstring &strItemName2);

		bool    WriteCoreDebugItemValue(const wstring &strItemName, const wstring &strItemName2, const wstring &strItemName3, const wstring &strItemValue);
		bool    RemoveCoreDebugItem(const wstring &strItemName, const wstring &strItemName2,const wstring &strItemName3);
		//julian 0824
		bool WriteMysqlGlobalVariable( const wstring &strItemName,const wstring &strGlobalVariable);
		wstring ReadGlobalMysqlVariable_Defalut( wstring strItemName );
		bool	InsertLogicParameters(wstring strThreadName, wstring strLogicName, wstring strSetType, wstring strVariableName, wstring strLinkName, wstring strLinkType, wstring strCondition);

		bool    ReadWarningHistory(const wstring &strWarningInfo, std::vector<tag_WarningHistoryStruct> & warningHistoryList);
		bool	ReadWarningHistory(std::vector<tag_WarningHistoryStruct> & warningHistoryList, int nRowCount);
		bool	ReadWarningHistoryByPointName(const wstring strPointName,std::vector<tag_WarningHistoryStruct> & warningHistoryList, int nRowCount);

		bool	ReadRecentWarningActionByPointName(const wstring strPointName,wstring& strAction);

		bool	GetHistoryValue(const wstring& strName,const SYSTEMTIME &st,const int &nTimeFormat,  wstring &strParam);
		bool	GetHistoryValue(const wstring& strName,const SYSTEMTIME &stStart,const SYSTEMTIME &stEnd,const int &nTimeFormat,  wstring &strParam);
		bool    GetHistoryValueInOneDay(const wstring& strName,const SYSTEMTIME &stStart,const SYSTEMTIME &stEnd,const int &nTimeFormat, wstring &strParam);
		string	GetHistoryTableNameByStoreCycle(const SYSTEMTIME &stNow, const POINT_STORE_CYCLE &tCycle);
		bool	ReadHistoryDataInTableFix(wstring & strValue, const COleDateTime &sTime,const wstring &strPointName, const wstring & strTableName, const POINT_STORE_CYCLE &tCycle);
		bool	ReadHistoryDataInTableFix(wstring & strValue, const COleDateTime &sStartTime, const COleDateTime &sEndTime, const wstring &strPointName, const wstring & strTableName, const POINT_STORE_CYCLE &tCycle);
		bool	GetLatestEntryByPointName(const wstring& strPointName, const SYSTEMTIME &stTime, const POINT_STORE_CYCLE &tCycle, CRealTimeDataEntry& resultEntry, bool bGetCount, int& nCount);
		bool	GetHistoryDataFixedMulti( const vector<wstring>& strNameVec,const SYSTEMTIME &stStart,const SYSTEMTIME &stEnd,const int &nTimeFormat, vector<vector<CRealTimeDataEntry>>& entryListVec );
		bool	UpdateHistoryDataPointName( const wstring& wstrTableName, const wstring& wstrOldName, const wstring& wstrNewName  );
		bool	UpdateHistoryDataPointName( const vector<wstring>& wstrTableNameList, const vector<wstring>& wstrOldNameList, const vector<wstring>& wstrNewNameList );
		bool	IsTableExist( const wstring strTableName );		
		bool	UpdateRemark(const wstring strRemark, const wstring& strPointName, const SYSTEMTIME st);
		bool	ReadRemark(wstring& strRemark, const wstring& strPointName, const SYSTEMTIME st);
// create work order talbes if not exist
	public:
		void	CreateWorkOrderAllTables(void);
		bool	ReadWorkOrderItems(vector<WorkOrderItem>& vecWoItem);
		bool	ReadWorkOrderProgress(vector<WorkOrderProgress>& vecWoProgress);
		bool	ReadWorkOrderDepartment(vector<WorkOrderDepartment>& vecWoDepartm);
		bool	ReadWorkOrderPeople(vector<WorkOrderPeople>& vecWoPeople);
		bool	ReadWorkOrderDevice(vector<WorkOrderDevice>& vecWoDevice);
		bool	ReadWorkOrderResult(vector<WorkOrderResult>& vecWoRes);
		bool	ReadWorkOrderWarnRelate(vector<WorkOrderWarnRelate>& vecWoWarnRelate);
		bool	InsertWorkOrderItem(const WorkOrderItem& woItem);
		bool	UpdateWorkOrderItem(const WorkOrderItem& woItem);
		bool	DeleteWorkOrderItem(const wstring strItemName);
	private:
		bool	CreateWorkOrderItem(void);
		bool	CreateWorkOrderProgress(void);
		bool	CreateWorkOrderDepartment(void);
		bool	CreateWorkOrderPeople(void);
		bool	CreateWorkOrderDevice(void);
		bool	CreateWorkOrderResult(void);
		bool	CreateWorkOrderWarningRelate(void);
	public:	// for online loading
		bool	UpdateLibInsertIntoFilestorage(const wstring strFilePathName, const wstring strFileName, const wstring strFileId);
		bool	UpdateLibInsertIntoUnit02(const wstring strFileName, bool bNeedSaveToDB);
		bool	UpdateLibInsertIntoUnit02(wstring strUpdateState, wstring strThreadName, wstring strDLLName, wstring strFilePath, wstring strImportTime,wstring strAuthor, wstring strPeriod, wstring strDllContent, wstring strRunStatus,wstring strVersion,wstring strDescription,wstring strDllOldName, bool bNeedSaveToDB);
		bool	UpdateLibReloadIntoUnit02(const wstring strFileName);


		int     GetMaxNoteID();
		int		GetMaxBenchmarkLineID();
		//for online notice board
		int     InsertNote(BoardNoteInfo& noteInfo);
		bool    MoveNote(int nNoteID, const double& fPosX, const double& fPosY);
		bool    ResizeNote(int nNoteID, const double& fWidth, const double& fHeight);
		bool    ModifyNoteProperty(int nNoteID, const BoardNoteInfo& noteInfo);
		bool    RemoveNote(int nNoteID);
		bool    ReadNoteList(const int &nBoardID, vector<BoardNoteInfo>& noteList);
		

		//for arrow
		bool    MoveArrow(int nNoteID, const double& fArrowEndX, const double & fArrowEndY);

		//add benchmark line
		/** ���һ��������  */
		int    AddBechmarkLine(BoardBenchmarkLineInfo & lineInfo);
		/** �޸ĸ����ߵ����̷�Χ  */
		bool    ModifyBechmarkLine(int nLineID, const BoardBenchmarkLineInfo& benchmarkLineInfo);
		/** �Ƴ�һ��������  */
		bool    RemoveBenchmarkLine(int nLineID);
		/** ��ȡ���и������嵥  */
		bool    ReadBenchmarkLineList(const int &nBoardID, vector<BoardBenchmarkLineInfo>& benchmarklineList);

		
		//ʵʱ����
		bool AddTrendLine(TrendLineInfo lineInfo);
		bool ModifyTrendLine(TrendLineInfo lineInfo);
		bool RemoveTrendLine(wstring strPointName,int BoardID);
		bool ReadTrendLineList(const int BoardID, vector<TrendLineInfo>& trendLineList);

		// ����� ��״ͼ�����ߴ洢   Index,boardID,color,valuelist
		bool AddBarLine(BarLineInfo & lineInfo);
		bool ReadBarLineList(int nBoardID, vector<BarLineInfo> &barLineList);
		bool ModifyBarLine(int nlineID,const BarLineInfo& lineInfo);
		bool RemoveBarLine(int nLineID);		
		int	GetMaxBarLineID();

		bool ReadRealTimeDataInputValue(const vector<wstring>& vecPtName, map<wstring, float>& mapRes);

		bool GetCoreVersion(wstring &strCoreVersion);
		bool GetLogicVersion(wstring &strLogicVersion);
		bool GetPysiteVersion(wstring &strPysiteVersion);
	
	public:
		bool Insert_logic_output_point_record(wstring wstrLogicName, wstring wstrPointName, wstring wstrPointValue );
		bool update_unit01_ThreadName(wstring wstrOldName, wstring wstrNewName);
		bool ReadBeopdataAllTableList(vector<wstring>& vecPtName);
		bool ReadBeopdataAllHistoryDataTableList(vector<wstring>& vecPtName);


		CString m_strProjectDBFilePath;
	private:
		Project::Tools::Mutex	m_lock;
		static int BAR_LINE_ID_INCRE;
	};
}
