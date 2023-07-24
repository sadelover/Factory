// SelectBacnetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelectBacnetDlg.h"
#include "afxdialogex.h"


// CSelectBacnetDlg dialog

IMPLEMENT_DYNAMIC(CSelectBacnetDlg, CDialog)

CSelectBacnetDlg::CSelectBacnetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectBacnetDlg::IDD, pParent)
{

}

CSelectBacnetDlg::~CSelectBacnetDlg()
{
}

void CSelectBacnetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrl);
}


BEGIN_MESSAGE_MAP(CSelectBacnetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelectBacnetDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSelectBacnetDlg::OnNMDblclkList1)
END_MESSAGE_MAP()

BOOL CSelectBacnetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitList();
	ShowList();
	return TRUE;
}


// CSelectBacnetDlg message handlers


void CSelectBacnetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_vecSelectBacnetList.clear();
	for(int i=0;i < m_listctrl.GetItemCount(); i++)
	{ 
		if( m_listctrl.GetCheck(i)) 
		{
			CString strKey = m_listctrl.GetItemText(i, 1);
			m_vecSelectBacnetList.push_back(strKey.GetString());
		}
	}
	CDialog::OnOK();
}


void CSelectBacnetDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int i = 0;
	for(;i < m_listctrl.GetItemCount(); i++) { 
		if( m_listctrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ) 
			break;
	}
	*pResult = 0;
}

void CSelectBacnetDlg::InitList()
{
	m_listctrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );
	m_listctrl.InsertColumn(0, L"Select", LVCFMT_CENTER, 50);
	m_listctrl.InsertColumn(1, L"Bacnet Device", LVCFMT_LEFT, 250);
}

void CSelectBacnetDlg::ShowList()
{
	m_listctrl.SetRedraw(FALSE);
	m_listctrl.DeleteAllItems();
	for (unsigned int i = 0; i < m_vecBacnetList.size(); i++){
		m_listctrl.InsertItem(i,L"");
		m_listctrl.SetItemText(i, 1, m_vecBacnetList[i].c_str());
	}
	m_listctrl.SetRedraw(TRUE);
}
