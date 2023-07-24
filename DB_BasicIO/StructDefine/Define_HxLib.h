#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-HxLib 表内的所有内容
*/
typedef struct tag_gHxLib
{
	int		iHxID;		//换热设备ID
	int     iHxType;	//换热设备类型
	double  dHxDP1;		//换热设备一次侧阻力系数
	double  dHxDP2;		//换热设备二次侧阻力系数
	double  dTin1;		//一次侧进水温度
	double  dTin2;		//二次侧进水温度
	double  dTout1;		//一次侧出水温度
	double  dTout2;		//二次侧出水温度
	double  dHxQ;		//换热量(kw)
}gHxLib, *pHxLib;

enum eHXLIB
{
	HXLIB_HXID,
	HXLIB_HXTYPE,
	HXLIB_HXDP1,
	HXLIB_HXDP2,
	HXLIB_TIN1,
	HXLIB_TIN2,
	HXLIB_TOUT1,
	HXLIB_TOUT2,
	HXLIB_HXQ,
	HXLIB_MAXNUM
};