// PointEditAPISelect.cpp : 实现文件
//

#include "stdafx.h"
#include "PointEditAPISelect.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "json/json.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
// CPointEditAPISelect 对话框

IMPLEMENT_DYNAMIC(CPointEditAPISelect, CDialog)

CPointEditAPISelect::CPointEditAPISelect(CWnd* pParent /*=NULL*/)
	: CDialog(CPointEditAPISelect::IDD, pParent)
	, m_strAPIDes(_T(""))
{

}

CPointEditAPISelect::~CPointEditAPISelect()
{
}

void CPointEditAPISelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_API_LISTt, m_ItemAPIList);
	DDX_Text(pDX, IDC_EDIT_API_DES, m_strAPIDes);
}


BEGIN_MESSAGE_MAP(CPointEditAPISelect, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_API_LISTt, &CPointEditAPISelect::OnCbnSelchangeComboApiListt)
	ON_BN_CLICKED(IDOK, &CPointEditAPISelect::OnBnClickedOk)
	ON_CBN_EDITUPDATE(IDC_COMBO_API_LISTt, &CPointEditAPISelect::OnCbnEditupdateComboApiListt)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CPointEditAPISelect::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CPointEditAPISelect 消息处理程序


BOOL CPointEditAPISelect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(true == readFileIntoAPIList())
	{
		for(int i=0;i<m_APIList.size();i++)
		{
			m_ItemAPIList.InsertString(i, m_APIList[i].API);
			m_ItemAPIList.SetItemDataPtr(i, &m_APIList[i]);
		}
	}
	else
	{
		EndDialog(IDD);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool CPointEditAPISelect::readFileIntoAPIList(void)
{
	CStdioFile fileCtrl;
	string strfileContent;
	CString asd;
	Project::Tools::GetSysPath(asd);
	asd+=L"\\config\\ScriptAPI.json";
	if(TRUE == fileCtrl.Open(asd,CFile::modeRead|CFile::shareDenyNone))
	{
		char * charList = new char[1024];
		while(true)
		{
			int readCountt = fileCtrl.Read(charList, 1024);
			if(readCountt == 0 ||readCountt == -1)
				break;
			string str(charList, readCountt);
			strfileContent += str;
		}	
		delete[] charList;
	}
	else
	{
		AfxMessageBox(L"配置文件“config/ScriptAPI.json”打开失败。");
		return false;
	}

	Json::Reader reader;
	if(strfileContent.length() > 0)
	{
		Json::Value jsonValue;
		if(true == reader.parse(strfileContent, jsonValue))
		{
			Json::Value& jsonAPIList = jsonValue.get("APIList", NULL);
			if(false == jsonAPIList.isArray() || jsonAPIList == NULL){
				AfxMessageBox(L"配置文件损坏,ScriptAPI.json。");
				return false;
			}
			int nSize = jsonAPIList.size();
			for(int i=0; i<nSize; i++)
			{
				_APIContent api;
				if(jsonAPIList[i]["api"].isString()){
					api.API = Project::Tools::UTF8ToWideChar(jsonAPIList[i]["api"].asString().c_str()).c_str();
				}
				if(jsonAPIList[i]["brief"].isString()){
					api.brief = Project::Tools::UTF8ToWideChar(jsonAPIList[i]["brief"].asString().c_str()).c_str();
				}
				if(jsonAPIList[i]["demo"].isString()){
					api.demo = Project::Tools::UTF8ToWideChar(jsonAPIList[i]["demo"].asString().c_str()).c_str();
				}
				m_APIList.push_back(api);
			}
		}
		else
		{
			AfxMessageBox(L"配置文件“config/ScriptAPI.json”内容被修改。");
			return false;
		}
	}

	return true;
}

void CPointEditAPISelect::OnCbnSelchangeComboApiListt()
{
	// TODO: 在此添加控件通知处理程序代码
	int nAPI = m_ItemAPIList.GetCurSel();
	if(CB_ERR == nAPI){
		return;
	}

	_APIContent * api= (_APIContent *)m_ItemAPIList.GetItemDataPtr(nAPI);
	if(api == NULL)
		return;
	m_strAPIDes = _T("简介：\r\n")
		+ api->brief
		+ _T("\r\n")
		+ _T("举例：\r\n")
		+ api->demo;
	UpdateData(FALSE);
}
/*
	确认按键
	将组合框选中项赋值给m_strAPI,提供给外部使用
*/

void CPointEditAPISelect::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nAPI = m_ItemAPIList.GetCurSel();
	m_ItemAPIList.GetLBText(nAPI,m_strAPIFunction);
	UpdateData(FALSE);
	CDialog::OnOK();
}

void CPointEditAPISelect::OnCbnEditupdateComboApiListt()
{
	// TODO: 在此添加控件通知处理程序代码
	
	UpdateData(FALSE);
	WORD nStartChar = 0;
	WORD nEndChar = 0;
	{//获取光标位置
		DWORD dwSel = m_ItemAPIList.GetEditSel();
		nStartChar = LOWORD(dwSel);//低字记录了编辑控件中选中的字符串中起始位置
		nEndChar= HIWORD(dwSel);		//高字记录了紧接着选中的字符串的第一个字符的位置
	}
	
	CString searchName;
	{//判断当前字符是否为NULL
		m_ItemAPIList.GetWindowText(searchName);
		if (searchName.IsEmpty() == TRUE)
		{
			TRACE(_T("On Combo-box Edit Update\n"));
			m_ItemAPIList.ShowDropDown(FALSE);
			m_ItemAPIList.Invalidate(FALSE);
			return;
		}
	}

	{//清空下列列表
		int countnum = m_ItemAPIList.GetCount();
		if (countnum>0)
		{
			for (int i =0;i<countnum;i++)
			{
				m_ItemAPIList.DeleteString(0);
			}
		}
	}

	{//搜索并显示匹配的字符串
		CStringArray addString ;
		addString.SetSize(100);
		addString.RemoveAll();
		m_ItemAPIList.AddString(searchName);
		addString.Add(searchName);
		//添加字符串
		for (int i = 0; i <m_APIList.size();i++)
		{
			CString& str = m_APIList[i].API;
			int n = str.Find(searchName);
			if(n>=0)
			{
				BOOL bAdd = TRUE;
				//判断有没有重复添加的
				for(int j =0;j<addString.GetCount();j++)
				{
					if (str == addString[j])
					{
						bAdd = FALSE;
						break;
					}
				}
				if(bAdd == TRUE)
				{
					m_ItemAPIList.SetItemDataPtr(m_ItemAPIList.AddString(m_APIList[i].API), &m_APIList[i]);
					addString.Add(m_APIList[i].API);
				}
			}
		}
		if(addString.GetCount()<=1)
		{
			addString.RemoveAll();
			int countnum = m_ItemAPIList.GetCount();
			if (countnum>0)
			{
				for (int i =0;i<countnum;i++)
				{
					m_ItemAPIList.DeleteString(0);
				}
			}
			m_ItemAPIList.ShowDropDown(false);
			Invalidate(FALSE);

			//SetEditSel(searchName.GetLength(),searchName.GetLength());
			//m_ItemAPIList.SetEditSel(nStartChar,nEndChar); //光标跳到相应位置
			return;
		}
		addString.RemoveAll();
	}

	{
		::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW))); //防止鼠标消失
		m_ItemAPIList.ShowDropDown(TRUE);//显示下拉框
		m_ItemAPIList.SetCurSel(0);
		m_ItemAPIList.SetEditSel(searchName.GetLength(),searchName.GetLength()); //光标跳到末尾,//置光标于文本最后，防止全部选中
		//m_ItemAPIList.SetEditSel(nStartChar,nEndChar); //恢复光标位置 光标跳到相应位置
	}
}


void CPointEditAPISelect::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_APIList.size()>0)
	{
		m_ItemAPIList.Clear();
		m_ItemAPIList.ResetContent();
		for(int i=0;i<m_APIList.size();i++)
		{
			if(&(m_APIList[i]) == NULL)
				continue;
			m_ItemAPIList.InsertString(i, m_APIList[i].API);
			m_ItemAPIList.SetItemDataPtr(i, &m_APIList[i]);
		}
		m_ItemAPIList.SetCurSel(0);
		OnCbnSelchangeComboApiListt();
	}
}
