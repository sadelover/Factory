#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-ValveLib ���ڵ���������
*/
typedef struct tag_gValveLib
{
   int		iValveID;	//����ID
   int		iValveType;	//��������
   double	dValves;	//��������ϵ��
}gValveLib, *pValveLib;

enum eVALVELIB
{
	VALVELIB_VALVEID,
	VALVELIB_VALVETYPE,
	VALVELIB_VALVES,
	VALVELIB_MAXNUM
};