// PageAreaShow.cpp : implementation file
//

#include "stdafx.h"
#include "PageAreaShow.h"
#include "afxdialogex.h"
#include "AreaCfgDlg.h"
#include "SheetDeviceArea.h"


// CPageAreaShow dialog

IMPLEMENT_DYNAMIC(CPageAreaShow, CPropertyPage)

CPageAreaShow::CPageAreaShow()
	: CPropertyPage(CPageAreaShow::IDD)
{
}

CPageAreaShow::~CPageAreaShow()
{
}

void CPageAreaShow::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REGION, m_listRegion);
}


BEGIN_MESSAGE_MAP(CPageAreaShow, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_ADD, &CPageAreaShow::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CPageAreaShow::OnBnClickedBtnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_REGION, &CPageAreaShow::OnNMDblclkListRegion)
END_MESSAGE_MAP()


// CPageAreaShow message handlers


BOOL CPageAreaShow::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_pSheet = dynamic_cast<CSheetDeviceArea*>(GetParent());
	ASSERT(m_pSheet != NULL);
	if (NULL == m_pSheet)
	{
		return FALSE;
	}

	InitControls();
	RefreshList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPageAreaShow::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_pSheet->UpdateDbData();

	int nIdx = m_listRegion.GetSelectionMark();
	if (-1 != nIdx)
	{
		CString	strSelName(m_listRegion.GetItemText(nIdx, 2));
		m_pSheet->SetAreaSelectName(strSelName);
	}

	CPropertyPage::OnOK();
}


void CPageAreaShow::OnBnClickedBtnAdd()
{
	// TODO: Add your control notification handler code here
	CAreaCfgDlg dlg(_T(""), _T(""));
	if (IDOK == dlg.DoModal())
	{
		TextAreaInfo info;
		info.nPageId = dlg.m_nPageId;
		info.strPageName = dlg.m_strPageName;
		info.strRegion = dlg.m_strRegion;
		m_pSheet->AppendAreaInfo(info);

		int nIndex = m_listRegion.GetItemCount();
		CString	strIndex;
		strIndex.Format(_T("%d"), nIndex + 1);
		m_listRegion.InsertItem(nIndex, strIndex);
		m_listRegion.SetItemText(nIndex, 1, info.strPageName);
		m_listRegion.SetItemText(nIndex, 2, info.strRegion);
	}
}


void CPageAreaShow::OnBnClickedBtnDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = NULL;
	vector<int> vecDel;
	int nItem = 0;

	pos = m_listRegion.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请选择所需删除项！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	while (pos != NULL)
	{
		nItem = m_listRegion.GetNextSelectedItem(pos);
		vecDel.push_back(nItem);
	}
	for (int i=vecDel.size()-1; i>=0; --i)
	{
		m_listRegion.DeleteItem(vecDel.at(i));
		m_pSheet->DeleteAreaInfo(vecDel.at(i));
	}
}


void CPageAreaShow::OnNMDblclkListRegion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	TextAreaInfo info;
	const int nCurSel = pNMItemActivate->iItem;
	try
	{
		info = (m_pSheet->GetAreaInfo()).at(nCurSel);
	}
	catch (...)
	{
		MessageBox(_T("选择项时数据异常！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	CAreaCfgDlg dlg(info.strPageName, info.strRegion);
	if (IDOK == dlg.DoModal())
	{
		TextAreaInfo inf;
		inf.nPageId = dlg.m_nPageId;
		inf.strPageName = dlg.m_strPageName;
		inf.strRegion = dlg.m_strRegion;
		m_pSheet->ModifyAreaInfo(nCurSel, inf);

		m_listRegion.SetItemText(nCurSel, 1, inf.strPageName);
		m_listRegion.SetItemText(nCurSel, 2, inf.strRegion);
	}
}

void CPageAreaShow::InitControls(void)
{
	m_listRegion.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listRegion.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 80);
	m_listRegion.InsertColumn(1, _T("页面"), LVCFMT_LEFT, 160);
	m_listRegion.InsertColumn(2, _T("区域"), LVCFMT_LEFT, 80);
	m_listRegion.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}

void CPageAreaShow::RefreshList(void)
{
	vector<TextAreaInfo> vec(m_pSheet->GetAreaInfo());
	vector<TextAreaInfo>::const_iterator iter;
	int i = 0;
	CString	strIndex;

	m_listRegion.DeleteAllItems();
	for (iter = vec.begin(); iter != vec.end(); iter++,i++)
	{
		strIndex.Format(_T("%d"), i + 1);
		m_listRegion.InsertItem(i, strIndex);
		m_listRegion.SetItemText(i, 1, iter->strPageName);
		m_listRegion.SetItemText(i, 2, iter->strRegion);
	}
}
