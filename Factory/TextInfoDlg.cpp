// TestInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TextInfoDlg.h"
#include "DataComConfigDlg.h"
#include <afxwin.h>
#include "DataPoint/DataPointManager.h"
// CTextInfoDlg dialog
#include "MainFrm.h"
#include "../ComponentDraw/EqmDrawText.h"
#include "../ComponentDraw/ParseStringParam3.h"
#include "afxtoolbarfontcombobox.h"
#include "SheetDeviceArea.h"

IMPLEMENT_DYNAMIC(CTextInfoDlg, CSkinDlg)
    ;
BEGIN_MESSAGE_MAP(CTextInfoDlg, CSkinDlg)
    ON_BN_CLICKED(IDOK, &CTextInfoDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_ChooseDataCom, &CTextInfoDlg::OnBnClickedButtonChoosedatacom)	
    ON_STN_CLICKED(IDC_ChooseTEXTBORDERCOLOR, &CTextInfoDlg::OnStnClickedChoosetextbordercolor)
    ON_STN_CLICKED(IDC_STATIC_Choosecolor,&CTextInfoDlg::OnStnClickedStaticChoosecolor)
    ON_CBN_SELCHANGE(IDC_COMBO_BORDERTYPE, &CTextInfoDlg::OnCbnSelchangeComboBordertype)
	ON_BN_CLICKED(IDC_BUTTON_FONTSELECT, &CTextInfoDlg::OnBnClickedButtonFontselect)
	ON_CBN_SELCHANGE(IDC_COMBO_TextMode, &CTextInfoDlg::OnCbnSelchangeComboTextmode)
	ON_BN_CLICKED(IDC_BUTTON_ChooseData_time, &CTextInfoDlg::OnBnClickedButtonChoosedatatime)
	ON_BN_CLICKED(IDC_BUTTON_ChooseData_point, &CTextInfoDlg::OnBnClickedButtonChoosedatapoint)

	ON_BN_CLICKED(IDC_BTN_TEMP_FIND, &CTextInfoDlg::OnBnClickedBtnTemperFind)
	ON_BN_CLICKED(IDC_BTN_TEMP_FIND, &CTextInfoDlg::OnBnClickedBtnTempFind)
	ON_EN_CHANGE(IDC_EDIT_TEXT_SCRIPT, &CTextInfoDlg::OnEnChangeEditTextScript)
	ON_EN_CHANGE(IDC_EDIT3, &CTextInfoDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT_DATETIME_TIME, &CTextInfoDlg::OnEnChangeEditDatetimeTime)
	ON_EN_CHANGE(IDC_EDIT_DATETIME_POINT, &CTextInfoDlg::OnEnChangeEditDatetimePoint)
	ON_EN_CHANGE(IDC_EDIT_TEXT_MODE_STRING, &CTextInfoDlg::OnEnChangeEditTextModeString)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGN, &CTextInfoDlg::OnCbnSelchangeComboAlign)
	ON_CBN_SELCHANGE(IDC_COMBO_BKTYPE, &CTextInfoDlg::OnCbnSelchangeComboBktype)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CTextInfoDlg::CTextInfoDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CTextInfoDlg::IDD, pParent)
	, m_strText(_T(""))
	, m_nColor(0)
	, m_nFontSize(0)
	, m_fontname(_T("微软雅黑"))
	, m_showmode(1)
	, m_strPlcValue(_T(""))
    ,m_layer(E_LAYER_10)
	, m_TextBorderType(1)
    ,m_align(0)
	,m_nID(0)
	, m_nTextBKType(0)
	, m_nTextAttri(TEXT_ATTR_NO_USE)
	, m_strBindScript(_T(""))
	, m_cstrUnit(_T(""))
{
	m_pointNum = 0;
	m_pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(m_pMainFrm != NULL);
}

CTextInfoDlg::~CTextInfoDlg()
{
}

void CTextInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strText);
	//	DDX_CBString(pDX, IDC_COMBO_TextMode, m_combostring);

	DDX_Text(pDX, IDC_EDIT3, m_strPlcValue);
	DDX_Control(pDX, IDC_COMBO_TextMode, m_comboshowmode);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_Control(pDX, IDC_ChooseTEXTBORDERCOLOR, m_choosetextbordercolor);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERTYPE, m_TextBorderType);
	DDX_Text(pDX,IDC_EDIT_FONTSIZE,m_nFontSize);
	DDX_Control(pDX,IDC_STATIC_Choosecolor,m_choostcolorstatic);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_ALIGN, m_comboAlign);
	DDX_Text(pDX, IDC_EDIT_FONTNAME, m_fontname);
	DDX_Text(pDX, IDC_EDIT_NUM_AFTER_POINT, m_pointNum);
	DDV_MinMaxInt(pDX, m_pointNum, 0, 4);
	DDX_CBIndex(pDX, IDC_COMBO_BKTYPE, m_nTextBKType);
	DDX_Control(pDX, IDC_COMBO_BORDERTYPE, m_comboBorderType);
	DDX_Control(pDX, IDC_COMBO_TEXT_ATTRI, m_comboTextAttri);
	DDX_CBIndex(pDX, IDC_COMBO_TEXT_ATTRI, m_nTextAttri);
	DDX_Text(pDX, IDC_EDIT_TEXT_SCRIPT, m_strBindScript);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_cstrUnit);
}

// CTextInfoDlg message handlers

void CTextInfoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strTemp;
	GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->GetWindowText(strTemp);
	CString str;
	m_showmode = (TEXTSHOWMODE)m_comboshowmode.GetCurSel();
	if (m_showmode == MODE_ENUM)
	{
		if (strTemp.GetLength() == 0)
		{
			AfxMessageBox(L"请配置显示字符串之后再试!!");
			return;
		}
		CParseSplitString& instance = CParseSplitString::Instance();
		if (instance.IsExist(m_nID))
		{
			instance.DeleteOne(m_nID);
		}
	}
	if (m_showmode == MODE_TIME)
	{
		GetDlgItem(IDC_EDIT_DATETIME_TIME)->GetWindowText(strTemp);
		if (strTemp.GetLength() == 0)
		{
			AfxMessageBox(L"请配置时间日期点之后再试!!");
			return;
		}
		GetDlgItem(IDC_EDIT_DATETIME_POINT)->GetWindowText(strTemp);
		if (strTemp.GetLength() == 0)
		{
			AfxMessageBox(L"请配置时间序列点之后再试!!");
			return;
		}
	}
	GetDlgItem(IDC_EDIT_DATETIME_TIME)->GetWindowText(m_strDateTime);
	GetDlgItem(IDC_EDIT_DATETIME_POINT)->GetWindowText(m_strDateTimePoint);
	m_strTextModeString = strTemp;
    m_layer = m_comboLayer.GetCurSel();
    m_align = m_comboAlign.GetCurSel();

	if (m_pMainFrm != NULL)
	{
		int nLen = m_strText.GetLength();
		if (nLen > m_pMainFrm->m_nItemNameLen)
		{
			CString strErr;
			strErr.Format(_T("名称长度不能大于%d！"), m_pMainFrm->m_nItemNameLen);
			MessageBox(strErr, _T("错误"), MB_OK|MB_ICONERROR);
			CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT1));
			if (pEdit != NULL)
			{
				pEdit->SetSel(0, -1);
				pEdit->SetFocus();
			}
		}
		/*
		if (m_strDeviceCfg != _T(""))
		{
			vector<TextDeviceInfo>::iterator iter;
			for (iter=m_pMainFrm->m_pPrjSqlite->m_vecTextDevInf.begin();
				iter!=m_pMainFrm->m_pPrjSqlite->m_vecTextDevInf.end();
				iter++)
			{
				if (m_strDeviceCfg == iter->strBindPageName)
				{
					iter->nBindTextId = m_nID;
					break;
				}
			}
		}
		*/
	}

	return CDialog::OnOK();
}

BOOL CTextInfoDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();
	m_choosetextbordercolor.SetBkColor(m_nBorderColor,m_nBorderColor,CSkinStatic::Normal).SetTransparent(FALSE);
	m_choostcolorstatic.SetBkColor(m_nColor,m_nColor,CSkinStatic::Normal).SetTransparent(FALSE);
    InitComboLayer();
    InitComboAlign();
	InitComboMode();
	InitComboBorderType();
	InitComboTextAttribute();//m_showmode
	if (m_showmode == 0)//文本
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
	}
	else if(m_showmode == 1)//值
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
	}
	else if (m_showmode == 2)//时间
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
	}
	else if (m_showmode == 3)//字符串
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
	}
	else if (m_showmode == 4)//按间隔加减修改值模式
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
	}
	else if (m_showmode == 5)//值模式带单位显示
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_SHOW);
	}

	GetDlgItem(IDC_EDIT_DATETIME_TIME)->SetWindowText(m_strDateTime);
	GetDlgItem(IDC_EDIT_DATETIME_POINT)->SetWindowText(m_strDateTimePoint);
	GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->SetWindowTextW(m_strTextModeString);
	m_choosetextbordercolor.EnableWindow(m_TextBorderType==2||m_TextBorderType==3||m_TextBorderType==4);
	m_choosetextbordercolor.Invalidate(TRUE);
	m_btnCancel.SetButtonProjectStyle(Silver);
	//GetDlgItem(IDC_BUTTON_FONTSELECT)->EnableWindow(0);
	TextBcakgroundUPdate(m_nTextBKType);
	m_nTempTextBKType = m_nTextBKType;
	UpdateData(FALSE);
	return TRUE;
}

void CTextInfoDlg::OnStnDblclickStaticChoosecolor()
{
	CColorDialog dlg(m_nColor);

	if (dlg.DoModal() == IDOK)
	{
		m_nColor = dlg.GetColor();
		m_choostcolorstatic.SetBkColor(m_nColor,m_nColor, CSkinStatic::Normal);
	}
}

void CTextInfoDlg::OnBnClickedButtonChoosedatacom()
{
	UpdateData(TRUE);
	
	CDataComConfigDlg dlg(*m_refpointmap);
	//dlg.m_selectedId = m_nComID;
	dlg.m_strPointName = m_strPlcValue;
	
	if (dlg.DoModal() == IDOK)
	{
		m_strPlcValue = dlg.m_strPointName;

		UpdateData(FALSE); //value to control

		InvalidateRect(NULL);
	}
}

TEXTSHOWMODE CTextInfoDlg::GetShowMode()
{
	return static_cast<TEXTSHOWMODE>(m_showmode);
}

void  CTextInfoDlg::InitComboLayer()
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
    m_comboLayer.SetCurSel(m_layer);
}

void CTextInfoDlg::InitComboAlign()
{
    m_comboAlign.ResetContent();
    m_comboAlign.AddString(L"左中对齐");
    m_comboAlign.AddString(L"右中对齐");
    m_comboAlign.AddString(L"中中对齐");
	m_comboAlign.AddString(L"左上对齐");
	m_comboAlign.AddString(L"右上对齐");
	m_comboAlign.AddString(L"中上对齐");
	m_comboAlign.AddString(L"左下对齐");
	m_comboAlign.AddString(L"右下对齐");
	m_comboAlign.AddString(L"中下对齐");
    m_comboAlign.SetCurSel(m_align);
}

void CTextInfoDlg::OnStnClickedChoosetextbordercolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(m_nBorderColor);

	if (dlg.DoModal() == IDOK)
	{
		m_nBorderColor = dlg.GetColor();
		m_choosetextbordercolor.SetBkColor(m_nBorderColor,m_nBorderColor, CSkinStatic::Normal);
	}
}
void CTextInfoDlg::OnStnClickedStaticChoosecolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(m_nColor);

	if (dlg.DoModal() == IDOK)
	{
		m_nColor = dlg.GetColor();
		m_choostcolorstatic.SetBkColor(m_nColor,m_nColor, CSkinStatic::Normal);
	}
}

void CTextInfoDlg::OnCbnSelchangeComboBordertype()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_choosetextbordercolor.EnableWindow(m_TextBorderType==2||m_TextBorderType==3||m_TextBorderType==4);

	if (0 == m_TextBorderType || 1 == m_TextBorderType)
	{
		m_nBorderColor = RGB(128, 128, 128);
		m_choosetextbordercolor.SetBkColor(m_nBorderColor, m_nBorderColor, CSkinStatic::Normal).SetTransparent(FALSE);
		UpdateData(FALSE);
	}

	m_choosetextbordercolor.Invalidate(TRUE);
}

void CTextInfoDlg::SetPointMap( CDataPointManager* pointmap )
{
	m_refpointmap = pointmap;
}


void CTextInfoDlg::OnBnClickedButtonFontselect()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	LOGFONT logfont;
	memset(&logfont, 0x00, sizeof(logfont));
	wcscpy_s(logfont.lfFaceName, m_fontname.GetString());
	CDC* dc = this->GetDC();
	logfont.lfHeight =  MulDiv(m_nFontSize, GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSY), 72);
	ReleaseDC(dc);

	CFontDialog dlg(&logfont);
	dlg.m_cf.rgbColors = m_nColor;
	CString str_font;

	if(dlg.DoModal()==IDOK)
	{
		str_font = dlg.GetFaceName();
		//////////////////////////////////////////////////////////////////////////
		Gdiplus::FontFamily fontFamily(str_font);
		if(!fontFamily.IsAvailable()){
			MessageBox(_T("您的系统未安装该字体"),NULL,MB_OK);
			return;
		}
		//
		m_fontname= dlg.GetFaceName();
		m_nFontSize = dlg.GetSize()/10;
		m_nColor = dlg.GetColor();
	}

	m_choostcolorstatic.SetBkColor(m_nColor,m_nColor,CSkinStatic::Normal);
	UpdateData(FALSE);
}

void CTextInfoDlg::InitComboMode()
{
	m_comboshowmode.InsertString(0,L"枚举模式");
	m_comboshowmode.InsertString(1,L"值模式");
	m_comboshowmode.InsertString(2,L"时间模式");
	m_comboshowmode.InsertString(3,L"字符串模式");
	m_comboshowmode.InsertString(4,L"按间隔加减修改值模式");
	m_comboshowmode.InsertString(5,L"值模式带单位显示");
	m_comboshowmode.SetCurSel(m_showmode);
}

void CTextInfoDlg::OnCbnSelchangeComboTextmode()
{//IDC_EDIT_UNIT
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_comboshowmode.GetCurSel();
	if (nIndex == 0)//文本
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BKTYPE)->EnableWindow(TRUE);
		//背景图处理，从备份中读出值
		m_nTextBKType = m_nTempTextBKType;
		TextBcakgroundUPdate(m_nTextBKType);
	}
	else if(nIndex == 1)//值
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BKTYPE)->EnableWindow(TRUE);

		m_nTextBKType = m_nTempTextBKType;
		TextBcakgroundUPdate(m_nTextBKType);
	}
	else if (nIndex == 2)//时间
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BKTYPE)->EnableWindow(TRUE);

		m_nTextBKType = m_nTempTextBKType;
		TextBcakgroundUPdate(m_nTextBKType);
	}
	else if (nIndex == 3)//字符串
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BKTYPE)->EnableWindow(TRUE);

		m_nTextBKType = m_nTempTextBKType;
		TextBcakgroundUPdate(m_nTextBKType);
	}
	else if (nIndex == 4)//加减按钮模式
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BKTYPE)->EnableWindow(FALSE);
		//取消背景图
		m_nTextBKType = 0;
		TextBcakgroundUPdate(m_nTextBKType);
	}
	else if (nIndex == 5)//单位显示模式
	{
		GetDlgItem(IDC_EDIT_TEXT_MODE_STRING)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUM_AFTER_POINT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ChooseData_time)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseData_point)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ChooseDataCom)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_BKTYPE)->EnableWindow(FALSE);
		
		m_nTextBKType = 0;
		TextBcakgroundUPdate(m_nTextBKType);
	}
	UpdateData(FALSE);
}


void CTextInfoDlg::OnBnClickedButtonChoosedatatime()
{
	CDataComConfigDlg dlg(*m_refpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strDateTime = dlg.m_strPointName;
		GetDlgItem(IDC_EDIT_DATETIME_TIME)->SetWindowText(m_strDateTime);
	}
}


void CTextInfoDlg::OnBnClickedButtonChoosedatapoint()
{
	CDataComConfigDlg dlg(*m_refpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strDateTimePoint = dlg.m_strPointName;
		GetDlgItem(IDC_EDIT_DATETIME_POINT)->SetWindowText(m_strDateTimePoint);
	}
}

void CTextInfoDlg::InitComboBorderType()
{
	m_comboBorderType.AddString(_T("无边框"));
	m_comboBorderType.AddString(_T("普通边框"));
	m_comboBorderType.AddString(_T("黑框白底"));
	m_comboBorderType.AddString(_T("3D凹边框"));
	m_comboBorderType.AddString(_T("3D凸边框"));
	m_comboBorderType.SetCurSel(m_TextBorderType);
	m_comboBorderType.EnableWindow(FALSE);	// useless now
}

void CTextInfoDlg::InitComboTextAttribute()
{
	m_comboTextAttri.AddString(_T("无需配置"));
	m_comboTextAttri.AddString(_T("温度"));
	m_comboTextAttri.AddString(_T("湿度"));
	m_comboTextAttri.AddString(_T("SP"));
	m_comboTextAttri.SetCurSel(0);
}




void CTextInfoDlg::OnEnChangeEditTextScript()
{
	/*
	CDataComConfigDlg dlg(*m_refpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strDateTimePoint = dlg.m_strPointName;
		GetDlgItem(IDC_EDIT_DATETIME_POINT)->SetWindowText(m_strBindScript);
	}
	*/
	
}

void CTextInfoDlg::OnBnClickedBtnTemperFind()
{

}

void CTextInfoDlg::OnBnClickedBtnTempFind()
{

}



void CTextInfoDlg::OnEnChangeEditDatetimePoint()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CSkinDlg::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
void CTextInfoDlg::OnEnChangeEdit3()
{

}
void CTextInfoDlg::OnEnChangeEditDatetimeTime()
{

}

void CTextInfoDlg::OnEnChangeEditTextModeString()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CSkinDlg::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CTextInfoDlg::OnCbnSelchangeComboAlign()
{
	// TODO: Add your control notification handler code here
}
/*
*文本背景选择
*nTextBG：0-N
*/
void CTextInfoDlg::TextBcakgroundUPdate(int nTextBG )
{
	CString strImagePath;
	CString m_strSysPath;
	Project::Tools::GetSysPath(m_strSysPath);//获取运行地址

	//获取到DC
	CStatic * pStatic = (CStatic *)GetDlgItem(IDC_STATIC_TextBG);

	if (nTextBG == 0)
		strImagePath = L"";
	else if (nTextBG == 1)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_1.png" ;
	else if (nTextBG == 2)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_2.png" ;
	else if (nTextBG == 3)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_3.png" ;
	else if (nTextBG == 4)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_4.png" ;
	else if (nTextBG == 5)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_5.png" ;
	else if (nTextBG == 6)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_6.png" ;
	else if (nTextBG == 7)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_7.png" ;
	else if (nTextBG == 8)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_8.png" ;
	else if (nTextBG == 9)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_9.png" ;
	else if (nTextBG == 10)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_10.png";

	int wight = 0;
	int hight = 0;
	if(strImagePath != L"")
	{
		pStatic->ShowWindow(TRUE);
		image.Load(strImagePath);
		wight = image.GetWidth();
		hight = image.GetHeight();
		HBITMAP hBmp = image.Detach();
		pStatic->SetBitmap(hBmp);
		pStatic->SetWindowPos(NULL, 
			0, 
			0, 
			wight,
			hight, 
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	}else{
		pStatic->ShowWindow(FALSE);
	}
}

void CTextInfoDlg::OnCbnSelchangeComboBktype()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	TextBcakgroundUPdate(m_nTextBKType);
	m_nTempTextBKType = m_nTextBKType;
}
