// ChillerSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChillerSelectDlg.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "MainFrm.h"

IMPLEMENT_DYNAMIC(CChillerSelectDlg, CDialog)

CChillerSelectDlg::CChillerSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChillerSelectDlg::IDD, pParent),
	m_pProjectSqlite(NULL),
	m_nID(-1)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pProjectSqlite = pFrame->m_pPrjSqlite;
}

CChillerSelectDlg::~CChillerSelectDlg()
{

}

void CChillerSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChillerSelectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CChillerSelectDlg::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CChillerSelectDlg::SetEqmPicID()
{
	UpdateData(TRUE);
	if(((CButton*)GetDlgItem(IDC_RADIO_DEFAULT_CHILLER))->GetCheck() == 1)
	{
		m_nID = 20;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_YUEKE_LIXIN))->GetCheck() == 1)
	{
		m_nID = 21;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_YUEKE_LUOGAN))->GetCheck() == 1)
	{
		m_nID = 22;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_TELING_LIXIN))->GetCheck() == 1)
	{
		m_nID = 23;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_TELING_LUOGAN))->GetCheck() == 1)
	{
		m_nID = 24;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_KAILI_LIXIN))->GetCheck() == 1)
	{
		m_nID = 25;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_KAILI_LUOGAN))->GetCheck() == 1)
	{
		m_nID = 26;
	}
}


// CChillerSelectDlg 消息处理程序


void CChillerSelectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SetEqmPicID();
	CDialog::OnOK();
}


void CChillerSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	Gdiplus::Graphics graphics(dc.m_hDC);
	Image* pBitmapDefault = m_pProjectSqlite->GetBitmapByIDFromPictureLib(20);
	if (pBitmapDefault)
	{
		graphics.DrawImage(pBitmapDefault, 200, 8, 100, 50);
	}
	Image* pBitmapYueKeLiXin = m_pProjectSqlite->GetBitmapByIDFromPictureLib(21);
	if (pBitmapYueKeLiXin)
	{
		graphics.DrawImage(pBitmapYueKeLiXin, 200, 72, 100, 50);
	}
	Image* pBitmapYueKeLuoGan = m_pProjectSqlite->GetBitmapByIDFromPictureLib(22);
	if (pBitmapYueKeLuoGan)
	{
		graphics.DrawImage(pBitmapYueKeLuoGan, 200, 136, 100, 50);
	}
	Image* pBitmapTeLingLiXin = m_pProjectSqlite->GetBitmapByIDFromPictureLib(23);
	if (pBitmapTeLingLiXin)
	{
		graphics.DrawImage(pBitmapTeLingLiXin, 200, 208, 100, 50);
	}
	Image* pBitmapTeLingLuoGan = m_pProjectSqlite->GetBitmapByIDFromPictureLib(24);
	if (pBitmapTeLingLuoGan)
	{
		graphics.DrawImage(pBitmapTeLingLuoGan, 200, 272, 100, 50);
	}
	Image* pBitmapKaiLiLiXin = m_pProjectSqlite->GetBitmapByIDFromPictureLib(25);
	if (pBitmapKaiLiLiXin)
	{
		graphics.DrawImage(pBitmapKaiLiLiXin, 200, 338, 100, 50);
	}
	Image* pBitmapKaiLiLuoGan = m_pProjectSqlite->GetBitmapByIDFromPictureLib(26);
	if (pBitmapKaiLiLuoGan)
	{
		graphics.DrawImage(pBitmapKaiLiLuoGan, 200, 407, 100, 50);
	}
	graphics.ReleaseHDC(dc.m_hDC);
}


BOOL CChillerSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CButton*)GetDlgItem(IDC_RADIO_DEFAULT_CHILLER))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
