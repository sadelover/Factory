// CoreVersionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoreVersionDlg.h"
#include "afxdialogex.h"


// CCoreVersionDlg dialog

IMPLEMENT_DYNAMIC(CCoreVersionDlg, CDialogEx)

CCoreVersionDlg::CCoreVersionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCoreVersionDlg::IDD, pParent)
	, m_strCoreVersion(_T(""))
	, m_strPysiteVersion(_T(""))
	, m_strLogicVersion(_T(""))
{

}

CCoreVersionDlg::~CCoreVersionDlg()
{
}

void CCoreVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VERSION_CORE, m_strCoreVersion);
	DDX_Text(pDX, IDC_STATIC_VERSION_PYSITE, m_strPysiteVersion);
	DDX_Text(pDX, IDC_STATIC_VERSION_LOGIC, m_strLogicVersion);
}


BEGIN_MESSAGE_MAP(CCoreVersionDlg, CDialogEx)
END_MESSAGE_MAP()


// CCoreVersionDlg message handlers
