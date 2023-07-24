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
	//清空注册表
	if (SHDeleteKey(HKEY_CURRENT_USER,_T("SOFTWARE\\Local AppWizard-Generated Applications\\BEOP-Designer\\Workspace"))!=ERROR_SUCCESS)
	{
		//AfxMessageBox(_T("清空注册表失败！"));
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
	m_cstrUpdateLog+=L"[2.5.77 2020-12-31]：矩形增加蓝白文本框类型\r\n";
	m_cstrUpdateLog+=L"[2.5.76 2020-12-28]：增加MoxaTCPServer点类型。\r\n";
	m_cstrUpdateLog+=L"[2.5.75 2020-12-25]：双击线程和跳转至线程操作时会为选中线程选择一个红色作为突出色彩。\r\n";
	m_cstrUpdateLog+=L"[2.5.74 2020-12-24]：“策略指令查询”增加选择单挑日志跳转至线程界面功能。\r\n";
	m_cstrUpdateLog+=L"[2.5.73 2020-12-24]：“图源丢失情况检查”功能修复并增加导出功能。\r\n";
	m_cstrUpdateLog+=L"[2.5.72 2020-12-23]：“工具”菜单增加“图源丢失情况检查”功能。\r\n";
	m_cstrUpdateLog+=L"[2.5.71 2020-12-21]：“项目配置”界面增加“显示管理”。\r\n图元指向的图片资源不存在时显示404图片。\r\n";
	m_cstrUpdateLog+=L"[2.5.70 2020-12-16]：“项目配置”界面改版。\r\n";
	m_cstrUpdateLog+=L"[2.5.69 2020-12-15]：“策略健康分析”功能改版。\r\n";
	m_cstrUpdateLog+=L"[2.5.68 2020-12-14]：“搜索最接近选择时间产生日志的策略”窗口增加搜索结果，可以双击打开策略一分钟内容。\r\n";
	m_cstrUpdateLog+=L"[2.5.67 2020-12-13]：“点表管理”窗口“屏蔽点”增加选择点直接追加功能。\r\n";
	m_cstrUpdateLog+=L"[2.5.66 2020-12-10]：“被控点搜索”功能增加跳转至线程界面功能。\r\n";
	m_cstrUpdateLog+=L"[2.5.65 2020-12-09]：增加模糊搜索策略功能,此功能可以直接跳转到策略所在线程中。\r\n";
	m_cstrUpdateLog+=L"[2.5.64 2020-12-08]：增加API。\r\n";
	m_cstrUpdateLog+=L"[2.5.63 2020-12-07]：直线绘制优化。增加多边形的拖动功能。修复图元删除后无法恢复的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.62 2020-12-07]：绘制矩形，增加圆角和填充色属性。修复会选中已删除图元的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.61 2020-12-04]：修正“历史数据补算窗口”不显示滚动条的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.60 2020-12-02]：增加：图元绘制过程中可以右键取消绘制。\r\n";
	m_cstrUpdateLog+=L"[2.5.59 2020-12-02]：“搜索最接近选择时间产生日志的策略”功能增加1分钟的log日志的显示。\r\n";
	m_cstrUpdateLog+=L"[2.5.58 2020-12-01]：“策略”树增加右键选项“搜索最接近选择时间产生日志的策略”的功能。\r\n";
	m_cstrUpdateLog+=L"[2.5.57 2020-12-01]：管道创建绘制中和长度角度改变时效果的改善。\r\n";
	m_cstrUpdateLog+=L"[2.5.56 2020-11-30]：为“管道”增加创建时和调整时的斜线和shift按键按下时只能画直线，并修复选中错误的问题。\r\n“多边形”默认线宽改为2,并修复点创建绘制时点击位置与显示位置不符的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.55 2020-11-30]：修复绘画界面中任意点击会选中图元“多边形”的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.54 2020-11-30]：“点表配置”增加“补算历史数据”可根据选择的点做批量点计算。\r\n“策略管理”中策略复制不再会带有dll的字样\r\n";
	m_cstrUpdateLog+=L"[2.5.53 2020-11-26]：“通用组件设置”增加“通用表格热图组件”。\r\n";
	m_cstrUpdateLog+=L"[2.5.52 2020-11-23]：增加多边形绘制功能。\r\n";
	m_cstrUpdateLog+=L"[2.5.51 2020-11-18]：增加直线在shift按下时只能画直线的功能。\r\n修正通讯与调试中部分保存成功后重新打开不能正确显示。\r\n";
	m_cstrUpdateLog+=L"[2.5.50 2020-11-17]：修正“图元删除后关闭工程且不保存，再次打开工程图元消失”的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.49 2020-11-13]：修正自定义组件在切换视图时重复添加的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.48 2020-11-12]：增加绘制直线功能。\r\n";
	m_cstrUpdateLog+=L"[2.5.47 2020-11-11]：自定义组件窗口优化。\r\n";
	m_cstrUpdateLog+=L"[2.5.46 2020-11-10]：修正：自定义组件“数据查询组件”编码错误。\r\n";
	m_cstrUpdateLog+=L"[2.5.45 2020-11-08]：“API选择”窗口增加自动补全功能。\r\n";
	m_cstrUpdateLog+=L"[2.5.44 2020-11-08]：自定义组件增加“叠加柱状图组件”。\r\n";
	m_cstrUpdateLog+=L"[2.5.43 2020-11-04]：1.自定义组件的子窗口参数快速查询控件重构为树状图。\r\n2.“点表管理”窗口的点类型筛选功能增加在保存时充值筛选列表。\r\n";
	m_cstrUpdateLog+=L"[2.5.40 2020-11-01]：管道增加斜向绘制的功能。标准设备配置增加禁止显示的功能。\r\n";
	
	m_cstrUpdateLog+=L"[2.4.98 2020-07-03]：增加“被控点搜索”搜不到点的提示，增加“系统后台进程设置”增加项的时候提示dompysite没开启的提示。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.4.99 2020-07-07]：“策略日志查询”窗口 增加：关键字检索功能，直接列出所有关键字所有列出的行，双击滚动。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.00 2020-07-09]：“策略线程管理”新建和改名的的弹窗都加入非英文数字和下划线的判断。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.01 2020-07-09]：“策略指令记录查询”增加对新版domlogic新log存储方式支持，同时兼容原来的数据库形式，一同显示在编辑框中。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.02 2020-07-16]：“点表配置”窗口中“导入点表”按钮优化导入速度。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.03 2020-07-17]：“点表配置”窗口中“导入点表”按钮优化导入速度。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.04 2020-07-18]：“管道属性”窗口中增加测算按钮和扩大绑定点输入框大小。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.05 2020-07-22]：“图元选择窗口”窗口调整翻页按钮至中间显眼位置位置。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.06 2020-07-22]：“策略日志查询”窗口大小调整。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.07 2020-07-27]：“策略线程管理”窗口“从选中复制新建”按钮从取策略名称改为取自定义名称。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.08 2020-08-06]：“数据”菜单增加“精确时间导出历史数据”选项。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.09 2020-08-07]：“调试”菜单中所有重启和停止操作全部增加log（core\log\CRITICAL.txt）;“策略日志查询”窗口扩大。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.10 2020-08-12]： 修正“导入页面后拖动页面至其他位置”的动作无法保存拖动的位置问题。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.11 2020-08-14]： 修正在保存页面时保存失败没有提示也无法继续保存的问题。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.12 2020-08-18]： “策略指令记录查询”窗口的“查询”按钮限制查询最近三日记录。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.13 2020-08-21]： “标准设备配置”窗口的参数设定窗口增加显示原参数。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.14 2020-08-24]： “策略指令记录查询”窗口的最新指令改在最下方显示。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.15 2020-08-27]： “标准设备配置”窗口增加支持中文。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.16 2020-08-27]： “设备系统定义配置”窗口增加简易化配置界面。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.17 2020-09-01]： “重载虚拟点实时值”和“备份当前虚拟点实时值”功能增加对domcore正在运行的判断。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.18 2020-09-01]： “设备系统定义配置”界面修正脚本界面的字符限制问题和ESC按键导致界面消失的问题。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.19 2020-09-02]： “设备系统定义配置”修复Ctrl+A快捷键失效的问题。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.20 2020-09-02]： “点表配置”窗口“删除点”按钮机制更改为：需要按下保存键才会生效。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.21 2020-09-02]： “点表配置”窗口增加点类型筛选。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.22 2020-09-03]： “策略记录查询”窗口解决查询速度慢的问题。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.23 2020-09-08]： “数据”菜单中增加两个功能“数据库恢复”以及“数据库备份管理”。\r\n\
		“系统设备定义配置”窗口增加“二次冷冻泵组”；修正脚本界面出现全空且无法切换到配置见面的问题。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.24 2020-09-09]： “策略线程管理”窗口的“定义配置脚本”子窗口保存后不再需要重启Factory。\r\n\
		“数据”菜单中“数据库恢复”功能，修正子程序闪退的问题。\r\n\r\n";
	m_cstrUpdateLog+=L"[2.5.25 2020-09-12]： “编辑点”窗口vpoint模式下增加单点补算功能按钮“历史数据补算”。\r\n";
	m_cstrUpdateLog+=L"[2.5.26 2020-09-12]： “运维”菜单中的“重载虚拟点实时值”功能解决在在超过8880个数据后就失败的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.27 2020-09-13]： 编辑点界面升级。\r\n";
	m_cstrUpdateLog+=L"[2.5.28 2020-09-14]：“编辑点”窗口“历史数据补算”按钮修复插入错误。\r\n";
	m_cstrUpdateLog+=L"[2.5.29 2020-09-14]：“编辑点”窗口“历史数据补算”按钮弹窗增加插入等待问题提示，不再会死等。\r\n\
		“被控点搜索”窗口修复无法搜索到脚本中的点的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.29 2020-09-14]：“编辑点”窗口“历史数据补算”按钮弹窗增加计算等待问题提示，不再会死等导致Factory假死。\r\n";
	m_cstrUpdateLog+=L"[2.5.30 2020-09-16]：“编辑点”窗口“历史数据补算”按钮弹窗界面调整。\r\n";
	m_cstrUpdateLog+=L"[2.5.31 2020-09-16]：“设置控制策略参数”窗口启用输出参数“当前值”列，可以实时观察当前值。\r\n";
	m_cstrUpdateLog+=L"[2.5.32 2020-09-17]：修正“策略日志查询”查询窗口的乱码问题\r\n";
	m_cstrUpdateLog+=L"[2.5.33 2020-09-17]：修正右键“策略”项中的“连接调试”与其子项中双击自动提示的连接不关联的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.34 2020-09-18]：修复“通讯点位选择”窗口添加点名后无法在点表管理查到。“配置参数”窗口调用上述操作时会提示无法查到此点的问题。\r\n";
	m_cstrUpdateLog+=L"[2.5.35 2020-09-21]：修复通过各种进入调试方式互不兼容的导致几乎所有方式进入调试模式后再进入线程时仍然提示需要链接。\r\n";
	m_cstrUpdateLog+=L"[2.5.36 2020-10-11]：修复“设置控制策略参数”窗口线程组合框显示错误导致的保存错误。\r\n";
	m_cstrUpdateLog+=L"[2.5.37 2020-10-12]：增加“禁用计算型虚拟点实时计算”选项\r\n";
	m_cstrUpdateLog+=L"[2.5.38 2020-10-13]：修复“设备系统定义配置”窗口“二次冷冻泵组”没有成功加载。\r\n";
	m_cstrUpdateLog+=L"[2.5.39 2020-10-22]：修复“导入页面”窗口导入页面是管道属性缺失和导入页面的背景图丢失的问题。\r\n";
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
	
	//先判断该键值是否存在.
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
	//写入子键xxxfile的默认打开路径 
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
			AfxMessageBox(_T("打开项目文件格式错误！"));
			return FALSE;
		}
	}

	return TRUE;
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}
