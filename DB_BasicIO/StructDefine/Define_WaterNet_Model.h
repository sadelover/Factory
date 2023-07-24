#pragma once
#include "../../Tools/StringDef.h"

/*
define the data structure  for the table waternetmodel.
*/

typedef struct tag_gWaterNet_Model
{
	int         iModelID; //
	tstring     strModelName;
	int         iType;
	SYSTEMTIME  tmModel_Gen_Time;
	tstring     strModel_Param;
}gWaterNet_Model, *pWaterNet_Model;

enum eWaterNet_MODEL
{
	WM_MODELID,
	WM_MODELNAME,
	WM_MODELTYPE,
	WM_MODELGENTIME,
	WM_MODELPARAM
};





