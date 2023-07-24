// TimeSelectPropertyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TimeSelectPropertyDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "Tools/CustomTools/CustomTools.h"

// CTimeSelectPropertyDlg �Ի���

IMPLEMENT_DYNAMIC(CTimeSelectPropertyDlg, CDialog)

CTimeSelectPropertyDlg::CTimeSelectPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeSelectPropertyDlg::IDD, pParent)
	, m_radioDateTimeSelect(0)
{
	m_pPointMap = NULL;
	m_rwProperty = 0;
}

CTimeSelectPropertyDlg::~CTimeSelectPropertyDlg()
{
}

void CTimeSelectPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DATETIME, m_radioDateTimeSelect);
}


BEGIN_MESSAGE_MAP(CTimeSelectPropertyDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTimeSelectPropertyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_TIME_POINT_SELECT, &CTimeSelectPropertyDlg::OnBnClickedButtonTimePointSelect)
	ON_BN_CLICKED(IDC_BUTTON_TIME_SELECT_RW, &CTimeSelectPropertyDlg::OnBnClickedButtonTimeSelectRw)
END_MESSAGE_MAP()


// CTimeSelectPropertyDlg ��Ϣ�������


void CTimeSelectPropertyDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CDialog::OnOK();
}


void CTimeSelectPropertyDlg::OnBnClickedButtonTimePointSelect()
{
	CDataComConfigDlg dlg(*m_pPointMap);
	if (dlg.DoModal() == IDOK)
	{
		m_strBindPointName = dlg.m_strPointName;
		GetDlgItem(IDC_EDIT_BIND_TIME_POINT)->SetWindowTextW(m_strBindPointName);
	}
}

void CTimeSelectPropertyDlg::OnBnClickedButtonTimeSelectRw()
{
	CString str;
	GetDlgItem(IDC_BUTTON_TIME_SELECT_RW)->GetWindowText(str);
	if (str == L"��")
	{
		GetDlgItem(IDC_BUTTON_TIME_SELECT_RW)->SetWindowText(L"д");
		m_rwProperty = 1;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_TIME_SELECT_RW)->SetWindowText(L"��");
		m_rwProperty = 0;
	}
}

void CTimeSelectPropertyDlg::SetPointMap( CDataPointManager* pPointMap )
{
	ASSERT(pPointMap);

	m_pPointMap = pPointMap;
}

BOOL CTimeSelectPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (m_strBindPointName.GetLength() > 0)
	{
		GetDlgItem(IDC_EDIT_BIND_TIME_POINT)->SetWindowTextW(m_strBindPointName);
	}
	if (m_rwProperty == 0)
	{
		GetDlgItem(IDC_BUTTON_TIME_SELECT_RW)->SetWindowText(L"��");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_TIME_SELECT_RW)->SetWindowText(L"д");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}