// SetEqmPositionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SetEqmPositionDlg.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"

// CSetEqmPositionDlg 对话框

IMPLEMENT_DYNAMIC(CSetEqmPositionDlg, CDialog)

CSetEqmPositionDlg::CSetEqmPositionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetEqmPositionDlg::IDD, pParent)
	, m_x(0)
	, m_y(0)
	, m_width(0)
	, m_height(0)
{

}

CSetEqmPositionDlg::~CSetEqmPositionDlg()
{
}

void CSetEqmPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POSITION_X, m_x);
	DDX_Text(pDX, IDC_EDIT_POSITION_Y, m_y);
	DDX_Text(pDX, IDC_EDIT_POSITION_WIDTH, m_width);
	DDX_Text(pDX, IDC_EDIT_POSITION_HEIGHT, m_height);
}


BEGIN_MESSAGE_MAP(CSetEqmPositionDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetEqmPositionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COPY_PARAMS, &CSetEqmPositionDlg::OnBnClickedButtonCopyParams)
	ON_BN_CLICKED(IDC_BUTTON_SET_PARAMS, &CSetEqmPositionDlg::OnBnClickedButtonSetParams)
END_MESSAGE_MAP()


// CSetEqmPositionDlg 消息处理程序


void CSetEqmPositionDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(FALSE);
	CDialog::OnOK();
}


void CSetEqmPositionDlg::OnBnClickedButtonCopyParams()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(FALSE);

	CString strSys;
	GetSysPath(strSys);
	strSys += L"\\factory.ini";

	CString strx;
	CString stry;
	CString strwidth;
	CString strheight;
	strx.Format(L"%d",m_x);
	stry.Format(L"%d",m_y);
	strwidth.Format(L"%d",m_width);
	strheight.Format(L"%d",m_height);
	WritePrivateProfileString(L"USER",L"posx",strx,strSys);
	WritePrivateProfileString(L"USER",L"posy",stry,strSys);
	WritePrivateProfileString(L"USER",L"width",strwidth,strSys);
	WritePrivateProfileString(L"USER",L"height",strheight,strSys);
}


void CSetEqmPositionDlg::OnBnClickedButtonSetParams()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CString strSys;
	GetSysPath(strSys);
	strSys += L"\\factory.ini";

	wchar_t strx[MAX_PATH] = {0};
	wchar_t stry[MAX_PATH] = {0};
	wchar_t strwidth[MAX_PATH] = {0};
	wchar_t strheight[MAX_PATH] = {0};

	GetPrivateProfileString(L"USER",L"posx",L"0",strx,MAX_PATH,strSys);
	GetPrivateProfileString(L"USER",L"posy",L"0",stry,MAX_PATH,strSys);
	GetPrivateProfileString(L"USER",L"width",L"0",strwidth,MAX_PATH,strSys);
	GetPrivateProfileString(L"USER",L"height",L"0",strheight,MAX_PATH,strSys);

	m_x = _wtoi(strx);
	m_y = _wtoi(stry);
	m_width = _wtoi(strwidth);
	m_height = _wtoi(strheight);

	UpdateData(FALSE);
}
