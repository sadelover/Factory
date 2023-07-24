#pragma once
#include "../../Tools/StringDef.h"

enum CHStartType	//�����������ʽ
{
	Solide_State,
	S_Delta,
	Auto_Trans_per57,
	Auto_Trans_per65,
	Auto_Trans_per80,
	Across_the_line,
	VSD_W_O_Filter,
	VSD_W_Filter,
};

/*
Author:		Juneay
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-ChillerLib���ڵ���������
*/
typedef struct tag_gCHLib
{
	int		iChillerID;			// �����ʶ��
	tstring	strModelName;		// ����ͺ�
	tstring strBrand;			//Ʒ��
	int     iType;				//���ͣ��ݸˡ����ġ�ֱȼ�� 
	int     iRefrig;			//���������  0: R123/1:R134A/2: R22/3:
	int     iMotor_Start_Type;	//���������ʽ
	double  dRating_CoolingCap;	//�������
	double  dRating_HeatCap;	//�������
	double  dRating_Flow_Evap;	//��䶳ˮ����
	double  dRating_Flow_Cond;	//���ȴˮ����
	double  dRating_COP;		//�COP
	double  dRating_LEWT;		//��䶳ˮ��ˮ�¶�
	double  dRating_ECWT;		//���ȴˮ��ˮ�¶�
	double  dSurge_Param1;		//�������1 
	double  dSurge_Param2;		//�������2
	double  dSurge_Param3;		//�������3
	double  dSurge_Param4;		//�������4
	double  dRating_DP_Evap;    //������ѹ��
	double  dRating_DP_Cond;    // ������ѹ��
}gCHLib, *pCHLib;

enum eCHLIB
{
	CHLIB_CHILLERID,
	CHLIB_MODELNAME,
	CHLIB_BRAND,
	CHLIB_TYPE,
	CHLIB_REFRIG,
	CHLIB_MOTOR_START_TYPE,
	CHLIB_RATING_COOLINGCAP,
	CHLIB_RATING_HEATCAP,
	CHLIB_RATING_FLOW_EVAP,
	CHLIB_RATING_FLOW_COND,
	CHLIB_RATING_COP,
	CHLIB_RATING_LEWT,
	CHLIB_RATING_ECWT,
	CHLIB_SURGE_PARAM1,
	CHLIB_SURGE_PARAM2,
	CHLIB_SURGE_PARAM3,
	CHLIB_SURGE_PARAM4,
	CHLIB_RATING_DP_EVAP,
	CHLIB_RATING_DP_COND,
	CHLIB_MAXNUM
};