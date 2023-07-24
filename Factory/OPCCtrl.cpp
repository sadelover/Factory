
// OPC 变量读写的实现类
//
//

#include "StdAfx.h"

#include "OPCCtrl.h"
//#include "Tools/vld.h"


void COPCConnection_AutoBrowse::SetPointList( const vector<DataPointEntry>& pointlist )
{
	m_pointlist = pointlist;
}


vector<DataPointEntry> COPCConnection_AutoBrowse::GetPointList() const
{
	return m_pointlist;
}

bool COPCConnection_AutoBrowse::Init(wstring strOPCServerIP)
{
	if (m_pointlist.empty())
	{
		ASSERT(false);
		return false;
	}


	//build search map;
	for (unsigned int i = 0; i < m_pointlist.size(); i++)
	{
		const DataPointEntry& entry= m_pointlist[i];
		m_namemap[entry.GetShortName()] = entry.GetPlcAddress();
	}

	//connect opc server 
	bool bresult = Connect(strOPCServerIP);
	if (!bresult)
	{
		return false;
	}

	AddGroup();
	AddItems();

	return true;
}

wstring COPCConnection_AutoBrowse::GetPlcNameByShortName( const wstring& shortname )
{
	hash_map<wstring, wstring>::const_iterator it = m_namemap.find(shortname);
	if (it != m_namemap.end()){
		return it->second;
	}

	return _T("");
}

void COPCConnection_AutoBrowse::AddErrorLog(const wstring& shortname)
{

}

VARTYPE COPCConnection_AutoBrowse::GetItemTypeByShortName( const wstring& strShortName )
{
	wstring plcaddress = GetPlcNameByShortName(strShortName);
	VARTYPE vartype = GetVarType(plcaddress);
	return vartype;
}

LPCTSTR COPCConnection_AutoBrowse::GetItemQualityByShortName( const wstring& strShortName )
{
	wstring plcaddress = GetPlcNameByShortName(strShortName);
	LPCTSTR itemquality = GetVarQuality(plcaddress);
	return itemquality;
}

bool COPCConnection_AutoBrowse::SearchOPCServer( wstring strOPCServerIP,hash_map<wstring,CLSID>& clsidmap ,int nType)
{
	if(nType == 0)
	{
		return SearchOPC(strOPCServerIP,clsidmap);
	}
	else if(nType == 1)
	{
		return SearchOPC_(strOPCServerIP,clsidmap,nType);
	}
	else
	{
		return SearchOPC_(strOPCServerIP,clsidmap,nType);
	}
}

bool COPCConnection_AutoBrowse::Exit()
{
	Project::Tools::Scoped_Lock<Mutex>	scopelock(m_lock);
	if(m_pServer && m_pServer->IsConnected())
	{
		if(m_pGroup)
			m_pGroup->RemoveAllItems(true);
		m_pServer->RemoveAllGroups(true);
		DisConnect();
		m_pGroup = NULL;
		m_pServer = NULL;
	}
	return true;
}

bool COPCConnection_AutoBrowse::AddItem( const wstring& strItemID )
{
	if(strItemID.length() <= 0)
		return false;

	CObArray	itemlist;	//保存所有的Item
	CKItem *pItem = FindItem(strItemID);
	if (NULL != pItem )
	{
		return false;
	}
	else
	{
		pItem	= new CKItem(m_pGroup);
		pItem->SetAccessPath(_T(""));
		pItem->SetItemID(strItemID.c_str());
		pItem->SetActive(TRUE);
		pItem->SetDataType(VT_R4);
		itemlist.Add(pItem);
		//m_searchmap.insert(std::make_pair(strItemID.c_str(), pItem));
		m_pGroup->AddItems(itemlist, (DWORD)itemlist.GetCount());
		m_searchmap.insert(std::make_pair(strItemID.c_str(), pItem));
		/*if(!pItem->IsValid())
		{
		delete pItem;
		pItem = NULL;
		}
		else
		{
		m_searchmap.insert(std::make_pair(strItemID.c_str(), pItem));
		}*/
		return true;
	}

	return false;
}

bool COPCConnection_ManualBrowse::AddItems()
{
	if (m_pointlist.empty()){
		return false;
	}
	
	for (unsigned int i = 0; i < m_pointlist.size(); i++)
	{
		const DataPointEntry& entry = m_pointlist[i];
		CKItem* pitem	= new CKItem(m_pGroup);
		pitem->SetAccessPath(_T(""));
		pitem->SetItemID(entry.GetPlcAddress().c_str());
		pitem->SetActive(TRUE);
		pitem->SetDataType(entry.GetOpcPointType() );
		m_itemlist.Add(pitem);

		// added by 桂必成
		m_searchmap.insert(std::make_pair(entry.GetPlcAddress().c_str(), pitem));

		//add by golding
		DataPointEntry entryTemp = entry;
		m_searchEntrymap.insert(std::make_pair(entry.GetPlcAddress().c_str(), entryTemp));
	}
	
	m_pGroup->AddItems(m_itemlist, (DWORD)m_itemlist.GetCount());

	return true;
}

bool COPCConnection_ManualBrowse::GetItemValue( const wstring& strItemID, wstring &strValue )
{
	//_tprintf(L"COPCConnection_ManualBrowse::GetItemValue");
	CKItem *pItem = FindItem(strItemID);
	//	assert(pItem);
	if (NULL == pItem)
	{
		pItem	= new CKItem(m_pGroup);
		pItem->SetAccessPath(_T(""));
		pItem->SetItemID(strItemID.c_str());
		pItem->SetActive(TRUE);
		pItem->SetDataType(VT_R4);
		m_itemlist.Add(pItem);
		m_searchmap.insert(std::make_pair(strItemID.c_str(), pItem));
		m_pGroup->RemoveAllItems(true);
		m_pGroup->AddItems(m_itemlist, (DWORD)m_itemlist.GetCount());
	}

	CString strTmpVal = _T(""); 
	try
	{
		CObArray		cItems;
		CStringArray	cValues;
		DWORD			cdwItems	= 0;

		// Construct an item and value list for this write operation:
		cItems.SetSize(1);
		cValues.SetSize(1);
		cItems[cdwItems] = pItem;

		// Add corresponding write value to value array:
		cValues[cdwItems] = strValue.c_str();
		++cdwItems;
		bool bReadSuccess = m_pGroup->ReadSync(cItems, cdwItems, false);
		if(!bReadSuccess)
			return false;

		if(pItem->GetDataType()!=pItem->GetDataType2())
		{
			int xx=0;
		}

	}
	catch (...)
	{
		_tprintf(L"catch exception");
	}

	pItem->GetValue(strTmpVal);
	strValue = (LPCTSTR)strTmpVal;

	return true;
}

bool COPCConnection_ManualBrowse::SetItemValue( const wstring& strItemID, wstring &strValue )
{
	CKItem *pItem = FindItem(strItemID);
	assert(pItem);
	if (NULL == pItem)
	{
		CKItem* pitem	= new CKItem(m_pGroup);
		pitem->SetAccessPath(_T(""));
		pitem->SetItemID(strItemID.c_str());
		pitem->SetActive(TRUE);
		pitem->SetDataType(VT_R4);
		m_itemlist.Add(pitem);
		m_searchmap.insert(std::make_pair(strItemID.c_str(), pitem));
		m_pGroup->RemoveAllItems(true);
		m_pGroup->AddItems(m_itemlist, (DWORD)m_itemlist.GetCount());
	}

	try
	{
		CObArray	  cItems;
		CStringArray  cValues;
		DWORD		  cdwItems = 0;

		// Construct an item and value list for this write operation:
		cItems.SetSize (1);
		cValues.SetSize (1);
		cItems[cdwItems] = pItem;

		// Add corresponding write value to value array:
		cValues[cdwItems] = strValue.c_str();
		++cdwItems;

		m_pGroup->WriteSync(cItems, cValues, cdwItems);
	}
	catch (...)
	{
	}

	return true;
}
