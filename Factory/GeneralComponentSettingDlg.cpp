/*
* Copyright (c) 2020,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：CGeneralComponentSettingDlg.c
* 文件标识：
* 摘    要：“通用组件设置”窗口
*
* 当前版本： 1.1
* 作 者： Mark
* 完成日期： 2020年11月11日
*
* 取代版本：1.0 
* 原作者  ：Mark
* 完成日期：2020年2月24日
*/

#include "stdafx.h"
#include "GeneralComponentSettingDlg.h"
#include "afxdialogex.h"
#include "json/json.h"
#include "./ControlEx/WarningDlg.h"
#include "./Tools/CustomTools/CustomTools.h"

// CGeneralComponentSettingDlg 对话框

IMPLEMENT_DYNAMIC(CGeneralComponentSettingDlg, CDialogEx)

CGeneralComponentSettingDlg::CGeneralComponentSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGeneralComponentSettingDlg::IDD, pParent)
	, m_nLayer(4)
	, m_strCfgValue(_T("{\r\n}"))
{

}

CGeneralComponentSettingDlg::~CGeneralComponentSettingDlg()
{
}

void CGeneralComponentSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
	DDX_Text(pDX, IDC_EDIT_CFG_VALUE, m_strCfgValue);
	DDX_Control(pDX, IDC_COMBO_TEMPLATE, m_comboTempalte);
}


BEGIN_MESSAGE_MAP(CGeneralComponentSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGeneralComponentSettingDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLATE, &CGeneralComponentSettingDlg::OnCbnSelchangeComboTemplate)
	ON_BN_CLICKED(ID_SwitchCodeView, &CGeneralComponentSettingDlg::OnBnClickedSwitchCodeView)
END_MESSAGE_MAP()


// CGeneralComponentSettingDlg 消息处理程序


BOOL CGeneralComponentSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitControls();
	InitComponent();
	//m_strCfgValue_Backup = m_strCfgValue;
	m_nSelectedTempalte = m_comboTempalte.GetCurSel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGeneralComponentSettingDlg::InitControls(void)
{
	//图层combobox初始化
	m_comboLayer.AddString(_T("1层"));
	m_comboLayer.AddString(_T("2层"));
	m_comboLayer.AddString(_T("3层"));
	m_comboLayer.AddString(_T("4层"));
	m_comboLayer.AddString(_T("5层"));
	m_comboLayer.AddString(_T("6层"));
	m_comboLayer.AddString(_T("7层"));
	m_comboLayer.AddString(_T("8层"));
	m_comboLayer.AddString(_T("9层"));
	m_comboLayer.AddString(_T("10层"));
	m_comboLayer.SetCurSel(m_nLayer);

	int nLimit = ((CEdit*)GetDlgItem(IDC_EDIT_CFG_VALUE))->GetLimitText();
	((CEdit*)GetDlgItem(IDC_EDIT_CFG_VALUE))->SetLimitText(UINT_MAX);
	nLimit = ((CEdit*)GetDlgItem(IDC_EDIT_CFG_VALUE))->GetLimitText();
}

void CGeneralComponentSettingDlg::InitComponent(void)
{
	//组件combobox初始化
	m_comboTempalte.InsertString(None,				_T("编辑"));
	m_comboTempalte.InsertString(energy,			_T("能源管理组件"));
	m_comboTempalte.InsertString(ParamQuery,		_T("参数快速查询组件"));
	m_comboTempalte.InsertString(reportHistory,		_T("报表下载列表组件"));
	m_comboTempalte.InsertString(echarts,			_T("迷你折线图"));
	m_comboTempalte.InsertString(BigDataAnalysis,	_T("大数据相关性分析组件"));
	m_comboTempalte.InsertString(evaluationBar,		_T("评估条组件"));
	m_comboTempalte.InsertString(modelControl,		_T("模式控制自定义组件"));
	m_comboTempalte.InsertString(accumbar,			_T("叠加柱状图组件"));
	m_comboTempalte.InsertString(heatmap_table,		_T("通用表格热图组件"));

	//combo box绑定组件窗口
	m_comboTempalte.SetItemDataPtr(None,			NULL);
	m_comboTempalte.SetItemDataPtr(energy,			&m_ChildPanel_Energy);
	m_comboTempalte.SetItemDataPtr(ParamQuery,		&m_ChildPanel_ParamQuery);
	m_comboTempalte.SetItemDataPtr(reportHistory,	&m_ChildPanel_ReportHistory);
	m_comboTempalte.SetItemDataPtr(echarts,			&m_ChildPanel_Echarts);
	m_comboTempalte.SetItemDataPtr(BigDataAnalysis,	&m_ChildPanel_BigDataAnalysis);
	m_comboTempalte.SetItemDataPtr(evaluationBar,	&m_ChildPanel_EvaluationBar);
	m_comboTempalte.SetItemDataPtr(modelControl,	&m_ChildPanel_ModeControl);
	m_comboTempalte.SetItemDataPtr(accumbar,		&m_ChildPanel_Accumbar);
	m_comboTempalte.SetItemDataPtr(heatmap_table,	&m_ChildPanel_HeatmapTable);

	//获取组件将要移动的位置
	CRect rc;
	GetDlgItem(IDC_STATIC_LAYOUT)->GetWindowRect(rc); //获取被隐藏的布局底板的坐标和位置，将后续对话框都调整在相同位置上
	ScreenToClient(&rc); 

	//子窗口注册
	m_ChildPanel_Energy.Create(IDD_DIALOG_GENE_COMP_SETTING_Energy, this);
	m_ChildPanel_ParamQuery.Create(IDD_DIALOG_GENE_COMP_SETTING_ParamQuery, this);	
	m_ChildPanel_ReportHistory.Create(IDD_DIALOG_GENE_COMP_SETTING_ReportHistory, this);	
	m_ChildPanel_Echarts.Create(IDD_DIALOG_GENE_COMP_SETTING_Echarts, this);	
	m_ChildPanel_BigDataAnalysis.Create(IDD_DIALOG_GENE_COMP_SETTING_BigDataAnalysis, this);	
	m_ChildPanel_EvaluationBar.Create(IDD_DIALOG_GENE_COMP_SETTING_EvaluationBar, this);	
	m_ChildPanel_ModeControl.Create(IDD_DIALOG_GENE_COMP_SETTING_ModelControl, this);	
	m_ChildPanel_Accumbar.Create(IDD_DIALOG_GENE_COMP_SETTING_accumbar, this);
	m_ChildPanel_HeatmapTable.Create(IDD_DIALOG_GENE_COMP_SETTING_Heatmap_table, this);

	//组件窗口移动位置
	m_ChildPanel_Energy.MoveWindow(rc);
	m_ChildPanel_ParamQuery.MoveWindow(rc);
	m_ChildPanel_ReportHistory.MoveWindow(rc);
	m_ChildPanel_Echarts.MoveWindow(rc);
	m_ChildPanel_BigDataAnalysis.MoveWindow(rc);
	m_ChildPanel_EvaluationBar.MoveWindow(rc);
	m_ChildPanel_ModeControl.MoveWindow(rc);
	m_ChildPanel_Accumbar.MoveWindow(rc);
	m_ChildPanel_HeatmapTable.MoveWindow(rc);

	//根据m_strCfgValue的值来判断应该显示哪个Child或编辑窗
	Json::Value		jsonRoot;
	Json::Reader	jsonReader;
	string strResult_Ansi;
	strResult_Ansi = Project::Tools::WideCharToAnsi(m_strCfgValue.GetString());
	if(jsonReader.parse(strResult_Ansi, jsonRoot))//用jsonReader.parse来分析tye是多少
	{
		if(jsonRoot.isMember("type"))//判断key的存在
		{
			string strType;
			strType = jsonRoot["type"].asString();
			//找到匹配的窗口索引
			int nType = GetIndexInComboBoxType(strType);
			m_comboTempalte.SetCurSel(nType);//设置ComBox显示
			SwitchShowWindow(nType,jsonRoot);//选择显示的界面
			return;
		}
	}
	SwitchShowWindow(None);
}

int CGeneralComponentSettingDlg::GetIndexInComboBoxType(string& strType)
{
	int nType = 0;
	for (int i=1; i<m_comboTempalte.GetCount(); i++)
	{
		CComponentDlgBase * pBase = (CComponentDlgBase *)m_comboTempalte.GetItemDataPtr(i);
		if(pBase != NULL)
		{
			if(true == pBase->IsMe(strType))
			{
				nType = i;
				break;
			}
		}
	}
	return nType;
}

BOOL CGeneralComponentSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x80;
		switch (pMsg->wParam)
		{
		case 'a':
		case 'A':
			if (bCtrl)
			{
				CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CFG_VALUE);
				if (pEdit != NULL)
				{
					pEdit->SetFocus();
					pEdit->SetSel(0, -1);
				}
			}  
			break;
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGeneralComponentSettingDlg::SwitchShowWindow(int nCurSel, Json::Value JsonRoot)
{
	HideAllChildDlg();
	
	if(nCurSel >= m_comboTempalte.GetCount())
	{
		GetDlgItem(IDC_EDIT_CFG_VALUE)->ShowWindow(SW_SHOW);

		Json::StyledWriter style_writer;
		string strTemp = style_writer.write(JsonRoot);
		wstring wstrTemp = Project::Tools::AnsiToWideChar(strTemp.c_str());
		m_strCfgValue = wstrTemp.c_str();
		m_strCfgValue.Replace(L"\n", L"\r\n");
	}

	CComponentDlgBase * pBase = (CComponentDlgBase *)m_comboTempalte.GetItemDataPtr(nCurSel);
	if (pBase != NULL)
	{
		pBase->ShowWindow(SW_SHOW);
		pBase->RefreshControls(JsonRoot);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CFG_VALUE)->ShowWindow(SW_SHOW);

		Json::StyledWriter style_writer;
		string strTemp = style_writer.write(JsonRoot);
		wstring wstrTemp = Project::Tools::AnsiToWideChar(strTemp.c_str());
		m_strCfgValue = wstrTemp.c_str();
		m_strCfgValue.Replace(L"\n", L"\r\n");
	}
}

void CGeneralComponentSettingDlg::SwitchShowWindow(int nCurSel)
{
	HideAllChildDlg();
	CComponentDlgBase * pBase = (CComponentDlgBase *)m_comboTempalte.GetItemDataPtr(nCurSel);
	if(pBase != NULL)
	{
		pBase->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CFG_VALUE)->ShowWindow(SW_SHOW);//json编辑框
	}
}

void CGeneralComponentSettingDlg::HideAllChildDlg(void)
{
	for (int i=1; i<m_comboTempalte.GetCount(); i++)
	{
		CComponentDlgBase * pBase = (CComponentDlgBase *)m_comboTempalte.GetItemDataPtr(i);
		if(pBase != NULL)
		{
			pBase->ShowWindow(SW_HIDE);
		}
	}
	GetDlgItem(IDC_EDIT_CFG_VALUE)->ShowWindow(SW_HIDE);//json编辑框
}

void CGeneralComponentSettingDlg::OnCbnSelchangeComboTemplate()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nCurSel = m_comboTempalte.GetCurSel();
	if(m_nSelectedTempalte == nCurSel)
		return;
	m_nSelectedTempalte = nCurSel;
	SwitchShowWindow(nCurSel);
}

void CGeneralComponentSettingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	/*编辑框状态下*/
	if(IsJsonEditVisible() == true)
	{
		Json::Reader JsonReader;
		Json::Value JsonValue;
		string strJsonPack;
		strJsonPack = Project::Tools::WideCharToAnsi(m_strCfgValue.GetString());
		if(!JsonReader.parse(strJsonPack,JsonValue))
		{
			AfxMessageBox(L"Json格式有误");
			return;
		}
	}
	else//子窗口模式下
	{
		Json::Value cJsonPacket;
		int nCurSel = m_comboTempalte.GetCurSel();
		CComponentDlgBase * pBase = (CComponentDlgBase *)m_comboTempalte.GetItemDataPtr(nCurSel);
		if(pBase != NULL)
		{
			cJsonPacket = pBase->GenerateJson();
		}

		if(cJsonPacket.isNull() == false)
		{
			/*Json变量格式化打包CString*/
			string strJsonPacket;
			Json::StyledWriter style_writer;
			strJsonPacket = style_writer.write(cJsonPacket);
			wstring wstrJsonPacket;
			wstrJsonPacket = Project::Tools::AnsiToWideChar(strJsonPacket.c_str());
			m_strCfgValue = wstrJsonPacket.c_str();
		}
		else
		{
			return;
		}
	}
	UpdateData(FALSE);
	CGeneralComponentSettingDlg::OnOK();
}

void CGeneralComponentSettingDlg::OnBnClickedSwitchCodeView()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//切换脚本界面
	if (IsJsonEditVisible() == false)
	{
		Json::Value cJsonPacket;

		int nType = m_comboTempalte.GetCurSel();
		CComponentDlgBase * pBase = (CComponentDlgBase *)m_comboTempalte.GetItemDataPtr(nType);
		if(pBase != NULL)
		{
			cJsonPacket = pBase->GenerateJson();
		}
		SwitchShowWindow(None, cJsonPacket);
	}
	else//切换到配置界面
	{
		//根据m_strCfgValue的值来判断应该显示哪个Child或编辑窗
		Json::Value		jsonRoot;
		Json::Reader	jsonReader;
		string strResult_Ansi = Project::Tools::WideCharToAnsi(m_strCfgValue.GetString());
		if(jsonReader.parse(strResult_Ansi, jsonRoot))//用jsonReader.parse来分析tye是多少
		{
			if(jsonRoot.isMember("type"))//判断key的存在
			{
				string strType;
				strType = jsonRoot["type"].asString();
				int nType = GetIndexInComboBoxType(strType);
				m_comboTempalte.SetCurSel(nType);//设置ComBox显示
				SwitchShowWindow(nType, jsonRoot);//选择显示的界面
				return;
			}
		}
		//上述判断全false
		SwitchShowWindow(None);
	}
	UpdateData(FALSE);
}