#pragma once

#include "../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-Room_Contain_CoolingTower 表内的所有内容
*/
typedef struct tag_gRoom_Contain_CoolingTower
{
	int     iTowerID_In_Room;		//冷塔编号
	tstring strTowerName_InRoom;	//冷塔标识标识名 
	int     iTowerID;				//库中标识
	int     iOF_CPR_ID;				//所属机房编号，与ChillerPlantRoom表中的CPR_ID绑定
	BOOL    bEnabled;				//是否可用
	double	dMax_Capacity;			//最大散热能力(kw)
	int		iX;						//X座标
	int		iY;						//Y座标
	int     iCTModelID;             //冷却塔模型ID
	int		iPlcCtrl_id;			//PLC 控制的ID
	BOOL    bWithVSD;           //是否带变频
	double  fScale;             //图标放大缩小系数
	double  fRotateAngel;       //图标旋转度数
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