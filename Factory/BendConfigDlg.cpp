// BendConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BendConfigDlg.h"
#include "afxdialogex.h"


// CBendConfigDlg dialog

IMPLEMENT_DYNAMIC(CBendConfigDlg, CDialogEx)

CBendConfigDlg::CBendConfigDlg(const int nBendSize, CWnd* pParent /*=NULL*/)
	: CDialogEx(CBendConfigDlg::IDD, pParent)
	, m_nBendSize(nBendSize)
{

}

CBendConfigDlg::~CBendConfigDlg()
{
}

void CBendConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BEND_SIZE, m_comboBendSize);
}


BEGIN_MESSAGE_MAP(CBendConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBendConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBendConfigDlg message handlers


BOOL CBendConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitComboSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBendConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString	strCombo;
	int nSel = m_comboBendSize.GetCurSel();
	m_comboBendSize.GetLBText(nSel, strCombo);
	m_nBendSize = _ttoi(strCombo);
	
	CDialogEx::OnOK();
}

void CBendConfigDlg::InitComboSize(void)
{
	m_comboBendSize.AddString(_T("6"));
	m_comboBendSize.AddString(_T("7"));
	m_comboBendSize.AddString(_T("8"));
	m_comboBendSize.AddString(_T("9"));
	m_comboBendSize.AddString(_T("10"));
	m_comboBendSize.AddString(_T("11"));
	m_comboBendSize.AddString(_T("12"));
	m_comboBendSize.AddString(_T("13"));
	m_comboBendSize.AddString(_T("14"));
	if (m_nBendSize >= 6 && m_nBendSize <= 14)
	{
		m_comboBendSize.SetCurSel(m_nBendSize - 6);
	}
	else
	{
		CString strCustomSize;
		strCustomSize.Format(_T("%d"), m_nBendSize);
		m_comboBendSize.AddString(strCustomSize);
		m_comboBendSize.SetCurSel(9);
	}
}