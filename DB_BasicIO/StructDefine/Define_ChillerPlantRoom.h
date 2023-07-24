#pragma once

#include "../Tools/StringDef.h"
/*
Author:		Chang
Time:		2010-07-12
Comments:	��Ӧ�i�I���G-ChillerPlantRoom���ڵ����G���I
*/
typedef struct tag_gCHPlantRoom
{
	int		iCPR_ID;											//�������
	tstring	strCPR_NAME;										//��������
	int		iOF_PROJECT_ID;										//������Ŀ���

	double	dSetting_Max_LEWT;									//�䶳ˮ��ˮ�¶����ֵ
	double  dSetting_Min_LEWT;
	double  dSetting_Max_ECWT;
	double	dSetting_Min_ECWT;									//��ȴˮ��ˮ�¶����ֵ

	double	dSetting_Max_Cooling_DT;							//��ȴˮ�²�����S��ֵ
	double	dSetting_Max_Chilled_DT;							//ĩ�˲���ˮ�²�����S��ֵ

	double  dSetting_Min_Chiller_LoadRatio;						  //����䶳�����ɰٷֱ�
	double  dSetting_Min_Pump_FlowRatio;  						  //���ˮ�������ٷֱ�
	double  dSetting_Min_Tower_LoadRatio; 						  //�����ȴ�s���ɰٷֱ�

	double  dSetting_Sys_Min_ChillFlow;   						 //ϵͳ�䶳ˮ�������
	double  dSetting_Sys_Min_CoolFlow;    						 //ϵͳ��ȴˮ�������
	
	tstring strHVACType;                 						 //������G?
	double  dSChillOther;
	double	dSCoolOther;

	int		iRoomType;
	tstring strPumpGroup_Param;

	double  dTowerChillerCountRatio;         					//��ȴ�������̨���ı�ֵ
	double  dPumpChillerCountRatio;          					//ˮ�������̨���ı�ֵ
	bool    bRatioUseable;										 //��ȴ����ˮ�������̨���ı�ֵ�Ƿ�ʹ��

	double  dSetting_Common_LEWT;       						  //����ĳ��湩ˮ�¶�ֵ
	bool    bVSD_CoolPump;              						  //�����������ȴ���Ƿ��ѱ�Ƶ
	bool    bVSD_Pri_ChillPump;         						  //���������һ�α��Ƿ��Ѿ���Ƶ
	bool    bVSD_Sec_ChillPump;									  //��������¶��α��Ƿ��Ѿ���Ƶ
	double  dVSD_Value;                 						  //������ˮ�ñ�Ƶ���Ƶ��ֵ

	double  dSetting_CoolPump_MinPowerRatio;					 //��ȴˮ������ܺı�,��ʵ���ܺ������ʵı�ֵ
	double  dSetting_PriChillPump_MinPowerRatio;				 //һ��ˮ������ܺı�
	double  dSetting_SecChillPump_MinPowerRatio;				 //����ˮ������ܺı�
	double  dSetting_Tower_PowerIndex;							 //��Ƶ��ȴ���ܺļ���ָ��
	
	double  dSetting_dH0;		                                 //�����ֵ 	                             	
	double  dSetting_dChiller_K;								 //���ͨһKֵ
	double  dSetting_dCT_MinHz;									 //��ȴ�����Ƶ��
	double  dSetting_dCT_eff_Correction;						 //��ȴ��Ч������ֵ

	//�ֳ��Ӽ�����жϿ��Ʋ���
	double			dSetting_dElectricPercent;					//�ӻ�����ʱ��������ٷֱ�����
	double          dSetting_dApproachT_Add;					//�ӻ�����ʱ�����¶�(�����ˮ�¶��빩ˮ�¶��趨ֵ֮��)
	double          dSetting_dApproachT_Subtract;				//��������ʱ�����¶�
	double          dSetting_dDiffFlow;							//�ɽ��мӼ���ʱ������������

	double          dSetting_dVSD_Value_PRI;                               //��������£�һ�αñ�Ƶ
	double          dSetting_dVSD_Value_SEC;                               //��������£�һ�αñ�Ƶ
	double          dSetting_dVSD_Value_CT;                                //��������£���ȴ����Ƶ
	bool			dSetting_bVSD_CoolingTower;                            //��������£���ȴ���Ƿ�

	double			dSetting_fChillPumpChiller_CountRatio;      		   //�䶳�������̨���ı�ֵ
	bool            dSetting_bCT_OutT_Control;                             //�Ƿ�ʹ�ó���ˮ�¿���
	bool            dSetting_bDeltaT_Control;                              //�Ƿ�ʹ���²����
	double          dSetting_dDeltaT_Chill;                                //��������µ��䶳ˮ�²�
	double          dSetting_dDeltaT_Cool;                                 //��������µ���ȴˮ�²�

}gCHPlantRoom, *pCHPlantRoom;

enum eCHPLANTRM
{
	CHPLANTRM_CPR_ID,
	CHPLANTRM_CPR_NAME,
	CHPLANTRM_OF_PROJECT_ID,
	CHPLANTRM_SETMAX_LEWT,
	CHPLANTRM_SETMIN_LEWT,
	CHPLANTRM_SETMAX_ECWT,
	CHPLANTRM_SETMIN_ECWT,
	CHPLANTRM_SETMAX_COOLING_DT,
	CHPLANTRM_SETMAX_CH_DT,
	CHPLANTRM_MIN_CHILLER_LR,
	CHPLANTRM_MIN_PUMP_LR,
	CHPLANTRM_MIN_TOWER_LR,
	CHPLANTRM_SYS_MIN_CHILLFLOW,
	CHPLANTRM_SYS_MIN_COOLFLOW,
	CHPLANTRM_HVACTYPE,
	CHPLANTRM_SCHILL,
	CHPLANTRM_SCOOL,
	CHPLANTRM_ROOMTYPE,
	CHPLANTRM_PUMPGROUP_PARAM,
	CHPLANTRM_TOWER_CHILLER_RATIO,
	CHPLANTRM_PUMP_CHILLER_RATIO,
	CHPLANTRM_RATIOUAEABLE,
	CHPLANTRM_SETCOMMON_LEWT,
	CHPLANTRM_VSD_COOLPUMP,
	CHPLANTRM_VSD_PRICHILLPUMP,
	CHPLANTRM_VSD_SECCHILLPUMP,
	CHPLANTRM_VSD_VALUE,
	CHPLANTRM_COOLPUMP_MINPOWERRATIO,
	CHPLANTRM_PRICHILLPUMP_MINPOWERRATIO,
	CHPLANTRM_SECCHILLPUMP_MINPOWERRATIO,
	CHPLANTRM_TOWER_POWERINDEX,
	CHPLANTRM_dSetting_dH0,		        
	CHPLANTRM_dSetting_dChiller_K,		
	CHPLANTRM_dSetting_dCT_MinHz,		
	CHPLANTRM_dSetting_dCT_eff_Correction,
	CHPLANTRM_dSetting_dElectricPercent,	
	CHPLANTRM_dSetting_dApproachT_Add,	
	CHPLANTRM_dSetting_dApproachT_Subtract,
	CHPLANTRM_dSetting_dDiffFlow,
	CHPLANTRM_dSetting_dVSD_Value_PRI,             
	CHPLANTRM_dSetting_dVSD_Value_SEC,             
	CHPLANTRM_dSetting_dVSD_Value_CT,              
	CHPLANTRM_dSetting_bVSD_CoolingTower,          
	CHPLANTRM_dSetting_fChillPumpChiller_CountRatio,
	CHPLANTRM_dSetting_bCT_OutT_Control,           
	CHPLANTRM_dSetting_bDeltaT_Control,            
	CHPLANTRM_dSetting_dDeltaT_Chill,              
	CHPLANTRM_dSetting_dDeltaT_Cool,               
	CHPLANTRM_MAXNUM,
};