// ValveSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ValveSelectDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "MainFrm.h"

// CValveSelectDlg 对话框

IMPLEMENT_DYNAMIC(CValveSelectDlg, CDialog)

CValveSelectDlg::CValveSelectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CValveSelectDlg::IDD, pParent),
m_pProjectSqlite(NULL)
,m_nID(-1)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pProjectSqlite = pFrame->m_pPrjSqlite;
}

CValveSelectDlg::~CValveSelectDlg()
{
}

void CValveSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CValveSelectDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CValveSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CValveSelectDlg 消息处理程序


BOOL CValveSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CButton*)GetDlgItem(IDC_RADIO_VALVE_SWITCH))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CValveSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	Gdiplus::Graphics graphics(dc.m_hDC);
	Image* pBitmapValveSwitch = m_pProjectSqlite->GetBitmapByIDFromPictureLib(29);
	if (pBitmapValveSwitch)
	{
		graphics.DrawImage(pBitmapValveSwitch, 180, 15, 64, 64);
	}
	Image* pBitmapValveDigit = m_pProjectSqlite->GetBitmapByIDFromPictureLib(30);
	if (pBitmapValveDigit)
	{
		graphics.DrawImage(pBitmapValveDigit, 180, 120, 64, 64);
	}
	Image* pBitmapValve3 = m_pProjectSqlite->GetBitmapByIDFromPictureLib(31);
	if (pBitmapValve3)
	{
		graphics.DrawImage(pBitmapValve3, 180, 220, 64, 64);
	}
	graphics.ReleaseHDC(dc.m_hDC);
}


void CValveSelectDlg::OnBnClickedOk()
{
	SetEqmName();
	CDialog::OnOK();
}

void CValveSelectDlg::SetEqmName()
{
	UpdateData(TRUE);
	if(((CButton*)GetDlgItem(IDC_RADIO_VALVE_SWITCH))->GetCheck() == 1)
	{
		m_nID =29;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_VALVE_DIGIT))->GetCheck() == 1)
	{
		m_nID = 30;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_VALVE_3))->GetCheck() == 1)
	{
		m_nID = 31;
	}
}
