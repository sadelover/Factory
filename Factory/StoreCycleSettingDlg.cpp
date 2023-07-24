// StoreCycleSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StoreCycleSettingDlg.h"
#include "afxdialogex.h"


// CStoreCycleSettingDlg 对话框

IMPLEMENT_DYNAMIC(CStoreCycleSettingDlg, CDialog)

CStoreCycleSettingDlg::CStoreCycleSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStoreCycleSettingDlg::IDD, pParent)
	, m_str_combo_cycle(_T(""))
{

}

CStoreCycleSettingDlg::~CStoreCycleSettingDlg()
{
}

void CStoreCycleSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STORE_CYCLE_FOR_MULTI, m_combo_cycle);
	DDX_CBString(pDX, IDC_COMBO_STORE_CYCLE_FOR_MULTI, m_str_combo_cycle);
}


BEGIN_MESSAGE_MAP(CStoreCycleSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CStoreCycleSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CStoreCycleSettingDlg 消息处理程序


BOOL CStoreCycleSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_combo_cycle.AddString(L"不存储");
	m_combo_cycle.AddString(L"5秒钟");
	m_combo_cycle.AddString(L"1分钟");
	m_combo_cycle.AddString(L"5分钟");
	m_combo_cycle.AddString(L"半小时");
	m_combo_cycle.AddString(L"1小时");
	m_combo_cycle.AddString(L"1天");
	m_combo_cycle.AddString(L"1周（7天）");
	m_combo_cycle.AddString(L"1个月");
	m_combo_cycle.AddString(L"1年");
	m_combo_cycle.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CStoreCycleSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCycle = m_combo_cycle.GetCurSel();
	CString	strSelName;
	m_combo_cycle.GetLBText(m_nCycle, strSelName);
	if (L"半小时" == strSelName)
	{
		MessageBox(_T("存储周期已不支持半小时！\n请重新选择。"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}
