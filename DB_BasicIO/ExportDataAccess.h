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
		int nType;		//存储类型
		string strSql;		//语句
		bool bResult;	//结果
		string strErr;	//错误信息
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

		 //将传感器数据导出到内存里。
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
		int index;				//编号
		WCHAR pointname[POINTNAME_SIZE];	//点名
		WCHAR comment[POINTCOMMENT_SIZE];		//注释
		int type;				//类型
		bool useindex;			//描述dtu历史数据使用编号还是点名来标识点位，true使用编号，false使用点名
		int	 reserved1;			//保留
		int  reserved2;			//保留

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

		//读数据中心的点。
		bool	ReadPointList(const string& databasename, vector<DtuPointEntry>& resultlist);

		//读本地数据库的点
		bool	ReadPointList_Local(const string& databasename, vector<DtuPointEntry>& resultlist);

		//导出DTU数据到文件
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
		LPImportDataProc	m_lpRecDataProc;	//回调
	};
}

