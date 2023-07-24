#pragma once
#include "../../Tools/StringDef.h"

/*
Author:		huafengyou
Time:		2011-08-24
Comments:	��Ӧ���ݿ���-WaterSoil_Model ���ڵ���������
*/
typedef struct tag_gWaterSoil_Model
{
	int			iWaterSoilModelID;		//�����ģ�ͱ��
	tstring		strWaterSoilModelName;		//ģ������
	int			iWaterSoilID;			//����ܱ�ʶ��
	int			iType;				//ģ�����ͣ�0: ���ģ�� 1: ������� 2: �����㷨
	SYSTEMTIME	tmModel_Gen_Time;	//ģ������ʱ��
	tstring		strModel_Param;		//���в����Էֺ�Ϊ����������ݿ� �������������ģ�ͱ�д��Ա����������Ӱ�����ݿ����
}gWaterSoil_Model, *pWaterSoil_Model;

enum eWATERSOIL_MODEL
{
	WS_MODEL_WATERSOILMODELID,
	WS_MODEL_WATERSOILMODELNAME,
	WS_MODEL_WATERSOILID,
	WS_MODEL_TYPE,
	WS_MODEL_GEN_TIME,
	WS_MODEL_PARAM,
	WS_MODEL_MAXNUM
};