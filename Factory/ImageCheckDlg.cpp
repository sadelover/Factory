// ImageCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageCheckDlg.h"
#include "afxdialogex.h"


// CImageCheckDlg dialog

IMPLEMENT_DYNAMIC(CImageCheckDlg, CDialogEx)

CImageCheckDlg::CImageCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageCheckDlg::IDD, pParent)
	, m_nCount(0)
{

}

CImageCheckDlg::~CImageCheckDlg()
{
}

void CImageCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_listInfo);
}


BEGIN_MESSAGE_MAP(CImageCheckDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageCheckDlg message handlers


BOOL CImageCheckDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();

	ShowListInfo();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImageCheckDlg::InitControls(void)
{
	m_listInfo.InsertColumn(0, _T("编号"), LVCFMT_LEFT, 40);
	m_listInfo.InsertColumn(1, _T("页面名称"), LVCFMT_LEFT, 100);
	m_listInfo.InsertColumn(2, _T("图元ID"), LVCFMT_LEFT, 80);
	m_listInfo.InsertColumn(3, _T("图元名称"), LVCFMT_LEFT, 120);
	m_listInfo.InsertColumn(4, _T("绑定点位"), LVCFMT_LEFT, 120);
}

void CImageCheckDlg::ShowListInfo(void)
{
	m_listInfo.DeleteAllItems();

	CString	strId;
	CString	strElement;
	vector<ImageCheckInfo>::const_iterator	iter;
	for (iter=m_vecImgChkInfo.begin(); iter!=m_vecImgChkInfo.end(); ++iter)
	{
		strId.Format(_T("%d"), iter->nCount+1);
		strElement.Format(_T("%d"), iter->nElementId);

		m_listInfo.InsertItem(iter->nCount, strId);
		m_listInfo.SetItemText(iter->nCount, 1, iter->strPageName);
		m_listInfo.SetItemText(iter->nCount, 2, strElement);
		m_listInfo.SetItemText(iter->nCount, 3, iter->strImageName);
		m_listInfo.SetItemText(iter->nCount, 4, iter->strBindPtName);
	}
}

void CImageCheckDlg::SetImageCheckInfo(const CString strPageName, const int nElementId, const CString strImageName, const CString strBindPtName)
{
	ImageCheckInfo	imgChkInf;
	imgChkInf.nCount		= m_nCount;
	imgChkInf.strPageName	= strPageName;
	imgChkInf.nElementId	= nElementId;
	imgChkInf.strImageName	= strImageName;
	imgChkInf.strBindPtName	= strBindPtName;
	m_vecImgChkInfo.push_back(imgChkInf);

	++m_nCount;
}
