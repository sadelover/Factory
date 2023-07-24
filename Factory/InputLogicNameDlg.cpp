// InputLogicNameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "InputLogicNameDlg.h"
#include "afxdialogex.h"
#include "Tools/Tools_String.h"

// CInputLogicNameDlg �Ի���

IMPLEMENT_DYNAMIC(CInputLogicNameDlg, CSkinDlg)

CInputLogicNameDlg::CInputLogicNameDlg(bool bIsCreate, CWnd* pParent /*=NULL*/)
	: CSkinDlg(CInputLogicNameDlg::IDD, pParent)
	, m_strLogicName(_T(""))
	, m_bIsCreate(bIsCreate)
{

}

CInputLogicNameDlg::~CInputLogicNameDlg()
{
}

void CInputLogicNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOGIC_NAME, m_strLogicName);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CInputLogicNameDlg, CSkinDlg)
	ON_BN_CLICKED(IDOK, &CInputLogicNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputLogicNameDlg ��Ϣ�������


BOOL CInputLogicNameDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	if (m_bIsCreate)
	{
		SetWindowText(_T("�½�����"));
	}
	else
	{
		SetWindowText(_T("�޸Ĳ�����"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CInputLogicNameDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(Tools_String::Whether_Only_LettersNumbersUnderscores(m_strLogicName) == false)
	{
		AfxMessageBox(L"�����к��з�Ӣ�����ֺ��»��ߵ��ַ�");
		return;
	}
	CSkinDlg::OnOK();
}
