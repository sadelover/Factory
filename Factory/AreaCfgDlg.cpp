// AreaCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AreaCfgDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CAreaCfgDlg dialog

IMPLEMENT_DYNAMIC(CAreaCfgDlg, CDialogEx)

CAreaCfgDlg::CAreaCfgDlg(const CString strPage, const CString strRegion, CWnd* pParent /*=NULL*/)
	: CDialogEx(CAreaCfgDlg::IDD, pParent)
	, m_strPageName(strPage)
	, m_nPageId(0)
	, m_strRegion(strRegion)
{

}

CAreaCfgDlg::~CAreaCfgDlg()
{
}

void CAreaCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PAGE, m_comboPage);
	DDX_CBString(pDX, IDC_COMBO_PAGE, m_strPageName);
	DDX_Text(pDX, IDC_EDIT_REGION, m_strRegion);
}


BEGIN_MESSAGE_MAP(CAreaCfgDlg, CDialogEx)
END_MESSAGE_MAP()


// CAreaCfgDlg message handlers


BOOL CAreaCfgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAreaCfgDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	if (_T("") == m_strPageName)
	{
		MessageBox(_T("«Î—°‘Ò«¯”Ú£°"), _T("¥ÌŒÛ"), MB_OK|MB_ICONERROR);
		return;
	}

	map<int, CString>::const_iterator iter;
	for (iter=m_mapPageInfo.begin(); iter!=m_mapPageInfo.end(); iter++)
	{
		if (iter->second == m_strPageName)
		{
			m_nPageId = iter->first;
			break;
		}
	}

	CDialogEx::OnOK();
}

void CAreaCfgDlg::InitControls(void)
{
	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	CEqmDrawProject* pProject = pMainFrm->m_DrawContext.GetProject();
	ASSERT(pProject != NULL);
	if (NULL == pProject)
	{
		return;
	}

	vector<CEqmDrawPage*> pageList = pProject->GetPageList();
	vector<CEqmDrawPage*>::const_iterator iter;
	m_mapPageInfo.clear();
	m_comboPage.ResetContent();
	for (iter = pageList.begin(); iter != pageList.end(); iter++)
	{
		int nPageId = (*iter)->GetID();
		CString strPageName;
		strPageName.Format(_T("%s"), (*iter)->GetPageName().c_str());
		m_mapPageInfo[nPageId] = strPageName;

		m_comboPage.AddString(strPageName);
	}

	UpdateData(FALSE);
}
