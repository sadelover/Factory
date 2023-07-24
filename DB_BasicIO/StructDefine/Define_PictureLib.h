
/************************************************************************/
// This header file refers to the table uppc_libmodel.picturelib                          
/************************************************************************/
#ifndef DEFINE_PICTURELIB_H
#define DEFINE_PICTURELIB_H

#include <string>
using std::string;

typedef struct tag_PictureLib
{
	int iID;					//	ͼƬID
	int iType;					//	ͼƬ����
	int iPortNum;				//	ͼƬ�˿���
	string strPortPintList;		//	�˿ں���
	int iStateNum;				//  ״̬����
	string strImageBuffer;		//  ͼƬ����
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



