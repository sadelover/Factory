#pragma once

#include "../Tools/StringDef.h"

/*
Author:		Chang
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-Project���ڵ���������
*/
typedef struct tag_gProject
{
	int		iProjectID;			//��ĿΨһ���
	tstring	strName;			//��Ŀ����
	tstring	strLocation;		//��ַ
	int		iProjectType;		//��������,0: �칫��	1: �̳�	2: �Ƶ�	3: �ۺ���ҵ����
	int		iWeatherStation;	//��������վ���
	double	dFloorArea;			//�������
	sstring	strPicture;			//���ͼ, ��Ϊ������ͼƬ����
	tstring	strMemo;			//��ע
	tstring	strCreator;			//������
}gProject, *pProject;

enum ePROJECT
{
	PROJ_PROJECTID,
	PROJ_NAME,
	PROJ_LOCATION,
	PROJ_PROJECTTYPE,
	PROJ_WEATHERSTATION,
	PROJ_FLOORAREA,
	PROJ_PICTURE,
	PROJ_MEMO,
	PROJ_CREATOR,
	PROJ_MAXNUM
};