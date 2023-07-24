
#ifndef Record_Point_List_H__
#define Record_Point_List_H__

#pragma  once

#include <ATLComTime.h>

#define POINT_LIST_BUFFERSIZE 256
#include <string>
// this is a pod struct.
// if you use struct, please make all the data members pod.
typedef struct tag_Record_Point_List_Entry{
	int index;
	wchar_t shortname[POINT_LIST_BUFFERSIZE];
	wchar_t longname[POINT_LIST_BUFFERSIZE];
	wchar_t remark[POINT_LIST_BUFFERSIZE];
	wchar_t unit[POINT_LIST_BUFFERSIZE];
}Record_Point_List_Entry, *pRecord_Point_List_Entry;


enum RECORD_POINT_LIST_COLUMN
{
	RECORD_POINT_LIST_INDEX,
	RECORD_POINT_LIST_SHORTNAME,
	RECORD_POINT_LIST_LONGNAME,
	RECORD_POINT_LIST_REMARK,
	RECORD_POINT_LIST_UNIT,
};

class RecordPointValue
{
public:
    RecordPointValue()
		:mPointIndex(0)
		,mValueDouble(0)
	{};
    std::wstring             mStrTime;
    std::wstring             mStrValue;
	double				mValueDouble;
    COleDateTime        mOleTime;
    UINT                mPointIndex;

    enum POINT_LIST_
    {
        POINT_LIST_TIME = 0,
        POINT_LIST_INDEX,
        POINT_LIST_VALUE,
    };

};

typedef struct tag_gMonthReportParam 
{
	tag_gMonthReportParam()
		:strReportYM(_T(""))
		,strEqmName(_T(""))
	{
		dPowerTotal = 0;
		dTimeTotal = 0;
	}

	CString strReportYM;
	CString strEqmName;
	double dPowerTotal;
	double dTimeTotal;
}MonthReportParam;

typedef struct tag_gPointlistParam
{
	tag_gPointlistParam()
		:strPoint(TEXT(""))
		,strVar(TEXT(""))
		,strName(TEXT(""))
	{

	}
	std::wstring strPoint;
	std::wstring strVar;
	std::wstring strName;
}PointlistParam;
#endif