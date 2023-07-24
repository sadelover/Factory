// InputPipeWidthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputPipeWidthDlg.h"
#include "afxdialogex.h"


// CInputPipeWidthDlg dialog

IMPLEMENT_DYNAMIC(CInputPipeWidthDlg, CDialogEx)

CInputPipeWidthDlg::CInputPipeWidthDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputPipeWidthDlg::IDD, pParent)
	, m_nPipeWidth(6)
	, m_nSpeed(0)
	, m_nFlowDensityIndex(0)
	,m_nWaterFashion(1)
{

}

CInputPipeWidthDlg::~CInputPipeWidthDlg()
{
}

void CInputPipeWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PIPE_WIDTH, m_nPipeWidth);
	DDX_CBIndex(pDX, IDC_COMBO_FLOWSPEED, m_nSpeed);
	DDX_CBIndex(pDX, IDC_COMBO_DENSITY, m_nFlowDensityIndex);
	DDX_Control(pDX, IDC_COMBO_WaterFashion, m_cWaterFashion);
}


BEGIN_MESSAGE_MAP(CInputPipeWidthDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputPipeWidthDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CInputPipeWidthDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	m_cWaterFashion.InsertString(0,L"矩形");
	m_cWaterFashion.InsertString(1,L"圆形");
	m_cWaterFashion.SetCurSel(m_nWaterFashion);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CInputPipeWidthDlg message handlers


void CInputPipeWidthDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_nWaterFashion = m_cWaterFashion.GetCurSel();
	CDialogEx::OnOK();
}


