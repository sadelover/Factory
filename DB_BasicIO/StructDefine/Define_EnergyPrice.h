#pragma once

#include "../Tools/StringDef.h"

/*
Author:		Chang
Time:		2010-07-12
Comments:	对应数据库中-EnergyPrice表内的所有内容
*/
typedef struct tag_gEnergyPrice
{
	SYSTEMTIME	tmFromTime;		//指定时间起点
	SYSTEMTIME	tmToTime;		//指定时间终点
	double		dElecPrice;		//每度电价
	double		dGasPrice;		//每m3气价
	tstring		strUnit;		//单位：RBM,USD
}gEnergyPrice, *pEnergyPrice;

enum eENERGYPRICE
{
	ENPRICE_FROMTIME,
	ENPRICE_TOTIME,
	ENPRICE_ELECPRICE,
	ENPRICE_GASPRICE,
	ENPRICE_UNIT,
	ENPRICE_MAXNUM
};