// NewSecChWPPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewSecChWPPointsDlg.h"
#include "afxdialogex.h"


// CNewSecChWPPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewSecChWPPointsDlg, CDialogEx)

CNewSecChWPPointsDlg::CNewSecChWPPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewSecChWPPointsDlg::IDD, pParent)
	, m_nSingleSecChWPCount(0)
	, m_strRoomNameSecChWP(_T(""))
	, m_nStartNumSecChWP(0)
{

}

CNewSecChWPPointsDlg::~CNewSecChWPPointsDlg()
{
}

void CNewSecChWPPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_SINGLE_SecChWPPOINT, m_nSingleSecChWPCount);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME_SecChWP, m_strRoomNameSecChWP);
	DDX_Text(pDX, IDC_EDIT_NUM_SecChWPSTART, m_nStartNumSecChWP);
}


BEGIN_MESSAGE_MAP(CNewSecChWPPointsDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewSecChWPPointsDlg message handlers
