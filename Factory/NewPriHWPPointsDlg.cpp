// NewPriHWPPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewPriHWPPointsDlg.h"
#include "afxdialogex.h"


// CNewPriHWPPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewPriHWPPointsDlg, CDialogEx)

CNewPriHWPPointsDlg::CNewPriHWPPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewPriHWPPointsDlg::IDD, pParent)
	, m_nSinglePriHWPCount(0)
	, m_strRoomNamePriHWP(_T(""))
	, m_nStartNumPriHWP(0)
{

}

CNewPriHWPPointsDlg::~CNewPriHWPPointsDlg()
{
}

void CNewPriHWPPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_SINGLE_PriHWPPOINT, m_nSinglePriHWPCount);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME_PriHWP, m_strRoomNamePriHWP);
	DDX_Text(pDX, IDC_EDIT_NUM_PriHWPSTART, m_nStartNumPriHWP);
}


BEGIN_MESSAGE_MAP(CNewPriHWPPointsDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewPriHWPPointsDlg message handlers
