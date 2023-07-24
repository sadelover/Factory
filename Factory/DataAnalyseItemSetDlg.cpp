// DataAnalyseItemSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataAnalyseItemSetDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"


// CDataAnalyseItemSetDlg dialog

IMPLEMENT_DYNAMIC(CDataAnalyseItemSetDlg, CDialogEx)

CDataAnalyseItemSetDlg::CDataAnalyseItemSetDlg(CDataPointManager* pDataPointMgr, const CString strPtName, const CString strRemarks, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataAnalyseItemSetDlg::IDD, pParent)
	, m_pDataPointMgr(pDataPointMgr)
	, m_strPtName(strPtName)
	, m_strRemarks(strRemarks)
{

}

CDataAnalyseItemSetDlg::~CDataAnalyseItemSetDlg()
{
}

void CDataAnalyseItemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PT_NAME, m_strPtName);
	DDX_Text(pDX, IDC_EDIT_REMARKS, m_strRemarks);
}


BEGIN_MESSAGE_MAP(CDataAnalyseItemSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CDataAnalyseItemSetDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_BROWSE_REMARKS, &CDataAnalyseItemSetDlg::OnBnClickedBtnBrowseRemarks)
END_MESSAGE_MAP()


// CDataAnalyseItemSetDlg message handlers


BOOL CDataAnalyseItemSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDataAnalyseItemSetDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}


void CDataAnalyseItemSetDlg::OnBnClickedBtnBrowse()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_pDataPointMgr)
	{
		return;
	}

	CDataComConfigDlg dlg(*m_pDataPointMgr);
	if (IDOK == dlg.DoModal())
	{
		UpdateData(TRUE);
		m_strPtName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CDataAnalyseItemSetDlg::OnBnClickedBtnBrowseRemarks()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_pDataPointMgr)
	{
		return;
	}

	CDataComConfigDlg dlg(*m_pDataPointMgr);
	if (IDOK == dlg.DoModal())
	{
		UpdateData(TRUE);
		m_strRemarks = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}
