#pragma once

#include "../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Room_Contain_Chiller ���ڵ���������
*/
typedef struct tag_gRoom_Contain_Chiller
{
	int     iChillerID_In_Room;		//������
	tstring strCHillerName_InRoom;	//�����ʶ�� 
	int     iChillerID;				//���л����ʶ
	int     iOF_CPR_ID;				//����������ţ���ChillerPlantRoom���е�CPR_ID��
	BOOL    bEnabled;				//�Ƿ����
	int		iX;						//X����
	int		iY;						//Y����
	int     iChillerModelID;		//���ģ��ID
	int		iChillerComID;			//�������ͨѶID
	int		iPlcCtrl_id;			//PLC ���Ƶ�ID
	double  fScale;					//ͼ��Ŵ���Сϵ��
	double  fRotateAngel;			//ͼ����ת����
	double  fSChill;
	double  fSCool;
	double  fChillerRate;			//��������������ٷֱ�
	int     iHeatPump;              //�Ƿ�Ϊ�ȱ�
}gRoom_Contain_Chiller, *pRoom_Contain_Chiller;

enum eRoom_Contain_Chiller
{
	RM_C_CH_CHID_IN_ROOM,
	RM_C_CH_CHNAME_INROOM,
	RM_C_CH_CHID,
	RM_C_CH_OF_CPR_ID,
	RM_C_CH_ENABLED,
	RM_C_CH_X,
	RM_C_CH_Y,
	RM_C_CH_COMID,
	RM_C_CH_MODELID,
	RM_C_CH_MAXNUM,
};
