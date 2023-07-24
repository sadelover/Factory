// CalcPointProperty.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CalcPointProperty.h"
#include "afxdialogex.h"


// CCalcPointProperty �Ի���

IMPLEMENT_DYNAMIC(CCalcPointProperty, CSkinDlg)

CCalcPointProperty::CCalcPointProperty(const BOOL bIsAdd, CWnd* pParent /*=NULL*/)
	: CSkinDlg(CCalcPointProperty::IDD, pParent)
	, m_strName(_T(""))
	, m_bIsAdd(bIsAdd)
	, m_nPeriod(1)
{

}

CCalcPointProperty::~CCalcPointProperty()
{
}

void CCalcPointProperty::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CALC_NAME, m_strName);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_PERIOD, m_comboPeriod);
}


BEGIN_MESSAGE_MAP(CCalcPointProperty, CSkinDlg)
END_MESSAGE_MAP()


// CCalcPointProperty ��Ϣ�������


BOOL CCalcPointProperty::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (m_bIsAdd)
	{
		SetWindowText(_T("�½������߳�"));
	}
	else
	{
		SetWindowText(_T("�༭�����߳�"));
		CEdit* pEditName = (CEdit*)GetDlgItem(IDC_EDIT_CALC_NAME);
		if (pEditName != NULL)
		{
			pEditName->EnableWindow(FALSE);
		}
	}

	m_comboPeriod.AddString(_T("1"));
	m_comboPeriod.AddString(_T("5"));
	m_comboPeriod.AddString(_T("30"));
	m_comboPeriod.AddString(_T("60"));
	switch (m_nPeriod)
	{
	case 1:
		m_comboPeriod.SetCurSel(0);
		break;
	case 5:
		m_comboPeriod.SetCurSel(1);
		break;
	case 30:
		m_comboPeriod.SetCurSel(2);
		break;
	case 60:
		m_comboPeriod.SetCurSel(3);
		break;
	default:
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCalcPointProperty::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	int nPos = m_strName.Find(_T("("));
	if (nPos != -1)
	{
		CEdit* pName = (CEdit*)GetDlgItem(IDC_EDIT_CALC_NAME);
		if (pName != NULL)
		{
			pName->SetSel(0, -1);
			pName->SetFocus();
		}
		MessageBox(_T("���Ʋ����԰����ַ�("));
		return;
	}

	CString strPeriod;
	int nSel = m_comboPeriod.GetCurSel();
	m_comboPeriod.GetLBText(nSel, strPeriod);
	m_nPeriod = _ttoi(strPeriod);

	CSkinDlg::OnOK();
}
