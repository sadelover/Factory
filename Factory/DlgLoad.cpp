// DlgSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "Dlgload.h"
#include "afxdialogex.h"
#include "../ControlEx/WarningDlg.h"
#include "BEOP-Designer.h"
#include "MainFrm.h"
#include <sstream>

#include "../Tools/Util/UtilString.h"
#include "../ComponentDraw/sqlite/SqliteAcess.h"
#include "BEOPGatewayDlg.h"


// CDlgLoad 对话框


IMPLEMENT_DYNAMIC(CDlgLoad, CDialogEx)

CDlgLoad::CDlgLoad(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLoad::IDD, pParent)
	, m_strIP(_T("localhost"))
	, m_bAutoLogin(FALSE)
{

//	m_pRedundencyManager = new CRedundencyManager;
}

CDlgLoad::~CDlgLoad()
{


}

void CDlgLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_strIP);
	//DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	//DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	//DDX_Text(pDX, IDC_EDIT_REALDBNAME, m_strRealDBName);
//	DDX_Text(pDX, IDC_EDIT_DBNAME, m_strDBName);
//	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strDataSourceName);
//	DDX_Control(pDX, IDOK, m_BtnOK);
//	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	//DDX_Control(pDX, IDC_BUTTON_WRITE_TO_SERVER, m_BtnUpload);
	DDX_Control(pDX, IDC_PROGRESS_LOAD, m_ProgressCtrl);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_ProgessStatic);

//	DDX_Check(pDX, IDC_CHECK_SIMULATE, m_bSimulatee);
}


BEGIN_MESSAGE_MAP(CDlgLoad, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON_CHOOSEFILE, &CDlgLoad::OnBnClickedButtonChoosefile)
	ON_BN_CLICKED(IDOK, &CDlgLoad::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CDlgLoad::OnBnClickedCancel)
END_MESSAGE_MAP()


void CDlgLoad::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

}

void CDlgLoad::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
//	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	UpdateData(TRUE);

	if(m_strIP.GetLength()==0 )
	{
		CWarningDlg dlgWarn;
		dlgWarn.InitWarn(L"错误", L"参数内容不合法，请确认.", TRUE);
		dlgWarn.DoModal();
		return;
	}

	if(!IsIPValid())
	{
		CWarningDlg dlgWarn;
		dlgWarn.InitWarn(L"错误", L"数据库地址不合法，请确认.", TRUE);
		dlgWarn.DoModal();
		return;

	}

	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	//DataBaseSetting *pDBSetting = &(pFrame->m_dbset);
	//pDBSetting->strDBIP = Project::Tools::WideCharToAnsi(m_strIP.GetString());

	//pDBSetting->strDBName = 	Project::Tools::WideCharToAnsi(m_strDBName.GetString());
	//pDBSetting->strRealTimeDBName = 	Project::Tools::WideCharToAnsi(m_strRealDBName.GetString()); 
	//pDBSetting->strUserName = 	Project::Tools::WideCharToAnsi(m_strUserName.GetString());
	//pDBSetting->nPort = (m_nPort);
	//pDBSetting->DTUBaudrate = 19200;
	//pDBSetting->nAutoLogin = 0;
	//pDBSetting->DTUPortNumber = 9600;
//	pFrame->m_bUpdateInputTableFromSite = !m_bSimulatee;

	//pFrame->SaveConfig(*pDBSetting);

	//CString strS3DBVersion = pFrame->GetConfigVersion();
	//double fVersion = 0.f;
	//if(strS3DBVersion.GetLength()==0)
	//	fVersion = 2.0;
	//else
	//	fVersion = _wtof(strS3DBVersion.GetString());
	//if(fVersion <2.f)
	//{
	//	CWarningDlg dlgWarn;
	//	dlgWarn.InitWarn(L"错误", L"配置文件版本过低，请升级，程序将退出", FALSE);
	//	return;
	//}

	//m_ProgessStatic.SetWindowText(L"正在扫描检测网络,检测冗余...");

	//m_pRedundencyManager->Init(pFrame->m_nMainDropInterval, m_strIP, m_strIP2);

	//;//no need wait   判断绑定端是否运行了主机
	//if(!m_pRedundencyManager->CheckIfNeedActiveSlave())			//网络上已有运行主机  等待
	//{
	//	pFrame->m_nSystemType = 1;//双机热备
	//	
	//	m_ProgessStatic.SetWindowText(L"已检测到网络主机在运行，本机自动转为冗余辅机，热备待命中...");
	//	m_pRedundencyManager->WaitForActiveCommand();
	//	//	pDlg->SetWindowTextW(L"冗余热备激活，本机可以运行主机模式.");
	//}

	////m_strIP2不为空，启动备份数据库线程，若能网络正常，传输文件
	//if(m_strIP2.GetLength()>0)
	//{
	//	m_pRedundencyManager->StartBackupDatabaseThread();
	//	
	//}

	//保存路径参数
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	CBEOPGatewayDlg *pParentDlg = (CBEOPGatewayDlg *) GetParent();
	pParentDlg->m_strServerIP = m_strIP.GetString();
	pParentDlg->InitData();


//	CDialogEx::OnOK(); 
}

BOOL CDlgLoad::IsIPValid()
{
	if(m_strIP==L"localhost")
		return TRUE;

	if(m_strIP.FindOneOf(L"abcdefghijklmdopqrstuvwxyz,;'")>=0)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgLoad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon,TRUE); //设置为大图标


//	m_BtnOK.SetButtonProjectStyle(Blue);
//	m_BtnCancel.SetButtonProjectStyle(Silver);

	//CMainFrame *pFrame  = (CMainFrame *) AfxGetMainWnd();
	//CString cstrFile = pFrame->m_strDBFileName.c_str();
	//pFrame->m_dbset = pFrame->LoadConfig();

	//m_strIP = Project::Tools::AnsiToWideChar(pFrame->m_dbset.strDBIP.data()).c_str();

//	m_bSimulatee = !(pFrame->m_bUpdateInputTableFromSite);


	UpdateData(FALSE);

	//m_bUpdateInputTableFromSite = theApp.GetUpdateFromSite();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSEe
}

/*
void CDlgLoad::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	EndDialog(0);
}
*/


void CDlgLoad::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
//	CDialogEx::OnCancel();
}
