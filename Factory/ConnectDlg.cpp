// ConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConnectDlg.h"
#include "afxdialogex.h"


// CConnectDlg dialog

IMPLEMENT_DYNAMIC(CConnectDlg, CSkinDlg)

CConnectDlg::CConnectDlg(const CString strDbIp, CWnd* pParent /*=NULL*/)
	: CSkinDlg(CConnectDlg::IDD, pParent)
	, m_strDbIp(strDbIp)
	, m_bIsLocalhost(FALSE)
{
}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDR_DB, m_ipDatabase);
	DDX_Check(pDX, IDC_CHECK_LOCALHOST, m_bIsLocalhost);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CSkinDlg)
	ON_BN_CLICKED(IDOK, &CConnectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_LOCALHOST, &CConnectDlg::OnBnClickedCheckLocalhost)
END_MESSAGE_MAP()


// CConnectDlg message handlers


BOOL CConnectDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	USES_CONVERSION;
	char* pIp = T2A(m_strDbIp.GetBuffer());
	m_strDbIp.ReleaseBuffer();

	m_ipDatabase.SetAddress(htonl(inet_addr(pIp)));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CConnectDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	BYTE bIp[4] = {0};
	m_ipDatabase.GetAddress(bIp[0], bIp[1], bIp[2], bIp[3]);
	m_strDbIp.Format(_T("%d.%d.%d.%d"), bIp[0], bIp[1], bIp[2], bIp[3]);

	CSkinDlg::OnOK();
}


void CConnectDlg::OnBnClickedCheckLocalhost()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bIsLocalhost)
	{
		m_strDbIp = _T("127.0.0.1");

		USES_CONVERSION;
		char* pIp = T2A(m_strDbIp.GetBuffer());
		m_strDbIp.ReleaseBuffer();

		m_ipDatabase.SetAddress(htonl(inet_addr(pIp)));
		m_ipDatabase.EnableWindow(FALSE);
	}
	else
	{
		m_ipDatabase.EnableWindow(TRUE);
	}
}
