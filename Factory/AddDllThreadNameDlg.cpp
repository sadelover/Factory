// AddDllThreadNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AddDllThreadNameDlg.h"
#include "afxdialogex.h"
#include "../Tools/CustomTools/CustomTools.h"


// CAddDllThreadNameDlg dialog

IMPLEMENT_DYNAMIC(CAddDllThreadNameDlg, CSkinDlg)

CAddDllThreadNameDlg::CAddDllThreadNameDlg(bool bIsAddNew, CWnd* pParent /*=NULL*/)
	: CSkinDlg(CAddDllThreadNameDlg::IDD, pParent)
	, m_strDllName(_T(""))
	, m_nWidth(1920)
	, m_nHeight(955)
	, m_page_type(FULLSCREEN)
	, m_bIsAddNew(bIsAddNew)
{

}

CAddDllThreadNameDlg::~CAddDllThreadNameDlg()
{
}

void CAddDllThreadNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strDllName);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CAddDllThreadNameDlg, CSkinDlg)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CAddDllThreadNameDlg::OnEnChangeEditName)
	ON_BN_CLICKED(IDOK, &CAddDllThreadNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CAddDllThreadNameDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();
	if (m_bIsAddNew)
	{
		SetWindowText(_T("新建线程"));
	}
	else
	{
		SetWindowText(_T("线程名称修改"));
	}

	return TRUE;
}


// CAddDllThreadNameDlg message handlers


void CAddDllThreadNameDlg::OnEnChangeEditName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CSkinDlg::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CAddDllThreadNameDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(!Project::Tools::IsStrCharacterOrNumberOrUnderline(m_strDllName))
	{
		AfxMessageBox(_T("线程名称只能是字母或数字及下划线！"));
		return;
	}
	

	CDialog::OnOK();
}
