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
	//0 报警  1 忽略   2:直接确认 3 正在解决  4 解决完  5 误报
	ACTION_UNSOLVE =0,   //0 报警未处理
	ACTION_IGNORE,		//1 忽略 
	ACTION_CONFIRM,		//2:直接确认 
	ACTION_SOLVING,		//3 正在解决
	ACTION_RESOLVED,	// 4 解决完
	ACTION_BADWARNING,	//5 误报
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
	int nID;//note 的唯一数字ID
	int nOfBoardID;//所属board的ID
	wstring strUserName;//哪个用户加入的note
	int nStyle; //note样式
	double fPosX;//note左下角X坐标
	double fPosY;//note左下角Y坐标
	double fWidth;//note宽度
	double fHeight;//note高度
	wstring strContent; //note内容
	//wstring strFontName; //note字体名
	int    nFontSize;  //note 字体大小
	bool   bHasArrow;  //是否有箭头
	double fArrowEndX;//箭头线指向的终点X坐标
	double fArrowEndY;//箭头线指向的终点Y坐标
	int nArrowStyle;//箭头线的式样
	int nArrowWidth;//箭头线粗细
	DWORD dwTextColor;//
	wstring strNoteTitle;
}BoardNoteInfo;

typedef struct tag_board_benchmarkline_info
{
	tag_board_benchmarkline_info()
	{
		 bConst = false;
	};
	int nID; //线ID
	int nOfBoardID; //所属board的ID
	bool bConst; //是否是常数
	wstring strPointName; //线对应的点名 
	double fYMin; //量程最小值
	double fYMax;//量程最大值
	int nStyle;//线型
	int nWidth; //线粗细
	DWORD dwColor;//线颜色
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

		bool	SetForceClose();	//强行终止标志位

		bool	ReadForceClose();	//读取强行终止标志位

		bool	DeleteAction();

		//读取最近的历史记录，防止数据库重启时memory点丢失。
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
		/** 修改用户信息 */
		bool ModifyServerUserInfo(wstring strUser, wstring strPwd, wstring strRightLevel);

		/** 增加系统用户 */
		bool AddServerUserInfo(int nUserID, wstring strUser, wstring strPwd, wstring strRightLevel);

		/** 删除系统用户 */
		bool DelServerUserInfo(int nIndex);

		/** 读取系统用户信息 **/
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
		/** 添加一根辅助线  */
		int    AddBechmarkLine(BoardBenchmarkLineInfo & lineInfo);
		/** 修改辅助线的量程范围  */
		bool    ModifyBechmarkLine(int nLineID, const BoardBenchmarkLineInfo& benchmarkLineInfo);
		/** 移除一根辅助线  */
		bool    RemoveBenchmarkLine(int nLineID);
		/** 读取所有辅助线清单  */
		bool    ReadBenchmarkLineList(const int &nBoardID, vector<BoardBenchmarkLineInfo>& benchmarklineList);

		
		//实时曲线
		bool AddTrendLine(TrendLineInfo lineInfo);
		bool ModifyTrendLine(TrendLineInfo lineInfo);
		bool RemoveTrendLine(wstring strPointName,int BoardID);
		bool ReadTrendLineList(const int BoardID, vector<TrendLineInfo>& trendLineList);

		// 公告板 柱状图辅助线存储   Index,boardID,color,valuelist
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
