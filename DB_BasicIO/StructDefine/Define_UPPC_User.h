#pragma once

#include "../Tools/StringDef.h"

/*
	This file defines the structure of the table Uppc_User.Users
	Author: david
	Date:   2010-8-18
*/


typedef struct UPPC_USER
{
	tstring		username;			//	用户名
	tstring		password;			//	密码
	int			readright;			//	读权限
	int			writeright;			//	写权限
	int			modelright;			//	模型权限
	int			simulateright;		//	模拟权限
	SYSTEMTIME	lastlogintime;		//  最后登录时间
}gUppcSimulateUser,*pUppcSimulateUser;


