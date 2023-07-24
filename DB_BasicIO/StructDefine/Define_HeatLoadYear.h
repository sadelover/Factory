#pragma  once
//define_heatLoadYear.h

#include <ATLComTime.h>

class CHeatLoadData
{
public:
    CHeatLoadData()
        :mHeatingLoad(0)
    {}
	COleDateTime    mRecordTime;
	double          mHeatingLoad;		 // 
};

enum eHeatLoadType
{
    HEAT_LOAD_YEAR = 0,
    HEAT_LOAD_MONTH,

    HEAT_LOAD_TYPE_MAX,
};

enum eCHeatLoadYearData
{
	HLY_TIME = 0,		// TIME
	HLY_HEATING_LOAD,			// VALUE

    HLY_MAX, 
};
