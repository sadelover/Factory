// DataPointConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataPointConfigDlg.h"
#include "MainFrm.h"
#include "DataPoint/DataPointManager.h"
#include <atlsimpstr.h>
#include "DataComConfigDlg.h"
#include "Tools/ColorDefines.h"
#include "Tools/Util/UtilString.h"
#include "DataPoint/DataPointEntry.h"
#include <locale.h>
#include "PointParamEditDlg.h"
#include "ExcelOperator.h"
#include "VPointSettingDlg.h"
#include "Tools/CustomTools/CustomTools.h"
#include "StoreCycleSettingDlg.h"
#include "oleExcel/OperateExcelFile.h"
#include "ImportTipsDlg.h"
#include"NewEquipmentPointsDlg.h"
#include "NewControlPointsDlg.h"
#include "NewStatisticsPointsDlg.h"
#include "NewCHPointsDlg.h"
#include "NewCTPointsDlg.h"
#include "NewCWPPointsDlg.h"
#include "NewPriChWPPointsDlg.h"
#include "NewSecChWPPointsDlg.h"
#include "NewPriHWPPointsDlg.h"
#include "NewSecHWPPointsDlg.h"
#include "ServerDataAccess/UPPCServerDataAccess.h"
#include "PointIgnoreDefDlg.h"
#include "json/json.h"

#include "HttpOperation.h"
#include "MainFrm.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "VPointHistoricalDataCalculation.h"//��ʷ���ݲ��㴰��
#include "BatchPointsDlg.h"
IMPLEMENT_DYNAMIC(CDataPointConfigDlg, CSkinDlg)

bool fav = true;

static CString s_strChillerNameArray[MAX_CHILLER_DEFAULT_POINT_NUM] = 
{
	L"ChOnOff",
	L"ChOnOffSetting",
	L"ChAmpLmtSetPointFeedback",
	L"ChAMPS",
	L"ChAutoMode",
	L"ChChWTempSupplySet",
	L"ChChWTempSupplySetPoint",
	L"ChCondPressure",
	L"ChCondTemp",
	L"ChEnterCondTemp",
	L"ChEnterEvapTemp",
	L"ChErr",
	L"ChEvapPressure",
	L"ChEvapTemp",
	L"ChGasExhaustTemp",
	L"ChLeaveCondTemp",
	L"ChLeaveEvapTemp",
	L"ChMotorRunHour",
	L"ChOilDP",
	L"ChOilT",
	L"ChWarningFaultCode",
	L"ChPower",
	L"ChPowerTotal",
	L"ChEvaValveAutoMode",
	L"ChEvaValveEnabled",
	L"ChEvaValveErr",
	L"ChEvaValveOnOffSetting",
	L"ChEvaValveStatus",
	L"ChConValveAutoMode",
	L"ChConValveEnabled",
	L"ChConValveErr",
	L"ChConValveOnOffSetting",
	L"ChConValveStatus",
	L"ChEnabled",
	L"ChEvaValveOn",
	L"ChEvaValveOff ",
	L"ChConValveOn",	
	L"ChConValveOff",	
	L"ChEvapFlowSwitchOnOff",
	L"ChCondFlowSwitchOnOff",
	L"ChComErr",	
	L"ChStartupCount",
	L"ChChWTempSupplySetAutoMode",
	L"ChPowerUAB",
	L"ChPowerUBC",
	L"ChPowerUAC",
	L"ChPowerIA",
	L"ChPowerIB",
	L"ChPowerIC",
	L"CHPowerPF",
	L"ChRealtimeLoad",
	L"ChFlow",
	L"ChCondFlow",	
	L"ChRealtimeEfficiency",
	L"ChRealtimeEfficiencyCOP",
	L"ChLoadTotal",
	L"ChAmpLmtSetPoint",
	L"ChAutoModeSetting",
	L"ChWarningReset",
	L"ChMotor01AMPS",
	L"ChMotor02AMPS",
	L"ChMotor03AMPS",
	L"ChMotor01OnOff01",
	L"ChMotor01OnOff02",
	L"ChMotor01OnOff03",
	L"ChRunStatus",
	L"ChRunStatusString",
	L"ChRunMode",
	L"ChOilSumpTemp01",
	L"ChCondRegrigerantLevel",
	L"ChOilSumpPressure",
	L"ChOilPumpPressure",	
	L"ChLiquidLineSoleniod",
	L"ChEvapPumpOnOff",
	L"ChPanelStopSwitch"	
};
static CString s_strChillerNameExplainArray[MAX_CHILLER_DEFAULT_POINT_NUM] = 
{
	L"������ػ�״̬",
	L"����ػ�������������",
	L"��������ٷֱ��޶�ֵ����",
	L"��������ٷֱ�",		  
	L"�������Զ�̾͵�״̬",	 
	L"�����ˮ�¶�ʵ���趨����",	
	L"�����ˮ�¶��趨	",		 
	L"�����������ýѹ��	",	 
	L"�����������ý�¶�		 ",
	L"�����������ˮ�¶�		 ",
	L"�����������ˮ�¶�		 ",
	L"������ϱ���				 ",
	L"�����������ýѹ��		 ",
	L"�����������ý�¶�		 ",
	L"��������¶�				 ",
	L"�����������ˮ�¶�		 ",
	L"�����������ˮ�¶�		 ",
	L"���ѹ��������ʱ��		 ",
	L"�����ѹ��				 ",
	L"�������					 ",
	L"������ϴ���				 ",
	L"������ʵʱ�й�����	   ",
	L"�������й����ֵ		 ",
	L"�����䶳�綯��Զ�̾͵�״̬",	
	L"�����䶳�綯��ʹ��		 ",
	L"�����䶳�綯��ʹ��		 ",
	L"�����䶳�綯����������	",	
	L"�����䶳�綯������״̬	",	
	L"������ȴ�綯��Զ�̾͵�״̬",	
	L"������ȴ�綯��ʹ��		��",
	L"������ȴ�綯������		��",
	L"������ȴ�綯�����ؿ���	 ",
	L"������ȴ�綯������״̬	 ",
	L"�����䶳�綯������λ״̬	 ",
	L"�����䶳�綯���ص�λ״̬	 ",
	L"������ȴ�綯������λ״̬	 ",
	L"������ȴ�綯���ص�λ״̬	",	
	L"������ȴ�綯���ص�λ״̬	 ",
	L"����������ˮ������״̬	 ",
	L"����������ˮ������״̬	 ",
	L"�����������-ͨѶ����		 ",
	L"��������������			 ",
	L"�����ˮ�¶��趨���Զ�	 ",
	L"����AB�ߵ�ѹ				 ",
	L"����BC�ߵ�ѹ				 ",
	L"����AC�ߵ�ѹ				 ",
	L"����A�ߵ���				 ",
	L"����B�ߵ���				 ",
	L"����C�ߵ���				 ",
	L"������������			   ",
	L"����ʵʱ����			   ",
	L"�����䶫������			 ",
	L"������ȴ������			  ",
	L"������Ч��(kW/ton)		 ",
	L"����COP(kW/kW)			 ",
	L"�����ۻ��为��			 ",
	L"���������ٷֱ��޶�ֵ�趨	 ",
	L"�������Զ��趨",
	L"������λָ��	",
	L"�����ͷ1�����ٷֱ�",
	L"�����ͷ2�����ٷֱ�",
	L"�����ͷ3�����ٷֱ�",
	L"�����ͷ1����״̬",
	L"�����ͷ2����״̬",
	L"�����ͷ3����״̬",
	L"����ڲ�״̬ö����",
	L"����ڲ�״̬ö��������",
	L"����ڲ�����ģʽ",
	L"���������¶�",
	L"����������Һλ�߶Ȱٷֱ�",
	L"��������ѹ��",
	L"�����ͱ�ѹ��",
	L"������ˮ���ŷ�����״̬",	
	L"������ˮ��ÿ���״̬	",
	L"���������忪��״̬	"
};
static CString s_strTowerNameArray[MAX_TOWER_DEFAULT_POINT_NUM] = 
{
	L"CTPressureSupply",
	L"CTVSDFreq	",
	L"CTVSDFreqSetting",
	L"CTErr",
	L"CTAutoMode",
	L"CTOnOff",
	L"CTOnOffSetting",
	L"CTPower",
	L"CTPowerTotal",
	L"CTEnabled",
	L"CHourCT",
	L"HourCT",
	L"CTRealtimeEfficiency",
	L"CTFlow",
	L"CTAirFlow",
	L"CTEmergencyStop",
	L"CTAutoModeSetting",	
	L"CTVSDOnOff",
	L"CTVSDErr",
	L"CTVSDPower",
	L"CTVSDPowerIA"
};
static CString s_strTowerNameExplainArray[MAX_TOWER_DEFAULT_POINT_NUM] = 
{
L"��ȴ���Ĺ�ˮѹ��	",
L"��ȴ��Ƶ�ʷ���	       ",     
L"��ȴ��Ƶ���趨		   ",
L"��ȴ�����ϱ���			",
L"��ȴ�����Զ�״̬		   ",
L"��ȴ������״̬		   ",
L"��ȴ����ͣ����		   ",
L"��ȴ�����й�����		   ",
L"��ȴ���й�����		   ",
L"��ȴ���Ƿ����		   ",
L"��ȴ���ۻ�����Сʱ��		",			
L"��ȴ��������������Сʱ��	",				
L"��ȴ��Ч��			   ",
L"��ȴ������			   ",
L"��ȴ������			   ",
L"��ȴ����ͣ�ź�		   ",
L"��ȴ�����Զ�����		   ",
L"��ȴ����Ƶ����ͣ״̬	   ",
L"��ȴ����Ƶ�������ź�	   ",
L"��ȴ����Ƶ������		   ",
L"��ȴ����Ƶ���������	 "
};
static CString s_strValveNameArray[MAX_VALVE_DEFAULT_POINT_NUM] = 
{
	L"ChEvapValveAutoMode",
	L"ChEvapValveEnabled",
	L"ChEvapValveErr",
	L"ChEvapValveOnOffSetting",
	L"ChEvapValveStatus",
	L"ChCondValveAutoMode",
	L"ChCondValveEnabled",
	L"ChCondValveErr",
	L"ChCondValveOnOffSetting",
	L"ChCondValveStatus"
};
static CString s_strValveNameExplainArray[MAX_VALVE_DEFAULT_POINT_NUM] = 
{
	L"�����䶳�綯��Զ�̾͵�״̬",
	L"�����䶳�綯��ʹ��",
	L"�����䶳�綯������",
	L"�����䶳�綯����������",
	L"�����䶳�綯������״̬",
	L"������ȴ�綯��Զ�̾͵�״̬",
	L"������ȴ�綯��ʹ��",
	L"������ȴ�綯������",
	L"������ȴ�綯�����ؿ���",
	L"������ȴ�綯������״̬"
};
static CString s_strPumpNameArray[MAX_PUMP_DEFAULT_POINT_NUM] = 
{
		L"CWPPressureSupply	",
		L"CWPVSDFreq",
		L"CWPVSDFreqSetting",
		L"CWPErr",
		L"CWPAutoMode ",
		L"CWPOnOff",
		L"CWPOnOffSetting",
		L"CWPPower",
		L"CWPPowerTotal",
		L"CWPEnabled",
		L"CHourCWP",
		L"HourCWP",
		L"CWPRealtimeEfficiency",	
		L"CWPFlow",
		L"CWPHead",
		L"CWPEmergencyStop",
		L"CWPAutoModeSetting",
		L"CWPVSDOnOff",
		L"CWPVSDErr",
		L"CWPVSDPower",
		L"CWPVSDPowerIA"
};
static CString s_strPumpNameExplainArray[MAX_PUMP_DEFAULT_POINT_NUM] = 
{
L"��ȴѭ���õĹ�ˮѹ��	",
L"��ȴѭ����Ƶ�ʷ���	      ",
L"��ȴѭ����Ƶ���趨		  ",
L"��ȴѭ���ù��ϱ���		  ",
L"��ȴѭ�������Զ�״̬		  ",
L"��ȴѭ��������״̬		  ",
L"��ȴѭ������ͣ����		  ",
L"��ȴ�����й�����			  ",
L"��ȴ���й�����			   ",
L"��ȴ���Ƿ����		  ",
L"��ȴ���ۻ�����Сʱ��		",
L"��ȴ�ñ�����������Сʱ��",	
L"��ȴ��Ч��				  ",
L"��ȴ������				  ",
L"��ȴ�����				  ",
L"��ȴ�ü�ͣ�ź�			  ",
L"��ȴ�����Զ�����		  ",
L"��ȴ�ñ�Ƶ����ͣ״̬	  ",
L"��ȴ�ñ�Ƶ�������ź�		",
L"��ȴ�ñ�Ƶ������		  ",
L"��ȴ�ñ�Ƶ���������		"
};
static CString s_strPriPumpNameArray[MAX_PUMP_DEFAULT_POINT_NUM] = 
{
	L"PrichWPPressureSupply	",
	L"PrichWPVSDFreq",
	L"PrichWPVSDFreqSetting	",
	L"PrichWPErr",
	L"PrichWPAutoMode",
	L"PrichWPOnOff",
	L"PrichWPOnOffSetting",
	L"PrichWPPower",
	L"PrichWPPowerTotal",
	L"PrichWPEnabled",
	L"CHourPrichWP",
	L"HourPrichWP",
	L"PrichWPRealtimeEfficiency",	
	L"PrichWPFlow ",
	L"PrichWPHead",
	L"PrichWPEmergencyStop",
	L"PrichWPAutoModeSetting",
	L"PrichWPVSDOnOff",
	L"PrichWPVSDErr ",
	L"PrichWPVSDPower",
	L"PrichWPVSDPowerIA"
};
static CString s_strPriPumpNameExplainArray[MAX_PUMP_DEFAULT_POINT_NUM] = 
{
	L"һ����ȴѭ���õĹ�ˮѹ��	",
	L"һ����ȴѭ����Ƶ�ʷ���	      ",
	L"һ����ȴѭ����Ƶ���趨		  ",
	L"һ����ȴѭ���ù��ϱ���		  ",
	L"һ����ȴѭ�������Զ�״̬		  ",
	L"һ����ȴѭ��������״̬		  ",
	L"һ����ȴѭ������ͣ����		  ",
	L"һ����ȴ�����й�����			  ",
	L"һ����ȴ���й�����			   ",
	L"һ����ȴ���Ƿ����		  ",
	L"һ����ȴ���ۻ�����Сʱ��		",
	L"һ����ȴ�ñ�����������Сʱ��",	
	L"һ����ȴ��Ч��				  ",
	L"һ����ȴ������				  ",
	L"һ����ȴ�����				  ",
	L"һ����ȴ�ü�ͣ�ź�			  ",
	L"һ����ȴ�����Զ�����		  ",
	L"һ����ȴ�ñ�Ƶ����ͣ״̬	  ",
	L"һ����ȴ�ñ�Ƶ�������ź�		",
	L"һ����ȴ�ñ�Ƶ������		  ",
	L"һ����ȴ�ñ�Ƶ���������		"
};
static CString s_strSecPumpNameArray[MAX_PUMP_DEFAULT_POINT_NUM] = 
{
	L"SecChWPPressureSupply	",
	L"SecChWPVSDFreq",
	L"SecChWPVSDFreqSetting",
	L"SecChWPErr ",
	L"SecChWPAutoMode ",
	L"SecChWPOnOff ",
	L"SecChWPOnOffSetting",
	L"SecChWPPower",
	L"SecChWPPowerTotal",
	L"SecChWPEnabled",
	L"CHourSecChWP",
	L"HourSecChWP",
	L"SecChWPRealtimeEfficiency",	
	L"SecChWPFlow",
	L"SecChWPHead ",
	L"SecChWPEmergencyStop",
	L"SecChWPAutoModeSetting",
	L"SecChWPVSDOnOff ",
	L"SecChWPVSDErr",
	L"SecChWPVSDPower ",
	L"SecChWPVSDPowerIA"
};
static CString s_strSecPumpNameExplainArray[MAX_PUMP_DEFAULT_POINT_NUM] =
{
	L"������ȴѭ���õĹ�ˮѹ��	",
	L"������ȴѭ����Ƶ�ʷ���	      ",
	L"������ȴѭ����Ƶ���趨		  ",
	L"������ȴѭ���ù��ϱ���		  ",
	L"������ȴѭ�������Զ�״̬		  ",
	L"������ȴѭ��������״̬		  ",
	L"������ȴѭ������ͣ����		  ",
	L"������ȴ�����й�����			  ",
	L"������ȴ���й�����			   ",
	L"������ȴ���Ƿ����		  ",
	L"������ȴ���ۻ�����Сʱ��		",
	L"������ȴ�ñ�����������Сʱ��",	
	L"������ȴ��Ч��				  ",
	L"������ȴ������				  ",
	L"������ȴ�����				  ",
	L"������ȴ�ü�ͣ�ź�			  ",
	L"������ȴ�����Զ�����		  ",
	L"������ȴ�ñ�Ƶ����ͣ״̬	  ",
	L"������ȴ�ñ�Ƶ�������ź�		",
	L"������ȴ�ñ�Ƶ������		  ",
	L"������ȴ�ñ�Ƶ���������		"
};
static CString s_strPriPumpHNameArray[MAX_PUMP_DEFAULT_POINT_NUM] = 
{
	L"PriHWPPressureSupply	",
	L"PriHWPVSDFreq",
	L"PriHWPVSDFreqSetting",
	L"PriHWPErr",
	L"PriHWPAutoMode",
	L"PriHWPOnOff",
	L"PriHWPOnOffSetting",
	L"PriHWPPower",
	L"PriHWPPowerTotal",
	L"PriHWPEnabled	",
	L"CHourPriHWP",
	L"HourPriHWP",
	L"PriHWPRealtimeEfficiency",	
	L"PriHWPFlow ",
	L"PriHWPHead",
	L"PriHWPEmergencyStop",
	L"PriHWPAutoModeSetting	",
	L"PriHWPVSDOnOff",
	L"PriHWPVSDErr",
	L"PriHWPVSDPower",
	L"PriHWPVSDPowerIA"
};
static CString s_strPriPumpHNameExplainArray[MAX_PUMP_DEFAULT_POINT_NUM] = 
{
	L"һ����ˮѭ���õĹ�ˮѹ��	",
	L"һ����ˮѭ����Ƶ�ʷ���	      ",
	L"һ����ˮѭ����Ƶ���趨		  ",
	L"һ����ˮѭ���ù��ϱ���		  ",
	L"һ����ˮѭ�������Զ�״̬		  ",
	L"һ����ˮѭ��������״̬		  ",
	L"һ����ˮѭ������ͣ����		  ",
	L"һ����ˮ�����й�����			  ",
	L"һ����ˮ���й�����			   ",
	L"һ����ˮ���Ƿ����		  ",
	L"һ����ˮ���ۻ�����Сʱ��		",
	L"һ����ˮ�ñ�����������Сʱ��",	
	L"һ����ˮ��Ч��				  ",
	L"һ����ˮ������				  ",
	L"һ����ˮ�����				  ",
	L"һ����ˮ�ü�ͣ�ź�			  ",
	L"һ����ˮ�����Զ�����		  ",
	L"һ����ˮ�ñ�Ƶ����ͣ״̬	  ",
	L"һ����ˮ�ñ�Ƶ�������ź�		",
	L"һ����ˮ�ñ�Ƶ������		  ",
	L"һ����ˮ�ñ�Ƶ���������		"
};
static CString s_strSecPumpHNameArray[MAX_PUMP_DEFAULT_POINT_NUM] = 
{
	L"SecHWPPressureSupply	",
	L"SecHWPVSDFreq",
	L"SecHWPVSDFreqSetting",
	L"SecHWPErr ",
	L"SecHWPAutoMode",
	L"SecHWPOnOff",
	L"SecHWPOnOffSetting",
	L"SecHWPPower ",
	L"SecHWPPowerTotal",
	L"SecHWPEnabled",
	L"CHourSecHWP",
	L"HourSecHWP",
	L"SecHWPRealtimeEfficiency",	
	L"SecHWPFlow",
	L"SecHWPHead ",
	L"SecHWPEmergencyStop  ",
	L"SecHWPAutoModeSetting	",
	L"SecHWPVSDOnOff",
	L"SecHWPVSDErr",
	L"SecHWPVSDPower",
	L"SecHWPVSDPowerIA"
};
static CString s_strSecPumpHNameExplainArray[MAX_PUMP_DEFAULT_POINT_NUM] = 
{
	L"������ˮѭ���õĹ�ˮѹ��	",
	L"������ˮѭ����Ƶ�ʷ���	      ",
	L"������ˮѭ����Ƶ���趨		  ",
	L"������ˮѭ���ù��ϱ���		  ",
	L"������ˮѭ�������Զ�״̬		  ",
	L"������ˮѭ��������״̬		  ",
	L"������ˮѭ������ͣ����		  ",
	L"������ˮ�����й�����			  ",
	L"������ˮ���й�����			   ",
	L"������ˮ���Ƿ����		  ",
	L"������ˮ���ۻ�����Сʱ��		",
	L"������ˮ�ñ�����������Сʱ��",	
	L"������ˮ��Ч��				  ",
	L"������ˮ������				  ",
	L"������ˮ�����				  ",
	L"������ˮ�ü�ͣ�ź�			  ",
	L"������ˮ�����Զ�����		  ",
	L"������ˮ�ñ�Ƶ����ͣ״̬	  ",
	L"������ˮ�ñ�Ƶ�������ź�		",
	L"������ˮ�ñ�Ƶ������		  ",
	L"������ˮ�ñ�Ƶ���������		"
};
CDataPointConfigDlg::CDataPointConfigDlg(CDataPointManager& plcmap,  CWnd* pParent /*=NULL*/)
	: CSkinDlg(CDataPointConfigDlg::IDD, pParent)
	, m_plcpointmap(plcmap)
	, m_bNeedSave(false)
{
}

CDataPointConfigDlg::~CDataPointConfigDlg()
{	
}

void CDataPointConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCTRL_POINT, m_listctrl);
	DDX_Control(pDX, IDC_BUTTON_IMPORT, m_btnImportPtTable);
	DDX_Control(pDX, IDC_BUTTON_EXPORT, m_btnExportPtTable);
	DDX_Control(pDX, ID_BUTTON_DELETE, m_btnDeletePt);
	DDX_Control(pDX, IDC_BTN_CHK_POINT, m_btnCheckPtTable);
	DDX_Control(pDX, IDC_BUTTON_POINT_SAVE, m_btnSavePtTable);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_POINT_IGNORE, m_Btn_Ignore_Point_List);
	DDX_Control(pDX, IDC_COMBO_POINT_TYPE_SORT, m_cbb_pointTypeSort);
}

BEGIN_MESSAGE_MAP(CDataPointConfigDlg, CSkinDlg)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDataPointConfigDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_ADDPT, &CDataPointConfigDlg::OnBnClickedButtonAddpt)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CDataPointConfigDlg::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CDataPointConfigDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_ADD_CHILLER, &CDataPointConfigDlg::OnBnClickedButtonAddChiller)
	ON_BN_CLICKED(ID_BUTTON_DELETE, &CDataPointConfigDlg::OnBnClickedButtonDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL_POINT, &CDataPointConfigDlg::OnDblclkListctrlPoint)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTCTRL_POINT, &CDataPointConfigDlg::OnLvnColumnclickListctrlPoint)
	ON_BN_CLICKED(IDC_BUTTON_ADDVPT, &CDataPointConfigDlg::OnBnClickedButtonAddvpt)
	ON_NOTIFY(NM_RCLICK, IDC_LISTCTRL_POINT, &CDataPointConfigDlg::OnNMRClickListctrlPoint)
	ON_COMMAND(ID_SET_STORE_CYCLE, &CDataPointConfigDlg::OnSetStoreCycle)
	ON_COMMAND(ID_MENU_SET_VPOINT, &CDataPointConfigDlg::OnMenuSetVpoint)
	ON_BN_CLICKED(IDC_BUTTON_POINT_SAVE, &CDataPointConfigDlg::OnBnClickedButtonPointSave)
	ON_BN_CLICKED(IDCANCEL, &CDataPointConfigDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SHOWALL, &CDataPointConfigDlg::OnBnClickedButtonShowall)
	ON_BN_CLICKED(IDC_BTN_AUTOGEN_POINTNAME, &CDataPointConfigDlg::OnBnClickedBtnAutogenPointname)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_TEMPLATE, &CDataPointConfigDlg::OnBnClickedButtonDownloadTemplate)
	ON_BN_CLICKED(IDC_BUTTON_POINT_IGNORE, &CDataPointConfigDlg::OnBnClickedButtonPointIgnore)
	ON_BN_CLICKED(IDC_BUTTON_Point_Copy, &CDataPointConfigDlg::OnBnClickedButtonPointCopy)
	ON_CBN_SELENDOK(IDC_COMBO_POINT_TYPE_SORT, &CDataPointConfigDlg::OnCbnSelendokComboPointTypeSort)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_UP_HISTORY_DATA, &CDataPointConfigDlg::OnBnClickedButtonMakeUpHistoryData)
END_MESSAGE_MAP()

void CDataPointConfigDlg::InitList()
{
	m_listctrl.SetBkColor(RGB(235, 235, 235));
	m_listctrl.SetTextColor(RGB(0, 0, 128));

	m_listctrl.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_listctrl.InsertColumn( 0, _T("���"), LVCFMT_LEFT, 70 );
	m_listctrl.InsertColumn( 1, _T("Physical ID"), LVCFMT_LEFT, 160 );
	m_listctrl.InsertColumn( 2, _T("Դ/����"), LVCFMT_LEFT, 100 );
	m_listctrl.InsertColumn( 3, _T("����/ע��"), LVCFMT_LEFT, 240 );
	m_listctrl.InsertColumn( 4, _T("��λ"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 5, _T("��д����"), LVCFMT_LEFT, 120 );
	m_listctrl.InsertColumn( 6, _T("����1"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 7, _T("����2"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 8, _T("����3"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 9, _T("����4"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 10, _T("����5"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 11, _T("����6"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 12, _T("����7"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 13, _T("����8"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 14, _T("����9"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 15, _T("����10"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 16, _T("����11"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 17, _T("����12"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 18, _T("����13"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 19, _T("����14"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 20, _T("�洢����"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 21, _T("�Զ���"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 22, _T("ϵͳ"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 23, _T("�豸"), LVCFMT_LEFT, 80 );
	m_listctrl.InsertColumn( 24, _T("����"), LVCFMT_LEFT, 80 );
}

void CDataPointConfigDlg::OnCancel()
{
	if(m_bNeedSave)
	{
		const int nRet = MessageBox(L"���޸�,�Ƿ񱣴���˳���",L"��ʾ",MB_YESNOCANCEL);
		if (IDYES == nRet)
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			pFrame->m_plcpointmap = m_plcpointmap;
			if(pFrame->m_pPrjSqlite->SaveProjectSqlite_opcid3())
			{
				AfxMessageBox(L"������ɣ�");
				m_bNeedSave = false;
			}
		}
		else if (IDNO == nRet)
		{
		}
		else if (IDCANCEL == nRet)
		{
			return;
		}
		else
		{
			return;
		}
	}
	m_bNeedSave = false; // used?
	CDialog::OnCancel();
}


BOOL CDataPointConfigDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();
	////�ƶ���ť
	//{
	//	{
	//		CRect rectBtn;
	//		GetDlgItem(IDC_BUTTON_IMPORT)->GetWindowRect(&rectBtn);
	//		rectBtn.top	+= 15;
	//		rectBtn.bottom	+= 15;
	//		ScreenToClient(&rectBtn);
	//		GetDlgItem(IDC_BUTTON_IMPORT)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
	//	}
	//	{
	//		CRect rectBtn;
	//		GetDlgItem(IDC_BUTTON_EXPORT)->GetWindowRect(&rectBtn);
	//		rectBtn.top	+= 15;
	//		rectBtn.bottom	+= 15;
	//		ScreenToClient(&rectBtn);
	//		GetDlgItem(IDC_BUTTON_EXPORT)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
	//	}
	//	{
	//		CRect rectBtn;
	//		GetDlgItem(ID_BUTTON_DELETE)->GetWindowRect(&rectBtn);
	//		rectBtn.top	+= 15;
	//		rectBtn.bottom	+= 15;
	//		ScreenToClient(&rectBtn);
	//		GetDlgItem(ID_BUTTON_DELETE)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
	//	}
	//	{
	//		CRect rectBtn;
	//		GetDlgItem(IDCANCEL)->GetWindowRect(&rectBtn);
	//		rectBtn.top	+= 15;
	//		rectBtn.bottom	+= 15;
	//		ScreenToClient(&rectBtn);
	//		GetDlgItem(IDCANCEL)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
	//	}
	//	{
	//		CRect rectBtn;
	//		GetDlgItem(IDC_BUTTON_POINT_SAVE)->GetWindowRect(&rectBtn);
	//		rectBtn.top	+= 15;
	//		rectBtn.bottom	+= 15;
	//		ScreenToClient(&rectBtn);
	//		GetDlgItem(IDC_BUTTON_POINT_SAVE)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
	//	}
	//	{
	//		CRect rectBtn;
	//		GetDlgItem(IDC_BTN_CHK_POINT)->GetWindowRect(&rectBtn);
	//		rectBtn.top	+= 15;
	//		rectBtn.bottom	+= 15;
	//		ScreenToClient(&rectBtn);
	//		GetDlgItem(IDC_BTN_CHK_POINT)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
	//	}
	//}

	InitTypeCombo();
	InitList();
	MakeHeaderArray();
	m_menu.LoadMenu(IDR_MENU_STORE_CYCLE);

	map<int, DataPointEntry>& mapping = m_plcpointmap.GetPointMap();
	if (mapping.size()>0)
	{
		ParsePoints();
		ShowList(m_allentrylist);
	}
	CenterWindow();
	MakeAllList();
	Iinit_ComboBox_POINT_TYPE_SORT();

	//vector<CString> vecName;
	//IsPointMapValid(m_plcpointmap,vecName);
	//const size_t size = vecName.size();
	//if (size > 0)
	//{
	//	CString strInfo(L"���ظ��ĵ�:");
	//	for (size_t i=0;i<size;++i)
	//	{
	//		strInfo += vecName[i];
	//		if (i != size-1)
	//		{
	//			strInfo += L",";
	//		}
	//	}
	//	AfxMessageBox(strInfo);
	//}
	
	return TRUE;
}

void CDataPointConfigDlg::Iinit_ComboBox_POINT_TYPE_SORT(void)
{
	m_cbb_pointTypeSort.ResetContent();
	m_cbb_pointTypeSort.InsertString(0, L"ȫ��");

	//��ȡ���
	vector<DataPointEntry> & cPointlist = m_plcpointmap.GetPointList();
	//�����ʹ�õ�����
	vector<wstring> wstrAlreadyFoundList;
	//�����б�
	vector<wstring> wstrTypeList;
	wstrTypeList.push_back(PH370);
	wstrTypeList.push_back(SIMENS1200);
	wstrTypeList.push_back(SIMENS300);
	wstrTypeList.push_back(SIMENS1200TCP);
	wstrTypeList.push_back(SIMENS300TCP);
	wstrTypeList.push_back(AB500);
	wstrTypeList.push_back(HONEYWELLEBI);
	wstrTypeList.push_back(L"modbus");
	wstrTypeList.push_back(MODBUSDIRECTREADINSERVER);
	wstrTypeList.push_back(L"wireless");
	wstrTypeList.push_back(L"vpoint");
	wstrTypeList.push_back(L"bacnet");
	wstrTypeList.push_back(L"protocol104");
	wstrTypeList.push_back(L"lonworks");
	wstrTypeList.push_back(L"DB-Access");
	wstrTypeList.push_back(L"DB-SQLServer");
	wstrTypeList.push_back(L"DB-Oracle");
	wstrTypeList.push_back(L"DB-MySQL");
	wstrTypeList.push_back(L"custom1");
	wstrTypeList.push_back(L"custom2");
	wstrTypeList.push_back(L"custom3");
	wstrTypeList.push_back(L"custom4");
	wstrTypeList.push_back(L"custom5");
	wstrTypeList.push_back(L"dateTime");
	wstrTypeList.push_back(L"DanfossFCProtocol");
	wstrTypeList.push_back(L"Insight");
	wstrTypeList.push_back(L"WinCC1");
	wstrTypeList.push_back(L"KingView1");
	wstrTypeList.push_back(L"ArchestrA3");
	wstrTypeList.push_back(L"KEPware4");
	wstrTypeList.push_back(L"ABBScada");
	wstrTypeList.push_back(L"OPC");
	wstrTypeList.push_back(L"ModbusEquipment");
	wstrTypeList.push_back(L"ThirdParty");
	wstrTypeList.push_back(L"PersagyController");
	wstrTypeList.push_back(L"bacnet-mstp");
	wstrTypeList.push_back(L"OPCUA");
	wstrTypeList.push_back(L"bacnet-py");
	wstrTypeList.push_back(L"obix");
	wstrTypeList.push_back(L"logix");
	wstrTypeList.push_back(L"MoxaTCPServer");

	//�ж������Ƿ�ʹ��
	vector<wstring>::iterator iter = wstrTypeList.begin();
	for(; iter!=wstrTypeList.end(); iter++)
	{
		vector<DataPointEntry>::iterator  iter_point = cPointlist.begin();
		for(; iter_point!= cPointlist.end(); iter_point++)
		{
			if((*iter)==iter_point->GetSourceType())
			{
				wstrAlreadyFoundList.push_back(*iter);
				break;
			}
		}
	}

	//��ʹ�õ����ͼ��뵽ɸѡ
	vector<wstring>::iterator iter_AF = wstrAlreadyFoundList.begin();
	for(;iter_AF!=wstrAlreadyFoundList.end();iter_AF++)
	{
		m_cbb_pointTypeSort.AddString((*iter_AF).c_str());
	}
	m_cbb_pointTypeSort.SetCurSel(0);
}

BOOL CDataPointConfigDlg::PreTranslateMessage( MSG* msg )
{
	if (msg->message == WM_KEYDOWN)
	{
		switch(msg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return FALSE;
		}
	}
	
	return CDialog::PreTranslateMessage(msg);
}
/*
**	������������ť���������ķ�����
*/
void CDataPointConfigDlg::OnBnClickedButtonSearch()
{
	CString strSearch;
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowTextW(strSearch);
	if (strSearch.GetLength() == 0)
	{
			AfxMessageBox(L"�����ַ���Ϊ��!");
			return;
	}
	MakeSearchList(strSearch);
}

void CDataPointConfigDlg::ParsePoints()
{
	m_allentrylist.clear();

	map<int, DataPointEntry>& datalist = m_plcpointmap.GetPointMap();
	map<int, DataPointEntry>::const_iterator it = datalist.begin();
	for (;it != datalist.end(); it++)
	{
		const DataPointEntry& entry = it->second;
		m_allentrylist.push_back(entry);
	}
}


void CDataPointConfigDlg::RefreshIgnorePointList()
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//pFrame->SaveProjectFile();

	if(pFrame->m_wndFileView.GetDataAccess())
	{
		wstring wstrIgnorePointList = pFrame->m_wndFileView.GetDataAccess()->ReadOrCreateCoreDebugItemValue(L"point_ignore_list");

		vector<CString> strIgnoreList;
		Project::Tools::SplitStringByChar(wstrIgnorePointList.c_str(),',', strIgnoreList);

		CString strTemp;
		strTemp.Format(_T("��λ����(%d��)"),strIgnoreList.size());
		m_Btn_Ignore_Point_List.SetWindowTextW(strTemp.GetString());
	}
	else
	{
		CString strTemp;
		strTemp.Format(_T("��λ����(δ֪,�����ӵ���)"));
		m_Btn_Ignore_Point_List.SetWindowTextW(strTemp.GetString());

		//AfxMessageBox(L"δ�������ߵ��ԣ����ε������޷�������ʾ��");
	}
}

void CDataPointConfigDlg::ShowList( const vector<DataPointEntry>& entrylist )
{
	m_listctrl.SetRedraw(FALSE);
	m_listctrl.DeleteAllItems();

	for (size_t j = 0; j < entrylist.size(); j++)
	{
		const DataPointEntry& entry = entrylist[j];
		long itemcount = m_listctrl.GetItemCount();
		CString index;
		index.Format(_T("%d"), entry.GetPointIndex());
		m_listctrl.InsertItem(itemcount, index);
		m_listctrl.SetItemText(itemcount, 1, entry.GetShortName().c_str());
		m_listctrl.SetItemText(itemcount, 2, entry.GetSourceType().c_str());
		m_listctrl.SetItemText(itemcount, 3, entry.GetDescription().c_str());
		m_listctrl.SetItemText(itemcount, 4, entry.GetUnit().c_str());

		if(entry.GetProperty() == PLC_READ)
		{
			m_listctrl.SetItemText(itemcount, 5, _T("R"));
		}
		else if(entry.GetProperty() == PLC_WRITE)
		{
			m_listctrl.SetItemText(itemcount, 5, _T("W"));
		}
		else if ((entry.GetProperty() == PLC_PROP_MAX))
		{
			m_listctrl.SetItemText(itemcount, 5, _T("R/W"));
		}
		else
		{
			m_listctrl.SetItemText(itemcount, 5, _T(""));
		}
		m_listctrl.SetItemText(itemcount, 6, entry.GetParam(1).c_str());
		m_listctrl.SetItemText(itemcount, 7, entry.GetParam(2).c_str());
		m_listctrl.SetItemText(itemcount, 8, entry.GetParam(3).c_str());
		m_listctrl.SetItemText(itemcount, 9, entry.GetParam(4).c_str());
		m_listctrl.SetItemText(itemcount, 10, entry.GetParam(5).c_str());
		m_listctrl.SetItemText(itemcount, 11, entry.GetParam(6).c_str());
		m_listctrl.SetItemText(itemcount, 12, entry.GetParam(7).c_str());
		m_listctrl.SetItemText(itemcount, 13, entry.GetParam(8).c_str());
		m_listctrl.SetItemText(itemcount, 14, entry.GetParam(9).c_str());
		m_listctrl.SetItemText(itemcount, 15, entry.GetParam(10).c_str());
		CString strTemp;
		strTemp.Format(L"%.2f",entry.GetHighAlarm());
		m_listctrl.SetItemText(itemcount, 16, strTemp);
		//strTemp.Format(L"%.2f",entry.GetHighHighAlarm());
		//m_listctrl.SetItemText(itemcount, 17, strTemp);
		//strTemp.Format(L"%.2f",entry.GetLowAlarm());
		//m_listctrl.SetItemText(itemcount, 18, strTemp);
		//strTemp.Format(L"%.2f",entry.GetLowLowAlarm());
		//m_listctrl.SetItemText(itemcount, 19, strTemp);
		m_listctrl.SetItemText(itemcount, 17, entry.GetParam(12).c_str());
		m_listctrl.SetItemText(itemcount, 18, entry.GetParam(13).c_str());
		m_listctrl.SetItemText(itemcount, 19, entry.GetParam(14).c_str());
		m_listctrl.SetItemText(itemcount, 20, GetStoreCycle(entry.GetStoreCycle()));
		m_listctrl.SetItemText(itemcount, 21, entry.GetParam(15).c_str());
		m_listctrl.SetItemText(itemcount, 22, entry.GetParam(16).c_str());
		m_listctrl.SetItemText(itemcount, 23, entry.GetParam(17).c_str());
		m_listctrl.SetItemText(itemcount, 24, entry.GetParam(18).c_str());
	}
	m_listctrl.SetRedraw(TRUE);
}


void CDataPointConfigDlg::OnBnClickedButtonAddpt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ParsePoints();	
	ShowList(m_allentrylist);
	CPointParamEditDlg dlg(this);
	dlg.SetDlgType(enum_Add);
	dlg.SetPointMap(&m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
				/*
		����Ƿ��Сд�ظ�
		*/
		//���ĩβ�Ƿ��зǷ��ַ�
		CString strTemp = dlg.m_strPhysicalID;
		//CString b= strTemp.Right(1);
		int nErr_1 = strTemp.Find(_T(" "));
		int nErr_2 = strTemp.Find(_T("\n"));
		int nErr_3 = strTemp.Find(_T("\r"));
		int nErr_4 = strTemp.Find(_T("@"));
		int nErr_5 = strTemp.Find(_T("*"));
		if (nErr_1!=-1||nErr_2!=-1||nErr_3!=-1||nErr_4!=-1||nErr_5!=-1)
		{

			dlg.m_strPhysicalID.Replace(' ','x');
			dlg.m_strPhysicalID.Replace('\n','x');
			dlg.m_strPhysicalID.Replace('\r','x');
			dlg.m_strPhysicalID.Replace('@','x');
			dlg.m_strPhysicalID.Replace('*','x');
			AfxMessageBox(_T("�зǷ��ַ������Զ��滻!"));
		}
		for (int i=0;i<m_allentrylist.size();i++)
		{
			wstring str=m_allentrylist[i].GetShortName();
			CString strExistPointName( str.c_str() );
			int bEqual=strExistPointName.CompareNoCase(dlg.m_strPhysicalID);
			if (bEqual==0)
			{
				AfxMessageBox(_T("�Ѵ��ڸõ������ظ���"));
				return;

			}
			else
			{
				continue;
			}
			
		}

		const int nCount = m_listctrl.GetItemCount();
		CString strIndex;
		strIndex.Format(L"%d", nCount+1);
		m_listctrl.InsertItem(nCount, strIndex);
		m_listctrl.SetItemText(nCount, 1, dlg.m_strPhysicalID);
		m_listctrl.SetItemText(nCount, 2, dlg.m_strSource);
		m_listctrl.SetItemText(nCount, 3, dlg.m_strRemark);
		m_listctrl.SetItemText(nCount, 4, dlg.m_strUnit);
		m_listctrl.SetItemText(nCount, 5, dlg.m_strProperty);
		m_listctrl.SetItemText(nCount, 6, dlg.m_strParam1);
		m_listctrl.SetItemText(nCount, 7, dlg.m_strParam2);
		m_listctrl.SetItemText(nCount, 8, dlg.m_strParam3);
		m_listctrl.SetItemText(nCount, 9, dlg.m_strParam4);
		m_listctrl.SetItemText(nCount, 10, dlg.m_strParam5);
		m_listctrl.SetItemText(nCount, 11, dlg.m_strParam6);
		m_listctrl.SetItemText(nCount, 12, dlg.m_strParam7);
		m_listctrl.SetItemText(nCount, 13, dlg.m_strParam8);
		m_listctrl.SetItemText(nCount, 14, dlg.m_strParam9);
		m_listctrl.SetItemText(nCount, 15, dlg.m_strParam10);
		m_listctrl.SetItemText(nCount, 16, dlg.m_strParam11);
		m_listctrl.SetItemText(nCount, 17, dlg.m_strParam12);
		m_listctrl.SetItemText(nCount, 18, dlg.m_strParam13);
		m_listctrl.SetItemText(nCount, 19, dlg.m_strParam14);
		m_listctrl.SetItemText(nCount, 20, dlg.m_str_store_cycle);
		m_listctrl.SetItemText(nCount, 21, dlg.m_strParam15);
		m_listctrl.SetItemText(nCount, 22, dlg.m_strParam16);
		m_listctrl.SetItemText(nCount, 23, dlg.m_strParam17);
		m_listctrl.SetItemText(nCount, 24, dlg.m_strParam18);
		m_listctrl.EnsureVisible(m_listctrl.GetItemCount()-1,FALSE);

		GetDlgItem(IDC_EDIT_SEARCH)->SetWindowTextW(_T(""));
		m_bNeedSave = true;
		MakeAllList();
	}
}

void CDataPointConfigDlg::OnBnClickedButtonImport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
 	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	CFileDialog dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xls;*.xlsx)|*.xls;*.xlsx|")_T("CSV File (*.csv)|*.csv|")_T("XUP File (*.xup)|*.xup|"), NULL);
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{
		//pFrame->m_pPrjSqlite->DeleteAllPoint();
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		int nNameLen = strName.GetLength();
		CString strExt(strName.Right(nNameLen - strName.ReverseFind(_T('.'))-1));
		if (strExt.CompareNoCase(L"xup") == 0)
		{
			if (!LoadXUPPointFile(strFilePath))
			{
				AfxMessageBox(L"����xup����ļ�ʧ��!");
				return;
			}
			m_listctrl.DeleteAllItems();
			pFrame->m_pPrjSqlite->DeleteAllPoint();
			//ParsePoints();
			MakeAllList();
			ShowList(m_allentrylist);
			m_bNeedSave = true;
		}
		else if (strExt.CompareNoCase(L"xls") == 0 || strExt.CompareNoCase(L"xlsx") == 0)
		{
			if (!LoadExcelPointFile(strFilePath))
			{
				//AfxMessageBox(L"�������ļ�ʧ��!");
				return;
			}
			m_listctrl.DeleteAllItems();
			//ParsePoints();
			MakeAllList();
			ShowList(m_allentrylist);
			m_bNeedSave = true;
			pFrame->m_pPrjSqlite->DeleteAllPoint();
			pFrame->m_pPrjSqlite->SaveProjectSqlite_opcid3(); //��ղ�����
			// reset pointer of point table
			pFrame->m_Onlineplcpointmap.Clear();
			pFrame->m_Onlineplcpointmap = m_plcpointmap;
		}
		else if (strExt.CompareNoCase(L"csv") == 0)
		{
			if (!LoadCSVPointFile(strFilePath))
			{
				AfxMessageBox(L"����CSV�ļ�ʧ��!");
				return;
			}
			m_listctrl.DeleteAllItems();
			MakeAllList();
			ShowList(m_allentrylist);
			m_bNeedSave = true;
			//pFrame->m_pPrjSqlite->DeleteAllPoint();
		}
		else
		{
			AfxMessageBox(L"�Ƿ����ļ���ʽ!");
		}
		
	}
}


void CDataPointConfigDlg::OnBnClickedButtonExport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("Excel File (*.xlsx)|*.xlsx|"), NULL);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(L"�ļ�������Ϊ��!");
			return;
		}
		COperateExcelFile::Instance()->SaveToExcel(strFilePath, m_plcpointmap);
		AfxMessageBox(_T("�����ɹ�"));
	}
}

void CDataPointConfigDlg::SetPointMap( CDataPointManager& pointMap )
{

	m_plcpointmap = pointMap;
}

void CDataPointConfigDlg::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
	{
		vector<int> vec;
		POSITION pos = m_listctrl.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_listctrl.GetNextSelectedItem(pos);
			CString strName = m_listctrl.GetItemText(nItem,1);
			vec.push_back(nItem);
			m_wstrDelPointList.push_back(strName.GetString());
			//DeleteOneEntry(strName);
			//DeleteOneNameMap(strName);
			//DeleteOneOfPointList(strName);
			m_plcpointmap.DeletePoint(strName);
		}
		
		//�ӽ�����ɾ��
		for (int i=vec.size()-1;i>=0;--i)
		{
			m_listctrl.DeleteItem(vec[i]);
		}		
		ResetIndex();
		m_bNeedSave = true;
	}
}

void CDataPointConfigDlg::InitTypeCombo()
{

}

void CDataPointConfigDlg::ResetIndex()
{
	int nCount = m_listctrl.GetItemCount();
	for (int i=0;i<nCount;++i)
	{
		CString strIndex;
		strIndex.Format(L"%d",i+1);
		m_listctrl.SetItemText(i,0,strIndex);
	}
}

bool CDataPointConfigDlg::DeleteOneEntry( CString strPhysicalID )
{
	map<int, DataPointEntry>& mapping = m_plcpointmap.GetPointMap();
	map<int, DataPointEntry>::iterator it = mapping.begin();
	while (it != mapping.end())
	{
		if (it->second.GetShortName() == strPhysicalID.GetString())
		{
			it = mapping.erase(it);
			return true;
		}
		else
		{
			++it;
		}
	}
	return false;
}

bool CDataPointConfigDlg::DeleteOneNameMap(const CString strName)
{
	bool bRet = false;
	map<wstring, DataPointEntry>& mapping = m_plcpointmap.GetNameMap();
	map<wstring, DataPointEntry>::iterator	iter;

	for (iter=mapping.begin(); iter!=mapping.end(); ++iter)
	{
		if (strName.GetString() == iter->second.GetShortName())
		{
			mapping.erase(iter);
			bRet = true;
			break;
		}
	}

	return bRet;
}

bool CDataPointConfigDlg::DeleteOneOfPointList(const CString strPtName)
{
	bool bRet = false;
	vector<DataPointEntry>& vec = m_plcpointmap.GetPointList();
	vector<DataPointEntry>::iterator	iter;

	for (iter=vec.begin(); iter!=vec.end(); ++iter)
	{
		if (strPtName.GetString() == iter->GetShortName())
		{
			vec.erase(iter);
			bRet = true;
			break;
		}
	}

	return bRet;
}

void CDataPointConfigDlg::OnDblclkListctrlPoint(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem < 0)
	{
		return;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPointParamEditDlg dlg(this);
	dlg.SetPointMap(&m_plcpointmap);
	dlg.m_strPhysicalID = m_listctrl.GetItemText(pNMItemActivate->iItem, 1);
	dlg.m_strSource = m_listctrl.GetItemText(pNMItemActivate->iItem, 2);
	dlg.m_strRemark = m_listctrl.GetItemText(pNMItemActivate->iItem, 3);
	dlg.m_strUnit = m_listctrl.GetItemText(pNMItemActivate->iItem, 4);
	dlg.m_strProperty = m_listctrl.GetItemText(pNMItemActivate->iItem, 5);
	dlg.m_strParam1 = m_listctrl.GetItemText(pNMItemActivate->iItem, 6);
	dlg.m_strParam2 = m_listctrl.GetItemText(pNMItemActivate->iItem, 7);
	dlg.m_strParam3 = m_listctrl.GetItemText(pNMItemActivate->iItem, 8);
	dlg.m_strParam4 = m_listctrl.GetItemText(pNMItemActivate->iItem, 9);
	dlg.m_strParam5 = m_listctrl.GetItemText(pNMItemActivate->iItem, 10);
	dlg.m_strParam6 = m_listctrl.GetItemText(pNMItemActivate->iItem, 11);
	dlg.m_strParam7 = m_listctrl.GetItemText(pNMItemActivate->iItem, 12);
	dlg.m_strParam8 = m_listctrl.GetItemText(pNMItemActivate->iItem, 13);
	dlg.m_strParam9 = m_listctrl.GetItemText(pNMItemActivate->iItem, 14);
	dlg.m_strParam10 = m_listctrl.GetItemText(pNMItemActivate->iItem, 15);
	dlg.m_strParam11 = m_listctrl.GetItemText(pNMItemActivate->iItem, 16);
	dlg.m_strParam12 = m_listctrl.GetItemText(pNMItemActivate->iItem, 17);
	dlg.m_strParam13 = m_listctrl.GetItemText(pNMItemActivate->iItem, 18);
	//dlg.m_strParam14 = m_listctrl.GetItemText(pNMItemActivate->iItem, 19);
	dlg.m_bIsVisited = (m_listctrl.GetItemText(pNMItemActivate->iItem, 19) == _T("1")) ? TRUE : FALSE;
	dlg.m_str_store_cycle = m_listctrl.GetItemText(pNMItemActivate->iItem, 20);
	dlg.m_strParam15 = m_listctrl.GetItemText(pNMItemActivate->iItem, 21);
	dlg.m_strParam16 = m_listctrl.GetItemText(pNMItemActivate->iItem, 22);
	dlg.m_strParam17 = m_listctrl.GetItemText(pNMItemActivate->iItem, 23);
	dlg.m_strParam18 = m_listctrl.GetItemText(pNMItemActivate->iItem, 24);
	dlg.SetDlgType(enum_Edit);
	if (dlg.DoModal() == IDOK)
	{
		m_listctrl.SetItemText(pNMItemActivate->iItem, 1, dlg.m_strPhysicalID);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 2, dlg.m_strSource);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 3, dlg.m_strRemark);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 4, dlg.m_strUnit);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 5, dlg.m_strProperty);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 6, dlg.m_strParam1);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 7, dlg.m_strParam2);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 8, dlg.m_strParam3);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 9, dlg.m_strParam4);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 10, dlg.m_strParam5);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 11, dlg.m_strParam6);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 12, dlg.m_strParam7);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 13, dlg.m_strParam8);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 14, dlg.m_strParam9);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 15, dlg.m_strParam10);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 16, dlg.m_strParam11);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 17, dlg.m_strParam12);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 18, dlg.m_strParam13);
		//m_listctrl.SetItemText(pNMItemActivate->iItem, 19, dlg.m_strParam14);
		CString strVisit(dlg.m_bIsVisited ? _T("1") : _T("0"));
		m_listctrl.SetItemText(pNMItemActivate->iItem, 19, strVisit);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 20, dlg.m_str_store_cycle);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 21, dlg.m_strParam15);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 22, dlg.m_strParam16);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 23, dlg.m_strParam17);
		m_listctrl.SetItemText(pNMItemActivate->iItem, 24, dlg.m_strParam18);
		map<int, DataPointEntry>& datalist = m_plcpointmap.GetPointMap();
		map<int, DataPointEntry>::iterator it = datalist.begin();
		for (;it != datalist.end(); it++)
		{
			if ((*it).second.GetShortName() == dlg.m_strPhysicalID.GetString())
			{
				(*it).second.SetSourceType(dlg.m_strSource.GetString());
				(*it).second.SetDescription(dlg.m_strRemark.GetString());
				(*it).second.SetUnit(dlg.m_strUnit.GetString());
				if(dlg.m_strUnit.GetLength() > 0)
				{
					if(dlg.m_strUnit == L"R")
					{
						(*it).second.SetProperty(PLC_READ);
					}
					else if(dlg.m_strUnit == L"W")
					{
						(*it).second.SetProperty(PLC_WRITE);
					}
					else if(dlg.m_strUnit == L"R/W")
					{
						(*it).second.SetProperty(PLC_PROP_MAX);
					}
				}
				(*it).second.SetParam(1,dlg.m_strParam1.GetString());
				(*it).second.SetParam(2,dlg.m_strParam2.GetString());
				(*it).second.SetParam(3,dlg.m_strParam3.GetString());
				(*it).second.SetParam(4,dlg.m_strParam4.GetString());
				(*it).second.SetParam(5,dlg.m_strParam5.GetString());
				(*it).second.SetParam(6,dlg.m_strParam6.GetString());
				(*it).second.SetParam(7,dlg.m_strParam7.GetString());
				(*it).second.SetParam(8,dlg.m_strParam8.GetString());
				(*it).second.SetParam(9,dlg.m_strParam9.GetString());
				(*it).second.SetParam(10,dlg.m_strParam10.GetString());
				(*it).second.SetHighAlarm(-9999);
				//(*it).second.SetHighHighAlarm(-9999);
				//(*it).second.SetLowAlarm(-9999);
				//(*it).second.SetLowLowAlarm(-9999);
				(*it).second.SetParam(12,dlg.m_strParam12.GetString());
				(*it).second.SetParam(13,dlg.m_strParam13.GetString());
				(*it).second.SetParam(14,dlg.m_strParam14.GetString());
				(*it).second.SetStoreCycle(GetStoreCycle(dlg.m_str_store_cycle));
				break;
			}
		}
		m_bNeedSave = true;
		MakeAllList();
	}
	*pResult = 0;
}

void CDataPointConfigDlg::AddOneEntry( DataPointEntry entry )
{
	m_plcpointmap.AddEntry(entry);
}
/*
**	�������������ղ����ӵ����������ˢ�µ�����
**	������
**		strSearch����Ҫ���������ַ���
*/
void CDataPointConfigDlg::MakeSearchList(CString strSearch)
{
	m_searchlist.clear();
	strSearch.MakeLower();

	vector<CString> vSearchList;//�ؼ�������
	Project::Tools::SplitStringByChar(strSearch, L' ', vSearchList);

	map<int, DataPointEntry>& datalist = m_plcpointmap.GetPointMap();
	map<int, DataPointEntry>::const_iterator it = datalist.begin();

	if(vSearchList.size() == 1)//���ؼ�������
	{
		for (;it != datalist.end(); it++)
		{
			const DataPointEntry& entry = it->second;
			CString strPhysicalID = entry.GetShortName().c_str();
			CString strDescription = entry.GetDescription().c_str();
			CString	strParm1 = entry.GetParam(1).c_str();
			CString	strParm2 = entry.GetParam(2).c_str();
			CString	strParm3 = entry.GetParam(3).c_str();
			CString	strParm4 = entry.GetParam(4).c_str();
			CString	strParm5 = entry.GetParam(5).c_str();
			strPhysicalID.MakeLower();
			strDescription.MakeLower();
			strParm1.MakeLower();
			strParm2.MakeLower();
			strParm3.MakeLower();
			strParm4.MakeLower();
			strParm5.MakeLower();
			int pos1 = strPhysicalID.Find(strSearch);
			int pos2 = strDescription.Find(strSearch);
			int posParm1 = strParm1.Find(strSearch);
			int posParm2 = strParm2.Find(strSearch);
			int posParm3 = strParm3.Find(strSearch);
			int posParm4 = strParm4.Find(strSearch);
			int posParm5 = strParm5.Find(strSearch);
			if (pos1 != -1 || pos2 != -1 || posParm1 != -1 || posParm2 != -1 || posParm3 != -1 || posParm4 != -1 || posParm5 != -1)
			{
				m_searchlist.push_back(entry);
			}
		}
	}//END if(vSearchList.size() == 1)
	else if(vSearchList.size() >1)//��ؼ�������
	{
		for (;it != datalist.end(); it++)
		{
			const DataPointEntry& entry = it->second;
			CString strPhysicalID = entry.GetShortName().c_str();
			CString strDescription = entry.GetDescription().c_str();
			strPhysicalID.MakeLower();
			strDescription.MakeLower();
			
			vector<CString>::const_iterator cit = vSearchList.begin();
			int pos1 = 0;
			for(; cit != vSearchList.end(); cit++)//�����ؼ���
			{
				if(strPhysicalID.Find(*cit) != -1)
					pos1 +=1;
			}
			int pos2 = 0;
			for(cit= vSearchList.begin(); cit != vSearchList.end(); cit++)
			{
				if(strDescription.Find(*cit) != -1)
					pos2 +=1;
			}
			if (pos1 == vSearchList.size() || pos2 == vSearchList.size())//Ѱ�ҵ��Ĵ�����ؼ���������ͬ
			{
				m_searchlist.push_back(entry);
			}
		}
	}//END else if(vSearchList.size() >1) 

	ShowList(m_searchlist);
	if (m_searchlist.size()>0)
	{

	}
	else
	{
		AfxMessageBox(L"û�ҵ���ؼ�¼!");
	}
}

int CALLBACK listCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{ 
	DATA* pData = (DATA*)lParamSort; 
	int col = pData->subitem;
	CDataPointConfigDlg* pWnd = (CDataPointConfigDlg*)pData->pWnd;
	if (0 == col)
	{
		CString strItem1 = pWnd->m_listctrl.GetItemText(lParam1, 0); 
		CString strItem2 = pWnd->m_listctrl.GetItemText(lParam2, 0); 
		int n1 = _wtoi(strItem1.GetString()); 
		int n2 = _wtoi(strItem2.GetString());

		CString csStr;
		if (fav)
		{
			csStr = pWnd->m_strHeaderArray[col] + L"  ��";
		} 
		else
		{
			csStr = pWnd->m_strHeaderArray[col] + L"  ��";
		}

		LVCOLUMN VolHeader;
		VolHeader.pszText = (LPWSTR)csStr.GetString(); 
		VolHeader.mask = LVCF_TEXT; 
		VolHeader.cchTextMax = MAX_PATH*2; 
		pWnd->m_listctrl.SetColumn(col,&VolHeader);

		for (int i=0;i<MAX_HEADER_NUM;++i)
		{
			if (i != col)
			{
				LVCOLUMN VolHeader;
				VolHeader.pszText = (LPWSTR)pWnd->m_strHeaderArray[i].GetString(); 
				VolHeader.mask = LVCF_TEXT; 
				VolHeader.cchTextMax = MAX_PATH*2; 
				pWnd->m_listctrl.SetColumn(i,&VolHeader);
			}
		}

		if (fav)
		{
			if (n1<n2)
			{
				return -1;
			}
			else
			{
				return 1;
			}
		} 
		else
		{
			if (n1<n2) 
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		CString strItem1 =  pWnd->m_listctrl.GetItemText(lParam1,1); 
		CString strItem2 =  pWnd->m_listctrl.GetItemText(lParam2,1); 

		CString csStr;
		if (fav)
		{
			csStr = pWnd->m_strHeaderArray[col] + L"  ��";
		} 
		else
		{
			csStr = pWnd->m_strHeaderArray[col] + L"  ��";
		}

		LVCOLUMN VolHeader;
		VolHeader.pszText = (LPWSTR)csStr.GetString(); 
		VolHeader.mask = LVCF_TEXT; 
		VolHeader.cchTextMax = MAX_PATH*2;
		pWnd->m_listctrl.SetColumn(col,&VolHeader);

		for (int i=0;i<MAX_HEADER_NUM;++i)
		{
			if (i != col)
			{
				LVCOLUMN VolHeader;
				VolHeader.pszText = (LPWSTR)pWnd->m_strHeaderArray[i].GetString(); 
				VolHeader.mask = LVCF_TEXT; 
				VolHeader.cchTextMax = MAX_PATH*2; 
				pWnd->m_listctrl.SetColumn(i,&VolHeader);
			}
		}

		if (!fav) 
		{ 
			return _tcscmp(strItem2.GetBuffer(MAX_PATH),strItem1.GetBuffer(MAX_PATH)); 
		} 
		else
		{ 
			return _tcscmp(strItem1.GetBuffer(MAX_PATH),strItem2.GetBuffer(MAX_PATH)); 
		} 
	}
}

void CDataPointConfigDlg::OnLvnColumnclickListctrlPoint(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(int i = 0; i < m_listctrl.GetItemCount(); ++i)
	{
		m_listctrl.SetItemData(i,i); 
	}
	DATA data;
	data.subitem = pNMLV->iSubItem;
	data.pWnd = this;
	fav = !fav;
	m_listctrl.SortItems(listCompare,(LPARAM)&data);
	*pResult = 0;
}

void CDataPointConfigDlg::MakeHeaderArray()
{
	for (int i=0;i<MAX_HEADER_NUM;++i)
	{
		LVCOLUMN Vol;
		TCHAR szCol[MAX_PATH] = {0}; 
		Vol.pszText = szCol; 
		Vol.mask = LVCF_TEXT; 
		Vol.cchTextMax = sizeof(szCol); 
		m_listctrl.GetColumn(i ,&Vol);
		m_strHeaderArray[i] = Vol.pszText;
	}
}

//���ӹ���
void CDataPointConfigDlg::OnBnClickedButtonAddChiller()
{
	CNewEquipmentPointsDlg   dlg;
	if(dlg.DoModal()==IDOK)
	{
		int nEquipmentStartNum = dlg.m_nEquipmentStartNum;
		int nChillerCount = dlg.m_nChillerCount;
		CString strRoomname = dlg.m_strNewRoomName;
		int nChTowerCount = dlg.m_nChTowerCount;
		int nChPumpCount = dlg.m_nChTowerCount;
		int nPriChCWPCount = dlg.m_nPriChCount;
		int nSecChCWPCount = dlg.m_nSecCWPCount;
		int nPriHWPCount = dlg.m_nPriHWPCount;
		int nSecHWPCount = dlg.m_nSecHCWCount;
		int nFanBlowerCount = dlg.m_nFanBlowerCount;//Mark add

		int nEquipmentStartChillNum = nEquipmentStartNum;
		int nEquipmentStartTowerNum= nEquipmentStartNum;
		int nEquipmentStartPumpNum= nEquipmentStartNum;
		int nEquipmentStartValueNum= nEquipmentStartNum;
		int nEquipmentStartPriCWPNum= nEquipmentStartNum;
		int nEquipmentStartSecCwpNum= nEquipmentStartNum;
		int nEquipmentStartPriHWPNum= nEquipmentStartNum;
		int nEquipmentStartSecHWPNum= nEquipmentStartNum;
		
		CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
		int nId = -1;
		CString strSrc;
		CString strResult;
		CHttpOperation http(pFrame->m_strCloudServiceIp, pFrame->m_nCloudServicePort, _T("api/pointtool/autoGeneratePointList"));

		wstring wstrTimeStart,wstrTimeEnd ;

		Json::Value myRoot;
		Json::Value nameData;
		Json::Value desData;
		vector<int> nIndexList;

		
		myRoot["strPlantName"] = Project::Tools::WideCharToUtf8(dlg.m_strNewRoomName.GetString());//��������
		myRoot["strPlantDesc"] = Project::Tools::WideCharToUtf8(dlg.m_PlantDescription.GetString());
		myRoot["nCh"] = dlg.m_nChillerCount;//���̨��
		if(dlg.m_bChillerHasValve)
		{
			myRoot["ChillerHasValve"] = 1;
		}
		else
		{
			myRoot["ChillerHasValve"] = 0;
		}
		myRoot["nCWP"] = dlg.m_nCWPCount;//�����̨��
		myRoot["nPriChWP"] = dlg.m_nPriChCount;//һ���䶳��̨��
		myRoot["nSecChWP"] = dlg.m_nSecCWPCount;//�����䶳��̨��
		myRoot["nPriHWP"] = dlg.m_nPriHWPCount;//һ����ˮ��̨��
		myRoot["nSecHWP"] = dlg.m_nSecHCWCount;//������ˮ��̨��
		myRoot["bGenCtrlPoint"] = dlg.m_bAddControlPoints?1:0;
		
		Json::Value towerListJson;
		for(int xx=0;xx<dlg.m_nChTowerCount;xx++)
		{
			Json::Value oneTower;
			oneTower["nFan"]= dlg.m_nFanBlowerCount;

			towerListJson.append(oneTower);
		}
		myRoot["listCT"] = towerListJson;//Mark add


		string szBuf = myRoot.toStyledString();

		bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
		if(bRet)
		{
			Json::Reader jsReader;
			string strResult_utf8;
			Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
			Json::Value myReturnRoot;
			if (!jsReader.parse(strResult_utf8, myReturnRoot))
			{
				AfxMessageBox(_T("��׼�����Զ�����ʧ�ܣ�ԭ����ע����Ϣ�����"));
				return;
			}
			else
			{
				if(myReturnRoot.isObject())
				{
					Json::Value dataObj = myReturnRoot["data"];

					if(!dataObj.isNull())
					{
						string file_url_relative = dataObj.asString();
						wstring wstr_file_url_relative = Project::Tools::UTF8ToWideChar(file_url_relative.data());
						CString cstr_file_url_relative = wstr_file_url_relative.c_str();

						int nSplit = cstr_file_url_relative.ReverseFind('/');
						CString cstr_file_name = cstr_file_url_relative.Mid(nSplit+1);
		
						CString strFileURL;

						CFileDialog dlg(FALSE, _T(""), cstr_file_name, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, _T("*.xlsx"));
						//dlg.m_ofn.lpstrTitle = _T("�����ļ�");


						CString strNewFilePath;
						if ( IDOK == dlg.DoModal() )
						{	
							CString strNewFilePath=  dlg.GetPathName()	;
							CHttpOperation httpFile(pFrame->m_strCloudServiceIp, pFrame->m_nCloudServicePort,_T("/")+ cstr_file_url_relative);
							bool bRet = httpFile.HttpGetFile(strNewFilePath);
							if(bRet)
							{
								//string str_utf8;
								//Project::Tools::WideCharToUtf8(strNewFilePath.GetString(), str_utf8);
								try{
									//LPCSTR lpstr = (LPCSTR)str_utf8.data();
									::ShellExecute(NULL,L"open",strNewFilePath,NULL,NULL,SW_SHOWNORMAL);
								}
								catch(CException *e)
								{
									int xx=0;
								}

							}
							else
							{
								AfxMessageBox(_T("��׼������ɳɹ�����������ʧ�ܣ�"));
							}

						}



					}
				}
			}
		}
		else
		{
			AfxMessageBox(_T("��׼�����Զ�����ʧ�ܣ��������������Ƿ�����"));
			return;
		}

	}
}

int CDataPointConfigDlg::GetMaxKeyID()
{
	int nMaxID = 0;
	map<int, DataPointEntry>& datalist = m_plcpointmap.GetPointMap();
	map<int, DataPointEntry>::iterator iter = datalist.begin();
	for (;iter!=datalist.end();++iter)
	{
		if (iter->first > nMaxID)
		{
			nMaxID = iter->first;
		}
	}
	return nMaxID;
}

void CDataPointConfigDlg::AddAndDisplayOneItemRow(int nIndex,CString strIndex,int nCount,CString strFinalName,CString strFinalExpression)
{	//����
	map<int, DataPointEntry>& datalist = m_plcpointmap.GetPointMap();
    map<int, DataPointEntry>::iterator iter = datalist.begin();
	for(;iter != datalist.end(); iter++)
	{
		const DataPointEntry& entryMap = iter->second;
		if (entryMap.GetShortName().c_str() == strFinalName)
		{
			return;
		}
		
	}
	strIndex.Format(L"%d", nIndex);
	m_listctrl.InsertItem(nCount, strIndex);
	m_listctrl.SetItemText(nCount, 1, strFinalName);
	m_listctrl.SetItemText(nCount, 2, L"vpoint");
	m_listctrl.SetItemText(nCount, 3, strFinalExpression);
	m_listctrl.SetItemText(nCount, 4, L"");
	m_listctrl.SetItemText(nCount, 5, L"R");
	m_listctrl.SetItemText(nCount, 6, L"");
	m_listctrl.SetItemText(nCount, 7, L"");
	m_listctrl.SetItemText(nCount, 8, L"");
	m_listctrl.SetItemText(nCount, 9, L"");
	m_listctrl.SetItemText(nCount, 10, L"");
	m_listctrl.SetItemText(nCount, 11, L"");
	m_listctrl.SetItemText(nCount, 12, L"");
	m_listctrl.SetItemText(nCount, 13, L"");
	m_listctrl.SetItemText(nCount, 14, L"");
	m_listctrl.SetItemText(nCount, 15, L"");
	m_listctrl.SetItemText(nCount, 16, L"-9999");
	m_listctrl.SetItemText(nCount, 17, L"0");
	m_listctrl.SetItemText(nCount, 18, L"0");
	m_listctrl.SetItemText(nCount, 19, L"0");
	m_listctrl.SetItemText(nCount, 20, L"���洢");
	m_listctrl.SetItemText(nCount, 21, L"");
	m_listctrl.SetItemText(nCount, 22, L"");
	m_listctrl.SetItemText(nCount, 23, L"");
	m_listctrl.SetItemText(nCount, 24, L"");
	DataPointEntry entry;
	
	const size_t size = datalist.size(); 
	entry.SetPointIndex(nIndex);
	entry.SetShortName(strFinalName.GetString());
	entry.SetSourceType(L"vpoint");
	entry.SetDescription(strFinalExpression.GetString());
	entry.SetUnit(L"");
	entry.SetProperty(PLC_READ);
	entry.SetParam(1, L"");
	entry.SetParam(2, L"");
	entry.SetParam(3, L"");
	entry.SetParam(4, L"");
	entry.SetParam(5, L"");
	entry.SetParam(6, L"");
	entry.SetParam(7, L"");
	entry.SetParam(8, L"");
	entry.SetParam(9, L"");
	entry.SetParam(10, L"");
	entry.SetHighAlarm(-9999);
	//entry.SetHighHighAlarm(-9999);
	//entry.SetLowAlarm(-9999);
	//entry.SetLowLowAlarm(-9999);
	entry.SetParam(12, L"0");
	entry.SetParam(13, L"0");
	entry.SetParam(14, L"0");
	entry.SetStoreCycle(E_STORE_NULL);
	entry.SetParam(15, L"");
	entry.SetParam(16, L"");
	entry.SetParam(17, L"");
	entry.SetParam(18, L"");
	AddOneEntry(entry);
}


bool CDataPointConfigDlg::LoadCSVPointFile(const CString& strFilePath)
{
	char* old_locale=_strdup( setlocale(LC_CTYPE,NULL) ); 
	setlocale( LC_CTYPE,"chs");
	ASSERT(strFilePath.GetLength()>0);

	vector<wstring> vecContent;
	CStdioFile file;
	if (!file.Open(strFilePath,CFile::modeRead | CFile::shareDenyWrite))
	{
		TRACE(_T("Load (file): Error opening file %s\n"),strFilePath);
		return false;
	}
	CString strLine;
	BOOL bRet = file.ReadString(strLine);
	ASSERT(bRet);
	m_plcpointmap.Clear();
	while(file.ReadString(strLine))
	{
		DataPointEntry entry;
		vector<wstring> vecContent;
		SplitStringByChar(strLine, L',', vecContent);
		if(vecContent[0].size()>0)
		{
			entry.SetPointIndex(_wtoi(vecContent[0].c_str()));
		}
		entry.SetShortName(vecContent[1].c_str());
		entry.SetSourceType(vecContent[2].c_str());
		entry.SetDescription(vecContent[3].c_str());
		entry.SetUnit(vecContent[4].c_str());
		if(vecContent[5].size()>0)
		{
			if(vecContent[5] == L"R")
			{
				entry.SetProperty(PLC_READ);
			}
			else if(vecContent[5] == L"W")
			{
				entry.SetProperty(PLC_WRITE);
			}
			else if(vecContent[5] == L"R/W")
			{
				entry.SetProperty(PLC_PROP_MAX);
			}
		}
		entry.SetParam(1,vecContent[6].c_str());
		entry.SetParam(2,vecContent[7].c_str());
		entry.SetParam(3,vecContent[8].c_str());
		entry.SetParam(4,vecContent[9].c_str());
		entry.SetParam(5,vecContent[10].c_str());
		entry.SetParam(6,vecContent[11].c_str());
		entry.SetParam(7,vecContent[12].c_str());
		entry.SetParam(8,vecContent[13].c_str());
		entry.SetParam(9,vecContent[14].c_str());
		entry.SetParam(10,vecContent[15].c_str());
		entry.SetParam(11,vecContent[16].c_str());
		entry.SetParam(12,vecContent[17].c_str());
		entry.SetParam(13,vecContent[18].c_str());
		entry.SetParam(14,vecContent[19].c_str());

		entry.SetStoreCycle((POINT_STORE_CYCLE)_wtoi(vecContent[20].c_str()));
		if (vecContent.size() > 21)
		{
			entry.SetParam(15,vecContent[21].c_str());
			entry.SetParam(16,vecContent[22].c_str());
			entry.SetParam(17,vecContent[23].c_str());
			entry.SetParam(18,vecContent[24].c_str());
		}

		m_plcpointmap.AddEntry(entry);
	}
	file.Close();
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
	AfxMessageBox(L"�������ļ��ɹ�!");
	return true;
}

bool CDataPointConfigDlg::LoadExcelPointFile( const CString& strFilePath )
{
	CDataPointManager	pointmap;
	COperateExcelFile::Instance()->ReadFromExcel(strFilePath, pointmap);
	map<int, DataPointEntry>& datalist = pointmap.GetPointMap();

	if (datalist.size()>0)
	{
		m_plcpointmap = pointmap;
		AfxMessageBox(L"�������ļ��ɹ�!");
		return true;
	}
	return false;
}

void CDataPointConfigDlg::ExportToCSV()
{
	char* old_locale=_strdup( setlocale(LC_CTYPE,NULL) ); 
	setlocale( LC_CTYPE,"chs");
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|"), NULL);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(L"�ļ�������Ϊ��!");
			return;
		}
		CFile file;
		file.Open(strFilePath, CFile::modeCreate|CFile::modeWrite, NULL,NULL);
		CString str = _T("index,physicalid,,source,remark,Unit,RWProperty,param1,param2,param3,param4,param5,param6,param7,param8,param9,param10\r\n");
		string strBytes = UtilString::ConvertWideCharToMultiByte(str.GetString());
		file.Write(strBytes.c_str(), strBytes.size());
		int lItemCount = m_listctrl.GetItemCount();
		for ( int lIndex = 0; lIndex < lItemCount; lIndex++ )
		{
			CString tIndex = m_listctrl.GetItemText( lIndex, 0 );
			CString tphysicalID = m_listctrl.GetItemText( lIndex, 1 );
			CString tsource = m_listctrl.GetItemText( lIndex, 2 );
			CString tremark = m_listctrl.GetItemText( lIndex, 3 );
			CString tunit = m_listctrl.GetItemText( lIndex, 4 );
			CString trwproperty = m_listctrl.GetItemText( lIndex, 5 );
			CString tparam1 = m_listctrl.GetItemText( lIndex, 6 );
			CString tparam2 = m_listctrl.GetItemText( lIndex, 7 );
			CString tparam3 = m_listctrl.GetItemText( lIndex, 8 );

			CString tparam4 = m_listctrl.GetItemText( lIndex, 9 );
			CString tparam5 = m_listctrl.GetItemText( lIndex, 10 );
			CString tparam6 = m_listctrl.GetItemText( lIndex, 11 );
			CString tparam7 = m_listctrl.GetItemText( lIndex, 12 );
			CString tparam8 = m_listctrl.GetItemText( lIndex, 13 );
			CString tparam9 = m_listctrl.GetItemText( lIndex, 14 );
			CString tparam10 = m_listctrl.GetItemText( lIndex, 15 );
			
			CString strLine = tIndex+_T("|")+tphysicalID+_T("|")+tremark+_T("|")+tunit+_T("|")+tsource+_T("|")+tparam1+_T("|")+tparam2+_T("|")+tparam3+_T("|")+tparam4+_T("|")+tparam5+_T("|")+tparam6+_T("|")+tparam7+_T("|")+tparam8+_T("|")+tparam9+_T("|")+tparam10+_T("|")+trwproperty;
			string strLineBytes = UtilString::ConvertWideCharToMultiByte(strLine.GetString());
			file.Write(strLineBytes.c_str(), strLineBytes.size());
			if (lIndex != lItemCount-1)
			{
				file.Write("\r\n", 2);
			}
		}
		file.Close();
		setlocale( LC_CTYPE, old_locale );
		free( old_locale );
	}
}

bool CDataPointConfigDlg::LoadXUPPointFile( const CString& strFilePath )
{
	/*TiXmlDocument xmlDoc;
	const string strFileName = UtilString::ConvertWideCharToMultiByte(strFilePath.GetString());
	const bool ifload = xmlDoc.LoadFile(strFileName.c_str() );
	if(!ifload)
	{
		return false;
	}
	TiXmlElement* pElemRoot = xmlDoc.RootElement();
	ASSERT(pElemRoot);
	if(!pElemRoot)
	{
		return false;
	}
	TiXmlElement* pElemOpcidRoot =  pElemRoot->FirstChildElement("opcid");
	if(!pElemOpcidRoot)
	{
		AfxMessageBox(L"This version is not supported!!!");
		return false;
	}
	TiXmlElement* pElem = pElemOpcidRoot->FirstChildElement();
	DataPointEntry entry;
	const char* pchar = NULL;
	std::string str; 
	int i = 1;
	m_plcpointmap.Clear();
	while(pElem)
	{
		entry.Clear();
		entry.SetPointIndex(i++);
		str =pElem->Attribute("physicalid");
		entry.SetShortName(UtilString::ConvertMultiByteToWideChar(str));
		str =pElem->Attribute("source");
		entry.SetSourceType(UtilString::ConvertMultiByteToWideChar(str));
		str = pElem->Attribute("param1");
		entry.SetParam(1, UtilString::ConvertMultiByteToWideChar(str)) ;
		str = pElem->Attribute("param2");
		entry.SetParam(2, UtilString::ConvertMultiByteToWideChar(str)) ;
		str = pElem->Attribute("param3");
		entry.SetParam(3, UtilString::ConvertMultiByteToWideChar(str)) ;
		str = pElem->Attribute("param4");
		entry.SetParam(4, UtilString::ConvertMultiByteToWideChar(str)) ;
		str = pElem->Attribute("param5");
		entry.SetParam(5, UtilString::ConvertMultiByteToWideChar(str)) ;
		str = pElem->Attribute("param6");
		entry.SetParam(6, UtilString::ConvertMultiByteToWideChar(str)) ;
		str = pElem->Attribute("param7");
		entry.SetParam(7, UtilString::ConvertMultiByteToWideChar(str)) ;
		str = pElem->Attribute("param8");
		entry.SetParam(8, UtilString::ConvertMultiByteToWideChar(str)) ;
		str = pElem->Attribute("param9");
		entry.SetParam(9, UtilString::ConvertMultiByteToWideChar(str)) ;
		str = pElem->Attribute("param10");
		entry.SetParam(10, UtilString::ConvertMultiByteToWideChar(str)) ;
		pchar = pElem->Attribute("RWProperty");
		if(pchar)
		{
			str = pchar;
			entry.SetProperty((str == ("R")) ? PLC_READ : PLC_WRITE);
		}
		else
		{
			entry.SetProperty(PLC_READ);
		}
		str = pElem->Attribute("remark");
		entry.SetDescription(UtilString::ConvertMultiByteToWideChar(str));
		pchar = pElem->Attribute("Unit");
		if(pchar)
		{
			str = pchar;  
		}
		else
		{
			str = "";            
		}
		std::wstring wstr = UtilString::ConvertMultiByteToWideChar(str);
		entry.SetUnit(wstr);
		m_plcpointmap.AddEntry(entry);
		pElem = pElem->NextSiblingElement("Data");
	}*/
	return true;
}



//�������������
void CDataPointConfigDlg::OnBnClickedButtonAddvpt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ParsePoints();	
	ShowList(m_allentrylist);
	CBatchPointsDlg dlg(this);
	dlg.SetDlgType(enum_Add);
	dlg.SetPointMap(&m_plcpointmap);

	if(dlg.DoModal() == IDOK)
	{
		for(int nReallNum=0;nReallNum<dlg.m_nReallNum;nReallNum++)
		{
			///*����Ƿ��ظ�*/
			//for (int i=0;i<m_allentrylist.size();i++)
			//{
			//	wstring str=m_allentrylist[i].GetShortName();
			//	CString strExistPointName = str.c_str();
			//	int bEqual=strExistPointName.CompareNoCase(dlg.m_TableOfName.at(nReallNum));
			//	if (bEqual==0)
			//	{
			//		AfxMessageBox(_T("�Ѵ��ڸõ������ظ���"));
			//		return;
			//	}
			//	else
			//	{
			//		continue;
			//	}
			//}

			const int nCount = m_listctrl.GetItemCount();
			CString strIndex;
			strIndex.Format(L"%d", nCount+1);
			m_listctrl.InsertItem(nCount, strIndex);
			m_listctrl.SetItemText(nCount, 1, dlg.m_TableOfName.at(nReallNum));
			m_listctrl.SetItemText(nCount, 2, L"vpoint");
			m_listctrl.SetItemText(nCount, 3, dlg.m_TableOfNotation.at(nReallNum));
			m_listctrl.SetItemText(nCount, 4, L"");
			m_listctrl.SetItemText(nCount, 5, L"W");
			m_listctrl.SetItemText(nCount, 6, L"");
			m_listctrl.SetItemText(nCount, 7, L"");
			m_listctrl.SetItemText(nCount, 8, L"");
			m_listctrl.SetItemText(nCount, 9, L"");
			m_listctrl.SetItemText(nCount, 10, L"");
			m_listctrl.SetItemText(nCount, 11, L"");
			m_listctrl.SetItemText(nCount, 12, L"");
			m_listctrl.SetItemText(nCount, 13, L"");
			m_listctrl.SetItemText(nCount, 14, L"");
			m_listctrl.SetItemText(nCount, 15, L"");
			m_listctrl.SetItemText(nCount, 16, L"");
			m_listctrl.SetItemText(nCount, 17, L"0.0");
			m_listctrl.SetItemText(nCount, 18, L"0.0");
			m_listctrl.SetItemText(nCount, 19, L"0");
			m_listctrl.SetItemText(nCount, 20, L"1����");
			m_listctrl.SetItemText(nCount, 21, L"");
			m_listctrl.SetItemText(nCount, 22, L"");
			m_listctrl.SetItemText(nCount, 23, L"");
			m_listctrl.SetItemText(nCount, 24, L"");
			m_listctrl.EnsureVisible(m_listctrl.GetItemCount()-1,FALSE);

			GetDlgItem(IDC_EDIT_SEARCH)->SetWindowTextW(_T(""));
			m_bNeedSave = true;
			MakeAllList();
		}
		return ;
	}
}

void CDataPointConfigDlg::IsPointMapValid(CDataPointManager& plcpointmap, vector<CString>& vecName)
{
	BeginWaitCursor();
	bool bRet = true;
	map<int, DataPointEntry>& datalist = m_plcpointmap.GetPointMap();
	map<int, DataPointEntry>::iterator iter = datalist.begin();
	for (;iter!=datalist.end();++iter)
	{
		CString strName = iter->second.GetShortName().c_str();
		map<int, DataPointEntry>::iterator iter_temp = datalist.begin();
		for (;iter_temp!=datalist.end();++iter_temp)
		{
			if (iter_temp == iter)
			{
				continue;
			}
			CString strShortName = iter_temp->second.GetShortName().c_str();
			if (strShortName.CompareNoCase(strName) == 0)
			{
				bool bFind = false;
				for (size_t i=0;i<vecName.size();++i)
				{
					if (vecName[i] == strName)
					{
						bFind = true;
					}
				}
				if (!bFind)
				{
					vecName.push_back(strName);
				}
			}
		}
	}
}

void CDataPointConfigDlg::ClearListAll()
{
	m_listctrl.DeleteAllItems();
	m_allentrylist.clear();
	m_searchlist.clear();
}

BOOL CDataPointConfigDlg::ShowWindow( int nCmdShow )
{
	switch (nCmdShow)
	{
	case SW_SHOW:
	case SW_SHOWNA:
	case SW_SHOWNORMAL:
		{
			ParsePoints();
			//cursel
			switch (1)
			{
			case SHOW_ALL:
				ShowList(m_allentrylist);
				break;
			case SHOW_FIND:
				ShowList(m_searchlist);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return CDialog::ShowWindow(nCmdShow);
}

void CDataPointConfigDlg::MakeAllList()
{
	ParsePoints();
	/*MakeOPCList();
	MakeModbusList();
	MakeBacnetList();
	MakeCustomList();*/

	CString strSearch;
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowTextW(strSearch);
	if (strSearch.GetLength() != 0)
	{
		MakeSearchList(strSearch);
	}
}

const CString CDataPointConfigDlg::GetStoreCycle( const POINT_STORE_CYCLE cycle )
{
	switch (cycle)
	{
	case E_STORE_NULL:
		return L"���洢";
	case E_STORE_FIVE_SECOND:
		return L"5����";
	case E_STORE_ONE_MINUTE:
		return L"1����";
	case E_STORE_FIVE_MINUTE:
		return L"5����";
	case E_STORE_HALF_HOUR:
		return L"��Сʱ";
	case E_STORE_ONE_HOUR:
		return L"1Сʱ";
	case E_STORE_ONE_DAY:
		return L"1��";
	case E_STORE_ONE_WEEK:
		return L"1�ܣ�7�죩";
	case E_STORE_ONE_MONTH:
		return L"1����";
	case E_STORE_ONE_YEAR:
		return L"1��";
	default:
		return L"���洢";
	}
	return L"���洢";
}

const POINT_STORE_CYCLE CDataPointConfigDlg::GetStoreCycle(const CString str)
{
	if (str == L"���洢")
	{
		return E_STORE_NULL;
	}
	else if (str == L"5����")
	{
		return E_STORE_FIVE_SECOND;
	}
	else if (str == L"1����")
	{
		return E_STORE_ONE_MINUTE;
	}
	else if (str == L"5����")
	{
		return E_STORE_FIVE_MINUTE;
	}
	else if (str == L"��Сʱ")
	{
		return E_STORE_HALF_HOUR;
	}
	else if (str == L"1Сʱ")
	{
		return E_STORE_ONE_HOUR;
	}
	else if (str == L"1��")
	{
		return E_STORE_ONE_DAY;
	}
	else if (str == L"1�ܣ�7�죩")
	{
		return E_STORE_ONE_WEEK;
	}
	else if (str == L"1����")
	{
		return E_STORE_ONE_MONTH;
	}
	else if (str == L"1��")
	{
		return E_STORE_ONE_YEAR;
	}
	return E_STORE_NULL;
}


void CDataPointConfigDlg::OnNMRClickListctrlPoint(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	CMenu* pSubMenu = m_menu.GetSubMenu(0);
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	*pResult = 0;
}


void CDataPointConfigDlg::OnSetStoreCycle()
{
	CStoreCycleSettingDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		const POINT_STORE_CYCLE cycle = (POINT_STORE_CYCLE)dlg.m_nCycle;
		vector<int> vec;
		POSITION pos = m_listctrl.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_listctrl.GetNextSelectedItem(pos);
			CString strName = m_listctrl.GetItemText(nItem,1);
			map<int, DataPointEntry>& mapping = m_plcpointmap.GetPointMap();
			map<int, DataPointEntry>::iterator it = mapping.begin();
			while (it != mapping.end())
			{
				if (it->second.GetShortName() == strName.GetString())
				{
					it->second.SetStoreCycle(cycle);
					break;
				}
				++it;
			}
			m_listctrl.SetItemText(nItem,20,GetStoreCycle(cycle));
		}
		m_bNeedSave = true;
	}
}

void CDataPointConfigDlg::OnMenuSetVpoint()
{
	// TODO: �ڴ���������������
	POSITION pos = m_listctrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("No items were selected!\n"));
		return;
	}

	while (pos != NULL)
	{
		int nItem = m_listctrl.GetNextSelectedItem(pos);
		CString strName = m_listctrl.GetItemText(nItem,1);
		map<int, DataPointEntry>& mapping = m_plcpointmap.GetPointMap();
		map<int, DataPointEntry>::iterator it = mapping.begin();
		while (it != mapping.end())
		{
			if (it->second.GetShortName() == strName.GetString())
			{
				it->second.SetSourceType(L"vpoint");
				break;
			}
			++it;
		}
		m_listctrl.SetItemText(nItem, 2, _T("vpoint"));
	}
	m_bNeedSave = true;
}

void CDataPointConfigDlg::OnBnClickedButtonPointSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_bNeedSave)
	{
		AfxMessageBox(_T("���豣�棡"));
		return;
	}

	//if (!CheckPointTable(false))
	//{
	//	return;
	//}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_plcpointmap = m_plcpointmap;
	//pFrame->SaveProjectFile();
	if(m_wstrDelPointList.size() > 0)
		pFrame->m_pPrjSqlite->DeletePoints(m_wstrDelPointList);
	if(pFrame->m_pPrjSqlite->SaveProjectSqlite_opcid3())
	{
		if(pFrame->m_wndFileView.GetDataAccess())
		{
			pFrame->m_wndFileView.GetDataAccess()->WriteCoreDebugItemValue(_T("core_reload_point_table"), _T("1"));
		}
		Iinit_ComboBox_POINT_TYPE_SORT();
		AfxMessageBox(L"������ɣ�");
		m_bNeedSave = false;
	}
	else
	{
		AfxMessageBox(_T("����ʧ�ܣ�"));
	}
}


void CDataPointConfigDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_bNeedSave)
	{
		const int nRet = MessageBox(L"���޸�,�Ƿ񱣴���˳���",L"��ʾ",MB_YESNOCANCEL);
		if (IDYES == nRet)
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			pFrame->m_plcpointmap = m_plcpointmap;
			if(pFrame->m_pPrjSqlite->SaveProjectSqlite_opcid3())
			{
				AfxMessageBox(L"������ɣ�");
				m_bNeedSave = false;
			}
		}
		else if (IDNO == nRet)
		{
		}
		else if (IDCANCEL == nRet)
		{
			return;
		}
		else
		{
			return;
		}
	}
	m_bNeedSave = false;
	CDialog::OnCancel();
}

void CDataPointConfigDlg::OnBnClickedClose()
{
	if(m_bNeedSave)
	{
		const int nRet = MessageBox(L"���޸�,�Ƿ񱣴���˳���",L"��ʾ",MB_YESNOCANCEL);
		if (IDYES == nRet)
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			pFrame->m_plcpointmap = m_plcpointmap;
			if(pFrame->m_pPrjSqlite->SaveProjectSqlite_opcid3())
			{
				AfxMessageBox(L"������ɣ�");
				m_bNeedSave = false;
			}
		}
		else if (IDNO == nRet)
		{
		}
		else if (IDCANCEL == nRet)
		{
			return;
		}
		else
		{
			return;
		}
	}
	m_bNeedSave = false;
	CDialog::OnCancel();
}


//void CDataPointConfigDlg::OnBnClickedBtnChkPoint()
//{
	// TODO: Add your control notification handler code here
	//CheckPointByType();
//}

bool CDataPointConfigDlg::CheckPointTable(const bool bIsCalInCheck)
{
	// check m_allentrylist
	CString	strErr;
	CString	strErrTitle(_T("����"));
	UINT	nErrType = MB_OK | MB_ICONERROR;

	map<int, DataPointEntry>& mapping = m_plcpointmap.GetPointMap();
	map<int, DataPointEntry>::const_iterator	iter;
	DataPointEntry	dataPoint;
	wstring			wstrName;
	bool			bFindError = false;

	for (iter=mapping.begin(); iter!=mapping.end(); ++iter)
	{
		dataPoint = iter->second;

		// source type
		wstrName = dataPoint.GetSourceType();
		if (!PointTypeValid(wstrName))
		{
			bFindError = true;
			strErr.Format(_T("���%d��Դ��������ƴд����ȷ��"), dataPoint.GetPointIndex());
			MessageBox(strErr, strErrTitle, nErrType);
			break;
		}

		// ip address
		if (SIMENS1200TCP == wstrName || SIMENS300TCP == wstrName)
		{
			CString strIp;
			strIp.Format(_T("%s"), dataPoint.GetParam(3).c_str());

			CPointParamEditDlg dlg;
			if (!dlg.CheckIpAddr(strIp))
			{
				bFindError = true;
				strErr.Format(_T("���%d��simense300/1200TCP�����ַ��ʽ����ȷ��"), dataPoint.GetPointIndex());
				MessageBox(strErr, strErrTitle, nErrType);
				break;
			}
		}
		else if (L"modbus" == wstrName)
		{
			CString strIp;
			strIp.Format(_T("%s"), dataPoint.GetParam(5).c_str());

			CPointParamEditDlg dlg;
			if (!dlg.CheckIpAddr(strIp))
			{
				bFindError = true;
				strErr.Format(_T("���%d��modbus�����ַ��ʽ����ȷ��"), dataPoint.GetPointIndex());
				MessageBox(strErr, strErrTitle, nErrType);
				break;
			}
		}

		// max/min value
		if (_wtoi((dataPoint.GetParam(12)).c_str()) < _wtoi((dataPoint.GetParam(13)).c_str()))
		{
			bFindError = true;
			strErr.Format(_T("���%d���ֵС����Сֵ��"), dataPoint.GetPointIndex());
			MessageBox(strErr, strErrTitle, nErrType);
			break;
		}
	}

	// no wrong report
	if (!bFindError)
	{
		if (bIsCalInCheck)
		{
			MessageBox(_T("δ�����쳣"), _T("�������"), MB_OK|MB_ICONQUESTION);
		}
	}


	return !bFindError;
}

bool CDataPointConfigDlg::PointTypeValid(wstring wstrType)
{
	if (PH370 != wstrType
		&& SIMENS1200 != wstrType
		&& SIMENS300 != wstrType
		&& SIMENS1200TCP != wstrType
		&& SIMENS300TCP != wstrType
		&& L"simense1200tcp" != wstrType
		&& L"simense300tcp" != wstrType
		&& AB500 != wstrType
		&& HONEYWELLEBI != wstrType
		&& L"ModbusDirecctReadInServer" !=wstrType
		&& L"ModbusEquipment" !=wstrType
		&& L"bacnet-py" !=wstrType
		&& L"PersagyController" !=wstrType
		&& L"ThirdParty" !=wstrType
		&& L"modbus" != wstrType
		&& L"vpoint" != wstrType
		&& L"bacnet" != wstrType
		&& L"protocol104" != wstrType
		&& L"lonworks" != wstrType
		&& L"DB-Access" != wstrType
		&& L"DB-SQLServer" != wstrType
		&& L"DB-Oracle" != wstrType
		&& L"DB-MySQL" != wstrType
		&& L"custom1" != wstrType
		&& L"custom2" != wstrType
		&& L"custom3" != wstrType
		&& L"custom4" != wstrType
		&& L"custom5" != wstrType
		&& L"dateTime" != wstrType
		&& L"DanfossFCProtocol" != wstrType
		&& L"Insight" != wstrType
		&& L"WinCC1" != wstrType
		&& L"KingView1" != wstrType
		&& L"ArchestrA3" != wstrType
		&& L"KEPware4" != wstrType
		&& L"wireless" != wstrType
		&& L"bacnet-mstp" != wstrType
		&& L"OPCUA" !=wstrType
		&& L"obix" !=wstrType
		&& L"logix" !=wstrType
		&& L"MoxaTCPServer" !=wstrType)
		return false;

	return true;
}

bool CDataPointConfigDlg::CheckPoint( const vector<DataPointEntry>& entrylist )
{
	CString	strErr;
	CString	strErrTitle(_T("����"));
	UINT	nErrType = MB_OK | MB_ICONERROR;

	DataPointEntry	dataPoint;
	wstring			wstrName;
	bool			bFindError = false;

	for(int i=0; i<entrylist.size(); ++i)
	{
		dataPoint = entrylist[i];

		// source type
		wstrName = dataPoint.GetSourceType();
		if(!PointTypeValid(wstrName))
		{
			bFindError = true;
			strErr.Format(_T("���%d��Դ��������ƴд����ȷ��"), dataPoint.GetPointIndex());
			MessageBox(strErr, strErrTitle, nErrType);
			break;
		}

		// ip address
		if (SIMENS1200TCP == wstrName || SIMENS300TCP == wstrName)
		{
			CString strIp;
			strIp.Format(_T("%s"), dataPoint.GetParam(3).c_str());

			CPointParamEditDlg dlg;
			if (!dlg.CheckIpAddr(strIp))
			{
				bFindError = true;
				strErr.Format(_T("���%d��simense300/1200TCP�����ַ��ʽ����ȷ��"), dataPoint.GetPointIndex());
				MessageBox(strErr, strErrTitle, nErrType);
				break;
			}
		}
		else if (L"modbus" == wstrName)
		{
			CString strIp;
			strIp.Format(_T("%s"), dataPoint.GetParam(5).c_str());

			CPointParamEditDlg dlg;
			if (!dlg.CheckIpAddr(strIp))
			{
				bFindError = true;
				strErr.Format(_T("���%d��modbus�����ַ��ʽ����ȷ��"), dataPoint.GetPointIndex());
				MessageBox(strErr, strErrTitle, nErrType);
				break;
			}
		}

		// max/min value
		if (_wtoi((dataPoint.GetParam(12)).c_str()) < _wtoi((dataPoint.GetParam(13)).c_str()))
		{
			bFindError = true;
			strErr.Format(_T("���%d���ֵС����Сֵ��"), dataPoint.GetPointIndex());
			MessageBox(strErr, strErrTitle, nErrType);
			break;
		}
	}

	// no wrong report
	if (!bFindError)
	{
		MessageBox(_T("δ�����쳣"), _T("�������"), MB_OK|MB_ICONQUESTION);
	}
	return !bFindError;
}


 void CDataPointConfigDlg::AddDeviceTogether(int nDeviceNum,int nDeviceStartNum,CString nRoomNameDevice,CString strFilePath)
 {
	 for(int i=0;i<nDeviceNum;i++,nDeviceStartNum++)
	 {
		 CString strDir;
		 int nDevicecount =nDeviceStartNum;
		 CStdioFile ff;
		 vector<CString> strPointNameList;
		 //vector<CString> strDescriptionList;

		 if (!ff.Open(strFilePath,CFile::modeRead | CFile::shareDenyWrite))
		 {	
			 return ;
		 }

		 CString strLine;
		 CString strID;
		 int nChillernumber = i+1;
		 strID.Format(L"%02d", nDevicecount );//���ӵĵ�ֵ
		 CString strExpression;
		 strExpression.Format(L"%d#",nDevicecount );	
		 while(ff.ReadString(strLine))
		 {
			 vector<wstring >vecContent;
			 SplitStringByChar(strLine, L',', vecContent);
			 CString point_template_ctrl;
			 if(vecContent.size()>=2)
			 {
				 const int nCount = m_listctrl.GetItemCount();
				 CString strFinalName = nRoomNameDevice+vecContent[0].c_str()+strID;
				 strPointNameList.push_back(strFinalName);
				 CString strFinalExpression=strExpression+vecContent[1].c_str();
				 int nIndex = GetMaxKeyID()+1;
				 CString strIndex;
				 strIndex.Format(L"%d", nIndex);
				 map<int, DataPointEntry>& datalist = m_plcpointmap.GetPointMap();
				 map<int, DataPointEntry>::iterator iter = datalist.begin();
				 for(;iter != datalist.end(); iter++)
				 {
					 const DataPointEntry& entryMap = iter->second;
					 if (entryMap.GetShortName().c_str() == strFinalName)
					 {
						 AfxMessageBox(L"�����ظ�!");
						 return;
					 }

				 }
				 strIndex.Format(L"%d", nIndex);
				 m_listctrl.InsertItem(nCount, strIndex);
				 m_listctrl.SetItemText(nCount, 1, strFinalName);
				 m_listctrl.SetItemText(nCount, 2, L"vpoint");
				 m_listctrl.SetItemText(nCount, 3, strFinalExpression);
				 m_listctrl.SetItemText(nCount, 4, L"");
				 m_listctrl.SetItemText(nCount, 5, L"R");
				 m_listctrl.SetItemText(nCount, 6, L"");
				 m_listctrl.SetItemText(nCount, 7, L"");
				 m_listctrl.SetItemText(nCount, 8, L"");
				 m_listctrl.SetItemText(nCount, 9, L"");
				 m_listctrl.SetItemText(nCount, 10, L"");
				 m_listctrl.SetItemText(nCount, 11, L"");
				 m_listctrl.SetItemText(nCount, 12, L"");
				 m_listctrl.SetItemText(nCount, 13, L"");
				 m_listctrl.SetItemText(nCount, 14, L"");
				 m_listctrl.SetItemText(nCount, 15, L"");
				 m_listctrl.SetItemText(nCount, 16, L"-9999");
				 m_listctrl.SetItemText(nCount, 17, L"0");
				 m_listctrl.SetItemText(nCount, 18, L"0");
				 m_listctrl.SetItemText(nCount, 19, L"0");
				 m_listctrl.SetItemText(nCount, 20, L"���洢");
				 m_listctrl.SetItemText(nCount, 21, L"");
				 m_listctrl.SetItemText(nCount, 22, L"");
				 m_listctrl.SetItemText(nCount, 23, L"");
				 m_listctrl.SetItemText(nCount, 24, L"");
				 DataPointEntry entry;

				 const size_t size = datalist.size(); 
				 entry.SetPointIndex(nIndex);
				 entry.SetShortName(strFinalName.GetString());
				 entry.SetSourceType(L"vpoint");
				 entry.SetDescription(strFinalExpression.GetString());
				 entry.SetUnit(L"");
				 entry.SetProperty(PLC_READ);
				 entry.SetParam(1, L"");
				 entry.SetParam(2, L"");
				 entry.SetParam(3, L"");
				 entry.SetParam(4, L"");
				 entry.SetParam(5, L"");
				 entry.SetParam(6, L"");
				 entry.SetParam(7, L"");
				 entry.SetParam(8, L"");
				 entry.SetParam(9, L"");
				 entry.SetParam(10, L"");
				 entry.SetHighAlarm(-9999);
				 entry.SetParam(12, L"0");
				 entry.SetParam(13, L"0");
				 entry.SetParam(14, L"0");
				 entry.SetStoreCycle(E_STORE_NULL);
				 entry.SetParam(15, L"");
				 entry.SetParam(16, L"");
				 entry.SetParam(17, L"");
				 entry.SetParam(18, L"");
				 AddOneEntry(entry);
			 }

		 }
		 ff.Close(); 

	 }
 }


 void CDataPointConfigDlg::OnBnClickedButtonShowall()
 {
	 // TODO: Add your control notification handler code here
	 ShowList(m_allentrylist);
	 m_cbb_pointTypeSort.SetCurSel(0);
 }


 void CDataPointConfigDlg::OnBnClickedBtnAutogenPointname()
 {
	 // TODO: Add your control notification handler code here

	 CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	 int nId = -1;
	 CString strSrc;
	 CString strResult;
	 CHttpOperation http(pFrame->m_strCloudServiceIp, pFrame->m_nCloudServicePort, _T("/api/pointtool/transferPointName"));

	 wstring wstrTimeStart,wstrTimeEnd ;

	 Json::Value myRoot;
	 Json::Value nameData;
	 Json::Value desData;
	 vector<int> nIndexList;

	 POSITION pos = m_listctrl.GetFirstSelectedItemPosition();

	 if (pos == NULL)

	 {

		 AfxMessageBox(_T("û��ѡ���κ���"));
		 return;

	 }
	 else

	 {

		 while (pos)

		 {

			 int nItem = m_listctrl.GetNextSelectedItem(pos);
			 CString strTemp = m_listctrl.GetItemText(nItem, 1);

			 string strTemp_utf8;
			 Project::Tools::WideCharToUtf8(strTemp.GetString(), strTemp_utf8);
			 nameData.append(strTemp_utf8);

			 strTemp = m_listctrl.GetItemText(nItem, 3);
			 Project::Tools::WideCharToUtf8(strTemp.GetString(), strTemp_utf8);
			 desData.append(strTemp_utf8);

			 nIndexList.push_back(nItem);
		 }
	 }


	 myRoot["descriptionList"] = desData;
	 myRoot["nameList"] = nameData;


	 string szBuf = myRoot.toStyledString();

	 bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
	 if(bRet)
	 {
		 Json::Reader jsReader;
		 string strResult_utf8;
		 Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
		 Json::Value myReturnRoot;
		 if (!jsReader.parse(strResult_utf8, myReturnRoot))
		 {
			 AfxMessageBox(_T("��׼�����Զ�����ʧ�ܣ�ԭ����ע����Ϣ�����"));
			 return;
		 }
		 else
		 {
			 if(myReturnRoot.isObject())
			 {
				 Json::Value dataObj = myReturnRoot["data"];

				 if(dataObj.isArray())
				 {
					 map<int, DataPointEntry> &ppmap = m_plcpointmap.GetPointMap();

					 for(int tt=0;tt<dataObj.size();tt++)
					 {
						 string strPointName = dataObj[Json::UInt(tt)].asString();
						 CString strTemp;
						 wstring wstrPointName;
						 Project::Tools::UTF8ToWideChar(strPointName, wstrPointName);
						 strTemp = wstrPointName.c_str();

						 int nIndex = nIndexList[tt];

						 m_listctrl.SetItemText(nIndex, 1, strTemp);

						 DataPointEntry &dd = ppmap[nIndex+1];
						 ppmap[nIndex+1].SetShortName(wstrPointName);
					 }




					 m_listctrl.Invalidate(TRUE);
					 m_bNeedSave = true;
					 AfxMessageBox(_T("��׼�����Զ����ɳɹ�!"));




				 }
			 }
		 }
	 }
	 else
	 {
		 AfxMessageBox(_T("��׼�����Զ�����ʧ�ܣ��������������Ƿ�����"));
		 return;
	 }



	 CenterWindow();
 }


 void CDataPointConfigDlg::OnBnClickedButtonDownloadTemplate()
 {
	 // TODO: Add your control notification handler code here
	 CString strPointTableTemplateFile;

	 CString strTemplatePath;
	 wstring wstr = _T("");
	 CString strDir;
	 TCHAR szAppDir[MAX_PATH] = {0};
	 GetModuleFileName(NULL, szAppDir, MAX_PATH);
	 PathRemoveFileSpec(szAppDir);
	 CString strFilePath;
	 strFilePath.Format(_T("%s\\config\\point_table_template.xlsx"), szAppDir);
	 string strKey_utf8;

	 COleDateTime tnow = COleDateTime::GetCurrentTime();
	 CString cstr_file_name;
	 cstr_file_name.Format(_T("point_table_template_copy_%s.xlsx"),  tnow.Format(_T("%Y%m%d_%H%M")));
	 CFileDialog dlg(FALSE, _T(""), cstr_file_name, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, _T("*.xlsx"));
	 //dlg.m_ofn.lpstrTitle = _T("�����ļ�");


	 CString strNewFilePath;
	 if ( IDOK == dlg.DoModal() )
	 {	
		 
		 CString DisPath;
		 DisPath=  dlg.GetPathName()	;

		 BOOL bSuccess = CopyFile(strFilePath.GetString(),DisPath.GetString(),FALSE);

		 if(bSuccess)
		 {
			 //string str_utf8;
			 //Project::Tools::WideCharToUtf8(strNewFilePath.GetString(), str_utf8);
			 try{
				 //LPCSTR lpstr = (LPCSTR)str_utf8.data();
				 ::ShellExecute(NULL,L"open",DisPath,NULL,NULL,SW_SHOWNORMAL);
			 }
			 catch(CException *e)
			 {
				 int xx=0;
			 }

		 }
		 else
		 {
			 AfxMessageBox(_T("ģ�治���ڻ�����ʧ�ܣ�������������汾��"));
		 }
	 }








 }


 void CDataPointConfigDlg::OnBnClickedButtonPointIgnore()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 CPointIgnoreDefDlg dlg(this);
	 CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	 map<int, DataPointEntry> pointMap= m_plcpointmap.GetPointMap();//��ȡ���

	 if(pFrame->m_wndFileView.GetDataAccess())
	 {
		 wstring strInitValue = pFrame->m_wndFileView.GetDataAccess()->ReadOrCreateCoreDebugItemValue(_T("point_ignore_list"));

		 dlg.m_point_ignore_list = strInitValue.c_str();

		 //��ȡ���б�ѡ�еĵ�Ķ���
		 POSITION pos = m_listctrl.GetFirstSelectedItemPosition();
		 if (pos == NULL)
		 {
			 ;
		 }else
		 {
			 if(dlg.m_point_ignore_list == L"0")
				 dlg.m_point_ignore_list.Empty();
		 }
		 while(pos)
		 {
			 int nItem = m_listctrl.GetNextSelectedItem(pos);
			 CString strItem = m_listctrl.GetItemText(nItem,0);
			 CString strName = m_listctrl.GetItemText(nItem,1);
			 int nnItem = _wtoi(strItem.GetString());

			 map<int, DataPointEntry>::iterator iter;
			 iter = pointMap.find(nnItem);
			 if(iter!= pointMap.end())
			 {
				 if(dlg.m_point_ignore_list.GetLength() != 0)
					dlg.m_point_ignore_list += L",";
				 dlg.m_point_ignore_list += iter->second.GetShortName().c_str();
			 }
		 }

		 if(dlg.DoModal()==IDOK)
		 {
			 pFrame->m_wndFileView.GetDataAccess()->WriteCoreDebugItemValue(_T("point_ignore_list"), dlg.m_point_ignore_list.GetString());
		 }
	 }
	 else
	 {
		 AfxMessageBox(L"�����������ߵ���");
	 }
	 RefreshIgnorePointList();


	 /*unit01��
	 unitproperty01=  'point_ignore_list'
		 unitproperty02 = �Զ��ż�������ε��嵥

		 */


 }


 void CDataPointConfigDlg::OnBnClickedButtonPointCopy()
 {
	//// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_listctrl.GetFirstSelectedItemPosition();
	int nItem = 0;
	if (pos == NULL)
	{
		AfxMessageBox(L"��ѡ������һ��");
	}
	else
	{
		while (pos)
		{
			nItem = m_listctrl.GetNextSelectedItem(pos);
			TRACE(_T("Item %d was selected!\n"), nItem);
			
			/*
			**	���б��л�ȡ���ݲ��������ظ�
			*/
			CString strSetShortName			= m_listctrl.GetItemText(nItem, 1) + L"_Copy";//���б��л�ȡ�����͵�һ��ƴ��
			
			//�����ظ���ѯ��ƴ�ӵ���
			map<int, DataPointEntry>& datalist = m_plcpointmap.GetPointMap();
			map<int, DataPointEntry>::const_iterator it = datalist.begin();
			int nRepeat = 1;//�ظ�����
			CString cstrCompare = strSetShortName.GetString();//��ȡԭʼ����
			while(it != datalist.end())
			{
				cstrCompare.Format(L"%s_%d",strSetShortName,nRepeat);//ʹ���ظ�������ϵ���
				wstring wstrCompare = cstrCompare.GetString();
				for (;it != datalist.end(); it++)
				{
					const DataPointEntry& entryMap = it->second;
					if (entryMap.GetShortName() == wstrCompare)
					{
						nRepeat++;//�����ظ�����
						break;
					}
				}
			}
			strSetShortName = cstrCompare;//��ȡ��Ϻõĵ���

			CString strSetShortNameOrigin	= strSetShortName;//������������Ϊ�ظ���
			CString strSetSourceType		= m_listctrl.GetItemText(nItem, 2);
			CString strSetDescription		= m_listctrl.GetItemText(nItem, 3);
			CString strSetUnit				= m_listctrl.GetItemText(nItem, 4);
			CString strSetProperty			= m_listctrl.GetItemText(nItem, 5);
			CString strSetParam1  = m_listctrl.GetItemText(nItem, 6);
			CString strSetParam2  = m_listctrl.GetItemText(nItem, 7);
			CString strSetParam3  = m_listctrl.GetItemText(nItem, 8);
			CString strSetParam4  = m_listctrl.GetItemText(nItem, 9);
			CString strSetParam5  = m_listctrl.GetItemText(nItem, 10);
			CString strSetParam6  = m_listctrl.GetItemText(nItem, 11);
			CString strSetParam7  = m_listctrl.GetItemText(nItem, 12);
			CString strSetParam8  = m_listctrl.GetItemText(nItem, 13);
			CString strSetParam9  = m_listctrl.GetItemText(nItem, 14);
			CString strSetParam10 = m_listctrl.GetItemText(nItem, 15);
			CString strSetParam11 = m_listctrl.GetItemText(nItem, 16);
			CString strSetParam12 = m_listctrl.GetItemText(nItem, 17);
			CString strSetParam13 = m_listctrl.GetItemText(nItem, 18);
			CString strSetParam14 = m_listctrl.GetItemText(nItem, 19);
			CString strSetStoreCycle = m_listctrl.GetItemText(nItem, 20);
			CString strSetCustomName = m_listctrl.GetItemText(nItem, 21);
			CString strSetSystem = m_listctrl.GetItemText(nItem, 22);
			CString strSetDevice = m_listctrl.GetItemText(nItem, 23);
			CString strSetType	 = m_listctrl.GetItemText(nItem, 24);

			/*
			**	�������ݵ����
			*/
			DataPointEntry entry;
			entry.SetPointIndex(GetMaxKeyID()+1);
			entry.SetShortName(strSetShortName.GetString());
			entry.SetShortNameOrigin(strSetShortNameOrigin.GetString());
			entry.SetSourceType( strSetSourceType.GetString() );
			entry.SetDescription( strSetDescription.GetString() );
			entry.SetUnit( strSetUnit.GetString() );
			if(strSetProperty == L"R")
			{
				entry.SetProperty(PLC_READ);
			}
			else if(strSetProperty == L"W")
			{
				entry.SetProperty(PLC_WRITE);
			}
			else if(strSetProperty == L"R/W")
			{
				entry.SetProperty(PLC_PROP_MAX);
			}
			entry.SetParam(1, strSetParam1.GetString() );
			entry.SetParam(2, strSetParam2.GetString() );
			entry.SetParam(3, strSetParam3.GetString() );
			entry.SetParam(4, strSetParam4.GetString() );
			entry.SetParam(5, strSetParam5.GetString() );
			entry.SetParam(6, strSetParam6.GetString() );
			entry.SetParam(7, strSetParam7.GetString() );
			entry.SetParam(8, strSetParam8.GetString() );
			entry.SetParam(9, strSetParam9.GetString() );
			entry.SetParam(10, strSetParam10.GetString() );
			entry.SetParam(11, strSetParam11.GetString() );
			entry.SetParam(12, strSetParam12.GetString() );
			entry.SetParam(13, strSetParam13.GetString() );
			entry.SetParam(14, strSetParam14.GetString() );
			POINT_STORE_CYCLE nTemp;
			if (strSetStoreCycle == L"���洢")	nTemp = E_STORE_NULL;
			else if (strSetStoreCycle == L"5����")	nTemp =E_STORE_FIVE_SECOND;
			else if (strSetStoreCycle == L"1����")	nTemp = E_STORE_ONE_MINUTE;
			else if (strSetStoreCycle == L"5����")	nTemp = E_STORE_FIVE_MINUTE;
			else if (strSetStoreCycle == L"��Сʱ")	nTemp =  E_STORE_HALF_HOUR;
			else if (strSetStoreCycle == L"1Сʱ")	nTemp =  E_STORE_ONE_HOUR;
			else if (strSetStoreCycle == L"1��")	nTemp =  E_STORE_ONE_DAY;		
			else if (strSetStoreCycle == L"1�ܣ�7�죩")	nTemp =  E_STORE_ONE_WEEK;
			else if (strSetStoreCycle == L"1����")	nTemp =  E_STORE_ONE_MONTH;			
			else if (strSetStoreCycle == L"1��")	nTemp =  E_STORE_ONE_YEAR;
			else nTemp = E_STORE_NULL;
			entry.SetStoreCycle(nTemp);
			entry.SetParam(15, strSetCustomName.GetString());
			entry.SetParam(16, strSetSystem.GetString());
			entry.SetParam(17, strSetDevice.GetString());
			entry.SetParam(18, strSetType.GetString());
			AddOneEntry(entry);//��ӵ����
			
			/*
			** ��������ʾ���б�
			*/
			const int nCount = m_listctrl.GetItemCount();
			CString strIndex;
			strIndex.Format(L"%d", nCount+1);
			m_listctrl.InsertItem(nCount, strIndex);
			m_listctrl.SetItemText(nCount, 1, strSetShortName);
			m_listctrl.SetItemText(nCount, 2, strSetSourceType);
			m_listctrl.SetItemText(nCount, 3, strSetDescription);
			m_listctrl.SetItemText(nCount, 4, strSetUnit);
			m_listctrl.SetItemText(nCount, 5, strSetProperty);
			m_listctrl.SetItemText(nCount, 6, strSetParam1);
			m_listctrl.SetItemText(nCount, 7, strSetParam2);
			m_listctrl.SetItemText(nCount, 8, strSetParam3);
			m_listctrl.SetItemText(nCount, 9, strSetParam4);
			m_listctrl.SetItemText(nCount, 10, strSetParam5);
			m_listctrl.SetItemText(nCount, 11, strSetParam6);
			m_listctrl.SetItemText(nCount, 12, strSetParam7);
			m_listctrl.SetItemText(nCount, 13, strSetParam8);
			m_listctrl.SetItemText(nCount, 14, strSetParam9);
			m_listctrl.SetItemText(nCount, 15, strSetParam10);
			m_listctrl.SetItemText(nCount, 16, strSetParam11);
			m_listctrl.SetItemText(nCount, 17, strSetParam12);
			m_listctrl.SetItemText(nCount, 18, strSetParam13);
			m_listctrl.SetItemText(nCount, 19, strSetParam14);
			m_listctrl.SetItemText(nCount, 20, strSetStoreCycle);
			m_listctrl.SetItemText(nCount, 21, strSetCustomName);
			m_listctrl.SetItemText(nCount, 22, strSetSystem);
			m_listctrl.SetItemText(nCount, 23, strSetDevice);
			m_listctrl.SetItemText(nCount, 24, strSetType);
			m_listctrl.EnsureVisible(m_listctrl.GetItemCount()-1,FALSE);

			GetDlgItem(IDC_EDIT_SEARCH)->SetWindowTextW(_T(""));
			m_bNeedSave = true;
			MakeAllList();

		}//end while (pos)
	}//end if (pos == NULL) else
 }


 void CDataPointConfigDlg::OnCbnSelendokComboPointTypeSort()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 CString cstrPointType;
	 m_cbb_pointTypeSort.GetLBText(m_cbb_pointTypeSort.GetCurSel(), cstrPointType);
	 if(cstrPointType == L"ȫ��"){
		 ShowList(m_allentrylist);
	 }
	 else
	 {
		 m_searchlist.clear();
		 map<int, DataPointEntry>& datalist = m_plcpointmap.GetPointMap();
		 map<int, DataPointEntry>::const_iterator it = datalist.begin();

		 for (;it != datalist.end(); it++)
		 {
			 const DataPointEntry& entry = it->second;

			 if (entry.GetSourceType() == cstrPointType.GetString())
			 {
				 m_searchlist.push_back(entry);
			 }
		 }
		 ShowList(m_searchlist);
	 }
 }

 void CDataPointConfigDlg::OnBnClickedButtonMakeUpHistoryData()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 vector<DataPointEntry> vecDataPointList;
	 map<int, DataPointEntry> pointMap= m_plcpointmap.GetPointMap();//��ȡ���

	 //��ȡ���б�ѡ�еĵ�Ķ���
	 POSITION pos = m_listctrl.GetFirstSelectedItemPosition();
	 if (pos == NULL)
	 {
		 AfxMessageBox(_T("No items were selected!\n"));
		 return;
	 }
	 while(pos)
	 {
		 int nItem = m_listctrl.GetNextSelectedItem(pos);
		 CString strItem = m_listctrl.GetItemText(nItem,0);
		 CString strName = m_listctrl.GetItemText(nItem,1);
		 int nnItem = _wtoi(strItem.GetString());
		 
		 map<int, DataPointEntry>::iterator iter;
		 iter = pointMap.find(nnItem);
		 if(iter!= pointMap.end())
		 {
			 if(	(iter->second.GetSourceType() == L"vpoint")
				 && (iter->second.GetParam(1).size() != 0)		)
			 {
				 vecDataPointList.push_back(iter->second);
			 }
		 }
	 }
	 if(vecDataPointList.size() == 0){
		 AfxMessageBox(_T("ѡ�е���û��vpiont���ű�ȫ��Ϊ�ա�\n"));
		 return;
	 }
	 //���봰��ѡ����
	 CVPointHistoricalDataCalculation dlg(vecDataPointList, &m_plcpointmap);
	 dlg.DoModal();
 }
