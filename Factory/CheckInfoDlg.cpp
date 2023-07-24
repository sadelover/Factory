// CheckInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CheckInfoDlg.h"
#include "afxdialogex.h"


// CCheckInfoDlg 对话框

IMPLEMENT_DYNAMIC(CCheckInfoDlg, CDialogEx)

CCheckInfoDlg::CCheckInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckInfoDlg::IDD, pParent)
	, m_strShow(_T(""))
{

}

CCheckInfoDlg::~CCheckInfoDlg()
{
}

void CCheckInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHOW, m_strShow);
}


BEGIN_MESSAGE_MAP(CCheckInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CCheckInfoDlg 消息处理程序
