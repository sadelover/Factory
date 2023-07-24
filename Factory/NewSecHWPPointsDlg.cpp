// NewSecHWPPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewSecHWPPointsDlg.h"
#include "afxdialogex.h"


// CNewSecHWPPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewSecHWPPointsDlg, CDialogEx)

CNewSecHWPPointsDlg::CNewSecHWPPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewSecHWPPointsDlg::IDD, pParent)
	, m_nSingleSecHWPCount(0)
	, m_strRoomNameSecHWP(_T(""))
	, m_nStartNameSecHWP(0)
{

}

CNewSecHWPPointsDlg::~CNewSecHWPPointsDlg()
{
}

void CNewSecHWPPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_SINGLE_SecHWPPOINT, m_nSingleSecHWPCount);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME_SecHWP, m_strRoomNameSecHWP);
	DDX_Text(pDX, IDC_EDIT_NUM_SecHWPSTART, m_nStartNameSecHWP);
}


BEGIN_MESSAGE_MAP(CNewSecHWPPointsDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NUM_SINGLE_SecHWPPOINT, &CNewSecHWPPointsDlg::OnEnChangeEditNumSingleSechwppoint)
END_MESSAGE_MAP()


// CNewSecHWPPointsDlg message handlers


void CNewSecHWPPointsDlg::OnEnChangeEditNumSingleSechwppoint()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
