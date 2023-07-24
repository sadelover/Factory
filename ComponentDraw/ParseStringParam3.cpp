#include "StdAfx.h"
#include "ParseStringParam3.h"
#include "Tools/Util/UtilString.h"
using namespace UtilString;
#include <cassert>

CParseSplitString* CParseSplitString::m_instane = NULL;

CParseSplitString::CParseSplitString(void)
{

}


CParseSplitString::~CParseSplitString(void)
{
}

CParseSplitString& CParseSplitString::Instance()
{
	if (m_instane == NULL)
	{
		m_instane = new CParseSplitString();
		assert(m_instane);
	}
	return *m_instane;
}

void CParseSplitString::DestoryInstance()
{
	if (m_instane)
	{
		delete m_instane;
		m_instane = NULL;
	}
}

void CParseSplitString::ParseParam3(const int nID, const CString strParam3 )
{
	assert(strParam3.GetLength()>0);

	vector<wstring> vecContent;
	SplitStringByChar(strParam3, L'|', vecContent);
	StructParam3 param3;
	param3.nID = nID;
	for (size_t i=0;i<vecContent.size();++i)
	{
		vector<wstring> vecContentSub;
		SplitStringByChar(vecContent[i].c_str(), L':', vecContentSub);
		if(vecContentSub.size()<2)
			continue;
		const int nValue = _wtoi(vecContentSub[0].c_str());
		CString strText = vecContentSub[1].c_str();
		param3.mapping[nValue] = strText;
	}
	m_vecStructParam3.push_back(param3);
}

const CString CParseSplitString::GetText( const int nID, const int nValue )
{
	const int size = m_vecStructParam3.size();
	for (int i=0;i<size;++i)
	{
		if (nID == m_vecStructParam3[i].nID)
		{
			map<int,CString>::iterator iter = m_vecStructParam3[i].mapping.begin();
			map<int,CString>::iterator iter_end = m_vecStructParam3[i].mapping.end();
			for (;iter!=iter_end;++iter)
			{
				if (iter->first == nValue)
				{
					return iter->second;
				}
			}
		}
	}
	return L"";
}

const bool CParseSplitString::IsExist( const int nID )
{
	const size_t size = m_vecStructParam3.size();
	for (size_t i=0;i<size;++i)
	{
		if (m_vecStructParam3[i].nID == nID)
		{
			return true;
		}
	}
	return false;
}

void CParseSplitString::DeleteOne( const int nID )
{
	vector<StructParam3>::iterator iter = m_vecStructParam3.begin();
	while (iter != m_vecStructParam3.end())
	{
		if ((*iter).nID == nID)
		{
			iter = m_vecStructParam3.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

const int CParseSplitString::GetValue( const int nID, const CString strText )
{
	const int size = m_vecStructParam3.size();
	for (int i=0;i<size;++i)
	{
		if (nID == m_vecStructParam3[i].nID)
		{
			map<int,CString>::iterator iter = m_vecStructParam3[i].mapping.begin();
			map<int,CString>::iterator iter_end = m_vecStructParam3[i].mapping.end();
			for (;iter!=iter_end;++iter)
			{
				if (iter->second == strText)
				{
					return iter->first;
				}
			}
		}
	}
	return -1;
}

