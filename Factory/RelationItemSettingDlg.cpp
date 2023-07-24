// RelationItemSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RelationItemSettingDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "MainFrm.h"
// CRelationItemSettingDlg 对话框

IMPLEMENT_DYNAMIC(CRelationItemSettingDlg, CDialog)

CRelationItemSettingDlg::CRelationItemSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRelationItemSettingDlg::IDD, pParent)
	, m_strPointName(_T(""))
	, m_fResult(0.0)
{
	m_nRelation = 0;
}

CRelationItemSettingDlg::~CRelationItemSettingDlg()
{
}

void CRelationItemSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RELATION_BIND_POINT, m_strPointName);
	DDX_Control(pDX, IDC_COMBO_RELATION_SELECT, m_combo_relation_item);
	//DDX_Text(pDX, IDC_EDIT_RELATION_MATCH_VALUE, m_nResult);
	//DDV_MinMaxInt(pDX, m_nResult, 0, 50);
}


BEGIN_MESSAGE_MAP(CRelationItemSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RELATION_BIND_POINT_SELECT, &CRelationItemSettingDlg::OnBnClickedButtonRelationBindPointSelect)
	ON_BN_CLICKED(IDOK, &CRelationItemSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRelationItemSettingDlg 消息处理程序


BOOL CRelationItemSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_combo_relation_item.AddString(L"等于");
	m_combo_relation_item.AddString(L"大于");
	m_combo_relation_item.AddString(L"小于");
	m_combo_relation_item.AddString(L"大于等于");
	m_combo_relation_item.AddString(L"小于等于");
	m_combo_relation_item.AddString(L"不等于");
	m_combo_relation_item.SetCurSel(m_nRelation);

	CString strResult;
	strResult.Format(L"%.2f",m_fResult);
	GetDlgItem(IDC_EDIT_RELATION_MATCH_VALUE)->SetWindowText(strResult);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRelationItemSettingDlg::OnBnClickedButtonRelationBindPointSelect()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strPointName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CRelationItemSettingDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_nRelation = m_combo_relation_item.GetCurSel();

	CString strTemp;
	GetDlgItem(IDC_EDIT_RELATION_MATCH_VALUE)->GetWindowTextW(strTemp);
	if (strTemp.GetLength() == 0)
	{
		AfxMessageBox(L"请设置设定值!");
		return;
	}
	m_fResult = _ttof(strTemp.GetString());

	CDialog::OnOK();
}
