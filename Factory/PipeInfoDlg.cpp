// PipeInfoDlg.cpp : implementation file
#include "stdafx.h"
#include "PipeInfoDlg.h"
#include "SelectDataComId.h"
#include "MetaFileSelectDlg.h"
#include "../ComponentDraw/EqmDrawDefines.h"

#include "Tools/Util/UtilString.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"
//////////////////////////////////////////////////////////////////////////
// CPipeInfoDlg dialog

IMPLEMENT_DYNAMIC(CPipeInfoDlg, CSkinDlg)

BEGIN_MESSAGE_MAP(CPipeInfoDlg, CSkinDlg)
    ON_STN_CLICKED(IDC_STATIC_INNERCOLOR, &CPipeInfoDlg::OnStnClickedStaticInnercolor)
    ON_STN_CLICKED(IDC_STATIC_SIDECOLOR, &CPipeInfoDlg::OnStnClickedStaticSidecolor)
    ON_CBN_SELCHANGE(IDC_COMBO_PipeWidth, &CPipeInfoDlg::OnCbnSelchangeComboPipewidth)
    ON_BN_CLICKED(IDOK, &CPipeInfoDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_SELECTID, &CPipeInfoDlg::OnBnClickedButtonSelectid)
    ON_BN_CLICKED(IDC_BUTTON_OPPOSITE, &CPipeInfoDlg::OnBnClickedButtonOpposite)
	ON_BN_CLICKED(IDC_BUTTON_PIPE_CLEAR, &CPipeInfoDlg::OnBnClickedButtonPipeClear)
	ON_BN_CLICKED(IDC_BUTTON_ADD_IMAGE, &CPipeInfoDlg::OnBnClickedButtonAddImage)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CPipeInfoDlg::OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDC_BUTTON_TEST_SCRIPT, &CPipeInfoDlg::OnBnClickedButtonTestScript)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
CPipeInfoDlg::CPipeInfoDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CPipeInfoDlg::IDD, pParent)
    ,m_layer(E_LAYER_10)
	, m_strID(_T(""))
	,m_nDirection(0)
	,m_nLogic(0)
	, m_nPipeType(0)
	, m_nPipeBitMapID(0)
	, m_nPipeDenSity(0)
	, m_nPipeSpeed(0)
	,m_WaterFashion(1)
{
	m_pipewidth = DEFAULT_PIPE_WIDTH;
}

CPipeInfoDlg::~CPipeInfoDlg()
{
}

void CPipeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INNERCOLOR, m_stcInnerColor);
	DDX_Control(pDX, IDC_STATIC_SIDECOLOR, m_stcSideColor);
	DDX_Control(pDX, IDC_COMBO_PipeWidth, m_combo_pipewidth);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_LOGIC_LINE, m_combo_logic);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nPipeType);
	DDX_Text(pDX, IDC_EDIT_IMAGE, m_nPipeBitMapID);
	DDX_CBIndex(pDX, IDC_COMBO_DENSITY, m_nPipeDenSity);
	DDX_CBIndex(pDX, IDC_COMBO_SPEED, m_nPipeSpeed);
	DDX_Control(pDX, IDC_COMBO_WaterFashion, m_cWaterFashion);
}
// CPipeInfoDlg message handlers

void CPipeInfoDlg::OnStnClickedStaticInnercolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(m_nInnerColor);

	if (dlg.DoModal() == IDOK)
	{
		m_nInnerColor = dlg.GetColor();
		m_stcInnerColor.SetBkColor(m_nInnerColor, m_nInnerColor, CSkinStatic::Normal);
	}
}

void CPipeInfoDlg::OnStnClickedStaticSidecolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(m_nSideColor);

	if (dlg.DoModal() == IDOK)
	{
		m_nSideColor = dlg.GetColor();
		m_stcSideColor.SetBkColor(m_nSideColor, m_nSideColor, CSkinStatic::Normal);
	}
}
/************************************************************************
CPipeInfoDlg界面初始化
***********************************************************************/
BOOL CPipeInfoDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();


	m_stcInnerColor.SetBkColor(m_nInnerColor, m_nInnerColor, CSkinStatic::Normal).SetTransparent(FALSE);
	m_stcSideColor.SetBkColor(m_nSideColor, m_nSideColor, CSkinStatic::Normal).SetTransparent(FALSE);

	if(m_nPipeType>5)
	{

		m_stcInnerColor.ShowWindow(TRUE);
		m_stcSideColor.ShowWindow(TRUE);
	}
	else
	{
		m_stcInnerColor.ShowWindow(FALSE);
		m_stcSideColor.ShowWindow(FALSE);
	}



	m_combo_pipewidth.SetCurSel(m_pipewidth - 1);

    InitComboLayer();
	InitSelectID();
	InitComboLogic();
	m_btnCancel.SetButtonProjectStyle(Silver);

	if (m_nDirection == 0)
	{
		GetDlgItem(IDC_BUTTON_OPPOSITE)->SetWindowTextW(L"正向");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OPPOSITE)->SetWindowTextW(L"反向");
	}
	m_combo_logic.SetCurSel(m_nLogic);
	InitWaterFashionComboBox();//初始化水流形状的Combo
	return TRUE;
}

void CPipeInfoDlg::InitSelectID()
{
	CString str;
	m_strID = _T("");
	for(UINT idx=0; idx< m_vecDataName.size(); idx++)
	{
		str = m_vecDataName[idx];
		if (idx != m_vecDataName.size()-1)
		{
			str += _T(",");
		}
		m_strID += str;
	}
	UpdateData(FALSE);
}
void CPipeInfoDlg::OnCbnSelchangeComboPipewidth()
{
	// TODO: Add your control notification handler code here
	
	m_pipewidth = m_combo_pipewidth.GetCurSel() + 1;
}

void  CPipeInfoDlg::InitComboLayer()
{
    m_comboLayer.ResetContent();
	m_comboLayer.AddString(L"1层");
	m_comboLayer.AddString(L"2层");
	m_comboLayer.AddString(L"3层");
	m_comboLayer.AddString(L"4层");
	m_comboLayer.AddString(L"5层");
	m_comboLayer.AddString(L"6层");
	m_comboLayer.AddString(L"7层");
	m_comboLayer.AddString(L"8层");
	m_comboLayer.AddString(L"9层");
	m_comboLayer.AddString(L"10层");

    //UpdateData(false); //value to control
    m_comboLayer.SetCurSel(m_layer);
}
//////////////////////////////////////////////////////////////////////////


void CPipeInfoDlg::OnBnClickedOk()
{
    UpdateData(TRUE);  //control to value

    m_layer = m_comboLayer.GetCurSel();
	m_nLogic = m_combo_logic.GetCurSel();

	CString strFlag(_T(","));
	CString strSrc(m_strID);
	strSrc.TrimRight(strFlag);
	strSrc += strFlag;
	m_vecDataName.clear();
	while (TRUE)
	{
		int nPos = strSrc.Find(strFlag);
		if (-1 == nPos)
		{
			break;
		}
		CString strTemp = strSrc.Left(nPos);
		m_vecDataName.push_back(strTemp);
		strSrc = strSrc.Mid(nPos + 1);
	}
	m_WaterFashion = m_cWaterFashion.GetCurSel();
    OnOK();
}

void CPipeInfoDlg::OnBnClickedButtonSelectid()
{
	// TODO: Add your control notification handler code here
	SelectDataComId dlg;
	dlg.m_vecDataName = m_vecDataName;
	dlg.SetPointMap(m_refpointmap);
	if(IDOK==dlg.DoModal())
	{
		m_vecDataName = dlg.m_vecDataName;
	}
	InitSelectID();
}

void CPipeInfoDlg::OnBnClickedButtonOpposite()
{
	// TODO: Add your control notification handler code here
	if (m_nDirection == 0)
	{
		GetDlgItem(IDC_BUTTON_OPPOSITE)->SetWindowTextW(L"反向");
		m_nDirection = 1;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OPPOSITE)->SetWindowTextW(L"正向");
		m_nDirection = 0;
	}
}

void CPipeInfoDlg::SetPointmap( CDataPointManager* pointmap )
{
	m_refpointmap = pointmap;
}

void CPipeInfoDlg::InitComboLogic()
{
	m_combo_logic.AddString(L"无");
	m_combo_logic.AddString(L"与");
	m_combo_logic.AddString(L"或");
}


void CPipeInfoDlg::OnBnClickedButtonPipeClear()
{
	m_vecDataName.clear();
	InitSelectID();
}


void CPipeInfoDlg::OnBnClickedButtonAddImage()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		m_nPipeBitMapID = dlg.m_nID;
		pFrame->m_nSelectPicID = m_nPipeBitMapID;
	}
	UpdateData(FALSE);
}


void CPipeInfoDlg::OnCbnSelchangeComboType()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_nPipeType>5)
	{

		m_stcInnerColor.ShowWindow(TRUE);
		m_stcSideColor.ShowWindow(TRUE);
	}
	else
	{
		m_stcInnerColor.ShowWindow(FALSE);
		m_stcSideColor.ShowWindow(FALSE);
	}
}
void CPipeInfoDlg::InitWaterFashionComboBox()
{
	m_cWaterFashion.InsertString(0,L"矩形");
	m_cWaterFashion.InsertString(1,L"圆形");
	m_cWaterFashion.SetCurSel(m_WaterFashion);
}

void CPipeInfoDlg::OnBnClickedButtonTestScript()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(Project::Tools::IsPointNameValid(m_strID) == true)
	{
		AfxMessageBox(L"点名不需要测算。");
		return;
	}
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	int nId = -1;
	CString strSrc;
	CString strResult;
	CHttpOperation http(pFrame->m_strCalcIp, pFrame->m_nCalcPort, _T("/tool/evalStringExpression"));
	wstring wstrTimeStart,wstrTimeEnd ;

	Json::Value myRoot;


	myRoot["str"] = Project::Tools::WideCharToUtf8(m_strID.GetString());

	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));//
	if(bRet)
	{
		Json::Reader jsReader;
		string strResult_utf8;
		Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
		Json::Value myReturnRoot;
		if (!jsReader.parse(strResult_utf8, myReturnRoot))
		{
			AfxMessageBox(_T("标准点名自动生成失败，原因是注释信息不充分"));
			return;
		}
		else
		{
			if(myReturnRoot.isObject())
			{
				Json::Value dataObj = myReturnRoot["err"];
				if(dataObj.isInt())
				{
					int nErr = dataObj.asInt();
					if(nErr==1)
					{
						CString strTemp;
						strTemp.Format(_T("公式格式有误，计算失败"));
						AfxMessageBox(strTemp);
					}
					else
					{
						Json::Value dataObj = myReturnRoot["data"];
						if(dataObj.isDouble())
						{
							CString strTemp;
							strTemp.Format(_T("结果为:%.4f"), dataObj.asDouble());
							AfxMessageBox(strTemp);
						}
						else if (dataObj.isInt())
						{
							CString strTemp;
							strTemp.Format(_T("结果为:%d"), dataObj.asInt());
							AfxMessageBox(strTemp);
						}else if (dataObj.isString())
						{
							CString strTemp;
							wstring wstr = Project::Tools::AnsiToWideChar(dataObj.asString().data());
							strTemp.Format(_T("结果为:%s"), wstr.c_str());
							AfxMessageBox(strTemp);
						}
					}

				}
				else{
					CString strTemp;
					strTemp.Format(_T("后台有误，计算失败"));
					AfxMessageBox(strTemp);
				}

			}
		}
	}
	else
	{
		AfxMessageBox(_T("公式计算失败"));
		return;
	}
}
