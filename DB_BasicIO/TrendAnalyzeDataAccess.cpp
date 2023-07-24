#include "StdAfx.h"
#include "TrendAnalyzeDataAccess.h"
#include "DBAccessContainer.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "../Tools/Util/UtilString.h"

//const wchar_t* defalutdbname = _T("uppcdatarecord");

const wchar_t* defalutdbname = _T("uppcdata");


CTrendAnalyzeDataAccess::CTrendAnalyzeDataAccess( void )
	: m_threadhandle_UpdatePhyPointValRecord(NULL)
{

}

CTrendAnalyzeDataAccess::~CTrendAnalyzeDataAccess( void )
{
	if (m_threadhandle_UpdatePhyPointValRecord != NULL){
		CloseHandle(m_threadhandle_UpdatePhyPointValRecord);
		m_threadhandle_UpdatePhyPointValRecord = NULL;
	}
}

void CTrendAnalyzeDataAccess::SetDBparam( gDataBaseParam mdbParam )
{
	m_dbParam = mdbParam;
}

bool CTrendAnalyzeDataAccess::InitConnection( void )
{
	if (!ConnectToDB())
	{
		::MessageBox(NULL, 
			TEXT("Unable to connect to the database server£¡"), 
			TEXT("Login database"), 
			MB_OK | MB_ICONERROR | MB_TOPMOST);
		return false;
	}

	return true;
}

bool CTrendAnalyzeDataAccess::StartUpdatePTValThread( void )
{
	CreateThreads();
	return true;
}

bool CTrendAnalyzeDataAccess::ConnectToDB()
{
	bool bresult1 =	m_pRealTimeDataAccess.ConnectToDB(UtilString::ConvertMultiByteToWideChar(m_dbParam.strDBIP), 
		UtilString::ConvertMultiByteToWideChar(m_dbParam.strUserName), 
		UtilString::ConvertMultiByteToWideChar(m_dbParam.strDBPassword), 
		defalutdbname,
		m_dbParam.nPort);

	bool bresult2 =	m_pHistoryDataDBAccess.ConnectToDB(UtilString::ConvertMultiByteToWideChar(m_dbParam.strDBIP), 
		UtilString::ConvertMultiByteToWideChar(m_dbParam.strUserName), 
		UtilString::ConvertMultiByteToWideChar(m_dbParam.strDBPassword), 
		defalutdbname,
		m_dbParam.nPort);

	return bresult1&& bresult2;
}

void CTrendAnalyzeDataAccess::TerminateAllThreads()
{
	::TerminateThread(m_threadhandle_UpdatePhyPointValRecord, 0);
	WaitForSingleObject(m_threadhandle_UpdatePhyPointValRecord,INFINITE);

}

bool CTrendAnalyzeDataAccess::GetPhyPointValueRecord()
{
	Project::Tools::Scoped_Lock<Mutex>	scope(m_lock);
	m_entrylist.clear();
	if(m_pRealTimeDataAccess.ReadRealtimeData_Input(m_entrylist))
	{
		if (m_entrylist.size() == 0)
		{
			return false;
		}

		wstring phyID = _T("");
		gPhyPointValueStr gPhyValueStruct;
		for (int i=0;i<m_entrylist.size();i++)
		{
			phyID = UtilString::ConvertMultiByteToWideChar(m_entrylist[i].GetName());

			gPhyValueStruct.time = m_entrylist[i].GetTimestamp();
			gPhyValueStruct.value = m_entrylist[i].GetValue();

			m_mapPhyPointValueRecord[phyID] = gPhyValueStruct;
		}
	}
	return true;
}

int CTrendAnalyzeDataAccess::GetSpanTimeMonths( COleDateTime& start, const COleDateTime& end )
{
	return (end.GetYear()-start.GetYear())*12 + (end.GetMonth()-start.GetMonth());
}

bool CTrendAnalyzeDataAccess::ReadHistoryData( vector<CRealTimeDataEntry>& resultlist, COleDateTime& start, const COleDateTime& end,const wstring strPointName )
{
	int nMonths = GetSpanTimeMonths(start,end);
	COleDateTime mDate;
	COleDateTime mStart = start;
	COleDateTime mEnd = end;
	if(mStart.GetMonth()==12)
	{
		mDate.SetDate(mStart.GetYear()+1,1,1);	
	}
	else
	{
		mDate.SetDate(mStart.GetYear(),mStart.GetMonth()+1,1);	
	}
	resultlist.clear();
	if(nMonths==0)
	{	
//		m_pHistoryDataDBAccess.ReadHistoryDataInTable( resultlist,mStart ,mEnd,strPointName);
	}
	else if(nMonths>0)
	{
		for(int i =0;i<nMonths;i++)
		{
//			m_pHistoryDataDBAccess.ReadHistoryData( resultlist,mStart ,mEnd,strPointName);
			mStart = mDate;
			int nYear = mDate.GetYear();
			int nMonth = mDate.GetMonth()+1;
			if(nMonth>12)
			{
				nYear = nYear+1;
				nMonth = nMonth -12;
			}

			mDate.SetDate(nYear,nMonth,1);
		}
//		m_pHistoryDataDBAccess.ReadHistoryData( resultlist,mStart ,mEnd,strPointName);
	}
	return true;
}

void CTrendAnalyzeDataAccess::CreateThreads()
{
	UINT dwThreadId = 0;

	m_threadhandle_UpdatePhyPointValRecord = (HANDLE)_beginthreadex(NULL,
		0,
		ThreadFunc_UpdatePhyPointValRecord, 
		(LPVOID)this,
		0,
		&dwThreadId);
}

UINT WINAPI CTrendAnalyzeDataAccess::ThreadFunc_UpdatePhyPointValRecord( LPVOID lparam )
{
	CTrendAnalyzeDataAccess* pThis = (CTrendAnalyzeDataAccess*)lparam;
	if (pThis != NULL)
	{
		while(TRUE)
		{
			pThis->UpdatePhyPointValRecord();

			Sleep(2*1000);
		}

	}

	return 0;
}

void CTrendAnalyzeDataAccess::UpdatePhyPointValRecord()
{
	GetPhyPointValueRecord();
}

hash_map<wstring, gPhyPointValueStr> CTrendAnalyzeDataAccess::GetValueRecordMap()
{
	return m_mapPhyPointValueRecord;
}

bool CTrendAnalyzeDataAccess::GetValue( const tstring& strName, double &dbParam )
{
	if (m_mapPhyPointValueRecord.size() == 0)
	{
		return false;
	}

	Project::Tools::Scoped_Lock<Mutex>	scope(m_lock);
	hash_map<wstring, gPhyPointValueStr>::const_iterator it = m_mapPhyPointValueRecord.find(strName);
	if (it != m_mapPhyPointValueRecord.end())
	{
		dbParam = _wtof(((*it).second).value.c_str());
	}

	return true;
}
