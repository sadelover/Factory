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
	static CParseSplitString& Instance();//��ȡ�����ʵ��
	static void DestoryInstance();//���ٸ����ʵ��
	void ParseParam3(const int nID, const CString strParam3 );//�������Ϊ������ƺ�param3�ַ���
	const CString GetText(const int nID, const int nValue);//ͨ��������ƺ�param3�����úõ�ֵ���õ���ֵҪ��ʾ���ı�
	const int GetValue(const int nID, const CString strText);
	const bool IsExist(const int nID);
	void DeleteOne(const int nID);
private:
	static CParseSplitString* m_instane;
	vector<StructParam3> m_vecStructParam3;
};

