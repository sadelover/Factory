#include "StdAfx.h"
#include "MDOPointConfigDataAccess.h"
#include "../Tools/Util/UtilString.h"
#include <locale.h>

CMDOPointConfigDataAccess::CMDOPointConfigDataAccess( void )
{
}

CMDOPointConfigDataAccess::~CMDOPointConfigDataAccess( void )
{

}

bool CMDOPointConfigDataAccess::CreateConnectToDB( const wstring& host, const wstring& username, const wstring& password, const wstring& database, unsigned int port )
{
	if(!ConnectToDB(host,username,password,database,port))
	{
		return  false;
	}
	return true;
}

bool CMDOPointConfigDataAccess::ReadMDOPointConfig( vector<MDOPointInfo>& vecPoint )
{
	Scoped_Lock<Mutex> mut(m_mutex);

	bool							bResult			= false;
	MYSQL_ROW						row;								//记录集
	static size_t					converted		= 0;

	SetConnectionCharacterSet(MYSQL_UTF8);

	MYSQL_RES* result = QueryAndReturnResult("select * from list_point where Source != 12 order by id ASC;");
	if (!result)
	{
		return false;
	}
	vecPoint.clear();
	while(row = FetchRow(result))		//读每一个page
	{
		//nIndex, strName, strSource, strParam1...strParam10, strRemark

		wstring str;
	
		MDOPointInfo strMDOPoint;
		strMDOPoint.nIndex = atoi(row[0]);
		Project::Tools::UTF8ToWideChar(row[1], str);
		strMDOPoint.strName = str;
		Project::Tools::UTF8ToWideChar(row[8], str);
		strMDOPoint.strRemark = str;
		Project::Tools::UTF8ToWideChar(row[7], str);
		strMDOPoint.strUnit = str;
		strMDOPoint.strSource = GetMDOPointSourceType(atoi(row[13]));
		Project::Tools::UTF8ToWideChar(row[14], str);
		strMDOPoint.strParam1 = str;
		Project::Tools::UTF8ToWideChar(row[15], str);
		strMDOPoint.strParam2 = str;
		Project::Tools::UTF8ToWideChar(row[16], str);
		strMDOPoint.strParam3 = str;
		Project::Tools::UTF8ToWideChar(row[17], str);
		strMDOPoint.strParam4 = str;
		Project::Tools::UTF8ToWideChar(row[18], str);
		strMDOPoint.strParam5 = str;
		Project::Tools::UTF8ToWideChar(row[19], str);
		strMDOPoint.strParam6 = str;
		Project::Tools::UTF8ToWideChar(row[20], str);
		strMDOPoint.strParam7 = str;
		Project::Tools::UTF8ToWideChar(row[21], str);
		strMDOPoint.strParam8 = str;
		Project::Tools::UTF8ToWideChar(row[22], str);
		strMDOPoint.strParam9 = str;
		Project::Tools::UTF8ToWideChar(row[23], str);
		strMDOPoint.strParam10 = str;

		vecPoint.push_back(strMDOPoint);
	}
	FreeReturnResult(result);
	return true;
}

wstring CMDOPointConfigDataAccess::GetMDOPointSourceType( int nSource )
{
	wstring strSourceType;
	switch(nSource)
	{
	case 0:
		strSourceType = L"";
			break;
	case 1:
		strSourceType = L"PH370";
			break;
	case 2:
		strSourceType = L"SIMENS1200";
			break;
	case 3:
		strSourceType = L"SIMENS300";
			break;
	case 4:
		strSourceType = L"AB500";
			break;
	case 5:
		strSourceType = L"modbus";
			break;
	case 6:
		strSourceType = L"vpoint";
			break;
	case 7:
		strSourceType = L"bacnet";
			break;
	case 8:
		strSourceType = L"lonworks";
			break;
	case 9:
		strSourceType = L"DB-Access";
			break;
	case 10:
		strSourceType = L"DB-SQLServer";
			break;
	case 11:
		strSourceType = L"DB-Oracle";
			break;
	case 12:
		strSourceType = L"DB-MySQL";
			break;
	case 13:
		strSourceType = L"custom1";
			break;
	case 14:
		strSourceType = L"custom2";
			break;
	case 15:
		strSourceType = L"custom3";
			break;
	case 16:
		strSourceType = L"custom4";
			break;
	case 17:
		strSourceType = L"custom5";
			break;
	default:
		break;
	}
	return strSourceType;
}
