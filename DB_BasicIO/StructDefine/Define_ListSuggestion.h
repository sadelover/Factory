#pragma once

#include "../Tools/StringDef.h"

using std::string;

/*
Author:		Chang
Time:		2010-08-26
Comments:	对应数据库中-list_suggestion 表内的所有内容
*/

#define EQM_MAX_NUM		50
typedef struct tag_gListSuggestion
{
	int		iIdent;					//编号
	string  strChillerIDList;		//冷机组合
	string  strCTIDList;				//冷却塔的组合
	string  strCWPIDList;			//冷却水泵组合
	string  strPriCHWPIDList;		//一次泵组合
	string  strSecCHWPIDList;		//二次泵组合
	string  strThiCHWPIDList;		//三次泵组合
	int	    iFunction;				//预冷，待机
	int		iOF_CPR_ID;					//机房号
}gListSuggestion, *pListSuggestion;

enum eListSuggestion
{
	ListSugg_IDENT,
	ListSugg_CH,
	ListSugg_CT,
	ListSugg_CWP,
	ListSugg_PriCHWP,
	ListSugg_SecCHWP,
	ListSugg_ThrCHWP,
	ListSugg_FuncType,
	ListSugg_OF_CPR_ID,
	ListSugg_MaxNum,
};

enum SuggestType
{
	Sugg_CoolingDown,
	Sugg_Standby,
};