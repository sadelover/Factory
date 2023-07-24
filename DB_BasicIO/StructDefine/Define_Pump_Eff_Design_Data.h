#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-Pump_Eff_Design_Data 表内的所有内容
*/
typedef struct tag_gPump_Eff_Design_Data
{
	int      iPumpID;  //水泵标识号
	double   dFlow;    //流量(m3/h)
	double   dHead;    //扬程(m)
	double   dFreq;    //频率(hz)
	double   dPower;   //电功率(kw)
}gPump_Eff_Design_Data, *pPump_Eff_Design_Data;

enum ePUMP_EFF_DESIGN_DATA
{
	PM_EFF_DD_PMID,
	PM_EFF_DD_FLOW,
	PM_EFF_DD_HEAD,
	PM_EFF_DD_FREQ,
	PM_EFF_DD_POWER,
	PM_EFF_DD_MAXNUM
};