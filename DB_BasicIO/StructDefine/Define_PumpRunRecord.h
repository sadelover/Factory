#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-PumpRunRecord ���ڵ���������
*/
typedef struct tag_gPumpRunRecord
{
	SYSTEMTIME  tmTime;		//ʱ��
	int         iPumpID;	//�����е�PumpID,ע���ⲻ�Ǳñ���ı�ʶ�ţ���Room_Contain_Pump���е�Ψһ��ʶ�� 
	double      dFlow;		//ˮ��
	double      dHead;		//���
	double      dFreq;		//Ƶ��
	double      dPower;		//����
	BOOL        bONOFF;		//�����ر�
	int         iOF_CPR_ID; //����������
}gPumpRunRecord, *pPumpRunRecord;

enum ePUMPRUNRECORD
{
	PUMPRUNREC_TIME,
	PUMPRUNREC_PUMPID,
	PUMPRUNREC_FLOW,
	PUMPRUNREC_HEAD,
	PUMPRUNREC_FREQ,
	PUMPRUNREC_POWER,
	PUMPRUNREC_ONOFF,
	PUMPRUNREC_OF_CPR_ID,
	PUMPRUNREC_MAXNUM
};