// SetSiemens.cpp : implementation file
//

#include "stdafx.h"
#include "SetSiemens.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>

// CSetSiemens dialog

IMPLEMENT_DYNAMIC(CSetSiemens, CDialog)

CSetSiemens::CSetSiemens(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSiemens::IDD, pParent)
	, m_nPLCErrReConnectCount(0)
	, m_bNetCP243(FALSE)
	, nAccessInterfaceType(0)
{

}

CSetSiemens::~CSetSiemens()
{
}
BOOL CSetSiemens::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

void CSetSiemens::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PLC_ERR_COUNT, m_nPLCErrReConnectCount);
	DDX_Check(pDX, IDC_CHECK_CP243, m_bNetCP243);
	DDX_Radio(pDX, IDC_RADIO1, nAccessInterfaceType);
}


BEGIN_MESSAGE_MAP(CSetSiemens, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_BACNET2, &CSetSiemens::OnBnClickedButtonSettingBacnet2)
END_MESSAGE_MAP()


// CSetSiemens message handlers


void CSetSiemens::OnBnClickedButtonSettingBacnet2()
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	if(m_pDataAccess)
	{
		CString str;
		str.Format(_T("%d"),m_nPLCErrReConnectCount);
		m_pDataAccess->WriteCoreDebugItemValue(L"errrconnectcount",str.GetString());

		if(m_bNetCP243)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"dave_proto_iso_tcp243", L"1");
		}
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"dave_proto_iso_tcp243", L"0");

		}
		
		//访问类型接口
		if(nAccessInterfaceType == 0)
			m_pDataAccess->WriteCoreDebugItemValue(L"siemense_communication_type", L"1");
		else if(nAccessInterfaceType == 1)
			m_pDataAccess->WriteCoreDebugItemValue(L"siemense_communication_type", L"2");

		AfxMessageBox(_T("保存成功"));
	}	
		
}
bool CSetSiemens::InitData()
{
	//plc
	wstring strPLCErrReConnectCount = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"errrconnectcount",L"30");
	if(strPLCErrReConnectCount==L"")
		strPLCErrReConnectCount = L"30";
	m_nPLCErrReConnectCount = _wtoi(strPLCErrReConnectCount.c_str());


	wstring strCP243 = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"dave_proto_iso_tcp243",L"0");
	m_bNetCP243 = (_wtoi(strCP243.c_str())==1);

	wstring strAccessInterfaceType = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"siemense_communication_type",L"1");
	if(strAccessInterfaceType == L"1")
	{
		nAccessInterfaceType = 0;
	}
	else if(strAccessInterfaceType == L"2")
	{
		nAccessInterfaceType = 1;
	}
	

	UpdateData(FALSE);
	return true;
}