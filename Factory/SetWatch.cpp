// SetWatch.cpp : implementation file
//

#include "stdafx.h"
#include "SetWatch.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>

// CSetWatch dialog

IMPLEMENT_DYNAMIC(CSetWatch, CDialog)

CSetWatch::CSetWatch(CWnd* pParent /*=NULL*/)
	: CDialog(CSetWatch::IDD, pParent)
	, m_bDisableWatch(FALSE)
	, m_bCoreNotRun(FALSE)
	, m_bServertimeTimeOut(FALSE)
	, m_bPLCTimeOut(FALSE)
	, m_bDTUTimeOut(FALSE)
	, m_strDTURouterIP(_T(""))
{

}

CSetWatch::~CSetWatch()
{
}
BOOL CSetWatch::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}
void CSetWatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_CORE_NOT_RUN, m_bCoreNotRun);
	DDX_Check(pDX, IDC_CHECK_ENABLE_WATCH, m_bDisableWatch);
	DDX_Check(pDX, IDC_CHECK_SERVERTIME_TIMEOUT, m_bServertimeTimeOut);
	DDX_Check(pDX, IDC_CHECK_PLC_TIMEOUT, m_bPLCTimeOut);
	DDX_Check(pDX, IDC_CHECK_DTU_TIMEOUT, m_bDTUTimeOut);
	DDX_Text(pDX, IDC_EDIT5, m_strDTURouterIP);
}

BEGIN_MESSAGE_MAP(CSetWatch, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_WATCH, &CSetWatch::OnBnClickedButtonSettingWatch)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_WATCH, &CSetWatch::OnBnClickedCheckEnableWatch)
END_MESSAGE_MAP()


// CSetWatch message handlers

void CSetWatch::OnBnClickedButtonSettingWatch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pDataAccess)
	{
		if(m_bDisableWatch)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"disablewatch", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"disablewatch", L"0");
		}

		if(m_bCoreNotRun)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"autostartcore", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"autostartcore", L"0");
		}

		if(m_bServertimeTimeOut)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"checkservertime", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"checkservertime", L"0");
		}

		if(m_bPLCTimeOut)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"checks7udp", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"checks7udp", L"0");
		}

		if(m_bDTUTimeOut)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"checkdtu", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"checkdtu", L"0");
		}

		m_pDataAccess->WriteCoreDebugItemValue(L"DTURouterIP",m_strDTURouterIP.GetString());

		AfxMessageBox(_T("±£´æ³É¹¦"));
	}		
	
}
bool CSetWatch::EnableWatchControl(bool bEnableWatch)
{
	((CButton*)GetDlgItem(IDC_CHECK_CORE_NOT_RUN))->EnableWindow(!bEnableWatch);
	((CButton*)GetDlgItem(IDC_CHECK_SERVERTIME_TIMEOUT))->EnableWindow(!bEnableWatch);
	((CButton*)GetDlgItem(IDC_CHECK_PLC_TIMEOUT))->EnableWindow(!bEnableWatch);
	((CButton*)GetDlgItem(IDC_CHECK_DTU_TIMEOUT))->EnableWindow(!bEnableWatch);
	return true;
}

bool CSetWatch::InitData()
{
	//watch
	wstring strDisableWatch= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"disablewatch",L"1");
	if(strDisableWatch==L"")
		strDisableWatch = L"0";
	m_bDisableWatch = _wtoi(strDisableWatch.c_str());

	wstring strCoreNotRun= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"autostartcore",L"1");
	if(strCoreNotRun==L"")
		strCoreNotRun = L"1";
	m_bCoreNotRun = _wtoi(strCoreNotRun.c_str());

	wstring strDTUTimeOut= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"checkdtu",L"1");
	if(strDTUTimeOut==L"")
		strDTUTimeOut = L"1";
	m_bDTUTimeOut = _wtoi(strDTUTimeOut.c_str());

	wstring strServertimeTimeOut= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"checkservertime",L"1");
	if(strServertimeTimeOut==L"")
		strServertimeTimeOut = L"1";
	m_bServertimeTimeOut = _wtoi(strServertimeTimeOut.c_str());

	wstring strDTURouterIP = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"DTURouterIP",L"0");
	if(strDTURouterIP==L"")
		strDTURouterIP = L"0";
	m_strDTURouterIP = strDTURouterIP.c_str();

	wstring strPLCTimeOut= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"checks7udp",L"1");
	if(strPLCTimeOut==L"")
		strPLCTimeOut = L"1";
	m_bPLCTimeOut = _wtoi(strPLCTimeOut.c_str());
	EnableWatchControl(m_bDisableWatch);
	UpdateData(FALSE);
	return true;
}
void CSetWatch::OnBnClickedCheckEnableWatch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableWatchControl(m_bDisableWatch);
}