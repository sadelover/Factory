// ImportHistoryDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportHistoryDataDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "HttpOperation.h"
#include <iostream>
#include "json/json.h"

// CImportHistoryDataDlg dialog

IMPLEMENT_DYNAMIC(CImportHistoryDataDlg, CDialogEx)

CImportHistoryDataDlg::CImportHistoryDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportHistoryDataDlg::IDD, pParent)
	, m_strInfo(_T(""))
	, m_strEditFileName1(_T(""))
	, m_strEditFileName2(_T(""))
	, m_strEditFileName3(_T(""))
	, m_strEditFileName4(_T(""))
	, m_strEditFileName5(_T(""))
	, m_strEditFileName6(_T(""))
	, m_strInfo2(_T(""))
	, m_strInfo3(_T(""))
{
	m_strServerIP = _T("127.0.0.1");
	m_nServerPort = 5000;
}

CImportHistoryDataDlg::~CImportHistoryDataDlg()
{
}

void CImportHistoryDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PROGRESS, m_strInfo);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strEditFileName1);
	DDX_Text(pDX, IDC_EDIT_FILENAME2, m_strEditFileName2);
	DDX_Text(pDX, IDC_EDIT_FILENAME3, m_strEditFileName3);
	DDX_Text(pDX, IDC_EDIT_FILENAME4, m_strEditFileName4);
	DDX_Text(pDX, IDC_EDIT_FILENAME5, m_strEditFileName5);
	DDX_Text(pDX, IDC_EDIT_FILENAME6, m_strEditFileName6);
	DDX_Text(pDX, IDC_STATIC_PROGRESS2, m_strInfo2);
	DDX_Text(pDX, IDC_STATIC_PROGRESS3, m_strInfo3);
}


BEGIN_MESSAGE_MAP(CImportHistoryDataDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_HISTORY, &CImportHistoryDataDlg::OnBnClickedButtonImportHistory)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE, &CImportHistoryDataDlg::OnBnClickedButtonLoadfile)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE2, &CImportHistoryDataDlg::OnBnClickedButtonLoadfile2)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE3, &CImportHistoryDataDlg::OnBnClickedButtonLoadfile3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE4, &CImportHistoryDataDlg::OnBnClickedButtonLoadfile4)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE5, &CImportHistoryDataDlg::OnBnClickedButtonLoadfile5)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE6, &CImportHistoryDataDlg::OnBnClickedButtonLoadfile6)
END_MESSAGE_MAP()


// CImportHistoryDataDlg message handlers


void CImportHistoryDataDlg::OnBnClickedButtonImportHistory()
{
	// TODO: Add your control notification handler code here

	HANDLE hThread = NULL;
	unsigned threadID;
	hThread = (HANDLE)_beginthreadex( NULL, 0, RunDownloadDataFileThread, (LPVOID)this, 0, &threadID );	
	if (hThread)
	{
		CloseHandle(hThread);
		hThread = NULL;
	}

}



unsigned __stdcall CImportHistoryDataDlg::RunDownloadDataFileThread( void * pParam )
{//@todo       need Timestamp

	CImportHistoryDataDlg *pThis = (CImportHistoryDataDlg *) pParam;


	
	
	pThis->GetDlgItem(IDC_BUTTON_IMPORT_HISTORY)->EnableWindow(FALSE);
	pThis->GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	pThis->GetDlgItem(IDC_BUTTON_LOADFILE)->EnableWindow(FALSE);
	pThis->GetDlgItem(IDC_BUTTON_LOADFILE2)->EnableWindow(TRUE);
	pThis->GetDlgItem(IDC_BUTTON_LOADFILE3)->EnableWindow(TRUE);
	
	CFile cfile; 
	CFileStatus rStatus;
 	CFile::GetStatus(pThis->m_strEditFileName1,rStatus);



	CHttpOperation http(pThis->m_strServerIP, 5000, _T("data/importHistoryDataFile"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();

	CString strErrorMsg;
	
	if(pThis->m_strEditFileName1.GetLength()>0)
	{
		pThis->SetTimer(1, 5000,NULL);
		strErrorMsg+=http.UploadPic(pThis->m_strEditFileName1, pThis->m_strServerIP, pThis->m_nServerPort, _T("data/importHistoryDataFile")); 
	}
	

	
	if(pThis->m_strEditFileName2.GetLength()>0)
	{
		pThis->SetTimer(2, 5000,NULL);
		strErrorMsg+=http.UploadPic(pThis->m_strEditFileName2, pThis->m_strServerIP, pThis->m_nServerPort, _T("data/importHistoryDataFile")); 
	}

	
	if(pThis->m_strEditFileName3.GetLength()>0)
	{
		pThis->SetTimer(3, 5000,NULL);
		strErrorMsg+=http.UploadPic(pThis->m_strEditFileName3, pThis->m_strServerIP, pThis->m_nServerPort, _T("data/importHistoryDataFile")); 
	}

	if(pThis->m_strEditFileName4.GetLength()>0)
		strErrorMsg+=http.UploadPic(pThis->m_strEditFileName4, pThis->m_strServerIP, pThis->m_nServerPort, _T("data/importHistoryDataFile")); 

	if(pThis->m_strEditFileName5.GetLength()>0)
		strErrorMsg+=http.UploadPic(pThis->m_strEditFileName5, pThis->m_strServerIP, pThis->m_nServerPort, _T("data/importHistoryDataFile")); 

	if(pThis->m_strEditFileName6.GetLength()>0)
		strErrorMsg+=http.UploadPic(pThis->m_strEditFileName6, pThis->m_strServerIP, pThis->m_nServerPort, _T("data/importHistoryDataFile")); 

	pThis->GetDlgItem(IDC_BUTTON_IMPORT_HISTORY)->EnableWindow(TRUE);
	pThis->GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	pThis->GetDlgItem(IDC_BUTTON_LOADFILE)->EnableWindow(TRUE);
	pThis->GetDlgItem(IDC_BUTTON_LOADFILE2)->EnableWindow(TRUE);
	pThis->GetDlgItem(IDC_BUTTON_LOADFILE3)->EnableWindow(TRUE);
	if(strErrorMsg.GetLength()>0)
	{
		AfxMessageBox(strErrorMsg);
	}
	else
	{
		AfxMessageBox(_T("导入成功"));
	}
	return 1;
}

void CImportHistoryDataDlg::OnBnClickedButtonLoadfile()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	CFileDialog dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xls;*.xlsx)|*.xls;*.xlsx|"), NULL);
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{
		//pFrame->m_pPrjSqlite->DeleteAllPoint();
		CString strFilePath = dlg.GetPathName();
		m_strEditFileName1= dlg.GetPathName();
		UpdateData(FALSE);
	}
} 


void CImportHistoryDataDlg::OnBnClickedButtonLoadfile2()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  

	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	CFileDialog dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xls;*.xlsx)|*.xls;*.xlsx|"), NULL);
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{
		//pFrame->m_pPrjSqlite->DeleteAllPoint();
		CString strFilePath = dlg.GetPathName();
		m_strEditFileName2= dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CImportHistoryDataDlg::OnBnClickedButtonLoadfile3()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	CFileDialog dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xls;*.xlsx)|*.xls;*.xlsx|"), NULL);
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{
		//pFrame->m_pPrjSqlite->DeleteAllPoint();
		CString strFilePath = dlg.GetPathName();
		m_strEditFileName3= dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CImportHistoryDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{
		CHttpOperation http(m_strServerIP, 5000, _T("data/getImportHistoryDataProcessInfo"));

		Json::Value myRoot;
		myRoot["data"] = "test";
		string szBuf = myRoot.toStyledString();
		CString strResult;
		//请求接口
		bool bRet= http.HttpPostGetRequestEx(0, "1", strResult,  _T("Content-Type: application/json"));

		//拿到json数据里的进度信息，字符串
		if(bRet)
		{
			Json::Reader jsReader;
			string strResult_utf8;
			Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
			Json::Value myReturnRoot;
			if (!jsReader.parse(strResult_utf8, myReturnRoot))
			{
				return ;
			}
			else
			{
				if(myReturnRoot.isObject())
				{
					Json::Value myData =  myReturnRoot["data"];
					Json::Value::Members mem =myData.getMemberNames();//key的值
					int i=0;
					for (auto iter = mem.begin(); iter != mem.end(),i<mem.size(); iter++,i++)
					{

						//string DllName = mem[i];
						string DllName = (*iter); //key
						Json::Value oneItem = myData[DllName];//value
						Json::Value ProcessID=oneItem["ProcessID"];
						wstring wstritem;
						Project::Tools::UTF8ToWideChar(ProcessID.asString(), wstritem );
		                m_strInfo = wstritem.c_str();
						if(m_strInfo==_T("finish"))
							KillTimer(1);
						UpdateData(FALSE);// 内存变量更新到界面.
		             }
		

	            }
			}
		}
	}
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==2)
	{
		CHttpOperation http(m_strServerIP, 5000, _T("data/getImportHistoryDataProcessInfo"));

		Json::Value myRoot;
		myRoot["data"] = "test";
		string szBuf = myRoot.toStyledString();
		CString strResult;
		//请求接口
		bool bRet= http.HttpPostGetRequestEx(0, "1", strResult,  _T("Content-Type: application/json"));

		//拿到json数据里的进度信息，字符串
		if(bRet)
		{
			Json::Reader jsReader;
			string strResult_utf8;
			Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
			Json::Value myReturnRoot;
			if (!jsReader.parse(strResult_utf8, myReturnRoot))
			{
				return ;
			}
			else
			{
				if(myReturnRoot.isObject())
				{
					Json::Value myData =  myReturnRoot["data"];
					Json::Value::Members mem =myData.getMemberNames();//key的值
					int i=0;
					for (auto iter = mem.begin(); iter != mem.end(),i<mem.size(); iter++,i++)
					{

						//string DllName = mem[i];
						string DllName = (*iter); //key
						Json::Value oneItem = myData[DllName];//value
						Json::Value ProcessID=oneItem["ProcessID"];
						wstring wstritem;
						Project::Tools::UTF8ToWideChar(ProcessID.asString(), wstritem );
						m_strInfo2 = wstritem.c_str();
						if(m_strInfo2==_T("finish"))
							KillTimer(2);
						UpdateData(FALSE);// 内存变量更新到界面.
					}


				}
			}
		}
	}
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==3)
	{
		CHttpOperation http(m_strServerIP, 5000, _T("data/getImportHistoryDataProcessInfo"));

		Json::Value myRoot;
		myRoot["data"] = "test";
		string szBuf = myRoot.toStyledString();
		CString strResult;
		//请求接口
		bool bRet= http.HttpPostGetRequestEx(0, "1", strResult,  _T("Content-Type: application/json"));

		//拿到json数据里的进度信息，字符串
		if(bRet)
		{
			Json::Reader jsReader;
			string strResult_utf8;
			Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
			Json::Value myReturnRoot;
			if (!jsReader.parse(strResult_utf8, myReturnRoot))
			{
				return ;
			}
			else
			{
				if(myReturnRoot.isObject())
				{
					Json::Value myData =  myReturnRoot["data"];
					Json::Value::Members mem =myData.getMemberNames();//key的值
					int i=0;
					for (auto iter = mem.begin(); iter != mem.end(),i<mem.size(); iter++,i++)
					{

						//string DllName = mem[i];
						string DllName = (*iter); //key
						Json::Value oneItem = myData[DllName];//value
						Json::Value ProcessID=oneItem["ProcessID"];
						wstring wstritem;
						Project::Tools::UTF8ToWideChar(ProcessID.asString(), wstritem );
						m_strInfo3 = wstritem.c_str();
						if(m_strInfo3==_T("finish"))
							KillTimer(3);
						UpdateData(FALSE);// 内存变量更新到界面.
					}


				}
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CImportHistoryDataDlg::OnBnClickedButtonLoadfile4()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  

	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	CFileDialog dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xls;*.xlsx)|*.xls;*.xlsx|"), NULL);
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{
		//pFrame->m_pPrjSqlite->DeleteAllPoint();
		CString strFilePath = dlg.GetPathName();
		m_strEditFileName4= dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CImportHistoryDataDlg::OnBnClickedButtonLoadfile5()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  

	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	CFileDialog dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xls;*.xlsx)|*.xls;*.xlsx|"), NULL);
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{
		//pFrame->m_pPrjSqlite->DeleteAllPoint();
		CString strFilePath = dlg.GetPathName();
		m_strEditFileName5= dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CImportHistoryDataDlg::OnBnClickedButtonLoadfile6()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  

	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	CFileDialog dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xls;*.xlsx)|*.xls;*.xlsx|"), NULL);
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{
		//pFrame->m_pPrjSqlite->DeleteAllPoint();
		CString strFilePath = dlg.GetPathName();
		m_strEditFileName6= dlg.GetPathName();
		UpdateData(FALSE);
	}
}
