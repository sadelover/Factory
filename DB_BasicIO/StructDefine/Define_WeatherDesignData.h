#pragma once

// define the structure of the table weather_design_data 

#include "../Tools/StringDef.h"

typedef struct Weather_Design_Data
{
	long	lstation_id;			//վ̨��
	double	dw_t_heating;			//������ů��������¶�(��)
	double	dw_t_vent;				//����ͨ����������¶�(��)
	double	dw_t_ac;				//��������������������¶�(��)
	double	dw_rh_ac;				//����������������������ʪ��(%)
	double	dw_ws_mean;				//��������ƽ������(m/s)
	double	dw_ws_meanmain;			//���������������ƽ������(m/s)
	tstring strw_wd_main;			//����������
	double	dw_f_wd_main;			//����������Ƶ��(%)
	double	dw_p_air;				//�����������ѹ��(Pa)
	double  dw_s_ratio;				//�������հٷ���(%)
	double  dw_date_heating;		//��Ƽ����ò�ů������
	double  dw_date_heating_start;	//��Ƽ����ò�ů�ڳ���
	double  dw_date_heating_end;	//��Ƽ����ò�ů������
	double	ds_t_vent;				//�ļ�ͨ����������¶�(��)
	double  ds_rh_vent;				//�ļ�ͨ������������ʪ��(%
	double  ds_t_ac;				//�ļ��������������������¶�(��)
	double  ds_ts_ac;				//�ļ����������������ʪ���¶�(��)
	double  ds_t_mean_ac;			//�ļ������������������ƽ���¶�(��)
	double  ds_ws_mean;				//�ļ�����ƽ������(m/s)
	tstring strs_wd_main;			//�ļ�������
	double	ds_f_wd_main;			//�ļ�������Ƶ��(%)
	double  ds_p_air;				//�ļ��������ѹ��(Pa)
	double  dt_min;					//��������¶�(��)
	double  dt_max;					//��������¶�(��)
	tstring	stry_wd_main;			//ȫ��������
	double  dy_f_wd_main;			//ȫ��������Ƶ��(%)
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







