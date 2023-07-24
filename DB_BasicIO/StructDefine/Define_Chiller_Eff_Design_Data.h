#pragma once

/*
Author:		Juneay
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Chiller_Eff_Design_Data ���ڵ���������
*/
typedef struct tag_gChiller_Eff_Design_Data
{
	int		iChillerID;		// ������ʶ��,��ChillerLib��id��
	double	dLEWT;			//�䶳ˮ��ˮ�¶�
	double	dECWT;			//��ȴˮ��ˮ�¶�
	double	dEvap_Flow;		//������������m3/h��
	double	dCond_Flow;		//������������m3/h��
	double	dCooling_Cap;	//������(kw)
	double	dHeating_Cap;	//������(kw)
	double	dPower;			//���빦��(kw)
}gChiller_Eff_Design_Data, *pChiller_Eff_Design_Data;

enum eCH_EFF_DD
{
	CH_EFF_DD_CHILLERID,
	CH_EFF_DD_LEWT,
	CH_EFF_DD_ECWT,
	CH_EFF_DD_EVAP_FLOW,
	CH_EFF_DD_COND_FLOW,
	CH_EFF_DD_COOLING_CAP,
	CH_EFF_DD_HEATING_CAP,
	CH_EFF_DD_POWER,
	CH_EFF_DD_MAXNUM,
};