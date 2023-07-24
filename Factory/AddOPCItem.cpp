// AddOPCItem.cpp : implementation file
//

#include "stdafx.h"
#include "AddOPCItem.h"
#include "afxdialogex.h"


// CAddOPCItem dialog

IMPLEMENT_DYNAMIC(CAddOPCItem, CDialog)

CAddOPCItem::CAddOPCItem(CWnd* pParent /*=NULL*/)
	: CDialog(CAddOPCItem::IDD, pParent)
	, m_strOPCItemName(_T(""))
{

}

CAddOPCItem::~CAddOPCItem()
{
}

void CAddOPCItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPCITEMNAME, m_strOPCItemName);
}


BEGIN_MESSAGE_MAP(CAddOPCItem, CDialog)
END_MESSAGE_MAP()

BOOL CAddOPCItem::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


// CAddOPCItem message handlers
