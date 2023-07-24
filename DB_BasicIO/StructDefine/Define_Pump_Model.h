#pragma once
#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Pump_Model ���ڵ���������
*/
typedef struct tag_gPump_Model
{
   int			iPumpModelID;		//ˮ��ģ�ͱ��
   tstring		strPumpModelName;		//ģ������
   int			iPumpID;			//ˮ�ñ�ʶ�ţ���ChillerLib��id��
   int			iType;				//ģ�����ͣ�0: ���ģ�� 1: ������� 2: �����㷨
   SYSTEMTIME	tmModel_Gen_Time;	//ģ������ʱ��
   tstring		strModel_Param;		//���в����Էֺ�Ϊ����������ݿ� �������������ģ�ͱ�д��Ա����������Ӱ�����ݿ����
}gPump_Model, *pPump_Model;

enum ePUMP_MODEL
{
	PM_MODEL_PUMPMODELID,
	PM_MODEL_PUMPMODELNAME,
	PM_MODEL_PUMPID,
	PM_MODEL_TYPE,
	PM_MODEL_GEN_TIME,
	PM_MODEL_PARAM,
	PM_MODEL_MAXNUM
};