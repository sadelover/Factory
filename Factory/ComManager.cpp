// ComManager.cpp : implementation file
//

#include "stdafx.h"
#include "ComManager.h"
#include "CommPLCSearch.h"

// CComManager dialog

IMPLEMENT_DYNAMIC(CComManager, CDialog)

CComManager::CComManager(CWnd* pParent /*=NULL*/)
	: CDialog(CComManager::IDD, pParent)
{

}

CComManager::~CComManager()
{
}

void CComManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCom);
}


BEGIN_MESSAGE_MAP(CComManager, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CComManager::OnTcnSelchangeTab1)
END_MESSAGE_MAP()

BOOL CComManager::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_tabCom.InsertItem(0,_T("Bacnet"));
	m_tabCom.InsertItem(1,_T("OPC"));	
	m_tabCom.InsertItem(2,_T("PLC"));
	m_tabCom.InsertItem(3,_T("Bacnet Old"));

	//建立属性页各页
	m_BacnetDlg.Create(IDD_DIALOG_COM_SEARCH,GetDlgItem(IDC_TAB1));
	m_pBacnetNewDlg.Create(IDD_DIALOG_BACNET,GetDlgItem(IDC_TAB1));
	m_OPCDlg.Create(IDD_DIALOG_OPC,GetDlgItem(IDC_TAB1));
	m_PlcDlg.Create(IDD_DIALOG_PLC,GetDlgItem(IDC_TAB1));
	//m_BacnetDlg.Create(IDD_DIALOG_COM_SEARCH,GetDlgItem(IDC_TAB1));

	//设置页面的位置在m_tab控件范围内
	CRect rect;
	m_tabCom.GetClientRect(&rect);
	rect.top+=28;
	rect.bottom-=2;
	rect.left+=2;
	rect.right-=2;
	m_BacnetDlg.MoveWindow(&rect);
	m_pBacnetNewDlg.MoveWindow(&rect);
	m_OPCDlg.MoveWindow(&rect);
	m_PlcDlg.MoveWindow(&rect);
	m_BacnetDlg.MoveWindow(&rect);
	m_PlcDlg.ShowWindow(FALSE);
	m_BacnetDlg.ShowWindow(FALSE);
	m_OPCDlg.ShowWindow(FALSE);
	m_pBacnetNewDlg.ShowWindow(TRUE);
	m_tabCom.SetCurSel(0);
	return TRUE;
}

bool CComManager::InitData( bool bConnected /*= true*/ )
{
	m_OPCDlg.InitData(bConnected);
	m_BacnetDlg.InitData(bConnected);
	m_PlcDlg.InitData(bConnected);
	m_pBacnetNewDlg.InitData(bConnected);
	return true;
}

bool CComManager::Exit()
{
	m_OPCDlg.Exit();
	m_BacnetDlg.Exit();
	m_PlcDlg.Exit();
	m_pBacnetNewDlg.Exit();
	return true;
}


// CComManager message handlers


void CComManager::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nCurSel;
	nCurSel = m_tabCom.GetCurSel();
	switch( nCurSel )
	{
	case 0:
		m_BacnetDlg.ShowWindow(FALSE);
		m_OPCDlg.ShowWindow(FALSE);
		m_PlcDlg.ShowWindow(FALSE);
		m_pBacnetNewDlg.ShowWindow(TRUE);
		break;
	case 1:
		m_BacnetDlg.ShowWindow(FALSE);
		m_OPCDlg.ShowWindow(TRUE);
		m_PlcDlg.ShowWindow(FALSE);
		m_pBacnetNewDlg.ShowWindow(FALSE);
		break;
	case 2:
		m_BacnetDlg.ShowWindow(FALSE);
		m_OPCDlg.ShowWindow(FALSE);
		m_PlcDlg.ShowWindow(TRUE);
		m_pBacnetNewDlg.ShowWindow(FALSE);
		break;
	case 3:
		m_BacnetDlg.ShowWindow(TRUE);
		m_OPCDlg.ShowWindow(FALSE);
		m_PlcDlg.ShowWindow(FALSE);
		m_pBacnetNewDlg.ShowWindow(FALSE);
		break;
	default: ;
		break;
	}
	*pResult = 0;
}
