#include "StdAfx.h"
#include "ExportDataAccess.h"

#include <sstream>
#include <fstream>

#include "TableAnalyzer.h"
const char* tablename_historydata = "historydata";

const char* sql_createtable =  "(`time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,\
							   `pointname` varchar(64) NOT NULL,\
							   `value` varchar(256) NOT NULL,\
							   PRIMARY KEY (`time`,`pointname`)) \
							   ENGINE=InnoDB DEFAULT CHARSET=utf8";

namespace ExportDataAccess
{

	bool ExportDataAccess::ExportRecordCommon( const ExportParam& exportparam )
	{
		//
		//Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
		//string str_start = Project::Tools::SystemTimeToString(exportparam.st_begin);
		//string str_end = Project::Tools::SystemTimeToString(exportparam.st_end);
		//
		////real escape the filepath
		//unsigned int len = exportparam.exportpath.size();
		//char* buffer = new char[2*len+1];
		//memset(buffer, 0x00, 2 * len + 1);
		//mysql_real_escape_string(&m_mysql, buffer, exportparam.exportpath.c_str(), len);
		//string filepath_escape = buffer;
		//delete buffer;

		////get the field name of the timestamp field
		//std::ostringstream sqlstream;
		//sqlstream << "select * from "
		//	<< GetDBName() 
		//	<<"."
		//	<<exportparam.tablename
		//	<< " where false";
		//
		//MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
		//if (result){
		//	return false;
		//}

		//unsigned int num_fields;
		//unsigned int i;
		//MYSQL_FIELD *fields;

		//num_fields = mysql_num_fields(result);
		//fields = mysql_fetch_fields(result);
		//
		////时间字段
		//string fieldname_time = "";
		//for(i = 0; i < num_fields; i++){
		//	if (fields[i].type == FIELD_TYPE_TIMESTAMP ||
		//		fields[i].type == FIELD_TYPE_DATE || 
		//		fields[i].type == FIELD_TYPE_DATETIME ||
		//		fields[i].type == FIELD_TYPE_TIME)
		//	{
		//		fieldname_time = fields[i].name;
		//	}
		//}

		//sqlstream.str("");
		//if (fieldname_time.empty())
		//{
		//	sqlstream << "select * from "
		//		<< GetDBName() 
		//		<<"."
		//		<<exportparam.tablename
		//		<< "' into outfile \""
		//		<< filepath_escape
		//		<< "\" character set utf8 fields terminated by ',' optionally enclosed by '' lines terminated by '\\n' ";
		//}else
		//{
		//	sqlstream << "select * from "
		//		<< GetDBName() 
		//		<<"."
		//		<<exportparam.tablename
		//		<< " where " << fieldname_time << " >= '"
		//		<< str_start
		//		<< "' and " << fieldname_time << " <= '"
		//		<< str_end 
		//		<< "' into outfile \""
		//		<< filepath_escape
		//		<< "\" character set utf8 fields terminated by ',' optionally enclosed by '' lines terminated by '\\n' ";
		//}
		//
		//string sql = sqlstream.str();
		//return Execute(sql);
		// 
		return false;
	}


	// export the dtu data to to specified path.
	// 问题在于跨表查询。
	// we need to calculate table name from the start time to end time.
	bool ExportDataAccess::ExportSensorDataDTU( const ExportParam& exportparam )
	{
		
		return true;
	}

	bool ExportDataAccess::ExportSensorDataDTU( const ExportParam& exportparam, vector<ExportValueEntry>& resultlist )
	{
		Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
		
		CString cstrtablename = exportparam.tablename.c_str();
		char szTemp[256] = {0};
		wsprintfA(szTemp, "%S", cstrtablename.GetBuffer());
		cstrtablename.ReleaseBuffer();

		const char* strtabname = szTemp;

		string str_start = Project::Tools::SystemTimeToString(exportparam.st_begin);
		string str_end = Project::Tools::SystemTimeToString(exportparam.st_end);

		std::ostringstream sqlstream;
		sqlstream << "select * from " 
			<< GetDBName() 
			<< "."
			<< strtabname
			<< " where time >= '" << str_start << "' and time <= '" << str_end
			<< "' and pointname = '" << exportparam.pointname << "'""'";

		MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
		if (result == NULL){
			return false;
		}
		
		int length = static_cast<int>(GetRowNum(result));
		if (0 == length){
			return false;
		}
		resultlist.reserve(length);
		MYSQL_ROW  row = NULL;
		ExportValueEntry entry;

		while(row = FetchRow(result) )
		{   
			SYSTEMTIME sttime;
			Project::Tools::String_To_SysTime(row[0], sttime);
			entry.st_begin = sttime;
			//entry.pointname = row[1];
			entry.strvalue = Project::Tools::Utf8ToMultiByte(row[2]);

			resultlist.push_back(entry);
		}
		FreeReturnResult(result);
		
		return true;
	}

	bool CDTUDatabasePointInfoAccess::ReadDatabaseList( vector<DTUDatabaseInfo>& dblist )
	{
		Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);

		string sql = "select * from dtudatabaseinfo.databaseprojectinfo order by projectname";
		MYSQL_RES* result = QueryAndReturnResult(sql.c_str());
		if (result)
		{
			MYSQL_ROW row = NULL;
			while (row =FetchRow(result))
			{
				DTUDatabaseInfo info;
				info.databasename = Project::Tools::UTF8ToWideChar(row[0]);
				info.projectname = Project::Tools::UTF8ToWideChar(row[1]);

				dblist.push_back(info);
			}

			FreeReturnResult(result);
			return true;
		}
		
		return false;
	}


	bool CDTUDatabasePointInfoAccess::ReadPointList( const string& databasename,vector<DtuPointEntry>& resultlist )
	{

		Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);
		string sql_readpointlist = "select * from ";
		sql_readpointlist += databasename;
		sql_readpointlist+=".pointlist order by name";
		MYSQL_RES* result = QueryAndReturnResult(sql_readpointlist.c_str());
		if (!result){
			return false;
		}

		long rownum = GetRowNum(result);
		if (rownum == 0){
			return true;
		}
		resultlist.reserve(rownum);
		MYSQL_ROW row = NULL;
		wstring pointname;
		wstring pointcomment;
		while(row = FetchRow(result))
		{
			DtuPointEntry entry;
			entry.index = ATOI(row[0]);
			Project::Tools::UTF8ToWideChar(row[1], pointname);
			wcscpy_s(entry.pointname,pointname.c_str());
			Project::Tools::UTF8ToWideChar(row[2], pointcomment);
			wcscpy_s(entry.comment,pointcomment.c_str());
			entry.type = ATOI(row[3]);
			entry.useindex = (ATOI(row[4]) == 0) ? false : true;
			entry.reserved1 = ATOI(row[5]);
			entry.reserved2 = ATOI(row[6]);

			resultlist.push_back(entry);
		}

		FreeReturnResult(result);
		return true;
	}

	bool CDTUDatabasePointInfoAccess::ExportDTUDataToFile( const ExportParam& exportparam )
	{
		Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
		
		string str_start = Project::Tools::SystemTimeToString(exportparam.st_begin);
		string str_end = Project::Tools::SystemTimeToString(exportparam.st_end);
		std::ostringstream sqlstream;
		sqlstream << "select * from " 
			<< exportparam.dbname
			<< "."
			<< exportparam.tablename
			<< " where time >= '" << str_start << "' and time <= '" << str_end
			<< "' and pointname = '" << exportparam.pointname << "'";

		MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
		if (result == NULL){
			return false;
		}
		
		std::ofstream filestream(exportparam.exportpath.c_str(), std::ios::out | std::ios::_Noreplace);
		if(!filestream.good()){
			return false;
		}
		filestream.seekp(0, std::ios::beg);;

		const string cstrLineEnd = "\n";
		const string cstrComma = ",";
		//title
		filestream << "time"  << cstrComma << exportparam.pointname << cstrLineEnd;
		MYSQL_ROW  row = NULL;
		while(row = FetchRow(result) )
		{   
			filestream << row[0] << cstrComma<< row[2] << cstrLineEnd;
		}
		filestream.close();
		FreeReturnResult(result);
		return true;
	}


	/*
	 *分表查询。
	 *数据库的数据会按月分表。
	 *查询时，根据传进来的时间段，自动判断表名，并组装sql语句。
	 */

	bool CDTUDatabasePointInfoAccess::ExportDTUDataToMemory( const ExportParam& exportparam, vector<ExportValueEntry>& resultlist )
	{
		Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

		std::vector<CTableAnalyzer::queryentry> scantablelist;		//要扫描的表名
		CTableAnalyzer	cta(exportparam.tablename);
		cta.AnalyzeTableQuery(scantablelist, exportparam.st_begin, exportparam.st_end);
		if (scantablelist.empty()){
			return false;
		}

		std::ostringstream sqlstream;
		for (unsigned int i = 0; i < scantablelist.size(); i++)
		{
			const CTableAnalyzer::queryentry& entry = scantablelist[i];
			string str_start = Project::Tools::SystemTimeToString(exportparam.st_begin);
			string str_end = Project::Tools::SystemTimeToString(exportparam.st_end);
			
			if (i == 0)
			{
				sqlstream << "select * from " 
					<< exportparam.dbname
					<< "."
					<< entry.tablename
					<< " where time >= '" << str_start << "' and time <= '" << str_end
					<< "' and pointname = '" << exportparam.pointname << "'";
			}else
			{
				sqlstream << " union all select * from " 
					<< exportparam.dbname
					<< "."
					<< entry.tablename
					<< " where time >= '" << str_start << "' and time <= '" << str_end
					<< "' and pointname = '" << exportparam.pointname << "'";
			}
			
		}

		string sqlstatement = sqlstream.str();		
		MYSQL_RES* result = QueryAndReturnResult(sqlstatement.c_str());
		if (result == NULL){
			return false;
		}
		
		my_ulonglong rownum = GetRowNum(result);
		if (rownum == 0){
			return true;
		}
		resultlist.reserve(rownum);

		MYSQL_ROW  row = NULL;
		while(row = FetchRow(result) )
		{   
			ExportValueEntry entry;
			Project::Tools::String_To_SysTime(row[0], entry.st_begin);
			entry.st_begin.wSecond = 0;
			//entry.pointname = row[1];
			entry.strvalue = Project::Tools::Utf8ToMultiByte(row[2]);

			resultlist.push_back(entry);
			
		}
		FreeReturnResult(result);
		return true;
	}

	bool CDTUDatabasePointInfoAccess::ExportDTUDataToMemory(const string& dbname,const string& tbname,const string& pointname,const string& start, const string& end, vector<ExportValueEntry>& resultlist )
	{
		Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
		std::ostringstream sqlstream;
		sqlstream << "select * from " 
			<< dbname
			<< "."
			<< tbname
			//<< " where time >= '" << start << "' and time <= '" << end
			<< " where pointname = '" << pointname << "'";
		
		string sqlstatement = sqlstream.str();		
		MYSQL_RES* result = QueryAndReturnResult(sqlstatement.c_str());
		if (result == NULL){
			return false;
		}

		MYSQL_ROW  row = NULL;
		while(row = FetchRow(result) )
		{   
			ExportValueEntry entry;
			Project::Tools::String_To_SysTime(row[0], entry.st_begin);
			entry.st_begin.wSecond = 0;
			//entry.pointname = row[1];
			entry.strvalue = Project::Tools::Utf8ToMultiByte(row[2]);
			resultlist.push_back(entry);
		}
		FreeReturnResult(result);
		return true;
	}

	bool CDTUDatabasePointInfoAccess::ReadPointList_Local( const string& databasename, vector<DtuPointEntry>& resultlist )
	{
		Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);

		CStringA tablename;
		SYSTEMTIME st;
		GetLocalTime(&st);
		tablename.Format("historydata_%d_%02d", st.wYear, st.wMonth);

		string sql_readpointlist = "select distinct pointname from ";
		sql_readpointlist += databasename;
		sql_readpointlist += ".";
		sql_readpointlist += tablename.GetString();
		sql_readpointlist+=" order by pointname";
		MYSQL_RES* result = QueryAndReturnResult(sql_readpointlist.c_str());
		if (!result){
			return false;
		}

		long rownum = GetRowNum(result);
		if (rownum == 0){
			return true;
		}
		resultlist.reserve(rownum);
		MYSQL_ROW row = NULL;
		wstring pointname;
		wstring pointcomment;
		while(row = FetchRow(result))
		{
			DtuPointEntry entry;
			Project::Tools::UTF8ToWideChar(row[0], pointname);
			wcscpy_s(entry.pointname,pointname.c_str());
			resultlist.push_back(entry);
		}

		FreeReturnResult(result);
		return true;
	}

	bool CDTUDatabasePointInfoAccess::GetHistoryValue( const string& strName,const SYSTEMTIME &stStart, SYSTEMTIME stEnd,const int &nTimeFormat, vector<ExportValueEntry>& entryList )
	{
		Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);
		int nCount = 0;
		SYSTEMTIME tNow;
		GetLocalTime(&tNow);
		COleDateTime oleEnd(stEnd);
		if(oleEnd>COleDateTime::GetCurrentTime())
			stEnd = tNow;

		int nSpan = Project::Tools::SystemTimeToTime_t(stEnd) - Project::Tools::SystemTimeToTime_t(stStart);
		COleDateTimeSpan tspan;
		if(nTimeFormat==E_STORE_FIVE_SECOND)
		{
			tspan.SetDateTimeSpan(0,0,0,5);
			nCount = nSpan/5 +1;
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

		std::ostringstream sqlstream;
		sqlstream << "select * from " << histablename 
			<< " where pointname = '"
			<< strName
			<< "' order by time";


		string sql = sqlstream.str();
		// execute the query
		MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
		if (result == NULL)
		{
			return false;
		}
		
		long rownum = GetRowNum(result);
		if (rownum == 0){
			return false;
		}

		MYSQL_ROW row = NULL;
		static vector<ExportValueTime> tQueryList;
		static vector<wstring>  strQueryList;

		static vector<COleDateTime> tQueryListOrg;
		static vector<wstring>  strQueryListOrg;

		tQueryList.clear();
		strQueryList.clear();
		tQueryListOrg.clear();
		strQueryListOrg.clear();

		while(row = FetchRow(result))
		{
			SYSTEMTIME st;
			Project::Tools::String_To_SysTime(row[0], st);
			COleDateTime   oleQt(st);
			tQueryListOrg.push_back(oleQt);

			wstring wstrValue;
			Project::Tools::UTF8ToWideChar(row[2], wstrValue);
			strQueryListOrg.push_back(wstrValue);
		}
		FreeReturnResult(result);

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
					ExportValueTime entry1;
					entry1.oleTime = tQueryListOrg[i];
					tQueryList.push_back(entry1);		
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
							ExportValueTime entry2;
							entry2.oleTime = tQueryListOrg[i]+t;
							entry2.bMissing = true;
							tQueryList.push_back(entry2);
							strQueryList.push_back(strQueryListOrg[i]);
						}
					}
				}
				ExportValueTime entry3;
				entry3.oleTime = tQueryListOrg[tQueryListOrg.size()-1];
				tQueryList.push_back(entry3);
				strQueryList.push_back(strQueryListOrg[strQueryListOrg.size()-1]);
			}

			SYSTEMTIME stQueryMin;
			SYSTEMTIME stQueryMax;
			tQueryList[0].oleTime.GetAsSystemTime(stQueryMin);
			tQueryList[tQueryList.size()-1].oleTime.GetAsSystemTime(stQueryMax);
			if (Project::Tools::SystemTimeToTime_t(stStart) >= Project::Tools::SystemTimeToTime_t(stQueryMax))		//查询时间>结果 全部补齐
			{
				COleDateTime ole(stStart);
				for (int i=0;i<nCount;++i)
				{	
					SYSTEMTIME st;
					ole.GetAsSystemTime(st);
					ExportValueEntry entry;
					if(i==0)
						entry.bMissing = false;
					entry.st_begin = st;
					entry.st_begin.wSecond = 0;
					//entry.pointname = strName;
					entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[strQueryList.size()-1].c_str());
					ole += tspan;
					entryList.push_back(entry);
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stEnd) <= Project::Tools::SystemTimeToTime_t(stQueryMin))  //查询时间<结果 全部补齐
			{
				COleDateTime ole(stStart);
				for (int i=0;i<nCount;++i)
				{
					SYSTEMTIME st;
					ole.GetAsSystemTime(st);				
					ExportValueEntry entry;
					entry.st_begin = st;
					entry.st_begin.wSecond = 0;
					//entry.pointname = strName;
					entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[0].c_str());
					if(i==nCount-1)
						entry.bMissing = false;
					ole += tspan;
					entryList.push_back(entry);
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stStart) >= Project::Tools::SystemTimeToTime_t(stQueryMin) && Project::Tools::SystemTimeToTime_t(stEnd) < Project::Tools::SystemTimeToTime_t(stQueryMax)) //之间
			{
				COleDateTime ole(stStart);
				for (size_t i=0;i<tQueryList.size();++i)
				{
					SYSTEMTIME st;
					tQueryList[i].oleTime.GetAsSystemTime(st);
					if (Project::Tools::SystemTimeToTime_t(st) >= Project::Tools::SystemTimeToTime_t(stStart) && Project::Tools::SystemTimeToTime_t(st) <= Project::Tools::SystemTimeToTime_t(stEnd))
					{
						if(entryList.size()<nCount)
						{
							SYSTEMTIME st1;
							ole.GetAsSystemTime(st1);

							ExportValueEntry entry;
							entry.st_begin = st1;
							entry.st_begin.wSecond = 0;
							//entry.pointname = strName;
							entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[i].c_str());
							if(!tQueryList[i].bMissing)
								entry.bMissing = false;
							ole += tspan;
							entryList.push_back(entry);
						}
					}
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stStart) <= Project::Tools::SystemTimeToTime_t(stQueryMin) && Project::Tools::SystemTimeToTime_t(stEnd) >= Project::Tools::SystemTimeToTime_t(stQueryMax))	//起始时间<结果<结束时间
			{
				COleDateTime ole(stStart);
				int nFixFront = (int)(Project::Tools::SystemTimeToTime_t(stQueryMin)-Project::Tools::SystemTimeToTime_t(stStart))/(int)tspan.GetTotalSeconds();
				int nFixBack = (int)(Project::Tools::SystemTimeToTime_t(stEnd)-Project::Tools::SystemTimeToTime_t(stQueryMax))/(int)tspan.GetTotalSeconds();
				for (int i=0;i<nFixFront;++i)
				{
					if(entryList.size()<nCount)
					{
						SYSTEMTIME st1;
						ole.GetAsSystemTime(st1);
						ExportValueEntry entry;
						entry.st_begin = st1;
						entry.st_begin.wSecond = 0;
						//entry.pointname = strName;
						entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[0].c_str());

						ole += tspan;
						entryList.push_back(entry);
					}
				}
				for (int j=0;j<tQueryList.size();++j)
				{	
					SYSTEMTIME st;
					tQueryList[j].oleTime.GetAsSystemTime(st);
					if (Project::Tools::SystemTimeToTime_t(st) >= Project::Tools::SystemTimeToTime_t(stStart) && Project::Tools::SystemTimeToTime_t(st) <= Project::Tools::SystemTimeToTime_t(stEnd))
					{
						if(entryList.size()<nCount)
						{
							SYSTEMTIME st1;
							ole.GetAsSystemTime(st1);

							ExportValueEntry entry;
							entry.st_begin = st1;
							entry.st_begin.wSecond = 0;
							//entry.pointname = strName;
							entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[j].c_str());
							if(!tQueryList[j].bMissing)
								entry.bMissing = false;
							ole += tspan;
							entryList.push_back(entry);
						}
					}
				}
				for (int m=0;m<nFixBack;++m)
				{
					if(entryList.size()<nCount)
					{
						SYSTEMTIME st1;
						ole.GetAsSystemTime(st1);
					
						ExportValueEntry entry;
						entry.st_begin = st1;
						entry.st_begin.wSecond = 0;
						//entry.pointname = strName;
						entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[strQueryList.size()-1].c_str());

						ole += tspan;
						entryList.push_back(entry);
					}
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stEnd) >= Project::Tools::SystemTimeToTime_t(stQueryMax) 
				&& Project::Tools::SystemTimeToTime_t(stStart) < Project::Tools::SystemTimeToTime_t(stQueryMax)
				&& Project::Tools::SystemTimeToTime_t(stStart) > Project::Tools::SystemTimeToTime_t(stQueryMin))		//结束时间>结果      起始时间在中间
			{
				COleDateTime ole(stStart);
				int nFixBack = (int)(Project::Tools::SystemTimeToTime_t(stEnd)-Project::Tools::SystemTimeToTime_t(stQueryMax))/(int)tspan.GetTotalSeconds();
				for (int i=0;i<tQueryList.size();++i)
				{
					SYSTEMTIME st;
					tQueryList[i].oleTime.GetAsSystemTime(st);
					if (Project::Tools::SystemTimeToTime_t(st) >= Project::Tools::SystemTimeToTime_t(stStart) && Project::Tools::SystemTimeToTime_t(st) <= Project::Tools::SystemTimeToTime_t(stEnd))
					{
						if(entryList.size()<nCount)
						{
							SYSTEMTIME st1;
							ole.GetAsSystemTime(st1);
						
							ExportValueEntry entry;
							entry.st_begin = st1;
							entry.st_begin.wSecond = 0;
							//entry.pointname = strName;
							entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[i].c_str());
							if(!tQueryList[i].bMissing)
								entry.bMissing = false;
							ole += tspan;
							entryList.push_back(entry);
						}
					}
				}
				for (int m=0;m<nFixBack;++m)
				{
					if(entryList.size()<nCount)
					{
						SYSTEMTIME st1;
						ole.GetAsSystemTime(st1);

						ExportValueEntry entry;
						entry.st_begin = st1;
						entry.st_begin.wSecond = 0;
						//entry.pointname = strName;
						entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[strQueryList.size()-1].c_str());
						ole += tspan;
						entryList.push_back(entry);
					}
				}
			}
			else if (Project::Tools::SystemTimeToTime_t(stStart) <= Project::Tools::SystemTimeToTime_t(stQueryMin) 
				&& Project::Tools::SystemTimeToTime_t(stEnd) < Project::Tools::SystemTimeToTime_t(stQueryMax)
				&& Project::Tools::SystemTimeToTime_t(stEnd) > Project::Tools::SystemTimeToTime_t(stQueryMin))					//起始时间<结果      结束时间在中间
			{
				COleDateTime ole(stStart);
				int nFixFront = (int)(Project::Tools::SystemTimeToTime_t(stQueryMin)-Project::Tools::SystemTimeToTime_t(stStart))/(int)tspan.GetTotalSeconds();
				for (int i=0;i<nFixFront;++i)
				{
					if(entryList.size()<nCount)
					{
						SYSTEMTIME st1;
						ole.GetAsSystemTime(st1);
					
						ExportValueEntry entry;
						entry.st_begin = st1;
						entry.st_begin.wSecond = 0;
						//entry.pointname = strName;
						entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[0].c_str());

						ole += tspan;
						entryList.push_back(entry);
					}
				} 
				for (int i=0;i<tQueryList.size();++i)
				{
					SYSTEMTIME st;
					tQueryList[i].oleTime.GetAsSystemTime(st);
					if (Project::Tools::SystemTimeToTime_t(st)>Project::Tools::SystemTimeToTime_t(stStart) && Project::Tools::SystemTimeToTime_t(st)<=Project::Tools::SystemTimeToTime_t(stEnd))
					{
						if(entryList.size()<nCount)
						{
							SYSTEMTIME st1;
							ole.GetAsSystemTime(st1);
						
							ExportValueEntry entry;
							entry.st_begin = st1;
							entry.st_begin.wSecond = 0;
							//entry.pointname = strName;
							entry.strvalue = Project::Tools::WideCharToAnsi(strQueryList[i].c_str());
							if(!tQueryList[i].bMissing)
								entry.bMissing = false;
							ole += tspan;
							entryList.push_back(entry);
						}
					}
				}
			}
		}
		return true;
	}

	//bool CDTUDatabasePointInfoAccess::GetHistoryValue( const string& strName,const SYSTEMTIME &stStart, SYSTEMTIME stEnd,const int &nTimeFormat, const int &nNeedTimeFormat,const int &nMinTimeFormat,vector<ExportValueEntry>& entryList )
	//{
	//	Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);

	//	if(nTimeFormat == E_STORE_NULL || nNeedTimeFormat == E_STORE_NULL || nMinTimeFormat == E_STORE_NULL)
	//		return false;

	//	int nCount = 0;
	//	SYSTEMTIME tNow;
	//	GetLocalTime(&tNow);
	//	COleDateTime oleEnd(stEnd);
	//	if(oleEnd>COleDateTime::GetCurrentTime())
	//		stEnd = tNow;

	//	//初始化所需点表
	//	int nSpan = Project::Tools::SystemTimeToTime_t(stEnd) - Project::Tools::SystemTimeToTime_t(stStart);
	//	COleDateTimeSpan tspan;
	//	if(nNeedTimeFormat==E_STORE_FIVE_SECOND)
	//	{
	//		tspan.SetDateTimeSpan(0,0,0,5);
	//		nCount = nSpan/5 +1;
	//	}
	//	else if(nNeedTimeFormat==E_STORE_ONE_MINUTE)
	//	{
	//		tspan.SetDateTimeSpan(0,0,1,0);
	//		nCount = nSpan/60+1;
	//	}
	//	else if(nNeedTimeFormat==E_STORE_FIVE_MINUTE)
	//	{
	//		tspan.SetDateTimeSpan(0,0,5,0);
	//		nCount = nSpan/(60*5)+1;
	//	}
	//	else if(nNeedTimeFormat==E_STORE_HALF_HOUR)
	//	{
	//		tspan.SetDateTimeSpan(0,0,30,0);
	//		nCount = nSpan/(60*30)+1;
	//	}
	//	else if(nNeedTimeFormat==E_STORE_ONE_HOUR)
	//	{
	//		tspan.SetDateTimeSpan(0,1,0,0);
	//		nCount = nSpan/(60*60)+1;
	//	}
	//	else if(nNeedTimeFormat==E_STORE_ONE_DAY)
	//	{
	//		tspan.SetDateTimeSpan(1,0,0,0);
	//		nCount = nSpan/(60*60*24)+1;
	//	}
	//	else if(nNeedTimeFormat==E_STORE_ONE_MONTH)
	//	{
	//		tspan.SetDateTimeSpan(30,0,0,0);
	//		nCount = nSpan/(60*60*24*30)+1;
	//	}
	//	else if(nNeedTimeFormat==E_STORE_ONE_YEAR)
	//	{
	//		tspan.SetDateTimeSpan(365,0,0,0);
	//		nCount = nSpan/(60*60*24*365)+1;
	//	}

	//	vector<ExportValueEntry> vecNeedEntry;
	//	COleDateTime oleInitStart(stStart);	
	//	for(int i=0; i<nCount; ++i)
	//	{		
	//		SYSTEMTIME st;
	//		oleInitStart.GetAsSystemTime(st);
	//		ExportValueEntry entry;
	//		entry.st_begin = st;
	//		entry.st_begin.wSecond = 0;
	//		entry.pointname = strName;
	//		oleInitStart += tspan;
	//		vecNeedEntry.push_back(entry);
	//	}

	//	//查找
	//	int nMaxSearch = (nTimeFormat>nNeedTimeFormat)?nTimeFormat:nNeedTimeFormat;
	//	nMaxSearch = (nMaxSearch>nMinTimeFormat)?nMaxSearch:nMinTimeFormat;

	//	if(nMaxSearch == E_STORE_HALF_HOUR)	//半小时无
	//	{
	//		nMaxSearch = E_STORE_ONE_HOUR;
	//	}

	//	string histablename = GetHistoryTableNameByStoreCycle(stStart, (POINT_STORE_CYCLE) nMaxSearch);

	//	COleDateTime oleTimeStart(stStart),oleTimeEnd(stEnd);

	//	wstring wstrStart, wstrStop;
	//	string strStart, strStop;

	//	Project::Tools::OleTime_To_String(oleTimeStart, wstrStart);
	//	Project::Tools::OleTime_To_String(oleTimeEnd, wstrStop);
	//	Project::Tools::WideCharToUtf8(wstrStart, strStart);
	//	Project::Tools::WideCharToUtf8(wstrStop, strStop);

	//	std::ostringstream sqlstream;
	//	sqlstream << "select * from " << histablename 
	//		<< " where pointname = '"
	//		<< strName
	//		<< "' order by time";


	//	string sql = sqlstream.str();
	//	// execute the query
	//	MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
	//	if (result == NULL)
	//	{
	//		return false;
	//	}

	//	int nBegin = 0;
	//	int nFirstNotNull = 0;
	//	bool bFirst = true;
	//	double dValue = 0;
	//	MYSQL_ROW row = NULL;

	//	long rownum = GetRowNum(result);
	//	if (rownum == 0)				//不存在
	//	{
	//
	//	}
	//	else
	//	{
	//		while(row = FetchRow(result))
	//		{
	//			SYSTEMTIME st;
	//			Project::Tools::String_To_SysTime(row[0], st);
	//			st.wSecond = 0;
	//			dValue = ATOF(row[2]);
	//			for(nBegin=0; nBegin<vecNeedEntry.size(); nBegin++)
	//			{
	//				if(CompareTwoSystemTime(st,vecNeedEntry[nBegin].st_begin) == 0)
	//				{
	//					if(bFirst)
	//					{
	//						nFirstNotNull = nBegin;
	//						bFirst = false;
	//					}
	//					vecNeedEntry[nBegin].value = dValue;
	//					vecNeedEntry[nBegin].bMissing = false;
	//					break;
	//				}
	//			}
	//		}
	//		FreeReturnResult(result);
	//	}

	//	if(bFirst)		//1个也没找到
	//	{
	//		sqlstream.str("");
	//		sqlstream << "select * from historydata_day_2014 "
	//			<< " where pointname = '"
	//			<< strName
	//			<< "' and time<='"
	//			<< Project::Tools::SystemTimeToString(stStart)
	//			<< "' order by time desc limit 0,1";

	//		sql = sqlstream.str();
	//		// execute the query
	//		result = QueryAndReturnResult(sql.c_str() );
	//		if (result != NULL)
	//		{
	//			long rownum = GetRowNum(result);
	//			if (rownum > 0){				
	//				while(row = FetchRow(result))
	//				{						
	//					dValue = ATOF(row[2]);						
	//				}
	//			}
	//			FreeReturnResult(result);				
	//		}	
	//		vecNeedEntry[0].value = dValue;
	//		vecNeedEntry[0].bMissing = false;
	//	}

	//	//第一个不为空值前的点用第一个点值补齐
	//	for(int i=0; i<nFirstNotNull; ++i)
	//	{
	//		vecNeedEntry[i].value = vecNeedEntry[nFirstNotNull].value;
	//	}

	//	for(int i=nFirstNotNull; i<vecNeedEntry.size(); ++i)
	//	{
	//		if(i>0 && vecNeedEntry[i].bMissing)
	//		{
	//			vecNeedEntry[i].value = vecNeedEntry[i-1].value;
	//		}
	//	}

	//	//
	//	entryList.insert(entryList.end(),vecNeedEntry.begin(),vecNeedEntry.end());
	//	return true;
	//}

bool CDTUDatabasePointInfoAccess::GetHistoryValue( const string& strName,const SYSTEMTIME &stStart, SYSTEMTIME stEnd,const int &nTimeFormat, const int &nNeedTimeFormat,const int &nMinTimeFormat,vector<ExportValueEntry>& entryList )
{
	Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);

	if(nTimeFormat == E_STORE_NULL || nNeedTimeFormat == E_STORE_NULL || nMinTimeFormat == E_STORE_NULL)
		return false;

	int nCount = 0;
	SYSTEMTIME tNow;
	GetLocalTime(&tNow);
	COleDateTime oleEnd(stEnd);
	if(oleEnd>COleDateTime::GetCurrentTime())
		stEnd = tNow;

	COleDateTime oleEnd_(stEnd);
	COleDateTime oleStart_(stStart);
	if(oleStart_ != oleEnd_ && stEnd.wHour == 0 && stEnd.wMinute == 0 && stEnd.wSecond == 0)	//起始时间等于结束时间时，结束时间不能为零点
	{
		oleEnd_ = oleEnd_ - COleDateTimeSpan(0,0,0,1);
		oleEnd_.GetAsSystemTime(stEnd);
	}

	//初始化所需点表
	int nSpan = Project::Tools::SystemTimeToTime_t(stEnd) - Project::Tools::SystemTimeToTime_t(stStart);
	COleDateTimeSpan tspan;
	if(nNeedTimeFormat==E_STORE_FIVE_SECOND)
	{
		tspan.SetDateTimeSpan(0,0,0,5);
		nCount = nSpan/5 +1;
	}
	else if(nNeedTimeFormat==E_STORE_ONE_MINUTE)
	{
		tspan.SetDateTimeSpan(0,0,1,0);
		nCount = nSpan/60+1;
	}
	else if(nNeedTimeFormat==E_STORE_FIVE_MINUTE)
	{
		tspan.SetDateTimeSpan(0,0,5,0);
		nCount = nSpan/(60*5)+1;
	}
	else if(nNeedTimeFormat==E_STORE_HALF_HOUR)
	{
		tspan.SetDateTimeSpan(0,0,30,0);
		nCount = nSpan/(60*30)+1;
	}
	else if(nNeedTimeFormat==E_STORE_ONE_HOUR)
	{
		tspan.SetDateTimeSpan(0,1,0,0);
		nCount = nSpan/(60*60)+1;
	}
	else if(nNeedTimeFormat==E_STORE_ONE_DAY)
	{
		tspan.SetDateTimeSpan(1,0,0,0);
		nCount = nSpan/(60*60*24)+1;
	}
	else if(nNeedTimeFormat==E_STORE_ONE_MONTH)
	{
		tspan.SetDateTimeSpan(30,0,0,0);
		nCount = nSpan/(60*60*24*30)+1;
	}
	else if(nNeedTimeFormat==E_STORE_ONE_YEAR)
	{
		tspan.SetDateTimeSpan(365,0,0,0);
		nCount = nSpan/(60*60*24*365)+1;
	}

	vector<ExportValueEntry> vecNeedEntry;
	COleDateTime oleInitStart(stStart);	
	for(int i=0; i<nCount; ++i)
	{		
		SYSTEMTIME st;
		oleInitStart.GetAsSystemTime(st);
		ExportValueEntry entry;
		entry.st_begin = st;
		entry.st_begin.wSecond = 0;
		//entry.pointname = strName;
		oleInitStart += tspan;
		vecNeedEntry.push_back(entry);
	}

	//查找
	int nMaxSearch = (nTimeFormat>nNeedTimeFormat)?nTimeFormat:nNeedTimeFormat;
	nMaxSearch = (nMaxSearch>nMinTimeFormat)?nMaxSearch:nMinTimeFormat;

	if(nMaxSearch == E_STORE_HALF_HOUR)	//半小时无
	{
		nMaxSearch = E_STORE_ONE_HOUR;
	}

	string histablename = GetHistoryTableNameByStoreCycle(stStart, (POINT_STORE_CYCLE) nMaxSearch);

	COleDateTime oleTimeStart(stStart),oleTimeEnd(stEnd);

	wstring wstrStart, wstrStop;
	string strStart, strStop;

	Project::Tools::OleTime_To_String(oleTimeStart, wstrStart);
	Project::Tools::OleTime_To_String(oleTimeEnd, wstrStop);
	Project::Tools::WideCharToUtf8(wstrStart, strStart);
	Project::Tools::WideCharToUtf8(wstrStop, strStop);

	std::ostringstream sqlstream;
	sqlstream << "select * from " << histablename 
		<< " where pointname = '"
		<< strName
		<< "' and time>='"
		<< Project::Tools::SystemTimeToString(stStart)
		<< "' and time<='"
		<< Project::Tools::SystemTimeToString(stEnd)
		<< "' order by time";


	string sql = sqlstream.str();
	// execute the query
	MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
	if (result == NULL)
	{
		return false;
	}

	int nBegin = 0;
	int nFirstNotNull = 0;
	bool bFirst = true;
	string strValue = "1.QNAN";
	MYSQL_ROW row = NULL;

	long rownum = GetRowNum(result);
	if (rownum == 0)				//不存在
	{

	}
	else
	{
		while(row = FetchRow(result))
		{
			SYSTEMTIME st;
			Project::Tools::String_To_SysTime(row[0], st);
			st.wSecond = 0;
			strValue = Project::Tools::Utf8ToMultiByte(row[2]);
			for(nBegin=0; nBegin<vecNeedEntry.size(); nBegin++)
			{
				if(CompareTwoSystemTime(st,vecNeedEntry[nBegin].st_begin) == 0)
				{
					if(bFirst)
					{
						nFirstNotNull = nBegin;
						bFirst = false;
					}
					vecNeedEntry[nBegin].strvalue = strValue;
					vecNeedEntry[nBegin].bMissing = false;
					break;
				}
			}
		}
		FreeReturnResult(result);
	}

	//第一个点不能为空 否则查找前一张表
	if(vecNeedEntry[0].bMissing)		//需要往前
	{
		string strValue = GetLastValue(strName,stStart,nMaxSearch);
		if(strValue.length()>0)
		{
			vecNeedEntry[0].strvalue = strValue;
			vecNeedEntry[0].bMissing = false;
		}
	}

	//实在找不到 就用-1
	if(vecNeedEntry[0].bMissing)
	{
		vecNeedEntry[0].strvalue = "1.QNAN";
		vecNeedEntry[0].bMissing = false;
	}

	//补齐
	for(int i=0; i<vecNeedEntry.size(); ++i)
	{
		if(i>0 && vecNeedEntry[i].bMissing)
		{
			vecNeedEntry[i].strvalue = vecNeedEntry[i-1].strvalue;
		}
	}

	//
	entryList.insert(entryList.end(),vecNeedEntry.begin(),vecNeedEntry.end());
	return true;
}

	string CDTUDatabasePointInfoAccess::GetHistoryTableNameByStoreCycle( const SYSTEMTIME &stNow, const POINT_STORE_CYCLE &tCycle )
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

	int CDTUDatabasePointInfoAccess::CompareTwoSystemTime( SYSTEMTIME st1, SYSTEMTIME st2 )
	{
		 int re=memcmp(&st1,&st2,sizeof(SYSTEMTIME));
		 return re;
	}

	bool CDTUDatabasePointInfoAccess::ImportDTUDataToMysql( vector<ImportSqlStruct> vecSqlCmd,LPImportDataProc proc1/*=NULL*/,DWORD userdata/*=NULL*/ )
	{
		Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);
		m_lpRecDataProc = proc1;
		StartTransaction();
		int nCount = 0;
		bool bResult = true;
		for(int i=0; i<vecSqlCmd.size(); ++i)
		{
			//创建表
			string sqlstatement = vecSqlCmd[i].strSql;
			bool bresult = Execute(sqlstatement);
			int nRow = vecSqlCmd[i].nRow;
			if (!bresult)
			{
				string strErr = GetMysqlError();
				vecSqlCmd[i].strErr = strErr;
				if(m_lpRecDataProc)
				{
					m_lpRecDataProc(nRow,false,strErr,userdata);
				}

				bResult = false;
			}
			else
			{
				if(m_lpRecDataProc)
				{
					m_lpRecDataProc(nRow,true,"",userdata);
				}
			}
			nCount++;
			if(nCount == 20)
			{
				Commit();
				//
				StartTransaction();
				nCount=0;
			}
		}
		Commit();
		return bResult;
	}

	bool CDTUDatabasePointInfoAccess::CheckCreateHistoryTable( vector<ImportSqlStruct> vecSqlCmd,LPImportDataProc proc1/*=NULL*/,DWORD userdata/*=NULL*/ )
	{
		Project::Tools::Scoped_Lock<Mutex> scopelock(m_lock);
		m_lpRecDataProc = proc1;
		StartTransaction();
		int nCount = 0;
		for(int i=0; i<vecSqlCmd.size(); ++i)
		{
			//创建表
			std::ostringstream sqlstreamCreate;
			sqlstreamCreate << "create table if not exists " << vecSqlCmd[i].strTableName <<  sql_createtable;
			string sqlstatement = sqlstreamCreate.str();
			int nRow = vecSqlCmd[i].nRow;
			bool bresult = Execute(sqlstatement);
			if (!bresult)
			{
				string strErr = GetMysqlError();
				vecSqlCmd[i].strErr = strErr;
				if(m_lpRecDataProc)
				{
					m_lpRecDataProc(nRow,false,strErr,userdata);
				}
			}
			else
			{
				if(m_lpRecDataProc)
				{
					m_lpRecDataProc(nRow,true,"",userdata);
				}
			}
			nCount++;

			if(nCount == 20)
			{
				Commit();
				//
				StartTransaction();
				nCount=0;
			}
		}

		Commit();
		return true;
	}

	bool CDTUDatabasePointInfoAccess::ReadHistoryDataInTable( vector<ExportValueEntry>& resultlist, const COleDateTime& start, const COleDateTime& end, const wstring &strPointName, const wstring &strTableName )
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
		
		int length = static_cast<int>(GetRowNum(result));
		if (0 == length){
			return false;
		}
		//resultlist.reserve(length);
		MYSQL_ROW  row = NULL;
		ExportValueEntry entry;

		while(row = FetchRow(result) )
		{   
			SYSTEMTIME sttime;
			Project::Tools::String_To_SysTime(row[0], sttime);
			entry.st_begin = sttime;
			entry.strvalue = Project::Tools::Utf8ToMultiByte(row[2]);
			resultlist.push_back(entry);
		}
		FreeReturnResult(result);				
		return true;
	}

	string CDTUDatabasePointInfoAccess::GetLastValue( const string& strName,SYSTEMTIME stTime, const int &nTimeFormat )
	{
		string strValue = "";
		COleDateTime oleTime(stTime);
		//先直接查当前表
		string histablename = GetHistoryTableNameByStoreCycle(stTime, (POINT_STORE_CYCLE) nTimeFormat);
	
		std::ostringstream sqlstream;
		sqlstream << "select * from " << histablename 
			<< " where pointname = '"
			<< strName
			<< "' and time<='"
			<< Project::Tools::SystemTimeToString(stTime)
			<< "' order by time desc limit 0,1;";

		string sql = sqlstream.str();
		// execute the query
		bool bExist = true;
		MYSQL_RES* result = QueryAndReturnResult(sql.c_str() );
		if (result == NULL)
		{
			bExist = false;
		}
		if(bExist)
		{
			long rownum = GetRowNum(result);
			if (rownum == 0){
				bExist = false;
			}

			if(bExist)
			{
				MYSQL_ROW  row = NULL;
				while(row = FetchRow(result))
				{
					strValue = Project::Tools::Utf8ToMultiByte(row[2]);
				}
				FreeReturnResult(result);
			}
		}

		if(!bExist)
		{
			for(int i=0; i<10; ++i)			//最多执行10次
			{
				if(strValue.length()>0)
					return strValue;
				if(nTimeFormat==E_STORE_FIVE_SECOND || nTimeFormat==E_STORE_ONE_MINUTE || nTimeFormat==E_STORE_FIVE_MINUTE)
				{	
					//存放于天表中
					oleTime = oleTime - COleDateTimeSpan(1,0,0,0);	
				}
				else if ( nTimeFormat==E_STORE_ONE_HOUR)
				{
					//
					oleTime = oleTime - COleDateTimeSpan(30,0,0,0);	
				}

				oleTime.GetAsSystemTime(stTime);
				histablename = GetHistoryTableNameByStoreCycle(stTime, (POINT_STORE_CYCLE) nTimeFormat);

				sqlstream.str("");
				sqlstream << "select * from " << histablename 
					<< " where pointname = '"
					<< strName
					<< "' and time>='"
					<< Project::Tools::SystemTimeToString(stTime)
					<< "' order by time desc limit 0,1;";

				sql = sqlstream.str();
				// execute the query
				bExist = true;
				result = QueryAndReturnResult(sql.c_str() );
				if (result == NULL)
				{
					bExist = false;
				}
				if(bExist)
				{
					long rownum1 = GetRowNum(result);
					if (rownum1 == 0){
						bExist = false;
					}

					if(bExist)
					{
						MYSQL_ROW  row = NULL;
						while(row = FetchRow(result))
						{
							strValue = Project::Tools::Utf8ToMultiByte(row[2]);
						}
						FreeReturnResult(result);
					}
				}
			}
		}
		return strValue;
	}

	int CDTUDatabasePointInfoAccess::CheckMinDataType()
	{
		Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
		int nDataType = E_STORE_ONE_DAY;
		//5s
		std::ostringstream sqlstream;
		sqlstream << "SELECT * FROM information_schema.`TABLES` where TABLE_SCHEMA='" 
			<< GetDBName() 
			<< "' and TABLE_NAME like '%5second%' order by TABLE_NAME limit 0,1;";

		MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
		if (result == NULL){
			return nDataType;
		}

		int length = static_cast<int>(GetRowNum(result));
		if (0 < length){
			nDataType = E_STORE_FIVE_SECOND;
			return nDataType;
		}

		//1m
		sqlstream.str("");
		sqlstream << "SELECT * FROM information_schema.`TABLES` where TABLE_SCHEMA='" 
			<< GetDBName() 
			<< "' and TABLE_NAME like '%_minute%' order by TABLE_NAME limit 0,1;";

		result = QueryAndReturnResult(sqlstream.str().c_str());
		if (result == NULL){
			return nDataType;
		}

		length = static_cast<int>(GetRowNum(result));
		if (0 < length){
			nDataType = E_STORE_ONE_MINUTE;
			return nDataType;
		}

		//5m
		sqlstream.str("");
		sqlstream << "SELECT * FROM information_schema.`TABLES` where TABLE_SCHEMA='" 
			<< GetDBName() 
			<< "' and TABLE_NAME like '%_5minute%' order by TABLE_NAME limit 0,1;";

		result = QueryAndReturnResult(sqlstream.str().c_str());
		if (result == NULL){
			return nDataType;
		}

		length = static_cast<int>(GetRowNum(result));
		if (0 < length){
			nDataType = E_STORE_FIVE_MINUTE;
			return nDataType;
		}

		//1h
		sqlstream.str("");
		sqlstream << "SELECT * FROM information_schema.`TABLES` where TABLE_SCHEMA='" 
			<< GetDBName() 
			<< "' and TABLE_NAME like '%_hour%' order by TABLE_NAME limit 0,1;";

		result = QueryAndReturnResult(sqlstream.str().c_str());
		if (result == NULL){
			return nDataType;
		}

		length = static_cast<int>(GetRowNum(result));
		if (0 < length){
			nDataType = E_STORE_ONE_HOUR;
			return nDataType;
		}

		//1d
		sqlstream.str("");
		sqlstream << "SELECT * FROM information_schema.`TABLES` where TABLE_SCHEMA='" 
			<< GetDBName() 
			<< "' and TABLE_NAME like '%_day_%' order by TABLE_NAME limit 0,1;";

		result = QueryAndReturnResult(sqlstream.str().c_str());
		if (result == NULL){
			return nDataType;
		}

		length = static_cast<int>(GetRowNum(result));
		if (0 < length){
			nDataType = E_STORE_ONE_DAY;
			return nDataType;
		}

		return nDataType;
	}

}

