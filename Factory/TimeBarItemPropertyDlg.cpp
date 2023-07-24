// TimeBarItemPropertyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TimeBarItemPropertyDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"

// CTimeBarItemPropertyDlg �Ի���

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


// CTimeBarItemPropertyDlg ��Ϣ�������


BOOL CTimeBarItemPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_EDIT_TIME_BAR_ITEM_INDEX)->SetWindowText(m_strIndex);
	GetDlgItem(IDC_EDIT_TIME_BAR_ITEM_NAME)->SetWindowText(m_strName);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CTimeBarItemPropertyDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT_TIME_BAR_ITEM_INDEX)->GetWindowText(m_strIndex);
	GetDlgItem(IDC_EDIT_TIME_BAR_ITEM_NAME)->GetWindowText(m_strName);
	CDialog::OnOK();
}
