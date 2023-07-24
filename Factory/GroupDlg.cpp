// GroupDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GroupDlg.h"
#include "afxdialogex.h"


// CGroupDlg �Ի���

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


// CGroupDlg ��Ϣ�������


void CGroupDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_strGroupName.GetLength() == 0)
	{
		MessageBox(L"�����Ʋ���Ϊ��!");
		return;
	}
	UpdateData(FALSE);
	CSkinDlg::OnOK();
}
