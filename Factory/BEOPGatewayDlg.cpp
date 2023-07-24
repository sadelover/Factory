
// BeopEngineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BEOP-Designer.h"
#include "BEOPGatewayDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include "Tools/warning/MsgDefine.h"
#include "ControlEx/WarningDlg.h"
#include "dlgload.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "MainFrm.h"
#include "BEOPLogicManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CBEOPGatewayDlg::CBEOPGatewayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBEOPGatewayDlg::IDD, pParent)
{
	m_pDataAccess = NULL;
	m_EventStopThread = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hupdatedatahandle = NULL;
}

void CBEOPGatewayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BUTTON_ARBITER, m_btnLogic);
	DDX_Control(pDX, IDC_BUTTON_Point, m_btnPoint);
	DDX_Control(pDX, IDC_BUTTON_SERVER_MANAGER, m_btnServerManager);
	DDX_Control(pDX, IDC_BUTTON_CURVE, m_btnRealtimeCurve);
	DDX_Control(pDX, IDC_BUTTON_CURVE2, m_btnRealtimeCurve2);
	DDX_Control(pDX, IDC_BUTTON_CURVE3, m_btnRealtimeCurve3);
	DDX_Control(pDX, IDC_BUTTON_CURVE4, m_btnRealtimeCurve4);
}

BEGIN_MESSAGE_MAP(CBEOPGatewayDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DATAENGINELOG, LogMsgFunc)
	ON_WM_SIZE()

	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CBEOPGatewayDlg::OnBnClickedButtonExport)
	ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_BUTTON_ARBITER, &CBEOPGatewayDlg::OnBnClickedButtonArbiter)
	ON_BN_CLICKED(IDC_BUTTON_Point, &CBEOPGatewayDlg::OnBnClickedButtonPoint)
	ON_BN_CLICKED(IDC_BUTTON_CURVE, &CBEOPGatewayDlg::OnBnClickedButtonCurve)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SERVER_MANAGER, &CBEOPGatewayDlg::OnBnClickedButtonServerManager)
	ON_BN_CLICKED(IDC_BUTTON_CURVE2, &CBEOPGatewayDlg::OnBnClickedButtonCurve2)
	ON_BN_CLICKED(IDC_BUTTON_CURVE3, &CBEOPGatewayDlg::OnBnClickedButtonCurve3)
	ON_BN_CLICKED(IDC_BUTTON_CURVE4, &CBEOPGatewayDlg::OnBnClickedButtonCurve4)
END_MESSAGE_MAP()


// CBEOPGatewayDlg 消息处理程序


BOOL CBEOPGatewayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// IDM_ABOUTBOX 必须在系统命令范围内。

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	////停止更新Factory画布
	//CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	//if(pFrame)
	//	pFrame->m_bUpdateScreen = false;

	//button
	//m_btnLogic.SetFlat(FALSE);
	//	m_btnLogic.SubclassDlgItem(IDC_BUTTON_ARBITER,this); 
	//m_btnLogic.SetIcon(IDI_ICON_LOGIC); 
	//m_btnLogic.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208)); 
	//m_btnLogic.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(208,208,208)); 
	//m_btnLogic.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(188,188,188)); 
	//m_btnLogic.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnPoint.SetFlat(FALSE);
	m_btnPoint.SetIcon(IDI_ICON_POINT); 
	m_btnPoint.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(208,208,208)); 
	m_btnPoint.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208)); 
	m_btnPoint.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(188,188,188)); 
	m_btnPoint.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);


	m_btnServerManager.SetFlat(FALSE);
	m_btnServerManager.SetIcon(IDI_ICON_SERVERMANAGER); 
	m_btnServerManager.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(208,208,208)); 
	m_btnServerManager.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208)); 
	m_btnServerManager.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(188,188,188)); 
	m_btnServerManager.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	

	m_btnRealtimeCurve.SetFlat(FALSE);
	m_btnRealtimeCurve.SetIcon(IDI_ICON_REALTIMECURVE); 
	m_btnRealtimeCurve.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(208,208,208)); 
	m_btnRealtimeCurve.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208)); 
	m_btnRealtimeCurve.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(188,188,188)); 
	m_btnRealtimeCurve.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnRealtimeCurve2.SetFlat(FALSE);
	m_btnRealtimeCurve2.SetIcon(IDI_ICON_REALTIMECURVE); 
	m_btnRealtimeCurve2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(208,208,208)); 
	m_btnRealtimeCurve2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208)); 
	m_btnRealtimeCurve2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(188,188,188)); 
	m_btnRealtimeCurve2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnRealtimeCurve3.SetFlat(FALSE);
	m_btnRealtimeCurve3.SetIcon(IDI_ICON_REALTIMECURVE); 
	m_btnRealtimeCurve3.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(208,208,208)); 
	m_btnRealtimeCurve3.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208)); 
	m_btnRealtimeCurve3.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(188,188,188)); 
	m_btnRealtimeCurve3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnRealtimeCurve4.SetFlat(FALSE);
	m_btnRealtimeCurve4.SetIcon(IDI_ICON_REALTIMECURVE); 
	m_btnRealtimeCurve4.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(208,208,208)); 
	m_btnRealtimeCurve4.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208)); 
	m_btnRealtimeCurve4.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(188,188,188)); 
	m_btnRealtimeCurve4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);


	// TODO: 在此添加额外的初始化代码
	//	MoveWindow(0,0,0,0);

	//m_btnLogic.ShowWindow(FALSE);
	m_btnPoint.ShowWindow(FALSE);
	m_btnRealtimeCurve.ShowWindow(FALSE);
	m_btnServerManager.ShowWindow(FALSE);
	m_btnRealtimeCurve2.ShowWindow(FALSE);
	m_btnRealtimeCurve3.ShowWindow(FALSE);
	m_btnRealtimeCurve4.ShowWindow(FALSE);


	CRect rc;
	GetClientRect(rc);
	rc.top+=60;
	rc.left+=5;
	rc.right-=5;
	rc.bottom-=5;


	//m_PaneArbiter.Create(IDD_DIALOG_ARBITER, this);
	//m_PaneArbiter.MoveWindow(rc);
	//m_PaneArbiter.ShowWindow(FALSE);

	//m_pPointDisplay = new CPointDialog;
	//m_pPointDisplay->Create(IDD_DIALOG_Point);
	//m_PointDisplay.MoveWindow(rc);
	//m_pPointDisplay->ShowWindow(FALSE);

	m_PaneServerManager.Create(IDD_DIALOG_SERVERMAMAGER, this);
	m_PaneServerManager.MoveWindow(rc);
	m_PaneServerManager.ShowWindow(FALSE);	


	m_PaneLoad.Create(IDD_LOAD, this);
	m_PaneLoad.MoveWindow(rc);
	m_PaneLoad.ShowWindow(TRUE);	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE


//	m_Btn_DataCom.SetIcon(LoadIcon(NULL,MAKEINTRESOURCE(IDI_ICON1)));

}

bool CBEOPGatewayDlg::InitData()
{

	CString strTitle;
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	wstring strIP = Project::Tools::AnsiToWideChar(pFrame->m_dbset.strDBIP.data());

	gDataBaseParam realDBParam;
	realDBParam.strDBIP = 	pFrame->m_dbset.strDBIP;
	realDBParam.strDBName = "";
	realDBParam.strRealTimeDBName = "";
	realDBParam.strUserName = "root";
	realDBParam.nPort = 3306;
	realDBParam.strDBIP2 = "";

	m_pDataAccess = new CUPPCServerDataAccess;
	m_pDataAccess->SetDBparam(realDBParam);
	if(m_pDataAccess->InitConnection()==false)
	{
		return false;
	}

	m_pDataAccess->ReadServerPointsList(m_vecServerPointList);

	StartUpdateThread();


	CString strExePath;
	Project::Tools::GetSysPath(strExePath);
	strExePath += L"\\debugtool.ini";
	wchar_t strRight[MAX_PATH] = {0};
	GetPrivateProfileString(L"debugtool", L"right", L"", strRight, 1024, strExePath.GetString());
	wstring wstrRight = strRight;
	
		m_PaneServerManager.m_pDataAccess = m_pDataAccess;
		m_PaneServerManager.m_pGatewayDlg = this;
		m_PaneServerManager.InitData();

		m_btnServerManager.ShowWindow(TRUE);
	

	//读入点表开始
	pFrame->m_Onlineplcpointmap.Clear();
	pFrame->m_OnlinelogicPotManager.Clear();
	pFrame->m_OnlineAlarmPotManager.Clear();

	pFrame->m_pOnlinePrjSqlite->SetFilePath(pFrame->m_strDBFileName.c_str());

	pFrame->m_pOnlinePrjSqlite->SetOwner(eFactory);
	pFrame->m_pOnlinePrjSqlite->LoadProjectConfig();
	pFrame->m_pOnlinePrjSqlite->LoadProjectSqlite_opcid3();
	//读入点表结束

	//m_pPointDisplay->m_pDataAccess = m_pDataAccess;
	//m_PaneArbiter.m_pDataAccess = m_pDataAccess;
	//m_PaneArbiter.m_pLogicManager->m_pDataAccess = m_pDataAccess;

	//m_PaneArbiter.m_pointmanager = &(pFrame->GetOnlinePointMap()); //theApp.m_pDataEngineCore->GetDataPointManager();


		//m_pRealTimeTrendDlg = new CRealTimeTrendDlg(m_pDataAccess, m_PaneArbiter.m_pointmanager, NULL);
		//ASSERT(m_pRealTimeTrendDlg);


	//setting pane
	//	m_PaneDataEngine.InitData();
	//m_PointDisplay.m_pGatewayDlg = this;
	//m_PointDisplay.InitData();
	//m_PaneArbiter.InitData();




	//m_btnLogic.ShowWindow(TRUE);

	m_btnPoint.ShowWindow(TRUE);

	m_PaneLoad.ShowWindow(FALSE);

	return true;
}

void CBEOPGatewayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
	CDialog::OnSysCommand(nID, lParam);
	
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBEOPGatewayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBEOPGatewayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CBEOPGatewayDlg::PreTranslateMessage( MSG* pMsg )
{
	DWORD msg = pMsg->message;
	if (msg == WM_KEYDOWN)
	{
		DWORD keytype = pMsg->wParam;
		if (keytype == VK_ESCAPE || keytype == VK_RETURN){
			return true;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


LRESULT CBEOPGatewayDlg::LogMsgFunc( WPARAM wparam, LPARAM lparam )
{
//	m_PaneDataEngine.LogDataEngineMsgFunc(wparam, lparam);

	return 1;
}



void CBEOPGatewayDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_MINIMIZED){
//		MoveToTaskBar();
	}
}


void CBEOPGatewayDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_PaneServerManager.m_pFileTransfer->m_bIsTransmitting)
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("提示"), _T("正在下装中，无法关闭在线调试."), TRUE);
		WarnDlg.DoModal();
		return;
	}
	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("警告"), _T("将关闭在线调试, 是否确定?"), FALSE);
	if(WarnDlg.DoModal()==IDOK)
	{
		//CSkinDlg::OnClose();
		//m_PaneArbiter.Exit();
		m_PaneServerManager.Exit();
		//m_pPointDisplay->ExitThreads();

		SetEvent(m_EventStopThread);
		::WaitForSingleObject(m_hupdatedatahandle, INFINITE);

		if(m_pDataAccess)
		{
			delete m_pDataAccess;
			m_pDataAccess = NULL;
		}

		////开始更新Factory画布
		//CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
		//if(pFrame)
		//	pFrame->m_bUpdateScreen = true;

		CDialog::OnCancel();
	}

}


void CBEOPGatewayDlg::OnBnClickedButtonExport()
{
	// TODO: 在此添加控件通知处理程序代码
//	if(m_PaneDataEngine.m_nIndexCurDisplay<0)
	//	return;
	
	//CFileDialog dlgOpenFile(FALSE, NULL,L"*.txt",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"*.txt");
	//if(dlgOpenFile.DoModal()==IDOK)
	//{
	//	CString dbName = dlgOpenFile.GetPathName();//用GetFileName只能得到文件名，没有Path信息！

	//	CStdioFile ff(dbName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);

	//	ff.WriteString(m_PaneDataEngine.m_strLogAll);
	//	
	//	ff.Close();
	//}

}


void CBEOPGatewayDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==1)
	{
		//feng
		//golding2014.1.16if(theApp.m_pDataEngineCore->m_bUpdateInputTableFromSite) //与现场保持连接时的处理
		//golding2014.1.16{

		//golding2014.1.16	theApp.m_pDataEngineCore->UpdateInputTable();

		//golding2014.1.16}
		//golding2014.1.16else  //不与现场通讯，仅模拟测试
		//golding2014.1.16{//

		//golding2014.1.16	SYSTEMTIME stNow;
		//golding2014.1.16	GetLocalTime(&stNow);

		//golding2014.1.16	vector<CRealTimeDataEntry>	inputentrylist;
		//golding2014.1.16	theApp.m_pDataEngineCore->GetRealTimeDataAccess()->ReadRealtimeData_Input(inputentrylist);
		//golding2014.1.16	unsigned int i = 0;
		//golding2014.1.16	for(i=0;i<inputentrylist.size();i++)
		//golding2014.1.16	{
		//golding2014.1.16		inputentrylist[i].SetTimestamp(stNow);
		//golding2014.1.16	}
		//golding2014.1.16	theApp.m_pDataEngineCore->GetDataLink()->SetRealTimeDataEntryList(inputentrylist);
		//golding2014.1.16	inputentrylist.clear();//golding add

		//golding2014.1.16}

		//输出表为了毫秒级扫描，将在另一个线程中处理
		//golding2014.1.16wstring strValueChanged;
		//golding2014.1.16theApp.m_pDataEngineCore->GetDataLink()->UpdateOutputParams(strValueChanged);
		//golding2014.1.16 m_PaneDataEngine.LogDataEngineMsgFunc((WPARAM)_T("RealtimeOutput"), (LPARAM) strValueChanged.c_str());
	}


	CDialog::OnTimer(nIDEvent);
}



void CBEOPGatewayDlg::OnBnClickedButtonPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	//if(m_PaneArbiter.GetWatchPointUpdate())
	{
		//m_pPointDisplay->RefreshFromDB();
	//	m_PaneArbiter.SetWatchPointUpdate(false);
	}
//	m_PaneDataEngine.ShowWindow(FALSE);
	//m_PaneArbiter.ShowWindow(FALSE);
	//m_pPointDisplay->ShowWindow(TRUE);
	//m_PaneServerManager.ShowWindow(FALSE);


}


void CBEOPGatewayDlg::OnBnClickedButtonCurve()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//m_PaneArbiter.ShowWindow(FALSE);
	//m_pPointDisplay->ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(FALSE);

}

void CBEOPGatewayDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码


}

void CBEOPGatewayDlg::StartUpdateThread()
{
	m_hupdatedatahandle = (HANDLE)_beginthreadex(NULL, 0, ThreadFuncUpdateData, this, NORMAL_PRIORITY_CLASS, NULL);
}

UINT WINAPI CBEOPGatewayDlg::ThreadFuncUpdateData( LPVOID lparam )
{
	CBEOPGatewayDlg* pThis = (CBEOPGatewayDlg*)lparam;
	if (pThis != NULL)
	{
		while(TRUE)
		{
			if(::WaitForSingleObject(pThis->m_EventStopThread, 0)== WAIT_OBJECT_0)
			{
				break;
			}

			//更新点值 , 注意只在主界面线程中做
			pThis->m_pDataAccess->UpdatePhyPointValRecord();

			if(::WaitForSingleObject(pThis->m_EventStopThread, 0)== WAIT_OBJECT_0)
			{
				break;
			}

			Sleep(2*1000);
		}
	}
	return 0;
}


void CBEOPGatewayDlg::OnBnClickedButtonServerManager()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_PaneArbiter.ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(TRUE);
	//m_pPointDisplay->ShowWindow(FALSE);
}


void CBEOPGatewayDlg::OnBnClickedButtonCurve2()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_PaneArbiter.ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(FALSE);
	////m_pPointDisplay->ShowWindow(FALSE);
}


void CBEOPGatewayDlg::OnBnClickedButtonCurve3()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_PaneArbiter.ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(FALSE);
	//m_pPointDisplay->ShowWindow(FALSE);
}


void CBEOPGatewayDlg::OnBnClickedButtonCurve4()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_PaneArbiter.ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(FALSE);
	//m_pPointDisplay->ShowWindow(FALSE);
}
