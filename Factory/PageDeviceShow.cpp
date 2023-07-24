// PageDeviceShow.cpp : implementation file
//

#include "stdafx.h"
#include "PageDeviceShow.h"
#include "afxdialogex.h"
#include "DeviceCfgDlg.h"
#include "SheetDeviceArea.h"


// CPageDeviceShow dialog

IMPLEMENT_DYNAMIC(CPageDeviceShow, CPropertyPage)

CPageDeviceShow::CPageDeviceShow()
	: CPropertyPage(CPageDeviceShow::IDD)
	, m_pSheet(NULL)
{
}

CPageDeviceShow::~CPageDeviceShow()
{
}

void CPageDeviceShow::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_listDevice);
}


BEGIN_MESSAGE_MAP(CPageDeviceShow, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_ADD, &CPageDeviceShow::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CPageDeviceShow::OnBnClickedBtnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DEVICE, &CPageDeviceShow::OnNMDblclkListDevice)
END_MESSAGE_MAP()


// CPageDeviceShow message handlers


BOOL CPageDeviceShow::OnInitDialog()
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


void CPageDeviceShow::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_pSheet->UpdateDbData();

	int nIdx = m_listDevice.GetSelectionMark();
	if (-1 != nIdx)
	{
		CString	strSelName(m_listDevice.GetItemText(nIdx, 2));
		m_pSheet->SetDeviceSelectName(strSelName);
	}

	CPropertyPage::OnOK();
}


void CPageDeviceShow::OnBnClickedBtnAdd()
{
	// TODO: Add your control notification handler code here
	CDeviceCfgDlg dlg(_T(""), _T(""), this);
	if (IDOK == dlg.DoModal())
	{
		TextDeviceInfo info;
		info.strRegion = dlg.m_strRegion;
		info.nBindPageId = dlg.m_nBindPageId;
		info.strBindPageName = dlg.m_strBindPageName;
		m_pSheet->AppendDeviceInfo(info);

		int nIndex = m_listDevice.GetItemCount();
		CString	strIndex;
		strIndex.Format(_T("%d"), nIndex + 1);
		m_listDevice.InsertItem(nIndex, strIndex);
		m_listDevice.SetItemText(nIndex, 1, info.strRegion);
		m_listDevice.SetItemText(nIndex, 2, info.strBindPageName);
	}
}


void CPageDeviceShow::OnBnClickedBtnDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = NULL;
	vector<int> vecDel;
	int nItem = 0;

	pos = m_listDevice.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请选择所需删除项！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	while (pos != NULL)
	{
		nItem = m_listDevice.GetNextSelectedItem(pos);
		vecDel.push_back(nItem);
	}
	for (int i=vecDel.size()-1; i>=0; --i)
	{
		m_listDevice.DeleteItem(vecDel.at(i));
		m_pSheet->DeleteDeviceInfo(vecDel.at(i));
	}
}

void CPageDeviceShow::OnNMDblclkListDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	TextDeviceInfo info;
	const int nCurSel = pNMItemActivate->iItem;
	try
	{
		info = (m_pSheet->GetDeviceInfo()).at(nCurSel);
	}
	catch (...)
	{
		MessageBox(_T("选择项时数据异常！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	CDeviceCfgDlg dlg(info.strRegion, info.strBindPageName, this);
	if (IDOK == dlg.DoModal())
	{
		TextDeviceInfo inf;
		inf.strRegion = dlg.m_strRegion;
		inf.nBindPageId = dlg.m_nBindPageId;
		inf.strBindPageName = dlg.m_strBindPageName;
		m_pSheet->ModifyDeviceInfo(nCurSel, inf);

		m_listDevice.SetItemText(nCurSel, 1, inf.strRegion);
		m_listDevice.SetItemText(nCurSel, 2, inf.strBindPageName);
	}
}

void CPageDeviceShow::InitControls(void)
{
	m_listDevice.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listDevice.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 80);
	m_listDevice.InsertColumn(1, _T("区域"), LVCFMT_LEFT, 160);
	m_listDevice.InsertColumn(2, _T("设备"), LVCFMT_LEFT, 80);
	m_listDevice.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}

void CPageDeviceShow::RefreshList(void)
{
	vector<TextDeviceInfo> vec(m_pSheet->GetDeviceInfo());
	vector<TextDeviceInfo>::const_iterator iter;
	int i = 0;
	CString	strIndex;

	m_listDevice.DeleteAllItems();
	for (iter = vec.begin(); iter != vec.end(); iter++,i++)
	{
		strIndex.Format(_T("%d"), i + 1);
		m_listDevice.InsertItem(i, strIndex);
		m_listDevice.SetItemText(i, 1, iter->strRegion);
		m_listDevice.SetItemText(i, 2, iter->strBindPageName);
	}
}
