

#ifndef DEFINE_ROOM_CONTAIN_POWERMETER_H__
#define DEFINE_ROOM_CONTAIN_POWERMETER_H__

#include "../Tools/StringDef.h"

typedef struct tagRoomContainPowerMeterEntry
{
	int	id_in_room;			//	id in room
	int room_id;			//	roomid
	int xpos;				//	xpos
	int ypos;				//	ypos
	
	int PLCID;				//	plc id
	int ComID;				//	com id
	
	double	scale;			// scale
	double	rotateangle;	// rotateangle.
	
	tstring name_s;			// name_s.
}RoomContainPowerMeterEntry, *pRoomContainPowerMeterEntry;

enum ERoomContainPowerMeterEntry
{
	RCPM_ID_IN_ROOM,
	RCPM_ROOM_ID,
	RCPM_X_POS,
	RCPM_Y_POS,
	RCPM_PLCID,
	RCPM_COMID,
	RCPM_SCALE,
	RCPM_ROTATEANGLE,
	RCPM_NAME_S,
	RCPM_MAXNUM
};











#endif