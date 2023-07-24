#pragma once

#include "Tools/DllDefine.h"
#include <map>
#include <vector>
using namespace std;

struct StructParam3
{
	int nID;
	map<int,CString> mapping; 
};

class DLLEXPORT CParseSplitString
{
public:
	CParseSplitString(void);
	~CParseSplitString(void);
	static CParseSplitString& Instance();//获取该类的实例
	static void DestoryInstance();//销毁该类的实例
	void ParseParam3(const int nID, const CString strParam3 );//输入参数为点的名称和param3字符串
	const CString GetText(const int nID, const int nValue);//通过点的名称和param3中配置好的值，得到该值要显示的文本
	const int GetValue(const int nID, const CString strText);
	const bool IsExist(const int nID);
	void DeleteOne(const int nID);
private:
	static CParseSplitString* m_instane;
	vector<StructParam3> m_vecStructParam3;
};

