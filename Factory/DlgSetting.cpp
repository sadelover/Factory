// DlgSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "BEOP-Designer.h"
#include "DlgSetting.h"
#include "LocalConfigManageDlg.h"
#include "MainFrm.h"
#include "json/json.h"

#include "DlgSetting_EditDlg.h"
#include "DlgSetting_AccountManagerDlg.h"
#include "DlgSetting_OMDlg.h"

// CDlgSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSetting, CDialog)

CDlgSetting::CDlgSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetting::IDD, pParent)
{

}

CDlgSetting::~CDlgSetting()
{
	auto p = m_childDlgList.begin();
	for(; p!=m_childDlgList.end(); p++ )
	{
		if(*p != NULL)
			delete (*p);
	}
}

void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CHILD, m_tab_ctrl);
}

BEGIN_MESSAGE_MAP(CDlgSetting, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CHILD, &CDlgSetting::OnTcnSelchangeTabChild)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_CHILD, &CDlgSetting::OnTcnSelchangingTabChild)
END_MESSAGE_MAP()


// CDlgSetting 消息处理程序

BOOL CDlgSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_tab_ctrl.InsertItem(0,L"编辑");
	m_tab_ctrl.InsertItem(1,L"账户管理");
	m_tab_ctrl.InsertItem(1,L"显示管理");

	CRect rc;
	m_tab_ctrl.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;

	CDlgSetting_EditDlg* pDlg1 = new CDlgSetting_EditDlg;
	pDlg1->Create(IDD_CONN_SETTING_EDIT, &m_tab_ctrl);
	pDlg1->MoveWindow(&rc);
	pDlg1->ShowWindow(SW_SHOW);
	m_childDlgList.push_back(pDlg1);

	CDlgSetting_AccountManagerDlg * pDlg2 = new CDlgSetting_AccountManagerDlg;
	pDlg2->Create(IDD_CONN_SETTING_ACCOUNT_MANAGER, &m_tab_ctrl);
	pDlg2->MoveWindow(&rc);
	m_childDlgList.push_back(pDlg2);
	pDlg2->ShowWindow(SW_HIDE);

	CDlgSetting_OMDlg * pDlg3 = new CDlgSetting_OMDlg;
	pDlg3->Create(IDD_CONN_SETTING_OM, &m_tab_ctrl);
	pDlg3->MoveWindow(&rc);
	m_childDlgList.push_back(pDlg3);
	pDlg3->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetting::OnTcnSelchangeTabChild(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_tab_ctrl.GetCurSel();
	if(nSel < m_childDlgList.size() && nSel >= 0)
	{
		CDialogEx* p = NULL;
		p = m_childDlgList.at(nSel);
		if(p != NULL)
			p->ShowWindow(SW_SHOW);
	}
	
	*pResult = 0;
}


void CDlgSetting::OnTcnSelchangingTabChild(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_tab_ctrl.GetCurSel();
	if(nSel < m_childDlgList.size() && nSel >= 0)
	{
		CDialogEx* p = NULL;
		p = m_childDlgList.at(nSel);
		if(p != NULL)
			p->ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}
