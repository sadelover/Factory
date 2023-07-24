#pragma  once


// define the structure of the table "weather_hour_data"

typedef struct WeatherHourData
{
	long lstation_id;  // վ̨��
	double dtype;		 // ���ͱ��:0-������,1-��ֵ������,2-���伫����,3-���伫����,4-���¼�����,5-���¼�����
	double dhour;		 // ʱ��
	double dT;			 // �����¶�(��)
	double dD;			 // ��ʪ��(g/kg.dra)
	double dRT;			 // ˮƽ���ܷ���(W/m^2)
	double dRR;          // ˮƽ��ɢ�����(W/m^2)
	double dt_ground;	 // �ر��¶�(��)
	double dt_sky;		 // �����Ч�¶�(K)
	double dWS;			 // ����(m/s)
	double dWD;			 // ����:0-C,1-N,2-NNE,3-NE,4-ENE,5-E...9-S...13-W...16-NNW
	double dB;			 // ����ѹ��
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
