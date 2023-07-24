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
	
	m_listPictureType.AddString(_T("���"));
	m_listPictureType.AddString(_T("ˮ��"));
	m_listPictureType.AddString(_T("��ȴ��"));
	m_listPictureType.AddString(_T("�յ���"));
	m_listPictureType.AddString(_T("����"));
	m_listPictureType.AddString(_T("�ܵ�"));
	m_listPictureType.AddString(_T("��ť"));
	m_listPictureType.AddString(_T("ͼ��"));
	m_listPictureType.AddString(_T("����"));
	m_listPictureType.AddString(_T("ͼ��"));
	m_listPictureType.AddString(_T("����"));

	m_listPictureType.SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;
}


// CPictureTypeSelectDlg message handlers
