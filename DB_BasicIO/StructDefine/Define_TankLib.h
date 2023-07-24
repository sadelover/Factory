#pragma once

#include <string>

using std::wstring;
/*
 This header defines the table structure Tanklib
*/

typedef struct tag_TankLib
{
	int		iTankID;			// id
	int		iType;				//����
	wstring strBrand;			//Ʒ��	
	double  dVolumn;			//���
	double  dCapacity;			//����
	wstring strModelName;		//�ͺ�����
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



