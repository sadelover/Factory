#pragma once
#include "../../Tools/StringDef.h"

/*
Author:		hua
Time:		2011-01-14
Comments:	��Ӧ���ݿ���-Tank_Model ���ڵ���������
*/
typedef struct tag_gTank_Model
{
	int			iTankModelID;		//����ģ�ͱ��
	tstring		strTankModelName;	//ģ������
	int			iTankID;			//���۱�ʶ�ţ���TankLib��id��
	int			iType;				//ģ�����ͣ�0: ���ģ�� 1: ������� 2: �����㷨
	SYSTEMTIME	tmModel_Gen_Time;	//ģ������ʱ��
	tstring		strModel_Param;		//���в����Էֺ�Ϊ����������ݿ� �������������ģ�ͱ�д��Ա����������Ӱ�����ݿ����
}gTank_Model, *pTank_Model;

//enum eTANK_MODEL
//{
//	PM_MODEL_TANKMODELID,
//	PM_MODEL_TANKMODELNAME,
//	PM_MODEL_TANKID,
//	//PM_MODEL_TYPE,
//	PM_MODEL_GEN_TIME,
//	PM_MODEL_PARAM,
//	PM_MODEL_MAXNUM
//};