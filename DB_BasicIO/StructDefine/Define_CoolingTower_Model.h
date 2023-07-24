#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-CoolingTower_Model 表内的所有内容
*/
typedef struct tag_gCT_Model
{
   int			iModelID;			//冷却塔模型编号
   tstring		strModelName;		//模型名称
   int			iCoolingTowerID;	//冷却塔标识号,与CoolingTower表id绑定
   int			iType;				//模型类型，0: 最简模型1: 二次拟合2: 复杂算法
   SYSTEMTIME	tmModel_Gen_Time;	//模型生成时间
   tstring		strModel_Param;		//所有参数以分号为间隔存入数据库 具体解析过程由模型编写人员决定，不会影响数据库设计
}gCT_Model, *pCT_Model;

enum eCT_MODEL
{
	CT_MODEL_ID,
	CT_MODEL_NAME,
	CT_MODEL_CTID,
	CT_MODEL_TYPE,
	CT_MODEL_GEN_TIME,
	CT_MODEL_PARAM,
	CT_MODEL_MAXNUM
};