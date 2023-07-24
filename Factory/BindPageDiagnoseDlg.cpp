// BindPageDiagnoseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BindPageDiagnoseDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CBindPageDiagnoseDlg 对话框

IMPLEMENT_DYNAMIC(CBindPageDiagnoseDlg, CDialog)

CBindPageDiagnoseDlg::CBindPageDiagnoseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBindPageDiagnoseDlg::IDD, pParent)
	, m_str_page_diagnose(_T(""))
	, m_strPageID(_T(""))
	, m_str_combo_page_name(_T(""))
{

}

CBindPageDiagnoseDlg::~CBindPageDiagnoseDlg()
{
}

void CBindPageDiagnoseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DIAGNOSE_PAGE_NAME, m_str_page_diagnose);
	DDX_Text(pDX, IDC_EDIT_DIAGNOSE_PAGE_ID, m_strPageID);
	DDX_Control(pDX, IDC_COMBO_PAGE_LIST, m_combo_page_list);
	DDX_CBString(pDX, IDC_COMBO_PAGE_LIST, m_str_combo_page_name);
}


BEGIN_MESSAGE_MAP(CBindPageDiagnoseDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBindPageDiagnoseDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE_LIST, &CBindPageDiagnoseDlg::OnCbnSelchangeComboPageList)
END_MESSAGE_MAP()


// CBindPageDiagnoseDlg 消息处理程序


BOOL CBindPageDiagnoseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBindPageDiagnoseDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(FALSE);
	CDialog::OnOK();
}

void CBindPageDiagnoseDlg::Init()
{
	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	if (pMainFrm)
	{
		CEqmDrawProject* pProject = pMainFrm->m_DrawContext.GetProject();
		if (pProject)
		{
			vector<CEqmDrawPage*> pageList = pProject->GetPageList();
			const size_t size = pageList.size();
			int nCurSel = 0;
			CString	strPageName;

			for (size_t i=0;i<size;++i)
			{
				strPageName.Format(L"%s", pageList[i]->GetPageName().c_str());
				m_combo_page_list.AddString(strPageName);
				if (strPageName == m_str_page_diagnose)
				{
					nCurSel = i;
				}
			}

			if (m_strPageID != _T("-1") && m_combo_page_list.GetCount() > 0)
			{
				m_combo_page_list.SetCurSel(nCurSel);
			}
		}
	}
}


void CBindPageDiagnoseDlg::OnCbnSelchangeComboPageList()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_str_page_diagnose = m_str_combo_page_name;
	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	if (pMainFrm)
	{
		CEqmDrawProject* pProject = pMainFrm->m_DrawContext.GetProject();
		if (pProject)
		{
			vector<CEqmDrawPage*> pageList = pProject->GetPageList();
			const size_t size = pageList.size();
			for (size_t i=0;i<size;++i)
			{
				if (pageList[i]->GetPageName() == m_str_page_diagnose.GetString())
				{
					m_strPageID.Format(L"%d",pageList[i]->GetID());
					break;
				}
			}
		}
	}
	UpdateData(FALSE);
}
