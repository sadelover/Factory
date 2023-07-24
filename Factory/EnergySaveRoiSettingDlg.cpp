// EnergySaveRoiSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EnergySaveRoiSettingDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"


// CEnergySaveRoiSettingDlg dialog

IMPLEMENT_DYNAMIC(CEnergySaveRoiSettingDlg, CDialogEx)

CEnergySaveRoiSettingDlg::CEnergySaveRoiSettingDlg(CDataPointManager* pDataPointMgr, CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnergySaveRoiSettingDlg::IDD, pParent)
	, m_pDataPointMgr(pDataPointMgr)
	, m_strBindPointName(_T(""))
	, m_dCost(0)
	, m_nLayer(4)
{

}

CEnergySaveRoiSettingDlg::~CEnergySaveRoiSettingDlg()
{
}

void CEnergySaveRoiSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_Text(pDX, IDC_EDIT_BIND_POINT_NAME, m_strBindPointName);
	DDX_Text(pDX, IDC_EDIT_COST, m_dCost);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_dateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_dateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATE_OCCUR, m_dateOccur);
	DDX_DateTimeCtrl(pDX, IDC_TIME_START2, m_timeStart);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_timeEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIME_OCCUR, m_timeOccur);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
}


BEGIN_MESSAGE_MAP(CEnergySaveRoiSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BIND_POINT, &CEnergySaveRoiSettingDlg::OnBnClickedBtnBindPoint)
END_MESSAGE_MAP()


// CEnergySaveRoiSettingDlg message handlers


BOOL CEnergySaveRoiSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CEnergySaveRoiSettingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	m_dateTimeStart.SetDateTime(m_dateStart.GetYear(), m_dateStart.GetMonth(), m_dateStart.GetDay(), m_timeStart.GetHour(), m_timeStart.GetMinute(), m_timeStart.GetSecond());
	m_dateTimeEnd.SetDateTime(m_dateEnd.GetYear(), m_dateEnd.GetMonth(), m_dateEnd.GetDay(), m_timeEnd.GetHour(), m_timeEnd.GetMinute(), m_timeEnd.GetSecond());
	m_dateTimeOccur.SetDateTime(m_dateOccur.GetYear(), m_dateOccur.GetMonth(), m_dateOccur.GetDay(), m_timeOccur.GetHour(), m_timeOccur.GetMinute(), m_timeOccur.GetSecond());

	COleDateTimeSpan span(m_dateTimeEnd - m_dateTimeStart);
	if (span.GetTotalSeconds() < 0)
	{
		MessageBox(_T("结束时间早于开始时间！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	span = m_dateTimeOccur - m_dateTimeStart;
	if (span.GetTotalSeconds() < 0)
	{
		MessageBox(_T("发生时间早于开始时间！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	span = m_dateTimeEnd - m_dateTimeOccur;
	if (span.GetTotalSeconds() < 0)
	{
		MessageBox(_T("结束时间早于发生时间！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	CDialogEx::OnOK();
}


void CEnergySaveRoiSettingDlg::OnBnClickedBtnBindPoint()
{
	// TODO: Add your control notification handler code here
	CDataComConfigDlg dlg(*m_pDataPointMgr);
	if (IDOK == dlg.DoModal())
	{
		m_strBindPointName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}

void CEnergySaveRoiSettingDlg::SetLayer(const int nLayer)
{
	m_nLayer = nLayer;
}

void CEnergySaveRoiSettingDlg::SetBindPointName(const CString strBindPointName)
{
	m_strBindPointName = strBindPointName;
}

void CEnergySaveRoiSettingDlg::SetCost(const double dCost)
{
	m_dCost = dCost;
}

void CEnergySaveRoiSettingDlg::SetStartTime(const COleDateTime time)
{
	m_dateTimeStart = time;
}

void CEnergySaveRoiSettingDlg::SetEndTime(const COleDateTime time)
{
	m_dateTimeEnd = time;
}

void CEnergySaveRoiSettingDlg::SetOccurTime(const COleDateTime time)
{
	m_dateTimeOccur = time;
}

int CEnergySaveRoiSettingDlg::GetPicId(void)
{
	return g_nPicIndexEnergySaveRoi;
}

int CEnergySaveRoiSettingDlg::GetLayer(void)
{
	return m_nLayer;
}

CString CEnergySaveRoiSettingDlg::GetBindPointName(void)
{
	return m_strBindPointName;
}

double CEnergySaveRoiSettingDlg::GetCost(void)
{
	return	m_dCost;
}

COleDateTime CEnergySaveRoiSettingDlg::GetStartTime(void)
{
	return	m_dateTimeStart;
}

COleDateTime CEnergySaveRoiSettingDlg::GetEndTime(void)
{
	return	m_dateTimeEnd;
}

COleDateTime CEnergySaveRoiSettingDlg::GetOccurTime(void)
{
	return	m_dateTimeOccur;
}

void CEnergySaveRoiSettingDlg::InitControls(void)
{
	// combobox
	CString	strLayerName;
	for (int i=1; i<=10; ++i)
	{
		strLayerName.Format(_T("%d层"), i);
		m_comboLayer.AddString(strLayerName);
	}
	m_comboLayer.SetCurSel(m_nLayer);
}

void CEnergySaveRoiSettingDlg::InitData(void)
{
	// date time control values
	m_dateStart.SetDate(m_dateTimeStart.GetYear(), m_dateTimeStart.GetMonth(), m_dateTimeStart.GetDay());
	m_dateEnd.SetDate(m_dateTimeEnd.GetYear(), m_dateTimeEnd.GetMonth(), m_dateTimeEnd.GetDay());
	m_dateOccur.SetDate(m_dateTimeOccur.GetYear(), m_dateTimeOccur.GetMonth(), m_dateTimeOccur.GetDay());

	m_timeStart.SetTime(m_dateTimeStart.GetHour(), m_dateTimeStart.GetMinute(), m_dateTimeStart.GetSecond());
	m_timeEnd.SetTime(m_dateTimeEnd.GetHour(), m_dateTimeEnd.GetMinute(), m_dateTimeEnd.GetSecond());
	m_timeOccur.SetTime(m_dateTimeOccur.GetHour(), m_dateTimeOccur.GetMinute(), m_dateTimeOccur.GetSecond());

	UpdateData(FALSE);
}