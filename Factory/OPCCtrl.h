//OPCCtrl.h file


/********************************************************************************
*COPCCtrl.h - declare the class COPCCtrl
*
*       Copyright (c) WeldTeh Corporation. All rights reserved.
*
*Purpose:
*       提供对PLC变量的直接访问.
*       包括写值到PLC。
		从PLC读值（用于实时性要求较高的操作）
		从OPC本地缓存读值。（读到的值有可能不是最新的。只是本地缓存的值）
*		
		为常见设备的开关，频率等属性值提供便捷的接口.
*Note:
*       
*s
*Author:
*      桂必成
*******************************************************************************/
#ifndef Beop_OPCCTRL_H
#define Beop_OPCCTRL_H
#pragma once

#include <sstream>

#include "OPC_Connection.h"
#include "Tools/CustomTools/CustomTools.h"



class COPCConnection_AutoBrowse : public COPCConnection
{
public:
	/*
	 本地启动OPC Server，并加载所有OPC变量到本地的group
	 strProgID :　OPC Server的程序集的名字
	 strremotename： OPC server运行的主机名(ip也可)
	 */
	virtual bool Init(wstring strOPCServerIP);
	bool	Exit();	
	void SetPointList( const vector<DataPointEntry>& pointlist );
	vector<DataPointEntry> GetPointList() const;

	bool SearchOPCServer(wstring strOPCServerIP,hash_map<wstring,CLSID>& clsidmap,int nType =0);
	bool AddItem(const wstring& strItemID);
	//bool	GetPrograss(DWORD& nCurrentCount,DWORD& nTotalCount);
public:
		
	//模板函数.通过映射名读
	template <typename T>
	bool	GetValue(const wstring& strName, T& result);

	//模板函数.通过映射名读
	template <typename T>
	bool	GetValueByLName(const wstring& strName, T& result);
	
	//通过plc短名，从PLC读取值。
	//这个函数会通过点表,做一个从短名到全名的映射
	template <typename T>
	bool    ReadValue(const wstring& strName, T& result);
	
		
	//通过映射名，向PLC写入变量的值.
	template<typename T>
	bool SetValue(const wstring& strName, const T& uiParam);

	//通过映射名，向PLC写入变量的值.
	template<typename T>
	bool SetValueByLName(const wstring& strName, const T& uiParam);
	
protected:
	//模板函数.通过地址读
	template <typename T>
	bool	GetValueByLongName(const wstring& strName, T& result);

	//通过PLC全名，从PLC读值
	// 举例： 一般以NewResource.Main.开头
	// 或者是NewResource.开头
	template<typename T>
	bool	ReadValueByLongName(const wstring& strLongName, T& result);

	//通过变量的实际地址，向PLC写入变量的值.
	template<typename T>
	bool	SetValueByLongName(const wstring& strname, const T& value);

	wstring	GetPlcNameByShortName(const wstring& shortname);

	void	AddErrorLog(const wstring& shortname);

public:
	
	VARTYPE	GetItemTypeByShortName(const wstring& strShortName);
		
	LPCTSTR GetItemQualityByShortName(const wstring& strShortName);
		
protected:
	vector<DataPointEntry> m_pointlist;
	hash_map<wstring, wstring>	m_namemap;			//映射名和plc地址的关联
};

template<typename T>
bool COPCConnection_AutoBrowse::SetValue( const wstring& strName, const T& uiParam )
{
	wstring strLongName = GetPlcNameByShortName(strName);
	if (strLongName.empty()){
		AddErrorLog(strName);
		return false;
	}
	return SetValueByLongName(strLongName, uiParam);
}

template<typename T>
bool COPCConnection_AutoBrowse::SetValueByLName( const wstring& strName, const T& uiParam )
{
	return SetValueByLongName(strName, uiParam);
}

template <typename T>
bool COPCConnection_AutoBrowse::GetValue( const wstring& strName, T& result )
{
	wstring strLongName = GetPlcNameByShortName(strName);
	if (strLongName.empty()){
		_tprintf(L"Error::PlcName empty.\r\n");
		return false;
	}
	return GetValueByLongName(strLongName, result);
}


template <typename T>
bool COPCConnection_AutoBrowse::GetValueByLName( const wstring& strName, T& result )
{
	return GetValueByLongName(strName, result);
}

template <typename T>
bool COPCConnection_AutoBrowse::ReadValue( const wstring& strName, T& result )
{
	wstring strLongName = GetPlcNameByShortName(strName);
	if (strLongName.empty()){
		AddErrorLog(strName);
		return false;
	}
	return ReadValueByLongName(strLongName, result);
}


//--------------------------------------------------------------------
template<typename T>
bool COPCConnection_AutoBrowse::SetValueByLongName( const wstring& strname, const T& value )
{
	std::wostringstream stream_T;
	stream_T << value ;

	return SetItemValue(strname, stream_T.str());
}

template <typename T>
bool COPCConnection_AutoBrowse::GetValueByLongName( const wstring& strName, T& result )
{
	wstring value;
	
	if (!GetItemValue(strName, value)){
		return false;
	}

	std::wistringstream t_stringstream(value);
	t_stringstream >> result;
	return true;
}


template<typename T>
bool COPCConnection_AutoBrowse::ReadValueByLongName( const wstring& strLongName, T& result )
{
	wstring str_value(_T(""));
	//更新值
	if (!UpdateItemValue(strLongName, str_value)){
		return false;
	}
	
	//转换为T类型.
	std::wistringstream t_stringstream(str_value);
	t_stringstream >> result;

	return true;
}

////////////////////////
class COPCConnection_ManualBrowse: public COPCConnection_AutoBrowse
{
public:
    
	virtual bool AddItems();
	
	virtual bool GetItemValue(const wstring& strItemID, wstring &strValue);

	virtual bool SetItemValue(const wstring& strItemID, wstring &strValue);
};


#endif  //_OPCCTRL_H
