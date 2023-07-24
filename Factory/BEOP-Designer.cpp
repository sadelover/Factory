// BEOP-Designer.cpp : Defines the class behaviors for the application.
#include "stdafx.h"
#include "BEOP-Designer.h"
#include "MainFrm.h"
#include "BEOP-DesignerDoc.h"
#include "BEOP-DesignerView.h"
#include <afxwin.h>
#include "../ComponentDraw/ProjectSqlite.h"
#include "afxadv.h "
#include "Tools/CustomTools/CustomTools.h"

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define str_server_log_file   _T("desinger.log")
//////////////////////////////////////////////////////////////////////////
// CBEOPDesignerApp
BEGIN_MESSAGE_MAP(CBEOPDesignerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CBEOPDesignerApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND_RANGE(ID_FILE_MRU_FILE1,ID_FILE_MRU_FILE4,&CBEOPDesignerApp::OnFileMruFile1)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
CBEOPDesignerApp::CBEOPDesignerApp()
{
	m_bHiColorIcons = TRUE;
	m_pRecFileList = NULL;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_wstr_app_name = _T("Factory");
}
// The one and only CBEOPDesignerApp object
CBEOPDesignerApp theApp;
// CBEOPDesignerApp initialization
BOOL CBEOPDesignerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinAppEx::InitInstance();
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	//���ע���
	if (SHDeleteKey(HKEY_CURRENT_USER,_T("SOFTWARE\\Local AppWizard-Generated Applications\\BEOP-Designer\\Workspace"))!=ERROR_SUCCESS)
	{
		//AfxMessageBox(_T("���ע���ʧ�ܣ�"));
	}
	
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	m_pRecFileList = m_pRecentFileList;

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBEOPDesignerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CBEOPDesignerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	const wstring wstrFilePathName = cmdInfo.m_strFileName;
	m_nCmdShow = SW_SHOWMAXIMIZED; 
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

    m_pMainWnd-> DragAcceptFiles(); 
	AssociateFile();
	if(wstrFilePathName.length() )
	{
		( (CMainFrame *)AfxGetApp()->GetMainWnd() )->LoadProjectFile(wstrFilePathName );
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
    DECLARE_MESSAGE_MAP()
public:
	CAboutDlg();
// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
public:
	CString m_strAppNameDisplay;
private:
	CString m_cstrUpdateLog;
public:
	afx_msg void OnBnClickedOk();
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
	, m_strAppNameDisplay(_T(""))
	, m_cstrUpdateLog(_T(""))
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_VERSION, mStrVersion);

	DDX_Text(pDX, IDC_EDIT1, m_cstrUpdateLog);
}
BOOL CAboutDlg::OnInitDialog()
{
    __super::OnInitDialog(); 
	m_cstrUpdateLog+=L"[2.5.77 2020-12-31]���������������ı�������\r\n";
	m_cstrUpdateLog+=L"[2.5.76 2020-12-28]������MoxaTCPServer�����͡�\r\n";
	m_cstrUpdateLog+=L"[2.5.75 2020-12-25]��˫���̺߳���ת���̲߳���ʱ��Ϊѡ���߳�ѡ��һ����ɫ��Ϊͻ��ɫ�ʡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.74 2020-12-24]��������ָ���ѯ������ѡ������־��ת���߳̽��湦�ܡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.73 2020-12-24]����ͼԴ��ʧ�����顱�����޸������ӵ������ܡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.72 2020-12-23]�������ߡ��˵����ӡ�ͼԴ��ʧ�����顱���ܡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.71 2020-12-21]������Ŀ���á��������ӡ���ʾ������\r\nͼԪָ���ͼƬ��Դ������ʱ��ʾ404ͼƬ��\r\n";
	m_cstrUpdateLog+=L"[2.5.70 2020-12-16]������Ŀ���á�����İ档\r\n";
	m_cstrUpdateLog+=L"[2.5.69 2020-12-15]�������Խ������������ܸİ档\r\n";
	m_cstrUpdateLog+=L"[2.5.68 2020-12-14]����������ӽ�ѡ��ʱ�������־�Ĳ��ԡ����������������������˫���򿪲���һ�������ݡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.67 2020-12-13]�������������ڡ����ε㡱����ѡ���ֱ��׷�ӹ��ܡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.66 2020-12-10]�������ص�����������������ת���߳̽��湦�ܡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.65 2020-12-09]������ģ���������Թ���,�˹��ܿ���ֱ����ת�����������߳��С�\r\n";
	m_cstrUpdateLog+=L"[2.5.64 2020-12-08]������API��\r\n";
	m_cstrUpdateLog+=L"[2.5.63 2020-12-07]��ֱ�߻����Ż������Ӷ���ε��϶����ܡ��޸�ͼԪɾ�����޷��ָ������⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.62 2020-12-07]�����ƾ��Σ�����Բ�Ǻ����ɫ���ԡ��޸���ѡ����ɾ��ͼԪ�����⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.61 2020-12-04]����������ʷ���ݲ��㴰�ڡ�����ʾ�����������⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.60 2020-12-02]�����ӣ�ͼԪ���ƹ����п����Ҽ�ȡ�����ơ�\r\n";
	m_cstrUpdateLog+=L"[2.5.59 2020-12-02]����������ӽ�ѡ��ʱ�������־�Ĳ��ԡ���������1���ӵ�log��־����ʾ��\r\n";
	m_cstrUpdateLog+=L"[2.5.58 2020-12-01]�������ԡ��������Ҽ�ѡ�������ӽ�ѡ��ʱ�������־�Ĳ��ԡ��Ĺ��ܡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.57 2020-12-01]���ܵ����������кͳ��ȽǶȸı�ʱЧ���ĸ��ơ�\r\n";
	m_cstrUpdateLog+=L"[2.5.56 2020-11-30]��Ϊ���ܵ������Ӵ���ʱ�͵���ʱ��б�ߺ�shift��������ʱֻ�ܻ�ֱ�ߣ����޸�ѡ�д�������⡣\r\n������Ρ�Ĭ���߿��Ϊ2,���޸��㴴������ʱ���λ������ʾλ�ò��������⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.55 2020-11-30]���޸��滭��������������ѡ��ͼԪ������Ρ������⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.54 2020-11-30]����������á����ӡ�������ʷ���ݡ��ɸ���ѡ��ĵ�����������㡣\r\n�����Թ����в��Ը��Ʋ��ٻ����dll������\r\n";
	m_cstrUpdateLog+=L"[2.5.53 2020-11-26]����ͨ��������á����ӡ�ͨ�ñ����ͼ�������\r\n";
	m_cstrUpdateLog+=L"[2.5.52 2020-11-23]�����Ӷ���λ��ƹ��ܡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.51 2020-11-18]������ֱ����shift����ʱֻ�ܻ�ֱ�ߵĹ��ܡ�\r\n����ͨѶ������в��ֱ���ɹ������´򿪲�����ȷ��ʾ��\r\n";
	m_cstrUpdateLog+=L"[2.5.50 2020-11-17]��������ͼԪɾ����رչ����Ҳ����棬�ٴδ򿪹���ͼԪ��ʧ�������⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.49 2020-11-13]�������Զ���������л���ͼʱ�ظ���ӵ����⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.48 2020-11-12]�����ӻ���ֱ�߹��ܡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.47 2020-11-11]���Զ�����������Ż���\r\n";
	m_cstrUpdateLog+=L"[2.5.46 2020-11-10]���������Զ�����������ݲ�ѯ������������\r\n";
	m_cstrUpdateLog+=L"[2.5.45 2020-11-08]����APIѡ�񡱴��������Զ���ȫ���ܡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.44 2020-11-08]���Զ���������ӡ�������״ͼ�������\r\n";
	m_cstrUpdateLog+=L"[2.5.43 2020-11-04]��1.�Զ���������Ӵ��ڲ������ٲ�ѯ�ؼ��ع�Ϊ��״ͼ��\r\n2.�����������ڵĵ�����ɸѡ���������ڱ���ʱ��ֵɸѡ�б�\r\n";
	m_cstrUpdateLog+=L"[2.5.40 2020-11-01]���ܵ�����б����ƵĹ��ܡ���׼�豸�������ӽ�ֹ��ʾ�Ĺ��ܡ�\r\n";
	
	m_cstrUpdateLog+=L"[2.4.98 2020-07-03]�����ӡ����ص��������Ѳ��������ʾ�����ӡ�ϵͳ��̨�������á��������ʱ����ʾdompysiteû��������ʾ��\r\n\r\n";
	m_cstrUpdateLog+=L"[2.4.99 2020-07-07]����������־��ѯ������ ���ӣ��ؼ��ּ������ܣ�ֱ���г����йؼ��������г����У�˫��������\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.00 2020-07-09]���������̹߳����½��͸����ĵĵ����������Ӣ�����ֺ��»��ߵ��жϡ�\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.01 2020-07-09]��������ָ���¼��ѯ�����Ӷ��°�domlogic��log�洢��ʽ֧�֣�ͬʱ����ԭ�������ݿ���ʽ��һͬ��ʾ�ڱ༭���С�\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.02 2020-07-16]����������á������С���������ť�Ż������ٶȡ�\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.03 2020-07-17]����������á������С���������ť�Ż������ٶȡ�\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.04 2020-07-18]�����ܵ����ԡ����������Ӳ��㰴ť������󶨵�������С��\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.05 2020-07-22]����ͼԪѡ�񴰿ڡ����ڵ�����ҳ��ť���м�����λ��λ�á�\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.06 2020-07-22]����������־��ѯ�����ڴ�С������\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.07 2020-07-27]���������̹߳������ڡ���ѡ�и����½�����ť��ȡ�������Ƹ�Ϊȡ�Զ������ơ�\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.08 2020-08-06]�������ݡ��˵����ӡ���ȷʱ�䵼����ʷ���ݡ�ѡ�\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.09 2020-08-07]�������ԡ��˵�������������ֹͣ����ȫ������log��core\log\CRITICAL.txt��;��������־��ѯ����������\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.10 2020-08-12]�� ����������ҳ����϶�ҳ��������λ�á��Ķ����޷������϶���λ�����⡣\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.11 2020-08-14]�� �����ڱ���ҳ��ʱ����ʧ��û����ʾҲ�޷�������������⡣\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.12 2020-08-18]�� ������ָ���¼��ѯ�����ڵġ���ѯ����ť���Ʋ�ѯ������ռ�¼��\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.13 2020-08-21]�� ����׼�豸���á����ڵĲ����趨����������ʾԭ������\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.14 2020-08-24]�� ������ָ���¼��ѯ�����ڵ�����ָ��������·���ʾ��\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.15 2020-08-27]�� ����׼�豸���á���������֧�����ġ�\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.16 2020-08-27]�� ���豸ϵͳ�������á��������Ӽ��׻����ý��档\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.17 2020-09-01]�� �����������ʵʱֵ���͡����ݵ�ǰ�����ʵʱֵ���������Ӷ�domcore�������е��жϡ�\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.18 2020-09-01]�� ���豸ϵͳ�������á����������ű�������ַ����������ESC�������½�����ʧ�����⡣\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.19 2020-09-02]�� ���豸ϵͳ�������á��޸�Ctrl+A��ݼ�ʧЧ�����⡣\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.20 2020-09-02]�� ��������á����ڡ�ɾ���㡱��ť���Ƹ���Ϊ����Ҫ���±�����Ż���Ч��\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.21 2020-09-02]�� ��������á��������ӵ�����ɸѡ��\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.22 2020-09-03]�� �����Լ�¼��ѯ�����ڽ����ѯ�ٶ��������⡣\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.23 2020-09-08]�� �����ݡ��˵��������������ܡ����ݿ�ָ����Լ������ݿⱸ�ݹ�����\r\n\
		��ϵͳ�豸�������á��������ӡ������䶳���顱�������ű��������ȫ�����޷��л������ü�������⡣\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.24 2020-09-09]�� �������̹߳������ڵġ��������ýű����Ӵ��ڱ��������Ҫ����Factory��\r\n\
		�����ݡ��˵��С����ݿ�ָ������ܣ������ӳ������˵����⡣\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.25 2020-09-12]�� ���༭�㡱����vpointģʽ�����ӵ��㲹�㹦�ܰ�ť����ʷ���ݲ��㡱��\r\n";
	m_cstrUpdateLog+=L"[2.5.26 2020-09-12]�� ����ά���˵��еġ����������ʵʱֵ�����ܽ�����ڳ���8880�����ݺ��ʧ�ܵ����⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.27 2020-09-13]�� �༭�����������\r\n";
	m_cstrUpdateLog+=L"[2.5.28 2020-09-14]�����༭�㡱���ڡ���ʷ���ݲ��㡱��ť�޸��������\r\n";
	m_cstrUpdateLog+=L"[2.5.29 2020-09-14]�����༭�㡱���ڡ���ʷ���ݲ��㡱��ť�������Ӳ���ȴ�������ʾ�����ٻ����ȡ�\r\n\
		�����ص������������޸��޷��������ű��еĵ�����⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.29 2020-09-14]�����༭�㡱���ڡ���ʷ���ݲ��㡱��ť�������Ӽ���ȴ�������ʾ�����ٻ����ȵ���Factory������\r\n";
	m_cstrUpdateLog+=L"[2.5.30 2020-09-16]�����༭�㡱���ڡ���ʷ���ݲ��㡱��ť�������������\r\n";
	m_cstrUpdateLog+=L"[2.5.31 2020-09-16]�������ÿ��Ʋ��Բ������������������������ǰֵ���У�����ʵʱ�۲쵱ǰֵ��\r\n";
	m_cstrUpdateLog+=L"[2.5.32 2020-09-17]��������������־��ѯ����ѯ���ڵ���������\r\n";
	m_cstrUpdateLog+=L"[2.5.33 2020-09-17]�������Ҽ������ԡ����еġ����ӵ��ԡ�����������˫���Զ���ʾ�����Ӳ����������⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.34 2020-09-18]���޸���ͨѶ��λѡ�񡱴�����ӵ������޷��ڵ�����鵽�������ò��������ڵ�����������ʱ����ʾ�޷��鵽�˵�����⡣\r\n";
	m_cstrUpdateLog+=L"[2.5.35 2020-09-21]���޸�ͨ�����ֽ�����Է�ʽ�������ݵĵ��¼������з�ʽ�������ģʽ���ٽ����߳�ʱ��Ȼ��ʾ��Ҫ���ӡ�\r\n";
	m_cstrUpdateLog+=L"[2.5.36 2020-10-11]���޸������ÿ��Ʋ��Բ����������߳���Ͽ���ʾ�����µı������\r\n";
	m_cstrUpdateLog+=L"[2.5.37 2020-10-12]�����ӡ����ü����������ʵʱ���㡱ѡ��\r\n";
	m_cstrUpdateLog+=L"[2.5.38 2020-10-13]���޸����豸ϵͳ�������á����ڡ������䶳���顱û�гɹ����ء�\r\n";
	m_cstrUpdateLog+=L"[2.5.39 2020-10-22]���޸�������ҳ�桱���ڵ���ҳ���ǹܵ�����ȱʧ�͵���ҳ��ı���ͼ��ʧ�����⡣\r\n";
	UpdateData(FALSE);
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// App command to run the dialog
void CBEOPDesignerApp::OnAppAbout()
{
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	CAboutDlg aboutDlg;
	//aboutDlg.m_strAppNameDisplay = pFrame->GetAppName();
	aboutDlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////
const wstring c_wstr_default_file_sufix = _T("xup3");
void CBEOPDesignerApp::AssociateFile()
{
	//c_wstr_default_file_sufix;
	HKEY   hk; 
	ULONG   disposition; 
	//char   szName[256]; 
	CString cstrName;
	cstrName.Format(_T(".%s\\"), c_wstr_default_file_sufix.c_str() );
	
	//���жϸü�ֵ�Ƿ����.
	long lresult = RegCreateKeyEx(HKEY_CLASSES_ROOT, cstrName, 0,NULL,REG_OPTION_NON_VOLATILE,	
		KEY_ALL_ACCESS ,	NULL,   &hk,   &disposition);

	if(lresult != ERROR_SUCCESS) {     
		return; 
	}
	
	CString cstrValue;
	cstrValue.Format(_T("%s_file"), c_wstr_default_file_sufix.c_str() );
	if(RegSetValue(hk,NULL,REG_SZ, cstrValue, 9)!=ERROR_SUCCESS) { 
		return; 
	} 
	//д���Ӽ�xxxfile��Ĭ�ϴ�·�� 
	CString cstrDefaultCmd;
	cstrDefaultCmd.Format(_T("%s\\shell\\open\\command"), cstrValue);
	//strcpy(szName, "xxxfile\\shell\\open\\command "); 
	if(RegCreateKeyEx(HKEY_CLASSES_ROOT,cstrDefaultCmd, 0, NULL,REG_OPTION_NON_VOLATILE, 
		KEY_ALL_ACCESS,   NULL,   &hk,   &disposition)!=ERROR_SUCCESS) 
	{ 
		return; 
	} 
	TCHAR   szBuf[MAX_PATH]; 
	GetModuleFileName(AfxGetInstanceHandle(),szBuf, MAX_PATH); 
	CString cstrOpenCmdValue;
	cstrOpenCmdValue += "\"";
	cstrOpenCmdValue += szBuf;
	cstrOpenCmdValue += "\"";
	cstrOpenCmdValue += _T(" \"%1\"");
	if(RegSetValue(hk,NULL,REG_SZ,cstrOpenCmdValue,9)   !=   ERROR_SUCCESS) { 
		return; 
	}	
}

void CBEOPDesignerApp::OnFileMruFile1(UINT nID)
{
	int nMRUIndex = 0;
			switch (nID) {
			   case ID_FILE_MRU_FILE1:
				   nMRUIndex = 0;
				   break;
			   case ID_FILE_MRU_FILE2:
				   nMRUIndex = 1;
				   break;
			   case ID_FILE_MRU_FILE3:
				   nMRUIndex = 2;
				   break;
			   case ID_FILE_MRU_FILE4:
				   nMRUIndex = 3;
				   break;
			}	

			sFilePathName = m_pRecentFileList->m_arrNames[nMRUIndex];
			wstring  sre_filename = sFilePathName.AllocSysString();
			if(sre_filename.length())
			{
				CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
				ASSERT(pMainFrm != NULL);
				if (pMainFrm != NULL)
				{
					CString strPathName(sre_filename.c_str());
					if(CheckFileName(strPathName) == true)
						pMainFrm->OpenProjectContain(strPathName);
				}
				//bool ifclose = ( (CMainFrame *)AfxGetApp()->GetMainWnd() )->CloseProjectBeforeOpen();
				//if(ifclose)
				//((CMainFrame *)AfxGetApp()->GetMainWnd())->LoadProjectFile(sre_filename );
			}
			if(!(( (CMainFrame *)AfxGetApp()->GetMainWnd() )->GetLoadIfFail()))
				m_pRecentFileList->Remove(nMRUIndex);
}

BOOL CBEOPDesignerApp::CheckFileName(CString cstrPathName)
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (!pMainFrm->CheckFileName(cstrPathName))
	{
		return FALSE;
	}

	if (pMainFrm->m_pPrjSqlite != NULL)
	{
		pMainFrm->m_pPrjSqlite->SetFilePath(cstrPathName.GetString());
		pMainFrm->m_pPrjSqlite->ParseFileType(cstrPathName);

		if (!pMainFrm->m_pPrjSqlite->LoadProjectConfig())
		{
			AfxMessageBox(_T("����Ŀ�ļ���ʽ����"));
			return FALSE;
		}
	}

	return TRUE;
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}
