#pragma once

#include "../../Tools/StringDef.h"

typedef struct tag_gWSLib
{
	int      iWSID;	                            //标识号
	tstring  strName;		                    //型号
	tstring  strBrand;			                //品牌
	int      iType;				                //类型
	double	 GroutDensity;                      //回填料密度
	double	 GroutSpecificHeat;                 //回填料比热容
	double	 GroutThermalConductivity;          //回填料导热系数
	double	 GroundThermalConductivity;         //土壤导热系数
	double	 GroundDensity;                     //土壤密度
	double	 GroundSpecificHeat;                //土壤比热容
	int		 BoreholeType;                      //管井类型
	double	 FluidSpecificHeat;                 //流体比热容
	double	 BoreholeDepth;                     //管井深度
	double	 BoreholeRadius;                    //管井半径
	int		 RowCount;                          //管井横数量
	int	     ColumnCount;                       //管井数数量
	double	 RowSpace;                          //管井横间距
	double	 ColumnSpace;                       //管井竖间距
	double	 UTubeShanks;                       //U形管管距
	double	 UPipeOuterRadius;                  //U形管外径
	double	 UPipeInterRadius;                  //U形管内径
	double	 ConvectiveHeatTransferCoefficient; //对流换热系数
	double	 PipeThermalConductivity;           //材料导热系数
	double	 FlowRate;                          //流体质量流量
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