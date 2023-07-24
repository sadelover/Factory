// SetDTU.cpp : implementation file
//

#include "stdafx.h"
#include "SetDTU.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>

// CSetDTU dialog

IMPLEMENT_DYNAMIC(CSetDTU, CDialog)

CSetDTU::CSetDTU(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDTU::IDD, pParent)
	, m_bDTUEnable(FALSE)
	, m_bDTURecCmd(FALSE)
	, m_bDTUChecked(FALSE)
	, m_bDisableSendAll(FALSE)
	, m_bTCPEnable(FALSE)
	, m_nDTUPort(0)
	, m_nTCPServerPort(502)
	, m_nDTUMinType(2)
	, m_nDTUType(0)
	, m_nSendAll(0)
	, m_nTCPSendFilePackageSize(0)
	, m_nTCPSendFilePackageInterval(0)
	, m_strTCPName(_T("XXXXXXXXXXX"))
	, m_strTCPIP(_T(""))
{

}

CSetDTU::~CSetDTU()
{
}

BOOL CSetDTU::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}
void CSetDTU::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nDTUPort);
	DDX_Check(pDX, IDC_CHECK_SEND_ALL, m_nSendAll);
	DDX_Check(pDX, IDC_CHECK_DISABLE, m_bDisableSendAll);
	DDX_Text(pDX, IDC_EDIT_TCP_PORT, m_nTCPServerPort);
	DDX_CBIndex(pDX, IDC_COMBO_DTUMINNTYPE, m_nDTUMinType);
	DDX_CBIndex(pDX, IDC_COMBO_DTU_TYPE, m_nDTUType);
	DDX_Text(pDX, IDC_EDIT_TCP_PACKAGE_INTERVAL, m_nTCPSendFilePackageInterval);
	DDV_MinMaxInt(pDX, m_nTCPSendFilePackageInterval, 200, 2000);
	DDX_Text(pDX, IDC_EDIT_TCP_PACKAGE_LENGTH, m_nTCPSendFilePackageSize);
	DDV_MinMaxInt(pDX, m_nTCPSendFilePackageSize, 0, 4000);
	DDX_Text(pDX, IDC_EDIT_TCP_NAME, m_strTCPName);
	DDX_Text(pDX, IDC_EDIT_TCP_IP, m_strTCPIP);
}


BEGIN_MESSAGE_MAP(CSetDTU, CDialog)
		ON_BN_CLICKED(IDC_BUTTON_SETTING_DTU, &CSetDTU::OnBnClickedButtonSettingDtu)
		ON_CBN_SELCHANGE(IDC_COMBO_DTU_TYPE, &CSetDTU::OnCbnSelchangeComboDtuType)
		ON_CBN_SELCHANGE(IDC_COMBO_DTUMINNTYPE, &CSetDTU::OnCbnSelchangeComboDtuminntype)
		ON_BN_CLICKED(IDC_CHECK_DTUCHECK, &CSetDTU::OnBnClickedCheckDtucheck)
		ON_BN_CLICKED(IDC_CHECK_DISABLE, &CSetDTU::OnBnClickedCheckDisable)
		ON_BN_CLICKED(IDC_CHECK_SEND_ALL, &CSetDTU::OnBnClickedCheckSendAll)
END_MESSAGE_MAP()


// CSetDTU message handlers


void CSetDTU::OnBnClickedButtonSettingDtu()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pDataAccess)
	{
		if(m_bDTUEnable)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"DTUEnabled", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"DTUEnabled", L"0");
		}

		if(m_bDTURecCmd)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"DTURecCmd", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"DTURecCmd", L"0");
		}
		if(m_bDTUChecked)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"DTUChecked", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"DTUChecked", L"0");
		}

		if(m_bDisableSendAll)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"DTUDisableSendAll", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"DTUDisableSendAll", L"0");
		}

		if(m_bTCPEnable)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"TcpSenderEnabled", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"TcpSenderEnabled", L"0");
		}

		CString str;
		str.Format(_T("%d"),m_nTCPServerPort);
		m_pDataAccess->WriteCoreDebugItemValue(L"TcpSenderPort",str.GetString());
		str.Format(_T("%d"),m_nDTUMinType);
		m_pDataAccess->WriteCoreDebugItemValue(L"DTUMinType", str.GetString());

		str.Format(_T("%d"),m_nDTUType);
		m_pDataAccess->WriteCoreDebugItemValue(L"DTUType",str.GetString());

		m_pDataAccess->WriteCoreDebugItemValue(L"TcpSenderIP", m_strTCPIP.GetString());
		m_pDataAccess->WriteCoreDebugItemValue(L"TcpSenderName", m_strTCPName.GetString());

		str.Format(_T("%d"),m_nDTUPort);
		m_pDataAccess->WriteCoreDebugItemValue(L"DTUComPort",str.GetString());
		str.Format(_T("%d"),m_nDTUMinType);
		m_pDataAccess->WriteCoreDebugItemValue(L"DTUMinType", str.GetString());

		//
		str.Format(_T("%d"),m_nTCPSendFilePackageSize);
		m_pDataAccess->WriteCoreDebugItemValue(L"TCPSendFilePackageSize",str.GetString());
		str.Format(_T("%d"),m_nTCPSendFilePackageInterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"TCPSendFilePackageInterval", str.GetString());

		AfxMessageBox(_T("保存成功,需要手动重新启动Core."));
	}
}


void CSetDTU::OnCbnSelchangeComboDtuType()
{
	// TODO: Add your control notification handler code here
	m_nDTUType = ((CComboBox*)GetDlgItem(IDC_COMBO_DTU_TYPE))->GetCurSel();
	m_bDTUEnable = false;
	m_bTCPEnable = false;
	if(m_nDTUType == 1 || m_nDTUType == 2)
		m_bDTUEnable = true;
	else if (m_nDTUType == 3)
		m_bTCPEnable = true;

	EnableControl();
}
bool CSetDTU::EnableControl()
{
	if(m_nDTUType == 0)		//全禁用
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_DTUMINNTYPE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_DTUCHECK))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_DISABLE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_IP))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PORT))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_NAME))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_SEND_ALL))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PACKAGE_LENGTH))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PACKAGE_INTERVAL))->EnableWindow(FALSE);
	}
	else if(m_nDTUType == 1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_DTUMINNTYPE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK_DTUCHECK))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK_DISABLE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_IP))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PORT))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_NAME))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_SEND_ALL))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_SETTING_DTU))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PACKAGE_LENGTH))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PACKAGE_INTERVAL))->EnableWindow(FALSE);
	}
	else if(m_nDTUType == 2)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_DTUMINNTYPE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK_DTUCHECK))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_DISABLE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_IP))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PORT))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_NAME))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK_SEND_ALL))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_SETTING_DTU))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PACKAGE_LENGTH))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PACKAGE_INTERVAL))->EnableWindow(FALSE);
	}
	else if(m_nDTUType == 3)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_DTUMINNTYPE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_DTUCHECK))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_DISABLE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_IP))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PORT))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_NAME))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK_SEND_ALL))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_SETTING_DTU))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PACKAGE_LENGTH))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_TCP_PACKAGE_INTERVAL))->EnableWindow(TRUE);
	}
	return true;
}

void CSetDTU::OnCbnSelchangeComboDtuminntype()
{
	// TODO: Add your control notification handler code here
	m_nDTUMinType = ((CComboBox*)GetDlgItem(IDC_COMBO_DTUMINNTYPE))->GetCurSel();
}


void CSetDTU::OnBnClickedCheckDtucheck()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bDTUChecked)
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"DTUChecked", L"1");
	}
	else
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"DTUChecked", L"0");
	}
}


void CSetDTU::OnBnClickedCheckDisable()
{
	// TODO: Add your control notification handler code here
	
	m_bDisableSendAll = ((CButton*)GetDlgItem(IDC_CHECK_DISABLE))->GetCheck();
	if(m_bDisableSendAll)
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"DTUDisableSendAll", L"1");
	}
	else
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"DTUDisableSendAll", L"0");
	}
	
}


void CSetDTU::OnBnClickedCheckSendAll()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_nSendAll == 0)
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"sendall", L"0");
	}
	else
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"sendall", L"1");
	}

}

bool CSetDTU::InitData()
{
	//DTU Info
	wstring strDTUEnabled = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"DTUEnabled",L"0");
	if(strDTUEnabled==L"")
		strDTUEnabled = L"0";
	m_bDTUEnable = _wtoi(strDTUEnabled.c_str());

	wstring strDTURecCmd = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"DTURecCmd",L"0");
	if(strDTURecCmd==L"")
		strDTURecCmd = L"0";
	m_bDTURecCmd = _wtoi(strDTURecCmd.c_str());

	wstring strDTUChecked= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"DTUChecked",L"0");
	if(strDTUChecked==L"")
		strDTUChecked = L"0";
	m_bDTUChecked = _wtoi(strDTUChecked.c_str());

	wstring strDTUDisableSendAll = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"DTUDisableSendAll",L"0");
	if(strDTUDisableSendAll==L"")
		strDTUDisableSendAll = L"0";
	m_bDisableSendAll = _wtoi(strDTUDisableSendAll.c_str());

	wstring strDTUPort = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"DTUComPort",L"0");
	if(strDTUPort==L"")
		strDTUPort = L"0";
	m_nDTUPort = _wtoi(strDTUPort.c_str());

	wstring strDTUMinType = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"DTUMinType",L"2");
	if(strDTUMinType==L"")
		strDTUMinType = L"2";
	m_nDTUMinType = _wtoi(strDTUMinType.c_str());

	//tcp
	wstring strTcpSenderEnabled = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"TcpSenderEnabled",L"0");
	if(strTcpSenderEnabled==L"")
		strTcpSenderEnabled = L"0";
	m_bTCPEnable = _wtoi(strTcpSenderEnabled.c_str());

	wstring strTcpSenderPort = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"TcpSenderPort",L"9500");
	if(strTcpSenderPort==L"" || strTcpSenderPort==L"0")
		strTcpSenderPort = L"9500";
	m_nTCPServerPort = _wtoi(strTcpSenderPort.c_str());

	wstring strTcpSenderName = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"TcpSenderName",L"XXXXXXXXXXX");
	m_strTCPName = strTcpSenderName.c_str();

	wstring strTCPSendFilePackageSize = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"TCPSendFilePackageSize",L"4000");
	m_nTCPSendFilePackageSize = _wtoi(strTCPSendFilePackageSize.c_str());

	wstring strTCPSendFilePackageInterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"TCPSendFilePackageInterval",L"1000");
	m_nTCPSendFilePackageInterval = _wtoi(strTCPSendFilePackageInterval.c_str());

	wstring strTcpSenderIP = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"TcpSenderIP",L"");
	if(strTcpSenderIP==L"" || strTcpSenderIP==L"0.0.0.0")
		strTcpSenderIP = L"";

	m_strTCPIP = strTcpSenderIP.c_str();

	wstring strSendAll = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"sendall",L"1");
	if(strSendAll==L"")
		strSendAll = L"1";
	m_nSendAll = _wtoi(strSendAll.c_str());

	if(m_bDTUEnable && !m_bTCPEnable)
	{
		if(m_strTCPName != L"" && m_strTCPName != L"XXXXXXXXXXX" && m_nSendAll == 1)
		{
			m_nDTUType = 2;
		}
		else
		{
			m_nDTUType = 1;
		}
	}
	else if(m_bTCPEnable && !m_bDTUEnable)
	{
		m_nDTUType = 3;
	}
	else
	{
		m_nDTUType = _wtoi(m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"DTUType",L"0").c_str());
	}
	EnableControl();
	UpdateData(FALSE);

	return true;
}
