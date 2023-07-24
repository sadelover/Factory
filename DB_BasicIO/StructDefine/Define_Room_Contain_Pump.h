#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Room_Contain_Pump ���ڵ���������
*/
typedef struct tag_gRoom_Contain_Pump
{
	int     iPumpID_In_Room;	//������
	tstring strPumpName_InRoom;	//�����ʶ�� 
	int     iPumpID;			//���л����ʶ
	int     iOF_CPR_ID;			//����������ţ���ChillerPlantRoom���е�CPR_ID��
	int     iPump_Location;		//��λ�ã�0: һ���䶳��2: �����䶳��1: ��ȴ��3: ���������䶳��4: ���� 
	BOOL    bEnabled;			//�Ƿ����
	int    bStandby;			//�Ƿ��Ǳ��ã�Ĭ��False
	int		iX;					//X����
	int		iY;					//Y����
	int     iPumpModelID;       //��ģ��ID
	int		iPlcCtrl_id;		//PLC ���Ƶ�ID
	BOOL    bWithVSD;           //�Ƿ����Ƶ
	double  fScale;             //ͼ��Ŵ���Сϵ��
	double  fRotateAngel;       //ͼ����ת����
	int		iImageID;			//ͼƬ����id
}gRoom_Contain_Pump, *pRoom_Contain_Pump;

enum eROOM_CONTAIN_PUMP
{
	RM_C_PUMP_PUMPID_IN_ROOM,
	RM_C_PUMP_PUMPNAME_INROOM,
	RM_C_PUMP_PUMPID,
	RM_C_PUMP_OF_CPR_ID,
	RM_C_PUMP_PUMP_LOCATION,
	RM_C_PUMP_ENABLED,
	RM_C_PUMP_STANDBY,
	RM_C_PUMP_X,
	RM_C_PUMP_Y,
	RM_C_PUMP_MODELID,
	RM_C_PUMP_PLCID,
	RM_C_PUMP_WITHVSD,
	RM_C_PUMP_SCALE,
	RM_C_PUMP_ROTATEANGLE,
	RM_C_PUMP_IMAGEID,
	RM_C_PUMP_MAXNUM,

};
