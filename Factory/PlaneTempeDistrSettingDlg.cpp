// PlaneTempeDistrSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlaneTempeDistrSettingDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"


// CPlaneTempeDistrSettingDlg dialog

IMPLEMENT_DYNAMIC(CPlaneTempeDistrSettingDlg, CDialogEx)

CPlaneTempeDistrSettingDlg::CPlaneTempeDistrSettingDlg(CDataPointManager* pDataPointMgr, CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlaneTempeDistrSettingDlg::IDD, pParent)
	, m_pDataPointMgr(pDataPointMgr)
	, m_nLayer(4)
	, m_strPointName(_T(""))
	, m_nCfgType(0)
{

}

CPlaneTempeDistrSettingDlg::~CPlaneTempeDistrSettingDlg()
{
}

void CPlaneTempeDistrSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME, m_strPointName);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nCfgType);
}


BEGIN_MESSAGE_MAP(CPlaneTempeDistrSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CPlaneTempeDistrSettingDlg::OnBnClickedBtnBrowse)
END_MESSAGE_MAP()


// CPlaneTempeDistrSettingDlg message handlers


BOOL CPlaneTempeDistrSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPlaneTempeDistrSettingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}


void CPlaneTempeDistrSettingDlg::OnBnClickedBtnBrowse()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_pDataPointMgr)
	{
		return;
	}

	CDataComConfigDlg dlg(*m_pDataPointMgr);
	if (IDOK == dlg.DoModal())
	{
		UpdateData(TRUE);
		m_strPointName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}

void CPlaneTempeDistrSettingDlg::InitControls(void)
{
	// combobox
	m_comboLayer.AddString(_T("1²ã"));
	m_comboLayer.AddString(_T("2²ã"));
	m_comboLayer.AddString(_T("3²ã"));
	m_comboLayer.AddString(_T("4²ã"));
	m_comboLayer.AddString(_T("5²ã"));
	m_comboLayer.AddString(_T("6²ã"));
	m_comboLayer.AddString(_T("7²ã"));
	m_comboLayer.AddString(_T("8²ã"));
	m_comboLayer.AddString(_T("9²ã"));
	m_comboLayer.AddString(_T("10²ã"));

	m_comboType.AddString(_T("ÎÂ¶È"));
	m_comboType.AddString(_T("CO"));
	m_comboType.AddString(_T("×Ô¶¨Òå"));
}


void CPlaneTempeDistrSettingDlg::InitData(void)
{
	m_comboLayer.SetCurSel(m_nLayer);
	m_comboType.SetCurSel(m_nCfgType);
}
