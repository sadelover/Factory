#pragma once

#include "../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Room_Contain_Pipe ���ڵ���������
*/
typedef struct tag_gRoom_Contain_Pipe
{
	int     iPipeID_In_Room;	//�����йܵ����
	tstring strPipeName_InRoom;	//�ܵ���ʶ�� 
	int     iPipeID;			//���йܵ���ʶ
	int     iOF_CPR_ID;			//����������ţ���ChillerPlantRoom���е�CPR_ID��
	BOOL    bEnabled;			//�Ƿ����
	int		from_service_id;	//�ܵ���������豸ID
	int		from_service_type;	//�ܵ���������豸����
	int		from_service_port;	//�ܵ���������豸�˿�
	int		to_service_id;		//�ܵ������յ��豸ID
	int		to_service_type;	//�ܵ������յ��豸����
	int		to_service_port;	//�ܵ������յ��豸�˿�
}gRoom_Contain_Pipe, *pRoom_Contain_Pipe;

enum eROOM_CONTAIN_PIPE
{
	RM_C_PIPE_PIPEID_IN_ROOM,
	RM_C_PIPE_PIPENAME_INROOM,
	RM_C_PIPE_PIPEID,
	RM_C_PIPE_OF_CPR_ID,
	RM_C_PIPE_ENABLED,
	RM_C_PIPE_FROM_SERVICE_ID,
	RM_C_PIPE_FROM_SERVICE_TYPE,
	RM_C_PIPE_FROM_SERVICE_PORT,
	RM_C_PIPE_TO_SERVICE_ID,
	RM_C_PIPE_TO_SERVICE_TYPE,
	RM_C_PIPE_TO_SERVICE_PORT,
	RM_C_PIPE_MAXNUM
};