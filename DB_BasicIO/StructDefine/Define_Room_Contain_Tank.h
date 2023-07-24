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
	int iId_In_Room;			// ��room�е�id
	wstring strName_In_Room;	//��room�е�name
	int iOf_CPR_ID;				//room id
	int iX;						//x����
	int iY;						//y����
	int iTankID_In_Lib;			//lib�е�id
	//double dVolumn;              //�ݻ�
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