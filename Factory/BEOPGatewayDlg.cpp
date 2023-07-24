
// BeopEngineDlg.cpp : ʵ���ļ�
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


// CBEOPGatewayDlg ��Ϣ�������


BOOL CBEOPGatewayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	////ֹͣ����Factory����
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


	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE


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
	

	//������ʼ
	pFrame->m_Onlineplcpointmap.Clear();
	pFrame->m_OnlinelogicPotManager.Clear();
	pFrame->m_OnlineAlarmPotManager.Clear();

	pFrame->m_pOnlinePrjSqlite->SetFilePath(pFrame->m_strDBFileName.c_str());

	pFrame->m_pOnlinePrjSqlite->SetOwner(eFactory);
	pFrame->m_pOnlinePrjSqlite->LoadProjectConfig();
	pFrame->m_pOnlinePrjSqlite->LoadProjectSqlite_opcid3();
	//���������

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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBEOPGatewayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

	// TODO: �ڴ˴������Ϣ����������
	if (nType == SIZE_MINIMIZED){
//		MoveToTaskBar();
	}
}


void CBEOPGatewayDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(m_PaneServerManager.m_pFileTransfer->m_bIsTransmitting)
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("��ʾ"), _T("������װ�У��޷��ر����ߵ���."), TRUE);
		WarnDlg.DoModal();
		return;
	}
	CWarningDlg WarnDlg;
	WarnDlg.InitWarn(_T("����"), _T("���ر����ߵ���, �Ƿ�ȷ��?"), FALSE);
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

		////��ʼ����Factory����
		//CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
		//if(pFrame)
		//	pFrame->m_bUpdateScreen = true;

		CDialog::OnCancel();
	}

}


void CBEOPGatewayDlg::OnBnClickedButtonExport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	if(m_PaneDataEngine.m_nIndexCurDisplay<0)
	//	return;
	
	//CFileDialog dlgOpenFile(FALSE, NULL,L"*.txt",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"*.txt");
	//if(dlgOpenFile.DoModal()==IDOK)
	//{
	//	CString dbName = dlgOpenFile.GetPathName();//��GetFileNameֻ�ܵõ��ļ�����û��Path��Ϣ��

	//	CStdioFile ff(dbName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);

	//	ff.WriteString(m_PaneDataEngine.m_strLogAll);
	//	
	//	ff.Close();
	//}

}


void CBEOPGatewayDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==1)
	{
		//feng
		//golding2014.1.16if(theApp.m_pDataEngineCore->m_bUpdateInputTableFromSite) //���ֳ���������ʱ�Ĵ���
		//golding2014.1.16{

		//golding2014.1.16	theApp.m_pDataEngineCore->UpdateInputTable();

		//golding2014.1.16}
		//golding2014.1.16else  //�����ֳ�ͨѶ����ģ�����
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

		//�����Ϊ�˺��뼶ɨ�裬������һ���߳��д���
		//golding2014.1.16wstring strValueChanged;
		//golding2014.1.16theApp.m_pDataEngineCore->GetDataLink()->UpdateOutputParams(strValueChanged);
		//golding2014.1.16 m_PaneDataEngine.LogDataEngineMsgFunc((WPARAM)_T("RealtimeOutput"), (LPARAM) strValueChanged.c_str());
	}


	CDialog::OnTimer(nIDEvent);
}



void CBEOPGatewayDlg::OnBnClickedButtonPoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//m_PaneArbiter.ShowWindow(FALSE);
	//m_pPointDisplay->ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(FALSE);

}

void CBEOPGatewayDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������


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

			//���µ�ֵ , ע��ֻ���������߳�����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_PaneArbiter.ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(TRUE);
	//m_pPointDisplay->ShowWindow(FALSE);
}


void CBEOPGatewayDlg::OnBnClickedButtonCurve2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_PaneArbiter.ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(FALSE);
	////m_pPointDisplay->ShowWindow(FALSE);
}


void CBEOPGatewayDlg::OnBnClickedButtonCurve3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_PaneArbiter.ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(FALSE);
	//m_pPointDisplay->ShowWindow(FALSE);
}


void CBEOPGatewayDlg::OnBnClickedButtonCurve4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_PaneArbiter.ShowWindow(FALSE);
	m_PaneServerManager.ShowWindow(FALSE);
	//m_pPointDisplay->ShowWindow(FALSE);
}
