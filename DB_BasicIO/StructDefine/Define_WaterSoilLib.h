#pragma once

#include "../../Tools/StringDef.h"

typedef struct tag_gWSLib
{
	int      iWSID;	                            //��ʶ��
	tstring  strName;		                    //�ͺ�
	tstring  strBrand;			                //Ʒ��
	int      iType;				                //����
	double	 GroutDensity;                      //�������ܶ�
	double	 GroutSpecificHeat;                 //�����ϱ�����
	double	 GroutThermalConductivity;          //�����ϵ���ϵ��
	double	 GroundThermalConductivity;         //��������ϵ��
	double	 GroundDensity;                     //�����ܶ�
	double	 GroundSpecificHeat;                //����������
	int		 BoreholeType;                      //�ܾ�����
	double	 FluidSpecificHeat;                 //���������
	double	 BoreholeDepth;                     //�ܾ����
	double	 BoreholeRadius;                    //�ܾ��뾶
	int		 RowCount;                          //�ܾ�������
	int	     ColumnCount;                       //�ܾ�������
	double	 RowSpace;                          //�ܾ�����
	double	 ColumnSpace;                       //�ܾ������
	double	 UTubeShanks;                       //U�ιܹܾ�
	double	 UPipeOuterRadius;                  //U�ι��⾶
	double	 UPipeInterRadius;                  //U�ι��ھ�
	double	 ConvectiveHeatTransferCoefficient; //��������ϵ��
	double	 PipeThermalConductivity;           //���ϵ���ϵ��
	double	 FlowRate;                          //������������
}gWSLib, *pWSLib;

enum eWSLIB
{
	WSLIB_CTID,
	WSLIB_NAME,
	WSLIB_BRAND,
	WSLIB_TYPE,
	WSLIB_GroutDensity,                   
	WSLIB_GroutSpecificHeat,               
	WSLIB_GroutThermalConductivity,        
	WSLIB_GroundThermalConductivity,       
	WSLIB_GroundDensity,                   
	WSLIB_GroundSpecificHeat,              
	WSLIB_BoreholeType,                    
	WSLIB_FluidSpecificHeat,               
	WSLIB_BoreholeDepth,                   
	WSLIB_BoreholeRadius,                  
	WSLIB_RowCount,                        
	WSLIB_ColumnCount,                     
	WSLIB_RowSpace,                        
	WSLIB_ColumnSpace,                     
	WSLIB_UTubeShanks,                     
	WSLIB_UPipeOuterRadius,                
	WSLIB_UPipeInterRadius,                
	WSLIB_ConvectiveHeatTransferCoefficient,
	WSLIB_PipeThermalConductivity,         
	WSLIB_FlowRate,
	WSLIB_MAX
};