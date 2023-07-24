// ComponenthHeatmapTableDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComponenthHeatmapTableDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "DataComConfigDlg.h"
// CComponenthHeatmapTableDlg �Ի���

IMPLEMENT_DYNAMIC(CComponenthHeatmapTableDlg, CDialogEx)

CComponenthHeatmapTableDlg::CComponenthHeatmapTableDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlgBase(CComponenthHeatmapTableDlg::IDD, pParent)
	, m_nXCount(DEFAULT_NUMBER)
	, m_nYCount(DEFAULT_NUMBER)
	, m_pointName(_T(""))
	, m_nColorValueStart(0)
	, m_nColorValueEnd(0)
{

}

CComponenthHeatmapTableDlg::~CComponenthHeatmapTableDlg()
{
}

void CComponenthHeatmapTableDlg::RefreshControls(Json::Value jsonRoot)
{
	//�������
	for(int i=1;i<m_grid_x.GetColumnCount(); i++)
	{
		m_grid_x.DeleteColumn(i);
	}
	for(int i=1;i<m_grid_y.GetColumnCount(); i++)
	{
		m_grid_y.DeleteColumn(i);
	}
	m_grid_x.SetColumnCount(DEFAULT_NUMBER+1);
	m_grid_y.SetColumnCount(DEFAULT_NUMBER+1);
	OnBnClickedButtonSetXCount();
	OnBnClickedButtonSetYCount();

	if(true == jsonRoot.isMember("point"))
	{
		if(true == jsonRoot["point"].isString())
		{
			m_pointName = Project::Tools::AnsiToWideChar(jsonRoot["point"].asString().c_str()).c_str();
		}
	}

	Json::Value & jxAxis = jsonRoot.get("xAxis", NULL);
	if(jxAxis != NULL)
	{
		if(m_grid_x.GetColumnCount() < jxAxis.size()+1){
			m_grid_x.SetColumnCount(jxAxis.size()+1);
			m_nXCount = jxAxis.size();
			OnBnClickedButtonSetXCount();
		}

		for(int i = 0; i< jxAxis.size();i++)
		{
			m_grid_x.SetItemText(1, i+1, Project::Tools::AnsiToWideChar(jxAxis[i].asString().c_str()).c_str());
		}
	}
	
	Json::Value & jyAxis = jsonRoot.get("yAxis", NULL);
	if(jyAxis != NULL)
	{
		if(m_grid_y.GetColumnCount() < jyAxis.size()+1){
			m_grid_y.SetColumnCount(jyAxis.size()+1);
			m_nYCount = jyAxis.size();
			OnBnClickedButtonSetYCount();
		}

		for(int i = 0; i< jyAxis.size();i++)
		{
			m_grid_y.SetItemText(1, i+1, Project::Tools::AnsiToWideChar(jyAxis[i].asString().c_str()).c_str());
		}
	}

	if(true == jsonRoot.isMember("valueRange"))
	{
		Json::Value& jStart = jsonRoot["valueRange"].get("colorValueStart", NULL);
		Json::Value& jEnd = jsonRoot["valueRange"].get("colorValueEnd", NULL);
		if(jStart != NULL){
			if(true == jStart.isInt()){
				m_nColorValueStart = jStart.asInt();
			}
		}
		if(jEnd != NULL){
			if(true == jEnd.isInt()){
				m_nColorValueEnd = jEnd.asInt();
			}
		}
	}
	UpdateData(FALSE);
}

Json::Value CComponenthHeatmapTableDlg::GenerateJson()
{
	Json::Value jsonPacket;
	UpdateData(TRUE);
	jsonPacket["type"] ="heatmap-table";
	jsonPacket["point"] = Project::Tools::WideCharToAnsi(m_pointName.GetString()).c_str();
	jsonPacket["xAxis"];
	jsonPacket["yAxis"];
	jsonPacket["valueRange"];

	for(int i=1; i<m_grid_x.GetColumnCount(); i++)
	{
		CString temp = m_grid_x.GetItemText(1, i);
		if(temp.GetLength() > 0)
			jsonPacket["xAxis"].append( Project::Tools::WideCharToAnsi(temp.GetString()).c_str());
	}

	for(int i=1; i<m_grid_y.GetColumnCount(); i++)
	{
		CString temp = m_grid_y.GetItemText(1, i);
		if(temp.GetLength() > 0)
			jsonPacket["yAxis"].append( Project::Tools::WideCharToAnsi(temp.GetString()).c_str());
	}
	
	jsonPacket["valueRange"]["colorValueStart"]	= m_nColorValueStart;
	jsonPacket["valueRange"]["colorValueEnd"]	= m_nColorValueEnd;

	return jsonPacket;
}

void CComponenthHeatmapTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_X, m_grid_x);
	DDX_Control(pDX, IDC_GRID_Y, m_grid_y);
	DDX_Text(pDX, IDC_EDIT_X_COUNT, m_nXCount);
	DDX_Text(pDX, IDC_EDIT_Y_COUNT, m_nYCount);
	DDX_Text(pDX, IDC_EDIT_POINT, m_pointName);
	DDX_Text(pDX, IDC_EDIT_START, m_nColorValueStart);
	DDX_Text(pDX, IDC_EDIT_END, m_nColorValueEnd);
}


BEGIN_MESSAGE_MAP(CComponenthHeatmapTableDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SET_X_COUNT, &CComponenthHeatmapTableDlg::OnBnClickedButtonSetXCount)
	ON_BN_CLICKED(IDC_BUTTON_SET_Y_COUNT, &CComponenthHeatmapTableDlg::OnBnClickedButtonSetYCount)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_POINT, &CComponenthHeatmapTableDlg::OnBnClickedButtonInsertPoint)
END_MESSAGE_MAP()


// CComponenthHeatmapTableDlg ��Ϣ�������


BOOL CComponenthHeatmapTableDlg::OnInitDialog()
{
	CComponentDlgBase::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_grid_x.SetDefCellWidth(50);
	m_grid_x.SetRowCount(2);//����������
	m_grid_x.SetColumnCount(DEFAULT_NUMBER+1);//����������
	m_grid_x.SetFixedRowCount(1);
	m_grid_x.SetFixedColumnCount(1);//��ͷһ��һ��
	m_grid_x.SetEditable(true);//������ݲ��ɱ༭����
	m_grid_x.SetItemText(0, 0, _T(" "));//��������
	m_grid_x.SetItemText(1, 0, _T("����"));//��������
	m_grid_x.SetColumnWidth(0,40);

	m_grid_y.SetDefCellWidth(50);
	m_grid_y.SetRowCount(2);//����������
	m_grid_y.SetColumnCount(DEFAULT_NUMBER+1);//����������
	m_grid_y.SetFixedRowCount(1);
	m_grid_y.SetFixedColumnCount(1);//��ͷһ��һ��
	m_grid_y.SetEditable(true);//������ݲ��ɱ༭����
	m_grid_y.SetItemText(0, 0, _T(" "));//��������
	m_grid_y.SetItemText(1, 0, _T("����"));//��������
	m_grid_y.SetColumnWidth(0,40);

	CString strRowNum;
	for(int i=1; i<m_grid_x.GetColumnCount(); i++)
	{
		strRowNum.Empty();
		strRowNum.Format(_T("%d"),i);
		m_grid_x.SetItemText(0, i, strRowNum);
	}
	for(int i=1; i<m_grid_y.GetColumnCount(); i++)
	{
		strRowNum.Empty();
		strRowNum.Format(_T("%d"),i);
		m_grid_y.SetItemText(0, i, strRowNum);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CComponenthHeatmapTableDlg::OnBnClickedButtonSetXCount()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_nXCount <= 0)
		return;
	m_grid_x.SetColumnCount(m_nXCount+1);
	CString strRowNum;
	for(int i=1; i<=m_grid_x.GetColumnCount(); i++)
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_grid_x.SetItemText(0, i, strRowNum);
	}
}


void CComponenthHeatmapTableDlg::OnBnClickedButtonSetYCount()
{
	UpdateData(TRUE);
	if(m_nYCount <= 0)
		return;
	m_grid_y.SetColumnCount(m_nYCount+1);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strRowNum;
	for(int i=1; i<=m_grid_y.GetColumnCount(); i++)
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_grid_y.SetItemText(0, i, strRowNum);
	}
}

void CComponenthHeatmapTableDlg::OnBnClickedButtonInsertPoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_pointName = dlg.m_strPointName;
	}
	UpdateData(FALSE);
}
