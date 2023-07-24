// ComponentAcumbarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComponentAcumbarDlg.h"
#include "afxdialogex.h"

#include "PointParamEditDlg.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"

// ComponentAcumbarDlg 对话框

IMPLEMENT_DYNAMIC(ComponentAcumbarDlg, CDialogEx)

ComponentAcumbarDlg::ComponentAcumbarDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlgBase(ComponentAcumbarDlg::IDD, pParent)
	, m_radio_ShowStyle(0)
	, m_edit_nMax(1000)
	, m_edit_nMin(0)
	, m_nGridRow(2)
{

}

ComponentAcumbarDlg::~ComponentAcumbarDlg()
{
}

void ComponentAcumbarDlg::RefreshControls(Json::Value jsonRoot)
{
	int nRow = m_grid_list.GetRowCount();
	for(int i=1; i<=nRow; i++)//第一行不写
	{
		m_grid_list.DeleteRow(i);
	}
	m_edit_nMax = 0;
	m_edit_nMin = 0;
	m_nGridRow = 2;
	SetGridRowCount();

	if(jsonRoot.isMember("defaultTimeRange") == true)
	{
		if (jsonRoot.get("defaultTimeRange", NULL).asString() == "today"){
			m_radio_ShowStyle = 0;
		}
		else if(jsonRoot.get("defaultTimeRange", NULL).asString() == "thismonth"){
			m_radio_ShowStyle = 1;
		}
	}

	if(jsonRoot.isMember("filter") == true)
	{
		if(jsonRoot["filter"].isMember("max") == true)
		{	
			if(jsonRoot["filter"]["max"].isInt()){
				m_edit_nMax = jsonRoot["filter"]["max"].asInt();
			}
		}
		if(jsonRoot["filter"].isMember("min") == true)
		{
			if(jsonRoot["filter"]["min"].isInt()){
				m_edit_nMin = jsonRoot["filter"]["min"].asInt();
			}
		}
	}

	//插入足够多的空行，预防pointList与header数量不统一
	int MaxRow = 0;
	if(jsonRoot.isMember("header") == true)
		MaxRow = jsonRoot["header"].size();
	if(jsonRoot.isMember("pointList") == true){
		if(MaxRow < jsonRoot["pointList"].size()){
			MaxRow = jsonRoot["pointList"].size();
		}
	}
	m_nGridRow = MaxRow;
	SetGridRowCount();

	if(jsonRoot.isMember("header") == true)
	{
		Json::Value jHeader=  jsonRoot.get("header", NULL);
		for(int i=0; i<jHeader.size(); i++){
			m_grid_list.SetItemText(i+1, 1, Project::Tools::AnsiToWideChar(jHeader[i].asString().c_str()).c_str());
		}
	}

	if(jsonRoot.isMember("pointList") == true)
	{
		Json::Value jPointList =  jsonRoot.get("pointList", NULL);
		for(int i=0; i<jPointList.size(); i++){
			m_grid_list.SetItemText(i+1, 2, Project::Tools::AnsiToWideChar(jPointList[i].asString().c_str()).c_str());
		}
	}
	UpdateData(FALSE);
}

Json::Value ComponentAcumbarDlg::GenerateJson()
{
	UpdateData(TRUE);
	Json::Value jsonPacket;

	jsonPacket["type"] = "accumbar";

	if(m_radio_ShowStyle == 0){
		jsonPacket["defaultTimeRange"] = "today";
	}
	else{
		jsonPacket["defaultTimeRange"] = "thismonth";
	}

	Json::Value jFilter;
	jFilter["max"] = m_edit_nMax;
	jFilter["min"] = m_edit_nMin;
	jsonPacket["filter"] = jFilter;

	jsonPacket["header"];
	jsonPacket["pointList"];
	for (int i=0; i<m_nGridRow; i++)
	{
		CString header = m_grid_list.GetItemText(i+1,1);
		CString point = m_grid_list.GetItemText(i+1,2);
		jsonPacket["header"].append(Project::Tools::WideCharToAnsi(header.GetString()));
		jsonPacket["pointList"].append(Project::Tools::WideCharToAnsi(point.GetString()));
	}

	return jsonPacket;
}

void ComponentAcumbarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_TPDAY, m_radio_ShowStyle);
	DDX_Text(pDX, IDC_EDIT_MAX, m_edit_nMax);
	DDX_Text(pDX, IDC_EDIT_MIN, m_edit_nMin);
	DDX_Control(pDX, IDC_GRID, m_grid_list);
	DDX_Text(pDX, IDC_EDIT_GRID_ROW, m_nGridRow);
}


BEGIN_MESSAGE_MAP(ComponentAcumbarDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SET_GRID_ROW, &ComponentAcumbarDlg::OnBnClickedButtonSetGridRow)
	ON_BN_CLICKED(IDC_BUTTON_ADD_POINT, &ComponentAcumbarDlg::OnBnClickedButtonAddPoint)
END_MESSAGE_MAP()


// ComponentAcumbarDlg 消息处理程序


BOOL ComponentAcumbarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect cr;
	m_grid_list.GetClientRect(&cr);

	m_grid_list.SetRowCount(5+1);//设置行数量
	m_grid_list.SetColumnCount(3);//设置列数量
	m_grid_list.SetFixedRowCount(1);
	m_grid_list.SetFixedColumnCount(1);//表头一行一列
	m_grid_list.SetEditable(true);//表格内容不可编辑设置
	m_grid_list.SetItemText(0, 1, _T("标题"));//设置内容
	m_grid_list.SetItemText(0, 2, _T("点名"));//设置内容

	int nRow = m_grid_list.GetRowCount();
	CString strRowNum;
	for(int i=1; i<=nRow; i++)//第一行不写
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_grid_list.SetItemText(i, 0, strRowNum);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void ComponentAcumbarDlg::OnBnClickedButtonSetGridRow()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	SetGridRowCount();
	UpdateData(FALSE);
}

void ComponentAcumbarDlg::SetGridRowCount(void)
{
	int nRow = m_nGridRow +1;
	m_grid_list.SetRowCount(nRow);

	CString strRowNum;
	for(int i=1; i<=nRow; i++)//第一行不写
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_grid_list.SetItemText(i, 0, strRowNum);
	}
}


void ComponentAcumbarDlg::OnBnClickedButtonAddPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	if(1 == m_grid_list.GetFocusCell().IsValid())
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CDataComConfigDlg dlg(pFrame->m_plcpointmap);
		if (dlg.DoModal() == IDOK)
		{
			m_grid_list.SetRedraw(FALSE);//禁止重绘画面
			m_grid_list.SetItemText(m_grid_list.GetFocusCell().row, m_grid_list.GetFocusCell().col, dlg.m_strPointName);
			m_grid_list.SetRedraw(TRUE);//允许重绘画面
		}
		UpdateData(FALSE);
	}
}
