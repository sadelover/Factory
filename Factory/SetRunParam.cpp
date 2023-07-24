// SetRunParam.cpp : implementation file
//

#include "stdafx.h"
#include "SetRunParam.h"
#include "afxdialogex.h"


// CSetRunParam dialog

IMPLEMENT_DYNAMIC(CSetRunParam, CDialog)

CSetRunParam::CSetRunParam(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRunParam::IDD, pParent)
	, m_StrBacnetBroadIP(_T(""))
	, m_StrBacnetIP(_T(""))
	, m_StrEthIP(_T(""))
	, m_StrBacnetDevice(_T(""))
	, m_StrLastoperationtime(_T(""))
{
	m_bExitThread = false;
	m_bThreadExitFinished = false;
	m_hThreadRunRaramThread = NULL;
	m_EventStopThread = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CSetRunParam::~CSetRunParam()
{
	//KillTimer(192);
}
bool CSetRunParam::ThreadExitFinish()
{
	return m_bThreadExitFinished;
}
void  CSetRunParam::ExitThreads()
{
	m_bExitThread = true;
	if(m_hThreadRunRaramThread)
	{
		::WaitForSingleObject(m_EventStopThread, 3000);
		CloseHandle(m_hThreadRunRaramThread);
		m_hThreadRunRaramThread = NULL;
	}
}
void CSetRunParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COREVERSION, m_StrCoreVersion);
	DDX_Text(pDX, IDC_EDIT_WATCHVERSION, m_StrWatchVersion);
	DDX_Text(pDX, IDC_EDIT_LOGICVERSION, m_StrLogicVersion);
	DDX_Text(pDX, IDC_EDIT_COREBEATTIM, m_StrCoreBEATTIM);
	DDX_Text(pDX, IDC_EDIT_WATCHBEATTIM, m_StrWatchBEATTIM);
	DDX_Text(pDX, IDC_EDIT_LOGICBEATTIM, m_StrLogicBEATTIM);
	DDX_Text(pDX, IDC_EDIT_LOCALIP,m_StrlocalIP);
	DDX_Text(pDX, IDC_EDIT_PROGRAMPATH, m_StrProgramPath);
	DDX_Text(pDX, IDC_EDIT_CORESTARTIME, m_StrCoreStartTime);
	DDX_Text(pDX, IDC_EDIT_LOGICSTARTIME ,m_StrLogicStartTime);
	DDX_Text(pDX, IDC_EDIT_DTUUPDATATIME, m_StrDTUUpdataTime);
	DDX_Text(pDX, IDC_EDIT_PLCUPDATATIME, m_StrPLCUpdataTime);
	DDX_Text(pDX, IDC_EDIT_CORERAM,m_StrCoreRAM);
	DDX_Text(pDX, IDC_EDIT_LOGICRAM,m_StrLogicRAM);
	DDX_Text(pDX, IDC_EDITPOINTTOTAL,m_StrPointTotal);

	DDX_Text(pDX, IDC_EDIT_LOCALIP2, m_StrBacnetBroadIP);
	DDX_Text(pDX, IDC_EDIT_CORESTARTIME2, m_StrBacnetIP);
	DDX_Text(pDX, IDC_EDIT_LOCALIP3, m_StrEthIP);
	DDX_Text(pDX, IDC_EDIT_CORESTARTIME3, m_StrBacnetDevice);
	DDX_Text(pDX, IDC_EDIT_LOCALIP4, m_StrLastoperationtime);

}


BEGIN_MESSAGE_MAP(CSetRunParam, CDialog)
	//ON_WM_TIMER()
END_MESSAGE_MAP()


// CSetRunParam message handlers



bool CSetRunParam::InitData()
{
	UpdataParam();
	/*****************************************************/
	//SetTimer(192, 1000, NULL);
	RunParamUpdataThread(0,0);
	return  true;
}

BOOL CSetRunParam::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

//void CSetRunParam::OnTimer(UINT_PTR nIDEvent)
//{
	/*if (nIDEvent == 192)
	{
		UpdataParam();
	}
	CDialog::OnTimer(nIDEvent);*/
//}
void  CSetRunParam::UpdataParam()
{
	wstring strCoreBEATTIM = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"servertime",L"");
	m_StrCoreBEATTIM = strCoreBEATTIM.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_COREBEATTIM))->SetWindowTextW(m_StrCoreBEATTIM);

	wstring strWatchBEATTIM = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"watchtime",L"");
	m_StrWatchBEATTIM = strWatchBEATTIM.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_WATCHBEATTIM))->SetWindowTextW(m_StrWatchBEATTIM);

	wstring strLogicBEATTIM = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"",L"");
	m_StrLogicBEATTIM = strLogicBEATTIM.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_LOGICBEATTIM))->SetWindowTextW(m_StrLogicBEATTIM);

	wstring strCoreVersion = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"version",L"");
	m_StrCoreVersion = strCoreVersion.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_COREVERSION))->SetWindowTextW(m_StrCoreVersion);

	wstring strWatchVersion = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"watchversion",L"");
	m_StrWatchVersion = strWatchVersion.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_WATCHVERSION))->SetWindowTextW(m_StrWatchVersion);

	wstring strLogicVersion = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"",L"");
	m_StrLogicVersion = strLogicVersion.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_LOGICVERSION))->SetWindowTextW(m_StrLogicVersion);

	wstring strCoreStartTime = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"starttime",L"");
	m_StrCoreStartTime = strCoreStartTime.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_CORESTARTIME))->SetWindowTextW(m_StrCoreStartTime);

	wstring strLogicStartTime = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"",L"");
	m_StrLogicStartTime = strLogicStartTime.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_LOGICSTARTIME))->SetWindowTextW(m_StrLogicStartTime);

	wstring strDTUUpdataTime = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"dtusendtime",L"");
	m_StrDTUUpdataTime = strDTUUpdataTime.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_DTUUPDATATIME))->SetWindowTextW(m_StrDTUUpdataTime);

	wstring strPLCUpdataTime = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"s7updateuime",L"");
	m_StrPLCUpdataTime = strPLCUpdataTime.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_PLCUPDATATIME))->SetWindowTextW(m_StrPLCUpdataTime);

	wstring strProgramPath = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"corefilepath",L"");
	m_StrProgramPath = strProgramPath.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_PROGRAMPATH))->SetWindowTextW(m_StrProgramPath);

	wstring strlocalIP = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"activeip",L"");
	m_StrlocalIP = strlocalIP.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_LOCALIP))->SetWindowTextW(m_StrlocalIP);

	wstring strCoreRAM = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"corememory",L"");
	m_StrCoreRAM = strCoreRAM.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_CORERAM))->SetWindowTextW(m_StrCoreRAM);

	wstring strLogicRAM = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"logicmemory",L"");
	m_StrLogicRAM = strLogicRAM.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_LOGICRAM))->SetWindowTextW(m_StrLogicRAM);

	wstring strPointTotal = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"pointcount",L"");
	m_StrPointTotal = strPointTotal.c_str();
	((CEdit*)GetDlgItem(IDC_EDITPOINTTOTAL))->SetWindowTextW(m_StrPointTotal);

	
	//julian
	wstring strBacnetBroadIP = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"bacnetBroadIP",L"");
	m_StrBacnetBroadIP = strBacnetBroadIP.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_LOCALIP2))->SetWindowTextW(m_StrBacnetBroadIP);

	wstring strBacnetIP = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"bacnetIP",L"");
	m_StrBacnetIP = strBacnetIP.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_CORESTARTIME2))->SetWindowTextW(m_StrBacnetIP);

	wstring strEthIP = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"bacnetEthip",L"");
	m_StrEthIP = strEthIP.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_LOCALIP3))->SetWindowTextW(m_StrEthIP);

	wstring strBacnetDevice = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"bacnetDevice",L"");
	m_StrBacnetDevice = strBacnetDevice.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_CORESTARTIME3))->SetWindowTextW(m_StrBacnetDevice);

	wstring strLastoperationtime = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"lastOperationTime",L"");
	m_StrLastoperationtime = strLastoperationtime.c_str();
	((CEdit*)GetDlgItem(IDC_EDIT_LOCALIP4))->SetWindowTextW(m_StrLastoperationtime);


	//UpdateData(FALSE);
}
LRESULT CSetRunParam::RunParamUpdataThread( WPARAM wParam,LPARAM lParam )
{
	m_hThreadRunRaramThread = CreateThread(NULL, 0, ThreadFuncRunParam, this, 0, NULL); 
	assert(m_hThreadRunRaramThread);
	return 0; 
}
DWORD WINAPI CSetRunParam::ThreadFuncRunParam( LPVOID lpParameter )
{
	CSetRunParam* pDlg = (CSetRunParam*)lpParameter;
	assert(pDlg);
	while(TRUE)
	{
		Sleep(1000);
		if (pDlg->m_bExitThread)
		{
			pDlg->m_bThreadExitFinished = true;
			SetEvent(pDlg->m_EventStopThread);
			return 0;
		}
		pDlg->UpdataParam();
		if (pDlg->m_bExitThread)
		{
			pDlg->m_bThreadExitFinished = true;
			SetEvent(pDlg->m_EventStopThread);
			return 0;
		}
	}
	pDlg->m_bThreadExitFinished = true;
	return 0;
}
