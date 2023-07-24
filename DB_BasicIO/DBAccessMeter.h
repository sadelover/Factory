//bird 2011 12 26

#pragma once

#include "DBInterfaceImp.h"
#include "DatabaseSession.h"

#include "StructDefine/Define_Record_Point_List.h"

#include "../Tools/CustomTools/CustomTools.h"

//////////////////////////////////////////////////////////////////////////
//DBDLL
class DBDLL CDBAccessMeter : public CDatabaseSeesion
{
private:
    CDBAccessMeter();

public:
    ~CDBAccessMeter();
    static CDBAccessMeter *GetInstance();
    static void DestroyInstance();

public:
    //bool    ReadPowerTotal(vector<RecordPointValue>& pointlist, const wstring& tablename, UINT pointIdx,
        //const COleDateTime& oleTimeStart, COleDateTime& oleTimeEnd, bool ifOrder);
    bool    ReadPowerByHourInDay(vector<RecordPointValue>& pointlist, UINT pointIdx);

    bool	ReadPowerPointList(vector<Record_Point_List_Entry>& pointlist);
    bool	ReadPowerConsumptionPointList(vector<Record_Point_List_Entry>& pointlist);

private:
    static Mutex               m_mutex;

    Mutex       m_recordmutex;
    static CDBAccessMeter *      g_pThis;
    //CUseLog					   m_log;   // log object.
};
