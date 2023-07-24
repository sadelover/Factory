#ifndef UPPC_DBBASICIO_ACTIONINFO_DEFINE_H_
#define UPPC_DBBASICIO_ACTIONINFO_DEFINE_H_


/********************************************************************************
*ActionInfo_Define.h - declare the class ActionInfo_Define
*
*       Copyright (c) WeldTeh Corporation. All rights reserved.
*
*Purpose:
*		define the struct which used to save the ActionInfo.
*		 such as infor, actionName,
*		status and so on.
*     
*
*Note:
*       This class will not be exported.
*       
*
*Author:
*      sany
*******************************************************************************/
enum actInfoStatus
{
	ready = 0,
	running,
	success,
	fail
};


// it contains actioninfo struct data. 
typedef struct tag_ActinInfo_Record
{
	wstring				info;					// action information
	wstring				actionname;	// action name
	actInfoStatus		status;					// action status.
} gActinInfo_Record, *pActinInfo_Record;

#endif