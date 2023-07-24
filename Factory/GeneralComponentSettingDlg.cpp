/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�CGeneralComponentSettingDlg.c
* �ļ���ʶ��
* ժ    Ҫ����ͨ��������á�����
*
* ��ǰ�汾�� 1.1
* �� �ߣ� Mark
* ������ڣ� 2020��11��11��
*
* ȡ���汾��1.0 
* ԭ����  ��Mark
* ������ڣ�2020��2��24��
*/

#include "stdafx.h"
#include "GeneralComponentSettingDlg.h"
#include "afxdialogex.h"
#include "json/json.h"
#include "./ControlEx/WarningDlg.h"
#include "./Tools/CustomTools/CustomTools.h"

// CGeneralComponentSettingDlg �Ի���

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


// CGeneralComponentSettingDlg ��Ϣ�������


BOOL CGeneralComponentSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitControls();
	InitComponent();
	//m_strCfgValue_Backup = m_strCfgValue;
	m_nSelectedTempalte = m_comboTempalte.GetCurSel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGeneralComponentSettingDlg::InitControls(void)
{
	//ͼ��combobox��ʼ��
	m_comboLayer.AddString(_T("1��"));
	m_comboLayer.AddString(_T("2��"));
	m_comboLayer.AddString(_T("3��"));
	m_comboLayer.AddString(_T("4��"));
	m_comboLayer.AddString(_T("5��"));
	m_comboLayer.AddString(_T("6��"));
	m_comboLayer.AddString(_T("7��"));
	m_comboLayer.AddString(_T("8��"));
	m_comboLayer.AddString(_T("9��"));
	m_comboLayer.AddString(_T("10��"));
	m_comboLayer.SetCurSel(m_nLayer);

	int nLimit = ((CEdit*)GetDlgItem(IDC_EDIT_CFG_VALUE))->GetLimitText();
	((CEdit*)GetDlgItem(IDC_EDIT_CFG_VALUE))->SetLimitText(UINT_MAX);
	nLimit = ((CEdit*)GetDlgItem(IDC_EDIT_CFG_VALUE))->GetLimitText();
}

void CGeneralComponentSettingDlg::InitComponent(void)
{
	//���combobox��ʼ��
	m_comboTempalte.InsertString(None,				_T("�༭"));
	m_comboTempalte.InsertString(energy,			_T("��Դ�������"));
	m_comboTempalte.InsertString(ParamQuery,		_T("�������ٲ�ѯ���"));
	m_comboTempalte.InsertString(reportHistory,		_T("���������б����"));
	m_comboTempalte.InsertString(echarts,			_T("��������ͼ"));
	m_comboTempalte.InsertString(BigDataAnalysis,	_T("����������Է������"));
	m_comboTempalte.InsertString(evaluationBar,		_T("���������"));
	m_comboTempalte.InsertString(modelControl,		_T("ģʽ�����Զ������"));
	m_comboTempalte.InsertString(accumbar,			_T("������״ͼ���"));
	m_comboTempalte.InsertString(heatmap_table,		_T("ͨ�ñ����ͼ���"));

	//combo box���������
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

	//��ȡ�����Ҫ�ƶ���λ��
	CRect rc;
	GetDlgItem(IDC_STATIC_LAYOUT)->GetWindowRect(rc); //��ȡ�����صĲ��ֵװ�������λ�ã��������Ի��򶼵�������ͬλ����
	ScreenToClient(&rc); 

	//�Ӵ���ע��
	m_ChildPanel_Energy.Create(IDD_DIALOG_GENE_COMP_SETTING_Energy, this);
	m_ChildPanel_ParamQuery.Create(IDD_DIALOG_GENE_COMP_SETTING_ParamQuery, this);	
	m_ChildPanel_ReportHistory.Create(IDD_DIALOG_GENE_COMP_SETTING_ReportHistory, this);	
	m_ChildPanel_Echarts.Create(IDD_DIALOG_GENE_COMP_SETTING_Echarts, this);	
	m_ChildPanel_BigDataAnalysis.Create(IDD_DIALOG_GENE_COMP_SETTING_BigDataAnalysis, this);	
	m_ChildPanel_EvaluationBar.Create(IDD_DIALOG_GENE_COMP_SETTING_EvaluationBar, this);	
	m_ChildPanel_ModeControl.Create(IDD_DIALOG_GENE_COMP_SETTING_ModelControl, this);	
	m_ChildPanel_Accumbar.Create(IDD_DIALOG_GENE_COMP_SETTING_accumbar, this);
	m_ChildPanel_HeatmapTable.Create(IDD_DIALOG_GENE_COMP_SETTING_Heatmap_table, this);

	//��������ƶ�λ��
	m_ChildPanel_Energy.MoveWindow(rc);
	m_ChildPanel_ParamQuery.MoveWindow(rc);
	m_ChildPanel_ReportHistory.MoveWindow(rc);
	m_ChildPanel_Echarts.MoveWindow(rc);
	m_ChildPanel_BigDataAnalysis.MoveWindow(rc);
	m_ChildPanel_EvaluationBar.MoveWindow(rc);
	m_ChildPanel_ModeControl.MoveWindow(rc);
	m_ChildPanel_Accumbar.MoveWindow(rc);
	m_ChildPanel_HeatmapTable.MoveWindow(rc);

	//����m_strCfgValue��ֵ���ж�Ӧ����ʾ�ĸ�Child��༭��
	Json::Value		jsonRoot;
	Json::Reader	jsonReader;
	string strResult_Ansi;
	strResult_Ansi = Project::Tools::WideCharToAnsi(m_strCfgValue.GetString());
	if(jsonReader.parse(strResult_Ansi, jsonRoot))//��jsonReader.parse������tye�Ƕ���
	{
		if(jsonRoot.isMember("type"))//�ж�key�Ĵ���
		{
			string strType;
			strType = jsonRoot["type"].asString();
			//�ҵ�ƥ��Ĵ�������
			int nType = GetIndexInComboBoxType(strType);
			m_comboTempalte.SetCurSel(nType);//����ComBox��ʾ
			SwitchShowWindow(nType,jsonRoot);//ѡ����ʾ�Ľ���
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
	// TODO: �ڴ����ר�ô����/����û���
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
		GetDlgItem(IDC_EDIT_CFG_VALUE)->ShowWindow(SW_SHOW);//json�༭��
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
	GetDlgItem(IDC_EDIT_CFG_VALUE)->ShowWindow(SW_HIDE);//json�༭��
}

void CGeneralComponentSettingDlg::OnCbnSelchangeComboTemplate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	/*�༭��״̬��*/
	if(IsJsonEditVisible() == true)
	{
		Json::Reader JsonReader;
		Json::Value JsonValue;
		string strJsonPack;
		strJsonPack = Project::Tools::WideCharToAnsi(m_strCfgValue.GetString());
		if(!JsonReader.parse(strJsonPack,JsonValue))
		{
			AfxMessageBox(L"Json��ʽ����");
			return;
		}
	}
	else//�Ӵ���ģʽ��
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
			/*Json������ʽ�����CString*/
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//�л��ű�����
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
	else//�л������ý���
	{
		//����m_strCfgValue��ֵ���ж�Ӧ����ʾ�ĸ�Child��༭��
		Json::Value		jsonRoot;
		Json::Reader	jsonReader;
		string strResult_Ansi = Project::Tools::WideCharToAnsi(m_strCfgValue.GetString());
		if(jsonReader.parse(strResult_Ansi, jsonRoot))//��jsonReader.parse������tye�Ƕ���
		{
			if(jsonRoot.isMember("type"))//�ж�key�Ĵ���
			{
				string strType;
				strType = jsonRoot["type"].asString();
				int nType = GetIndexInComboBoxType(strType);
				m_comboTempalte.SetCurSel(nType);//����ComBox��ʾ
				SwitchShowWindow(nType, jsonRoot);//ѡ����ʾ�Ľ���
				return;
			}
		}
		//�����ж�ȫfalse
		SwitchShowWindow(None);
	}
	UpdateData(FALSE);
}