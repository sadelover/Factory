#pragma once

/*
Author:		Chang
Time:		2010-07-12
Comments:	对应数据库中-pipelib 表内的所有内容
*/
typedef struct tag_gPipeLib
{
   int		iPipeID;	//管道ID
   double	dDiameter;	//
   int		iMaterial;	//
}gPipeLib, *pPipeLib;

enum ePIPELIB
{
	PIPELIB_PIPEID,
	PIPELIB_DIAMETER,
	PIPELIB_MATERIAL,
	PIPELIB_MAXNUM
};