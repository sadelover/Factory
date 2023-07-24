#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-ValveLib 表内的所有内容
*/
typedef struct tag_gValveLib
{
   int		iValveID;	//阀门ID
   int		iValveType;	//阀门类型
   double	dValves;	//阀门阻力系数
}gValveLib, *pValveLib;

enum eVALVELIB
{
	VALVELIB_VALVEID,
	VALVELIB_VALVETYPE,
	VALVELIB_VALVES,
	VALVELIB_MAXNUM
};