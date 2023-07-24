#ifndef DEFINE_ROOM_CONTAIN_WARTERSOIL_H__
#define DEFINE_ROOM_CONTAIN_WARTERSOIL_H__

/*
 This header file defines the columns of the table room_contain_wartersoil.
	
 Author: David
 Date: 2010/9/26
*/

typedef struct tag_Room_Contain_WaterSoil 
{
	int iId;				//�ڻ�����id
	int iType;				//����
	double dTemp_Winter;	//�������¶�
	double dTemp_Summer;	//�ļ����¶�
	double dTGrade;			//�¶��ݶ�
	int iOf_CPR_ID;			//����ID
	int iX;					//x����
	int iY;					//Y����
	int iWaterSoilID;       //���������ID
	int iWSModelID;         //�����ģ��id
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

