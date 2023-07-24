// CommPLCSearch.cpp : implementation file
//

#include "stdafx.h"
#include "CommPLCSearch.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"
#include "S7UDPCtrl.h"

// CCommPLCSearch dialog

IMPLEMENT_DYNAMIC(CCommPLCSearch, CDialog)

CCommPLCSearch::CCommPLCSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CCommPLCSearch::IDD, pParent)
	, m_nSlack(-1)
	, m_nSlot(-1)
	, m_strPlcPointAddress(_T("S7:[XX]DB1,REAL0"))
	, m_strPLCIP(_T("127.0.0.1"))
	, m_dValue(0)
	, m_pS7UDPCtrl(NULL)
	, m_bConnectOK(false)
	, m_strLOG(_T(""))
{
	
}

CCommPLCSearch::~CCommPLCSearch()
{
	m_bConnectOK = false;
	if(m_pS7UDPCtrl)
	{
		m_pS7UDPCtrl->ExitPLCConnection();
		delete m_pS7UDPCtrl;
		m_pS7UDPCtrl = NULL;
	}
}

void CCommPLCSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLC_ADDRESS, m_IPLCIP);
	DDX_Text(pDX, IDC_EDIT_SLACK, m_nSlack);
	DDX_Text(pDX, IDC_EDIT_SLOT, m_nSlot);
	DDX_Text(pDX, IDC_EDIT_PLC_POINT_ADDRESS, m_strPlcPointAddress);
	DDX_Text(pDX, IDC_EDIT_PLC_POINT_VALUE, m_dValue);
	DDX_Control(pDX, IDC_EDIT_LOG_COM, m_EditLog);
	DDX_Text(pDX, IDC_EDIT_LOG_COM, m_strLOG);
}


BEGIN_MESSAGE_MAP(CCommPLCSearch, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_PLC, &CCommPLCSearch::OnBnClickedButtonConnectPlc)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CCommPLCSearch::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CCommPLCSearch::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CCommPLCSearch::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_PLC_MAUAL, &CCommPLCSearch::OnBnClickedButtonConnectPlcMaual)
	ON_STN_CLICKED(IDC_STATIC_2, &CCommPLCSearch::OnStnClickedStatic2)
END_MESSAGE_MAP()


BOOL CCommPLCSearch::OnInitDialog()
{
	CDialog::OnInitDialog();
	((CButton*)GetDlgItem(IDC_BUTTON_DISCONNECT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_READ))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_WRITE))->EnableWindow(FALSE);
	m_IPLCIP.SetAddress(127,0,0,1);
	return TRUE;
}

bool CCommPLCSearch::Exit()
{
	if(m_pS7UDPCtrl)
	{
		m_pS7UDPCtrl->ExitPLCConnection();
		delete m_pS7UDPCtrl;
		m_pS7UDPCtrl = NULL;
	}
	return true;
}

bool CCommPLCSearch::InitData( bool bConnected /*= true*/ )
{
	return true;
}


// CCommPLCSearch message handlers

LRESULT CCommPLCSearch::OnRecData( const char* buffer,DWORD userdata )
{
	CCommPLCSearch *pCommunication =(CCommPLCSearch *)userdata; 
	if(pCommunication)
	{
		pCommunication->AddLog(Project::Tools::AnsiToWideChar(buffer).c_str());
	}
	return 0;
}

bool CCommPLCSearch::AddLog( CString strLog )
{
	m_strLOG+= strLog;
	m_strLOG+= L"\r\n";
	m_EditLog.SetWindowTextW(m_strLOG);
	m_EditLog.LineScroll(m_EditLog.GetLineCount()-1, 0);
	return true;
}

void CCommPLCSearch::OnBnClickedButtonConnectPlc()
{
	UpdateData(TRUE);
	BYTE f0,f1,f2,f3;
	m_IPLCIP.GetAddress(f0,f1,f2,f3);
	m_strPLCIP.Format(_T("%d.%d.%d.%d"),f0,f1,f2,f3);

	if(m_pS7UDPCtrl == NULL)
	{
		m_pS7UDPCtrl = new CS7UDPCtrl();
	}
	m_pS7UDPCtrl->ExitPLCConnection();
	if(m_pS7UDPCtrl->InitPLCConnection(m_strPLCIP.GetString(),m_nSlack,m_nSlot,OnRecData,(DWORD)this))
	{
		MessageBox(_T("PLC连接成功"));
		AddLog(_T("PLC连接成功"));
		m_bConnectOK = true;
		((CButton*)GetDlgItem(IDC_BUTTON_DISCONNECT))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_READ))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_WRITE))->EnableWindow(TRUE);
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(_T("PLC连接失败"));
		AddLog(_T("PLC连接失败"));
	}
}

void CCommPLCSearch::OnBnClickedButtonDisconnect()
{
	m_bConnectOK = false;
	if(m_pS7UDPCtrl)
	{
		m_pS7UDPCtrl->ExitPLCConnection();
		delete m_pS7UDPCtrl;
		m_pS7UDPCtrl = NULL;
	}
	((CButton*)GetDlgItem(IDC_BUTTON_DISCONNECT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_READ))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_WRITE))->EnableWindow(FALSE);

}

void CCommPLCSearch::OnBnClickedButtonRead()
{
	if(!m_bConnectOK)
	{
		MessageBox(_T("请连接PLC"));
		AddLog(_T("请连接PLC"));
		return;
	}

	UpdateData(TRUE);
	BYTE f0,f1,f2,f3;
	m_IPLCIP.GetAddress(f0,f1,f2,f3);
	m_strPLCIP.Format(_T("%d.%d.%d.%d"),f0,f1,f2,f3);

	if(m_strPlcPointAddress.GetLength() <= 0)
	{
		MessageBox(_T("PLC地址是空的"));
		AddLog(_T("PLC地址是空的"));
		return;
	}

	if(m_pS7UDPCtrl)
	{
		if(m_pS7UDPCtrl->ReadPLCData(m_strPlcPointAddress.GetString(),m_dValue))
		{
			UpdateData(FALSE);
			MessageBox(_T("读取成功"));
			CString str;
			str.Format(_T("读取成功(%s:%f)"),m_strPlcPointAddress,m_dValue);
			AddLog(str);
			return;
		}
	}

	MessageBox(_T("读取失败"));
	AddLog(_T("读取失败"));
	return;
}

void CCommPLCSearch::OnBnClickedButtonWrite()
{
	if(!m_bConnectOK)
	{
		MessageBox(_T("请连接PLC"));
		AddLog(_T("请连接PLC"));
		return;
	}

	UpdateData(TRUE);
	BYTE f0,f1,f2,f3;
	m_IPLCIP.GetAddress(f0,f1,f2,f3);
	m_strPLCIP.Format(_T("%d.%d.%d.%d"),f0,f1,f2,f3);

	if(m_strPlcPointAddress.GetLength() <= 0)
	{
		MessageBox(_T("PLC地址是空的"));
		AddLog(_T("PLC地址是空的"));
		return;
	}

	if(m_pS7UDPCtrl)
	{
		if(m_pS7UDPCtrl->WritePLCData(m_strPlcPointAddress.GetString(),m_dValue))
		{
			MessageBox(_T("写入成功"));
			AddLog(_T("写入成功"));
			return;
		}
	}

	MessageBox(_T("写入失败"));
	AddLog(_T("写入失败"));
	return;
}

void CCommPLCSearch::OnBnClickedButtonConnectPlcMaual()
{
	UpdateData(TRUE);
	BYTE f0,f1,f2,f3;
	m_IPLCIP.GetAddress(f0,f1,f2,f3);
	m_strPLCIP.Format(_T("%d.%d.%d.%d"),f0,f1,f2,f3);

	if(m_pS7UDPCtrl == NULL)
	{
		m_pS7UDPCtrl = new CS7UDPCtrl();
	}
	m_pS7UDPCtrl->ExitPLCConnection();
	if(m_pS7UDPCtrl->AutoConnectPLC(m_strPLCIP.GetString(),m_nSlack,m_nSlot))
	{
		MessageBox(_T("PLC连接成功"));
		AddLog(_T("PLC连接成功"));
		m_bConnectOK = true;
		((CButton*)GetDlgItem(IDC_BUTTON_DISCONNECT))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_READ))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_WRITE))->EnableWindow(TRUE);
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(_T("PLC连接失败"));
		AddLog(_T("PLC连接失败"));
	}
}


void CCommPLCSearch::OnStnClickedStatic2()
{
	// TODO: Add your control notification handler code here
}
