// AddAreaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AddAreaDlg.h"
#include "afxdialogex.h"
#include "MetaFileSelectDlg.h"
#include "MainFrm.h"
// CAddAreaDlg dialog

IMPLEMENT_DYNAMIC(CAddAreaDlg, CDialog)

CAddAreaDlg::CAddAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddAreaDlg::IDD, pParent)
	, m_strPointList(_T(""))
	, m_dXpos(0)
	, m_dYpos(0)
	, m_nBitMapID(0)
	, m_dWidth(0)
	, m_dHeight(0)
	, m_nLinkPageID(0)
{

}

CAddAreaDlg::~CAddAreaDlg()
{
}

void CAddAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POINT, m_strPointList);
	DDX_Text(pDX, IDC_EDIT_XPOS, m_dXpos);
	DDX_Text(pDX, IDC_EDIT_YPOX, m_dYpos);
	DDX_Text(pDX, IDC_EDIT_IMAGE, m_nBitMapID);
	DDX_Control(pDX, IDC_COMBO_PAGE_ID, m_combo_page);
}

BEGIN_MESSAGE_MAP(CAddAreaDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_POINT, &CAddAreaDlg::OnBnClickedButtonAddPoint)
	ON_BN_CLICKED(IDC_BUTTON_ADD_IMAGE, &CAddAreaDlg::OnBnClickedButtonAddImage)
	ON_BN_CLICKED(IDOK, &CAddAreaDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CAddAreaDlg message handlers

void CAddAreaDlg::OnBnClickedButtonAddPoint()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str;
	str.Format(_T("|%d,%d"),m_dXpos,m_dYpos);
	m_strPointList += str;
	UpdateData(FALSE);
}

void CAddAreaDlg::OnBnClickedButtonAddImage()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CMetaFileSelectDlg dlg;
	dlg.SetWndType(e_picture);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		m_nBitMapID = dlg.m_nID;
		pFrame->m_nSelectPicID = m_nBitMapID;
	}
	UpdateData(FALSE);
}

void CAddAreaDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_nLinkPageID = m_combo_page.GetCurSel();
	CDialog::OnOK();
}

BOOL CAddAreaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	str.Format(_T("(%d)"),m_dWidth);
	((CStatic*)GetDlgItem(IDC_STATIC_X))->SetWindowText(str);

	str.Format(_T("(%d)"),m_dHeight);
	((CStatic*)GetDlgItem(IDC_STATIC_Y))->SetWindowText(str);


	InitComboPage();

	UpdateData(FALSE);
	return TRUE;
}

void CAddAreaDlg::InitComboPage()
{
	m_combo_page.ResetContent();
	m_combo_page.AddString(L"²»Ö¸¶¨");
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	CEqmDrawProject* pProject = pMainFrame->m_DrawContext.GetProject();
	ASSERT(pProject);
	vector<CEqmDrawPage*> pageList = pProject->GetPageList();
	const size_t size = pageList.size();
	for (size_t i=0;i<size;++i)
	{
		if (pageList[i])
		{
			CString strPageName = pageList[i]->GetPageName().c_str();
			m_combo_page.AddString(strPageName);
		}
	}

	CEqmDrawPage* pPage = pMainFrame->m_DrawContext.GetProject()->GetPageByID(m_nLinkPageID);
	if (pPage)
	{
		CString strPageName = pPage->GetPageName().c_str();
		ASSERT(strPageName.GetLength()>0);
		for (int j=0;j<m_combo_page.GetCount();++j)
		{
			CString strContent;
			m_combo_page.GetLBText(j,strContent);
			ASSERT(strContent.GetLength()>0);
			if (strContent == strPageName)
			{
				m_combo_page.SetCurSel(j);
				return;
			}
		}
	}
	else
	{
		m_combo_page.SetCurSel(0);
	}
}
