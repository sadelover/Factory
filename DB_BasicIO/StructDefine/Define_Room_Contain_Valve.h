#pragma once

#include "../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Room_Contain_Valve ���ڵ���������
*/
typedef struct tag_gRoom_Contain_Valve
{
	int     iValveID_In_Room;		//�����з����
	tstring strValveName_InRoom;	//����ʶ�� 
	int     iValveID;				//���з���ʶ
	int     iOF_CPR_ID;				//����������ţ���ChillerPlantRoom���е�CPR_ID��
	BOOL    bEnabled;				//�Ƿ����
	int		iX;						//X����
	int		iY;						//Y����
	int		iPlcCtrl_id;			//PLC ���Ƶ�ID
	double  fScale;             //ͼ��Ŵ���Сϵ��
	double  fRotateAngel;       //ͼ����ת����
	int		iImageID;
}gRoom_Contain_Valve, *pRoom_Contain_Valve;

enum eROOM_CONTAIN_VALVE
{
	RM_C_VALVE_VALVEID_IN_ROOM,
	RM_C_VALVE_VALVENAME_INROOM,
	RM_C_VALVE_VALVEID,
	RM_C_VALVE_OF_CPR_ID,
	RM_C_VALVE_ENABLED,
	RM_C_VALVE_X,
	RM_C_VALVE_Y,
	RM_C_VALVE_PLCID,
	RM_C_VALVE_SCALE,
	RM_C_VALVE_ROTATEANGLE,
	RM_C_VALVE_IMAGEID,
	RM_C_VALVE_MAXNUM
};