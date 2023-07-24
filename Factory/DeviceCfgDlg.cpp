// DeviceCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DeviceCfgDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "PageDeviceShow.h"
#include "SheetDeviceArea.h"


// CDeviceCfgDlg dialog

IMPLEMENT_DYNAMIC(CDeviceCfgDlg, CDialogEx)

CDeviceCfgDlg::CDeviceCfgDlg(const CString strRegion, const CString strBindPageName, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceCfgDlg::IDD, pParent)
	, m_strRegion(strRegion)
	, m_strBindPageName(strBindPageName)
	, m_nBindPageId(0)
	, m_pPageDevice(NULL)
{
	if (pParent != NULL)
	{
		m_pPageDevice = dynamic_cast<CPageDeviceShow*>(pParent);
		ASSERT(m_pPageDevice != NULL);
	}
}

CDeviceCfgDlg::~CDeviceCfgDlg()
{
}

void CDeviceCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_AREA, m_comboRegion);
	DDX_CBString(pDX, IDC_COMBO_AREA, m_strRegion);
	DDX_Control(pDX, IDC_COMBO_BIND_PAGE, m_comboBindPage);
	DDX_CBString(pDX, IDC_COMBO_BIND_PAGE, m_strBindPageName);
}


BEGIN_MESSAGE_MAP(CDeviceCfgDlg, CDialogEx)
END_MESSAGE_MAP()


// CDeviceCfgDlg message handlers


BOOL CDeviceCfgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDeviceCfgDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	if (_T("") == m_strRegion)
	{
		MessageBox(_T("«Î—°‘Ò«¯”Ú£°"), _T("¥ÌŒÛ"), MB_OK|MB_ICONERROR);
		return;
	}

	map<int, CString>::const_iterator iter;
	for (iter=m_mapPageInfo.begin(); iter!=m_mapPageInfo.end(); iter++)
	{
		if (iter->second == m_strBindPageName)
		{
			m_nBindPageId = iter->first;
			break;
		}
	}

	CDialogEx::OnOK();
}

void CDeviceCfgDlg::InitControls(void)
{
	ASSERT(m_pPageDevice != NULL);
	if (m_pPageDevice != NULL)
	{
		vector<TextAreaInfo> vecArea(m_pPageDevice->m_pSheet->GetAreaInfo());
		vector<TextAreaInfo>::const_iterator iter;
		m_comboRegion.ResetContent();
		for (iter=vecArea.begin(); iter!=vecArea.end(); iter++)
		{
			m_comboRegion.AddString(iter->strRegion);
		}
	}

	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}

	CEqmDrawProject* pProject = pMainFrm->m_DrawContext.GetProject();
	ASSERT(pProject != NULL);
	if (NULL == pProject)
	{
		return;
	}

	vector<CEqmDrawPage*> pageList = pProject->GetPageList();
	vector<CEqmDrawPage*>::const_iterator iter;
	m_mapPageInfo.clear();
	m_comboBindPage.ResetContent();
	for (iter = pageList.begin(); iter != pageList.end(); iter++)
	{
		int nPageId = (*iter)->GetID();
		CString strPageName;
		strPageName.Format(_T("%s"), (*iter)->GetPageName().c_str());
		m_mapPageInfo[nPageId] = strPageName;

		m_comboBindPage.AddString(strPageName);
	}

	UpdateData(FALSE);
}