#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-CoolingTowerLib 表内的所有内容
*/
typedef struct tag_gCTLib
{
   int      iCTID;	//标识号
   tstring  strModelName;		//型号
   tstring  strBrand;			//品牌
   int      iType;				//类型
   double	dRating_Air_Flow;   //风量(m3/h)
   double	dRating_Water_Flow; //风量(m3/h)
   double	dRating_TWB_Air;    //额定工况环境湿球温度
   double	dRating_T_Water_In; //额定工况进水温度
   double	dRating_T_Water_Out;//额定工况出水温度
   double	dPower;             //功率
   int      iFansCount;			//风扇个数
}gCTLib, *pCTLib;

enum eCTLIB
{
	CTLIB_CTID,
	CTLIB_MODELNAME,
	CTLIB_BRAND,
	CTLIB_TYPE,
	CTLIB_RATING_AIR_FLOW,
	CTLIB_RATING_WATER_FLOW,
	CTLIB_RATING_TWB_AIR,
	CTLIB_RATING_T_WATER_IN,
	CTLIB_RATING_T_WATER_OUT,
	CTLIB_POWER,
	CTLIB_FANSCOUNT,
	CTLIB_MAXNUM
};