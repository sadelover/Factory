#pragma once

#include "../Tools/StringDef.h"

/*
Author:		Guibicheng(David)
Time:		2010-07-12
Comments:	对应数据库中-ControlParamRecord 表内的所有内容
*/

#define  CONTROLPARAM_BUFFERSIZE 256

typedef struct tag_gControlParam
{
	SYSTEMTIME	tmTime;												//时间	
	double		dT;													//室外气温
	double		dFai;												//室外相对湿度
	double		dTwb;												//室外湿球温度
	char		LEWT_Setpoint[CONTROLPARAM_BUFFERSIZE];				//冷冻水出水温度设定
	double		dLCWT_Setpoint;										//冷却水进塔温度设定值
	char		ChillerCombination[CONTROLPARAM_BUFFERSIZE];		//冷机组合 以/为分隔符，chilleridinbuilding记录下来
	char		CTCombination[CONTROLPARAM_BUFFERSIZE];				//冷却塔组合
	char		ChilledPumpCombination[CONTROLPARAM_BUFFERSIZE];	//冷冻泵组合
	char		SecChilledPumpCombination[CONTROLPARAM_BUFFERSIZE];	//二次冷冻泵组合

	char        CoolingPumpCombination[CONTROLPARAM_BUFFERSIZE];	//冷却泵组合
	double		dLoad_Actual;				//实际负荷
	double   	dLoad_UseCalc;              //参与计算负荷
  	double      dChill_Flow;                //冷冻水流量
    double      dCool_Flow;                 //冷却水流量
    double      dDTChill;                   //冷冻水温差
    double      dDTCool;                    //冷却水温差
    double      dChiller_K;                 //冷机的k值
    double      dRoomCOP;                   //机房效率
    double      dCHGroup_Power;             //冷机总功率
    double      dCTGroup_Power;             //冷却塔总功率
    double      dPriCHWPumpGroup_Power;         //一次泵总功率
    double      dSecCHWPumpGroup_Power;         //二次泵总功率
    double      dCWPumpGroup_Power;             //冷却泵总功率
}gControlParamRecord, *pControlParamRecord;

enum eCTRLPARAM
{
	CTRLPM_TIME,					//时间
	CTRLPM_T,						//室外干球温度
	CTRLPM_FAI,						//室外相对湿度
	CTRLPM_TWB,						//室外湿球温度
	CTRLPM_LEWT_Setpoint,			//冷冻水出水温度设定
	CTRLPM_LCWT_Setpoint,			//冷却水进塔温度设定
	CTRLPM_ChillerCombination,		//冷机列表
	CTRLPM_CoolingTowerCombination,	//冷却塔列表
	CTRLPM_ChilledPumpCombination,			//泵列表
	CTRLPM_CoolingPumpCombination,			//泵列表
	CTRLPM_Load_Actual,				//实际负荷
	CTRLPM_Load_Calc_Aver,			//负荷的平均值。
	CTRLPM_Chill_Flow,
    CTRLPM_Cool_Flow,
    CTRLPM_DTChill,
    CTRLPM_DTCool,
    CTRLPM_Chiller_K,
    CTRLPM_RoomCOP,
    CTRLPM_CHGroup_Power,
    CTRLPM_CTGroup_Power,
    CTRLPM_PriCHWPumpGroup_Power,
    CTRLPM_SecCHWPumpGroup_Power,
    CTRLPM_CWPumpGroup_Power,
	CTRLPM_MAXNUM
};