#pragma once

/*
Author:		juneay
Time:		2010-07-13
Comments:	��Ӧ���ݿ���-UPPCUSERS ���ڵ���������
*/
typedef struct tag_gUPPCUSERS
{
	int     iUserID;	 //�û�ID
	tstring strUserName; //�û���
	tstring strUserPwd;  //�û�����
	int     iUserRight;  //�û�Ȩ��
	
}gUPPCUSERS, *pUPPCUSERS;

enum eUPPCUSERS
{
	UPPCUSER_ID,
	UPPCUSER_NAME,
	UPPCUSER_PASSWORD,
	UPPCUSER_RIGHT,
	UPPCUSER_MAXNUM
};
