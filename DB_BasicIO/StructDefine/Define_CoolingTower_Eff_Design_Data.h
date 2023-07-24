#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-CoolingTower_Eff_Design_Data ���ڵ���������
*/
typedef struct tag_gCT_Eff_Design_Data
{
   int		iCTID;	//��ȴ����ʶ��
   double	dTwb_Air;			//��������ʪ���¶�
   double	dT_Water_In;		//��ȴ������ˮ��
   double	dT_Water_Out;		//��ȴ������ˮ��
   double	dFreq;				//���Ƶ��
   double	dFlow_Water;		//ˮ��
   double	dFlow_Air;			//����
}gCT_Eff_Design_Data, *pCT_Eff_Design_Data;
enum eCT_EFF_DD
{
	CT_EFF_DD_CTID,
	CT_EFF_DD_TWB_AIR,
	CT_EFF_DD_T_WATER_IN,
	CT_EFF_DD_T_WATER_OUT,
	CT_EFF_DD_FREQ,
	CT_EFF_DD_FLOW_WATER,
	CT_EFF_DD_FLOW_AIR,
	CT_EFF_DD_MAXNUM
};

