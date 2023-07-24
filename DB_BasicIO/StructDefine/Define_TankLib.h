#pragma once

#include <string>

using std::wstring;
/*
 This header defines the table structure Tanklib
*/

typedef struct tag_TankLib
{
	int		iTankID;			// id
	int		iType;				//类型
	wstring strBrand;			//品牌	
	double  dVolumn;			//体积
	double  dCapacity;			//容量
	wstring strModelName;		//型号名字
}TankLibEntry, *pTankLibEntry;


enum TANKLIB
{
	TANKLIB_ID,
	TANKLIB_TYPE,
	TANKLIB_BRAND,
	TANKLIB_VOLUMN,
	TANKLIB_CAPACITY,
	TANKLIB_MODELNAME,
};



