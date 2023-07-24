#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-PumpLib ���ڵ���������
*/
typedef struct tag_gPumpLib
{
	int      iPumpID;		//ˮ�ñ�ʶ��
	tstring  strModelName;	//ˮ���ͺ�
	tstring  strBrand;		//Ʒ��
	int      iType;			//����
	int      iRating_RPM;	//ת��
	double   dRating_Flow;	//�����
	double   dRating_Head;	//����
	double   dRating_Power;	//����빦��
}gPumpLib, *pPumpLib;

enum ePUMPLIB
{
	PUMPLIB_PUMPID,
	PUMPLIB_MODELNAME,
	PUMPLIB_BRAND,
	PUMPLIB_TYPE,
	PUMPLIB_RATING_RPM,
	PUMPLIB_RATING_FLOW,
	PUMPLIB_RATING_HEAD,
	PUMPLIB_RATING_POWER,
	PUMPLIB_MAXNUM
};