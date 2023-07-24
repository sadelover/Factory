#pragma once

/*
Author:		Juneay
Time:		2010-07-12
Comments:	对应数据库中-Chiller_Eff_Design_Data 表内的所有内容
*/
typedef struct tag_gChiller_Eff_Design_Data
{
	int		iChillerID;		// 主机标识号,与ChillerLib表id绑定
	double	dLEWT;			//冷冻水出水温度
	double	dECWT;			//冷却水回水温度
	double	dEvap_Flow;		//蒸发器流量（m3/h）
	double	dCond_Flow;		//冷凝器流量（m3/h）
	double	dCooling_Cap;	//制冷量(kw)
	double	dHeating_Cap;	//制热量(kw)
	double	dPower;			//输入功率(kw)
}gChiller_Eff_Design_Data, *pChiller_Eff_Design_Data;

enum eCH_EFF_DD
{
	CH_EFF_DD_CHILLERID,
	CH_EFF_DD_LEWT,
	CH_EFF_DD_ECWT,
	CH_EFF_DD_EVAP_FLOW,
	CH_EFF_DD_COND_FLOW,
	CH_EFF_DD_COOLING_CAP,
	CH_EFF_DD_HEATING_CAP,
	CH_EFF_DD_POWER,
	CH_EFF_DD_MAXNUM,
};