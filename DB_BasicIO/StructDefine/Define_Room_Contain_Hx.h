#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-Room_Contain_Hx 表内的所有内容
*/
typedef struct tag_gRoom_Contain_Hx
{
	int     iHxID_In_Room;		//机房中管道编号
	tstring strHxName_InRoom;	//管道标识名 
	int     iHxID;				//库中管道标识
	int     iOF_CPR_ID;			//所属机房编号，与ChillerPlantRoom表中的CPR_ID绑定
	BOOL    bEnabled;			//是否可用
	double	dMax_Capacity;		//最大散热能力(kw)
	int		iX;					//X座标
	int		iY;					//Y座标
	int		iPlcCtrl_id;			//PLC 控制的ID
	double  fScale;             //图标放大缩小系数
	double  fRotateAngel;       //图标旋转度数
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