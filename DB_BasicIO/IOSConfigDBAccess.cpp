#include "StdAfx.h"
#include "IOSConfigDBAccess.h"
#include <iostream>
#include <sstream>
#include "MysqlDeleter.h"
#include "../Tools/Scope_Ptr.h"
#include "../Tools/Util/UtilString.h"

//Operation record will hold a session
const char* tablename_user			= "user";
const char* tablename_user_to_prj   = "user_to_project";
const char* tablename_project		= "project";
const char* tablename_page			= "page";
const char* tablename_paramnode		= "paramnode";
const char* tablename_paramnodeitem = "paramnodeitem";
const char* tablename_chart			= "chart";
const char* tablename_chartitem		= "chartitem";
const char* tablename_warning		= "warning";
const char* tablename_setting		= "setting";
const char* tablename_token			= "equipment_token";
const char* tablename_compare		= "compareitem";
const char* tablename_comparechart  = "comparechart";
const char* tablename_comparechartitem = "comparechartitem";
const char* tablename_usergroup		= "usergroup";
const char* tablename_usertogroup	= "user_to_group";

CIOSConfigDBAccess::CIOSConfigDBAccess( void )
{

}

CIOSConfigDBAccess::~CIOSConfigDBAccess( void )
{

}

bool CIOSConfigDBAccess::LoginIOSConfigOK( const wstring strUser, const wstring strPsw )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strUser_utf8;
	Project::Tools::WideCharToUtf8(strUser, strUser_utf8);

	string strPsw_utf8;
	Project::Tools::WideCharToUtf8(strPsw, strPsw_utf8);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_user
		<< " where name = '"
		<< strUser_utf8 
		<< "' and psw = '"
		<< strPsw_utf8
		<< "';";


	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}

	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}
	
	return true;
}

bool CIOSConfigDBAccess::ReadAllProjecs( vector<IOSProjectInfo>& vecProject )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecProject.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_project
		<< "  order by id";

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
	vecProject.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSProjectInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nPort = ATOI(row[6]);
		wcItem.strPrjName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strDBIP = Project::Tools::UTF8ToWideChar(row[2]);
		wcItem.strDBUser = Project::Tools::UTF8ToWideChar(row[3]);
		wcItem.strPsw = Project::Tools::UTF8ToWideChar(row[4]);
		wcItem.strDBDataBase = Project::Tools::UTF8ToWideChar(row[5]);
		wcItem.strPrjDescription = Project::Tools::UTF8ToWideChar(row[7]);
		wcItem.strCountry = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.strLocation = Project::Tools::UTF8ToWideChar(row[9]);
		wcItem.strPicPath = Project::Tools::UTF8ToWideChar(row[10]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[11]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[12]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[13]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[14]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[15]);
		vecProject.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOneProject( const IOSProjectInfo sProjectInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	
	string strPrjName_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strPrjName, strPrjName_utf8);
	string strDBIP_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strDBIP, strDBIP_utf8);
	string strDBUser_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strDBUser, strDBUser_utf8);
	string strPsw_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strPsw, strPsw_utf8);
	string strDBDataBase_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strDBDataBase, strDBDataBase_utf8);
	string strPrjDescription_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strPrjDescription, strPrjDescription_utf8);
	string strCountry_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strCountry, strCountry_utf8);
	string strLocation_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strLocation, strLocation_utf8);
	string strPicPath_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strPicPath, strPicPath_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_project 
		<< "(name, Host, Username, Password, DbSchema, Port, Description, Country, Location, Picture, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
		<< strPrjName_utf8 << "','"<< strDBIP_utf8 << "','"
		<< strDBUser_utf8 << "','" << strPsw_utf8 << "','" << strDBDataBase_utf8 << "',"
		<< sProjectInfo.nPort << ",'" << strPrjDescription_utf8 << "','" << strCountry_utf8 << "','"
		<< strLocation_utf8 << "','" << strPicPath_utf8  << "','" << strUnitproperty01_utf8  << "','"
		<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
		<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	
	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneProjectByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_project
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneProjectByID( const int nID, const IOSProjectInfo sProjectInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strPrjName_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strPrjName, strPrjName_utf8);
	string strDBIP_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strDBIP, strDBIP_utf8);
	string strDBUser_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strDBUser, strDBUser_utf8);
	string strPsw_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strPsw, strPsw_utf8);
	string strDBDataBase_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strDBDataBase, strDBDataBase_utf8);
	string strPrjDescription_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strPrjDescription, strPrjDescription_utf8);
	string strCountry_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strCountry, strCountry_utf8);
	string strLocation_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strLocation, strLocation_utf8);
	string strPicPath_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strPicPath, strPicPath_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sProjectInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	if(sProjectInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_project 
			<< " set name = '" << strPrjName_utf8 << "',Host='" << strDBIP_utf8
			<< "', Username='" << strDBUser_utf8 << "',Password='" << strPsw_utf8
			<< "',DbSchema='" << strDBDataBase_utf8 << "',Port=" << sProjectInfo.nPort
			<< ", Description='"<< strPrjDescription_utf8 << "',Country='" << strCountry_utf8
			<< "',Location='" << strLocation_utf8 << "',Picture='" << strPicPath_utf8
			<< "',unitproperty01='" << strUnitproperty01_utf8 << "',unitproperty02='" << strUnitproperty02_utf8
			<< "',unitproperty03='" << strUnitproperty03_utf8 << "',unitproperty04='" << strUnitproperty04_utf8
			<< "',unitproperty05='" << strUnitproperty04_utf8 << "' where id=" << sProjectInfo.nID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::ReadOneProjecByID( const int nID, IOSProjectInfo& sProjectInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_project
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sProjectInfo.nID = ATOI(row[0]);
		sProjectInfo.nPort = ATOI(row[6]);
		sProjectInfo.strPrjName = Project::Tools::UTF8ToWideChar(row[1]);
		sProjectInfo.strDBIP = Project::Tools::UTF8ToWideChar(row[2]);
		sProjectInfo.strDBUser = Project::Tools::UTF8ToWideChar(row[3]);
		sProjectInfo.strPsw = Project::Tools::UTF8ToWideChar(row[4]);
		sProjectInfo.strDBDataBase = Project::Tools::UTF8ToWideChar(row[5]);
		sProjectInfo.strPrjDescription = Project::Tools::UTF8ToWideChar(row[7]);
		sProjectInfo.strCountry = Project::Tools::UTF8ToWideChar(row[8]);
		sProjectInfo.strLocation = Project::Tools::UTF8ToWideChar(row[9]);
		sProjectInfo.strPicPath = Project::Tools::UTF8ToWideChar(row[10]);
		sProjectInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[11]);
		sProjectInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[12]);
		sProjectInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[13]);
		sProjectInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[14]);
		sProjectInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[15]);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadAllPagesByProjectID( const int nPrjID, vector<IOSPageInfo>& vecPage )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecPage.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_page
		<< " where projectid="
		<< nPrjID
		<< "  order by id";

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
	vecPage.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSPageInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nType = ATOI(row[2]);
		wcItem.nProjectID = ATOI(row[3]);
		wcItem.nOrder	= ATOI(row[4]);
		wcItem.strPageName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[5]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[6]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[7]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[9]);
		vecPage.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOnePageByID( const int nID, IOSPageInfo& sPageInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_page
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sPageInfo.nID = ATOI(row[0]);
		sPageInfo.nType = ATOI(row[2]);
		sPageInfo.nProjectID = ATOI(row[3]);
		sPageInfo.nOrder	= ATOI(row[4]);
		sPageInfo.strPageName = Project::Tools::UTF8ToWideChar(row[1]);
		sPageInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[5]);
		sPageInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[6]);
		sPageInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[7]);
		sPageInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[8]);
		sPageInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[9]);
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOnePage( const IOSPageInfo sPageInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strPageName_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strPageName, strPageName_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_page 
		<< "(name, pagetype, projectid, pageorder, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
		<< strPageName_utf8 << "'," << sPageInfo.nType << ","
		<< sPageInfo.nProjectID << "," << sPageInfo.nOrder << ",'"
		<< strUnitproperty01_utf8  << "','"
		<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
		<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	
	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOnePageByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_page
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOnePageByID( const int nID, const IOSPageInfo sPageInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strPageName_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strPageName, strPageName_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sPageInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	if(sPageInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_page 
			<< " set name='" << strPageName_utf8 << "',pagetype=" << sPageInfo.nType
			<< ",projectid=" << sPageInfo.nProjectID << ",pageorder=" << sPageInfo.nOrder
			<< ",unitproperty01='" << strUnitproperty01_utf8 << "',unitproperty02='" << strUnitproperty02_utf8
			<< "',unitproperty03='" << strUnitproperty03_utf8 << "',unitproperty04='" << strUnitproperty04_utf8
			<< "',unitproperty05='" << strUnitproperty04_utf8 << "' where id=" << sPageInfo.nID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::ReadAllParamByPageID( const int nPage, vector<IOSParamInfo>& vecParam )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecParam.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_paramnode
		<< " where pageid="
		<< nPage
		<< "  order by id";

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
	vecParam.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSParamInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nOrder	= ATOI(row[3]);
		wcItem.nPageID = ATOI(row[2]);
		wcItem.strTitle = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[4]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[5]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[6]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[7]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[8]);
		vecParam.push_back(wcItem);
	}

	for(int i=0; i<vecParam.size(); ++i)
	{
		ReadAllParamItemByParamID(vecParam[i].nID, vecParam[i].vecItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneParamByID( const int nID, IOSParamInfo& sParamInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_paramnode
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sParamInfo.nID = ATOI(row[0]);
		sParamInfo.nOrder	= ATOI(row[3]);
		sParamInfo.nPageID = ATOI(row[2]);
		sParamInfo.strTitle = Project::Tools::UTF8ToWideChar(row[1]);
		sParamInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[4]);
		sParamInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[5]);
		sParamInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[6]);
		sParamInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[7]);
		sParamInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[8]);
	}
	ReadAllParamItemByParamID(sParamInfo.nID, sParamInfo.vecItem);
	return true;
}

bool CIOSConfigDBAccess::InsertOneParam( const IOSParamInfo sParamInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strParamTitle_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strTitle, strParamTitle_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_paramnode 
		<< "(title, pageid, paramorder, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
		<< strParamTitle_utf8 << "'," << sParamInfo.nPageID << ","
		<< sParamInfo.nOrder << ",'"
		<< strUnitproperty01_utf8  << "','"
		<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
		<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";	

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneParamByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_paramnode
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneParamByID( const int nID, const IOSParamInfo sParamInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strParamTitle_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strTitle, strParamTitle_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sParamInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	if(sParamInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_paramnode 
			<< " set title='" << strParamTitle_utf8 << "',pageid=" << sParamInfo.nPageID
			<< ",paramorder=" << sParamInfo.nOrder 
			<< ",unitproperty01='" << strUnitproperty01_utf8 << "',unitproperty02='" << strUnitproperty02_utf8
			<< "',unitproperty03='" << strUnitproperty03_utf8 << "',unitproperty04='" << strUnitproperty04_utf8
			<< "',unitproperty05='" << strUnitproperty04_utf8 << "' where id=" << sParamInfo.nID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::ReadAllParamItemByParamID( const int nParamID, vector<IOSParamItemInfo>& vecItem )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecItem.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_paramnodeitem
		<< " where paramid="
		<< nParamID
		<< "  order by id";

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
	vecItem.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSParamItemInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nOrder	= ATOI(row[7]);
		wcItem.nParamID = ATOI(row[8]);
		wcItem.nPointType = ATOI(row[3]);
		wcItem.dHigh = ATOF(row[5]);
		wcItem.dLow = ATOF(row[6]);
		wcItem.strName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strPointName = Project::Tools::UTF8ToWideChar(row[2]);
		wcItem.strUnit = Project::Tools::UTF8ToWideChar(row[4]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[9]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[10]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[11]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[12]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[13]);
		vecItem.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneParamItemByID( const int nID, IOSParamItemInfo& sParamItemInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_paramnodeitem
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sParamItemInfo.nID = ATOI(row[0]);
		sParamItemInfo.nOrder	= ATOI(row[7]);
		sParamItemInfo.nParamID = ATOI(row[8]);
		sParamItemInfo.nPointType = ATOI(row[3]);
		sParamItemInfo.dHigh = ATOF(row[5]);
		sParamItemInfo.dLow = ATOF(row[6]);
		sParamItemInfo.strName = Project::Tools::UTF8ToWideChar(row[1]);
		sParamItemInfo.strPointName = Project::Tools::UTF8ToWideChar(row[2]);
		sParamItemInfo.strUnit = Project::Tools::UTF8ToWideChar(row[4]);
		sParamItemInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[9]);
		sParamItemInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[10]);
		sParamItemInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[11]);
		sParamItemInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[12]);
		sParamItemInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[13]);
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOneParamItem( const IOSParamItemInfo sParamItemInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strParamName_utf8;
	Project::Tools::WideCharToUtf8(sParamItemInfo.strName, strParamName_utf8);
	string strPointName_utf8;
	Project::Tools::WideCharToUtf8(sParamItemInfo.strPointName, strPointName_utf8);
	string strUnit_utf8;
	Project::Tools::WideCharToUtf8(sParamItemInfo.strUnit, strUnit_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sParamItemInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sParamItemInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sParamItemInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sParamItemInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sParamItemInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	if(sParamItemInfo.nID > 0)
	{
		sqlstream << "insert into "
			<< GetDBName()
			<<"."
			<< tablename_paramnodeitem 
			<< " value ("
			<< sParamItemInfo.nID << ",'"
			<< strParamName_utf8 << "','" << strPointName_utf8 << "'," << sParamItemInfo.nPointType << ",'" 
			<< strUnit_utf8 << "'," << sParamItemInfo.dHigh << "," << sParamItemInfo.dLow << ","
			<< sParamItemInfo.nOrder << ","<< sParamItemInfo.nParamID << ",'"
			<< strUnitproperty01_utf8  << "','"
			<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
			<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	}
	else
	{
		sqlstream << "insert into "
			<< GetDBName()
			<<"."
			<< tablename_paramnodeitem 
			<< "(name, pointname, pointtype,unit, high, low, itemorder, paramid, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
			<< strParamName_utf8 << "','" << strPointName_utf8 << "'," << sParamItemInfo.nPointType << ",'"  
			<< strUnit_utf8 << "'," << sParamItemInfo.dHigh << "," << sParamItemInfo.dLow << ","
			<< sParamItemInfo.nOrder << ","<< sParamItemInfo.nParamID << ",'"
			<< strUnitproperty01_utf8  << "','"
			<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
			<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	}

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneParamItemByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_paramnodeitem
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneParamItemByID( const int nID, const IOSParamItemInfo sParamItemInfo )
{
	DeleteOneParamItemByID(nID);
	return InsertOneParamItem(sParamItemInfo);
}

bool CIOSConfigDBAccess::ReadAllChartByPageID( const int nPage, vector<IOSChartInfo>& vecChart )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecChart.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_chart
		<< " where pageid = "
		<< nPage
		<< "  order by id";

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
	vecChart.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSChartInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nType	= ATOI(row[2]);
		wcItem.nYInterval = ATOI(row[7]);
		wcItem.nPageID = ATOI(row[9]);
		wcItem.strName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strXName = Project::Tools::UTF8ToWideChar(row[3]);
		wcItem.strXUnit = Project::Tools::UTF8ToWideChar(row[4]);
		wcItem.strYName = Project::Tools::UTF8ToWideChar(row[5]);
		wcItem.strYUnit = Project::Tools::UTF8ToWideChar(row[6]); 
		wcItem.strDescription = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[10]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[11]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[12]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[13]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[14]);
		vecChart.push_back(wcItem);
	}

	for(int i=0; i<vecChart.size(); ++i)
	{
		ReadAllChartItemByChartID(vecChart[i].nID, vecChart[i].vecItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneChartByID( const int nID, IOSChartInfo& sChartInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_chart
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sChartInfo.nID = ATOI(row[0]);
		sChartInfo.nType	= ATOI(row[2]);
		sChartInfo.nYInterval = ATOI(row[7]);
		sChartInfo.nPageID = ATOI(row[9]);
		sChartInfo.strName = Project::Tools::UTF8ToWideChar(row[1]);
		sChartInfo.strXName = Project::Tools::UTF8ToWideChar(row[3]);
		sChartInfo.strXUnit = Project::Tools::UTF8ToWideChar(row[4]);
		sChartInfo.strYName = Project::Tools::UTF8ToWideChar(row[5]);
		sChartInfo.strYUnit = Project::Tools::UTF8ToWideChar(row[6]); 
		sChartInfo.strDescription = Project::Tools::UTF8ToWideChar(row[8]);
		sChartInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[10]);
		sChartInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[11]);
		sChartInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[12]);
		sChartInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[13]);
		sChartInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[14]);
	}
	ReadAllChartItemByChartID(sChartInfo.nID, sChartInfo.vecItem);
	return true;
}

bool CIOSConfigDBAccess::InsertOneChart( const IOSChartInfo sChartInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strChartName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strName, strChartName_utf8);
	string strChartXName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strXName, strChartXName_utf8);
	string strChartXUnit_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strXUnit, strChartXUnit_utf8);
	string strChartYName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strYName, strChartYName_utf8);
	string strChartYUnit_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strYUnit, strChartYUnit_utf8);
	string strDescription_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strDescription, strDescription_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_chart 
		<< "(name, charttype, x_name, x_unit, y_name, y_unit, y_interval, Description, pageid, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
		<< strChartName_utf8 << "'," << sChartInfo.nType << ",'" << strChartXName_utf8 << "','"
		<< strChartXUnit_utf8 << "','" << strChartYName_utf8 << "','" << strChartYUnit_utf8 << "',"
		<< sChartInfo.nYInterval << ",'" 
		<< strDescription_utf8 << "'," << sChartInfo.nPageID << ",'" 
		<< strUnitproperty01_utf8  << "','"
		<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
		<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	
	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneChartByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_chart
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneChartByID( const int nID, const IOSChartInfo sChartInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strChartName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strName, strChartName_utf8);
	string strChartXName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strXName, strChartXName_utf8);
	string strChartXUnit_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strXUnit, strChartXUnit_utf8);
	string strChartYName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strYName, strChartYName_utf8);
	string strChartYUnit_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strYUnit, strChartYUnit_utf8);
	string strDescription_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strDescription, strDescription_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	if(sChartInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_chart 
			<< " set name='" << strChartName_utf8 << "',charttype=" << sChartInfo.nType
			<< ",x_name='" << strChartXName_utf8 << "',x_unit='" << strChartXUnit_utf8
			<< "',y_name='" << strChartYName_utf8 << "',y_unit='" << strChartYUnit_utf8
			<< "',y_interval=" << sChartInfo.nYInterval << ",Description='" << strDescription_utf8
			<< "',pageid=" << sChartInfo.nPageID
			<< ",unitproperty01='" << strUnitproperty01_utf8 << "',unitproperty02='" << strUnitproperty02_utf8
			<< "',unitproperty03='" << strUnitproperty03_utf8 << "',unitproperty04='" << strUnitproperty04_utf8
			<< "',unitproperty05='" << strUnitproperty04_utf8 << "' where id=" << sChartInfo.nID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::ReadAllChartItemByChartID( const int nChartID, vector<IOSChartItemInfo>& vecItem )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecItem.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_chartitem
		<< " where chartid = "
		<< nChartID
		<< "  order by id";

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
	vecItem.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSChartItemInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nChartID	= ATOI(row[5]);
		wcItem.nColor	= ATOI(row[2]);
		wcItem.strName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strPointName = Project::Tools::UTF8ToWideChar(row[3]);
		wcItem.strDescription = Project::Tools::UTF8ToWideChar(row[4]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[6]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[7]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[9]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[10]);
		vecItem.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneChartItemByID( const int nID, IOSChartItemInfo& sChartItemInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_chartitem
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sChartItemInfo.nID = ATOI(row[0]);
		sChartItemInfo.nColor	= ATOI(row[2]);
		sChartItemInfo.nPointType	= ATOI(row[4]);
		sChartItemInfo.nChartID	= ATOI(row[6]);
		sChartItemInfo.strName = Project::Tools::UTF8ToWideChar(row[1]);
		sChartItemInfo.strPointName = Project::Tools::UTF8ToWideChar(row[3]);
		sChartItemInfo.strDescription = Project::Tools::UTF8ToWideChar(row[5]);
		sChartItemInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[7]);
		sChartItemInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[8]);
		sChartItemInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[9]);
		sChartItemInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[10]);
		sChartItemInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[11]);
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOneChartItem( const IOSChartItemInfo sChartItemInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strChartName_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strName, strChartName_utf8);
	string strPointName_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strPointName, strPointName_utf8);
	string strDescription_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strDescription, strDescription_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	if(sChartItemInfo.nID > 0)
	{
		sqlstream << "insert into "
			<< GetDBName()
			<<"."
			<< tablename_chartitem 
			<< " value ("
			<< sChartItemInfo.nID << ",'"
			<< strChartName_utf8 << "'," << sChartItemInfo.nColor << ",'" << strPointName_utf8 
			<< "',"<< sChartItemInfo.nPointType << ",'"
			<< strDescription_utf8 << "'," << sChartItemInfo.nChartID << ",'"
			<< strUnitproperty01_utf8  << "','"
			<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
			<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	}
	else
	{
		sqlstream << "insert into "
			<< GetDBName()
			<<"."
			<< tablename_chartitem 
			<< "(name, color, bindpointname,pointtype, description, chartid, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
			<< strChartName_utf8 << "'," << sChartItemInfo.nColor << ",'" << strPointName_utf8 
			<< "'," << sChartItemInfo.nPointType << ",'"
			<< strDescription_utf8 << "'," << sChartItemInfo.nChartID << ",'"
			<< strUnitproperty01_utf8  << "','"
			<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
			<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	}

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneChartItemByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_chartitem
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneChartItemByID( const int nID, const IOSChartItemInfo sChartItemInfo )
{
	DeleteOneChartItemByID(nID);
	return InsertOneChartItem(sChartItemInfo);
}

bool CIOSConfigDBAccess::ReadAllUser( vector<IOSUserInfo>& vecUser )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecUser.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_user
		<< "  order by id";

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
	vecUser.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSUserInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.strName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strPsw = Project::Tools::UTF8ToWideChar(row[2]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[6]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[7]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[9]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[10]);
		vecUser.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneUserByID( const int nID, IOSUserInfo& sUserInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_user
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sUserInfo.nID = ATOI(row[0]);
		sUserInfo.strName = Project::Tools::UTF8ToWideChar(row[1]);
		sUserInfo.strPsw = Project::Tools::UTF8ToWideChar(row[2]);		
		sUserInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[6]);
		sUserInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[7]);
		sUserInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[8]);
		sUserInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[9]);
		sUserInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[10]);
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOneUser( const IOSUserInfo sUserInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strUserName_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strName, strUserName_utf8);
	string strUserPsw_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strPsw, strUserPsw_utf8);	
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty05, strUnitproperty05_utf8);

	SYSTEMTIME st;
	GetLocalTime(&st);
	string strSysTime = Project::Tools::SystemTimeToString(st);

	string strCreateDate_utf8 = strSysTime;
	string strLastActiveDate_utf8 = strSysTime;
	string strLastLoginDate_utf8 = strSysTime;

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	
	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_user 
		<< "(name, psw, CreationDate, LastActivityDate, LastLoginDate, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
		<< strUserName_utf8 << "','"<< strUserPsw_utf8 << "','"
		<< strCreateDate_utf8 << "','"<< strLastActiveDate_utf8 << "','" << strLastLoginDate_utf8 << "','" 
		<< strUnitproperty01_utf8  << "','"
		<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
		<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	
	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneUserByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_user
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneUserByID( const int nID, const IOSUserInfo sUserInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strUserName_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strName, strUserName_utf8);
	string strUserPsw_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strPsw, strUserPsw_utf8);	
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sUserInfo.strUnitproperty05, strUnitproperty05_utf8);

	SYSTEMTIME st;
	GetLocalTime(&st);
	string strSysTime = Project::Tools::SystemTimeToString(st);
	string strCreateDate_utf8 = strSysTime;
	string strLastActiveDate_utf8 = strSysTime;
	string strLastLoginDate_utf8 = strSysTime;

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	if(sUserInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_user 
			<< " set name = '" << strUserName_utf8 << "',psw='" << strUserPsw_utf8
			<< "', CreationDate='" << strCreateDate_utf8 << "',LastActivityDate='" << strLastActiveDate_utf8
			<< "',LastLoginDate='" << strLastLoginDate_utf8 
			<< "',unitproperty01='" << strUnitproperty01_utf8 << "',unitproperty02='" << strUnitproperty02_utf8
			<< "',unitproperty03='" << strUnitproperty03_utf8 << "',unitproperty04='" << strUnitproperty04_utf8
			<< "',unitproperty05='" << strUnitproperty04_utf8 << "' where id=" << sUserInfo.nID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::ReadRelationByUserName( const wstring strUserName, vector<wstring>& vecProject)
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	
	string strUserName_utf8;
	Project::Tools::WideCharToUtf8(strUserName, strUserName_utf8);
	vecProject.clear();

	std::ostringstream sqlstream;
	sqlstream << "select p.name from "  
		<< GetDBName() << "." << tablename_project << " p,"
		<< GetDBName() << "." << tablename_user << " u,"
		<< GetDBName() << "." << tablename_user_to_prj << " r"
		<< " where r.userid = u.id and r.projectid = p.id and u.name = '"
		<< strUserName_utf8
		<< "' order by p.name;";

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
	vecProject.reserve(length);
	MYSQL_ROW  row = NULL;

	while(row = FetchRow(result) )
	{
		wstring strPrjName = Project::Tools::UTF8ToWideChar(row[0]);
		vecProject.push_back(strPrjName);
	}
	return true;
}

bool CIOSConfigDBAccess::DeleteRelationByProjectName( const wstring strUserName, const wstring strProjectName )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strUserName_utf8;
	Project::Tools::WideCharToUtf8(strUserName, strUserName_utf8);
	string strProjectName_utf8;
	Project::Tools::WideCharToUtf8(strProjectName, strProjectName_utf8);

	std::ostringstream sqlstream;
	sqlstream << "delete from r using "
		<< GetDBName() << "." << tablename_project << " p,"
		<< GetDBName() << "." << tablename_user << " u,"
		<< GetDBName() << "." << tablename_user_to_prj << " r"
		<< " where r.userid = u.id and r.projectid = p.id and u.name = '"
		<< strUserName_utf8 << "' and p.name = '"
		<< strProjectName_utf8 << "';";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::InsertRelation( const int nUserID, const int nProjectID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

		sqlstream << "insert into "
			<< GetDBName()
			<<"."
			<< tablename_user_to_prj 
			<< "(userid, projectid) values ("
			<< nUserID << "," << nProjectID << ");";

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::InsertProjectConfig( const IOSConfigInfo sProjectConfigInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_setting 
		<< "(userid, ispush, ispushall, ispushmime, refreshwarning, refreshtime, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ("
		<< sProjectConfigInfo.nUserID << "," << sProjectConfigInfo.nPush << "," 
		<< sProjectConfigInfo.nPushAll << "," << sProjectConfigInfo.nPushMine << "," 
		<< sProjectConfigInfo.nWarningRefresh << "," << sProjectConfigInfo.nPageRefresh << ",'" 
		<< strUnitproperty01_utf8  << "','"
		<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
		<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";	

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteProjcetConfigByUserID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_setting
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateProjectConfigByUserID( const int nID, const IOSConfigInfo sProjectConfigInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sProjectConfigInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	if(sProjectConfigInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_setting 
			<< " set ispush =" << sProjectConfigInfo.nPush
			<< ", ispushall=" << sProjectConfigInfo.nPushAll << ",ispushmime =" << sProjectConfigInfo.nPushMine
			<< ", refreshwarning=" << sProjectConfigInfo.nWarningRefresh << ",refreshtime =" << sProjectConfigInfo.nPageRefresh
			<< ",unitproperty01='" << strUnitproperty01_utf8 << "',unitproperty02='" << strUnitproperty02_utf8
			<< "',unitproperty03='" << strUnitproperty03_utf8 << "',unitproperty04='" << strUnitproperty04_utf8
			<< "',unitproperty05='" << strUnitproperty04_utf8 << "' where userid=" << sProjectConfigInfo.nUserID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::ReadProjectConfigByUserID( const int nID, IOSConfigInfo& sProjectConfigInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_setting
		<< "  where userid = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sProjectConfigInfo.nID = ATOI(row[0]);
		sProjectConfigInfo.nUserID	= ATOI(row[1]);
		sProjectConfigInfo.nPush	= ATOI(row[2]);
		sProjectConfigInfo.nPushAll	= ATOI(row[3]);
		sProjectConfigInfo.nPushMine	= ATOI(row[4]);
		sProjectConfigInfo.nWarningRefresh	= ATOI(row[5]);
		sProjectConfigInfo.nPageRefresh	= ATOI(row[6]);
		sProjectConfigInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[7]);
		sProjectConfigInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[8]);
		sProjectConfigInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[9]);
		sProjectConfigInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[10]);
		sProjectConfigInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[11]);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadAllWarningByPageID( const int nPageID, vector<IOSWarningInfo>& vecWarning )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecWarning.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_warning
		<< " where pageid="
		<< nPageID
		<< "  order by id";

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
	vecWarning.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSWarningInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nPageID	= ATOI(row[2]);
		wcItem.nImageID = ATOI(row[3]);
		wcItem.strName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strUserName = Project::Tools::UTF8ToWideChar(row[4]);
		wcItem.strPointName = Project::Tools::UTF8ToWideChar(row[5]);
		wcItem.strFilter = Project::Tools::UTF8ToWideChar(row[6]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[7]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[9]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[10]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[11]);
		vecWarning.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneWarningByID( const int nID, IOSWarningInfo& sWarningInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_warning
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sWarningInfo.nID = ATOI(row[0]);
		sWarningInfo.nPageID	= ATOI(row[2]);
		sWarningInfo.nImageID = ATOI(row[3]);
		sWarningInfo.strName = Project::Tools::UTF8ToWideChar(row[1]);
		sWarningInfo.strUserName = Project::Tools::UTF8ToWideChar(row[4]);
		sWarningInfo.strPointName = Project::Tools::UTF8ToWideChar(row[5]);
		sWarningInfo.strFilter = Project::Tools::UTF8ToWideChar(row[6]);
		sWarningInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[7]);
		sWarningInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[8]);
		sWarningInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[9]);
		sWarningInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[10]);
		sWarningInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[11]);
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOneWarning( const IOSWarningInfo sWarningInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strName_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strName, strName_utf8);
	string strUserName_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUserName, strUserName_utf8);
	string strPointName_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strPointName, strPointName_utf8);
	string strFilter_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strFilter, strFilter_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_warning 
		<< "(name, pageid, imageid, username, bindpointname, filter, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
		<< strName_utf8 << "',"
		<< sWarningInfo.nPageID << "," << sWarningInfo.nImageID << ",'"
		<< strUserName_utf8 << "','" << strPointName_utf8 << "','"
		<< strFilter_utf8 << "','"
		<< strUnitproperty01_utf8  << "','"
		<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
		<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	
	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneWarningByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_warning
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneWarningByID( const int nID, const IOSWarningInfo sWarningInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strName_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strName, strName_utf8);
	string strUserName_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUserName, strUserName_utf8);
	string strPointName_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strPointName, strPointName_utf8);
	string strFilter_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strFilter, strFilter_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sWarningInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	if(sWarningInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_warning 
			<< " set name = '" << strName_utf8 << "',pageid=" << sWarningInfo.nPageID
			<< ",imageid=" << sWarningInfo.nImageID << ",username='" << strUserName_utf8
			<< "',bindpointname='" << strPointName_utf8 << "',filter='" << strFilter_utf8
			<< "',unitproperty01='" << strUnitproperty01_utf8 << "',unitproperty02='" << strUnitproperty02_utf8
			<< "',unitproperty03='" << strUnitproperty03_utf8 << "',unitproperty04='" << strUnitproperty04_utf8
			<< "',unitproperty05='" << strUnitproperty04_utf8 << "' where id=" << sWarningInfo.nID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::IsNameExist( const wstring strUserName )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	string strUser_utf8;
	Project::Tools::WideCharToUtf8(strUserName, strUser_utf8);

	std::ostringstream sqlstream;
	sqlstream << "select * from " 
		<< GetDBName() 
		<< "."
		<< tablename_user
		<< " where name = '"
		<< strUser_utf8 
		<< "';";
		
	MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
	if (result == NULL)
	{
		return false;
	}

	Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

	int length = static_cast<int>(GetRowNum(result));
	if (0 == length){
		return false;
	}

	return true;
}

bool CIOSConfigDBAccess::ReadAllDeviceToken( vector<IOSDeviceTokenInfo>& vecDeviceToken )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecDeviceToken.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_token
		<< "  order by id";

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
	vecDeviceToken.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSDeviceTokenInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.strProject = Project::Tools::UTF8ToWideChar(row[2]);
		wcItem.strUserName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strToken = Project::Tools::UTF8ToWideChar(row[3]);		
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[4]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[5]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[6]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[7]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[8]);
		vecDeviceToken.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadAllCompares( vector<IOSCompareInfo>& vecCompare )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecCompare.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_compare
		<< "  order by id";

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
	vecCompare.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSCompareInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nChartType = ATOI(row[7]);
		wcItem.strPointName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strName = Project::Tools::UTF8ToWideChar(row[2]);
		wcItem.strProjects = Project::Tools::UTF8ToWideChar(row[3]);
		wcItem.strXUnit = Project::Tools::UTF8ToWideChar(row[5]);
		wcItem.strYUnit = Project::Tools::UTF8ToWideChar(row[6]);	
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[9]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[10]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[11]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[12]);
		vecCompare.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneCompareByID( const int nID, IOSCompareInfo& sCompareInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_compare
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sCompareInfo.nID = ATOI(row[0]);
		sCompareInfo.nChartType = ATOI(row[7]);
		sCompareInfo.strPointName = Project::Tools::UTF8ToWideChar(row[1]);
		sCompareInfo.strName = Project::Tools::UTF8ToWideChar(row[2]);
		sCompareInfo.strProjects = Project::Tools::UTF8ToWideChar(row[3]);
		sCompareInfo.strXUnit = Project::Tools::UTF8ToWideChar(row[5]);
		sCompareInfo.strYUnit = Project::Tools::UTF8ToWideChar(row[6]);	
		sCompareInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[8]);
		sCompareInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[9]);
		sCompareInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[10]);
		sCompareInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[11]);
		sCompareInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[12]);
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOneCompare( const IOSCompareInfo sCompareInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strPointName_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strPointName, strPointName_utf8);
	string strName_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strName, strName_utf8);
	string strProjects_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strProjects, strProjects_utf8);
	string strXUnit_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strXUnit, strXUnit_utf8);
	string strYUnit_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strYUnit, strYUnit_utf8);	
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_compare 
		<< "(pointname, name, projectname, pointtype, xunit, yunit, charttype, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
		<< strPointName_utf8 << "','"<< strName_utf8 << "','"
		<< strProjects_utf8 << "',3,'" << strXUnit_utf8 << "','" << strYUnit_utf8 << "'," << sCompareInfo.nChartType 
		<< ",'" << strUnitproperty01_utf8  << "','"
		<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
		<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneCompareByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_compare
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneCompareByID( const int nID, const IOSCompareInfo sCompareInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strPointName_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strPointName, strPointName_utf8);
	string strName_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strName, strName_utf8);
	string strProjects_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strProjects, strProjects_utf8);
	string strXUnit_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strXUnit, strXUnit_utf8);
	string strYUnit_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strYUnit, strYUnit_utf8);	
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sCompareInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string	
	std::ostringstream sqlstream;
	if(sCompareInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_compare 
			<< " set pointname = '" << strPointName_utf8 << "',name='" << strName_utf8
			<< "', projectname='" << strProjects_utf8 << "',xunit='" << strXUnit_utf8
			<< "',yunit='" << strYUnit_utf8 << "',charttype=" << sCompareInfo.nChartType		
			<< ",unitproperty01='" << strUnitproperty01_utf8 << "',unitproperty02='" << strUnitproperty02_utf8
			<< "',unitproperty03='" << strUnitproperty03_utf8 << "',unitproperty04='" << strUnitproperty04_utf8
			<< "',unitproperty05='" << strUnitproperty04_utf8 << "' where id=" << sCompareInfo.nID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::ReadAllUserGroup( vector<IOSUserGroupInfo>& vecUserGroup )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecUserGroup.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_usergroup
		<< "  order by id";

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
	vecUserGroup.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSUserGroupInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);		
		wcItem.strName = Project::Tools::UTF8ToWideChar(row[1]);		
		vecUserGroup.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneUserGroupByID( const int nID, IOSUserGroupInfo& sUserGroupInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_usergroup
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sUserGroupInfo.nID = ATOI(row[0]);	
		sUserGroupInfo.strName = Project::Tools::UTF8ToWideChar(row[1]);		
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOneUserGroup( const IOSUserGroupInfo sUserGroupInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strName_utf8;
	Project::Tools::WideCharToUtf8(sUserGroupInfo.strName, strName_utf8);
	
	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_usergroup 
		<< "(name) values ('"
		<< strName_utf8  << "');";

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneUserGroupByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_usergroup
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneUserGroupByID( const int nID, const IOSUserGroupInfo sUserGroupInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strName_utf8;
	Project::Tools::WideCharToUtf8(sUserGroupInfo.strName, strName_utf8);
	
	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;
	if(sUserGroupInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_usergroup 
			<< " set name = '" << strName_utf8 << "' where id=" << sUserGroupInfo.nID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::ReadAllCompareChartByGroupID( const int nGroupID, vector<IOSChartInfo>& vecChart )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecChart.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_comparechart
		<< " where usergroupid = "
		<< nGroupID
		<< "  order by id";

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
	vecChart.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSChartInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nType	= ATOI(row[2]);
		wcItem.nYInterval = ATOI(row[7]);
		wcItem.nGroupID = ATOI(row[9]);
		wcItem.strName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strXName = Project::Tools::UTF8ToWideChar(row[3]);
		wcItem.strXUnit = Project::Tools::UTF8ToWideChar(row[4]);
		wcItem.strYName = Project::Tools::UTF8ToWideChar(row[5]);
		wcItem.strYUnit = Project::Tools::UTF8ToWideChar(row[6]); 
		wcItem.strDescription = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[10]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[11]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[12]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[13]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[14]);
		vecChart.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneCompareChartByChartID( const int nID, IOSChartInfo& sChartInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_comparechart
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sChartInfo.nID = ATOI(row[0]);
		sChartInfo.nType	= ATOI(row[2]);
		sChartInfo.nYInterval = ATOI(row[7]);
		sChartInfo.nGroupID = ATOI(row[9]);
		sChartInfo.strName = Project::Tools::UTF8ToWideChar(row[1]);
		sChartInfo.strXName = Project::Tools::UTF8ToWideChar(row[3]);
		sChartInfo.strXUnit = Project::Tools::UTF8ToWideChar(row[4]);
		sChartInfo.strYName = Project::Tools::UTF8ToWideChar(row[5]);
		sChartInfo.strYUnit = Project::Tools::UTF8ToWideChar(row[6]); 
		sChartInfo.strDescription = Project::Tools::UTF8ToWideChar(row[8]);
		sChartInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[10]);
		sChartInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[11]);
		sChartInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[12]);
		sChartInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[13]);
		sChartInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[14]);
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOneCompareChart( const IOSChartInfo sChartInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strChartName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strName, strChartName_utf8);
	string strChartXName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strXName, strChartXName_utf8);
	string strChartXUnit_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strXUnit, strChartXUnit_utf8);
	string strChartYName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strYName, strChartYName_utf8);
	string strChartYUnit_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strYUnit, strChartYUnit_utf8);
	string strDescription_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strDescription, strDescription_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_comparechart 
		<< "(name, charttype, x_name, x_unit, y_name, y_unit, y_interval, Description, usergroupid, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
		<< strChartName_utf8 << "'," << sChartInfo.nType << ",'" << strChartXName_utf8 << "','"
		<< strChartXUnit_utf8 << "','" << strChartYName_utf8 << "','" << strChartYUnit_utf8 << "',"
		<< sChartInfo.nYInterval << ",'" 
		<< strDescription_utf8 << "'," << sChartInfo.nGroupID << ",'" 
		<< strUnitproperty01_utf8  << "','"
		<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
		<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneCompareChartByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_comparechart
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneCompareChartByID( const int nID, const IOSChartInfo sChartInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strChartName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strName, strChartName_utf8);
	string strChartXName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strXName, strChartXName_utf8);
	string strChartXUnit_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strXUnit, strChartXUnit_utf8);
	string strChartYName_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strYName, strChartYName_utf8);
	string strChartYUnit_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strYUnit, strChartYUnit_utf8);
	string strDescription_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strDescription, strDescription_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sChartInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	if(sChartInfo.nID > 0)
	{
		sqlstream << "update "
			<< GetDBName()
			<<"."
			<< tablename_comparechart
			<< " set name='" << strChartName_utf8 << "',charttype=" << sChartInfo.nType
			<< ",x_name='" << strChartXName_utf8 << "',x_unit='" << strChartXUnit_utf8
			<< "',y_name='" << strChartYName_utf8 << "',y_unit='" << strChartYUnit_utf8
			<< "',y_interval=" << sChartInfo.nYInterval << ",Description='" << strDescription_utf8
			<< "',usergroupid=" << sChartInfo.nGroupID
			<< ",unitproperty01='" << strUnitproperty01_utf8 << "',unitproperty02='" << strUnitproperty02_utf8
			<< "',unitproperty03='" << strUnitproperty03_utf8 << "',unitproperty04='" << strUnitproperty04_utf8
			<< "',unitproperty05='" << strUnitproperty04_utf8 << "' where id=" << sChartInfo.nID << ";";
	}

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::ReadAllCompareChartItemByChartID( const int nChartID, vector<IOSChartItemInfo>& vecItem )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	vecItem.clear();

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_comparechartitem
		<< " where chartid = "
		<< nChartID
		<< "  order by id";

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
	vecItem.reserve(length);
	MYSQL_ROW  row = NULL;

	IOSChartItemInfo wcItem;
	while(row = FetchRow(result) )
	{
		wcItem.nID = ATOI(row[0]);
		wcItem.nChartID	= ATOI(row[6]);
		wcItem.nColor	= ATOI(row[2]);
		wcItem.nProjectID	= ATOI(row[3]);
		wcItem.strName = Project::Tools::UTF8ToWideChar(row[1]);
		wcItem.strPointName = Project::Tools::UTF8ToWideChar(row[4]);
		wcItem.strDescription = Project::Tools::UTF8ToWideChar(row[5]);
		wcItem.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[7]);
		wcItem.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[8]);
		wcItem.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[9]);
		wcItem.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[10]);
		wcItem.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[11]);
		vecItem.push_back(wcItem);
	}
	return true;
}

bool CIOSConfigDBAccess::ReadOneCompareChartItemByID( const int nID, IOSChartItemInfo& sChartItemInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	std::ostringstream sqlstream;
	sqlstream << "select * from "  
		<< GetDBName() 
		<< "."
		<< tablename_comparechartitem
		<< "  where id = "
		<< nID
		<< ";";

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
	MYSQL_ROW  row = NULL;
	while(row = FetchRow(result) )
	{
		sChartItemInfo.nID = ATOI(row[0]);
		sChartItemInfo.nChartID	= ATOI(row[6]);
		sChartItemInfo.nColor	= ATOI(row[2]);
		sChartItemInfo.nProjectID	= ATOI(row[3]);
		sChartItemInfo.strName = Project::Tools::UTF8ToWideChar(row[1]);
		sChartItemInfo.strPointName = Project::Tools::UTF8ToWideChar(row[4]);
		sChartItemInfo.strDescription = Project::Tools::UTF8ToWideChar(row[5]);
		sChartItemInfo.strUnitproperty01 = Project::Tools::UTF8ToWideChar(row[7]);
		sChartItemInfo.strUnitproperty02 = Project::Tools::UTF8ToWideChar(row[8]);
		sChartItemInfo.strUnitproperty03 = Project::Tools::UTF8ToWideChar(row[9]);
		sChartItemInfo.strUnitproperty04 = Project::Tools::UTF8ToWideChar(row[10]);
		sChartItemInfo.strUnitproperty05 = Project::Tools::UTF8ToWideChar(row[11]);
	}
	return true;
}

bool CIOSConfigDBAccess::InsertOneCompareChartItem( const IOSChartItemInfo sChartItemInfo )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	string strChartName_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strName, strChartName_utf8);
	string strPointName_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strPointName, strPointName_utf8);
	string strDescription_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strDescription, strDescription_utf8);
	string strUnitproperty01_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty01, strUnitproperty01_utf8);
	string strUnitproperty02_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty02, strUnitproperty02_utf8);
	string strUnitproperty03_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty03, strUnitproperty03_utf8);
	string strUnitproperty04_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty04, strUnitproperty04_utf8);
	string strUnitproperty05_utf8;
	Project::Tools::WideCharToUtf8(sChartItemInfo.strUnitproperty05, strUnitproperty05_utf8);

	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	if(sChartItemInfo.nID > 0)
	{
		sqlstream << "insert into "
			<< GetDBName()
			<<"."
			<< tablename_comparechartitem
			<< " value ("
			<< sChartItemInfo.nID << ",'"
			<< strChartName_utf8 << "'," << sChartItemInfo.nColor << "," << sChartItemInfo.nProjectID << ",'" << strPointName_utf8 
			<< "','"
			<< strDescription_utf8 << "'," << sChartItemInfo.nChartID << ",'"
			<< strUnitproperty01_utf8  << "','"
			<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
			<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	}
	else
	{
		sqlstream << "insert into "
			<< GetDBName()
			<<"."
			<< tablename_comparechartitem 
			<< "(name, color, projectid, bindpointname, description, chartid, unitproperty01, unitproperty02, unitproperty03, unitproperty04, unitproperty05) values ('"
			<< strChartName_utf8 << "'," << sChartItemInfo.nColor 
			<< "," << sChartItemInfo.nProjectID << ",'" << strPointName_utf8 
			<< "','"
			<< strDescription_utf8 << "'," << sChartItemInfo.nChartID << ",'"
			<< strUnitproperty01_utf8  << "','"
			<< strUnitproperty02_utf8 << "','" << strUnitproperty03_utf8 << "','" 
			<< strUnitproperty04_utf8 << "','" << strUnitproperty05_utf8 << "');";
	}

	string sqlstatement = sqlstream.str();

	return Execute(sqlstatement);
}

bool CIOSConfigDBAccess::DeleteOneCompareChartItemByID( const int nID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from " 
		<< GetDBName() 
		<< "."
		<< tablename_comparechartitem
		<< " where id = " << nID << ";";

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::UpdateOneCompareChartItemByID( const int nID, const IOSChartItemInfo sChartItemInfo )
{
	DeleteOneCompareChartItemByID(nID);
	return InsertOneCompareChartItem(sChartItemInfo);
}

bool CIOSConfigDBAccess::ReadAllUsersByUserGroupID( const int nGroupID, vector<IOSUserInfo>& vecUser )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);

	vecUser.clear();

	std::ostringstream sqlstream;
	sqlstream << "select u.id,u.name from "  
		<< GetDBName() << "." << tablename_usergroup << " g,"
		<< GetDBName() << "." << tablename_user << " u,"
		<< GetDBName() << "." << tablename_usertogroup << " r"
		<< " where r.userid = u.id and r.groupid = g.id and g.id ="
		<< nGroupID;

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
	vecUser.reserve(length);
	MYSQL_ROW  row = NULL;

	while(row = FetchRow(result) )
	{
		IOSUserInfo sUser;
		sUser.strName = Project::Tools::UTF8ToWideChar(row[1]);
		sUser.nID = ATOI(row[0]);
		vecUser.push_back(sUser);
	}
	return true;
}

bool CIOSConfigDBAccess::DeleteGroupRelationByUserID( const int nGroupID, const int nUserID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);

	std::ostringstream sqlstream;
	sqlstream << "delete from "  << GetDBName()
		<< "." << tablename_usertogroup
		<< " where userid=" << nUserID << " and groupid=" << nGroupID;

	return Execute(sqlstream.str());
}

bool CIOSConfigDBAccess::InsertGroupRelation( const int nGroupID, const int nUserID )
{
	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	SetConnectionCharacterSet(MYSQL_UTF8);
	//generate the insert string
	std::ostringstream sqlstream;

	sqlstream << "insert into "
		<< GetDBName()
		<<"."
		<< tablename_usertogroup 
		<< "(userid, groupid) values ("
		<< nUserID << "," << nGroupID << ");";

	string sqlstatement = sqlstream.str();
	return Execute(sqlstatement);
}
