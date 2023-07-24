// InputRedundencyIPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InputRedundencyIPDlg.h"
#include "afxdialogex.h"



// CInputRedundencyIPDlg 对话框

IMPLEMENT_DYNAMIC(CInputRedundencyIPDlg, CDialogEx)

CInputRedundencyIPDlg::CInputRedundencyIPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputRedundencyIPDlg::IDD, pParent)
	, m_strIP(_T(""))
{

}

CInputRedundencyIPDlg::~CInputRedundencyIPDlg()
{
}

void CInputRedundencyIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REDUNDENCY_IP, m_strIP);
}


BEGIN_MESSAGE_MAP(CInputRedundencyIPDlg, CDialogEx)
END_MESSAGE_MAP()


// CInputRedundencyIPDlg 消息处理程序
