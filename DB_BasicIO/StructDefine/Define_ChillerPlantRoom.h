#pragma once

#include "../Tools/StringDef.h"
/*
Author:		Chang
Time:		2010-07-12
Comments:	对应蔵綢库諫-ChillerPlantRoom表内的所覩内菼
*/
typedef struct tag_gCHPlantRoom
{
	int		iCPR_ID;											//机房编号
	tstring	strCPR_NAME;										//机房名称
	int		iOF_PROJECT_ID;										//所属项目编号

	double	dSetting_Max_LEWT;									//冷冻水出水温度最高值
	double  dSetting_Min_LEWT;
	double  dSetting_Max_ECWT;
	double	dSetting_Min_ECWT;									//冷却水回水温度最低值

	double	dSetting_Max_Cooling_DT;							//冷却水温差最高蟂定值
	double	dSetting_Max_Chilled_DT;							//末端侧冷水温差最高蟂定值

	double  dSetting_Min_Chiller_LoadRatio;						  //最低冷冻机负荷百分比
	double  dSetting_Min_Pump_FlowRatio;  						  //最低水泵流量百分比
	double  dSetting_Min_Tower_LoadRatio; 						  //最低冷却藄负荷百分比

	double  dSetting_Sys_Min_ChillFlow;   						 //系统冷冻水最低流量
	double  dSetting_Sys_Min_CoolFlow;    						 //系统冷却水最低流量
	
	tstring strHVACType;                 						 //机房类G?
	double  dSChillOther;
	double	dSCoolOther;

	int		iRoomType;
	tstring strPumpGroup_Param;

	double  dTowerChillerCountRatio;         					//冷却塔与冷机台数的比值
	double  dPumpChillerCountRatio;          					//水泵与冷机台数的比值
	bool    bRatioUseable;										 //冷却塔、水泵与冷机台数的比值是否使用

	double  dSetting_Common_LEWT;       						  //冷机的常规供水温度值
	bool    bVSD_CoolPump;              						  //常规控制下冷却泵是否已变频
	bool    bVSD_Pri_ChillPump;         						  //常规控制下一次泵是否已经变频
	bool    bVSD_Sec_ChillPump;									  //常规控制下二次泵是否已经变频
	double  dVSD_Value;                 						  //常规下水泵变频后的频率值

	double  dSetting_CoolPump_MinPowerRatio;					 //冷却水泵最低能耗比,即实际能耗与额定功率的比值
	double  dSetting_PriChillPump_MinPowerRatio;				 //一次水泵最低能耗比
	double  dSetting_SecChillPump_MinPowerRatio;				 //二次水泵最低能耗比
	double  dSetting_Tower_PowerIndex;							 //变频冷却塔能耗计算指数
	
	double  dSetting_dH0;		                                 //设计焓值 	                             	
	double  dSetting_dChiller_K;								 //冷机通一K值
	double  dSetting_dCT_MinHz;									 //冷却塔最低频率
	double  dSetting_dCT_eff_Correction;						 //冷却塔效率修正值

	//现场加减冷机判断控制参数
	double			dSetting_dElectricPercent;					//加机控制时冷机电流百分比上限
	double          dSetting_dApproachT_Add;					//加机控制时趋近温度(冷机供水温度与供水温度设定值之差)
	double          dSetting_dApproachT_Subtract;				//减机控制时趋近温度
	double          dSetting_dDiffFlow;							//可进行加减机时流量波动上限

	double          dSetting_dVSD_Value_PRI;                               //常规控制下，一次泵变频
	double          dSetting_dVSD_Value_SEC;                               //常规控制下，一次泵变频
	double          dSetting_dVSD_Value_CT;                                //常规控制下，冷却塔变频
	bool			dSetting_bVSD_CoolingTower;                            //常规控制下，冷却塔是否

	double			dSetting_fChillPumpChiller_CountRatio;      		   //冷冻泵与冷机台数的比值
	bool            dSetting_bCT_OutT_Control;                             //是否使用出塔水温控制
	bool            dSetting_bDeltaT_Control;                              //是否使用温差控制
	double          dSetting_dDeltaT_Chill;                                //常规控制下的冷冻水温差
	double          dSetting_dDeltaT_Cool;                                 //常规控制下的冷却水温差

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