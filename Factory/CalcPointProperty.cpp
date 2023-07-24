// CalcPointProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "CalcPointProperty.h"
#include "afxdialogex.h"


// CCalcPointProperty 对话框

IMPLEMENT_DYNAMIC(CCalcPointProperty, CSkinDlg)

CCalcPointProperty::CCalcPointProperty(const BOOL bIsAdd, CWnd* pParent /*=NULL*/)
	: CSkinDlg(CCalcPointProperty::IDD, pParent)
	, m_strName(_T(""))
	, m_bIsAdd(bIsAdd)
	, m_nPeriod(1)
{

}

CCalcPointProperty::~CCalcPointProperty()
{
}

void CCalcPointProperty::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CALC_NAME, m_strName);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_PERIOD, m_comboPeriod);
}


BEGIN_MESSAGE_MAP(CCalcPointProperty, CSkinDlg)
END_MESSAGE_MAP()


// CCalcPointProperty 消息处理程序


BOOL CCalcPointProperty::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_bIsAdd)
	{
		SetWindowText(_T("新建计算线程"));
	}
	else
	{
		SetWindowText(_T("编辑计算线程"));
		CEdit* pEditName = (CEdit*)GetDlgItem(IDC_EDIT_CALC_NAME);
		if (pEditName != NULL)
		{
			pEditName->EnableWindow(FALSE);
		}
	}

	m_comboPeriod.AddString(_T("1"));
	m_comboPeriod.AddString(_T("5"));
	m_comboPeriod.AddString(_T("30"));
	m_comboPeriod.AddString(_T("60"));
	switch (m_nPeriod)
	{
	case 1:
		m_comboPeriod.SetCurSel(0);
		break;
	case 5:
		m_comboPeriod.SetCurSel(1);
		break;
	case 30:
		m_comboPeriod.SetCurSel(2);
		break;
	case 60:
		m_comboPeriod.SetCurSel(3);
		break;
	default:
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCalcPointProperty::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	int nPos = m_strName.Find(_T("("));
	if (nPos != -1)
	{
		CEdit* pName = (CEdit*)GetDlgItem(IDC_EDIT_CALC_NAME);
		if (pName != NULL)
		{
			pName->SetSel(0, -1);
			pName->SetFocus();
		}
		MessageBox(_T("名称不可以包含字符("));
		return;
	}

	CString strPeriod;
	int nSel = m_comboPeriod.GetCurSel();
	m_comboPeriod.GetLBText(nSel, strPeriod);
	m_nPeriod = _ttoi(strPeriod);

	CSkinDlg::OnOK();
}
