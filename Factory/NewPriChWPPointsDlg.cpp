
// CNewPriChWPPointsDlg message handlers
// NewPriChWPPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewPriChWPPointsDlg.h"
#include "afxdialogex.h"


// CNewPriChWPPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewPriChWPPointsDlg, CDialogEx)

	CNewPriChWPPointsDlg::CNewPriChWPPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewPriChWPPointsDlg::IDD, pParent)
	, m_nSinglePriChWPCount(0)
	, m_strRoomNamePriChWP(_T(""))
	, m_nStartNumPriChWP(0)
{

}

CNewPriChWPPointsDlg::~CNewPriChWPPointsDlg()
{
}

void CNewPriChWPPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_SINGLE_PriChWPPOINT, m_nSinglePriChWPCount);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME_PriCHWP, m_strRoomNamePriChWP);
	DDX_Text(pDX, IDC_EDIT_NUM_PriChWPSTART, m_nStartNumPriChWP);
}

BEGIN_MESSAGE_MAP(CNewPriChWPPointsDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewPriChWPPointsDlg message handlers
