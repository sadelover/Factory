#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Room_Contain_Hx ���ڵ���������
*/
typedef struct tag_gRoom_Contain_Hx
{
	int     iHxID_In_Room;		//�����йܵ����
	tstring strHxName_InRoom;	//�ܵ���ʶ�� 
	int     iHxID;				//���йܵ���ʶ
	int     iOF_CPR_ID;			//����������ţ���ChillerPlantRoom���е�CPR_ID��
	BOOL    bEnabled;			//�Ƿ����
	double	dMax_Capacity;		//���ɢ������(kw)
	int		iX;					//X����
	int		iY;					//Y����
	int		iPlcCtrl_id;			//PLC ���Ƶ�ID
	double  fScale;             //ͼ��Ŵ���Сϵ��
	double  fRotateAngel;       //ͼ����ת����
}gRoom_Contain_Hx, *pRoom_Contain_Hx;

enum eROOM_CONTAIN_HX
{
	RM_C_HX_HXID_IN_ROOM,
	RM_C_HX_HXNAME_INROOM,
	RM_C_HX_HXID,
	RM_C_HX_OF_CPR_ID,
	RM_C_HX_ENABLED,
	RM_C_HX_MAX_CAPACITY,
	RM_C_HX_X,
	RM_C_HX_Y,	
};