// DlgShowDupOpc.cpp : implementation file
//

#include "stdafx.h"
#include "DlgShowDupOpc.h"

#include "Tools/CustomTools/CustomTools.h"

// CDlgShowDupOpc dialog
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDlgShowDupOpc, CSkinDlg)

BEGIN_MESSAGE_MAP(CDlgShowDupOpc, CSkinDlg)
	ON_BN_CLICKED(IDOK, &CDlgShowDupOpc::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DUP_OPC, &CDlgShowDupOpc::OnLvnItemchangedListDupOpc)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
CDlgShowDupOpc::CDlgShowDupOpc(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CDlgShowDupOpc::IDD, pParent)
{

}

CDlgShowDupOpc::~CDlgShowDupOpc()
{
}

void CDlgShowDupOpc::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DUP_OPC, m_listctrl_dup_opc);
}

BOOL CDlgShowDupOpc::OnInitDialog()
{
    __super::OnInitDialog();

    InitList();
    UpdateList();

    return TRUE;
}

// CDlgShowDupOpc message handlers
//////////////////////////////////////////////////////////////////////////
void  CDlgShowDupOpc::SetVecEntryDuplicate(const VEC_ENTRY& vecentry)
{
    mVecEntryDuplicate = vecentry;
}

//////////////////////////////////////////////////////////////////////////
void CDlgShowDupOpc::InitList()
{
	m_listctrl_dup_opc.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );  //add by hun 8.12
	m_listctrl_dup_opc.InsertColumn( 0, _T("点位(必填)"), LVCFMT_LEFT, 70 );
	m_listctrl_dup_opc.InsertColumn( 1, _T("PLC变量短名"), LVCFMT_LEFT, 160 );
	m_listctrl_dup_opc.InsertColumn( 2, _T("PLC变量全名(必填)"), LVCFMT_LEFT, 200 );
	m_listctrl_dup_opc.InsertColumn( 3, _T("读写属性"), LVCFMT_LEFT, 40 );
	m_listctrl_dup_opc.InsertColumn( 4, _T("点位含义"), LVCFMT_LEFT, 200 );
	m_listctrl_dup_opc.InsertColumn( 5, _T("单位"), LVCFMT_LEFT, 60 );
	m_listctrl_dup_opc.InsertColumn( 6, _T("上限"),LVCFMT_LEFT, 70);
	m_listctrl_dup_opc.InsertColumn( 7, _T("下限"),LVCFMT_LEFT, 70);
	m_listctrl_dup_opc.InsertColumn( 8, _T("上上限"),	LVCFMT_LEFT, 50);
	m_listctrl_dup_opc.InsertColumn( 9, _T("下下限"),	LVCFMT_LEFT, 50);
	m_listctrl_dup_opc.InsertColumn(10, _T("类型"), LVCFMT_LEFT, 70);


	m_listctrl_dup_opc.SetEditBox(-1, 0);
	m_listctrl_dup_opc.SetEditBox(-1, 1);
	m_listctrl_dup_opc.SetEditBox(-1, 2);
	m_listctrl_dup_opc.SetEditBox(-1, 4);
	m_listctrl_dup_opc.SetEditBox(-1, 5);
	m_listctrl_dup_opc.SetEditBox(-1, 6);
	m_listctrl_dup_opc.SetEditBox(-1, 7);
	m_listctrl_dup_opc.SetEditBox(-1, 8);
	m_listctrl_dup_opc.SetEditBox(-1, 9);
	m_listctrl_dup_opc.SetEditBox(-1, 10);

	m_arrayProperty.Add(_T("R"));
	m_arrayProperty.Add(_T("W"));
	m_arrayProperty.Add(_T("R/W"));

	m_arrayUnit.Add(_T(" "));
	m_arrayUnit.Add(_T("%"));
	m_arrayUnit.Add(_T("kPa"));
	m_arrayUnit.Add(_T("℃"));
	m_arrayUnit.Add(_T("A"));
	m_arrayUnit.Add(_T("h"));
	m_arrayUnit.Add(_T("Hz"));
	m_arrayUnit.Add(_T("V"));
	m_arrayUnit.Add(_T("kW"));
	m_arrayUnit.Add(_T("l/s"));
	m_arrayUnit.Add(_T("kWh"));
	m_arrayUnit.Add(_T("m3/h"));

	m_arraytype.Add(_T("VT_BOOL"));	//布尔
	m_arraytype.Add(_T("VT_I1"));	
	m_arraytype.Add(_T("VT_I2"));
	m_arraytype.Add(_T("VT_I4"));
	m_arraytype.Add(_T("VT_INT"));	//整数
	m_arraytype.Add(_T("VT_UI1"));
	m_arraytype.Add(_T("VT_UI2"));
	m_arraytype.Add(_T("VT_UI4"));
	m_arraytype.Add(_T("VT_UINT"));
	m_arraytype.Add(_T("VT_R4"));	//浮点数
	m_arraytype.Add(_T("VT_R8"));
	m_arraytype.Add(_T("VT_BSTR"));	//字符串

	m_listctrl_dup_opc.SetComboBox(-1, 3, m_arrayProperty);
	m_listctrl_dup_opc.SetComboBox(-1, 5, m_arrayUnit);
	m_listctrl_dup_opc.SetComboBox(-1,10,m_arraytype);
	
}
void CDlgShowDupOpc::UpdateList()
{
	m_listctrl_dup_opc.DeleteAllItems();	                                 //add by hun 8.12
	 const UINT vecNum = (UINT)mVecEntryDuplicate.size();
	for(UINT idx0 = 0; idx0 < vecNum; idx0++)
	{
		const DataPointEntry& entry0 = mVecEntryDuplicate[idx0];
		long itemcount = m_listctrl_dup_opc.GetItemCount();
		CString index;
		index.Format(_T("%d"), entry0.GetPointIndex());
		m_listctrl_dup_opc.InsertItem(itemcount, index);
		m_listctrl_dup_opc.SetItemText(itemcount, 1, entry0.GetShortName().c_str());
		m_listctrl_dup_opc.SetItemText(itemcount, 2, entry0.GetParam(1).c_str());
		m_listctrl_dup_opc.SetItemText(itemcount, 3, (entry0.GetProperty() == PLC_READ) ? _T("R") : _T("W"));
		m_listctrl_dup_opc.SetItemText(itemcount, 4, entry0.GetDescription().c_str());
		m_listctrl_dup_opc.SetItemText(itemcount, 5, entry0.GetUnit().c_str());
		CString strPLCpointmaxval;

		wstring strtype;
		strtype = Project::Tools::AnsiToWideChar(entry0.GetTypeString().c_str());
		m_listctrl_dup_opc.SetItemText(itemcount, 10, strtype.c_str());


	}		
}

//////////////////////////////////////////////////////////////////////////
void CDlgShowDupOpc::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	mVecEntry.clear();
	DataPointEntry entry;
	long lItemCount = m_listctrl_dup_opc.GetItemCount();
	for ( long lIndex=0; lIndex<lItemCount; lIndex++ )
	{
		CString temp		= m_listctrl_dup_opc.GetItemText( lIndex, 0 );
		if (temp.IsEmpty())
			continue;
		
		entry.SetPointIndex(_ttoi((LPCTSTR)temp));
		
		entry.SetShortName(m_listctrl_dup_opc.GetItemText( lIndex, 1 ).GetString());
		
		entry.SetParam(2, m_listctrl_dup_opc.GetItemText( lIndex, 2 ).GetString());
		
		if (entry.GetParam(1).empty())
			continue;
		
		entry.SetProperty((m_listctrl_dup_opc.GetItemText( lIndex, 3 ) == _T("R")) ? PLC_READ : PLC_WRITE ) ;
		
		entry.SetDescription(m_listctrl_dup_opc.GetItemText( lIndex, 4 ).GetString());
		
		entry.SetUnit(m_listctrl_dup_opc.GetItemText( lIndex, 5 ).GetString());

		//entry.SetPLCPointMaxVal(_tstof((LPCTSTR)m_listctrl_dup_opc.GetItemText( lIndex, 6 )));
		//
		//entry.SetPLCPointMinVal(_tstof((LPCTSTR)m_listctrl_dup_opc.GetItemText( lIndex, 7 )));
		//
		//entry.SetPLCPointMaxMaxVal(_tstof((LPCTSTR)m_listctrl_dup_opc.GetItemText( lIndex, 8 )));
		//
		//entry.SetPLCPointMinMinVal(_tstof((LPCTSTR)m_listctrl_dup_opc.GetItemText( lIndex, 9 )));

		string pointttype;
		pointttype = Project::Tools::WideCharToAnsi(m_listctrl_dup_opc.GetItemText( lIndex, 10).GetString());
		entry.SetType(pointttype);

		mVecEntry.push_back(entry);
	}
	OnOK();
}

void CDlgShowDupOpc::OnLvnItemchangedListDupOpc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
