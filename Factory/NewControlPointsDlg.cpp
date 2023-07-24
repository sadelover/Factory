// NewControlPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewControlPointsDlg.h"
#include "afxdialogex.h"


// CNewControlPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewControlPointsDlg, CDialogEx)

CNewControlPointsDlg::CNewControlPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewControlPointsDlg::IDD, pParent)
	, m_nstrControlRoomName(_T(""))
{

}

CNewControlPointsDlg::~CNewControlPointsDlg()
{
}

void CNewControlPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROOM_NAME, m_nstrControlRoomName);
}


BEGIN_MESSAGE_MAP(CNewControlPointsDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewControlPointsDlg message handlers
