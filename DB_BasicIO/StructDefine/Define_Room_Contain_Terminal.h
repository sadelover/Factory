#pragma once

/*
Author:		Chang
Time:		2010-07-12
Comments:	对应数据库中-room_contain_terminal 表内的所有内容
*/
typedef struct tag_gRoom_Contain_Terminal
{
	int	iTerminal_ID_InRoom;		//机房中终端的编号
	int	iTerminal_ID;				//终端ID
	int	iX;							//座标X
	int	iY;							//座标Y
	int iOF_CPR_ID;					//机房编号
	double  fScale;             //图标放大缩小系数
	double  fRotateAngel;       //图标旋转度数
	double fSChill;
}gRoom_Contain_Terminal, *pRoom_Contain_Terminal;

typedef enum eROOM_CONTAIN_TERMINAL
{
	RM_C_TERMINAL_ID_INROOM = 0x00
	, RM_C_TERMINAL_ID
	, RM_C_TERMINAL_X
	, RM_C_TERMINAL_Y
	, RM_C_TERMINALOF_CPR_ID
	, RM_C_TERMINALMAXNUM
}eRoomContainT;