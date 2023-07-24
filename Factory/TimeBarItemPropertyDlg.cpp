// TimeBarItemPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TimeBarItemPropertyDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"

// CTimeBarItemPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CTimeBarItemPropertyDlg, CDialog)

CTimeBarItemPropertyDlg::CTimeBarItemPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeBarItemPropertyDlg::IDD, pParent)
{

}

CTimeBarItemPropertyDlg::~CTimeBarItemPropertyDlg()
{
}

void CTimeBarItemPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTimeBarItemPropertyDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTimeBarItemPropertyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTimeBarItemPropertyDlg 消息处理程序


BOOL CTimeBarItemPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT_TIME_BAR_ITEM_INDEX)->SetWindowText(m_strIndex);
	GetDlgItem(IDC_EDIT_TIME_BAR_ITEM_NAME)->SetWindowText(m_strName);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTimeBarItemPropertyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_TIME_BAR_ITEM_INDEX)->GetWindowText(m_strIndex);
	GetDlgItem(IDC_EDIT_TIME_BAR_ITEM_NAME)->GetWindowText(m_strName);
	CDialog::OnOK();
}
