#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		Juneay
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Chiller_Model ���ڵ���������
*/
typedef struct tag_gCH_Model
{
	int			iChillerModelID;		//���ģ�ͱ��
	tstring		strCHillerModelName;	//ģ������
	int			iChillerID;				//�����ʶ�ţ���ChillerLib��id��
	int			iType;					//ģ�����ͣ�0: ���ģ�� 1: ������� 2: �����㷨 
	SYSTEMTIME	tmModel_Gen_Time;		//ģ������ʱ��
	tstring		strModel_Param;			//ģ�Ͳ��� ���в�����"/"Ϊ����������ݿ�������������ģ�ͱ�д��Ա����������Ӱ�����ݿ����
}gCH_Model, *pCH_Model;

enum eCH_MODEL
{
	CH_MODEL_CHILLERMODELID,
	CH_MODEL_CHILLERMODELNAME,
	CH_MODEL_CHILLERID,
	CH_MODEL_TYPE,
	CH_MODEL_GEN_TIME,
	CH_MODEL_PARAM,
	CH_MODEL_MAXNUM,
};