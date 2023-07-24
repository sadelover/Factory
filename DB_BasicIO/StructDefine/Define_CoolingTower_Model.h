#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-CoolingTower_Model ���ڵ���������
*/
typedef struct tag_gCT_Model
{
   int			iModelID;			//��ȴ��ģ�ͱ��
   tstring		strModelName;		//ģ������
   int			iCoolingTowerID;	//��ȴ����ʶ��,��CoolingTower��id��
   int			iType;				//ģ�����ͣ�0: ���ģ��1: �������2: �����㷨
   SYSTEMTIME	tmModel_Gen_Time;	//ģ������ʱ��
   tstring		strModel_Param;		//���в����Էֺ�Ϊ����������ݿ� �������������ģ�ͱ�д��Ա����������Ӱ�����ݿ����
}gCT_Model, *pCT_Model;

enum eCT_MODEL
{
	CT_MODEL_ID,
	CT_MODEL_NAME,
	CT_MODEL_CTID,
	CT_MODEL_TYPE,
	CT_MODEL_GEN_TIME,
	CT_MODEL_PARAM,
	CT_MODEL_MAXNUM
};