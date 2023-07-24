#pragma  once

#include "DBAccessContainer.h"

// this class will copy or update a local server
// to a remote server
enum SYNCMODE
{
	UPDATE = 0,	 // ����
	COPY		 // ����
};

class DBDLL DBCopy
{
public:
	DBCopy(CDatabaseSeesion* localcon, CDatabaseSeesion *serverconnection);
	~DBCopy();

public:
	// copy the table contents from the server
	// ������server֮�俽���������
	// �ٶ������������ϵı���Ѵ���
	bool CopyTable(string tablename, string dbname, CDatabaseSeesion* source, CDatabaseSeesion* dest, bool truncatetable);

	bool CopyTablesToServer(string dbname, bool truncatetable);

	bool CopyTablesToLocal(string dbname, bool truncatetable);
		
	bool CreateDatabaseOnServer(string dbname);	// �ڷ������ϴ������ݿ�

	bool CreateDatabaseOnLocal(string dbname);  // �ڱ��ط������������ݿ�.
			
	bool GetTableListOnLocal(string dbname);			// ȡ��local���������б���

	bool GetTableListOnServer(string dbname);			// ȡ��server���������б���

	void LogDBError(const char* error);					//��¼���ݿ����	

	void LogDBError(const string& error);				// ��¼���ݿ����

	bool SyncDatabaseToServer(string dbname);	// �������ͬ�����ݿ�

	bool SyncDatabaseFromServer(string dbname); // �ӷ�����ͬ�����ݿ�

	bool CreateTable(CDatabaseSeesion* con, string sql_create);	// �������б�

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


