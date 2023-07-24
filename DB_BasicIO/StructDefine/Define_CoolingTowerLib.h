#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-CoolingTowerLib ���ڵ���������
*/
typedef struct tag_gCTLib
{
   int      iCTID;	//��ʶ��
   tstring  strModelName;		//�ͺ�
   tstring  strBrand;			//Ʒ��
   int      iType;				//����
   double	dRating_Air_Flow;   //����(m3/h)
   double	dRating_Water_Flow; //����(m3/h)
   double	dRating_TWB_Air;    //���������ʪ���¶�
   double	dRating_T_Water_In; //�������ˮ�¶�
   double	dRating_T_Water_Out;//�������ˮ�¶�
   double	dPower;             //����
   int      iFansCount;			//���ȸ���
}gCTLib, *pCTLib;

enum eCTLIB
{
	CTLIB_CTID,
	CTLIB_MODELNAME,
	CTLIB_BRAND,
	CTLIB_TYPE,
	CTLIB_RATING_AIR_FLOW,
	CTLIB_RATING_WATER_FLOW,
	CTLIB_RATING_TWB_AIR,
	CTLIB_RATING_T_WATER_IN,
	CTLIB_RATING_T_WATER_OUT,
	CTLIB_POWER,
	CTLIB_FANSCOUNT,
	CTLIB_MAXNUM
};