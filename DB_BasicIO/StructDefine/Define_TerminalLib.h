#pragma once

/*
Author:		Chang
Time:		2010-07-13
Comments:	��Ӧ���ݿ���-terminallib ���ڵ���������
*/
typedef struct tag_gTerminalLib
{
   int		iTerminal_id;	//�ն�ID
   int		iTerminal_type;	//�ն�����
}gTerminalLib, *pTerminalLib;

enum eTERMINALLIB
{
	TERMINALLIB_ID,
	TERMINALLIB_TYPE,
	TERMINALLIB_MAXNUM
};