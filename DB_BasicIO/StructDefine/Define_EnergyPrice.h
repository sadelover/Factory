#pragma once

#include "../Tools/StringDef.h"

/*
Author:		Chang
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-EnergyPrice���ڵ���������
*/
typedef struct tag_gEnergyPrice
{
	SYSTEMTIME	tmFromTime;		//ָ��ʱ�����
	SYSTEMTIME	tmToTime;		//ָ��ʱ���յ�
	double		dElecPrice;		//ÿ�ȵ��
	double		dGasPrice;		//ÿm3����
	tstring		strUnit;		//��λ��RBM,USD
}gEnergyPrice, *pEnergyPrice;

enum eENERGYPRICE
{
	ENPRICE_FROMTIME,
	ENPRICE_TOTIME,
	ENPRICE_ELECPRICE,
	ENPRICE_GASPRICE,
	ENPRICE_UNIT,
	ENPRICE_MAXNUM
};