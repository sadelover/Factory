// SetRemote.cpp : implementation file
//

#include "stdafx.h"
#include "SetRemote.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>

// CSetRemote dialog

IMPLEMENT_DYNAMIC(CSetRemote, CDialog)

CSetRemote::CSetRemote(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRemote::IDD, pParent)
	, m_bRemoteSetOPC(FALSE)
	, m_bRemoteSetModbus(FALSE)
	, m_bRemoteSetBacnet(FALSE)
	, m_bRemoteSetPLC(FALSE)
	, m_bRemoteSetMysql(FALSE)
	, m_bRemoteSetSqlite(FALSE)
	, m_bRemoteSetSQLserver(FALSE)
{

}

CSetRemote::~CSetRemote()
{
}
BOOL CSetRemote::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}
void CSetRemote::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_REMOTE_SET_OPC, m_bRemoteSetOPC);
	DDX_Check(pDX, IDC_CHECK_REMOTE_SET_BACNET, m_bRemoteSetBacnet);
	DDX_Check(pDX, IDC_CHECK_REMOTE_SET_MYSQL, m_bRemoteSetMysql);
	DDX_Check(pDX, IDC_CHECK_REMOTE_SET_MODBUS, m_bRemoteSetModbus);
	DDX_Check(pDX, IDC_CHECK_REMOTE_SET_PLC, m_bRemoteSetPLC);
	DDX_Check(pDX, IDC_CHECK_REMOTE_SET_SQLITE, m_bRemoteSetSqlite);
	DDX_Check(pDX, IDC_CHECK_REMOTE_SET_SQLSERVER, m_bRemoteSetSQLserver);
}


BEGIN_MESSAGE_MAP(CSetRemote, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_REMOTE, &CSetRemote::OnBnClickedButtonSettingRemote)
END_MESSAGE_MAP()


// CSetRemote message handlers


void CSetRemote::OnBnClickedButtonSettingRemote()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if(m_pDataAccess)
	{		
		if(m_bRemoteSetOPC)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetopc", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetopc", L"0");
		}

		if(m_bRemoteSetModbus)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetmodbus", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetmodbus", L"0");
		}

		if(m_bRemoteSetBacnet)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetbacnet", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetbacnet", L"0");
		}

		if(m_bRemoteSetPLC)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetsimens", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetsimens", L"0");
		}

		if(m_bRemoteSetMysql)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetmysql", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remotesetmysql", L"0");
		}

		if(m_bRemoteSetSqlite)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remoteSetSqlite", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remoteSetSqlite", L"0");
		}

		if(m_bRemoteSetSQLserver)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remoteSetSQLserver", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"remoteSetSQLserver", L"0");
		}
		AfxMessageBox(_T("±£´æ³É¹¦"));
	}
	
}

bool CSetRemote::InitData()
{
	//remoteset
	wstring strRemoteSetOPC= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"remotesetopc",L"0");
	if(strRemoteSetOPC==L"")
		strRemoteSetOPC = L"0";
	m_bRemoteSetOPC = _wtoi(strRemoteSetOPC.c_str());

	wstring strRemoteSetModbus= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"remotesetmodbus",L"0");
	if(strRemoteSetModbus==L"")
		strRemoteSetModbus = L"0";
	m_bRemoteSetModbus = _wtoi(strRemoteSetModbus.c_str());

	wstring strRemoteSetBacnet= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"remotesetbacnet",L"0");
	if(strRemoteSetBacnet==L"")
		strRemoteSetBacnet = L"0";
	m_bRemoteSetBacnet = _wtoi(strRemoteSetBacnet.c_str());

	wstring strRemoteSetPLC= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"remotesetsimens",L"0");
	if(strRemoteSetPLC==L"")
		strRemoteSetPLC = L"0";
	m_bRemoteSetPLC = _wtoi(strRemoteSetPLC.c_str());

	wstring strRemoteSetMysql = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"remotesetmysql",L"0");
	if(strRemoteSetMysql==L"")
		strRemoteSetMysql = L"0";
	m_bRemoteSetMysql = _wtoi(strRemoteSetMysql.c_str());
	
	wstring strRemoteSetSqlite = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"remoteSetSqlite",L"0");
	if(strRemoteSetSqlite==L"")
		strRemoteSetSqlite = L"0";
	m_bRemoteSetSqlite = _wtoi(strRemoteSetSqlite.c_str());

	wstring strRemoteSetSQLserver = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"remoteSetSQLserver",L"0");
	if(strRemoteSetSQLserver==L"")
		strRemoteSetSQLserver = L"0";
	m_bRemoteSetSQLserver = _wtoi(strRemoteSetSQLserver.c_str());

	UpdateData(FALSE);
	return true;
}