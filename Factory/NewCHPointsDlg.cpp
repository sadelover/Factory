// NewCHPointsDlg.cpp : implementation file
//
#include "stdafx.h"
#include "NewCHPointsDlg.h"
#include "afxdialogex.h"


// CNewCHPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewCHPointsDlg, CDialogEx)

	CNewCHPointsDlg::CNewCHPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewCHPointsDlg::IDD, pParent)
	, m_nSingleChillerCount(0)
	, m_nsinglestartChNum(0)
	, m_strRoomNameCH(_T(""))
{

}

CNewCHPointsDlg::~CNewCHPointsDlg()
{
}

void CNewCHPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_SINGLE_CHPOINT, m_nSingleChillerCount);
	DDX_Text(pDX, IDC_EDIT_NUM_CHSTART, m_nsinglestartChNum);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME_CHILLER, m_strRoomNameCH);
}



BEGIN_MESSAGE_MAP(CNewCHPointsDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewCHPointsDlg message handlers
