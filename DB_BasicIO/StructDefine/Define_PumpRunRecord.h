#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-PumpRunRecord 表内的所有内容
*/
typedef struct tag_gPumpRunRecord
{
	SYSTEMTIME  tmTime;		//时间
	int         iPumpID;	//建筑中的PumpID,注意这不是泵本身的标识号，是Room_Contain_Pump表中的唯一标识号 
	double      dFlow;		//水量
	double      dHead;		//扬程
	double      dFreq;		//频率
	double      dPower;		//功率
	BOOL        bONOFF;		//开启关闭
	int         iOF_CPR_ID; //所属机房号
}gPumpRunRecord, *pPumpRunRecord;

enum ePUMPRUNRECORD
{
	PUMPRUNREC_TIME,
	PUMPRUNREC_PUMPID,
	PUMPRUNREC_FLOW,
	PUMPRUNREC_HEAD,
	PUMPRUNREC_FREQ,
	PUMPRUNREC_POWER,
	PUMPRUNREC_ONOFF,
	PUMPRUNREC_OF_CPR_ID,
	PUMPRUNREC_MAXNUM
};