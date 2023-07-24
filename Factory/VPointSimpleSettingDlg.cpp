// VPointSimpleSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VPointSimpleSettingDlg.h"
#include "afxdialogex.h"


const int g_nAttriLen = 2;
const CString g_strReadWriteAttri[g_nAttriLen] = {_T("R"), _T("W")};


// CVPointSimpleSettingDlg dialog

IMPLEMENT_DYNAMIC(CVPointSimpleSettingDlg, CDialogEx)

CVPointSimpleSettingDlg::CVPointSimpleSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVPointSimpleSettingDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strRw(_T(""))
	, m_strDescription(_T(""))
{

}

CVPointSimpleSettingDlg::~CVPointSimpleSettingDlg()
{
}

void CVPointSimpleSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Control(pDX, IDC_COMBO_RW, m_comboRw);
	DDX_CBString(pDX, IDC_COMBO_RW, m_strRw);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
}


BEGIN_MESSAGE_MAP(CVPointSimpleSettingDlg, CDialogEx)
END_MESSAGE_MAP()


// CVPointSimpleSettingDlg message handlers


BOOL CVPointSimpleSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CVPointSimpleSettingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	m_strName.TrimLeft(_T(" "));
	if (_T("") == m_strName)
	{
		MessageBox(_T("点名不能为空！"), _T("错误"), MB_OK|MB_ICONERROR);
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_NAME));
		if (pEdit != NULL)
		{
			pEdit->SetSel(0, -1);
			pEdit->SetFocus();
		}
		return;
	}

	CDialogEx::OnOK();
}

void CVPointSimpleSettingDlg::InitControls(void)
{
	for (int i=0; i<g_nAttriLen; ++i)
	{
		m_comboRw.AddString(g_strReadWriteAttri[i]);
	}
	m_comboRw.SetCurSel(0);
}

CString	CVPointSimpleSettingDlg::GetVPointName(void) const
{
	return m_strName;
}

CString	CVPointSimpleSettingDlg::GetVPointRwAttri(void) const
{
	return m_strRw;
}
