#pragma once

#include "../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Room_Contain_CoolingTower ���ڵ���������
*/
typedef struct tag_gRoom_Contain_CoolingTower
{
	int     iTowerID_In_Room;		//�������
	tstring strTowerName_InRoom;	//������ʶ��ʶ�� 
	int     iTowerID;				//���б�ʶ
	int     iOF_CPR_ID;				//����������ţ���ChillerPlantRoom���е�CPR_ID��
	BOOL    bEnabled;				//�Ƿ����
	double	dMax_Capacity;			//���ɢ������(kw)
	int		iX;						//X����
	int		iY;						//Y����
	int     iCTModelID;             //��ȴ��ģ��ID
	int		iPlcCtrl_id;			//PLC ���Ƶ�ID
	BOOL    bWithVSD;           //�Ƿ����Ƶ
	double  fScale;             //ͼ��Ŵ���Сϵ��
	double  fRotateAngel;       //ͼ����ת����
	double fSCool;
}gRoom_Contain_CoolingTower, *pRoom_Contain_CoolingTower;

enum eROOM_CONTAIN_COOLINGTOWER
{
	RM_C_CT_TOWERID_IN_ROOM,
	RM_C_CT_TOWERNAME_INROOM,
	RM_C_CT_TOWERID,
	RM_C_CT_OF_CPR_ID,
	RM_C_CT_ENABLED,
	RM_C_CT_MAX_CAPACITY,
	RM_C_CT_X,
	RM_C_CT_Y,
	RM_C_CT_MODELID,
	RM_C_CT_MAXNUM
};