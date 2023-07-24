// ValveSelectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MeterPanSelectDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "MainFrm.h"
#include "Tools/CustomTools/CustomTools.h"

// CValveSelectDlg �Ի���

IMPLEMENT_DYNAMIC(CMeterPanSelectDlg, CDialog)

CMeterPanSelectDlg::CMeterPanSelectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMeterPanSelectDlg::IDD, pParent),
m_pProjectSqlite(NULL)
,m_nID(-1)
,m_strMeterPan1Path(_T(""))
,m_strMeterPan2Path(_T(""))
,m_strMeterPan3Path(_T(""))
,m_nDashBoardType(0)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pProjectSqlite = pFrame->m_pPrjSqlite;

	//�Ǳ���3ͼƬ��ַ
	CString strSys;
	GetSysPath(strSys);
	m_strMeterPan1Path = strSys + L"\\Images\\meterpan_0.png";
	m_strMeterPan2Path = strSys + L"\\Images\\meterpan_1.png";
	m_strMeterPan3Path = strSys + L"\\Images\\meterpan_2.png";
	m_strMeterPan4Path = strSys + L"\\Images\\meterpan_3.png";
}

CMeterPanSelectDlg::~CMeterPanSelectDlg()
{
}

void CMeterPanSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMeterPanSelectDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CMeterPanSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CValveSelectDlg ��Ϣ�������


BOOL CMeterPanSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CButton*)GetDlgItem(IDC_RADIO_METER_PAN_1))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMeterPanSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	Gdiplus::Graphics graphics(dc.m_hDC);
	//��һ��ͼ����
	if(m_strMeterPan1Path.GetLength()>0)
	{
		Image* pBitmapMeterPan1 = Image::FromFile(m_strMeterPan1Path);
		if (pBitmapMeterPan1)
		{
			graphics.DrawImage(pBitmapMeterPan1, 92, 20, 120, 120);
		}
	}
	//�ڶ���ͼ����
	if(m_strMeterPan2Path.GetLength()>0)
	{
		Image* pBitmapMeterPan2 = Image::FromFile(m_strMeterPan2Path);
		if (pBitmapMeterPan2)
		{
			graphics.DrawImage(pBitmapMeterPan2, 245, 20, 120, 120);
		}
	}
	//������ͼ����
	if(m_strMeterPan3Path.GetLength()>0)
	{
		Image* pBitmapMeterPan3 = Image::FromFile(m_strMeterPan3Path);
		if (pBitmapMeterPan3)
		{
			graphics.DrawImage(pBitmapMeterPan3, 413, 20, 120, 120);
		}
	}
	//������ͼ����
	if(m_strMeterPan4Path.GetLength()>0)
	{
		Image* pBitmapMeterPan4 = Image::FromFile(m_strMeterPan4Path);
		if (pBitmapMeterPan4)
		{
			graphics.DrawImage(pBitmapMeterPan4, 572, 20, 120, 120);
		}
	}

	graphics.ReleaseHDC(dc.m_hDC);
}


void CMeterPanSelectDlg::OnBnClickedOk()
{
	SetEqmName();
	CDialog::OnOK();
}

void CMeterPanSelectDlg::SetEqmName()
{
	UpdateData(TRUE);
	if(((CButton*)GetDlgItem(IDC_RADIO_METER_PAN_1))->GetCheck() == 1)
	{
		m_nID = g_nPicIndexDashBoard1;
		m_nDashBoardType = 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_METER_PAN_2))->GetCheck() == 1)
	{
		m_nID = g_nPicIndexDashBoard2;
		m_nDashBoardType = 1;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_METER_PAN_3))->GetCheck() == 1)
	{
		m_nDashBoardType = 2;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_METER_PAN_4))->GetCheck() == 1)
	{
		m_nDashBoardType = 3;
	}
}
