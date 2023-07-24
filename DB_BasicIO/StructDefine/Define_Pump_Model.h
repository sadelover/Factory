#pragma once
#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-Pump_Model 表内的所有内容
*/
typedef struct tag_gPump_Model
{
   int			iPumpModelID;		//水泵模型编号
   tstring		strPumpModelName;		//模型名称
   int			iPumpID;			//水泵标识号，与ChillerLib表id绑定
   int			iType;				//模型类型，0: 最简模型 1: 二次拟合 2: 复杂算法
   SYSTEMTIME	tmModel_Gen_Time;	//模型生成时间
   tstring		strModel_Param;		//所有参数以分号为间隔存入数据库 具体解析过程由模型编写人员决定，不会影响数据库设计
}gPump_Model, *pPump_Model;

enum ePUMP_MODEL
{
	PM_MODEL_PUMPMODELID,
	PM_MODEL_PUMPMODELNAME,
	PM_MODEL_PUMPID,
	PM_MODEL_TYPE,
	PM_MODEL_GEN_TIME,
	PM_MODEL_PARAM,
	PM_MODEL_MAXNUM
};