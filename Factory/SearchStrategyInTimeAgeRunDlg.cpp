// SearchStrategyInTimeAgeRunDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SearchStrategyInTimeAgeRunDlg.h"
#include "afxdialogex.h"
#include "Tools\DirectoryOperations.h"
#include "Tools\CustomTools\CustomTools.h"

// CSearchStrategyInTimeAgeRunDlg 对话框

IMPLEMENT_DYNAMIC(CSearchStrategyInTimeAgeRunDlg, CDialogEx)

CSearchStrategyInTimeAgeRunDlg::CSearchStrategyInTimeAgeRunDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchStrategyInTimeAgeRunDlg::IDD, pParent)
	, m_edit_log(_T(""))
{
}

CSearchStrategyInTimeAgeRunDlg::~CSearchStrategyInTimeAgeRunDlg()
{
}

void CSearchStrategyInTimeAgeRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATE, m_DTPicker_date);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIME, m_DTPicker_time);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressctrl1);
	DDX_Text(pDX, IDC_EDIT_LOG, m_edit_log);
	DDX_Control(pDX, IDC_LIST_STRATEGY, m_list_strategy);
}


void CSearchStrategyInTimeAgeRunDlg::ReadLogFileContents(CString& strLogFilePath, CString& strLogFileContents)
{
	CStdioFile fileControl;

	if(fileControl.Open(strLogFilePath, CFile::modeRead | CFile::typeText | CFile::shareDenyNone | CFile::shareCompat ) == TRUE)
	{
		string strAllLog;//所有的缓存
		string strTempLogLine;//每一行的缓存

		while(true)//读文件中的log
		{
			char *ch = new char[1024];
			int nRdCount = fileControl.Read(ch,1024);
			if(nRdCount>0){//不到文件末尾返回值不会小于1
				string s(ch,ch+nRdCount);
				strAllLog+=s;
			}
			else{//到达文件末尾			
				break;
			}
			delete [] ch;
		}

		fileControl.Close();
		wstring wstrAllLog = Project::Tools::AnsiToWideChar(strAllLog.c_str());
		strLogFileContents = wstrAllLog.c_str();
	}
}
#include <regex>
void CSearchStrategyInTimeAgeRunDlg::SearchClosestTime(CString& strLogFileContents, COleDateTime& selectTime, COleDateTime& closestTime)
{
#if 0
	COleDateTime date;
	m_DTPicker_date.GetTime(date);
	CString strDate = date.Format(_T("%Y-%m-%d \\d\\d:\\d\\d:\\d\\d"));//获取到寻址目录
	wregex e(strDate.GetString());
	wcmatch sm;
	bool found = regex_search(strLogFileContents.GetString(), sm, e);
	if(found)
	{
		for(int i=0;i<sm.size();i++)
		{
			wstring strFind = sm[i].str();
			wstring strTime = strFind.substr(11,8);
			COleDateTime thisTime;
			thisTime.ParseDateTime(strTime.c_str(),VAR_TIMEVALUEONLY);
			CString strTest = thisTime.Format(L"%H:%M:%S");
			if(time>thisTime){
				continue;
			}
			else{
				closestTime = thisTime;
			}
		}
	}
#else
	wregex e(_T("\\d\\d:\\d\\d:\\d\\d"));
	vector<CString> strRow;
	Project::Tools::SplitStringByChar(strLogFileContents,L'\n',strRow);
	for(int i=0;i<strRow.size();i++)
	{
		wcmatch sm;
		bool found = regex_search(strRow[i].GetString(), sm, e);
		if(!found){
			continue;
		}
		wcsub_match sub1 = sm[0];
		wstring wstr = sub1.str();
		COleDateTime thisTime;
		thisTime.ParseDateTime(wstr.c_str(),VAR_TIMEVALUEONLY);
		//CString strThisTime = thisTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		//CString strTime = time_ectype.Format(_T("%Y-%m-%d %H:%M:%S"));
		if(selectTime>=thisTime)
		{
			closestTime = thisTime;
		}
		else{
			return;
		}
	}
#endif
}

void CSearchStrategyInTimeAgeRunDlg::BnClickedButtonSearch(void)
{
	int nDivisionValue = 0;//进度条分度值
	COleDateTime date;
	COleDateTime time;
	m_DTPicker_date.GetTime(date);
	m_DTPicker_time.GetTime(time);
	//设置此时间类是因为直接从界面中获取到的带日期子，在后续判断中日期会影响时间的判断。
	COleDateTime selectTime;
	selectTime.SetTime(time.GetHour(),time.GetMinute(),time.GetSecond());

	CString strLogDirName = date.Format(_T("logic-%Y-%m-%d"));//获取到寻址目录
	CString strCoreLogDirPath;
	Project::Tools::GetCoreLogPath(strCoreLogDirPath);

	if(false == DirectoryOperations::CheckFolderExist(strCoreLogDirPath, strLogDirName))
	{
		AfxMessageBox(L"当天没有产生日志目录");
		return ;
	}

	vector<CString> strLogFilePathList;
	DirectoryOperations::GetAllFilePathList(strCoreLogDirPath+L"\\"+strLogDirName, strLogFilePathList);
	if(strLogFilePathList.size() == 0){
		AfxMessageBox(L"当天没有产生日志");
		return;
	}
	nDivisionValue = (int)(100/strLogFilePathList.size());
	//时间段全部取出来，挨个判断其时间是不是比选择时间小，判断到一个时间大于的就是找到了，找到的这一个的上一个便是需要的最接近的时间
	//整个循环取得的所有时间拿出，再做比较
	vector<LogAndFindTime> findTimeList;
	for (int i=0; i<strLogFilePathList.size();i++)
	{
		CString strLogFileContents;
		LogAndFindTime temp;
		temp.strlogName = strLogFilePathList[i].Mid(strLogFilePathList[i].ReverseFind(L'\\')+1);
		temp.strlogName = temp.strlogName.Left(temp.strlogName.Find(L".txt"));
		temp.time = selectTime;
		temp.strLogPath = strLogFilePathList[i];
		ReadLogFileContents(strLogFilePathList[i], strLogFileContents);
		SearchClosestTime(strLogFileContents, selectTime, temp.time);
		findTimeList.push_back(temp);
		m_progressctrl1.SetPos(m_progressctrl1.GetPos()+nDivisionValue);
	}

	if(findTimeList.size()>0)
	{
		//找到最大时间的索引
		int findStrategyIndex = 0;
		for(int i=1;i<findTimeList.size();i++)
		{
			//CString temp1 = findTimeList[i].time.Format(_T("%Y-%m-%d %H:%M:%S"));
			if(findTimeList[i].time >=findTimeList[i].time){
				findStrategyIndex = i;
			}
		}

		for(int i=1;i<findTimeList.size();i++)
		{
			if(findTimeList[findStrategyIndex].time ==findTimeList[i].time)
			{
				m_findTimeList.push_back(findTimeList[i]);
				m_list_strategy.AddString(findTimeList[i].strlogName);
			}
		}
	}
	m_progressctrl1.SetPos(100);
}

LRESULT CSearchStrategyInTimeAgeRunDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}

BEGIN_MESSAGE_MAP(CSearchStrategyInTimeAgeRunDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSearchStrategyInTimeAgeRunDlg::OnBnClickedButtonSearch)
	 ON_MESSAGE(WM_UPDATEDATA, OnUpdateData)
	 ON_LBN_DBLCLK(IDC_LIST_STRATEGY, &CSearchStrategyInTimeAgeRunDlg::OnLbnDblclkListStrategy)
END_MESSAGE_MAP()


// CSearchStrategyInTimeAgeRunDlg 消息处理程序


void CSearchStrategyInTimeAgeRunDlg::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_progressctrl1.SetPos(0);
	m_edit_log.Empty();
	m_findTimeList.clear();
	m_list_strategy.ResetContent();
	UpdateData(FALSE);
	AfxBeginThread(Thread_ButtonSearch,(PVOID*)this);
}

UINT Thread_ButtonSearch(PVOID param)
{
	CSearchStrategyInTimeAgeRunDlg* pThis = (CSearchStrategyInTimeAgeRunDlg*)param;
	pThis->GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(FALSE);
	pThis->BnClickedButtonSearch();
	::SendMessage(pThis->m_hWnd, WM_UPDATEDATA, FALSE,0);
	pThis->GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(TRUE);
	return 0;
}


BOOL CSearchStrategyInTimeAgeRunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_progressctrl1.SetRange(0,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSearchStrategyInTimeAgeRunDlg::OnLbnDblclkListStrategy()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nSel = m_list_strategy.GetCurSel();
	m_edit_log.Empty();

	COleDateTime date;
	COleDateTime time;
	m_DTPicker_date.GetTime(date);
	m_DTPicker_time.GetTime(time);
	//设置此时间类是因为直接从界面中获取到的带日期子，在后续判断中日期会影响时间的判断。
	COleDateTime selectTime;
	selectTime.SetTime(time.GetHour(),time.GetMinute(),time.GetSecond());
	//返回到Log列表中重新解码
	COleDateTime oneMinAgo;
	COleDateTime oneMin;
	oneMin.SetTime(0,1,0);
	oneMinAgo = selectTime - oneMin;
	//CString strThisTime = oneMinAgo.Format(_T("%Y-%m-%d %H:%M:%S"));
	wregex e(_T("\\d\\d:\\d\\d:\\d\\d"));
	vector<CString> strRow;
	CString strLogFileContents;
	ReadLogFileContents(m_findTimeList[nSel].strLogPath, strLogFileContents);
	Project::Tools::SplitStringByChar(strLogFileContents,L'\n',strRow);
	for(int i=0;i<strRow.size();i++)
	{
		wcmatch sm;
		bool found = regex_search(strRow[i].GetString(), sm, e);
		if(!found){
			continue;
		}
		wcsub_match sub1 = sm[0];
		wstring wstr = sub1.str();
		COleDateTime thisTime;
		thisTime.ParseDateTime(wstr.c_str(),VAR_TIMEVALUEONLY);
		if(selectTime>=thisTime)
		{
			if(thisTime>=oneMinAgo){
				m_edit_log = m_edit_log + strRow[i] + L"\r\n";
			}
		}
		else{
			break;
		}
	}
	UpdateData(FALSE);
}
