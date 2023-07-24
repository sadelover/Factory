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
	m_comboLayer.AddString(_T("1��"));
	m_comboLayer.AddString(_T("2��"));
	m_comboLayer.AddString(_T("3��"));
	m_comboLayer.AddString(_T("4��"));
	m_comboLayer.AddString(_T("5��"));
	m_comboLayer.AddString(_T("6��"));
	m_comboLayer.AddString(_T("7��"));
	m_comboLayer.AddString(_T("8��"));
	m_comboLayer.AddString(_T("9��"));
	m_comboLayer.AddString(_T("10��"));
	m_comboLayer.SetCurSel(m_nLayer);
}
