// SystemDefineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SystemDefineDlg.h"
#include "afxdialogex.h"

//#include "json/json.h"
#include "./ControlEx/WarningDlg.h"
#include "./Tools/CustomTools/CustomTools.h"


// CSystemDefineDlg dialog

IMPLEMENT_DYNAMIC(CSystemDefineDlg, CDialogEx)

CSystemDefineDlg::CSystemDefineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemDefineDlg::IDD, pParent)
	, m_strSystemDefine(_T(""))
{

}

CSystemDefineDlg::~CSystemDefineDlg()
{
}

void CSystemDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSystemDefineDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSystemDefineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_Script, &CSystemDefineDlg::OnBnClickedButtonScript)
END_MESSAGE_MAP()


// CSystemDefineDlg message handlers


void CSystemDefineDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_ChildDlg_List.IsWindowVisible())
	{
		m_ChildDlg_List.GetData(m_strSystemDefine);
	}
	else if(m_ChildDlg_Script.IsWindowVisible())
	{
		if(m_ChildDlg_Script.GetData(m_strSystemDefine) == false)
			return;
	}
	CDialogEx::OnOK();
}


BOOL CSystemDefineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	
	CRect rect;
	GetDlgItem(IDC_STATIC)->GetWindowRect(rect);//获取控件的屏幕坐标
	ScreenToClient(&rect);//转换为对话框上的客户坐标
	m_ChildDlg_List.Create(IDD_DIALOG_SYSTEM_DEFINE_List, this);
	m_ChildDlg_List.MoveWindow(rect);
	m_ChildDlg_Script.Create(IDD_DIALOG_SYSTEM_DEFINE_Script, this);
	m_ChildDlg_Script.MoveWindow(rect);

	int mode = m_ChildDlg_List.SetData(m_strSystemDefine);
	if(mode == 0) 
	{
		m_ChildDlg_List.ShowWindow(SW_SHOW);
		m_ChildDlg_Script.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"脚本");
	}
	else if(mode == 1)
	{
		m_ChildDlg_Script.SetData(m_strSystemDefine);
		m_ChildDlg_List.ShowWindow(SW_HIDE);
		m_ChildDlg_Script.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"界面");
	}
	else
	{
		m_ChildDlg_Script.SetData(m_strSystemDefine);
		m_ChildDlg_List.ShowWindow(SW_HIDE);
		m_ChildDlg_Script.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"界面");
		AfxMessageBox(L"json格式不正确。");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSystemDefineDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x80;
		switch (pMsg->wParam)
		{
		case 'a':
		case 'A':
			if (bCtrl)
			{
				CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CONFIG);
				if (pEdit != NULL)
				{
					pEdit->SetFocus();
					pEdit->SetSel(0, -1);
				}
			}  
			break;
		default:
			break;
		}
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSystemDefineDlg::OnBnClickedButtonScript()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ChildDlg_List.IsWindowVisible())
	{
		//从json::value中取出Cstring然后给m_strSysDefine
		//隐藏这个窗口显示另一个
		m_ChildDlg_List.GetData(m_strSystemDefine);
		m_ChildDlg_Script.SetData(m_strSystemDefine);
		m_ChildDlg_List.ShowWindow(SW_HIDE);
		m_ChildDlg_Script.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"界面");
	}
	else if(m_ChildDlg_Script.IsWindowVisible())
	{
		//从Ctring中取出给m_strSysdefine，再解析json然后给m_ChildDlg_List的窗口数据初始化函数
		//隐藏这个窗口显示另一个
		if(m_ChildDlg_Script.GetData(m_strSystemDefine) == true)
		{
			m_ChildDlg_List.SetData(m_strSystemDefine);
			m_ChildDlg_Script.ShowWindow(SW_HIDE);
			m_ChildDlg_List.ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"脚本");
		}
	}
}
