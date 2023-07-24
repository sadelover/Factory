#pragma once


/*

*/

typedef struct WaternetParam
{
	int        iWaternetId;    // waternet id.
	double     dFrequency;     // frequency.
	double     dHead;          // head.
	double     dPower;         // power.
	double     dWaterFlow;     // waterflow.
}gWaternetParam, *pWaternetParam;

enum WATERNETPARAM
{
	WP_WATERNETID,
	WP_FREQUENCY,
	WP_HEAD,
	WP_POWER,
	WP_WATERFLOW,
};




