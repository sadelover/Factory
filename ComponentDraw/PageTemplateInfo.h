#pragma once


#include "Tools/DllDefine.h"
#include "VECTOR"

using namespace std;


class DLLEXPORT CPageTemplateInfo
{
public:
	CPageTemplateInfo(void);
	~CPageTemplateInfo(void);

	int m_nID;
	CString m_strName;
	vector<CString> m_strVNameList;
	vector<CString> m_strVDescriptionList;
};

