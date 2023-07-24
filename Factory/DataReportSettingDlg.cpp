// DataReportSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataReportSettingDlg.h"
#include "afxdialogex.h"


// CDataReportSettingDlg dialog

IMPLEMENT_DYNAMIC(CDataReportSettingDlg, CDialogEx)

CDataReportSettingDlg::CDataReportSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataReportSettingDlg::IDD, pParent)
	, m_nLayer(4)
	, m_strPath(_T(""))
{

}

CDataReportSettingDlg::~CDataReportSettingDlg()
{
}

void CDataReportSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
}


BEGIN_MESSAGE_MAP(CDataReportSettingDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataReportSettingDlg message handlers


BOOL CDataReportSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDataReportSettingDlg::InitControls(void)
{
	// combobox
	m_comboLayer.AddString(_T("1²ã"));
	m_comboLayer.AddString(_T("2²ã"));
	m_comboLayer.AddString(_T("3²ã"));
	m_comboLayer.AddString(_T("4²ã"));
	m_comboLayer.AddString(_T("5²ã"));
	m_comboLayer.AddString(_T("6²ã"));
	m_comboLayer.AddString(_T("7²ã"));
	m_comboLayer.AddString(_T("8²ã"));
	m_comboLayer.AddString(_T("9²ã"));
	m_comboLayer.AddString(_T("10²ã"));
	m_comboLayer.SetCurSel(m_nLayer);
}
