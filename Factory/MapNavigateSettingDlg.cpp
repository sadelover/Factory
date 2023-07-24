// MapNavigateSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MapNavigateSettingDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/EqmDrawDefines.h"
#include "MapNavigateItemSetDlg.h"


// CMapNavigateSettingDlg dialog

IMPLEMENT_DYNAMIC(CMapNavigateSettingDlg, CDialogEx)

CMapNavigateSettingDlg::CMapNavigateSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMapNavigateSettingDlg::IDD, pParent)
	, m_nLayer(4)
	, m_nScale(0)
{

}

CMapNavigateSettingDlg::~CMapNavigateSettingDlg()
{
}

void CMapNavigateSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
	DDX_Control(pDX, IDC_LIST_MAP_CFG, m_listMapCfg);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_nScale);
}


BEGIN_MESSAGE_MAP(CMapNavigateSettingDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAP_CFG, &CMapNavigateSettingDlg::OnNMDblclkListMapCfg)
	ON_BN_CLICKED(IDC_BTN_ADD, &CMapNavigateSettingDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CMapNavigateSettingDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDOK, &CMapNavigateSettingDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT2, &CMapNavigateSettingDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CMapNavigateSettingDlg message handlers


BOOL CMapNavigateSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();
	InitData();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CMapNavigateSettingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CDialogEx::OnOK();
}

void CMapNavigateSettingDlg::SetLayer(const int nLayer)
{
	m_nLayer = nLayer;
}

void CMapNavigateSettingDlg::SetScaleRuler(const int nScale)
{
	m_nScale = nScale;
}

void CMapNavigateSettingDlg::SetMapPageInfo(const vector<CMapPageInfo>& vecMapPageInfo)
{
	m_vecMapPageInfo = vecMapPageInfo;
}

int CMapNavigateSettingDlg::GetPicId(void) const
{
	return g_nPicIndexMapNavigate;
}

int CMapNavigateSettingDlg::GetLayer(void) const
{
	return m_nLayer;
}

int CMapNavigateSettingDlg::GetScaleRuler(void) const
{
	return m_nScale;
}

vector<CMapPageInfo> CMapNavigateSettingDlg::GetMapPageInfo(void) const
{
	return m_vecMapPageInfo;
}

void CMapNavigateSettingDlg::InitControls(void)
{
	// combo box
	
	m_comboLayer.AddString(_T("1层"));
	m_comboLayer.AddString(_T("2层"));
	m_comboLayer.AddString(_T("3层"));
	m_comboLayer.AddString(_T("4层"));
	m_comboLayer.AddString(_T("5层"));
	m_comboLayer.AddString(_T("6层"));
	m_comboLayer.AddString(_T("7层"));
	m_comboLayer.AddString(_T("8层"));
	m_comboLayer.AddString(_T("9层"));
	m_comboLayer.AddString(_T("10层"));


	// list
	m_listMapCfg.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listMapCfg.InsertColumn(0, _T("经度1"), LVCFMT_LEFT, 80);
	m_listMapCfg.InsertColumn(1, _T("纬度1"), LVCFMT_LEFT, 80);
	m_listMapCfg.InsertColumn(2, _T("经度2"), LVCFMT_LEFT, 80);
	m_listMapCfg.InsertColumn(3, _T("纬度2"), LVCFMT_LEFT, 80);
	m_listMapCfg.InsertColumn(4, _T("页面号"), LVCFMT_LEFT, 80);
	
}

void CMapNavigateSettingDlg::InitData(void)
{
	// list
	vector<CMapPageInfo>::const_iterator	iter;
	int i = 0;
	CString strPageId;
	for (iter=m_vecMapPageInfo.begin(); iter!=m_vecMapPageInfo.end(); ++iter,++i)
	{
		AddOneToList(i, *iter);
	}
}

void CMapNavigateSettingDlg::OnNMDblclkListMapCfg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	CMapPageInfo mapPageInfo;
	try
	{
		mapPageInfo = m_vecMapPageInfo.at(pNMItemActivate->iItem);
	}
	catch (...)
	{
		MessageBox(_T("选择项时数据异常！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}
	
	OpenSettingDlg(mapPageInfo, pNMItemActivate->iItem);
}

void CMapNavigateSettingDlg::OpenSettingDlg(const CMapPageInfo& mapPageInfo, const int nSelCount)
{
	CMapNavigateItemSetDlg dlg(mapPageInfo);
	if (IDOK == dlg.DoModal())
	{
		CMapPageInfo info(dlg.GetMapPageInfo());

		vector<CMapPageInfo>::iterator	iter;
		int	i = 0;
		for (iter=m_vecMapPageInfo.begin(); iter!=m_vecMapPageInfo.end(); ++iter,++i)
		{
			if (nSelCount == i)
			{
				*iter = info;
				break;
			}
		}

		// update list control
		CString	strLongit1;
		CString	strLatit1;
		CString	strLongit2;
		CString	strLatit2;

		strLongit1.Format(_T("%0.6f"), info.GetLongitudeValue(1));
		strLongit2.Format(_T("%0.6f"), info.GetLongitudeValue(2));
		strLatit1.Format(_T("%0.6f"), info.GetLatitudeValue(1));
		strLatit2.Format(_T("%0.6f"), info.GetLatitudeValue(2));

		m_listMapCfg.SetItemText(i, 0, strLongit1);
		m_listMapCfg.SetItemText(i, 1, strLatit1);
		m_listMapCfg.SetItemText(i, 2, strLongit2);
		m_listMapCfg.SetItemText(i, 3, strLatit2);

		CString strPageId;
		strPageId.Format(_T("%d"), info.GetPageId());
		m_listMapCfg.SetItemText(i, 4, strPageId);
	}
}

void CMapNavigateSettingDlg::OnBnClickedBtnAdd()
{
	// TODO: Add your control notification handler code here
	//CCoorInfo longitInf;
	//CCoorInfo latitInf;
	//longitInf.SetCoordinate(east, 0);
	//latitInf.SetCoordinate(north, 0);
	//AddOneItem(longitInf, latitInf, longitInf, latitInf, 0);

	CMapPageInfo	item;
	item.SetLongitudeValue(1, 0);
	item.SetLongitudeValue(2, 0);
	item.SetLatitudeValue(1, 0);
	item.SetLatitudeValue(2, 0);
	item.SetPageId(0);
	m_vecMapPageInfo.push_back(item);

	int nCount = m_vecMapPageInfo.size() - 1;
	m_listMapCfg.InsertItem(nCount, _T("0.0000"));
	m_listMapCfg.SetItemText(nCount, 1, _T("0.0000"));
	m_listMapCfg.SetItemText(nCount, 2, _T("0.0000"));
	m_listMapCfg.SetItemText(nCount, 3, _T("0.0000"));
	m_listMapCfg.SetItemText(nCount, 4, _T("0"));
}


void CMapNavigateSettingDlg::OnBnClickedBtnDel()
{
	// TODO: Add your control notification handler code here
	DeleteItems();
}

//void CMapNavigateSettingDlg::AddOneItem(const CCoorInfo longitude1, const CCoorInfo latitdue1, const CCoorInfo longitude2, const CCoorInfo latitdue2, const int nPageId)
//{
//	CMapPageInfo item;
//	item.SetLongitude(1, longitude1);
//	item.SetLatitude(1, latitdue1);
//	item.SetLongitude(2, longitude2);
//	item.SetLatitude(2, latitdue2);
//	item.SetPageId(nPageId);
//	m_vecMapPageInfo.push_back(item);
//
//	int nCount = m_vecMapPageInfo.size() - 1;
//	AddOneToList(nCount, item);
//}

void CMapNavigateSettingDlg::DeleteItems(void)
{
	POSITION pos = NULL;
	vector<int> vecDel;
	int nItem = 0;
	vector<CMapPageInfo>::const_iterator	iter;
	vector<int>::const_iterator				iterDel;

	pos = m_listMapCfg.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请选择所需删除项！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	while (pos != NULL)
	{
		nItem = m_listMapCfg.GetNextSelectedItem(pos);
		vecDel.push_back(nItem);
	}
	for (int i=vecDel.size()-1; i>=0; --i)
	{
		m_listMapCfg.DeleteItem(vecDel.at(i));

		nItem = 0;
		for (iter=m_vecMapPageInfo.begin(); iter!=m_vecMapPageInfo.end(); ++iter,++nItem)
		{
			if (nItem == vecDel[i])
			{
				m_vecMapPageInfo.erase(iter);
				break;
			}
		}
	}
}

void CMapNavigateSettingDlg::AddOneToList(const int nCount, const CMapPageInfo& mapPageInf)
{
	//m_listMapCfg.InsertItem(nCount, mapPageInf.GetLongitudeName(1));
	//m_listMapCfg.SetItemText(nCount, 1, mapPageInf.GetLatitudeName(1));
	//m_listMapCfg.SetItemText(nCount, 2, mapPageInf.GetLongitudeName(2));
	//m_listMapCfg.SetItemText(nCount, 3, mapPageInf.GetLatitudeName(2));

	CString	strLongit1;
	CString	strLongit2;
	CString	strLatit1;
	CString	strLatit2;

	strLongit1.Format(_T("%0.6f"), mapPageInf.GetLongitudeValue(1));
	strLongit2.Format(_T("%0.6f"), mapPageInf.GetLongitudeValue(2));
	strLatit1.Format(_T("%0.6f"), mapPageInf.GetLatitudeValue(1));
	strLatit2.Format(_T("%0.6f"), mapPageInf.GetLatitudeValue(2));

	m_listMapCfg.InsertItem(nCount, strLongit1);
	m_listMapCfg.SetItemText(nCount, 1, strLatit1);
	m_listMapCfg.SetItemText(nCount, 2, strLongit2);
	m_listMapCfg.SetItemText(nCount, 3, strLatit2);

	CString strPageId;
	strPageId.Format(_T("%d"), mapPageInf.GetPageId());
	m_listMapCfg.SetItemText(nCount, 4, strPageId);
}


void CMapNavigateSettingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CMapNavigateSettingDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
