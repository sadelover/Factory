// ComponentEchartsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComponentEchartsDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"

#include "MainFrm.h"
#include "DataComConfigDlg.h"
// CComponentEcharts 对话框

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
/* 函数说明：控件刷新函数                                               */
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
			if(jsonRoot["point"].isArray())//前端代码问题这里必须是数组
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
			//config->splitLineColor//网格线颜色；
			if(jsonRoot["config"].isMember("splitLineColor"))
			{
				if(jsonRoot["config"]["splitLineColor"].isString())
				{
					CString  splitLineColor;
					splitLineColor = jsonRoot["config"]["splitLineColor"].asCString();
					m_colorbtGridLineColor.SetColor(CstrRGBToUIntBGR(splitLineColor));
				}
			}
			//config->backgroundColor//网格区域颜色；
			if(jsonRoot["config"].isMember("backgroundColor"))
			{
				if(jsonRoot["config"]["backgroundColor"].isString())
				{
					CString  backgroundColor;
					backgroundColor = jsonRoot["config"]["backgroundColor"].asCString();
					m_colorbtGridAreaColor.SetColor(CstrRGBToUIntBGR(backgroundColor));
				}
			}
			//config->xLabelColor//X轴刻度标签颜色
			if(jsonRoot["config"].isMember("xLabelColor"))
			{
				if(jsonRoot["config"]["xLabelColor"].isString())
				{
					CString  xLabelColor;
					xLabelColor = jsonRoot["config"]["xLabelColor"].asCString();
					m_colorbtXColor.SetColor(CstrRGBToUIntBGR(xLabelColor));
				}
			}
			//config->yLabelColor//Y轴刻度标签颜色
			if(jsonRoot["config"].isMember("yLabelColor"))
			{
				if(jsonRoot["config"]["yLabelColor"].isString())
				{
					CString  yLabelColor;
					yLabelColor = jsonRoot["config"]["yLabelColor"].asCString();
					m_colorbtYColor.SetColor(CstrRGBToUIntBGR(yLabelColor));
				}
			}
			//config->Xwidth//x轴刻度标签预留宽度
			if(jsonRoot["config"].isMember("Xwidth"))
			{
				if(jsonRoot["config"]["Xwidth"].isInt())
				{
					m_nXWidth = jsonRoot["config"]["Xwidth"].asInt();
				}
			}
			//config->Ywidth//y轴刻度标签预留宽度
			if(jsonRoot["config"].isMember("Ywidth"))
			{
				if(jsonRoot["config"]["Ywidth"].isInt())
				{
					m_nYWidth = jsonRoot["config"]["Ywidth"].asInt();
				}
			}
			//config->top//标题预留高度
			if(jsonRoot["config"].isMember("top"))
			{
				if(jsonRoot["config"]["top"].isInt())
				{
					m_nTitleRsvdHeight = jsonRoot["config"]["top"].asInt();
				}
			}
			//config->x//标题对齐
			if(jsonRoot["config"].isMember("x"))
			{
				if(jsonRoot["config"]["x"].isString())
				{
					SetTitleAlignmentComBox(jsonRoot["config"]["x"].asString());
				}
			}
			//config->titleColor//标题颜色
			if(jsonRoot["config"].isMember("titleColor"))
			{
				if(jsonRoot["config"]["titleColor"].isString())
				{
					CString  titleColor;
					titleColor = jsonRoot["config"]["titleColor"].asCString();
					m_colorbtTitleColor.SetColor(CstrRGBToUIntBGR(titleColor));
				}
			}
			//config->titleFontSIze//标题字体大小
			if(jsonRoot["config"].isMember("titleFontSIze"))
			{
				if(jsonRoot["config"]["titleFontSIze"].isInt())
				{
					m_nTitleFontSize = jsonRoot["config"]["titleFontSIze"].asInt();
				}
			}
			//config->titleFontWeight//标题字体粗细
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
/*函数说明：Json数据打包                                                */
/************************************************************************/
Json::Value CComponentEcharts::GenerateJson()
{
	UpdateData(TRUE);
	string strTemp;
	CString cstrTemp;
	/*打包*/
	Json::Value cJsonPacket;
	cJsonPacket["type"] = "echarts";//组件类型
	cJsonPacket["max"] = m_nMax;//纵坐标取值范围
	Project::Tools::WideCharToUtf8(m_strBindPoint.GetString(),strTemp);
	cJsonPacket["point"];
	cJsonPacket["point"].append(strTemp);//绑定点//前端代码问题这里必须是数组
	cJsonPacket["timeType"] = "today";

	cJsonPacket["config"];
	cJsonPacket["config"]["splitLineColor"] = UIntBGRToStrRGB(m_colorbtGridLineColor.GetColor());//网格线颜色；
	cJsonPacket["config"]["backgroundColor"] = UIntBGRToStrRGB(m_colorbtGridAreaColor.GetColor()); //网格区域颜色；
	cJsonPacket["config"]["xLabelColor"] = UIntBGRToStrRGB(m_colorbtXColor.GetColor());//X轴刻度标签颜色
	cJsonPacket["config"]["yLabelColor"] = UIntBGRToStrRGB(m_colorbtYColor.GetColor()); //Y轴刻度标签颜色
	cJsonPacket["config"]["Xwidth"] = m_nXWidth;//x轴刻度标签预留宽度
	cJsonPacket["config"]["Ywidth"] = m_nYWidth;//y轴刻度标签预留宽度
	cJsonPacket["config"]["top"] = m_nTitleRsvdHeight;//标题预留高度
	cJsonPacket["config"]["x"] = GetTitleAlignmentStr();//标题位置
	cJsonPacket["config"]["titleColor"] = UIntBGRToStrRGB(m_colorbtTitleColor.GetColor());
	cJsonPacket["config"]["titleFontSIze"] = m_nTitleFontSize;
	cJsonPacket["config"]["titleFontWeight"] = GetTitleFontWeightStr();//标题字体宽度

	return cJsonPacket;
}
/*
* 函数介绍：CString类型的RGB格式颜色转成UINT的BGR字符串
* 输入参数：
* 输出参数：
* 返回值  ：
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
* 函数介绍：UINT类型的BGR格式颜色转成String的RGB字符串
* 输入参数：
* 输出参数：
* 返回值  ：
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
* 函数介绍：UINT类型的BGR格式颜色转成CString的RGB字符串
* 输入参数：
* 输出参数：
* 返回值  ：
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
* 函数介绍：获取标题位置的str英文
* 输入参数：
* 输出参数：
* 返回值  ：
*/
string CComponentEcharts::GetTitleAlignmentStr(void)
{
	///标题居中（center）//居左（left）/居右显示（right）
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
* 函数介绍：设置标题位置Combox
* 输入参数：
* 输出参数：
* 返回值  ：
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
* 函数介绍：获取标题字体宽度的str英文
* 输入参数：
* 输出参数：
* 返回值  ：
*/
string CComponentEcharts::GetTitleFontWeightStr(void)
{
	//标题字体粗细：可选：（normal：正常，bold：粗体，bolder：特粗，lighter：较细）
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
* 函数介绍：设置标题字体宽度Combox
* 输入参数：
* 输出参数：
* 返回值  ：
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


// CComponentEcharts 消息处理程序


BOOL CComponentEcharts::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	///标题居中（center）//居左（left）/居右显示（right）
	m_comboxTitleAlignment.InsertString(0,L"居中");
	m_comboxTitleAlignment.InsertString(1,L"居左");
	m_comboxTitleAlignment.InsertString(2,L"居右");
	m_comboxTitleAlignment.SetCurSel(0);

	//标题字体粗细：可选：（normal：正常，bold：较粗，bolder：特粗，lighter：较细）
	m_comboxTitleFontWeight.InsertString(0, L"正常");
	m_comboxTitleFontWeight.InsertString(1, L"粗体");
	m_comboxTitleFontWeight.InsertString(2, L"特粗");
	m_comboxTitleFontWeight.InsertString(3, L"较细");
	m_comboxTitleFontWeight.SetCurSel(0);

	m_colorbtTitleColor.SetColor(RGB(255,0,0));
	m_colorbtXColor.SetColor(RGB(0,255,255));
	m_colorbtYColor.SetColor(RGB(255,0,0));
	m_colorbtGridLineColor.SetColor(RGB(0,255,255));
	m_colorbtGridAreaColor.SetColor(RGB(0,0,255));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CComponentEcharts::OnBnClickedButtonSeletepoint()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	m_strBindPoint.Empty();
	UpdateData(FALSE);
}
