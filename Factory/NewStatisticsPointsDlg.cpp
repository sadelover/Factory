// NewStatisticsPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewStatisticsPointsDlg.h"
#include "afxdialogex.h"


// CNewStatisticsPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewStatisticsPointsDlg, CDialogEx)

CNewStatisticsPointsDlg::CNewStatisticsPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewStatisticsPointsDlg::IDD, pParent)
	, m_strNewRoomName(_T(""))
{

}

CNewStatisticsPointsDlg::~CNewStatisticsPointsDlg()
{
}

void CNewStatisticsPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROOM_NAME, m_strNewRoomName);
}


BEGIN_MESSAGE_MAP(CNewStatisticsPointsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewStatisticsPointsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewStatisticsPointsDlg message handlers


void CNewStatisticsPointsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
