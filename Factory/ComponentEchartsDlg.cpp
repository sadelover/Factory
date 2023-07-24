// ComponentEchartsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComponentEchartsDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"

#include "MainFrm.h"
#include "DataComConfigDlg.h"
// CComponentEcharts �Ի���

IMPLEMENT_DYNAMIC(CComponentEcharts, CDialogEx)

CComponentEcharts::CComponentEcharts(CWnd* pParent /*=NULL*/)
	: CComponentDlgBase(CComponentEcharts::IDD, pParent)
	, m_strBindPoint(_T(""))
	, m_nTitleRsvdHeight(20)
	, m_nTitleFontSize(12)
	, m_nXWidth(30)
	, m_nYWidth(30)
	, m_nMax(500)
{

}

CComponentEcharts::~CComponentEcharts()
{
}

void CComponentEcharts::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BINDPOINT, m_strBindPoint);
	DDX_Text(pDX, IDC_EDIT2, m_nTitleRsvdHeight);
	DDX_Text(pDX, IDC_EDIT4, m_nTitleFontSize);
	DDX_Control(pDX, IDC_COMBO1, m_comboxTitleAlignment);
	DDX_Control(pDX, IDC_COMBO2, m_comboxTitleFontWeight);
	DDX_Text(pDX, IDC_EDIT6, m_nXWidth);
	DDX_Text(pDX, IDC_EDIT8, m_nYWidth);
	DDX_Text(pDX, IDC_EDIT7, m_nMax);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, m_colorbtTitleColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_colorbtXColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON3, m_colorbtYColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON4, m_colorbtGridLineColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON5, m_colorbtGridAreaColor);
}


/************************************************************************/
/* ����˵�����ؼ�ˢ�º���                                               */
/************************************************************************/
void CComponentEcharts::RefreshControls(Json::Value jsonRoot)
{
	if(jsonRoot.isMember("type") == false)
	{
		return;
	}
	if(jsonRoot["type"] == "echarts")
	{
		//point
		if(jsonRoot.isMember("point"))
			if(jsonRoot["point"].isArray())//ǰ�˴��������������������
				if(jsonRoot["point"][0U].isString())
					m_strBindPoint = jsonRoot["point"][0U].asCString();
		//max
		if(jsonRoot.isMember("max"))
			if(jsonRoot["max"].isInt())
				m_nMax = jsonRoot["max"].asInt();
		//timeType 

		/*config*/
		if(jsonRoot.isMember("config"))
		{
			//config->splitLineColor//��������ɫ��
			if(jsonRoot["config"].isMember("splitLineColor"))
			{
				if(jsonRoot["config"]["splitLineColor"].isString())
				{
					CString  splitLineColor;
					splitLineColor = jsonRoot["config"]["splitLineColor"].asCString();
					m_colorbtGridLineColor.SetColor(CstrRGBToUIntBGR(splitLineColor));
				}
			}
			//config->backgroundColor//����������ɫ��
			if(jsonRoot["config"].isMember("backgroundColor"))
			{
				if(jsonRoot["config"]["backgroundColor"].isString())
				{
					CString  backgroundColor;
					backgroundColor = jsonRoot["config"]["backgroundColor"].asCString();
					m_colorbtGridAreaColor.SetColor(CstrRGBToUIntBGR(backgroundColor));
				}
			}
			//config->xLabelColor//X��̶ȱ�ǩ��ɫ
			if(jsonRoot["config"].isMember("xLabelColor"))
			{
				if(jsonRoot["config"]["xLabelColor"].isString())
				{
					CString  xLabelColor;
					xLabelColor = jsonRoot["config"]["xLabelColor"].asCString();
					m_colorbtXColor.SetColor(CstrRGBToUIntBGR(xLabelColor));
				}
			}
			//config->yLabelColor//Y��̶ȱ�ǩ��ɫ
			if(jsonRoot["config"].isMember("yLabelColor"))
			{
				if(jsonRoot["config"]["yLabelColor"].isString())
				{
					CString  yLabelColor;
					yLabelColor = jsonRoot["config"]["yLabelColor"].asCString();
					m_colorbtYColor.SetColor(CstrRGBToUIntBGR(yLabelColor));
				}
			}
			//config->Xwidth//x��̶ȱ�ǩԤ�����
			if(jsonRoot["config"].isMember("Xwidth"))
			{
				if(jsonRoot["config"]["Xwidth"].isInt())
				{
					m_nXWidth = jsonRoot["config"]["Xwidth"].asInt();
				}
			}
			//config->Ywidth//y��̶ȱ�ǩԤ�����
			if(jsonRoot["config"].isMember("Ywidth"))
			{
				if(jsonRoot["config"]["Ywidth"].isInt())
				{
					m_nYWidth = jsonRoot["config"]["Ywidth"].asInt();
				}
			}
			//config->top//����Ԥ���߶�
			if(jsonRoot["config"].isMember("top"))
			{
				if(jsonRoot["config"]["top"].isInt())
				{
					m_nTitleRsvdHeight = jsonRoot["config"]["top"].asInt();
				}
			}
			//config->x//�������
			if(jsonRoot["config"].isMember("x"))
			{
				if(jsonRoot["config"]["x"].isString())
				{
					SetTitleAlignmentComBox(jsonRoot["config"]["x"].asString());
				}
			}
			//config->titleColor//������ɫ
			if(jsonRoot["config"].isMember("titleColor"))
			{
				if(jsonRoot["config"]["titleColor"].isString())
				{
					CString  titleColor;
					titleColor = jsonRoot["config"]["titleColor"].asCString();
					m_colorbtTitleColor.SetColor(CstrRGBToUIntBGR(titleColor));
				}
			}
			//config->titleFontSIze//���������С
			if(jsonRoot["config"].isMember("titleFontSIze"))
			{
				if(jsonRoot["config"]["titleFontSIze"].isInt())
				{
					m_nTitleFontSize = jsonRoot["config"]["titleFontSIze"].asInt();
				}
			}
			//config->titleFontWeight//���������ϸ
			if(jsonRoot["config"].isMember("titleFontWeight"))
			{
				if(jsonRoot["config"]["titleFontWeight"].isString())
				{
					SetTitleFontWeightComBox(jsonRoot["config"]["titleFontWeight"].asString());
				}
			}
		}//END if(jsonRoot.isMember("config"))
	}//END if(jsonRoot["type"] == "echarts")
	UpdateData(FALSE);
}

/************************************************************************/
/*����˵����Json���ݴ��                                                */
/************************************************************************/
Json::Value CComponentEcharts::GenerateJson()
{
	UpdateData(TRUE);
	string strTemp;
	CString cstrTemp;
	/*���*/
	Json::Value cJsonPacket;
	cJsonPacket["type"] = "echarts";//�������
	cJsonPacket["max"] = m_nMax;//������ȡֵ��Χ
	Project::Tools::WideCharToUtf8(m_strBindPoint.GetString(),strTemp);
	cJsonPacket["point"];
	cJsonPacket["point"].append(strTemp);//�󶨵�//ǰ�˴��������������������
	cJsonPacket["timeType"] = "today";

	cJsonPacket["config"];
	cJsonPacket["config"]["splitLineColor"] = UIntBGRToStrRGB(m_colorbtGridLineColor.GetColor());//��������ɫ��
	cJsonPacket["config"]["backgroundColor"] = UIntBGRToStrRGB(m_colorbtGridAreaColor.GetColor()); //����������ɫ��
	cJsonPacket["config"]["xLabelColor"] = UIntBGRToStrRGB(m_colorbtXColor.GetColor());//X��̶ȱ�ǩ��ɫ
	cJsonPacket["config"]["yLabelColor"] = UIntBGRToStrRGB(m_colorbtYColor.GetColor()); //Y��̶ȱ�ǩ��ɫ
	cJsonPacket["config"]["Xwidth"] = m_nXWidth;//x��̶ȱ�ǩԤ�����
	cJsonPacket["config"]["Ywidth"] = m_nYWidth;//y��̶ȱ�ǩԤ�����
	cJsonPacket["config"]["top"] = m_nTitleRsvdHeight;//����Ԥ���߶�
	cJsonPacket["config"]["x"] = GetTitleAlignmentStr();//����λ��
	cJsonPacket["config"]["titleColor"] = UIntBGRToStrRGB(m_colorbtTitleColor.GetColor());
	cJsonPacket["config"]["titleFontSIze"] = m_nTitleFontSize;
	cJsonPacket["config"]["titleFontWeight"] = GetTitleFontWeightStr();//����������

	return cJsonPacket;
}
/*
* �������ܣ�CString���͵�RGB��ʽ��ɫת��UINT��BGR�ַ���
* ���������
* ���������
* ����ֵ  ��
*/
UINT CComponentEcharts::CstrRGBToUIntBGR(CString cstrRGB)
{
	cstrRGB.Delete(cstrRGB.Find(L'#',0));
	int nRGB = wcstol(cstrRGB, NULL, 16);
	/*int nRGB = (UINT)_ttoi(cstrRGB);*/
	UINT R = (nRGB & 0xFF0000) >> 16;
	UINT G = nRGB & 0x00FF00;
	UINT B = (nRGB & 0x0000FF) << 16;
	return B|G|R;
}
/*
* �������ܣ�UINT���͵�BGR��ʽ��ɫת��String��RGB�ַ���
* ���������
* ���������
* ����ֵ  ��
*/
string CComponentEcharts::UIntBGRToStrRGB(UINT nBGRColor)
{
	CString cstrColor;
	string strCStrToStr;
	UINT R = (nBGRColor & 0x0000FF) << 16;
	UINT G = nBGRColor & 0x00FF00;
	UINT B = (nBGRColor & 0xFF0000) >> 16;
	cstrColor.Format(L"#%06X",R|G|B);
	Project::Tools::WideCharToUtf8(cstrColor.GetString(),strCStrToStr);
	return strCStrToStr;
}
/*
* �������ܣ�UINT���͵�BGR��ʽ��ɫת��CString��RGB�ַ���
* ���������
* ���������
* ����ֵ  ��
*/

CString CComponentEcharts::UIntBGRToCStrRGB(UINT nBGRColor)
{
	CString cstrColor;
	string strCStrToStr;
	UINT R = (nBGRColor & 0x0000FF) << 16;
	UINT G = nBGRColor & 0x00FF00;
	UINT B = (nBGRColor & 0xFF0000) >> 16;
	cstrColor.Format(L"#%06X",R|G|B);
	return cstrColor;
}
/*
* �������ܣ���ȡ����λ�õ�strӢ��
* ���������
* ���������
* ����ֵ  ��
*/
string CComponentEcharts::GetTitleAlignmentStr(void)
{
	///������У�center��//����left��/������ʾ��right��
	int nSel = m_comboxTitleAlignment.GetCurSel();
	if(nSel == 0)
		return "center";
	else if(nSel == 1)
		return "left";
	else if(nSel == 2)
		return "right";
	else
		return "center";
}
/*
* �������ܣ����ñ���λ��Combox
* ���������
* ���������
* ����ֵ  ��
*/
void CComponentEcharts::SetTitleAlignmentComBox(string str)
{
	if(str == "center")
		m_comboxTitleAlignment.SetCurSel(0);
	else if(str == "left")
		m_comboxTitleAlignment.SetCurSel(1);
	else if(str == "right")
		m_comboxTitleAlignment.SetCurSel(2);
	else
		m_comboxTitleAlignment.SetCurSel(0);
}
/*
* �������ܣ���ȡ���������ȵ�strӢ��
* ���������
* ���������
* ����ֵ  ��
*/
string CComponentEcharts::GetTitleFontWeightStr(void)
{
	//���������ϸ����ѡ����normal��������bold�����壬bolder���ش֣�lighter����ϸ��
	int nSel = m_comboxTitleAlignment.GetCurSel();
	if(nSel == 0)
		return "normal";
	else if(nSel == 1)
		return "bold";
	else if(nSel == 2)
		return "bolder";
	else if(nSel == 3)
		return "lighter";
	else
		return "normal";
}
/*
* �������ܣ����ñ���������Combox
* ���������
* ���������
* ����ֵ  ��
*/
void CComponentEcharts::SetTitleFontWeightComBox(string str)
{
	if(str == "normal")
		m_comboxTitleFontWeight.SetCurSel(0);
	else if(str == "bold")
		m_comboxTitleFontWeight.SetCurSel(1);
	else if(str == "bolder")
		m_comboxTitleFontWeight.SetCurSel(2);
	else if(str == "lighter")
		m_comboxTitleFontWeight.SetCurSel(3);
	else
		m_comboxTitleFontWeight.SetCurSel(0);
}

BEGIN_MESSAGE_MAP(CComponentEcharts, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELETEPOINT, &CComponentEcharts::OnBnClickedButtonSeletepoint)
	ON_BN_CLICKED(IDC_BUTTON_CLEARPOINT, &CComponentEcharts::OnBnClickedButtonClearpoint)
END_MESSAGE_MAP()


// CComponentEcharts ��Ϣ�������


BOOL CComponentEcharts::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	///������У�center��//����left��/������ʾ��right��
	m_comboxTitleAlignment.InsertString(0,L"����");
	m_comboxTitleAlignment.InsertString(1,L"����");
	m_comboxTitleAlignment.InsertString(2,L"����");
	m_comboxTitleAlignment.SetCurSel(0);

	//���������ϸ����ѡ����normal��������bold���ϴ֣�bolder���ش֣�lighter����ϸ��
	m_comboxTitleFontWeight.InsertString(0, L"����");
	m_comboxTitleFontWeight.InsertString(1, L"����");
	m_comboxTitleFontWeight.InsertString(2, L"�ش�");
	m_comboxTitleFontWeight.InsertString(3, L"��ϸ");
	m_comboxTitleFontWeight.SetCurSel(0);

	m_colorbtTitleColor.SetColor(RGB(255,0,0));
	m_colorbtXColor.SetColor(RGB(0,255,255));
	m_colorbtYColor.SetColor(RGB(255,0,0));
	m_colorbtGridLineColor.SetColor(RGB(0,255,255));
	m_colorbtGridAreaColor.SetColor(RGB(0,0,255));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CComponentEcharts::OnBnClickedButtonSeletepoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strBindPoint = dlg.m_strPointName;
	}
	UpdateData(FALSE);
}


void CComponentEcharts::OnBnClickedButtonClearpoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strBindPoint.Empty();
	UpdateData(FALSE);
}
