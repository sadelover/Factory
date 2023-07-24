// PictureTypeSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PictureTypeSelectDlg.h"
#include "afxdialogex.h"


// CPictureTypeSelectDlg dialog

IMPLEMENT_DYNAMIC(CPictureTypeSelectDlg, CDialog)

CPictureTypeSelectDlg::CPictureTypeSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPictureTypeSelectDlg::IDD, pParent)
	, m_nSelectType(0)
{

}

CPictureTypeSelectDlg::~CPictureTypeSelectDlg()
{
}

void CPictureTypeSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TYPE, m_listPictureType);
	DDX_LBIndex(pDX, IDC_LIST_TYPE, m_nSelectType);
}


BEGIN_MESSAGE_MAP(CPictureTypeSelectDlg, CDialog)
END_MESSAGE_MAP()

BOOL CPictureTypeSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_listPictureType.AddString(_T("冷机"));
	m_listPictureType.AddString(_T("水泵"));
	m_listPictureType.AddString(_T("冷却塔"));
	m_listPictureType.AddString(_T("空调箱"));
	m_listPictureType.AddString(_T("阀门"));
	m_listPictureType.AddString(_T("管道"));
	m_listPictureType.AddString(_T("按钮"));
	m_listPictureType.AddString(_T("图表"));
	m_listPictureType.AddString(_T("布局"));
	m_listPictureType.AddString(_T("图标"));
	m_listPictureType.AddString(_T("其他"));

	m_listPictureType.SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;
}


// CPictureTypeSelectDlg message handlers
