#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-CoolingTower_Eff_Design_Data 表内的所有内容
*/
typedef struct tag_gCT_Eff_Design_Data
{
   int		iCTID;	//冷却塔标识号
   double	dTwb_Air;			//环境空气湿球温度
   double	dT_Water_In;		//冷却塔进塔水温
   double	dT_Water_Out;		//冷却塔出塔水温
   double	dFreq;				//风机频率
   double	dFlow_Water;		//水量
   double	dFlow_Air;			//风量
}gCT_Eff_Design_Data, *pCT_Eff_Design_Data;
enum eCT_EFF_DD
{
	CT_EFF_DD_CTID,
	CT_EFF_DD_TWB_AIR,
	CT_EFF_DD_T_WATER_IN,
	CT_EFF_DD_T_WATER_OUT,
	CT_EFF_DD_FREQ,
	CT_EFF_DD_FLOW_WATER,
	CT_EFF_DD_FLOW_AIR,
	CT_EFF_DD_MAXNUM
};

