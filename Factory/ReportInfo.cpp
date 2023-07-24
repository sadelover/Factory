// ReportInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ReportInfo.h"
#include "afxdialogex.h"


// CReportInfo dialog

IMPLEMENT_DYNAMIC(CReportInfo, CDialogEx)

CReportInfo::CReportInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReportInfo::IDD, pParent)
	, m_strReportName(_T(""))
	, m_nFileType(0)
	, m_nTimeType(0)
	, m_strTimeConfig(_T(""))
{

}

CReportInfo::~CReportInfo()
{
}

void CReportInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REPORT_NAME, m_strReportName);
	DDX_Text(pDX, IDC_EDIT_REPORT_DESCRIPTION, m_strReportDescription);

	DDX_CBIndex(pDX, IDC_COMBO_FILE_TYPE, m_nFileType);
	DDX_CBIndex(pDX, IDC_COMBO_TIME_TYPE, m_nTimeType);
	DDX_Text(pDX, IDC_EDIT_TIME_CONFIG, m_strTimeConfig);
}


BEGIN_MESSAGE_MAP(CReportInfo, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_FILE_TYPE, &CReportInfo::OnCbnSelchangeComboFileType)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_TYPE, &CReportInfo::OnCbnSelchangeComboTimeType)
	ON_BN_CLICKED(IDOK, &CReportInfo::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_TIME_CONFIG, &CReportInfo::OnEnChangeEditTimeConfig)
	ON_EN_CHANGE(IDC_EDIT_REPORT_DESCRIPTION, &CReportInfo::OnEnChangeEditReportDescription)
END_MESSAGE_MAP()


// CReportInfo message handlers


void CReportInfo::OnCbnSelchangeComboFileType()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); //把界面状态同步进内存
	GetDlgItem(IDC_STATIC_TIME_TYPE)->ShowWindow(m_nFileType==2);
	GetDlgItem(IDC_COMBO_TIME_TYPE)->ShowWindow(m_nFileType==2);
	GetDlgItem(IDC_STATIC_TIME_CONFIG)->ShowWindow(m_nFileType==2);
	GetDlgItem(IDC_EDIT_TIME_CONFIG)->ShowWindow(m_nFileType==2);

}


void CReportInfo::OnCbnSelchangeComboTimeType()
{
	// TODO: Add your control notification handler code here
}


void CReportInfo::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CReportInfo::OnEnChangeEditTimeConfig()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CReportInfo::OnEnChangeEditReportDescription()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
