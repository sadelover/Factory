// HistoryTrendSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HistoryTrendSettingDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "HistoryTrendItemSetDlg.h"
#include "MainFrm.h"


const int		g_nImageTypeLen = 2;
const CString	g_strImageTypeName[g_nImageTypeLen] = {_T("曲线"), _T("柱")};


// CHistoryTrendSettingDlg dialog

IMPLEMENT_DYNAMIC(CHistoryTrendSettingDlg, CDialogEx)

CHistoryTrendSettingDlg::CHistoryTrendSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoryTrendSettingDlg::IDD, pParent)
	, m_pMainFrame(NULL)
	, m_nColorIndex(0)
	, m_nLayer(4)
	, m_nImageType(0)
{

}

CHistoryTrendSettingDlg::~CHistoryTrendSettingDlg()
{
}

void CHistoryTrendSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_Control(pDX, IDC_COMBO_IMAGE_TYPE, m_comboImageType);
	DDX_Control(pDX, IDC_LIST_HISTORY_TREND, m_listHistoryTrend);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
	DDX_CBIndex(pDX, IDC_COMBO_IMAGE_TYPE, m_nImageType);
}


BEGIN_MESSAGE_MAP(CHistoryTrendSettingDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_HIS_ADD, &CHistoryTrendSettingDlg::OnBnClickedBtnHisAdd)
	ON_BN_CLICKED(IDC_BTN_HIS_DEL, &CHistoryTrendSettingDlg::OnBnClickedBtnHisDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_HISTORY_TREND, &CHistoryTrendSettingDlg::OnNMDblclkListHistoryTrend)
END_MESSAGE_MAP()


// CHistoryTrendSettingDlg message handlers


BOOL CHistoryTrendSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CHistoryTrendSettingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	//m_historyTrendInfo.nLayer = m_comboLayer.GetCurSel();
	//m_historyTrendInfo.imageType = (HistoryTrendType)m_comboImageType.GetCurSel();

	//COleDateTime tmStart;
	//COleDateTime tmEnd;
	//tmStart.SetDateTime(m_dateStart.GetYear(), m_dateStart.GetMonth(), m_dateStart.GetDay(), m_timeStart.GetHour(), m_timeStart.GetMinute(), m_timeStart.GetSecond());
	//tmEnd.SetDateTime(m_dateEnd.GetYear(), m_dateEnd.GetMonth(), m_dateEnd.GetDay(), m_timeEnd.GetHour(), m_timeEnd.GetMinute(), m_timeEnd.GetSecond());
	//m_historyTrendInfo.dateTimeStart = tmStart;
	//m_historyTrendInfo.dateTimeEnd = tmEnd;

	//m_historyTrendInfo.strBindPointName = m_strBindPointName;
	//m_historyTrendInfo.strRemarks = m_strRemarks;
	//m_historyTrendInfo.colorImage = m_colorImage;
	//m_historyTrendInfo.nLineWidth = m_nLineWidth;
	//m_historyTrendInfo.bIsCalcRange = m_bIsAutoCalcRange ? true : false;
	//m_historyTrendInfo.nUpdateCycle = m_nUpdateCycle;
	//m_historyTrendInfo.fScale = m_fScale;


	CDialogEx::OnOK();
}

void CHistoryTrendSettingDlg::InitControls(void)
{
	// layer
	CString strLayerName;
	for (int i=1; i<=10; ++i)
	{
		strLayerName.Format(_T("%d层"), i);
		m_comboLayer.AddString(strLayerName);
	}

	// image type
	for (int i=0; i<g_nImageTypeLen; ++i)
	{
		m_comboImageType.AddString(g_strImageTypeName[i]);
	}

	// list
	m_listHistoryTrend.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listHistoryTrend.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 80);
	m_listHistoryTrend.InsertColumn(1, _T("颜色"), LVCFMT_LEFT, 80);
	m_listHistoryTrend.InsertColumn(2, _T("绑定点"), LVCFMT_LEFT, 80);
	m_listHistoryTrend.InsertColumn(3, _T("更新周期"), LVCFMT_LEFT, 120);
}

void CHistoryTrendSettingDlg::InitData(void)
{
	m_pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	ASSERT(m_pMainFrame != NULL);
	if (NULL == m_pMainFrame)
	{
		MessageBox(_T("获取MainFrame指针失败！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	// combo box
	// no need to do

	// list
	m_listHistoryTrend.DeleteAllItems();

	vector<HistoryTrendInfo>::const_iterator	iter;
	int i = 0;
	CString	strVal;
	for (iter=m_vecHistoryTrendInfo.begin(); iter!=m_vecHistoryTrendInfo.end(); ++iter,++i)
	{
		strVal.Format(_T("%d"), (*iter).nCount);
		m_listHistoryTrend.InsertItem(i, strVal);

		m_listHistoryTrend.SetItemBkColor(i, 1, (*iter).colorImage);
		m_listHistoryTrend.SetItemText(i, 2, (*iter).strBindPointName);

		strVal.Format(_T("%d"), (*iter).nUpdateCycle);
		m_listHistoryTrend.SetItemText(i, 3, strVal);
	}

	UpdateData(FALSE);
}

int CHistoryTrendSettingDlg::GetPicId(void) const
{
	return g_nPicIndexHistoryTrend;
}


void CHistoryTrendSettingDlg::OnBnClickedBtnHisAdd()
{
	// TODO: Add your control notification handler code here
	COLORREF color = m_pMainFrame->m_colorArray[m_nColorIndex];
	++m_nColorIndex;
	if (m_nColorIndex >= 8)
	{
		m_nColorIndex = 0;
	}

	HistoryTrendInfo stHisTrendInfo;
	int i = m_vecHistoryTrendInfo.size();
	stHisTrendInfo.nCount = i + 1;
	stHisTrendInfo.colorImage = color;
	CString	strVal;

	strVal.Format(_T("%d"), stHisTrendInfo.nCount);
	m_listHistoryTrend.InsertItem(i, strVal);

	m_listHistoryTrend.SetItemBkColor(i, 1, color);
	m_listHistoryTrend.SetItemText(i, 2, _T(""));

	strVal.Format(_T("%d"), stHisTrendInfo.nUpdateCycle);
	m_listHistoryTrend.SetItemText(i, 3, strVal);


	COleDateTime time = COleDateTime::GetCurrentTime();
	time.SetDateTime(time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0);
	stHisTrendInfo.dateTimeStart = time;
	stHisTrendInfo.dateTimeEnd = time;

	m_vecHistoryTrendInfo.push_back(stHisTrendInfo);
}


void CHistoryTrendSettingDlg::OnBnClickedBtnHisDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = NULL;
	vector<int> vecDel;
	int nItem = 0;
	vector<HistoryTrendInfo>::const_iterator	iter;
	vector<int>::const_iterator					iterDel;

	pos = m_listHistoryTrend.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请选择所需删除项！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	while (pos != NULL)
	{
		nItem = m_listHistoryTrend.GetNextSelectedItem(pos);
		vecDel.push_back(nItem);
	}
	for (int i=vecDel.size()-1; i>=0; --i)
	{
		m_listHistoryTrend.DeleteItem(vecDel.at(i));

		nItem = 0;
		for (iter=m_vecHistoryTrendInfo.begin(); iter!=m_vecHistoryTrendInfo.end(); ++iter,++nItem)
		{
			if (nItem == vecDel[i])
			{
				m_vecHistoryTrendInfo.erase(iter);
				break;
			}
		}
	}
}

void CHistoryTrendSettingDlg::OnNMDblclkListHistoryTrend(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nType = m_comboImageType.GetCurSel();
	OpenSettingDlg(pNMItemActivate->iItem, (HistoryTrendType)nType);
}

void CHistoryTrendSettingDlg::OpenSettingDlg(const int nCurSel, const HistoryTrendType& /*hisTrendType*/)
{
	CHistoryTrendItemSetDlg dlg(&(m_pMainFrame->m_plcpointmap));
	HistoryTrendInfo stInfo;

	try
	{
		stInfo = m_vecHistoryTrendInfo.at(nCurSel);
	}
	catch (...)
	{
		MessageBox(_T("选择项时数据异常！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	// set before open dlg
	dlg.SetHistoryTrendInfo(stInfo);

	if (IDOK == dlg.DoModal())
	{
		HistoryTrendInfo inf = dlg.GetHistoryTrendInfo();

		vector<HistoryTrendInfo>::iterator	iter;
		for (iter=m_vecHistoryTrendInfo.begin(); iter!=m_vecHistoryTrendInfo.end(); ++iter)
		{
			if (inf.nCount == iter->nCount)
			{
				iter->dateTimeStart		= inf.dateTimeStart;
				iter->dateTimeEnd		= inf.dateTimeEnd;
				iter->strBindPointName	= inf.strBindPointName;
				iter->strRemarks		= inf.strRemarks;
				iter->colorImage		= inf.colorImage;
				iter->nLineWidth		= inf.nLineWidth;
				iter->bIsCalcRange		= inf.bIsCalcRange;
				iter->nUpdateCycle		= inf.nUpdateCycle;
				iter->fScale			= inf.fScale;
				break;
			}
		}

		CString	strVal;
		strVal.Format(_T("%d"), inf.nCount);
		m_listHistoryTrend.SetItemText(nCurSel, 0, strVal);
		m_listHistoryTrend.SetItemBkColor(nCurSel, 1, inf.colorImage);
		m_listHistoryTrend.SetItemText(nCurSel, 2, inf.strBindPointName);

		strVal.Format(_T("%d"), inf.nUpdateCycle);
		m_listHistoryTrend.SetItemText(nCurSel, 3, strVal);
	}
}

void CHistoryTrendSettingDlg::SetLayer(const int nLayer)
{
	m_nLayer = nLayer;
}

void CHistoryTrendSettingDlg::SetHistoryTrendType(const HistoryTrendType& hisTrendType)
{
	m_nImageType = (int)hisTrendType;
}

void CHistoryTrendSettingDlg::SetHistoryTrendVector(const vector<HistoryTrendInfo>& vecHisTrendInfo)
{
	m_vecHistoryTrendInfo = vecHisTrendInfo;
}

int CHistoryTrendSettingDlg::GetLayer(void) const
{
	return m_nLayer;
}

HistoryTrendType CHistoryTrendSettingDlg::GetHistoryTrendType(void) const
{
	return (HistoryTrendType)m_nImageType;
}

vector<HistoryTrendInfo> CHistoryTrendSettingDlg::GetHistoryTrendVector(void) const
{
	return m_vecHistoryTrendInfo;
}
