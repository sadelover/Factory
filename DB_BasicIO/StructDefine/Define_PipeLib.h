#pragma once

/*
Author:		Chang
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-pipelib ���ڵ���������
*/
typedef struct tag_gPipeLib
{
   int		iPipeID;	//�ܵ�ID
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