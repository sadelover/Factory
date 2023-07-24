// NavigateWindowSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NavigateWindowSettingDlg.h"
#include "afxdialogex.h"
#include "NavigateWindowItemSetDlg.h"


// CNavigateWindowSettingDlg dialog

IMPLEMENT_DYNAMIC(CNavigateWindowSettingDlg, CDialogEx)

CNavigateWindowSettingDlg::CNavigateWindowSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNavigateWindowSettingDlg::IDD, pParent)
	, m_nLayer(4)
{

}

CNavigateWindowSettingDlg::~CNavigateWindowSettingDlg()
{
}

void CNavigateWindowSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NAVI_SET, m_listNavigateSet);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
}


BEGIN_MESSAGE_MAP(CNavigateWindowSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD, &CNavigateWindowSettingDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CNavigateWindowSettingDlg::OnBnClickedBtnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_NAVI_SET, &CNavigateWindowSettingDlg::OnNMDblclkListNaviSet)
END_MESSAGE_MAP()


// CNavigateWindowSettingDlg message handlers


BOOL CNavigateWindowSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CNavigateWindowSettingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}


void CNavigateWindowSettingDlg::OnBnClickedBtnAdd()
{
	// TODO: Add your control notification handler code here
	CNavigateWindowItemSetDlg dlg(_T(""), _T(""), 0);
	if (IDOK == dlg.DoModal())
	{
		PageInfo pageInfo;
		pageInfo.strPageDesc = dlg.m_strDesc;
		pageInfo.strPageName = dlg.m_strPageName;
		pageInfo.nPageId = dlg.m_nPageId;
		m_vecPageInfo.push_back(pageInfo);

		int nCount = m_vecPageInfo.size() - 1;
		m_listNavigateSet.InsertItem(nCount, dlg.m_strDesc);
		m_listNavigateSet.SetItemText(nCount, 1, dlg.m_strPageName);

		CString strPageId;
		strPageId.Format(_T("%d"), dlg.m_nPageId);
		m_listNavigateSet.SetItemText(nCount, 2, strPageId);
	}
}


void CNavigateWindowSettingDlg::OnBnClickedBtnDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = NULL;
	vector<int> vecDel;
	int nItem = 0;
	vector<PageInfo>::const_iterator	iter;
	vector<int>::const_iterator			iterDel;

	pos = m_listNavigateSet.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请选择所需删除项！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	while (pos != NULL)
	{
		nItem = m_listNavigateSet.GetNextSelectedItem(pos);
		vecDel.push_back(nItem);
	}
	for (int i=vecDel.size()-1; i>=0; --i)
	{
		m_listNavigateSet.DeleteItem(vecDel.at(i));

		nItem = 0;
		for (iter=m_vecPageInfo.begin(); iter!=m_vecPageInfo.end(); ++iter,++nItem)
		{
			if (nItem == vecDel[i])
			{
				m_vecPageInfo.erase(iter);
				break;
			}
		}
	}
}


void CNavigateWindowSettingDlg::OnNMDblclkListNaviSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	PageInfo pageInfo;
	const int nCurSel = pNMItemActivate->iItem;
	try
	{
		pageInfo = m_vecPageInfo.at(nCurSel);
	}
	catch (...)
	{
		MessageBox(_T("选择项时数据异常！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	CString	strPageName = m_listNavigateSet.GetItemText(nCurSel, 1);
	CNavigateWindowItemSetDlg dlg(pageInfo.strPageDesc, strPageName, pageInfo.nPageId);
	if (IDOK == dlg.DoModal())
	{
		CString	strDesc(dlg.m_strDesc);
		CString	strPageName(dlg.m_strPageName);
		int		nPageId = dlg.m_nPageId;

		vector<PageInfo>::iterator	iter;
		int i = 0;
		for (iter=m_vecPageInfo.begin(); iter!=m_vecPageInfo.end(); ++iter,++i)
		{
			if (nCurSel == i)
			{
				iter->strPageDesc	= strDesc;
				iter->strPageName	= strPageName;
				iter->nPageId		= nPageId;

				m_listNavigateSet.SetItemText(i, 0, strDesc);
				m_listNavigateSet.SetItemText(i, 1, strPageName);

				CString	strPageId;
				strPageId.Format(_T("%d"), dlg.m_nPageId);
				m_listNavigateSet.SetItemText(i, 2, strPageId);

				break;
			}
		}
	}
}

void CNavigateWindowSettingDlg::InitControls(void)
{
	// combobox
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
	m_listNavigateSet.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listNavigateSet.InsertColumn(0, _T("页面描述"), LVCFMT_LEFT, 120);
	m_listNavigateSet.InsertColumn(1, _T("对应页面"), LVCFMT_LEFT, 120);
	m_listNavigateSet.InsertColumn(2, _T("对应页面ID"), LVCFMT_LEFT, 120);
}

void CNavigateWindowSettingDlg::InitData(void)
{
	m_comboLayer.SetCurSel(m_nLayer);

	vector<PageInfo>::const_iterator	iter;
	int i = 0;
	CString strPageId;
	for (iter=m_vecPageInfo.begin(); iter!=m_vecPageInfo.end(); ++iter,++i)
	{
		m_listNavigateSet.InsertItem(i, iter->strPageDesc);
		m_listNavigateSet.SetItemText(i, 1, iter->strPageName);

		strPageId.Format(_T("%d"), iter->nPageId);
		m_listNavigateSet.SetItemText(i, 2, strPageId);
	}
}

void CNavigateWindowSettingDlg::SetLayer(const int nLayer)
{
	m_nLayer = nLayer;
}

void CNavigateWindowSettingDlg::SetPageInfo(const vector<PageInfo>& vecPageInfo)
{
	m_vecPageInfo = vecPageInfo;
}

int CNavigateWindowSettingDlg::GetLayer(void)
{
	return m_nLayer;
}

vector<PageInfo> CNavigateWindowSettingDlg::GetPageInfo(void)
{
	return m_vecPageInfo;
}
