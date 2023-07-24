// ProgressBarProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "ProgressBarProperty.h"
#include "../ComponentDraw/EqmDrawProgressBar.h"
#include "DataComConfigDlg.h"
#include "Tools/CustomTools/CustomTools.h"
// CProgressBarProperty 对话框

IMPLEMENT_DYNAMIC(CProgressBarProperty, CDialog)

CProgressBarProperty::CProgressBarProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressBarProperty::IDD, pParent)
{

}

CProgressBarProperty::CProgressBarProperty( CEqmDrawProgressBar* pProgressBar, CDataPointManager* pMap, CWnd* pParent /*= NULL*/ ): CDialog(CProgressBarProperty::IDD, pParent),
m_pProgressBar(pProgressBar),
m_pMap(pMap)
{

}

CProgressBarProperty::~CProgressBarProperty()
{
}

void CProgressBarProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PROGRESS_BAR_LAYER, m_combo_layer);
}


BEGIN_MESSAGE_MAP(CProgressBarProperty, CDialog)
	ON_BN_CLICKED(IDOK, &CProgressBarProperty::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_PROGRESS_BAR_COLOR, &CProgressBarProperty::OnStnClickedStaticProgressBarColor)
	ON_BN_CLICKED(IDC_BUTTON_PROGRESS_BAR_SELECT_POINT, &CProgressBarProperty::OnBnClickedButtonProgressBarSelectPoint)
	ON_BN_CLICKED(IDC_BUTTON_PROGRESS_BAR_SELECT_POINT_BASE, &CProgressBarProperty::OnBnClickedButtonProgressBarSelectPointBase)
END_MESSAGE_MAP()


// CProgressBarProperty 消息处理程序


void CProgressBarProperty::OnBnClickedOk()
{
	CString strFactorValue;
	GetDlgItem(IDC_EDIT_PROGRESS_BAR_FACTOR_VALUE)->GetWindowText(strFactorValue);
	double dFactor = _wtof(strFactorValue.GetString());
	m_pProgressBar->SetFactorValue(dFactor);

	CString strBind;
	GetDlgItem(IDC_EDIT_PROGRESS_BAR_BIND_POINT)->GetWindowTextW(strBind);
	m_pProgressBar->SetBindPoint(strBind);

	CString strBindBase;
	GetDlgItem(IDC_EDIT_PROGRESS_BAR_BIND_POINT_BASE)->GetWindowTextW(strBindBase);
	m_pProgressBar->SetBindPointBase(strBindBase);

	m_pProgressBar->SetLayer(m_combo_layer.GetCurSel());
	CDialog::OnOK();
}


BOOL CProgressBarProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_combo_layer.AddString(L"1层");
	m_combo_layer.AddString(L"2层");
	m_combo_layer.AddString(L"3层");
	m_combo_layer.AddString(L"4层");
	m_combo_layer.AddString(L"5层");
	m_combo_layer.AddString(L"6层");
	m_combo_layer.AddString(L"7层");
	m_combo_layer.AddString(L"8层");
	m_combo_layer.AddString(L"9层");
	m_combo_layer.AddString(L"10层");
	m_combo_layer.SetCurSel((int)m_pProgressBar->GetLayer());

	if (m_pProgressBar)
	{
		const double dFactor = m_pProgressBar->GetFactorValue();
		CString strFactorValue;
		strFactorValue.Format(L"%.2f",dFactor);
		GetDlgItem(IDC_EDIT_PROGRESS_BAR_FACTOR_VALUE)->SetWindowText(strFactorValue);

		CString strPoint =  m_pProgressBar->GetBindPoint();
		if (strPoint.GetLength() > 0)
		{
			GetDlgItem(IDC_EDIT_PROGRESS_BAR_BIND_POINT)->SetWindowTextW(strPoint);
		}

		CString strPointBase =  m_pProgressBar->GetBindPointBase();
		if (strPointBase.GetLength() > 0)
		{
			GetDlgItem(IDC_EDIT_PROGRESS_BAR_BIND_POINT_BASE)->SetWindowTextW(strPointBase);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CProgressBarProperty::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_PROGRESS_BAR_COLOR)
	{
		HBRUSH hBrush = CreateSolidBrush(m_pProgressBar->GetBarColor());
		if (hBrush)
		{
			return hBrush;
		}
	}
	return hbr;
}


void CProgressBarProperty::OnStnClickedStaticProgressBarColor()
{
	COLORREF cusColor[16];
	memset( cusColor, 0, sizeof( COLORREF) * 16 );
	CString strSys;
	GetSysPath(strSys);
	strSys += L"\\factory.ini";
	for (int i=0;i<16;++i)
	{
		CString strTemp;
		strTemp.Format(L"usercolor%02d",i+1);
		cusColor[i] = GetPrivateProfileInt(L"USER",strTemp,0,strSys);
	}
	CHOOSECOLOR cc = {sizeof( CHOOSECOLOR ),m_hWnd,NULL,m_pProgressBar->GetBarColor(),cusColor,CC_FULLOPEN,NULL,NULL,NULL};
	if (ChooseColor(&cc))
	{
		for (int i=0;i<16;++i)
		{
			CString strTemp;
			strTemp.Format(L"usercolor%02d",i+1);
			CString strColor;
			strColor.Format(L"%d",cusColor[i]);
			WritePrivateProfileString(L"USER",strTemp,strColor,strSys);
		}
		m_pProgressBar->SetBarColor(cc.rgbResult);
		Invalidate();
	}
}


void CProgressBarProperty::OnBnClickedButtonProgressBarSelectPoint()
{
	CDataComConfigDlg dlg(*m_pMap);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strPointName.GetLength() > 0)
		{
			GetDlgItem(IDC_EDIT_PROGRESS_BAR_BIND_POINT)->SetWindowTextW(dlg.m_strPointName);
			m_pProgressBar->SetBindPoint(dlg.m_strPointName);
		}
	}
}

void CProgressBarProperty::OnBnClickedButtonProgressBarSelectPointBase()
{
	CDataComConfigDlg dlg(*m_pMap);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strPointName.GetLength() > 0)
		{
			GetDlgItem(IDC_EDIT_PROGRESS_BAR_BIND_POINT_BASE)->SetWindowTextW(dlg.m_strPointName);
			m_pProgressBar->SetBindPointBase(dlg.m_strPointName);
		}
	}
}
