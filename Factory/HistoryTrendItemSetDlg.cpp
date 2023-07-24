// HistoryTrendItemSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HistoryTrendItemSetDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"


// CHistoryTrendItemSetDlg dialog

IMPLEMENT_DYNAMIC(CHistoryTrendItemSetDlg, CDialogEx)

CHistoryTrendItemSetDlg::CHistoryTrendItemSetDlg(CDataPointManager* pDataPointMgr, CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoryTrendItemSetDlg::IDD, pParent)
	, m_pDataPointMgr(pDataPointMgr)
	, m_timeStart(COleDateTime::GetCurrentTime())
	, m_timeEnd(COleDateTime::GetCurrentTime())
	, m_bIsAutoCalcRange(FALSE)
	, m_strBindPointName(_T(""))
	, m_strRemarks(_T(""))
	, m_nUpdateCycle(0)
	, m_nLineWidth(0)
	, m_fScale(0)
	, m_colorImage(RGB(0, 0, 0))
	, m_dateStart(COleDateTime::GetCurrentTime())
	, m_dateEnd(COleDateTime::GetCurrentTime())
{

}

CHistoryTrendItemSetDlg::~CHistoryTrendItemSetDlg()
{
}

void CHistoryTrendItemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_dateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_dateEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIME_START, m_timeStart);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_timeEnd);
	DDX_Check(pDX, IDC_CHECK_CALC_RANGE, m_bIsAutoCalcRange);
	DDX_Text(pDX, IDC_EDIT_BIND_POINT_NAME, m_strBindPointName);
	DDX_Control(pDX, IDC_BTN_BIND_POINT, m_btnBindPoint);
	DDX_Text(pDX, IDC_EDIT_REMARKS, m_strRemarks);
	DDX_Text(pDX, IDC_EDIT_UPDATE_CYCLE, m_nUpdateCycle);
	DDX_Text(pDX, IDC_EDIT_LINE_WIDTH, m_nLineWidth);
	DDX_Text(pDX, IDC_EDIT_BACKGROUND_SCALE, m_fScale);
}


BEGIN_MESSAGE_MAP(CHistoryTrendItemSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BIND_POINT, &CHistoryTrendItemSetDlg::OnBnClickedBtnBindPoint)
	ON_STN_CLICKED(IDC_IMAGE_COLOR, &CHistoryTrendItemSetDlg::OnStnClickedImageColor)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CHistoryTrendItemSetDlg message handlers


BOOL CHistoryTrendItemSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CHistoryTrendItemSetDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	COleDateTime tmStart;
	COleDateTime tmEnd;
	tmStart.SetDateTime(m_dateStart.GetYear(), m_dateStart.GetMonth(), m_dateStart.GetDay(), m_timeStart.GetHour(), m_timeStart.GetMinute(), m_timeStart.GetSecond());
	tmEnd.SetDateTime(m_dateEnd.GetYear(), m_dateEnd.GetMonth(), m_dateEnd.GetDay(), m_timeEnd.GetHour(), m_timeEnd.GetMinute(), m_timeEnd.GetSecond());

	COleDateTimeSpan span(tmEnd - tmStart);
	if (span.GetTotalSeconds() < 0)
	{
		MessageBox(_T("开始时间不晚于结束时间！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	m_historyTrendInfo.dateTimeStart = tmStart;
	m_historyTrendInfo.dateTimeEnd = tmEnd;

	m_historyTrendInfo.strBindPointName = m_strBindPointName;
	m_historyTrendInfo.strRemarks = m_strRemarks;
	m_historyTrendInfo.colorImage = m_colorImage;
	m_historyTrendInfo.nLineWidth = m_nLineWidth;
	m_historyTrendInfo.bIsCalcRange = m_bIsAutoCalcRange ? true : false;
	m_historyTrendInfo.nUpdateCycle = m_nUpdateCycle;
	m_historyTrendInfo.fScale = m_fScale;

	CDialogEx::OnOK();
}


void CHistoryTrendItemSetDlg::OnBnClickedBtnBindPoint()
{
	// TODO: Add your control notification handler code here
	CDataComConfigDlg dlg(*m_pDataPointMgr);
	if (IDOK == dlg.DoModal())
	{
		m_strBindPointName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}

void CHistoryTrendItemSetDlg::InitData(void)
{
	COleDateTime dateTimeStart(m_historyTrendInfo.dateTimeStart);
	COleDateTime dateTimeEnd(m_historyTrendInfo.dateTimeEnd);
	m_dateStart.SetDate(dateTimeStart.GetYear(), dateTimeStart.GetMonth(), dateTimeStart.GetDay());
	m_dateEnd.SetDate(dateTimeEnd.GetYear(), dateTimeEnd.GetMonth(), dateTimeEnd.GetDay());
	m_timeStart.SetTime(dateTimeStart.GetHour(), dateTimeStart.GetMinute(), dateTimeStart.GetSecond());
	m_timeEnd.SetTime(dateTimeEnd.GetHour(), dateTimeEnd.GetMinute(), dateTimeEnd.GetSecond());

	m_bIsAutoCalcRange = m_historyTrendInfo.bIsCalcRange;
	m_strBindPointName = m_historyTrendInfo.strBindPointName;
	m_strRemarks = m_historyTrendInfo.strRemarks;
	m_colorImage = m_historyTrendInfo.colorImage;
	m_nUpdateCycle = m_historyTrendInfo.nUpdateCycle;
	m_nLineWidth = m_historyTrendInfo.nLineWidth;
	m_fScale = m_historyTrendInfo.fScale;

	UpdateData(FALSE);
}


void CHistoryTrendItemSetDlg::OnStnClickedImageColor()
{
	// TODO: Add your control notification handler code here
	COLORREF cusColor[16] = {0};
	memset(cusColor, 0, sizeof(COLORREF)*16);

	CHOOSECOLOR cc = {sizeof(CHOOSECOLOR), m_hWnd, NULL, m_colorImage, cusColor, CC_FULLOPEN, NULL, NULL, NULL};
	if (ChooseColor(&cc))
	{
		m_colorImage = cc.rgbResult;
		Invalidate();
	}
}


HBRUSH CHistoryTrendItemSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_IMAGE_COLOR)
	{
		HBRUSH hBrush = CreateSolidBrush(m_colorImage);
		if (hBrush)
		{
			return hBrush;
		}
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CHistoryTrendItemSetDlg::SetHistoryTrendInfo(const HistoryTrendInfo& historyTrendInfo)
{
	m_historyTrendInfo = historyTrendInfo;
}

HistoryTrendInfo CHistoryTrendItemSetDlg::GetHistoryTrendInfo(void)
{
	return m_historyTrendInfo;
}
