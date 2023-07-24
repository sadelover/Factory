// PointIgnoreDefDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PointIgnoreDefDlg.h"
#include "afxdialogex.h"


// CPointIgnoreDefDlg 对话框

IMPLEMENT_DYNAMIC(CPointIgnoreDefDlg, CDialogEx)

CPointIgnoreDefDlg::CPointIgnoreDefDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPointIgnoreDefDlg::IDD, pParent)
	, m_point_ignore_list(_T(""))
{

}

CPointIgnoreDefDlg::~CPointIgnoreDefDlg()
{
}

void CPointIgnoreDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POINT, m_point_ignore_list);
}


BEGIN_MESSAGE_MAP(CPointIgnoreDefDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPointIgnoreDefDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_POINT, &CPointIgnoreDefDlg::OnEnChangeEditPoint)
END_MESSAGE_MAP()


// CPointIgnoreDefDlg 消息处理程序


void CPointIgnoreDefDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	/*
		获取编辑框中的数据
	*/

	/*
		发送至数据库
		unit01表
		unitproperty01=  'point_ignore_list'
		unitproperty02 = 以逗号间隔的屏蔽点清单
	*/


}


void CPointIgnoreDefDlg::OnEnChangeEditPoint()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
