#pragma once

#include "../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-Room_Contain_Valve 表内的所有内容
*/
typedef struct tag_gRoom_Contain_Valve
{
	int     iValveID_In_Room;		//机房中阀编号
	tstring strValveName_InRoom;	//阀标识名 
	int     iValveID;				//库中阀标识
	int     iOF_CPR_ID;				//所属机房编号，与ChillerPlantRoom表中的CPR_ID绑定
	BOOL    bEnabled;				//是否可用
	int		iX;						//X座标
	int		iY;						//Y座标
	int		iPlcCtrl_id;			//PLC 控制的ID
	double  fScale;             //图标放大缩小系数
	double  fRotateAngel;       //图标旋转度数
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