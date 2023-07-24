#include "StdAfx.h"
#include "CommonDBAccess.h"
#include <iostream>
#include <sstream>

#include "MysqlDeleter.h"
#include "../Tools/Scope_Ptr.h"

#include "../Tools/Util/UtilString.h"
const wstring g_strUnit01DefaultEng = L"System|Default";
const wstring g_strUnit01DefaultChn = L"System|系统报警";

namespace Beopdatalink
{
	//  this three will hold a session


	// history data will hold a session
	const char* tablename_historydata = "historydata";
	const char* sql_createtable =  "(`time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,\
								   `pointname` varchar(64) NOT NULL,\
								   `value` double(20,2) NOT NULL,\
								   PRIMARY KEY (`time`,`pointname`)) \
								   ENGINE=InnoDB DEFAULT CHARSET=utf8";

	const char* sql_createlog = "(`time` timestamp NOT NULL DEFAULT '2000-01-01 00:00:00',\
								`loginfo` varchar(1024) \
								) ENGINE=InnoDB DEFAULT CHARSET=utf8";

	//warning record will hold a session
	const char* tablename_warningrecord = "warningrecord";
	const char* tablename_warningconfig = "warning_config";
	//user online record will hold a session
	const char* tablename_userrecord = "userlist_online";


	//controlparam record will hold a session
	const char* tablename_controlparamrecord = "controlparam_record";
	//action info will hold a session
	const char* tablename_actioninfo = "actioninfo";
	// this 2 will hold a session.
	const char* tablename_datacom_status = "datacom_status";


	// action will hold a session
	const char* tablename_action = "action";
	//Operation record will hold a session
	const char* tablename_operationrecord = "operation_record";

	const char* tablename_log = "log";
	
	const char* tablename_warningoperation = "unit05";

	const char* tablename_user = "beopuser";
	const char* tablename_role = "beoprole";

	const char* tablename_realtimedata_input = "realtimedata_input";
}

int Beopdatalink::CCommonDBAccess::BAR_LINE_ID_INCRE  = -1;

Beopdatalink::CCommonDBAccess::CCommonDBAccess(void)
{
}


Beopdatalink::CCommonDBAccess::~CCommonDBAccess(void)
{
}




bool Beopdatalink::CCommonDBAccess::RegisterUserRecord( const CUserOnlineEntry& entry )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strtime = Project::Tools::SystemTimeToString(entry.GetTimestamp());

	string username_utf8;
	Project::Tools::WideCharToUtf8(entry.GetUserName(), username_utf8);

	string userhost_utf8;
	Project::Tools::WideCharToUtf8(entry.GetUserHost(), userhost_utf8);

	string usertype_utf8;
	Project::Tools::WideCharToUtf8(entry.GetUserType(), usertype_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);

	//generate the insert string
	std::ostringstream sqlstream;
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_userrecord 
		<< " value"
		<< "('" << username_utf8 << "'" << ",'" <<  userhost_utf8 << "',"
		<< entry.GetUserPriority() << ",'" <<  usertype_utf8 << "'" << ",'" << strtime << "')"; 

	string sqlstatement = sqlstream.str();

	bool bresult =  Execute(sqlstatement);
	if (!bresult)
	{
		sqlstream.str("");
		sqlstream << "update  "
			<< GetDBName()
			<< "."
			<< tablename_userrecord
			<< " set time='" << strtime << "', priority=" << entry.GetUserPriority() << " "
			<< " where username='" << username_utf8 << "'"
			<< " and usertype='" << usertype_utf8 << "'";

		sqlstatement = sqlstream.str();
		return Execute(sqlstatement);
	}

	return true;

}



bool Beopdatalink::CCommonDBAccess::UpdateUserOnlineTime(wstring username, wstring usertype, SYSTEMTIME time)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strtime = Project::Tools::SystemTimeToString(time);

	string username_utf8;
	Project::Tools::WideCharToUtf8(username, username_utf8);

	string usertype_utf8;
	Project::Tools::WideCharToUtf8(usertype, usertype_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);

	//generate the insert string
	std::ostringstream sqlstream;
	sqlstream << "update "
		<< GetDBName()
		<<"."
		<< tablename_userrecord 
		<< " set time='" << strtime << "'"
		<< " where username='" << username_utf8 << "'"
		<< " and usertype='" << usertype_utf8 << "'";

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}




bool Beopdatalink::CCommonDBAccess::ReadUserRecord( vector<CUserOnlineEntry>& resultlist)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	//string str_start = Project::Tools::SystemTimeToString(start);
	//string str_end = Project::Tools::SystemTimeToString(end);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_userrecord;
	//<< " where time >= '" << str_start << "' and time <= '" << str_end << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	resultlist.reserve(length);
	MYSQL_ROW  row = NULL;

	CUserOnlineEntry entry;
	SYSTEMTIME st;
	while(row = FetchRow(result) )
	{
		wstring username = Project::Tools::UTF8ToWideChar(row[0]);
		entry.SetUserName(username);

		wstring userhost = Project::Tools::UTF8ToWideChar(row[1]);
		entry.SetUserHost(userhost);

		entry.SetUserPriority(ATOI(row[2]));

		wstring usertype = Project::Tools::UTF8ToWideChar(row[3]);
		entry.SetUserType(usertype);

		Project::Tools::String_To_SysTime(row[4], st);
		entry.SetTimestamp(st);

		resultlist.push_back(entry);
	}

	return true;
}


bool Beopdatalink::CCommonDBAccess::DeleteUserRecord(wstring username, wstring usertype)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string username_utf8;
	Project::Tools::WideCharToUtf8(username, username_utf8);

	string usertype_utf8;
	Project::Tools::WideCharToUtf8(usertype, usertype_utf8);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_userrecord
		<< " where username = '" << username_utf8 << "' and usertype = '" << usertype_utf8 << "'";

	return Execute(sqlstream.str());
}


bool Beopdatalink::CCommonDBAccess::GetServerTime(SYSTEMTIME &tServerTime)
{
	bool bReadSuccess = false;
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	std::ostringstream sqlstream;
	sqlstream << "select now()"; 

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	MYSQL_ROW  row = NULL;

	SYSTEMTIME st;
	if(row = FetchRow(result) )
	{
		Project::Tools::String_To_SysTime(row[0], st);
		tServerTime = st;
		bReadSuccess  = true;
	}

	return bReadSuccess;
}

bool	Beopdatalink::CCommonDBAccess::CheckDBVersion(wstring &strVersion)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	//string str_start = Project::Tools::SystemTimeToString(start);
	//string str_end = Project::Tools::SystemTimeToString(end);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< "beopinfo where infoname = 'version'";
	//<< " where time >= '" << str_start << "' and time <= '" << str_end << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}

	MYSQL_ROW  row = NULL;

	CUserOnlineEntry entry;

	if(row = FetchRow(result) )
	{
		strVersion = Project::Tools::UTF8ToWideChar(row[1]);
		return true;
	}

	return false;
}


bool Beopdatalink::CCommonDBAccess::IsUserOnline(wstring username, wstring usertype)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	vector<CUserOnlineEntry> userOnlinelist;

	if(ReadUserRecord(userOnlinelist))
	{
		for(unsigned int i=0;i<userOnlinelist.size();i++)
		{
			if((username == userOnlinelist[i].GetUserName()) &&
				(usertype == userOnlinelist[i].GetUserType()) )

			{
				COleDateTime   usetime(userOnlinelist[i].GetTimestamp());

				if((usertype == _T("dataengine")) || (usertype == _T("serverengine")))
				{
					std::ostringstream sqlstream;
					sqlstream << "select now()"; 

					MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
					if (result == NULL){
						return false;
					}
					Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

					int length = static_cast<int>(GetRowNum(result));
					if (0 == length){
						return false;
					}
					MYSQL_ROW  row = NULL;

					SYSTEMTIME st;
					if(row = FetchRow(result) )
					{
						Project::Tools::String_To_SysTime(row[0], st);
						COleDateTime   engineCurTime(st);

						COleDateTimeSpan enginespan = engineCurTime - usetime;	

						double tempsec = enginespan.GetTotalSeconds();
						if (tempsec < 1*60)	
						{	
							return true;
						}
					}
				}
				else
				{
					COleDateTimeSpan standbyspan = COleDateTime::GetCurrentTime() - usetime;	

					double temptotalsec = standbyspan.GetTotalSeconds();
					if (temptotalsec < 1*60)	
					{	
						return true;
					}
				}
			}
		}
	}

	return false;
}


bool	Beopdatalink::CCommonDBAccess::ReadHistoryDataInTable(wstring & strValue, const COleDateTime &sTime,const wstring &strPointName, const wstring & strTableName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);


	string strTableName_ansi;
	Project::Tools::WideCharToUtf8(strTableName, strTableName_ansi);

	wstring wstr_time;
	Project::Tools::OleTime_To_String(sTime, wstr_time);

	string time_ansi;
	Project::Tools::WideCharToUtf8(wstr_time, time_ansi);

	string pname_ansi;
	Project::Tools::WideCharToUtf8(strPointName, pname_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName()
		<< "."
		<< strTableName_ansi << " where time = '"<< time_ansi<<"' and pointname = '"<<  pname_ansi<<"'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	//resultlist.reserve(length);
	MYSQL_ROW  row = NULL;
	CRealTimeDataEntry entry;

	if(row = FetchRow(result) )
	{   
		Project::Tools::UTF8ToWideChar(row[2], strValue);
	}

	return true;
}

bool	Beopdatalink::CCommonDBAccess::ReadHistoryDataInTableFix(wstring & strValue, const COleDateTime &sTime,const wstring &strPointName, const wstring & strTableName, const POINT_STORE_CYCLE &tCycle)
{
	bool bResult = false;

	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);


	string strTableName_ansi;
	Project::Tools::WideCharToUtf8(strTableName, strTableName_ansi);

	wstring wstr_time;
	Project::Tools::OleTime_To_String(sTime, wstr_time);

	string time_ansi;
	Project::Tools::WideCharToUtf8(wstr_time, time_ansi);

	string pname_ansi;
	Project::Tools::WideCharToUtf8(strPointName, pname_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName()
		<< "."
		<< strTableName_ansi << " where time = '"<< time_ansi<<"' and pointname = '"<<  pname_ansi<<"'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);
	if (result->row_count == 0)
	{
		COleDateTimeSpan tspan;
		if(tCycle==E_STORE_FIVE_SECOND)
		{
			tspan.SetDateTimeSpan(0,0,0,5);
		}
		else if(tCycle==E_STORE_ONE_MINUTE)
		{
			tspan.SetDateTimeSpan(0,0,1,0);
		}
		else if(tCycle==E_STORE_FIVE_MINUTE)
		{
			tspan.SetDateTimeSpan(0,0,5,0);
		}
		else if(tCycle==E_STORE_ONE_HOUR)
		{
			tspan.SetDateTimeSpan(0,1,0,0);
		}
		else if(tCycle==E_STORE_ONE_DAY)
		{
			tspan.SetDateTimeSpan(1,0,0,0);
		}
		else if(tCycle==E_STORE_ONE_MONTH)
		{
			tspan.SetDateTimeSpan(30,0,0,0);
		}
		else if(tCycle==E_STORE_ONE_YEAR)
		{
			tspan.SetDateTimeSpan(365,0,0,0);
		}
		int nSeconds = tspan.GetTotalSeconds();
		COleDateTime oletime = sTime - tspan;
		wstring strStart;
		wstring strEnd;
		Project::Tools::OleTime_To_String(oletime, strStart);
		Project::Tools::OleTime_To_String(sTime, strEnd);
		string str1;
		string str2;
		Project::Tools::WideCharToUtf8(strStart, str1);
		Project::Tools::WideCharToUtf8(strEnd, str2);
		std::ostringstream sqlstream1;
		vector<CRealTimeDataEntry> entryList; 
		sqlstream1 << "select * from "
			<< GetDBName()
			<<"." 
			<< strTableName_ansi 
			<< " where pointname = '"
			<< pname_ansi<<"' and time>'"<<str1<<"' and time<'"<<str2<<"'";
		MYSQL_RES* result1 = QueryAndReturnResult(sqlstream1.str().c_str());
		if(result1 == NULL)
		{
			bResult = false;
		}
		else
		{
			Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result1);
			int length = static_cast<int>(GetRowNum(result1));
			if (0 == length)
			{
				bResult = false;	
			}
			else
			{
				MYSQL_ROW  row = NULL;
				if(row = FetchRow(result1) )
				{   
					Project::Tools::UTF8ToWideChar(row[2], strValue);
					bResult = true;
				}
			}
		}
	}
	else
	{
		int length = static_cast<int>(GetRowNum(result));
		if (0 == length)
		{
			bResult = false;
		}
		else
		{
			MYSQL_ROW  row = NULL;
			if(row = FetchRow(result) )
			{   
				Project::Tools::UTF8ToWideChar(row[2], strValue);
				bResult = true;
			}
		}
	}
	return bResult;
}

bool Beopdatalink::CCommonDBAccess::ReadLatestHistoryData( vector<CRealTimeDataEntry>& resultlist )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	SYSTEMTIME st_time;
	GetLocalTime(&st_time);
	CStringA tablename;
	tablename.Format("historydata_%d_%02d",st_time.wYear, st_time.wMonth);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName()
		<< "."
		<< tablename.GetString() << " where time = (select max(time) from " << GetDBName() << "." << tablename.GetString() << ")";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	//resultlist.reserve(length);
	MYSQL_ROW  row = NULL;
	CRealTimeDataEntry entry;

	while(row = FetchRow(result) )
	{   
		SYSTEMTIME sttime;
		Project::Tools::String_To_SysTime(row[0], sttime);
		entry.SetTimestamp(sttime);
		entry.SetName(row[1]);

		wstring wstrValue;
		Project::Tools::UTF8ToWideChar(row[2], wstrValue);
		entry.SetValue(wstrValue);

		resultlist.push_back(entry);
	}

	return true;

}



bool  Beopdatalink::CCommonDBAccess::ConfirmWarning(const CWarningEntry& entry, wstring strUser)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strtime = Project::Tools::SystemTimeToString(entry.GetTimestamp());
	string warninginfo_utf8;
	Project::Tools::WideCharToUtf8(entry.GetWarningInfo(), warninginfo_utf8);

	string strtUser_utf8;
	Project::Tools::WideCharToUtf8(strUser, strtUser_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	std::ostringstream sqlstreamUpdate;
	sqlstreamUpdate << "update "
		<< GetDBName()
		<<"."
		<< tablename_warningrecord 
		<< " set confirmed = 1,  confirmeduser = '" << strtUser_utf8 << "'"
		<< " where info = '" << warninginfo_utf8 << "'  and time = '" << strtime <<"'";

	string sqlstatement = sqlstreamUpdate.str();

	return Execute(sqlstatement);

}

bool Beopdatalink::CCommonDBAccess::ConfirmWarning( const CWarningEntry& entry, wstring strUser, int nResolveType )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strtime = Project::Tools::SystemTimeToString(entry.GetTimestamp());
	string warninginfo_utf8;
	Project::Tools::WideCharToUtf8(entry.GetWarningInfo(), warninginfo_utf8);

	string strtUser_utf8;
	Project::Tools::WideCharToUtf8(strUser, strtUser_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	std::ostringstream sqlstreamUpdate;
	sqlstreamUpdate << "update "
		<< GetDBName()
		<<"."
		<< tablename_warningrecord 
		<< " set confirmed = " << nResolveType <<",  confirmeduser = '" << strtUser_utf8 << "'"
		<< " where info = '" << warninginfo_utf8 << "'  and time = '" << strtime <<"'";

	string sqlstatement = sqlstreamUpdate.str();

	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::ConfirmWarning( const CWarningEntry& entry, wstring strUser, int nResolveType,bool bIsEnglishMode )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strtime = Project::Tools::SystemTimeToString(entry.GetTimestamp());
	string warninginfo_utf8;
	Project::Tools::WideCharToUtf8(entry.GetWarningInfo(), warninginfo_utf8);

	string warningpoint_utf8;
	Project::Tools::WideCharToUtf8(entry.m_strBindPointName, warningpoint_utf8);

	SYSTEMTIME time;
	GetLocalTime(&time);
	string strConfirmTime = Project::Tools::SystemTimeToString(time);
	string strActionType = GetWarningAction(nResolveType,bIsEnglishMode); 
	
	string strtUser_utf8;
	Project::Tools::WideCharToUtf8(strUser, strtUser_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	std::ostringstream sqlstreamUpdate;

	//处理报警状态
	sqlstreamUpdate << "update "
		<< GetDBName()
		<<"."
		<< tablename_warningrecord 
		<< " set confirmed = " << nResolveType <<",  confirmeduser = '"  << strtUser_utf8 << "'"
		<< " where info = '" << warninginfo_utf8 << "'  and time = '" << strtime <<"'";

	string sqlstatement = sqlstreamUpdate.str();
	bool bResult = Execute(sqlstatement);

	//插入报警操作  time,info,point,user,operation
	sqlstreamUpdate.str("");
	sqlstreamUpdate << "insert into " << GetDBName() << "." << tablename_warningoperation 
		<< " (unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
		<< strConfirmTime << "','" << warninginfo_utf8 << "','" << warningpoint_utf8 << "','"
		<< strtUser_utf8 << "','" << strActionType << "');";
	sqlstatement = sqlstreamUpdate.str();
	bResult = Execute(sqlstatement) && bResult;
	return bResult;
}


bool  Beopdatalink::CCommonDBAccess::ConfirmWarningAll(const wstring &strUser)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strtUser_utf8;
	Project::Tools::WideCharToUtf8(strUser, strtUser_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	std::ostringstream sqlstreamUpdate;
	sqlstreamUpdate << "update "
		<< GetDBName()
		<<"."
		<< tablename_warningrecord 
		<< " set confirmed = 1,  confirmeduser = '" << strtUser_utf8 << "'"
		<< " where confirmed = 0";

	string sqlstatement = sqlstreamUpdate.str();

	return Execute(sqlstatement);

}

bool	Beopdatalink::CCommonDBAccess::ModifyWarningConfig(const CWarningConfigItem & configItem)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);


	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	string strPointName_utf8, strContentHH_utf8, strContentH_utf8, strContentL_utf8, strContentLL_utf8;
	Project::Tools::WideCharToUtf8(configItem.m_strPointName, strPointName_utf8);
	Project::Tools::WideCharToUtf8(configItem.m_strWarningContent_HH, strContentHH_utf8);
	Project::Tools::WideCharToUtf8(configItem.m_strWarningContent_H, strContentH_utf8);
	Project::Tools::WideCharToUtf8(configItem.m_strWarningContent_L, strContentL_utf8);
	Project::Tools::WideCharToUtf8(configItem.m_strWarningContent_LL, strContentLL_utf8);

	string strBoolContent_utf8;
	Project::Tools::WideCharToUtf8(configItem.m_strWarningContent, strBoolContent_utf8);
	string strSystem_utf8;
	Project::Tools::WideCharToUtf8(configItem.m_strUnitProperty01, strSystem_utf8);

	sqlstream << "update "
		<< GetDBName()
		<<"."
		<< tablename_warningconfig 
		<< " set HHEnable = "
		<< (configItem.m_bEnableHH?"1":"0") << ","  
		<< " HEnable = "
		<< (configItem.m_bEnableH?"1":"0") << ","  
		<< " LEnable = "
		<< (configItem.m_bEnableL?"1":"0") << ","  
		<< " LLEnable = "
		<< (configItem.m_bEnableLL?"1":"0") << ","  
		<< " HHLimit = "
		<< configItem.m_fLimitHH << ","  
		<< " HLimit = "
		<< configItem.m_fLimitH << ","  
		<< " LLimit = "
		<< configItem.m_fLimitL << ","  
		<< " LLLimit = "
		<< configItem.m_fLimitLL << ","
		<< " HHwarninginfo = '"
		 <<  strContentHH_utf8  << "',"
		<< " Hwarninginfo = '"
		<<  strContentH_utf8  << "',"
		<< " Lwarninginfo = '"
		<<  strContentL_utf8  << "',"
		<< " LLwarninginfo = '"
		<<  strContentLL_utf8 << "',"
		<< " boolwarninginfo = '"
		<<  strBoolContent_utf8 << "',"
		<< "unitproperty01 = '"
		<<  strSystem_utf8
		<< "' where pointname = '"
		<< strPointName_utf8 <<"'";


	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool	Beopdatalink::CCommonDBAccess::AddWarningConfig(const CWarningConfigItem & configItem)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);


	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

		string strPointName_utf8, strContentHH_utf8, strContentH_utf8, strContentL_utf8, strContentLL_utf8;
		Project::Tools::WideCharToUtf8(configItem.m_strPointName, strPointName_utf8);
		Project::Tools::WideCharToUtf8(configItem.m_strWarningContent_HH, strContentHH_utf8);
		Project::Tools::WideCharToUtf8(configItem.m_strWarningContent_H, strContentH_utf8);
		Project::Tools::WideCharToUtf8(configItem.m_strWarningContent_L, strContentL_utf8);
		Project::Tools::WideCharToUtf8(configItem.m_strWarningContent_LL, strContentLL_utf8);

		string strBoolContent_utf8;
		Project::Tools::WideCharToUtf8(configItem.m_strWarningContent, strBoolContent_utf8);

		string strSystem_utf8;
		Project::Tools::WideCharToUtf8(configItem.m_strUnitProperty01, strSystem_utf8);

		char szWarningType[10] = {0};
		sprintf_s(szWarningType,sizeof(szWarningType)/sizeof(char),"%d",configItem.m_nWarningType);

		sqlstream << "insert into "
			<< GetDBName()
			<<"."
			<< tablename_warningconfig 
			<< " value"
			<< "(" 
			<< (configItem.m_bEnableHH?"1":"0") << ","  
			<< (configItem.m_bEnableH?"1":"0") << ","  
			<< (configItem.m_bEnableL?"1":"0") << ","  
			<< (configItem.m_bEnableLL?"1":"0") << ","  
			<< configItem.m_fLimitHH << ","  
			<< configItem.m_fLimitH << ","  
			<< configItem.m_fLimitL << ","  
			<< configItem.m_fLimitLL << ","
			<< "'" <<  strPointName_utf8 
			<< "', '" <<  strContentHH_utf8 
			<< "', '" <<  strContentH_utf8 
			<< "', '" <<  strContentL_utf8 
			<< "', '" <<  strContentLL_utf8 << "',"
			<< szWarningType<< ",'"
			<< strBoolContent_utf8 <<"',"
			<< configItem.m_nWarningLevel
			<<",'"
			<<strSystem_utf8
			<< "','','','','')";


	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool	Beopdatalink::CCommonDBAccess::DeleteWarningConfig(wstring strConfigPointName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	std::ostringstream sqlstream;

	string strPointName_utf8;
	Project::Tools::WideCharToUtf8(strConfigPointName, strPointName_utf8);


	sqlstream << "delete from " 
			<< GetDBName() 
			<< ".warning_config"
			<<" where pointname = '"
			<< strPointName_utf8
			<<"'";


	return Execute(sqlstream.str());
}


bool Beopdatalink::CCommonDBAccess::ReadWarningConfig(vector<CWarningConfigItem>& configList)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_warningconfig
		<< "  order by pointname";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length)
	{
		return false;
	}
	configList.reserve(length);
	MYSQL_ROW  row = NULL;

	CWarningConfigItem wcItem;

	while(row = FetchRow(result) )
	{
		wcItem.m_bEnableHH = ATOI(row[0])==1;
		wcItem.m_bEnableH = ATOI(row[1])==1;
		wcItem.m_bEnableL = ATOI(row[2])==1;
		wcItem.m_bEnableLL = ATOI(row[3])==1;
		wcItem.m_fLimitHH = ATOF(row[4]);
		wcItem.m_fLimitH = ATOF(row[5]);
		wcItem.m_fLimitL = ATOF(row[6]);
		wcItem.m_fLimitLL = ATOF(row[7]);
		wcItem.m_strPointName = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.m_strWarningContent_HH = Project::Tools::UTF8ToWideChar(row[9]);
		wcItem.m_strWarningContent_H = Project::Tools::UTF8ToWideChar(row[10]);
		wcItem.m_strWarningContent_L = Project::Tools::UTF8ToWideChar(row[11]);
		wcItem.m_strWarningContent_LL = Project::Tools::UTF8ToWideChar(row[12]);
		wcItem.m_nWarningType = ATOI(row[13]);
		wcItem.m_strWarningContent = Project::Tools::UTF8ToWideChar(row[14]);
		wcItem.m_nWarningLevel = ATOI(row[15]);

		wstring strUnit01 = Project::Tools::UTF8ToWideChar(row[16]);
		if(strUnit01==L"" || strUnit01==L"0")
		{
			CString strSysIni;
			GetSysPath(strSysIni);
			strSysIni += L"\\factory.ini";
			int nLanguage = GetPrivateProfileInt(L"beopobserver", L"language", 0, strSysIni);
			if (nLanguage == 1)
			{
				strUnit01 = g_strUnit01DefaultEng;
			}
			else
			{
				strUnit01 = g_strUnit01DefaultChn;
			}
		}		
		wcItem.m_strUnitProperty01 = strUnit01;
		wcItem.m_strUnitProperty02 = Project::Tools::UTF8ToWideChar(row[17]);
		wcItem.m_strUnitProperty03 = Project::Tools::UTF8ToWideChar(row[18]);
		wcItem.m_strUnitProperty04 = Project::Tools::UTF8ToWideChar(row[19]);
		wcItem.m_strUnitProperty05 = Project::Tools::UTF8ToWideChar(row[20]);
		configList.push_back(wcItem);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWarning( vector<CWarningEntry>& resultlist, const SYSTEMTIME& start, const SYSTEMTIME& end , bool bUnconfirmed)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string str_start = Project::Tools::SystemTimeToString(start);
	string str_end = Project::Tools::SystemTimeToString(end);

	std::ostringstream sqlstream;

	if(bUnconfirmed)
	{
		sqlstream << "select * from " 
			<< GetDBName() 
			<< "."
			<< tablename_warningrecord
			<< " where  confirmed = 0  order by endtime desc limit 0, 500;";

	}
	else
	{
		sqlstream << "select * from " 
			<< GetDBName() 
			<< "."
			<< tablename_warningrecord
			<< " where (time >= '" << str_start << "' and time <= '" << str_end << "') or (endtime >= '" << str_start << "' and endtime <= '" << str_end << "') order by endtime desc limit 0, 500;";
	}

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	resultlist.reserve(length);
	MYSQL_ROW  row = NULL;

	CWarningEntry entry;
	while(row = FetchRow(result) ){
		SYSTEMTIME stStart;
		Project::Tools::String_To_SysTime(row[0], stStart);
		entry.SetTimestamp(stStart);
		entry.SetWarningCode(ATOI(row[1]));
		wstring warninginfo = Project::Tools::UTF8ToWideChar(row[2]);
		entry.SetWarningInfo(warninginfo);
		entry.SetWarningLevel(ATOI(row[3]));

		SYSTEMTIME stEnd;
		Project::Tools::String_To_SysTime(row[4], stEnd);
		entry.SetEndTimestamp(stEnd);
		bool bConfirmed = true;
		if(ATOI(row[5])==0)
			bConfirmed = false;
		entry.SetConfirmed(bConfirmed);
		wstring strConfirmedUser = Project::Tools::UTF8ToWideChar(row[6]);
		entry.SetConfirmedUser(strConfirmedUser);

		entry.m_strBindPointName = Project::Tools::UTF8ToWideChar(row[7]);


		resultlist.push_back(entry);
	}

	return true;
}


bool Beopdatalink::CCommonDBAccess::DeleteWarning()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_warningrecord;

	return Execute(sqlstream.str());
}

bool Beopdatalink::CCommonDBAccess::SetStatus( const wstring& statustype, int value )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	// first, check whether
	string name = Project::Tools::WideCharToAnsi(statustype.c_str());
	std::ostringstream sqlstream;
	// try to insert the entry.

	sqlstream << "insert into  "
		<< GetDBName() 
		<< "."
		<< tablename_datacom_status 
		<< " value " << "('" << name << "'," << value << ")";

	string sqlstatement = sqlstream.str();
	bool bresult =  Execute(sqlstatement);
	if (!bresult)
	{
		sqlstream.str("");
		sqlstream << "update  "
			<< GetDBName()
			<< "."
			<< tablename_datacom_status
			<< " set status=" << value
			<< " where typename='" << name << "'";
		sqlstatement = sqlstream.str();
		return Execute(sqlstatement);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::GetStatus( vector< pair<wstring, int> >& statuslist )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_datacom_status;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	statuslist.reserve(length);
	MYSQL_ROW  row = NULL;

	while(row = FetchRow(result) )
	{
		wstring statusname = Project::Tools::AnsiToWideChar(row[0]);
		int value = ATOI(row[1]);

		statuslist.push_back(make_pair(statusname, value));
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::SetPLCConnectionStatus( bool bconnected )
{
	return SetStatus(_T("PLC Connection"), bconnected ? 1 : 0);
}

bool Beopdatalink::CCommonDBAccess::GetPLCConnectionStatus()
{
	vector< pair<wstring, int> > statuslist;
	GetStatus(statuslist);
	for (unsigned int i = 0; i < statuslist.size(); i++)
	{
		const pair<wstring, int>& entry = statuslist[i];
		if (entry.first == _T("PLC Connection")){
			return entry.second != 0;
		}
	}

	return false;
}

bool Beopdatalink::CCommonDBAccess::GetModbusConnectionStatus()
{
	return true;
}

int Beopdatalink::CCommonDBAccess::GetSystemActionStatus()
{
	vector< pair<wstring, int> > statuslist;
	GetStatus(statuslist);
	for (unsigned int i = 0; i < statuslist.size(); i++)
	{
		const pair<wstring, int>& entry = statuslist[i];
		if (entry.first == _T("systemaction"))
		{
			return entry.second;
		}
	}

	return 0;
}

bool Beopdatalink::CCommonDBAccess::InsertOperationRecord(const SYSTEMTIME& st, const wstring& remark, const wstring& wstrUser)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	//string actioninfo_utf8;
	//Project::Tools::WideCharToUtf8(actioninfo, actioninfo_utf8);
	//string actionname_utf8;
	//Project::Tools::WideCharToUtf8(actionname, actionname_utf8);

	//SetConnectionCharacterSet(MYSQL_UTF8);
	////generate the insert string
	//std::ostringstream sqlstream;
	//sqlstream << "insert into "
	//	<< GetDBName()
	//	<<"."
	//	<< tablename_actioninfo 
	//	<< " value"
	//	<< "('" << actioninfo_utf8 << "','" << actionname_utf8 << "')"; 

	//string sqlstatement = sqlstream.str();

	//return Execute(sqlstatement);


	SetConnectionCharacterSet(MYSQL_UTF8);
	string strRemark;
	Project::Tools::WideCharToUtf8(remark.c_str(),strRemark);
	string strUser;
	Project::Tools::WideCharToUtf8(wstrUser.c_str(),strUser);

	std::ostringstream sqlstream;
	sqlstream	<< "insert into "
		<< GetDBName() << "."<<tablename_operationrecord <<" values('"				
		<< Project::Tools::SystemTimeToString(st) << "'"
		<< "," <<"'"<< strUser << "'"
		<< "," <<"'"<< strRemark << "'"
		<< ")";

	string sql = sqlstream.str();
	bool bResult = Execute(sql);
	//if (!bResult)
	//{
	//	LogDBError(sql);
	//}

	return bResult;
}

bool Beopdatalink::CCommonDBAccess::ReadAllOperationRecord(VEC_OPT_RECORD&  vecOperation, const COleDateTime& timeStart, const COleDateTime& timeStop)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet();
	bool ifok = false;

	//const int hourMax = 24;
	wstring wstrStart, wstrStop;
	string strStart, strStop;
	Project::Tools::OleTime_To_String(timeStart, wstrStart);
	Project::Tools::OleTime_To_String(timeStop, wstrStop);
	strStart = UtilString::ConvertWideCharToMultiByte(wstrStart);
	strStop = UtilString::ConvertWideCharToMultiByte(wstrStop);

	std::ostringstream sqlstream;
	//SELECT * FROM operation_record where recordtime >= '2011-05-30 00:00:00' and recordtime <= '2011-05-30 14:00:00';
	sqlstream << "select * from " << tablename_operationrecord <<" where recordtime >= '"<< strStart.c_str() <<"' and recordtime <= '"<< strStop.c_str() << "'";

	string sql = sqlstream.str();
	COperationRecords  tempOpera;
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
			//LogDBError(sql);
		}
	}
	//else
	//{
	//    ifok = false;
	//    LogDBError(sql);
	//}

	return ifok;
}

bool Beopdatalink::CCommonDBAccess::DeleteOperationRecord()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_operationrecord;

	return Execute(sqlstream.str());
}

bool Beopdatalink::CCommonDBAccess::InsertActionInfo(const gActinInfo_Record& record )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	string actioninfo_utf8;
	Project::Tools::WideCharToUtf8(record.info, actioninfo_utf8);
	string actionname_utf8;
	Project::Tools::WideCharToUtf8(record.actionname, actionname_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_actioninfo 
		<< " value"
		<< "('" << actioninfo_utf8 << "','" << actionname_utf8 << "',"<< record.status<< ")"; 

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::ReadAllActionInfo( vector< gActinInfo_Record >& actioninfolist )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	gActinInfo_Record gRecord;
	std::ostringstream sqlstream;
	sqlstream << "select * from "
		<< GetDBName()
		<< "."
		<< tablename_actioninfo;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return true;
	}

	actioninfolist.reserve(length);
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) ){
		gRecord.info = Project::Tools::UTF8ToWideChar(row[0]);
		gRecord.actionname = Project::Tools::UTF8ToWideChar(row[1]);

		int row2 = atoi(row[2]);
		switch(row2)
		{
		case 0:
			gRecord.status = ready;
			break;
		case 1:
			gRecord.status = running;
			break;
		case 2:
			gRecord.status = success;
			break;
		case 3:
			gRecord.status = fail;
			break;
		default:
			gRecord.status = ready;
			break;

		}

		actioninfolist.push_back(gRecord);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::DeleteAllActionInfo()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_actioninfo;

	return Execute(sqlstream.str());
}

bool Beopdatalink::CCommonDBAccess::ModifyActionStatus(const wstring& info, int nStatus)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string actioninfo_utf8;
	Project::Tools::WideCharToUtf8(info, actioninfo_utf8);

	string sqlstatement;
	std::ostringstream sqlstream;

	sqlstream.str("");
	sqlstream << "update  "
		<< GetDBName()
		<< "."
		<< tablename_actioninfo
		<< " set status=" << nStatus
		<< " where actioninfo='" << actioninfo_utf8 << "'";
	sqlstatement = sqlstream.str();
	bool result = Execute(sqlstatement);
	return result;	
}

bool Beopdatalink::CCommonDBAccess::InsertAction( const wstring& systemaction, const wstring& chillerlist )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	string action_utf8;
	string strchillerlist;
	Project::Tools::WideCharToUtf8(systemaction, action_utf8);
	Project::Tools::WideCharToUtf8(chillerlist, strchillerlist);
	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_action
		<< " value"
		<< "('" << action_utf8 << "',"
		<< "0,'"
		<< strchillerlist << "')";

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::ReadAction( wstring& systemaction, wstring& chillerlist )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "select * from "
		<< GetDBName()
		<< "."
		<< tablename_action;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return true;
	}

	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) ){
		systemaction = Project::Tools::UTF8ToWideChar(row[0]);
		chillerlist = Project::Tools::UTF8ToWideChar(row[2]);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::DeleteAction()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_action;

	return Execute(sqlstream.str());
}


// write the force close flag.
bool Beopdatalink::CCommonDBAccess::SetForceClose()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "update table "
		<< GetDBName() 
		<< "."
		<< tablename_action
		<< " set forceclose=" << 1;

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);

	return true;
}


// read force close flag
bool Beopdatalink::CCommonDBAccess::ReadForceClose()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	bool bresult = false;
	std::ostringstream sqlstream;
	sqlstream << "select forceclose from "
		<< GetDBName()
		<< "."
		<< tablename_action;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}

	MYSQL_ROW  row = FetchRow(result);
	if (row != NULL){
		bresult = (ATOI(row[0]) == 0 ? false : true);
	}

	return bresult;
}

// insert history reacord data.
// 数据记录会分表，按照一月一表的方式组合。
// 插入历史记录表时，需先判断当前表是否存在，如果不存在，则创建。
// 

bool Beopdatalink::CCommonDBAccess::ReadHistoryDataInTable( vector<CRealTimeDataEntry>& resultlist, const COleDateTime& start, const COleDateTime& end ,const wstring &strTableName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strTableName_ansi;
	Project::Tools::WideCharToUtf8(strTableName, strTableName_ansi);


	wstring wstr_start, wstr_end;
	string str_start, str_end;
	Project::Tools::OleTime_To_String(start, wstr_start);
	Project::Tools::OleTime_To_String(end, wstr_end);
	Project::Tools::WideCharToUtf8(wstr_start, str_start);
	Project::Tools::WideCharToUtf8(wstr_end, str_end);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< strTableName_ansi
		<< " where time >= '" << str_start << "' and time <= '" << str_end << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	resultlist.reserve(length);
	MYSQL_ROW  row = NULL;
	CRealTimeDataEntry entry;

	while(row = FetchRow(result) )
	{   
		SYSTEMTIME sttime;
		Project::Tools::String_To_SysTime(row[0], sttime);
		entry.SetTimestamp(sttime);
		entry.SetName(row[1]);

		wstring wstrValue;
		Project::Tools::UTF8ToWideChar(row[2], wstrValue);
		entry.SetValue(wstrValue);

		resultlist.push_back(entry);
	}
	FreeReturnResult(result);
	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadHistoryDataInTable( vector<CRealTimeDataEntry>& resultlist, const COleDateTime& start, const COleDateTime& end,const wstring &strPointName, const wstring &strTableName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strTableName_ansi;
	Project::Tools::WideCharToUtf8(strTableName, strTableName_ansi);

	string strPointName_ansi;
	Project::Tools::WideCharToUtf8( strPointName, strPointName_ansi);

	wstring wstr_start, wstr_end;
	string str_start, str_end;
	Project::Tools::OleTime_To_String(start, wstr_start);
	Project::Tools::OleTime_To_String(end, wstr_end);
	Project::Tools::WideCharToUtf8(wstr_start, str_start);
	Project::Tools::WideCharToUtf8(wstr_end, str_end);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< strTableName_ansi
		<< " where time >= '" << str_start << "' and time <= '" << str_end << "' and pointname = '" << strPointName_ansi << "'";
		//<< " where  pointname = '" << strPointName_ansi << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	//resultlist.reserve(length);
	MYSQL_ROW  row = NULL;
	CRealTimeDataEntry entry;

	while(row = FetchRow(result) )
	{   
		SYSTEMTIME sttime;
		Project::Tools::String_To_SysTime(row[0], sttime);
		entry.SetTimestamp(sttime);
		entry.SetName(row[1]);

		wstring wstrValue;
		Project::Tools::UTF8ToWideChar(row[2], wstrValue);
		entry.SetValue(wstrValue);

		resultlist.push_back(entry);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWatchingPoinitList(std::vector<wstring> &strWatchPointNameList)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	strWatchPointNameList.clear();

	string sql = "select * from unit09";

	MYSQL_RES* result = QueryAndReturnResult(sql.c_str());
	if (result == NULL)
	{
		return false;
	}

	MYSQL_ROW row = NULL;
	while(row = FetchRow(result))
	{
		wstring wstrShortName = Project::Tools::UTF8ToWideChar(row[0]);
		
		strWatchPointNameList.push_back(wstrShortName);
	}

	FreeReturnResult(result);
	return true;
}

bool Beopdatalink::CCommonDBAccess::SetPointWatching(wstring wstrPointName, bool bWatching)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	std::ostringstream sqlstream;

	string strPointName;
	Project::Tools::WideCharToUtf8(wstrPointName, strPointName);

	
	sqlstream.str("");

	if(bWatching)
	{
		sqlstream << "insert into "
			<< GetDBName()
			<<".unit09 "
			<< "values('"
			<< strPointName << "','','','','','','','','','','','','','','')"; 
	}
	else
	{
		sqlstream << "delete from "
			<< GetDBName()
			<<".unit09 "
			<< "where unitproperty01 = '"
			<< strPointName << "'"; 

	}

	string sqlstatement = sqlstream.str();
	bool bSuccess = Execute(sqlstatement);

	return bSuccess;
}

bool Beopdatalink::CCommonDBAccess::ClearLogicBadRecord()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	std::ostringstream sqlstream;
	sqlstream << "delete from unit01 where unitproperty01 = 'system_bad_logic_info'";

	return Execute(sqlstream.str());
}

bool    Beopdatalink::CCommonDBAccess::ReadServerPointsList(vector<DataPointEntry> &vecPtList)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	vecPtList.clear();

	string sql = "select * from unit04 order by unitproperty02";

	MYSQL_RES* result = QueryAndReturnResult(sql.c_str());
	if (result == NULL)
	{
		return false;
	}

	MYSQL_ROW row = NULL;
	while(row = FetchRow(result))
	{
		wstring wstrID = Project::Tools::UTF8ToWideChar(row[0]);
		wstring wstrShortName = Project::Tools::UTF8ToWideChar(row[1]);
		wstring wstrSource = Project::Tools::UTF8ToWideChar(row[2]);
		string strType = Project::Tools::Utf8ToMultiByte(row[3]);
		wstring wstrDescription = Project::Tools::UTF8ToWideChar(row[4]);
		DataPointEntry ptEntry;
		ptEntry.SetPointIndex(_wtoi(wstrID.c_str()));
		ptEntry.SetShortName(wstrShortName);
		ptEntry.SetType(strType);
		ptEntry.SetSourceType(wstrSource);
		ptEntry.SetDescription(wstrDescription);

		vecPtList.push_back(ptEntry);
	}

	FreeReturnResult(result);
	return true;
}


bool Beopdatalink::CCommonDBAccess::InsertControlParamRecord( const gControlParamRecord& record)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;	

	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_controlparamrecord;

	bool bresult =  Execute(sqlstream.str());

	if(bresult)
	{
		sqlstream.str("");
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
			//<< record.SecChilledPumpCombination << "','"

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
			<< record.dCWPumpGroup_Power <<  ",'" 
			<< record.SecChilledPumpCombination << "')";

		string sql = sqlstream.str();
		bool bresult = Execute(sql);
		//if (!bresult)
		//{
		//	LogDBError(sql);
		//}

		return bresult;
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadControlParamRunRecord(gControlParamRecord & mControlParamRecord)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	//char sql[256];
	//memset(sql, 0x00, sizeof(sql));

	////string from = SystemTimeToString(start);
	////string to = SystemTimeToString(end);

	//sprintf_s(sql, sizeof(sql), "select * from controlparam_record ;");
	////where time >= '%s' and time <= '%s' ;" ,\
	////	from.c_str(), to.c_str());

	//if (QueryAndStoreResult(sql))
	//{
	//	return m_pQuery;
	//}
	////else
	////	LogDBError(sql);
	//return NULL;
	string sql = "select * from controlparam_record";

	MYSQL_RES* result = QueryAndReturnResult(sql.c_str());
	if (result == NULL)
	{
		return false;
	}

	MYSQL_ROW row = NULL;
	if(row = FetchRow(result))
	{
		SYSTEMTIME sttime;
		Project::Tools::String_To_SysTime(row[0], sttime);

		mControlParamRecord.tmTime = sttime;
		mControlParamRecord.dT = ATOF(row[1]);
		mControlParamRecord.dFai = ATOF(row[2]);
		mControlParamRecord.dTwb = ATOF(row[3]);

		string temp;
		temp = row[4];
		strcpy_s(mControlParamRecord.LEWT_Setpoint, sizeof(mControlParamRecord.LEWT_Setpoint), temp.c_str());

		//mControlParamRecord.LEWT_Setpoint = row[4];

		mControlParamRecord.dLCWT_Setpoint = ATOF(row[5]);
		//mControlParamRecord.ChillerCombination = row[6];
		//mControlParamRecord.CTCombination = row[7];
		//mControlParamRecord.ChilledPumpCombination = row[8];
		//mControlParamRecord.CoolingPumpCombination = row[9];
		temp = row[6];
		strcpy_s(mControlParamRecord.ChillerCombination, sizeof(mControlParamRecord.ChillerCombination), temp.c_str());

		temp = row[7];
		strcpy_s(mControlParamRecord.CTCombination, sizeof(mControlParamRecord.CTCombination), temp.c_str());

		temp = row[8];
		strcpy_s(mControlParamRecord.ChilledPumpCombination, sizeof(mControlParamRecord.ChilledPumpCombination), temp.c_str());

		temp = row[9];
		strcpy_s(mControlParamRecord.CoolingPumpCombination, sizeof(mControlParamRecord.CoolingPumpCombination), temp.c_str());

		mControlParamRecord.dLoad_Actual =  ATOF(row[10]);
		mControlParamRecord.dLoad_UseCalc =  ATOF(row[11]);
		mControlParamRecord.dChill_Flow =  ATOF(row[12]);
		mControlParamRecord.dCool_Flow =  ATOF(row[13]);
		mControlParamRecord.dDTChill =  ATOF(row[14]);
		mControlParamRecord.dDTCool =  ATOF(row[15]);
		mControlParamRecord.dChiller_K =  ATOF(row[16]);
		mControlParamRecord.dRoomCOP =  ATOF(row[17]);
		mControlParamRecord.dCHGroup_Power =  ATOF(row[18]);
		mControlParamRecord.dCTGroup_Power =  ATOF(row[19]);
		mControlParamRecord.dPriCHWPumpGroup_Power =  ATOF(row[20]);
		mControlParamRecord.dSecCHWPumpGroup_Power =  ATOF(row[21]);
		mControlParamRecord.dCWPumpGroup_Power =  ATOF(row[22]);

		temp = row[23];
		strcpy_s(mControlParamRecord.SecChilledPumpCombination, sizeof(mControlParamRecord.ChilledPumpCombination), temp.c_str());

		FreeReturnResult(result);
		return true;
	}

	return false;
}



bool Beopdatalink::CCommonDBAccess::InsertLog(SYSTEMTIME st,  const wstring& loginfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	char buff[64];

	sprintf_s(buff, "log_%d_%02d_%02d", st.wYear, st.wMonth, st.wDay);
	string newtablename = buff;
	std::ostringstream sqlstream;
	sqlstream << "create table if not exists " << newtablename <<  sql_createlog;

	bool bresult = Execute(sqlstream.str());
	if (!bresult)
	{
		return false;
	}

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	string loginfo_utf8, strtime;
	sqlstream.str("");
	Project::Tools::WideCharToUtf8(loginfo, loginfo_utf8);
	strtime = Project::Tools::SystemTimeToString(st);
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< newtablename 
		<< " value"
		<< "('" << strtime << "','" << loginfo_utf8 << "')"; 

	string sqlstatement = sqlstream.str();

	wstring wstr;
	Project::Tools::UTF8ToWideChar(sqlstatement, wstr);

	OutputDebugString( wstr.c_str() );

	/*string sqlstatement = "insert into";
	sqlstatement += GetDBName();
	sqlstatement += ".";
	sqlstatement += tablename_log;
	sqlstatement += " value ('";
	sqlstatement += strtime;
	sqlstatement += "','";
	sqlstatement += loginfo_utf8;
	sqlstatement += "');";*/
	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::InsertLog( const wstring& loginfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SYSTEMTIME st;
	GetLocalTime(&st);
	char buff[64];

	sprintf_s(buff, "log_%d_%02d_%02d", st.wYear, st.wMonth, st.wDay);
	string newtablename = buff;
	std::ostringstream sqlstream;
	sqlstream << "create table if not exists " << newtablename <<  sql_createlog;

	bool bresult = Execute(sqlstream.str());
	if (!bresult)
	{
		return false;
	}

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	string loginfo_utf8, strtime;
	Project::Tools::WideCharToUtf8(loginfo, loginfo_utf8);
	strtime = Project::Tools::SystemTimeToString(st);
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< newtablename 
		<< " value"
		<< "('" << strtime << "','" << loginfo_utf8 << "')"; 

	string sqlstatement = sqlstream.str();

	/*string sqlstatement = "insert into";
	sqlstatement += GetDBName();
	sqlstatement += ".";
	sqlstatement += tablename_log;
	sqlstatement += " value ('";
	sqlstatement += strtime;
	sqlstatement += "','";
	sqlstatement += loginfo_utf8;
	sqlstatement += "');";*/
	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::ReadLog( vector< pair<wstring,wstring> >& loglist, const SYSTEMTIME& start)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);

	string str_start = Project::Tools::SystemTimeToString(start);

	char buff[64];
	sprintf_s(buff, "log_%d_%02d_%02d", start.wYear, start.wMonth, start.wDay);
	string newtablename = buff;


	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< newtablename
		<< " where time >= '" << str_start << "' order by time";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	loglist.reserve(length);
	MYSQL_ROW  row = NULL;

	while(row = FetchRow(result) ){
		wstring logtime = Project::Tools::UTF8ToWideChar(row[0]);
		string str_mb = Project::Tools::Utf8ToMultiByte(row[1]);
		wstring loginfo = Project::Tools::AnsiToWideChar(str_mb.data());
		loglist.push_back(make_pair(logtime,loginfo));
	}

	return true;
}


bool    Beopdatalink::CCommonDBAccess::DeleteLog(const SYSTEMTIME tEnd)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	std::ostringstream sqlstream;
	string str_end = Project::Tools::SystemTimeToString(tEnd);
	sqlstream << "delete from "
		<< GetDBName() 
		<<"."
		<<tablename_log
		<< " where time <='"
		<< str_end
		<<"'";

	return Execute(sqlstream.str());
}

bool Beopdatalink::CCommonDBAccess::ExportLog( const SYSTEMTIME& start, const SYSTEMTIME& end, const wstring& filepath )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	string filepath_ansi = Project::Tools::WideCharToAnsi(filepath.c_str());
	string str_start = Project::Tools::SystemTimeToString(start);
	string str_end = Project::Tools::SystemTimeToString(end);

	std::ostringstream sqlstream;
	sqlstream << "select * from "
		<< GetDBName() 
		<<"."
		<<tablename_log
		<< " where time >='"
		<< str_start
		<< "' and time <='"
		<< str_end
		<<"'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());

	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}

	MYSQL_ROW  row = NULL;

	vector<wstring> vcline;
	while(row = FetchRow(result) ){
		wstring logtime = Project::Tools::UTF8ToWideChar(row[0]);
		wstring loginfo = Project::Tools::UTF8ToWideChar(row[1]);

		wstring strtemp = logtime + _T("    ")+ loginfo;
		vcline.push_back(strtemp);
	}
	//写文件
	{
		assert(filepath.size()>0);

		FILE* fd = NULL;
		const errno_t rs = _wfopen_s(&fd, filepath.c_str(), L"wt,ccs=UNICODE");
		assert(0 == rs);
		assert(fd);
		for (UINT i=0;i<vcline.size();++i)
		{
			fwprintf_s(fd, L"%s\n",vcline[i].c_str());
		}
		fclose(fd);
	}

	return true;
}

bool     Beopdatalink::CCommonDBAccess::InsertLogicParameters(wstring strThreadName, wstring strLogicName, wstring strSetType, wstring strVariableName, wstring strLinkName, wstring strLinkType, wstring strCondition)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	// first, check whether

	std::ostringstream sqlstream;
	// try to insert the entry.

	string strThreadName_utf8;
	Project::Tools::WideCharToUtf8(strThreadName, strThreadName_utf8);
	string strLogicName_utf8;
	Project::Tools::WideCharToUtf8(strLogicName, strLogicName_utf8);
	string strSetType_utf8;
	Project::Tools::WideCharToUtf8(strSetType, strSetType_utf8);
	string strVariableName_utf8;
	Project::Tools::WideCharToUtf8(strVariableName, strVariableName_utf8);
	string strLinkName_utf8;
	Project::Tools::WideCharToUtf8(strLinkName, strLinkName_utf8);
	string strLinkType_utf8;
	Project::Tools::WideCharToUtf8(strLinkType, strLinkType_utf8);
	string strCondition_utf8;
	Project::Tools::WideCharToUtf8(strCondition, strCondition_utf8);

	sqlstream.str("");
	sqlstream << "insert into  "
		<< GetDBName() 
		<< ".unit02"
		<< " value " << "('" <<strThreadName_utf8 << "', '" << strLogicName_utf8 << "', '" << strSetType_utf8<< "', '" << strVariableName_utf8
		<< "', '" << strLinkName_utf8<< "', '" << strLinkType_utf8<< "', '" << strCondition_utf8<< "','','','','','','','','')";

	string sqlstatement = sqlstream.str();
	bool bresult =  Execute(sqlstatement);		


	return bresult;
}


bool    Beopdatalink::CCommonDBAccess::WriteCoreDebugItemValue(const wstring &strItemName, const wstring &strItemName2, const wstring &strItemName3, const wstring &strItemValue)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	// first, check whether

	std::ostringstream sqlstream;
	// try to insert the entry.

	string strItemName_utf8, strItemName2_utf8, strItemName3_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);
	Project::Tools::WideCharToUtf8(strItemName2, strItemName2_utf8);
	Project::Tools::WideCharToUtf8(strItemName3, strItemName3_utf8);

	string strItemValue_utf8;
	Project::Tools::WideCharToUtf8(strItemValue, strItemValue_utf8);

	sqlstream << "select * from " 
		<< GetDBName() 
		<< ".unit01 where unitproperty01 = '"
		<< strItemName_utf8 << "' and unitproperty02 = '"
		<< strItemName2_utf8 << "' and unitproperty03 = '"
		<< strItemName3_utf8 << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length)
	{
		sqlstream.str("");
		sqlstream << "insert into  "
			<< GetDBName() 
			<< ".unit01"
			<< " value " << "('" <<strItemName_utf8 << "', '" <<strItemName2_utf8 << "', '"<<strItemName3_utf8 << "', '"<< strItemValue_utf8 << "','','','','','','','','','','','')";

		string sqlstatement = sqlstream.str();
		bool bresult =  Execute(sqlstatement);		
	}
	else
	{
		sqlstream.str("");
		sqlstream << "update  "
			<< GetDBName()
			<< ".unit01"
			<< " set unitproperty04 = '" << strItemValue_utf8
			<< "' where unitproperty01 ='" << strItemName_utf8
			<< "' and unitproperty02 = '" << strItemName2_utf8 
			<< "' and unitproperty03 = '" << strItemName3_utf8 << "'";
		string sqlstatement = sqlstream.str();
		return Execute(sqlstatement);
	}

	return true;
}
bool    Beopdatalink::CCommonDBAccess::RemoveCoreDebugItem(const wstring &strItemName, const wstring &strItemName2,const wstring &strItemName3 )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	// first, check whether

	std::ostringstream sqlstream;
	// try to insert the entry.

	string strItemName_utf8, strItemName2_utf8, strItemName3_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);
	Project::Tools::WideCharToUtf8(strItemName2, strItemName2_utf8);
	Project::Tools::WideCharToUtf8(strItemName3, strItemName3_utf8);


	sqlstream << "delete from " 
		<< GetDBName() 
		<< ".unit01 where unitproperty01 = '"
		<< strItemName_utf8 << "' and unitproperty02 = '"
		<< strItemName2_utf8 << "' and unitproperty03 = '"
		<< strItemName3_utf8 << "'";

	string sqlstatement = sqlstream.str();
	bool bresult =  Execute(sqlstatement);	

	return bresult;
}

bool    Beopdatalink::CCommonDBAccess::RemoveCoreDebugItem(const wstring &strItemName, const wstring &strItemName2)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	// first, check whether

	std::ostringstream sqlstream;
	// try to insert the entry.

	string strItemName_utf8, strItemName2_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);
	Project::Tools::WideCharToUtf8(strItemName2, strItemName2_utf8);


	sqlstream << "delete from " 
		<< GetDBName() 
		<< ".unit01 where unitproperty01 = '"
		<< strItemName_utf8 << "' and unitproperty02 = '"
		<< strItemName2_utf8 << "'";

	string sqlstatement = sqlstream.str();
	bool bresult =  Execute(sqlstatement);	

	return bresult;
}

bool     Beopdatalink::CCommonDBAccess::WriteCoreDebugItemValue(const wstring &strItemName, const wstring &strItemName2, const wstring &strItemValue)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	// first, check whether

	std::ostringstream sqlstream;
	// try to insert the entry.

	string strItemName_utf8, strItemName2_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);
	Project::Tools::WideCharToUtf8(strItemName2, strItemName2_utf8);

	string strItemValue_utf8;
	Project::Tools::WideCharToUtf8(strItemValue, strItemValue_utf8);

	sqlstream << "select * from " 
		<< GetDBName() 
		<< ".unit01 where unitproperty01 = '"
		<< strItemName_utf8 << "' and unitproperty02 = '"
		<< strItemName2_utf8 << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length)
	{
		sqlstream.str("");
		sqlstream << "insert into  "
			<< GetDBName() 
			<< ".unit01"
			<< " value " << "('" <<strItemName_utf8 << "', '" <<strItemName2_utf8 << "', '"<< strItemValue_utf8 << "','','','','','','','','','','','','')";

		string sqlstatement = sqlstream.str();
		bool bresult =  Execute(sqlstatement);		
	}
	else
	{
		sqlstream.str("");
		sqlstream << "update  "
			<< GetDBName()
			<< ".unit01"
			<< " set unitproperty03 = '" << strItemValue_utf8
			<< "' where unitproperty01 ='" << strItemName_utf8
			<< "' and unitproperty02 = '" << strItemName2_utf8 << "'";
		string sqlstatement = sqlstream.str();
		return Execute(sqlstatement);
	}

	return true;
}


bool     Beopdatalink::CCommonDBAccess::WriteCoreDebugItemValue(const wstring &strItemName, const wstring &strItemValue)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	// first, check whether

	std::ostringstream sqlstream;
	// try to insert the entry.

	string strItemName_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);
	string strItemValue_utf8;
	Project::Tools::WideCharToUtf8(strItemValue, strItemValue_utf8);

	sqlstream << "select * from " 
		<< GetDBName() 
		<< ".unit01 where unitproperty01 = '"
		<< strItemName_utf8 << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length)
	{
		sqlstream.str("");
		sqlstream << "insert into  "
			<< GetDBName() 
			<< ".unit01"
			<< " value " << "('" <<strItemName_utf8 << "', '" << strItemValue_utf8 << "','','','','','','','','','','','','','')";

		string sqlstatement = sqlstream.str();
		bool bresult =  Execute(sqlstatement);		
	}
	else
	{
		sqlstream.str("");
		sqlstream << "update  "
			<< GetDBName()
			<< ".unit01"
			<< " set unitproperty02 = '" << strItemValue_utf8
			<< "' where unitproperty01 ='" << strItemName_utf8 << "'";
		string sqlstatement = sqlstream.str();
		return Execute(sqlstatement);
	}

	return true;
}

//julian 0824
bool Beopdatalink::CCommonDBAccess::WriteMysqlGlobalVariable( const wstring &strItemName,const wstring &strGlobalVariable)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	
	string strItemName_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);
	string strGlobalVariable_utf8;
	Project::Tools::WideCharToUtf8(strGlobalVariable, strGlobalVariable_utf8);

	
	std::ostringstream sqlstream;
	sqlstream <<"set @@global."
		<<strItemName_utf8<<"="
		<<strGlobalVariable_utf8;
	/*
	sqlstream <<"set @@global.tmp_table_size="
		<<strGlobalVariable_utf8<<"";
	*/

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}


wstring     Beopdatalink::CCommonDBAccess::ReadOrCreateCoreDebugItemValue(wstring strItemName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strItemName_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< ".unit01 where unitproperty01 = '"
		<< strItemName_utf8 << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return L"";
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length)
	{
		sqlstream.str("");
		sqlstream << "insert into  "
			<< GetDBName() 
			<< ".unit01"
			<< " value " << "('" <<strItemName_utf8 << "', '0','','','','','','','','','','','','','')";

		string sqlstatement = sqlstream.str();
		bool bresult =  Execute(sqlstatement);		

		return L"0";
	}

	MYSQL_ROW  row = NULL;
	if(row = FetchRow(result) )
	{
		return Project::Tools::UTF8ToWideChar(row[1]);
	}

	return L"0";
}



wstring     Beopdatalink::CCommonDBAccess::ReadOrCreateCoreDebugItemValue(wstring strItemName, wstring strItemName2, wstring strItemName3)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strItemName_utf8, strItemName2_utf8, strItemName3_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);
	Project::Tools::WideCharToUtf8(strItemName2, strItemName2_utf8);
	Project::Tools::WideCharToUtf8(strItemName3, strItemName3_utf8);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< ".unit01 where unitproperty01 = '"
		<< strItemName_utf8 << "' and unitproperty02 = '"
		<< strItemName2_utf8<< "' and unitproperty03 = '"
		<< strItemName3_utf8 << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return L"";
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	MYSQL_ROW  row = NULL;
	if(row = FetchRow(result) )
	{
		return Project::Tools::UTF8ToWideChar(row[3]);
	}

	return L"0";
}

bool Beopdatalink::CCommonDBAccess::ReadLastHistoryDataInTable( wstring & strValue, const wstring &strPointName, const wstring &strTableName )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strTableName_ansi;
	Project::Tools::WideCharToUtf8(strTableName, strTableName_ansi);

	string pname_ansi;
	Project::Tools::WideCharToUtf8(strPointName, pname_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName()
		<< "."
		<< strTableName_ansi << " where pointname = '"<<  pname_ansi<<"' order by time desc  limit 0,1;";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	//resultlist.reserve(length);
	MYSQL_ROW  row = NULL;
	CRealTimeDataEntry entry;

	if(row = FetchRow(result) )
	{   
		Project::Tools::UTF8ToWideChar(row[2], strValue);
	}

	return true;
}


bool    Beopdatalink::CCommonDBAccess::ReadWarningHistory(const wstring &strWarningInfo, std::vector<tag_WarningHistoryStruct> & warningHistoryList)
{
	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWarningHistory( std::vector<tag_WarningHistoryStruct> & warningHistoryList, int nRowCount)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_warningrecord
		<< " where  confirmed = 0  order by time desc limit 0,"
		<< nRowCount;	

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	warningHistoryList.reserve(length);
	MYSQL_ROW  row = NULL;

	tag_WarningHistoryStruct entry;
	while(row = FetchRow(result) ){
		SYSTEMTIME stStart;
		Project::Tools::String_To_SysTime(row[0], stStart);
		entry.timeFrom = stStart;
		SYSTEMTIME stEnd;
		Project::Tools::String_To_SysTime(row[4], stEnd);
		entry.timeTo = stEnd;
		wstring warninginfo = Project::Tools::UTF8ToWideChar(row[2]);
		entry.strWarningInfo = warninginfo;
		entry.strPointName = Project::Tools::UTF8ToWideChar(row[7]);
		wstring strAction = Project::Tools::UTF8ToWideChar(row[6]);
		Project::Tools::SplitStringByChar(strAction.c_str(), L';', entry.vecAction);
		warningHistoryList.push_back(entry);
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWarningConfigGroupType( const WARNING_CONFIG_TYPE type, wstring& GroupName, vector<wstring>& vecSeries )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	wstring strColumnName;

	switch(type)
	{
	case WARNING_CONFIG_UNITPROPERTY01:
		strColumnName = L"unitproperty01";
		break;
	case WARNING_CONFIG_UNITPROPERTY02:
		strColumnName = L"unitproperty02";
		break;
	case WARNING_CONFIG_UNITPROPERTY03:
		strColumnName = L"unitproperty03";
		break;
	case WARNING_CONFIG_UNITPROPERTY04:
		strColumnName = L"unitproperty04";
		break;
	case WARNING_CONFIG_UNITPROPERTY05:
		strColumnName = L"unitproperty05";
		break;
	default:
		break;
	}

	if(strColumnName == L"")
		return false;

	string strColumnName_ansi;
	Project::Tools::WideCharToUtf8(strColumnName, strColumnName_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select distinct "
			  << strColumnName_ansi
			  <<" from " 
		<< GetDBName()
		<< "."
		<< tablename_warningconfig;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	
	MYSQL_ROW  row = NULL;
	wstring strValue;
	vecSeries.clear();
	while(row = FetchRow(result) )
	{   
		Project::Tools::UTF8ToWideChar(row[0], strValue);
		/*vector<wstring> vec;
		Project::Tools::SplitStringByChar(strValue.c_str(),'|',vec);
		if(vec.size()>1)
		{
			GroupName = vec[0];
			if(!FindIfExistInVec(vec[1],vecSeries))
				vecSeries.push_back(vec[1]);
		}*/

		if(strValue == L"" || strValue == L"0" )
		{
			CString strSysIni;
			GetSysPath(strSysIni);
			strSysIni += L"\\factory.ini";
			int nLanguage = GetPrivateProfileInt(L"beopobserver", L"language", 0, strSysIni);
			if (nLanguage == 1)
			{
				strValue = g_strUnit01DefaultEng;
			}
			else
			{
				strValue = g_strUnit01DefaultChn;
			}
		}
		if(!FindIfExistInVec(strValue,vecSeries))
			vecSeries.push_back(strValue);
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::FindIfExistInVec( wstring str, vector<wstring> vec )
{
	bool bFind = false;
	for(int i=0; i<vec.size(); i++)
	{
		if(vec[i] == str)
		{
			bFind = true;
			break;
		}
	}
	return bFind;
}

bool Beopdatalink::CCommonDBAccess::ReadWarningHistoryByPointName( const wstring strPointName,std::vector<tag_WarningHistoryStruct> & warningHistoryList, int nRowCount )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strPointName_ansi;
	Project::Tools::WideCharToUtf8(strPointName, strPointName_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_warningrecord
		<< " where  bindpointname = '"
		<< strPointName_ansi
		<<"'  order by time desc limit 0,"
		<< nRowCount;	

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	warningHistoryList.reserve(length);
	MYSQL_ROW  row = NULL;

	tag_WarningHistoryStruct entry;
	while(row = FetchRow(result) ){
		SYSTEMTIME stStart;
		Project::Tools::String_To_SysTime(row[0], stStart);
		entry.timeFrom = stStart;
		SYSTEMTIME stEnd;
		Project::Tools::String_To_SysTime(row[4], stEnd);
		entry.timeTo = stEnd;
		wstring warninginfo = Project::Tools::UTF8ToWideChar(row[2]);
		entry.strWarningInfo = warninginfo;
		entry.strPointName = Project::Tools::UTF8ToWideChar(row[7]);
		wstring strAction = Project::Tools::UTF8ToWideChar(row[6]);
		Project::Tools::SplitStringByChar(strAction.c_str(), L';', entry.vecAction);
		warningHistoryList.push_back(entry);
	}

	//for(int i=0; i<warningHistoryList.size(); ++i)
	//{
	//	ReadWarningOperationByPointName(warningHistoryList[i].vecAction,warningHistoryList[i].strPointName,warningHistoryList[i].timeFrom,warningHistoryList[i].timeTo);
	//}
	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadRecentWarningActionByPointName( const wstring strPointName,wstring& strAction )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strPointName_ansi;
	Project::Tools::WideCharToUtf8(strPointName, strPointName_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_warningrecord
		<< " where  bindpointname = '"
		<< strPointName_ansi
		<<"'  order by time desc limit 0,1;";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}

	MYSQL_ROW  row = FetchRow(result);
	if (row != NULL){
		strAction = Project::Tools::UTF8ToWideChar(row[6]);		
	}
	return true;
}

string Beopdatalink::CCommonDBAccess::GetWarningAction( int nType , bool bIsEnglishMode)
{
	string strActionType;

	if(bIsEnglishMode)
	{
		switch(nType)
		{
		case 0:
			strActionType = "UnResolve";
			break;
		case 1:
			strActionType = "Solving";
			break;
		case 2:
			strActionType = "Resolved";
			break;
		case 3:
			strActionType = "It's a bad warning";
			break;
		default:
			strActionType = "UnResolve";
			break;
		}
	}
	else
	{
		switch(nType)
		{
		case 0:
			strActionType = "报警未处理";
			break;		
		case 3:
			strActionType = "已知晓 正在解决";
			break;
		case 4:
			strActionType = "已解决";
			break;
		case 5:
			strActionType = "误报";
			break;
		default:
			strActionType = "报警未处理";
			break;
		}
	}
	return strActionType;
}

bool Beopdatalink::CCommonDBAccess::GetHistoryValue(const wstring& strName,const SYSTEMTIME &st,const int &nTimeFormat,  wstring &strParam)
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string histablename = GetHistoryTableNameByStoreCycle(st, (POINT_STORE_CYCLE)nTimeFormat);

	COleDateTime oleTimeStart(st);

	wstring wstrStart, wstrStop;
	string strStart, strStop;
	Project::Tools::OleTime_To_String(oleTimeStart, wstrStart);
	Project::Tools::WideCharToUtf8(wstrStart, strStart);

	string strPointName_ansi;
	Project::Tools::WideCharToUtf8(strName, strPointName_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select * from " << histablename 
		<< " where time <= '" << strStart <<"' and pointname = '"
		<< strPointName_ansi
		<< "' order by time desc";


	string sql = sqlstream.str();
	// execute the query
	MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
	wstring wstrSQL = Project::Tools::AnsiToWideChar(sql.c_str());
	_tprintf(wstrSQL.c_str());

	if (result == NULL)
	{
		_tprintf(_T("\r\nquery result is NULL\r\n ERROR is: "));
		string strErr = GetMysqlError();
		wstring wstrErr = Project::Tools::AnsiToWideChar(strErr.c_str());
		_tprintf(wstrErr.c_str());
		return false;
	}

	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);


	long rownum = GetRowNum(result);
	if (rownum == 0){
		return false;
	}

	MYSQL_ROW row = NULL;
	if(row = FetchRow(result))
	{
		strParam = Project::Tools::UTF8ToWideChar(row[2]);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::GetHistoryValue(const wstring& strName,const SYSTEMTIME &stStart,const SYSTEMTIME &stEnd,const int &nTimeFormat, wstring &strParam)
{

	COleDateTime oleTimeStart(stStart),oleTimeEnd(stEnd);

	if(oleTimeEnd<oleTimeStart)
	{
		strParam = _T("end time < start time");
		return false;
	}


	COleDateTime tStartDayBegin(stStart);
	tStartDayBegin.SetDateTime(tStartDayBegin.GetYear(), tStartDayBegin.GetMonth(), tStartDayBegin.GetDay(), 0,0,0);
	COleDateTime tEndDayBegin(stEnd);
	tEndDayBegin.SetDateTime(tEndDayBegin.GetYear(), tEndDayBegin.GetMonth(), tEndDayBegin.GetDay(), 0,0,0);

	COleDateTimeSpan tspan = tEndDayBegin - tStartDayBegin;
	if(tspan.GetTotalMinutes()>60*24) //超过一天
	{
		strParam = _T("can not query more than 2 days data.");
		return false;
	}
	else if(tspan.GetTotalMinutes()==0)
	{//相同一天
		return GetHistoryValueInOneDay(strName, stStart, stEnd, nTimeFormat, strParam);
	}
	else //相隔两天
	{
		COleDateTime tStartDayEnd = oleTimeStart;
		tStartDayEnd.SetDateTime(tStartDayEnd.GetYear(), tStartDayEnd.GetMonth(), tStartDayEnd.GetDay(), 23,59,59);
		SYSTEMTIME stStartDayEnd;
		tStartDayEnd.GetAsSystemTime(stStartDayEnd);
		bool bReadDay1 = GetHistoryValueInOneDay(strName, stStart, stStartDayEnd, nTimeFormat, strParam);

		//next day
		SYSTEMTIME stEndDayBegin;
		tEndDayBegin.GetAsSystemTime(stEndDayBegin);
		bool bReadDay2 = GetHistoryValueInOneDay(strName, stEndDayBegin, stEnd, nTimeFormat, strParam);

		return bReadDay1 && bReadDay2;
	}


	return true;
}


bool Beopdatalink::CCommonDBAccess::GetHistoryValueInOneDay(const wstring& strName,const SYSTEMTIME &stStart,const SYSTEMTIME &stEnd,const int &nTimeFormat, wstring &strParam)
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	COleDateTimeSpan tspan;
	if(nTimeFormat==E_STORE_FIVE_SECOND)
	{
		tspan.SetDateTimeSpan(0,0,0,5);
	}
	else if(nTimeFormat==E_STORE_ONE_MINUTE)
	{
		tspan.SetDateTimeSpan(0,0,1,0);
	}
	else if(nTimeFormat==E_STORE_FIVE_MINUTE)
	{
		tspan.SetDateTimeSpan(0,0,5,0);
	}
	else if(nTimeFormat==E_STORE_ONE_HOUR)
	{
		tspan.SetDateTimeSpan(0,1,0,0);
	}
	else if(nTimeFormat==E_STORE_ONE_DAY)
	{
		tspan.SetDateTimeSpan(1,0,0,0);
	}
	else if(nTimeFormat==E_STORE_ONE_MONTH)
	{
		tspan.SetDateTimeSpan(30,0,0,0);
	}
	else if(nTimeFormat==E_STORE_ONE_YEAR)
	{
		tspan.SetDateTimeSpan(365,0,0,0);
	}

	string histablename = GetHistoryTableNameByStoreCycle(stStart, (POINT_STORE_CYCLE) nTimeFormat);

	COleDateTime oleTimeStart(stStart),oleTimeEnd(stEnd);

	wstring wstrStart, wstrStop;
	string strStart, strStop;

	Project::Tools::OleTime_To_String(oleTimeStart, wstrStart);
	Project::Tools::OleTime_To_String(oleTimeEnd, wstrStop);
	Project::Tools::WideCharToUtf8(wstrStart, strStart);
	Project::Tools::WideCharToUtf8(wstrStop, strStop);

	string strPointName_ansi;
	Project::Tools::WideCharToUtf8(strName, strPointName_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select * from " << histablename 
		<< " where pointname = '"
		<< strPointName_ansi
		<< "' order by time";


	string sql = sqlstream.str();

	// execute the query
	MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );

	wstring wstrSQL = Project::Tools::AnsiToWideChar(sql.c_str());
	_tprintf(wstrSQL.c_str());

	if (result == NULL)
	{
		_tprintf(_T("\r\nquery result is NULL\r\n"));
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);


	long rownum = GetRowNum(result);
	if (rownum == 0){
		return false;
	}

	MYSQL_ROW row = NULL;
	vector<COleDateTime> tQueryList;
	vector<wstring>  strQueryList;

	while(row = FetchRow(result))
	{
		SYSTEMTIME st;
		Project::Tools::String_To_SysTime(row[0], st);
		COleDateTime   oleQt(st);
		tQueryList.push_back(oleQt);

		wstring wstrValue;
		Project::Tools::UTF8ToWideChar(row[2], wstrValue);
		strQueryList.push_back(wstrValue);
	}

	//将查到的数据补齐
	unsigned int i = 0;
	for(i=0;(i+1)<tQueryList.size();i++)
	{
		COleDateTime sti = tQueryList[i];
		COleDateTime stj = tQueryList[i+1];

		COleDateTime tNextRight = sti + tspan;
		if(tNextRight < stj)
		{
			tQueryList.insert(tQueryList.begin()+ i+1, tNextRight);
			wstring wstrTemp = strQueryList[i];
			strQueryList.insert(strQueryList.begin()+ i+1, wstrTemp);
		}

	}


	//补齐后面的
	if(tQueryList.size()>0)
	{
		COleDateTime tCur = tQueryList.back();
		wstring strValueLack = strQueryList.back(); // _T("err");
		while(tCur<oleTimeEnd)
		{
			tCur += tspan;
			tQueryList.push_back(tCur);
			strQueryList.push_back(strValueLack);
		}
	}


	//从补齐的数据中挑选
	for(i=0;i<tQueryList.size();i++)
	{
		if(tQueryList[i]>=oleTimeStart && tQueryList[i]<= oleTimeEnd)
		{
			wstring strValueItem = strQueryList[i];
			strParam  = strParam + strValueItem;
			strParam = strParam + L",";
		}
	}

	_tprintf(_T("gethistory result:\r\n"));
	_tprintf(strParam.c_str());

	return true;
}

string  Beopdatalink::CCommonDBAccess::GetHistoryTableNameByStoreCycle(const SYSTEMTIME &stNow, const POINT_STORE_CYCLE &tCycle)
{
	SYSTEMTIME st = stNow;
	char buff[64];

	if(tCycle==E_STORE_FIVE_SECOND)
	{
		st.wSecond = 5*(st.wSecond/5);
		sprintf_s(buff, "%s_5second_%d_%02d_%02d", tablename_historydata, st.wYear, st.wMonth, st.wDay);
	}
	else if(tCycle==E_STORE_ONE_MINUTE)
	{
		st.wSecond = 0;
		sprintf_s(buff, "%s_minute_%d_%02d_%02d", tablename_historydata, st.wYear, st.wMonth, st.wDay);
	}
	else if(tCycle==E_STORE_FIVE_MINUTE)
	{	
		st.wMinute = 5*(st.wMinute/5);
		sprintf_s(buff, "%s_5minute_%d_%02d_%02d", tablename_historydata, st.wYear, st.wMonth, st.wDay);
	}
	else if(tCycle==E_STORE_ONE_HOUR)
	{	
		st.wMinute = 0;
		st.wSecond = 0;
		sprintf_s(buff, "%s_hour_%d_%02d", tablename_historydata, st.wYear, st.wMonth);
	}
	else if(tCycle==E_STORE_ONE_DAY)
	{	
		st.wHour = 0;
		st.wMinute = 0;
		st.wSecond = 0;
		sprintf_s(buff, "%s_day_%d", tablename_historydata, st.wYear);
	}
	else if(tCycle==E_STORE_ONE_MONTH)
	{	
		st.wDay = 1;
		st.wHour = 0;
		st.wMinute = 0;
		st.wSecond = 0;
		sprintf_s(buff, "%s_month_%02d", tablename_historydata, st.wYear);
	}
	else if(tCycle==E_STORE_ONE_YEAR)
	{	
		st.wMonth = 1;
		st.wDay = 1;
		st.wHour = 0;
		st.wMinute = 0;
		st.wSecond = 0;
		sprintf_s(buff, "%s_year", tablename_historydata);
	}

	string newtablename = buff;

	return newtablename;
}

bool Beopdatalink::CCommonDBAccess::ReadWarningOperationByInfo( vector<ActionInfo>& resultlist, const wstring strInfo, int nRecentCount /*= 10*/ )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strInfo_ansi;
	Project::Tools::WideCharToUtf8(strInfo, strInfo_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_warningoperation
		<< " where unitproperty02='"
		<< strInfo_ansi
		<< "'  order by unitproperty01 desc limit 0,"
		<< nRecentCount;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length)
	{
		return false;
	}
	resultlist.reserve(length);
	MYSQL_ROW  row = NULL;

	ActionInfo wcItem;

	while(row = FetchRow(result) )
	{
		wcItem.strActionTime = Project::Tools::UTF8ToWideChar(row[0]);
		wcItem.strActionUser = Project::Tools::UTF8ToWideChar(row[3]);
		wcItem.strAction = Project::Tools::UTF8ToWideChar(row[4]);
		resultlist.push_back(wcItem);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWarningOperationByPointName( vector<wstring>& resultlist, const wstring strPointName, const SYSTEMTIME& start, const SYSTEMTIME& end )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strPointName_ansi;
	Project::Tools::WideCharToUtf8(strPointName, strPointName_ansi);

	string strStart_ansi = Project::Tools::SystemTimeToString(start);

	string strEnd_ansi = Project::Tools::SystemTimeToString(end);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_warningoperation
		<< " where unitproperty03='"
		<< strPointName_ansi
		<< "' and unitproperty01>='"
		<< strStart_ansi
		<< "' and unitproperty01<='"
		<< strEnd_ansi
		<< "'order by unitproperty01";
		
	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length)
	{
		return false;
	}
	resultlist.reserve(length);
	MYSQL_ROW  row = NULL;

	wstring wcItem;

	while(row = FetchRow(result) )
	{
		wcItem = Project::Tools::UTF8ToWideChar(row[0]);
		wcItem += L",";
		wcItem += Project::Tools::UTF8ToWideChar(row[3]);
		wcItem += L",";
		wcItem += Project::Tools::UTF8ToWideChar(row[4]);
		resultlist.push_back(wcItem);
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::GetLatestEntryByPointName( const wstring& strPointName, const SYSTEMTIME &stTime, const POINT_STORE_CYCLE &tCycle, CRealTimeDataEntry& resultEntry, bool bGetCount, int& nCount )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	string histablename = GetHistoryTableNameByStoreCycle(stTime, tCycle);
	//获取最近的一个值
	{
		string strPointName_ansi;
		Project::Tools::WideCharToUtf8(strPointName, strPointName_ansi);
		std::ostringstream sqlstream;
		sqlstream << "select * from "
			<< GetDBName()
			<<"."
			<< histablename 
			<< " where pointname = '"
			<< strPointName_ansi
			<< "' order by time desc limit 0,1";
		string sql = sqlstream.str();
		MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
		if (result == NULL)
		{
			return false;
		}
		Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);
		long rownum = GetRowNum(result);
		if (rownum == 0)
		{
			return false;
		}
		MYSQL_ROW row = NULL;
		while(row = FetchRow(result))
		{
			SYSTEMTIME st;
			Project::Tools::String_To_SysTime(row[0], st);
			wstring wstrValue;
			Project::Tools::UTF8ToWideChar(row[2], wstrValue);
			resultEntry.SetTimestamp(st);
			resultEntry.SetValue(wstrValue);
		}
	}
	if(bGetCount)
	{
		std::ostringstream sqlstream;
		sqlstream << "select count(*) from "
			<< GetDBName()
			<<"."
			<< histablename;
		string sql = sqlstream.str();
		MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
		if (result == NULL)
		{
			return false;
		}
		Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);
		long rownum = GetRowNum(result);
		if (rownum == 0)
		{
			return false;
		}
		MYSQL_ROW row = NULL;
		while(row = FetchRow(result))
		{
			nCount = atoi(row[0]);
		}
	}
	return true;
}

void Beopdatalink::CCommonDBAccess::CreateWorkOrderAllTables(void)
{
	CreateWorkOrderItem();
	CreateWorkOrderProgress();
	CreateWorkOrderDepartment();
	CreateWorkOrderPeople();
	CreateWorkOrderDevice();
	CreateWorkOrderResult();
	CreateWorkOrderWarningRelate();
}

bool Beopdatalink::CCommonDBAccess::CreateWorkOrderItem(void)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);
	string strSql = "CREATE TABLE IF NOT EXISTS `beopdata`.`work_order_item`(\
		`itemName` VARCHAR(255),\
		`departmentNum` INT(10) UNSIGNED,\
		`peopleNum` INT(10) UNSIGNED,\
		`time` TIMESTAMP,\
		`deviceNum` INT(10) UNSIGNED,\
		`description` TEXT,\
		`cost` FLOAT(10,2),\
		`resultNum` INT(10) UNSIGNED,\
		`remark` TEXT,\
		`enable` TINYINT(3),\
		`unitproperty01` TEXT,\
		`unitproperty02` TEXT,\
		`unitproperty03` TEXT,\
		`unitproperty04` TEXT,\
		`unitproperty05` TEXT\
		)\
		ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;";

	return Execute(strSql);
}

bool Beopdatalink::CCommonDBAccess::CreateWorkOrderProgress(void)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);
	string strSql = "CREATE TABLE IF NOT EXISTS `beopdata`.`work_order_progress`(\
		`itemNum` INT(10) UNSIGNED,\
		`progressNum` INT(10) UNSIGNED,\
		`peopleNum` INT(10) UNSIGNED,	\
		`enable` TINYINT(3),\
		`unitproperty01` TEXT,\
		`unitproperty02` TEXT,\
		`unitproperty03` TEXT,\
		`unitproperty04` TEXT,\
		`unitproperty05` TEXT\
		)\
		ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;";

	return Execute(strSql);
}

bool Beopdatalink::CCommonDBAccess::CreateWorkOrderDepartment(void)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);
	string strSql = "CREATE TABLE IF NOT EXISTS `beopdata`.`work_order_department`(\
		`departmentNum` INT(10) UNSIGNED UNIQUE,\
		`departmentName` VARCHAR(255),\
		`enable` TINYINT(3),\
		`unitproperty01` TEXT,\
		`unitproperty02` TEXT,\
		`unitproperty03` TEXT,\
		`unitproperty04` TEXT,\
		`unitproperty05` TEXT\
		)\
		ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;";

	return Execute(strSql);
}

bool Beopdatalink::CCommonDBAccess::CreateWorkOrderPeople(void)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);
	string strSql = "CREATE TABLE IF NOT EXISTS `beopdata`.`work_order_people`(\
		`peopleNum` INT(10) UNSIGNED UNIQUE,\
		`peopleName` VARCHAR(255),\
		`enable` TINYINT(3),\
		`unitproperty01` TEXT,\
		`unitproperty02` TEXT,\
		`unitproperty03` TEXT,\
		`unitproperty04` TEXT,\
		`unitproperty05` TEXT\
		)\
		ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;";

	return Execute(strSql);
}

bool Beopdatalink::CCommonDBAccess::CreateWorkOrderDevice(void)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);
	string strSql = "CREATE TABLE IF NOT EXISTS `beopdata`.`work_order_device`(\
		`deviceNum` INT(10) UNSIGNED UNIQUE,\
		`deviceName` VARCHAR(255),\
		`enable` TINYINT(3),\
		`unitproperty01` TEXT,\
		`unitproperty02` TEXT,\
		`unitproperty03` TEXT,\
		`unitproperty04` TEXT,\
		`unitproperty05` TEXT\
		)\
		ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;";

	return Execute(strSql);
}

bool Beopdatalink::CCommonDBAccess::CreateWorkOrderResult(void)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);
	string strSql = "CREATE TABLE IF NOT EXISTS `beopdata`.`work_order_result`(\
		`resultNum` INT(10) UNSIGNED UNIQUE,\
		`resultName` VARCHAR(255),\
		`enable` TINYINT(3),\
		`unitproperty01` TEXT,\
		`unitproperty02` TEXT,\
		`unitproperty03` TEXT,\
		`unitproperty04` TEXT,\
		`unitproperty05` TEXT\
		)\
		ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;";

	return Execute(strSql);
}

bool Beopdatalink::CCommonDBAccess::CreateWorkOrderWarningRelate(void)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);
	string strSql = "CREATE TABLE IF NOT EXISTS `beopdata`.`work_order_warning_relate`(\
		`itemName` VARCHAR(255),\
		`warnStartTime` TIMESTAMP,\
		`warnInfo` VARCHAR(255),\
		`enable` TINYINT(3),\
		`unitproperty01` TEXT,\
		`unitproperty02` TEXT,\
		`unitproperty03` TEXT,\
		`unitproperty04` TEXT,\
		`unitproperty05` TEXT\
		)\
		ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;";

	return Execute(strSql);
}

bool Beopdatalink::CCommonDBAccess::ReadWorkOrderItems(vector<WorkOrderItem>& vecWoItem)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	const char* pSql = "select * from `beopdata`.`work_order_item` where enable = 1;";
	MYSQL_RES* result = QueryAndReturnResult(pSql);
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int nLength = static_cast<int>(GetRowNum(result));
	if (0 == nLength)
	{
		return false;
	}
	vecWoItem.reserve(nLength);

	MYSQL_ROW	row = NULL;
	WorkOrderItem woItem;
	wstring strTime;
	while (row = FetchRow(result))
	{
		woItem.strItemName.Format(_T("%s"), Project::Tools::UTF8ToWideChar(row[0]).c_str());
		woItem.nDepartmentNum = atoi(row[1]);
		woItem.nPeopleNum = atoi(row[2]);
		woItem.tmStart.ParseDateTime(Project::Tools::UTF8ToWideChar(row[3]).c_str(), 0, LANG_USER_DEFAULT);
		woItem.nDeviceNum = atoi(row[4]);
		woItem.strDesc.Format(_T("%s"), Project::Tools::UTF8ToWideChar(row[5]).c_str());
		woItem.dCost = atof(row[6]);
		woItem.nResultNum = atoi(row[7]);
		woItem.strRemark.Format(_T("%s"), Project::Tools::UTF8ToWideChar(row[8]).c_str());

		vecWoItem.push_back(woItem);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWorkOrderProgress(vector<WorkOrderProgress>& vecWoProgress)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	const char* pSql = "select * from `beopdata`.`work_order_progress` where enable = 1;";
	MYSQL_RES* result = QueryAndReturnResult(pSql);
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int nLength = static_cast<int>(GetRowNum(result));
	if (0 == nLength)
	{
		return false;
	}
	vecWoProgress.reserve(nLength);

	MYSQL_ROW	row = NULL;
	WorkOrderProgress woProgress;
	wstring strTime;
	while (row = FetchRow(result))
	{
		woProgress.nItemNum = atoi(row[0]);
		woProgress.nProgressNum = atoi(row[1]);
		woProgress.nPeopleNum = atoi(row[2]);

		vecWoProgress.push_back(woProgress);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWorkOrderDepartment(vector<WorkOrderDepartment>& vecWoDepartm)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	const char* pSql = "select * from `beopdata`.`work_order_department` where enable = 1;";
	MYSQL_RES* result = QueryAndReturnResult(pSql);
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int nLength = static_cast<int>(GetRowNum(result));
	if (0 == nLength)
	{
		return false;
	}
	vecWoDepartm.reserve(nLength);

	MYSQL_ROW	row = NULL;
	WorkOrderDepartment woDepartm;
	wstring strTime;
	while (row = FetchRow(result))
	{
		woDepartm.nDepartmentNum = atoi(row[0]);
		woDepartm.strDepartmentName.Format(_T("%s"), Project::Tools::UTF8ToWideChar(row[1]).c_str());

		vecWoDepartm.push_back(woDepartm);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWorkOrderPeople(vector<WorkOrderPeople>& vecWoPeople)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	const char* pSql = "select * from `beopdata`.`work_order_people` where enable = 1;";
	MYSQL_RES* result = QueryAndReturnResult(pSql);
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int nLength = static_cast<int>(GetRowNum(result));
	if (0 == nLength)
	{
		return false;
	}
	vecWoPeople.reserve(nLength);

	MYSQL_ROW	row = NULL;
	WorkOrderPeople woPeople;
	wstring strTime;
	while (row = FetchRow(result))
	{
		woPeople.nPeopleNum = atoi(row[0]);
		woPeople.strPeopleName.Format(_T("%s"), Project::Tools::UTF8ToWideChar(row[1]).c_str());

		vecWoPeople.push_back(woPeople);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWorkOrderDevice(vector<WorkOrderDevice>& vecWoDevice)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	const char* pSql = "select * from `beopdata`.`work_order_device` where enable = 1;";
	MYSQL_RES* result = QueryAndReturnResult(pSql);
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int nLength = static_cast<int>(GetRowNum(result));
	if (0 == nLength)
	{
		return false;
	}
	vecWoDevice.reserve(nLength);

	MYSQL_ROW	row = NULL;
	WorkOrderDevice woDev;
	wstring strTime;
	while (row = FetchRow(result))
	{
		woDev.nDeviceNum = atoi(row[0]);
		woDev.strDeviceName.Format(_T("%s"), Project::Tools::UTF8ToWideChar(row[1]).c_str());

		vecWoDevice.push_back(woDev);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadWorkOrderResult(vector<WorkOrderResult>& vecWoRes)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	const char* pSql = "select * from `beopdata`.`work_order_result` where enable = 1;";
	MYSQL_RES* result = QueryAndReturnResult(pSql);
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int nLength = static_cast<int>(GetRowNum(result));
	if (0 == nLength)
	{
		return false;
	}
	vecWoRes.reserve(nLength);

	MYSQL_ROW	row = NULL;
	WorkOrderResult woRes;
	wstring strTime;
	while (row = FetchRow(result))
	{
		woRes.nResultNum = atoi(row[0]);
		woRes.strResultName.Format(_T("%s"), Project::Tools::UTF8ToWideChar(row[1]).c_str());

		vecWoRes.push_back(woRes);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::InsertWorkOrderItem(const WorkOrderItem& woItem)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string	strItemName;
	string	strDesc;
	string	strRemark;
	Project::Tools::WideCharToUtf8(woItem.strItemName.GetString(), strItemName);
	Project::Tools::WideCharToUtf8(woItem.strDesc.GetString(), strDesc);
	Project::Tools::WideCharToUtf8(woItem.strRemark.GetString(), strRemark);

	std::ostringstream sqlstreamUpdate;
	sqlstreamUpdate << "insert into `beopdata`.`work_order_item`(itemName, departmentNum, peopleNum, deviceNum, description, cost, resultNum, remark, enable) value('" 
		<< strItemName << "', "<< woItem.nDepartmentNum <<", "<< woItem.nPeopleNum <<", "<< woItem.nDeviceNum <<", '"
		<< strDesc <<"', " << woItem.dCost << ", " << woItem.nResultNum << ", '" << strRemark << "', 1);";

	string sqlstatement = sqlstreamUpdate.str();
	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::UpdateWorkOrderItem(const WorkOrderItem& woItem)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string	strDesc;
	string	strRemark;
	string	strItemName;
	Project::Tools::WideCharToUtf8(woItem.strDesc.GetString(), strDesc);
	Project::Tools::WideCharToUtf8(woItem.strRemark.GetString(), strRemark);
	Project::Tools::WideCharToUtf8(woItem.strItemName.GetString(), strItemName);

	std::ostringstream sqlstreamUpdate;
	sqlstreamUpdate << "update `beopdata`.`work_order_item` set description = '"<< strDesc 
		<<"', remark = '"<< strRemark <<"', departmentNum = "<< woItem.nDepartmentNum
		<<", peopleNum = "<< woItem.nPeopleNum <<", deviceNum = "<< woItem.nDeviceNum
		<<", resultNum = "<< woItem.nResultNum <<" where itemName = '"<< strItemName <<"';";

	string sqlstatement = sqlstreamUpdate.str();
	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::DeleteWorkOrderItem(const wstring strItemName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string	strUtf8ItemName;
	Project::Tools::WideCharToUtf8(strItemName, strUtf8ItemName);

	std::ostringstream sqlstreamUpdate;
	sqlstreamUpdate << "delete from `beopdata`.`work_order_item` where itemName = '" << strUtf8ItemName << "';";

	string sqlstatement = sqlstreamUpdate.str();
	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::ReadWorkOrderWarnRelate(vector<WorkOrderWarnRelate>& vecWoWarnRelate)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	const char* pSql = "select * from `beopdata`.`work_order_warning_relate` where enable = 1;";
	MYSQL_RES* result = QueryAndReturnResult(pSql);
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int nLength = static_cast<int>(GetRowNum(result));
	if (0 == nLength)
	{
		return false;
	}
	vecWoWarnRelate.reserve(nLength);

	MYSQL_ROW	row = NULL;
	WorkOrderWarnRelate woWarnRelate;
	wstring strTime;
	while (row = FetchRow(result))
	{
		woWarnRelate.strItemName.Format(_T("%s"), Project::Tools::UTF8ToWideChar(row[0]).c_str());
		woWarnRelate.tmStart.ParseDateTime(Project::Tools::UTF8ToWideChar(row[1]).c_str(), 0, LANG_USER_DEFAULT);
		woWarnRelate.strWarnInfo.Format(_T("%s"), Project::Tools::UTF8ToWideChar(row[2]).c_str());

		vecWoWarnRelate.push_back(woWarnRelate);
	}

	return true;
}

bool	Beopdatalink::CCommonDBAccess::ReadHistoryDataInTableFix(wstring & strValue,  const COleDateTime &sStartTime, const COleDateTime &sEndTime, const wstring &strPointName, const wstring & strTableName, const POINT_STORE_CYCLE &tCycle)
{
	bool bResult = false;

	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);


	string strTableName_ansi;
	Project::Tools::WideCharToUtf8(strTableName, strTableName_ansi);

	wstring wstr_time;
	Project::Tools::OleTime_To_String(sStartTime, wstr_time);

	string time_ansi;
	Project::Tools::WideCharToUtf8(wstr_time, time_ansi);

	string pname_ansi;
	Project::Tools::WideCharToUtf8(strPointName, pname_ansi);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName()
		<< "."
		<< strTableName_ansi << " where time = '"<< time_ansi<<"' and pointname = '"<<  pname_ansi<<"'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}

	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);
	if (result->row_count == 0)
	{
		COleDateTimeSpan tspan;
		if(tCycle==E_STORE_FIVE_SECOND)
		{
			tspan.SetDateTimeSpan(0,0,0,5);
		}
		else if(tCycle==E_STORE_ONE_MINUTE)
		{
			tspan.SetDateTimeSpan(0,0,1,0);
		}
		else if(tCycle==E_STORE_FIVE_MINUTE)
		{
			tspan.SetDateTimeSpan(0,0,5,0);
		}
		else if(tCycle==E_STORE_ONE_HOUR)
		{
			tspan.SetDateTimeSpan(0,1,0,0);
		}
		else if(tCycle==E_STORE_ONE_DAY)
		{
			tspan.SetDateTimeSpan(1,0,0,0);
		}
		else if(tCycle==E_STORE_ONE_MONTH)
		{
			tspan.SetDateTimeSpan(30,0,0,0);
		}
		else if(tCycle==E_STORE_ONE_YEAR)
		{
			tspan.SetDateTimeSpan(365,0,0,0);
		}
		wstring strStart;
		wstring strEnd;
		Project::Tools::OleTime_To_String(sStartTime, strStart);
		Project::Tools::OleTime_To_String(sEndTime, strEnd);
		string str1;
		string str2;
		Project::Tools::WideCharToUtf8(strStart, str1);
		Project::Tools::WideCharToUtf8(strEnd, str2);
		std::ostringstream sqlstream1;
		vector<CRealTimeDataEntry> entryList; 
		sqlstream1 << "select * from "
			<< GetDBName()
			<<"."
			<< strTableName_ansi 
			<< " where pointname = '"
			<< pname_ansi<<"' and time>='"<<str1<<"' and time<'"<<str2<<"'"<<" order by time";
		MYSQL_RES* result1 = QueryAndReturnResult(sqlstream1.str().c_str());
		if(result1 == NULL)
		{
			bResult = false;
		}
		else
		{
			Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result1);
			int length = static_cast<int>(GetRowNum(result1));
			if (0 == length)
			{
				bResult = false;	
			}
			else
			{
				MYSQL_ROW  row = NULL;
				if(row = FetchRow(result1) )
				{   
					Project::Tools::UTF8ToWideChar(row[2], strValue);
					bResult = true;
					return bResult;
				}
			}
		}
	}
	else
	{
		int length = static_cast<int>(GetRowNum(result));
		if (0 == length)
		{
			bResult = false;
		}
		else
		{
			MYSQL_ROW  row = NULL;
			if(row = FetchRow(result) )
			{   
				Project::Tools::UTF8ToWideChar(row[2], strValue);
				bResult = true;
				return bResult;
			}
		}
	}
	return bResult;
}

bool Beopdatalink::CCommonDBAccess::GetHistoryDataFixedMulti( const vector<wstring>& strNameVec,const SYSTEMTIME &stStart,const SYSTEMTIME &stEnd,const int &nTimeFormat, vector<vector<CRealTimeDataEntry>>& entryListVec )
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);
	int nCount = 0;
	int nSpan = Project::Tools::SystemTimeToTime_t(stEnd) - Project::Tools::SystemTimeToTime_t(stStart);
	COleDateTimeSpan tspan;
	if(nTimeFormat==E_STORE_FIVE_SECOND)
	{
		tspan.SetDateTimeSpan(0,0,0,5);
		nCount = nSpan/5+1;
	}
	else if(nTimeFormat==E_STORE_ONE_MINUTE)
	{
		tspan.SetDateTimeSpan(0,0,1,0);
		nCount = nSpan/60+1;
	}
	else if(nTimeFormat==E_STORE_FIVE_MINUTE)
	{
		tspan.SetDateTimeSpan(0,0,5,0);
		nCount = nSpan/(60*5)+1;
	}
	else if(nTimeFormat==E_STORE_ONE_HOUR)
	{
		tspan.SetDateTimeSpan(0,1,0,0);
		nCount = nSpan/(60*60)+1;
	}
	else if(nTimeFormat==E_STORE_ONE_DAY)
	{
		tspan.SetDateTimeSpan(1,0,0,0);
		nCount = nSpan/(60*60*24)+1;
	}
	else if(nTimeFormat==E_STORE_ONE_MONTH)
	{
		tspan.SetDateTimeSpan(30,0,0,0);
		nCount = nSpan/(60*60*24*30)+1;
	}
	else if(nTimeFormat==E_STORE_ONE_YEAR)
	{
		tspan.SetDateTimeSpan(365,0,0,0);
		nCount = nSpan/(60*60*24*365)+1;
	}

	string histablename = GetHistoryTableNameByStoreCycle(stStart, (POINT_STORE_CYCLE) nTimeFormat);

	COleDateTime oleTimeStart(stStart),oleTimeEnd(stEnd);

	wstring wstrStart, wstrStop;
	string strStart, strStop;

	Project::Tools::OleTime_To_String(oleTimeStart, wstrStart);
	Project::Tools::OleTime_To_String(oleTimeEnd, wstrStop);
	Project::Tools::WideCharToUtf8(wstrStart, strStart);
	Project::Tools::WideCharToUtf8(wstrStop, strStop);

	string strNameSerial;
	for (size_t i=0;i<strNameVec.size();++i)
	{
		string strPointName_ansi;
		Project::Tools::WideCharToUtf8(strNameVec[i], strPointName_ansi);
		strNameSerial += "'";
		strNameSerial += strPointName_ansi;
		strNameSerial += "'";
		if (i != strNameVec.size()-1)
		{
			strNameSerial += ",";
		}
	}
	
	std::ostringstream sqlstream;
	sqlstream << "select * from "
		<< GetDBName()
		<<"."
		<< histablename 
		<< " where pointname IN("
		<<strNameSerial
		<< ") order by time";


	string sql = sqlstream.str();
	MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
	if (result == NULL)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	long rownum = GetRowNum(result);
	if (rownum == 0)
	{
		return false;
	}

	for (size_t ii=0;ii<strNameVec.size();++ii)
	{
		MYSQL_ROW row = NULL;
		static vector<COleDateTime> tQueryList;
		static vector<wstring>  strQueryList;

		static vector<COleDateTime> tQueryListOrg;
		static vector<wstring>  strQueryListOrg;

		tQueryList.clear();
		strQueryList.clear();
		tQueryListOrg.clear();
		strQueryListOrg.clear();

		while(row = FetchRow(result))
		{
			wstring wstrPointName;
			Project::Tools::UTF8ToWideChar(row[1], wstrPointName);

			if (wstrPointName == strNameVec[ii])
			{
				SYSTEMTIME st;
				Project::Tools::String_To_SysTime(row[0], st);
				COleDateTime   oleQt(st);
				tQueryListOrg.push_back(oleQt);

				wstring wstrValue;
				Project::Tools::UTF8ToWideChar(row[2], wstrValue);
				strQueryListOrg.push_back(wstrValue);
			}
		}
		if (row == NULL)
		{
			mysql_data_seek(result,0);
		}
		size_t nQueryCount = tQueryListOrg.size();
		if (nQueryCount > 0)
		{
			//数据补齐算法，严观微编写
			{
				unsigned int i = 0;
				for(i=0;(i+1)<tQueryListOrg.size();i++)
				{
					COleDateTime sti = tQueryListOrg[i];
					COleDateTime stj = tQueryListOrg[i+1];
					tQueryList.push_back(tQueryListOrg[i]);
					strQueryList.push_back(strQueryListOrg[i]);
					COleDateTimeSpan span = stj - sti;
					int nInterval = span.GetTotalSeconds();
					int nSub = tspan.GetTotalSeconds();
					if(nInterval > nSub)
					{
						int nCount = nInterval/nSub - 1;
						for (int j=0;j<nCount;++j)
						{
							COleDateTimeSpan t(0,0,0,tspan.GetTotalSeconds()*(j+1));
							tQueryList.push_back(tQueryListOrg[i]+t);
							strQueryList.push_back(strQueryListOrg[i]);
						}
					}
				}
				tQueryList.push_back(tQueryListOrg[tQueryListOrg.size()-1]);
				strQueryList.push_back(strQueryListOrg[strQueryListOrg.size()-1]);
			}

			SYSTEMTIME stQueryMin;
			SYSTEMTIME stQueryMax;
			tQueryList[0].GetAsSystemTime(stQueryMin);
			tQueryList[tQueryList.size()-1].GetAsSystemTime(stQueryMax);
			if (Project::Tools::SystemTimeToTime_t(stStart) >= Project::Tools::SystemTimeToTime_t(stQueryMax))
			{
				COleDateTime ole(stStart);
				for (int i=0;i<nCount;++i)
				{	
					CRealTimeDataEntry entry;
					SYSTEMTIME st;
					ole.GetAsSystemTime(st);
					entry.SetTimestamp(st);
					entry.SetValue(strQueryList[strQueryList.size()-1]);
					ole += tspan;
					entryListVec[ii].push_back(entry);
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stEnd) <= Project::Tools::SystemTimeToTime_t(stQueryMin))
			{
				COleDateTime ole(stStart);
				for (int i=0;i<nCount;++i)
				{
					CRealTimeDataEntry entry;
					SYSTEMTIME st;
					ole.GetAsSystemTime(st);
					entry.SetTimestamp(st);
					entry.SetValue(strQueryList[0]);
					ole += tspan;
					entryListVec[ii].push_back(entry);
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stStart) >= Project::Tools::SystemTimeToTime_t(stQueryMin) && Project::Tools::SystemTimeToTime_t(stEnd) < Project::Tools::SystemTimeToTime_t(stQueryMax))
			{
				COleDateTime ole(stStart);
				for (size_t i=0;i<tQueryList.size();++i)
				{
					SYSTEMTIME st;
					tQueryList[i].GetAsSystemTime(st);
					if (Project::Tools::SystemTimeToTime_t(st) >= Project::Tools::SystemTimeToTime_t(stStart) && Project::Tools::SystemTimeToTime_t(st) <= Project::Tools::SystemTimeToTime_t(stEnd))
					{
						if(entryListVec[ii].size()<nCount)
						{
							CRealTimeDataEntry entry;
							SYSTEMTIME st1;
							ole.GetAsSystemTime(st1);
							entry.SetTimestamp(st1);
							entry.SetValue(strQueryList[i]);
							ole += tspan;
							entryListVec[ii].push_back(entry);
						}
					}
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stStart) <= Project::Tools::SystemTimeToTime_t(stQueryMin) && Project::Tools::SystemTimeToTime_t(stEnd) >= Project::Tools::SystemTimeToTime_t(stQueryMax))
			{
				COleDateTime ole(stStart);
				int nFixFront = (int)(Project::Tools::SystemTimeToTime_t(stQueryMin)-Project::Tools::SystemTimeToTime_t(stStart))/(int)tspan.GetTotalSeconds();
				int nFixBack = (int)(Project::Tools::SystemTimeToTime_t(stEnd)-Project::Tools::SystemTimeToTime_t(stQueryMax))/(int)tspan.GetTotalSeconds();
				for (int i=0;i<nFixFront;++i)
				{
					if(entryListVec[ii].size()<nCount)
					{
						CRealTimeDataEntry entry;
						SYSTEMTIME st1;
						ole.GetAsSystemTime(st1);
						entry.SetTimestamp(st1);
						entry.SetValue(strQueryList[0]);
						ole += tspan;
						entryListVec[ii].push_back(entry);
					}
				}
				for (int j=0;j<tQueryList.size();++j)
				{	
					SYSTEMTIME st;
					tQueryList[j].GetAsSystemTime(st);
					if (Project::Tools::SystemTimeToTime_t(st) >= Project::Tools::SystemTimeToTime_t(stStart) && Project::Tools::SystemTimeToTime_t(st) <= Project::Tools::SystemTimeToTime_t(stEnd))
					{
						if(entryListVec[ii].size()<nCount)
						{
							CRealTimeDataEntry entry;
							SYSTEMTIME st1;
							ole.GetAsSystemTime(st1);
							entry.SetTimestamp(st1);
							entry.SetValue(strQueryList[j]);
							ole += tspan;
							entryListVec[ii].push_back(entry);
						}
					}
				}
				for (int m=0;m<nFixBack;++m)
				{
					if(entryListVec[ii].size()<nCount)
					{
						CRealTimeDataEntry entry;
						SYSTEMTIME st1;
						ole.GetAsSystemTime(st1);
						entry.SetTimestamp(st1);
						entry.SetValue(strQueryList[strQueryList.size()-1]);
						ole += tspan;
						entryListVec[ii].push_back(entry);
					}
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stEnd) >= Project::Tools::SystemTimeToTime_t(stQueryMax) 
				&& Project::Tools::SystemTimeToTime_t(stStart) < Project::Tools::SystemTimeToTime_t(stQueryMax)
				&& Project::Tools::SystemTimeToTime_t(stStart) > Project::Tools::SystemTimeToTime_t(stQueryMin))
			{
				COleDateTime ole(stStart);
				int nFixBack = (int)(Project::Tools::SystemTimeToTime_t(stEnd)-Project::Tools::SystemTimeToTime_t(stQueryMax))/(int)tspan.GetTotalSeconds();
				for (int i=0;i<tQueryList.size();++i)
				{
					SYSTEMTIME st;
					tQueryList[i].GetAsSystemTime(st);
					if (Project::Tools::SystemTimeToTime_t(st) >= Project::Tools::SystemTimeToTime_t(stStart) && Project::Tools::SystemTimeToTime_t(st) <= Project::Tools::SystemTimeToTime_t(stEnd))
					{
						if(entryListVec[ii].size()<nCount)
						{
							CRealTimeDataEntry entry;
							SYSTEMTIME st1;
							ole.GetAsSystemTime(st1);
							entry.SetTimestamp(st1);
							entry.SetValue(strQueryList[i]);
							ole += tspan;
							entryListVec[ii].push_back(entry);
						}
					}
				}
				for (int m=0;m<nFixBack;++m)
				{
					if(entryListVec[ii].size()<nCount)
					{
						CRealTimeDataEntry entry;
						SYSTEMTIME st1;
						ole.GetAsSystemTime(st1);
						entry.SetTimestamp(st1);
						entry.SetValue(strQueryList[strQueryList.size()-1]);
						ole += tspan;
						entryListVec[ii].push_back(entry);
					}
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stStart) <= Project::Tools::SystemTimeToTime_t(stQueryMin) 
				&& Project::Tools::SystemTimeToTime_t(stEnd) < Project::Tools::SystemTimeToTime_t(stQueryMax)
				&& Project::Tools::SystemTimeToTime_t(stEnd) > Project::Tools::SystemTimeToTime_t(stQueryMin))
			{
				COleDateTime ole(stStart);
				int nFixFront = (int)(Project::Tools::SystemTimeToTime_t(stQueryMin)-Project::Tools::SystemTimeToTime_t(stStart))/(int)tspan.GetTotalSeconds();
				for (int i=0;i<nFixFront;++i)
				{
					if(entryListVec[ii].size()<nCount)
					{
						CRealTimeDataEntry entry;
						SYSTEMTIME st1;
						ole.GetAsSystemTime(st1);
						entry.SetTimestamp(st1);
						entry.SetValue(strQueryList[0]);
						ole += tspan;
						entryListVec[ii].push_back(entry);
					}
				} 
				for (int i=0;i<tQueryList.size();++i)
				{
					SYSTEMTIME st;
					tQueryList[i].GetAsSystemTime(st);
					if (Project::Tools::SystemTimeToTime_t(st)>Project::Tools::SystemTimeToTime_t(stStart) && Project::Tools::SystemTimeToTime_t(st)<=Project::Tools::SystemTimeToTime_t(stEnd))
					{
						if(entryListVec[ii].size()<nCount)
						{
							CRealTimeDataEntry entry;
							SYSTEMTIME st1;
							ole.GetAsSystemTime(st1);
							entry.SetTimestamp(st1);
							entry.SetValue(strQueryList[i]);
							ole += tspan;
							entryListVec[ii].push_back(entry);
						}
					}
				}
			}
		}
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::UpdateLibInsertIntoFilestorage(const wstring strFilePathName, const wstring strFileName, const wstring strFileId)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	off_t		iSize	= 0;
	char*		pSql	= NULL;
	FILE*		pFile	= NULL;
	char*		pBuf	= NULL;
	size_t		n		= 0;
	char*		pEnd	= NULL;
	size_t		lSize	= 0;
	string strUtf8PathName;
	string strUtf8FileName;
	string strUtf8FileId;
	USES_CONVERSION;
	strUtf8PathName = T2A(strFilePathName.c_str());
	strUtf8FileName = T2A(strFileName.c_str());
	strUtf8FileId = T2A(strFileId.c_str());
	if ((pFile = fopen(strUtf8PathName.c_str(),"rb")) == NULL)
	{
		perror("fopen file");
		return false;
	}
	long cur_pos;
	long len;
	cur_pos = ftell(pFile);
	fseek(pFile, 0, SEEK_END);
	len = ftell(pFile);
	//将文件流的读取位置还原为原先的值
	fseek(pFile, cur_pos, SEEK_SET);
	if ((pBuf = new char[sizeof(char)*(len+1)]) == NULL)
	{
		return false;
	}
	iSize = len;
	if ((n = fread(pBuf, 1, iSize, pFile)) < 0)
	{
		SAFE_DELETE_ARRAY(pBuf);
		return false;
	}
	lSize = sizeof(char) * n * 2 + 256;
	pSql = new char[lSize];
	if (pSql == NULL)
	{
		SAFE_DELETE_ARRAY(pBuf);
		return false;
	}

	//删除选文件
	std::ostringstream sqlstream;
	string sqlstatement;
	sqlstream << "delete from filestorage where fileid = '" << strUtf8FileId << "';";
	sqlstatement = sqlstream.str();
	Execute(sqlstatement);


	sqlstream.str("");
	sqlstream << "insert into filestorage(fileid, filename, filedescription, reserve01, reserve02, reserve03, reserve04, reserve05, fileblob) values('"
		<< strUtf8FileId << "','" << strUtf8FileName << "', '', '', '', '', '', '', ";
	sqlstatement = sqlstream.str();
	strcpy_s(pSql, lSize, sqlstatement.c_str());

	pEnd = pSql;
	pEnd += strlen(pSql); //point sql tail
	*pEnd++ = '\'';
	pEnd += RealEscapeString(pEnd, pBuf, n);
	*pEnd++ = '\'';
	*pEnd++ = ')';

	sqlstatement = pSql;
	if (!Execute(sqlstatement, (unsigned int)(pEnd-pSql)))
	{
		string strErr = GetMysqlError();

		fclose(pFile);
		pFile = NULL;
		SAFE_DELETE_ARRAY(pSql);
		SAFE_DELETE_ARRAY(pBuf);
		return false;
	}

	fclose(pFile);
	pFile = NULL;
	SAFE_DELETE_ARRAY(pSql);
	SAFE_DELETE_ARRAY(pBuf);

	return true;
}

bool Beopdatalink::CCommonDBAccess::UpdateLibReloadIntoUnit02(const wstring strFileName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string	strUtf8FileName;
	Project::Tools::WideCharToUtf8(strFileName, strUtf8FileName);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate << "delete from `beopdata`.`unit02` where unitproperty04 = '" << strUtf8FileName << "';";
	sqlstatement = sqlstreamUpdate.str();
	Execute(sqlstatement);

	sqlstreamUpdate.str("");
	string strNeedSaveToDB = "0";

	sqlstreamUpdate << "insert into `beopdata`.`unit02`(unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05, unitproperty06, unitproperty07, unitproperty08, unitproperty09, unitproperty10, unitproperty11, unitproperty12, unitproperty13, unitproperty14, unitproperty15) value('', '', '7', '"
		<< strUtf8FileName << "', '', '', '', '', '', '', '', '', '', '', '0');";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::UpdateLibInsertIntoUnit02(const wstring strFileName, bool bNeedSaveToDB)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string	strUtf8FileName;
	Project::Tools::WideCharToUtf8(strFileName, strUtf8FileName);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate << "delete from `beopdata`.`unit02` where unitproperty04 = '" << strUtf8FileName << "';";
	sqlstatement = sqlstreamUpdate.str();
	Execute(sqlstatement);

	sqlstreamUpdate.str("");
	string strNeedSaveToDB = "0";
	if(bNeedSaveToDB)
		strNeedSaveToDB = "1";
	sqlstreamUpdate << "insert into `beopdata`.`unit02`(unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05, unitproperty06, unitproperty07, unitproperty08, unitproperty09, unitproperty10, unitproperty11, unitproperty12, unitproperty13, unitproperty14, unitproperty15) value('', '', '3', '"
		<< strUtf8FileName << "', '', '', '', '', '', '', '', '', '', '', '"<< strNeedSaveToDB  <<"');";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::UpdateLibInsertIntoUnit02( wstring strUpdateState, wstring strThreadName, wstring strDLLName, wstring strFilePath, wstring strImportTime,wstring strAuthor, wstring strPeriod, wstring strDllContent, wstring strRunStatus,wstring strVersion,wstring strDescription,wstring strDllOldName , bool bNeedSaveToDB)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string	strUpdateState_Utf8,strThreadName_Utf8,strDLLName_Utf8,strFilePath_Utf8,strDllOldName_Utf8, strNeedSaveToDB;
	Project::Tools::WideCharToUtf8(strThreadName, strThreadName_Utf8);
	Project::Tools::WideCharToUtf8(strUpdateState, strUpdateState_Utf8);
	Project::Tools::WideCharToUtf8(strDLLName, strDLLName_Utf8);
	Project::Tools::WideCharToUtf8(strFilePath, strFilePath_Utf8);
	Project::Tools::WideCharToUtf8(strDllOldName, strDllOldName_Utf8);

	if(bNeedSaveToDB)
		strNeedSaveToDB = "1";
	else
		strNeedSaveToDB = "0";

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate << "delete from `beopdata`.`unit02` where unitproperty04 = '" << strFilePath_Utf8 << "' and unitproperty01='" << strThreadName_Utf8 << "' and unitproperty02='" << strDLLName_Utf8  << "'  and unitproperty03='" << strUpdateState_Utf8 <<"';";
	sqlstatement = sqlstreamUpdate.str();
	Execute(sqlstatement);

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "insert into `beopdata`.`unit02`(unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05, unitproperty06, unitproperty07, unitproperty08, unitproperty09, unitproperty10, unitproperty11, unitproperty12, unitproperty13, unitproperty14, unitproperty15) value('"
		<< strThreadName_Utf8 << "','" << strDLLName_Utf8 << "','" << strUpdateState_Utf8 << "','" << strFilePath_Utf8 << "', '', '', '', '', '', '', '', '','', '" << strDllOldName_Utf8 << "', "<<"'"<< strNeedSaveToDB<<"');";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::UpdateHistoryDataPointName(const wstring& wstrTableName, const wstring& wstrOldName, const wstring& wstrNewName)
{
	if((wstrOldName.length()<=0) || (wstrNewName.length()<=0))
		return false;

	string strTableName = Project::Tools::WideCharToUtf8(wstrTableName.c_str());
	string strOldName = Project::Tools::WideCharToUtf8(wstrOldName.c_str());
	string strNewName = Project::Tools::WideCharToUtf8(wstrNewName.c_str());

	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate << "update "<< GetDBName()
		<<"."
		<< strTableName 
		<< " set pointname=" << strNewName
		<<" where pointname='"<<strOldName<<"';";
	sqlstatement = sqlstreamUpdate.str();
	return Execute(sqlstatement);

}

/*
	说明：更新所有历史数据库中的点名
	Param：
		wstrTableNameList：数据库中表名
		wstrOldNameList：原名列表
		wstrNewNameList：新名列表
*/
bool Beopdatalink::CCommonDBAccess::UpdateHistoryDataPointName( const vector<wstring>& wstrTableNameList, const vector<wstring>& wstrOldNameList, const vector<wstring>& wstrNewNameList )
{
	if((wstrOldNameList.size()<=0) ||  (wstrNewNameList.size()<=0))
		return false;
	if(wstrOldNameList.size() != wstrNewNameList.size())
		return false;
	
	string strTableName;
	string strOldName;
	string strNewName;

	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	std::ostringstream sqlstreamUpdate;
	string sqlstatement;
	
	for(int nTableCounnt=0; nTableCounnt<wstrTableNameList.size(); nTableCounnt++)
	{
		string strTableName = Project::Tools::WideCharToUtf8(wstrTableNameList[nTableCounnt].c_str());
		for(int i=0; i<wstrOldNameList.size(); i++)
		{
			string strOldName = Project::Tools::WideCharToUtf8(wstrOldNameList[i].c_str());
			string strNewName = Project::Tools::WideCharToUtf8(wstrNewNameList[i].c_str());
			sqlstreamUpdate.str("");
			sqlstreamUpdate << "update "<< GetDBName()
				<<"."
				<< strTableName 
				<< " set pointname='" << strNewName<<"'"
				<<" where pointname='"<<strOldName<<"';";
			sqlstatement = sqlstreamUpdate.str();
			Execute(sqlstatement);
		}
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::IsTableExist( const wstring strTableName )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string	strTableNameTemp;
	Project::Tools::WideCharToUtf8(strTableName, strTableNameTemp);

	char sql[MAX_PATH] = {0};
	sprintf_s(sql, sizeof(sql), "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_SCHEMA='beopdata' and TABLE_NAME='%s'",strTableNameTemp.c_str());

	MYSQL_RES* result = QueryAndReturnResult(sql);
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int nLength = static_cast<int>(GetRowNum(result));
	if (0 == nLength)
	{
		return false;
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::UpdateRemark( const wstring strRemark, const wstring& strPointName, const SYSTEMTIME st )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strtime = Project::Tools::SystemTimeToString(st);
	string strRemarkTemp;
	Project::Tools::WideCharToUtf8(strRemark, strRemarkTemp);
	string strPointNameTemp;
	Project::Tools::WideCharToUtf8(strPointName, strPointNameTemp);

	std::ostringstream sqlstreamUpdate;
	sqlstreamUpdate << "update "
		<< GetDBName()
		<<"."
		<< tablename_warningrecord 
		<< " set confirmeduser = '" << strRemarkTemp << "'"
		<< " where `time`<='" << strtime << "' and `endtime`>='" << strtime <<"'"
		<<" and bindpointname='"<<strPointNameTemp<<"'";

	string sqlstatement = sqlstreamUpdate.str();

	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::ReadRemark( wstring& strRemark, const wstring& strPointName, const SYSTEMTIME st )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strtime = Project::Tools::SystemTimeToString(st);
	string strPointNameTemp;
	Project::Tools::WideCharToUtf8(strPointName, strPointNameTemp);

	std::ostringstream sqlstream;
	sqlstream << "select confirmeduser from "
		<< GetDBName()
		<<"."
		<< tablename_warningrecord 
		<< " where `time`<='" << strtime << "' and `endtime`>='" << strtime <<"'"
		<<" and bindpointname='"<<strPointNameTemp<<"'";

	string sqlstatement = sqlstream.str();

	MYSQL_RES* result = QueryAndReturnResult(sqlstatement.c_str());
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int nLength = static_cast<int>(GetRowNum(result));
	if (0 == nLength)
	{
		return false;
	}
	MYSQL_ROW	row = NULL;
	while (row = FetchRow(result))
	{
		strRemark = Project::Tools::UTF8ToWideChar(row[0]);
	}
	return true;
}

int     Beopdatalink::CCommonDBAccess::GetMaxNoteID()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	int nMaxID = 10000;

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;
	std::ostringstream sqlstream;
	sqlstream << "select max(unitproperty01) from " 
		<< GetDBName() 
		<< ".unit07"
		<< " where unitproperty02 = 'note'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return nMaxID;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return nMaxID;
	}
	MYSQL_ROW  row = NULL;

	if(row = FetchRow(result) )
	{
		wstring strID = Project::Tools::UTF8ToWideChar(row[0]);
		nMaxID = _wtoi(strID.c_str());
	}

	if(nMaxID<10000)
		nMaxID = 10000;

	return nMaxID;
}

int		Beopdatalink::CCommonDBAccess::GetMaxBenchmarkLineID()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	int nMaxID = 10000;

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;
	std::ostringstream sqlstream;
	sqlstream << "select max(unitproperty01) from " 
		<< GetDBName() 
		<< ".unit07"
		<< " where unitproperty02 = 'benchmarkline'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return nMaxID;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return nMaxID;
	}
	MYSQL_ROW  row = NULL;

	if(row = FetchRow(result) )
	{
		wstring strID = Project::Tools::UTF8ToWideChar(row[0]);
		nMaxID = _wtoi(strID.c_str());
	}

	if(nMaxID<10000)
		nMaxID = 10000;

	return nMaxID;
}

int     Beopdatalink::CCommonDBAccess::InsertNote(BoardNoteInfo& noteInfo)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;
	
	noteInfo.nID = GetMaxNoteID() +1;


	string	strUserName_utf8;
	Project::Tools::WideCharToUtf8(noteInfo.strUserName, strUserName_utf8);

	string	strContent_utf8;
	Project::Tools::WideCharToUtf8(noteInfo.strContent, strContent_utf8);

	string	strFontName_utf8;
	Project::Tools::WideCharToUtf8(noteInfo.strNoteTitle, strFontName_utf8);

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "insert into `beopdata`.`unit07`(unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05, unitproperty06, unitproperty07, unitproperty08, unitproperty09, unitproperty10, unitproperty11, unitproperty12, unitproperty13, unitproperty14, unitproperty15) "
		<< "value('"<< noteInfo.nID <<"', 'note', '"<< strUserName_utf8 <<"', '"<< noteInfo.nOfBoardID << "', '"<< noteInfo.fPosX << ","<< noteInfo.fPosY << "', '"<< noteInfo.fWidth << ","<< noteInfo.fHeight << "', '"<< noteInfo.fArrowEndX << ","<< noteInfo.fArrowEndY << "', '"<< (noteInfo.bHasArrow?1:0) << "', '"<< noteInfo.nStyle << "', '"<< strContent_utf8 << "', '"<< noteInfo.nFontSize << "', '"<< strFontName_utf8 << "','"<<noteInfo.nArrowStyle <<"','"<< noteInfo.nArrowWidth <<"','"<< noteInfo.dwTextColor <<"');";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return noteInfo.nID;
}


bool    Beopdatalink::CCommonDBAccess::MoveNote(int nNoteID, const double& fPosX, const double& fPosY)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "update `beopdata`.`unit07` set unitproperty05 = '"<< fPosX <<"," << fPosY <<"' where unitproperty01 = '"<< nNoteID <<"';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;

}

bool    Beopdatalink::CCommonDBAccess::ResizeNote(int nNoteID, const double& fWidth, const double& fHeight)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "update `beopdata`.`unit07` set unitproperty06 = '"<< fWidth <<"," << fHeight <<"' where unitproperty01 = '"<< nNoteID <<"';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;

}

bool    Beopdatalink::CCommonDBAccess::ModifyNoteProperty(int nNoteID, const BoardNoteInfo& noteInfo)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	string	strContent_utf8;
	Project::Tools::WideCharToUtf8(noteInfo.strContent, strContent_utf8);

	string	strFontName_utf8;
	Project::Tools::WideCharToUtf8(noteInfo.strNoteTitle, strFontName_utf8);

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "update `beopdata`.`unit07` set unitproperty07 = '"<< noteInfo.fArrowEndX <<","<<noteInfo.fArrowEndY<<"', unitproperty08 = '" << (noteInfo.bHasArrow?1:0)
		<<"', unitproperty09 = '"<< noteInfo.nStyle <<"', unitproperty10 = '"<< strContent_utf8 <<"', unitproperty11 = '"<< noteInfo.nFontSize <<"', unitproperty12 = '"<< strFontName_utf8<<"', unitproperty13 = '"<< noteInfo.nArrowStyle <<"', unitproperty14 = '"<<noteInfo.nArrowWidth <<"', unitproperty15 = '"<<noteInfo.dwTextColor <<"' where unitproperty01 = '"<< nNoteID <<"' and unitproperty02 = 'note';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}


bool    Beopdatalink::CCommonDBAccess::RemoveNote(int nNoteID)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "delete from `beopdata`.`unit07` where unitproperty01 = '"<< nNoteID <<"' and unitproperty02 = 'note';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

bool    Beopdatalink::CCommonDBAccess::ReadNoteList(const int &nBoardID, vector<BoardNoteInfo>& noteList)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;

	sqlstream << "select * from `beopdata`.`unit07` where unitproperty04 = '"<< nBoardID <<"' and unitproperty02 = 'note';";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return 0;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return 0;
	}
	MYSQL_ROW  row = NULL;

	while(row = FetchRow(result) )
	{
		BoardNoteInfo noteInfo;
		noteInfo.nID = atoi(row[0]);
		noteInfo.strUserName = Project::Tools::UTF8ToWideChar(row[2]);
		noteInfo.nOfBoardID = nBoardID;
		wstring strPos = Project::Tools::UTF8ToWideChar(row[4]);
		vector<wstring> vecPos;
		Project::Tools::SplitStringByChar(strPos.c_str(), L',', vecPos);
		if(vecPos.size()==2)
		{
			noteInfo.fPosX = _wtof(vecPos[0].c_str());
			noteInfo.fPosY = _wtof(vecPos[1].c_str());
		}

		wstring strWH = Project::Tools::UTF8ToWideChar(row[5]);
		vecPos.clear();
		Project::Tools::SplitStringByChar(strWH.c_str(), L',', vecPos);
		if(vecPos.size()==2)
		{
			noteInfo.fWidth = _wtof(vecPos[0].c_str());
			noteInfo.fHeight = _wtof(vecPos[1].c_str());
		}

		wstring strArrowEndXY = Project::Tools::UTF8ToWideChar(row[6]);
		vecPos.clear();
		Project::Tools::SplitStringByChar(strArrowEndXY.c_str(), L',', vecPos);
		if(vecPos.size()==2)
		{
			noteInfo.fArrowEndX = _wtof(vecPos[0].c_str());
			noteInfo.fArrowEndY = _wtof(vecPos[1].c_str());
		}

		int nHasArrow = atoi(row[7]);
		noteInfo.bHasArrow = (nHasArrow==1);
		noteInfo.nStyle = atoi(row[8]);
		noteInfo.strContent = Project::Tools::UTF8ToWideChar(row[9]);
		noteInfo.nFontSize = atoi(row[10]);
		noteInfo.strNoteTitle = Project::Tools::UTF8ToWideChar(row[11]);
		noteInfo.nArrowStyle = atoi(row[12]);
		noteInfo.nArrowWidth = atoi(row[13]);
		noteInfo.dwTextColor = atoi(row[14]);
		noteList.push_back(noteInfo);
	}


	return true;
}


//for arrow
bool    Beopdatalink::CCommonDBAccess::MoveArrow(int nNoteID, const double& fArrowEndX, const double & fArrowEndY)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "update `beopdata`.`unit07` set unitproperty07 = '"<< fArrowEndX <<"," << fArrowEndY <<"' where unitproperty01 = '"<< nNoteID <<"';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}


//add benchmark line
/** 添加一根辅助线  */
int    Beopdatalink::CCommonDBAccess::AddBechmarkLine(BoardBenchmarkLineInfo & lineInfo)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;


	lineInfo.nID =  GetMaxBenchmarkLineID() +1;

	string	strPointName_utf8;
	Project::Tools::WideCharToUtf8(lineInfo.strPointName, strPointName_utf8);

	string strPointDescription_utf8;
	Project::Tools::WideCharToUtf8(lineInfo.strPointDescription, strPointDescription_utf8);


	sqlstreamUpdate.str("");
	sqlstreamUpdate << "insert into `beopdata`.`unit07`(unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05, unitproperty06, unitproperty07, unitproperty08, unitproperty09, unitproperty10, unitproperty11, unitproperty12, unitproperty13, unitproperty14, unitproperty15) "
		<< "value('"<< lineInfo.nID <<"', 'benchmarkline', '"<< lineInfo.nOfBoardID <<"', '"<< strPointName_utf8 << "', '"<< lineInfo.fYMin << ","<< lineInfo.fYMax << "','"<< lineInfo.nStyle << "', '"<< lineInfo.nWidth << "','"<< lineInfo.dwColor << "','"<<strPointDescription_utf8 <<"','','','','','','');";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return lineInfo.nID;
}

/** 修改辅助线的量程范围  */
bool    Beopdatalink::CCommonDBAccess::ModifyBechmarkLine(int nLineID, const BoardBenchmarkLineInfo& benchmarkLineInfo)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	string	strPointName_utf8;
	Project::Tools::WideCharToUtf8(benchmarkLineInfo.strPointName, strPointName_utf8);

	string strPointDescription_utf8;
	Project::Tools::WideCharToUtf8(benchmarkLineInfo.strPointDescription, strPointDescription_utf8);

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "update `beopdata`.`unit07` set unitproperty05 = '"<< benchmarkLineInfo.fYMin <<","<<benchmarkLineInfo.fYMax<<"', unitproperty06 = '" << benchmarkLineInfo.nStyle
		<<"', unitproperty07 = '"<< benchmarkLineInfo.nWidth <<"', unitproperty08 = '"<< benchmarkLineInfo.dwColor <<"', unitproperty09 = '"<< strPointDescription_utf8 <<"' where unitproperty01 = '"<< nLineID <<"' and unitproperty02 = 'benchmarkline';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

/** 移除一根辅助线  */
bool    Beopdatalink::CCommonDBAccess::RemoveBenchmarkLine(int nLineID)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "delete from `beopdata`.`unit07` where unitproperty01 = '"<< nLineID <<"' and unitproperty02 = 'benchmarkline';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

/** 读取所有辅助线清单  */
bool    Beopdatalink::CCommonDBAccess::ReadBenchmarkLineList(const int &nBoardID, vector<BoardBenchmarkLineInfo>& benchmarklineList)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;

	sqlstream << "select * from `beopdata`.`unit07` where unitproperty03 = '"<< nBoardID <<"' and unitproperty02 = 'benchmarkline';";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return 0;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return 0;
	}
	MYSQL_ROW  row = NULL;

	while(row = FetchRow(result) )
	{
		BoardBenchmarkLineInfo lineInfo;
		lineInfo.nID = atoi(row[0]);
		lineInfo.nOfBoardID = atoi(row[2]);
		lineInfo.strPointName = Project::Tools::UTF8ToWideChar(row[3]);

		wstring strPos = Project::Tools::UTF8ToWideChar(row[4]);
		vector<wstring> vecPos;
		Project::Tools::SplitStringByChar(strPos.c_str(), L',', vecPos);
		if(vecPos.size()==2)
		{
			lineInfo.fYMin = _wtof(vecPos[0].c_str());
			lineInfo.fYMax = _wtof(vecPos[1].c_str());
		}

		lineInfo.nStyle = atoi(row[5]);

		lineInfo.nWidth = atoi(row[6]);

		lineInfo.dwColor = atoi(row[7]);
		lineInfo.strPointDescription =  Project::Tools::UTF8ToWideChar(row[8]);

		benchmarklineList.push_back(lineInfo);
	}


	return true;
}

bool Beopdatalink::CCommonDBAccess::AddTrendLine( TrendLineInfo lineInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;
	

	string	strPointName_utf8;
	Project::Tools::WideCharToUtf8(lineInfo.strPointName, strPointName_utf8);

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "insert into `beopdata`.`unit07`(unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05, unitproperty06, unitproperty07, unitproperty08, unitproperty09, unitproperty10, unitproperty11, unitproperty12, unitproperty13, unitproperty14, unitproperty15) "
		<< "value('"<< strPointName_utf8 <<"', 'trendline', '"<< lineInfo.nOfBoardID <<"', '"<<lineInfo.dwColor << "','','','','','','','','','','','');";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;

}

bool Beopdatalink::CCommonDBAccess::ModifyTrendLine( TrendLineInfo lineInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	string	strPointName_utf8;
	Project::Tools::WideCharToUtf8(lineInfo.strPointName, strPointName_utf8);	

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "update `beopdata`.`unit07` set unitproperty04 = '"<< lineInfo.dwColor
		<<"' where unitproperty01 = '"<< strPointName_utf8 <<"' and unitproperty02 = 'trendline' and  unitproperty03 = '"
		<<lineInfo.nOfBoardID<<"';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::RemoveTrendLine( wstring strPointName,int BoardID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;
	string	strPointName_utf8;
	Project::Tools::WideCharToUtf8(strPointName, strPointName_utf8);

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "delete from `beopdata`.`unit07` where unitproperty01 = '"<< strPointName_utf8 
		<<"' and unitproperty02 = 'trendline' and unitproperty03 = '"
		<<BoardID<<"';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadTrendLineList( const int BoardID, vector<TrendLineInfo>& trendLineList )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;

	sqlstream << "select * from `beopdata`.`unit07` where unitproperty03 = '"<< BoardID <<"' and unitproperty02 = 'trendline';";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return 0;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return 0;
	}
	MYSQL_ROW  row = NULL;

	while(row = FetchRow(result) )
	{
		TrendLineInfo lineInfo;
		lineInfo.strPointName = Project::Tools::UTF8ToWideChar(row[0]);
		lineInfo.nOfBoardID = atoi(row[2]);	
		lineInfo.dwColor = atoi(row[3]);
		trendLineList.push_back(lineInfo);
	}


	return true;
}

int Beopdatalink::CCommonDBAccess::GetMaxBarLineID()
{	
	{
		Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
		SetConnectionCharacterSet(MYSQL_UTF8);

		int nMaxID = 10000;

		std::ostringstream sqlstreamUpdate;
		string sqlstatement;
		std::ostringstream sqlstream;
		sqlstream << "select max(unitproperty01) from " 
			<< GetDBName() 
			<< ".unit07"
			<< " where unitproperty02 = 'barline'";

		MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
		if (result == NULL){
			return nMaxID;
		}
		Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

		int length = static_cast<int>(GetRowNum(result));
		if (0 == length){
			return nMaxID;
		}
		MYSQL_ROW  row = NULL;

		if(row = FetchRow(result) )
		{
			wstring strID = Project::Tools::UTF8ToWideChar(row[0]);
			nMaxID = _wtoi(strID.c_str());
		}

		if(nMaxID<10000)
			nMaxID = 10000;

		return nMaxID;
	} 
	
}



bool Beopdatalink::CCommonDBAccess::AddBarLine( BarLineInfo & lineInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	if (BAR_LINE_ID_INCRE < 0)
	{
		BAR_LINE_ID_INCRE = GetMaxBarLineID();
	} 	
	lineInfo.nID = ++BAR_LINE_ID_INCRE;

	string	strPointName_utf8;
	Project::Tools::WideCharToUtf8(lineInfo.strPointName, strPointName_utf8);

	int tmpshow = lineInfo.bShow;

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "insert into `beopdata`.`unit07`(unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05, unitproperty06, unitproperty07, unitproperty08, unitproperty09, unitproperty10, unitproperty11, unitproperty12, unitproperty13, unitproperty14, unitproperty15) "
		<< "value('"<< lineInfo.nID <<"', 'barline', '"<< lineInfo.nOfBoardID <<"', '"<< lineInfo.dwColor<<"','";
	
	int end  = lineInfo.valuelist.size()-1;
	for (int i = 0;i<end;i++ )
	{
		sqlstreamUpdate <<lineInfo.valuelist[i]<<",";
	}
	sqlstreamUpdate<<lineInfo.valuelist[end]
	<<"', '"<<strPointName_utf8<<"','"<<tmpshow<<"','','','','','','','','');";

	sqlstatement = sqlstreamUpdate.str();
	
	//TRACE(_T("jl0904 %s \n"),sqlstatement);	
	

	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadBarLineList( int nBoardID, vector<BarLineInfo> &barLineList )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;

	sqlstream << "select * from `beopdata`.`unit07` where unitproperty03 = '"<< nBoardID <<"' and unitproperty02 = 'barline';";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return 0;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return 0;
	}
	MYSQL_ROW  row = NULL;

	while(row = FetchRow(result) )
	{
		BarLineInfo lineInfo;
		lineInfo.nID = atoi(row[0]);
		lineInfo.nOfBoardID = atoi(row[2]);
		lineInfo.dwColor = atoi(row[3]);

		wstring strValueList = Project::Tools::UTF8ToWideChar(row[4]);		
		vector<wstring> vecPos;
		vecPos.clear();
		Project::Tools::SplitStringByChar(strValueList.c_str(), L',', vecPos);
		lineInfo.valuelist.clear();
		for (int i = 0;i< vecPos.size();i++)
		{
			lineInfo.valuelist.push_back(_wtof(vecPos[i].c_str()));
		}

		lineInfo.strPointName = Project::Tools::UTF8ToWideChar(row[5]);
		lineInfo.bShow =  atoi(row[6]);
		barLineList.push_back(lineInfo);
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::ModifyBarLine( int nlineID,const BarLineInfo& lineInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;	

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "update `beopdata`.`unit07` set unitproperty04 = '"<< lineInfo.dwColor
		<<"',unitproperty05 = '";

	int end  = lineInfo.valuelist.size()-1;
	for (int i = 0;i<end;i++ )
	{
		sqlstreamUpdate <<lineInfo.valuelist[i]<<",";
	}
	sqlstreamUpdate<<lineInfo.valuelist[end]<<"', unitproperty07 = '";

	if (lineInfo.bShow)
	{
		sqlstreamUpdate<<1;
	} 
	else
	{
		sqlstreamUpdate<<0;
	}
		sqlstreamUpdate<<"' where unitproperty01 = '"<< lineInfo.nID <<"' and unitproperty02 = 'barline';";
	
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;

}

bool Beopdatalink::CCommonDBAccess::RemoveBarLine( int nLineID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstreamUpdate;
	string sqlstatement;

	sqlstreamUpdate.str("");
	sqlstreamUpdate << "delete from `beopdata`.`unit07` where unitproperty01 = '"<< nLineID <<"' and unitproperty02 = 'barline';";
	sqlstatement = sqlstreamUpdate.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}


bool Beopdatalink::CCommonDBAccess::ModifyServerUserInfo( wstring strUser, wstring strPwd, wstring strRightLevel )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	string sqlstatement;
	sqlstream.str("");

	string strUser_Utf8, strPwd_Utf8, strRightLevel_Utf8;
	Project::Tools::WideCharToUtf8(strUser,strUser_Utf8);
	Project::Tools::WideCharToUtf8(strPwd,strPwd_Utf8);
	Project::Tools::WideCharToUtf8(strRightLevel,strRightLevel_Utf8);

	sqlstream << "update "
		<< GetDBName()
		<< "."
		<< tablename_user
		<< " set userpwd='" << strPwd_Utf8 << "',userofrole='" << strRightLevel_Utf8
		<< "' where username='" << strUser_Utf8 << "';";
	sqlstatement = sqlstream.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::AddServerUserInfo( int nUserID, wstring strUser, wstring strPwd, wstring strRightLevel )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	string sqlstatement;
	sqlstream.str("");

	string strUser_Utf8, strPwd_Utf8, strRightLevel_Utf8;
	Project::Tools::WideCharToUtf8(strUser,strUser_Utf8);
	Project::Tools::WideCharToUtf8(strPwd,strPwd_Utf8);
	Project::Tools::WideCharToUtf8(strRightLevel,strRightLevel_Utf8);

	sqlstream << "insert into "
		<< GetDBName()
		<< "."
		<< tablename_user
		<< " (userid,username,userpwd,userofrole) values ('"
		<< nUserID << "','" << strUser_Utf8  << "','" << strPwd_Utf8 
		<< "','" << strRightLevel_Utf8 << "');";
	sqlstatement = sqlstream.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::DelServerUserInfo( int nIndex )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	string sqlstatement;
	sqlstream.str("");
	sqlstream << "delete from beopuser where userid = '"<< nIndex <<"';";
	sqlstatement = sqlstream.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::GetLogDatabaseExist()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = \'domlog\'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	

	return true;
}

bool Beopdatalink::CCommonDBAccess::GetServerUserInfo( vector<UserInfo>& vecUsers )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	vecUsers.clear();
	std::ostringstream sqlstream;
	sqlstream << "select * from "
		<< GetDBName()
		<< "."
		<< tablename_user
		<< " order by userid";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return true;
	}
	vecUsers.reserve(length);
	MYSQL_ROW  row = NULL;
	UserInfo user;
	while(row = FetchRow(result) ){
		user.nUserID = atoi(row[0]);
		user.strUserName =  Project::Tools::UTF8ToWideChar(row[1]);
		user.strUserPsw =  Project::Tools::UTF8ToWideChar(row[2]);
		user.strRoleName =  Project::Tools::UTF8ToWideChar(row[10]);		
		vecUsers.push_back(user);
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::CheckAndInsertDefaultUserInfo()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	bool bResult = true;

	//创建beoprole
	std::ostringstream sqlstream;
	sqlstream << "CREATE TABLE IF NOT EXISTS "
		<< GetDBName()
		<< "."
		<< tablename_role
		<< " (\
		   `roleid` varchar(255) DEFAULT NULL,\
		   `rolename` varchar(255) DEFAULT NULL,\
		   `roledesc` varchar(255) DEFAULT NULL,\
		   `rolecreatedt` varchar(255) DEFAULT NULL,\
		   `rolestatus` varchar(255) DEFAULT NULL,\
		   `rolerightbasic` text,\
		   `rolerightpage` text,\
		   `rolerightwritepoint` text,\
		   `roleright1` text,\
		   `roleright2` text,\
		   `roleright3` text,\
		   `roleright4` text,\
		   `roleright5` text,\
		   `unitproperty01` varchar(255) DEFAULT NULL,\
		   `unitproperty02` varchar(255) DEFAULT NULL,\
		   `unitproperty03` varchar(255) DEFAULT NULL,\
		   `unitproperty04` varchar(255) DEFAULT NULL,\
		   `unitproperty05` varchar(255) DEFAULT NULL\
		   ) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
	bResult = Execute(sqlstream.str().c_str());

	//创建beopuser
	sqlstream.str("");
	sqlstream << "CREATE TABLE IF NOT EXISTS "
		<< GetDBName()
		<< "."
		<< tablename_user
		<< " (\
		`userid` varchar(255) DEFAULT NULL,\
		`username` varchar(255) DEFAULT NULL,\
		`userpwd` varchar(255) DEFAULT NULL,\
		`userfullname` varchar(255) DEFAULT NULL,\
		`usersex` varchar(255) DEFAULT NULL,\
		`usermobile` varchar(255) DEFAULT NULL,\
		`useremail` varchar(255) DEFAULT NULL,\
		`usercreatedt` varchar(255) DEFAULT NULL,\
		`userstatus` varchar(255) DEFAULT NULL,\
		`userpic` longblob,\
		`userofrole` varchar(255) DEFAULT NULL,\
		`unitproperty01` varchar(255) DEFAULT NULL,\
		`unitproperty02` varchar(255) DEFAULT NULL,\
		`unitproperty03` varchar(255) DEFAULT NULL,\
		`unitproperty04` varchar(255) DEFAULT NULL,\
		`unitproperty05` varchar(255) DEFAULT NULL\
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
	bResult = Execute(sqlstream.str().c_str());

	//查找用户表为空，则插入
	bool bExist = true;
	sqlstream.str("");	
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_user
		<< " where username = 'admin'";
	
	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		bExist = false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		bExist =  false;
	}

	MYSQL_ROW  row = NULL;
	bool bPswNot111 = false;
	while(row = FetchRow(result) ){		
		wstring strUserPsw =  Project::Tools::UTF8ToWideChar(row[2]);
		if(strUserPsw != L"111")
			bPswNot111 = true;
	}

	if(!bExist)
	{
		sqlstream.str("");
		sqlstream << "insert into "
			<< GetDBName() 
			<< "."
			<< tablename_user
			<< " (userid,username,userpwd,userofrole) values ('0','admin','111','3'),('1','operator','111','2'),('2','guest','111','1');";
		bResult = Execute(sqlstream.str().c_str());
	}
	else
	{
		if(bPswNot111)
		{
			sqlstream.str("");
			sqlstream << "update "
				<< GetDBName() 
				<< "."
				<< tablename_user
				<< " set userpwd='111' where username='admin';";			
			bResult = Execute(sqlstream.str().c_str());
		}
	}
	return bResult;
}

bool Beopdatalink::CCommonDBAccess::ExportTableToCSV( const wstring strTableName, const wstring strCSVPath )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strTableName_Utf8, strCSVPath_Utf8;

	Project::Tools::WideCharToUtf8(strCSVPath,strCSVPath_Utf8);
	Project::Tools::WideCharToUtf8(strTableName,strTableName_Utf8);
	//
	std::ostringstream sqlstream;
	sqlstream << "select * from "
		<< GetDBName()
		<< "."
		<< tablename_realtimedata_input
		<< " into outfile '"
		<< strCSVPath_Utf8
		<< "' fields terminated by ',' optionally enclosed by '\"' escaped by '\"' lines terminated by '\r\n'";

	return Execute(sqlstream.str().c_str());
}

bool Beopdatalink::CCommonDBAccess::ImportCSVToTable( const wstring strTableName, const wstring strCSVPath )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strTableName_Utf8, strCSVPath_Utf8;

	strCSVPath_Utf8 = Project::Tools::WideCharToAnsi(strCSVPath.c_str());
	Project::Tools::WideCharToUtf8(strTableName,strTableName_Utf8);
	//
	std::ostringstream sqlstream;
	sqlstream << "delete from "
		<< GetDBName()
		<< "."
		<< tablename_realtimedata_input;
	Execute(sqlstream.str().c_str());

	sqlstream.str("");
	sqlstream << "load data local infile '"
		<< strCSVPath_Utf8
		<< "' into table "
		<< GetDBName()
		<< "."
		<< tablename_realtimedata_input
		<< " character set utf8 fields terminated by ',' optionally enclosed by '\"' escaped by '\"' lines terminated by '\r\n'";

	bool bResult = Execute(sqlstream.str().c_str());
	if(!bResult)
	{
		string strErr = GetMysqlError();
	}
	return bResult;
}

bool Beopdatalink::CCommonDBAccess::ReadLogByFilter( vector< pair<wstring,wstring> >& loglist, const SYSTEMTIME& start, const SYSTEMTIME& end, const wstring& strLogFilter )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);

	string str_start = Project::Tools::SystemTimeToString(start);
	string str_end = Project::Tools::SystemTimeToString(end);

	char buff[64];
	sprintf_s(buff, "log_%d_%02d_%02d", start.wYear, start.wMonth, start.wDay);
	string newtablename = buff;

	string strLogFilter_Utf8;
	Project::Tools::WideCharToUtf8(strLogFilter,strLogFilter_Utf8);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< newtablename
		<< " where time >= '" << str_start << "' and time <= '" << str_end << "' and loginfo like '%"
		<<	strLogFilter_Utf8
		<< "%' order by time";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	loglist.reserve(length);
	MYSQL_ROW  row = NULL;

	while(row = FetchRow(result) ){
		wstring logtime = Project::Tools::UTF8ToWideChar(row[0]);
		string str_mb = Project::Tools::Utf8ToMultiByte(row[1]);
		wstring loginfo = Project::Tools::AnsiToWideChar(str_mb.data());
		loglist.push_back(make_pair(logtime,loginfo));
	}

	return true;
}

wstring Beopdatalink::CCommonDBAccess::ReadOrCreateCoreDebugItemValue_Defalut( wstring strItemName,wstring strItemValue /*= L"0"*/ )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strItemName_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< ".unit01 where unitproperty01 = '"
		<< strItemName_utf8 << "'";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return L"";
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length)
	{
		sqlstream.str("");
		string strItemValue_utf8;
		Project::Tools::WideCharToUtf8(strItemValue, strItemValue_utf8);
		sqlstream << "insert into  "
			<< GetDBName() 
			<< ".unit01"
			<< " value " << "('" <<strItemName_utf8 << "', '" << strItemValue_utf8 <<"','','','','','','','','','','','','','')";

		string sqlstatement = sqlstream.str();
		bool bresult =  Execute(sqlstatement);		

		return strItemValue;
	}

	MYSQL_ROW  row = NULL;
	if(row = FetchRow(result) )
	{
		return Project::Tools::UTF8ToWideChar(row[1]);
	}

	return L"0";
}

//julian 0824
/*
wstring Beopdatalink::CCommonDBAccess::ReadGlobalMysqlVariable_Defalut( wstring strItemName )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	

	string strItemName_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);
	
	char strItemValue[20];

	char line[200],key[50];
	FILE *fp=fopen("D:/Program Files (x86)/MySQL/MySQL Server 5.1/my.ini","r");
	if(fp==NULL)
		return L"";
	while(!feof(fp))
	{
		fgets(line,sizeof(line),fp);
		sscanf(line,"%s",key);
		if(key[0]=='#')
			continue;
		if(strcmp(key,strItemName_utf8.c_str())==0)
		{
			sscanf(line,"%*s%*s%s",strItemValue);
			string temp1(strItemValue);
			CString temp2=temp1.c_str();
			wstring str=temp2.GetString();
			return str;
		}

	}
	return L"";

	
	
}
*/

wstring Beopdatalink::CCommonDBAccess::ReadGlobalMysqlVariable_Defalut( wstring strItemName )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	

	string strItemName_utf8;
	Project::Tools::WideCharToUtf8(strItemName, strItemName_utf8);

	std::ostringstream sqlstream;
	sqlstream <<"select @@global."
		<<strItemName_utf8;
	string sqlstatement=sqlstream.str();

	MYSQL_RES* result = QueryAndReturnResult(sqlstatement.c_str());
	if (result == NULL)
	{
		return L"";
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	
	MYSQL_ROW  row = NULL;
	if(row = FetchRow(result) )
	{
		//int nValue = ATOI(row[0]);
		return Project::Tools::UTF8ToWideChar(row[0]);
	}
	
}

bool Beopdatalink::CCommonDBAccess::ReadRealTimeDataInputValue(const vector<wstring>& vecPtName, map<wstring, float>& mapRes)
{
	if(vecPtName.size()<=0)
	{
		return true;
	}
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);


	wstring wstrName(L"(");
	vector<wstring>::const_iterator iter;
	iter=vecPtName.begin();
	wstrName += L"'" + *iter + L"'";
	iter++;
	for (; iter!=vecPtName.end(); iter++)
	{
		wstrName += L",'" + *iter + L"'";
	}
	wstrName += L")";

	string strItemName_utf8;
	Project::Tools::WideCharToUtf8(wstrName, strItemName_utf8);

	std::ostringstream sqlstream;
	sqlstream << "select pointname, pointvalue from " 
		<< GetDBName() 
		<< ".realtimedata_input where pointname in "
		<< strItemName_utf8 << ";";

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (NULL == result)
	{
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	mapRes.clear();
	MYSQL_ROW  row = NULL;
	while (row = FetchRow(result))
	{
		mapRes[Project::Tools::UTF8ToWideChar(row[0])] = ATOF(row[1]);
	}

	return true;
}

//end


bool Beopdatalink::CCommonDBAccess::GetCoreVersion(wstring &strCoreVersion)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);


	//查core 版本
	std::ostringstream sqlstream;
	sqlstream <<"select unitproperty03 from unit01 where unitproperty01 = 'domcore' and unitproperty02 = 'version'; " ;
	string sqlstatement=sqlstream.str();

	MYSQL_RES* result = QueryAndReturnResult(sqlstatement.c_str());
	if (result == NULL)
	{
		return L"";
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));

	MYSQL_ROW  row = NULL;
	if(row = FetchRow(result) )
	{
		//int nValue = ATOI(row[0]);
		strCoreVersion =  Project::Tools::UTF8ToWideChar(row[0]);
		return true;
	}
	return false;
}
bool Beopdatalink::CCommonDBAccess::GetLogicVersion(wstring &strLogicVersion)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);


	//查core 版本
	std::ostringstream sqlstream;
	sqlstream <<"select unitproperty02 from unit01 where unitproperty01 = 'logicversion'; " ;
	string sqlstatement=sqlstream.str();

	MYSQL_RES* result = QueryAndReturnResult(sqlstatement.c_str());
	if (result == NULL)
	{
		return L"";
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));

	MYSQL_ROW  row = NULL;
	if(row = FetchRow(result) )
	{
		//int nValue = ATOI(row[0]);
		strLogicVersion =  Project::Tools::UTF8ToWideChar(row[0]);
		return true;
	}
	return false;
}

bool Beopdatalink::CCommonDBAccess::GetPysiteVersion(wstring &strPysiteVersion)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);


	//查core 版本
	std::ostringstream sqlstream;
	sqlstream <<"select unitproperty03 from unit01 where unitproperty01 = 'dompysite' and unitproperty02 = 'version'; " ;
	string sqlstatement=sqlstream.str();

	MYSQL_RES* result = QueryAndReturnResult(sqlstatement.c_str());
	if (result == NULL)
	{
		return L"";
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));

	MYSQL_ROW  row = NULL;
	if(row = FetchRow(result) )
	{
		//int nValue = ATOI(row[0]);
		strPysiteVersion =  Project::Tools::UTF8ToWideChar(row[0]);
	}
	return false;
}

bool Beopdatalink::CCommonDBAccess::Insert_logic_output_point_record(wstring wstrLogicName, wstring wstrPointName, wstring wstrPointValue)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	string sqlstatement;
	sqlstream.str("");

	string strLogicName, strPointName, strPointValue;
	Project::Tools::WideCharToUtf8(wstrLogicName,strLogicName);
	Project::Tools::WideCharToUtf8(wstrPointName,strPointName);
	Project::Tools::WideCharToUtf8(wstrPointValue,strPointValue);


	SYSTEMTIME st;
	GetLocalTime(&st);
	char strPointTime[64];
	sprintf_s(strPointTime, "%d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);


	SetConnectionCharacterSet(MYSQL_UTF8);

	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< "logic_output_point_record"
		<< " (logicname,pointtime,pointname,pointvalue) values ('"
		<< strLogicName << "','" << strPointTime  << "','" << strPointName 
		<< "','" << strPointValue << "');";
	sqlstatement = sqlstream.str();
	if (!Execute(sqlstatement))
	{
		return false;
	}
	return true;
}

bool Beopdatalink::CCommonDBAccess::update_unit01_ThreadName(wstring wstrOldName, wstring wstrNewName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	
	string strOldName = Project::Tools::WideCharToUtf8(wstrOldName.c_str());
	string strNewName = Project::Tools::WideCharToUtf8(wstrNewName.c_str());

	std::ostringstream sqlstream;
	string sqlstatement;
	sqlstream << "update  "
		<< GetDBName()
		<< "."
		<< "unit01"
		<< " set unitproperty01='" <<strNewName << "'"
		<< " where unitproperty01='" <<strOldName<< "'" ;

	sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool Beopdatalink::CCommonDBAccess::ReadBeopdataAllTableList(vector<wstring>& vecPtName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);


	std::ostringstream sqlstream;
	sqlstream << "select table_name from information_schema.tables where table_schema='beopdata';";
	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (NULL == result)
	{
		return false;
	}

	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	MYSQL_ROW  row = NULL;
	while (row = FetchRow(result))
	{
		wstring wstrTableName = Project::Tools::UTF8ToWideChar(row[0]);
		vecPtName.push_back(wstrTableName);
	}

	return true;
}

bool Beopdatalink::CCommonDBAccess::ReadBeopdataAllHistoryDataTableList(vector<wstring>& vecPtName)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);


	std::ostringstream sqlstream;
	sqlstream << "select table_name from information_schema.tables where table_schema='beopdata' and table_name REGEXP '^historydata_.*$';";
	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (NULL == result)
	{
		return false;
	}

	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	MYSQL_ROW  row = NULL;
	while (row = FetchRow(result))
	{
		wstring wstrTableName = Project::Tools::UTF8ToWideChar(row[0]);
		vecPtName.push_back(wstrTableName);
	}

	return true;

}
