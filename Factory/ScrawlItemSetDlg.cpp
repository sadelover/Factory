// ScrawlItemSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScrawlItemSetDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"


// CScrawlItemSetDlg dialog

IMPLEMENT_DYNAMIC(CScrawlItemSetDlg, CDialogEx)

CScrawlItemSetDlg::CScrawlItemSetDlg(CDataPointManager* pPointMgr, CWnd* pParent /*=NULL*/)
	: CDialogEx(CScrawlItemSetDlg::IDD, pParent)
	, m_nMax(0)
	, m_nMin(0)
	, m_strRemarks(_T(""))
	, m_color(RGB(0, 0, 0))
	, m_pPointMgr(pPointMgr)
{

}

CScrawlItemSetDlg::~CScrawlItemSetDlg()
{
}

void CScrawlItemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strPointName);
	DDX_Text(pDX, IDC_EDIT_MAX, m_nMax);
	DDX_Text(pDX, IDC_EDIT_MIN, m_nMin);
	DDX_Text(pDX, IDC_EDIT_REMARK, m_strRemarks);
}


BEGIN_MESSAGE_MAP(CScrawlItemSetDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_COLOR, &CScrawlItemSetDlg::OnStnClickedStaticColor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_BIND_POINT, &CScrawlItemSetDlg::OnBnClickedBtnBindPoint)
END_MESSAGE_MAP()


// CScrawlItemSetDlg message handlers


BOOL CScrawlItemSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CScrawlItemSetDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CDialogEx::OnOK();
}


void CScrawlItemSetDlg::OnStnClickedStaticColor()
{
	// TODO: Add your control notification handler code here
	COLORREF cusColor[16] = {0};
	CHOOSECOLOR cc = {sizeof(CHOOSECOLOR), m_hWnd, NULL, m_color, cusColor, CC_FULLOPEN, NULL, NULL, NULL};
	if (ChooseColor(&cc))
	{
		m_color = cc.rgbResult;
		Invalidate();
	}
}


HBRUSH CScrawlItemSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_COLOR)
	{
		HBRUSH hBrush = CreateSolidBrush(m_color);
		if (hBrush)
		{
			return hBrush;
		}
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CScrawlItemSetDlg::OnBnClickedBtnBindPoint()
{
	// TODO: Add your control notification handler code here
	CDataComConfigDlg dlg(*m_pPointMgr);
	if (IDOK == dlg.DoModal())
	{
		m_strPointName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CScrawlItemSetDlg::SetPointName(const CString strName)
{
	m_strPointName = strName;
}

void CScrawlItemSetDlg::SetColor(const COLORREF color)
{
	m_color = color;
}

void CScrawlItemSetDlg::SetMaxValue(const int nMax)
{
	m_nMax = nMax;
}

void CScrawlItemSetDlg::SetMinValue(const int nMin)
{
	m_nMin = nMin;
}

void CScrawlItemSetDlg::SetRemarks(const CString strRemarks)
{
	m_strRemarks = strRemarks;
}

CString CScrawlItemSetDlg::GetPointName(void) const
{
	return m_strPointName;
}

COLORREF CScrawlItemSetDlg::GetColor(void) const
{
	return m_color;
}

int CScrawlItemSetDlg::GetMaxValue(void) const
{
	return m_nMax;
}

int CScrawlItemSetDlg::GetMinValue(void) const
{
	return m_nMin;
}

CString CScrawlItemSetDlg::GetRemarks(void) const
{
	return m_strRemarks;
}
