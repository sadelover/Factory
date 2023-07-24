// ReplacePointName.cpp : implementation file
//

#include "stdafx.h"
#include "ReplacePointName.h"
#include "afxdialogex.h"


// CReplacePointName dialog

IMPLEMENT_DYNAMIC(CReplacePointName, CDialog)

CReplacePointName::CReplacePointName(CWnd* pParent /*=NULL*/)
	: CDialog(CReplacePointName::IDD, pParent)
	, m_strSourcePointName(_T(""))
	, m_strReplacePointName(_T(""))
{

}

CReplacePointName::~CReplacePointName()
{
}

void CReplacePointName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REPLACE_FROM, m_strSourcePointName);
	DDX_Text(pDX, IDC_EDIT_REPLACE_TO, m_strReplacePointName);
}


BEGIN_MESSAGE_MAP(CReplacePointName, CDialog)
	ON_BN_CLICKED(IDOK, &CReplacePointName::OnBnClickedOk)
END_MESSAGE_MAP()


// CReplacePointName message handlers


void CReplacePointName::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
