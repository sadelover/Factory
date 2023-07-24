#pragma once

#include "../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-Room_Contain_Chiller 表内的所有内容
*/
typedef struct tag_gRoom_Contain_Chiller
{
	int     iChillerID_In_Room;		//机组编号
	tstring strCHillerName_InRoom;	//机组标识名 
	int     iChillerID;				//库中机组标识
	int     iOF_CPR_ID;				//所属机房编号，与ChillerPlantRoom表中的CPR_ID绑定
	BOOL    bEnabled;				//是否可用
	int		iX;						//X座标
	int		iY;						//Y座标
	int     iChillerModelID;		//冷机模型ID
	int		iChillerComID;			//冷机串口通讯ID
	int		iPlcCtrl_id;			//PLC 控制的ID
	double  fScale;					//图标放大缩小系数
	double  fRotateAngel;			//图标旋转度数
	double  fSChill;
	double  fSCool;
	double  fChillerRate;			//冷机加载最大电流百分比
	int     iHeatPump;              //是否为热泵
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
