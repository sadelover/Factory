// NavigateWindowItemSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NavigateWindowItemSetDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CNavigateWindowItemSetDlg dialog

IMPLEMENT_DYNAMIC(CNavigateWindowItemSetDlg, CDialogEx)

CNavigateWindowItemSetDlg::CNavigateWindowItemSetDlg(const CString strDesc, const CString strPageName, const int nPageId, CWnd* pParent /*=NULL*/)
	: CDialogEx(CNavigateWindowItemSetDlg::IDD, pParent)
	, m_strDesc(strDesc)
	, m_strPageName(strPageName)
	, m_nPageId(nPageId)
	, m_pMainFrm(NULL)
{

}

CNavigateWindowItemSetDlg::~CNavigateWindowItemSetDlg()
{
}

void CNavigateWindowItemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DESC, m_strDesc);
	DDX_Control(pDX, IDC_COMBO_PAGE, m_comboPage);
	DDX_CBString(pDX, IDC_COMBO_PAGE, m_strPageName);
	DDX_Text(pDX, IDC_EDIT_PAGE_ID, m_nPageId);
}


BEGIN_MESSAGE_MAP(CNavigateWindowItemSetDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE, &CNavigateWindowItemSetDlg::OnCbnSelchangeComboPage)
END_MESSAGE_MAP()


// CNavigateWindowItemSetDlg message handlers


BOOL CNavigateWindowItemSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CNavigateWindowItemSetDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CDialogEx::OnOK();
}

void CNavigateWindowItemSetDlg::InitControls(void)
{
	m_pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(m_pMainFrm != NULL);
	if (NULL == m_pMainFrm)
	{
		return;
	}

	CEqmDrawProject* pProject = m_pMainFrm->m_DrawContext.GetProject();
	ASSERT(pProject != NULL);
	if (NULL == pProject)
	{
		return;
	}

	vector<CEqmDrawPage*> pageList = pProject->GetPageList();
	vector<CEqmDrawPage*>::const_iterator	iter;

	m_vecProjPage.clear();
	for (iter=pageList.begin(); iter!=pageList.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			ProjPage	projPage;
			projPage.nPageId = (*iter)->GetID();
			projPage.strPageName = (*iter)->GetPageName().c_str();
			m_vecProjPage.push_back(projPage);
		}
	}


	m_comboPage.ResetContent();
	vector<ProjPage>::const_iterator	it;
	for (it=m_vecProjPage.begin(); it!=m_vecProjPage.end(); ++it)
	{
		m_comboPage.AddString(it->strPageName);
	}
	if (m_comboPage.GetCount() > 0)
	{
		int i = 0;
		for (it=m_vecProjPage.begin(); it!=m_vecProjPage.end(); ++it,++i)
		{
			if (it->nPageId == m_nPageId)
			{
				m_comboPage.SetCurSel(i);
				break;
			}
		}
	}
}


void CNavigateWindowItemSetDlg::OnCbnSelchangeComboPage()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	vector<ProjPage>::const_iterator	it;
	for (it=m_vecProjPage.begin(); it!=m_vecProjPage.end(); ++it)
	{
		if (it->strPageName == m_strPageName)
		{
			m_nPageId = it->nPageId;
			UpdateData(FALSE);
			break;
		}
	}
}
