#pragma once

#include "../Tools/StringDef.h"

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	对应数据库中-ChillerRunRecord 表内的所有内容
*/
typedef struct tag_gCHRunRecord
{
	tag_gCHRunRecord()
		: iChillerIDInRoom(0)
	,dEvapFlow(0)
	,dCondFLow(0)
	,dPower(0)
	,iMODE(0)           //0
	,iSTATUS(0)		    //1
	,iSYS_ALM(0)	    //2
	, 	   iCHIL_S_S(0)		//3
	,	   dLCW_STPT(0)		//4
	,   dC_HRS(0)			//5
	,	   dAMPS_Ratio(0)		//6
	,	   dCMPD(0)			//7
	,     dECW(0)		//8
	,     dLCW(0)			//9
	, 	   dERT(0)			//10
	,     dERP(0)			//11
	, 	   dECDW(0)			//12
	,     dLCDW	(0)		//13
	,	   dCRT	(0)		//14
	,	   dCRP	(0)		//15
	,	   dAMPS_A(0)		    //16
	,     dVOLT_P  (0)		//17
	,     dVOLT_A (0)    	//18
	,        iOF_CPR_ID (0)//所属机房号
	{
	}
	SYSTEMTIME stTime;
	int        iChillerIDInRoom;
	double     dEvapFlow;
	double     dCondFLow;
	double     dPower;
	int        iMODE;           //0
	int    	   iSTATUS;		    //1
	int   	   iSYS_ALM;	    //2
	int 	   iCHIL_S_S;		//3
	double	   dLCW_STPT;		//4
	double	   dC_HRS;			//5
	double	   dAMPS_Ratio;		//6
	double	   dCMPD;			//7
	double     dECW;			//8
	double     dLCW;			//9
	double 	   dERT;			//10
	double     dERP;			//11
	double 	   dECDW;			//12
	double     dLCDW;			//13
	double	   dCRT;			//14
	double	   dCRP;			//15
	double	   dAMPS_A;		    //16
	double     dVOLT_P;  		//17
	double     dVOLT_A;     	//18
	int        iOF_CPR_ID; //所属机房号
}gCHRunRecord, *pCHRunRecord;

enum eCHRUNREC
{
	CHRUNREC_TIME,
	CHRUNREC_ChillerIDInRoom,
	CHRUNREC_EvapFlow,
	CHRUNREC_CondFLow,
	CHRUNREC_Power,
	CHRUNREC_MODE,          //0
	CHRUNREC_STATUS,		//1
	CHRUNREC_SYS_ALM,	    //2
	CHRUNREC_CHIL_S_S,		//3
	CHRUNREC_LCW_STPT,		//4
	CHRUNREC_C_HRS,			//5
	CHRUNREC_AMPS_Ratio,	//6
	CHRUNREC_CMPD,			//7
	CHRUNREC_ECW,			//8
	CHRUNREC_LCW,			//9
	CHRUNREC_ERT,			//10
	CHRUNREC_ERP,			//11
	CHRUNREC_ECDW,			//12
	CHRUNREC_LCDW,			//13
	CHRUNREC_CRT,			//14
	CHRUNREC_CRP,			//15
	CHRUNREC_AMPS_A,		//16
	CHRUNREC_VOLT_P,  		//17
	CHRUNREC_VOLT_A,     	//18
	CHRUNREC_OF_CPR_ID,
	CHRUNREC_MAXNUM
};