#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-PumpLib 表内的所有内容
*/
typedef struct tag_gPumpLib
{
	int      iPumpID;		//水泵标识号
	tstring  strModelName;	//水泵型号
	tstring  strBrand;		//品牌
	int      iType;			//类型
	int      iRating_RPM;	//转速
	double   dRating_Flow;	//额定流量
	double   dRating_Head;	//额定扬程
	double   dRating_Power;	//额定输入功率
}gPumpLib, *pPumpLib;

enum ePUMPLIB
{
	PUMPLIB_PUMPID,
	PUMPLIB_MODELNAME,
	PUMPLIB_BRAND,
	PUMPLIB_TYPE,
	PUMPLIB_RATING_RPM,
	PUMPLIB_RATING_FLOW,
	PUMPLIB_RATING_HEAD,
	PUMPLIB_RATING_POWER,
	PUMPLIB_MAXNUM
};