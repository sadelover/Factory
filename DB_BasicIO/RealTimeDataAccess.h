
/*
 *
 *������ʵʱ��λ���ݵķ��ʣ���ȡ�ࡣ
 *
 *
 *
 *
 **/

#pragma once


#include "DatabaseSession.h"

#include "../Tools/CustomTools/CustomTools.h"
#include "../DB_BasicIO/StructDefine/Define_ControlParamRecord.h"
#include "../DB_BasicIO/StructDefine/ActionInfo_Define.h"
#include "../DB_BasicIO/WarningConfigItem.h"

#include "../Tools/USingletonTemplate.h"

#include "wtypes.h"
#include <vector>
#include <string>
#include <utility>


using std::vector;
using std::string;
using std::wstring;
using std::pair;


class CEqmDrawPage;
class CEqmDrawText;
typedef struct tag_DataBaseParam
{// ���ݿ���Ϣ�����ṹ��
	tag_DataBaseParam()
		:nPort(0)
	{
		strDBPassword = "RNB.beop-2013";
	}
	string strDBIP;        //  host��ַ
	string strDBIP2;	   //  ����IP
	string strDBName;      //  ���ݿ�����
	string strRealTimeDBName;      //  ʵʱ���ݿ�����
	string strUserName;    //  �û���
	string strDBPassword;		//���ݿ�����
	int     nPort;          //  �˿�
}gDataBaseParam;

namespace Beopdatalink
{

	class _declspec(dllexport) CRealTimeDataEntry
	{
	public:
		
		SYSTEMTIME GetTimestamp() const;
		void	SetTimestamp(const SYSTEMTIME& stime);

		string	GetName() const;
		void	SetName(const string& name);
		
		wstring	GetValue() const;
		void	SetValue(wstring strValue);
	private:
		SYSTEMTIME	m_timestamp;					// current timestamp
		string		m_pointname;	// the point name. shortname
		wstring		m_pointvalue;					// the real value.
	};

	//
	

	class _declspec(dllexport) CRealTimeDataEntry_Output : public CRealTimeDataEntry
	{
	public:
		CRealTimeDataEntry_Output();
		void	SetInit(bool binit);

		bool	IsValueInit();
	private:
		bool	m_binit;
	};

	// class 
	class _declspec(dllexport) CRealTimeDataAccess : public CDatabaseSeesion
	{
	public:
		
		//==================���doubleֵ�Ĳ��룬��ȡ==============================
		// for double values
		bool	UpdatePointValue_Input(const CRealTimeDataEntry& entry);
		bool	DeleteRealTimeData_Input();
		// read double entries.
		bool	ReadRealtimeData_Input(vector<CRealTimeDataEntry>& entrylist);
		bool	ImportPointValue_Input(vector<CRealTimeDataEntry>& entrylist);
		bool    UpdatePointBuffer(vector<CRealTimeDataEntry>& entrylist);
		// for write doublevalues
		bool	ReadRealTimeData_Output(vector<CRealTimeDataEntry>& entrylist);
		bool	UpdatePointData(const CRealTimeDataEntry& entry);
		bool	UpdatePointDataList(vector<CRealTimeDataEntry>& entrylist);
		bool	DeleteRealTimeData_Output();
		bool	DeleteEntryInOutput(const string& pointname);

	private:
		Project::Tools::Mutex m_lock;
	};

	// Operation record
	class _declspec(dllexport) COperationRecords
	{
	public:
		COperationRecords(){};

		std::wstring    m_strTime;
		std::wstring    m_strOperation;
		std::wstring    m_strUser;
	};
	typedef std::vector<COperationRecords>   VEC_OPT_RECORD;

	// user online record
	class _declspec(dllexport) CUserOnlineEntry
	{
	public:
		CUserOnlineEntry();

		wstring	GetUserName() const;
		void	SetUserName(const wstring& username);


		wstring	GetUserHost() const;
		void	SetUserHost(const wstring& userhost);


		int		GetUserPriority() const;
		void	SetUserPriority(int userpriority);

		wstring	GetUserType() const;
		void	SetUserType(const wstring& usertype);

		SYSTEMTIME GetTimestamp() const;
		void	SetTimestamp(const SYSTEMTIME& stime);

	private:	
		wstring m_username;
		wstring m_userhost;
		int	m_priority;

		wstring m_usertype;		
		SYSTEMTIME m_time;
	};

	struct _declspec(dllexport) optrecordfordtu
	{
		string time;		
		string username;	//utf8 format
		string info;	//utf8 format
	};

	class _declspec(dllexport) CDBAccessToDTU : public CDatabaseSeesion
	{
	public:
		CDBAccessToDTU();
		 bool    GetOperationRecordAsString(vector<optrecordfordtu>&  resultlist,
											const COleDateTime& timeStart,
											const COleDateTime& timeStop);
		 bool    GetOptimizeRecordAsString(string& strrecord, 
											const COleDateTime& timeStart,
											const COleDateTime& timeStop);
	private:
		Project::Tools::Mutex	m_lock;
	};

	class _declspec(dllexport) DTUDBAccessSingleton : public CDBAccessToDTU
	{
	public:
		static DTUDBAccessSingleton* GetInstance();
		
	private:
		static DTUDBAccessSingleton* _pinstance;
		static void DestroyInstance();

		DTUDBAccessSingleton(const DTUDBAccessSingleton&);
		DTUDBAccessSingleton& operator=(const DTUDBAccessSingleton&);

	private:
		static Project::Tools::Mutex m_lock;

	protected:
		DTUDBAccessSingleton();
		~DTUDBAccessSingleton(){};
	};
}








