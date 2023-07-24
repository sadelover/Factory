// StoreCycleSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StoreCycleSettingDlg.h"
#include "afxdialogex.h"


// CStoreCycleSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CStoreCycleSettingDlg, CDialog)

CStoreCycleSettingDlg::CStoreCycleSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStoreCycleSettingDlg::IDD, pParent)
	, m_str_combo_cycle(_T(""))
{

}

CStoreCycleSettingDlg::~CStoreCycleSettingDlg()
{
}

void CStoreCycleSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STORE_CYCLE_FOR_MULTI, m_combo_cycle);
	DDX_CBString(pDX, IDC_COMBO_STORE_CYCLE_FOR_MULTI, m_str_combo_cycle);
}


BEGIN_MESSAGE_MAP(CStoreCycleSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CStoreCycleSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CStoreCycleSettingDlg ��Ϣ�������


BOOL CStoreCycleSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_combo_cycle.AddString(L"���洢");
	m_combo_cycle.AddString(L"5����");
	m_combo_cycle.AddString(L"1����");
	m_combo_cycle.AddString(L"5����");
	m_combo_cycle.AddString(L"��Сʱ");
	m_combo_cycle.AddString(L"1Сʱ");
	m_combo_cycle.AddString(L"1��");
	m_combo_cycle.AddString(L"1�ܣ�7�죩");
	m_combo_cycle.AddString(L"1����");
	m_combo_cycle.AddString(L"1��");
	m_combo_cycle.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CStoreCycleSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nCycle = m_combo_cycle.GetCurSel();
	CString	strSelName;
	m_combo_cycle.GetLBText(m_nCycle, strSelName);
	if (L"��Сʱ" == strSelName)
	{
		MessageBox(_T("�洢�����Ѳ�֧�ְ�Сʱ��\n������ѡ��"), _T("����"), MB_OK|MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}
