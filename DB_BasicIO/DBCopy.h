#pragma  once

#include "DBAccessContainer.h"

// this class will copy or update a local server
// to a remote server
enum SYNCMODE
{
	UPDATE = 0,	 // 更新
	COPY		 // 拷贝
};

class DBDLL DBCopy
{
public:
	DBCopy(CDatabaseSeesion* localcon, CDatabaseSeesion *serverconnection);
	~DBCopy();

public:
	// copy the table contents from the server
	// 在两个server之间拷贝表的内容
	// 假定两个服务器上的表均已存在
	bool CopyTable(string tablename, string dbname, CDatabaseSeesion* source, CDatabaseSeesion* dest, bool truncatetable);

	bool CopyTablesToServer(string dbname, bool truncatetable);

	bool CopyTablesToLocal(string dbname, bool truncatetable);
		
	bool CreateDatabaseOnServer(string dbname);	// 在服务器上创建数据库

	bool CreateDatabaseOnLocal(string dbname);  // 在本地服务器创建数据库.
			
	bool GetTableListOnLocal(string dbname);			// 取得local服务器所有表名

	bool GetTableListOnServer(string dbname);			// 取得server服务器所有表名

	void LogDBError(const char* error);					//记录数据库错误	

	void LogDBError(const string& error);				// 记录数据库错误

	bool SyncDatabaseToServer(string dbname);	// 向服务器同步数据库

	bool SyncDatabaseFromServer(string dbname); // 从服务器同步数据库

	bool CreateTable(CDatabaseSeesion* con, string sql_create);	// 创建所有表

	string GetCreateTableSql(string dbname, string tablename, CDatabaseSeesion* con);

	bool CheckDataBaseExists(CDatabaseSeesion* con, string dbname);	

	bool ReadAllProjectsFromServer(vector<Project_DB>& projectlist, const wstring& creator, CDatabaseSeesion* con);

private:
	vector<string>	m_table_list_local;
	vector<string>	m_table_list_server;
	Mutex			m_mutex;
	CUseLog			m_log;
	CDatabaseSeesion* m_localconnection;
	CDatabaseSeesion* m_serverconnection;
};


