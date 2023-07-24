// ValveSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterPanSelectDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "MainFrm.h"
#include "Tools/CustomTools/CustomTools.h"

// CValveSelectDlg 对话框

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

	//仪表盘3图片地址
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


// CValveSelectDlg 消息处理程序


BOOL CMeterPanSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CButton*)GetDlgItem(IDC_RADIO_METER_PAN_1))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMeterPanSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	Gdiplus::Graphics graphics(dc.m_hDC);
	//第一张图绘制
	if(m_strMeterPan1Path.GetLength()>0)
	{
		Image* pBitmapMeterPan1 = Image::FromFile(m_strMeterPan1Path);
		if (pBitmapMeterPan1)
		{
			graphics.DrawImage(pBitmapMeterPan1, 92, 20, 120, 120);
		}
	}
	//第二张图绘制
	if(m_strMeterPan2Path.GetLength()>0)
	{
		Image* pBitmapMeterPan2 = Image::FromFile(m_strMeterPan2Path);
		if (pBitmapMeterPan2)
		{
			graphics.DrawImage(pBitmapMeterPan2, 245, 20, 120, 120);
		}
	}
	//第三张图绘制
	if(m_strMeterPan3Path.GetLength()>0)
	{
		Image* pBitmapMeterPan3 = Image::FromFile(m_strMeterPan3Path);
		if (pBitmapMeterPan3)
		{
			graphics.DrawImage(pBitmapMeterPan3, 413, 20, 120, 120);
		}
	}
	//第四张图绘制
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
