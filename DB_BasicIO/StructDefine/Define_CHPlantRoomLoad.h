#pragma once

#include "../../Tools/StringDef.h"

/*
Author:		Chang
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Chiller Plant Room Load Define���ڵ���������
*/
class gRoomLoad
{
public:
    gRoomLoad()
        :dCoolingLoad(0)
        ,dHeatingLoad(0)	//�ȸ���
        ,dTotal_Flow(0)		//���䶳ˮ��
        ,dTotal_T_Supply(0)	//�ܹ�ˮ�¶�
        ,dTotal_T_Return(0)	//�ܻ�ˮ�¶�
        , iRoomID(0)
        ,dTdry(0)
        ,dTwet(0)
		,fPowerBill(0)
    {}
	SYSTEMTIME	tmTime;				//ʱ��
	double		dCoolingLoad;		//�为��
	double      dCoolingLoad1;		
	double      dCoolingLoad2;
	double      dCoolingLoad3;
	double      dCoolingLoad4;
	double		dHeatingLoad;		//�ȸ���
	double		dTotal_Flow;		//���䶳ˮ��
	double		dTotal_T_Supply;	//�䶳��ˮ�¶�
	double		dTotal_T_Return;	//��ȴ��ˮ�¶�
	int         iRoomID;
	double	    dTdry;
	double		dTwet;
	double      fPowerBill;
};

enum eRMLOAD
{
	RMLOAD_TIME,
	RMLOAD_COOLINGLOAD,
	RMLOAD_HEATINGLOAD,
	RMLOAD_TOTAL_FLOW,
	RMLOAD_TOTAL_T_SUPPLY,
	RMLOAD_TOTAL_T_RETURN,
	RMLOAD_ROOMID,
	RMLOAD_TDRY,
	RMLOAD_TWET,
	RMLOAD_PB,
	RMLOAD_COOLINGLOAD1,
	RMLOAD_COOLINGLOAD2,
	RMLOAD_COOLINGLOAD3,
	RMLOAD_COOLINGLOAD4,
	RMLOAD_MAXNUM
};