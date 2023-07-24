// SelectDataComId.cpp : implementation file
//

#include "stdafx.h"
#include "SelectDataComId.h"


// SelectDataComId dialog

IMPLEMENT_DYNAMIC(SelectDataComId, CSkinDlg)

SelectDataComId::SelectDataComId(CWnd* pParent /*=NULL*/)
	: CSkinDlg(SelectDataComId::IDD, pParent)
{
	
}

SelectDataComId::~SelectDataComId()
{
}

void SelectDataComId::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SELECTID, m_list_select_id);
}


BEGIN_MESSAGE_MAP(SelectDataComId, CSkinDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADDID, &SelectDataComId::OnBnClickedButtonAddid)
	ON_BN_CLICKED(IDC_BUTTON_DELETEID, &SelectDataComId::OnBnClickedButtonDeleteid)
	ON_BN_CLICKED(IDOK, &SelectDataComId::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SelectDataComId::OnBnClickedCancel)
END_MESSAGE_MAP()


// SelectDataComId message handlers
BOOL SelectDataComId::OnInitDialog()
{
	CSkinDlg::OnInitDialog();
	InitList();
	InitIDTable();
	return TRUE;
}
void SelectDataComId::InitList()
{
	m_list_select_id.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_list_select_id.InsertColumn( 0, _T("PLC变量短名"), LVCFMT_LEFT, 200 );
	m_list_select_id.InsertColumn( 1, _T("点位含义"), LVCFMT_LEFT, 200 );
}
void  SelectDataComId::InitIDTable()
{
	UpdateList();

}
void  SelectDataComId::UpdateList()
{
	m_list_select_id.DeleteAllItems();
	for(UINT idx=0; idx< m_vecDataName.size(); idx++)
	{
		m_list_select_id.InsertItem(idx,L"");
		DataPointEntry entry = m_refpointmap->GetEntry(m_vecDataName[idx].GetString());
		m_list_select_id.SetItemText(idx, 0, entry.GetShortName().c_str());
		m_list_select_id.SetItemText(idx, 1, entry.GetDescription().c_str());
	}
}
void SelectDataComId::OnBnClickedButtonAddid()
{
	// TODO: Add your control notification handler code here
	CDataComConfigDlg dlg(*m_refpointmap);
	CString strName;
	if (dlg.DoModal() == IDOK)
	{
		// get the data comid.
		strName = dlg.m_strPointName;
	}
	if(strName.GetLength() == 0)
	{
		return;
	}
	for(UINT idx=0; idx< m_vecDataName.size(); idx++)
	{
		if(m_vecDataName[idx] == strName )
		{
			AfxMessageBox(L"改点已经存在!");
			return;
		}
	}
	m_vecDataName.push_back(strName);
	UpdateList();
}

void SelectDataComId::OnBnClickedButtonDeleteid()
{
	// TODO: Add your control notification handler code here
	CString str = _T("");
	int nIndex = m_list_select_id.GetSelectionMark();
	str = m_list_select_id.GetItemText(nIndex,0);
	vector<CString>::iterator it = m_vecDataName.begin();
	for(;it!=m_vecDataName.end();)
	{
		if(*it==str)
		{
			it = m_vecDataName.erase(it);
			m_list_select_id.DeleteItem(nIndex);
		}
		else
		{
			++it;
		}
	}	
	UpdateList();
}

void SelectDataComId::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void SelectDataComId::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void SelectDataComId::SetPointMap( CDataPointManager* pointmap )
{
	m_refpointmap = pointmap;
}
