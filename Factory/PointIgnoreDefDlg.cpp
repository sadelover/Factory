// PointIgnoreDefDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PointIgnoreDefDlg.h"
#include "afxdialogex.h"


// CPointIgnoreDefDlg �Ի���

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


// CPointIgnoreDefDlg ��Ϣ�������


void CPointIgnoreDefDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
	/*
		��ȡ�༭���е�����
	*/

	/*
		���������ݿ�
		unit01��
		unitproperty01=  'point_ignore_list'
		unitproperty02 = �Զ��ż�������ε��嵥
	*/


}


void CPointIgnoreDefDlg::OnEnChangeEditPoint()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
