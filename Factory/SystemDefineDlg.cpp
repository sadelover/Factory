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
	GetDlgItem(IDC_STATIC)->GetWindowRect(rect);//��ȡ�ؼ�����Ļ����
	ScreenToClient(&rect);//ת��Ϊ�Ի����ϵĿͻ�����
	m_ChildDlg_List.Create(IDD_DIALOG_SYSTEM_DEFINE_List, this);
	m_ChildDlg_List.MoveWindow(rect);
	m_ChildDlg_Script.Create(IDD_DIALOG_SYSTEM_DEFINE_Script, this);
	m_ChildDlg_Script.MoveWindow(rect);

	int mode = m_ChildDlg_List.SetData(m_strSystemDefine);
	if(mode == 0) 
	{
		m_ChildDlg_List.ShowWindow(SW_SHOW);
		m_ChildDlg_Script.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"�ű�");
	}
	else if(mode == 1)
	{
		m_ChildDlg_Script.SetData(m_strSystemDefine);
		m_ChildDlg_List.ShowWindow(SW_HIDE);
		m_ChildDlg_Script.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"����");
	}
	else
	{
		m_ChildDlg_Script.SetData(m_strSystemDefine);
		m_ChildDlg_List.ShowWindow(SW_HIDE);
		m_ChildDlg_Script.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"����");
		AfxMessageBox(L"json��ʽ����ȷ��");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ChildDlg_List.IsWindowVisible())
	{
		//��json::value��ȡ��CstringȻ���m_strSysDefine
		//�������������ʾ��һ��
		m_ChildDlg_List.GetData(m_strSystemDefine);
		m_ChildDlg_Script.SetData(m_strSystemDefine);
		m_ChildDlg_List.ShowWindow(SW_HIDE);
		m_ChildDlg_Script.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"����");
	}
	else if(m_ChildDlg_Script.IsWindowVisible())
	{
		//��Ctring��ȡ����m_strSysdefine���ٽ���jsonȻ���m_ChildDlg_List�Ĵ������ݳ�ʼ������
		//�������������ʾ��һ��
		if(m_ChildDlg_Script.GetData(m_strSystemDefine) == true)
		{
			m_ChildDlg_List.SetData(m_strSystemDefine);
			m_ChildDlg_Script.ShowWindow(SW_HIDE);
			m_ChildDlg_List.ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_Script)->SetWindowText(L"�ű�");
		}
	}
}
