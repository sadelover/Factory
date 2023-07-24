#pragma once

/*
Author:		Chang
Time:		2010-07-13
Comments:	对应数据库中-terminallib 表内的所有内容
*/
typedef struct tag_gTerminalLib
{
   int		iTerminal_id;	//终端ID
   int		iTerminal_type;	//终端类型
}gTerminalLib, *pTerminalLib;

enum eTERMINALLIB
{
	TERMINALLIB_ID,
	TERMINALLIB_TYPE,
	TERMINALLIB_MAXNUM
};