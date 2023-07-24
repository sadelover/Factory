// TemperDistriItemSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TemperDistriItemSetDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "MetaFileSelectDlg.h"
#include "MainFrm.h"
#include "CanvasDlg.h"


// CTemperDistriItemSetDlg dialog

IMPLEMENT_DYNAMIC(CTemperDistriItemSetDlg, CDialogEx)

CTemperDistriItemSetDlg::CTemperDistriItemSetDlg(
		const int nPicId,
		const CString strPointList,
		const CString strBindPtName,
		CDataPointManager* pDataPointMgr,
		CCanvasDlg* pCanvas,
		CWnd* pParent /*=NULL*/)
	: CDialogEx(CTemperDistriItemSetDlg::IDD, pParent)
	, m_nPicId(nPicId)
	, m_strBindPtName(strBindPtName)
	, m_strPointList(strPointList)
	, m_pDataPointMgr(pDataPointMgr)
	, m_pCanvas(pCanvas)
{

}

CTemperDistriItemSetDlg::~CTemperDistriItemSetDlg()
{
}

void CTemperDistriItemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POINT_SET, m_strPointList);
	DDX_Text(pDX, IDC_EDIT_BINDPT_NAME, m_strBindPtName);
	DDX_Text(pDX, IDC_EDIT_PIC_ID, m_nPicId);
}


BEGIN_MESSAGE_MAP(CTemperDistriItemSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BINDPT_NAME, &CTemperDistriItemSetDlg::OnBnClickedBtnBindptName)
	ON_BN_CLICKED(IDC_BTN_PIC, &CTemperDistriItemSetDlg::OnBnClickedBtnPic)
END_MESSAGE_MAP()


// CTemperDistriItemSetDlg message handlers


BOOL CTemperDistriItemSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTemperDistriItemSetDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}


void CTemperDistriItemSetDlg::OnBnClickedBtnPic()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CMetaFileSelectDlg dlg;
	dlg.SetWndType(e_picture);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame != NULL);
	if (NULL == pFrame)
	{
		return;
	}

	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (IDOK == dlg.DoModal())
	{
		int nSelId = dlg.m_nID;
		int nUseId = 0;
		if (m_pCanvas != NULL)
		{
			m_pCanvas->InsertImageIntoOpenS3db(nSelId, nUseId);
		}

		m_nPicId = nUseId;
		pFrame->m_nSelectPicID = nSelId;
	}
	UpdateData(FALSE);
}


void CTemperDistriItemSetDlg::OnBnClickedBtnBindptName()
{
	// TODO: Add your control notification handler code here
	CDataComConfigDlg dlg(*m_pDataPointMgr);
	if (IDOK == dlg.DoModal())
	{
		UpdateData(TRUE);
		m_strBindPtName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}
