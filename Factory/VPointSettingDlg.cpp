// VPointSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VPointSettingDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "MainFrm.h"
#include "DataPoint/DataPointEntry.h"
#include "DataPoint/DataPointManager.h"

// CVPointSettingDlg 对话框

IMPLEMENT_DYNAMIC(CVPointSettingDlg, CSkinDlg)

CVPointSettingDlg::CVPointSettingDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CVPointSettingDlg::IDD, pParent)
	, m_strName(_T("Stream"))
	, m_strParam1(_T("LogicAnd"))
	, m_strParam2(_T(""))
	, m_strParam3(_T(""))
	, m_strParam4(_T(""))
	, m_strParam5(_T(""))
	, m_strParam6(_T(""))
	, m_strParam7(_T(""))
	, m_strParam8(_T(""))
	, m_strParam9(_T(""))
	, m_strParam10(_T(""))
	, m_strHighAlarm(_T("-9999"))
	, m_strHighHighAlarm(_T("-9999"))
	, m_strLowAlarm(_T("-9999"))
	, m_strLowLowAlarm(_T("-9999"))
{

}

CVPointSettingDlg::~CVPointSettingDlg()
{
}

void CVPointSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VPOINT_NAME, m_strName);
	DDX_CBString(pDX, IDC_COMBO_RELATION, m_strParam1);
	DDX_Text(pDX, IDC_EDIT_VPOINT_PARAM2, m_strParam2);
	DDX_Text(pDX, IDC_EDIT_VPOINT_PARAM3, m_strParam3);
	DDX_Text(pDX, IDC_EDIT_VPOINT_PARAM4, m_strParam4);
	DDX_Text(pDX, IDC_EDIT_VPOINT_PARAM5, m_strParam5);
	DDX_Text(pDX, IDC_EDIT_VPOINT_PARAM6, m_strParam6);
	DDX_Text(pDX, IDC_EDIT_VPOINT_PARAM7, m_strParam7);
	DDX_Text(pDX, IDC_EDIT_VPOINT_PARAM8, m_strParam8);
	DDX_Text(pDX, IDC_EDIT_VPOINT_PARAM9, m_strParam9);
	DDX_Text(pDX, IDC_EDIT_VPOINT_PARAM10, m_strParam10);
	DDX_Control(pDX, IDC_COMBO_RELATION, m_combo_param1);
	DDX_Text(pDX, IDC_EDIT_HIGH_ALARM_V, m_strHighAlarm);
	DDX_Text(pDX, IDC_EDIT_HIGHHIGH_ALARM_V, m_strHighHighAlarm);
	DDX_Text(pDX, IDC_EDIT_LOW_ALARM_V, m_strLowAlarm);
	DDX_Text(pDX, IDC_EDIT_LOWLOW_ALARM_V, m_strLowLowAlarm);
}


BEGIN_MESSAGE_MAP(CVPointSettingDlg, CSkinDlg)
	ON_BN_CLICKED(IDC_BUTTON_PARAM2, &CVPointSettingDlg::OnBnClickedButtonParam2)
	ON_BN_CLICKED(IDC_BUTTON_PARAM3, &CVPointSettingDlg::OnBnClickedButtonParam3)
	ON_BN_CLICKED(IDC_BUTTON_PARAM4, &CVPointSettingDlg::OnBnClickedButtonParam4)
	ON_BN_CLICKED(IDC_BUTTON_PARAM5, &CVPointSettingDlg::OnBnClickedButtonParam5)
	ON_BN_CLICKED(IDC_BUTTON_PARAM6, &CVPointSettingDlg::OnBnClickedButtonParam6)
	ON_BN_CLICKED(IDC_BUTTON_PARAM7, &CVPointSettingDlg::OnBnClickedButtonParam7)
	ON_BN_CLICKED(IDC_BUTTON_PARAM8, &CVPointSettingDlg::OnBnClickedButtonParam8)
	ON_BN_CLICKED(IDC_BUTTON_PARAM9, &CVPointSettingDlg::OnBnClickedButtonParam9)
	ON_BN_CLICKED(IDC_BUTTON_PARAM10, &CVPointSettingDlg::OnBnClickedButtonParam10)
	ON_BN_CLICKED(IDOK, &CVPointSettingDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_VPOINT_NAME, &CVPointSettingDlg::OnEnChangeEditVpointName)
	ON_CBN_SELCHANGE(IDC_COMBO_RELATION, &CVPointSettingDlg::OnCbnSelchangeComboRelation)
END_MESSAGE_MAP()


// CVPointSettingDlg 消息处理程序


void CVPointSettingDlg::OnBnClickedButtonParam2()
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


void CVPointSettingDlg::OnBnClickedButtonParam3()
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


void CVPointSettingDlg::OnBnClickedButtonParam4()
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


void CVPointSettingDlg::OnBnClickedButtonParam5()
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


void CVPointSettingDlg::OnBnClickedButtonParam6()
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


void CVPointSettingDlg::OnBnClickedButtonParam7()
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


void CVPointSettingDlg::OnBnClickedButtonParam8()
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


void CVPointSettingDlg::OnBnClickedButtonParam9()
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


void CVPointSettingDlg::OnBnClickedButtonParam10()
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


BOOL CVPointSettingDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	InitParam1Combo();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CVPointSettingDlg::InitParam1Combo()
{
	m_combo_param1.AddString(L"LogicAnd");
	m_combo_param1.AddString(L"LogicOr");
	m_combo_param1.AddString(L"LogicXor");
	m_combo_param1.AddString(L"RelationOP");
}

void CVPointSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (AfxMessageBox(L"确定编辑完成了吗?", MB_OKCANCEL) == IDOK)
	{
		UpdateData();
		if (IsNameValid(m_strName))
		{
			CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
			ASSERT(pMainWnd);
			CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
			map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
			DataPointEntry entry;
			entry.SetPointIndex(datalist.size()+1);
			entry.SetShortName(m_strName.GetString());
			entry.SetSourceType(L"vpoint");
			entry.SetDescription(L"");
			entry.SetUnit(L"");
			entry.SetProperty(PLC_PROP_MAX);
			entry.SetParam(1, m_strParam1.GetString());
			entry.SetParam(2, m_strParam2.GetString());
			entry.SetParam(3, m_strParam3.GetString());
			entry.SetParam(4, m_strParam4.GetString());
			entry.SetParam(5, m_strParam5.GetString());
			entry.SetParam(6, m_strParam6.GetString());
			entry.SetParam(7, m_strParam7.GetString());
			entry.SetParam(8, m_strParam8.GetString());
			entry.SetParam(9, m_strParam9.GetString());
			entry.SetParam(10, m_strParam10.GetString());
			if (m_strHighAlarm.GetLength()>0)
			{
				entry.SetHighAlarm(_wtof(m_strHighAlarm.GetString()));
			}
			if (m_strHighHighAlarm.GetLength()>0)
			{
				entry.SetHighHighAlarm(_wtof(m_strHighHighAlarm.GetString()));
			}
			if (m_strLowAlarm.GetLength()>0)
			{
				entry.SetLowAlarm(_wtof(m_strLowAlarm.GetString()));
			}
			if (m_strLowLowAlarm.GetLength()>0)
			{
				entry.SetLowLowAlarm(_wtof(m_strLowLowAlarm.GetString()));
			}
			point_manager.AddEntry(entry);
		}
		else
		{
			AfxMessageBox(L"名称有重复,请更改!");
			return;
		}
	}
	CSkinDlg::OnOK();
}

bool CVPointSettingDlg::IsNameValid( const CString strName )
{
	CMainFrame* pMainWnd = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	ASSERT(pMainWnd);
	CDataPointManager& point_manager = pMainWnd->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();
	map<int, DataPointEntry>::const_iterator it = datalist.begin();
	for (;it != datalist.end(); it++)
	{
		const DataPointEntry& entryMap = it->second;
		if (entryMap.GetShortName() == strName.GetString())
		{
			return false;
		}
	}
	return true;
}


void CVPointSettingDlg::OnEnChangeEditVpointName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CSkinDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(FALSE);
}


void CVPointSettingDlg::OnCbnSelchangeComboRelation()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(FALSE);
}
