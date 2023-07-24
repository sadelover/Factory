// CommBacnetSearch.cpp : implementation file
//

#include "stdafx.h"
#include "CommBacnetSearch.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"
#include "BacnetSettingDlg.h"
#include "ExportBacnetIDDlg.h"
#include "DB_BasicIO/Sqlite/SqliteAcess.h"
#include "SelectBacnetDlg.h"
#include "BacnetSetDlg.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

object_functions_t Object_Table_[] = {
	{DEVICE_OBJ_FUNCTIONS},
	{MAX_BACNET_OBJECT_TYPE, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
};

struct property_value_list_t {
	int32_t property_id;
	BACNET_APPLICATION_DATA_VALUE *value;
};
static struct property_value_list_t Property_Value_List[] = {
	//{PROP_OBJECT_NAME, NULL},
	//{PROP_PRESENT_VALUE, NULL},
	//{PROP_DESCRIPTION, NULL},
	{PROP_ALL,NULL},
	//{PROP_REQUIRED,NULL},
	//{PROP_DESCRIPTION, NULL},
	{-1, NULL}
};

static const char first_char[4] = { 0x00, 0xC0, 0xE0, 0xF0 };
static const int  g_nBacnetReqHeaderLength = 57;
static const int  g_nBacnetOneReqLength = 11;

int			CCommBacnetSearch::m_nReadMode;
int			CCommBacnetSearch::m_nCharacterType;
CViewTree	CCommBacnetSearch::m_wndFileView;
HTREEITEM	CCommBacnetSearch::m_hRoot;
HTREEITEM	CCommBacnetSearch::m_hSelectNode;
CRITICAL_SECTION CCommBacnetSearch::m_criticalSection = {0};
hash_map<wstring,BacnetStructInfo*>	CCommBacnetSearch::m_mapDeveice;
CString		CCommBacnetSearch::m_strSavepath;
vector<DWORD>	CCommBacnetSearch::m_vecDeviceID;
HANDLE		CCommBacnetSearch::m_event;
bool		CCommBacnetSearch::m_bBusy;	
bool		CCommBacnetSearch::m_bSearch;
int			CCommBacnetSearch::m_nEvent;
DWORD		CCommBacnetSearch::m_dwCurrentOldDeviceID;		//当前查询设备ID
COleDateTime CCommBacnetSearch::m_oleSendTime;				//发送和接收时候更新下
COleDateTime CCommBacnetSearch::m_oleRecTime;		
int			CCommBacnetSearch::m_nTimeOut;
int			CCommBacnetSearch::m_nRetryReadCount;
wstring     CCommBacnetSearch::m_strSelectKey;
BacnetStructInfo* CCommBacnetSearch::m_pCurrentBacnet;
CString		CCommBacnetSearch::m_strLOG;
CEdit		CCommBacnetSearch::m_EditLog;
int			CCommBacnetSearch::m_nReadDeviceCount;		//已读取多少个设备（导出时候）
int			CCommBacnetSearch::m_nDeviceCount;
bool		CCommBacnetSearch::m_bUpdateProgress;
int			CCommBacnetSearch::m_nBacnetScanInterval;
bool		CCommBacnetSearch::m_bStoreS3db;
vector<wstring>	CCommBacnetSearch::m_vecDeviceIPID;
bool		CCommBacnetSearch::m_bExistS3dbModel;
int			CCommBacnetSearch::m_nResponseTimeOut;
int			CCommBacnetSearch::m_nCmdInterval;
int			CCommBacnetSearch::m_nMutilReadCount;
bool		CCommBacnetSearch::m_bEndEndFile;
BacnetStructInfo* CCommBacnetSearch::m_pBacnetFile;

bool CompareDeviceIPID(const wstring p1, const wstring p2)
{
	return p1 < p2;
}

// CCommBacnetSearch dialog

IMPLEMENT_DYNAMIC(CCommBacnetSearch, CDialog)

CCommBacnetSearch::CCommBacnetSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CCommBacnetSearch::IDD, pParent)
	, m_readdata_thread(NULL)
	, m_read_thread(NULL)
	, m_timeout_thread(NULL)
	, m_progress_thread(NULL)
	, m_exit_readdata_thread(false)
{
	m_bBusy = false;
	m_bSearch = false;
	m_nTimeOut = 5;
	m_nRetryReadCount = 2;
	m_strSelectKey = L"";
	m_pCurrentBacnet = NULL;
	m_strLOG = _T("");
	m_strSavepath = _T("");
	m_nReadDeviceCount = 0;
	m_nBacnetScanInterval = 300;
	m_nDeviceCount = 0;
	m_bUpdateProgress = false;
	m_bStoreS3db = false;
	m_oleSendTime =  COleDateTime::GetCurrentTime();
	m_oleRecTime =  COleDateTime::GetCurrentTime();
	m_event = ::CreateEvent(NULL, false, false, L"");

	CString strExePath,strSysBEOPFileName;
	Project::Tools::GetSysPath(strExePath);
	strSysBEOPFileName.Format(L"%s\\config\\beopsys.s3db", strExePath);
	fstream _file;
	_file.open(Project::Tools::WideCharToAnsi(strSysBEOPFileName).c_str(),ios::in);
	m_bExistS3dbModel = false;
	if(_file)
		m_bExistS3dbModel = true;

	//////////////////////////////////////////////////////////////////////////
	m_nMutilReadCount = 19;
	m_nCmdInterval = 1;
	m_nResponseTimeOut = 2000;
	//////////////////////////////////////////////////////////////////////////

	m_bEndEndFile = true;
	m_pBacnetFile = NULL;

	InitializeCriticalSection(&m_criticalSection);  //初始化临界区
}

CCommBacnetSearch::~CCommBacnetSearch()
{
}

void CCommBacnetSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_SEND, m_pProgressSend);
	DDX_Control(pDX, IDC_EDIT_LOG_COM, m_EditLog);
	DDX_Text(pDX, IDC_EDIT_LOG_COM, m_strLOG);
}


BEGIN_MESSAGE_MAP(CCommBacnetSearch, CDialog)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_BACNET_ROOT,BacnetSearch)
	ON_COMMAND(ID_MENU_BACNET_DEVICE,BacnetReadAllPoint)
	ON_COMMAND(ID_MENU_BACNET_POINT,BacnetReadPointProperty)
	ON_COMMAND(ID_MENU_BACNET_DEVICE_EXPORT,BacnetExportSelectOne)
	ON_COMMAND(ID_MENU_BACNET_SETTING,BacnetSetting)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_ALL,BacnetExportAll)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_ALL_FILE,BacnetExportAllToPointFile)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_SELECT,BacnetExportSelectMulti)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_CONTINUE,BacnetExportContinue_)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_BACNET,BacnetExportBacnetInfo)
	ON_COMMAND(ID_MENU_BACNET_SORT,BacnetSort)
END_MESSAGE_MAP()

BOOL CCommBacnetSearch::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

void CCommBacnetSearch::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);

		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);

			BacnetStructInfo *temStru=reinterpret_cast<BacnetStructInfo *>(pWndTree->GetItemData(hTreeItem));
			if(temStru == NULL)
				return;

			// 创建自定义菜单
			CMenu *pMenu = new CMenu();
			VERIFY(pMenu->CreatePopupMenu());

			if(temStru->type==0 && m_bBusy==false)		//根节点
			{
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_ROOT,_T("Bacnet搜索"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_SETTING,_T("Bacnet设置"));
				if(m_bSearch == true)
				{
					pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_SORT,_T("Bacnet排序"));
					pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_BACNET,_T("Bacnet输出设备信息"));
					pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_ALL,_T("Bacnet全部导出"));
					if(m_bExistS3dbModel)
						pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_ALL_FILE,_T("Bacnet导出全部到s3db"));
					pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_SELECT,_T("Bacnet导出选择"));
					pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_CONTINUE,_T("Bacnet继续导出"));
				}
			}
			else if(temStru->type==1 && m_bBusy==false)  //Bacnet设备
			{								
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_DEVICE,_T("读取设备列表"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_DEVICE_EXPORT,_T("导出设备点位"));
			}
			else if(m_bBusy==false)
			{
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_POINT,_T("读点属性"));
			}

			pMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
			pMenu->DestroyMenu();

			if(pMenu!=NULL)
			{
				delete pMenu;
				pMenu=NULL;
			}		
		}
	}

	pWndTree->SetFocus();
}

bool CCommBacnetSearch::Exit()
{
	m_exit_readdata_thread = true;
	m_vecDeviceID.clear();
	m_vecDeviceIPID.clear();
	hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.begin();
	while(iterDevice != m_mapDeveice.end())		//未找到显示
	{
		BacnetStructInfo* info = (*iterDevice).second;
		if(info != NULL)
		{
			hash_map<wstring,BacnetStructInfo*>::iterator iterPoint = info->mapPoint.begin();
			while(iterPoint != info->mapPoint.end())
			{
				BacnetStructInfo* info1 = (*iterPoint).second;
				if(info1 != NULL)
				{
					delete info1;
					info1 = NULL;
				}
				++iterPoint;
			}
			info->mapPoint.clear();
			info->mapArrayIndex.clear();
			delete info;
			info = NULL;
		}
		++iterDevice;
	}
	m_mapDeveice.clear();
	WaitForSingleObject(m_readdata_thread, 2000);
	SetEventHandle(0);
	WaitForSingleObject(m_read_thread, 2000);
	WaitForSingleObject(m_progress_thread,2000);
	WaitForSingleObject(m_timeout_thread,2000);
	if (m_readdata_thread){
		CloseHandle(m_readdata_thread);
		m_readdata_thread = NULL;
	}

	if (m_read_thread){
		CloseHandle(m_read_thread);
		m_read_thread = NULL;
	}


	datalink_cleanup();
	return true;
}

bool CCommBacnetSearch::InitData(bool bConnected /*= true*/)
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, IDC_TREE))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	CRect rectClient;
	GetClientRect(rectClient);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 20, rectClient.top+30, 250, rectClient.Height() - 50, SWP_NOACTIVATE | SWP_NOZORDER);

	BacnetStructInfo* bacnetTreeNode = new BacnetStructInfo;
	bacnetTreeNode->type = 0;
	HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T("Bacnet搜索"),0,0,0,0,reinterpret_cast<DWORD>(bacnetTreeNode),TVI_ROOT,NULL);

	return true;
}

void CCommBacnetSearch::BacnetSearch()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}
	m_bBusy = true;
	AddLog(_T("Bacnet搜索...\r\n"));
	Exit();
	m_exit_readdata_thread = false;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	DeleteNode(m_hSelectNode,true);
	Device_Set_Object_Instance_Number(BACNET_MAX_INSTANCE);
	address_init(m_nReadMode);
	InitServiceHandlers();
	dlenv_init();
	atexit(datalink_cleanup);
	CreateThreadRead();
	m_bSearch = true;
	m_bBusy = false;
}

bool CCommBacnetSearch::DeleteNode(HTREEITEM hItem,bool bReleaseNode/*=true*/)
{
	if(bReleaseNode)
	{
		BacnetStructInfo *temStru = reinterpret_cast<BacnetStructInfo *>(m_wndFileView.GetItemData(hItem));
		if(temStru)
		{
			if(temStru->type == 0)			//根节点
			{		
				hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.begin();
				while(iter != m_mapDeveice.end())
				{
					BacnetStructInfo* pBacnet = (*iter).second;
					if(pBacnet)
					{
						hash_map<wstring,BacnetStructInfo*>::iterator iter1 = pBacnet->mapPoint.begin();
						while(iter1 != pBacnet->mapPoint.end())
						{
							BacnetStructInfo* pBacnetPoint = (*iter1).second;
							if(pBacnetPoint)
							{
								delete pBacnetPoint;
								pBacnetPoint = NULL;
							}
							iter1 = pBacnet->mapPoint.erase(iter1);
						}
						delete pBacnet;
						pBacnet = NULL;
					}
					iter = m_mapDeveice.erase(iter);
				}
				m_vecDeviceIPID.clear();
			}
			else if(temStru->type == 1)			//bacnet
			{
				CString strKey = _T("");
				strKey.Format(_T("%s/%u"),temStru->strIP,temStru->nOldDeviceID);
				wstring wstrKey = strKey.GetString();
				hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.find(wstrKey);
				if(iter != m_mapDeveice.end())
				{
					BacnetStructInfo* pBacnet = (*iter).second;
					if(pBacnet)
					{
						hash_map<wstring,BacnetStructInfo*>::iterator iter1 = pBacnet->mapPoint.begin();
						while(iter1 != pBacnet->mapPoint.end())
						{
							BacnetStructInfo* pBacnetPoint = (*iter1).second;
							if(pBacnetPoint)
							{
								delete pBacnetPoint;
								pBacnetPoint = NULL;
							}
							iter1 = pBacnet->mapPoint.erase(iter1);
						}
						pBacnet->mapArrayIndex.clear();
					}
				}
			}
			else if(temStru->type == 2)			//bacnet点
			{
				/*CString strKey = _T("");
				strKey.Format(_T("%s/%u"),temStru->strIP,temStru->nOldDeviceID);
				wstring wstrKey = strKey.GetString();
				hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.find(wstrKey);
				if(iter != m_mapDeveice.end())
				{
				BacnetStructInfo* pBacnet = (*iter).second;
				if(pBacnet)
				{
				wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(temStru->nPointType));
				CString strDeviceInfo;
				strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),temStru->nPointAddress);
				hash_map<wstring,BacnetStructInfo*>::iterator iter1 = pBacnet->mapPoint.find(strDeviceInfo.GetString());
				if(iter1 != pBacnet->mapPoint.end())
				{
				BacnetStructInfo* pBacnetPoint = (*iter1).second;
				if(pBacnetPoint)
				{
				delete pBacnetPoint;
				pBacnetPoint = NULL;
				}
				pBacnet->mapPoint.erase(iter1);
				}
				}
				}*/
			}
		}
	}
	HTREEITEM hChild = m_wndFileView.GetChildItem(hItem);
	vector<HTREEITEM> vecDelete;
	while (hChild)
	{
		vecDelete.push_back(hChild);
		hChild = m_wndFileView.GetNextItem(hChild, TVGN_NEXT);
	}
	for (size_t i=0;i<vecDelete.size();++i)
	{
		m_wndFileView.DeleteItem(vecDelete[i]);
	}
	return true;
}

void CCommBacnetSearch::InitServiceHandlers()
{
	Device_Initialize_Object_Functions(&Object_Table_[0]);
	Device_Init();
	// we need to handle who-is
	//   to support dynamic device binding to us */
	//apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);

	// handle i-am to support binding to other devices */
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, CCommBacnetSearch::HandleIAM);

	// set the handler for all the services we don't implement
	//  It is required to send the proper reject message... */
	apdu_set_unrecognized_service_handler_handler(handler_unrecognized_service);

	// we must implement read property - it's required! */
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY, handler_read_property);

	//handle the ack coming back 
	apdu_set_confirmed_simple_ack_handler(SERVICE_CONFIRMED_WRITE_PROPERTY, CCommBacnetSearch::HandleWritePropertyAck);
	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_ATOMIC_READ_FILE,CCommBacnetSearch::MyAtomicReadFileAckHandler);
	apdu_set_error_handler(SERVICE_CONFIRMED_ATOMIC_READ_FILE,CCommBacnetSearch::MyAtomic_Read_File_Error_Handler);
	// handle the data coming back from confirmed requests */
	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY, CCommBacnetSearch::HandleReadPropertyAck);
	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE, CCommBacnetSearch::HandleReadPropertyMultiAck);

	// handle any errors coming back */
	apdu_set_error_handler(SERVICE_CONFIRMED_READ_PROPERTY, MyErrorHandler);
	apdu_set_abort_handler(MyAbortHandler);
	apdu_set_reject_handler(MyRejectHandler);
}

void CCommBacnetSearch::OnReceiveRead( BACNET_READ_PROPERTY_DATA* pData, BACNET_ADDRESS * src,UINT invokeId )
{
	ASSERT(pData);
	if(!pData)  
		return;

	if(!src)
		return;

	/* for decode value data */
	BACNET_APPLICATION_DATA_VALUE* dataValue = new BACNET_APPLICATION_DATA_VALUE;
	ASSERT(dataValue);
	memset(dataValue, 0x00, sizeof(BACNET_APPLICATION_DATA_VALUE));

	int len = 0;
	uint8_t *application_data;
	int application_data_len;

	application_data = pData->application_data;
	application_data_len = pData->application_data_len;
	// FIXME: what if application_data_len is bigger than 255? 
	// value? need to loop until all of the len is gone... 
	len =  bacapp_decode_application_data(application_data,
		(uint8_t) application_data_len, dataValue);
	if (len < 0){
		SAFE_DELETE(dataValue);
		return;
	}
	m_oleRecTime = COleDateTime::GetCurrentTime();
	if(dataValue)
	{
		if(dataValue->tag == 2)
		{
			BacnetStructInfo* pBacnet = FindBacnetInfoFromDeviceIDTypeIndex(m_dwCurrentOldDeviceID,src,pData->object_type,0,true);
			if(pBacnet)
			{
				pBacnet->nPointCount = dataValue->type.Unsigned_Int;
				pBacnet->nReadCount = 0;
				pBacnet->bReadDeviceCountSuccess = true;
				SetEventHandle(m_nEvent);
			}
		}
	}
	SAFE_DELETE(dataValue);
}

void CCommBacnetSearch::OnReceiveReadMultiple( BACNET_READ_ACCESS_DATA* pRpmData, BACNET_ADDRESS * src,UINT invokeId )
{
	ASSERT(pRpmData);
	if(!pRpmData) 
		return;

	if(!src)
		return;
	BACNET_PROPERTY_REFERENCE *rpm_property = NULL;
	BACNET_APPLICATION_DATA_VALUE *dataValue = NULL;
	BACNET_APPLICATION_DATA_VALUE* old_value = NULL;
	BACNET_PROPERTY_REFERENCE *old_rpm_property = NULL;

	//AddLog(L"Enter handler multiple data back");
	BACNET_PROPERTY_ID pType = PROP_ACKED_TRANSITIONS;
	BacnetStructInfo* pBacnetDevice = FindBacnetInfoFromDeviceIDTypeIndex(m_dwCurrentOldDeviceID,src,pRpmData->object_type,0,true);
	if(pBacnetDevice)
	{
		bool bFindPointObjectName = false;
		BacnetStructInfo* pBacnetPoint = NULL;
		while (pRpmData)
		{
			rpm_property = pRpmData->listOfProperties;
			while (rpm_property) 
			{			
				dataValue = rpm_property->value;
				if(dataValue)
				{
					m_oleRecTime = COleDateTime::GetCurrentTime();
					if(rpm_property->propertyIdentifier == PROP_OBJECT_LIST)						//查询设备结构
					{
						if(dataValue->tag == BACNET_APPLICATION_TAG_OBJECT_ID)
						{
							if(dataValue->type.Object_Id.type < MAX_BACNET_OBJECT_TYPE) 
							{
								DWORD nInstance = dataValue->type.Object_Id.instance;
								BacnetStructInfo* pBacnetInfo = new BacnetStructInfo;
								pBacnetInfo->type = 2;
								pBacnetInfo->nPointAddress = nInstance;
								pBacnetInfo->nPointType = dataValue->type.Object_Id.type;
								pBacnetInfo->nArrayIndex = rpm_property->propertyArrayIndex;
								pBacnetInfo->nOldDeviceID = pBacnetDevice->nOldDeviceID;
								pBacnetInfo->nNewDeviceID = pBacnetDevice->nNewDeviceID;
								pBacnetInfo->strIP = pBacnetDevice->strIP;
								pBacnetInfo->nPointCount = pBacnetDevice->nPointCount;
								wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(pBacnetInfo->nPointType));
								CString strDeviceInfo;
								strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),pBacnetInfo->nPointAddress);
								pBacnetDevice->mapPoint[strDeviceInfo.GetString()] = pBacnetInfo;
								pBacnetDevice->mapArrayIndex[pBacnetInfo->nArrayIndex] = strDeviceInfo.GetString();
							}
							if(pBacnetDevice)
								pBacnetDevice->bReadDeviceInfoSuccess = true;
						}
					}
					else
					{
						if(pBacnetPoint == NULL)
							pBacnetPoint = FindBacnetInfoFromDeviceIDTypeIndex(m_dwCurrentOldDeviceID,src,pRpmData->object_type,pRpmData->object_instance);

						if(pBacnetPoint)
						{
							if(rpm_property->propertyIdentifier == PROP_OBJECT_NAME)		//ObjectName
							{
								if(dataValue->tag == BACNET_APPLICATION_TAG_CHARACTER_STRING)
								{
									bFindPointObjectName = true;
									if(pBacnetDevice)
										pBacnetDevice->bReadDevicePointPropertySuccess = true;
								}
							}
							CopyValue(dataValue,rpm_property->propertyIdentifier,pRpmData->object_type,pRpmData->object_instance,pBacnetPoint);
						}
						else
						{
							CString strError;
							strError.Format(_T("错误:CopyValue找点失败(%u,%d,%d).\r\n"), m_dwCurrentOldDeviceID,pRpmData->object_type,pRpmData->object_instance);
							AddLog(strError);
						}				
					}
				}
				rpm_property = rpm_property->next;
			}
			pRpmData = pRpmData->next;
		}
	}
	else
	{
		CString strLog;
		strLog.Format(_T("ERROR:FindBacnetInfoFromDeviceIDTypeIndex Fail(device:%u, type:%d)"),m_dwCurrentOldDeviceID,pRpmData->object_type);
		AddLog(strLog);
	}
	SetEventHandle(m_nEvent);
}

void CCommBacnetSearch::MyAbortHandler( BACNET_ADDRESS * src, uint8_t invoke_id, uint8_t abort_reason, bool server )
{
	CString strError;
	strError.Format(_T("错误:BACnet终止(%s)...\r\n"), Project::Tools::AnsiToWideChar(bactext_reject_reason_name(abort_reason)).c_str());
	AddLog(strError);
}

void CCommBacnetSearch::MyErrorHandler( BACNET_ADDRESS * src, uint8_t invoke_id, BACNET_ERROR_CLASS error_class, BACNET_ERROR_CODE error_code )
{
	//do nothing
	CString strError;
	strError.Format(_T("错误 :BACnet错误(%s:%s)...\r\n"), Project::Tools::AnsiToWideChar(bactext_error_class_name(error_class)).c_str(),Project::Tools::AnsiToWideChar(bactext_error_code_name(error_code)).c_str());
	AddLog(strError);
}

void CCommBacnetSearch::MyRejectHandler( BACNET_ADDRESS * src, uint8_t invoke_id, uint8_t reject_reason )
{
	//do nothing
	CString strError;
	strError.Format(_T("错误 :BACnet拒绝(%s)...\r\n"), Project::Tools::AnsiToWideChar(bactext_reject_reason_name(reject_reason)).c_str());
	AddLog(strError);
}

void CCommBacnetSearch::HandleReadPropertyAck( UINT8 * service_request, UINT16 service_len, BACNET_ADDRESS * src, BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data )
{
	int len = 0;
	BACNET_READ_PROPERTY_DATA   data;

	len = rp_ack_decode_service_request(service_request, service_len, &data);
	if (len > 0) {
		OnReceiveRead(&data, src,service_data->invoke_id);
	}
}

void CCommBacnetSearch::HandleReadPropertyMultiAck( UINT8 * service_request, UINT16 service_len, BACNET_ADDRESS * src, BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data )
{
	int len = 0;
	BACNET_READ_ACCESS_DATA *pRpmData = NULL;
	BACNET_READ_ACCESS_DATA *old_rpm_data = NULL;
	BACNET_PROPERTY_REFERENCE *rpm_property = NULL;
	BACNET_PROPERTY_REFERENCE *old_rpm_property = NULL;
	BACNET_APPLICATION_DATA_VALUE *dataValue = NULL;
	BACNET_APPLICATION_DATA_VALUE *old_value = NULL;

	const UINT invokeId = service_data->invoke_id;
	pRpmData = new BACNET_READ_ACCESS_DATA; //must delete
	EnterCriticalSection(&m_criticalSection); //等待进入临界区，进入后加锁使其他线程不能进入
	if (pRpmData) {
		len = rpm_ack_decode_service_request(service_request, service_len, pRpmData);
		OnReceiveReadMultiple(pRpmData, src,invokeId);
	}
	/*if (len > 0) {
		OnReceiveReadMultiple(pRpmData, src,invokeId);
	}*/

	//clear data
	ClearReadAccessData(&pRpmData);
	LeaveCriticalSection(&m_criticalSection);  //离开 开锁
}

void CCommBacnetSearch::HandleWritePropertyAck( BACNET_ADDRESS * src, uint8_t invoke_id )
{
	//do nothing
}

void CCommBacnetSearch::HandleIAM( uint8_t * service_request, uint16_t service_len, BACNET_ADDRESS * src )
{
	int len = 0;
	uint32_t device_id = 0;
	unsigned max_apdu = 0;
	int segmentation = 0;
	uint16_t vendor_id = 0;

	EnterCriticalSection(&m_criticalSection); //等待进入临界区，进入后加锁使其他线程不能进入
	(void) service_len;
	len =
		iam_decode_service_request(service_request, &device_id, &max_apdu,
		&segmentation, &vendor_id);

	if (len != -1)
	{
		int nReadLimit = (max_apdu-g_nBacnetReqHeaderLength)/g_nBacnetOneReqLength;
		nReadLimit = (nReadLimit < m_nMutilReadCount)?nReadLimit:m_nMutilReadCount;

		CString strDeviceIp;
		strDeviceIp.Format(_T("%d.%d.%d.%d"),src->mac[0], src->mac[1], src->mac[2],
			src->mac[3]);

		CString strDeviceInfo;
		strDeviceInfo.Format(_T("%s:47808 Device ID:%d"),strDeviceIp,device_id);

		int nOldID = device_id;
		CString strKey = _T("");
		strKey.Format(_T("%s/%u"),strDeviceIp,device_id);
		wstring wstrKey = strKey.GetString();
		hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.find(wstrKey);
		if(iter == m_mapDeveice.end())
		{
			//
			BacnetStructInfo* bacnetTreeNode = new BacnetStructInfo;
			bacnetTreeNode->type = 1;
			bacnetTreeNode->nOldDeviceID = device_id;
			int nIndex = m_mapDeveice.size()+1;
			device_id = nIndex;
			bacnetTreeNode->nNewDeviceID = device_id;
			bacnetTreeNode->strIP = strDeviceIp;
			bacnetTreeNode->nReadLimit = nReadLimit;
			HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,reinterpret_cast<DWORD>(bacnetTreeNode),m_hSelectNode,TVI_LAST);
			m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);
			m_mapDeveice[wstrKey] = bacnetTreeNode;
			if(!FindIsExist(m_vecDeviceIPID,wstrKey))			//paixu
			{
				//sort vecDeviceID
				std::sort(m_vecDeviceIPID.begin(), m_vecDeviceIPID.end(), CompareDeviceIPID);
			}
		}
		else
		{
			BacnetStructInfo* bacnetTreeNode = (*iter).second;
			if(bacnetTreeNode)
			{
				device_id = bacnetTreeNode->nNewDeviceID;
				bacnetTreeNode->nReadLimit = nReadLimit;
			}
		}
		address_add(device_id, max_apdu, src);
	}

	LeaveCriticalSection(&m_criticalSection);
	return;
}

bool CCommBacnetSearch::FindIsExist(vector<DWORD>& vec,DWORD deviceid)
{
	bool bExist = false;
	for(int i=0; i<vec.size(); ++i)
	{
		if(vec[i] == deviceid)
		{
			bExist = true;
			break;
		}
	}
	if(!bExist)
		vec.push_back(deviceid);
	return true;
}

bool CCommBacnetSearch::FindIsExist(vector<wstring>& vec,wstring key)
{
	bool bExist = false;
	for(int i=0; i<vec.size(); ++i)
	{
		if(vec[i] == key)
		{
			bExist = true;
			break;
		}
	}
	if(!bExist)
		vec.push_back(key);
	return bExist;
}

bool CCommBacnetSearch::CreateThreadRead()
{
	m_readdata_thread = (HANDLE)_beginthreadex(NULL,
		0,
		ThreadWhois, 
		(LPVOID)this,
		NORMAL_PRIORITY_CLASS,// CREATE_SUSPENDED,
		NULL);
	m_progress_thread = (HANDLE)_beginthreadex(NULL, 0 , ThreadUpdateProgressFunc, this, NORMAL_PRIORITY_CLASS, NULL);
	//m_read_thread = (HANDLE)_beginthreadex(NULL, 0 , ThreadReadPointFunc, this, NORMAL_PRIORITY_CLASS, NULL);
	//m_timeout_thread = (HANDLE)_beginthreadex(NULL, 0 , ThreadTimeOutActiveFunc, this, NORMAL_PRIORITY_CLASS, NULL);
	return true;
}

UINT WINAPI CCommBacnetSearch::ThreadWhois(LPVOID pParam)
{
	CCommBacnetSearch *pThis = static_cast<CCommBacnetSearch*>(pParam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	pThis->OnTimerWhois();

	InitServiceHandlers();

	const UINT c_sleep_ms = 10;
	while(!pThis->GetReadDataThreadExit())
	{
		pThis->OnTimerRead();
	}
	return 0;
}

void CCommBacnetSearch::OnTimerRead()
{
	const UINT timeout = 100;     // milliseconds */
	UINT16 pdu_len = 0;
	BACNET_ADDRESS srcAddr = { 0 };  // address where message came from */    
	UINT8 Rx_Buf[MAX_MPDU] = { 0 }; // buffer used for receive /
	pdu_len = datalink_receive(&srcAddr, &Rx_Buf[0], MAX_MPDU, timeout);
	if (pdu_len) {
		npdu_handler(&srcAddr, &Rx_Buf[0], pdu_len);		
	}
}

void CCommBacnetSearch::OnTimerWhois()
{
	uint16_t pdu_len = 0;
	unsigned timeout = 100;     /* milliseconds */
	time_t total_seconds = 0;
	time_t elapsed_seconds = 0;
	time_t last_seconds = time(NULL);
	time_t current_seconds = 0;
	time_t timeout_seconds = apdu_timeout() / 1000 +10;
	bool Error_Detected = false;
	BACNET_ADDRESS src = {  0  }; 
	uint8_t Rx_Buf[MAX_MPDU] = { 0 };
	UINT addressCount = 0;

	//init again
	InitServiceHandlers();

	// send the request */
	Send_WhoIs(-1, -1);

	for (;;) {
		if(GetReadDataThreadExit())
			break;
		/* increment timer - exit if timed out */
		current_seconds = time(NULL);
		/* returns 0 bytes on timeout */
		pdu_len = datalink_receive(&src, &Rx_Buf[0], MAX_MPDU, timeout);
		/* process */
		if (pdu_len) {
			npdu_handler(&src, &Rx_Buf[0], pdu_len);
		}

		if (Error_Detected)
			continue;
		/* increment timer - exit if timed out */
		elapsed_seconds = current_seconds - last_seconds;
		if (elapsed_seconds) {
#if defined(BACDL_BIP) && BBMD_ENABLED
			bvlc_maintenance_timer(elapsed_seconds);
#endif
		}
		total_seconds += elapsed_seconds;
		if (total_seconds > timeout_seconds)
			break;
		/* keep track of time for next check */
		last_seconds = current_seconds;
	}
}

bool CCommBacnetSearch::GetReadDataThreadExit() const
{
	return m_exit_readdata_thread;
}

void CCommBacnetSearch::BacnetExportAll()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}
	m_bBusy = true;
	AddLog(_T("Bacnet全部导出...\r\n"));
	if(m_mapDeveice.size() <= 0)
	{
		AfxMessageBox(_T("请先扫描Bacnet设备"));
		m_bBusy = false;
		return;
	}
	m_bStoreS3db = false;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_bUpdateProgress = true;
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			m_bBusy = false;
			return;
		}
		m_strSavepath = strFilePath;

		BacnetStructInfo *temStru=reinterpret_cast<BacnetStructInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
		if(temStru != NULL)
		{
			m_pCurrentBacnet = temStru;
			if(temStru->type == 0)
			{
				m_nDeviceCount = m_mapDeveice.size();
				m_nReadDeviceCount = 0;
				hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.begin();
				while(iter != m_mapDeveice.end())
				{
					(*iter).second->ExportClear();
					++iter;
				}
			}
		}
		
		HANDLE hExportAll = (HANDLE)_beginthreadex(NULL, 0 , ThreadExportAllFunc, this, NORMAL_PRIORITY_CLASS, NULL);

	}
	m_bBusy = false;
}

UINT WINAPI CCommBacnetSearch::ThreadReadPointFunc(LPVOID lparam)
{
	CCommBacnetSearch *pThis = static_cast<CCommBacnetSearch*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	while(!pThis->GetReadDataThreadExit())
	{
		DWORD waitstatus = WaitForSingleObject(pThis->GetEventHandle(), INFINITE);
		if (waitstatus == WAIT_OBJECT_0)
		{
			int nEvent = pThis->GetEventCode();
			if(nEvent == 0)				//不处理
			{

			}
			else if(nEvent == 1)		//读取点数
			{
				bool bNeedhandle = false;
				for(int i=0; i<m_vecDeviceIPID.size(); ++i)
				{
					wstring strKey = m_vecDeviceIPID[i];
					hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(strKey);
					if(iterDevice != m_mapDeveice.end())
					{
						BacnetStructInfo* pBacnet = (*iterDevice).second;
						if(pBacnet)
						{
							if(!pBacnet->bReadFinished && pBacnet->bNeedRead && !pBacnet->bInore && pBacnet->nReadCount<m_nRetryReadCount)
							{
								ReadObjectCount(pBacnet);
								m_pCurrentBacnet = pBacnet;
								bNeedhandle = true;
								break;
							}
						}
					}
				}
				if(!bNeedhandle)
				{
					++m_nEvent;
					SetEventHandle(m_nEvent);
				}
			}
			else if(nEvent == 2)		//查询设备点结构信息
			{
				bool bNeedhandle = false;
				for(int i=0; i<m_vecDeviceIPID.size(); ++i)
				{
					wstring strKey = m_vecDeviceIPID[i];
					hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(strKey);
					if(iterDevice != m_mapDeveice.end())
					{
						BacnetStructInfo* pBacnet = (*iterDevice).second;
						if(pBacnet)
						{
							if(!pBacnet->bReadFinished && pBacnet->bNeedRead && !pBacnet->bInore && pBacnet->nReadCount<m_nRetryReadCount)
							{
								ReadObjectInfo(pBacnet);
								bNeedhandle = true;
								break;
							}
						}
					}
				}
				if(!bNeedhandle)
				{
					++m_nEvent;
					SetEventHandle(m_nEvent);
				}
			}
			else if(nEvent == 3)		//查询设备点属性信息
			{
				bool bNeedhandle = false;
				for(int i=0; i<m_vecDeviceIPID.size(); ++i)
				{
					wstring strKey = m_vecDeviceIPID[i];
					hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(strKey);
					if(iterDevice != m_mapDeveice.end())
					{
						BacnetStructInfo* pBacnet = (*iterDevice).second;
						if(pBacnet)
						{
							if(!pBacnet->bReadFinished && pBacnet->bNeedRead && !pBacnet->bInore && pBacnet->nReadCount<m_nRetryReadCount)
							{
								hash_map<DWORD,wstring>::iterator iterArray = pBacnet->mapArrayIndex.find(pBacnet->nCurrentArrayIndex);
								if(iterArray != pBacnet->mapArrayIndex.end())
								{
									wstring strKey = (*iterArray).second;
									hash_map<wstring,BacnetStructInfo*>::iterator iterPoint = pBacnet->mapPoint.find(strKey);
									if(iterPoint != pBacnet->mapPoint.end())
									{
										BacnetStructInfo* pBacnetPoint = (*iterPoint).second;
										if(pBacnetPoint)
										{
											if(pBacnetPoint->nReadCount>m_nRetryReadCount)
											{
												pBacnet->nCurrentArrayIndex++;
												SetEventHandle(m_nEvent);
												bNeedhandle = true;
												break;
											}
											else
											{
												ReadObjectProperty(pBacnetPoint);
												bNeedhandle = true;
												break;
											}
										}
									}
								}
							}
						}
					}
				}
				if(!bNeedhandle)
				{
					++m_nEvent;
					SetEventHandle(m_nEvent);
				}
			}
			else if(nEvent == 5)		//导出设备点结构信息
			{
				hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.begin();
				while(iter != m_mapDeveice.end())
				{
					BacnetStructInfo* pBacnet = (*iter).second;
					if(pBacnet)
					{
						if(pBacnet->bReadFinished && pBacnet->bExportedFinished && !pBacnet->bInore && pBacnet->nReadCount<m_nRetryReadCount)
						{
							
							break;
						}
					}
					++iter;
				}
			}
			else if(nEvent == 4)		//导出设备点属性信息
			{
				for(int i=0; i<m_vecDeviceIPID.size(); ++i)
				{
					wstring strKey = m_vecDeviceIPID[i];
					hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(strKey);
					if(iterDevice != m_mapDeveice.end())
					{
						BacnetStructInfo* pBacnet = (*iterDevice).second;
						if(pBacnet)
						{
							if(pBacnet->bReadFinished && !pBacnet->bExportedFinished && !pBacnet->bInore && pBacnet->nReadCount<m_nRetryReadCount)
							{
								ExportPoints(pBacnet);
								break;
							}
						}
					}
				}

				//如果未读完 继续查询
				bool bContinueRead = false;
				hash_map<wstring,BacnetStructInfo*>::iterator iterRead = m_mapDeveice.begin();
				while(iterRead != m_mapDeveice.end())
				{
					BacnetStructInfo* pBacnet = (*iterRead).second;
					if(pBacnet)
					{
						if(!pBacnet->bReadFinished && !pBacnet->bInore && pBacnet->nReadCount<m_nRetryReadCount)
							bContinueRead = true;
					}
					++iterRead;
				}
				if(bContinueRead)
				{
					SetEventHandle(1);
				}
				else
				{
					//如果读完未全部导出则继续导出
					bool bContinueExport = false;
					hash_map<wstring,BacnetStructInfo*>::iterator iterExport = m_mapDeveice.begin();
					while(iterExport != m_mapDeveice.end())
					{
						BacnetStructInfo* pBacnet = (*iterExport).second;
						if(pBacnet)
						{
							if(!pBacnet->bExportedFinished && !pBacnet->bInore)
								bContinueExport = true;
						}
						++iterExport;
					}

					if(bContinueExport)
					{
						SetEventHandle(4);
					}
					else
					{
						AddLog(_T("导出成功...\r\n"));
						m_bUpdateProgress = false;
					}
				}
			}	
			else if(nEvent == 6)		//显示读取点数
			{
				int nDeciceCount = 0;
				hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.find(m_strSelectKey);
				if(iter != m_mapDeveice.end())
				{
					BacnetStructInfo* pBacnet = (*iter).second;
					if(pBacnet)
					{
						if(!pBacnet->bReadFinished && pBacnet->bNeedRead && !pBacnet->bInore && pBacnet->nReadCount<m_nRetryReadCount)
						{
							nDeciceCount++;
							ReadObjectCount(pBacnet);
							m_pCurrentBacnet = pBacnet;
						}
					}
				}
			}
			else if(nEvent == 7)		//显示查询设备点结构信息
			{
				hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.find(m_strSelectKey);
				if(iter != m_mapDeveice.end())
				{
					BacnetStructInfo* pBacnet = (*iter).second;
					if(pBacnet)
					{
						if(!pBacnet->bReadFinished && pBacnet->bNeedRead && !pBacnet->bInore && pBacnet->nReadCount<m_nRetryReadCount)
						{
							ReadObjectInfo(pBacnet);
						}
					}
				}
			}
			else if(nEvent == 8)		//显示查询设备点结构信息
			{
				if(m_pCurrentBacnet)
				{
					CString strKey = _T("");
					strKey.Format(_T("%s/%u"),m_pCurrentBacnet->strIP,m_pCurrentBacnet->nOldDeviceID);
					wstring strKey_ = strKey.GetString();

					hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(strKey_);
					if(iterDevice != m_mapDeveice.end())
					{
						BacnetStructInfo* pBacnet = (*iterDevice).second;
						if(pBacnet)
						{
							wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(m_pCurrentBacnet->nPointType));
							CString strDeviceInfo;
							strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),m_pCurrentBacnet->nPointAddress);
							hash_map<wstring,BacnetStructInfo*>::iterator iterPoint = pBacnet->mapPoint.find(strDeviceInfo.GetString());
							if(iterPoint != pBacnet->mapPoint.end())
							{
								ReadObjectProperty((*iterPoint).second);
							}
						}
					}
				}
			}
		}
	}

	return 0;
}

void CCommBacnetSearch::SetEventHandle(int nEvent)
{
	Sleep(m_nBacnetScanInterval);
	m_nEvent = nEvent;
	SetEvent(m_event);
}

HANDLE CCommBacnetSearch::GetEventHandle()
{
	return m_event;
}

bool CCommBacnetSearch::ReadObjectInfo(BacnetStructInfo* pBacnetInfo,int nOneCmdPoint)
{
	if(pBacnetInfo)
	{
		//pBacnetInfo->nReadCount++;
		DWORD deviceObjInstance = 0;
		deviceObjInstance = pBacnetInfo->nNewDeviceID;

		UINT8 buffer[MAX_PDU] = {0};
		BACNET_READ_ACCESS_DATA *pReadAccessData = new BACNET_READ_ACCESS_DATA;
		if(!pReadAccessData) 
			return false;
		int nEnd = pBacnetInfo->nCurrentArrayIndex+nOneCmdPoint;
		nEnd = (nEnd>=pBacnetInfo->nPointCount)?pBacnetInfo->nPointCount:nEnd;

		BACNET_READ_ACCESS_DATA* pRpmData = pReadAccessData;
		for(int i=pBacnetInfo->nCurrentArrayIndex; i<nEnd; ++i)
		{
			pRpmData->object_type = OBJECT_DEVICE;	//point type
			pRpmData->object_instance = pBacnetInfo->nOldDeviceID; //address

			pRpmData->listOfProperties = new BACNET_PROPERTY_REFERENCE;
			pRpmData->listOfProperties->propertyIdentifier = PROP_OBJECT_LIST;
			pRpmData->listOfProperties->propertyArrayIndex = i;
			pRpmData->listOfProperties->next = NULL;
			if(i!=nEnd-1)
			{
				pRpmData->next = new BACNET_READ_ACCESS_DATA;
				pRpmData = pRpmData->next ;
			}
			pRpmData->next =NULL;
		}
		EnterCriticalSection(&m_criticalSection); //等待进入临界区，进入后加锁使其他线程不能进入
		pBacnetInfo->nCurrentArrayIndex = nEnd;
		UINT max_apdu = 0;
		BACNET_ADDRESS  targetAddr;
		const bool ifFound = address_bind_request(deviceObjInstance, &max_apdu,  &targetAddr);
		if(ifFound)
		{
			m_dwCurrentOldDeviceID = pBacnetInfo->nOldDeviceID;
			const UINT8 reqInvokeId = Send_Read_Property_Multiple_Request(&buffer[0], sizeof(buffer), 
				deviceObjInstance,
				pReadAccessData);
			m_oleSendTime = COleDateTime::GetCurrentTime();
		}
		LeaveCriticalSection(&m_criticalSection);  //离开 开锁
		Sleep(20); //sleep after network send.
		ClearReadAccessData(&pReadAccessData);
		return true;
	}
	return false;
}

bool CCommBacnetSearch::ReadObjectInfo( BacnetStructInfo* pBacnetInfo,int nStart,int nEnd )
{
	if(pBacnetInfo)
	{
		DWORD deviceObjInstance = 0;
		deviceObjInstance = pBacnetInfo->nNewDeviceID;

		UINT8 buffer[MAX_PDU] = {0};
		BACNET_READ_ACCESS_DATA *pReadAccessData = new BACNET_READ_ACCESS_DATA;
		if(!pReadAccessData) 
			return false;

		BACNET_READ_ACCESS_DATA* pRpmData = pReadAccessData;
		for(int i=nStart; i<nEnd; ++i)
		{
			pRpmData->object_type = OBJECT_DEVICE;	//point type
			pRpmData->object_instance = pBacnetInfo->nOldDeviceID; //address

			pRpmData->listOfProperties = new BACNET_PROPERTY_REFERENCE;
			pRpmData->listOfProperties->propertyIdentifier = PROP_OBJECT_LIST;
			pRpmData->listOfProperties->propertyArrayIndex = i;
			pRpmData->listOfProperties->next = NULL;
			if(i!=nEnd-1)
			{
				pRpmData->next = new BACNET_READ_ACCESS_DATA;
				pRpmData = pRpmData->next ;
			}
			pRpmData->next =NULL;
		}

		EnterCriticalSection(&m_criticalSection); //等待进入临界区，进入后加锁使其他线程不能进入
		pBacnetInfo->nCurrentArrayIndex = nEnd;
		UINT max_apdu = 0;
		BACNET_ADDRESS  targetAddr;
		const bool ifFound = address_bind_request(deviceObjInstance, &max_apdu,  &targetAddr);
		if(ifFound)
		{
			m_dwCurrentOldDeviceID = pBacnetInfo->nOldDeviceID;
			const UINT8 reqInvokeId = Send_Read_Property_Multiple_Request(buffer, MAX_PDU, 
				deviceObjInstance,
				pReadAccessData);
			m_oleSendTime = COleDateTime::GetCurrentTime();
			CString strLog;
			strLog.Format(_T("Success: Send_Read_Property_Multiple_Request(device:%u, start:%d, end:%d, max_npdu:%d,req:%d)"),deviceObjInstance,nStart,nEnd,max_apdu,reqInvokeId);
			AddLog(strLog);
		}
		else
		{
			CString strLog;
			strLog.Format(_T("ERROR: address_bind_request fail(%u)"),deviceObjInstance);
			AddLog(strLog);
		}
		LeaveCriticalSection(&m_criticalSection);  //离开 开锁
		Sleep(20); //sleep after network send.
		ClearReadAccessData(&pReadAccessData);
		return true;
	}
	return false;
}

bool CCommBacnetSearch::ReadObjectProperty(BacnetStructInfo* pBacnetInfo)
{
	if(pBacnetInfo)
	{
		pBacnetInfo->nReadCount++;
		BACNET_READ_ACCESS_DATA *rpm_object = new BACNET_READ_ACCESS_DATA;
		StartNextObject(rpm_object,pBacnetInfo->nPointType,pBacnetInfo->nPointAddress);
		rpm_object->next = NULL;
		BuildPropRequest(rpm_object);

		/*BACNET_READ_ACCESS_DATA *pReadAccessData = new BACNET_READ_ACCESS_DATA;
		BACNET_READ_ACCESS_DATA *rpm_object = pReadAccessData;
		StartNextObject(rpm_object,pBacnetInfo->nPointType,pBacnetInfo->nPointAddress);
		rpm_object->next = NULL;
		BuildPropRequest(rpm_object);
		rpm_object->next = new BACNET_READ_ACCESS_DATA;
		rpm_object = rpm_object->next;
		StartNextObject(rpm_object,pBacnetInfo->nPointType,pBacnetInfo->nPointAddress);
		BuildPropRequest_Description(rpm_object);
		rpm_object->next = NULL;*/
		
		EnterCriticalSection(&m_criticalSection); //等待进入临界区，进入后加锁使其他线程不能进入
		UINT max_apdu = 0;
		UINT8 buffer[MAX_PDU] = {0};
		BACNET_ADDRESS  targetAddr;
		DWORD deviceObjInstance = 0;
		deviceObjInstance = pBacnetInfo->nNewDeviceID;
		const bool ifFound = address_bind_request(deviceObjInstance, &max_apdu,  &targetAddr);
		if(ifFound)
		{
			m_dwCurrentOldDeviceID = pBacnetInfo->nOldDeviceID;
			pBacnetInfo->nInvokeId = Send_Read_Property_Multiple_Request(&buffer[0], sizeof(buffer), 
				deviceObjInstance,
				rpm_object);
			m_oleSendTime = COleDateTime::GetCurrentTime();
		}
		LeaveCriticalSection(&m_criticalSection);  //离开 开锁
		Sleep(20); //sleep after network send.
		ClearReadAccessData(&rpm_object);
		return true;
	}
}

void CCommBacnetSearch::CopyValue(BACNET_APPLICATION_DATA_VALUE* pDataValue,BACNET_PROPERTY_ID proType,BACNET_OBJECT_TYPE obType,DWORD obInstance,BacnetStructInfo* pBacnetInfo)
{
	if(!pDataValue) 
		return;

	if(pBacnetInfo)
	{
		pBacnetInfo->bReadFinished = true;
		if(proType == PROP_DESCRIPTION)			//描述
		{
			if(pDataValue->tag == BACNET_APPLICATION_TAG_CHARACTER_STRING)
			{
				//
				wstring strDescription;
				if(pDataValue->type.Character_String.encoding == CHARACTER_ANSI_X34)
				{
					if(m_nCharacterType == 0)
					{
						strDescription = Project::Tools::AnsiToWideChar(pDataValue->type.Character_String.value);
					}
					else
					{
						strDescription = Project::Tools::UTF8ToWideChar(pDataValue->type.Character_String.value);
					}
				}
				else if (pDataValue->type.Character_String.encoding == CHARACTER_UCS2)
				{
					char cOut1[256] = {0};
					UCS2toUTF8((UINT8 *)pDataValue->type.Character_String.value,pDataValue->type.Character_String.length,cOut1);
					strDescription = Project::Tools::UTF8ToWideChar(cOut1);
				}	
				else if (pDataValue->type.Character_String.encoding == CHARACTER_UCS4)
				{
					char cOut1[256] = {0};
					UCS4toUTF8((UINT8 *)pDataValue->type.Character_String.value,pDataValue->type.Character_String.length,cOut1);
					strDescription = Project::Tools::UTF8ToWideChar(cOut1);
				}	
				else
				{
					if(m_nCharacterType == 0)
					{
						strDescription = AnsiToWideChar_Del0(pDataValue->type.Character_String.value,pDataValue->type.Character_String.length);
					}
					else
					{
						strDescription = UTF8ToWideChar_Del0(pDataValue->type.Character_String.value,pDataValue->type.Character_String.length);
					}
				}			
				pBacnetInfo->type = 2;
				pBacnetInfo->nPointAddress = obInstance;
				pBacnetInfo->nPointType = obType;
				pBacnetInfo->strDescription =  strDescription.c_str();
			}
		}
		else if(proType == PROP_FILE_SIZE)
		{
			pBacnetInfo->nFileSize = pDataValue->type.Unsigned_Int;
		}
		else if(proType == PROP_PRESENT_VALUE)		//值
		{
			float fValue = 0.0;
			switch(pDataValue->tag){ 
			case BACNET_APPLICATION_TAG_BOOLEAN:
				fValue = pDataValue->type.Boolean;
				break;
			case BACNET_APPLICATION_TAG_UNSIGNED_INT:
				fValue = pDataValue->type.Unsigned_Int;
				break;
			case BACNET_APPLICATION_TAG_SIGNED_INT:
				fValue = pDataValue->type.Signed_Int;
				break;
			case BACNET_APPLICATION_TAG_REAL:
				fValue = pDataValue->type.Real;
				break;
			case BACNET_APPLICATION_TAG_DOUBLE:
				fValue = pDataValue->type.Double;
				break;
			case BACNET_APPLICATION_TAG_ENUMERATED:
				fValue = pDataValue->type.Enumerated;
				break;
			default:
				break;
			}
			pBacnetInfo->type = 2;
			pBacnetInfo->nPointAddress = obInstance;
			pBacnetInfo->nPointType = obType;
			pBacnetInfo->fValue = fValue;
			pBacnetInfo->bReadPointPropertySuccess = true;
		}
		else if(proType == PROP_OBJECT_NAME)		//ObjectName
		{
			if(pDataValue->tag == BACNET_APPLICATION_TAG_CHARACTER_STRING)
			{
				pBacnetInfo->nReadCount = 0;
				wstring strDescription;
				if(pDataValue->type.Character_String.encoding == CHARACTER_ANSI_X34)
				{
					if(m_nCharacterType == 0)
					{
						strDescription = Project::Tools::AnsiToWideChar(pDataValue->type.Character_String.value);
					}
					else
					{
						strDescription = Project::Tools::UTF8ToWideChar(pDataValue->type.Character_String.value);
					}
				}
				else
				{
					if(m_nCharacterType == 0)
					{
						strDescription = AnsiToWideChar_Del0(pDataValue->type.Character_String.value,pDataValue->type.Character_String.length);
					}
					else
					{
						strDescription = UTF8ToWideChar_Del0(pDataValue->type.Character_String.value,pDataValue->type.Character_String.length);
					}
				}
				pBacnetInfo->type = 2;
				pBacnetInfo->nPointAddress = obInstance;
				pBacnetInfo->nPointType = obType;
				pBacnetInfo->strObjectName = strDescription.c_str();
				pBacnetInfo->bReadPointPropertySuccess = true;
			}
		}
		else if(proType == PROP_OBJECT_LIST)		//列表
		{
			if(pDataValue->tag == BACNET_APPLICATION_TAG_OBJECT_ID)
			{
				if(pDataValue->type.Object_Id.type < MAX_BACNET_OBJECT_TYPE) 
				{
					DWORD nInstance = pDataValue->type.Object_Id.instance;
					pBacnetInfo->type = 2;
					pBacnetInfo->nPointAddress = nInstance;
					pBacnetInfo->nPointType = pDataValue->type.Object_Id.type;
				}
			}
		}
	}	
}

BacnetStructInfo* CCommBacnetSearch::FindBacnetInfoFromDeviceIDTypeIndex(DWORD deviceid,BACNET_ADDRESS * src,int nPointType,DWORD arrayIndex,bool bFindDevice)
{
	if(src)
	{
		CString strDeviceIp;
		strDeviceIp.Format(_T("%d.%d.%d.%d"),src->mac[0], src->mac[1], src->mac[2],
			src->mac[3]);

		CString strKey = _T("");
		strKey.Format(_T("%s/%u"),strDeviceIp,deviceid);
		wstring wstrKey = strKey.GetString();
		hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.find(wstrKey);
		if(iter != m_mapDeveice.end())
		{
			BacnetStructInfo* pBacnet = (*iter).second;
			if(bFindDevice)
			{
				return pBacnet;
			}
			else
			{
				if(pBacnet)
				{
					wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(nPointType));
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),arrayIndex);
					hash_map<wstring,BacnetStructInfo*>::iterator iter1 = pBacnet->mapPoint.find(strDeviceInfo.GetString());
					if(iter1 != pBacnet->mapPoint.end())
						return (*iter1).second;
				}
			}
		}
	}
	return NULL;
}

wstring CCommBacnetSearch::UTF8ToWideChar_Del0( char* cOld, int length )
{
	char cObjectName[255] = {0};
	length = (length>255)?255:length;
	int nSize = 0;
	for(int i=0; i<length; ++i)
	{
		if(cOld[i] == '\0')
			continue;
		memcpy(cObjectName+nSize,cOld+i,1);
		nSize++;
	}

	if(nSize==0)
		return L"";

	//预转换，得到所需空间的大小 
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, cObjectName, nSize, NULL, 0); 
	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间 
	wchar_t* wszString = new wchar_t[wcsLen + 1]; 
	//转换 
	::MultiByteToWideChar(CP_UTF8, NULL, cObjectName, nSize, wszString, wcsLen); 
	//最后加上'\0' 
	wszString[wcsLen] = L'\0'; 

	wstring result = wszString;
	delete[] wszString;

	return result;
}

wstring CCommBacnetSearch::AnsiToWideChar_Del0( char* cOld, int length )
{
	char cObjectName[255] = {0};
	length = (length>255)?255:length;
	int nSize = 0;
	for(int i=0; i<length; ++i)
	{
		if(cOld[i] == '\0')
			continue;
		memcpy(cObjectName+nSize,cOld+i,1);
		nSize++;
	}

	if(nSize==0)
		return L"";

	int wlen = MultiByteToWideChar(CP_ACP, 0, cObjectName, nSize, NULL, 0);
	wchar_t* buffer = new wchar_t[wlen + 1];
	int rflag = MultiByteToWideChar(CP_ACP, 0, cObjectName, nSize, buffer, wlen);
	buffer[wlen] = L'\0';

	wstring result = buffer;
	delete[] buffer;

	return result;
}

bool CCommBacnetSearch::ReadObjectCount(BacnetStructInfo* pBacnetInfo)
{
	if(pBacnetInfo)
	{
		pBacnetInfo->nReadCount++;
		m_dwCurrentOldDeviceID = pBacnetInfo->nOldDeviceID;
		int invoke_id =
			Send_Read_Property_Request(pBacnetInfo->nNewDeviceID, OBJECT_DEVICE,
			pBacnetInfo->nOldDeviceID, PROP_OBJECT_LIST,0);
		m_oleSendTime = COleDateTime::GetCurrentTime();
		return true;
	}
	return false;
}

UINT WINAPI CCommBacnetSearch::ThreadUpdateProgressFunc(LPVOID lparam)
{
	CCommBacnetSearch *pThis = static_cast<CCommBacnetSearch*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	while(!pThis->GetReadDataThreadExit())
	{
		if(pThis->m_bUpdateProgress)
			pThis->UpdateProgress();
		Sleep(1000);
	}

	return 0;
}

bool CCommBacnetSearch::UpdateProgress()
{
	CString strSendInfo,strRecInfo,strDeviceInfo;
	if(m_pCurrentBacnet && m_pCurrentBacnet->nPointCount>0)
	{
		int nSendPos = m_pCurrentBacnet->nCurrentArrayIndex*100/m_pCurrentBacnet->nPointCount;
		m_nDeviceCount = (m_nDeviceCount>0)?m_nDeviceCount:1;
		int nDevicePos = m_nReadDeviceCount*100/m_nDeviceCount;
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_SEND))->SetPos(nSendPos);
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_SEND_DEVICE))->SetPos(nDevicePos);
	}

	switch(m_nEvent)
	{
	case 2:
	case 7:
		{
			if(m_pCurrentBacnet && m_pCurrentBacnet->nPointCount>0)
			{				
				strSendInfo.Format(_T("读取设备点信息(%d/%d)..."),m_pCurrentBacnet->nCurrentArrayIndex,m_pCurrentBacnet->nPointCount);
				((CStatic*)GetDlgItem(IDC_STATIC_SEND))->SetWindowText(strSendInfo);
				strDeviceInfo.Format(_T("扫描设备:%s/%u点属性(%d/%d)..."),m_pCurrentBacnet->strIP,m_pCurrentBacnet->nOldDeviceID,m_nReadDeviceCount,m_nDeviceCount);
				((CStatic*)GetDlgItem(IDC_STATIC_DEVICE))->SetWindowText(strDeviceInfo);
			}
			break;
		}
	default:
		{
			if(m_pCurrentBacnet && m_pCurrentBacnet->nPointCount>0)
			{		
				strSendInfo.Format(_T("读取设备点属性(%d / %d)..."),m_pCurrentBacnet->nCurrentArrayIndex,m_pCurrentBacnet->nPointCount);
				((CStatic*)GetDlgItem(IDC_STATIC_SEND))->SetWindowText(strSendInfo);
				strDeviceInfo.Format(_T("扫描设备:%s/%u点属性(%d/%d)..."),m_pCurrentBacnet->strIP,m_pCurrentBacnet->nOldDeviceID,m_nReadDeviceCount,m_nDeviceCount);
				((CStatic*)GetDlgItem(IDC_STATIC_DEVICE))->SetWindowText(strDeviceInfo);
			}
			break;
		}
	}
	return true;
}

void CCommBacnetSearch::BacnetExportBacnetInfo()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}
	m_bBusy = true;
	if(m_mapDeveice.size() <= 0)
	{
		AfxMessageBox(_T("请先扫描Bacnet设备!"));
		m_bBusy = false;
		return;
	}
	m_bStoreS3db = false;
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			m_bBusy = false;
			return;
		}

		//写文件
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
		setlocale( LC_ALL, "chs" );
		CStdioFile filehandle;
		if(filehandle.Open(strFilePath, CFile::modeCreate|CFile::modeWrite))
		{
			filehandle.Seek(0, CFile::begin);
			filehandle.WriteString(_T("IP:Port,Device ID\n"));

			for(int i=0; i<m_vecDeviceIPID.size(); ++i)
			{
				wstring strKey = m_vecDeviceIPID[i];
				hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(strKey);
				if(iterDevice != m_mapDeveice.end())
				{
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("%s\n"),(*iterDevice).first.c_str());
					filehandle.WriteString(strDeviceInfo);	
				}
			}
			filehandle.Close();
			setlocale( LC_ALL, old_locale );
			AddLog(_T("导出设备列表成功!...\r\n"));
		}
	}
	m_bBusy = false;
}

void CCommBacnetSearch::BacnetExportContinue_()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}
	m_bBusy = true;
	SetEventHandle(m_nEvent);
	AddLog(_T("Bacnet继续...\r\n"));
	m_bBusy = false;
}

void CCommBacnetSearch::BacnetExportSelectOne()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}
	m_bBusy = true;
	if(m_mapDeveice.size() <= 0)
	{
		AfxMessageBox(_T("请先扫描Bacnet设备"));
		m_bBusy = false;
		return;
	}
	m_bStoreS3db = false;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_bUpdateProgress = true;
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			m_bBusy = false;
			return;
		}
		m_strSavepath = strFilePath;
		//DeleteNode(m_hSelectNode,false);
		BacnetStructInfo *temStru=reinterpret_cast<BacnetStructInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
		if(temStru != NULL)
		{
			m_pCurrentBacnet = temStru;
			m_nDeviceCount = 1;
			m_nReadDeviceCount = 0;
			if(temStru->type == 1)
			{
				CString strKey = _T("");
				strKey.Format(_T("%s/%u"),temStru->strIP,temStru->nOldDeviceID);
				m_strSelectKey = strKey.GetString();

				if(temStru->bReadFinished)
				{
					ExportPoints(temStru);
				}
				else
				{
					hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.begin();
					while(iter != m_mapDeveice.end())
					{
						CString strKey_ = (*iter).first.c_str();
						if(strKey_ != strKey)
						{
							(*iter).second->bInore = true;
						}
						else
						{
							(*iter).second->ExportClear();
						}
						++iter;
					}
					HANDLE hExportAll = (HANDLE)_beginthreadex(NULL, 0 , ThreadExportAllFunc, this, NORMAL_PRIORITY_CLASS, NULL);
				}
				CString strLog;
				strLog.Format(_T("Bacnet导出一个设备(%s)...\r\n"),strKey);
				AddLog(strLog);
			}
		}
	}
	m_bBusy = false;
}

void CCommBacnetSearch::BacnetReadPointProperty()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}

	m_bBusy = true;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	DeleteNode(m_hSelectNode,false);
	BacnetStructInfo *temStru=reinterpret_cast<BacnetStructInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		m_pCurrentBacnet = temStru;
		if(temStru->type == 2)
		{
			CString strKey = _T("");
			strKey.Format(_T("%s/%u"),temStru->strIP,temStru->nOldDeviceID);
			m_strSelectKey = strKey.GetString();

			CString strLog;
			strLog.Format(_T("Bacnet读点属性(%s/%s/%u)...\r\n"),strKey,Project::Tools::AnsiToWideChar(bactext_object_type_name(temStru->nPointType)).c_str(),temStru->nPointAddress);
			AddLog(strLog);

			HANDLE hExportAll = (HANDLE)_beginthreadex(NULL, 0 , ThreadReadOnePropertyFunc, this, NORMAL_PRIORITY_CLASS, NULL);
		}
	}
	m_bBusy = false;
}

void CCommBacnetSearch::BacnetExportSelectMulti()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}
	m_bBusy = true;
	if(m_mapDeveice.size() <= 0)
	{
		AfxMessageBox(_T("请先扫描Bacnet设备"));
		m_bBusy = false;
		return;
	}
	m_bStoreS3db = false;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_bUpdateProgress = true;
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			m_bBusy = false;
			return;
		}
		m_strSavepath = strFilePath;

		CSelectBacnetDlg dlg;
		vector<wstring> vecAll,vecSelect;
		for(int i=0; i<m_vecDeviceIPID.size(); ++i)
		{
			hash_map<wstring,BacnetStructInfo*>::iterator itertDevice = m_mapDeveice.find(m_vecDeviceIPID[i]);
			if(itertDevice != m_mapDeveice.end())
			{
				wstring strDevice = (*itertDevice).first;
				(*itertDevice).second->bInore = true;
				vecAll.push_back(strDevice);
			}
		}
		dlg.m_vecBacnetList = vecAll;
		if(dlg.DoModal() == IDOK)
		{
			vecSelect = dlg.m_vecSelectBacnetList;
			m_nDeviceCount = vecSelect.size();
			m_nReadDeviceCount = 0;
			for(int i=0; i<vecSelect.size(); ++i)
			{
				wstring wstrKey = vecSelect[i];
				hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(wstrKey);
				if(iterDevice != m_mapDeveice.end())
				{
					BacnetStructInfo* pBacnetDevice = (*iterDevice).second;
					if(pBacnetDevice)
					{
						pBacnetDevice->ExportClear();
					}
				}
			}
			AddLog(_T("Bacnet导出选择...\r\n"));

			HANDLE hExportAll = (HANDLE)_beginthreadex(NULL, 0 , ThreadExportAllFunc, this, NORMAL_PRIORITY_CLASS, NULL);
		}
	}
	m_bBusy = false;
}

void CCommBacnetSearch::BacnetExportAllToPointFile()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}
	m_bBusy = true;
	AddLog(_T("Bacnet全部导出...\r\n"));
	if(m_mapDeveice.size() <= 0)
	{
		AfxMessageBox(_T("请先扫描Bacnet设备"));
		m_bBusy = false;
		return;
	}
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("POINT File (*.s3db)|*.s3db|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_bUpdateProgress = true;
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			return;
		}
		m_strSavepath = strFilePath;
		bool	bIsCreateOnly = false;
		CString	strSysBEOPFileName;
		CString strExePath;
		Project::Tools::GetSysPath(strExePath);
		strSysBEOPFileName.Format(L"%s\\config\\beopsys.s3db", strExePath);
		bIsCreateOnly = true;

		if(::CopyFile(strSysBEOPFileName, m_strSavepath, FALSE)==0)
		{
			AfxMessageBox(_T("创建点文件失败"));
			return;
		}
		m_bStoreS3db = true;
		CSqliteAcess access(m_strSavepath);
		access.DeleteAllPoint();

		BacnetStructInfo *temStru=reinterpret_cast<BacnetStructInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
		if(temStru != NULL)
		{
			m_pCurrentBacnet = temStru;
			if(temStru->type == 0)
			{
				m_nDeviceCount = m_mapDeveice.size();
				m_nReadDeviceCount = 0;
				hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.begin();
				while(iter != m_mapDeveice.end())
				{
					(*iter).second->ExportClear();
					++iter;
				}
			}
		}
		HANDLE hExportAll = (HANDLE)_beginthreadex(NULL, 0 , ThreadExportAllFunc, this, NORMAL_PRIORITY_CLASS, NULL);
	}

	AddLog(_T("Bacnet导出全部到s3db...\r\n"));
	m_bBusy = false;
}

void CCommBacnetSearch::BacnetSetting()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}
	m_bBusy = true;
	CBacnetSetDlg dlg;
	dlg.m_nBacnetTimeOut = m_nResponseTimeOut;
	dlg.m_nBacnetScanInterval = m_nBacnetScanInterval;
	dlg.m_nCharacterType = m_nCharacterType;
	dlg.m_nBacnetRetry = m_nRetryReadCount;
	dlg.m_nMutilReadCount = m_nMutilReadCount;
	if(dlg.DoModal() == IDOK)
	{
		m_nResponseTimeOut = dlg.m_nBacnetTimeOut;
		m_nBacnetScanInterval = dlg.m_nBacnetScanInterval;
		m_nCharacterType = dlg.m_nCharacterType;
		m_nRetryReadCount = dlg.m_nBacnetRetry;
		m_nMutilReadCount = dlg.m_nMutilReadCount;
	}
	m_bBusy = false;
	AddLog(_T("Bacnet设置...\r\n"));
}

void CCommBacnetSearch::BacnetReadAllPoint()
{
	if(m_bBusy)
	{
		MessageBox(_T("繁忙,请稍等"));
		return;
	}
	m_bBusy = true;
	if(m_mapDeveice.size() <= 0)
	{
		AfxMessageBox(_T("请先扫描Bacnet设备"));
		m_bBusy = false;
		return;
	}

	m_hSelectNode = m_wndFileView.GetSelectedItem();
	DeleteNode(m_hSelectNode);
	BacnetStructInfo *temStru=reinterpret_cast<BacnetStructInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		m_pCurrentBacnet = temStru;
		temStru->clear();
		if(temStru->type == 1)
		{
			m_bUpdateProgress = true;
			CString strKey = _T("");
			strKey.Format(_T("%s/%u"),temStru->strIP,temStru->nOldDeviceID);
			m_strSelectKey = strKey.GetString();

			CString strLog;
			strLog.Format(_T("Bacnet读取设备(%s)...\r\n"),strKey);
			AddLog(strLog);

			HANDLE hExportAll = (HANDLE)_beginthreadex(NULL, 0 , ThreadReadOneByOneFunc, this, NORMAL_PRIORITY_CLASS, NULL);
		}
	}
	m_bBusy = false;
}

void CCommBacnetSearch::ClearReadAccessData(BACNET_READ_ACCESS_DATA** ppRpmData)
{
	if(!ppRpmData) 
		return;

	BACNET_READ_ACCESS_DATA *pRpmData = *ppRpmData;
	BACNET_READ_ACCESS_DATA *old_rpm_object = NULL;
	BACNET_PROPERTY_REFERENCE *rpm_property = NULL;
	BACNET_PROPERTY_REFERENCE *old_rpm_property = NULL;
	BACNET_APPLICATION_DATA_VALUE *value = NULL;
	BACNET_APPLICATION_DATA_VALUE *old_value = NULL;
	while (pRpmData) 
	{
		rpm_property = pRpmData->listOfProperties;
		while (rpm_property)
		{
			value = rpm_property->value;			//释放value
			while (value) 
			{
				old_value = value;
				value = value->next;
				SAFE_DELETE(old_value);
			}
			old_rpm_property = rpm_property;
			rpm_property = rpm_property->next;
			SAFE_DELETE(old_rpm_property);
		}
		old_rpm_object = pRpmData;
		pRpmData = pRpmData->next;
		SAFE_DELETE(old_rpm_object);
	}
	*ppRpmData = NULL;
}

void CCommBacnetSearch::BuildPropRequest(BACNET_READ_ACCESS_DATA * rpm_object)
{
	 int i;
    /* To start with, StartNextObject() has prepopulated one propEntry,
     * but we will overwrite it and link more to it
     */
    BACNET_PROPERTY_REFERENCE *propEntry = rpm_object->listOfProperties;
    BACNET_PROPERTY_REFERENCE *oldEntry = rpm_object->listOfProperties;
    for (i = 0; Property_Value_List[i].property_id != -1; i++) {
        if (propEntry == NULL) {
            propEntry = new BACNET_PROPERTY_REFERENCE;
            assert(propEntry);
            oldEntry->next = propEntry;
        }
        propEntry->propertyIdentifier = (BACNET_PROPERTY_ID)Property_Value_List[i].property_id;
        propEntry->propertyArrayIndex = BACNET_ARRAY_ALL;
        oldEntry = propEntry;
		oldEntry->next = NULL;
        propEntry = NULL;
    }
}
//
//void CCommBacnetSearch::BuildPropRequest(BACNET_READ_ACCESS_DATA * rpm_object)
//{
//	BACNET_PROPERTY_REFERENCE *propEntry = rpm_object->listOfProperties;
//	BACNET_PROPERTY_REFERENCE *oldEntry = rpm_object->listOfProperties;
//	if (propEntry == NULL) {
//		propEntry = new BACNET_PROPERTY_REFERENCE;
//		assert(propEntry);
//		oldEntry->next = propEntry;
//	}
//	propEntry->propertyIdentifier = PROP_REQUIRED;
//	propEntry->propertyArrayIndex = BACNET_ARRAY_ALL;
//	oldEntry = propEntry;
//	oldEntry->next = NULL;
//	propEntry = NULL;
//}

void CCommBacnetSearch::BuildPropRequest_Description(BACNET_READ_ACCESS_DATA * rpm_object)
{
	BACNET_PROPERTY_REFERENCE *propEntry = rpm_object->listOfProperties;
	BACNET_PROPERTY_REFERENCE *oldEntry = rpm_object->listOfProperties;
	if (propEntry == NULL) {
		propEntry = new BACNET_PROPERTY_REFERENCE;
		assert(propEntry);
		oldEntry->next = propEntry;
	}
	propEntry->propertyIdentifier = PROP_DESCRIPTION;
	propEntry->propertyArrayIndex = BACNET_ARRAY_ALL;
	oldEntry = propEntry;
	oldEntry->next = NULL;
	propEntry = NULL;
}

void CCommBacnetSearch::StartNextObject(BACNET_READ_ACCESS_DATA * rpm_object,int nPointType,DWORD nPointAddress)
{
	BACNET_PROPERTY_REFERENCE *rpm_property;
	rpm_object->object_type = (BACNET_OBJECT_TYPE)nPointType;
	rpm_object->object_instance = nPointAddress;
	rpm_property = new BACNET_PROPERTY_REFERENCE;
	rpm_object->listOfProperties = rpm_property;
	assert(rpm_property);
	rpm_property->propertyIdentifier = PROP_OBJECT_LIST;
}

int CCommBacnetSearch::GetEventCode()
{
	return m_nEvent;
}

bool CCommBacnetSearch::ExportPoints(BacnetStructInfo* pBacnetInfo)
{
	if(pBacnetInfo)
	{
		m_nReadDeviceCount++;
		if(m_bStoreS3db == false)
		{
			char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
			setlocale( LC_ALL, "chs" );
			CStdioFile filehandle;
			if(filehandle.Open(m_strSavepath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
			{
				filehandle.Seek(0, CFile::end);
				CString strHeader,strInfo;
				strHeader.Format(_T("IP,oldId,ObjectName,pointType,pointAddress,Value,Description\n"));
				filehandle.WriteString(strHeader);
				int nErrCount = 0;
				for(int i=2; i<pBacnetInfo->nPointCount; ++i)
				{
					hash_map<DWORD,wstring>::iterator iterArray = pBacnetInfo->mapArrayIndex.find(i);
					if(iterArray != pBacnetInfo->mapArrayIndex.end())
					{
						wstring strKey = (*iterArray).second;
						hash_map<wstring,BacnetStructInfo*>::iterator iterPoint = pBacnetInfo->mapPoint.find(strKey);
						bool bExportSuccess = false;
						if(iterPoint != pBacnetInfo->mapPoint.end())
						{
							BacnetStructInfo* pBacnetPoint = (*iterPoint).second;
							if(pBacnetPoint)
							{
								bExportSuccess = true;
								CString strOut = GenerateBacnetInfo(pBacnetPoint);
								filehandle.WriteString(strOut);
								filehandle.WriteString(_T("\n"));
							}
						}
						if(!bExportSuccess)
						{
							nErrCount++;
						}
					}
				}
				if(nErrCount > 0)
				{
					CString strErr;
					strErr.Format(_T("\nERROR: %d/%d"),nErrCount,pBacnetInfo->nPointCount);
					filehandle.WriteString(strErr);
				}
				else
				{
					CString strErr;
					strErr.Format(_T("\nSuccess: %s:%u Export %d/%d"),pBacnetInfo->strIP,pBacnetInfo->nOldDeviceID,pBacnetInfo->nPointCount,pBacnetInfo->nPointCount);
					filehandle.WriteString(strErr);
				}
				filehandle.WriteString(_T("\n"));
				filehandle.Close();
				setlocale( LC_ALL, old_locale );	
				pBacnetInfo->bExportedFinished = true;

				CString strLog;
				strLog.Format(_T("导出Bacnet成功(%s/%d)...\r\n"),pBacnetInfo->strIP,pBacnetInfo->nOldDeviceID);
				AddLog(strLog);
				return true;
			}
			else
			{
				setlocale( LC_ALL, old_locale );
			}
		}
		else
		{
			SaveS3db(m_strSavepath,pBacnetInfo);
		}
	}
	return false;
}

CString CCommBacnetSearch::GenerateBacnetInfo(BacnetStructInfo* info)
{
	if(info)
	{
		CString strIP,strType;
		switch(info->nPointType)
		{
		case OBJECT_ANALOG_INPUT:
			strType = _T("AI");
			break;
		case OBJECT_ANALOG_OUTPUT:
			strType = _T("AO");
			break;
		case OBJECT_ANALOG_VALUE:
			strType = _T("AV");
			break;
		case OBJECT_BINARY_INPUT:
			strType = _T("BI");
			break;
		case OBJECT_BINARY_OUTPUT:
			strType = _T("BO");
			break;
		case OBJECT_BINARY_VALUE:
			strType = _T("BV");
			break;
		case OBJECT_MULTI_STATE_INPUT:
			strType = _T("MI");
			break;
		case OBJECT_MULTI_STATE_OUTPUT:
			strType = _T("MO");
			break;
		case OBJECT_MULTI_STATE_VALUE:
			strType = _T("MV");
			break;
		default:
			strType.Format(_T("未知类型(%d)"),info->nPointType);
			break;
		}

		CString strOut;
		strOut.Format(_T("%s,%u,%s,%s,%u,%f,%s"),info->strIP,info->nOldDeviceID,info->strObjectName,strType,info->nPointAddress,info->fValue,info->strDescription);
		return strOut;
	}
	return _T("");
}

bool CCommBacnetSearch::AddLog(CString strLog)
{
	m_strLOG+= strLog;
	m_strLOG += _T("\r\n");
	m_EditLog.SetWindowText(m_strLOG);
	m_EditLog.LineScroll(m_EditLog.GetLineCount()-1, 0);
	return true;
}

UINT WINAPI CCommBacnetSearch::ThreadTimeOutActiveFunc(LPVOID lparam)
{
	CCommBacnetSearch *pThis = static_cast<CCommBacnetSearch*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	while(!pThis->GetReadDataThreadExit())
	{
		COleDateTimeSpan oleSpan = COleDateTime::GetCurrentTime() - pThis->m_oleSendTime;
		int nTimeout = abs(oleSpan.GetTotalSeconds());
		if(((m_nEvent>=2 && m_nEvent <4) || m_nEvent == 7) && m_bUpdateProgress && nTimeout>=pThis->m_nTimeOut)
		{
			//激活
			pThis->TimeOutActive();
			int nActiveSleep = 5;
			while(!pThis->GetReadDataThreadExit())
			{
				if(nActiveSleep<=0)
					break;
				nActiveSleep--;
				Sleep(1000);
			}
		}
		Sleep(1000);
	}
	return 0;
}

bool CCommBacnetSearch::ShowDeviceInfo(BacnetStructInfo* info, HTREEITEM	hSelectNode)
{
	BacnetStructInfo *temStru=reinterpret_cast<BacnetStructInfo *>(m_wndFileView.GetItemData(hSelectNode));
	if(temStru != NULL)
	{
		if(temStru->type == 1)
		{
			CString strKey = _T("");
			strKey.Format(_T("%s/%u"),temStru->strIP,temStru->nOldDeviceID);
			wstring wstrKey = strKey.GetString();
			hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.find(wstrKey);
			if(iter != m_mapDeveice.end())
			{
				BacnetStructInfo* pBacnet = (*iter).second;
				if(pBacnet)
				{
					for(int i=2; i<pBacnet->nPointCount; ++i)
					{
						hash_map<DWORD,wstring>::iterator iterArray = pBacnet->mapArrayIndex.find(i);
						if(iterArray != pBacnet->mapArrayIndex.end())
						{
							wstring strKey = (*iterArray).second;
							hash_map<wstring,BacnetStructInfo*>::iterator iterPoint = pBacnet->mapPoint.find(strKey);
							if(iterPoint != pBacnet->mapPoint.end())
							{
								BacnetStructInfo* pBacnetPoint = (*iterPoint).second;
								if(pBacnetPoint)
								{
									HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strKey.c_str(),0,0,0,0,reinterpret_cast<DWORD>(pBacnetPoint),hSelectNode,TVI_LAST);
									m_wndFileView.Expand(hSelectNode,TVE_EXPAND);
								}
							}
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}

bool CCommBacnetSearch::ShowDevicePointProperty(BacnetStructInfo* info, HTREEITEM hSelectNode)
{
	BacnetStructInfo *temStru=reinterpret_cast<BacnetStructInfo *>(m_wndFileView.GetItemData(hSelectNode));
	DeleteNode(hSelectNode);
	if(temStru != NULL)
	{
		if(temStru->type == 2)
		{
			CString strKey = _T("");
			strKey.Format(_T("%s/%u"),temStru->strIP,temStru->nOldDeviceID);
			wstring wstrKey = strKey.GetString();
			hash_map<wstring,BacnetStructInfo*>::iterator iter = m_mapDeveice.find(wstrKey);
			if(iter != m_mapDeveice.end())
			{
				BacnetStructInfo* pBacnet = (*iter).second;
				if(pBacnet)
				{
					wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(temStru->nPointType));
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),temStru->nPointAddress);
					hash_map<wstring,BacnetStructInfo*>::iterator iter1 = pBacnet->mapPoint.find(strDeviceInfo.GetString());
					if(iter1 != pBacnet->mapPoint.end())
					{
						BacnetStructInfo* pBacnetPoint = (*iter1).second;
						if(pBacnetPoint)
						{
							CString strDeviceInfo;
							strDeviceInfo.Format(_T("对象名称:%s"),pBacnetPoint->strObjectName);
							HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,0,hSelectNode,TVI_LAST);
							strDeviceInfo.Format(_T("描述:%s"),pBacnetPoint->strDescription);
							hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,0,hSelectNode,TVI_LAST);
							strDeviceInfo.Format(_T("数值:%f"),pBacnetPoint->fValue);
							hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,0,hSelectNode,TVI_LAST);
							m_wndFileView.Expand(hSelectNode,TVE_EXPAND);
						}
					}
				}
			}
		}
	}
	return false;
}

bool CCommBacnetSearch::TimeOutActive()
{
	try
	{
		//激活
		CString strLog;
		strLog.Format(_T("激活事件(%d)...\r\n"),m_nEvent);
		AddLog(strLog);
		SetEventHandle(m_nEvent);
		return true;
	}
	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
	return false;
}

void CCommBacnetSearch::SaveS3db(CString strS3dbPath,BacnetStructInfo* pBacnetInfo)
{
	if(pBacnetInfo && strS3dbPath.GetLength()>0)
	{
		CSqliteAcess access(strS3dbPath);
		bool bOK = false;
		int nNeedSave = 0;
		int nSaved = 0;
		access.BeginTransaction();
		int nIndex = 0;

		for(int i=2; i<pBacnetInfo->nPointCount; ++i)
		{
			hash_map<DWORD,wstring>::iterator iterArray = pBacnetInfo->mapArrayIndex.find(i);
			if(iterArray != pBacnetInfo->mapArrayIndex.end())
			{
				wstring strKey = (*iterArray).second;
				hash_map<wstring,BacnetStructInfo*>::iterator iterPoint = pBacnetInfo->mapPoint.find(strKey);
				if(iterPoint != pBacnetInfo->mapPoint.end())
				{
					BacnetStructInfo* pBacnetPoint = (*iterPoint).second;
					if(pBacnetPoint)
					{
						CString strName_;
						strName_.Format(_T("%s_%d"),pBacnetPoint->strObjectName,nIndex);
						string strName;
						Project::Tools::WideCharToUtf8(strName_.GetString(),strName);
						string strDescription = "";
						if(pBacnetPoint->strDescription.GetLength()>0)
						{
							Project::Tools::WideCharToUtf8(pBacnetPoint->strDescription.GetString(),strDescription);
						}
						else
						{
							Project::Tools::WideCharToUtf8(pBacnetPoint->strObjectName.GetString(),strDescription);
						}

						std::ostringstream sqlstream;
						string strParam1 = Project::Tools::WideCharToAnsi((*iterPoint).first.c_str());
						
						string strParam2 = "";
						if(pBacnetPoint->nPointType>=0)
						{
							switch(pBacnetPoint->nPointType)
							{
							case OBJECT_ANALOG_INPUT:
								strParam2 = "AI";
								break;
							case OBJECT_ANALOG_OUTPUT:
								strParam2 = "AO";
								break;
							case OBJECT_ANALOG_VALUE:
								strParam2 = "AV";
								break;
							case OBJECT_BINARY_INPUT:
								strParam2 = "BI";
								break;
							case OBJECT_BINARY_OUTPUT:
								strParam2 = "BO";
								break;
							case OBJECT_BINARY_VALUE:
								strParam2 = "BV";
								break;
							case OBJECT_MULTI_STATE_INPUT:
								strParam2 = "MI";
								break;
							case OBJECT_MULTI_STATE_OUTPUT:
								strParam2 = "MO";
								break;
							case OBJECT_MULTI_STATE_VALUE:
								strParam2 = "MV";
								break;
							default:
								strParam2 = "UnSupport";
								break;
							}
						}
						string strParam3 = "";
						sqlstream.str("");
						sqlstream << pBacnetPoint->nPointAddress;
						strParam3 = sqlstream.str();

						if(pBacnetPoint->strObjectName.GetLength()>0 && strParam2 != "UnSupport")
						{
							if(access.InsertRecordToOPCPoint(nIndex,0,strName.c_str(),"bacnet",0,strDescription.c_str(),"",0,0,0,0,strParam1.c_str(),
								strParam2.c_str(),strParam3.c_str(),"","","","","","","","3","","","","",0,0,0) == 0)
							{
								nSaved++;
							}
						}
						nIndex++;
					}
				}
			}
		}
		access.CommitTransaction();
		pBacnetInfo->bExportedFinished = true;
		CString strLog;
		strLog.Format(_T("导出Bacnet成功(%s/%d)...\r\n"),pBacnetInfo->strIP,pBacnetInfo->nOldDeviceID);
		AddLog(strLog);
	}
}

void CCommBacnetSearch::BacnetSort()
{
	if(m_mapDeveice.size() >0)
	{
		m_hSelectNode = m_wndFileView.GetSelectedItem();
		DeleteNode(m_hSelectNode,false);

		m_vecDeviceIPID.clear();
		hash_map<wstring,BacnetStructInfo*>::iterator itertDevice = m_mapDeveice.begin();
		while(itertDevice != m_mapDeveice.end())
		{
			m_vecDeviceIPID.push_back((*itertDevice).first);
			++itertDevice;
		}

		//sort vecDeviceID
		std::sort(m_vecDeviceIPID.begin(), m_vecDeviceIPID.end(), CompareDeviceIPID);

		for(int i=0; i<m_vecDeviceIPID.size(); ++i)
		{
			hash_map<wstring,BacnetStructInfo*>::iterator itertDevice = m_mapDeveice.find(m_vecDeviceIPID[i]);
			if(itertDevice != m_mapDeveice.end())
			{
				BacnetStructInfo* pBacnetDevice = (*itertDevice).second;
				if(pBacnetDevice)
				{
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("%s:47808 Device ID:%u"),pBacnetDevice->strIP,pBacnetDevice->nOldDeviceID);
					HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,reinterpret_cast<DWORD>(pBacnetDevice),m_hSelectNode,TVI_LAST);
					m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);
				}
			}
		}
	}
}

void CCommBacnetSearch::StartNextObjectProperty(BACNET_READ_ACCESS_DATA * rpm_object,int nPointType,DWORD nPointAddress)
{
	BACNET_READ_ACCESS_DATA* pRpmData = rpm_object;
	pRpmData->object_type = (BACNET_OBJECT_TYPE)nPointType;	//point type
	pRpmData->object_instance = nPointAddress; //address
	pRpmData->listOfProperties = new BACNET_PROPERTY_REFERENCE;
	pRpmData->listOfProperties->propertyIdentifier = PROP_REQUIRED;
	//pRpmData->listOfProperties->propertyArrayIndex = BACNET_ARRAY_ALL;
	pRpmData->listOfProperties->next = NULL;
	pRpmData->next = new BACNET_READ_ACCESS_DATA;
	pRpmData = pRpmData->next ;
	pRpmData->object_type = (BACNET_OBJECT_TYPE)nPointType;	//point type
	pRpmData->object_instance = nPointAddress; //address
	pRpmData->listOfProperties = new BACNET_PROPERTY_REFERENCE;
	pRpmData->listOfProperties->propertyIdentifier = PROP_DESCRIPTION;
	//pRpmData->listOfProperties->propertyArrayIndex = BACNET_ARRAY_ALL;
	pRpmData->listOfProperties->next = NULL;
	pRpmData->next =NULL;
}

void CCommBacnetSearch::UCS2toUTF8(UINT8 *ucs2, int count, char *utf8)
{
	try
	{
		UINT16 unicode;
		unsigned char bytes[4] = {0};
		int nbytes = 0;
		int i = 0, j = 0;
		int len=0;

		if((ucs2 != NULL) && (utf8 != NULL))
		{
			if(count  == 0)
			{
				len = 0;
			}
			else
			{
				for (i=0; i<count/2; i++)
				{
					unicode = ucs2[2*i];
					unicode = (unicode&0x00ff)<<8;
					unicode |= ( ucs2[2*i+1]&0x00ff);

					if (unicode < 0x80)			//128
					{
						nbytes = 1;
						bytes[0] = unicode & 0x7F;
					}
					else if (unicode < 0x800)			//2048
					{
						nbytes = 2;
						bytes[1] = (unicode & 0x3F) | 0x80;
						bytes[0] = ((unicode >> 6) & 0x1F) | 0xc0;
					}
					else
					{
						nbytes = 3;
						bytes[2] = (unicode & 0x3F) | 0x80;
						bytes[1] = ((unicode >> 6) & 0x3F) | 0x80;
						bytes[0] = ((unicode >> 12) & 0x0F) | 0xE0;
					}

					for (j=0; j<nbytes; j++)
					{
						utf8[len] = bytes[j];
						len++;
					}
				}
			}
		}
	}
	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
}

void CCommBacnetSearch::UCS4toUTF8(UINT8 *ucs2, int count, char *utf8)
{
	try
	{
		UINT32 unicode;
		unsigned char bytes[4] = {0};
		int nbytes = 0;
		int i = 0, j = 0;
		int len=0;

		if((ucs2 != NULL) && (utf8 != NULL))
		{
			if(count  == 0)
			{
				len = 0;
			}
			else
			{
				for (i=0; i<count/4; i++)
				{
					unicode = ((ucs2[4*i] &0x00ff) << 24) | ((ucs2[4*i+1] & 0x00ff) << 16) | ((ucs2[4*i+2] & 0x00ff) << 8) | (ucs2[4*i+3] & 0x00ff);
					if (unicode < 0x80)			//128
					{
						nbytes = 1;
						bytes[0] = unicode & 0x7F;
					}
					else if (unicode < 0x800 && unicode >= 0x80)			//2048
					{
						nbytes = 2;
						bytes[1] = (unicode & 0x3F) | 0x80;
						bytes[0] = ((unicode >> 6) & 0x1F) | 0xc0;
					}
					else if (unicode < 0x00010000 && unicode >= 0x800)	
					{
						nbytes = 3;
						bytes[2] = (unicode & 0x3F) | 0x80;
						bytes[1] = ((unicode >> 6) & 0x3F) | 0x80;
						bytes[0] = ((unicode >> 12) & 0x0F) | 0xE0;
					}
					else if (unicode < 0x00200000 && unicode >= 0x00010000)	
					{
						nbytes = 4;
						bytes[3] = (unicode & 0x3F) | 0x80;
						bytes[2] = ((unicode >> 6) & 0x3F) | 0x80;
						bytes[1] = ((unicode >> 12) & 0x3F) | 0x80;
						bytes[0] = ((unicode >> 18) & 0x07) | 0xF0;
					}
					else if (unicode < 0x04000000 && unicode >= 0x00200000)	
					{
						nbytes = 5;
						bytes[4] = (unicode & 0x3F) | 0x80;
						bytes[3] = ((unicode >> 6) & 0x3F) | 0x80;
						bytes[2] = ((unicode >> 12) & 0x3F) | 0x80;
						bytes[1] = ((unicode >> 18) & 0x3F) | 0x80;
						bytes[0] = ((unicode >> 24) & 0x03) | 0xF8;
					}
					else if (unicode <= 0x7FFFFFFF && unicode >= 0x04000000)	
					{
						nbytes = 6;
						bytes[5] = (unicode & 0x3F) | 0x80;
						bytes[4] = ((unicode >> 6) & 0x3F) | 0x80;
						bytes[3] = ((unicode >> 12) & 0x3F) | 0x80;
						bytes[2] = ((unicode >> 18) & 0x3F) | 0x80;
						bytes[1] = ((unicode >> 24) & 0x3F) | 0x80;
						bytes[0] = ((unicode >> 24) & 0x01) | 0xFC;
					}

					for (j=0; j<nbytes; j++)
					{
						utf8[len] = bytes[j];
						len++;
					}
				}
			}
		}
	}
	catch (CMemoryException* e)
	{

	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
}

UINT WINAPI CCommBacnetSearch::ThreadSendCmdFunc( LPVOID lparam )
{
	CCommBacnetSearch *pThis = static_cast<CCommBacnetSearch*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;
	return 0;
}

UINT WINAPI CCommBacnetSearch::ThreadExportAllFunc( LPVOID lparam )
{
	CCommBacnetSearch *pThis = static_cast<CCommBacnetSearch*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	while(!pThis->GetReadDataThreadExit())
	{		
		//////////////////////////////////////////////////////////////////////////
		m_bBusy = true;
		for(int i=0; i<m_vecDeviceIPID.size(); ++i)
		{
			SetEventHandle(1);			//触发下一次命令事件

			if(pThis->GetReadDataThreadExit())
			{
				m_bBusy = false;
				return 0;
			}

			wstring strKey = m_vecDeviceIPID[i];
			hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(strKey);
			if(iterDevice != m_mapDeveice.end())
			{
				BacnetStructInfo* pBacnet = (*iterDevice).second;
				if(pBacnet)
				{
					if(!pBacnet->bReadDeviceCountSuccess && pBacnet->bNeedRead && !pBacnet->bInore)		//需要读取设备点数
					{
						int nTry = m_nRetryReadCount;
						while(nTry > 0 && !pBacnet->bReadDeviceCountSuccess)
						{
							nTry--;
							WaitForSingleObject(pThis->GetEventHandle(), m_nResponseTimeOut);			//等待触发下一次命令
							ReadObjectCount(pBacnet);
							m_pCurrentBacnet = pBacnet;
							Sleep(m_nCmdInterval);
						}
					}

					//读取设备点结构信息
					SetEventHandle(2);			//触发下一次命令事件
					if(pBacnet->bReadDeviceCountSuccess && !pBacnet->bReadDeviceInfoSuccess && pBacnet->bNeedRead && !pBacnet->bInore)		//需要读取设备点结构信息
					{
						int nReadLimit = pBacnet->nReadLimit;
						nReadLimit = (nReadLimit<m_nMutilReadCount)?nReadLimit:m_nMutilReadCount;
						int nReadNum = pBacnet->nPointCount/nReadLimit;
						int nLast = pBacnet->nPointCount%nReadLimit;				//说明还剩余一组
						if(nLast != 0)
							nReadNum = nReadNum+1;

						for(int i=0; i<nReadNum; ++i)
						{
							if(pThis->GetReadDataThreadExit())
							{
								m_bBusy = false;
								return 0;
							}

							WaitForSingleObject(pThis->GetEventHandle(), m_nResponseTimeOut);			//等待触发下一次命令
							if(i != nReadNum-1)		//不是最后一组
							{
								ReadObjectInfo(pBacnet,i*nReadLimit,(i+1)*nReadLimit);
							}
							else
							{
								ReadObjectInfo(pBacnet,i*nReadLimit,i*nReadLimit+nLast);
							}
							Sleep(m_nCmdInterval);
						}
					}

					//读取设备点属性信息
					SetEventHandle(3);			//触发下一次命令事件
					if(pBacnet->bReadDeviceInfoSuccess && !pBacnet->bReadDevicePointPropertySuccess && pBacnet->bNeedRead && !pBacnet->bInore)		//需要读取设备点属性信息
					{
						for(int i=0; i<pBacnet->nPointCount; ++i)
						{
							if(pThis->GetReadDataThreadExit())
							{
								m_bBusy = false;
								return 0;
							}

							pBacnet->nCurrentArrayIndex = i;
							hash_map<DWORD,wstring>::iterator iterArray = pBacnet->mapArrayIndex.find(i);
							if(iterArray != pBacnet->mapArrayIndex.end())
							{
								wstring strKey = (*iterArray).second;
								hash_map<wstring,BacnetStructInfo*>::iterator iterPoint = pBacnet->mapPoint.find(strKey);
								if(iterPoint != pBacnet->mapPoint.end())
								{
									BacnetStructInfo* pBacnetPoint = (*iterPoint).second;
									if(!pBacnetPoint->bReadPointPropertySuccess)
									{
										int nTry = m_nRetryReadCount;
										while(nTry > 0 && !pBacnetPoint->bReadPointPropertySuccess)
										{
											nTry--;
											WaitForSingleObject(pThis->GetEventHandle(), m_nResponseTimeOut);			//等待触发下一次命令
											ReadObjectProperty(pBacnetPoint);
											Sleep(m_nCmdInterval);
										}
									}
								}
							}
						}
					}

					//导出
					if(pBacnet->bReadDeviceCountSuccess && pBacnet->bReadDeviceInfoSuccess && !pBacnet->bInore)
					{
						ExportPoints(pBacnet);
					}
					else if(!pBacnet->bInore)		//失败时候导出 设备信息
					{
						ExportErr(pBacnet);
					}
				}
			}
		}
		m_bBusy = false;
		return 1;
	}
	return 0;
}

UINT WINAPI CCommBacnetSearch::ThreadReadOneByOneFunc( LPVOID lparam )
{
	CCommBacnetSearch *pThis = static_cast<CCommBacnetSearch*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	while(!pThis->GetReadDataThreadExit())
	{		
		//////////////////////////////////////////////////////////////////////////
		m_bBusy = true;
		SetEventHandle(1);			//触发下一次命令事件
		HTREEITEM	hSelectNode = m_hSelectNode;
		hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(m_strSelectKey);
		if(iterDevice != m_mapDeveice.end())
		{
			BacnetStructInfo* pBacnet = (*iterDevice).second;
			if(pBacnet)
			{
				if(!pBacnet->bReadDeviceCountSuccess && pBacnet->bNeedRead && !pBacnet->bInore)		//需要读取设备点数
				{
					int nTry = m_nRetryReadCount;
					while(nTry > 0 && !pBacnet->bReadDeviceCountSuccess)
					{
						nTry--;
						WaitForSingleObject(pThis->GetEventHandle(), m_nResponseTimeOut);			//等待触发下一次命令
						ReadObjectCount(pBacnet);
						m_pCurrentBacnet = pBacnet;
						Sleep(m_nCmdInterval);
					}
				}

				//读取设备点结构信息
				SetEventHandle(2);			//触发下一次命令事件
				if(pBacnet->bReadDeviceCountSuccess && !pBacnet->bReadDeviceInfoSuccess && pBacnet->bNeedRead && !pBacnet->bInore)		//需要读取设备点结构信息
				{
					int nReadLimit = pBacnet->nReadLimit;
					nReadLimit = (nReadLimit<m_nMutilReadCount)?nReadLimit:m_nMutilReadCount;
					int nReadNum = pBacnet->nPointCount/nReadLimit;
					int nLast = pBacnet->nPointCount%nReadLimit;				//说明还剩余一组
					if(nLast != 0)
						nReadNum = nReadNum+1;

					for(int i=0; i<nReadNum; ++i)
					{
						if(pThis->GetReadDataThreadExit())
						{
							m_bBusy = false;
							return 0;
						}
						WaitForSingleObject(pThis->GetEventHandle(), m_nResponseTimeOut);			//等待触发下一次命令
						if(i != nReadNum-1)		//不是最后一组
						{
							ReadObjectInfo(pBacnet,i*nReadLimit,(i+1)*nReadLimit);
						}
						else
						{
							ReadObjectInfo(pBacnet,i*nReadLimit,i*nReadLimit+nLast);
						}
						Sleep(m_nCmdInterval);
					}
				}

				//显示设备点结构信息
				if(pBacnet->bReadDeviceInfoSuccess)
					ShowDeviceInfo(pBacnet,hSelectNode);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		m_bBusy = false;
		return 1;
	}
	return 0;
}

UINT WINAPI CCommBacnetSearch::ThreadReadOnePropertyFunc( LPVOID lparam )
{
	CCommBacnetSearch *pThis = static_cast<CCommBacnetSearch*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	while(!pThis->GetReadDataThreadExit())
	{		
		//////////////////////////////////////////////////////////////////////////
		m_bBusy = true;
		SetEventHandle(1);			//触发下一次命令事件
		HTREEITEM	hSelectNode = m_hSelectNode;

		if(m_pCurrentBacnet)
		{
			CString strKey = _T("");
			strKey.Format(_T("%s/%u"),m_pCurrentBacnet->strIP,m_pCurrentBacnet->nOldDeviceID);
			wstring strKey_ = strKey.GetString();

			hash_map<wstring,BacnetStructInfo*>::iterator iterDevice = m_mapDeveice.find(strKey_);
			if(iterDevice != m_mapDeveice.end())
			{
				BacnetStructInfo* pBacnet = (*iterDevice).second;
				if(pBacnet)
				{
					wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(m_pCurrentBacnet->nPointType));
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),m_pCurrentBacnet->nPointAddress);
					hash_map<wstring,BacnetStructInfo*>::iterator iterPoint = pBacnet->mapPoint.find(strDeviceInfo.GetString());
					if(iterPoint != pBacnet->mapPoint.end())
					{
						int nTry = m_nRetryReadCount;
						while(nTry > 0 && !(*iterPoint).second->bReadPointPropertySuccess)
						{
							nTry--;
							WaitForSingleObject(pThis->GetEventHandle(), m_nResponseTimeOut);			//等待触发下一次命令
							ReadObjectProperty((*iterPoint).second);
							Sleep(m_nCmdInterval);
						}

						//如果是File
						if((*iterPoint).second->nPointType == OBJECT_FILE)
						{
							AtomicReadFile((*iterPoint).second);
							AddLog(_T("End AtomicReadFile"));
						}
						
						//显示
						ShowDevicePointProperty((*iterPoint).second,hSelectNode);
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		m_bBusy = false;
		return 1;
	}
	return 0;
}

bool CCommBacnetSearch::ExportErr( BacnetStructInfo* pBacnetInfo )
{
	if(pBacnetInfo)
	{
		m_nReadDeviceCount++;

		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
		setlocale( LC_ALL, "chs" );
		CStdioFile filehandle;
		if(filehandle.Open(m_strSavepath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			filehandle.Seek(0, CFile::end);
			CString strHeader,strInfo;
			strHeader.Format(_T("IP,oldId,ObjectName,pointType,pointAddress,Value,Description\n"));
			filehandle.WriteString(strHeader);
			
			CString strErr;
			strErr.Format(_T("ERROR: %d/%d."),pBacnetInfo->nPointCount,pBacnetInfo->nPointCount);
			filehandle.WriteString(strErr);

			filehandle.WriteString(_T("\n"));
			filehandle.Close();
			setlocale( LC_ALL, old_locale );	
			pBacnetInfo->bExportedFinished = true;

			CString strLog;
			strLog.Format(_T("导出Bacnet成功(%s/%d)...\r\n"),pBacnetInfo->strIP,pBacnetInfo->nOldDeviceID);
			AddLog(strLog);
			return true;
		}
		else
		{
			setlocale( LC_ALL, old_locale );
		}
	}
	return false;
}

void CCommBacnetSearch::MyAtomicReadFileAckHandler( uint8_t * service_request,uint16_t service_len,BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data )
{
	int len = 0;
	BACNET_ATOMIC_READ_FILE_DATA data;
	FILE *pFile = NULL; /* stream pointer */
	size_t octets_written = 0;

	(void) src; /* FIXME: validate the source address matches */
	len = arf_ack_decode_service_request(service_request, service_len, &data);
	if (len > 0) 
	{
		/* validate the parameters before storing data */
		if (data.access == FILE_STREAM_ACCESS) 
		{
			CString strObjectName = _T("");
			if(m_pBacnetFile)
			{
				strObjectName = m_pBacnetFile->strObjectName;
			}
			else
			{
				CString strLog;
				strLog.Format(_T("ERROR:Cannot Find DeviceID:%u,Type:%d,ddress %u"),m_dwCurrentOldDeviceID,data.object_type,data.object_instance);
				AddLog(strLog);
			}
			if(strObjectName.GetLength() <= 0)
			{
				strObjectName = _T("Default.txt");
			}
			string strObjectName_Ansi = Project::Tools::WideCharToAnsi(strObjectName);
			if (data.type.stream.fileStartPosition == 0)
			{
				pFile = fopen(strObjectName_Ansi.c_str(), "wb");
			}
			else
			{
				pFile = fopen(strObjectName_Ansi.c_str(), "rb+");
			}
			if (pFile)
			{
				/* is there anything to do with this? data.stream.requestedOctetCount */
				(void) fseek(pFile, data.type.stream.fileStartPosition,SEEK_SET);

				octets_written = fwrite(octetstring_value(&data.fileData), 1,   /* unit to write in bytes - in our case, an octet is one byte */
					octetstring_length(&data.fileData), pFile);
				fflush(pFile);
				fclose(pFile);

				CString strLog;
				if (octets_written != octetstring_length(&data.fileData)) 
				{
					strLog.Format(_T("Unable to write data to file %s"),strObjectName);
					AddLog(strLog);
				} 
				else if (octets_written == 0) 
				{
					AddLog(_T("Received 0 byte octet string"));
				} 
				else 
				{
					strLog.Format(_T("Received %d bytes"),data.type.stream.fileStartPosition + octets_written);
					AddLog(strLog);
				}
			}
			if (data.endOfFile) 
			{
				m_bEndEndFile = true;
			}
			
		}
		SetEventHandle(m_nEvent);
	}
}

void CCommBacnetSearch::MyAtomic_Read_File_Error_Handler( BACNET_ADDRESS * src,uint8_t invoke_id,BACNET_ERROR_CLASS error_class,BACNET_ERROR_CODE error_code )
{
	CString strError;
	strError.Format(_T("Error Class:%s Error Code: %s"),bactext_error_class_name(error_class),bactext_error_code_name(error_code));
	AddLog(strError);
}

//bool CCommBacnetSearch::AtomicReadFile( BacnetStructInfo* pBacnetInfo )
//{
//	if(pBacnetInfo)
//	{
//		m_bEndEndFile = false;
//
//		uint16_t my_max_apdu = 0;
//		uint8_t invoke_id = 0;
//		int fileStartPosition = 0;
//		unsigned requestedOctetCount = 0;
//		unsigned max_apdu = 0;
//		bool found = false;
//		bool bErrorDetected = false;
//		time_t elapsed_seconds = 0;
//		time_t last_seconds = 0;
//		time_t current_seconds = 0;
//		time_t timeout_seconds = 0;
//		DWORD dwDeviceID = pBacnetInfo->nNewDeviceID;
//		DWORD dwFileID = pBacnetInfo->nPointAddress;
//
//		/* configure the timeout values */
//		last_seconds = time(NULL);
//		timeout_seconds = (apdu_timeout() / 1000) * apdu_retries();
//		for (;;) 
//		{
//			/* increment timer - exit if timed out */
//			current_seconds = time(NULL);
//
//			/* wait until the device is bound, or timeout and quit */
//			BACNET_ADDRESS  targetAddr;
//			m_dwCurrentOldDeviceID = dwDeviceID;
//			found = address_bind_request(dwDeviceID, &max_apdu,&targetAddr);
//			if (found) 
//			{
//				/* calculate the smaller of our APDU size or theirs
//				   and remove the overhead of the APDU (about 16 octets max).
//				   note: we could fail if there is a bottle neck (router)
//				   and smaller MPDU in betweeen. */
//				if (max_apdu < MAX_APDU) 
//				{
//					my_max_apdu = max_apdu;
//				} else 
//				{
//					my_max_apdu = MAX_APDU;
//				}
//				/* Typical sizes are 50, 128, 206, 480, 1024, and 1476 octets */
//				if (my_max_apdu <= 50) 
//				{
//					requestedOctetCount = my_max_apdu - 20;
//				} 
//				else if (my_max_apdu <= 480) 
//				{
//					requestedOctetCount = my_max_apdu - 32;
//				} 
//				else if (my_max_apdu <= 1476) 
//				{
//					requestedOctetCount = my_max_apdu - 64;
//				} 
//				else 
//				{
//					requestedOctetCount = my_max_apdu / 2;
//				}
//
//				/* has the previous invoke id expired or returned?
//				   note: invoke ID = 0 is invalid, so it will be idle */
//				if ((invoke_id == 0) || tsm_invoke_id_free(invoke_id))
//				{
//					if (m_bEndEndFile || bErrorDetected)
//					{
//						break;
//					}
//					if (invoke_id != 0) 
//					{
//						fileStartPosition += requestedOctetCount;
//					}
//					/* we'll read the file in chunks
//					   less than max_apdu to keep unsegmented */
//					invoke_id = Send_Atomic_Read_File_Stream(dwDeviceID,dwFileID, fileStartPosition,requestedOctetCount);
//				} 
//				else if (tsm_invoke_id_failed(invoke_id)) 
//				{
//					AddLog(_T("ERROR: TSM Timeout!"));
//					tsm_free_invoke_id(invoke_id);
//					/* try again or abort? */
//					m_bEndEndFile = true;
//					break;
//				}
//			} 
//			else 
//			{
//				/* increment timer - exit if timed out */
//				elapsed_seconds += (current_seconds - last_seconds);
//				if (elapsed_seconds > timeout_seconds) 
//				{
//					AddLog(_T("ERROR: APDU Timeout!"));
//					m_bEndEndFile = true;
//					break;
//				}
//			}
//			/* keep track of time for next check */
//			last_seconds = current_seconds;
//		}
//		if (m_bEndEndFile) 
//		{
//			return true;
//		}
//	}
//	return false;
//}

bool CCommBacnetSearch::AtomicReadFile( BacnetStructInfo* pBacnetInfo )
{
	if(pBacnetInfo)
	{
		m_bEndEndFile = false;
		m_pBacnetFile = pBacnetInfo;

		uint16_t my_max_apdu = 0;
		uint8_t invoke_id = 0;
		int fileStartPosition = 0;
		unsigned requestedOctetCount = 0;
		unsigned max_apdu = 0;
		bool found = false;
		bool bErrorDetected = false;
		time_t elapsed_seconds = 0;
		time_t last_seconds = 0;
		time_t current_seconds = 0;
		time_t timeout_seconds = 0;
		m_dwCurrentOldDeviceID = pBacnetInfo->nOldDeviceID;
		DWORD dwDeviceID = pBacnetInfo->nNewDeviceID;
		DWORD dwFileID = pBacnetInfo->nPointAddress;
		int nTryCount = 0;
		/* configure the timeout values */
		last_seconds = time(NULL);
		timeout_seconds = (apdu_timeout() / 1000) * apdu_retries();
		
		tsm_timer_milliseconds(2000);
		tsm_free_all_invoke_id();

		for (;;) 
		{
			if(m_bEndEndFile)
				break;

			/* increment timer - exit if timed out */
			current_seconds = time(NULL);

			/* wait until the device is bound, or timeout and quit */
			BACNET_ADDRESS  targetAddr;
			found = address_bind_request(dwDeviceID, &max_apdu,&targetAddr);
			if (found) 
			{
				/* calculate the smaller of our APDU size or theirs
				   and remove the overhead of the APDU (about 16 octets max).
				   note: we could fail if there is a bottle neck (router)
				   and smaller MPDU in betweeen. */
				if (max_apdu < MAX_APDU) 
				{
					my_max_apdu = max_apdu;
				} else 
				{
					my_max_apdu = MAX_APDU;
				}
				/* Typical sizes are 50, 128, 206, 480, 1024, and 1476 octets */
				if (my_max_apdu <= 50) 
				{
					requestedOctetCount = my_max_apdu - 20;
				} 
				else if (my_max_apdu <= 480) 
				{
					requestedOctetCount = my_max_apdu - 32;
				} 
				else if (my_max_apdu <= 1476) 
				{
					requestedOctetCount = my_max_apdu - 64;
				} 
				else 
				{
					requestedOctetCount = my_max_apdu / 2;
				}

				/* has the previous invoke id expired or returned?
				   note: invoke ID = 0 is invalid, so it will be idle */
				WaitForSingleObject(GetEventHandle(), m_nResponseTimeOut);
				invoke_id = Send_Atomic_Read_File_Stream(dwDeviceID,dwFileID, fileStartPosition,requestedOctetCount);
				if (invoke_id != 0) 
				{
					if (current_seconds != last_seconds)
					{
						tsm_timer_milliseconds(((current_seconds - last_seconds) * 1000));
					}
					last_seconds = current_seconds;
					fileStartPosition += requestedOctetCount;
					//m_bEndEndFile = true;		//test
				}
				else
				{
					AddLog(_T("ERROR: invoke_id = 0"));
				}
			} 
			else
			{
				CString str;
				str.Format(_T("ERROR: Cannot Find Device:%u "),dwDeviceID);
				AddLog(str);
			}

			/* increment timer - exit if timed out */
			elapsed_seconds += (current_seconds - last_seconds);
			if (elapsed_seconds > timeout_seconds*10) 
			{
				AddLog(_T("ERROR: APDU Timeout!"));
				m_bEndEndFile = true;
				break;
			}
		}

		if (m_bEndEndFile) 
		{
			return true;
		}
	}
	return false;
}

// CCommBacnetSearch message handlers
