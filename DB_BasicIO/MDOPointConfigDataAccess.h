#pragma once
#include "DatabaseSession.h"
#include <iostream>
#include <sstream>
#include "comdef.h"
#include <hash_map>

#include "../Tools/CustomTools/CustomTools.h"

using namespace std;
using std::ostringstream;

//数据的结构
struct _declspec(dllexport) MDOPointInfo
{
	MDOPointInfo::MDOPointInfo()
	{
		nIndex = 0;
	}
	int		nIndex;
	wstring  strName;
	wstring	strUnit;
	wstring  strSource;
	wstring	strParam1;
	wstring	strParam2;
	wstring	strParam3;
	wstring	strParam4;
	wstring	strParam5;
	wstring	strParam6;
	wstring	strParam7;
	wstring	strParam8;
	wstring	strParam9;
	wstring	strParam10;	
	wstring strRemark;
};

class  __declspec(dllexport) CMDOPointConfigDataAccess : public CDatabaseSeesion
{
public:
	CMDOPointConfigDataAccess(void);
	~CMDOPointConfigDataAccess(void);

public:
	bool CreateConnectToDB( const wstring& host, const wstring& username, 
		const wstring& password, const wstring& database,
		unsigned int port);
	bool ReadMDOPointConfig(vector<MDOPointInfo>& vecPoint);

	wstring GetMDOPointSourceType(int nSource);

private:
	Mutex						m_mutex;
};
