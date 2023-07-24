// CoreStatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoreStatusDlg.h"
#include "afxdialogex.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"
#include "MainFrm.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"

// CCoreStatusDlg dialog

IMPLEMENT_DYNAMIC(CCoreStatusDlg, CDialogEx)

CCoreStatusDlg::CCoreStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCoreStatusDlg::IDD, pParent)
{

}

CCoreStatusDlg::~CCoreStatusDlg()
{
}

void CCoreStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_DATATABLE, m_Grid);
}


BEGIN_MESSAGE_MAP(CCoreStatusDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_YEAR, &CCoreStatusDlg::OnBnClickedButtonYear)
	ON_BN_CLICKED(IDC_BUTTON_WEEK, &CCoreStatusDlg::OnBnClickedButtonWeek)
	ON_BN_CLICKED(IDC_BUTTON_DAY, &CCoreStatusDlg::OnBnClickedButtonDay)
	ON_BN_CLICKED(IDC_BUTTON_MONTH, &CCoreStatusDlg::OnBnClickedButtonMonth)
END_MESSAGE_MAP()


// CCoreStatusDlg message handlers

BOOL CCoreStatusDlg::OnInitDialog()
{
  	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	RestGrid();
	ReadLogFile();

	COleDateTime a(COleDateTime::GetCurrentTime());
	m_Grid.AutoSize(GVS_BOTH);
	m_Grid.Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCoreStatusDlg::ReadLogFile(void)
{
	CString csLogPath = L"";
	Project::Tools::GetCoreLogPath(csLogPath);
	csLogPath+=L"\\domlogic_health_report.log";
	CStdioFile fileCtrl;
	if(TRUE == fileCtrl.Open(csLogPath, CFile::modeRead|CFile::shareDenyNone, NULL))
	{
		CString temp = L"";
		while(TRUE == fileCtrl.ReadString(temp))
		{
			m_LogList.push_back(temp);
			temp.Empty();
		}
	}
}


void CCoreStatusDlg::RestGrid(void)
{
	m_Grid.DeleteAllItems();
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(5);
	m_Grid.SetItemText(0,0, _T("线程"));
	m_Grid.SetItemText(0,1, _T("策略"));
	m_Grid.SetItemText(0,2, _T("开始时间"));
	m_Grid.SetItemText(0,3, _T("结束时间"));
	m_Grid.SetItemText(0,4, _T("问题"));
	m_Grid.SetDefCellMargin(2);
	m_Grid.SetFixedRowCount(1);
}

void CCoreStatusDlg::OnBnClickedButtonYear()
{
	// TODO: 在此添加控件通知处理程序代码
	RestGrid();
	CTime weekDate(CTime::GetCurrentTime());
	CTime tempDate;
	long int nFindIndex = -1;
	CString csDate = L"";
	for(long int nDay=365;nDay>=0;nDay--)
	{
		CTimeSpan day(nDay,0,0,0);
		tempDate = weekDate - day;
		csDate = tempDate.Format(L"%Y-%m-%d");
		for(long int i=0; i<m_LogList.size(); i++)
		{
			if(m_LogList[i].Left(10) == csDate)
			{
				nFindIndex = i;
				break;
			}
		}
		if(nFindIndex != -1)
		{
			break;
		}
	}

	if(nFindIndex != -1)
	{
		for(long int i=nFindIndex; i<m_LogList.size(); i++)
		{
			vector<CString> LineContents;
			Project::Tools::SplitStringByChar(m_LogList[i], L"    ", LineContents);
			if(LineContents.size() != 6)
				continue;

			m_Grid.InsertRow(LineContents.at(1));//线程名
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 1, LineContents.at(2));//策略名
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 2, LineContents.at(3));//开始时间
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 3, LineContents.at(4));//结束时间
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 4, LineContents.at(5));//问题
		}
	}
}

void CCoreStatusDlg::OnBnClickedButtonMonth()
{
	// TODO: 在此添加控件通知处理程序代码
	RestGrid();
	CTime weekDate(CTime::GetCurrentTime());
	CTime tempDate;
	long int nFindIndex = -1;
	CString csDate = L"";
	for(long int nDay=30;nDay>=0;nDay--)
	{
		CTimeSpan day(nDay,0,0,0);
		tempDate = weekDate - day;
		csDate = tempDate.Format(L"%Y-%m-%d");
		for(long int i=0; i<m_LogList.size(); i++)
		{
			if(m_LogList[i].Left(10) == csDate)
			{
				nFindIndex = i;
				break;
			}
		}
		if(nFindIndex != -1)
		{
			break;
		}
	}

	if(nFindIndex != -1)
	{
		for(long int i=nFindIndex; i<m_LogList.size(); i++)
		{
			vector<CString> LineContents;
			Project::Tools::SplitStringByChar(m_LogList[i], L"    ", LineContents);
			if(LineContents.size() != 6)
				continue;

			m_Grid.InsertRow(LineContents.at(1));//线程名
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 1, LineContents.at(2));//策略名
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 2, LineContents.at(3));//开始时间
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 3, LineContents.at(4));//结束时间
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 4, LineContents.at(5));//问题
		}
	}
}

void CCoreStatusDlg::OnBnClickedButtonWeek()
{
	// TODO: 在此添加控件通知处理程序代码
	RestGrid();
	CTime weekDate(CTime::GetCurrentTime());
	CTime tempDate;
	long int nFindIndex = -1;
	CString csWeekDate = L"";
	for(long int nDay=7;nDay>=0;nDay--)
	{
		CTimeSpan day(nDay,0,0,0);
		tempDate = weekDate - day;
		csWeekDate = tempDate.Format(L"%Y-%m-%d");
		for(long int i=0; i<m_LogList.size(); i++)
		{
			if(m_LogList[i].Left(10) == csWeekDate)
			{
				nFindIndex = i;
				break;
			}
		}
		if(nFindIndex != -1)
		{
			break;
		}
	}
	
	if(nFindIndex != -1)
	{
		for(long int i=nFindIndex; i<m_LogList.size(); i++)
		{
			vector<CString> LineContents;
			Project::Tools::SplitStringByChar(m_LogList[i], L"    ", LineContents);
			if(LineContents.size() != 6)
				continue;

			m_Grid.InsertRow(LineContents.at(1));//线程名
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 1, LineContents.at(2));//策略名
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 2, LineContents.at(3));//开始时间
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 3, LineContents.at(4));//结束时间
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 4, LineContents.at(5));//问题
		}
	}
}


void CCoreStatusDlg::OnBnClickedButtonDay()
{
	// TODO: 在此添加控件通知处理程序代码
	RestGrid();
	CTime Date(CTime::GetCurrentTime());
	CString csToday = Date.Format(L"%Y-%m-%d");
	for(long int i=m_LogList.size()-1; i>=0; i--)
	{
		if(m_LogList[i].Left(10) == csToday)
		{
			vector<CString> LineContents;
			Project::Tools::SplitStringByChar(m_LogList[i], L"    ", LineContents);
			if(LineContents.size() != 6)
				continue;

			m_Grid.InsertRow(LineContents.at(1));//线程名
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 1, LineContents.at(2));//策略名
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 2, LineContents.at(3));//开始时间
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 3, LineContents.at(4));//结束时间
			m_Grid.SetItemText(m_Grid.GetRowCount()-1, 4, LineContents.at(5));//问题
		}
	}
	m_Grid.Refresh();
	UpdateData(FALSE);
}
