// NoModelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NoModelDlg.h"
#include "afxdialogex.h"


// CNoModelDlg �Ի���

IMPLEMENT_DYNAMIC(CNoModelDlg, CDialogEx)

CNoModelDlg::CNoModelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNoModelDlg::IDD, pParent)
	, m_strShow(_T("���ڸ�����..."))
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


// CNoModelDlg ��Ϣ�������


BOOL CNoModelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	SetWindowText(_T("�Զ�����"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
