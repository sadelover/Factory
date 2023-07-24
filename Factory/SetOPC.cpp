// SetOPC.cpp : implementation file
//

#include "stdafx.h"
#include "SetOPC.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>

// CSetOPC dialog

IMPLEMENT_DYNAMIC(CSetOPC, CDialog)

CSetOPC::CSetOPC(CWnd* pParent /*=NULL*/)
	: CDialog(CSetOPC::IDD, pParent)
	, m_nOPCMaxPoint(0)
	, m_nOPCReadMutil(1)
	, m_nOPCCmdSleep(200)
	, m_nOPCCmdSleepFromDevice(50)
	, m_nOPCPollSleep(5)
	, m_nOPCPollSleepFromDevice(60)
	, m_nOPCReconnectInterval(10)
	, m_nOPCCheckReconnect(5)
	, m_bEnableSecurity(FALSE)
	, m_bEnableOPCThread(FALSE)
	, m_bRemoteSetOPC(FALSE)
	, m_bDisableQuality(FALSE)
	, m_strOPCServerIP(_T("0"))
	//0828julian
	, m_nOPCupdaterate(500)
	, m_nOPCmultiadd(1000)
	, m_nOPCasync20interval(60)
	, m_nOPClanguageid(0)
	, m_bOPCasync20mode(FALSE)
	, m_nOPCreconnectignore(0)
	, m_nOPCmainpollsleep(0)
{

}

CSetOPC::~CSetOPC()
{
}
BOOL CSetOPC::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}
void CSetOPC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPC_MAX_POINT, m_nOPCMaxPoint);
	DDX_Text(pDX, IDC_EDIT_OPC_READ_MUTIL, m_nOPCReadMutil);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL3, m_nOPCCmdSleep);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL5, m_nOPCPollSleep);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL4, m_nOPCCmdSleepFromDevice);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL6, m_nOPCPollSleepFromDevice);
	DDX_Text(pDX, IDC_EDIT_OPC_RECONNECT_INTERVAL, m_nOPCReconnectInterval);
	DDX_Text(pDX, IDC_EDIT_OPC_CHECK_INTERVAL, m_nOPCCheckReconnect);
	DDX_Text(pDX, IDC_EDIT_OPCSERVERIP, m_strOPCServerIP);
	DDX_Check(pDX, IDC_CHECK_ENABLE_THREAD, m_bEnableOPCThread);
	DDX_Check(pDX, IDC_CHECK2, m_bEnableSecurity);
	DDX_Check(pDX, IDC_CHECK_DISABLE_QUALITY, m_bDisableQuality);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL8, m_nOPCupdaterate);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL9, m_nOPCmultiadd);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL10, m_nOPCasync20interval);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL7, m_nOPClanguageid);
	//DDX_Text(pDX, IDC_EDIT5, m_nOPClanguageid);
	DDX_Check(pDX, IDC_CHECK3, m_bOPCasync20mode);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL12, m_nOPCreconnectignore);
	DDX_Text(pDX, IDC_EDIT_OPC_INTERVAL11, m_nOPCmainpollsleep);
}


BEGIN_MESSAGE_MAP(CSetOPC, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_OPC, &CSetOPC::OnBnClickedButtonSettingOpc)

END_MESSAGE_MAP()


// CSetOPC message handlers


void CSetOPC::OnBnClickedButtonSettingOpc()
{
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pDataAccess)
	{
		CString str;
		str.Format(_T("%d"),m_nOPCMaxPoint);
		m_pDataAccess->WriteCoreDebugItemValue(L"opcclientmaxpoint",str.GetString());

		str.Format(_T("%d"),m_nOPCReadMutil);
		m_pDataAccess->WriteCoreDebugItemValue(L"opcmutilcount",str.GetString());

		str.Format(_T("%d"),m_nOPCCmdSleep);
		m_pDataAccess->WriteCoreDebugItemValue(L"opccmdsleep",str.GetString());

		//
		str.Format(_T("%d"),m_nOPCCmdSleepFromDevice);
		m_pDataAccess->WriteCoreDebugItemValue(L"opccmdsleepfromdevice",str.GetString());
		str.Format(_T("%d"),m_nOPCPollSleep);
		m_pDataAccess->WriteCoreDebugItemValue(L"opcpollsleep",str.GetString());
		str.Format(_T("%d"),m_nOPCPollSleepFromDevice);
		m_pDataAccess->WriteCoreDebugItemValue(L"opcpollsleepfromdevice",str.GetString());
		str.Format(_T("%d"),m_nOPCCheckReconnect);
		m_pDataAccess->WriteCoreDebugItemValue(L"opccheckreconnect",str.GetString());
		str.Format(_T("%d"),m_nOPCReconnectInterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"opcreconnect",str.GetString());

		m_pDataAccess->WriteCoreDebugItemValue(L"OPCServerIP",m_strOPCServerIP.GetString());

		
		//write by julian start

		str.Format(_T("%d"),m_nOPCupdaterate);
		m_pDataAccess->WriteCoreDebugItemValue(L"opcupdaterate",str.GetString());

		str.Format(_T("%d"),m_nOPCmultiadd);
		m_pDataAccess->WriteCoreDebugItemValue(L"opcmultiadd",str.GetString());

		str.Format(_T("%d"),m_nOPCasync20interval);
		m_pDataAccess->WriteCoreDebugItemValue(L"opcasync20interval",str.GetString());

		str.Format(_T("%d"),m_nOPClanguageid );
		m_pDataAccess->WriteCoreDebugItemValue(L"opclanguageid ",str.GetString());

		str.Format(_T("%d"),m_nOPCreconnectignore );
		m_pDataAccess->WriteCoreDebugItemValue(L"opcreconnectignore ",str.GetString());

		str.Format(_T("%d"),m_nOPCmainpollsleep );
		m_pDataAccess->WriteCoreDebugItemValue(L"opcmainpollsleep ",str.GetString());


		if(m_bOPCasync20mode)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"opcasync20mode",L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"opcasync20mode",L"0");
		}


		if(m_bEnableOPCThread)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"opcclientthread",L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"opcclientthread",L"0");
		}

		if(m_bEnableSecurity)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"enableSecurity",L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"enableSecurity",L"0");
		}

		if(m_bDisableQuality)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"disableCheckQuality",L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"disableCheckQuality",L"0");
		}

		AfxMessageBox(_T("±£´æ³É¹¦"));
	}	
	
}

bool CSetOPC::InitData()
{
	//opc
	wstring strOPCMaxPoint= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcclientmaxpoint",L"10000");
	if(strOPCMaxPoint==L"")
		strOPCMaxPoint = L"10000";
	m_nOPCMaxPoint = _wtoi(strOPCMaxPoint.c_str());

	wstring strEnableOPCThread= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcclientthread",L"0");
	if(strEnableOPCThread==L"")
		strEnableOPCThread = L"0";
	m_bEnableOPCThread = _wtoi(strEnableOPCThread.c_str());

	wstring strEnableSecurity= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"enableSecurity",L"0");
	if(strEnableSecurity==L"")
		strEnableSecurity = L"0";
	m_bEnableSecurity = _wtoi(strEnableSecurity.c_str());

	wstring strDisableQuality= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"disableCheckQuality",L"0");
	if(strDisableQuality==L"")
		strDisableQuality = L"0";
	m_bDisableQuality = _wtoi(strDisableQuality.c_str());

	m_strOPCServerIP= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"OPCServerIP",L"0").c_str();

	wstring strOPCReadMutil= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcmutilcount",L"50");
	if(strOPCReadMutil==L"")
		strOPCReadMutil = L"50";
	m_nOPCReadMutil = _wtoi(strOPCReadMutil.c_str());

	wstring strOPCCmdSleep= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opccmdsleep",L"100");
	if(strOPCCmdSleep==L"")
		strOPCCmdSleep = L"200";
	m_nOPCCmdSleep = _wtoi(strOPCCmdSleep.c_str());

	wstring strOPCReconnectInterval= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcreconnect",L"10");
	if(strOPCReconnectInterval==L"")
		strOPCReconnectInterval = L"10";
	m_nOPCReconnectInterval = _wtoi(strOPCReconnectInterval.c_str());

	wstring strOPCCheckReconnect= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opccheckreconnect",L"5");
	if(strOPCCheckReconnect==L"")
		strOPCCheckReconnect = L"5";
	m_nOPCCheckReconnect = _wtoi(strOPCCheckReconnect.c_str());

	//
	wstring wstrOPCCmdSleepFromDevice,wstrOPCPollSleep,wstrOPCPollSleepFromDevice;
	wstrOPCCmdSleepFromDevice = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opccmdsleepfromdevice",L"50");
	if(wstrOPCCmdSleepFromDevice == L"")
		wstrOPCCmdSleepFromDevice = L"50";
	m_pDataAccess->WriteCoreDebugItemValue(L"opccmdsleepfromdevice",wstrOPCCmdSleepFromDevice);
	m_nOPCCmdSleepFromDevice = _wtoi(wstrOPCCmdSleepFromDevice.c_str());

	wstrOPCPollSleep = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcpollsleep",L"5");
	if(wstrOPCPollSleep == L"")
		wstrOPCPollSleep = L"5";
	m_pDataAccess->WriteCoreDebugItemValue(L"opcpollsleep",wstrOPCPollSleep);
	m_nOPCPollSleep = _wtoi(wstrOPCPollSleep.c_str());

	wstrOPCPollSleepFromDevice = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcpollsleepfromdevice",L"60");
	if(wstrOPCPollSleepFromDevice == L"")
		wstrOPCPollSleepFromDevice = L"60";
	m_pDataAccess->WriteCoreDebugItemValue(L"opcpollsleepfromdevice",wstrOPCPollSleepFromDevice);
	m_nOPCPollSleepFromDevice = _wtoi(wstrOPCPollSleepFromDevice.c_str());

	
	//write by julian start
	wstring strOPCupdaterate= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcupdaterate",L"500");
	if(strOPCupdaterate==L"")
		 strOPCupdaterate= L"500";
	m_nOPCupdaterate = _wtoi(strOPCupdaterate.c_str());
	
	wstring strOPCmultiadd= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcmultiadd",L"1000");
	if(strOPCmultiadd==L"")
		 strOPCmultiadd= L"1000";
	m_nOPCmultiadd = _wtoi(strOPCmultiadd.c_str());
	
	wstring strOPCasync20interval= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcasync20interval",L"60");
	if(strOPCasync20interval==L"")
		 strOPCasync20interval= L"60";
	m_nOPCasync20interval = _wtoi(strOPCasync20interval.c_str());
	
	wstring strOPClanguageid= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opclanguageid",L"0");
	if(strOPClanguageid==L"")
		 strOPClanguageid= L"0";
	m_nOPClanguageid= _wtoi(strOPClanguageid.c_str());

	wstring strOPCasync20mode= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcasync20mode",L"0");
	if(strOPCasync20mode==L"")
		strOPCasync20mode = L"0";
	m_bOPCasync20mode = _wtoi(strOPCasync20mode.c_str());


	wstring strOPCreconnectignore=m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcreconnectignore",L"0");
	if(strOPCreconnectignore==L"")
		strOPCreconnectignore=L"0";
	m_nOPCreconnectignore=_wtoi(strOPCreconnectignore.c_str());

	wstring strOPCmainpollsleep=m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"opcmainpollsleep",L"10");
	if(strOPCmainpollsleep==L"")
		strOPCmainpollsleep=L"10";
	m_nOPCmainpollsleep=_wtoi(strOPCmainpollsleep.c_str());


	//write by julian end
	

	UpdateData(FALSE);

	return true;
}