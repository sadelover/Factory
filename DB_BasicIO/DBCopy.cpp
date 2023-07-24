
#include "stdafx.h"

#include "DBInterfaceExp.h"
#include "DBCopy.h"
#include "../Tools/CustomTools/CustomTools.h"
#include <sstream>
#include <iostream>
#include <string>

using std::ostringstream;
using std::string;
using namespace Project::Tools;

DBCopy::DBCopy(CDatabaseSeesion *localcon, CDatabaseSeesion* servercon)
:m_log(_T("databasecopy.log")),
m_localconnection(localcon),
m_serverconnection(servercon)
{
	m_log.Set_LogConfig(TRUE, LOG_MINLEVEL_ALL);
}



bool DBCopy::CreateDatabaseOnServer( string dbname )
{
	assert(m_serverconnection != NULL);

	string  db_create = "CREATE DATABASE ";
	db_create += dbname;
	db_create += " DEFAULT CHARACTER SET utf8";

	return	m_serverconnection->Execute(db_create);
}




DBCopy::~DBCopy()
{

}


bool DBCopy::CopyTablesToServer(string dbname, bool truncatetable)
{
	bool bresult = false;
	for (vector<string>::iterator it = m_table_list_local.begin(); it != m_table_list_local.end(); it++)
	{	
		bresult = CopyTable(*it, dbname, m_localconnection, m_serverconnection, truncatetable);
		if (!bresult)
		{
			LogDBError( m_serverconnection->GetMysqlError());
			LogDBError(m_localconnection->GetMysqlError());
		}
	}
	
	return bresult;
}


void DBCopy::LogDBError(const char* sql)
{
	wstring mysqlerror;
	Project::Tools::String_To_Tstring(sql, mysqlerror);

	m_log.WriteLog(mysqlerror);
}

void DBCopy::LogDBError( const string& sql )
{
	LogDBError(sql.c_str());
}

// copy table from source to local
bool DBCopy::CopyTable( string tablename, string dbname, CDatabaseSeesion* source, CDatabaseSeesion* dest , bool truncatetable)
{
	string sql = "select * from ";
	sql += dbname;
	sql += ".";
	sql += tablename;
	
	MYSQL_RES* result = source->QueryAndReturnResult(sql.c_str());
	if (result == NULL)
	{
		return FALSE;
	}
	my_ulonglong rows = source->GetRowNum(result);
	unsigned int fieldnum = source->GetFieldNum(result);
	MYSQL_FIELD* fields= source->FetchField(result);

	// get the insert statement like 
	// insert into tablename values(,,,,), (,,,,,), (,,,,)
	// insert all the values at one time.
	ostringstream sqlstream;
	sqlstream << "insert into "<< tablename << " values";
		
	for (unsigned int i = 0; i < rows ; i++)
	{
		MYSQL_ROW row = source->FetchRow(result);
		for (unsigned int j = 0; j < fieldnum; j++)
		{
			string content;
			switch (fields[j].type)
			{
			case MYSQL_TYPE_TIMESTAMP:				
			case MYSQL_TYPE_DATETIME:				
			case MYSQL_TYPE_VARCHAR:
			case MYSQL_TYPE_VAR_STRING:
			case MYSQL_TYPE_DATE:
				 content = "'";
				 if (row[j] == NULL)
					content += "";
				 else
					content += row[j];
				 content += "'";
				 break;
			case MYSQL_TYPE_TINY_BLOB:				 
			case MYSQL_TYPE_MEDIUM_BLOB:
			case MYSQL_TYPE_LONG_BLOB:
			case MYSQL_TYPE_BLOB:
				{
					unsigned long* length = source->GetFieldLengths(result);
					content = "'";
					content.append(row[j], length[j]); 
					content += "'";
					break;
				}
			default:
				if (row[j] == NULL)
					content = "";
				else
					content = row[j];
			}

			if (j == 0)
				sqlstream << "(" << content << ",";
			else if (j < fieldnum - 1)
				sqlstream << content << ",";
			else
				sqlstream << content << ")" << ",";			
		}
	}
	source->FreeReturnResult(result);

	sql = sqlstream.str();
	sql.erase(sql.end() - 1);

	if (dest->StartTransaction())
	{
		if (truncatetable)
			dest->TruncateTable(tablename.c_str());
		dest->Execute(sql);
		dest->Commit();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
		
	return FALSE;
}

bool DBCopy::CopyTablesToLocal(string dbname, bool truncatetable)
{
	bool bresult = false;
	for (vector<string>::iterator it = m_table_list_local.begin(); it != m_table_list_local.end(); it++)
	{	
		bresult = CopyTable(*it, dbname, m_serverconnection, m_localconnection, truncatetable);
		if (!bresult)
		{
			LogDBError( m_serverconnection->GetMysqlError());
			LogDBError(m_localconnection->GetMysqlError());
		}
	}

	return bresult;
}

bool DBCopy::CreateDatabaseOnLocal( string dbname )
{
	assert(m_localconnection != NULL);

	string  db_create = "CREATE DATABASE ";
	db_create += dbname;
	db_create += "DEFAULT CHARACTER SET utf8";

	return	!m_localconnection->Execute(db_create);
}

bool DBCopy::GetTableListOnLocal( string dbname )
{
	assert(m_localconnection != NULL);

	m_table_list_local.clear();

	string sql = "show tables from ";
	sql += dbname;

	MYSQL_RES* result = m_localconnection->QueryAndReturnResult(sql.c_str());
	if (result != NULL)
	{
		MYSQL_ROW row;
		while (row = m_localconnection->FetchRow(result))
		{
			m_table_list_local.push_back(row[0]);
		}
		m_localconnection->FreeReturnResult(result);
	}

	return TRUE;
}

bool DBCopy::GetTableListOnServer( string dbname )
{
	assert(m_serverconnection != NULL);

	m_table_list_server.clear();

	string sql = "show tables from ";
	sql += dbname;

	MYSQL_RES* result = m_serverconnection->QueryAndReturnResult(sql.c_str());
	if (result != NULL)
	{
		MYSQL_ROW row;
		while (row = m_serverconnection->FetchRow(result))
		{
			m_table_list_server.push_back(row[0]);
		}
		m_serverconnection->FreeReturnResult(result);
	}

	return TRUE;
}

string DBCopy::GetCreateTableSql( string dbname, string tablename , CDatabaseSeesion* con)
{
	string sql = "show create table ";
	sql += dbname;
	sql += ".";
	sql += tablename;

	string resultsql;
	MYSQL_RES* result = con->QueryAndReturnResult(sql.c_str());
	if (result != NULL)
	{
		MYSQL_ROW row = con->FetchRow(result);
		if (row != NULL)
		{
			resultsql = row[1];
		}
		con->FreeReturnResult(result);
	}
	else
	{
		LogDBError(con->GetMysqlError());
	}

	return resultsql;
}

bool DBCopy::SyncDatabaseFromServer( string dbname )
{
	// check whether the database exist at the local server.

	m_localconnection->SetConnectionCharacterSet(MYSQL_UTF8);
	m_serverconnection->SetConnectionCharacterSet(MYSQL_UTF8);

	if (!CheckDataBaseExists(m_localconnection, dbname))
	{
		// create failed.
		if (!CreateDatabaseOnLocal(dbname))
		{
			LogDBError(m_localconnection->GetMysqlError());
			return FALSE;
		}
		
		m_localconnection->SelectDB(dbname.c_str());
		// get tables list from remote server.
		if (!GetTableListOnServer(dbname))
			return FALSE;
		
		// get server table create statement and create the table at the local database.
		for (vector<string>::iterator it = m_table_list_server.begin(); it != m_table_list_server.end(); it++)
		{
			string sql = GetCreateTableSql(dbname, *it, m_serverconnection);
			if (sql.empty())
			{
				return FALSE;
			}
			m_localconnection->Execute(sql.c_str());
		}
		
		return CopyTablesToLocal(dbname, FALSE);
	}

	else
	{
		if (!GetTableListOnServer(dbname))
			return FALSE;
		return CopyTablesToLocal(dbname, TRUE);
	}

	
}

bool DBCopy::CheckDataBaseExists( CDatabaseSeesion* con, string dbname)
{
	string sql = "use ";
	sql += dbname;
	return con->Execute(sql.c_str());
}

bool DBCopy::SyncDatabaseToServer( string dbname )
{
	assert(m_localconnection != NULL);
	assert(m_serverconnection != NULL);
	// check whether the database exist at the local server.
	m_localconnection->SetConnectionCharacterSet(MYSQL_UTF8);
	m_serverconnection->SetConnectionCharacterSet(MYSQL_UTF8);

	if (!CheckDataBaseExists(m_serverconnection, dbname))
	{
		// create failed.
		if (!CreateDatabaseOnServer(dbname))
		{
			LogDBError(m_serverconnection->GetMysqlError());
			return FALSE;
		}

		m_serverconnection->SelectDB(dbname.c_str());
		// get tables list from remote server.
		if (!GetTableListOnLocal(dbname))
			return FALSE;

		// get server table create statement and create the table at the local database.
		for (vector<string>::iterator it = m_table_list_local.begin(); it != m_table_list_local.end(); it++)
		{
			string sql = GetCreateTableSql(dbname, *it, m_localconnection);
			if (sql.empty())
			{
				return FALSE;
			}
			m_serverconnection->Execute(sql.c_str());
		}

		return CopyTablesToServer(dbname, FALSE);
	}

	else
	{
		if (!GetTableListOnLocal(dbname))
			return FALSE;
		return CopyTablesToServer(dbname, TRUE);
	}

	return FALSE;
}

bool DBCopy::ReadAllProjectsFromServer( vector<Project_DB>& projectlist, const wstring& creator, CDatabaseSeesion* con)
{

	con->SetConnectionCharacterSet(MYSQL_UTF8);
	Scoped_Lock<Mutex> local_lock(m_mutex);

	// get the database names;
	vector<string> dbnames;
	if(!con->QueryAndStoreResult("show databases like 'uppc%%'"))
	{
#if _DEBUG
		string error = con->GetMysqlError();
#endif
		return FALSE;
	}
	
	MYSQL_ROW row;
	while(row = con->GetRow())
	{
		string name  = row[0];
		dbnames.push_back(name);
	}
	con->FreeResultSet();

	for (vector<string>::iterator it = dbnames.begin(); it != dbnames.end(); it++)
	{
		string user = tstringToString(creator);
		string sql = "select name from ";
		sql += *it;
		sql += ".project where creator = '";
		sql += user;
		sql += "'";


		// get the project name
		MYSQL_RES* result = con->QueryAndReturnResult(sql.c_str());
		if (result != NULL)
		{						
			MYSQL_ROW row = con->FetchRow(result);
			if (row != NULL)
			{
				Project_DB projectdb;
				projectdb.strdatabasename = *it;

				if (row[0] != NULL)
					UTF8ToWideChar(row[0], projectdb.strprojectname);
                
				projectlist.push_back(projectdb);
			}
			con->FreeReturnResult(result);
		}		
	}

	return TRUE;
}




