// SetTable.cpp : implementation file
//

#include "stdafx.h"
#include "SetTable.h"
#include "afxdialogex.h"

// CSetTable dialog

IMPLEMENT_DYNAMIC(CSetTable, CDialog)

CSetTable::CSetTable(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTable::IDD, pParent)
{

}

CSetTable::~CSetTable()
{
}

void CSetTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB2, m_SetTab);
}


BEGIN_MESSAGE_MAP(CSetTable, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CSetTable::OnTcnSelchangeTab2)
END_MESSAGE_MAP()


// CSetTable message handlers
BOOL CSetTable::OnInitDialog()
{
	CDialog::OnInitDialog();
	//设置表格页面标题；
	m_SetTab.InsertItem(0,_T("  DTU设置  "));
	m_SetTab.InsertItem(1,_T("  Modbus设置  "));
	m_SetTab.InsertItem(2,_T("  Bacnet设置  "));
	m_SetTab.InsertItem(3,_T("  Watch设置  "));
	m_SetTab.InsertItem(4,_T("  OPC设置  "));
	m_SetTab.InsertItem(5,_T("  西门子300/400/1200设置  "));
	m_SetTab.InsertItem(6,_T("  数据库设置  "));
	m_SetTab.InsertItem(7,_T("  远程设置  "));
	m_SetTab.InsertItem(8,_T("  调试设置  "));
	m_SetTab.InsertItem(9,_T("  运行参数  "));
	//关联页面对话框;
	//建立属性页各页;
	m_SetBacnet.Create(IDD_DIALOG_SETBACNET,GetDlgItem(IDC_TAB2));
	m_SetDatabaseset.Create(IDD_DIALOG_SETDATABASE,GetDlgItem(IDC_TAB2));
	m_SetDebug.Create(IDD_DIALOG_SETDEBUG,GetDlgItem(IDC_TAB2));
	m_SetDTU.Create(IDD_DIALOG_SETDTU,GetDlgItem(IDC_TAB2));
	m_SetModbus.Create(IDD_DIALOG_SETMODBUS,GetDlgItem(IDC_TAB2));
	m_SetOPC.Create(IDD_DIALOG_SETOPC,GetDlgItem(IDC_TAB2));
	m_SetRemote.Create(IDD_DIALOG_SETREMOTE,GetDlgItem(IDC_TAB2));
	m_SetSiemens.Create(IDD_DIALOG_SETSIEMENS,GetDlgItem(IDC_TAB2));
	m_SetWatch.Create(IDD_DIALOG_SETWATCH,GetDlgItem(IDC_TAB2));
	m_SetRunParam.Create(IDD_DIALOG_SETRUNPARAM,GetDlgItem(IDC_TAB2));
	//设置表格显示范围;
	CRect rect;
	m_SetTab.GetClientRect(&rect);
	rect.top+=28;
	rect.bottom-=2;
	rect.left+=2;
	rect.right-=2;
	//设置各页面显示属性；
	m_SetDTU.MoveWindow(&rect);
	m_SetModbus.MoveWindow(&rect);
	m_SetBacnet.MoveWindow(&rect);
	m_SetOPC.MoveWindow(&rect);
	m_SetSiemens.MoveWindow(&rect);
	m_SetDatabaseset.MoveWindow(&rect);
	m_SetRemote.MoveWindow(&rect);
	m_SetDebug.MoveWindow(&rect);
	m_SetWatch.MoveWindow(&rect);
	m_SetRunParam.MoveWindow(&rect);

	m_SetBacnet.ShowWindow(FALSE);
	m_SetDatabaseset.ShowWindow(FALSE);
	m_SetDebug.ShowWindow(FALSE);
	m_SetModbus.ShowWindow(FALSE);
	m_SetOPC.ShowWindow(FALSE);
	m_SetRemote.ShowWindow(FALSE);
	m_SetSiemens.ShowWindow(FALSE);
	m_SetWatch.ShowWindow(FALSE);
	m_SetRunParam.ShowWindow(FALSE);
	m_SetDTU.ShowWindow(TRUE);
	m_SetTab.SetCurSel(0);
	return TRUE;
}

void CSetTable::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nCurSel;
	nCurSel = m_SetTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		m_SetBacnet.ShowWindow(FALSE);
		m_SetDatabaseset.ShowWindow(FALSE);
		m_SetDebug.ShowWindow(FALSE);
		m_SetModbus.ShowWindow(FALSE);
		m_SetOPC.ShowWindow(FALSE);
		m_SetRemote.ShowWindow(FALSE);
		m_SetSiemens.ShowWindow(FALSE);
		m_SetWatch.ShowWindow(FALSE);
		m_SetRunParam.ShowWindow(FALSE);
		m_SetDTU.ShowWindow(TRUE);
		break;
	case 1:
		m_SetBacnet.ShowWindow(FALSE);
		m_SetDatabaseset.ShowWindow(FALSE);
		m_SetDebug.ShowWindow(FALSE);
		m_SetModbus.ShowWindow(TRUE);
		m_SetOPC.ShowWindow(FALSE);
		m_SetRemote.ShowWindow(FALSE);
		m_SetSiemens.ShowWindow(FALSE);
		m_SetWatch.ShowWindow(FALSE);
		m_SetRunParam.ShowWindow(FALSE);
		m_SetDTU.ShowWindow(FALSE);
		break;
	case 2:
		m_SetBacnet.ShowWindow(TRUE);
		m_SetDatabaseset.ShowWindow(FALSE);
		m_SetDebug.ShowWindow(FALSE);
		m_SetModbus.ShowWindow(FALSE);
		m_SetOPC.ShowWindow(FALSE);
		m_SetRemote.ShowWindow(FALSE);
		m_SetSiemens.ShowWindow(FALSE);
		m_SetWatch.ShowWindow(FALSE);
		m_SetRunParam.ShowWindow(FALSE);
		m_SetDTU.ShowWindow(FALSE);
		break;
	case 3:
		m_SetBacnet.ShowWindow(FALSE);
		m_SetDatabaseset.ShowWindow(FALSE);
		m_SetDebug.ShowWindow(FALSE);
		m_SetModbus.ShowWindow(FALSE);
		m_SetOPC.ShowWindow(FALSE);
		m_SetRemote.ShowWindow(FALSE);
		m_SetSiemens.ShowWindow(FALSE);
		m_SetWatch.ShowWindow(TRUE);
		m_SetRunParam.ShowWindow(FALSE);
		m_SetDTU.ShowWindow(FALSE);
		break;
	case 4:
		m_SetBacnet.ShowWindow(FALSE);
		m_SetDatabaseset.ShowWindow(FALSE);
		m_SetDebug.ShowWindow(FALSE);
		m_SetModbus.ShowWindow(FALSE);
		m_SetOPC.ShowWindow(TRUE);
		m_SetRemote.ShowWindow(FALSE);
		m_SetSiemens.ShowWindow(FALSE);
		m_SetWatch.ShowWindow(FALSE);
		m_SetRunParam.ShowWindow(FALSE);
		m_SetDTU.ShowWindow(FALSE);
		break;
	case 5:
		m_SetBacnet.ShowWindow(FALSE);
		m_SetDatabaseset.ShowWindow(FALSE);
		m_SetDebug.ShowWindow(FALSE);
		m_SetModbus.ShowWindow(FALSE);
		m_SetOPC.ShowWindow(FALSE);
		m_SetRemote.ShowWindow(FALSE);
		m_SetSiemens.ShowWindow(TRUE);
		m_SetWatch.ShowWindow(FALSE);
		m_SetRunParam.ShowWindow(FALSE);
		m_SetDTU.ShowWindow(FALSE);
		break;
	case 6:
		m_SetBacnet.ShowWindow(FALSE);
		m_SetDatabaseset.ShowWindow(TRUE);
		m_SetDebug.ShowWindow(FALSE);
		m_SetModbus.ShowWindow(FALSE);
		m_SetOPC.ShowWindow(FALSE);
		m_SetRemote.ShowWindow(FALSE);
		m_SetSiemens.ShowWindow(FALSE);
		m_SetWatch.ShowWindow(FALSE);
		m_SetRunParam.ShowWindow(FALSE);
		m_SetDTU.ShowWindow(FALSE);
		break;
	case 7:
		m_SetBacnet.ShowWindow(FALSE);
		m_SetDatabaseset.ShowWindow(FALSE);
		m_SetDebug.ShowWindow(FALSE);
		m_SetModbus.ShowWindow(FALSE);
		m_SetOPC.ShowWindow(FALSE);
		m_SetRemote.ShowWindow(TRUE);
		m_SetSiemens.ShowWindow(FALSE);
		m_SetWatch.ShowWindow(FALSE);
		m_SetRunParam.ShowWindow(FALSE);
		m_SetDTU.ShowWindow(FALSE);
		break;
	case 8:
		m_SetBacnet.ShowWindow(FALSE);
		m_SetDatabaseset.ShowWindow(FALSE);
		m_SetDebug.ShowWindow(TRUE);
		m_SetModbus.ShowWindow(FALSE);
		m_SetOPC.ShowWindow(FALSE);
		m_SetRemote.ShowWindow(FALSE);
		m_SetSiemens.ShowWindow(FALSE);
		m_SetWatch.ShowWindow(FALSE);
		m_SetRunParam.ShowWindow(FALSE);
		m_SetDTU.ShowWindow(FALSE);
		break;
	case 9:
		m_SetBacnet.ShowWindow(FALSE);
		m_SetDatabaseset.ShowWindow(FALSE);
		m_SetDebug.ShowWindow(FALSE);
		m_SetModbus.ShowWindow(FALSE);
		m_SetOPC.ShowWindow(FALSE);
		m_SetRemote.ShowWindow(FALSE);
		m_SetSiemens.ShowWindow(FALSE);
		m_SetWatch.ShowWindow(FALSE);
		m_SetRunParam.ShowWindow(TRUE);
		m_SetDTU.ShowWindow(FALSE);
		break;
	default:
		break;
	}
	*pResult = 0;
}
// 新窗口数据初始化;
bool CSetTable::InitData()
{
	//
	m_SetBacnet.m_pDataAccess = m_pDataAccess;
	m_SetDatabaseset.m_pDataAccess = m_pDataAccess;
	m_SetDebug.m_pDataAccess = m_pDataAccess;
	m_SetDTU.m_pDataAccess = m_pDataAccess;
	m_SetModbus.m_pDataAccess = m_pDataAccess;
	m_SetOPC.m_pDataAccess = m_pDataAccess;
	m_SetRemote.m_pDataAccess = m_pDataAccess;
	m_SetSiemens.m_pDataAccess = m_pDataAccess;
	m_SetWatch.m_pDataAccess = m_pDataAccess;
	m_SetRunParam.m_pDataAccess = m_pDataAccess;
	//
	m_SetBacnet.InitData();
	m_SetDatabaseset.InitData();
	m_SetDebug.InitData();
	m_SetDTU.InitData();
	m_SetModbus.InitData();
	m_SetOPC.InitData();
	m_SetRemote.InitData();
	m_SetSiemens.InitData();
	m_SetWatch.InitData();
	m_SetRunParam.InitData();
	return true;
}
