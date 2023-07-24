#pragma once

// define the structure of the table weather_design_data 

#include "../Tools/StringDef.h"

typedef struct Weather_Design_Data
{
	long	lstation_id;			//站台号
	double	dw_t_heating;			//冬季采暖室外计算温度(℃)
	double	dw_t_vent;				//冬季通风室外计算温度(℃)
	double	dw_t_ac;				//冬季空气调节室外计算温度(℃)
	double	dw_rh_ac;				//冬季空气调节室外计算相对湿度(%)
	double	dw_ws_mean;				//冬季室外平均风速(m/s)
	double	dw_ws_meanmain;			//冬季室外最多风向的平均风速(m/s)
	tstring strw_wd_main;			//冬季最多风向
	double	dw_f_wd_main;			//冬季最多风向频率(%)
	double	dw_p_air;				//冬季室外大气压力(Pa)
	double  dw_s_ratio;				//冬季日照百分率(%)
	double  dw_date_heating;		//设计计算用采暖期日数
	double  dw_date_heating_start;	//设计计算用采暖期初日
	double  dw_date_heating_end;	//设计计算用采暖期终日
	double	ds_t_vent;				//夏季通风室外计算温度(℃)
	double  ds_rh_vent;				//夏季通风室外计算相对湿度(%
	double  ds_t_ac;				//夏季空气调节室外计算干球温度(℃)
	double  ds_ts_ac;				//夏季空气调节室外计算湿球温度(℃)
	double  ds_t_mean_ac;			//夏季空气调节室外计算日平均温度(℃)
	double  ds_ws_mean;				//夏季室外平均风速(m/s)
	tstring strs_wd_main;			//夏季最多风向
	double	ds_f_wd_main;			//夏季最多风向频率(%)
	double  ds_p_air;				//夏季室外大气压力(Pa)
	double  dt_min;					//极端最低温度(℃)
	double  dt_max;					//极端最高温度(℃)
	tstring	stry_wd_main;			//全年最多风向
	double  dy_f_wd_main;			//全年最多风向频率(%)
}gWeatherDesignData, *pWeatherDesignData;

enum WEATHER_DESIGN_DATA
{
	WD_STATION_ID,			//0
	W_T_HEATING,		//1
	W_T_VENT,			//2
	W_T_AC,				//3
	W_RH_AC,			//4
	W_WS_MEAN,			//5
	W_WS_MEAN_MAIN,		//6
	W_WD_MAIN,			//7
	W_F_WD_MAIN,		//8
	W_P_AIR,			//9
	W_S_RATIO,			//10
	W_DAYS_HEATING,		//11
	W_DATE_HEATING_START,	//12
	W_DATE_HEATING_END,		//13
	S_T_VENT,				//14
	S_RH_VENT,				//15
	S_T_AC,					//16
	S_TS_AC,				//17
	S_T_MEAN_AC,			//18
	S_WS_MEAN,				//19
	S_WD_MAIN,				//20
	S_F_WD_MAIN,			//21
	S_P_AIR,				//22
	T_MIN,					//23
	T_MAX,					//24
	Y_WD_MAIN,				//25
	Y_F_WD_MAIN,		//26
};







