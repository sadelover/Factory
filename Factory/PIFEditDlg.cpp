// PIFEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PIFEditDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "MainFrm.h"
#include "DataPoint/DataPointManager.h"
#include "DataPointConfigDlg.h"
// CPIFEditDlg 对话框

IMPLEMENT_DYNAMIC(CPIFEditDlg, CSkinDlg)

CPIFEditDlg::CPIFEditDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CPIFEditDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strType(_T(""))
	, m_strRemark(_T(""))
	, m_strParam1(_T(""))
	, m_strParam2(_T(""))
	, m_strParam3(_T(""))
	, m_strParam4(_T(""))
	, m_strParam5(_T(""))
	, m_strParam6(_T(""))
	, m_strParam7(_T(""))
	, m_strParam8(_T(""))
	, m_strParam9(_T(""))
	, m_strParam10(_T(""))
	, m_strParam11(_T(""))
	, m_strParam12(_T(""))
	, m_strParam13(_T(""))
	, m_strParam14(_T(""))
	, m_strParam15(_T(""))
	, m_strParam16(_T(""))
	, m_strParam17(_T(""))
	, m_strParam18(_T(""))
	, m_strParam19(_T(""))
	, m_strParam20(_T(""))
	, m_strParam21(_T(""))
	, m_strParam22(_T(""))
	, m_strParam23(_T(""))
	, m_strParam24(_T(""))
	, m_strParam25(_T(""))
	, m_strParam26(_T(""))
	, m_strParam27(_T(""))
	, m_strParam28(_T(""))
{

}

CPIFEditDlg::~CPIFEditDlg()
{
}

void CPIFEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PIF_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PIF_REMARK, m_strRemark);
	DDX_Text(pDX, IDC_EDIT_PARAM1, m_strParam1);
	DDX_Text(pDX, IDC_EDIT_PARAM2, m_strParam2);
	DDX_Text(pDX, IDC_EDIT_PARAM3, m_strParam3);
	DDX_Text(pDX, IDC_EDIT_PARAM4, m_strParam4);
	DDX_Text(pDX, IDC_EDIT_PARAM5, m_strParam5);
	DDX_Text(pDX, IDC_EDIT_PARAM6, m_strParam6);
	DDX_Text(pDX, IDC_EDIT_PARAM7, m_strParam7);
	DDX_Text(pDX, IDC_EDIT_PARAM8, m_strParam8);
	DDX_Text(pDX, IDC_EDIT_PARAM9, m_strParam9);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_combo_type);
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_strType);
	DDX_Text(pDX, IDC_EDIT_PARAM10, m_strParam10);
	DDX_Text(pDX, IDC_EDIT_PARAM11, m_strParam11);
	DDX_Text(pDX, IDC_EDIT_PARAM12, m_strParam12);
	DDX_Text(pDX, IDC_EDIT_PARAM13, m_strParam13);
	DDX_Text(pDX, IDC_EDIT_PARAM14, m_strParam14);
	DDX_Text(pDX, IDC_EDIT_PARAM15, m_strParam15);
	DDX_Text(pDX, IDC_EDIT_PARAM16, m_strParam16);
	DDX_Text(pDX, IDC_EDIT_PARAM17, m_strParam17);
	DDX_Text(pDX, IDC_EDIT_PARAM18, m_strParam18);
	DDX_Text(pDX, IDC_EDIT_PARAM19, m_strParam19);
	DDX_Text(pDX, IDC_EDIT_PARAM20, m_strParam20);
	DDX_Text(pDX, IDC_EDIT_PARAM21, m_strParam21);
	DDX_Text(pDX, IDC_EDIT_PARAM22, m_strParam22);
	DDX_Text(pDX, IDC_EDIT_PARAM23, m_strParam23);
	DDX_Text(pDX, IDC_EDIT_PARAM24, m_strParam24);
	DDX_Text(pDX, IDC_EDIT_PARAM25, m_strParam25);
	DDX_Text(pDX, IDC_EDIT_PARAM26, m_strParam26);
	DDX_Text(pDX, IDC_EDIT_PARAM27, m_strParam27);
	DDX_Text(pDX, IDC_EDIT_PARAM28, m_strParam28);
}


BEGIN_MESSAGE_MAP(CPIFEditDlg, CSkinDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PARAM1, &CPIFEditDlg::OnBnClickedButtonParam1)
	ON_BN_CLICKED(IDC_BUTTON_PARAM2, &CPIFEditDlg::OnBnClickedButtonParam2)
	ON_BN_CLICKED(IDC_BUTTON_PARAM3, &CPIFEditDlg::OnBnClickedButtonParam3)
	ON_BN_CLICKED(IDC_BUTTON_PARAM4, &CPIFEditDlg::OnBnClickedButtonParam4)
	ON_BN_CLICKED(IDC_BUTTON_PARAM5, &CPIFEditDlg::OnBnClickedButtonParam5)
	ON_BN_CLICKED(IDC_BUTTON_PARAM6, &CPIFEditDlg::OnBnClickedButtonParam6)
	ON_BN_CLICKED(IDC_BUTTON_PARAM7, &CPIFEditDlg::OnBnClickedButtonParam7)
	ON_BN_CLICKED(IDC_BUTTON_PARAM8, &CPIFEditDlg::OnBnClickedButtonParam8)
	ON_BN_CLICKED(IDC_BUTTON_PARAM9, &CPIFEditDlg::OnBnClickedButtonParam9)
	ON_BN_CLICKED(IDOK, &CPIFEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PARAM10, &CPIFEditDlg::OnBnClickedButtonParam10)
	ON_BN_CLICKED(IDC_BUTTON_PARAM11, &CPIFEditDlg::OnBnClickedButtonParam11)
	ON_BN_CLICKED(IDC_BUTTON_PARAM12, &CPIFEditDlg::OnBnClickedButtonParam12)
	ON_BN_CLICKED(IDC_BUTTON_PARAM13, &CPIFEditDlg::OnBnClickedButtonParam13)
	ON_BN_CLICKED(IDC_BUTTON_PARAM14, &CPIFEditDlg::OnBnClickedButtonParam14)
	ON_BN_CLICKED(IDC_BUTTON_PARAM15, &CPIFEditDlg::OnBnClickedButtonParam15)
	ON_BN_CLICKED(IDC_BUTTON_PARAM16, &CPIFEditDlg::OnBnClickedButtonParam16)
	ON_BN_CLICKED(IDC_BUTTON_PARAM17, &CPIFEditDlg::OnBnClickedButtonParam17)
	ON_BN_CLICKED(IDC_BUTTON_PARAM18, &CPIFEditDlg::OnBnClickedButtonParam18)
	ON_BN_CLICKED(IDC_BUTTON_PARAM19, &CPIFEditDlg::OnBnClickedButtonParam19)
	ON_BN_CLICKED(IDC_BUTTON_PARAM20, &CPIFEditDlg::OnBnClickedButtonParam20)
	ON_BN_CLICKED(IDC_BUTTON_PARAM21, &CPIFEditDlg::OnBnClickedButtonParam21)
	ON_BN_CLICKED(IDC_BUTTON_PARAM22, &CPIFEditDlg::OnBnClickedButtonParam22)
	ON_BN_CLICKED(IDC_BUTTON_PARAM23, &CPIFEditDlg::OnBnClickedButtonParam23)
	ON_BN_CLICKED(IDC_BUTTON_PARAM24, &CPIFEditDlg::OnBnClickedButtonParam24)
	ON_BN_CLICKED(IDC_BUTTON_PARAM25, &CPIFEditDlg::OnBnClickedButtonParam25)
	ON_BN_CLICKED(IDC_BUTTON_PARAM26, &CPIFEditDlg::OnBnClickedButtonParam26)
	ON_BN_CLICKED(IDC_BUTTON_PARAM27, &CPIFEditDlg::OnBnClickedButtonParam27)
	ON_BN_CLICKED(IDC_BUTTON_PARAM28, &CPIFEditDlg::OnBnClickedButtonParam28)
	ON_EN_CHANGE(IDC_EDIT_PIF_NAME, &CPIFEditDlg::OnEnChangeEditPifName)
	ON_EN_CHANGE(IDC_EDIT_PIF_REMARK, &CPIFEditDlg::OnEnChangeEditPifRemark)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CPIFEditDlg::OnCbnSelchangeComboType)
END_MESSAGE_MAP()


// CPIFEditDlg 消息处理程序


void CPIFEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CSkinDlg::OnPaint()
	dc.SetBkMode(TRANSPARENT);
	CRect rect;
	GetClientRect(rect);
	CBrush brush(RGB(255,255,255));
	rect.top += 40;
	rect.bottom -= 45;
	rect.left += 1;
	rect.right -= 5;
	dc.FillRect(&rect,&brush);
}


BOOL CPIFEditDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//移动按钮
	{
		{
			CRect rectBtn;
			GetDlgItem(IDOK)->GetWindowRect(&rectBtn);
			rectBtn.top	+= 15;
			rectBtn.bottom	+= 15;
			ScreenToClient(&rectBtn);
			GetDlgItem(IDOK)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
		}
		{
			CRect rectBtn;
			GetDlgItem(IDCANCEL)->GetWindowRect(&rectBtn);
			rectBtn.top	+= 15;
			rectBtn.bottom	+= 15;
			ScreenToClient(&rectBtn);
			GetDlgItem(IDCANCEL)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
		}
	}
	InitTypeCombo();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPIFEditDlg::OnBnClickedButtonParam1()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam1 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam2 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam3()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam3 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam4()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam4 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam5()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam5 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam6()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam6 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam7()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam7 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam8()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam8 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam9()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam9 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}

void CPIFEditDlg::OnBnClickedButtonParam10()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam10 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam11()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam11 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam12()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam12 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam13()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam13 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}

void CPIFEditDlg::OnBnClickedButtonParam14()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam14 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam15()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam15 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam16()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam16 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam17()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam17 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam18()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam18 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam19()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam19 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam20()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam20 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam21()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam21 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam22()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam22 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam23()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam23 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam24()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam24 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam25()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam25 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam26()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam26 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam27()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam27 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPIFEditDlg::OnBnClickedButtonParam28()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	if (datalist.size() == 0)
	{
		if (AfxMessageBox(L"请先加载点表!") == IDOK)
		{
			CDataPointConfigDlg dlg(point_manager);
			dlg.DoModal();
		}
	}
	CDataComConfigDlg dlg(point_manager);
	if (dlg.DoModal() == IDOK)
	{
		m_strParam28 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}

void CPIFEditDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (AfxMessageBox(L"确定编辑完成了吗?", MB_OKCANCEL) == IDOK)
	{
		UpdateData();
	}
	CSkinDlg::OnOK();
}

void CPIFEditDlg::InitTypeCombo()
{
	m_combo_type.AddString(L"");
	m_combo_type.AddString(L"PIF");
}


void CPIFEditDlg::OnEnChangeEditPifName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CSkinDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(FALSE);
}


void CPIFEditDlg::OnEnChangeEditPifRemark()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CSkinDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(FALSE);
}


void CPIFEditDlg::OnCbnSelchangeComboType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(FALSE);
}
