// HistoryDataDownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HistoryDataDownloadDlg.h"
#include "afxdialogex.h"
#include "../ControlEx/WarningDlg.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "MainFrm.h"
#include "json/reader.h"


// CHistoryDataDownloadDlg dialog

IMPLEMENT_DYNAMIC(CHistoryDataDownloadDlg, CDialogEx)

CHistoryDataDownloadDlg::CHistoryDataDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoryDataDownloadDlg::IDD, pParent)
	, m_strPointList(_T(""))
{
	m_strServerIP = L"127.0.0.1";
	m_nTimePeriod = 0;

	m_hThread = NULL;


	m_EventStopThread = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CHistoryDataDownloadDlg::~CHistoryDataDownloadDlg()
{
}

void CHistoryDataDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_DateFrom);
	DDX_DateTimeCtrl(pDX, IDC_TIME_START, m_TimeFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_DateTo);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_TimeTo);
	DDX_CBIndex(pDX, IDC_COMBO_STEP, m_nTimePeriod);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_Progress);
	DDX_Text(pDX, IDC_EDIT_POINTLIST, m_strPointList);
}


BEGIN_MESSAGE_MAP(CHistoryDataDownloadDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHistoryDataDownloadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHistoryDataDownloadDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CHistoryDataDownloadDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CHistoryDataDownloadDlg message handlers

bool CHistoryDataDownloadDlg::DownloadData(COleDateTime tFrom, COleDateTime tTo, CStdioFile *pFile)
{

	int nId = -1;
	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strServerIP, 5000, _T("get_history_data_padded"));

	wstring wstrTimeStart,wstrTimeEnd ;
	Project::Tools::OleTime_To_String(tFrom, wstrTimeStart );
	Project::Tools::OleTime_To_String(tTo, wstrTimeEnd );

	Json::Value myRoot;

	for(int i=0;i<m_strPointNameList.size();i++)
	{
		myRoot["pointList"].append(Project::Tools::WideCharToUtf8(m_strPointNameList[i].GetString()));

	}


	myRoot["timeStart"] = Project::Tools::WideCharToUtf8(wstrTimeStart.c_str());
	myRoot["timeEnd"] = Project::Tools::WideCharToUtf8(wstrTimeEnd.c_str());

	string strPeriod = "m1";


	myRoot["timeFormat"] = "m1";
	if(m_nTimePeriod==1)
		myRoot["timeFormat"] = "m5";
	else if(m_nTimePeriod==2){
		myRoot["timeFormat"] = "h1";
	}else if(m_nTimePeriod==3){
		myRoot["timeFormat"] = "d1";
	}
	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
	if(bRet)
	{
		Json::Reader jsReader;
		string strResult_utf8;
		Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
		Json::Value myReturnRoot;
		if (!jsReader.parse(strResult_utf8, myReturnRoot))
		{
			return false;
		}
		else
		{
			if(myReturnRoot.isObject())
			{
				Json::Value dataObj = myReturnRoot["map"];

				Json::Value timeObj = myReturnRoot["time"];

				if(dataObj.isObject())
				{

					CString strRowAll;
					for(int tt=0;tt<timeObj.size();tt++)
					{
						Json::Value strTime = timeObj[Json::UInt(tt)];
						wstring wstrTime ;
						Project::Tools::UTF8ToWideChar(strTime.asString(), wstrTime);
						strRowAll+= wstrTime.c_str();
						strRowAll+=",";
						for(int i=0;i<m_strPointNameList.size();i++)
						{
							string strPointName_utf8 = Project::Tools::WideCharToUtf8(m_strPointNameList[i].GetString());
							Json::Value myPointData = dataObj[strPointName_utf8];
							if(myPointData.isArray() && myPointData.size()>tt)
							{
								Json::Value pv = myPointData.get(Json::Value::UInt(tt), Json::Value(""));

								if(pv.isDouble())
								{
									CString wstrVV;
									wstrVV.Format(_T("%.3f"), pv.asDouble());
									strRowAll+= wstrVV;
									strRowAll+=",";

								}
								else if(pv.isString())
								{
									string strVV =  pv.asString();
									wstring wstrVV;
									Project::Tools::UTF8ToWideChar(strVV, wstrVV);
									strRowAll+= wstrVV.c_str();
									strRowAll+=",";
								}
								else
								{

									strRowAll+=",";
								}

							}
							else
							{
								strRowAll+=",";
							}
						}

						strRowAll+=_T("\n");
					}

					pFile->WriteString(strRowAll.GetString());


				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}



UINT WINAPI CHistoryDataDownloadDlg::RunDownloadThread( void * pParam )
{//@todo       need Timestamp

	CHistoryDataDownloadDlg *pThis = (CHistoryDataDownloadDlg *) pParam;

	


	if(pThis->m_nTimePeriod==0 || pThis->m_nTimePeriod==1)
	{

	}

	COleDateTimeSpan tspan = (pThis->m_tEnd - pThis->m_tStart);

	pThis->m_Progress.SetRange(0,int(tspan.GetTotalDays()));
	pThis->m_Progress.SetStep(1);

	pThis->GetDlgItem(IDOK)->EnableWindow(FALSE);
	pThis->GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	
	CStdioFile filehandle;
	CString strTemp;
	if(filehandle.Open(pThis->m_strSaveFileName, CFile::modeCreate|CFile::modeWrite))
	{


		COleDateTime tCur = pThis->m_tStart;

		CString strHeader = _T("Time");
		strHeader+=_T(",");
		for(int i=0;i<pThis->m_strPointNameList.size();i++)
		{

			strHeader+= pThis->m_strPointNameList[i];
			strHeader+=_T(",");
		}

		strHeader+=_T("\n");
		filehandle.WriteString(strHeader);

		while(tCur<=pThis->m_tEnd)
		{
			if(::WaitForSingleObject(pThis->m_EventStopThread, 0)== WAIT_OBJECT_0)
			{
				break;
			}

			COleDateTime tNext = COleDateTime(tCur.GetYear(), tCur.GetMonth(), tCur.GetDay(), 23,59,59);
			if(tNext>pThis->m_tEnd)
				tNext = pThis->m_tEnd;
			pThis->DownloadData(tCur, tNext, &filehandle);

			pThis->m_Progress.StepIt();

			tCur = tNext+COleDateTimeSpan(0,0,1,0);
		}

		filehandle.Close();

		pThis->m_Progress.SetPos(0);
		AfxMessageBox(_T("数据导出文件保存成功"));
	}


	pThis->GetDlgItem(IDOK)->EnableWindow(TRUE);
	pThis->GetDlgItem(IDCANCEL)->EnableWindow(TRUE);


	return 0;
}


void CHistoryDataDownloadDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//字符串替换
	m_strPointList.Replace(_T("\r"), _T(","));
	m_strPointList.Replace(_T("\n"), _T(","));
	m_strPointList.Replace(_T("\t"), _T(","));
	m_strPointList.Replace(_T(" "), _T(","));

	vector<CString> strArray;
	Project::Tools::SplitStringByChar(m_strPointList, ',', strArray);

	m_strPointNameList.clear();
	for(int i=0;i<strArray.size();i++)
	{
		if(strArray[i].GetLength()>0)
		{
			CString strPN = strArray[i];
			strPN.Replace(_T(" "), _T(""));

			if(strPN.GetLength()<=0)
				continue;
			bool bExist = false;
			for(int j=0;j<m_strPointNameList.size();j++)
			{
				if(m_strPointNameList[j]==strPN)
					bExist = true;
			}
			if(!bExist)
				m_strPointNameList.push_back(strPN);
		}
	}


	CString strTemp;

	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_strSaveFileName = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			return;
		}
		else
		{
		}	
	}
	else
	{
		return;
	}



	{

		COleDateTimeSpan tspan = COleDateTimeSpan(0,0,1,0);
		COleDateTime tStart(m_DateFrom.GetYear(), m_DateFrom.GetMonth(),m_DateFrom.GetDay(),
			m_TimeFrom.GetHour(), m_TimeFrom.GetMinute(), 0);
		COleDateTime tEnd(m_DateTo.GetYear(), m_DateTo.GetMonth(),m_DateTo.GetDay(),
			m_TimeTo.GetHour(), m_TimeTo.GetMinute(), 0);

		m_tStart = tStart;
		m_tEnd = tEnd;

		m_hThread = NULL;
		unsigned threadID;
		m_hThread = (HANDLE)_beginthreadex( NULL, 0, RunDownloadThread, (LPVOID)this, 0, &threadID );	
		if (m_hThread)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		CString strShow;
		CString strRet(_T("\n")); 
		CString strTips;

		
	}
}



void CHistoryDataDownloadDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here


	CDialogEx::OnCancel();
}


BOOL CHistoryDataDownloadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_strPointNameList.clear();

	if(m_pFrame->m_pPrjSqlite)
	{
		wstring strAll = m_pFrame->m_pPrjSqlite->GetValueInLocalConfigTable(_T("download_points") );
		string strResult_utf8;
		Project::Tools::WideCharToUtf8(strAll, strResult_utf8);
		Json::Value myConfig;
		Json::Reader jsReader;
		if (!jsReader.parse(strResult_utf8, myConfig))
		{

			AfxMessageBox(_T("项目组态文件配置中需包含download_points定义"));
			return TRUE;
		}
		Json::Value jsonPointList = myConfig["pointList"];
		if(jsonPointList.isNull() || !jsonPointList.isArray())
		{
			AfxMessageBox(_T("项目组态文件配置download_points定义格式不正确"));
			return TRUE;
		}
		for (int i=0;i<jsonPointList.size();i++)
		{
			Json::Value oneStr = jsonPointList[Json::UInt(i)];
			wstring wstrPName = Project::Tools::UTF8ToWideChar(oneStr.asString().data());
			m_strPointNameList.push_back(wstrPName.c_str());
		}
	}

	CString strAll;
	for(int i=0;i<m_strPointNameList.size();i++)
	{
		strAll+=m_strPointNameList[i];
		if((i+1)<m_strPointNameList.size())
			strAll+=_T(",");

	}

	m_strPointList = strAll;
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CHistoryDataDownloadDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here

	SetEvent(m_EventStopThread);
	::WaitForSingleObject(m_hThread, INFINITE);

	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}
