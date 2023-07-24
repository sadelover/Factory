// ModifyReport.cpp : implementation file
//

#include "stdafx.h"
#include "ModifyReport.h"
#include "afxdialogex.h"


// CModifyReport dialog

IMPLEMENT_DYNAMIC(CModifyReport, CDialogEx)

CModifyReport::CModifyReport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyReport::IDD, pParent)
	, m_strNewReportName(_T(""))
{

}

CModifyReport::~CModifyReport()
{
}

void CModifyReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REPORT_NAME, m_strNewReportName);
}


BEGIN_MESSAGE_MAP(CModifyReport, CDialogEx)
END_MESSAGE_MAP()


// CModifyReport message handlers
