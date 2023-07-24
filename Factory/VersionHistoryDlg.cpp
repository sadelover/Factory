// VersionHistoryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VersionHistoryDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "../ComponentDraw/ProjectSqlite.h"


// CVersionHistoryDlg �Ի���

IMPLEMENT_DYNAMIC(CVersionHistoryDlg, CDialogEx)

CVersionHistoryDlg::CVersionHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVersionHistoryDlg::IDD, pParent)
	, m_pMainFrm(NULL)
	, m_pPrjSql(NULL)
{

}

CVersionHistoryDlg::~CVersionHistoryDlg()
{
}

void CVersionHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VER_HIST, m_listVerHist);
}


BEGIN_MESSAGE_MAP(CVersionHistoryDlg, CDialogEx)
END_MESSAGE_MAP()


// CVersionHistoryDlg ��Ϣ�������


BOOL CVersionHistoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(m_pMainFrm != NULL);
	if (m_pMainFrm != NULL)
	{
		m_pPrjSql = m_pMainFrm->m_pPrjSqlite;
		ASSERT(m_pPrjSql != NULL);
	}

	InitControl();

	RefreshListControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CVersionHistoryDlg::InitControl(void)
{
	m_listVerHist.InsertColumn(0, _T("�汾��"), LVCFMT_LEFT, 60);
	m_listVerHist.InsertColumn(1, _T("�û�"), LVCFMT_LEFT, 80);
	m_listVerHist.InsertColumn(2, _T("ʱ��"), LVCFMT_LEFT, 100);
	m_listVerHist.InsertColumn(3, _T("��־"), LVCFMT_LEFT, 100);
	m_listVerHist.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	m_listVerHist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
}

void CVersionHistoryDlg::RefreshListControl(void)
{
	if (NULL == m_pPrjSql)
	{
		return;
	}

	vector<VersionInfo> vecVerInfo;
	vector<VersionInfo>::const_iterator	iter;
	int nCount = 0;
	CString strVer;
	CString strTmFmt;

	m_listVerHist.DeleteAllItems();
	vecVerInfo = m_pPrjSql->LoadVersionInfo();
	for (iter=vecVerInfo.begin(); iter!=vecVerInfo.end(); iter++)
	{
		strVer.Format(_T("%d"), iter->nVersion);
		strTmFmt = SetTimeShowFmt(iter->strTime);

		m_listVerHist.InsertItem(nCount, strVer);
		m_listVerHist.SetItemText(nCount, 1, iter->strUser);
		m_listVerHist.SetItemText(nCount, 2, strTmFmt);
		m_listVerHist.SetItemText(nCount, 3, iter->strLog);
	}
}

CString CVersionHistoryDlg::SetTimeShowFmt(CString strSrc)
{
	CString strFlag(_T("_"));
	CString strRet;

	int nPos = strSrc.Find(strFlag);
	CString strDate = strSrc.Left(nPos);
	CString strMin = strSrc.Mid(nPos+1, 2);
	CString strSec = strSrc.Mid(nPos+3, 2);
	strRet.Format(_T("%s %s:%s"), strDate, strMin, strSec);
	return strRet;
}
