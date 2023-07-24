#pragma once

#include "../Tools/StringDef.h"

/*
Author:		Guibicheng(David)
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-ControlParamRecord ���ڵ���������
*/

#define  CONTROLPARAM_BUFFERSIZE 256

typedef struct tag_gControlParam
{
	SYSTEMTIME	tmTime;												//ʱ��	
	double		dT;													//��������
	double		dFai;												//�������ʪ��
	double		dTwb;												//����ʪ���¶�
	char		LEWT_Setpoint[CONTROLPARAM_BUFFERSIZE];				//�䶳ˮ��ˮ�¶��趨
	double		dLCWT_Setpoint;										//��ȴˮ�����¶��趨ֵ
	char		ChillerCombination[CONTROLPARAM_BUFFERSIZE];		//������ ��/Ϊ�ָ�����chilleridinbuilding��¼����
	char		CTCombination[CONTROLPARAM_BUFFERSIZE];				//��ȴ�����
	char		ChilledPumpCombination[CONTROLPARAM_BUFFERSIZE];	//�䶳�����
	char		SecChilledPumpCombination[CONTROLPARAM_BUFFERSIZE];	//�����䶳�����

	char        CoolingPumpCombination[CONTROLPARAM_BUFFERSIZE];	//��ȴ�����
	double		dLoad_Actual;				//ʵ�ʸ���
	double   	dLoad_UseCalc;              //������㸺��
  	double      dChill_Flow;                //�䶳ˮ����
    double      dCool_Flow;                 //��ȴˮ����
    double      dDTChill;                   //�䶳ˮ�²�
    double      dDTCool;                    //��ȴˮ�²�
    double      dChiller_K;                 //�����kֵ
    double      dRoomCOP;                   //����Ч��
    double      dCHGroup_Power;             //����ܹ���
    double      dCTGroup_Power;             //��ȴ���ܹ���
    double      dPriCHWPumpGroup_Power;         //һ�α��ܹ���
    double      dSecCHWPumpGroup_Power;         //���α��ܹ���
    double      dCWPumpGroup_Power;             //��ȴ���ܹ���
}gControlParamRecord, *pControlParamRecord;

enum eCTRLPARAM
{
	CTRLPM_TIME,					//ʱ��
	CTRLPM_T,						//��������¶�
	CTRLPM_FAI,						//�������ʪ��
	CTRLPM_TWB,						//����ʪ���¶�
	CTRLPM_LEWT_Setpoint,			//�䶳ˮ��ˮ�¶��趨
	CTRLPM_LCWT_Setpoint,			//��ȴˮ�����¶��趨
	CTRLPM_ChillerCombination,		//����б�
	CTRLPM_CoolingTowerCombination,	//��ȴ���б�
	CTRLPM_ChilledPumpCombination,			//���б�
	CTRLPM_CoolingPumpCombination,			//���б�
	CTRLPM_Load_Actual,				//ʵ�ʸ���
	CTRLPM_Load_Calc_Aver,			//���ɵ�ƽ��ֵ��
	CTRLPM_Chill_Flow,
    CTRLPM_Cool_Flow,
    CTRLPM_DTChill,
    CTRLPM_DTCool,
    CTRLPM_Chiller_K,
    CTRLPM_RoomCOP,
    CTRLPM_CHGroup_Power,
    CTRLPM_CTGroup_Power,
    CTRLPM_PriCHWPumpGroup_Power,
    CTRLPM_SecCHWPumpGroup_Power,
    CTRLPM_CWPumpGroup_Power,
	CTRLPM_MAXNUM
};