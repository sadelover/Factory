#pragma once

/*
Author:		Chang
Time:		2010-07-12
Comments:	对应数据库中-room_contain_Corner 表内的所有内容
*/
typedef struct tag_gRoom_Contain_Corner
{
	int	iCorner_ID_InRoom;		//corner的编号
	int	iCorner_ID;			//终端ID
	int	iX;						//座标X
	int	iY;						//座标Y
	int iOF_CPR_ID;				//所属机房编号，与ChillerPlantRoom表中的CPR_ID绑定
}gRoom_Contain_Corner, *pRoom_Contain_Corner;

enum eROOM_CONTAIN_CORNER
{
	RM_C_CORNER_ID_INROOM,
	RM_C_CORNER_ID,
	RM_C_CORNER_X,
	RM_C_CORNER_Y,
	RM_C_OF_CPR_ID,	
};