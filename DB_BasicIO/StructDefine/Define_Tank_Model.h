#pragma once
#include "../../Tools/StringDef.h"

/*
Author:		hua
Time:		2011-01-14
Comments:	对应数据库中-Tank_Model 表内的所有内容
*/
typedef struct tag_gTank_Model
{
	int			iTankModelID;		//冰槽模型编号
	tstring		strTankModelName;	//模型名称
	int			iTankID;			//冰槽标识号，与TankLib表id绑定
	int			iType;				//模型类型，0: 最简模型 1: 二次拟合 2: 复杂算法
	SYSTEMTIME	tmModel_Gen_Time;	//模型生成时间
	tstring		strModel_Param;		//所有参数以分号为间隔存入数据库 具体解析过程由模型编写人员决定，不会影响数据库设计
}gTank_Model, *pTank_Model;

//enum eTANK_MODEL
//{
//	PM_MODEL_TANKMODELID,
//	PM_MODEL_TANKMODELNAME,
//	PM_MODEL_TANKID,
//	//PM_MODEL_TYPE,
//	PM_MODEL_GEN_TIME,
//	PM_MODEL_PARAM,
//	PM_MODEL_MAXNUM
//};