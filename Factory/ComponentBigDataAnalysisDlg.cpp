// ComponentBigDataAnalysisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComponentBigDataAnalysisDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"

#include "afxdialogex.h"
#include "DataPointConfigDlg.h"
// CComponentBigDataAnalysis 对话框

IMPLEMENT_DYNAMIC(CComponentBigDataAnalysis, CDialogEx)

CComponentBigDataAnalysis::CComponentBigDataAnalysis(CWnd* pParent /*=NULL*/)
	: CComponentDlgBase(CComponentBigDataAnalysis::IDD, pParent)
	, m_cstrBindName(_T(""))
	, m_nInputRow(6)
	, m_nOutputRow(3)
{

}

CComponentBigDataAnalysis::~CComponentBigDataAnalysis()
{
}

void CComponentBigDataAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_gridInput);
	DDX_Control(pDX, IDC_GRID1, m_gridOutput);
	DDX_Text(pDX, IDC_EDIT3, m_cstrBindName);
	DDX_Text(pDX, IDC_EDIT1, m_nInputRow);
	DDX_Text(pDX, IDC_EDIT2, m_nOutputRow);
}


/************************************************************************/
/* 函数说明：控件刷新函数                                               */
/************************************************************************/
void CComponentBigDataAnalysis::RefreshControls(Json::Value jsonRoot)
{
	int nRow = 0;
	nRow = m_gridInput.GetRowCount();
	for(int i=1; i<=nRow; i++)//第一行不写
	{
		m_gridInput.DeleteRow(i);
	}
	nRow = m_gridOutput.GetRowCount();
	for(int i=1; i<=nRow; i++)//第一行不写
	{
		m_gridOutput.DeleteRow(i);
	}
	m_nOutputRow = 0;
	m_nInputRow = 0;

	if(!jsonRoot.isMember("type"))
	{
		return ;
	}
	if(jsonRoot["type"] == "BigDataAnalysis")
	{
		if(jsonRoot.isMember("bindname"))
		{
			if(jsonRoot["bindname"].isString())
			{
				m_cstrBindName = Project::Tools::AnsiToWideChar(jsonRoot["bindname"].asString().c_str()).c_str();
			}
		}

		if(jsonRoot.isMember("input"))
		{
			if(jsonRoot["input"].isArray())
			{
				m_nInputRow = jsonRoot["input"].size();
				m_gridInput.SetRowCount(m_nInputRow+1);//设置行数
				for(int i = 0; i< m_nInputRow; i++)
				{
					CString name;
					/*name = jsonRoot["input"][i]["name"].asCString();*/
					name = Project::Tools::AnsiToWideChar(jsonRoot["input"][i]["name"].asString().c_str()).c_str();
					CString unit;
					/*unit = jsonRoot["input"][i]["unit"].asCString();*/
					unit = Project::Tools::AnsiToWideChar(jsonRoot["input"][i]["unit"].asString().c_str()).c_str();
					//设置表格内容（第一行不可更改）
					m_gridInput.SetItemText(i+1,1,name);
					m_gridInput.SetItemText(i+1,2,unit);
				}
			}
		}

		if(jsonRoot.isMember("output"))
		{
			if(jsonRoot["output"].isArray())
			{
				m_nOutputRow = jsonRoot["output"].size();//获取组大小
				m_gridOutput.SetRowCount(m_nOutputRow+1);//设置行数
				for(int i = 0; i< m_nOutputRow; i++)
				{
					CString name;
					//name = jsonRoot["output"][i]["name"].asCString();
					name = Project::Tools::AnsiToWideChar(jsonRoot["output"][i]["name"].asString().c_str()).c_str();
					CString unit;
					//unit = jsonRoot["output"][i]["unit"].asCString();
					unit = Project::Tools::AnsiToWideChar(jsonRoot["output"][i]["unit"].asString().c_str()).c_str();
					//设置表格内容（第一行不可更改）
					m_gridOutput.SetItemText(i+1,1,name);
					m_gridOutput.SetItemText(i+1,2,unit);
				}
			}
		}
	}//END if(jsonRoot["type"] == "BigDataAnalysis")
	UpdateData(FALSE);
}

/************************************************************************/
/*函数说明：Json数据打包                                                */
/************************************************************************/
Json::Value CComponentBigDataAnalysis::GenerateJson()
{
	UpdateData(TRUE);

	/*打包*/
	Json::Value jsonPacket;

	jsonPacket["type"] = "BigDataAnalysis";
	
	string strBindName;
	strBindName = Project::Tools::WideCharToAnsi(m_cstrBindName.GetString());
	jsonPacket["bindname"] = strBindName;

	jsonPacket["input"];
	{
		string strName;
		string strUnit;
		for(int i=1; i<=m_nInputRow; i++)
		{
			Json::Value jsonValue;
			CString cstrName = m_gridInput.GetItemText(i,1);
			CString cstrUntt = m_gridInput.GetItemText(i,2);
			strName = Project::Tools::WideCharToAnsi(cstrName.GetString());
			jsonValue["name"] = strName;
			strUnit = Project::Tools::WideCharToAnsi(cstrUntt.GetString());
			jsonValue["unit"] = strUnit;
			jsonPacket["input"].append(jsonValue);
		}
	}

	jsonPacket["output"];
	{
		string strName;
		string strUnit;
		for(int i=1; i<=m_nOutputRow; i++)
		{
			Json::Value jsonValue;
			CString cstrName = m_gridOutput.GetItemText(i,1);
			CString cstrUntt = m_gridOutput.GetItemText(i,2);
			strName = Project::Tools::WideCharToAnsi(cstrName.GetString());
			jsonValue["name"] = strName;
			strUnit = Project::Tools::WideCharToAnsi(cstrUntt.GetString());
			jsonValue["unit"] = strUnit;
			jsonPacket["output"].append(jsonValue);
		}
	}

	return jsonPacket;
}

BEGIN_MESSAGE_MAP(CComponentBigDataAnalysis, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SETINPUTLINE, &CComponentBigDataAnalysis::OnBnClickedButtonSetinputline)
	ON_BN_CLICKED(IDC_BUTTON_SETOUTPUTLINE, &CComponentBigDataAnalysis::OnBnClickedButtonSetoutputline)
END_MESSAGE_MAP()


// CComponentBigDataAnalysis 消息处理程序


BOOL CComponentBigDataAnalysis::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect cr;
	m_gridInput.GetClientRect(&cr);
	m_gridOutput.GetClientRect(&cr);

	m_gridInput.SetRowCount(m_nInputRow+1);//设置行数量
	m_gridInput.SetColumnCount(3);//设置列数量
	m_gridInput.SetFixedRowCount(1);
	m_gridInput.SetFixedColumnCount(1);//表头一行一列
	m_gridInput.SetEditable(true);//表格内容不可编辑设置
	m_gridInput.SetItemText(0, 1, _T("变量名称"));//设置内容
	m_gridInput.SetItemText(0, 2, _T("单位"));//设置内容

	

	m_gridOutput.SetRowCount(m_nOutputRow+1);//设置行数量
	m_gridOutput.SetColumnCount(3);//设置列数量
	m_gridOutput.SetFixedRowCount(1);
	m_gridOutput.SetFixedColumnCount(1);//表头一行一列
	m_gridOutput.SetEditable(true);//表格内容不可编辑设置
	m_gridOutput.SetItemText(0, 1, _T("变量名称"));//设置内容
	m_gridOutput.SetItemText(0, 2, _T("单位"));//设置内容

	//刷新行标识
	CString strRowNum;
	for(int i=1; i<=m_nInputRow; i++)
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_gridInput.SetItemText(i, 0, strRowNum);
	}
	for(int i=1; i<=m_nOutputRow; i++)
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_gridOutput.SetItemText(i, 0, strRowNum);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CComponentBigDataAnalysis::OnBnClickedButtonSetinputline()
{
	// TODO: 在此添加控件通知处理程序代码、
	UpdateData(TRUE);

	int nRow = m_nInputRow+1;
	m_gridInput.SetRowCount(nRow);
	CString strRowNum;
	for(int i=1; i<=m_nInputRow; i++)//第一行不写
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_gridInput.SetItemText(i, 0, strRowNum);
	}

	UpdateData(FALSE);
}


void CComponentBigDataAnalysis::OnBnClickedButtonSetoutputline()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	int nRow = m_nOutputRow+1;
	m_gridOutput.SetRowCount(nRow);
	CString strRowNum;
	for(int i=1; i<=m_nOutputRow; i++)//第一行不写
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_gridOutput.SetItemText(i, 0, strRowNum);
	}

	UpdateData(FALSE);
}