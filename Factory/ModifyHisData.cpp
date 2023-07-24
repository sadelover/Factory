// ModifyHisData.cpp : implementation file
//

#include "stdafx.h"
#include "ModifyHisData.h"
#include "afxdialogex.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"


// CModifyHisData dialog

IMPLEMENT_DYNAMIC(CModifyHisData, CDialogEx)

CModifyHisData::CModifyHisData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyHisData::IDD, pParent)
	, m_strValue(_T(""))
{

}

CModifyHisData::~CModifyHisData()
{
}

void CModifyHisData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEW_VALUE, m_strValue);
}


BEGIN_MESSAGE_MAP(CModifyHisData, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModifyHisData::OnBnClickedOk)
END_MESSAGE_MAP()


// CModifyHisData message handlers


void CModifyHisData::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
