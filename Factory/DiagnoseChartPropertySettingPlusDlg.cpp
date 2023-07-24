// DiagnoseChartPropertySettingPlusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DiagnoseChartPropertySettingPlusDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "MainFrm.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphicPlus.h"
#include "DiagnoseGraphicPlusItemPropertySettingDlg.h"
#include "MetaFileSelectDlg.h"
#include "BindPropertySettingDlg.h"

// CDiagnoseChartPropertySettingPlusDlg 对话框

IMPLEMENT_DYNAMIC(CDiagnoseChartPropertySettingPlusDlg, CDialog)

CDiagnoseChartPropertySettingPlusDlg::CDiagnoseChartPropertySettingPlusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiagnoseChartPropertySettingPlusDlg::IDD, pParent)
	, m_nTimeScope(0)
	, m_nTimeInterval(0)
	, m_nTimeExtendScope(0)
	,m_nColorIndex(0)
	, m_nDiagnoseNormalID(-1)
	, m_nDiagnoseAbnormalID(-1)
	,m_nLinkPageID(-1)
	,m_bIsAnimationByAbnormal(0)
	,m_nShowMode(0)
{

}

CDiagnoseChartPropertySettingPlusDlg::~CDiagnoseChartPropertySettingPlusDlg()
{
}

void CDiagnoseChartPropertySettingPlusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DIAGNOSE_CHART_PLUS_TIME_SCOPE, m_nTimeScope);
	DDX_Text(pDX, IDC_EDIT_DIAGNOSE_CHART_PLUS_TIME_INTERVAL, m_nTimeInterval);
	DDX_Text(pDX, IDC_EDIT_DIAGNOSE_CHART_PLUS_TIME_EXTEND_SCOPE, m_nTimeExtendScope);
	DDX_Control(pDX, IDC_LIST_DIAGNOSE_CHART_PLUS_ITEM, m_list_diagnose_chart_plus_item);
	DDX_Text(pDX, IDC_EDIT_DIAGNOSE_CHART_PLUS_NORMALBIND, m_nDiagnoseNormalID);
	DDX_Text(pDX, IDC_EDIT_DIAGNOSE_CHART_PLUS_ABNORMALBIND, m_nDiagnoseAbnormalID);
	DDX_Control(pDX, IDC_COMBO_DIAGNOSE_LINK_PAGE, m_combo_link_page);
	DDX_Control(pDX, IDC_LIST_DIAGNOSE_CHART_PLUS_BIND_ITEM, m_list_item);
}


BEGIN_MESSAGE_MAP(CDiagnoseChartPropertySettingPlusDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDiagnoseChartPropertySettingPlusDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_CHART_PLUS_ITEM_ADD, &CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusItemAdd)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_CHART_PLUS_ITEM_DELETE, &CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusItemDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DIAGNOSE_CHART_PLUS_ITEM, &CDiagnoseChartPropertySettingPlusDlg::OnNMDblclkListDiagnoseChartPlusItem)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_CHART_PLUS_NORMALBIND, &CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusNormalbind)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_CHART_PLUS_ABNORMALBIND, &CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusAbnormalbind)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_CHART_PLUS_ITEM_ADD2, &CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusItemAdd2)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_CHART_PLUS_ITEM_DELETE2, &CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusItemDelete2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DIAGNOSE_CHART_PLUS_BIND_ITEM, &CDiagnoseChartPropertySettingPlusDlg::OnNMDblclkListDiagnoseChartPlusBindItem)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIAGNOSE_CHART_PLUS_BIND_ITEM, &CDiagnoseChartPropertySettingPlusDlg::OnNMClickListDiagnoseChartPlusBindItem)
	ON_BN_CLICKED(IDC_RADIO_BREVIARY, &CDiagnoseChartPropertySettingPlusDlg::OnBnClickedRadioBreviary)
	ON_BN_CLICKED(IDC_RADIO_DETAIL, &CDiagnoseChartPropertySettingPlusDlg::OnBnClickedRadioDetail)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DIAGNOSE_CHART_PLUS_BIND_ITEM, &CDiagnoseChartPropertySettingPlusDlg::OnLvnItemchangedListDiagnoseChartPlusBindItem)
END_MESSAGE_MAP()


// CDiagnoseChartPropertySettingPlusDlg 消息处理程序


void CDiagnoseChartPropertySettingPlusDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CString strID;
	GetDlgItem(IDC_EDIT_DIAGNOSE_CHART_PLUS_NORMALBIND)->GetWindowText(strID);
	if (m_nDiagnoseNormalID == -1 || strID.GetLength() == 0)
	{
		AfxMessageBox(L"绑定图片（正常）设置不正确，请正确配置!");
		return;
	}
	m_bIsAnimationByAbnormal = ((CButton*)GetDlgItem(IDC_RADIO_ISANIMATION))->GetCheck();
	CString strText;
	m_combo_link_page.GetLBText(m_combo_link_page.GetCurSel(),strText);
	
	if (strText == L"不指定")
	{
		m_nLinkPageID = -1;
	}
	else
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CEqmDrawProject& project = pFrame->m_project;
		m_nLinkPageID = project.GetPageIDByPageName(strText.GetString());
	}
	
	const size_t size = m_vecBoolBind.size();
	for (size_t i=0;i<size;++i)
	{
		const size_t size_sub = m_vecBoolBind[i].vecDiagnoseItemPlus.size();
		if (size_sub == 0)
		{
			CString str;
			str.Format(L"第%d项没有配置曲线，请配置后再保存！",i+1);
			AfxMessageBox(str);
			return;
		}
	}

	CDialog::OnOK();
}

void CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusItemAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strCurSelPointName == L"")
	{
		AfxMessageBox(L"绑定项未选中!");
		return;
	}
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	COLORREF color = pMainFrm->m_colorArray[m_nColorIndex];
	m_nColorIndex++;
	if (m_nColorIndex >= 8)
	{
		m_nColorIndex = 0;
	}
	const int nCount = m_list_diagnose_chart_plus_item.GetItemCount();
	CString strItemCount;
	strItemCount.Format(L"Item%02d",nCount+1);
	m_list_diagnose_chart_plus_item.InsertItem(nCount,strItemCount);
	_tagDiagnoseItemPlus item;
	item.strItemName = strItemCount;
	item.color = color;
	const size_t size = m_vecBoolBind.size();
	for (size_t i=0;i<size;++i)
	{
		if (m_vecBoolBind[i].strBoolPointName == m_strCurSelPointName)
		{
			m_vecBoolBind[i].vecDiagnoseItemPlus.push_back(item);
		}
	}
	m_list_diagnose_chart_plus_item.SetItemBkColor(nCount,2,item.color);
}


void CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusItemDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strCurSelPointName == L"")
	{
		AfxMessageBox(L"绑定项未选中!");
		return;
	}
	if (AfxMessageBox(L"确认要删除吗?",MB_OKCANCEL) == IDOK)
	{
		vector<int> vec;
		POSITION pos = m_list_diagnose_chart_plus_item.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_list_diagnose_chart_plus_item.GetNextSelectedItem(pos);
			CString strItemName = m_list_diagnose_chart_plus_item.GetItemText(nItem,0);
			const size_t size = m_vecBoolBind.size();
			for (size_t i=0;i<size;++i)
			{
				if (m_vecBoolBind[i].strBoolPointName == m_strCurSelPointName)
				{
					vector<_tagDiagnoseItemPlus>& vecItem = m_vecBoolBind[i].vecDiagnoseItemPlus;
					vector<_tagDiagnoseItemPlus>::iterator iter = vecItem.begin();
					while (iter != vecItem.end())
					{
						if ((*iter).strItemName == strItemName)
						{
							iter = vecItem.erase(iter);
							vec.push_back(nItem);
						}
						else
						{
							++iter;
						}
					}
				}
			}
		}
		for (int j=vec.size()-1;j>=0;--j)
		{
			m_list_diagnose_chart_plus_item.DeleteItem(vec[j]);
		}
	}
}


void CDiagnoseChartPropertySettingPlusDlg::OnNMDblclkListDiagnoseChartPlusItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	const int nItem = pNMItemActivate->iItem;
	if (nItem != -1)
	{
		CDiagnoseGraphicPlusItemPropertySettingDlg dlg;
		dlg.m_strItemName = m_list_diagnose_chart_plus_item.GetItemText(nItem,0);
		CString strOldItemName = dlg.m_strItemName;
		dlg.m_strBindPointName = m_list_diagnose_chart_plus_item.GetItemText(nItem,1);
		dlg.m_nColor = m_list_diagnose_chart_plus_item.GetItemBkColor(nItem,2);
		if (dlg.DoModal() == IDOK)
		{
			const int nCount = m_list_diagnose_chart_plus_item.GetItemCount();
			for (int i=0;i<nCount;++i)
			{
				if (i != nItem)
				{
					CString strText = m_list_diagnose_chart_plus_item.GetItemText(i,0);
					if (strText == dlg.m_strItemName)
					{
						AfxMessageBox(L"有重复的名称!~~");
						return;
					}
				}
			}
			{
				m_list_diagnose_chart_plus_item.SetItemText(nItem,0,dlg.m_strItemName);
				m_list_diagnose_chart_plus_item.SetItemText(nItem,1,dlg.m_strBindPointName);
				m_list_diagnose_chart_plus_item.SetItemBkColor(nItem,2,dlg.m_nColor);
				const size_t size = m_vecBoolBind.size();
				for (size_t i=0;i<size;++i)
				{
					if (m_vecBoolBind[i].strBoolPointName == m_strCurSelPointName)
					{
						vector<_tagDiagnoseItemPlus>& vecItem = m_vecBoolBind[i].vecDiagnoseItemPlus;
						vector<_tagDiagnoseItemPlus>::iterator iter = vecItem.begin();
						while (iter != vecItem.end())
						{
							if ((*iter).strItemName == strOldItemName)
							{
								(*iter).strItemName = dlg.m_strItemName;
								(*iter).strPointName = dlg.m_strBindPointName;
								(*iter).color = dlg.m_nColor;
								break;
							}
							else
							{
								++iter;
							}
						}
					}
				}
			}
		}
	}
	*pResult = 0;
}

void CDiagnoseChartPropertySettingPlusDlg::InitList()
{
	m_list_diagnose_chart_plus_item.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_list_diagnose_chart_plus_item.InsertColumn( 0, L"项名称", LVCFMT_LEFT,100);
	m_list_diagnose_chart_plus_item.InsertColumn( 1, L"点名", LVCFMT_LEFT,100);
	m_list_diagnose_chart_plus_item.InsertColumn( 2, L"颜色", LVCFMT_LEFT,50);

	m_list_item.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_list_item.InsertColumn(0,L"诊断点",LVCFMT_LEFT,60);
	m_list_item.InsertColumn(1,L"诊断结论1",LVCFMT_LEFT,60);
	m_list_item.InsertColumn(2,L"诊断结论2",LVCFMT_LEFT,60);
	m_list_item.InsertColumn(3,L"诊断结论3",LVCFMT_LEFT,60);
	m_list_item.InsertColumn(4,L"处理状态",LVCFMT_LEFT,60);
}


BOOL CDiagnoseChartPropertySettingPlusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	if (m_nShowMode == 0)//带导航
	{
		((CButton*)GetDlgItem(IDC_RADIO_BREVIARY))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_DETAIL))->SetCheck(1);
	}
	else if (m_nShowMode == 1)//全屏
	{
		((CButton*)GetDlgItem(IDC_RADIO_BREVIARY))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_DETAIL))->SetCheck(0);
	}
	for (size_t i=0;i<m_vecBoolBind.size();++i)
	{
		m_list_item.InsertItem(i,m_vecBoolBind[i].strBoolPointName);
		m_list_item.SetItemText(i,1,m_vecBoolBind[i].strResultPointName);
		m_list_item.SetItemText(i,2,m_vecBoolBind[i].strResultPointName2);
		m_list_item.SetItemText(i,3,m_vecBoolBind[i].strResultPointName3);
		m_list_item.SetItemText(i,4,m_vecBoolBind[i].strTreatmentStatePointName);
	}
	m_combo_link_page.AddString(L"不指定");
	int nIndex = -1;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CEqmDrawProject& project = pFrame->m_project;
	vector<CEqmDrawPage*>& page_list = project.GetPageList();
	for (size_t i=0;i<page_list.size();++i)
	{
		if (page_list[i])
		{
			const CString strPageName = page_list[i]->GetPageName().c_str();
			const int nPageID = page_list[i]->GetID();
			if (nPageID == m_nLinkPageID)
			{
				nIndex = i;
			}
			m_combo_link_page.AddString(strPageName);
		}
	}
	m_combo_link_page.SetCurSel(nIndex+1);
	if (m_bIsAnimationByAbnormal == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_ISANIMATION))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_ISANIMATION))->SetCheck(1);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusNormalbind()
{
	UpdateData(1);
	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		m_nDiagnoseNormalID = dlg.m_nID;
		pFrame->m_nSelectPicID = m_nDiagnoseNormalID;
		UpdateData(0);
	}
}


void CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusAbnormalbind()
{
	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		m_nDiagnoseAbnormalID = dlg.m_nID;
		if (dlg.GetWndType() == e_animation)
		{
			((CButton*)GetDlgItem(IDC_RADIO_ISANIMATION))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_ISANIMATION))->SetCheck(0);
		}
		pFrame->m_nSelectPicID = m_nDiagnoseAbnormalID;
		UpdateData(0);
	}
}


void CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusItemAdd2()
{
	const int nCount = m_list_item.GetItemCount();
	CString strIndex;
	strIndex.Format(L"Item%02d",nCount+1);
	m_list_item.InsertItem(nCount,strIndex);
	_tagBoolBindInfo info;
	info.strBoolPointName = strIndex;
	info.strResultPointName = L"";
	info.strResultPointName2 = L"";
	info.strResultPointName3 = L"";
	info.strTreatmentStatePointName = L"";
	m_vecBoolBind.push_back(info);
}


void CDiagnoseChartPropertySettingPlusDlg::OnBnClickedButtonDiagnoseChartPlusItemDelete2()
{
	if (AfxMessageBox(L"确认要删除吗?",MB_OKCANCEL) == IDOK)
	{
		vector<int> vec;
		POSITION pos = m_list_item.GetFirstSelectedItemPosition();
		while (pos != NULL)
		{
			int nItem = m_list_item.GetNextSelectedItem(pos);
			vec.push_back(nItem);
		}
		m_list_diagnose_chart_plus_item.DeleteAllItems();
		for (int j=vec.size()-1;j>=0;--j)
		{
			CString strText = m_list_item.GetItemText(vec[j],0);
			vector<_tagBoolBindInfo>::iterator iter = m_vecBoolBind.begin();
			while (iter != m_vecBoolBind.end())
			{
				if ((*iter).strBoolPointName == strText)
				{
					iter = m_vecBoolBind.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			m_list_item.DeleteItem(vec[j]);
		}
	}
}

void CDiagnoseChartPropertySettingPlusDlg::OnNMDblclkListDiagnoseChartPlusBindItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem != -1)
	{
		CBindPropertySettingDlg dlg;
		CString strBoolPointName = dlg.m_strBoolPointName = m_list_item.GetItemText(pNMItemActivate->iItem,0);
		dlg.m_strResultPointName = m_list_item.GetItemText(pNMItemActivate->iItem,1);
		dlg.m_strResultPointName2 = m_list_item.GetItemText(pNMItemActivate->iItem,2);
		dlg.m_strResultPointName3 = m_list_item.GetItemText(pNMItemActivate->iItem,3);
		dlg.m_strTreatmentPointName = m_list_item.GetItemText(pNMItemActivate->iItem,4);
		if (dlg.DoModal() == IDOK)
		{
			const int nCount = m_list_item.GetItemCount();
			for (int i=0;i<nCount;++i)
			{
				if (i!=pNMItemActivate->iItem)
				{
					CString strText = m_list_item.GetItemText(i,0);
					if (strText == dlg.m_strBoolPointName)
					{
						AfxMessageBox(L"绑定的布尔点重复!~~");
						return;
					}
				}
			}
			{
				m_list_item.SetItemText(pNMItemActivate->iItem,0,dlg.m_strBoolPointName);
				m_list_item.SetItemText(pNMItemActivate->iItem,1,dlg.m_strResultPointName);
				m_list_item.SetItemText(pNMItemActivate->iItem,2,dlg.m_strResultPointName2);
				m_list_item.SetItemText(pNMItemActivate->iItem,3,dlg.m_strResultPointName3);
				m_list_item.SetItemText(pNMItemActivate->iItem,4,dlg.m_strTreatmentPointName);
				vector<_tagBoolBindInfo>::iterator iter = m_vecBoolBind.begin();
				while (iter != m_vecBoolBind.end())
				{
					if ((*iter).strBoolPointName == strBoolPointName)
					{
						(*iter).strBoolPointName = dlg.m_strBoolPointName;
						(*iter).strResultPointName = dlg.m_strResultPointName;
						(*iter).strResultPointName2 = dlg.m_strResultPointName2;
						(*iter).strResultPointName3 = dlg.m_strResultPointName3;
						(*iter).strTreatmentStatePointName = dlg.m_strTreatmentPointName;
						break;
					}
					iter++;
				}
			}
		}
	}
	*pResult = 0;
}


void CDiagnoseChartPropertySettingPlusDlg::OnNMClickListDiagnoseChartPlusBindItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem != -1)
	{
		m_list_item.SetItemState(pNMItemActivate->iItem,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
		CString strBoolPointName = m_list_item.GetItemText(pNMItemActivate->iItem,0);
		m_strCurSelPointName = strBoolPointName;
		vector<_tagBoolBindInfo>::iterator iter = m_vecBoolBind.begin();
		while (iter != m_vecBoolBind.end())
		{
			if ((*iter).strBoolPointName == strBoolPointName)
			{
				m_list_diagnose_chart_plus_item.DeleteAllItems();
				vector<_tagDiagnoseItemPlus>& vec = (*iter).vecDiagnoseItemPlus;
				const size_t size = vec.size();
				for (size_t i=0;i<size;++i)
				{
					m_list_diagnose_chart_plus_item.InsertItem(i,L"");
					m_list_diagnose_chart_plus_item.SetItemText(i,0,vec[i].strItemName);
					m_list_diagnose_chart_plus_item.SetItemText(i,1,vec[i].strPointName);
					m_list_diagnose_chart_plus_item.SetItemBkColor(i,2,vec[i].color);
				}
				break;
			}
			iter++;
		}
	}
	else
	{
		m_strCurSelPointName = L"";
	}
	*pResult = 0;
}


void CDiagnoseChartPropertySettingPlusDlg::OnBnClickedRadioBreviary()
{
	m_nShowMode = 1;
}


void CDiagnoseChartPropertySettingPlusDlg::OnBnClickedRadioDetail()
{
	m_nShowMode = 0;
}


void CDiagnoseChartPropertySettingPlusDlg::OnLvnItemchangedListDiagnoseChartPlusBindItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
