// SystemTactics.cpp : implementation file
//

#include "stdafx.h"
#include "SystemTactics.h"
#include "afxdialogex.h"
#include "Tools/Tools_String.h"

// CSystemTactics dialog

IMPLEMENT_DYNAMIC(CSystemTactics, CSkinDlg)

CSystemTactics::CSystemTactics(vector<SYS_TACTICS>& vecSysTactics, CWnd* pParent /*=NULL*/)
	: CSkinDlg(CSystemTactics::IDD, pParent)
	, m_strSysTacticsName(_T(""))
{
	m_vecSysTactics = vecSysTactics;
}

CSystemTactics::~CSystemTactics()
{
}

void CSystemTactics::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SYS_TACTICS, m_comboSysTactics);
	DDX_Text(pDX, IDC_EDIT_SYS_TACTICS_NAME, m_strSysTacticsName);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CSystemTactics, CSkinDlg)
	ON_BN_CLICKED(IDOK, &CSystemTactics::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_SYS_TACTICS, &CSystemTactics::OnCbnSelchangeComboSysTactics)
END_MESSAGE_MAP()


// CSystemTactics message handlers

BOOL CSystemTactics::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	InitSysTactics();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSystemTactics::InitSysTactics(void)
{
	if (m_vecSysTactics.empty())
	{
		return;
	}

	m_comboSysTactics.Clear();

	vector<SYS_TACTICS>::const_iterator iter;
	for (iter=m_vecSysTactics.begin(); iter!=m_vecSysTactics.end(); ++iter)
	{
		m_comboSysTactics.AddString(iter->strFileTitle);
	}
	
	m_comboSysTactics.SetCurSel(0);

	m_strSysTacticsName = GetCurSelSysTactics();
	UpdateData(FALSE);
}

CString CSystemTactics::GetCurSelSysTactics(void)
{
	int nCurSel = m_comboSysTactics.GetCurSel();
	CString strCurSel;
	m_comboSysTactics.GetLBText(nCurSel, strCurSel);

	return strCurSel;
}


void CSystemTactics::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_strCurSelName = GetCurSelSysTactics();
	UpdateData(TRUE);
	if(Tools_String::Whether_Only_LettersNumbersUnderscores(m_strSysTacticsName) == false)
	{
		AfxMessageBox(L"名字中含有非英文数字和下划线的字符");
		return;
	}
	CSkinDlg::OnOK();
}


void CSystemTactics::OnCbnSelchangeComboSysTactics()
{
	// TODO: Add your control notification handler code here
	m_strSysTacticsName = GetCurSelSysTactics();
	UpdateData(FALSE);
}
