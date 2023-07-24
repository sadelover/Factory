#pragma once

#include "../Tools/StringDef.h"

/*
Author:		Chang
Time:		2010-07-12
Comments:	对应数据库中-Project表内的所有内容
*/
typedef struct tag_gProject
{
	int		iProjectID;			//项目唯一编号
	tstring	strName;			//项目名称
	tstring	strLocation;		//地址
	int		iProjectType;		//建筑类型,0: 办公室	1: 商场	2: 酒店	3: 综合商业建筑
	int		iWeatherStation;	//所属气象站编号
	double	dFloorArea;			//建筑面积
	sstring	strPicture;			//外观图, 此为二进制图片内容
	tstring	strMemo;			//备注
	tstring	strCreator;			//创建者
}gProject, *pProject;

enum ePROJECT
{
	PROJ_PROJECTID,
	PROJ_NAME,
	PROJ_LOCATION,
	PROJ_PROJECTTYPE,
	PROJ_WEATHERSTATION,
	PROJ_FLOORAREA,
	PROJ_PICTURE,
	PROJ_MEMO,
	PROJ_CREATOR,
	PROJ_MAXNUM
};