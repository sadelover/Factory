#pragma once

/*
Author:		Chang
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-room_contain_Corner ���ڵ���������
*/
typedef struct tag_gRoom_Contain_Corner
{
	int	iCorner_ID_InRoom;		//corner�ı��
	int	iCorner_ID;			//�ն�ID
	int	iX;						//����X
	int	iY;						//����Y
	int iOF_CPR_ID;				//����������ţ���ChillerPlantRoom���е�CPR_ID��
}gRoom_Contain_Corner, *pRoom_Contain_Corner;

enum eROOM_CONTAIN_CORNER
{
	RM_C_CORNER_ID_INROOM,
	RM_C_CORNER_ID,
	RM_C_CORNER_X,
	RM_C_CORNER_Y,
	RM_C_OF_CPR_ID,	
};