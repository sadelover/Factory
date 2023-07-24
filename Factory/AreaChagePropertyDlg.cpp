// AreaChagePropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AreaChagePropertyDlg.h"
#include "afxdialogex.h"


// CAreaChagePropertyDlg dialog

IMPLEMENT_DYNAMIC(CAreaChagePropertyDlg, CDialog)

CAreaChagePropertyDlg::CAreaChagePropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAreaChagePropertyDlg::IDD, pParent)
	, m_dXPos(0)
	, m_dYPos(0)
	, m_dWidth(0)
	, m_dHeight(0)
	, m_layer(E_LAYER_10)
{

}

CAreaChagePropertyDlg::~CAreaChagePropertyDlg()
{
}

void CAreaChagePropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AREAS, m_listAreaChange);
	DDX_Text(pDX, IDC_EDIT1, m_dXPos);
	DDX_Text(pDX, IDC_EDIT2, m_dYPos);
	DDX_Text(pDX, IDC_EDIT6, m_dWidth);
	DDX_Text(pDX, IDC_EDIT5, m_dHeight);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
}


BEGIN_MESSAGE_MAP(CAreaChagePropertyDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_AREA, &CAreaChagePropertyDlg::OnBnClickedButtonAddArea)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_AREA, &CAreaChagePropertyDlg::OnBnClickedButtonDeleteArea)
	ON_BN_CLICKED(IDOK, &CAreaChagePropertyDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AREAS, &CAreaChagePropertyDlg::OnNMDblclkListAreas)
END_MESSAGE_MAP()


// CAreaChagePropertyDlg message handlers


void CAreaChagePropertyDlg::OnBnClickedButtonAddArea()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CAddAreaDlg dlg;
	dlg.m_dWidth = m_dWidth;
	dlg.m_dHeight = m_dHeight;
	if(dlg.DoModal() == IDOK)
	{
		_tagAreaInfo area;
		area.strPointList = dlg.m_strPointList;
		area.BnmID		  = dlg.m_nBitMapID;
		area.nPageID	  = dlg.m_nLinkPageID;
		const int nCount = m_listAreaChange.GetItemCount();
		CString str;
		str.Format(_T("%d"),nCount);

		m_listAreaChange.InsertItem(nCount,str);
		
		str.Format(_T("%s"),area.strPointList);
		m_listAreaChange.SetItemText(nCount,1,str);

		str.Format(_T("%d"),area.BnmID);
		m_listAreaChange.SetItemText(nCount,2,str);
	}

}


void CAreaChagePropertyDlg::OnBnClickedButtonDeleteArea()
{
	// TODO: Add your control notification handler code here
	vector<int> vec;
	POSITION pos = m_listAreaChange.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("No items were selected!\n"));
		return;
	}
	while(pos)
	{
		int nItem = m_listAreaChange.GetNextSelectedItem(pos);
		vec.push_back(nItem);
	}
	for (int i=vec.size()-1;i>=0;--i)
	{
		m_listAreaChange.DeleteItem(vec[i]);
	}

	UpdateData(FALSE);
}

void CAreaChagePropertyDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_layer = m_comboLayer.GetCurSel();
	m_vecAreaInfo.clear();
	const int nCount = m_listAreaChange.GetItemCount();
	for (int i=0;i<nCount;++i)
	{
		_tagAreaInfo item;
		item.strPointList = m_listAreaChange.GetItemText(i,1);
		item.BnmID = _wtoi(m_listAreaChange.GetItemText(i,2));
		item.nPageID = _wtoi(m_listAreaChange.GetItemText(i,3));
		m_vecAreaInfo.push_back(item);
	}

	CDialog::OnOK();
}

BOOL CAreaChagePropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitList();
	InitComboLayer();
	UpdateList();
	UpdateData(FALSE);
	return TRUE;
}

void CAreaChagePropertyDlg::InitList()
{
	m_listAreaChange.SetBkColor(RGB(235, 235, 235));
	m_listAreaChange.SetTextColor(RGB(0, 0, 128));

	m_listAreaChange.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_listAreaChange.InsertColumn( 0, _T("ÐòºÅ"), LVCFMT_LEFT, 70 );
	m_listAreaChange.InsertColumn( 1, _T("ÇøÓòµã¼¯"), LVCFMT_LEFT, 130 );
	m_listAreaChange.InsertColumn( 2, _T("Í¼Æ¬ID"), LVCFMT_LEFT, 80 );
	m_listAreaChange.InsertColumn( 3, _T("Ò³ÃæID"), LVCFMT_LEFT, 50 );
}

void CAreaChagePropertyDlg::UpdateList()
{
	m_listAreaChange.DeleteAllItems();
	for(int i=0; i<m_vecAreaInfo.size(); i++)
	{
		_tagAreaInfo area = m_vecAreaInfo[i];

		CString str;
		str.Format(_T("%d"),i);
		m_listAreaChange.InsertItem(i,str);

		str.Format(_T("%s"),area.strPointList);
		m_listAreaChange.SetItemText(i,1,str);
		
		str.Format(_T("%d"),area.BnmID);
		m_listAreaChange.SetItemText(i,2,str);

		str.Format(_T("%d"),area.nPageID);
		m_listAreaChange.SetItemText(i,3,str);
	}
	UpdateData(FALSE);
}

void CAreaChagePropertyDlg::SetAreaInfoVec( vector<_tagAreaInfo> vecAreaInfo )
{
	m_vecAreaInfo = vecAreaInfo;
}

vector<_tagAreaInfo> CAreaChagePropertyDlg::GetAreaInfoVec()
{
	return m_vecAreaInfo;
}

void CAreaChagePropertyDlg::OnNMDblclkListAreas(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CAddAreaDlg dlg;
	dlg.m_dWidth = m_dWidth;
	dlg.m_dHeight = m_dHeight;
	dlg.m_strPointList = m_listAreaChange.GetItemText(pNMItemActivate->iItem, 1);
	CString strBitMapID	= m_listAreaChange.GetItemText(pNMItemActivate->iItem, 2);
	dlg.m_nBitMapID = _wtoi(strBitMapID);
	CString strPageID	= m_listAreaChange.GetItemText(pNMItemActivate->iItem, 3);
	dlg.m_nLinkPageID = _wtoi(strPageID);

	if (dlg.DoModal() == IDOK)
	{
		m_listAreaChange.SetItemText(pNMItemActivate->iItem, 1, dlg.m_strPointList);
		strBitMapID.Format(_T("%d"),dlg.m_nBitMapID);
		m_listAreaChange.SetItemText(pNMItemActivate->iItem,2, strBitMapID);

		strPageID.Format(_T("%d"),dlg.m_nLinkPageID);
		m_listAreaChange.SetItemText(pNMItemActivate->iItem,3, strPageID);
	}

	*pResult = 0;
}

void CAreaChagePropertyDlg::InitComboLayer()
{
	m_comboLayer.ResetContent();
	m_comboLayer.AddString(L"1²ã");
	m_comboLayer.AddString(L"2²ã");
	m_comboLayer.AddString(L"3²ã");
	m_comboLayer.AddString(L"4²ã");
	m_comboLayer.AddString(L"5²ã");
	m_comboLayer.AddString(L"6²ã");
	m_comboLayer.AddString(L"7²ã");
	m_comboLayer.AddString(L"8²ã");
	m_comboLayer.AddString(L"9²ã");
	m_comboLayer.AddString(L"10²ã");

	//UpdateData(false); //value to control
	m_comboLayer.SetCurSel(m_layer);
}
