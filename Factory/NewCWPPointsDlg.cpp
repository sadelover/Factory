// NewCWPPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewCWPPointsDlg.h"
#include "afxdialogex.h"


// CNewCWPPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewCWPPointsDlg, CDialogEx)

CNewCWPPointsDlg::CNewCWPPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewCWPPointsDlg::IDD, pParent)
	, m_nSingleCWPowerCount(0)
	, m_strRoomNameCWP(_T(""))
	, m_nStartNumCWP(0)
{

}

CNewCWPPointsDlg::~CNewCWPPointsDlg()
{
}

void CNewCWPPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_SINGLE_CWPPOINT, m_nSingleCWPowerCount);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME_CWP, m_strRoomNameCWP);
	DDX_Text(pDX, IDC_EDIT_NUM_CWPSTART, m_nStartNumCWP);
}


BEGIN_MESSAGE_MAP(CNewCWPPointsDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewCWPPointsDlg message handlers
