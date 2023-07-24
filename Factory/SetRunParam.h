#pragma once
#include "stdafx.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>
#include <process.h> 
#include "afxwin.h"

// CSetRunParam dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;

class CSetRunParam : public CDialog
{
	DECLARE_DYNAMIC(CSetRunParam)

public:
	CSetRunParam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetRunParam();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_SETRUNPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CUPPCServerDataAccess*	m_pDataAccess;
	CString			m_StrCoreBEATTIM;
	CString			m_StrWatchBEATTIM;
	CString			m_StrLogicBEATTIM;
	CString			m_StrCoreVersion;
	CString			m_StrWatchVersion;
	CString			m_StrLogicVersion;
	CString		    m_StrCoreStartTime;
	CString			m_StrLogicStartTime;
	CString			m_StrDTUUpdataTime;
	CString			m_StrPLCUpdataTime;
	CString			m_StrProgramPath; 
	CString			m_StrlocalIP;
	CString			m_StrCoreRAM;
	CString			m_StrLogicRAM;
	CString			m_StrPointTotal;
	HANDLE			m_ParamupdataThread;
	bool            m_bExitThread;
	bool			m_bThreadExitFinished;
	HANDLE          m_hThreadRunRaramThread;
	HANDLE			m_EventStopThread;

public:
	bool InitData();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdataParam();
	LRESULT RunParamUpdataThread( WPARAM wParam,LPARAM lParam );
	static DWORD WINAPI ThreadFuncRunParam(LPVOID lpParameter);
	bool ThreadExitFinish();
	void ExitThreads();

	CString m_StrBacnetBroadIP;
	CString m_StrBacnetIP;
	CString m_StrEthIP;
	CString m_StrBacnetDevice;
	CString m_StrLastoperationtime;
};
