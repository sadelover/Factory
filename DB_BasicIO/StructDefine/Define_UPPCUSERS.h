#pragma once

/*
Author:		juneay
Time:		2010-07-13
Comments:	对应数据库中-UPPCUSERS 表内的所有内容
*/
typedef struct tag_gUPPCUSERS
{
	int     iUserID;	 //用户ID
	tstring strUserName; //用户名
	tstring strUserPwd;  //用户密码
	int     iUserRight;  //用户权限
	
}gUPPCUSERS, *pUPPCUSERS;

enum eUPPCUSERS
{
	UPPCUSER_ID,
	UPPCUSER_NAME,
	UPPCUSER_PASSWORD,
	UPPCUSER_RIGHT,
	UPPCUSER_MAXNUM
};
