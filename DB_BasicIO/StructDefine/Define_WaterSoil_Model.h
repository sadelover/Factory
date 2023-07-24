#pragma once
#include "../../Tools/StringDef.h"

/*
Author:		huafengyou
Time:		2011-08-24
Comments:	对应数据库中-WaterSoil_Model 表内的所有内容
*/
typedef struct tag_gWaterSoil_Model
{
	int			iWaterSoilModelID;		//地埋管模型编号
	tstring		strWaterSoilModelName;		//模型名称
	int			iWaterSoilID;			//地埋管标识号
	int			iType;				//模型类型，0: 最简模型 1: 二次拟合 2: 复杂算法
	SYSTEMTIME	tmModel_Gen_Time;	//模型生成时间
	tstring		strModel_Param;		//所有参数以分号为间隔存入数据库 具体解析过程由模型编写人员决定，不会影响数据库设计
}gWaterSoil_Model, *pWaterSoil_Model;

enum eWATERSOIL_MODEL
{
	WS_MODEL_WATERSOILMODELID,
	WS_MODEL_WATERSOILMODELNAME,
	WS_MODEL_WATERSOILID,
	WS_MODEL_TYPE,
	WS_MODEL_GEN_TIME,
	WS_MODEL_PARAM,
	WS_MODEL_MAXNUM
};