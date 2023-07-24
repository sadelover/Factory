// Connect.cpp : implementation file
//

#include "stdafx.h"
#include "Connect.h"
#include "afxdialogex.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "MainFrm.h"
#include <sstream>

const int g_nDbNum = 3;
const CString g_strDbName[g_nDbNum] = {_T("beopdata_changgiairport"), _T("beopdata_hkhuarun"), _T("beopdata_shhuawei")};


// CConnect dialog

IMPLEMENT_DYNAMIC(CConnect, CSkinDlg)

CConnect::CConnect(
	const DWORD dwIp,
	const int nPort,
	const CString strUser,
	const CString strPwd,
	const CString strDbName,
	CWnd* pParent /*=NULL*/)
	: CSkinDlg(CConnect::IDD, pParent)
	, m_pDataAccess(NULL)
	, m_pMainFrm(NULL)
	, m_nPort(nPort)
	, m_strUserName(strUser)
	, m_strPassword(strPwd)
	, m_strDbName(strDbName)
	, m_dwIp(dwIp)
{
	m_pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(m_pMainFrm != NULL);

	GetSysPath(m_strCfgPath);
	m_strCfgPath += _T("\\factory.ini");
}

CConnect::~CConnect()
{
}

void CConnect::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_LOGIN, m_ip);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPassword);
	DDX_Control(pDX, IDC_COMBO_DB_NAME, m_comboDbName);
	DDX_CBString(pDX, IDC_COMBO_DB_NAME, m_strDbName);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CConnect, CSkinDlg)
END_MESSAGE_MAP()


// CConnect message handlers


BOOL CConnect::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CConnect::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if (NULL == m_pMainFrm)
	{
		return;
	}

	vector<TextDeviceInfo> vecDevInfo(m_pMainFrm->m_pPrjSqlite->m_vecTextDevInf);
	vector<TextDeviceInfo>::const_iterator iterDev;
	std::ostringstream sqlstreamUpdate;
	string strUtf8PageName;
	int i = 1;
	sqlstreamUpdate << "INSERT INTO diagnosis_equipments(Id, Name, PageId, ModalTextId) VALUES";
	for (iterDev=vecDevInfo.begin(); iterDev!=vecDevInfo.end(); iterDev++,i++)
	{
		wstring wstrName(iterDev->strBindPageName);
		Project::Tools::WideCharToUtf8(wstrName, strUtf8PageName);
		if (1 != i)
		{
			sqlstreamUpdate << ",";
		}
		sqlstreamUpdate << "(" << i << ",'" << strUtf8PageName << "'," << iterDev->nBindPageId << "," << iterDev->nBindTextId << ")";
	}
	string strInsertSql = sqlstreamUpdate.str();

	UpdateData(TRUE);

	try
	{
		m_pDataAccess = new CUPPCServerDataAccess;
		ASSERT(m_pDataAccess != NULL);
		if (m_pDataAccess != NULL)
		{
			CRealTimeDataAccess* pDbAccess = m_pDataAccess->GetRealTimeDataAccess();
			ASSERT(pDbAccess != NULL);
			if (pDbAccess != NULL)
			{
				CString	strIp;
				BYTE bField[4] = {0};
				m_ip.GetAddress(bField[0], bField[1], bField[2], bField[3]);
				strIp.Format(_T("%d.%d.%d.%d"), bField[0], bField[1], bField[2], bField[3]);

				wstring wstrIp(strIp.GetBuffer());
				wstring wstrUser(m_strUserName.GetBuffer());
				wstring wstrPwd(m_strPassword.GetBuffer());
				wstring wstrDbName(m_strDbName.GetBuffer());
				if (!pDbAccess->ConnectToDB(wstrIp, wstrUser, wstrPwd, wstrDbName, m_nPort))
				{	// connect failed
					MessageBox(_T("数据库连接失败！"), _T("错误"), MB_OK|MB_ICONERROR);
					return;
				}

				char szSql[MAX_PATH] = {0};
				USES_CONVERSION;
				sprintf_s(szSql, MAX_PATH, "%s", "DELETE FROM diagnosis_equipments");
				pDbAccess->Execute(szSql);

				if (pDbAccess->Execute(strInsertSql))
				{
					DWORD dwIp = 0;
					m_ip.GetAddress(dwIp);
					CString strIpNum;
					strIpNum.Format(_T("%lu"), dwIp);

					CString strPort;
					strPort.Format(_T("%d"), m_nPort);

					m_pMainFrm->m_dwSyncIp = dwIp;
					m_pMainFrm->m_nSyncPort = m_nPort;
					m_pMainFrm->m_strSyncUsername = m_strUserName;
					m_pMainFrm->m_strSyncPassword = m_strPassword;
					m_pMainFrm->m_strSyncDbName = m_strDbName;

					WritePrivateProfileString(_T("BeopFactory"), _T("SyncIp"), strIpNum, m_strCfgPath);
					WritePrivateProfileString(_T("BeopFactory"), _T("SyncPort"), strPort, m_strCfgPath);
					WritePrivateProfileString(_T("BeopFactory"), _T("SyncUsername"), m_strUserName, m_strCfgPath);
					WritePrivateProfileString(_T("BeopFactory"), _T("SyncPassword"), m_strPassword, m_strCfgPath);
					WritePrivateProfileString(_T("BeopFactory"), _T("SyncDbName"), m_strDbName, m_strCfgPath);

					MessageBox(_T("同步完成！"), _T("完成"), MB_OK|MB_ICONASTERISK);
				}
				else
				{
					MessageBox(_T("同步失败！"), _T("错误"), MB_OK|MB_ICONERROR);
				}
			}
		}
	}
	catch (...)
	{
		return;
	}

	CSkinDlg::OnOK();
}

void CConnect::InitControls(void)
{
	m_ip.SetAddress(m_dwIp);

	int nCurNum = 0;
	bool bDbFind = false;
	for (int i=0; i<g_nDbNum; i++)
	{
		m_comboDbName.AddString(g_strDbName[i]);
		if (g_strDbName[i] == m_strDbName)
		{
			nCurNum = i;
			bDbFind = true;
		}
	}
	if (bDbFind)
	{
		m_comboDbName.SetCurSel(nCurNum);
	}
	else
	{
		m_comboDbName.AddString(m_strDbName);
		m_comboDbName.SetCurSel(g_nDbNum);
	}
}
