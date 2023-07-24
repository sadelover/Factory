// PaneServerManager.cpp : 实现文件
//

#include "stdafx.h"
#include "PaneServerManager.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include <sstream>

#include "../Tools/Util/UtilString.h"
#include "../ComponentDraw/sqlite/SqliteAcess.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "../ControlEx/WarningDlg.h"
#include "InputRedundencyIPDlg.h"
#include "BEOPGatewayDlg.h"
#include "BEOPGatewayDlg.h"

// CPaneServerManager 对话框

IMPLEMENT_DYNAMIC(CPaneServerManager, CDialogEx)

CPaneServerManager::CPaneServerManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPaneServerManager::IDD, pParent)
	, m_strLOG(_T(""))
	, m_bAutoScroll(FALSE)
	, m_strRedundencyIP(_T(""))
	, m_strDownloadFile(_T(""))
{

	m_pFileTransferRecv = new CDebugToolFileTransfer(3458);
	m_pFileTransfer = new CDebugToolFileTransfer(3459);

	m_strLastDownloadResponse = L"";
	m_nNoResponseTimeCount = 0;

	m_pGatewayDlg = NULL;

	GetLocalTime(&m_tLogRefreshTo);
}

CPaneServerManager::~CPaneServerManager()
{
}

void CPaneServerManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_ProgressStatic);
	DDX_Control(pDX, IDC_STATIC_COREMODE, m_StaticMode);
	//DDX_Control(pDX, IDC_EDIT_DBNAME, m_EditDBName);
	DDX_Control(pDX, IDC_EDIT_SERVERIP, m_EditServer);
	DDX_Control(pDX, IDC_CHECK_ALWAYSLOG, m_CheckAlwaysLog);
	DDX_Control(pDX, IDC_EDIT_LOG, m_EditLog);
	DDX_Text(pDX, IDC_EDIT_LOG, m_strLOG);
	DDX_Check(pDX, IDC_CHECK_AUTOSCROLL, m_bAutoScroll);
	DDX_Text(pDX, IDC_EDIT_REDUNDENCY_IP, m_strRedundencyIP);
	DDX_Control(pDX, IDC_EDIT_FILTER, m_EditFilter);
	DDX_Control(pDX, IDC_CHECK_FILTER, m_CheckFilter);
	DDX_Text(pDX, IDC_EDIT_DBNAME, m_strDownloadFile);
}


BEGIN_MESSAGE_MAP(CPaneServerManager, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_CORE_MODE, &CPaneServerManager::OnBnClickedButtonChangeCoreMode)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_TO_SERVER, &CPaneServerManager::OnBnClickedButtonWriteToServer)
	ON_BN_CLICKED(IDC_BUTTON_GETDB_FROM_SERVER, &CPaneServerManager::OnBnClickedButtonGetdbFromServer)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_ALWAYSLOG, &CPaneServerManager::OnBnClickedCheckAlwayslog)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_REDUNDENCY, &CPaneServerManager::OnBnClickedButtonModifyRedundency)
	ON_BN_CLICKED(IDC_BUTTON_RESETCORE, &CPaneServerManager::OnBnClickedButtonResetcore)
	ON_BN_CLICKED(IDC_CHECK_FILTER, &CPaneServerManager::OnBnClickedCheckFilter)
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE, &CPaneServerManager::OnBnClickedButtonLoadfile)
END_MESSAGE_MAP()


// CPaneServerManager 消息处理程序


void CPaneServerManager::OnBnClickedButtonChangeCoreMode()
{
	// TODO: 在此添加控件通知处理程序代码
	wstring strMode = m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"sitemode");
	if(strMode==L"1")
	{
		CWarningDlg dlgWarn;
		dlgWarn.InitWarn(L"提示", L"是否确认切换至仿真模式?", FALSE);
		if(dlgWarn.DoModal()==IDOK)
		{
			if(m_pDataAccess->WriteCoreDebugItemValue(L"sitemode", L"0"))
			{
				m_StaticMode.SetWindowTextW(L"当前模式: 仿真");
			}
		}

	}
	else
	{
		CWarningDlg dlgWarn;
		dlgWarn.InitWarn(L"提示", L"是否确认切换至现场模式?", FALSE);
		if(dlgWarn.DoModal()==IDOK)
		{
			if(m_pDataAccess->WriteCoreDebugItemValue(L"sitemode", L"1"))
			{
				m_StaticMode.SetWindowTextW(L"当前模式: 现场");
			}
		}
	}
	
}

bool CPaneServerManager::InitData()
{
	if(m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"sitemode")==L"1")
		m_StaticMode.SetWindowTextW(L"当前模式: 现场");
	else
		m_StaticMode.SetWindowTextW(L"当前模式: 仿真");

	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	//m_EditDBName.SetWindowTextW(pFrame->m_strDBFileName.c_str());
	//wstring wstrIP = Project::Tools::AnsiToWideChar(pFrame->m_dbset.strDBIP.data());
	m_EditServer.SetWindowTextW(m_pGatewayDlg->m_strServerIP.c_str());

	m_pFileTransferRecv->StartAsReceiver();
	m_pFileTransferRecv->m_bRecvS3DB = false;

	wstring strTime = m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"servertime");
	if(strTime.length()>=10)
	{
		SYSTEMTIME stServerTime;
		Project::Tools::String_To_SysTime(strTime, stServerTime);
		stServerTime.wHour = 0;
		stServerTime.wMinute = 0;
		stServerTime.wSecond = 0;
		m_pDataAccess->ReadLog(m_pairLogInfo,stServerTime);
		if(m_pairLogInfo.size()>0)
		{
			CString strTimeRefreshTo = m_pairLogInfo.back().first.c_str();
			Project::Tools::String_To_SysTime(Project::Tools::WideCharToAnsi(strTimeRefreshTo.GetString()), m_tLogRefreshTo);
		}

	}
	else
		GetLocalTime(&m_tLogRefreshTo); 

	wstring strValue = m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"alwayslog");
	if(strValue==L"1")
	{
		m_CheckAlwaysLog.SetCheck(1);

		if(strTime.length()>0)
		{
			SetTimer(5,1000,NULL);
		}
	}

	wstring strRedundencyIP = m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"redundencyip");
	m_strRedundencyIP = strRedundencyIP.c_str();
	UpdateData(FALSE);

	return true;
}

bool CPaneServerManager::Exit()
{

	m_pFileTransferRecv->Exit();
	m_pFileTransfer->Exit();

	delete(m_pFileTransferRecv);
	delete(m_pFileTransfer);
	m_pFileTransfer = NULL;
	m_pFileTransferRecv = NULL;

	return true;
}

void CPaneServerManager::OnBnClickedButtonWriteToServer()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_strDownloadFile.GetLength()<=0)
	{
		CWarningDlg WarnDlgTemp;
		WarnDlgTemp.InitWarn(_T("错误"), L"请先预载待下装的文件!", FALSE);
	}
	CWarningDlg WarnDlg;
	CString strTemp;
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	wstring wstrIP = m_pGatewayDlg->m_strServerIP;
	strTemp.Format(L"是否确认将数据源的文件下装至%s?", wstrIP.c_str());
	WarnDlg.InitWarn(_T("提示"), strTemp, FALSE);
	if(WarnDlg.DoModal()==IDOK)
	{

		m_pFileTransfer->m_strPath = m_strDownloadFile;
		m_pFileTransfer->StartAsSender(wstrIP.c_str(), 3456);
		m_nNoResponseTimeCount = 0;
		m_strLastDownloadResponse = L"";
		m_ProgressStatic.ShowWindow(TRUE);

		GetDlgItem(IDC_BUTTON_WRITE_TO_SERVER)->EnableWindow(FALSE);

		CBEOPGatewayDlg *pParentDlg = (CBEOPGatewayDlg *) GetParent();
		pParentDlg->GetDlgItem(IDC_BUTTON_ARBITER)->EnableWindow(FALSE);
		pParentDlg->GetDlgItem(IDC_BUTTON_Point)->EnableWindow(FALSE);
		pParentDlg->GetDlgItem(IDC_BUTTON_CURVE)->EnableWindow(FALSE);
		pParentDlg->GetDlgItem(IDC_BUTTON_CURVE2)->EnableWindow(FALSE);
		pParentDlg->GetDlgItem(IDC_BUTTON_CURVE3)->EnableWindow(FALSE);
		pParentDlg->GetDlgItem(IDC_BUTTON_CURVE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESETCORE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CHANGE_CORE_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MODIFY_REDUNDENCY)->EnableWindow(FALSE);

		SetTimer(1,2000,NULL);

		m_pDataAccess->WriteCoreDebugItemValue(L"filerecved", L"");
	}
}


void CPaneServerManager::OnBnClickedButtonGetdbFromServer()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CPaneServerManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPaneServerManager::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==1)
	{
		if(m_pDataAccess==NULL)
			return;
		wstring strValue = m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"filerecved");

		if(strValue==L"finish")
		{
			KillTimer(1);
			m_ProgressStatic.ShowWindow(FALSE);
			m_ProgressStatic.SetWindowTextW(L"");
			AfxMessageBox(L"文件下装成功!");
			GetDlgItem(IDC_BUTTON_WRITE_TO_SERVER)->EnableWindow(TRUE);


			CBEOPGatewayDlg *pParentDlg = (CBEOPGatewayDlg *) GetParent();
			pParentDlg->GetDlgItem(IDC_BUTTON_ARBITER)->EnableWindow(TRUE);
			pParentDlg->GetDlgItem(IDC_BUTTON_Point)->EnableWindow(TRUE);
			pParentDlg->GetDlgItem(IDC_BUTTON_CURVE)->EnableWindow(TRUE);
			pParentDlg->GetDlgItem(IDC_BUTTON_CURVE2)->EnableWindow(TRUE);
			pParentDlg->GetDlgItem(IDC_BUTTON_CURVE3)->EnableWindow(TRUE);
			pParentDlg->GetDlgItem(IDC_BUTTON_CURVE4)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RESETCORE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CHANGE_CORE_MODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MODIFY_REDUNDENCY)->EnableWindow(TRUE);



			return;
		}
		else
		{
			
			if(strValue.length()>0 && strValue!=m_strLastDownloadResponse)
			{
				CString strTemp;
				strTemp.Format(L"已下载%.2f MB", _wtof(strValue.c_str())/1000.f/1000.f), 
					m_ProgressStatic.SetWindowTextW(strTemp);
			}
			else
			{
				m_nNoResponseTimeCount++;
				if(m_nNoResponseTimeCount>=60 || !m_pFileTransfer->m_bIsTransmitting)//超过一分钟没有响应
				{
					KillTimer(1);
					m_ProgressStatic.ShowWindow(FALSE);
					m_ProgressStatic.SetWindowTextW(L"");
					AfxMessageBox(L"超时无响应，文件下装失败!");
					m_nNoResponseTimeCount = 0;
					m_strLastDownloadResponse = L"";
					GetDlgItem(IDC_BUTTON_WRITE_TO_SERVER)->EnableWindow(TRUE);


					CBEOPGatewayDlg *pParentDlg = (CBEOPGatewayDlg *) GetParent();
					pParentDlg->GetDlgItem(IDC_BUTTON_ARBITER)->EnableWindow(TRUE);
					pParentDlg->GetDlgItem(IDC_BUTTON_Point)->EnableWindow(TRUE);
					pParentDlg->GetDlgItem(IDC_BUTTON_CURVE)->EnableWindow(TRUE);
					pParentDlg->GetDlgItem(IDC_BUTTON_CURVE2)->EnableWindow(TRUE);
					pParentDlg->GetDlgItem(IDC_BUTTON_CURVE3)->EnableWindow(TRUE);
					pParentDlg->GetDlgItem(IDC_BUTTON_CURVE4)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_RESETCORE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_CHANGE_CORE_MODE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_MODIFY_REDUNDENCY)->EnableWindow(TRUE);


					return;
				}
			}

		}
	}
	else if(nIDEvent==2)
	{
		if(m_pFileTransferRecv->m_bNewFileRecved)
		{
			KillTimer(2);
			m_ProgressStatic.ShowWindow(FALSE);

			AfxMessageBox(L"LOG文件下载成功!");
			GetDlgItem(IDC_BUTTON_WRITE_TO_SERVER)->EnableWindow(TRUE);
			GetDlgItem(IDOK)->EnableWindow(TRUE);

			m_pFileTransferRecv->m_bNewFileRecved = false;


			return;
		}
	}
	else if(nIDEvent==5)
	{ //刷LOG
		
		vector<pair<wstring, wstring>> pLogList;
		SYSTEMTIME stStart = m_tLogRefreshTo;
		m_pDataAccess->ReadLog(pLogList, stStart);

		if(pLogList.size()>0)
		{
			for(int i=0;i<pLogList.size();i++)
			{
				CString strToFilter;
				GetDlgItem(IDC_EDIT_FILTER)->GetWindowText(strToFilter);
				if(m_CheckFilter.GetCheck()==1 && strToFilter.GetLength()>0)
				{
					CString strLog = pLogList[i].second.c_str();
					if(strLog.Find(strToFilter)>=0)
					{
						m_pairLogInfo.push_back(pLogList[i]);
						m_strLOG+= pLogList[i].first.c_str();
						m_strLOG+= L"  ";
						m_strLOG+= pLogList[i].second.c_str();
					}
				}
				else
				{
					m_pairLogInfo.push_back(pLogList[i]);
					m_strLOG+= pLogList[i].first.c_str();
					m_strLOG+= L"  ";
					m_strLOG+= pLogList[i].second.c_str();

				}
			}

			m_EditLog.SetWindowTextW(m_strLOG);
			UpdateData(TRUE);
			if(m_bAutoScroll)
				m_EditLog.LineScroll(m_EditLog.GetLineCount()-1, 0);

			CString strTimeRefreshTo = m_pairLogInfo.back().first.c_str();
			Project::Tools::String_To_SysTime(Project::Tools::WideCharToAnsi(strTimeRefreshTo.GetString()), m_tLogRefreshTo);
		}
	}


	CDialogEx::OnTimer(nIDEvent);
}


CString CPaneServerManager::GetLocalIP() 
{
	WSADATA wsaData;
	char name[155];
	CString strIP;

	PHOSTENT hostinfo; 
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 ) 
	{  
		if( gethostname ( name, sizeof(name)) == 0) 
		{ 
			if((hostinfo = gethostbyname(name)) != NULL) 
			{ //这些就是获得IP的函数
				strIP = CString(inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list)); 

			} 
		} 
		WSACleanup( );
	} 

	return strIP;
} 



void CPaneServerManager::OnBnClickedCheckAlwayslog()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_CheckAlwaysLog.GetCheck())
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"alwayslog", L"1");
		wstring strTime = m_pDataAccess->ReadOrCreateCoreDebugItemValue(L"servertime");
		if(strTime.length()>0)
		{
			SetTimer(5,1000,NULL);
		}
		else
		{
			AfxMessageBox(L"服务器时间格式不正确，启动LOG失败");
			m_CheckAlwaysLog.SetCheck(0);
		}
	}
	else
	{
		m_pDataAccess->WriteCoreDebugItemValue(L"alwayslog", L"0");
		KillTimer(5);
	}
}


void CPaneServerManager::OnBnClickedButtonModifyRedundency()
{
	// TODO: 在此添加控件通知处理程序代码

	CInputRedundencyIPDlg dlg;
	if(dlg.DoModal()==IDOK)
	{

		if(dlg.m_strIP==m_strRedundencyIP)
			return;
		m_strRedundencyIP = dlg.m_strIP;
		m_pDataAccess->WriteCoreDebugItemValue(L"redundencyip", m_strRedundencyIP.GetString());
		UpdateData(FALSE);
		AfxMessageBox(L"修改成功,生效需将core手动重启.");

	}

	
}


void CPaneServerManager::OnBnClickedButtonResetcore()
{
	// TODO: 在此添加控件通知处理程序代码
	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("提示"), _T("将重启core, 是否确定?"), FALSE);
	if(WarnDlg.DoModal()==IDOK)
	{
		if(m_pDataAccess)
		{
			m_pDataAccess->WriteCoreDebugItemValue(L"reset", L"1");
			AfxMessageBox(L"重启命令发送成功，需要一段时间，请勿连续重启.");

		}
	}

}


void CPaneServerManager::OnBnClickedCheckFilter()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_CheckFilter.GetCheck())
	{
		m_EditFilter.EnableWindow(TRUE);
	}
	else
		m_EditFilter.EnableWindow(FALSE);
}


void CPaneServerManager::OnBnClickedButtonLoadfile()
{
	// TODO: 在此添加控件通知处理程序代码

	CFileDialog dlgOpenFile(TRUE,NULL,L"*.s3db",OFN_HIDEREADONLY|OFN_HIDEREADONLY,L"s3db Files (*.s3db)|*.s3db|");
	if(dlgOpenFile.DoModal()==IDOK)
	{
		CString dbName = dlgOpenFile.GetPathName();
		m_strDownloadFile = dbName;

		UpdateData(FALSE);
	}
}
