#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-CornerLib 表内的所有内容
*/
typedef struct tag_gCornerLib
{
   int		iCornerID;	//弯头ID
   int		iCornerType;//弯头类型
   double	dCorners;	//弯头局部阻力系数
}gCornerLib, *pCornerLib;

enum eCORNERLIB
{
	CORNERLIB_CORNERLIBID,
	CORNERLIB_CORNERTYPE,
	CORNERLIB_CORNERS,
	CORNERLIB_MAXNUM
};