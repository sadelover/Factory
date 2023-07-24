// DataComConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataComConfigDlg.h"
#include "Tools/ColorDefines.h"
#include "Tools/Util/UtilString.h"
#include "VPointSimpleSettingDlg.h"
#include "MainFrm.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"

// CDataComConfigDlg dialog
const int c_select_id_default = -1;


IMPLEMENT_DYNAMIC(CDataComConfigDlg, CSkinDlg)

bool FindStringInString(const wstring& strSrc0, const wstring& strToFind0)
{
	bool ifFind = false;
	CString strSrc1 = strSrc0.c_str();
	strSrc1.MakeLower();
	CString strToFind1 = strToFind0.c_str();
	strToFind1.MakeLower();


	int findPos = (int)strSrc1.Find(strToFind1);
	if(findPos >=0 && findPos < (int)strSrc1.GetLength() )
		ifFind = true;
	return ifFind;
}
CDataComConfigDlg::CDataComConfigDlg(CDataPointManager& pointmap, CWnd* pParent /*=NULL*/)
: CSkinDlg(CDataComConfigDlg::IDD, pParent)
,m_refpointmap(pointmap)
,m_selectedId(c_select_id_default)
,m_selectedStoreType(0)
,m_strPointName(_T(""))

,m_strSearch(_T("") )
, m_itemNum(0)
{

}

CDataComConfigDlg::~CDataComConfigDlg()
{
	m_entryListBySearch.clear();
	m_currentEntryList.clear();  // add by Bruce.xu 2011/4/13
	m_selectedEntryMap.clear();  // add by Bruce.xu 2011/4/13
}

void CDataComConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_strSearch);	
	DDX_Control(pDX, IDC_LIST_DataPoint, m_pointlist);
	DDX_Control(pDX, ID_CONFIRM, m_btnOK);
	DDX_Control(pDX, ID_CANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CDataComConfigDlg, CSkinDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DataType, &CDataComConfigDlg::OnCbnSelchangeComboDatatype)
	ON_CBN_SELCHANGE(IDC_COMBO_NUMBER, &CDataComConfigDlg::OnCbnSelchangeComboNumber)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &CDataComConfigDlg::OnEnChangeEditSearch)
	ON_BN_CLICKED(ID_CONFIRM, &CDataComConfigDlg::OnBnClickedConfirm)
	ON_BN_CLICKED(ID_CANCEL, &CDataComConfigDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DataPoint, &CDataComConfigDlg::OnNMClickListDatapoint)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DataPoint, &CDataComConfigDlg::OnItemchangedList)  // add by Bruce.xu 2011/4/13
	ON_BN_CLICKED(IDC_BTN_ADD_VPOINT, &CDataComConfigDlg::OnBnClickedBtnAddVpoint)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CDataComConfigDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDataComConfigDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CDataComConfigDlg message handlers

void CDataComConfigDlg::OnBnClickedConfirm()
{
	// TODO: Add your control notification handler code here
	CString strPointName;
	POSITION pos = m_pointlist.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}
	else
	{
		CString	strSelName;
		while (pos)
		{
			int nItem = m_pointlist.GetNextSelectedItem(pos);
			strPointName += m_pointlist.GetItemText(nItem, 0);
			strPointName += _T(",");
		}
		strPointName.TrimRight(_T(","));
	}

	//int cur = m_pointlist.GetNextItem(-1, LVIS_SELECTED);
	//CString strPointName = m_pointlist.GetItemText(cur, 0);
	m_strPointName = strPointName;
	m_selectedId = m_refpointmap.GetEntry(strPointName.GetString()).GetSlaveID();
	m_selectedStoreType = (int)(m_refpointmap.GetEntry(strPointName.GetString()).GetStoreCycle());
	OnOK();
}

void CDataComConfigDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


void CDataComConfigDlg::OnCbnSelchangeComboDatatype()
{
	
	UpdateData(TRUE);
	RefreshList(e_type);
	UpdateData(FALSE);
}

void CDataComConfigDlg::OnCbnSelchangeComboNumber()
{
	UpdateData(TRUE);
	RefreshList(e_number);
}

void CDataComConfigDlg::OnEnChangeEditSearch()
{
	//UpdateData(TRUE); //control to value
	//ParseSearchString();
	//UpdateEntryListBySearch();
	//RefreshList(e_search);
}

void CDataComConfigDlg::ParseSearchString()   //add by hun 6.17
{
	const wstring c_str_space = _T(" ");
	wstring wstr = m_strSearch;
	wstring wstrPart;
	m_strVecSearch.clear();
	if(m_strSearch.GetLength() < 1){
		//no search string , must show all
		return;
	}
	int spacePos = 0;
	while(wstr.length()>0 ){
		spacePos = (int)wstr.find(c_str_space);
		if(spacePos < 0 
			|| spacePos >= (int)wstr.length() )
			break;
		wstrPart = wstr.substr(0, spacePos);
		m_strVecSearch.push_back(wstrPart);
		wstr = wstr.substr(spacePos+1);
	}
	if(wstr.length() ){
		m_strVecSearch.push_back(wstr);
	}
}

void CDataComConfigDlg::OnNMClickListDatapoint( NMHDR *pNMHDR, LRESULT *pResult )
{
	*pResult = 0;

	const int cur = m_pointlist.GetNextItem(-1, LVIS_SELECTED);
	const CString shortname = m_pointlist.GetItemText(cur, 0);
	m_selectedId = m_refpointmap.GetEntry(shortname.GetString()).GetSlaveID();
	m_selectedStoreType = (int)(m_refpointmap.GetEntry(shortname.GetString()).GetStoreCycle());
}

// Add by Bruce.xu 2011/4/13
// Response the LVN_ITEMCHANGED message.
void CDataComConfigDlg::OnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uChanged == LVIF_STATE)
	{
		if (pNMListView->uNewState & LVIS_SELECTED)
		{
			POSITION pos = ((CListCtrl*)GetDlgItem(IDC_LIST_DataPoint))->GetFirstSelectedItemPosition();
			if (pos == NULL)
			{
				TRACE0("No items were selected!\n");
				return ;
			}
			else
			{
				while(pos)
				{
					// Insert the selected items to the map.
					int nSelectIndex = ((CListCtrl*)GetDlgItem(IDC_LIST_DataPoint))->GetNextSelectedItem(pos);
					InsertSelectedEntryToMap(nSelectIndex);
					TRACE1("Item %d was selected!  \r\n", nSelectIndex);
				}
			}
		}
		else
		{
			// When the CTRL key is not pressed.
			if (!m_bCtrlKeyDown)
			{
				int nItemIndex = pNMListView->iItem;
				map<int, DataPointEntry>::iterator it = m_selectedEntryMap.find(nItemIndex);
				if (it != m_selectedEntryMap.end())
				{
					m_selectedEntryMap.erase(it);
				}
			}
		}
	}

	*pResult = 0;
}

void CDataComConfigDlg::InitList()
{
	CImageList m_l;  
	m_l.Create(1,18,TRUE|ILC_COLOR32,1,0);  
	m_pointlist.SetImageList(&m_l,LVSIL_SMALL);  

	DWORD oldstyle = m_pointlist.GetExtendedStyle();
	m_pointlist.SetExtendedStyle(oldstyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT |  LVS_EX_LABELTIP ); //LIST_CONTROL_STYLE , LVS_SHOWSELALWAYS

	m_pointlist.InsertColumn(0, _T("变量名"), LVCFMT_LEFT, 200);
	m_pointlist.InsertColumn(1, _T("变量含义"), LVCFMT_LEFT, 200);
	m_pointlist.InsertColumn(2, _T("来源"), LVCFMT_LEFT, 100);
	m_pointlist.InsertColumn(3, _T("地址"), LVCFMT_LEFT, 200);
	//m_pointlist.InsertColumn(4, _T("存储类型"), LVCFMT_LEFT, 170);

	m_entryList.clear();
	const map<int, DataPointEntry>& pointmap = m_refpointmap.GetPointMap();
	map<int, DataPointEntry>::const_iterator it = pointmap.begin();
	for (; it != pointmap.end(); it++)
	{
		int id = (*it).first;
		m_entryList.push_back((*it).second);
	}
	m_entryListBySearch = m_entryList;
	RefreshList(e_type);
}

void CDataComConfigDlg::RefreshList( E_REFRESH_TYPE eType )
{
	m_pointlist.DeleteAllItems();
	const vector<DataPointEntry>* dataList = NULL;
	dataList = &m_entryListBySearch;
	ASSERT(dataList);
	if(!dataList)
		return;

	int nNameLen = 0;
	int nNameLenMax = 0;
	for (unsigned int i = 0; i < dataList->size(); i++)
	{
		const DataPointEntry& entry = dataList->at(i);
		m_pointlist.InsertItem(i, entry.GetShortName().c_str());
		m_pointlist.SetItemText(i, 1, entry.GetDescription().c_str());
		m_pointlist.SetItemText(i, 2, entry.GetSourceType().c_str());
		m_pointlist.SetItemText(i, 3, entry.GetParam(1).c_str());

		nNameLen = wcslen(entry.GetShortName().c_str());
		if (nNameLenMax < nNameLen)
		{
			nNameLenMax = nNameLen;
		}

		/*CString strStoreType;
		strStoreType.Format(_T("%d"),(int)entry.GetStoreCycle());
		m_pointlist.SetItemText(i, 4, strStoreType);*/
	}
	nNameLenMax *= 7;
	if (nNameLenMax < 200)
	{
		nNameLenMax = 200;
	}
	m_pointlist.SetColumnWidth(0, nNameLenMax);

	////set choose one
	//if(c_select_id_default != m_selectedId)
	//{
	//	//m_pointlist.SetFocus();

	//	for (UINT idx = 0; idx < dataList->size(); idx++)
	//	{
	//		const DataPointEntry& entry = dataList->at(idx);
	//		if(entry.GetPointIndex() == m_selectedId)
	//		{
	//			//m_pointlist.SetHotItem(idx);

	//			m_pointlist.SetItemState(idx, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); //设置行index为选中并高亮（如果未设置Focuse，被选中行会变为灰色）
	//			m_pointlist.EnsureVisible(idx , FALSE);

	//			break;
	//		}
	//	}
	//}

	if(_T("") != m_strPointName)
	{
		//m_pointlist.SetFocus();

		for (UINT idx = 0; idx < dataList->size(); idx++)
		{
			const DataPointEntry& entry = dataList->at(idx);
			if(entry.GetShortName() == m_strPointName.GetString())
			{
				//m_pointlist.SetHotItem(idx);

				m_pointlist.SetItemState(idx, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); //设置行index为选中并高亮（如果未设置Focuse，被选中行会变为灰色）
				m_pointlist.EnsureVisible(idx , FALSE);

				break;
			}
		}
	}

}

void CDataComConfigDlg::UpdateEntryListBySearch()   // edit 6.20 多重搜索
{
	m_entryListBySearch.clear();
	const UINT searchTimeNum = (UINT)m_strVecSearch.size() ;
	if(searchTimeNum < 1 ){
		m_entryListBySearch = m_entryList;
		return;
	}

	UINT realIdx = 0;
	for(UINT idx = 0; idx< m_entryList.size(); idx++){
       // realIdx = m_entryListByNumber[idx];
		const DataPointEntry& entry = m_entryList[idx];
		bool ifFind = true;
			for(UINT searchTimeIdx=0; searchTimeIdx< searchTimeNum; searchTimeIdx++) {
				const wstring& wstrSearch = m_strVecSearch[searchTimeIdx];

				ifFind=( FindStringInString(entry.GetDescription(), wstrSearch)
					|| FindStringInString(entry.GetParam(1),wstrSearch )
					|| FindStringInString(entry.GetShortName(), wstrSearch )
					);
				if(ifFind)
					break;
			}

			if(ifFind){
				m_entryListBySearch.push_back(entry);
			}
	}

	m_currentEntryList = m_entryListBySearch;  // add by Bruce.xu 2011/4/13
}


UINT CDataComConfigDlg::GetItemNumber( const DataPointEntry& entry )
{
	UINT numberRet = 0;
	CString strTemp;
	for(int idx=20; idx> 0; idx--){
		strTemp.Format(_T("#%d"), idx);
		int findPos = (int)entry.GetDescription().find(strTemp);
		if(findPos >= 0 
			&& findPos < (int)entry.GetDescription().length() ){
				numberRet = idx;
				break;
		}
	}

	return numberRet;
}

UINT CDataComConfigDlg::GetNumberFromString( const CString& str )
{
	if(_T(" ") == str){
		return 0;
	}
	string str0= UtilString::ConvertWideCharToMultiByte(str.GetString() );  
	int num = atoi( str0.c_str() );
	return num;
}

BOOL CDataComConfigDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();
	InitList();
	m_btnCancel.SetButtonProjectStyle(Silver);

	

	return TRUE;
}

// Add by Bruce.xu 2011/4/13
// Get the selected entry list.
map<int, DataPointEntry> CDataComConfigDlg::GetSelectedEntryLists()
{
	return m_selectedEntryMap;
}

// Add by Bruce.xu 2011/4/14
// Set the selected entry list.
void CDataComConfigDlg::SetSelectedEntryLists(const map<int, DataPointEntry>& dataPointMap)
{
	m_selectedEntryMap = dataPointMap;
}

// Add by Bruce.xu 2011/4/13
// Insert the selected item to a list.
void CDataComConfigDlg::InsertSelectedEntryToMap(int nSelectedId)
{
	if(nSelectedId >= (int)m_currentEntryList.size())
		return;
	const DataPointEntry dataPoint = m_currentEntryList[nSelectedId];
	m_selectedEntryMap.insert(make_pair(nSelectedId, dataPoint));
}

BOOL CDataComConfigDlg::PreTranslateMessage( MSG* msg )   //add by hun 8.3
{
	if (msg->message == WM_KEYDOWN)
	{
		switch(msg->wParam)
		{
		case VK_RETURN:
			OnBnClickedBtnSearch();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(msg);
}


void CDataComConfigDlg::OnBnClickedBtnAddVpoint()
{
	// TODO: Add your control notification handler code here
	CVPointSimpleSettingDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		CString	strVpRw(dlg.GetVPointRwAttri());
		map<int, DataPointEntry>& mapData = m_refpointmap.GetPointMap();
		map<int, DataPointEntry>::const_iterator	iter;
		int nMaxId = 0;

		for (iter=mapData.begin(); iter!=mapData.end(); ++iter)
		{
			if (iter->first > nMaxId)
			{
				nMaxId = iter->first;
			}
		}

		DataPointEntry entry;

		entry.SetPointIndex(nMaxId + 1);
		entry.SetShortName((dlg.GetVPointName()).GetString());
		entry.SetSourceType(L"vpoint");

		if (_T("R") == strVpRw)
		{
			entry.SetProperty(PLC_READ);
		}
		else if (_T("W") == strVpRw)
		{
			entry.SetProperty(PLC_WRITE);
		}
		else if (_T("R/W") == strVpRw)
		{
			entry.SetProperty(PLC_PROP_MAX);
		}
		else
		{	// unknow attribute
			entry.SetProperty(PLC_READ);
		}
		entry.SetStoreCycle(E_STORE_ONE_MINUTE);

		//检查名称是否重复
		for (iter=mapData.begin(); iter!=mapData.end(); ++iter)
		{
			const DataPointEntry& entryMap = iter->second;
			if (entryMap.GetShortName() == entry.GetShortName())
			{
				MessageBox(_T("软点名称重复！"), _T("错误"), MB_OK|MB_ICONERROR);
				return;
			}
		}
		m_refpointmap.AddEntry(entry);
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
		pMainFrame->m_pPrjSqlite->SaveProjectSqlite_opcid3();//直接存入数据库。m_refpointmap指向的vector在函数中直接调用不用赋值

		// add into memory and show
		m_entryList.push_back(entry);
		m_entryListBySearch.push_back(entry);
		RefreshList(e_type);


		// set point table save flag
		
		ASSERT(pMainFrame != NULL);
		if (NULL == pMainFrame)
		{
			MessageBox(_T("获取MainFrame指针失败！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}
		pMainFrame->SetPointTableFlag(true);
		if(pMainFrame->m_wndFileView.GetDataAccess())
		{
			//如果在綫時通知core重新load虚拟点表
			pMainFrame->m_wndFileView.GetDataAccess()->WriteCoreDebugItemValue(_T("core_reload_point_table"), _T("1"));
		}
	}
}

void CDataComConfigDlg::OnBnClickedBtnSearch()
{
	UpdateData(TRUE);
	ResetPointList();
}


void CDataComConfigDlg::OnBnClickedBtnClear()
{
	m_strSearch = _T("");
	UpdateData(FALSE);
	ResetPointList();
}


void CDataComConfigDlg::ResetPointList(void)
{
	ParseSearchString();
	UpdateEntryListBySearch();
	RefreshList(e_search);
}
