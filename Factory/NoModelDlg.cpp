// NoModelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NoModelDlg.h"
#include "afxdialogex.h"


// CNoModelDlg 对话框

IMPLEMENT_DYNAMIC(CNoModelDlg, CDialogEx)

CNoModelDlg::CNoModelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNoModelDlg::IDD, pParent)
	, m_strShow(_T("正在更新中..."))
{

}

CNoModelDlg::~CNoModelDlg()
{
}

void CNoModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TXT_SHOW, m_strShow);
}


BEGIN_MESSAGE_MAP(CNoModelDlg, CDialogEx)
END_MESSAGE_MAP()


// CNoModelDlg 消息处理程序


BOOL CNoModelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	SetWindowText(_T("自动更新"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
