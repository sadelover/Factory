// ValveSelectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ValveSelectDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "MainFrm.h"

// CValveSelectDlg �Ի���

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


// CValveSelectDlg ��Ϣ�������


BOOL CValveSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CButton*)GetDlgItem(IDC_RADIO_VALVE_SWITCH))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CValveSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
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
