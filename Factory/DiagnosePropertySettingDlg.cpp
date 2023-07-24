// DiagnosePropertySettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DiagnosePropertySettingDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphic.h"
#include "DiagnoseItemPropertySettingDlg.h"
#include "MainFrm.h"

// CDiagnosePropertySettingDlg �Ի���

IMPLEMENT_DYNAMIC(CDiagnosePropertySettingDlg, CDialog)

CDiagnosePropertySettingDlg::CDiagnosePropertySettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiagnosePropertySettingDlg::IDD, pParent),m_pDiagnoseChart(NULL)
	,m_nColorIndex(0)
{

}

CDiagnosePropertySettingDlg::CDiagnosePropertySettingDlg( CEqmDrawDiagnoseGraphic* pDiagnoseChart, CWnd* pParent /*= NULL*/ )
	: CDialog(CDiagnosePropertySettingDlg::IDD, pParent),m_pDiagnoseChart(pDiagnoseChart)
	,m_nColorIndex(0)
{

}

CDiagnosePropertySettingDlg::~CDiagnosePropertySettingDlg()
{
}

void CDiagnosePropertySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIAGNOSE_ITEM, m_list_diagnose_item);
}


BEGIN_MESSAGE_MAP(CDiagnosePropertySettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDiagnosePropertySettingDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DIAGNOSE_ITEM, &CDiagnosePropertySettingDlg::OnNMDblclkListDiagnoseItem)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_ITEM_ADD, &CDiagnosePropertySettingDlg::OnBnClickedButtonDiagnoseItemAdd)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_ITEM_DELETE, &CDiagnosePropertySettingDlg::OnBnClickedButtonDiagnoseItemDelete)
END_MESSAGE_MAP()


// CDiagnosePropertySettingDlg ��Ϣ�������


BOOL CDiagnosePropertySettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitList();
	vector<_tagDiagnoseItem>& vec = m_pDiagnoseChart->GetItemVec();
	for (size_t i=0;i<vec.size();++i)
	{
		m_list_diagnose_item.InsertItem(i,vec[i].strItemName);
		m_list_diagnose_item.SetItemText(i,1,vec[i].strPointName);
		m_list_diagnose_item.SetItemBkColor(i,2,vec[i].color);
		const int nSubItemCount = vec[i].vecItemInLine.size();
		CString strItemCount;
		strItemCount.Format(L"%d",nSubItemCount);
		m_list_diagnose_item.SetItemText(i,3,strItemCount);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDiagnosePropertySettingDlg::OnBnClickedOk()
{
	CDialog::OnOK();
}

void CDiagnosePropertySettingDlg::InitList()
{
	m_list_diagnose_item.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_list_diagnose_item.InsertColumn( 0, L"������", LVCFMT_LEFT,150);
	m_list_diagnose_item.InsertColumn( 1, L"����", LVCFMT_LEFT,150);
	m_list_diagnose_item.InsertColumn( 2, L"��ɫ", LVCFMT_LEFT,50);
	m_list_diagnose_item.InsertColumn( 3, L"������Ŀ", LVCFMT_LEFT,80);
}


void CDiagnosePropertySettingDlg::OnNMDblclkListDiagnoseItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	const int nItem = pNMItemActivate->iItem;
	if (nItem != -1)
	{
		CString strItemName = m_list_diagnose_item.GetItemText(nItem,0);
		_tagDiagnoseItem* pItem = m_pDiagnoseChart->GetItem(strItemName);
		CDiagnoseItemPropertySettingDlg dlg(pItem);
		if (dlg.DoModal() == IDOK)
		{
			m_list_diagnose_item.SetItemText(nItem,0,pItem->strItemName);
			m_list_diagnose_item.SetItemText(nItem,1,pItem->strPointName);
			m_list_diagnose_item.SetItemBkColor(nItem,2,pItem->color);
			const int size = pItem->vecItemInLine.size();
			CString strNum;
			strNum.Format(L"%d",size);
			m_list_diagnose_item.SetItemText(nItem,3,strNum);
			m_pDiagnoseChart->SetNeedSave(true);
		}
	}
	*pResult = 0;
}


void CDiagnosePropertySettingDlg::OnBnClickedButtonDiagnoseItemAdd()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	COLORREF color = pMainFrm->m_colorArray[m_nColorIndex];
	m_nColorIndex++;
	if (m_nColorIndex >= 8)
	{
		m_nColorIndex = 0;
	}
	const int nCount = m_list_diagnose_item.GetItemCount();
	CString strItemCount;
	strItemCount.Format(L"Item%02d",nCount+1);
	m_list_diagnose_item.InsertItem(nCount,strItemCount);
	m_list_diagnose_item.SetItemText(nCount,3,L"0");
	_tagDiagnoseItem item;
	item.strItemName = strItemCount;
	item.color = color;
	m_pDiagnoseChart->InsertItem(item);
	m_list_diagnose_item.SetItemBkColor(nCount,2,item.color);
}


void CDiagnosePropertySettingDlg::OnBnClickedButtonDiagnoseItemDelete()
{
	if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
	{
		vector<int> vec;
		POSITION pos = m_list_diagnose_item.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_list_diagnose_item.GetNextSelectedItem(pos);
			CString strItemName = m_list_diagnose_item.GetItemText(nItem,0);
			m_pDiagnoseChart->DeleteItem(strItemName);
			vec.push_back(nItem);
		}
		for (int i=vec.size()-1;i>=0;--i)
		{
			m_list_diagnose_item.DeleteItem(vec[i]);
		}
	}
}
