#pragma once

#include "./bottcode/OPCdefine.h"

#include <hash_map>
using namespace stdext;

#define WRITE_SYNC		0
#define WRITE_ASYNC10	1
#define WRITE_ASYNC20	2

#include "Tools/CustomTools/CustomTools.h"
#include "DataPoint/DataPointEntry.h"

struct hash_A{
	wstring operator()(const wstring & A)const{
		return A;
	}
};

//2 define the equal function
struct greater_A{
	bool operator()(const wstring & a1, const wstring & a2)const{
		return  a1  > a2;
	}
};

// using namespace Project::Logs;
typedef LRESULT (*LPAddPointProc)(DWORD dwCurrent, DWORD dwTotal,DWORD userdata,char* pItemName); //接收数据的回调函数  add by robert 12.11
class COPCConnection
{
public:
	COPCConnection();
	~COPCConnection(void);

private:
	void GetValue(VARIANT &vtVal		// [in]
		        , TCHAR *pBuffer				// [out]
		        , int nBufLen);					// [in]
        
	void AddLeaves(LPENUMSTRING pIEnumString,bool bProperty = false,int nMutilLoad=100,int nSleep=10,int nMutilAdd =1000);

public:
    //设置连接参数
	void	SetServerProgName(const wstring& strProgID);
	wstring	GetServerProgName() const;

	bool SearchOPC(wstring strOPCServerIP,hash_map<wstring,CLSID>& clsidmap);
	bool SearchOPC_(wstring strOPCServerIP,hash_map<wstring,CLSID>& clsidmap,int nType);
	bool GetAllOPCMap(map<wstring, CKItem*>& searchmap);
	bool ReadItemProperty(const wstring& strItemID);
	bool	GetPrograss(DWORD& nCurrentCount,DWORD& nTotalCount);
	//-----------------------------------------------------------------
    //连接到OPC服务器
	bool Connect(wstring strOPCServerIP);

	//连接到OPC服务器
	bool ConnectRemoteOPC(wstring strOPCServerIP,CLSID clsid);
    
    //从opc服务器断开
	void DisConnect(void);

    //添加OPC group
	virtual void AddGroup(void);

    //添加所有opc变量
	virtual bool AddItems(bool bProperty = false,int nBrowseType=3,wstring strFilter=L"",int nMutilLoad=100 ,int nSleep=10,VARTYPE vtDataTypeFilter=VT_EMPTY,int nMutilAdd =1000,LPAddPointProc lpRecDataProc= NULL,DWORD userdata=NULL);
	
	//------------------------------------------------------------------------------
    virtual bool GetItemValue(const wstring& strItemID, wstring &strValue);
    
    bool	UpdateItemValue(const wstring& strItemID, wstring &strValue);
		
	virtual bool SetItemValue(const wstring& strItemID, const wstring& strValue);
	
	VARTYPE GetVarType(const wstring& strItemName);
	LPCTSTR	GetVarQuality(const wstring& strItemName);

	CKItem* FindItem(const wstring& strItemName);
    CString VariantToString(_variant_t var);

	bool	OutOPCEnumErrLog(wstring strOut);

	void	TryBrowseBranch(IOPCBrowseServerAddressSpace *pBSAS, LPCWSTR lpprestr,LPCWSTR lpcw);
public:
	
	OPCSERVER_TYPE	GetServerType()const;
	bool		IsConnect()const ;
	
protected:
	wstring		m_strserverprogid;
	wstring		m_strserverhost;

	wstring		m_strFilterLeaf;
	BOOL		m_bBrowseFlat;
	
	CObArray	m_itemlist;	//保存所有的Item
    
    map<wstring, CKItem*> m_searchmap;    //保存所有item的检索表
	hash_map<wstring, DataPointEntry> m_searchEntrymap;    //保存所有item entity的检索表
    hash_map<wstring,CLSID> m_clsidmap;				//clsid
	CKServer	*m_pServer;     // server对象
	CKGroup		*m_pGroup;      //group对象
	bool			m_isconnected;
	
	DWORD m_nCurrentCount;
	DWORD m_nTotalCount;
    Project::Tools::Mutex m_lock;
	LPAddPointProc	m_lpRecDataProc;	//收到数据回复回调
	DWORD m_dwUser;
	char m_cItemName[256];
};