#pragma once


#include "DatabaseSession.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "../DataPoint/DataPointEntry.h"

namespace ExportDataAccess
{

	struct	_declspec(dllexport) ExportParam{
		string	tablename;
		string  dbname;
		SYSTEMTIME	st_begin;
		SYSTEMTIME	st_end;
		string	exportpath;
		string pointname;
	};

	struct _declspec(dllexport) ExportValueEntry
	{
		SYSTEMTIME	st_begin;
		//string		pointname;
		string		strvalue;
		bool		bMissing;
		ExportValueEntry(){
			strvalue = "1.QNAN";
			GetLocalTime(&st_begin);
			bMissing = true;
		}
	};

	struct _declspec(dllexport) ExportValueTime
	{
		COleDateTime oleTime;
		bool bMissing;
		ExportValueTime(){
			bMissing = false;
		}
	};

	struct _declspec(dllexport) ImportSqlStruct
	{
		int nRow;
		int nType;		//�洢����
		string strSql;		//���
		bool bResult;	//���
		string strErr;	//������Ϣ
		string strTableName; //
		ImportSqlStruct(){
			bResult = true;
			strSql = "";
			nType = 0;
			nRow = 0;
			strErr = "";
			strTableName = "";
		}
	};

	class ExportDataAccess: public CDatabaseSeesion
	{
	public:
		 bool	ExportSensorDataDTU(const ExportParam& exportparam);
		 bool	ExportRecordCommon(const ExportParam& exportparam);

		 //�����������ݵ������ڴ��
		 bool	ExportSensorDataDTU(const ExportParam& exportparam, vector<ExportValueEntry>& resultlist);
	private:
		Project::Tools::Mutex	m_lock;
	};
	

	struct _declspec(dllexport) DTUDatabaseInfo{
		std::wstring databasename;
		std::wstring projectname;
	};
	
	struct _declspec(dllexport) DtuPointEntry
	{
		enum {
			POINTNAME_SIZE = 128,
			POINTCOMMENT_SIZE = 256
		};
		int index;				//���
		WCHAR pointname[POINTNAME_SIZE];	//����
		WCHAR comment[POINTCOMMENT_SIZE];		//ע��
		int type;				//����
		bool useindex;			//����dtu��ʷ����ʹ�ñ�Ż��ǵ�������ʶ��λ��trueʹ�ñ�ţ�falseʹ�õ���
		int	 reserved1;			//����
		int  reserved2;			//����

		DtuPointEntry(){
			index = 0;
			memset(pointname, 0, POINTNAME_SIZE);
			memset(comment, 0, POINTCOMMENT_SIZE);
			type = 0;
			useindex = false;
			reserved1 = reserved2 = 0;
		}
	};

	typedef LRESULT (*LPImportDataProc)(int nRow,bool bResult,string strErr,DWORD userdata); 

	class _declspec(dllexport) CDTUDatabasePointInfoAccess : public CDatabaseSeesion
	{
	public:
		bool	ReadDatabaseList(vector<DTUDatabaseInfo>& dblist);

		//���������ĵĵ㡣
		bool	ReadPointList(const string& databasename, vector<DtuPointEntry>& resultlist);

		//���������ݿ�ĵ�
		bool	ReadPointList_Local(const string& databasename, vector<DtuPointEntry>& resultlist);

		//����DTU���ݵ��ļ�
		bool	ExportDTUDataToFile(const ExportParam& exportparam);

		bool	ExportDTUDataToMemory(const ExportParam& exportparam, vector<ExportValueEntry>& resultlist);

		bool	ExportDTUDataToMemory(const string& dbname,const string& tbname,const string& pointname,const string& start, const string& end, vector<ExportValueEntry>& resultlist);

		bool	GetHistoryValue(const string& strName,const SYSTEMTIME &stStart, SYSTEMTIME stEnd,const int &nTimeFormat, vector<ExportValueEntry>& entryList);

		string	GetLastValue(const string& strName, SYSTEMTIME stTime, const int &nTimeFormat);

		bool	GetHistoryValue(const string& strName,const SYSTEMTIME &stStart, SYSTEMTIME stEnd,const int &nTimeFormat, const int &nNeedTimeFormat,const int &nMinTimeFormat,vector<ExportValueEntry>& entryList);
	
		bool	ReadHistoryDataInTable(vector<ExportValueEntry>& resultlist, const COleDateTime& start, const COleDateTime& end, const wstring &strPointName, const wstring &strTableName);

		string	GetHistoryTableNameByStoreCycle(const SYSTEMTIME &stNow, const POINT_STORE_CYCLE &tCycle);

		int		CompareTwoSystemTime(SYSTEMTIME st1, SYSTEMTIME st2);

		bool	ImportDTUDataToMysql(vector<ImportSqlStruct> vecSqlCmd,LPImportDataProc proc1=NULL,DWORD userdata=NULL);
		bool    CheckCreateHistoryTable(vector<ImportSqlStruct> vecSqlCmd,LPImportDataProc proc1=NULL,DWORD userdata=NULL);

		int		CheckMinDataType();
	private:
		Project::Tools::Mutex m_lock;
		LPImportDataProc	m_lpRecDataProc;	//�ص�
	};
}

