#ifndef TABLEDEF_H
#define TABLEDEF_H
#include "atlstr.h"

// 冷机相关表的定义
#include "./StructDefine/Define_Chiller_Eff_Design_Data.h"
#include "./StructDefine/Define_Chiller_Model.h"
#include "./StructDefine/Define_ChillerLib.h"
#include "./StructDefine/Define_ChillerPlantRoom.h"
#include "./StructDefine/Define_ChillerRunRecord.h"

//冷却塔相关表的定义
#include "./StructDefine/Define_CoolingTower_Eff_Design_Data.h"
#include "./StructDefine/Define_CoolingTower_Model.h"
#include "./StructDefine/Define_CoolingTowerLib.h"
//#include "./StructDefine/Define_CoolingTowerRunRecord.h"

//地埋管相关定义
#include "./StructDefine/Define_WaterSoil_Model.h"
#include "./StructDefine/Define_WaterSoilLib.h"

//泵相关表的定义
#include "./StructDefine/Define_Pump_Eff_Design_Data.h"
#include "./StructDefine/Define_Pump_Model.h"
#include "./StructDefine/Define_PumpLib.h"
#include "./StructDefine/Define_PumpRunRecord.h"


// 机房布局关系相关表
#include "./StructDefine/Define_Room_Contain_Chiller.h"
#include "./StructDefine/Define_Room_Contain_CoolingTower.h"
#include "./StructDefine/Define_Room_Contain_Corner.h"
#include "./StructDefine/Define_Room_Contain_Hx.h"
#include "./StructDefine/Define_Room_Contain_Pipe.h"
#include "./StructDefine/Define_Room_Contain_Pump.h"
#include "./StructDefine/Define_Room_Contain_Terminal.h"
#include "./StructDefine/Define_Room_Contain_Valve.h"
#include "./StructDefine/Define_Room_Contain_PowerMeter.h"

// 相关设备参数信息表
#include "./StructDefine/Define_CornerLib.h"
#include "./StructDefine/Define_HxLib.h"
#include "./StructDefine/Define_PipeLib.h"
#include "./StructDefine/Define_TerminalLib.h"
#include "./StructDefine/Define_ValveLib.h"

//项目工程表
#include "./StructDefine/Define_Project.h"

//能源价格表
#include "./StructDefine/Define_EnergyPrice.h"

// 用户管理表
#include "./StructDefine/Define_UPPCUSERS.h"

//机房负载表
#include "./StructDefine/Define_CHPlantRoomLoad.h"

//
#include "./StructDefine/Define_ControlParamRecord.h"

#include "./structdefine/Define_Room_Contain_Warternet.h"
#include "./structdefine/Define_Waternet_Param.h"
#include "./structdefine/Define_Waternet_Model.h"

//Weather tables
#include "./structdefine/Define_WeatherDesignData.h"
#include "./structdefine/Define_WeatherHourData.h"
#include "./structdefine/Define_UPPC_User.h"
#include "./structdefine/Define_ListSuggestion.h"
#include "./structdefine/Define_RunContiditonSimulate.h"

#include "./structdefine/Define_TankLib.h"
#include "./structdefine/Define_Room_Contain_Tank.h"
#include "./structdefine/Define_Room_Contain_WarterSoil.h"

#include "./StructDefine/Define_PointValue_Record.h"
#include "./StructDefine/PointRecord_Define.h"


#endif