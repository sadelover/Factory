#pragma once

#include "../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-Room_Contain_Pipe 表内的所有内容
*/
typedef struct tag_gRoom_Contain_Pipe
{
	int     iPipeID_In_Room;	//机房中管道编号
	tstring strPipeName_InRoom;	//管道标识名 
	int     iPipeID;			//库中管道标识
	int     iOF_CPR_ID;			//所属机房编号，与ChillerPlantRoom表中的CPR_ID绑定
	BOOL    bEnabled;			//是否可用
	int		from_service_id;	//管道连接起点设备ID
	int		from_service_type;	//管道连接起点设备类型
	int		from_service_port;	//管道连接起点设备端口
	int		to_service_id;		//管道连接终点设备ID
	int		to_service_type;	//管道连接终点设备类型
	int		to_service_port;	//管道连接终点设备端口
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