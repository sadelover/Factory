#include "StdAfx.h"

#include "RealTimeDataAccess.h"
#include <iostream>
#include <sstream>

#include "MysqlDeleter.h"
#include "../Tools/Scope_Ptr.h"

#include "../Tools/Util/UtilString.h"


namespace Beopdatalink
{
	//  this three will hold a session
	const char* tablename_input = "realtimedata_input";
	const char* tablename_output = "realtimedata_output";

	//log function will hold a session

	const char* tablename_userlistonline = "userlist_online";



}


SYSTEMTIME Beopdatalink::CRealTimeDataEntry::GetTimestamp() const
{
	return m_timestamp;
}

void Beopdatalink::CRealTimeDataEntry::SetTimestamp( const SYSTEMTIME& stime )
{
	m_timestamp = stime;
}

string Beopdatalink::CRealTimeDataEntry::GetName() const
{
	return m_pointname;
}

void Beopdatalink::CRealTimeDataEntry::SetName( const string& name )
{
	m_pointname = name;
}

wstring Beopdatalink::CRealTimeDataEntry::GetValue() const
{
	return m_pointvalue;
}

void Beopdatalink::CRealTimeDataEntry::SetValue(wstring strValue)
{
	m_pointvalue = strValue;
}

void Beopdatalink::CRealTimeDataEntry_Output::SetInit( bool binit )
{
	m_binit = binit;
}

bool Beopdatalink::CRealTimeDataEntry_Output::IsValueInit()
{
	return m_binit;
}

Beopdatalink::CRealTimeDataEntry_Output::CRealTimeDataEntry_Output()
	:CRealTimeDataEntry()
{
	m_binit = false;
}

bool Beopdatalink::CRealTimeDataAccess::ReadRealtimeData_Input( vector<CRealTimeDataEntry>& entrylist )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	SetConnectionCharacterSet(MYSQL_UTF8);
	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_input;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return true;
	}
	entrylist.reserve(length);
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

		entrylist.push_back(entry);
	}

	return true;
}

bool Beopdatalink::CRealTimeDataAccess::DeleteRealTimeData_Input()
{
	std::ostringstream sqlstream;
	sqlstream << "delete from "
			  << GetDBName() << "." << tablename_input;

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool Beopdatalink::CRealTimeDataAccess::UpdatePointData( const CRealTimeDataEntry& entry )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	// first, check whether

	string value_ansi;
	Project::Tools::WideCharToUtf8(entry.GetValue(), value_ansi);


	std::ostringstream sqlstream;
	// try to insert the entry.
	
	sqlstream << "insert into  "
		<< GetDBName() 
		<< "."
		<< tablename_output 
		<< " value " << "('" << entry.GetName() << "', '" << value_ansi << "')";

	string sqlstatement = sqlstream.str();
	bool bresult =  Execute(sqlstatement);
	if (!bresult)
	{
		sqlstream.str("");
		sqlstream << "update  "
			<< GetDBName()
			<< "."
			<< tablename_output
			<< " set value= '" << value_ansi
			<< "' where pointname='" << entry.GetName() << "'";
		sqlstatement = sqlstream.str();
		return Execute(sqlstatement);
	}

	return true;
}

bool Beopdatalink::CRealTimeDataAccess::UpdatePointDataList(vector<CRealTimeDataEntry>& entrylist)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "replace into " << GetDBName() << "." << tablename_output << " value";
	vector<CRealTimeDataEntry>::const_iterator	iter;
	for (iter=entrylist.begin(); iter!=entrylist.end(); iter++)
	{
		string strName = iter->GetName();
		string strVal;
		Project::Tools::WideCharToUtf8(iter->GetValue(), strVal);
		if (iter == entrylist.begin())
		{
			sqlstream << "('" << strName << "', '" << strVal <<  "')";
		}
		else
		{
			sqlstream << ",('" << strName << "', '" << strVal <<  "')";
		}
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}


bool Beopdatalink::CRealTimeDataAccess::ReadRealTimeData_Output( vector<CRealTimeDataEntry>& entrylist )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_output;

	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL){
		return false;
	}
	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	entrylist.reserve(length);
	MYSQL_ROW  row = NULL;
	CRealTimeDataEntry entry;

	while(row = FetchRow(result) )
	{
		entry.SetName(row[0]);

		wstring wstrValue;
		Project::Tools::UTF8ToWideChar(row[1], wstrValue);
		entry.SetValue(wstrValue);

		entrylist.push_back(entry);
	}

	return true;
}

bool Beopdatalink::CRealTimeDataAccess::DeleteRealTimeData_Output()
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	char* strdbname= GetDBName();
	if (!strdbname)
		return false;
	
	std::ostringstream sqlstream;
	sqlstream << "delete from "
		<< GetDBName() << "." << tablename_output;

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}


bool Beopdatalink::CRealTimeDataAccess::UpdatePointValue_Input( const CRealTimeDataEntry& entry )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	// first, check whether

	std::ostringstream sqlstream;
	// try to insert the entry.
	SYSTEMTIME  st;
	GetLocalTime(&st);
	string str_time = Project::Tools::SystemTimeToString(st);


	string value_ansi;
	Project::Tools::WideCharToUtf8(entry.GetValue(), value_ansi);

	sqlstream << "insert into  "
		<< GetDBName() 
		<< "."
		<< tablename_input 
		<< " value " << "('" <<str_time << "'," << entry.GetName() << "', '" << value_ansi << "')";

	string sqlstatement = sqlstream.str();
	bool bresult =  Execute(sqlstatement);
	if (!bresult)
	{
		sqlstream.str("");
		sqlstream << "update  "
			<< GetDBName()
			<< "."
			<< tablename_input
			<< " set pointvalue= '" << value_ansi
			<< "' where pointname='" << entry.GetName() << "'";
		sqlstatement = sqlstream.str();
		return Execute(sqlstatement);
	}

	return true;
}

bool Beopdatalink::CRealTimeDataAccess::ImportPointValue_Input( vector<CRealTimeDataEntry>& entrylist )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	if (entrylist.empty())
	{
		DeleteRealTimeData_Input();
		return true;
	}

	StartTransaction();
	DeleteRealTimeData_Input();

	//generate the insert string
	std::ostringstream sqlstream;
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_input 
		<< " values";

	vector<CRealTimeDataEntry>::const_iterator it = entrylist.begin();
	int nCount = 0;
	int nInsertCount = 0;
	bool bInsertResult = true;
	for (;it != entrylist.end(); ++it)
	{
		string str_temp = Project::Tools::SystemTimeToString((*it).GetTimestamp());
		string value_ansi;
		Project::Tools::WideCharToUtf8((*it).GetValue(), value_ansi);

		sqlstream << "('" << str_temp << "','" 
			<< (*it).GetName() << "', '" 
			<< value_ansi << "'),";
		nCount++;
		nInsertCount++;

		if(nInsertCount == 10000)
		{
			string sql_temp = sqlstream.str();
			sql_temp.erase(--sql_temp.end());
			bool bResult = Execute(sql_temp);
			if(!bResult)
			{
				string strMysqlErr = GetMysqlError();
				bInsertResult = false;
			}
			sqlstream.str("");
			sqlstream << "insert into "
				<< GetDBName()
				<<"."
				<< tablename_input 
				<< " values";
			nInsertCount = 0;
		}
	}
	string sql_temp = sqlstream.str();
	sql_temp.erase(--sql_temp.end());

	bool bResult = Execute(sql_temp);
	if(!bResult)
	{
		string strMysqlErr = GetMysqlError();
		bInsertResult = false;
	}
	Commit();
	return bInsertResult;
}

bool Beopdatalink::CRealTimeDataAccess::DeleteEntryInOutput( const string& pointname )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	char* strdbname= GetDBName();
	if (!strdbname)
		return false;

	std::ostringstream sqlstream;
	sqlstream << "delete from "
		<< GetDBName() << "." << tablename_output 
		<< " where pointname = '" << pointname << "'";

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool Beopdatalink::CRealTimeDataAccess::UpdatePointBuffer(vector<CRealTimeDataEntry>& entrylist )
{
	if (entrylist.empty()){
		return false;
	}
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	std::ostringstream sqlstream;

	StartTransaction();

	vector<CRealTimeDataEntry>::const_iterator it = entrylist.begin();
	for (;it != entrylist.end(); ++it)
	{
		string value_utf8;
		Project::Tools::WideCharToUtf8((*it).GetValue(), value_utf8);
		string time_ansi = Project::Tools::SystemTimeToString((*it).GetTimestamp());
		string name_ansi = (*it).GetName();
		sqlstream.str("");
		sqlstream << "update  "
			<< GetDBName()
			<< ".point_value_buffer"
			<< " set pointvalue= '" << value_utf8
			<< "', time = '"<<time_ansi<<"'  where pointname='" << name_ansi << "'";
		string sqlstatement = sqlstream.str();
		Execute(sqlstatement);
	}
	bool bResult2 = Commit();
	return bResult2;
}

Beopdatalink::CUserOnlineEntry::CUserOnlineEntry()
{
	GetLocalTime(&m_time);
	m_priority = 0;
}

wstring Beopdatalink::CUserOnlineEntry::GetUserName() const
{
	return m_username;
}

void Beopdatalink::CUserOnlineEntry::SetUserName( const wstring& username )
{
	m_username = username;
}


wstring Beopdatalink::CUserOnlineEntry::GetUserHost() const
{
	return m_userhost;
}

void Beopdatalink::CUserOnlineEntry::SetUserHost( const wstring& userhost )
{
	m_userhost = userhost;
}


wstring Beopdatalink::CUserOnlineEntry::GetUserType() const
{
	return m_usertype;
}

void Beopdatalink::CUserOnlineEntry::SetUserType( const wstring& usertype )
{
	m_usertype = usertype;
}


int Beopdatalink::CUserOnlineEntry::GetUserPriority() const
{
	return m_priority;
}

void Beopdatalink::CUserOnlineEntry::SetUserPriority( int userpriority )
{
	m_priority = userpriority;
}

SYSTEMTIME Beopdatalink::CUserOnlineEntry::GetTimestamp() const
{
	return m_time;
}

void Beopdatalink::CUserOnlineEntry::SetTimestamp( const SYSTEMTIME& stime )
{
	m_time = stime;
}

bool Beopdatalink::CDBAccessToDTU::GetOperationRecordAsString( vector<optrecordfordtu>& resultlist,
									const COleDateTime& timeStart, const COleDateTime& timeStop )
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);

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
	sqlstream << "select * from operation_record where recordtime >= '"<< strStart.c_str() <<"' and recordtime <= '"<< strStop.c_str() << "'";

	string sql = sqlstream.str();
	optrecordfordtu  tempOpera;
	COleDateTime old;
	// execute the query
	
	MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
	if (result){	
		MYSQL_ROW row = NULL;
		while (row = mysql_fetch_row(result)){
			if (row[0] && row[1] && row[2]){
				tempOpera.time = row[0];
				tempOpera.username = row[1];
				tempOpera.info = row[2];
				resultlist.push_back(tempOpera);
			}
		}
		FreeReturnResult(result);
	}
	
	return true;
}


bool Beopdatalink::CDBAccessToDTU::GetOptimizeRecordAsString( string& strrecord,
															  const COleDateTime& timeStart,
															  const COleDateTime& timeStop )
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);

	CString time_begin = timeStart.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString time_end = timeStop.Format(_T("%Y-%m-%d %H:%M:%S"));

	string timebegin_ansi = Project::Tools::WideCharToAnsi(time_begin.GetString());
	string timeend_ansi = Project::Tools::WideCharToAnsi(time_end.GetString());

	std::ostringstream sqlstream;
	sqlstream << "select * from " << "controlparam_record " 
		<< "where recordtime >= '" << timebegin_ansi <<"' "
		<< "and recordtime <= '" << timeend_ansi <<"' "
		<< "order by recordtime desc";

	string sql = sqlstream.str();
	MYSQL_RES* queryresult = QueryAndReturnResult(sql.c_str());
	if (queryresult == NULL){
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

Beopdatalink::CDBAccessToDTU::CDBAccessToDTU():CDatabaseSeesion()
{

}


Beopdatalink::DTUDBAccessSingleton* Beopdatalink::DTUDBAccessSingleton::GetInstance()
{
	if (_pinstance == NULL)
	{

		Project::Tools::Scoped_Lock<Project::Tools::Mutex> lock(m_lock);
		if (_pinstance == NULL)
		{
			_pinstance = new DTUDBAccessSingleton();
			atexit(DestroyInstance);
		}
	}

	return _pinstance;
}

void Beopdatalink::DTUDBAccessSingleton::DestroyInstance()
{
	if (_pinstance){
		delete _pinstance;
	}
}

Beopdatalink::DTUDBAccessSingleton::DTUDBAccessSingleton()
	:CDBAccessToDTU()
{

}


Project::Tools::Mutex Beopdatalink::DTUDBAccessSingleton::m_lock;
Beopdatalink::DTUDBAccessSingleton* Beopdatalink::DTUDBAccessSingleton::_pinstance = NULL;