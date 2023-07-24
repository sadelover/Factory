#ifndef ROOM_CONTAIN_TANK_H__
#define ROOM_CONTAIN_TANK_H__

#include <string>
using  std::wstring;

/*
 This header files defines the table structure Room_Contain_Tank;

 Author: David
 Date  : 2010/9/26
*/


typedef struct tag_Room_Contain_Tank 
{
	int iId_In_Room;			// 在room中的id
	wstring strName_In_Room;	//在room中的name
	int iOf_CPR_ID;				//room id
	int iX;						//x坐标
	int iY;						//y坐标
	int iTankID_In_Lib;			//lib中的id
	//double dVolumn;              //容积
	//double dCapacity;            //
}Tank_Entry_InRoom, *pTank_Entry_InRoom;

enum Room_Contain_Tank
{
	Room_Contain_Tank_ID_IN_ROOM,
	ROOM_CONTAIN_TANK_NAME,
	ROOM_CONTAIN_TANK_OF_CPR_ID,
	ROOM_CONTAIN_TANK_X,
	ROOM_CONTAIN_TANK_Y,
	ROOM_CONTAIN_TANK_TANKID,	
	ROOM_CONTAIN_TANK_ENDCOLUMN,
};


#endif