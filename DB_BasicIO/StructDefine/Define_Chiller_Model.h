#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		Juneay
Time:		2010-07-12
Comments:	对应数据库中-Chiller_Model 表内的所有内容
*/
typedef struct tag_gCH_Model
{
	int			iChillerModelID;		//冷机模型编号
	tstring		strCHillerModelName;	//模型名称
	int			iChillerID;				//冷机标识号，与ChillerLib表id绑定
	int			iType;					//模型类型，0: 最简模型 1: 二次拟合 2: 复杂算法 
	SYSTEMTIME	tmModel_Gen_Time;		//模型生成时间
	tstring		strModel_Param;			//模型参数 所有参数以"/"为间隔存入数据库具体解析过程由模型编写人员决定，不会影响数据库设计
}gCH_Model, *pCH_Model;

enum eCH_MODEL
{
	CH_MODEL_CHILLERMODELID,
	CH_MODEL_CHILLERMODELNAME,
	CH_MODEL_CHILLERID,
	CH_MODEL_TYPE,
	CH_MODEL_GEN_TIME,
	CH_MODEL_PARAM,
	CH_MODEL_MAXNUM,
};