// PointInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PointInfoDlg.h"
#include "afxdialogex.h"


// CPointInfoDlg dialog

IMPLEMENT_DYNAMIC(CPointInfoDlg, CDialog)

CPointInfoDlg::CPointInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointInfoDlg::IDD, pParent)
	, m_strShortName(_T(""))
	, m_strRemark(_T(""))
	, m_strUnit(_T(""))
	, m_strSource(_T(""))
	, m_strParam1(_T(""))
	, m_nNum(0)
	, m_nAvg(0)
	, m_nMax(0)
	, m_nMin(0)
{

}

CPointInfoDlg::~CPointInfoDlg()
{
}

void CPointInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_SHORT_NAME, m_strShortName);
	DDX_Text(pDX, IDC_STATIC_REMARK, m_strRemark);
	DDX_Text(pDX, IDC_STATIC_UNIT, m_strUnit);
	DDX_Text(pDX, IDC_STATIC_SOURCE, m_strSource);
	DDX_Text(pDX, IDC_STATIC_PARAM, m_strParam1);
	DDX_Text(pDX, IDC_STATIC_NUM, m_nNum);
	DDX_Text(pDX, IDC_STATIC_AVG, m_nAvg);
	DDX_Text(pDX, IDC_STATIC_MAX, m_nMax);
	DDX_Text(pDX, IDC_STATIC_MIN, m_nMin);
}


BEGIN_MESSAGE_MAP(CPointInfoDlg, CDialog)
END_MESSAGE_MAP()

BOOL CPointInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


// CPointInfoDlg message handlers
