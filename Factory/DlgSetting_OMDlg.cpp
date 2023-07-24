// DlgSetting_OMDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgSetting_OMDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "InputTextDlg.h"
// CDlgSetting_OMDlg �Ի���

IMPLEMENT_DYNAMIC(CDlgSetting_OMDlg, CDialogEx)

CDlgSetting_OMDlg::CDlgSetting_OMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetting_OMDlg::IDD, pParent)
	, m_bWeatherDisplay(FALSE)
{

}

CDlgSetting_OMDlg::~CDlgSetting_OMDlg()
{
}

void CDlgSetting_OMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_WEATHER_DISPLAY, m_bWeatherDisplay);
	DDX_Control(pDX, IDC_CHECK_0, m_check_0);
	DDX_Control(pDX, IDC_CHECK_1, m_check_1);
	DDX_Control(pDX, IDC_CHECK_2, m_check_2);
	DDX_Control(pDX, IDC_CHECK_3, m_check_3);
	DDX_Control(pDX, IDC_CHECK_4, m_check_4);
	DDX_Control(pDX, IDC_CHECK_5, m_check_5);
	DDX_Control(pDX, IDC_CHECK_6, m_check_6);
	DDX_Control(pDX, IDC_CHECK_7, m_check_7);
	DDX_Control(pDX, IDC_CHECK_8, m_check_8);
	DDX_Control(pDX, IDC_CHECK_9, m_check_9);
	DDX_Control(pDX, IDC_CHECK_10, m_check_10);
	DDX_Control(pDX, IDC_CHECK_11, m_check_11);
	DDX_Control(pDX, IDC_CHECK_12, m_check_12);
	DDX_Control(pDX, IDC_CHECK_13, m_check_13);
	DDX_Control(pDX, IDC_CHECK_14, m_check_14);
	DDX_Control(pDX, IDC_CHECK_15, m_check_15);
}


BEGIN_MESSAGE_MAP(CDlgSetting_OMDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Save, &CDlgSetting_OMDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CDlgSetting_OMDlg ��Ϣ�������

BOOL CDlgSetting_OMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	checkBtnList.clear();
	checkBtnList.push_back(&m_check_0);
	checkBtnList.push_back(&m_check_1);
	checkBtnList.push_back(&m_check_2);
	checkBtnList.push_back(&m_check_3);
	checkBtnList.push_back(&m_check_4);
	checkBtnList.push_back(&m_check_5);
	checkBtnList.push_back(&m_check_6);
	checkBtnList.push_back(&m_check_7);
	checkBtnList.push_back(&m_check_8);
	checkBtnList.push_back(&m_check_9);
	checkBtnList.push_back(&m_check_10);
	checkBtnList.push_back(&m_check_11);
	checkBtnList.push_back(&m_check_12);
	checkBtnList.push_back(&m_check_13);
	checkBtnList.push_back(&m_check_14);
	checkBtnList.push_back(&m_check_15);

	//������ʾ
	wstring wstrJsonRead = L"";
	wstrJsonRead = pFrame->m_pPrjSqlite->GetValueInLocalConfigTable(_T("weather_display"));
	if(wstrJsonRead.size()>0 && wstrJsonRead == L"1")
	{
		m_bWeatherDisplay = TRUE;
	}
	else
	{
		m_bWeatherDisplay = FALSE;
	}

	//����ÿ��ģʽ����������
	{
		wstring wstrJsonRead = L"";
		wstrJsonRead = pFrame->m_pPrjSqlite->GetValueInLocalConfigTable(_T("mode_group_define"));
		if(wstrJsonRead.size() >0)
		{
			string sjson = Project::Tools::WideCharToAnsi(wstrJsonRead.c_str());
			Json::Value jPkt;
			Json::Reader jReader;
			if(jReader.parse(sjson,jPkt))
			{
				Json::Value& groupList = jPkt["groupList"];
				if(groupList.isArray())
				{
					for(int i=0;i<groupList.size();i++)
					{
						wstring wsName = Project::Tools::AnsiToWideChar(groupList[i]["name"].asString().c_str());
						checkBtnList[i]->SetWindowTextW(wsName.c_str());
					}
				}
			}
		}
	}

	//����OMSITE�ײ�ģʽ��ť��ʾ����    �磺  "type": [0,1,2]��ʾ�ײ�����ʾ��0,1,2ָ���3�����͵�ģʽ��json����mode_group_defineһһ��Ӧ��
	{
		wstring wstrJsonRead = L"";
		wstrJsonRead = pFrame->m_pPrjSqlite->GetValueInLocalConfigTable(_T("mode_button_define"));
		if(wstrJsonRead.size() >0)
		{
			string sjson = Project::Tools::WideCharToAnsi(wstrJsonRead.c_str());
			Json::Value jPkt;
			Json::Reader jReader;
			if(jReader.parse(sjson,jPkt))
			{
				Json::Value& typeList = jPkt["type"];
				if(typeList.isArray())
				{
					for(int i=0;i<typeList.size();i++)
					{
						try
						{
							checkBtnList.at(typeList[i].asInt())->SetCheck(1);
						}
						catch (const std::out_of_range& oor)
						{
							;
						}
						catch (CException* e)
						{
							;
						}
					}
				}
			}
		}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgSetting_OMDlg::OnBnClickedButtonSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	
	//������ʾ
	{
		wstring wsTemp;
		if(m_bWeatherDisplay == TRUE)
			wsTemp = L"1";
		else
			wsTemp = L"";
		pFrame->m_pPrjSqlite->SetValueInLocalConfigTable(_T("weather_display"), wsTemp);
	}
	
	//����ÿ��ģʽ����������
	{
		wstring wsTemp;
		CString csTemp;
		Json::Value jElement;
		Json::Value jPacket;
		jPacket["groupList"];
		for(int i=0; i< checkBtnList.size(); i++)
		{
			checkBtnList[i]->GetWindowTextW(csTemp);
			jElement["name"] = Project::Tools::WideCharToAnsi(csTemp);
			jPacket["groupList"].append(jElement);
		}

		string sJsonPacket = jPacket.toStyledString();
		pFrame->m_pPrjSqlite->SetValueInLocalConfigTable(_T("mode_group_define"), sJsonPacket);
	}

	//����OMSITE�ײ�ģʽ��ť��ʾ����    �磺  "type": [0,1,2]��ʾ�ײ�����ʾ��0,1,2ָ���3�����͵�ģʽ��json����mode_group_defineһһ��Ӧ��
	{
		Json::Value jType;
		jType["type"];
		for(int i=0; i< checkBtnList.size(); i++)
		{
			if(checkBtnList[i]->GetCheck() == 1)
				jType["type"].append(i);
		}

		string temp = jType.toStyledString();
		pFrame->m_pPrjSqlite->SetValueInLocalConfigTable(_T("mode_button_define"), temp);
	}
}

BEGIN_MESSAGE_MAP(CButtonPlus, CButton)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


void CButtonPlus::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CInputTextDlg dlg;
	GetWindowTextW(dlg.m_strText);
	if(IDOK == dlg.DoModal())
	{
		SetWindowTextW(dlg.m_strText);
	}
	CButton::OnRButtonUp(nFlags, point);
}