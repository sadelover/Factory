// ComponentEvaluationBarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComponentEvaluationBarDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"

#include "MainFrm.h"
#include "DataComConfigDlg.h"

// CComponentEvaluationBar 对话框

IMPLEMENT_DYNAMIC(CComponentEvaluationBar, CDialogEx)

CComponentEvaluationBar::CComponentEvaluationBar(CWnd* pParent /*=NULL*/)
	: CComponentDlgBase(CComponentEvaluationBar::IDD, pParent)
	, m_cstrTitle(_T("Title"))
	, m_cstrBindingPoint(_T(""))
	, m_fStart(-20)
	, m_fEnd(20)
	, m_cstrSegments(_T("-15,-10,-5,0,5,10,15"))
	, m_cstrAreaColor(_T("#EE0000,#EE0000, #F8D717,#54E368,#54E368, #F8D717,#EE0000,#EE0000"))
	, m_cstrArea(_T(""))
	, m_nSubType(0)
	, m_bBGclolor(FALSE)
{

}

CComponentEvaluationBar::~CComponentEvaluationBar()
{
}

void CComponentEvaluationBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Edit_Title, m_cstrTitle);
	DDX_Text(pDX, IDC_EDIT_BindingPoint, m_cstrBindingPoint);
	DDX_Text(pDX, IDC_EDIT_START, m_fStart);
	DDX_Text(pDX, IDC_EDIT_END, m_fEnd);
	DDX_Text(pDX, IDC_EDIT_Segments, m_cstrSegments);
	DDX_Text(pDX, IDC_EDIT_AreaColor, m_cstrAreaColor);
	DDX_Text(pDX, IDC_EDIT_nColor, m_cstrArea);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_nColor, m_colorBTArreaColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_ThemesColor, m_colorbtThemesColor);
	DDX_Radio(pDX, IDC_RADIO_Default, m_nSubType);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_BgColor, m_colorbtBGColor);
	DDX_Check(pDX, IDC_CHECK_BgAllow, m_bBGclolor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_ARROW, m_colorbtArrow);
}


/************************************************************************/
/* 函数说明：控件刷新函数                                               */
/************************************************************************/
void CComponentEvaluationBar::RefreshControls(Json::Value jsonRoot)
{
	if(!jsonRoot.isMember("type"))
	{
		return;
	}
	if(jsonRoot["type"] == "evaluationBar")
	{
		//subType
		if(jsonRoot.isMember("subType"))
		{
			string strSubType;
			strSubType = jsonRoot["subType"].asString();
			if(strSubType == "default")
			{
				m_nSubType = 0;
			}
			else if(strSubType == "heat balance")
			{
				m_nSubType = 1;
			}
			else if(strSubType == "efficiency")
			{
				m_nSubType = 2;  
			}
			else
			{
				m_nSubType = 0;
			}
		}

		//start & end
		if(jsonRoot.isMember("start"))
		{
			if(jsonRoot["start"].isDouble())
			{
				m_fStart = jsonRoot["start"].asDouble();
			}
		}
		if(jsonRoot.isMember("end"))
		{
			if(jsonRoot["end"].isDouble())
			{
				m_fEnd	= jsonRoot["end"].asDouble();
			}
		}
			
		
		int njsonArraySize;
		//segments
		m_cstrSegments.Empty();
		if(jsonRoot.isMember("segments"))
		{
			njsonArraySize = jsonRoot["segments"].size();
			float fSegment;
			for(int i = 0; i<njsonArraySize; ++i)
			{
				fSegment = jsonRoot["segments"][i].asDouble();
				CString tempCstr;
				tempCstr.Format(L"%.2f", fSegment);
				m_cstrSegments += tempCstr;
				m_cstrSegments += L',';
			}
			if(int nDel = m_cstrSegments.Find(L',', m_cstrSegments.GetLength()-1))
			{
				m_cstrSegments.Delete(nDel);
			}
		}

		//color
		m_cstrAreaColor.Empty();
		if(jsonRoot.isMember("color"))
		{
			njsonArraySize = jsonRoot["color"].size();
			for(int i = 0; i<njsonArraySize; ++i)
			{
				m_cstrAreaColor += Project::Tools::AnsiToWideChar(jsonRoot["color"][i].asString().c_str()).c_str();
				m_cstrAreaColor += L',';
			}
			if(int nDel = m_cstrAreaColor.Find(L',', m_cstrAreaColor.GetLength()-1))
			{
				m_cstrAreaColor.Delete(nDel);
			}
		}

		
		if(jsonRoot.isMember("bindPoint"))
			m_cstrBindingPoint = Project::Tools::AnsiToWideChar(jsonRoot["bindPoint"].asString().c_str()).c_str();
		
		if(jsonRoot.isMember("title"))
			m_cstrTitle = Project::Tools::AnsiToWideChar(jsonRoot["title"].asString().c_str()).c_str();
		
		if(jsonRoot.isMember("themes"))
		{
			if(jsonRoot["themes"].isString())
			{
				CString cstrColorTemp;
				cstrColorTemp = Project::Tools::AnsiToWideChar(jsonRoot["themes"].asString().c_str()).c_str();
				m_colorbtThemesColor.SetColor(CstrRGBToUIntBGR(cstrColorTemp));
			}
		}
		
		if(jsonRoot.isMember("bgcolor"))
		{
			if(jsonRoot["bgcolor"].isString())
			{
				CString cstrColorTemp;
				cstrColorTemp = Project::Tools::AnsiToWideChar(jsonRoot["bgcolor"].asString().c_str()).c_str();	
				m_colorbtBGColor.SetColor(CstrRGBToUIntBGR(cstrColorTemp));
			}
			m_bBGclolor = true;
		}
		else
		{
			m_bBGclolor = false;
		}

		if(jsonRoot.isMember("caretDownColor"))
		{
			if(jsonRoot["caretDownColor"].isString())
			{
				CString cstrColorTemp;
				cstrColorTemp = Project::Tools::AnsiToWideChar(jsonRoot["caretDownColor"].asString().c_str()).c_str();		
				m_colorbtArrow.SetColor(CstrRGBToUIntBGR(cstrColorTemp));
			}
		}
	}
	UpdateData(FALSE);
}

Json::Value CComponentEvaluationBar::GenerateJson()
{
	UpdateData(TRUE);
	/*打包*/
	Json::Value JsonValue;
	string strCStrToStr;

	JsonValue["type"] = "evaluationBar";

	if(m_nSubType == 0)
		JsonValue["subType"] = "default";
	else if(m_nSubType == 1)
		JsonValue["subType"] = "heat balance";   
	else if(m_nSubType == 2)
		JsonValue["subType"] = "efficiency";  

	if(m_fEnd > m_fStart)
	{
		JsonValue["start"] = m_fStart;
		JsonValue["end"]   = m_fEnd;
	}
	else
	{
		AfxMessageBox(L"开始值要小于于结束值");
		JsonValue = Json::Value::null;
		return JsonValue;
	}

	//刻度
	vector<CString> segmentsList;
	vector<CString>::const_iterator it;
	Project::Tools::SplitStringByChar(m_cstrSegments, L',', segmentsList);
	//分区颜色
	vector<CString> areaColorList;
	vector<CString>::const_iterator itc;
	Project::Tools::SplitStringByChar(m_cstrAreaColor, L',', areaColorList);
	if(segmentsList.size() == (areaColorList.size()-1))
	{
		it = segmentsList.begin();
		for( ;it != segmentsList.end(); ++it)
		{
			JsonValue["segments"].append(_ttof(*it));
		}

		itc = areaColorList.begin();
		for(int i = 0;i<areaColorList.size();i++)
		{
			string strTemp;
			strTemp = Project::Tools::WideCharToAnsi(areaColorList[i].GetString());
			JsonValue["color"].append(strTemp);
		}
	}
	else if(segmentsList.size() <  areaColorList.size() )
	{
		int i= areaColorList.size()-segmentsList.size()-1;
		CString cstrMessage;
		cstrMessage.Format(L"分区颜色多了%d个,分区颜色数量应比刻度数量多一个",i);
		AfxMessageBox(cstrMessage);
		JsonValue = Json::Value::null;
		return JsonValue;
	}
	else if(segmentsList.size()>areaColorList.size())
	{
		 int i = segmentsList.size() - areaColorList.size()+1;
		 CString cstrMessage;
		 cstrMessage.Format(L"分区颜色数量少了%d个，分区颜色数量应比刻度数量多一个",i);
		AfxMessageBox(cstrMessage);
		JsonValue = Json::Value::null;
		return JsonValue;
	}
	else if(segmentsList.size() == areaColorList.size())
	{
		AfxMessageBox(L"分区颜色不应等于刻度数量");
		JsonValue = Json::Value::null;
		return JsonValue;
	}
	strCStrToStr = Project::Tools::WideCharToAnsi(m_cstrBindingPoint.GetString());
	JsonValue["bindPoint"] = strCStrToStr;

	strCStrToStr = Project::Tools::WideCharToAnsi(m_cstrTitle.GetString());
	JsonValue["title"]  = strCStrToStr;

	JsonValue["themes"]  = UIntBGRToStrRGB(m_colorbtThemesColor.GetColor());

	if(m_bBGclolor == true)
	{
		UINT nColor = m_colorbtBGColor.GetColor();
		JsonValue["bgcolor"]  = UIntBGRToStrRGB(nColor);
	}

	UINT nColor = m_colorbtArrow.GetColor();
	JsonValue["caretDownColor"]  = UIntBGRToStrRGB(nColor);

	return JsonValue;
}

UINT CComponentEvaluationBar::CstrRGBToUIntBGR(CString cstrRGB)
{
	cstrRGB.Delete(cstrRGB.Find(L'#',0));
	int nRGB = wcstol(cstrRGB, NULL, 16);
	/*int nRGB = (UINT)_ttoi(cstrRGB);*/
	UINT R = (nRGB & 0xFF0000) >> 16;
	UINT G = nRGB & 0x00FF00;
	UINT B = (nRGB & 0x0000FF) << 16;
	return B|G|R;
}

string CComponentEvaluationBar::UIntBGRToStrRGB(UINT nBGRColor)
{
	CString cstrColor;
	string strCStrToStr;
	UINT R = (nBGRColor & 0x0000FF) << 16;
	UINT G = nBGRColor & 0x00FF00;
	UINT B = (nBGRColor & 0xFF0000) >> 16;
	cstrColor.Format(L"#%06X",R|G|B);
	strCStrToStr = Project::Tools::WideCharToAnsi(cstrColor.GetString());
	return strCStrToStr;
}

CString CComponentEvaluationBar::UIntBGRToCStrRGB(UINT nBGRColor)
{
	CString cstrColor;
	string strCStrToStr;
	UINT R = (nBGRColor & 0x0000FF) << 16;
	UINT G = nBGRColor & 0x00FF00;
	UINT B = (nBGRColor & 0xFF0000) >> 16;
	cstrColor.Format(L"#%06X",R|G|B);
	return cstrColor;
}

BEGIN_MESSAGE_MAP(CComponentEvaluationBar, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Generate, &CComponentEvaluationBar::OnBnClickedButtonGenerate)
//	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_MFCCOLORBUTTON_BKColor, &CComponentEvaluationBar::OnBnHotItemChangeMfccolorbuttonBkcolor)
ON_BN_CLICKED(IDC_CHECK_BgAllow, &CComponentEvaluationBar::OnBnClickedCheckBgallow)
ON_BN_CLICKED(IDC_BUTTON_SELETEPOINT, &CComponentEvaluationBar::OnBnClickedButtonSeletepoint)
ON_BN_CLICKED(IDC_BUTTON_CLEARPOINT, &CComponentEvaluationBar::OnBnClickedButtonClearpoint)
END_MESSAGE_MAP()


// CComponentEvaluationBar 消息处理程序

void CComponentEvaluationBar::OnBnClickedButtonGenerate()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//m_nArea;
	//m_colorBTArreaColor
	int nColor = m_colorBTArreaColor.GetColor();
	m_cstrArea = UIntBGRToCStrRGB(nColor);
	UpdateData(FALSE);
}

void CComponentEvaluationBar::OnBnClickedCheckBgallow()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CComponentEvaluationBar::OnBnClickedButtonSeletepoint()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_cstrBindingPoint = dlg.m_strPointName;
	}
	UpdateData(FALSE);
}


void CComponentEvaluationBar::OnBnClickedButtonClearpoint()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cstrBindingPoint.Empty();
	UpdateData(FALSE);
}
