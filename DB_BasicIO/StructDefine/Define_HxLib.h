#pragma once

/*
Author:		hejuhua
Time:		2010-07-12
Comments:	��Ӧ���ݿ���-HxLib ���ڵ���������
*/
typedef struct tag_gHxLib
{
	int		iHxID;		//�����豸ID
	int     iHxType;	//�����豸����
	double  dHxDP1;		//�����豸һ�β�����ϵ��
	double  dHxDP2;		//�����豸���β�����ϵ��
	double  dTin1;		//һ�β��ˮ�¶�
	double  dTin2;		//���β��ˮ�¶�
	double  dTout1;		//һ�β��ˮ�¶�
	double  dTout2;		//���β��ˮ�¶�
	double  dHxQ;		//������(kw)
}gHxLib, *pHxLib;

enum eHXLIB
{
	HXLIB_HXID,
	HXLIB_HXTYPE,
	HXLIB_HXDP1,
	HXLIB_HXDP2,
	HXLIB_TIN1,
	HXLIB_TIN2,
	HXLIB_TOUT1,
	HXLIB_TOUT2,
	HXLIB_HXQ,
	HXLIB_MAXNUM
};