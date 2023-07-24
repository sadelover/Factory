// DownLoadFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DownLoadFileDlg.h"
#include "afxdialogex.h"
#include "../Tools/Util/UtilString.h"
#include "../ComponentDraw/sqlite/SqliteAcess.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "../ControlEx/WarningDlg.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "MainFrm.h"
#include "DataPoint/DataPointManager.h"
#include "oleExcel/OperateExcelFile.h"
// CDownLoadFileDlg dialog

IMPLEMENT_DYNAMIC(CDownLoadFileDlg, CDialog)

CDownLoadFileDlg::CDownLoadFileDlg(CString strPath,CWnd* pParent /*=NULL*/)
	: CDialog(CDownLoadFileDlg::IDD, pParent)
	, m_strDownloadFile(strPath)
	, m_strServerIP(_T("localhost"))
	, m_strErrTitle(_T("错误"))
	, m_unErrType(MB_OK|MB_ICONERROR)
	//, m_pPrjSqlite(NULL)
	, m_nTimeCount(0)
	//, m_pDataAccess(NULL)
	, m_nFileSize(0)
{
	m_pFileTransferRecv = new CDebugToolFileTransfer(3458);
	m_pFileTransfer = new CDebugToolFileTransfer(3459);

	m_strLastDownloadResponse = L"";
	m_nNoResponseTimeCount = 0;
}

CDownLoadFileDlg::~CDownLoadFileDlg()
{
	
	if(m_pFileTransfer)
	{
		delete m_pFileTransfer;
		m_pFileTransfer = NULL;
	}

	if(m_pFileTransferRecv)
	{
		delete m_pFileTransferRecv;
		m_pFileTransferRecv = NULL;
	}
}

void CDownLoadFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DBNAME, m_strDownloadFile);
	DDX_Control(pDX, IDC_PROGRESS1, m_pProgess);
	DDX_Text(pDX, IDC_EDIT_SERVER_IP, m_strServerIP);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_ProgressStatic);
}


BEGIN_MESSAGE_MAP(CDownLoadFileDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LOADFILE, &CDownLoadFileDlg::OnBnClickedButtonLoadfile)
	ON_BN_CLICKED(IDC_BUTTON_WRITEPOINTS_TO_SERVER, &CDownLoadFileDlg::OnBnClickedButtonWritepointsToServer)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_TO_SERVER, &CDownLoadFileDlg::OnBnClickedButtonWriteToServer)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CDownLoadFileDlg::OnNMCustomdrawProgress1)
END_MESSAGE_MAP()


// CDownLoadFileDlg message handlers


void CDownLoadFileDlg::OnBnClickedButtonLoadfile()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgOpenFile(TRUE,NULL,L"*.s3db",OFN_HIDEREADONLY|OFN_HIDEREADONLY,L"s3db Files (*.s3db)|*.s3db|");
	if(dlgOpenFile.DoModal()==IDOK)
	{
		CString dbName = dlgOpenFile.GetPathName();
		m_strDownloadFile = dbName;

		UpdateData(FALSE);
	}
}

void CDownLoadFileDlg::OnBnClickedButtonWritepointsToServer()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// 读取点表数据
     
	vector<DataPointEntry> vecPointEntry;

	CStringA strTemp = "";
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	
	CProjectSqlite* pPrjSqlite = pFrame->m_pPrjSqlite;
	pPrjSqlite->SetFilePath(m_strDownloadFile.GetString());
	pPrjSqlite->LoadProjectSqlite_opcid3();
	vecPointEntry = pPrjSqlite->GetPointList();


	// 将点表导出为?.csv文件
	CString strFilePath = _T("");
	CString strName;

	//new excel file name
	//new excel file name
	TCHAR szAppDir[MAX_PATH] = {0};
	TCHAR szPathName[MAX_PATH] = {0};
	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	CString strPathName;
	strPathName.Format(_T("%s"), szAppDir);
	strPathName = strPathName.Left(strPathName.ReverseFind(_T('\\')) + 1);

	strPathName += _T("tmppointtable.xlsx");


	

	CFile fileVer;
	CFileException ex;


	COperateExcelFile::Instance()->SaveToExcel(strPathName, pFrame->GetPointMap());
	//if (!SaveToCsvFile(strExcelFilePath.c_str(), vecPointEntry))
	//{
		//return;
	//}



	// 将?.csv文件上传至MySql表filestorage中
	//m_pDataAccess->UpdateLibInsertIntoFilestorageCal(strFilePath, strName, strName);
	CHttpOperation http(m_strServerIP, 5000, _T("corefile/uploadpointtable"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();


	CString strResult;
	http.UploadPic(strPathName , m_strServerIP, 5000, _T("corefile/uploadpointtable"));
	CString strShow;
	CString strRet(_T("\r\n")); 
	CString strTips;

	//m_pFileTransfer->StartAsSender(wstrIP.c_str(), 3456);
	m_nNoResponseTimeCount = 0;
	m_strLastDownloadResponse = L"";


	// 定时器判断pointdownload值，15秒之内返回结果
	//SetTimer(21, 1000, NULL);
}

void CDownLoadFileDlg::OnBnClickedButtonWriteToServer()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(!CreateConnection(m_strServerIP))
		return;

	CWarningDlg WarnDlg;
	CString strTemp;

	wstring wstrIP = m_strServerIP.GetString();
	strTemp.Format(L"是否确认将数据源的文件下装至%s?", wstrIP.c_str());
	WarnDlg.InitWarn(_T("提示"), strTemp, FALSE);
	if(WarnDlg.DoModal()==IDOK)
	{



		HANDLE hThread = NULL;
		unsigned threadID;
		hThread = (HANDLE)_beginthreadex( NULL, 0, RunDownloadDBFileThread, (LPVOID)this, 0, &threadID );	
		if (hThread)
		{
			CloseHandle(hThread);
			hThread = NULL;
		}

		CString strShow;
		CString strRet(_T("\r\n")); 
		CString strTips;

		//m_pFileTransfer->StartAsSender(wstrIP.c_str(), 3456);
		m_nNoResponseTimeCount = 0;
		m_strLastDownloadResponse = L"";
		//m_ProgressStatic.ShowWindow(TRUE);

		//GetDlgItem(IDC_EDIT_SERVER_IP)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_WRITEPOINTS_TO_SERVER)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_WRITE_TO_SERVER)->EnableWindow(FALSE);

		//SetTimer(1,2000,NULL);

		//m_pDataAccess->WriteCoreDebugItemValue(L"filerecved", L"");
	}
}


unsigned __stdcall CDownLoadFileDlg::RunDownloadDBFileThread( void * pParam )
{//@todo       need Timestamp

	CDownLoadFileDlg *pThis = (CDownLoadFileDlg *) pParam;



	pThis->GetDlgItem(IDC_BUTTON_WRITEPOINTS_TO_SERVER)->EnableWindow(FALSE);
	pThis->GetDlgItem(IDC_BUTTON_WRITE_TO_SERVER)->EnableWindow(FALSE);
	pThis->GetDlgItem(IDC_BUTTON_LOADFILE)->EnableWindow(FALSE);

	pThis->m_pProgess.SetRange(0,10);
	pThis->m_pProgess.SetStep(1);
	pThis->SetTimer(5000,1,NULL);


	CFile cfile; 
	CFileStatus rStatus;
	CFile::GetStatus(pThis->m_strDownloadFile,rStatus);



	CHttpOperation http(pThis->m_strServerIP, 5000, _T("corefile/uploaddb"));

	Json::Value myRoot;
	myRoot["data"] = "test";
	string szBuf = myRoot.toStyledString();


	CString strResult;
	http.UploadPic(pThis->m_strDownloadFile, pThis->m_strServerIP, 5000, _T("corefile/uploaddb"));

	pThis->GetDlgItem(IDC_BUTTON_WRITEPOINTS_TO_SERVER)->EnableWindow(TRUE);
	pThis->GetDlgItem(IDC_BUTTON_WRITE_TO_SERVER)->EnableWindow(TRUE);
	pThis->GetDlgItem(IDC_BUTTON_LOADFILE)->EnableWindow(TRUE);
	
	pThis->KillTimer(1);

	return 1;
}

BOOL CDownLoadFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
}

bool CDownLoadFileDlg::SaveToCsvFile( const CString strFilePathName, const vector<DataPointEntry>& vecPoint )
{
	CFile file;
	if (!file.Open(strFilePathName, CFile::modeCreate|CFile::modeReadWrite))
	{
		MessageBox(_T("创建CSV文件失败！"), m_strErrTitle, m_unErrType);
		return false;
	}

	CString	strContent;
	CString	strEachLine;
	PLCVALUEPROPERTY	nProperty = PLC_READ;
	CString	strProperty;
	vector<DataPointEntry>::const_iterator	iter;
	for (iter=vecPoint.begin(); iter!=vecPoint.end(); ++iter)
	{
		nProperty = iter->GetProperty();
		if (PLC_READ == nProperty)
		{
			strProperty = _T("R");
		}
		else if (PLC_WRITE == nProperty)
		{
			strProperty = _T("W");
		}
		else if (PLC_PROP_MAX == nProperty)
		{
			strProperty = _T("R/W");
		}
		else
		{
			// error
		}
		strEachLine.Format(_T("%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%d\r\n"),
			iter->GetPointIndex(),
			iter->GetShortName().c_str(),
			iter->GetSourceType().c_str(),
			iter->GetDescription().c_str(),
			iter->GetUnit().c_str(),
			strProperty,
			iter->GetParam(1).c_str(),
			iter->GetParam(2).c_str(),
			iter->GetParam(3).c_str(),
			iter->GetParam(4).c_str(),
			iter->GetParam(5).c_str(),	
			iter->GetParam(6).c_str(),
			iter->GetParam(7).c_str(),
			iter->GetParam(8).c_str(),
			iter->GetParam(9).c_str(),
			iter->GetParam(10).c_str(),
			_T(""),
			iter->GetParam(12).c_str(),
			iter->GetParam(13).c_str(),
			iter->GetParam(14).c_str(),
		iter->GetStoreCycle());
		strContent += strEachLine;
	}

	// write into file
	try
	{
		file.Write(strContent, (strContent.GetLength())*2);
		file.Close();
	}
	catch (CFileException* e)
	{
		TCHAR	szCause[MAX_PATH] = {0};
		CString strErr;

		e->GetErrorMessage(szCause, MAX_PATH);
		strErr.Format(_T("%s"), szCause);
		MessageBox(strErr, m_strErrTitle, m_unErrType);
		return false;
	}
	catch (...)
	{
		MessageBox(_T("写入CSV文件异常！"), m_strErrTitle, m_unErrType);
		return false;
	}


	return true;
}

void CDownLoadFileDlg::OnTimer( UINT_PTR nIDEvent )
{
	if(nIDEvent==1)
	{
		
		m_pProgess.StepIt();
	}
	else if(nIDEvent==2)
	{
		if(m_pFileTransferRecv->m_bNewFileRecved)
		{
			KillTimer(2);
			m_ProgressStatic.ShowWindow(FALSE);
			AfxMessageBox(L"LOG文件下载成功!");
			GetDlgItem(IDC_BUTTON_WRITE_TO_SERVER)->EnableWindow(TRUE);			
			m_pFileTransferRecv->m_bNewFileRecved = false;
			return;
		}
	}
	else if (21 == nIDEvent)
	{
		++m_nTimeCount;
		if (m_nTimeCount >= 15)
		{
			m_nTimeCount= 0;
			KillTimer(21);
			MessageBox(_T("下装DB点表失败！"), m_strErrTitle, m_unErrType);
			return;
		}

		// read unit01 pointdownload value
	}
	
	CDialog::OnTimer(nIDEvent);
}

bool CDownLoadFileDlg::CreateConnection( CString strServerIP )
{
	if(strServerIP.GetLength()==0 )
	{
		CWarningDlg dlgWarn;
		dlgWarn.InitWarn(L"错误", L"服务器地址为空，请确认.", TRUE);
		dlgWarn.DoModal();
		return false;
	}

	if(m_strDownloadFile.GetLength()<=0)
	{
		CWarningDlg WarnDlgTemp;
		WarnDlgTemp.InitWarn(_T("错误"), L"请先预载待下装的文件!", FALSE);
		WarnDlgTemp.DoModal();
		return false;
	}

	gDataBaseParam realDBParam;
	realDBParam.strDBIP = 	Project::Tools::WideCharToAnsi(strServerIP);
	realDBParam.strDBName = "";
	realDBParam.strRealTimeDBName = "";
	realDBParam.strUserName = "root";
	realDBParam.nPort = 3306;
	realDBParam.strDBIP2 = "";

	return true;
}


void CDownLoadFileDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
