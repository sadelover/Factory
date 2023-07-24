// TacticLogQueryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TacticLogQueryDlg.h"
#include "afxdialogex.h"
#include "./DB_BasicIO/mySqlInclude/mysql.h"
#include "../Tools/Util/UtilString.h"
#include "../ControlEx/WarningDlg.h"
#include "MainFrm.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/DirectoryOperations.h"
#pragma comment(lib, "libmysql.lib")

const int g_nSqlLen = 30000;


// CTacticLogQueryDlg �Ի���

IMPLEMENT_DYNAMIC(CTacticLogQueryDlg, CDialogEx)

CTacticLogQueryDlg::CTacticLogQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTacticLogQueryDlg::IDD, pParent)
	, m_strTactLog(_T(""))
	, m_dateLog(COleDateTime::GetCurrentTime())
	, m_tLocateTime(COleDateTime::GetCurrentTime())
	, m_bLocateTime(FALSE)
	, m_cstrKeyWordSearch(_T(""))
{

	m_bLogDBExist = FALSE;

}

CTacticLogQueryDlg::~CTacticLogQueryDlg()
{
}

void CTacticLogQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_TACT_LOG, m_dateLog);
	DDX_Text(pDX, IDC_EDIT_TACT_LOG, m_strTactLog);
	DDX_Control(pDX, IDC_EDIT_TACT_LOG, m_Edit_Log);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_TACT_LOCATETIME, m_tLocateTime);
	DDX_Check(pDX, IDC_CHECK_LOCATE_TIME, m_bLocateTime);
	DDX_Text(pDX, IDC_EDIT_KEYWORD_SEARCH, m_cstrKeyWordSearch);
	DDX_Control(pDX, IDC_TREE_KeyWord_Line, m_treeKeywordLine);
}


BEGIN_MESSAGE_MAP(CTacticLogQueryDlg, CDialogEx)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIME_TACT_LOG, &CTacticLogQueryDlg::OnDtnDatetimechangeDatetimeTactLog)
	ON_BN_CLICKED(IDC_BTN_QUERY, &CTacticLogQueryDlg::OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_BTN_CLEAR_TILL_NOW, &CTacticLogQueryDlg::OnBnClickedBtnClearTillNow)
	ON_BN_CLICKED(IDC_BUTTON_KEYWORD_SEARCH, &CTacticLogQueryDlg::OnBnClickedButtonKeywordSearch)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_KeyWord_Line, &CTacticLogQueryDlg::OnTvnSelchangedTreeKeywordLine)
END_MESSAGE_MAP()


// CTacticLogQueryDlg ��Ϣ�������


BOOL CTacticLogQueryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	TCHAR szAppDir[MAX_PATH] = {0};
	TCHAR szCfgPath[MAX_PATH] = {0};
	TCHAR szBuf[MAX_PATH] = {0};

	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	PathRemoveFileSpec(szAppDir);
	PathCombine(szCfgPath, szAppDir, _T("debugtool.ini"));
	GetPrivateProfileString(_T("beopdebugtool"), _T("server"), _T("127.0.0.1"), szBuf, MAX_PATH, szCfgPath);
	m_strDbIp.Format(_T("%s"), szBuf);




	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	if(pFrame->m_wndFileView.GetDataAccess())
	{
		m_bLogDBExist = pFrame->m_wndFileView.GetDataAccess()->GetLogDatabaseExist();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CTacticLogQueryDlg::OnDtnDatetimechangeDatetimeTactLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CTacticLogQueryDlg::OnBnClickedBtnQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString cstrLogicLogDir;
	CString	strDirName;
	CString cstrDllLogName;
	CString	strMysqleTableName;
	CString strShow;
	
	strDirName = m_dateLog.Format(_T("logic-%Y-%m-%d"));
	strMysqleTableName = m_dateLog.Format(_T("log_%Y_%m_%d"));
	
	Project::Tools::GetSysPath(cstrLogicLogDir);
	cstrLogicLogDir = cstrLogicLogDir.Left(cstrLogicLogDir.ReverseFind(L'\\')) + L"\\core\\log\\" + strDirName + L"\\";
	cstrDllLogName = m_strDllName + L".dll.txt";

	if (DirectoryOperations::FileExists(cstrLogicLogDir, cstrDllLogName) == true)
	{
		CStdioFile fileControl;

		if(fileControl.Open(cstrLogicLogDir + cstrDllLogName, CFile::modeRead | CFile::typeText | CFile::shareDenyNone | CFile::shareCompat ) == TRUE)
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
			strShow = wstrAllLog.c_str();
		}
		else
		{
			m_strTactLog = _T("log�ļ���ʧ�ܣ�");
		}

		// show in edit box
		if (_T("") == strShow)
		{
			strShow = _T("��ѯ���Ϊ�գ�");
		}
		m_strTactLog = strShow;
		UpdateData(FALSE);

		if(m_bLocateTime)
		{
			int nLocateLine = m_Edit_Log.GetLineCount()-1;
			CString strFind;

			m_tLocateTime.SetDateTime(m_dateLog.GetYear(), m_dateLog.GetMonth(), m_dateLog.GetDay(), m_tLocateTime.GetHour(), m_tLocateTime.GetMinute(), m_tLocateTime.GetSecond());
			strFind = m_tLocateTime.Format(_T("%Y-%m-%d %H:%M"));
			for(int xx=0;xx<m_Edit_Log.GetLineCount();xx++)
			{
				CString strTemp; 
				int nLL = m_Edit_Log.LineLength(m_Edit_Log.LineIndex(xx));
				wchar_t *sBuf = (wchar_t*)malloc( sizeof(wchar_t) * (nLL+1) );
				if(sBuf != NULL)
				{
					sBuf[nLL] = '\0';
					m_Edit_Log.GetLine(xx,sBuf,nLL);
					strTemp.Format(_T("%s"), sBuf);
					free(sBuf);
				}

				if(strTemp.Find(strFind)>=0)
				{
					nLocateLine = xx;
					strTemp.ReleaseBuffer();
					break;
				}
				strTemp.ReleaseBuffer();
			}

			m_Edit_Log.LineScroll(nLocateLine, 0);
		}
		else
		{
			m_Edit_Log.LineScroll(m_Edit_Log.GetLineCount()-1, 0);
		}
		return;
	}
	else
	{
		/*
			�����ϰ汾 ��ѯmysql����
		*/
		CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();

		// call mysql
		MYSQL	m_mysql;

		CString	strIp = m_strDbIp;
		CString	strUsername = _T("root");
		CString	strPassword = _T("RNB.beop-2013");
		CString	strDbName = pFrame->GetMySQLDatabaseName();

		if(m_bLogDBExist)
		{
			strDbName = _T("domlog");
		}
		int		nPort = 3306;
		CString strSql;

		strSql.Format(_T("SELECT * FROM %s WHERE loginfo LIKE '%%[%s]%%'"), strMysqleTableName, m_strDllName+_T(".dll"));

		try
		{
			if (NULL == mysql_init(&m_mysql))
			{
				// init error
				return;
			}

			USES_CONVERSION;
			if (NULL == mysql_real_connect(&m_mysql, T2A((LPTSTR)(LPCTSTR)strIp), T2A((LPTSTR)(LPCTSTR)strUsername), T2A((LPTSTR)(LPCTSTR)strPassword), T2A((LPTSTR)(LPCTSTR)strDbName), nPort, NULL, 0))
			{
				// connect error
				m_strTactLog = _T("�������ݿ�ʧ�ܣ�\r\n����������Ϣ��");
				UpdateData(FALSE);
				return;
			}

			if (mysql_set_character_set(&m_mysql, "gbk"))
			{
				return;
			}

			//
			char szBuf[g_nSqlLen] ={0};
			strcpy_s(szBuf, g_nSqlLen, T2A(strSql.GetBuffer()));
			if (0 != mysql_real_query(&m_mysql, szBuf, strlen(szBuf)))
			{
				// error
				m_strTactLog = _T("��ѯʧ�ܣ�\r\n�����Ӧ�����Ƿ����LOG��");
				UpdateData(FALSE);
				return;
			}
			else
			{
				MYSQL_RES *res_set;
				res_set = mysql_store_result(&m_mysql);
				if (res_set == NULL)
				{
					// failed
				}
				else
				{	// get log info
					MYSQL_ROW row;
					while ((row = mysql_fetch_row(res_set)) != NULL)
					{
						CString strTm;
						CString strInf;
						strTm.Format(_T("%s"), UtilString::ConvertMultiByteToWideChar(row[0]).c_str());
						strInf.Format(_T("%s"), UtilString::ConvertMultiByteToWideChar(row[1]).c_str());
						strShow += strTm + _T("  ") + strInf;
					}
					mysql_free_result(res_set);
				}
			}
			strSql.ReleaseBuffer();
			mysql_close(&m_mysql);
		}
		catch (...)
		{
		}


		// show in edit box
		if (_T("") == strShow)
		{
			strShow = _T("��ѯ���Ϊ�գ�");
		}
		m_strTactLog = strShow;
		UpdateData(FALSE);

		if(m_bLocateTime)
		{
			int nLocateLine = m_Edit_Log.GetLineCount()-1;
			CString strFind;
		
			m_tLocateTime.SetDateTime(m_dateLog.GetYear(), m_dateLog.GetMonth(), m_dateLog.GetDay(), m_tLocateTime.GetHour(), m_tLocateTime.GetMinute(), m_tLocateTime.GetSecond());
			strFind = m_tLocateTime.Format(_T("%Y-%m-%d %H:%M"));
			for(int xx=0;xx<m_Edit_Log.GetLineCount();xx++)
			{
				CString strTemp; 
				wchar_t sBuf[1024];
				int nLL = m_Edit_Log.LineLength(m_Edit_Log.LineIndex(xx));
				m_Edit_Log.GetLine(xx,sBuf,1024);
				strTemp.Format(_T("%s"), sBuf);

				if(strTemp.Find(strFind)>=0)
				{
					nLocateLine = xx;
					strTemp.ReleaseBuffer();
					break;
				}
				strTemp.ReleaseBuffer();
			}

			m_Edit_Log.LineScroll(nLocateLine, 0);
		}
		else
		{
			m_Edit_Log.LineScroll(m_Edit_Log.GetLineCount()-1, 0);
		}
	}//else for "if (DirectoryOperations::FileExists(cstrRunningPath, cstrDllLogName) == true)"
}


void CTacticLogQueryDlg::OnBnClickedBtnClearTillNow()
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox(L"�Ƿ�ɾ���������ڵ�����log��",MB_YESNO) == IDNO)
		return;
	CString cstrLogicLogDir;
	CString	strDirName;
	CString cstrDllLogName;

	UpdateData(TRUE);

	strDirName = m_dateLog.Format(_T("logic-%Y-%m-%d"));
	Project::Tools::GetSysPath(cstrLogicLogDir);
	cstrLogicLogDir = cstrLogicLogDir.Left(cstrLogicLogDir.ReverseFind(L'\\')) + L"\\core\\log\\" + strDirName + L"\\";
	cstrDllLogName = m_strDllName + L".dll.txt";

	if (DirectoryOperations::FileExists(cstrLogicLogDir, cstrDllLogName) == true)
	{
		CStdioFile fileControl;
		if(fileControl.Open(cstrLogicLogDir + cstrDllLogName, CFile:: modeCreate | CFile::modeWrite | CFile::shareDenyNone ) == TRUE)
		{
			fileControl.Close();
			m_strTactLog = _T("����ɹ���");
		}
		else
		{
			m_strTactLog = _T("���ʧ�ܣ����������ʹ�û���ģʽ��д��");
		}
		return;
	}
	
	
	
	// call mysql
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();

	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("����"), _T("��������յ�����ʱ�̵�LOG, �Ƿ�ȷ��?"), FALSE);
	if(WarnDlg.DoModal()!=IDOK)
		return;

	MYSQL	m_mysql;

	CString	strTableName;
	strTableName = m_dateLog.Format(_T("log_%Y_%m_%d"));

	CString	strIp = m_strDbIp;
	CString	strUsername = _T("root");
	CString	strPassword = _T("RNB.beop-2013");
	CString	strDbName = pFrame->GetMySQLDatabaseName();
	if(m_bLogDBExist)
	{
		strDbName = _T("domlog");
	}

	int		nPort = 3306;
	CString strSql;
	strSql.Format(_T("delete FROM %s WHERE loginfo LIKE '%%[%s]%%' OR loginfo LIKE '%%[%s]%%';"), strTableName, m_strDllName+_T(".dll"), m_strDllNameOrg+_T(".dll"));
	strSql.Format(_T("delete FROM %s WHERE loginfo LIKE '%%[%s]%%';"), strTableName, m_strDllName+_T(".dll"));

	try
	{
		if (NULL == mysql_init(&m_mysql))
		{
			// init error
			return;
		}

		USES_CONVERSION;
		if (NULL == mysql_real_connect(&m_mysql, T2A((LPTSTR)(LPCTSTR)strIp), T2A((LPTSTR)(LPCTSTR)strUsername), T2A((LPTSTR)(LPCTSTR)strPassword), T2A((LPTSTR)(LPCTSTR)strDbName), nPort, NULL, 0))
		{
			// connect error
			m_strTactLog = _T("�������ݿ�ʧ�ܣ�\r\n����������Ϣ��");
			UpdateData(FALSE);
			return;
		}

		if (mysql_set_character_set(&m_mysql, "gbk"))
		{
			return;
		}

		//
		char szBuf[g_nSqlLen] ={0};
		strcpy_s(szBuf, g_nSqlLen, T2A(strSql.GetBuffer()));
		if (0 != mysql_real_query(&m_mysql, szBuf, strlen(szBuf)))
		{
			// error
			m_strTactLog = _T("���ʧ�ܣ�\r\n�����Ӧ�����Ƿ����LOG��");
			return;
		}
		else
		{
			
		}
		strSql.ReleaseBuffer();
		mysql_close(&m_mysql);
	}
	catch (...)
	{
	}

}


void CTacticLogQueryDlg::OnBnClickedButtonKeywordSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_cstrKeyWordSearch.GetLength() == 0)
	{
		AfxMessageBox(L"�ؼ��ֲ�Ϊ��");
		return;
	}
	long nEditLineNum = m_Edit_Log.GetLineCount();
	CString cstrLine = L"";
	int rowlen = 0;
	for(int nCount=0; nCount<nEditLineNum; nCount++)
	{
		rowlen = m_Edit_Log.LineLength(m_Edit_Log.LineIndex(nCount));  
		m_Edit_Log.GetLine(nCount, cstrLine.GetBuffer(rowlen), rowlen);
		if(cstrLine.Find(m_cstrKeyWordSearch) >=0)
		{
			m_Edit_Log.LineScroll(nCount, 0);
			CString cstrLine;
			cstrLine.Format(L"%d",nCount);

			HTREEITEM hPA = m_treeKeywordLine.InsertItem(TVIF_TEXT,
				cstrLine, 0, 0, 0, 0, 0, TVI_ROOT, TVI_LAST);
			m_treeKeywordLine.SetItemData(hPA,nCount);
		}
		cstrLine.Empty();
	}
	UpdateData(FALSE);
}


void CTacticLogQueryDlg::OnTvnSelchangedTreeKeywordLine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	HTREEITEM hPa = m_treeKeywordLine.GetSelectedItem();
	unsigned long data = m_treeKeywordLine.GetItemData(hPa);
	m_Edit_Log.LineScroll(-m_Edit_Log.GetLineCount(), 0);
	m_Edit_Log.LineScroll(data, 0);
}
