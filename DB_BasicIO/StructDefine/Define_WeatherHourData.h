#pragma  once


// define the structure of the table "weather_hour_data"

typedef struct WeatherHourData
{
	long lstation_id;  // 站台号
	double dtype;		 // 类型编号:0-典型年,1-焓值极高年,2-辐射极低年,3-辐射极高年,4-气温极低年,5-气温极高年
	double dhour;		 // 时刻
	double dT;			 // 干球温度(℃)
	double dD;			 // 含湿量(g/kg.dra)
	double dRT;			 // 水平面总辐射(W/m^2)
	double dRR;          // 水平面散射辐射(W/m^2)
	double dt_ground;	 // 地表温度(℃)
	double dt_sky;		 // 天空有效温度(K)
	double dWS;			 // 风速(m/s)
	double dWD;			 // 风向:0-C,1-N,2-NNE,3-NE,4-ENE,5-E...9-S...13-W...16-NNW
	double dB;			 // 大气压力
}gWeatherHourData, *pWeatherHourData;

enum WEATHERHOURDATA
{
	WH_STATION_ID,		// 0
	WH_TYPE,			// 1
	WH_HOUR,			// 2
	WH_T,				// 3
	WH_D,				// 4
	WH_RT,				// 5
	WH_RR,				// 6
	WH_T_GROUND,		// 7
	WH_T_SKY,			// 8
	WH_WS,				// 9
	WH_WD,				// 10
	WH_B,				// 11
};
