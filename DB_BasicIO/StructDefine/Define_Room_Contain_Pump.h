#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-Room_Contain_Pump 表内的所有内容
*/
typedef struct tag_gRoom_Contain_Pump
{
	int     iPumpID_In_Room;	//机组编号
	tstring strPumpName_InRoom;	//机组标识名 
	int     iPumpID;			//库中机组标识
	int     iOF_CPR_ID;			//所属机房编号，与ChillerPlantRoom表中的CPR_ID绑定
	int     iPump_Location;		//泵位置，0: 一次冷冻泵2: 二次冷冻泵1: 冷却泵3: 高区二级冷冻泵4: 其他 
	BOOL    bEnabled;			//是否可用
	int    bStandby;			//是否是备用，默认False
	int		iX;					//X座标
	int		iY;					//Y座标
	int     iPumpModelID;       //泵模型ID
	int		iPlcCtrl_id;		//PLC 控制的ID
	BOOL    bWithVSD;           //是否带变频
	double  fScale;             //图标放大缩小系数
	double  fRotateAngel;       //图标旋转度数
	int		iImageID;			//图片库中id
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
