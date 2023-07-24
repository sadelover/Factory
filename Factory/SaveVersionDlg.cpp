// SaveVersionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SaveVersionDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "../ComponentDraw/ProjectSqlite.h"


// CSaveVersionDlg �Ի���

IMPLEMENT_DYNAMIC(CSaveVersionDlg, CSkinDlg)

CSaveVersionDlg::CSaveVersionDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CSaveVersionDlg::IDD, pParent)
	, m_strSaveUser(_T(""))
	, m_strShowVersion(_T(""))
	, m_strSaveLog(_T(""))
	, m_pMainFrm(NULL)
	, m_pPrjSql(NULL)
	, m_nNewVerNum(0)
{
	memset(m_szCfgPath, 0, MAX_PATH);
}

CSaveVersionDlg::~CSaveVersionDlg()
{
}

void CSaveVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_SAVE_USER, m_strSaveUser);
	DDX_Text(pDX, IDC_EDIT_SAVE_VERSION, m_strShowVersion);
	DDX_Text(pDX, IDC_EDIT_SAVE_LOG, m_strSaveLog);
}


BEGIN_MESSAGE_MAP(CSaveVersionDlg, CSkinDlg)
END_MESSAGE_MAP()


// CSaveVersionDlg ��Ϣ�������


BOOL CSaveVersionDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(m_pMainFrm != NULL);
	if (m_pMainFrm != NULL)
	{
		m_pPrjSql = m_pMainFrm->m_pPrjSqlite;
		ASSERT(m_pPrjSql != NULL);
	}

	TCHAR szAppDir[MAX_PATH] = {0};
	TCHAR szBuf[MAX_PATH] = {0};

	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	PathRemoveFileSpec(szAppDir);
	PathCombine(m_szCfgPath, szAppDir, _T("factory.ini"));
	GetPrivateProfileString(_T("BeopFactory"), _T("SaveUser"), _T(""), szBuf, MAX_PATH, m_szCfgPath);
	m_strSaveUser.Format(_T("%s"), szBuf);

	m_nNewVerNum = m_pPrjSql->ReadLastVersionNumber() + 1;
	CTime tmNow = CTime::GetCurrentTime();
	m_strTime = tmNow.Format(_T("%Y%m%d_%H%M"));
	m_strShowVersion.Format(_T("%04d_%s"), m_nNewVerNum, m_strTime);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSaveVersionDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	// set data into sqlite
	if (NULL == m_pPrjSql)
	{
		return;
	}

	UpdateData(TRUE);
	if (m_pPrjSql->InsertIntoOneVersion(m_nNewVerNum, m_strSaveUser, m_strTime, m_strSaveLog))
	{
		WritePrivateProfileString(_T("BeopFactory"), _T("SaveUser"), m_strSaveUser, m_szCfgPath);
	}

	CSkinDlg::OnOK();
}
