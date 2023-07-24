#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Pump_Eff_Design_Data ���ڵ���������
*/
typedef struct tag_gPump_Eff_Design_Data
{
	int      iPumpID;  //ˮ�ñ�ʶ��
	double   dFlow;    //����(m3/h)
	double   dHead;    //���(m)
	double   dFreq;    //Ƶ��(hz)
	double   dPower;   //�繦��(kw)
}gPump_Eff_Design_Data, *pPump_Eff_Design_Data;

enum ePUMP_EFF_DESIGN_DATA
{
	PM_EFF_DD_PMID,
	PM_EFF_DD_FLOW,
	PM_EFF_DD_HEAD,
	PM_EFF_DD_FREQ,
	PM_EFF_DD_POWER,
	PM_EFF_DD_MAXNUM
};