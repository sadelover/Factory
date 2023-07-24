// SetBacnet.cpp : implementation file
//

#include "stdafx.h"
#include "SetBacnet.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>

// CSetBacnet dialog

IMPLEMENT_DYNAMIC(CSetBacnet, CDialog)

CSetBacnet::CSetBacnet(CWnd* pParent /*=NULL*/)
	: CDialog(CSetBacnet::IDD, pParent)
	, m_nBacnetReadMode(0)
	, m_nBacnetReadAllInterval(0)
	, m_nBacnetReadTypeInterval(0)
	, m_nBacnetReadPointInterval(0)
	, m_nBacnetreadtimeout(0)
	, m_strBacnetReadCountPerTime(_T(""))
	, m_nBacnetWritePriority(0)
{

}

CSetBacnet::~CSetBacnet()
{

}
BOOL CSetBacnet::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}
void CSetBacnet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_BACNET_READ_MODE, m_nBacnetReadMode);
	DDX_Control(pDX, IDC_COMBO_BACNET, m_comBacnetIP);
	DDX_Text(pDX, IDC_EDIT_MODBUS_INTERVAL2, m_nBacnetReadAllInterval);
	DDX_Text(pDX, IDC_EDIT_BACNET_TYPE_INTERVAL, m_nBacnetReadTypeInterval);
	DDX_Text(pDX, IDC_EDIT_BACNET_POINT_INTERVAL, m_nBacnetReadPointInterval);
	DDX_Text(pDX, IDC_EDIT_BACNET_POINT_INTERVAL3, m_nBacnetreadtimeout);
	DDX_Text(pDX, IDC_EDIT_BACNET_READ_PERCOUNT, m_strBacnetReadCountPerTime);
	DDX_Text(pDX, IDC_EDIT_BACNET_WRITE_PRIORITY, m_nBacnetWritePriority);
}


BEGIN_MESSAGE_MAP(CSetBacnet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_BACNET, &CSetBacnet::OnBnClickedButtonSettingBacnet)
	ON_EN_CHANGE(IDC_EDIT_BACNET_READ_PERCOUNT, &CSetBacnet::OnEnChangeEditBacnetReadPercount)
	ON_EN_CHANGE(IDC_EDIT_MODBUS_INTERVAL2, &CSetBacnet::OnEnChangeEditModbusInterval2)
END_MESSAGE_MAP()


// CSetBacnet message handlers

void CSetBacnet::OnBnClickedButtonSettingBacnet()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_pDataAccess)
	{
		CString str;
		str.Format(_T("%d"),m_nBacnetReadMode);
		m_pDataAccess->WriteCoreDebugItemValue(L"readbacnetmode",str.GetString());

		str.Format(_T("%d"),m_nBacnetReadAllInterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"readinterval",str.GetString());

		str.Format(_T("%d"),m_nBacnetReadTypeInterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"readtypeinterval",str.GetString());

		str.Format(_T("%d"),m_nBacnetReadPointInterval);
		m_pDataAccess->WriteCoreDebugItemValue(L"readcmdinterval",str.GetString());

		str.Format(_T("%d"),m_nBacnetreadtimeout);
		m_pDataAccess->WriteCoreDebugItemValue(L"readtimeout",str.GetString());

		m_pDataAccess->WriteCoreDebugItemValue(L"bacnetreadlimit",m_strBacnetReadCountPerTime.GetString());

		str.Format(_T("%d"), m_nBacnetWritePriority);
		m_pDataAccess->WriteCoreDebugItemValue(L"writepriority",str.GetString());


		AfxMessageBox(_T("保存成功,请重启core.exe!"));
	}

	int nSelect = m_comBacnetIP.GetCurSel();
	if(nSelect >= 0)
	{
		CString strIP;
		m_comBacnetIP.GetLBText(nSelect,strIP);

		if(ChangeEnviromentVariables(_T("BACNET_IFACE"),strIP))
		{
			AfxMessageBox(_T("保存成功,首先可能需要重启你的计算机生效!"));
		}
	}	
}
bool CSetBacnet::ChangeEnviromentVariables( CString szName,CString szNewValue)
{
	HKEY hkResult;//键的句柄
	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"), 0, KEY_SET_VALUE | KEY_READ, &hkResult)) //打开键environment   
	{       
		AfxMessageBox(_T("错误：需要足够的权限访问注册表,使用管理员权限运行!"));
		return false;
	}     

	LPTSTR javaHome = szNewValue.GetBuffer();
	szNewValue.ReleaseBuffer(); 

	DWORD dwRet = RegSetValueEx(hkResult,szName,0, REG_EXPAND_SZ, (LPBYTE) javaHome,_tcsclen(javaHome)*sizeof(TCHAR));//设置某子键下特定名称的值。
	if(dwRet == ERROR_SUCCESS)
	{
		DWORD dwResult;     
		SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0,LPARAM("Environment"), SMTO_ABORTIFHUNG, 5000, &dwResult);//广播立即执行            
		//delete temp;
		AfxMessageBox(_T("保存成功,需要手动重新启动Core!"));
	}
	else
	{
		AfxMessageBox(_T("环境变量设置失败!"));
	}
	RegCloseKey(hkResult);//释放键句柄
	return true;
}
bool CSetBacnet::InitData()
{
	//bacnet
	wstring strBacnetReadAllInterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"readinterval",L"5");
	m_nBacnetReadAllInterval = _wtoi(strBacnetReadAllInterval.c_str());

	wstring strBacnetReadTypeInterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"readtypeinterval",L"1");
	m_nBacnetReadTypeInterval = _wtoi(strBacnetReadTypeInterval.c_str());

	wstring strBacnetReadPointInterval = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"readcmdinterval",L"100");
	m_nBacnetReadPointInterval = _wtoi(strBacnetReadPointInterval.c_str());

	wstring strBacnetReadMode = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"readbacnetmode",L"0");
	m_nBacnetReadMode = _wtoi(strBacnetReadMode.c_str());

	wstring strBacnetreadtimeout = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"readtimeout",L"2000");
	m_nBacnetreadtimeout = _wtoi(strBacnetreadtimeout.c_str());

	wstring strBacnetReadCountPerTime = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"bacnetreadlimit",L"20");
	m_strBacnetReadCountPerTime = strBacnetReadCountPerTime.c_str();


	wstring strBacnetWritePriority = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"writepriority",L"16");
	m_nBacnetWritePriority = _wtoi(strBacnetWritePriority.c_str());

	//search ip
	char* czBacnetIP;
	czBacnetIP = getenv("BACNET_IFACE");

	CToolsFunction toolsFunction;
	IP_ADAPTER_INFO* pIpAdapterInfo = toolsFunction.GetAllLocalMachineEthInfo();
	m_comBacnetIP.ResetContent();
	if(pIpAdapterInfo != NULL)
	{
		while (pIpAdapterInfo)
		{
			// Loop for multi-ip address in a ethernet card.
			IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			do 
			{
				if (*(pIpAddrString->IpAddress.String) != '\0' && strcmp(pIpAddrString->IpAddress.String, "0.0.0.0") != 0)
				{
					m_comBacnetIP.AddString(Project::Tools::AnsiToWideChar(pIpAddrString->IpAddress.String).c_str());
				}
				pIpAddrString = pIpAddrString->Next;

			} while (pIpAddrString);

			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}

	CString strBacnetIP;
	if(czBacnetIP)
		strBacnetIP = Project::Tools::AnsiToWideChar(czBacnetIP).c_str();
	int nSelect = -1;
	for(int i=0; i <m_comBacnetIP.GetCount(); ++i)
	{
		CString strIP;
		m_comBacnetIP.GetLBText(i,strIP); 
		if(strBacnetIP == strIP)
		{
			m_comBacnetIP.SetCurSel(i);
			break;
		}
	}
	UpdateData(FALSE);

	return true;
}


void CSetBacnet::OnEnChangeEditBacnetReadPercount()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CSetBacnet::OnEnChangeEditModbusInterval2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
