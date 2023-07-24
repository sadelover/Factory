// RealTimeTrendDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "Beop_TrendAnalysis.h"
#include "RealTimeTrendDlg.h"
#include "afxdialogex.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/Util/UtilString.h"
#include "PointInfoDlg.h"
#include "PageDlg.h"

#pragma warning(once:4244) //仅显示一个
//////////////////////////////////////////////////////////////////////////

const UINT c_buttonNum = 5;

const UINT c_timer_update_id = 10000;
const UINT c_timer_update_time_in_ms = 2000;  //2000ms

//实时曲线显示1800个点
const int C_ARRAY_SIZE = 920;



#define  RGB_PARAM1		 0x0a6eb4     //RGB 10， 110. 180
#define  RGB_PARAM2		 0x0abeb4		//RGB 10， 190，180
#define  RGB_PARAM3		 0x282828		//rgb40 40 40
#define  RGB_PARAM4		 0x8c1e64		//rgb140，30，100
#define  RGB_PARAM5		 0xc8d20a		//RGB 200， 210， 10
#define  RGB_PARAM6		 0xf01414		//RGB（240，20，20）
#define  RGB_BACKGROUND  0xebebeb
//////////////////////////////////////////////////////////////////////////

// CRealTimeTrendDlg dialog

IMPLEMENT_DYNAMIC(CRealTimeTrendDlg, CDialog)

CRealTimeTrendDlg::CRealTimeTrendDlg(CWnd* pParent)
	: CDialog(CRealTimeTrendDlg::IDD, pParent )
	, m_startDate(COleDateTime::GetCurrentTime() - COleDateTimeSpan(1,0,0,0))
	, m_endDate(COleDateTime::GetCurrentTime() + COleDateTimeSpan(1,0,0,0))
	, m_startTime(COleDateTime::GetCurrentTime())
	, m_endTime(COleDateTime::GetCurrentTime())
	, oleTimeStart(COleDateTime::GetCurrentTime() - COleDateTimeSpan(1,0,0,0))
	, oleTimeEnd(COleDateTime::GetCurrentTime() + COleDateTimeSpan(1,0,0,0))
	, pointIndex(0)
	, m__nTimeRange(0)
	, m_change_flag(FALSE)
	,m_pServerDataAccess(NULL)
	,m_bIsNeedUpdateLimits(false)
	,m_dYMax(0.0f)
	,m_dYMin(0.0f)
	,m_uDlgIdx(0)
{
	m_iniWatchPointList.clear();
}

CRealTimeTrendDlg::CRealTimeTrendDlg(CUPPCServerDataAccess*	pServerDataAccess, CWnd* pParent /*=NULL*/)
	: CDialog(CRealTimeTrendDlg::IDD, pParent)
	, m_startDate(COleDateTime::GetCurrentTime() - COleDateTimeSpan(1,0,0,0))
	, m_endDate(COleDateTime::GetCurrentTime() + COleDateTimeSpan(1,0,0,0))
	, m_startTime(COleDateTime::GetCurrentTime())
	, m_endTime(COleDateTime::GetCurrentTime())
	, oleTimeStart(COleDateTime::GetCurrentTime() - COleDateTimeSpan(1,0,0,0))
	, oleTimeEnd(COleDateTime::GetCurrentTime() + COleDateTimeSpan(1,0,0,0))
	, pointIndex(0)
	, m__nTimeRange(0)
	, m_change_flag(FALSE)
	,m_pServerDataAccess(pServerDataAccess)
	,m_bIsNeedUpdateLimits(false)
	,m_dYMax(0.0f)
	,m_dYMin(0.0f)
{
	m_iniWatchPointList.clear();
}

CRealTimeTrendDlg::~CRealTimeTrendDlg()
{

}

void CRealTimeTrendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PARAM1, m_btn_param1);
	DDX_Control(pDX, IDC_BUTTON_PARAM2, m_btn_param2);
	DDX_Control(pDX, IDC_BUTTON_PARAM3, m_btn_param3);
	DDX_Control(pDX, IDC_BUTTON_PARAM4, m_btn_param4);
	DDX_Control(pDX, IDC_BUTTON_PARAM5, m_btn_param5);

	m_vecBtnParam.clear();
	m_vecBtnParam.push_back(&m_btn_param1);
	m_vecBtnParam.push_back(&m_btn_param2);
	m_vecBtnParam.push_back(&m_btn_param3);
	m_vecBtnParam.push_back(&m_btn_param4);
	m_vecBtnParam.push_back(&m_btn_param5);

	DDX_Control(pDX, IDC_BUTTON_DELETE1, m_delete_param1);
	DDX_Control(pDX, IDC_BUTTON_DELETE2, m_delete_param2);
	DDX_Control(pDX, IDC_BUTTON_DELETE3, m_delete_param3);
	DDX_Control(pDX, IDC_BUTTON_DELETE4, m_delete_Param4);
	DDX_Control(pDX, IDC_BUTTON_DELETE5, m_delete_param5);

	DDX_Control(pDX, IDC_BUTTON_INFO1, m_info_param1);
	DDX_Control(pDX, IDC_BUTTON_INFO2, m_info_param2);
	DDX_Control(pDX, IDC_BUTTON_INFO3, m_info_param3);
	DDX_Control(pDX, IDC_BUTTON_INFO4, m_info_param4);
	DDX_Control(pDX, IDC_BUTTON_INFO5, m_info_param5);

	DDX_Control(pDX, IDC_BUTTON_DELETEALL, m_delete_all);
	DDX_Control(pDX, IDC_BUTTON_MOVELINE, m_btn_moveTime);
	DDX_Control(pDX, IDC_BUTTON_MOVERIGHT, m_btn_move_right);
}


BEGIN_MESSAGE_MAP(CRealTimeTrendDlg, CDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PARAM1, &CRealTimeTrendDlg::OnBnClickedButtonParam1)
	ON_BN_CLICKED(IDC_BUTTON_PARAM2, &CRealTimeTrendDlg::OnBnClickedButtonParam2)
	ON_BN_CLICKED(IDC_BUTTON_PARAM3, &CRealTimeTrendDlg::OnBnClickedButtonParam3)
	ON_BN_CLICKED(IDC_BUTTON_PARAM4, &CRealTimeTrendDlg::OnBnClickedButtonParam4)
	ON_BN_CLICKED(IDC_BUTTON_PARAM5, &CRealTimeTrendDlg::OnBnClickedButtonParam5)
	ON_BN_CLICKED(IDC_BUTTON_DELETE1, &CRealTimeTrendDlg::OnBnClickedButtonDelete1)
	ON_BN_CLICKED(IDC_BUTTON_DELETE2, &CRealTimeTrendDlg::OnBnClickedButtonDelete2)
	ON_BN_CLICKED(IDC_BUTTON_DELETE3, &CRealTimeTrendDlg::OnBnClickedButtonDelete3)
	ON_BN_CLICKED(IDC_BUTTON_DELETE4, &CRealTimeTrendDlg::OnBnClickedButtonDelete4)
	ON_BN_CLICKED(IDC_BUTTON_DELETE5, &CRealTimeTrendDlg::OnBnClickedButtonDelete5)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALL, &CRealTimeTrendDlg::OnBnClickedButtonDeleteAll)
	ON_BN_CLICKED(IDC_BUTTON_MOVELINE, &CRealTimeTrendDlg::OnBnClickedButtonMoveline)
	ON_BN_CLICKED(IDC_BUTTON_MOVERIGHT, &CRealTimeTrendDlg::OnBnClickedButtonMoveright)
	ON_BN_CLICKED(IDC_BUTTON_INFO1, &CRealTimeTrendDlg::OnBnClickedButtonInfo1)
	ON_BN_CLICKED(IDC_BUTTON_INFO2, &CRealTimeTrendDlg::OnBnClickedButtonInfo2)
	ON_BN_CLICKED(IDC_BUTTON_INFO3, &CRealTimeTrendDlg::OnBnClickedButtonInfo3)
	ON_BN_CLICKED(IDC_BUTTON_INFO4, &CRealTimeTrendDlg::OnBnClickedButtonInfo4)
	ON_BN_CLICKED(IDC_BUTTON_INFO5, &CRealTimeTrendDlg::OnBnClickedButtonInfo5)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL, &CRealTimeTrendDlg::OnBnClickedButtonManual)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CRealTimeTrendDlg::OnBnClickedButtonAuto)
END_MESSAGE_MAP()

HWND CRealTimeTrendDlg::GetParentHandle() const
{
	return m_hParentHandle;
}

void CRealTimeTrendDlg::SetParentHandle( const HWND& hWnd )
{
	m_hParentHandle = hWnd;
}


// CRealTimeTrendDlg message handlers

BOOL CRealTimeTrendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitSaveData();


	CRect rect;
	::GetWindowRect(m_hWnd,rect);
	ScreenToClient(rect);
	m_nDlgWidth = rect.right - rect.left;
	m_nDlgHeight = rect.bottom - rect.top;

	InitButton();
	InitChartLineColor();
	InitGraph();
	UpdateData(FALSE);
	FirstInitChart = TRUE;
	DrawChart();

	InitSize();

	SetTimer(c_timer_update_id, c_timer_update_time_in_ms, NULL);

	//知道几个点初始化点位
	//InitPointView();
	return TRUE;  
}

void   CRealTimeTrendDlg::InitSaveData()
{

	g_vecReadData.resize(c_buttonNum);
	for(UINT idx= 0; idx< g_vecReadData.size(); idx++){
		CReadRealData& entry = g_vecReadData[idx];
		entry.mIdx = idx;
		entry.m_vecData.resize(C_ARRAY_SIZE);
	}
	//init color
	g_vecReadData[0].mColor = RGB_PARAM1;
	g_vecReadData[1].mColor = RGB_PARAM2;
	g_vecReadData[2].mColor = RGB_PARAM3;
	g_vecReadData[3].mColor = RGB_PARAM4;
	g_vecReadData[4].mColor = RGB_PARAM5;
	//g_vecReadData[idx].mColor = RGB_PARAM6;

	//init btn name
	g_vecReadData[0].mStrBtnName = _T("Param 1");
	g_vecReadData[1].mStrBtnName = _T("Param 2");
	g_vecReadData[2].mStrBtnName = _T("Param 3");
	g_vecReadData[3].mStrBtnName = _T("Param 4");
	g_vecReadData[4].mStrBtnName = _T("Param 5");

	//double vector time
	m_vecTime.resize(C_ARRAY_SIZE);

	COleDateTime ct =COleDateTime::GetCurrentTime();
	m_startTime.SetTime(ct.GetHour(),0,0);
	m_endTime.SetTime(ct.GetHour(),0,0);
	oleTimeStart.SetDateTime(m_startDate.GetYear(),m_startDate.GetMonth(),m_startDate.GetDay(),m_startTime.GetHour(),m_startTime.GetMinute(),m_startTime.GetSecond());
	oleTimeEnd.SetDateTime(m_endDate.GetYear(),m_endDate.GetMonth(),m_endDate.GetDay(),m_endTime.GetHour(),m_endTime.GetMinute(),m_endTime.GetSecond());

}

void    CRealTimeTrendDlg::InitSize()
{
	//计算分辨率
	m_nWidth = GetSystemMetrics(SM_CXSCREEN);
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);

	//计算放大倍数
	m_Multiple_width = float(m_nWidth)/float(m_nDlgWidth);
	m_Mutiple_heith = float(m_nHeight)/float(m_nDlgHeight);
	m_change_flag= true;
}

CImageButton* CRealTimeTrendDlg::CreateButtonSt(UINT id, 
	const wstring& wstrName,  
	const CRect& rect )
{
	CFont* pFont = this->GetFont();

	CImageButton* pButton = new CImageButton();
	ASSERT(pButton);
	pButton->Create(wstrName.c_str(), WS_CHILD|WS_VISIBLE, rect, this,  id);
	pButton->SetFont(pFont);
	pButton->ShowWindow(SW_SHOW);

	return pButton;
}


void CRealTimeTrendDlg::InitButton()
{
	CString bitmappath;

	CRect  rect;
	this->GetClientRect(&rect);
	int cx = rect.right-rect.left;
	int cy = rect.bottom-rect.top;
	m_btn_param1.MoveWindow(cx*0.865,cy*0.02,150,42);
	m_btn_param2.MoveWindow(cx*0.865,cy*0.12,150,42);
	m_btn_param3.MoveWindow(cx*0.865,cy*0.22,150,42);
	m_btn_param4.MoveWindow(cx*0.865,cy*0.32,150,42);
	m_btn_param5.MoveWindow(cx*0.865,cy*0.42,150,42);

	m_delete_param1.MoveWindow(cx*0.973,cy*0.02,19,20);
	m_delete_param2.MoveWindow(cx*0.973,cy*0.12,19,20);
	m_delete_param3.MoveWindow(cx*0.973,cy*0.22,19,20);
	m_delete_Param4.MoveWindow(cx*0.973,cy*0.32,19,20);
	m_delete_param5.MoveWindow(cx*0.973,cy*0.42,19,20);

	m_info_param1.MoveWindow(cx*0.973,cy*0.06,19,18);
	m_info_param2.MoveWindow(cx*0.973,cy*0.16,19,18);
	m_info_param3.MoveWindow(cx*0.973,cy*0.26,19,18);
	m_info_param4.MoveWindow(cx*0.973,cy*0.36,19,18);
	m_info_param5.MoveWindow(cx*0.973,cy*0.46,19,18);

	m_delete_all.MoveWindow(cx*0.865,cy*0.50,150,42);
	m_btn_moveTime.MoveWindow(cx*0.865,cy*0.70,150,42);
	m_btn_move_right.MoveWindow(cx*0.865,cy*0.80,150,42);

}
void CRealTimeTrendDlg::InitChartLineColor()
{
	m_vecLineColor.clear();

	m_vecLineColor.push_back(RGB(240,20,20));
	m_vecLineColor.push_back(RGB(200,210,10));
	m_vecLineColor.push_back(RGB(40,40,40));
	m_vecLineColor.push_back(RGB(140,30,100));
	m_vecLineColor.push_back(RGB(10,180,190));
	m_vecLineColor.push_back(RGB(10,110,180));

}
void CRealTimeTrendDlg::InitGraph()
{
	CRect  rect;
	this->GetClientRect(&rect);
	rect.top +=10;
	rect.bottom -=60;
	rect.left +=20;
	rect.right -=195;
	m_RectChart = rect;
	m_ChartViewer.Create(_T("STatic"), WS_VISIBLE|WS_CHILD|SS_BITMAP, m_RectChart, this, 10);
	m_ChartViewer.MoveWindow(rect);
}
void CRealTimeTrendDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);

}

void CRealTimeTrendDlg::SetButtonProp(UINT buttonIdx,UINT pointIdx,
	const wstring& wstrPlcShort,  const wstring& strDescription, const wstring& strUnit, const wstring& strSourceType , const wstring& strParam1)
{
	if(buttonIdx >= g_vecReadData.size() )
		return;

	CReadRealData& readData = g_vecReadData[buttonIdx];
	readData.mStrPlcShortName = wstrPlcShort;
	readData.mPointIdx = pointIdx;
	readData.strDescription = strDescription;
	readData.strParam1 = strParam1;
	readData.strSourceType = strSourceType;
	readData.strUnit = strUnit;
	readData.m_vecRecord.clear();
	readData.mEnable = true;
}
void	CRealTimeTrendDlg::UpdateVectorData()
{
	//update time
	SYSTEMTIME st;
	GetLocalTime(&st);
	double currentime  = Chart::chartTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, 
		st.wSecond) + st.wMilliseconds / 1000.0;
	//entry.m_timestamp = currentime;

	double dtemp = 0;
	bool ifok = false;
	wstring wstr;

	for(UINT idx=0; idx< g_vecReadData.size(); idx++){
		CReadRealData& entry = g_vecReadData[idx];
		if(!entry.mEnable || entry.mStrPlcShortName == _T(""))
			continue;
		ifok = m_pServerDataAccess->GetValue(entry.mStrPlcShortName, dtemp);
		ShiftData(&entry.m_vecData[0], entry.m_vecData.size(), dtemp);
	}
	
	ShiftData(&m_vecTime[0], C_ARRAY_SIZE, currentime);
}

void CRealTimeTrendDlg::ShiftData(double *data, int len, double newValue)
{
	memmove(data, data + 1, sizeof(*data) * (len - 1));
	data[len - 1] = newValue;
}

void CRealTimeTrendDlg::OnBnClickedButtonParam1()
{
	OnBtnParam(0);
}

void CRealTimeTrendDlg::OnBnClickedButtonParam2()
{
	OnBtnParam(1);
}

void CRealTimeTrendDlg::OnBnClickedButtonParam3()
{
	OnBtnParam(2);
}

void CRealTimeTrendDlg::OnBnClickedButtonParam4()
{
	OnBtnParam(3);
}

void CRealTimeTrendDlg::OnBnClickedButtonParam5()
{
	OnBtnParam(4);
}

void CRealTimeTrendDlg::OnBtnParam(UINT idx)
{
	if(idx >= c_buttonNum)
		return;

	UpdateData(TRUE);
	CSelectPointDlg dlg(m_pBEOPGatewayDlg);
	wstring		strPointRemark;
	wstring		strPointName;
	wstring		strUnit;
	wstring		strSourceType;
	wstring		strParam1;
	if(dlg.DoModal()==IDOK)
	{
		vector<DataPointEntry>  & m_pointlist = m_pBEOPGatewayDlg->m_vecServerPointList;
		const size_t size = m_pointlist.size();
		for(size_t i=0;i<size;++i)
		{
			if(m_pointlist[i].GetPointIndex() == dlg.m_nSelectIndex)
			{
				DataPointEntry entry = m_pointlist[i];
				strPointRemark = entry.GetDescription();
				strPointName = entry.GetShortName();
				strUnit = entry.GetUnit();
				strSourceType = entry.GetSourceType();
				strParam1 = entry.GetParam(1);
				break;
			}
		}
	}
	else
	{
		return;
	}

	//SaveConfig(idx, strPointName, strPointRemark);

	oleTimeStart.SetDateTime(m_startDate.GetYear(),m_startDate.GetMonth(),m_startDate.GetDay(),m_startTime.GetHour(),m_startTime.GetMinute(),m_startTime.GetSecond());
	oleTimeEnd.SetDateTime(m_endDate.GetYear(),m_endDate.GetMonth(),m_endDate.GetDay(),m_endTime.GetHour(),m_endTime.GetMinute(),m_endTime.GetSecond());

	wstring strLongPlcName;
	SetButtonProp(idx,0,strPointName,strPointRemark,strUnit,strSourceType,strParam1);
	SaveRealPoint(m_uDlgIdx,idx,0,strPointName,strPointRemark,strUnit,strSourceType,strParam1);
	 CReadRealData& readData = g_vecReadData[idx];
	if(strPointName.length()>0)
	{
		CString strRemark = strPointName.c_str();
		strRemark+=L"\r\n";
		strRemark +=strPointRemark.c_str();

		m_vecBtnParam[idx]->SetWindowText(strRemark);
		//m_vecBtnParam[idx]->SetBtnTextColor( RGB(10, 110, 180));
	}
	else
	{
		m_vecBtnParam[idx]->SetWindowText(readData.mStrBtnName.c_str() );  //_T("参数四")
		//m_vecBtnParam[idx]->SetBtnTextColor(RGB(90, 90, 90));
	}



	UpdateData(TRUE);
	DisableControlsOper();


	DrawHiseryCurveChart();

	EnableControlsOper();
}


void CRealTimeTrendDlg::InitPointView()
{
	vector<DataPointEntry>  m_pointlist = m_pBEOPGatewayDlg->m_vecServerPointList;

	for (int k=0;k<m_iniWatchPointList.size();++k)
	{
		wstring		strPointRemark;
		wstring		strPointName;
		wstring		strUnit;
		wstring		strSourceType;
		wstring		strParam1;
		int         idx = k;
		const size_t size = m_pointlist.size();
		for(size_t i=0;i<size;++i)
		{
			wstring wstrName = m_pointlist[i].GetDescription();
			wstring pointName = m_pointlist[i].GetShortName();
			if (m_iniWatchPointList[k] == pointName)
			{
				CString strRemarkName = wstrName.c_str();
				//idx = m_pointlist[i].GetPointIndex();
				strPointRemark = wstrName;
				strPointName   = pointName;
				strUnit		   = m_pointlist[i].GetUnit();
				strSourceType  = m_pointlist[i].GetSourceType();
				strParam1      = m_pointlist[i].GetParam(1);
				break;
			}
		}
		oleTimeStart.SetDateTime(m_startDate.GetYear(),m_startDate.GetMonth(),m_startDate.GetDay(),m_startTime.GetHour(),m_startTime.GetMinute(),m_startTime.GetSecond());
		oleTimeEnd.SetDateTime(m_endDate.GetYear(),m_endDate.GetMonth(),m_endDate.GetDay(),m_endTime.GetHour(),m_endTime.GetMinute(),m_endTime.GetSecond());

		wstring strLongPlcName;
		SetButtonProp(idx,0,strPointName,strPointRemark,strUnit,strSourceType,strParam1);

		CReadRealData& readData = g_vecReadData[idx];
		if(strPointRemark.length()>0)
		{
			CString strRemark = strPointRemark.c_str();
			if(strRemark.GetLength()>8)
			{
				strRemark = strRemark.Left(8);
			}
			m_vecBtnParam[idx]->SetWindowText(strRemark);
		}
		else
		{
			m_vecBtnParam[idx]->SetWindowText(readData.mStrBtnName.c_str() );  //_T("参数四")
		}
		UpdateData(TRUE);
		DisableControlsOper();


		DrawHiseryCurveChart();
		EnableControlsOper();

	}
}
//////////////////////////////////////////////////////////////////////////
void CRealTimeTrendDlg::OnBnClickedButtonDelete1()
{
	OnBtnDelete(0);
}

void CRealTimeTrendDlg::OnBnClickedButtonDelete2()
{
	OnBtnDelete(1);
}

void CRealTimeTrendDlg::OnBnClickedButtonDelete3()
{
	OnBtnDelete(2);
}

void CRealTimeTrendDlg::OnBnClickedButtonDelete4()
{
	OnBtnDelete(3);
}

void CRealTimeTrendDlg::OnBnClickedButtonDelete5()
{
	OnBtnDelete(4);
}

void CRealTimeTrendDlg::OnBnClickedButtonDeleteAll()
{
	// TODO: Add your control notification handler code here
	if(IsShowInFoDlg())
	{
		return;
	}
	DisableControlsOper();
	
	wstring strNull;
	SetButtonProp(0,0,strNull,strNull,strNull,strNull,strNull);
	SetButtonProp(1,0,strNull,strNull,strNull,strNull,strNull);
	SetButtonProp(2,0,strNull,strNull,strNull,strNull,strNull);
	SetButtonProp(3,0,strNull,strNull,strNull,strNull,strNull);
	SetButtonProp(4,0,strNull,strNull,strNull,strNull,strNull);

	DeleteRealPoint(m_uDlgIdx,0);
	DeleteRealPoint(m_uDlgIdx,1);
	DeleteRealPoint(m_uDlgIdx,2);
	DeleteRealPoint(m_uDlgIdx,3);
	DeleteRealPoint(m_uDlgIdx,4);

	m_btn_param1.SetWindowText(_T("Param 1"));
//	m_btn_param1.SetBtnTextColor( RGB(90, 90, 90));
	m_btn_param2.SetWindowText(_T("Param 2"));
//	m_btn_param2.SetBtnTextColor( RGB(90, 90, 90));
	m_btn_param3.SetWindowText(_T("Param 3"));
//	m_btn_param3.SetBtnTextColor( RGB(90, 90, 90));
	m_btn_param4.SetWindowText(_T("Param 4"));
//	m_btn_param4.SetBtnTextColor( RGB(90, 90, 90));
	m_btn_param5.SetWindowText(_T("Param 5"));
//	m_btn_param5.SetBtnTextColor( RGB(90, 90, 90));

	//value_param1.RemoveAll();
	//value_param2.RemoveAll();
	//value_param3.RemoveAll();
	//value_param4.RemoveAll();
	//value_param5.RemoveAll();
	DrawHiseryCurveChart();

	EnableControlsOper();
	//EnableTimeCtrl();
}

void CRealTimeTrendDlg::OnBtnDelete(UINT idx)
{
	if(idx >= c_buttonNum)
		return;

	CReadRealData& readData = g_vecReadData[idx];

	readData.mEnable = false;
	if(readData.mStrPlcShortName.length()<0)
		return;
	DisableControlsOper();
	readData.m_vecRecord.clear();
	readData.m_vecData.clear();
	readData.m_vecData.resize(C_ARRAY_SIZE, 0);

	wstring strNull;
	SetButtonProp(idx,0,strNull,strNull,strNull,strNull,strNull);
	DeleteRealPoint(m_uDlgIdx,idx);

	m_vecBtnParam[idx]->SetWindowText(readData.mStrBtnName.c_str() );
//	m_vecBtnParam[idx]->SetBtnTextColor(RGB(90, 90, 90));

	DrawHiseryCurveChart();

	EnableControlsOper();
	//IsEnableOrDisable();

}
//////////////////////////////////////////////////////////////////////////

bool CRealTimeTrendDlg::IsShowInFoDlg()   
{
	///选择曲线数为0，返回TRUE,否则FALSE;
	for(UINT  i=0;i<g_vecReadData.size();i++)
	{
		CReadRealData& readData = g_vecReadData[i];
		if(readData.mStrPlcShortName.length()>0)
			return FALSE;	
	}
	return TRUE;
}
void CRealTimeTrendDlg::DrawChart()
{
	UpdateData(TRUE);
	if(IsShowInFoDlg()
		&& FirstInitChart==FALSE)
	{
		DrawHiseryCurveChart();
		return;
	}
	FirstInitChart = FALSE;

	DisableControlsOper();

	//UpdateVectorData();
	//GetParam2Data();
	//GetParam3Data();
	//GetParam4Data();
	//GetParam5Data();

	DrawHiseryCurveChart();
	EnableControlsOper();
}
void CRealTimeTrendDlg::DrawHiseryCurveChart()
{
	CRect rectchart = m_RectChart;
	XYChart *xyChart = new XYChart(rectchart.Width(), rectchart.Height(), 0xf4f4f4, -1, 0);
	CString str = _T("实时曲线");
	string str_title = Project::Tools::WideCharToUtf8(str.GetString());

	//xyChart->addTitle(("HistoryCurve"), "timesbi.ttf", 18);
	int xoffset = 43;
	int yoffset = 35;
xyChart->setPlotArea(xoffset, yoffset, rectchart.Width() -78, rectchart.Height()-65, 0xebebeb, -1, -1,  0xcccccc, 0xcccccc);
	xyChart->setClipping();

	LegendBox *b = xyChart->addLegend2(xoffset, yoffset, 3, "arialbd.ttf", 9);
	b->setBackground(Chart::Transparent, Chart::Transparent);
	b->setWidth(rectchart.Width() - 2*xoffset);

	xyChart->addTitle(str_title.c_str(), "", 18, 0xf01414)->setBackground(0xf4f4f4, -1,0);

	xyChart->setDefaultFonts("SIMSUN.TTC","simhei.ttf");

	xyChart->yAxis()->setTitle("", "arialbd.ttf", 10);

	xyChart->xAxis()->setTickDensity(75, 15);
	//xyChart->xAxis()->setTickDensity(rectchart.Width() - 2*xoffset,-1);

	xyChart->xAxis()->setWidth(1);
	xyChart->yAxis()->setWidth(1);

	const UINT vec_time_size = m_vecTime.size();
	if( vec_time_size < 1) 
	return;
	double endtime  = m_vecTime[vec_time_size-1];
	const int updateIntevalInSec = c_timer_update_time_in_ms / 1000;
	double starttime  = endtime - updateIntevalInSec* C_ARRAY_SIZE;//m_vecTime[0];

	xyChart->xAxis()->setDateScale(starttime, endtime);
	xyChart->xAxis()->setLabelFormat("{value|hh:nn}");

	LineLayer *layer = xyChart->addLineLayer();	
	layer->setXData(starttime,endtime);

	for(UINT idx= 0; idx< g_vecReadData.size(); idx++){
		CReadRealData& entry = g_vecReadData[idx];
		if(!entry.mEnable)
			continue;
		layer->addDataSet(DoubleArray(&entry.m_vecData[0], entry.m_vecData.size() ), entry.mColor, "");
	}
	if (m_bIsNeedUpdateLimits)
	{
		xyChart->yAxis()->setDateScale(m_dYMin,m_dYMax);
		xyChart->yAxis()->setLinearScale(m_dYMin,m_dYMax,(m_dYMax - m_dYMin) / 10);
	}
	m_ChartViewer.setChart(xyChart);
	if(xyChart)
	{
		delete xyChart;
		xyChart = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//void CRealTimeTrendDlg::OnDtnDatetimechangeStartdateCurve(NMHDR *pNMHDR, LRESULT *pResult)

//void CRealTimeTrendDlg::OnDtnDatetimechangeStartdateTimeCurve(NMHDR *pNMHDR, LRESULT *pResult)

bool CRealTimeTrendDlg::CompTwoTime()
{
	if(oleTimeStart>oleTimeEnd)
	{
		::MessageBox(this->m_hWnd, TEXT("Start time can not be greater than the end time!  \r\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR | MB_TOPMOST);
		return TRUE;
	}
	return FALSE;
}
bool CRealTimeTrendDlg::CompTwoTimeEnd()
{
	if(oleTimeStart>oleTimeEnd)
	{
		::MessageBox(this->m_hWnd, TEXT("End time can not be less than the end time!  \r\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR | MB_TOPMOST);
		return TRUE;
	}
	return FALSE;
}

void CRealTimeTrendDlg::OnDtnDatetimechangeEnddateCurve(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateData(TRUE);
}

void CRealTimeTrendDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if (m_change_flag)//如果确定oninitdlg已经调用完毕.
	{
		CRect  rect;
		this->GetClientRect(&rect);
		rect.top +=10;
		rect.bottom -=20;
		rect.left +=20;
		rect.right -=195;
		m_RectChart = rect;
		DrawHiseryCurveChart();
		//恢复放大倍数，并保存 (确保还原时候能够还原到原来的大小)

		m_Multiple_width = float(1)/ m_Multiple_width ;
		m_Mutiple_heith = float(1)/m_Mutiple_heith ;

		//修正在不同的分辨率屏幕下的刷新问题
		float m_x1 = cx -193;
		float m_x2 = cx -40;
		if(m_Multiple_width>1)
		{
			m_btn_param1.MoveWindow(m_x1,cy*0.02,150,50);
			m_btn_param2.MoveWindow(m_x1,cy*0.12,150,50);
			m_btn_param3.MoveWindow(m_x1,cy*0.22,150,50);
			m_btn_param4.MoveWindow(m_x1,cy*0.32,150,50);
			m_btn_param5.MoveWindow(m_x1,cy*0.42,150,50);

			m_delete_param1.MoveWindow(m_x2,cy*0.02,19,50);
			m_delete_param2.MoveWindow(m_x2,cy*0.12,19,50);
			m_delete_param3.MoveWindow(m_x2,cy*0.22,19,50);
			m_delete_Param4.MoveWindow(m_x2,cy*0.32,19,50);
			m_delete_param5.MoveWindow(m_x2,cy*0.42,19,50);
			m_delete_all.MoveWindow(m_x1,cy*0.50,150,42);
			m_btn_moveTime.MoveWindow(m_x1,cy*0.70,150,42);
			m_btn_move_right.MoveWindow(m_x1,cy*0.80,150,42);
		}
		else
		{
			m_btn_param1.MoveWindow(m_x1,cy*0.02,150,50);
			m_btn_param2.MoveWindow(m_x1,cy*0.12,150,50);
			m_btn_param3.MoveWindow(m_x1,cy*0.22,150,50);
			m_btn_param4.MoveWindow(m_x1,cy*0.32,150,50);
			m_btn_param5.MoveWindow(m_x1,cy*0.42,150,50);


			m_delete_param1.MoveWindow(m_x2,cy*0.02,19,50);
			m_delete_param2.MoveWindow(m_x2,cy*0.12,19,50);
			m_delete_param3.MoveWindow(m_x2,cy*0.22,19,50);
			m_delete_Param4.MoveWindow(m_x2,cy*0.32,19,50);
			m_delete_param5.MoveWindow(m_x2,cy*0.42,19,50);

			m_delete_all.MoveWindow(m_x1,cy*0.50,150,42);
			m_btn_moveTime.MoveWindow(m_x1,cy*0.70,150,42);
			m_btn_move_right.MoveWindow(m_x1,cy*0.80,150,42);

		}
	}	
}

void CRealTimeTrendDlg::ReSize( int nID )
{
	CRect Rect;
	GetDlgItem(nID)->GetWindowRect(Rect);
	ScreenToClient(Rect);
	//计算控件左上角点
	CPoint OldTLPoint,TLPoint;
	OldTLPoint = Rect.TopLeft();
	TLPoint.x = long(OldTLPoint.x *m_Multiple_width);
	TLPoint.y = long(OldTLPoint.y * m_Mutiple_heith );

	//计算控件右下角点
	CPoint OldBRPoint,BRPoint;
	OldBRPoint = Rect.BottomRight();
	BRPoint.x = long(OldBRPoint.x *m_Multiple_width);
	BRPoint.y = long(OldBRPoint.y * m_Mutiple_heith );
	//移动控件到新矩形

	Rect.SetRect(TLPoint,BRPoint);
	GetDlgItem(nID)->MoveWindow(Rect,TRUE);
	GetDlgItem(nID)->Invalidate(FALSE);
}
void CRealTimeTrendDlg::OnBnClickedButtonMoveline()   //左移
{
	UpdateData(TRUE);
	switch(m__nTimeRange)
	{
	case 0:
		oleTimeStart = oleTimeStart - COleDateTimeSpan(0,1,0,0);
		oleTimeEnd = oleTimeEnd  - COleDateTimeSpan(0,1,0,0);
		break;
	case 2:
		oleTimeStart = oleTimeStart - COleDateTimeSpan(0,6,0,0);
		oleTimeEnd = oleTimeEnd  - COleDateTimeSpan(0,6,0,0);
		break;
	case 3:
		oleTimeStart = oleTimeStart - COleDateTimeSpan(0,12,0,0);
		oleTimeEnd = oleTimeEnd  - COleDateTimeSpan(0,12,0,0);
		break;
	case 4:
		oleTimeStart = oleTimeStart - COleDateTimeSpan(1,0,0,0);
		oleTimeEnd = oleTimeEnd  - COleDateTimeSpan(1,0,0,0);
		break;
	case 5:
		oleTimeStart = oleTimeStart - COleDateTimeSpan(7,0,0,0);
		oleTimeEnd = oleTimeEnd  - COleDateTimeSpan(7,0,0,0);
		break;
	case 6:
		oleTimeStart = oleTimeStart - COleDateTimeSpan(30,0,0,0);
		oleTimeEnd = oleTimeEnd  - COleDateTimeSpan(30,0,0,0);
		break;
	default:
		break;
	}
	m_startTime.SetTime(oleTimeStart.GetHour(),oleTimeStart.GetMinute(),oleTimeStart.GetSecond());
	m_startDate.SetDate(oleTimeStart.GetYear(),oleTimeStart.GetMonth(),oleTimeStart.GetDay());
	m_endTime.SetTime(oleTimeEnd.GetHour(),oleTimeEnd.GetMinute(),oleTimeEnd.GetSecond());
	m_endDate.SetDate(oleTimeEnd.GetYear(),oleTimeEnd.GetMonth(),oleTimeEnd.GetDay());
	UpdateData(FALSE);
	DrawChart();
}

void CRealTimeTrendDlg::OnBnClickedButtonMoveright()
{
	UpdateData(TRUE);
	switch(m__nTimeRange)
	{
	case 0:
		oleTimeStart = oleTimeStart + COleDateTimeSpan(0,1,0,0);
		oleTimeEnd = oleTimeEnd  + COleDateTimeSpan(0,1,0,0);
		break;
	case 2:
		oleTimeStart = oleTimeStart + COleDateTimeSpan(0,6,0,0);
		oleTimeEnd = oleTimeEnd  + COleDateTimeSpan(0,6,0,0);
		break;
	case 3:
		oleTimeStart = oleTimeStart + COleDateTimeSpan(0,12,0,0);
		oleTimeEnd = oleTimeEnd  + COleDateTimeSpan(0,12,0,0);
		break;
	case 4:
		oleTimeStart = oleTimeStart + COleDateTimeSpan(1,0,0,0);
		oleTimeEnd = oleTimeEnd  + COleDateTimeSpan(1,0,0,0);
		break;
	case 5:
		oleTimeStart = oleTimeStart + COleDateTimeSpan(7,0,0,0);
		oleTimeEnd = oleTimeEnd  + COleDateTimeSpan(7,0,0,0);
		break;
	case 6:
		oleTimeStart = oleTimeStart + COleDateTimeSpan(30,0,0,0);
		oleTimeEnd = oleTimeEnd  + COleDateTimeSpan(30,0,0,0);
		break;
	default:
		break;
	}
	m_startTime.SetTime(oleTimeStart.GetHour(),oleTimeStart.GetMinute(),oleTimeStart.GetSecond());
	m_startDate.SetDate(oleTimeStart.GetYear(),oleTimeStart.GetMonth(),oleTimeStart.GetDay());
	m_endTime.SetTime(oleTimeEnd.GetHour(),oleTimeEnd.GetMinute(),oleTimeEnd.GetSecond());
	m_endDate.SetDate(oleTimeEnd.GetYear(),oleTimeEnd.GetMonth(),oleTimeEnd.GetDay());
	UpdateData(FALSE);
	DrawChart();
}
void CRealTimeTrendDlg::EnableControlsOper()
{
	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM1))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE1))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM2))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE2))->EnableWindow(TRUE);

	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM3))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE3))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM4))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE4))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM5))->EnableWindow(TRUE);  

	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE5))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETEALL))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_MOVELINE))->EnableWindow(TRUE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_MOVERIGHT))->EnableWindow(TRUE);

	MSG msg;         //按钮禁止未实现，清空消息队列
	while(PeekMessage(&msg, GetSafeHwnd(), WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE ))
		NULL;
}

void CRealTimeTrendDlg::DisableControlsOper()
{
	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM1))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE1))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM2))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE2))->EnableWindow(FALSE);

	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM3))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE3))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM4))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE4))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_PARAM5))->EnableWindow(FALSE);  

	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETE5))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_DELETEALL))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_MOVELINE))->EnableWindow(FALSE);
	((CImageButton*)GetDlgItem(IDC_BUTTON_MOVERIGHT))->EnableWindow(FALSE);
}

void CRealTimeTrendDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
	case c_timer_update_id:
		OnTimerUpdate();
		break;
	}
}

void CRealTimeTrendDlg::OnTimerUpdate()
{
	UpdateVectorData();

	DrawHiseryCurveChart();
}

//////////////////////////////////////////////////////////////////////////
CWnd*   CRealTimeTrendDlg::GetDlgItem(int nID) const
{
	CWnd* pwind = __super::GetDlgItem(nID);
	ASSERT(pwind);
	return pwind;
}

//////////////////////////////////////////////////////////////////////////


void CRealTimeTrendDlg::OnBnClickedButtonInfo1()
{
	// TODO: Add your control notification handler code here
	OnBtnInfo(0);
}


void CRealTimeTrendDlg::OnBnClickedButtonInfo2()
{
	// TODO: Add your control notification handler code here
	OnBtnInfo(1);
}


void CRealTimeTrendDlg::OnBnClickedButtonInfo3()
{
	// TODO: Add your control notification handler code here
	OnBtnInfo(2);
}


void CRealTimeTrendDlg::OnBnClickedButtonInfo4()
{
	// TODO: Add your control notification handler code here
	OnBtnInfo(3);
}


void CRealTimeTrendDlg::OnBnClickedButtonInfo5()
{
	// TODO: Add your control notification handler code here
	OnBtnInfo(4);
}

void CRealTimeTrendDlg::OnBtnInfo( UINT idx )
{
	if(idx >= c_buttonNum)
		return;
	CPointInfoDlg dlg;
	CReadRealData& readData = g_vecReadData[idx];
	dlg.m_strParam1 =  readData.strParam1.c_str();
	dlg.m_strRemark =  readData.strDescription.c_str();
	dlg.m_strShortName =  readData.mStrPlcShortName.c_str();
	dlg.m_strSource =  readData.strSourceType.c_str();
	dlg.m_strUnit =  readData.strUnit.c_str();
	GetPointInfo(readData.m_vecData,dlg.m_nNum,dlg.m_nAvg,dlg.m_nMax,dlg.m_nMin);
	dlg.DoModal();
}

void CRealTimeTrendDlg::GetPointInfo( vector<double> &m_vecRecord,int &nNum, double &nAvg, double &nMax, double &nMin )
{
	nNum = m_vecRecord.size();
	nMax = 0;
	nMin = 0;
	nAvg = 0;
	double nTotal = 0;
	for(int i=0; i<nNum; i++)
	{
		double n = m_vecRecord[i];
		if(n>nMax)
			nMax = n;
		if(n<nMin)
			nMin = n;
		nTotal += n;
	}
	if(nNum>0)
		nAvg = nTotal/nNum;
}

void CRealTimeTrendDlg::OnBnClickedButtonManual()
{
	if (MessageBox(_T("您确定要手动设置范围?")) == IDOK)
	{
		m_bIsNeedUpdateLimits = true;
		CString str;
		GetDlgItem(IDC_EDIT_MAX)->GetWindowTextW(str);
		if (str.GetLength() == 0)
		{
			MessageBox(_T("最大值未设置"));
			return;
		}
		m_dYMax = _wtof(str.GetString());
		GetDlgItem(IDC_EDIT_MIN)->GetWindowTextW(str);
		if (str.GetLength() == 0)
		{
			MessageBox(_T("最大值未设置"));
			return;
		}
		m_dYMin = _wtof(str.GetString());
	}
}

void CRealTimeTrendDlg::OnBnClickedButtonAuto()
{
	if (MessageBox(_T("您确定要自动设置范围?")) == IDOK)
	{
		m_bIsNeedUpdateLimits = false;
	}
}

void CRealTimeTrendDlg::SaveRealPoint( UINT dlgIdx,UINT buttonIdx,UINT pointIdx, const wstring& wstrPlcShort, const wstring& strDescription, const wstring& strUnit, const wstring& strSourceType , const wstring& strParam1 )
{
	CString strExePath;
	Project::Tools::GetSysPath(strExePath);
	strExePath += L"\\debugtool.ini";

	CString strNode;
	strNode.Format(_T("实时%d"),dlgIdx);

	CString strNodeChild;
	strNodeChild.Format(_T("点%d"),buttonIdx);

	CString strNodeChildInfo;
	strNodeChildInfo.Format(_T("%d,%d,%d,%s,%s,%s,%s,%s"),dlgIdx,buttonIdx,pointIdx,wstrPlcShort.c_str(),strDescription.c_str(),strUnit.c_str(),strSourceType.c_str(),strParam1.c_str());

	WritePrivateProfileString (strNode,strNodeChild, strNodeChildInfo,strExePath);
}

void CRealTimeTrendDlg::DeleteRealPoint( UINT dlgIdx,UINT buttonIdx )
{
	CString strExePath;
	Project::Tools::GetSysPath(strExePath);
	strExePath += L"\\debugtool.ini";

	CString strNode;
	strNode.Format(_T("实时%d"),dlgIdx);

	CString strNodeChild;
	strNodeChild.Format(_T("点%d"),buttonIdx);

	WritePrivateProfileString (strNode,strNodeChild, _T(""),strExePath);
}

void CRealTimeTrendDlg::SetDlgID( UINT dlgIdx )
{
	m_uDlgIdx = dlgIdx;
}

void CRealTimeTrendDlg::InitButtonData(UINT dlgIdx)
{
	CString strExePath;
	Project::Tools::GetSysPath(strExePath);
	strExePath += L"\\debugtool.ini";

	CString strNode;
	strNode.Format(_T("实时%d"),dlgIdx);

	wstring strPointInfo0,strPointInfo1,strPointInfo2,strPointInfo3,strPointInfo4;

	wchar_t strPointInfo[MAX_PATH] = {0};
	GetPrivateProfileString(strNode, _T("point0"), L"", strPointInfo, 1024, strExePath.GetString());
	strPointInfo0 = strPointInfo;

	GetPrivateProfileString(strNode, _T("point1"), L"", strPointInfo, 1024, strExePath.GetString());
	strPointInfo1 = strPointInfo;

	GetPrivateProfileString(strNode, _T("point2"), L"", strPointInfo, 1024, strExePath.GetString());
	strPointInfo2 = strPointInfo;

	GetPrivateProfileString(strNode, _T("point3"), L"", strPointInfo, 1024, strExePath.GetString());
	strPointInfo3 = strPointInfo;

	GetPrivateProfileString(strNode, _T("point4"), L"", strPointInfo, 1024, strExePath.GetString());
	strPointInfo4 = strPointInfo;

	vector<wstring> vecPointInfo;
	if(strPointInfo0 != L"")
	{
		Project::Tools::SplitStringByChar(strPointInfo0.c_str(),L',',vecPointInfo);
		if(vecPointInfo.size() == 8)
		{
			SetButtonProp(_wtoi(vecPointInfo[1].c_str()),_wtoi(vecPointInfo[2].c_str()),vecPointInfo[3],vecPointInfo[4],vecPointInfo[5],vecPointInfo[6],vecPointInfo[7]);
			CString strRemark = vecPointInfo[3].c_str();
			strRemark+=L"\r\n";
			strRemark +=vecPointInfo[4].c_str();
			m_vecBtnParam[0]->SetWindowText(strRemark);
		}
	}

	if(strPointInfo1 != L"")
	{
		Project::Tools::SplitStringByChar(strPointInfo1.c_str(),L',',vecPointInfo);
		if(vecPointInfo.size() == 8)
		{
			SetButtonProp(_wtoi(vecPointInfo[1].c_str()),_wtoi(vecPointInfo[2].c_str()),vecPointInfo[3],vecPointInfo[4],vecPointInfo[5],vecPointInfo[6],vecPointInfo[7]);
			CString strRemark = vecPointInfo[3].c_str();
			strRemark+=L"\r\n";
			strRemark +=vecPointInfo[4].c_str();
			m_vecBtnParam[1]->SetWindowText(strRemark);
		}
	}

	if(strPointInfo2 != L"")
	{
		Project::Tools::SplitStringByChar(strPointInfo2.c_str(),L',',vecPointInfo);
		if(vecPointInfo.size() == 8)
		{
			SetButtonProp(_wtoi(vecPointInfo[1].c_str()),_wtoi(vecPointInfo[2].c_str()),vecPointInfo[3],vecPointInfo[4],vecPointInfo[5],vecPointInfo[6],vecPointInfo[7]);
			CString strRemark = vecPointInfo[3].c_str();
			strRemark+=L"\r\n";
			strRemark +=vecPointInfo[4].c_str();
			m_vecBtnParam[2]->SetWindowText(strRemark);
		}
	}

	if(strPointInfo3 != L"")
	{
		Project::Tools::SplitStringByChar(strPointInfo3.c_str(),L',',vecPointInfo);
		if(vecPointInfo.size() == 8)
		{
			SetButtonProp(_wtoi(vecPointInfo[1].c_str()),_wtoi(vecPointInfo[2].c_str()),vecPointInfo[3],vecPointInfo[4],vecPointInfo[5],vecPointInfo[6],vecPointInfo[7]);
			CString strRemark = vecPointInfo[3].c_str();
			strRemark+=L"\r\n";
			strRemark +=vecPointInfo[4].c_str();
			m_vecBtnParam[3]->SetWindowText(strRemark);
		}
	}

	if(strPointInfo4 != L"")
	{
		Project::Tools::SplitStringByChar(strPointInfo4.c_str(),L',',vecPointInfo);
		if(vecPointInfo.size() == 8)
		{
			SetButtonProp(_wtoi(vecPointInfo[1].c_str()),_wtoi(vecPointInfo[2].c_str()),vecPointInfo[3],vecPointInfo[4],vecPointInfo[5],vecPointInfo[6],vecPointInfo[7]);
			CString strRemark = vecPointInfo[3].c_str();
			strRemark+=L"\r\n";
			strRemark +=vecPointInfo[4].c_str();
			m_vecBtnParam[4]->SetWindowText(strRemark);
		}
	}
	
	UpdateData(TRUE);
	DisableControlsOper();
	DrawHiseryCurveChart();
	EnableControlsOper();
}

void CRealTimeTrendDlg::Exit()
{
	KillTimer(c_timer_update_id);
}
