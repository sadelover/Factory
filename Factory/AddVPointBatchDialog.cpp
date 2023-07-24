// AddVPointBatchDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "AddVPointBatchDialog.h"
#include "afxdialogex.h"


// CAddVPointBatchDialog 对话框

IMPLEMENT_DYNAMIC(CAddVPointBatchDialog, CDialogEx)

CAddVPointBatchDialog::CAddVPointBatchDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddVPointBatchDialog::IDD, pParent)
	, m_strVpointBatchlist(_T(""))
{

}

CAddVPointBatchDialog::~CAddVPointBatchDialog()
{
}

void CAddVPointBatchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_POINT, m_strVpointBatchlist);
}


BEGIN_MESSAGE_MAP(CAddVPointBatchDialog, CDialogEx)
END_MESSAGE_MAP()


// CAddVPointBatchDialog 消息处理程序
