// SetMobus.cpp : implementation file
//

#include "stdafx.h"
#include "SetModbus.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>

// CSetModbus dialog

IMPLEMENT_DYNAMIC(CSetModbus, CDialog)

CSetModbus::CSetModbus(CWnd* pParent /*=NULL*/)
	: CDialog(CSetModbus::IDD, pParent)
	, m_bModbusOneByOne(FALSE)
	, m_nModbusInterval(100)
	, m_nModbusMutilReadCount(0)
	, m_nModbusIDInterval(0)
	, m_modbustimeout(5000)
	, m_modbuspollinterval(2)
	, m_co3pcmdinterval(50)
	, m_co3ptimeoutl(5000)
	, m_co3prollinterval(2)
{

}

CSetModbus::~CSetModbus()
{
}
BOOL CSetModbus::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}
void CSetModbus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_MODBUS_MODE, m_bModbusOneByOne);
	DDX_Text(pDX, IDC_EDIT_MODBUS_INTERVAL, m_nModbusInterval);
	DDX_Text(pDX, IDC_EDIT_MOBUS_MULTI_COUNT, m_nModbusMutilReadCount);
	DDX_Text(pDX, IDC_EDIT_MODBUS_INTERVAL3, m_nModbusIDInterval);
	DDX_Text(pDX, IDC_EDIT_MODBUS_TIMEOUT,m_modbustimeout);
	DDX_Text(pDX, IDC_EDIT_MODBUS_POLLINTERAL,m_modbuspollinterval);
	DDX_Text(pDX, IDC_EDIT_CO3P_CMDINTERVAL,m_co3pcmdinterval);
	DDX_Text(pDX,IDC_EDIT_CO3P_TIMEOUT,m_co3ptimeoutl);
	DDX_Text(pDX,IDC_EDIT_CO3P_PROLLINTERVAL,m_co3prollinterval);
}

BEGIN_MESSAGE_MAP(CSetModbus, CDialog)
	//ON_BN_CLICKED(IDC_BUTTON_SETTING_MODBUS, &CSetModbus::OnBnClickedButtonSettingModbus)
	ON_BN_CLICKED(IDC_CHECK_MODBUS_MODE, &CSetModbus::OnBnClickedCheckModbusMode)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_MODBUS,  &CSetModbus::OnBnClickedButtonSettingModbus)
	ON_EN_CHANGE(IDC_EDIT_MODBUS_INTERVAL, &CSetModbus::OnEnChangeEditModbusInterval)
END_MESSAGE_MAP()


// CSetModbus message handlers




void CSetModbus::OnBnClickedCheckModbusMode()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bModbusOneByOne)
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"modbusreadonebyone", L"1");
	}
	else
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"modbusreadonebyone", L"0");
	}
}

void  CSetModbus::OnBnClickedButtonSettingModbus()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pDataAccess)
	{
		if(m_bModbusOneByOne)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"modbusreadonebyone", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"modbusreadonebyone", L"0");
		}

		CString str;
		str.Format(_T("%d"),m_nModbusInterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"ModbusReadIntervalMS",str.GetString());

		str.Format(_T("%d"),m_nModbusMutilReadCount);
		m_pDataAccess->WriteCoreDebugItemValue(L"modbusmutilcount",str.GetString());

		str.Format(_T("%d"),m_nModbusIDInterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"modbusidinterval",str.GetString());
		/*m_modbustimeout;		
		m_modbuspollinterval;   
		m_co3pcmdinterval;		
		m_co3ptimeoutl;			
		m_co3prollinterval;*/
		///////////////////////////////////////////////////////////////////////////
		str.Format(_T("%d"),m_modbustimeout);
		m_pDataAccess->WriteCoreDebugItemValue(L"modbustimeout",str.GetString());

		str.Format(_T("%d"),m_modbuspollinterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"modbuspollinterval",str.GetString());

		str.Format(_T("%d"),m_co3pcmdinterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"co3pcmdinterval",str.GetString());

		str.Format(_T("%d"),m_co3ptimeoutl);
		m_pDataAccess->WriteCoreDebugItemValue(L"co3ptimeoutl",str.GetString());

		str.Format(_T("%d"),m_co3prollinterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"co3prollinterval",str.GetString());
		////////////////////////////////////////////////////////////////////////////
		AfxMessageBox(_T("±£´æ³É¹¦"));
	}
}
bool CSetModbus::InitData()
{
	//modbus
	wstring strModbusreadonebyone= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"modbusreadonebyone",L"0");
	if(strModbusreadonebyone==L"")
		strModbusreadonebyone = L"0";
	m_bModbusOneByOne = _wtoi(strModbusreadonebyone.c_str());

	wstring strModbusReadIntervalMS = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"ModbusReadIntervalMS",L"100");
	if(strModbusReadIntervalMS==L"")
		strModbusReadIntervalMS = L"100";
	m_nModbusInterval = _wtoi(strModbusReadIntervalMS.c_str());

	wstring strModbusMutilReadCount = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"modbusmutilcount",L"99");
	if(strModbusMutilReadCount==L"")
		strModbusMutilReadCount = L"99";
	m_nModbusMutilReadCount = _wtoi(strModbusMutilReadCount.c_str());

	wstring strModbusIDInterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"modbusidinterval",L"500");
	if(strModbusIDInterval==L"")
		strModbusIDInterval = L"500";
	m_nModbusIDInterval = _wtoi(strModbusIDInterval.c_str());
	/*m_modbustimeout;		
		m_modbuspollinterval;   
		m_co3pcmdinterval;		
		m_co3ptimeoutl;			
		m_co3prollinterval;*/
	////////////////////////////////////////////////////////////////////////////////////
	wstring strModbustimeout = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"modbustimeout",L"5000");
	if(strModbustimeout==L"")
		strModbustimeout = L"5000";
	m_modbustimeout = _wtoi(strModbustimeout.c_str());

	wstring strModbuspollinterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"modbuspollinterval",L"2");
	if(strModbuspollinterval==L"")
		strModbuspollinterval = L"2";
	m_modbuspollinterval = _wtoi(strModbuspollinterval.c_str());

	wstring strCo3pcmdinterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"co3pcmdinterval",L"50");
	if(strCo3pcmdinterval==L"")
		strCo3pcmdinterval = L"50";
	m_co3pcmdinterval = _wtoi(strCo3pcmdinterval.c_str());

	wstring strCo3ptimeoutl = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"co3ptimeoutl",L"5000");
	if(strCo3ptimeoutl==L"")
		strCo3ptimeoutl = L"5000";
	m_co3ptimeoutl = _wtoi(strCo3ptimeoutl.c_str());

	wstring strCo3prollinterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"co3prollinterval",L"2");
	if(strCo3prollinterval==L"")
		strCo3prollinterval = L"2";
	m_co3prollinterval = _wtoi(strCo3prollinterval.c_str());
	///////////////////////////////////////////////////////////////////////////////////
	UpdateData(FALSE);

	return true;
}

void CSetModbus::OnEnChangeEditModbusInterval()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
