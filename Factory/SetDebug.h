#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <hash_map>
// CCommunicationDlg dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;

class CSetDebug : public CDialog
{
	DECLARE_DYNAMIC(CSetDebug)

public:
	CSetDebug(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetDebug();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETDEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingDebug();
	bool InitData();
	virtual BOOL OnInitDialog();
public:
		BOOL m_bOutErrCode;
		BOOL m_bDebugBacnet;
		BOOL m_bOutErrPoint;
		BOOL m_bDebugOPC;
		BOOL m_bDebugModbus;
		int m_nErrInterval;
		int m_nTCPSendFilePackageSize;
		CUPPCServerDataAccess*	m_pDataAccess;
		int m_nOutputMemoryInterval;
		BOOL m_bLogicLogEnable;
		BOOL m_bThreadSingleMode;
		int m_nLogicLogBufferCount;
private:
	// 禁用计算型虚拟点实时计算
	BOOL m_realtime_vpoint_calculation_disabled;
};
