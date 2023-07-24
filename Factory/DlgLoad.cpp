// DlgSetting.cpp : ʵ���ļ�
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


// CDlgLoad �Ի���


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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

}

void CDlgLoad::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	UpdateData(TRUE);

	if(m_strIP.GetLength()==0 )
	{
		CWarningDlg dlgWarn;
		dlgWarn.InitWarn(L"����", L"�������ݲ��Ϸ�����ȷ��.", TRUE);
		dlgWarn.DoModal();
		return;
	}

	if(!IsIPValid())
	{
		CWarningDlg dlgWarn;
		dlgWarn.InitWarn(L"����", L"���ݿ��ַ���Ϸ�����ȷ��.", TRUE);
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
	//	dlgWarn.InitWarn(L"����", L"�����ļ��汾���ͣ��������������˳�", FALSE);
	//	return;
	//}

	//m_ProgessStatic.SetWindowText(L"����ɨ��������,�������...");

	//m_pRedundencyManager->Init(pFrame->m_nMainDropInterval, m_strIP, m_strIP2);

	//;//no need wait   �жϰ󶨶��Ƿ�����������
	//if(!m_pRedundencyManager->CheckIfNeedActiveSlave())			//������������������  �ȴ�
	//{
	//	pFrame->m_nSystemType = 1;//˫���ȱ�
	//	
	//	m_ProgessStatic.SetWindowText(L"�Ѽ�⵽�������������У������Զ�תΪ���ศ�����ȱ�������...");
	//	m_pRedundencyManager->WaitForActiveCommand();
	//	//	pDlg->SetWindowTextW(L"�����ȱ��������������������ģʽ.");
	//}

	////m_strIP2��Ϊ�գ������������ݿ��̣߳��������������������ļ�
	//if(m_strIP2.GetLength()>0)
	//{
	//	m_pRedundencyManager->StartBackupDatabaseThread();
	//	
	//}

	//����·������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon,TRUE); //����Ϊ��ͼ��


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
	// �쳣: OCX ����ҳӦ���� FALSEe
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CDialogEx::OnCancel();
}
