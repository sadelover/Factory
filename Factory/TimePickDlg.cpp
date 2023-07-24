// TimePickDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TimePickDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"


// CTimePickDlg 对话框

IMPLEMENT_DYNAMIC(CTimePickDlg, CDialogEx)

CTimePickDlg::CTimePickDlg(const RoiTimeType timeType, const COleDateTime tmStart, const COleDateTime tmOccur, const COleDateTime tmEnd, CWnd* pParent /*=NULL*/)
	: CDialogEx(CTimePickDlg::IDD, pParent)
	, m_roiTimeType(timeType)
	, m_tmStart(tmStart)
	, m_tmOccur(tmOccur)
	, m_tmEnd(tmEnd)
{

}

CTimePickDlg::~CTimePickDlg()
{
}

void CTimePickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATEPICKER, m_Date);
	DDX_Control(pDX, IDC_TIMEPICKER, m_Time);
}


BEGIN_MESSAGE_MAP(CTimePickDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTimePickDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTimePickDlg 消息处理程序


BOOL CTimePickDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Date.SetTime(Project::Tools::Time_tToSystemTime(m_nDateTime));
	m_Time.SetTime(Project::Tools::Time_tToSystemTime(m_nDateTime));	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTimePickDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	COleDateTime oleDate;
	COleDateTime oleTime;
	m_Date.GetTime(oleDate);
	m_Time.GetTime(oleTime);

	COleDateTime olelast;
	olelast.SetDateTime(oleDate.GetYear(),oleDate.GetMonth(),oleDate.GetDay(),oleTime.GetHour(),oleTime.GetMinute(),oleTime.GetSecond());

	COleDateTimeSpan	span;
	switch (m_roiTimeType)
	{
	case RoiTimeStart:
		{
			span = m_tmOccur - olelast;
			if (span.GetTotalSeconds() < 0)
			{
				MessageBox(_T("发生时间早于开始时间！"), _T("错误"), MB_OK|MB_ICONERROR);
				return;
			}

			span = m_tmEnd - olelast;
			if (span.GetTotalSeconds() < 0)
			{
				MessageBox(_T("结束时间早于开始时间！"), _T("错误"), MB_OK|MB_ICONERROR);
				return;
			}
		}
		break;
	case RoiTimeOccur:
		{
			span = olelast - m_tmStart;
			if (span.GetTotalSeconds() < 0)
			{
				MessageBox(_T("发生时间早于开始时间！"), _T("错误"), MB_OK|MB_ICONERROR);
				return;
			}

			span = m_tmEnd - olelast;
			if (span.GetTotalSeconds() < 0)
			{
				MessageBox(_T("结束时间早于发生时间！"), _T("错误"), MB_OK|MB_ICONERROR);
				return;
			}
		}
		break;
	case RoiTimeEnd:
		{
			span = olelast - m_tmStart;
			if (span.GetTotalSeconds() < 0)
			{
				MessageBox(_T("结束时间早于开始时间！"), _T("错误"), MB_OK|MB_ICONERROR);
				return;
			}

			span = olelast - m_tmOccur;
			if (span.GetTotalSeconds() < 0)
			{
				MessageBox(_T("结束时间早于发生时间！"), _T("错误"), MB_OK|MB_ICONERROR);
				return;
			}
		}
		break;
	default:
		break;
	}

	SYSTEMTIME st;
	if (olelast.GetAsSystemTime(st))
	{
		m_nDateTime = (int)Project::Tools::SystemTimeToTime_t(st);
	}

	CDialogEx::OnOK();
}
