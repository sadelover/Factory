#pragma once

#ifndef RUNCONTITIONSIMULATE_H__
#define RUNCONTITIONSIMULATE_H__

typedef struct tag_runconditionsimulate
{
	double		dLoad;
	SYSTEMTIME	stLoadTime;
	double		dTDry;
	double		dTWet;
	string		strRunChillerList;
	string		strRunChilledPumpList;
	string		strRunCoolingPumpList;
	string		strRunCoolingTowerList;
	string		strLewtList;
	double		dEcwt;
	string		strRunChillerLoadList;
	string		strRunChillerCopList;
	string		strRunChilledPumpPowerList;
	string		strRunCoolingPumpPowerList;
	string		strRunCoolingTowerPowerList;
	double		dRoomCop;
	int			iRoomControlType;
	double		dChilledDT;
	double		dCoolDT;
	string		strRunValveList;	
}gRunConditionSimulate, *pRunConditionSimulate;

enum	RUNCONDITION_SIMULATE
{
	RUNCONDITION_LOAD,						//����
	RUNCONDITION_LOADTIME,					//����ʱ��
	RUNCONDITION_TDRY,						//�����¶�
	RUNCONDITION_TWET,						//ʪ���¶�
	RUNCONDITION_RUNCHILLERLIST,				//������б�
	RUNCONDITION_RUNCHILLEDPUMPLIST,			//���䶳���б�
	RUNCONDITION_RUNCOOLINGPUMPLIST,			//����ȴ���б�
	RUNCONDITION_RUNCOOLINGTOWERLIST,		//����ȴ���б�
	RUNCONDITION_LEWTLIST,					//�����ˮ�¶��趨���б�
	RUNCONDITION_ECWT,						// ����ˮ���趨��
	RUNCONDITION_RUNCHILLERLOADLIST,			// ��������б�
	RUNCONDITION_RUNCHILLERCOPLIST,			// ����ܺ��б�
	RUNCONDITION_RUNCHILLEDPUMPPOWERLIST,	//�䶳���ܺ��б�
	RUNCONDITION_RUNCOOLINGPUMPPOWERLIST,	//��ȴ���ܺ��б�
	RUNCONDITION_RUNCOOLINGTOWERPOWERLIST,	//��ȴ���ܺ��б�
	RUNCONDITION_ROOMCOP,					//����cop
	RUNCONDITION_CONTROLTYPE,				//�������ͣ�2��ֵ, uppc���ƺ���������
	RUNCONDITION_CHILLEDDT,
	RUNCONDITION_COOLDT,
	RUNCONDITION_RUNVALVELIST,				//�����ķ����б�
};












#endif