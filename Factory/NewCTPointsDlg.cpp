// NewCTPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewCTPointsDlg.h"
#include "afxdialogex.h"


// CNewCTPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewCTPointsDlg, CDialogEx)

CNewCTPointsDlg::CNewCTPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewCTPointsDlg::IDD, pParent)
	, m_nSingleCTowerCount(0)
	, m_nRoomNameCT(_T(""))
	, m_nSatartNumCT(0)
{

}

CNewCTPointsDlg::~CNewCTPointsDlg()
{
}

void CNewCTPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_SINGLE_CTPOINT, m_nSingleCTowerCount);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME_CTOWER, m_nRoomNameCT);
	DDX_Text(pDX, IDC_EDIT_NUM_CTSTART, m_nSatartNumCT);
}


BEGIN_MESSAGE_MAP(CNewCTPointsDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NUM_SINGLE_CTPOINT, &CNewCTPointsDlg::OnEnChangeEditNumSingleCtpoint)
END_MESSAGE_MAP()


// CNewCTPointsDlg message handlers


void CNewCTPointsDlg::OnEnChangeEditNumSingleCtpoint()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
