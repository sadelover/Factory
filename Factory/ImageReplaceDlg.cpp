// ImageReplaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageReplaceDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "BEOP-DesignerView.h"


// CImageReplaceDlg dialog

IMPLEMENT_DYNAMIC(CImageReplaceDlg, CSkinDlg)

CImageReplaceDlg::CImageReplaceDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CImageReplaceDlg::IDD, pParent)
{

}

CImageReplaceDlg::~CImageReplaceDlg()
{
}

void CImageReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_listShow);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CImageReplaceDlg, CSkinDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHOW, &CImageReplaceDlg::OnNMDblclkListShow)
END_MESSAGE_MAP()


// CImageReplaceDlg message handlers


BOOL CImageReplaceDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImageReplaceDlg::InitControls(void)
{
	m_listShow.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listShow.InsertColumn(0, _T("页面ID"), LVCFMT_LEFT, 120);
	m_listShow.InsertColumn(1, _T("页面名称"), LVCFMT_LEFT, 120);
	m_listShow.InsertColumn(2, _T("图元编号"), LVCFMT_LEFT, 120);
	m_listShow.InsertColumn(3, _T("图元ID"), LVCFMT_LEFT, 120);
}

void CImageReplaceDlg::SetDataInfo(void)
{
	m_listShow.DeleteAllItems();


	CString strPageId;
	CString	strPageName;
	CString	strElementId;
	CString	strPicId;

	vector<ImgReplaceInfo>::const_iterator	iter;
	int i = 0;
	for (iter=m_vecReplaceInfo.begin(); iter!=m_vecReplaceInfo.end(); ++iter,++i)
	{
		strPageId.Format(_T("%d"), iter->nPageId);
		strPageName.Format(_T("%s"), iter->strPageName.c_str());
		strElementId.Format(_T("%d"), iter->nElementId);
		strPicId.Format(_T("%d"), iter->nPageId);

		m_listShow.InsertItem(i, strPageId);
		m_listShow.SetItemText(i, 1, strPageName);
		m_listShow.SetItemText(i, 2, strElementId);
		m_listShow.SetItemText(i, 3, strPicId);
	}
}

void CImageReplaceDlg::SetReplaceInfo(const vector<ImgReplaceInfo>& vecReplaceInfo)
{
	m_vecReplaceInfo = vecReplaceInfo;

	SetDataInfo();
}


void CImageReplaceDlg::OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	CString strPageId(m_listShow.GetItemText(pNMItemActivate->iItem,0));
	CString strElementId(m_listShow.GetItemText(pNMItemActivate->iItem,2));
	CString strImageId(m_listShow.GetItemText(pNMItemActivate->iItem,3));

	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrm->GetActiveView());
	ASSERT(pView != NULL);
	if (NULL == pView)
	{
		return;
	}

	pMainFrm->m_wndFileView.ShowPageByPageID(_ttoi(strPageId), _ttoi(strElementId), _ttoi(strImageId));
}
