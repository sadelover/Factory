#pragma once

#include "StdAfx.h"
#include "strsafe.h"
#include "OPC_Connection.h"
#include "Tools/CustomTools/CustomTools.h"
//#include "Tools/vld.h"
const GUID CLSID_OPCServerList={0x13486D51, 0x4821, 0x11D2, { 0xA4, 0x94, 0x3C, 0xB3, 0x06, 0xC1, 0x00, 0x00 } };

COPCConnection::COPCConnection()
: m_strFilterLeaf( L"*" )
, m_bBrowseFlat( TRUE )
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
	m_strserverprogid		= L"";

	m_strserverhost = L"localhost";

	m_pServer		= NULL;
	m_pGroup		= NULL;
	m_nCurrentCount = 0;
	m_nTotalCount = 0;
}

COPCConnection::~COPCConnection(void)
{
	if (NULL != m_pServer){
		delete m_pServer;
		m_pServer = NULL;
	}
	//for (int i = 0; i < m_itemlist.GetCount(); i++)
	//{
	//	CKItem* pitem = (CKItem*)m_itemlist.GetAt(i);
	//	delete pitem;
	//	pitem = NULL;
	//}

	//clear item;
	map<wstring, CKItem*>::iterator it = m_searchmap.begin();
	for(;it!=m_searchmap.end();it++)
	{
		CKItem *pitem = (*it).second;
		if(pitem)
		{
			delete(pitem);
			pitem = NULL;
		}
	}

	m_searchmap.clear();
	m_searchEntrymap.clear();


    CoUninitialize();
}

void COPCConnection::GetValue(VARIANT &vtVal	// [in]
							   , TCHAR *pBuffer	// [out]
							   , int nBufLen)	// [in]
{
	ASSERT (pBuffer != NULL);
	ASSERT (nBufLen > 0);

	// Declare a CString to help construct result string:
	CString strVal = _T("");

	// Format string according to data type:
	switch (vtVal.vt)
	{
	case VT_BOOL:
		strVal.Format (_T("%d"), vtVal.boolVal);
		break;

	case VT_UI1:
		strVal.Format (_T("%u"), vtVal.bVal);
		break;

	case VT_I1:
		strVal.Format (_T("%d"), vtVal.cVal);
		break;

	case VT_UI2:
		strVal.Format (_T("%u"), vtVal.uiVal);
		break;

	case VT_I2:
		strVal.Format (_T("%d"), vtVal.iVal);
		break;

	case VT_UI4:
		strVal.Format (_T("%u"), vtVal.ulVal);
		break;

	case VT_I4:
		strVal.Format (_T("%d"), vtVal.lVal);
		break;

	case VT_R4:
		strVal.Format (_T("%G"), vtVal.fltVal);
		break;

	case VT_R8:
		strVal.Format (_T("%G"), vtVal.dblVal);
		break;

	case VT_BSTR:
		strVal = vtVal.bstrVal;
		break;

	case VT_DATE:
		{
			bool bSuccess = false;

			// Cariant time to system time (UTC):
			SYSTEMTIME systime;
			if ( VariantTimeToSystemTime ( vtVal.dblVal, &systime ) )
			{
				// Get time zone information:
				TIME_ZONE_INFORMATION tTZI;
				if ( GetTimeZoneInformation (&tTZI) != TIME_ZONE_ID_INVALID )
				{
					// Localize system time:
					SYSTEMTIME systimelocal;
					if ( SystemTimeToTzSpecificLocalTime( &tTZI, &systime, &systimelocal ) )
					{
						strVal.Format (_T("%02d:%02d:%02d"), 
							systimelocal.wHour, systimelocal.wMinute, systimelocal.wSecond);			

						bSuccess = true;
					}
				}
			}
			if (!bSuccess)
				strVal = _T("???");
		}
		break;

	case VT_UI1	| VT_ARRAY:
	case VT_I1	| VT_ARRAY:
	case VT_UI2	| VT_ARRAY:
	case VT_I2	| VT_ARRAY:
	case VT_UI4	| VT_ARRAY:
	case VT_I4	| VT_ARRAY:
	case VT_R4	| VT_ARRAY:
	case VT_R8	| VT_ARRAY:
		{
			CSafeArray *pSafeArr = (CSafeArray *) vtVal.parray;
			DWORD dwCols = pSafeArr->GetNumCols ();
			DWORD dwSize = pSafeArr->GetByteLength ();
			ULONG cbElements = pSafeArr->cbElements;
			LPBYTE lpByte = (LPBYTE)pSafeArr->pvData;
			DWORD dwCol = 0;

			// Start row delimiter:
			strVal = L"[ ";

			// Cycle through the elements:
			for (DWORD i=0; i<dwSize; i+=cbElements, lpByte+=cbElements)
			{
				TCHAR szNum[32] = {0};

				// Format element according to data size:
				if (cbElements == 1)
				{
					if ( vtVal.vt == (VT_UI1 | VT_ARRAY) )
						StringCchPrintf( szNum, 32, _T("%u"), *lpByte);
					else
						StringCchPrintf( szNum, 32, _T("%d"), *(char *)lpByte);
				}
				else if (cbElements == 2)
				{
					if ( vtVal.vt ==	(VT_UI2 | VT_ARRAY) )
						StringCchPrintf ( szNum, 32, _T("%u"), *(WORD *)lpByte);
					else 
						StringCchPrintf (szNum, 32, _T("%d"), *(short *)lpByte);
				}
				else if (cbElements == 4)
				{
					if (vtVal.vt ==	(VT_R4	| VT_ARRAY))
						StringCchPrintf (szNum, 32, _T("%G"), *(float *)lpByte);
					else if (vtVal.vt ==	(VT_UI4	| VT_ARRAY))
						StringCchPrintf (szNum, 32, _T("%u"), *(DWORD *)lpByte);
					else if (vtVal.vt ==	(VT_I4	| VT_ARRAY))
						StringCchPrintf (szNum, 32, _T("%d"), *(DWORD *)lpByte);
				}

				else if (cbElements == 8)
					StringCchPrintf (szNum, 32, _T("%G"), *(double *)lpByte);
				else
				{
					ASSERT (FALSE);
				}

				// Delimit each element within the row with a comma:
				if (dwCol != 0)
					strVal += _T(", ");

				// Append the formatted element data:
				strVal += szNum;

				// Terminate each row (except the last):
				if (++dwCol == dwCols)
				{
					if (i < dwSize - cbElements)
						strVal += _T(" ] [ ");

					dwCol = 0;
				}
			}
			// End delimiter:
			strVal += _T(" ]");
		}
		break;
	default:
		// Unsupported datatype:
		strVal = _T ("<Bad VARTYPE>");
		break;
	}

	// Copy value to output buffer:
	lstrcpyn (pBuffer, strVal, nBufLen);
}

void COPCConnection::AddLeaves (LPENUMSTRING pIEnumString,bool bProperty,int nMutilLoad,int nSleep,int nMutilAdd)
{
	try
	{
#ifndef _UNICODE
		TCHAR szBuffer[DEFBUFFSIZE];
#endif

		HRESULT		res_next;
		LPWSTR* pName = new LPWSTR[nMutilLoad]; 
		ULONG count = 0;
		vector<wstring> vecAllName;
		pIEnumString->Reset();
		do
		{
			res_next = pIEnumString->Next(nMutilLoad, pName, &count);
			int nErrCode = GetLastError();
			// CString does translation from UNICODE to native type
			// (depends on the way the application is built)
			for( ULONG index=0; index<count; index++ )
			{
				wstring itemname = pName[index];
				vecAllName.push_back(itemname);
				CoTaskMemFree(pName[index]);
			}			
		}
		while(res_next == S_OK);
		delete[] pName;

		//
		if(m_lpRecDataProc)
		{
			m_lpRecDataProc(0,0,m_dwUser,NULL);
		}
		m_searchmap.clear();
		IOPCItemProperties *pIOPCItemProperties = m_pServer->GetIItemProps();
		IOPCBrowseServerAddressSpace* pIBrowser = m_pServer->GetIBrowse();
		//判断下是否InsightOPC
		bool bInsight = false;
		CString strProgName = m_strserverprogid.c_str();
		if(strProgName.Find(_T("Insight.OPCServerDA")) >= 0)
			bInsight = true;
		m_nTotalCount = vecAllName.size();
		for(int i=0; i<vecAllName.size(); ++i)
		{
			wstring itemname = vecAllName[i];
			CString strItemName = itemname.c_str();
			LPWSTR pItemID = T2OLE(strItemName.GetBuffer(0));
			if(bInsight)			//只输出后缀名为_CV的点
			{
				if(strItemName.Find(_T("_CV")) >= 0)
				{
					CKItem* pItem = new CKItem(m_pGroup);
					pItem->SetAccessPath(_T(""));
					pItem->SetItemID(strItemName);
					pItem->SetActive(TRUE);
					m_searchmap.insert(make_pair(itemname, pItem));
				}
				else if(strItemName.Find(_T("_DESC")) >= 0 && bProperty)
				{
					CString strItemN = strItemName;
					strItemN.Replace(_T("_DESC"),_T("_CV"));
					CKItem* pItem = FindItem(strItemN.GetString());
					if(pItem)
					{
						DWORD dwCount;
						DWORD *dwPropertyIDs;
						LPWSTR *ppDescriptions;
						VARTYPE *ppvtDataTypes;
						VARIANT *ppvData;
						HRESULT *ppErrors;
						HRESULT hr = pIOPCItemProperties->QueryAvailableProperties(pItemID, &dwCount, &dwPropertyIDs, &ppDescriptions, &ppvtDataTypes);
						hr = pIOPCItemProperties->GetItemProperties(pItemID, dwCount, dwPropertyIDs, &ppvData, &ppErrors);
						if(hr == S_OK)
						{
							if(dwCount >= 1)
							{
								VARIANT value = ppvData[1];
								CString strDescription = VariantToString(value);
								pItem->SetItemDescription(strDescription);
							}
						}	
						Sleep(10);
					}
				}
			}
			else
			{
				CKItem* pItem = new CKItem(m_pGroup);
				//
				if(bProperty)
				{
					DWORD dwCount;
					DWORD *dwPropertyIDs;
					LPWSTR *ppDescriptions;
					VARTYPE *ppvtDataTypes;
					VARIANT *ppvData;
					HRESULT *ppErrors;

					HRESULT hr = pIOPCItemProperties->QueryAvailableProperties(pItemID, &dwCount, &dwPropertyIDs, &ppDescriptions, &ppvtDataTypes);
					if(hr == S_OK)
					{
						hr = pIOPCItemProperties->GetItemProperties(pItemID, dwCount, dwPropertyIDs, &ppvData, &ppErrors);

						CString strDescription = _T("");
						if(hr == S_OK)
						{
							for (int dwIndex = 0; dwIndex<dwCount; dwIndex++)
							{
								if (ppDescriptions && ppDescriptions[dwIndex]) 
								{				
									CString str = ppDescriptions[dwIndex];
									if(str.Find(_T("Item Description"))>=0)
									{
										VARIANT value = ppvData[dwIndex];
										strDescription = VariantToString(value);
									}
									//可以增加其他属性
									CoTaskMemFree(ppDescriptions[dwIndex]);
								}
							}
						}
						pItem->SetItemDescription(strDescription);
					}

					if(i%20==0 && m_lpRecDataProc)
					{					
						m_lpRecDataProc(i,m_nTotalCount,m_dwUser,NULL);
					}
					Sleep(10);
				}
				pItem->SetAccessPath(_T(""));
				pItem->SetItemID(strItemName);
				pItem->SetActive(TRUE);
				m_searchmap.insert(make_pair(itemname, pItem));
			}
		}
		//插入点
		m_itemlist.RemoveAll();
		m_nCurrentCount = 0;
		map<wstring, CKItem*>::iterator iterItem = m_searchmap.begin();
		CObArray	itemList;	//保存所有的Item
		while(iterItem != m_searchmap.end())
		{
			CKItem* pItemAdd = (*iterItem).second;
			if(pItemAdd)
			{
				itemList.Add(pItemAdd);
				m_itemlist.Add(pItemAdd);
				DWORD dwSize = itemList.GetCount();
				if(dwSize >= nMutilAdd)
				{
					m_pGroup->AddItems(itemList, dwSize);
					itemList.RemoveAll();
					Sleep(nSleep);
				}
				m_nCurrentCount++;
				if(m_nCurrentCount%20==0 && m_lpRecDataProc)
				{
					wstring strItemName = pItemAdd->GetItemID();
					string strItemID_Ansi = Project::Tools::WideCharToAnsi(strItemName.c_str());
					memset(m_cItemName,0,256);
					memcpy(m_cItemName,strItemID_Ansi.c_str(),strItemID_Ansi.length());
					m_lpRecDataProc(m_nCurrentCount,m_nTotalCount,m_dwUser,(char*)m_cItemName);
				}
			}
			++iterItem;
		}
		DWORD dwSize = itemList.GetCount();
		if(dwSize>0)
		{
			m_pGroup->AddItems(itemList, dwSize);
			itemList.RemoveAll();
			Sleep(nSleep);
		}
	}
	catch (...)
	{
	}	
}

CString COPCConnection::VariantToString(_variant_t var)
{
	CString strValue;   
    _variant_t var_t;   
    _bstr_t bstr_t;   
  
    COleCurrency var_currency;   
  
    switch(var.vt)   
  
    {   
  
    case VT_EMPTY:   
  
    case VT_NULL:strValue=_T("");break;   
  
    case VT_UI1:strValue.Format(_T("%d"),var.bVal);break;   //bool   
  
    case VT_I2:strValue.Format(_T("%d"),var.iVal);break;    //int   
  
    case VT_I4:strValue.Format(_T("%d"),var.lVal);break;    //long   
  
    case VT_R4:strValue.Format(_T("%f"),var.fltVal);break;  //float   
  
    case VT_R8:strValue.Format(_T("%f"),var.dblVal);break;  //   
  
    case VT_CY:   
        var_currency=var;   
        strValue=var_currency.Format(0);break;   
  
    case VT_BSTR:   
        var_t =var;   
        bstr_t=var_t;   
		strValue.Format(_T("%s"),Project::Tools::AnsiToWideChar((const char *)bstr_t).c_str());   
        break;   
  
    case VT_DATE:           //时间类型   
        {   
            CTime  myTime(((COleDateTime)var).GetYear(),    
                ((COleDateTime)var).GetMonth(),    
                ((COleDateTime)var).GetDay(),    
                ((COleDateTime)var).GetHour(),    
                ((COleDateTime)var).GetMinute(),    
                ((COleDateTime)var).GetSecond());    
            strValue = myTime.Format(_T("%Y-%m-%d %H:%M:%S"));   
        }   
        break;   
  
    case VT_BOOL:strValue.Format(_T("%d"),var.boolVal);   
        break;   
  
    default:strValue=_T("");break;   
  
    }   
  
    return strValue; 
}

void COPCConnection::SetServerProgName(const wstring& strProgID)
{
	m_strserverprogid = strProgID;
}

bool COPCConnection::Connect(wstring strOPCServerIP)
{
	if(strOPCServerIP.length()>0)
		m_strserverhost = strOPCServerIP;

	CString progid = m_strserverprogid.c_str();
	CString serverhost = m_strserverhost.c_str();
	if (NULL == m_pServer)
	{
		_tprintf(_T("连接到OPC服务器:"));
		_tprintf(serverhost.GetString());
		_tprintf(L"\r\n");
		m_pServer = new CKServer(progid, serverhost);
		if (NULL == m_pServer){
			return false;
		}
	}

	m_isconnected = m_pServer->Connect();
	return m_isconnected;
}

void COPCConnection::DisConnect(void)
{
	m_isconnected = false;
	m_lpRecDataProc = NULL;
	m_nCurrentCount = 0;
	m_nTotalCount = 0;
	if (NULL != m_pServer){
		m_pServer->Disconnect();
	}
	m_itemlist.RemoveAll();
	//clear item;
	map<wstring, CKItem*>::iterator it = m_searchmap.begin();
	for(;it!=m_searchmap.end();it++)
	{
		CKItem *pitem = (*it).second;
		if(pitem)
		{
			delete(pitem);
			pitem = NULL;
		}
	}

	m_searchmap.clear();
	m_searchEntrymap.clear();
}

void COPCConnection::AddGroup(void)
{
	if (NULL == m_pGroup){
		m_pGroup = new CKGroup(m_pServer);
	}

	m_pGroup->SetName(_T("beopdebug"));
	m_pGroup->SetActive(TRUE);
	m_pGroup->SetLanguageID(0);
	m_pGroup->SetDeadband(0);
	m_pGroup->SetBias(0);
	m_pGroup->SetUpdateRate(100);
	m_pGroup->SetUpdateMethod(2);

	m_pServer->AddGroup(m_pGroup);
}

bool COPCConnection::AddItems(bool bProperty,int nBrowseType,wstring strFilter,int nMutilLoad,int nSleep,VARTYPE vtDataTypeFilter,int nMutilAdd,LPAddPointProc lpRecDataProc,DWORD userdata )
{
	m_lpRecDataProc = lpRecDataProc;
	m_dwUser = userdata;
	int			 nPos	= 0;
	HRESULT		 hr		= 0;
	LPENUMSTRING pIEnumString = NULL;
	CStringArray strBranches;
	m_strFilterLeaf = strFilter;

#ifndef _UNICODE
	WCHAR szFilter [DEFBUFFSIZE];
#endif
	hr = S_OK;

	// > 0 we do not want to include the "Root" item since the client
	// only uses this branch:
	IOPCBrowseServerAddressSpace* pIBrowser = m_pServer->GetIBrowse();
	do
	{
		hr = pIBrowser->ChangeBrowsePosition (OPC_BROWSE_UP, L"\0");
	} while (SUCCEEDED (hr));

	// Now browse down to the new position:
	strBranches.SetSize ( nPos + 1 );
	// 	hParentItem = hItem;

	for (int i = 0; i<=nPos; i++){
		strBranches.Add(m_strserverprogid.c_str());
	}

	while (SUCCEEDED(hr) && nPos-->0)
	{
#ifdef _UNICODE
		hr = pIBrowser->ChangeBrowsePosition(OPC_BROWSE_DOWN, strBranches[nPos]);
#else
		WCHAR szBranch [DEFBUFFSIZE];

		_mbstowcsz (szBranch, strBranches [nPos], sizeof (szBranch) / sizeof (WCHAR));
		hr = pIBrowser->ChangeBrowsePosition (OPC_BROWSE_DOWN, szBranch);
#endif
	}

	// Browse for root level:
#ifdef _UNICODE
	hr = pIBrowser->BrowseOPCItemIDs(
									//m_bBrowseFlat ? OPC_FLAT : OPC_LEAF,	// provide items with children
									(OPCBROWSETYPE)nBrowseType,
									m_strFilterLeaf.c_str(),						// item id filtering
									vtDataTypeFilter,							// datatype filter
									0,					// access rights filtering
									&pIEnumString);							// store the interface pointer here
#else
	_mbstowcsz (szFilter, m_strFilterLeaf, sizeof (szFilter) / sizeof (WCHAR));

	hr = pIBrowser->BrowseOPCItemIDs (
		(OPCBROWSETYPE)nBrowseType,	// provide items with children
		szFilter,								// item id filtering
		vtDataTypeFilter,							// datatype filter
		0,					// access rights filtering
		&pIEnumString);							// store the interface pointer here
#endif

	// On success add the branches to the root:
	if (SUCCEEDED(hr) && pIEnumString){
		AddLeaves(pIEnumString,bProperty,nMutilLoad,nSleep,nMutilAdd);
		pIEnumString->Release();

		return true;
	}
	else
		return false;
}

bool COPCConnection::GetItemValue(const wstring& strItemID, wstring &strValue)
{
	if (strItemID.empty()){
		return false;
	}
	CKItem *pItem = FindItem(strItemID);
	if (NULL == pItem){
		//没有找到此节点
		CString logoutput = _T("Error:(OPC)");
		logoutput += strItemID.c_str();
		logoutput += _T(" does not exist\n");
		//_tprintf(logoutput);
		return false;
	}

	wstring str_accesspath = pItem->GetAccessPath();
	int vtdatatype	= pItem->GetDataType();
	wstring str_ItemID		= pItem->GetItemID();
	IOPCItemProperties* pItempPro	= m_pServer->GetIItemProps();

	if(pItempPro==NULL)
	{
		//_tprintf(L"IOPCItemProperties OPC return NULL.\r\n");
		return false;
	}

	// Fill available 2.0 item properties (We won't be able to do this
	// unless we have a pointer to the item properties interface.)
	if (pItempPro != NULL)
	{
		TCHAR szBuffer[DEFBUFFSIZE] = {0};
		DWORD	dwCount			= 0;
		DWORD	dwIndex			= 0;
		DWORD	*pdwIDs			= NULL;
		LPWSTR	*pszDescriptions= NULL;
		LPWSTR	*pszLookupIDs	= NULL;
		VARTYPE *pvtDataTypes	= NULL;
		VARIANT *pvtValues		= NULL;
		HRESULT *pValErrs		= NULL;
		HRESULT *pLookupErrs	= NULL;
		HRESULT	hr				= E_FAIL;
		CString strTemp			= _T("");
		wstring	strQualifiedID  = _T("");

		// Obtain the qualified ID:
		ASSERT (!str_ItemID.empty());

		if (!str_accesspath.empty() && m_pServer->IsKepServerEx()){
			strQualifiedID = str_accesspath;
			strQualifiedID += _T(".");
			strQualifiedID += str_ItemID;
		}
		else{
			strQualifiedID = str_ItemID;
		}

		if (SUCCEEDED(hr))
		{
			for (dwIndex = 0; dwIndex<dwCount; dwIndex++)
			{
				// ID:
				_itot_s(pdwIDs[dwIndex], szBuffer, 10);
				// {roperty value:
				if (pValErrs && SUCCEEDED (pValErrs[dwIndex]))
				{
					GetValue(pvtValues[dwIndex], szBuffer, sizeof(szBuffer)/sizeof(TCHAR));
				}
				else
				{
					lstrcpyn(szBuffer, _T("???"), sizeof(szBuffer)/sizeof(TCHAR));
				}

				// Lookup item ID (may have to convert from UNICODE):
				if (pLookupErrs && SUCCEEDED(pLookupErrs[dwIndex]))
				{
#ifdef _UNICODE
					lstrcpyn(szBuffer, pszLookupIDs [dwIndex], sizeof(szBuffer)/sizeof(TCHAR));
#else
					_wcstombsz(szBuffer, pszLookupIDs [dwIndex], sizeof(szBuffer)/sizeof(TCHAR));
#endif
				}
				else
				{
					// No lookup item ID, so place "N/A" in list control:
					lstrcpyn(szBuffer, _T("N/A"), sizeof(szBuffer)/sizeof (TCHAR));
					pszLookupIDs[dwIndex] = NULL;
				}
			}
		}
		pItem->GetValue(strTemp);
		strValue = strTemp;

		// COM requires us to free memory for data passed back to us:
		for (dwIndex = 0; dwIndex<dwCount; dwIndex++)
		{
			if (pszDescriptions && pszDescriptions[dwIndex]) 
				CoTaskMemFree(pszDescriptions[dwIndex]);

			if (pszLookupIDs && pszLookupIDs[dwIndex])
				CoTaskMemFree(pszLookupIDs[dwIndex]);

			// Clear variants:
			if (pvtValues)
				VariantClear(&pvtValues[dwIndex]);
		}

		if (pdwIDs)
			CoTaskMemFree(pdwIDs);

		if (pszDescriptions)
			CoTaskMemFree(pszDescriptions);

		if (pszLookupIDs)
			CoTaskMemFree(pszLookupIDs);

		if (pvtDataTypes)
			CoTaskMemFree(pvtDataTypes);

		if (pvtValues)
			CoTaskMemFree(pvtValues);

		if (pValErrs)
			CoTaskMemFree(pValErrs);

		if (pLookupErrs)
			CoTaskMemFree(pLookupErrs);

		/*if (pszItemID)
			CoTaskMemFree(pszItemID);*/
	}

	return true;
}

bool COPCConnection::SetItemValue(const wstring& strItemID, const wstring& strValue)
{
	if (strItemID.empty()){
		return false;
	}

	Project::Tools::Scoped_Lock<Mutex>  scopelock(m_lock);
	CKItem *pItem = FindItem(strItemID);

	//没有找到此节点
	if (NULL == pItem){

		return false;
	}

	try
	{
		CObArray		cItems;
		CStringArray	cValues;
		DWORD			cdwItems	= 0;

		// Construct an item and value list for this write operation:
		cItems.SetSize (1);
		cValues.SetSize (1);
		ASSERT(pItem != NULL);
		cItems[cdwItems] = pItem;
		cValues[cdwItems] = strValue.c_str();
		++cdwItems;
		m_pGroup->WriteAsync20(cItems, cValues, cdwItems);
	}
	catch (...)
	{
	}
	
	return true;
}

bool COPCConnection::UpdateItemValue(const wstring& strItemID, wstring &strValue)
{
    //CWaitCursor wc;
    CKItem *pItem = FindItem(strItemID);
    //没有找到此节点
    if (NULL == pItem)
        return false;
    try
    {
        CObArray		cItems;
        CStringArray	cValues;
        DWORD			cdwItems	= 0;

        // Construct an item and value list for this write operation:
        cItems.SetSize (1);
        cValues.SetSize (1);

        ASSERT(pItem != NULL);
        cItems[cdwItems] = pItem;

        // Add corresponding write value to value array:
        cValues[cdwItems] = strValue.c_str();
        ++cdwItems;
        m_pGroup->ReadAsync20(cItems, cdwItems);
    }
    catch (...)
    {
    }

    CString value; 
    pItem->GetValue(value);
    strValue = (LPCTSTR)value;
	return true;
}

VARTYPE COPCConnection::GetVarType( const wstring& strItemName )
{
	VARTYPE result = VT_EMPTY;
	CKItem *pItem = FindItem(strItemName);
	
	if (pItem){
		return pItem->GetDataType();
	}
	return result;
}

CKItem* COPCConnection::FindItem( const wstring& strItemName )
{
	if (strItemName.empty())
		return NULL;

	map<wstring, CKItem*>::iterator it = m_searchmap.find(strItemName);
	if (it != m_searchmap.end())
	{
		CKItem *pitem =  (*it).second;
		return pitem;
	}

	return NULL;
}

wstring COPCConnection::GetServerProgName() const
{
	return m_strserverprogid;
}

LPCTSTR COPCConnection::GetVarQuality( const wstring& strItemName )
{
	CKItem *pItem = FindItem(strItemName);
	if (pItem){
		return pItem->GetQuality();
	}

	return _T("");
}

bool COPCConnection::SearchOPC( wstring strOPCServerIP,hash_map<wstring,CLSID>& clsidmap)
{
	//strOPCServerIP = L"localhost";
	try
	{
		bool bResult = false;
		MULTI_QI mqi;
		COSERVERINFO sin,*sinptr;
		DWORD clsctx;
		CString strError;
		unsigned long c = 0;
		CLSID clsid;
		IEnumGUID *pEnumGUID;
		CLSID catid = CATID_OPCDAServer20;
		IOPCServerList *gpOPC = NULL;
		clsid = CLSID_OPCServerList;
		sinptr = &sin;
		sin.dwReserved1 = sin.dwReserved2 = 0;
		sin.pwszName =  (LPWSTR)(LPCWSTR)strOPCServerIP.c_str();
		sin.pAuthInfo = NULL;
		clsctx = CLSCTX_ALL;
		mqi.pIID = &IID_IOPCServerList;
		mqi.hr = 0;
		mqi.pItf = NULL;
		HRESULT hr = CoCreateInstanceEx(clsid,NULL,clsctx,sinptr,1,&mqi);
		if(SUCCEEDED(hr))
		{
			if (FAILED(mqi.hr))
			{
				//Failed to connect to server
				strError.Format(_T("未能连接到服务器(错误代码:0x%x)"), mqi.hr);
				OutOPCEnumErrLog(strError.GetString());
			}
			gpOPC = (IOPCServerList *)mqi.pItf;
			if(gpOPC)
			{
				hr = gpOPC->EnumClassesOfCategories(1,&catid,1,&catid,&pEnumGUID);
				clsidmap.clear();
				if(SUCCEEDED(hr))
				{
					while(S_OK == pEnumGUID->Next(1,&clsid,&c))
					{
						LPOLESTR pszProgID;
						LPOLESTR pszUserType;
						hr = gpOPC->GetClassDetails(clsid,&pszProgID,&pszUserType);
						if(hr == S_OK)
						{
							CString str;
							str.Format(_T("%s"),pszProgID);
							clsidmap[str.GetString()] = clsid;
							bResult = true;
						}
						else
						{
							strError.Format(_T("GetClassDetails()失败(错误代码:0x%x)"), hr);
							OutOPCEnumErrLog(strError.GetString());
						}
						CoTaskMemFree(pszProgID);
						CoTaskMemFree(pszUserType);
					}
				}
				else
				{
					//EnumClassesOfCategories
					strError.Format(_T("EnumClassesOfCategories()失败(错误代码:0x%x"), hr);
					OutOPCEnumErrLog(strError.GetString());
				}
				if(gpOPC)
					gpOPC->Release();
			}
		}
		else
		{
			//CoCreateInstanceEx() fail
			strError.Format(_T("CoCreateInstanceEx() 失败 (错误代码:0x%x)"), hr);
			OutOPCEnumErrLog(strError.GetString());
		}
		return bResult;
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		CString strFormatted;

		e->GetErrorMessage(szCause, 255);
		strFormatted.Format(_T("SearchOPC 错误:(异常)%s\n"),szCause);
		OutOPCEnumErrLog(strFormatted.GetString());
	}
}

bool COPCConnection::SearchOPC_( wstring strOPCServerIP ,hash_map<wstring,CLSID>& clsidmap,int nType)
{
	try
	{
		USES_CONVERSION;
		clsidmap.clear();
		CString	sNode = strOPCServerIP.c_str();
		//1. connect to server list component
		CComPtr<IOPCServerList>	pServerList;
		COSERVERINFO			si;
		MULTI_QI				qi;
		HRESULT					hRes;
		CString strError;
		si.dwReserved1 = 0;
		if(sNode.IsEmpty())
			si.pwszName = NULL;
		else
			si.pwszName = T2OLE(sNode.GetBuffer(0));
		si.pAuthInfo = NULL;
		si.dwReserved2 = 0;
		qi.pIID = &IID_IOPCServerList;
		qi.pItf = NULL;
		qi.hr = 0;

		hRes = CoCreateInstanceEx(CLSID_OPCServerList, NULL, CLSCTX_ALL, &si, 1, &qi);
		if (FAILED(hRes))
		{
			strError.Format(_T("CoCreateInstanceEx(ErrCode:0x%x)"), hRes);
			OutOPCEnumErrLog(strError.GetString());
			return FALSE;
		}
		if (FAILED(qi.hr))
		{
			strError.Format(_T("未能连接到服务器(错误代码:0x%x)"), qi.hr);
			OutOPCEnumErrLog(strError.GetString());
			return FALSE;
		}
		
		pServerList = (IOPCServerList*) qi.pItf;
		qi.pItf->Release();
		//2. enumerate the list
		CComPtr<IEnumGUID>	pEnumGUID;

		CLSID m_catid;
		if(nType == 1)
		{
			m_catid = CATID_OPCDAServer10;
		}
		else
		{
			m_catid = CATID_OPCDAServer20;
		}
		hRes = pServerList->EnumClassesOfCategories(1, &m_catid, 1, &m_catid, &pEnumGUID);
		if(SUCCEEDED(hRes))
		{
			/*
			how many elemets to retrieve for each Next()
			not configurable in options (we are dealing with only a few servers)
			*/
			const unsigned long ulNext = 100;  
			unsigned long count = 0;
			CLSID clsid[ulNext];

			do
			{
				hRes = pEnumGUID->Next(ulNext, clsid, &count);
				for(unsigned long index = 0; index < count; index++)
				{
					LPOLESTR pszProgID;
					LPOLESTR pszUserType;
					if(SUCCEEDED(pServerList->GetClassDetails(clsid[index], &pszProgID, &pszUserType)))
					{				   
						CString sOPCServer;
						sOPCServer.Format( _T("%s  (%s)"), OLE2A(pszProgID), OLE2A(pszUserType));

						CString str;
						str.Format(_T("%s"),pszProgID);
						clsidmap[str.GetString()] = *clsid;
					
						CoTaskMemFree(pszProgID);
						CoTaskMemFree(pszUserType);
					}
				}
			}
			while(hRes == S_OK);

			pEnumGUID.Release();
		}
		else
		{
			if(nType == 1)
			{
				strError.Format(_T("EnumClassesOfCategories(CATID_OPCDAServer10)(错误代码:0x%x)"), hRes);
				OutOPCEnumErrLog(strError.GetString());
			}
			else
			{
				strError.Format(_T("EnumClassesOfCategories(CATID_OPCDAServer20)(错误代码:0x%x)"), hRes);
				OutOPCEnumErrLog(strError.GetString());
			}
		}

		//3. disconnect
		pServerList.Release();
		return TRUE;
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		CString strFormatted;

		e->GetErrorMessage(szCause, 255);
		strFormatted.Format(_T("SearchOPC 错误:(异常)%s\n"),szCause);
		OutOPCEnumErrLog(strFormatted.GetString());
	}
	return false;
}

bool COPCConnection::GetAllOPCMap( map<wstring, CKItem*>& searchmap )
{
	searchmap = m_searchmap;
	return m_isconnected;
}

bool COPCConnection::ConnectRemoteOPC( wstring strOPCServerIP ,CLSID clsid1)
{
	if(strOPCServerIP.length()>0)
		m_strserverhost = strOPCServerIP;

	CString progid = m_strserverprogid.c_str();
	CString serverhost = m_strserverhost.c_str();
	if (NULL == m_pServer)
	{
		_tprintf(_T("连接到OPC服务器:"));
		_tprintf(serverhost.GetString());
		_tprintf(L"\r\n");
		m_pServer = new CKServer(progid, serverhost);
		if (NULL == m_pServer){
			return false;
		}
	}
	m_isconnected = m_pServer->ConnectRemote(clsid1);
	//m_isconnected = m_pServer->Connect();
	return m_isconnected;
}

bool COPCConnection::OutOPCEnumErrLog( wstring strOut )
{
	wstring strFilePath;
	Project::Tools::GetSysPath(strFilePath);

	CString strLogPath;
	strLogPath.Format(_T("%s\\OPCEnumErr.log"),strFilePath.c_str());
	CStdioFile	ff;
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_ALL, "chs" );
	if(ff.Open(strLogPath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		wstring strTime;
		Project::Tools::SysTime_To_String(st,strTime);
		CString str;
		str.Format(_T("%s::%s...\n"),strTime.c_str(),strOut.c_str());
		ff.Seek(0,CFile::end);
		ff.WriteString(str);
		ff.Close();
		setlocale( LC_ALL, old_locale );
		free(old_locale);
		return true;
	}
	return false;
}

void COPCConnection::TryBrowseBranch( IOPCBrowseServerAddressSpace *pBSAS, LPCWSTR lpprestr,LPCWSTR lpcw )
{
	//HRESULT r1;
	//OPCNAMESPACETYPE onst;
	//LPWSTR pItemID;
	//WCHAR s,*lp;
	//IEnumString *pEnum = 0;

	//char szBuf[40];
	//long nPos = 1;
	//HRESULT r2 = S_OK;
	//LPOLESTR pStr;
	//ULONG actual;
	//				  　　
	////lp = WSTRFromSBCS("%ls,%ls",pIMalloc);
	//// 向lpcw节点的枝叶改变位置
	//pBSAS->ChangeBrowsePosition(OPC_BROWSE_DOWN,lpcw);
	//r1 = pBSAS->BrowseOPCItemIDs(OPC_FLAT,&s,VT_EMPTY,0,&pEnum);
	//pBSAS->QueryOrganization(&onst);
	//						  　　
	//// 提取该节点下的所有枝叶
	//while((r2 = pEnum->Next(1, &pStr, &actual)) == S_OK)
	//{
	//	printf("%ls%ls--%ls\n",lpprestr,lpcw, pStr);
	//	wprintf(lpprestr,lp,lpcw,pStr);
	//	if(onst == OPC_BRANCH)
	//		TryBrowseBranch(pBSAS,lpprestr,pStr);
	//}
	//// pBSAS->ChangeBrowsePosition(OPC_BROWSE_DOWN,
	//// printf("OPC_NS_HIERARCHIAL 含有枝干");
	//// get firset item
	//pIMalloc->Free(lp);
	//pIMalloc->Free(pStr);
	//pEnum->Release();
	//// 回到名叫lpcw的主干位置
	//pBSAS->ChangeBrowsePosition(OPC_BROWSE_UP,lpcw);
}

bool COPCConnection::ReadItemProperty( const wstring& strItemID )
{
	return true;
}

bool COPCConnection::GetPrograss( DWORD& nCurrentCount,DWORD& nTotalCount )
{
	nCurrentCount = m_nCurrentCount;
	nTotalCount = m_nTotalCount;
	return true;
}
