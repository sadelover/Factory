#pragma once

#include "../Tools/StringDef.h"

/*
	This file defines the structure of the table Uppc_User.Users
	Author: david
	Date:   2010-8-18
*/


typedef struct UPPC_USER
{
	tstring		username;			//	�û���
	tstring		password;			//	����
	int			readright;			//	��Ȩ��
	int			writeright;			//	дȨ��
	int			modelright;			//	ģ��Ȩ��
	int			simulateright;		//	ģ��Ȩ��
	SYSTEMTIME	lastlogintime;		//  ����¼ʱ��
}gUppcSimulateUser,*pUppcSimulateUser;


