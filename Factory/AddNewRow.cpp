// AddNewRow.cpp : implementation file
//

#include "stdafx.h"
#include "AddNewRow.h"
#include "DataPointConfigDlg.h"
#include "MainFrm.h"
#include "DataPoint/DataPointEntry.h"


// AddNewRow dialog

IMPLEMENT_DYNAMIC(AddNewRow, CSkinDlg)

AddNewRow::AddNewRow( CWnd* pParent /*=NULL*/)
	: CSkinDlg(AddNewRow::IDD, pParent)
	, m_nIndex(0)
	, m_strShortName(_T(""))
	, m_strPlcName(_T(""))
	, m_strRwProperty(_T(""))
	, m_strDescription(_T(""))
	, m_strUnit(_T(""))
	, m_nPLCPointMaxVal(0)
	, m_nPLCPointMinVal(0)
	, m_combotypestring(_T(""))
	, m_maxmaxval(0)
	, m_minminval(0)
{
	
}

AddNewRow::~AddNewRow()
{
}

void AddNewRow::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_INDEX,m_nIndex);
	DDX_Text(pDX,IDC_EDIT_SHORTNAME,m_strShortName);
	DDX_Text(pDX,IDC_EDIT_PLCNAME,m_strPlcName);
	DDX_Text(pDX,IDC_EDIT_DESCRTPTION,m_strDescription);
	DDX_CBString(pDX,IDC_COMBO_UNIT,m_strUnit);
	DDX_CBString(pDX,IDC_COMBO_RWPRPPERTY,m_strRwProperty);
	DDX_Text(pDX,IDC_EDIT_PLCPOINTMAXVAL,m_nPLCPointMaxVal);
	DDX_Text(pDX,IDC_EDIT_PLCPOINTMINVAL,m_nPLCPointMinVal);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_CLEAR, m_btnClear);
	DDX_Text(pDX, IDC_EDIT_PLCPOINTMINVAL2, m_minminval);
	DDX_Control(pDX, IDC_COMBO1, m_combotype);
	DDX_CBString(pDX, IDC_COMBO1, m_combotypestring);
	DDX_Text(pDX, IDC_EDIT_PLCPOINTMAXVAL2, m_maxmaxval);
	DDX_Text(pDX, IDC_EDIT_PLCPOINTMINVAL2, m_minminval);
}


BEGIN_MESSAGE_MAP(AddNewRow, CSkinDlg)
	ON_BN_CLICKED(IDOK, &AddNewRow::OnBnClickedOk)
	ON_BN_CLICKED(ID_CLEAR, &AddNewRow::OnBnClickedClear)
	ON_BN_CLICKED(IDCANCEL, &AddNewRow::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL AddNewRow::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	m_btnOK.SetButtonProjectStyle(Blue);
	m_btnClear.SetButtonProjectStyle(Silver);
	m_btnCancel.SetButtonProjectStyle(Silver);

	m_combotype.AddString(_T("VT_BOOL"));	//布尔
	m_combotype.AddString(_T("VT_I1"));	
	m_combotype.AddString(_T("VT_I2"));
	m_combotype.AddString(_T("VT_I4"));
	m_combotype.AddString(_T("VT_INT"));	//整数
	m_combotype.AddString(_T("VT_UI1"));
	m_combotype.AddString(_T("VT_UI2"));
	m_combotype.AddString(_T("VT_UI4"));
	m_combotype.AddString(_T("VT_UINT"));
	m_combotype.AddString(_T("VT_R4"));	//浮点数
	m_combotype.AddString(_T("VT_R8"));
	m_combotype.AddString(_T("VT_BSTR"));	//字符串

	CenterWindow();
	return TRUE;
}
// AddNewRow message handlers

void AddNewRow::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strPlcName.GetLength()==0||m_strShortName.GetLength()==0)
	{
		::MessageBox(this->m_hWnd, TEXT("变量别名和全名都不能为空！\r\n"), TEXT("提示"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
		return;
	}
	long itemcount = m_listctrl->GetItemCount();
	for(int i = 0; i < itemcount; i++)     //add 7.28
	{
		CString j =  m_listctrl->GetItemText(i,0);		  
		int k = _ttoi(j);
		if(k == m_nIndex){ 
			::MessageBox(this->m_hWnd, TEXT("该Index已存在.  \r\n"), TEXT("提示"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
			m_listctrl->SetItemState(i,LIS_FOCUSED | LIS_ENABLED|  LIS_VISITED,LIS_FOCUSED | LIS_ENABLED | LIS_VISITED);
			m_listctrl->EnsureVisible(i,TRUE);
			return;
		}
	}

	CString index;
	index.Format(_T("%d"), m_nIndex);
	m_listctrl->InsertItem(itemcount, index);
	m_listctrl->SetItemText(itemcount, 1, m_strShortName);
	m_listctrl->SetItemText(itemcount, 2, m_strPlcName);
	m_listctrl->SetItemText(itemcount, 3, (m_strRwProperty =='R') ? _T("R") : _T("W"));
	m_listctrl->SetItemText(itemcount, 4, m_strDescription);
	m_listctrl->SetItemText(itemcount, 5, m_strUnit);
	CString strPLCpointmaxval;
	strPLCpointmaxval.Format(_T("%.2f"),m_nPLCPointMaxVal);
	m_listctrl->SetItemText(itemcount, 6, strPLCpointmaxval);
	CString strPLCpointminval;
	strPLCpointminval.Format(_T("%.2f"),m_nPLCPointMinVal);
	m_listctrl->SetItemText(itemcount, 7, strPLCpointminval);

	CString maxmaxvalstr;
	maxmaxvalstr.Format(_T("%.2f"), m_maxmaxval);
	m_listctrl->SetItemText(itemcount, 8, maxmaxvalstr);

	CString minminvalstr;
	minminvalstr.Format(_T("%.2f"), m_minminval);
	m_listctrl->SetItemText(itemcount, 9, minminvalstr);

	m_listctrl->SetItemText(itemcount, 10, m_combotypestring);

	m_listctrl->SetItemState(itemcount,LIS_FOCUSED | LIS_ENABLED|  LIS_VISITED,LIS_FOCUSED | LIS_ENABLED | LIS_VISITED);
	m_listctrl->SetFocus();
	m_listctrl->EnsureVisible(itemcount,TRUE);

	OnOK();
}

void AddNewRow::OnBnClickedClear()
{
	// TODO: Add your control notification handler code here
	m_nIndex=0;
	m_strShortName=_T("");
	m_strPlcName=_T("");
	m_strRwProperty=_T("");
	m_strDescription=_T("");
	m_strUnit=_T("");
	m_nPLCPointMaxVal=0;
	m_nPLCPointMinVal=0;

	UpdateData(FALSE);
}

void AddNewRow::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void AddNewRow::SetDestListCtrl(CListCtrlEx* m_l)
{
	m_listctrl = m_l;
}

void AddNewRow::PostNcDestroy()
{
	delete this;
}
