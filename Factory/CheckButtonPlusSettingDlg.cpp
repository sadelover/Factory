// CheckButtonPlusSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckButtonPlusSettingDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/EqmDrawButtonPlus.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"


const CString g_strCtrlType[2] = {_T("单选"), _T("多选")};


// CCheckButtonPlusSettingDlg dialog

IMPLEMENT_DYNAMIC(CCheckButtonPlusSettingDlg, CDialogEx)

CCheckButtonPlusSettingDlg::CCheckButtonPlusSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckButtonPlusSettingDlg::IDD, pParent)
	, m_nTxtSize(14)
	, m_nStrExpression(_T(""))
	, m_refpointmap(NULL)
	//, m_nChkBtnStyle(0)
	, m_nLayer(9)
	, m_eBtnStyle(E_RADIO_BUTTON)
	, m_strSelVal(_T(""))
	, m_strUnSelVal(_T(""))
	, m_strBindPointName(_T(""))
	, m_strTxtContent(_T(""))
	, m_nGroupId(0)
	, m_colorTxt(RGB(0, 0, 0))
	, m_cstrOptionsDec(_T(""))
{

}

CCheckButtonPlusSettingDlg::~CCheckButtonPlusSettingDlg()
{
}

void CCheckButtonPlusSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_comboStyle);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_Text(pDX, IDC_EDIT_TXT_SIZE, m_nTxtSize);
	DDX_Text(pDX, IDC_EDIT_REMARK, m_nStrExpression);
	//DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nChkBtnStyle);
	//DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
	DDX_Text(pDX, IDC_EDIT_SEL_VAL, m_strSelVal);
	DDX_Text(pDX, IDC_EDIT_UNSEL_VAL, m_strUnSelVal);
	DDX_Text(pDX, IDC_EDIT_BIND_POINT, m_strBindPointName);
	DDX_Text(pDX, IDC_EDIT_TXT_CONTENT, m_strTxtContent);
	DDX_Text(pDX, IDC_EDIT_GROUP_ID, m_nGroupId);
	DDX_Text(pDX, IDC_EDIT_Dec, m_cstrOptionsDec);
}


BEGIN_MESSAGE_MAP(CCheckButtonPlusSettingDlg, CDialogEx)
	ON_STN_CLICKED(IDC_TXT_COLOR, &CCheckButtonPlusSettingDlg::OnStnClickedTxtColor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_BIND_POINT, &CCheckButtonPlusSettingDlg::OnBnClickedBtnBindPoint)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &CCheckButtonPlusSettingDlg::OnCbnSelchangeComboStyle)
END_MESSAGE_MAP()


// CCheckButtonPlusSettingDlg message handlers


BOOL CCheckButtonPlusSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControl();

	InitImagePath();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCheckButtonPlusSettingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if (!UpdateData(TRUE))
	{
		return;
	}

	m_nLayer = m_comboLayer.GetCurSel();
	m_eBtnStyle = (eButtonStyle)m_comboStyle.GetCurSel();
	int nLen = m_strTxtContent.GetLength();
	if (nLen > 99)
	{
		MessageBox(_T("文本描述字符串长度需小于99个字符！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	if (m_nGroupId < 0)
	{
		MessageBox(_T("组必须大于等于0！"), _T("错误"), MB_OK|MB_ICONERROR);

		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_GROUP_ID);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return;
	}


	CDialogEx::OnOK();
}

void CCheckButtonPlusSettingDlg::SetPointMap(CDataPointManager* pointmap)
{
	assert(pointmap != NULL);
	if (pointmap != NULL)
	{
		m_refpointmap = pointmap;
	}
}

void CCheckButtonPlusSettingDlg::SetLayerId(const int nLayerId)
{
	m_nLayer = nLayerId;
}

void CCheckButtonPlusSettingDlg::SetButtonStyle(const eButtonStyle eBtnStyle)
{
	m_eBtnStyle = eBtnStyle;
}

void CCheckButtonPlusSettingDlg::SetTextColor(const COLORREF colorTxt)
{
	m_colorTxt = colorTxt;
}

void CCheckButtonPlusSettingDlg::SetTextSize(const int nSize)
{
	m_nTxtSize = nSize;
}

void CCheckButtonPlusSettingDlg::SetSelectValue(const CString strSelVal)
{
	m_strSelVal = strSelVal;
}

void CCheckButtonPlusSettingDlg::SetUnSelectValue(const CString strUnSelVal)
{
	m_strUnSelVal = strUnSelVal;
}

void CCheckButtonPlusSettingDlg::SetBindPointName(const CString strBindPointName)
{
	m_strBindPointName = strBindPointName;
}

void CCheckButtonPlusSettingDlg::SetTextContent(const CString strTxtContent)
{
	m_strTxtContent = strTxtContent;
}

void CCheckButtonPlusSettingDlg::SetGroupId(const int nGroupId)
{
	m_nGroupId = nGroupId;
}

void CCheckButtonPlusSettingDlg::SetExpression(const CString strExpression)
{
	m_nStrExpression = strExpression;
}

void CCheckButtonPlusSettingDlg::SetOptionsDec(const CString cstrOptionsDec)
{
	m_cstrOptionsDec = cstrOptionsDec;
}

int CCheckButtonPlusSettingDlg::GetLayerId(void)
{
	return m_nLayer;
}

eButtonStyle CCheckButtonPlusSettingDlg::GetButtonStyle(void)
{
	return m_eBtnStyle;
}

COLORREF CCheckButtonPlusSettingDlg::GetTextColor(void)
{
	return m_colorTxt;
}

int CCheckButtonPlusSettingDlg::GetTextSize(void)
{
	return m_nTxtSize;
}

CString CCheckButtonPlusSettingDlg::GetSelectValue(void)
{
	return m_strSelVal;
}

CString CCheckButtonPlusSettingDlg::GetUnSelectValue(void)
{
	return m_strUnSelVal;
}

CString CCheckButtonPlusSettingDlg::GetBindPointName(void)
{
	return m_strBindPointName;
}

CString CCheckButtonPlusSettingDlg::GetTextContent(void)
{
	return m_strTxtContent;
}

int CCheckButtonPlusSettingDlg::GetGroupId(void)
{
	return m_nGroupId;
}

CString CCheckButtonPlusSettingDlg::GetExpression(void)
{
	return m_nStrExpression;
}

void CCheckButtonPlusSettingDlg::InitControl(void)
{
	// combobox
	m_comboStyle.AddString(g_strCtrlType[0]);
	m_comboStyle.AddString(g_strCtrlType[1]);
	m_comboStyle.SetCurSel(m_eBtnStyle);

	m_comboLayer.AddString(_T("1层"));
	m_comboLayer.AddString(_T("2层"));
	m_comboLayer.AddString(_T("3层"));
	m_comboLayer.AddString(_T("4层"));
	m_comboLayer.AddString(_T("5层"));
	m_comboLayer.AddString(_T("6层"));
	m_comboLayer.AddString(_T("7层"));
	m_comboLayer.AddString(_T("8层"));
	m_comboLayer.AddString(_T("9层"));
	m_comboLayer.AddString(_T("10层"));
	m_comboLayer.SetCurSel(m_nLayer);

	OnCbnSelchangeComboStyle();
}

void CCheckButtonPlusSettingDlg::InitImagePath(void)
{
	GetSysPath(m_strSysPath);
	m_strSysPath += _T("\\images\\");

	m_strRadioSelComPath = m_strSysPath + _T("radio_btn_sel_com.png");
	m_strRadioSelOverPath = m_strSysPath + _T("radio_btn_sel_over.png");
	m_strRadioUnSelComPath = m_strSysPath + _T("radio_btn_unsel_com.png");
	m_strRadioUnSelOverPath = m_strSysPath + _T("radio_btn_unsel_over.png");
	m_strCheckSelComPath = m_strSysPath + _T("check_btn_sel_com.png");
	m_strCheckSelOverPath = m_strSysPath + _T("check_btn_sel_over.png");
	m_strCheckUnSelComPath = m_strSysPath + _T("check_btn_unsel_com.png");
	m_strCheckUnSelOverPath = m_strSysPath + _T("check_btn_unsel_over.png");
}

void CCheckButtonPlusSettingDlg::OnStnClickedTxtColor()
{
	// TODO: Add your control notification handler code here
	COLORREF cusColor[16] = {0};
	memset(cusColor, 0, sizeof(COLORREF)*16);
	CString strSys;
	::GetSysPath(strSys);
	strSys += _T("\\factory.ini");

	CString strTemp;
	for (int i=0; i<16; ++i)
	{
		strTemp.Format(_T("usercolor%02d"), i+1);
		cusColor[i] = GetPrivateProfileInt(_T("USER"), strTemp, 0, strSys);
	}

	CHOOSECOLOR cc = {sizeof(CHOOSECOLOR), m_hWnd, NULL, m_colorTxt, cusColor, CC_FULLOPEN, NULL, NULL, NULL};
	if (ChooseColor(&cc))
	{
		CString strTemp;
		for (int i=0; i<16; ++i)
		{
			strTemp.Format(_T("usercolor%02d"), i+1);
			CString strColor;
			strColor.Format(_T("%d"), cusColor[i]);
			WritePrivateProfileString(_T("USER"), strTemp, strColor, strSys);
		}
		m_colorTxt = cc.rgbResult;
		Invalidate();
	}
}


HBRUSH CCheckButtonPlusSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_TXT_COLOR)
	{
		HBRUSH hBrush = CreateSolidBrush(m_colorTxt);
		if (hBrush)
		{
			return hBrush;
		}
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CCheckButtonPlusSettingDlg::OnBnClickedBtnBindPoint()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	assert(pFrame != NULL);
	if (pFrame != NULL)
	{
		CDataComConfigDlg dlg(pFrame->m_plcpointmap);
		if (IDOK == dlg.DoModal())
		{
			UpdateData(TRUE);
			m_strBindPointName = dlg.m_strPointName;
			UpdateData(FALSE);
		}
	}
}

CString CCheckButtonPlusSettingDlg::GetImagePathSelectComm(void)
{
	if (E_RADIO_BUTTON == m_eBtnStyle)
	{
		return m_strRadioSelComPath;
	} 
	else if (E_CHECK_BUTTON == m_eBtnStyle)
	{
		return m_strCheckSelComPath;
	}
	return _T("");
}

CString CCheckButtonPlusSettingDlg::GetImagePathSelectOver(void)
{
	if (E_RADIO_BUTTON == m_eBtnStyle)
	{
		return m_strRadioSelOverPath;
	} 
	else if (E_CHECK_BUTTON == m_eBtnStyle)
	{
		return m_strCheckSelOverPath;
	}
	return _T("");
}

CString CCheckButtonPlusSettingDlg::GetImagePathUnSelectComm(void)
{
	if (E_RADIO_BUTTON == m_eBtnStyle)
	{
		return m_strRadioUnSelComPath;
	} 
	else if (E_CHECK_BUTTON == m_eBtnStyle)
	{
		return m_strCheckUnSelComPath;
	}
	return _T("");
}

CString CCheckButtonPlusSettingDlg::GetImagePathUnSelectOver(void)
{
	if (E_RADIO_BUTTON == m_eBtnStyle)
	{
		return m_strRadioUnSelOverPath;
	} 
	else if (E_CHECK_BUTTON == m_eBtnStyle)
	{
		return m_strCheckUnSelOverPath;
	}
	return _T("");
}


CString CCheckButtonPlusSettingDlg::GetOptionsDec(void)
{
	return m_cstrOptionsDec;
}

void CCheckButtonPlusSettingDlg::OnCbnSelchangeComboStyle()
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_comboStyle.GetCurSel();
	CString strCurSel;
	m_comboStyle.GetLBText(nCurSel, strCurSel);

	if (g_strCtrlType[0] == strCurSel)
	{
		GetDlgItem(IDC_EDIT_UNSEL_VAL)->EnableWindow(FALSE);
		//GetDlgItem(IDC_TXT_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_GROUP_ID)->EnableWindow(TRUE);
	}
	else if (g_strCtrlType[1] == strCurSel)
	{
		GetDlgItem(IDC_EDIT_UNSEL_VAL)->EnableWindow(TRUE);
		//GetDlgItem(IDC_TXT_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GROUP_ID)->EnableWindow(FALSE);
	}
}
