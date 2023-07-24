// PointEditAPISelect.cpp : ʵ���ļ�
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
// CPointEditAPISelect �Ի���

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


// CPointEditAPISelect ��Ϣ�������


BOOL CPointEditAPISelect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
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
		AfxMessageBox(L"�����ļ���config/ScriptAPI.json����ʧ�ܡ�");
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
				AfxMessageBox(L"�����ļ���,ScriptAPI.json��");
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
			AfxMessageBox(L"�����ļ���config/ScriptAPI.json�����ݱ��޸ġ�");
			return false;
		}
	}

	return true;
}

void CPointEditAPISelect::OnCbnSelchangeComboApiListt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nAPI = m_ItemAPIList.GetCurSel();
	if(CB_ERR == nAPI){
		return;
	}

	_APIContent * api= (_APIContent *)m_ItemAPIList.GetItemDataPtr(nAPI);
	if(api == NULL)
		return;
	m_strAPIDes = _T("��飺\r\n")
		+ api->brief
		+ _T("\r\n")
		+ _T("������\r\n")
		+ api->demo;
	UpdateData(FALSE);
}
/*
	ȷ�ϰ���
	����Ͽ�ѡ���ֵ��m_strAPI,�ṩ���ⲿʹ��
*/

void CPointEditAPISelect::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nAPI = m_ItemAPIList.GetCurSel();
	m_ItemAPIList.GetLBText(nAPI,m_strAPIFunction);
	UpdateData(FALSE);
	CDialog::OnOK();
}

void CPointEditAPISelect::OnCbnEditupdateComboApiListt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	UpdateData(FALSE);
	WORD nStartChar = 0;
	WORD nEndChar = 0;
	{//��ȡ���λ��
		DWORD dwSel = m_ItemAPIList.GetEditSel();
		nStartChar = LOWORD(dwSel);//���ּ�¼�˱༭�ؼ���ѡ�е��ַ�������ʼλ��
		nEndChar= HIWORD(dwSel);		//���ּ�¼�˽�����ѡ�е��ַ����ĵ�һ���ַ���λ��
	}
	
	CString searchName;
	{//�жϵ�ǰ�ַ��Ƿ�ΪNULL
		m_ItemAPIList.GetWindowText(searchName);
		if (searchName.IsEmpty() == TRUE)
		{
			TRACE(_T("On Combo-box Edit Update\n"));
			m_ItemAPIList.ShowDropDown(FALSE);
			m_ItemAPIList.Invalidate(FALSE);
			return;
		}
	}

	{//��������б�
		int countnum = m_ItemAPIList.GetCount();
		if (countnum>0)
		{
			for (int i =0;i<countnum;i++)
			{
				m_ItemAPIList.DeleteString(0);
			}
		}
	}

	{//��������ʾƥ����ַ���
		CStringArray addString ;
		addString.SetSize(100);
		addString.RemoveAll();
		m_ItemAPIList.AddString(searchName);
		addString.Add(searchName);
		//����ַ���
		for (int i = 0; i <m_APIList.size();i++)
		{
			CString& str = m_APIList[i].API;
			int n = str.Find(searchName);
			if(n>=0)
			{
				BOOL bAdd = TRUE;
				//�ж���û���ظ���ӵ�
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
			//m_ItemAPIList.SetEditSel(nStartChar,nEndChar); //���������Ӧλ��
			return;
		}
		addString.RemoveAll();
	}

	{
		::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW))); //��ֹ�����ʧ
		m_ItemAPIList.ShowDropDown(TRUE);//��ʾ������
		m_ItemAPIList.SetCurSel(0);
		m_ItemAPIList.SetEditSel(searchName.GetLength(),searchName.GetLength()); //�������ĩβ,//�ù�����ı���󣬷�ֹȫ��ѡ��
		//m_ItemAPIList.SetEditSel(nStartChar,nEndChar); //�ָ����λ�� ���������Ӧλ��
	}
}


void CPointEditAPISelect::OnBnClickedButtonReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
