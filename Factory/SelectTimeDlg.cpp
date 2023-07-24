// SelectTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelectTimeDlg.h"
#include "afxdialogex.h"


// CSelectTimeDlg dialog

IMPLEMENT_DYNAMIC(CSelectTimeDlg, CDialogEx)

CSelectTimeDlg::CSelectTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectTimeDlg::IDD, pParent)
{

}

CSelectTimeDlg::~CSelectTimeDlg()
{
}

void CSelectTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATEPICKER, m_Date);
	DDX_Control(pDX, IDC_TIMEPICKER, m_Time);
}


BEGIN_MESSAGE_MAP(CSelectTimeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectTimeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectTimeDlg message handlers


void CSelectTimeDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	COleDateTime oleDate;
	COleDateTime oleTime;
	m_Date.GetTime(oleDate);
	m_Time.GetTime(oleTime);

	m_tSelect.SetDateTime(oleDate.GetYear(),oleDate.GetMonth(),oleDate.GetDay(),oleTime.GetHour(),oleTime.GetMinute(),oleTime.GetSecond());



	CDialogEx::OnOK();
}


BOOL CSelectTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_Date.SetTime(COleDateTime::GetCurrentTime()) ;
	m_Time.SetTime( COleDateTime::GetCurrentTime());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
