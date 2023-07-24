//bird 2011 12 26

#include "stdafx.h"
#include "DBInterfaceExp.h"
#include "DBAccessMeter.h"


//#include <iostream>
//#include <sstream>
//////////////////////////////////////////////////////////////////////////
//using std::ostringstream;

CDBAccessMeter* CDBAccessMeter::g_pThis = NULL;
Mutex CDBAccessMeter::m_mutex;

//////////////////////////////////////////////////////////////////////////
CDBAccessMeter::CDBAccessMeter()
    //:m_log(_T("DataBase.log"))
{
    //m_log.Set_LogConfig(TRUE, LOG_MINLEVEL_ALL);
}

CDBAccessMeter::~CDBAccessMeter()
{
}

CDBAccessMeter * CDBAccessMeter::GetInstance()
{
    //Scoped_Lock<Mutex> mut(m_mutex);
    if ( NULL == g_pThis )
    {
        Scoped_Lock<Mutex> mut(m_mutex); //bird 3.4
        if (!g_pThis){
            g_pThis = new CDBAccessMeter();
            atexit(DestroyInstance);
        }
    }
    return g_pThis;
}

void CDBAccessMeter::DestroyInstance()
{
    SAFE_DELETE(g_pThis);
}
//////////////////////////////////////////////////////////////////////////

bool CDBAccessMeter::ReadPowerByHourInDay(vector<RecordPointValue>& pointlist, UINT pointIdx)
{
    Project::Tools::Scoped_Lock<Mutex> scopelock(m_recordmutex);

    pointlist.clear();

    ////const UINT idxNum = (UINT)vecPointIndex.size();
    //char charIdx[11];
    //_itoa_s(pointIndex, charIdx, 10); //radix

    //wstring wstrTime; //2011-05-25 16:20:32
    //Project::Tools::OleTime_To_String(oleTimeStart, wstrTime);
    //const string strTimeStart = Project::Tools::WideCharToAnsi(wstrTime.c_str() );
    //Project::Tools::OleTime_To_String(oleTimeEnd, wstrTime);
    //const string strTimeEnd = Project::Tools::WideCharToAnsi(wstrTime.c_str() );

    //const string tablename_ansi = Project::Tools::WideCharToAnsi(tablename.c_str());

    //const string strDesc = ifOrder?"":"DESC ";

    //ostringstream sqlstream;
    //sqlstream << "select * from " << tablename_ansi << " "
    //    //<< "where PointIndex = 1 or pointindex = 6 or  pointindex = 13 or pointindex = 20 ) "
    //    << "where ( PointIndex = "<< charIdx << " ) "
    //    << "and recordtime >= '" << strTimeStart <<"' "
    //    << "and recordtime <= '" << strTimeEnd <<"' " 
    //    <<"order by recordtime " << strDesc
    //    <<"limit "<< 1<<" "
    //    ;

    //MYSQL_RES* result = QueryAndReturnResult(sqlstream.str().c_str());
    //if (result == NULL){
    //    return false;
    //}
    //Project::Tools::scope_ptr_holder<MYSQL_RES, Mysql_Result_Deleter>  resultholder(result);

    //int length = (int)GetRowNum(result);
    //if (0 == length){
    //    return true;
    //}
    //pointlist_out.reserve(length);

    //MYSQL_ROW  row = NULL;
    //RecordPointValue entry;

    //while(row = FetchRow(result) )
    //{              
    //    entry.mPointIndex = ATOI(row[RecordPointValue::POINT_LIST_INDEX] );

    //    Project::Tools::UTF8ToWideChar(row[RecordPointValue::POINT_LIST_TIME], entry.mStrTime);
    //    Project::Tools::UTF8ToWideChar(row[RecordPointValue::POINT_LIST_VALUE], entry.mStrValue);

    //    pointlist_out.push_back(entry);
    //}

    return true;
}

//////////////////////////////////////////////////////////////////////////
bool	CDBAccessMeter::ReadPowerPointList(vector<Record_Point_List_Entry>& pointlist)
{
    return true;
}
bool	CDBAccessMeter::ReadPowerConsumptionPointList(vector<Record_Point_List_Entry>& pointlist)
{
    return true;
}
//////////////////////////////////////////////////////////////////////////

