// SetDebug.cpp : implementation file
//

#include "stdafx.h"
#include "SetDebug.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>
#include "resource.h"

// CSetDebug dialog

IMPLEMENT_DYNAMIC(CSetDebug, CDialog)

CSetDebug::CSetDebug(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDebug::IDD, pParent)
	, m_bOutErrCode(FALSE)
	, m_bDebugBacnet(FALSE)
	, m_bOutErrPoint(FALSE)
	, m_bDebugOPC(FALSE)
	, m_bDebugModbus(FALSE)
	, m_nErrInterval(0)
	, m_nOutputMemoryInterval(0)
	, m_bLogicLogEnable(FALSE)
	, m_bThreadSingleMode(FALSE)
	, m_nLogicLogBufferCount(0)
	, m_realtime_vpoint_calculation_disabled(FALSE)
{

}

CSetDebug::~CSetDebug()
{
}
BOOL CSetDebug::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

void CSetDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_DEBUG_ERRCODE, m_bOutErrCode);
	DDX_Check(pDX, IDC_CHECK_DEBUG_BACNET, m_bDebugBacnet);
	DDX_Check(pDX, IDC_CHECK_DEBUG_ERRPOINT, m_bOutErrPoint);
	DDX_Check(pDX, IDC_CHECK_DEBUG_OPC, m_bDebugOPC);
	DDX_Check(pDX, IDC_CHECK_DEBUG_MODBUS, m_bDebugModbus);
	DDX_Text(pDX, IDC_EDIT_ERR_INTERVAL, m_nErrInterval);
	DDX_Text(pDX, IDC_EDIT_ERR_INTERVAL2, m_nOutputMemoryInterval);
	DDX_Check(pDX, IDC_CHECK_LOGIC_LOG_ENABLE, m_bLogicLogEnable);
	DDX_Check(pDX, IDC_CHECK_LOGIC_SINGLE_THREAD_MODE, m_bThreadSingleMode);
	DDX_Text(pDX, IDC_EDIT_LOGIC_LOG_BUFFER_COUNT, m_nLogicLogBufferCount);
	DDX_Check(pDX, IDC_CHECK_REALTIME_VPOINT_CALCULATION_DISABLED, m_realtime_vpoint_calculation_disabled);
}


BEGIN_MESSAGE_MAP(CSetDebug, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_DEBUG, &CSetDebug::OnBnClickedButtonSettingDebug)
END_MESSAGE_MAP()


// CSetDebug message handlers


void CSetDebug::OnBnClickedButtonSettingDebug()
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);
	if(m_pDataAccess)
	{		
		if(m_bDebugBacnet)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"debugbacnet", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"debugbacnet", L"0");
		}

		if(m_bDebugOPC)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"debugopc", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"debugopc", L"0");
		}

		if(m_bDebugModbus)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"debugmodbus", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"debugmodbus", L"0");
		}

		if(m_bOutErrCode)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"outputerrcode", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"outputerrcode", L"0");
		}

		if(m_bOutErrPoint)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"outputerrpoint", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"outputerrpoint", L"0");
		}

		if(m_bLogicLogEnable)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"logic_log_level", L"0");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"logic_log_level", L"-1");
		}

		if(m_bThreadSingleMode)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"logic_thread_mode", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"logic_thread_mode", L"0");
		}

		if(m_realtime_vpoint_calculation_disabled)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"realtime_vpoint_calculation_disabled", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"realtime_vpoint_calculation_disabled", L"0");
		}

		CString str;
		str.Format(_T("%d"),m_nErrInterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"outputerrminute",str.GetString());

		str.Format(_T("%d"),m_nOutputMemoryInterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"outputmemoryinterval",str.GetString());

		if(m_nLogicLogBufferCount>0)
		{
			CString strValue;
			strValue.Format(_T("%d"), m_nLogicLogBufferCount);
			m_pDataAccess->WriteCoreDebugItemValue(L"logic_batch_log_count", strValue.GetString());
		}

		AfxMessageBox(_T("±£´æ³É¹¦."));
	}
}
bool CSetDebug::InitData()
{
	wstring strLogicLogLevel= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"logic_log_level",L"0");
	if(strLogicLogLevel==L"-1")
		m_bLogicLogEnable = FALSE;
	else
		m_bLogicLogEnable = TRUE;

	wstring strLogicSingleThreadMode= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"logic_thread_mode",L"0");
	if(strLogicSingleThreadMode==L"1")
		m_bThreadSingleMode = TRUE;
	else
		m_bThreadSingleMode = FALSE;


	wstring strLogicLogBufferCount= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"logic_batch_log_count", L"100");
	int nLogicLogBufferCount = _wtoi(strLogicLogBufferCount.c_str());
	m_nLogicLogBufferCount = nLogicLogBufferCount;

	//debug
	wstring strDebugBacnet= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"logic_log_level",L"0");
	if(strDebugBacnet==L"")
		strDebugBacnet = L"0";
	m_bDebugBacnet = _wtoi(strDebugBacnet.c_str());

	wstring strDebugOPC = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"debugopc",L"0");
	if(strDebugOPC==L"")
		strDebugOPC = L"0";
	m_bDebugOPC = _wtoi(strDebugOPC.c_str());

	wstring strDebugModbus = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"debugmodbus",L"0");
	if(strDebugModbus==L"")
		strDebugModbus = L"0";
	m_bDebugModbus = _wtoi(strDebugModbus.c_str());

	wstring strOutErrCode = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"outputerrcode",L"0");
	if(strOutErrCode==L"")
		strOutErrCode = L"0";
	m_bOutErrCode = _wtoi(strOutErrCode.c_str());

	wstring strOutErrPoint = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"outputerrpoint",L"0");
	if(strOutErrPoint==L"")
		strOutErrPoint = L"0";
	m_bOutErrPoint = _wtoi(strOutErrPoint.c_str());

	wstring strErrInterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"outputerrminute",L"10");
	if(strErrInterval==L"")
		strErrInterval = L"10";
	m_nErrInterval = _wtoi(strErrInterval.c_str());

	//julian

	wstring strOutputMemoryInterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"outputmemoryinterval",L"10");
	if(strOutputMemoryInterval==L"")
		strOutputMemoryInterval = L"10";
	m_nOutputMemoryInterval = _wtoi(strOutputMemoryInterval.c_str());

	//end
	wstring strRealtime_vpoint_calculation_disabled = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"realtime_vpoint_calculation_disabled",L"0");
	if(strRealtime_vpoint_calculation_disabled==L"1")
		m_realtime_vpoint_calculation_disabled = TRUE;
	else
		m_realtime_vpoint_calculation_disabled = FALSE;
	UpdateData(FALSE);

	return true;
}
