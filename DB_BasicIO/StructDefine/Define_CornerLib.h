#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-CornerLib ���ڵ���������
*/
typedef struct tag_gCornerLib
{
   int		iCornerID;	//��ͷID
   int		iCornerType;//��ͷ����
   double	dCorners;	//��ͷ�ֲ�����ϵ��
}gCornerLib, *pCornerLib;

enum eCORNERLIB
{
	CORNERLIB_CORNERLIBID,
	CORNERLIB_CORNERTYPE,
	CORNERLIB_CORNERS,
	CORNERLIB_MAXNUM
};