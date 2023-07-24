// CurveTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CurveTabDlg.h"
#include "afxdialogex.h"
#include "BEOPGatewayDlg.h"
#include "PageDlg.h"
#include <sstream>
#include "Tools/warning/MsgDefine.h"
#include "ControlEx/WarningDlg.h"
#include "dlgload.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"

// CCurveTabDlg dialog

IMPLEMENT_DYNAMIC(CCurveTabDlg, CDialog)

CCurveTabDlg::CCurveTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurveTabDlg::IDD, pParent)
{

}

CCurveTabDlg::~CCurveTabDlg()
{
}

void CCurveTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB3, m_CurveTab);
}


BEGIN_MESSAGE_MAP(CCurveTabDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB3, &CCurveTabDlg::OnTcnSelchangeTab3)
END_MESSAGE_MAP()


// CCurveTabDlg message handlers

BOOL CCurveTabDlg ::OnInitDialog()
{
	CDialog::OnInitDialog();
	//设置页面名称;

	m_CurveTab.InsertItem(0,_T("  实时曲线1  "));
	m_CurveTab.InsertItem(1,_T("  实时曲线2  "));
	m_CurveTab.InsertItem(2,_T("  实时曲线3  "));
	m_CurveTab.InsertItem(3,_T("  实时曲线4  "));
	m_CurveTab.InsertItem(4,_T("  实时曲线5  "));
	//绑定页面;
	m_CurvePage1.Create(IDD_DIALOG_REALTIME,GetDlgItem(IDC_TAB3));
	m_CurvePage2.Create(IDD_DIALOG_REALTIME,GetDlgItem(IDC_TAB3));
	m_CurvePage3.Create(IDD_DIALOG_REALTIME,GetDlgItem(IDC_TAB3));
	m_CurvePage4.Create(IDD_DIALOG_REALTIME,GetDlgItem(IDC_TAB3));
	m_CurvePage5.Create(IDD_DIALOG_REALTIME,GetDlgItem(IDC_TAB3));
	//设置页面大小;
	CRect rect;
	m_CurveTab.GetClientRect(&rect);
	rect.top+=28;
	rect.bottom-=2;
	rect.left+=2;
	rect.right-=2;
	//设置各页面属性;
	m_CurvePage1.MoveWindow(&rect);
	m_CurvePage2.MoveWindow(&rect);
	m_CurvePage3.MoveWindow(&rect);
	m_CurvePage4.MoveWindow(&rect);
	m_CurvePage5.MoveWindow(&rect);
	//设置初始页面显示属性;
	m_CurvePage1.ShowWindow(TRUE);
	m_CurvePage2.ShowWindow(FALSE);
	m_CurvePage3.ShowWindow(FALSE);
	m_CurvePage4.ShowWindow(FALSE);
	m_CurvePage5.ShowWindow(FALSE);
	
	//初始化页面ID;
	m_CurvePage1.SetDlgID(0);
	m_CurvePage2.SetDlgID(1);
	m_CurvePage3.SetDlgID(2);
	m_CurvePage4.SetDlgID(3);
	m_CurvePage5.SetDlgID(4);

	m_CurveTab.SetCurSel(0);
	return TRUE;
}



void CCurveTabDlg::OnTcnSelchangeTab3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nCurSel;
	nCurSel = m_CurveTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		m_CurvePage1.ShowWindow(TRUE);
		m_CurvePage2.ShowWindow(FALSE);
		m_CurvePage3.ShowWindow(FALSE);
		m_CurvePage4.ShowWindow(FALSE);
		m_CurvePage5.ShowWindow(FALSE);
		break;
	case 1:
		m_CurvePage1.ShowWindow(FALSE);
		m_CurvePage2.ShowWindow(TRUE);
		m_CurvePage3.ShowWindow(FALSE);
		m_CurvePage4.ShowWindow(FALSE);
		m_CurvePage5.ShowWindow(FALSE);
		break;
	case 2:
		m_CurvePage1.ShowWindow(FALSE);
		m_CurvePage2.ShowWindow(FALSE);
		m_CurvePage3.ShowWindow(TRUE);
		m_CurvePage4.ShowWindow(FALSE);
		m_CurvePage5.ShowWindow(FALSE);
		break;
	case 3:
		m_CurvePage1.ShowWindow(FALSE);
		m_CurvePage2.ShowWindow(FALSE);
		m_CurvePage3.ShowWindow(FALSE);
		m_CurvePage4.ShowWindow(TRUE);
		m_CurvePage5.ShowWindow(FALSE);
		break;
	case 4:
		m_CurvePage1.ShowWindow(FALSE);
		m_CurvePage2.ShowWindow(FALSE);
		m_CurvePage3.ShowWindow(FALSE);
		m_CurvePage4.ShowWindow(FALSE);
		m_CurvePage5.ShowWindow(TRUE);
		break;
	default:
		break;
	}
	*pResult = 0;
}
void CCurveTabDlg::InitData()
{
	//指针传递;
	m_CurvePage1.m_pServerDataAccess = m_pServerDataAccess;
	m_CurvePage1.m_pBEOPGatewayDlg = m_pBEOPGatewayDlg;
	m_CurvePage2.m_pServerDataAccess = m_pServerDataAccess;
	m_CurvePage2.m_pBEOPGatewayDlg = m_pBEOPGatewayDlg;
	m_CurvePage3.m_pServerDataAccess = m_pServerDataAccess;
	m_CurvePage3.m_pBEOPGatewayDlg = m_pBEOPGatewayDlg;
	m_CurvePage4.m_pServerDataAccess = m_pServerDataAccess;
	m_CurvePage4.m_pBEOPGatewayDlg = m_pBEOPGatewayDlg;
	m_CurvePage5.m_pServerDataAccess = m_pServerDataAccess;
	m_CurvePage5.m_pBEOPGatewayDlg = m_pBEOPGatewayDlg;
	//初始化按钮数据;
	m_CurvePage1.InitButtonData(0);
	m_CurvePage2.InitButtonData(1);
	m_CurvePage3.InitButtonData(2);
	m_CurvePage4.InitButtonData(3);
	m_CurvePage5.InitButtonData(4);
	//m_CurvePage1.m_iniWatchPointList = m_iniWatchPointList;
	//m_CurvePage1.InitPointView();
}
void CCurveTabDlg::Exit()
{
	m_CurvePage1.Exit();
	m_CurvePage2.Exit();
	m_CurvePage3.Exit();
	m_CurvePage4.Exit();
	m_CurvePage5.Exit();
}
void CCurveTabDlg::DataPointVector(std::vector<wstring> iniWatchPointList)
{
	m_CurvePage1.m_iniWatchPointList = iniWatchPointList;
}
void CCurveTabDlg::CurvePageInitPointView()
{
	m_CurvePage1.InitPointView();
	m_CurvePage2.InitPointView();
	m_CurvePage3.InitPointView();
	m_CurvePage4.InitPointView();
	m_CurvePage5.InitPointView();
}