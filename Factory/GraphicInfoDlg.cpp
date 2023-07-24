// GraphicInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GraphicInfoDlg.h"
#include "MainFrm.h"

// CGraphicInfoDlg dialog
IMPLEMENT_DYNAMIC(CGraphicInfoDlg, CSkinDlg)
CGraphicInfoDlg::CGraphicInfoDlg(CWnd* pParent /*=NULL*/)
: CSkinDlg(CGraphicInfoDlg::IDD, pParent)
, m_bIsDashLine(FALSE)
, m_layer(E_LAYER_10)
{

}

CGraphicInfoDlg::~CGraphicInfoDlg()
{
}

void CGraphicInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nLineWidth);
	DDX_Control(pDX, IDC_STATIC_Linecolor, m_stcLineColor);
	DDX_Check(pDX, IDC_CHECK2, m_bIsDashLine);
	DDX_Control(pDX, IDC_STATIC_Fillcolor, m_stcFillColor);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_Control(pDX, IDOK, m_btn_ok);
	DDX_Control(pDX, IDCANCEL, m_btn_cancle);
}


BEGIN_MESSAGE_MAP(CGraphicInfoDlg, CSkinDlg)
	ON_BN_CLICKED(IDOK, &CGraphicInfoDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_STATIC_Linecolor, &CGraphicInfoDlg::OnStnClickedStaticLinecolor)
	ON_BN_CLICKED(IDCANCEL, &CGraphicInfoDlg::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_STATIC_Fillcolor, &CGraphicInfoDlg::OnStnClickedStaticFillcolor)
END_MESSAGE_MAP()


// CGraphicInfoDlg message handlers

void CGraphicInfoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_layer = m_comboLayer.GetCurSel();
	OnOK();
}

BOOL CGraphicInfoDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();
	
	m_stcLineColor.SetBkColor(m_LineColor,m_LineColor,CSkinStatic::Normal).SetTransparent(FALSE);
	m_stcFillColor.SetBkColor(m_FillColor,m_FillColor,CSkinStatic::Normal).SetTransparent(FALSE);
	CString str;
	
	m_btn_cancle.SetButtonProjectStyle(Silver);
	m_btn_ok.SetButtonProjectStyle(Blue);
	InitComboLayer();

	return TRUE;
}

void CGraphicInfoDlg::OnStnClickedStaticLinecolor()
{
	CColorDialog dlg(m_LineColor);

	if (dlg.DoModal() == IDOK){
		m_LineColor = dlg.GetColor();
		m_stcLineColor.SetBkColor(m_LineColor,m_LineColor,CSkinStatic::Normal);
	}
}


void CGraphicInfoDlg::InitComboLayer()
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

void CGraphicInfoDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


void CGraphicInfoDlg::OnStnClickedStaticFillcolor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg(m_FillColor);
		
	if (dlg.DoModal() == IDOK){
		m_FillColor = dlg.GetColor();
		m_stcFillColor.SetBkColor(m_FillColor,0,CSkinStatic::Normal);
	}
}
