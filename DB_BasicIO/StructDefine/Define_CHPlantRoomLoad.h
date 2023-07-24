#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		Chang
Time:		2010-07-12
Comments:	对应数据库中-Chiller Plant Room Load Define表内的所有内容
*/
class gRoomLoad
{
public:
    gRoomLoad()
        :dCoolingLoad(0)
        ,dHeatingLoad(0)	//热负荷
        ,dTotal_Flow(0)		//总冷冻水量
        ,dTotal_T_Supply(0)	//总供水温度
        ,dTotal_T_Return(0)	//总回水温度
        , iRoomID(0)
        ,dTdry(0)
        ,dTwet(0)
		,fPowerBill(0)
    {}
	SYSTEMTIME	tmTime;				//时间
	double		dCoolingLoad;		//冷负荷
	double      dCoolingLoad1;		
	double      dCoolingLoad2;
	double      dCoolingLoad3;
	double      dCoolingLoad4;
	double		dHeatingLoad;		//热负荷
	double		dTotal_Flow;		//总冷冻水量
	double		dTotal_T_Supply;	//冷冻供水温度
	double		dTotal_T_Return;	//冷却回水温度
	int         iRoomID;
	double	    dTdry;
	double		dTwet;
	double      fPowerBill;
};

enum eRMLOAD
{
	RMLOAD_TIME,
	RMLOAD_COOLINGLOAD,
	RMLOAD_HEATINGLOAD,
	RMLOAD_TOTAL_FLOW,
	RMLOAD_TOTAL_T_SUPPLY,
	RMLOAD_TOTAL_T_RETURN,
	RMLOAD_ROOMID,
	RMLOAD_TDRY,
	RMLOAD_TWET,
	RMLOAD_PB,
	RMLOAD_COOLINGLOAD1,
	RMLOAD_COOLINGLOAD2,
	RMLOAD_COOLINGLOAD3,
	RMLOAD_COOLINGLOAD4,
	RMLOAD_MAXNUM
};