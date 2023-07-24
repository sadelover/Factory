// SearchStrategyInTimeAgeRunDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SearchStrategyInTimeAgeRunDlg.h"
#include "afxdialogex.h"
#include "Tools\DirectoryOperations.h"
#include "Tools\CustomTools\CustomTools.h"

// CSearchStrategyInTimeAgeRunDlg �Ի���

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
		string strAllLog;//���еĻ���
		string strTempLogLine;//ÿһ�еĻ���

		while(true)//���ļ��е�log
		{
			char *ch = new char[1024];
			int nRdCount = fileControl.Read(ch,1024);
			if(nRdCount>0){//�����ļ�ĩβ����ֵ����С��1
				string s(ch,ch+nRdCount);
				strAllLog+=s;
			}
			else{//�����ļ�ĩβ			
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
	CString strDate = date.Format(_T("%Y-%m-%d \\d\\d:\\d\\d:\\d\\d"));//��ȡ��ѰַĿ¼
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
	int nDivisionValue = 0;//�������ֶ�ֵ
	COleDateTime date;
	COleDateTime time;
	m_DTPicker_date.GetTime(date);
	m_DTPicker_time.GetTime(time);
	//���ô�ʱ��������Ϊֱ�Ӵӽ����л�ȡ���Ĵ������ӣ��ں����ж������ڻ�Ӱ��ʱ����жϡ�
	COleDateTime selectTime;
	selectTime.SetTime(time.GetHour(),time.GetMinute(),time.GetSecond());

	CString strLogDirName = date.Format(_T("logic-%Y-%m-%d"));//��ȡ��ѰַĿ¼
	CString strCoreLogDirPath;
	Project::Tools::GetCoreLogPath(strCoreLogDirPath);

	if(false == DirectoryOperations::CheckFolderExist(strCoreLogDirPath, strLogDirName))
	{
		AfxMessageBox(L"����û�в�����־Ŀ¼");
		return ;
	}

	vector<CString> strLogFilePathList;
	DirectoryOperations::GetAllFilePathList(strCoreLogDirPath+L"\\"+strLogDirName, strLogFilePathList);
	if(strLogFilePathList.size() == 0){
		AfxMessageBox(L"����û�в�����־");
		return;
	}
	nDivisionValue = (int)(100/strLogFilePathList.size());
	//ʱ���ȫ��ȡ�����������ж���ʱ���ǲ��Ǳ�ѡ��ʱ��С���жϵ�һ��ʱ����ڵľ����ҵ��ˣ��ҵ�����һ������һ��������Ҫ����ӽ���ʱ��
	//����ѭ��ȡ�õ�����ʱ���ó��������Ƚ�
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
		//�ҵ����ʱ�������
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


// CSearchStrategyInTimeAgeRunDlg ��Ϣ�������


void CSearchStrategyInTimeAgeRunDlg::OnBnClickedButtonSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_progressctrl1.SetRange(0,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSearchStrategyInTimeAgeRunDlg::OnLbnDblclkListStrategy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nSel = m_list_strategy.GetCurSel();
	m_edit_log.Empty();

	COleDateTime date;
	COleDateTime time;
	m_DTPicker_date.GetTime(date);
	m_DTPicker_time.GetTime(time);
	//���ô�ʱ��������Ϊֱ�Ӵӽ����л�ȡ���Ĵ������ӣ��ں����ж������ڻ�Ӱ��ʱ����жϡ�
	COleDateTime selectTime;
	selectTime.SetTime(time.GetHour(),time.GetMinute(),time.GetSecond());
	//���ص�Log�б������½���
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
