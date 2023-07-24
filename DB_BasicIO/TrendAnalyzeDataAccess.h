#pragma once

#include <string>
#include <vector>
#include <hash_map>

using namespace std;

#include "DatabaseSession.h"
#include "RealTimeDataAccess.h"
#include "CommonDBAccess.h"

using Beopdatalink::CRealTimeDataEntry;
using Beopdatalink::CRealTimeDataAccess;
using Beopdatalink::CCommonDBAccess;

typedef struct _declspec(dllexport) tag_PhyPointValueStruct 
{
	SYSTEMTIME   time;
	//wstring   physicalid;
	wstring  value;

}gPhyPointValueStr;


class _declspec(dllexport) CTrendAnalyzeDataAccess
{
public:
	CTrendAnalyzeDataAccess(void);
	~CTrendAnalyzeDataAccess(void);

	void SetDBparam(gDataBaseParam mdbParam);
	 bool InitConnection(void);
	 bool StartUpdatePTValThread(void);

	 bool ConnectToDB();
	 void TerminateAllThreads();

	 bool GetPhyPointValueRecord();

	 int	GetSpanTimeMonths(COleDateTime& start, const COleDateTime& end);
	 bool	ReadHistoryData(vector<CRealTimeDataEntry>& resultlist, COleDateTime& start, const COleDateTime& end,const wstring strPointName);

	 void	CreateThreads();
	 void	ResumeAllThreads();

	 //2s“ª¥Œ£¨∂¡»°
	 static UINT WINAPI ThreadFunc_UpdatePhyPointValRecord(LPVOID lparam);

	 void UpdatePhyPointValRecord();

	 hash_map<wstring, gPhyPointValueStr> GetValueRecordMap();

	 bool GetValue(const tstring& strName, double &dbParam);

protected:
	CRealTimeDataAccess m_pRealTimeDataAccess;
	CCommonDBAccess m_pHistoryDataDBAccess;

	vector<CRealTimeDataEntry> m_entrylist;
	hash_map<wstring, gPhyPointValueStr>      m_mapPhyPointValueRecord;

private:
	HANDLE  m_threadhandle_UpdatePhyPointValRecord;

	gDataBaseParam         m_dbParam;
	Project::Tools::Mutex	m_lock;			

};

