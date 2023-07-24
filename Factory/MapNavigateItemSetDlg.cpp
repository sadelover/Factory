// MapNavigateItemSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MapNavigateItemSetDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CMapNavigateItemSetDlg dialog

IMPLEMENT_DYNAMIC(CMapNavigateItemSetDlg, CSkinDlg)

CMapNavigateItemSetDlg::CMapNavigateItemSetDlg(const CMapPageInfo& mapPageInf, CWnd* pParent /*=NULL*/)
	: CSkinDlg(CMapNavigateItemSetDlg::IDD, pParent)
	, m_mapPageInf(mapPageInf)
	, m_nLongit1(0)
	, m_nLongit2(0)
	, m_nLatit1(0)
	, m_nLatit2(0)
	, m_strLongit1(_T(""))
	, m_strLongit2(_T(""))
	, m_strLatit1(_T(""))
	, m_strLatit2(_T(""))
	, m_nPageId(0)
	, m_pMainFrm(NULL)
{

}

CMapNavigateItemSetDlg::~CMapNavigateItemSetDlg()
{
}

void CMapNavigateItemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LONGIT1, m_comboLongit1);
	DDX_Control(pDX, IDC_COMBO_LONGIT2, m_comboLongit2);
	DDX_Control(pDX, IDC_COMBO_LATIT1, m_comboLatit1);
	DDX_Control(pDX, IDC_COMBO_LATIT2, m_comboLatit2);
	DDX_CBIndex(pDX, IDC_COMBO_LONGIT1, m_nLongit1);
	DDX_CBIndex(pDX, IDC_COMBO_LONGIT2, m_nLongit2);
	DDX_CBIndex(pDX, IDC_COMBO_LATIT1, m_nLatit1);
	DDX_CBIndex(pDX, IDC_COMBO_LATIT2, m_nLatit2);
	DDX_Text(pDX, IDC_EDIT_LONGIT1, m_strLongit1);
	DDX_Text(pDX, IDC_EDIT_LONGIT2, m_strLongit2);
	DDX_Text(pDX, IDC_EDIT_LATIT1, m_strLatit1);
	DDX_Text(pDX, IDC_EDIT_LATIT2, m_strLatit2);
	DDX_Text(pDX, IDC_EDIT_PAGE_ID, m_nPageId);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_PAGE, m_comboPage);
}


BEGIN_MESSAGE_MAP(CMapNavigateItemSetDlg, CSkinDlg)
END_MESSAGE_MAP()


// CMapNavigateItemSetDlg message handlers


BOOL CMapNavigateItemSetDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();

	InitData();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CMapNavigateItemSetDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	//CCoorInfo corInf;

	//corInf.SetCoordinate((FaceType)m_nLongit1, _ttol(m_strLongit1));
	//m_mapPageInf.SetLongitude(1, corInf);

	//corInf.SetCoordinate((FaceType)m_nLongit2, _ttol(m_strLongit2));
	//m_mapPageInf.SetLongitude(2, corInf);

	//corInf.SetCoordinate((FaceType)(m_nLatit1 + 2), _ttol(m_strLatit1));
	//m_mapPageInf.SetLatitude(1, corInf);

	//corInf.SetCoordinate((FaceType)(m_nLatit2 + 2), _ttol(m_strLatit2));
	//m_mapPageInf.SetLatitude(2, corInf);

	double dLongit1 = _ttof(m_strLongit1);
	double dLongit2 = _ttof(m_strLongit2);
	double dLatit1 = _ttof(m_strLatit1);
	double dLatit2 = _ttof(m_strLatit2);

	if (dLongit1 < -180 || dLongit1 > 180)
	{
		MessageBox(_T("经度设置超出范围！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}
	if (dLongit2 < -180 || dLongit2 > 180)
	{
		MessageBox(_T("经度设置超出范围！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}
	if (dLatit1 < -90 || dLatit1 > 90)
	{
		MessageBox(_T("纬度设置超出范围！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}
	if (dLatit2 < -90 || dLatit2 > 90)
	{
		MessageBox(_T("纬度设置超出范围！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	m_mapPageInf.SetLongitudeValue(1, dLongit1);
	m_mapPageInf.SetLongitudeValue(2, dLongit2);
	m_mapPageInf.SetLatitudeValue(1, dLatit1);
	m_mapPageInf.SetLatitudeValue(2, dLatit2);


	CString	strPageName;
	int	nCurSel = m_comboPage.GetCurSel();
	m_comboPage.GetLBText(nCurSel, strPageName);

	vector<ProjPage>::const_iterator	iter;
	for (iter=m_vecProjPage.begin(); iter!=m_vecProjPage.end(); ++iter)
	{
		if (iter->strPageName == strPageName)
		{
			m_mapPageInf.SetPageId(iter->nPageId);
			break;
		}
	}


	CSkinDlg::OnOK();
}

CMapPageInfo CMapNavigateItemSetDlg::GetMapPageInfo(void) const
{
	return m_mapPageInf;
}

void CMapNavigateItemSetDlg::InitControls(void)
{
	//m_comboLongit1.AddString(_T("东经"));
	//m_comboLongit1.AddString(_T("西经"));

	//m_comboLongit2.AddString(_T("东经"));
	//m_comboLongit2.AddString(_T("西经"));

	//m_comboLatit1.AddString(_T("南纬"));
	//m_comboLatit1.AddString(_T("北纬"));

	//m_comboLatit2.AddString(_T("南纬"));
	//m_comboLatit2.AddString(_T("北纬"));

	//
	m_pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(m_pMainFrm != NULL);
	if (NULL == m_pMainFrm)
	{
		return;
	}

	CEqmDrawProject* pProject = m_pMainFrm->m_DrawContext.GetProject();
	ASSERT(pProject != NULL);
	if (NULL == pProject)
	{
		return;
	}

	vector<CEqmDrawPage*> pageList = pProject->GetPageList();
	vector<CEqmDrawPage*>::const_iterator	iter;

	m_vecProjPage.clear();
	for (iter=pageList.begin(); iter!=pageList.end(); ++iter)
	{
		if ((*iter) != NULL)
		{
			ProjPage	projPage;
			projPage.nPageId = (*iter)->GetID();
			projPage.strPageName = (*iter)->GetPageName().c_str();
			m_vecProjPage.push_back(projPage);
		}
	}


	m_comboPage.ResetContent();
	vector<ProjPage>::const_iterator	it;
	for (it=m_vecProjPage.begin(); it!=m_vecProjPage.end(); ++it)
	{
		m_comboPage.AddString(it->strPageName);
	}
	if (m_comboPage.GetCount() > 0)
	{
		const int nPageId = m_mapPageInf.GetPageId();
		int i = 0;
		for (it=m_vecProjPage.begin(); it!=m_vecProjPage.end(); ++it,++i)
		{
			if (it->nPageId == nPageId)
			{
				m_comboPage.SetCurSel(i);
				break;
			}
		}
	}
}

void CMapNavigateItemSetDlg::InitData(void)
{
	//CCoorInfo corInf;

	//corInf = m_mapPageInf.GetLongitude(1);
	//m_nLongit1 = (int)corInf.GetFaceType();
	//m_strLongit1.Format(_T("%0.4f"), corInf.GetValue());

	//corInf = m_mapPageInf.GetLongitude(2);
	//m_nLongit2 = (int)corInf.GetFaceType();
	//m_strLongit2.Format(_T("%0.4f"), corInf.GetValue());

	//corInf = m_mapPageInf.GetLatitude(1);
	//m_nLatit1 = (int)corInf.GetFaceType() - 2;
	//m_strLatit1.Format(_T("%0.4f"), corInf.GetValue());

	//corInf = m_mapPageInf.GetLatitude(2);
	//m_nLatit2 = (int)corInf.GetFaceType() -2;
	//m_strLatit2.Format(_T("%0.4f"), corInf.GetValue());

	m_strLongit1.Format(_T("%0.6f"), m_mapPageInf.GetLongitudeValue(1));
	m_strLongit2.Format(_T("%0.6f"), m_mapPageInf.GetLongitudeValue(2));
	m_strLatit1.Format(_T("%0.6f"), m_mapPageInf.GetLatitudeValue(1));
	m_strLatit2.Format(_T("%0.6f"), m_mapPageInf.GetLatitudeValue(2));
	m_nPageId = m_mapPageInf.GetPageId();

	UpdateData(FALSE);
}
