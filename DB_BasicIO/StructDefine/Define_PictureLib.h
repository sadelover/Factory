
/************************************************************************/
// This header file refers to the table uppc_libmodel.picturelib                          
/************************************************************************/
#ifndef DEFINE_PICTURELIB_H
#define DEFINE_PICTURELIB_H

#include <string>
using std::string;

typedef struct tag_PictureLib
{
	int iID;					//	图片ID
	int iType;					//	图片类型
	int iPortNum;				//	图片端口数
	string strPortPintList;		//	端口号数
	int iStateNum;				//  状态个数
	string strImageBuffer;		//  图片内容
}PictureLibEntry, *pPictureLibEntry;


enum PICTURELIB_COLUMN
{	
	PICTURELIB_ID,
	PICTURELIB_TYPE,
	PICTURELIB_PORTNUM,
	PICTURELIB_PORTPOINTLIST,
	PICTURELIB_STATENUM,
	PICTURELIB_IMAGEBUFFER,
	PICTURELIB_MAXNUM,
};

#endif



