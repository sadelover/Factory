#pragma once


/*
 define the structure of the table. room_contain_waternet
*/

typedef struct Room_Contain_Waternet
{
	int iWaternetId;
	tstring strPumpGroup;
	tstring strTowerGroup;
	tstring strChillerGroup;
	int iOF_CPR_ID;
	int iWaternetModelID;
	int iChillOrCool;
}gRoomContainWaternet, *pRoomContainWaternet;


enum ROOM_CONTAIN_WATERNET
{
	RM_C_WID,
	WM_C_PUMPGROUP,
	WM_C_TOWERGROUP,
	WM_C_CHILLERGROUP,
	WM_C_OF_CPR_ID,
	WM_C_WATERNETMODELID,
	WM_C_CHILLORCOOL,
};





