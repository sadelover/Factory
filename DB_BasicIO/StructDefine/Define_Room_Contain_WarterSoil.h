#ifndef DEFINE_ROOM_CONTAIN_WARTERSOIL_H__
#define DEFINE_ROOM_CONTAIN_WARTERSOIL_H__

/*
 This header file defines the columns of the table room_contain_wartersoil.
	
 Author: David
 Date: 2010/9/26
*/

typedef struct tag_Room_Contain_WaterSoil 
{
	int iId;				//在机房的id
	int iType;				//类型
	double dTemp_Winter;	//冬季常温度
	double dTemp_Summer;	//夏季常温度
	double dTGrade;			//温度梯度
	int iOf_CPR_ID;			//机房ID
	int iX;					//x坐标
	int iY;					//Y坐标
	int iWaterSoilID;       //地埋管属性ID
	int iWSModelID;         //地埋管模型id
}WaterSoil_Entry_InRoom, *pWaterSoil_Entry_InRoom;

enum ROOM_CONTAIN_WATERSOIL
{
	ROOM_CONTAIN_WATERSOIL_ID,
	ROOM_CONTAIN_WATERSOIL_TYPE,
	ROOM_CONTAIN_WATERSOIL_TEMP_WINTER,
	ROOM_CONTAIN_WATERSOIL_TEMP_SUMMER,
	ROOM_CONTAIN_WATERSOIL_TGRADE,
	ROOM_CONTAIN_WATERSOIL_OF_CPR_ID,
	ROOM_CONTAIN_WATERSOIL_X,
	ROOM_CONTAIN_WATERSOIL_Y
};


#endif

