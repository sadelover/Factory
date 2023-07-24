// BindPropertySettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BindPropertySettingDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "MainFrm.h"
// CBindPropertySettingDlg 对话框

IMPLEMENT_DYNAMIC(CBindPropertySettingDlg, CDialog)

CBindPropertySettingDlg::CBindPropertySettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBindPropertySettingDlg::IDD, pParent)
	, m_strBoolPointName(_T(""))
	, m_strResultPointName(_T(""))
	, m_strResultPointName2(_T(""))
	, m_strResultPointName3(_T(""))
	, m_strTreatmentPointName(_T(""))
{

}

CBindPropertySettingDlg::~CBindPropertySettingDlg()
{
}

void CBindPropertySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BIND_BOOL_POINT, m_strBoolPointName);
	DDX_Text(pDX, IDC_EDIT_BIND_RESULT_POINT, m_strResultPointName);
	DDX_Text(pDX, IDC_EDIT_BIND_RESULT_POINT2, m_strResultPointName2);
	DDX_Text(pDX, IDC_EDIT_BIND_RESULT_POINT3, m_strResultPointName3);
	DDX_Text(pDX, IDC_EDIT_BIND_TREATMENT_POINT, m_strTreatmentPointName);
}


BEGIN_MESSAGE_MAP(CBindPropertySettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESULT_SELECT, &CBindPropertySettingDlg::OnBnClickedButtonResultSelect)
	ON_BN_CLICKED(IDC_BUTTON_TREATMENT_SELECT, &CBindPropertySettingDlg::OnBnClickedButtonTreatmentSelect)
	ON_BN_CLICKED(IDOK, &CBindPropertySettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RESULT_SELECT2, &CBindPropertySettingDlg::OnBnClickedButtonResultSelect2)
	ON_BN_CLICKED(IDC_BUTTON_RESULT_SELECT3, &CBindPropertySettingDlg::OnBnClickedButtonResultSelect3)
	ON_BN_CLICKED(IDC_BUTTON_BOOL_POINT_SELECT, &CBindPropertySettingDlg::OnBnClickedButtonBoolPointSelect)
END_MESSAGE_MAP()


// CBindPropertySettingDlg 消息处理程序


void CBindPropertySettingDlg::OnBnClickedButtonResultSelect()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strResultPointName = dlg.m_strPointName;
		DataPointEntry entry = pFrame->m_plcpointmap.GetEntry(m_strResultPointName.GetString());
		const POINT_STORE_CYCLE cycle = entry.GetStoreCycle();
		if (cycle == E_STORE_NULL)
		{
			AfxMessageBox(L"该点的存储周期为空，请修正后再试！");
			return;
		}
		else
		{
			UpdateData(0);
		}
	}	
}


void CBindPropertySettingDlg::OnBnClickedButtonTreatmentSelect()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strTreatmentPointName = dlg.m_strPointName;
		DataPointEntry entry = pFrame->m_plcpointmap.GetEntry(m_strTreatmentPointName.GetString());
		const POINT_STORE_CYCLE cycle = entry.GetStoreCycle();
		if (cycle == E_STORE_NULL)
		{
			AfxMessageBox(L"该点的存储周期为空，请修正后再试！");
			return;
		}
		else
		{
			UpdateData(0);
		}
	}
}


void CBindPropertySettingDlg::OnBnClickedOk()
{
	
	CDialog::OnOK();
}


BOOL CBindPropertySettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBindPropertySettingDlg::OnBnClickedButtonResultSelect2()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strResultPointName2 = dlg.m_strPointName;
		DataPointEntry entry = pFrame->m_plcpointmap.GetEntry(m_strResultPointName2.GetString());
		const POINT_STORE_CYCLE cycle = entry.GetStoreCycle();
		if (cycle == E_STORE_NULL)
		{
			AfxMessageBox(L"该点的存储周期为空，请修正后再试！");
			return;
		}
		else
		{
			UpdateData(0);
		}
	}	
}

void CBindPropertySettingDlg::OnBnClickedButtonResultSelect3()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strResultPointName3 = dlg.m_strPointName;
		DataPointEntry entry = pFrame->m_plcpointmap.GetEntry(m_strResultPointName3.GetString());
		const POINT_STORE_CYCLE cycle = entry.GetStoreCycle();
		if (cycle == E_STORE_NULL)
		{
			AfxMessageBox(L"该点的存储周期为空，请修正后再试！");
			return;
		}
		else
		{
			UpdateData(0);
		}
	}	
}


void CBindPropertySettingDlg::OnBnClickedButtonBoolPointSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strBoolPointName = dlg.m_strPointName;
		DataPointEntry entry = pFrame->m_plcpointmap.GetEntry(m_strBoolPointName.GetString());
		const POINT_STORE_CYCLE cycle = entry.GetStoreCycle();
		if (cycle == E_STORE_NULL)
		{
			AfxMessageBox(L"该点的存储周期为空，请修正后再试！");
			return;
		}
		else
		{
			UpdateData(0);
		}
	}
}
