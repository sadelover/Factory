#pragma once

#include "../Tools/StringDef.h"

using std::string;

/*
Author:		Chang
Time:		2010-08-26
Comments:	��Ӧ���ݿ���-list_suggestion ���ڵ���������
*/

#define EQM_MAX_NUM		50
typedef struct tag_gListSuggestion
{
	int		iIdent;					//���
	string  strChillerIDList;		//������
	string  strCTIDList;				//��ȴ�������
	string  strCWPIDList;			//��ȴˮ�����
	string  strPriCHWPIDList;		//һ�α����
	string  strSecCHWPIDList;		//���α����
	string  strThiCHWPIDList;		//���α����
	int	    iFunction;				//Ԥ�䣬����
	int		iOF_CPR_ID;					//������
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