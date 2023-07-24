#pragma once

#ifndef RUNCONTITIONSIMULATE_H__
#define RUNCONTITIONSIMULATE_H__

typedef struct tag_runconditionsimulate
{
	double		dLoad;
	SYSTEMTIME	stLoadTime;
	double		dTDry;
	double		dTWet;
	string		strRunChillerList;
	string		strRunChilledPumpList;
	string		strRunCoolingPumpList;
	string		strRunCoolingTowerList;
	string		strLewtList;
	double		dEcwt;
	string		strRunChillerLoadList;
	string		strRunChillerCopList;
	string		strRunChilledPumpPowerList;
	string		strRunCoolingPumpPowerList;
	string		strRunCoolingTowerPowerList;
	double		dRoomCop;
	int			iRoomControlType;
	double		dChilledDT;
	double		dCoolDT;
	string		strRunValveList;	
}gRunConditionSimulate, *pRunConditionSimulate;

enum	RUNCONDITION_SIMULATE
{
	RUNCONDITION_LOAD,						//负荷
	RUNCONDITION_LOADTIME,					//生成时间
	RUNCONDITION_TDRY,						//干球温度
	RUNCONDITION_TWET,						//湿球温度
	RUNCONDITION_RUNCHILLERLIST,				//开冷机列表
	RUNCONDITION_RUNCHILLEDPUMPLIST,			//开冷冻泵列表
	RUNCONDITION_RUNCOOLINGPUMPLIST,			//开冷却泵列表
	RUNCONDITION_RUNCOOLINGTOWERLIST,		//开冷却塔列表
	RUNCONDITION_LEWTLIST,					//冷机出水温度设定点列表
	RUNCONDITION_ECWT,						// 进塔水温设定点
	RUNCONDITION_RUNCHILLERLOADLIST,			// 冷机负荷列表
	RUNCONDITION_RUNCHILLERCOPLIST,			// 冷机能耗列表
	RUNCONDITION_RUNCHILLEDPUMPPOWERLIST,	//冷冻泵能耗列表
	RUNCONDITION_RUNCOOLINGPUMPPOWERLIST,	//冷却泵能耗列表
	RUNCONDITION_RUNCOOLINGTOWERPOWERLIST,	//冷却塔能耗列表
	RUNCONDITION_ROOMCOP,					//机房cop
	RUNCONDITION_CONTROLTYPE,				//控制类型，2个值, uppc控制和正常控制
	RUNCONDITION_CHILLEDDT,
	RUNCONDITION_COOLDT,
	RUNCONDITION_RUNVALVELIST,				//开启的阀门列表
};












#endif