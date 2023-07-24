#pragma once

/*
Author:		Chang
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-room_contain_terminal ���ڵ���������
*/
typedef struct tag_gRoom_Contain_Terminal
{
	int	iTerminal_ID_InRoom;		//�������ն˵ı��
	int	iTerminal_ID;				//�ն�ID
	int	iX;							//����X
	int	iY;							//����Y
	int iOF_CPR_ID;					//�������
	double  fScale;             //ͼ��Ŵ���Сϵ��
	double  fRotateAngel;       //ͼ����ת����
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