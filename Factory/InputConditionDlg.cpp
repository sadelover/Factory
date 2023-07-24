// InputConditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputConditionDlg.h"
#include "afxdialogex.h"


// CInputConditionDlg dialog

IMPLEMENT_DYNAMIC(CInputConditionDlg, CDialogEx)

CInputConditionDlg::CInputConditionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputConditionDlg::IDD, pParent)
	, m_strCondition(_T(""))
{

}

CInputConditionDlg::~CInputConditionDlg()
{
}

void CInputConditionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONDITION, m_strCondition);
}


BEGIN_MESSAGE_MAP(CInputConditionDlg, CDialogEx)
END_MESSAGE_MAP()


// CInputConditionDlg message handlers
