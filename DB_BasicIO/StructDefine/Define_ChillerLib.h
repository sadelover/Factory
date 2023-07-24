#pragma once
#include "../../Tools/StringDef.h"

enum CHStartType	//冷机的启动方式
{
	Solide_State,
	S_Delta,
	Auto_Trans_per57,
	Auto_Trans_per65,
	Auto_Trans_per80,
	Across_the_line,
	VSD_W_O_Filter,
	VSD_W_Filter,
};

/*
Author:		Juneay
Time:		2010-07-12
Comments:	对应数据库中-ChillerLib表内的所有内容
*/
typedef struct tag_gCHLib
{
	int		iChillerID;			// 冷机标识号
	tstring	strModelName;		// 冷机型号
	tstring strBrand;			//品牌
	int     iType;				//类型：螺杆、离心、直燃等 
	int     iRefrig;			//制冷剂类型  0: R123/1:R134A/2: R22/3:
	int     iMotor_Start_Type;	//电机启动方式
	double  dRating_CoolingCap;	//额定制冷量
	double  dRating_HeatCap;	//额定制热量
	double  dRating_Flow_Evap;	//额定冷冻水流量
	double  dRating_Flow_Cond;	//额定冷却水流量
	double  dRating_COP;		//额定COP
	double  dRating_LEWT;		//额定冷冻水出水温度
	double  dRating_ECWT;		//额定冷却水回水温度
	double  dSurge_Param1;		//喘振参数1 
	double  dSurge_Param2;		//喘振参数2
	double  dSurge_Param3;		//喘振参数3
	double  dSurge_Param4;		//喘振参数4
	double  dRating_DP_Evap;    //蒸发器压降
	double  dRating_DP_Cond;    // 冷凝器压降
}gCHLib, *pCHLib;

enum eCHLIB
{
	CHLIB_CHILLERID,
	CHLIB_MODELNAME,
	CHLIB_BRAND,
	CHLIB_TYPE,
	CHLIB_REFRIG,
	CHLIB_MOTOR_START_TYPE,
	CHLIB_RATING_COOLINGCAP,
	CHLIB_RATING_HEATCAP,
	CHLIB_RATING_FLOW_EVAP,
	CHLIB_RATING_FLOW_COND,
	CHLIB_RATING_COP,
	CHLIB_RATING_LEWT,
	CHLIB_RATING_ECWT,
	CHLIB_SURGE_PARAM1,
	CHLIB_SURGE_PARAM2,
	CHLIB_SURGE_PARAM3,
	CHLIB_SURGE_PARAM4,
	CHLIB_RATING_DP_EVAP,
	CHLIB_RATING_DP_COND,
	CHLIB_MAXNUM
};