// GroupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GroupDlg.h"
#include "afxdialogex.h"


// CGroupDlg 对话框

IMPLEMENT_DYNAMIC(CGroupDlg, CSkinDlg)

CGroupDlg::CGroupDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CGroupDlg::IDD, pParent)
	, m_strGroupName(_T(""))
{

}

CGroupDlg::~CGroupDlg()
{
}

void CGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GROUP, m_strGroupName);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CGroupDlg, CSkinDlg)
	ON_BN_CLICKED(IDOK, &CGroupDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGroupDlg 消息处理程序


void CGroupDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_strGroupName.GetLength() == 0)
	{
		MessageBox(L"组名称不能为空!");
		return;
	}
	UpdateData(FALSE);
	CSkinDlg::OnOK();
}
