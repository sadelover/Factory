// ComunicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ComunicationDlg.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"
#include "BacnetSettingDlg.h"
#include "OPCCtrl.h"
#include <locale>
#include <codecvt>
#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>
#include "ExportBacnetIDDlg.h"
#include "DB_BasicIO/Sqlite/SqliteAcess.h"
#include "S7UDPCtrl.h"

object_functions_t Object_Table[] = {
	{DEVICE_OBJ_FUNCTIONS},
	{MAX_BACNET_OBJECT_TYPE, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
};

struct property_value_list_t {
	int32_t property_id;
	BACNET_APPLICATION_DATA_VALUE *value;
};
static struct property_value_list_t Property_Value_List[] = {
	{PROP_PRESENT_VALUE, NULL},
	{PROP_OBJECT_NAME, NULL},
	{PROP_DESCRIPTION, NULL},
	{-1, NULL}
};

int			CComunicationDlg::m_nReadMode;
int			CComunicationDlg::m_nCmdState;
int			CComunicationDlg::m_nPointCount;
int			CComunicationDlg::m_nCmdType;
int			CComunicationDlg::m_nLastReadObjectInvokeId;
int			CComunicationDlg::m_nLastReadPropertyInvokeId;
int			CComunicationDlg::m_nBacnetScanInterval;
int			CComunicationDlg::m_nProgressSend;
int			CComunicationDlg::m_nProgressRec;
int			CComunicationDlg::m_nProgressType;
int			CComunicationDlg::m_nExportMode;
int			CComunicationDlg::m_nCurrentDevice;
int			CComunicationDlg::m_nDeviceCount;
int			CComunicationDlg::m_nCurrendDeviceID;
int			CComunicationDlg::m_nDeviceStart;
int			CComunicationDlg::m_nDeviceEnd;
bool		CComunicationDlg::m_bStoreS3db;
int			CComunicationDlg::m_nCharacterType;
COleDateTime CComunicationDlg::m_oleLastReceiveResponse;
HANDLE		CComunicationDlg::m_sendcmdevent;
HANDLE		CComunicationDlg::m_exportcmdevent;
DWORD			CComunicationDlg::m_nSelectOldDeviceID;
DWORD			CComunicationDlg::m_nSelectNewDeviceID;
CViewTree	CComunicationDlg::m_wndFileView;
HTREEITEM	CComunicationDlg::m_hRoot;
HTREEITEM	CComunicationDlg::m_hSelectNode;
CRITICAL_SECTION CComunicationDlg::m_criticalSection = {0};
hash_map<wstring,BacnetInfo>	CComunicationDlg::m_mapAllPoint;
hash_map<wstring,BacnetInfo*>	CComunicationDlg::m_mapDeveice;
CString		CComunicationDlg::m_strSavepath;
vector<DWORD>	CComunicationDlg::m_vecDeviceID;

bool CompareDeviceID(const DWORD p1, const DWORD p2)
{
	return p1 < p2;
}

// CComunicationDlg dialog

IMPLEMENT_DYNAMIC(CComunicationDlg, CDialog)

CComunicationDlg::CComunicationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComunicationDlg::IDD, pParent)
	, m_readdata_thread(NULL)
	, m_read_thread(NULL)
	, m_progress_thread(NULL)
	, m_exit_readdata_thread(false)
	, m_nSlack(-1)
	, m_nSlot(-1)
	, m_strPlcPointAddress(_T("S7:[XX]DB1,REAL0"))
	, m_strPLCIP(_T(""))
	, m_dValue(0)
	, m_pS7UDPCtrl(NULL)
	, m_bConnectOK(false)
	, m_strLOG(_T(""))
	, m_OPCIP(0)
	, m_strOPCPointName(_T(""))
	, m_strOPCValue(_T(""))
	, m_strOPCType(_T(""))
	, m_strOPCIP(_T(""))
	, m_pOPC(NULL)
	, m_bConnectOPCOK(false)
	, m_bAddOPCItemSuccess(false)
	, m_bProperty(false)
{
	m_hRoot = NULL;
	m_hSelectNode = NULL;
	m_nReadMode = 0;
	m_nCmdState = 3;
	m_nExportMode = 0;
	m_nBacnetScanInterval = 50;
	m_nSelectOldDeviceID = 0;
	m_nSelectNewDeviceID = 0;
	m_nLastReadPropertyInvokeId = 0;
	m_nLastReadObjectInvokeId = 0;
	m_nProgressSend = 0;
	m_nProgressRec = 0;
	m_nProgressType = 0;
	m_nPointCount = 0;
	m_nCmdType = 0;
	m_nDeviceStart = 0;
	m_nDeviceEnd = 0;
	m_nCurrentDevice = 0;
	m_nDeviceCount = 0;
	m_nCurrendDeviceID = 0;
	m_nCharacterType = 0;
	m_bStoreS3db = false;

	m_oleLastReceiveResponse = COleDateTime::GetCurrentTime();
	m_mapAllPoint.clear();
	m_clsidmap.clear();
	m_mapDeveice.clear();
	m_strSavepath = _T("");
	m_nScanInterval = 1000;
	m_sendcmdevent = ::CreateEvent(NULL, false, false, L"");
	m_exportcmdevent = ::CreateEvent(NULL, false, false, L"");
}

CComunicationDlg::~CComunicationDlg()
{
	if (m_readdata_thread){
		CloseHandle(m_readdata_thread);
		m_readdata_thread = NULL;
	}

	if (m_read_thread){
		CloseHandle(m_read_thread);
		m_read_thread = NULL;
	}

	if(m_progress_thread)
	{
		CloseHandle(m_progress_thread);
		m_progress_thread = NULL;
	}

	m_bConnectOK = false;
	if(m_pS7UDPCtrl)
	{
		m_pS7UDPCtrl->ExitPLCConnection();
		delete m_pS7UDPCtrl;
		m_pS7UDPCtrl = NULL;
	}
}

void CComunicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_SEND, m_pProgressSend);
	DDX_Control(pDX, IDC_PROGRESS_REC, m_pProgressRec);
}


BEGIN_MESSAGE_MAP(CComunicationDlg, CDialog)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_BACNET_ROOT,SearchBacnet)
	ON_COMMAND(ID_MENU_BACNET_DEVICE,ReadAllBacnetPoint)
	ON_COMMAND(ID_MENU_BACNET_POINT,ReadPointProperty)
	ON_COMMAND(ID_MENU_BACNET_DEVICE_EXPORT,ExportAllBacnetPoint)
	ON_COMMAND(ID_MENU_BACNET_SETTING,BacnetSetting)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_ALL,BacnetExportAll)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_ALL_FILE,BacnetExportAllToPointFile)
	ON_COMMAND(ID_MENU_BACNET_SORT,BacnetSort)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_SELECT,BacnetExportSelect)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_CONTINUE,BacnetExportContinue)
	ON_COMMAND(ID_MENU_BACNET_EXPORT_BACNET,BacnetExportBacnet)
	//
END_MESSAGE_MAP()


// CComunicationDlg message handlers

bool CComunicationDlg::Exit()
{
	m_exit_readdata_thread = true;
	m_mapAllPoint.clear();
	m_vecDeviceID.clear();
	hash_map<wstring,BacnetInfo*>::iterator iter = m_mapDeveice.begin();
	while(iter != m_mapDeveice.end())		//未找到显示
	{
		BacnetInfo* info = (*iter).second;
		if(info != NULL)
		{
			delete info;
			info = NULL;
		}
		iter = m_mapDeveice.erase(iter);
	}
	m_nExportMode = 0;
	m_nCmdState = 3;
	m_nSelectOldDeviceID = 0;
	m_nSelectNewDeviceID = 0;
	m_nLastReadPropertyInvokeId = 0;
	m_nLastReadObjectInvokeId = 0;
	m_nPointCount = 0;
	m_nCmdType = 0;
	m_nCurrentDevice = 0;
	m_nDeviceCount = 0;
	m_nCurrendDeviceID = 0;
	//m_nScanInterval = 1000;
	WaitForSingleObject(m_readdata_thread, 2000);
	WaitForSingleObject(m_read_thread, 2000);
	WaitForSingleObject(m_progress_thread,2000);
	if (m_readdata_thread){
		CloseHandle(m_readdata_thread);
		m_readdata_thread = NULL;
	}

	if (m_read_thread){
		CloseHandle(m_read_thread);
		m_read_thread = NULL;
	}

	if(m_progress_thread)
	{
		CloseHandle(m_progress_thread);
		m_progress_thread = NULL;
	}
	datalink_cleanup();
	return true;
}

bool CComunicationDlg::InitData(bool bConnected)
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
	
	BacnetInfo* bacnetTreeNode=new BacnetInfo;
	bacnetTreeNode->type = 0;
	HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T("Bacnet搜索"),0,0,0,0,reinterpret_cast<DWORD>(bacnetTreeNode),TVI_ROOT,NULL);

	//if(Project::Tools::GetDebugLanguage() == 1)
	//	EnableControl(bConnected);
	return true;
}

void CComunicationDlg::OnContextMenu( CWnd* pWnd, CPoint point )
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

			BacnetInfo *temStru=reinterpret_cast<BacnetInfo *>(pWndTree->GetItemData(hTreeItem));
			if(temStru == NULL)
				return;

			// 创建自定义菜单
			CMenu *pMenu = new CMenu();
			VERIFY(pMenu->CreatePopupMenu());

			if(temStru->type==0)		//根节点
			{
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_ROOT,_T("Bacnet搜索"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_SETTING,_T("Bacnet设置"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_SORT,_T("Bacnet排序"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_BACNET,_T("Bacnet导出设备信息"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_ALL,_T("Bacnet导出全部"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_ALL_FILE,_T("Bacnet导出全部到s3db"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_SELECT,_T("Bacnet导出选择"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_EXPORT_CONTINUE,_T("Bacnet继续导出"));
			}
			else if(temStru->type==1)  //Bacnet设备
			{								
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_DEVICE,_T("读取设备列表"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_BACNET_DEVICE_EXPORT,_T("导出设备列表"));
			}
			else
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

void CComunicationDlg::SearchBacnet()
{
	Exit();
	m_exit_readdata_thread = false;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	DeleteNode(m_hSelectNode,false);
	Device_Set_Object_Instance_Number(BACNET_MAX_INSTANCE);
	//m_nReadMode = 0;
	//m_nBacnetScanInterval = 1000;
	address_init(m_nReadMode);
	InitServiceHandlers();
	dlenv_init();
	atexit(datalink_cleanup);
	CreateThreadRead();
	InitializeCriticalSection(&m_criticalSection);  //初始化临界区
}

void CComunicationDlg::ReadAllBacnetPoint()
{
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	DeleteNode(m_hSelectNode);
	m_mapAllPoint.clear();
	m_nProgressSend = 0;
	m_nProgressRec = 0;
	m_nProgressType = 0;
	BacnetInfo *temStru=reinterpret_cast<BacnetInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		if(temStru->type == 1)
		{
			m_nCmdState = 0;
			m_nSelectOldDeviceID = temStru->nOldDeviceID;
			m_nSelectNewDeviceID = temStru->nNewDeviceID;

			if(m_nReadMode == 0)
			{
				//0查询device个数
				int invoke_id =
					Send_Read_Property_Request(m_nSelectOldDeviceID, OBJECT_DEVICE,
					m_nSelectOldDeviceID, PROP_OBJECT_LIST,0);
			}
			else
			{
				//0查询device个数
				int invoke_id =
					Send_Read_Property_Request(m_nSelectNewDeviceID, OBJECT_DEVICE,
					m_nSelectOldDeviceID, PROP_OBJECT_LIST,0);
			}
		}
	}
}

void CComunicationDlg::ExportAllBacnetPoint()
{
	m_nExportMode = 0;
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			return;
		}
		m_strSavepath = strFilePath;
		
		m_hSelectNode = m_wndFileView.GetSelectedItem();
		BacnetInfo *temStru=reinterpret_cast<BacnetInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
		if(temStru != NULL)
		{
			if(temStru->type == 1)
			{
				hash_map<wstring,BacnetInfo>::iterator iter = m_mapAllPoint.begin();
				if(m_mapAllPoint.size()>0 && (*iter).second.nOldDeviceID == temStru->nOldDeviceID && (*iter).second.nNewDeviceID == temStru->nNewDeviceID)
				{
					ExportPoint();
				}
				else
				{
					ExportOneBacnetPoint(temStru);
				}
			}
		}
	}
}

void CComunicationDlg::ReadPointProperty()
{
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	DeleteNode(m_hSelectNode);
	BacnetInfo *temStru=reinterpret_cast<BacnetInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		if(temStru->type == 2)
		{
			m_nCmdState = 3;
			m_nCmdType = 1;
			m_nSelectOldDeviceID = temStru->nOldDeviceID;
			m_nSelectNewDeviceID = temStru->nNewDeviceID;

			//
			m_nProgressRec = 0;
			m_nProgressSend = 0;
			m_nProgressType = 4;
			ReadOnePointProperty(temStru->nPointType,temStru->nPointAddress);
		}
	}
}

void CComunicationDlg::InitServiceHandlers()
{
	Device_Initialize_Object_Functions(&Object_Table[0]);
	Device_Init();
	// we need to handle who-is
	//   to support dynamic device binding to us */
	//apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);

	// handle i-am to support binding to other devices */
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, CComunicationDlg::HandleIAM);

	// set the handler for all the services we don't implement
	//  It is required to send the proper reject message... */
	apdu_set_unrecognized_service_handler_handler(handler_unrecognized_service);

	// we must implement read property - it's required! */
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY, handler_read_property);

	//handle the ack coming back 
	apdu_set_confirmed_simple_ack_handler(SERVICE_CONFIRMED_WRITE_PROPERTY, CComunicationDlg::HandleWritePropertyAck);

	// handle the data coming back from confirmed requests */
	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY, CComunicationDlg::HandleReadPropertyAck);
	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE, CComunicationDlg::HandleReadPropertyMultiAck);

	// handle any errors coming back */
	apdu_set_error_handler(SERVICE_CONFIRMED_READ_PROPERTY, MyErrorHandler);
	apdu_set_abort_handler(MyAbortHandler);
	apdu_set_reject_handler(MyRejectHandler);
}

void CComunicationDlg::OnReceiveRead( BACNET_READ_PROPERTY_DATA* pData, UINT invokeId )
{
	ASSERT(pData);
	if(!pData)  return;
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

	switch(m_nCmdState)
	{
		case 0:
			{
				if(dataValue)
				{
					if(dataValue->tag == 2)
					{
						m_nCmdState = 2;
						m_nPointCount = dataValue->type.Unsigned_Int;
						SetBacnetDeviceCount(m_nSelectOldDeviceID,m_nSelectNewDeviceID,m_nPointCount);
						if(m_nPointCount <= 2)
						{
						
						}
						else
						{
							m_nCmdType = 0;
							SetCmdEvent();
						}				
					}
				}
			}
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			{
				if(dataValue)
				{
					if(dataValue->tag == 2)
					{
						m_nPointCount = dataValue->type.Unsigned_Int;
						SetBacnetDeviceCount(m_nSelectOldDeviceID,m_nSelectNewDeviceID,m_nPointCount);
						if(m_nPointCount <= 2)
						{
							m_nCmdType = 4;
							Sleep(2000);
							SetCmdEvent();
						}
						else
						{
							m_nCmdType = 2;
							SetCmdEvent();
						}					
					}
				}
			}
			break;
		default:
			break;
	}

	SAFE_DELETE(dataValue);
}

void CComunicationDlg::OnReceiveReadMultiple( BACNET_READ_ACCESS_DATA* pRpmData, UINT invokeId )
{
	ASSERT(pRpmData);
	if(!pRpmData)  return;

	BACNET_PROPERTY_REFERENCE *rpm_property = NULL;
	BACNET_APPLICATION_DATA_VALUE *dataValue = NULL;
	BACNET_APPLICATION_DATA_VALUE* old_value = NULL;
	BACNET_PROPERTY_REFERENCE *old_rpm_property = NULL;

	//AddLog(L"Enter handler multiple data back");
	BACNET_PROPERTY_ID pType = PROP_ACKED_TRANSITIONS;
	while (pRpmData)
	{
		rpm_property = pRpmData->listOfProperties;
		while (rpm_property) 
		{
			pType = rpm_property->propertyIdentifier;
			dataValue = rpm_property->value;
			CopyValue(dataValue,rpm_property->propertyIdentifier,pRpmData->object_type,pRpmData->object_instance,rpm_property->propertyArrayIndex);
			rpm_property = rpm_property->next;
		}
		pRpmData = pRpmData->next;
	}

	if(m_nLastReadObjectInvokeId == invokeId && pType == PROP_OBJECT_LIST)
	{
		m_nCmdType = 3;
		SetCmdEvent();
	}
	else if(m_nLastReadPropertyInvokeId == invokeId)
	{
		m_nCmdType = 4;
		SetCmdEvent();
	}
}

void CComunicationDlg::MyAbortHandler( BACNET_ADDRESS * src, uint8_t invoke_id, uint8_t abort_reason, bool server )
{
	//do nothing
}

void CComunicationDlg::MyErrorHandler( BACNET_ADDRESS * src, uint8_t invoke_id, BACNET_ERROR_CLASS error_class, BACNET_ERROR_CODE error_code )
{
	//do nothing
}

void CComunicationDlg::MyRejectHandler( BACNET_ADDRESS * src, uint8_t invoke_id, uint8_t reject_reason )
{
	//do nothing
}

void CComunicationDlg::HandleReadPropertyAck( UINT8 * service_request, UINT16 service_len, BACNET_ADDRESS * src, BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data )
{
	int len = 0;
	BACNET_READ_PROPERTY_DATA   data;

	len = rp_ack_decode_service_request(service_request, service_len, &data);
	if (len > 0) {
		OnReceiveRead(&data, service_data->invoke_id);
	}
}

void CComunicationDlg::HandleReadPropertyMultiAck( UINT8 * service_request, UINT16 service_len, BACNET_ADDRESS * src, BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data )
{
	int len = 0;
	//BACNET_READ_ACCESS_DATA rRmpdata;
	BACNET_READ_ACCESS_DATA *pRpmData = NULL;
	BACNET_READ_ACCESS_DATA *old_rpm_data = NULL;
	BACNET_PROPERTY_REFERENCE *rpm_property = NULL;
	BACNET_PROPERTY_REFERENCE *old_rpm_property = NULL;
	BACNET_APPLICATION_DATA_VALUE *dataValue = NULL;
	BACNET_APPLICATION_DATA_VALUE *old_value = NULL;

	const UINT invokeId = service_data->invoke_id;

	pRpmData = new BACNET_READ_ACCESS_DATA; //must delete
	//pRpmData = calloc(1, sizeof(BACNET_READ_ACCESS_DATA));
	EnterCriticalSection(&m_criticalSection); //等待进入临界区，进入后加锁使其他线程不能进入
	if (pRpmData) {
		len = rpm_ack_decode_service_request(service_request, service_len, pRpmData);
	}
	if (len > 0) {
		OnReceiveReadMultiple(pRpmData, invokeId);
	}

	//clear data
	ClearReadAccessData(&pRpmData);
	LeaveCriticalSection(&m_criticalSection);  //离开 开锁
}

void CComunicationDlg::HandleWritePropertyAck( BACNET_ADDRESS * src, uint8_t invoke_id )
{
	//do nothing
}

bool CComunicationDlg::CreateThreadRead()
{
	m_readdata_thread = (HANDLE)_beginthreadex(NULL,
		0,
		ThreadRead, 
		(LPVOID)this,
		NORMAL_PRIORITY_CLASS,// CREATE_SUSPENDED,
		NULL);

	m_read_thread = (HANDLE)_beginthreadex(NULL, 0 , ThreadReadAllDeviceFunc, this, NORMAL_PRIORITY_CLASS, NULL);
	m_progress_thread = (HANDLE)_beginthreadex(NULL, 0 , ThreadUpdateProgressFunc, this, NORMAL_PRIORITY_CLASS, NULL);
	return true;
}

void CComunicationDlg::ClearReadAccessData( BACNET_READ_ACCESS_DATA** ppRpmData )
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

UINT WINAPI CComunicationDlg::ThreadRead( LPVOID pParam )
{
	CComunicationDlg *pThis = static_cast<CComunicationDlg*>(pParam);
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

bool CComunicationDlg::GetReadDataThreadExit() const
{
	return m_exit_readdata_thread;
}

void CComunicationDlg::OnTimerRead()
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

void CComunicationDlg::HandleIAM( uint8_t * service_request, uint16_t service_len, BACNET_ADDRESS * src )
{
	int len = 0;
	uint32_t device_id = 0;
	unsigned max_apdu = 0;
	int segmentation = 0;
	uint16_t vendor_id = 0;

	(void) service_len;
	len =
		iam_decode_service_request(service_request, &device_id, &max_apdu,
		&segmentation, &vendor_id);

	if (len != -1)
	{
		CString strDeviceIp;
		strDeviceIp.Format(_T("%d.%d.%d.%d"),src->mac[0], src->mac[1], src->mac[2],
			src->mac[3]);

		CString strDeviceInfo;
		strDeviceInfo.Format(_T("%s:47808 Device ID:%d"),strDeviceIp,device_id);

		DWORD dwresult = 0;
		dwresult = inet_addr(Project::Tools::WideCharToAnsi(strDeviceIp).c_str());

		if(m_nReadMode == 0)
		{
			address_add(device_id, max_apdu, src);
		}
		else
		{
			address_add(dwresult, max_apdu, src);
		}

		hash_map<wstring,BacnetInfo*>::iterator iter = m_mapDeveice.find(strDeviceInfo.GetString());
		if(iter == m_mapDeveice.end())		//未找到显示
		{
			//BacnetInfo* bacnetTreeNode = NULL;
			BacnetInfo* bacnetTreeNode=new BacnetInfo;
			bacnetTreeNode->type = 1;
			bacnetTreeNode->nOldDeviceID = device_id;
			bacnetTreeNode->nNewDeviceID = dwresult;
			bacnetTreeNode->strIP = strDeviceIp;
			HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,reinterpret_cast<DWORD>(bacnetTreeNode),m_hSelectNode,TVI_LAST);
			m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);

			m_mapDeveice[strDeviceInfo.GetString()] = bacnetTreeNode;
			FindIsExist(m_vecDeviceID,device_id);
		}
	}


	return;
}

void CComunicationDlg::OnTimerWhois()
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

void CComunicationDlg::ReadObjectInfoByCount( DWORD nOldDeviceID,DWORD nNewDeviceID,int nCount ,int nOneCmdPoint)
{
	int nStart = 2;
	int nEnd = nStart;
	int nCmdCount = 0;
	for(int i=2; i<nCount; ++i)
	{
		nEnd++;
		if(nEnd-nStart >= nOneCmdPoint)
		{
			ReadObjectInfo(nOldDeviceID,nNewDeviceID,nStart,nEnd);
			nStart = nEnd;
			Sleep(m_nBacnetScanInterval);
		}
	}

	if(nStart != nEnd)
	{
		ReadObjectInfo(nOldDeviceID,nNewDeviceID,nStart,nEnd);
	}
}

void CComunicationDlg::ReadObjectInfo( DWORD nOldDeviceID,DWORD nNewDeviceID,int nStart,int nEnd )
{
	UINT deviceObjInstance = 0;
	if(m_nReadMode == 0)
	{
		deviceObjInstance = nOldDeviceID;
	}
	else
	{
		deviceObjInstance = nNewDeviceID;
	}

	UINT8 buffer[MAX_PDU] = {0};
	BACNET_READ_ACCESS_DATA *pReadAccessData = new BACNET_READ_ACCESS_DATA;
	if(!pReadAccessData) 
		return;
	BACNET_READ_ACCESS_DATA* pRpmData = pReadAccessData;
	for(int i=nStart; i<nEnd; ++i)
	{
		pRpmData->object_type = OBJECT_DEVICE;	//point type
		pRpmData->object_instance = nOldDeviceID; //address

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
	m_nProgressSend = nEnd;
	UINT max_apdu = 0;
	BACNET_ADDRESS  targetAddr;
	const bool ifFound = address_bind_request(deviceObjInstance, &max_apdu,  &targetAddr);
	if(ifFound)
	{
		const UINT8 reqInvokeId = Send_Read_Property_Multiple_Request(&buffer[0], sizeof(buffer), 
			deviceObjInstance,
			pReadAccessData);
	}
	LeaveCriticalSection(&m_criticalSection);  //离开 开锁
	Sleep(20); //sleep after network send.
	ClearReadAccessData(&pReadAccessData);
}

void CComunicationDlg::CopyValue( BACNET_APPLICATION_DATA_VALUE* pDataValue ,BACNET_PROPERTY_ID proType,BACNET_OBJECT_TYPE obType,DWORD obInstance,DWORD arrayIndex)
{
	if(!pDataValue) 
		return;
	m_oleLastReceiveResponse = COleDateTime::GetCurrentTime();
	if(proType == PROP_DESCRIPTION)			//描述
	{
		if(pDataValue->tag == BACNET_APPLICATION_TAG_CHARACTER_STRING)
		{
			if(m_nCmdState == 3|| m_nCmdState == 4)
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

				//strDescription = Project::Tools::AnsiToWideChar((const char*)characterstring_value(&pDataValue->type.Character_String));
				//Project::Tools::UTF8ToWideChar(characterstring_value(&pDataValue->type.Character_String),strDescription);
				if(m_nCmdType == 1)
				{
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("Description:%s"),strDescription.c_str());

					BacnetInfo* bacnetTreeNode=new BacnetInfo;
					bacnetTreeNode->type = 3;
					bacnetTreeNode->nOldDeviceID = m_nSelectOldDeviceID;
					bacnetTreeNode->nNewDeviceID = m_nSelectNewDeviceID;
					bacnetTreeNode->nPointAddress = obInstance;
					bacnetTreeNode->nPointType = obType;

					HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,reinterpret_cast<DWORD>(bacnetTreeNode),m_hSelectNode,TVI_LAST);
					m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);
				}
				else
				{
					wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(obType));
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),obInstance);

					m_mapAllPoint[strDeviceInfo.GetString()].nOldDeviceID = m_nSelectOldDeviceID;
					m_mapAllPoint[strDeviceInfo.GetString()].nNewDeviceID = m_nSelectNewDeviceID;
					m_mapAllPoint[strDeviceInfo.GetString()].strDescription = strDescription.c_str();
					m_mapAllPoint[strDeviceInfo.GetString()].nPointAddress = obInstance;
					m_mapAllPoint[strDeviceInfo.GetString()].nPointType = obType;
				}
			}
		}
	}
	else if(proType == PROP_PRESENT_VALUE)		//值
	{
		if(m_nCmdState == 3|| m_nCmdState == 4)
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
			default:
				break;
			}

			if(m_nCmdType == 1)
			{
				CString strDeviceInfo;
				strDeviceInfo.Format(_T("Value:%f"),fValue);
				BacnetInfo* bacnetTreeNode=new BacnetInfo;
				bacnetTreeNode->type = 3;
				bacnetTreeNode->nOldDeviceID = m_nSelectOldDeviceID;
				bacnetTreeNode->nNewDeviceID = m_nSelectNewDeviceID;
				bacnetTreeNode->nPointAddress = obInstance;
				bacnetTreeNode->nPointType = obType;

				HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,reinterpret_cast<DWORD>(bacnetTreeNode),m_hSelectNode,TVI_LAST);
				m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);
			}
			else
			{
				wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(obType));
				CString strDeviceInfo;
				strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),obInstance);

				m_mapAllPoint[strDeviceInfo.GetString()].nOldDeviceID = m_nSelectOldDeviceID;
				m_mapAllPoint[strDeviceInfo.GetString()].nNewDeviceID = m_nSelectNewDeviceID;
				m_mapAllPoint[strDeviceInfo.GetString()].nPointAddress = obInstance;
				m_mapAllPoint[strDeviceInfo.GetString()].nPointType = obType;
				m_mapAllPoint[strDeviceInfo.GetString()].fValue = fValue;
			}
		}
		
	}
	else if(proType == PROP_OBJECT_NAME)		//值
	{
		if(pDataValue->tag == BACNET_APPLICATION_TAG_CHARACTER_STRING)
		{
			if(m_nCmdState == 3|| m_nCmdState == 4)
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

				/*char* Description = characterstring_value(&pDataValue->type.Character_String);
				strDescription = Project::Tools::AnsiToWideChar((const char*)pDataValue->type.Character_String.value);
				strDescription = Project::Tools::UTF8ToWideChar(pDataValue->type.Character_String.value,pDataValue->type.Character_String.length);
				strDescription = UTF8ToWideChar_Del0(pDataValue->type.Character_String.value,pDataValue->type.Character_String.length);
				Project::Tools::UTF8ToWideChar(pDataValue->type.Character_String.value,strDescription);*/
				if(m_nCmdType == 1)
				{
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("项目名称:%s"),strDescription.c_str());

					BacnetInfo* bacnetTreeNode=new BacnetInfo;
					bacnetTreeNode->type = 3;
					bacnetTreeNode->nOldDeviceID = m_nSelectOldDeviceID;
					bacnetTreeNode->nNewDeviceID = m_nSelectNewDeviceID;
					bacnetTreeNode->nPointAddress = obInstance;
					bacnetTreeNode->nPointType = obType;

					HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,reinterpret_cast<DWORD>(bacnetTreeNode),m_hSelectNode,TVI_LAST);
					m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);
				}
				else
				{
					wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(obType));
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),obInstance);

					m_mapAllPoint[strDeviceInfo.GetString()].nOldDeviceID = m_nSelectOldDeviceID;
					m_mapAllPoint[strDeviceInfo.GetString()].nNewDeviceID = m_nSelectNewDeviceID;
					m_mapAllPoint[strDeviceInfo.GetString()].strObjectName = strDescription.c_str();
					m_mapAllPoint[strDeviceInfo.GetString()].nPointAddress = obInstance;
					m_mapAllPoint[strDeviceInfo.GetString()].nPointType = obType;
				}
				m_nProgressRec++;
			}
		}
	}
	else if(proType == PROP_OBJECT_LIST)		//列表
	{
		if(pDataValue->tag == BACNET_APPLICATION_TAG_OBJECT_ID)
		{
			if(pDataValue->type.Object_Id.type < MAX_BACNET_OBJECT_TYPE) 
			{
				if(m_nCmdState == 2 || m_nCmdState == 4)
				{
					wstring strTypeName = Project::Tools::AnsiToWideChar(bactext_object_type_name(pDataValue->type.Object_Id.type));
					DWORD nInstance = pDataValue->type.Object_Id.instance;
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("%s[%u]"),strTypeName.c_str(),nInstance);
					if(m_nCmdType != 2)
					{
						BacnetInfo* bacnetTreeNode=new BacnetInfo;
						bacnetTreeNode->type = 2;
						bacnetTreeNode->nOldDeviceID = m_nSelectOldDeviceID;
						bacnetTreeNode->nNewDeviceID = m_nSelectNewDeviceID;
						bacnetTreeNode->nPointAddress = nInstance;
						bacnetTreeNode->nPointType = pDataValue->type.Object_Id.type;
						bacnetTreeNode->nArrayIndex = arrayIndex;
						HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,reinterpret_cast<DWORD>(bacnetTreeNode),m_hSelectNode,TVI_LAST);
						m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);
					}
					else
					{
						BacnetInfo bInfo;
						bInfo.nNewDeviceID = m_nSelectNewDeviceID;
						bInfo.nOldDeviceID = m_nSelectOldDeviceID;
						bInfo.nPointType = pDataValue->type.Object_Id.type;
						bInfo.nPointAddress = nInstance;
						bInfo.nArrayIndex = arrayIndex;
						m_mapAllPoint[strDeviceInfo.GetString()] = bInfo;
					}
				}
				m_nProgressRec++;
			}
		}
	}
}

UINT WINAPI CComunicationDlg::ThreadReadAllDeviceFunc( LPVOID lparam )
{
	CComunicationDlg *pThis = static_cast<CComunicationDlg*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	while(!pThis->GetReadDataThreadExit())
	{
		// send Data
		DWORD waitstatus = WaitForSingleObject(pThis->GetSendCmdEvent(), INFINITE);
		if (waitstatus == WAIT_OBJECT_0)
		{
			//发送DTU
			pThis->SendCmd();					   			
		}

	}

	return 0;
}

HANDLE CComunicationDlg::GetSendCmdEvent()
{
	return m_sendcmdevent;
}

bool CComunicationDlg::SendCmd()
{
	if(m_nCmdType == 0)
	{
		//m_nPointCount = (m_nPointCount>20)?20:m_nPointCount;
		ReadObjectInfoByCount(m_nSelectOldDeviceID,m_nSelectNewDeviceID,m_nPointCount);
	}
	else if(m_nCmdType == 2)
	{
		m_nProgressType = 3;
		//m_nPointCount = (m_nPointCount>20)?20:m_nPointCount;
		ExportObjectInfoByCount(m_nSelectOldDeviceID,m_nSelectNewDeviceID,m_nPointCount,m_nLastReadObjectInvokeId);
	}
	else if(m_nCmdType == 3)
	{
		m_nProgressType = 4;
		m_nProgressSend = 0;
		m_nProgressRec = 0;
		ExportAllPointDescription();
	}
	else if(m_nCmdType == 4)
	{
		ExportPoint();
	}
	return true;
}

void CComunicationDlg::SetCmdEvent()
{
	SetEvent(m_sendcmdevent);
}

void CComunicationDlg::StartNextObject( BACNET_READ_ACCESS_DATA * rpm_object,int nPointType,DWORD nPointAddress)
{
	BACNET_PROPERTY_REFERENCE *rpm_property;
	rpm_object->object_type = (BACNET_OBJECT_TYPE)nPointType;
	rpm_object->object_instance = nPointAddress;
	rpm_property = new BACNET_PROPERTY_REFERENCE;
	rpm_object->listOfProperties = rpm_property;
	assert(rpm_property);
	rpm_property->propertyIdentifier = PROP_OBJECT_LIST;
}

bool CComunicationDlg::ReadOnePointProperty(int nPointType,DWORD nPointAddress)
{
	BACNET_READ_ACCESS_DATA *rpm_object = new BACNET_READ_ACCESS_DATA;
	StartNextObject(rpm_object,nPointType,nPointAddress);
	rpm_object->next = NULL;
	BuildPropRequest(rpm_object);

	EnterCriticalSection(&m_criticalSection); //等待进入临界区，进入后加锁使其他线程不能进入
	UINT max_apdu = 0;
	UINT8 buffer[MAX_PDU] = {0};
	BACNET_ADDRESS  targetAddr;

	UINT deviceObjInstance = 0;
	if(m_nReadMode == 0)
	{
		deviceObjInstance = m_nSelectOldDeviceID;
	}
	else
	{
		deviceObjInstance = m_nSelectNewDeviceID;
	}
	const bool ifFound = address_bind_request(deviceObjInstance, &max_apdu,  &targetAddr);
	if(ifFound)
	{
		const int nLastInvokeId = Send_Read_Property_Multiple_Request(&buffer[0], sizeof(buffer), 
			deviceObjInstance,
			rpm_object);
	}
	LeaveCriticalSection(&m_criticalSection);  //离开 开锁
	Sleep(20); //sleep after network send.
	ClearReadAccessData(&rpm_object);
	return true;
}

bool CComunicationDlg::ReadOnePointProperty( int nOldDeviceID,int nNewDeviceID,int nPointType,DWORD nPointAddress,int& nLastInvokeId )
{
	BACNET_READ_ACCESS_DATA *rpm_object = new BACNET_READ_ACCESS_DATA;
	StartNextObject(rpm_object,nPointType,nPointAddress);
	rpm_object->next = NULL;
	BuildPropRequest(rpm_object);

	EnterCriticalSection(&m_criticalSection); //等待进入临界区，进入后加锁使其他线程不能进入
	UINT max_apdu = 0;
	UINT8 buffer[MAX_PDU] = {0};
	BACNET_ADDRESS  targetAddr;
	UINT deviceObjInstance = 0;
	if(m_nReadMode == 0)
	{
		deviceObjInstance = nOldDeviceID;
	}
	else
	{
		deviceObjInstance = nNewDeviceID;
	}
	const bool ifFound = address_bind_request(deviceObjInstance, &max_apdu,  &targetAddr);
	if(ifFound)
	{
		nLastInvokeId = Send_Read_Property_Multiple_Request(&buffer[0], sizeof(buffer), 
			deviceObjInstance,
			rpm_object);
	}
	LeaveCriticalSection(&m_criticalSection);  //离开 开锁
	Sleep(20); //sleep after network send.
	ClearReadAccessData(&rpm_object);
	return true;
}

void CComunicationDlg::BuildPropRequest( BACNET_READ_ACCESS_DATA * rpm_object )
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

bool CComunicationDlg::DeleteNode( HTREEITEM hItem ,bool bReleaseNode)
{
	HTREEITEM hChild = m_wndFileView.GetChildItem(hItem);
	vector<HTREEITEM> vecDelete;
	while (hChild)
	{
		BacnetInfo *temStru=reinterpret_cast<BacnetInfo *>(m_wndFileView.GetItemData(hChild));
		if(temStru != NULL)
		{
			if(temStru->type == 1)
			{
				if(bReleaseNode)
				{
					CString strDeviceInfo;
					strDeviceInfo.Format(_T("%s:47808 Device ID:%d"),temStru->strIP,temStru->nOldDeviceID);
					hash_map<wstring,BacnetInfo*>::iterator iter = m_mapDeveice.find(strDeviceInfo.GetString());
					if(iter != m_mapDeveice.end())		//未找到显示
					{
						m_mapDeveice.erase(iter);
					}
				}
			}
			if(bReleaseNode)
			{
				delete temStru;
				temStru = NULL;
			}
		}
		vecDelete.push_back(hChild);
		hChild = m_wndFileView.GetNextItem(hChild, TVGN_NEXT);
	}
	for (size_t i=0;i<vecDelete.size();++i)
	{
		m_wndFileView.DeleteItem(vecDelete[i]);
	}

	return true;
}

void CComunicationDlg::ExportObjectInfoByCount( DWORD nOldDeviceID,DWORD nNewDeviceID,int nCount,int& nLastInvokeId,int nOneCmdPoint /*= 19*/ )
{
	/*int nStart = 2;
	int nEnd = nStart;
	int nCmdCount = 0;
	int nLastReadPropertyInvokeId = 0;
	for(int i=2; i<nCount; ++i)
	{
	nEnd++;
	if(nEnd-nStart >= nOneCmdPoint)
	{
	ExportObjectInfo(nOldDeviceID,nNewDeviceID,nStart,nEnd,nLastReadPropertyInvokeId);
	nStart = nEnd;
	Sleep(m_nBacnetScanInterval);
	}
	}

	if(nStart != nEnd)
	{
	ExportObjectInfo(nOldDeviceID,nNewDeviceID,nStart,nEnd,nLastReadPropertyInvokeId);
	}
	nLastInvokeId = nLastReadPropertyInvokeId;*/

	int nStart = 2;
	int nEnd = nStart;
	int nCmdCount = 0;
	int nLastReadObjectInvokeId = 0;
	for(int i=2; i<nCount; ++i)
	{
		nEnd++;
		if(nEnd-nStart >= nOneCmdPoint)
		{
			if(nEnd == nCount)
			{
				ExportObjectInfo(nOldDeviceID,nNewDeviceID,nStart,nEnd,nLastInvokeId);
			}
			else
			{
				ExportObjectInfo(nOldDeviceID,nNewDeviceID,nStart,nEnd,nLastReadObjectInvokeId);
			}
			nStart = nEnd;
			Sleep(m_nBacnetScanInterval);
		}
	}

	if(nStart != nEnd)
	{
		ExportObjectInfo(nOldDeviceID,nNewDeviceID,nStart,nEnd,nLastInvokeId);
	}
}

void CComunicationDlg::ExportObjectInfo( DWORD nOldDeviceID,DWORD nNewDeviceID,int nStart,int nEnd,int& nLastInvokeId )
{
	UINT deviceObjInstance = 0;
	if(m_nReadMode == 0)
	{
		deviceObjInstance = nOldDeviceID;
	}
	else
	{
		deviceObjInstance = nNewDeviceID;
	}

	UINT8 buffer[MAX_PDU] = {0};
	BACNET_READ_ACCESS_DATA *pReadAccessData = new BACNET_READ_ACCESS_DATA;
	if(!pReadAccessData) 
		return;
	BACNET_READ_ACCESS_DATA* pRpmData = pReadAccessData;
	for(int i=nStart; i<nEnd; ++i)
	{
		pRpmData->object_type = OBJECT_DEVICE;	//point type
		pRpmData->object_instance = nOldDeviceID; //address

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
	m_nProgressSend = nEnd;
	UINT max_apdu = 0;
	BACNET_ADDRESS  targetAddr;
	const bool ifFound = address_bind_request(deviceObjInstance, &max_apdu,  &targetAddr);
	if(ifFound)
	{
		nLastInvokeId = Send_Read_Property_Multiple_Request(&buffer[0], sizeof(buffer), 
			deviceObjInstance,
			pReadAccessData);
	}
	LeaveCriticalSection(&m_criticalSection);  //离开 开锁
	Sleep(20); //sleep after network send.
	ClearReadAccessData(&pReadAccessData);
}

void CComunicationDlg::ExportPoint()
{
	if(m_bStoreS3db == false)
	{
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
		setlocale( LC_ALL, "chs" );
		CStdioFile filehandle;
		if(filehandle.Open(m_strSavepath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			filehandle.Seek(0, CFile::end);
			hash_map<wstring,BacnetInfo>::const_iterator iter = m_mapAllPoint.begin();
			hash_map<DWORD,CString> mapAllPoint;
			mapAllPoint.clear();
			while(iter!= m_mapAllPoint.end())
			{
				BacnetInfo info = (*iter).second;
				CString strOut = GenerateBacnetInfo(info);
				mapAllPoint[info.nArrayIndex] = strOut;
				iter++;
			}
			CString strHeader,strInfo;
			strHeader.Format(_T("IP,oldId,newId,ObjectName,pointType,pointAddress,Description,Value\n"));
			filehandle.WriteString(_T("\n"));

			hash_map<wstring,BacnetInfo>::const_iterator iter3 = m_mapAllPoint.begin();
			if(iter3 != m_mapAllPoint.end())
			{
				BacnetInfo info = (*iter3).second;
				int nPointCount = 0;
				if(GetBacnetDeviceCount(info.nOldDeviceID,info.nNewDeviceID,nPointCount))
				{
					CString strIP;
					unsigned char *pIP;     
					pIP = (unsigned char*)&info.nNewDeviceID;   
					strIP.Format(_T("%u.%u.%u.%u"),*pIP,*(pIP+1),*(pIP+2),*(pIP+3)); 
					strInfo.Format(_T("IP:%s,oldId:%d,newId:%d,count:%d \n"),strIP,info.nOldDeviceID,info.nNewDeviceID,nPointCount);
					filehandle.WriteString(strInfo);
				}
			}

			filehandle.WriteString(strHeader);
			for(DWORD i=0; i<m_nPointCount; ++i)
			{
				hash_map<DWORD,CString>::const_iterator iter1 = mapAllPoint.find(i);
				if(iter1 != mapAllPoint.end())
				{
					filehandle.WriteString((*iter1).second);
					filehandle.WriteString(_T("\n"));
				}
			}
			filehandle.Close();
			setlocale( LC_ALL, old_locale );
			if(m_nExportMode == 0)
			{
				AfxMessageBox(_T("导出点成功"));
			}
			else
			{
				SetExportEvent();
			}
		}
		else
		{
			setlocale( LC_ALL, old_locale );
			if(m_nExportMode == 0)
			{
				AfxMessageBox(_T("Exporting Point Fail"));
			}
			else
			{
				SetExportEvent();
			}
		}
	}
	else
	{
		SaveS3db(m_strSavepath);
	}
}

void CComunicationDlg::ExportAllPointDescription()
{
	int nLastReadObjectInvokeId = 0;
	int nCount = 0;
	hash_map<wstring,BacnetInfo>::const_iterator iter = m_mapAllPoint.begin();
	while(iter!= m_mapAllPoint.end())
	{
		nCount++;
		m_nProgressSend = nCount;
		if(nCount != m_mapAllPoint.size())
		{
			ReadOnePointProperty((*iter).second.nOldDeviceID,(*iter).second.nNewDeviceID,(*iter).second.nPointType,(*iter).second.nPointAddress,nLastReadObjectInvokeId);
		}
		else
		{
			ReadOnePointProperty((*iter).second.nOldDeviceID,(*iter).second.nNewDeviceID,(*iter).second.nPointType,(*iter).second.nPointAddress,m_nLastReadPropertyInvokeId);
		}
		iter++;
		Sleep(m_nBacnetScanInterval);
	}
}

void CComunicationDlg::ReadObjectPropertyByCount( DWORD nOldDeviceID,DWORD nNewDeviceID,int nCount,int& nLastInvokeId,int nOneCmdPoint /*= 19*/ )
{
	int nStart = 2;
	int nEnd = nStart;
	int nCmdCount = 0;
	int nLastReadPropertyInvokeId = 0;
	for(int i=2; i<nCount; ++i)
	{
		nEnd++;
		if(nEnd-nStart >= nOneCmdPoint)
		{
			ReadObjectProperty(nOldDeviceID,nNewDeviceID,nStart,nEnd,nLastReadPropertyInvokeId);
			nStart = nEnd;
			Sleep(m_nBacnetScanInterval);
		}
	}

	if(nStart != nEnd)
	{
		ReadObjectProperty(nOldDeviceID,nNewDeviceID,nStart,nEnd,nLastReadPropertyInvokeId);
	}
	nLastInvokeId = nLastReadPropertyInvokeId;
}

void CComunicationDlg::ReadObjectProperty( DWORD nOldDeviceID,DWORD nNewDeviceID,int nStart,int nEnd ,int& nLastInvokeId)
{
	
}

bool CComunicationDlg::AddLog( CString strLog )
{
	m_strLOG+= strLog;
	m_strLOG+= L"\r\n";
	m_EditLog.SetWindowTextW(m_strLOG);
	m_EditLog.LineScroll(m_EditLog.GetLineCount()-1, 0);
	return true;
}

void CComunicationDlg::BacnetSetting()
{
	CBacnetSettingDlg dlg;
	dlg.m_nBacnetMode = m_nReadMode;
	dlg.m_nBacnetScanInterval = m_nBacnetScanInterval;
	dlg.m_nCharacterType = m_nCharacterType;
	if(dlg.DoModal() == IDOK)
	{
		m_nReadMode = dlg.m_nBacnetMode;
		m_nBacnetScanInterval = dlg.m_nBacnetScanInterval;
		m_nCharacterType = dlg.m_nCharacterType;
	}
}

UINT WINAPI CComunicationDlg::ThreadUpdateProgressFunc( LPVOID lparam )
{
	CComunicationDlg *pThis = static_cast<CComunicationDlg*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	while(!pThis->GetReadDataThreadExit())
	{
		pThis->UpdateProgress();
		//pThis->ActiveAndContinue();
		Sleep(1000);
	}

	return 0;
}

bool CComunicationDlg::UpdateProgress()
{
	if(m_nPointCount <= 0)
		return false;

	m_nDeviceCount = (m_nDeviceCount<=0)?1:m_nDeviceCount;
	int nSendPos = m_nProgressSend*100/m_nPointCount;
	int nRecPos = m_nProgressRec*100/m_nPointCount;
	int nDevicePos = m_nCurrentDevice*100/m_nDeviceCount;
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_SEND))->SetPos(nSendPos);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_REC))->SetPos(nRecPos);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_SEND_DEVICE))->SetPos(nDevicePos);
	CString strSendInfo,strRecInfo,strDeviceInfo;
	if(m_nProgressType == 0)
	{
		strSendInfo.Format(_T("发送读取设备信息(%d/%d)..."),m_nProgressSend,m_nPointCount);
		strRecInfo.Format(_T("接收读取设备信息(%d/%d)..."),m_nProgressRec,m_nPointCount);	
		((CStatic*)GetDlgItem(IDC_STATIC_SEND))->SetWindowText(strSendInfo);
		((CStatic*)GetDlgItem(IDC_STATIC_REC))->SetWindowText(strRecInfo);
		((CStatic*)GetDlgItem(IDC_STATIC_DEVICE))->SetWindowText(strDeviceInfo);
	}
	else if(m_nProgressType == 1)
	{
		strSendInfo.Format(_T("发送读取设备点属性(%d/%d)..."),m_nProgressSend,m_nPointCount);
		strRecInfo.Format(_T("接收读取设备点属性(%d/%d)..."),m_nProgressRec,m_nPointCount);	
		((CStatic*)GetDlgItem(IDC_STATIC_SEND))->SetWindowText(strSendInfo);
		((CStatic*)GetDlgItem(IDC_STATIC_REC))->SetWindowText(strRecInfo);
		((CStatic*)GetDlgItem(IDC_STATIC_DEVICE))->SetWindowText(strDeviceInfo);
	}
	else if(m_nProgressType == 2)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_SEND))->SetWindowText(_T("发送读取设备点最大数量信息命令"));
		((CStatic*)GetDlgItem(IDC_STATIC_REC))->SetWindowText(_T("发送读取设备点最大数量信息命令"));
		((CStatic*)GetDlgItem(IDC_STATIC_DEVICE))->SetWindowText(strDeviceInfo);
	}
	else if(m_nProgressType == 3)
	{
		strSendInfo.Format(_T("发送读取设备信息(%d/%d)..."),m_nProgressSend,m_nPointCount);
		strRecInfo.Format(_T("接收读取设备信息(%d/%d)..."),m_nProgressRec,m_nPointCount);	
		strDeviceInfo.Format(_T("扫描设备ID:%d点信息(%d/%d)..."),m_nCurrendDeviceID,m_nCurrentDevice,m_nDeviceCount);
		((CStatic*)GetDlgItem(IDC_STATIC_SEND))->SetWindowText(strSendInfo);
		((CStatic*)GetDlgItem(IDC_STATIC_REC))->SetWindowText(strRecInfo);
		((CStatic*)GetDlgItem(IDC_STATIC_DEVICE))->SetWindowText(strDeviceInfo);
	}
	else if(m_nProgressType == 4)
	{
		strSendInfo.Format(_T("发送读取设备点属性(%d/%d)..."),m_nProgressSend,m_nPointCount);
		strRecInfo.Format(_T("接收读取设备点属性(%d/%d)..."),m_nProgressRec,m_nPointCount);	
		strDeviceInfo.Format(_T("扫描设备ID:%d点属性(%d/%d)..."),m_nCurrendDeviceID,m_nCurrentDevice,m_nDeviceCount);
		((CStatic*)GetDlgItem(IDC_STATIC_SEND))->SetWindowText(strSendInfo);
		((CStatic*)GetDlgItem(IDC_STATIC_REC))->SetWindowText(strRecInfo);
		((CStatic*)GetDlgItem(IDC_STATIC_DEVICE))->SetWindowText(strDeviceInfo);
	}
}

CString CComunicationDlg::GenerateBacnetInfo( BacnetInfo info )
{
	CString strIP,strType;
	switch(info.nPointType)
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
		strType.Format(_T("UnSupport(%d)"),info.nPointType);
		break;
	}

	unsigned char *pIP;     
	pIP = (unsigned char*)&info.nNewDeviceID;   
	strIP.Format(_T("%u.%u.%u.%u"),*pIP,*(pIP+1),*(pIP+2),*(pIP+3));  

	CString strOut;
	strOut.Format(_T("%s,%u,%u,%s,%s,%u,%s,%f"),strIP,info.nOldDeviceID,info.nNewDeviceID,info.strObjectName,strType,info.nPointAddress,info.strDescription,info.fValue);
	return strOut;
}

BOOL CComunicationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

LRESULT CComunicationDlg::OnRecData( const char* buffer,DWORD userdata )
{
	CComunicationDlg *pCommunication =(CComunicationDlg *)userdata; 
	if(pCommunication)
	{
		pCommunication->AddLog(Project::Tools::AnsiToWideChar(buffer).c_str());
	}
	return 0;
}

void CComunicationDlg::GetValueSet( vector< pair<wstring, wstring> >& valuelist )
{

}

bool CComunicationDlg::UCS2_to_UTF8( UINT16* ucs2_code, UINT8* utf8_code )
{
	UINT8* out = utf8_code;

	if(!utf8_code)
	{
		return FALSE;
	}

	while(*ucs2_code != 0)
	{
		if(0x0080 > *ucs2_code)
		{
			/* 1 byte UTF-8 Character.*/
			*out = (UINT8)*ucs2_code;
			++out;
		}
		else if(0x0800 > *ucs2_code)
		{
			/*2 bytes UTF-8 Character.*/
			*out = ((UINT8)(*ucs2_code >> 6)) | 0xc0;
			*(out+1) = ((UINT8)(*ucs2_code & 0x003F)) | 0x80;
			out += 2;
		}
		else
		{
			/* 3 bytes UTF-8 Character .*/
			*out = ((UINT8)(*ucs2_code >> 12)) | 0xE0;
			*(out+1) = ((UINT8)((*ucs2_code & 0x0FC0)>> 6)) | 0x80;
			*(out+2) = ((UINT8)(*ucs2_code & 0x003F)) | 0x80;
			out += 3;
		}

		//挪动两个字节
		++ucs2_code;
	}

	return TRUE;
}

void CComunicationDlg::BacnetExportAll()
{
	m_nExportMode = 1;
	if(m_mapDeveice.size() <= 0)
	{
		AfxMessageBox(_T("请扫描Bacnet设备!"));
		return;
	}

	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			return;
		}
		m_strSavepath = strFilePath;
		m_bStoreS3db = false;
		HANDLE hExport = (HANDLE)_beginthreadex(NULL, 0 , ThreadExportAllPointFunc, this, NORMAL_PRIORITY_CLASS, NULL);
	}
}

bool CComunicationDlg::ExportOneBacnetPoint( BacnetInfo* temStru )
{
	if(temStru != NULL)
	{
		m_mapAllPoint.clear();
		m_nProgressSend = 0;
		m_nProgressRec = 0;
		m_nProgressType = 2;

		m_nCmdState = 4;
		m_nSelectOldDeviceID = temStru->nOldDeviceID;
		m_nSelectNewDeviceID = temStru->nNewDeviceID;

		if(m_nReadMode == 0)
		{
			//0查询device个数
			int invoke_id =
				Send_Read_Property_Request(m_nSelectOldDeviceID, OBJECT_DEVICE,
				m_nSelectOldDeviceID, PROP_OBJECT_LIST,0);
		}
		else
		{
			//0查询device个数
			int invoke_id =
				Send_Read_Property_Request(m_nSelectNewDeviceID, OBJECT_DEVICE,
				m_nSelectOldDeviceID, PROP_OBJECT_LIST,0);
		}
	}
	return true;
}

UINT WINAPI CComunicationDlg::ThreadExportAllPointFunc( LPVOID lparam )
{
	CComunicationDlg *pThis = static_cast<CComunicationDlg*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	vector<BacnetInfo*> vecAllDevice;
	hash_map<DWORD,vector<BacnetInfo*>> mapSum;
	m_vecDeviceID.clear();
	hash_map<wstring,BacnetInfo*>::iterator itert = m_mapDeveice.begin();
	while(itert != m_mapDeveice.end())
	{
		BacnetInfo* info = (*itert).second;
		if(info != NULL)
		{
			DWORD deviceID = info->nOldDeviceID;
			FindIsExist(m_vecDeviceID,deviceID);
			hash_map<DWORD,vector<BacnetInfo*>>::iterator iter1 = mapSum.find(deviceID);
			if(iter1 != mapSum.end())
			{
				(*iter1).second.push_back(info);
			}
			else
			{
				vector<BacnetInfo*> vec;
				vec.push_back(info);
				mapSum[deviceID] = vec;
			}
		}
		++itert;
	}

	//sort vecDeviceID
	std::sort(m_vecDeviceID.begin(), m_vecDeviceID.end(), CompareDeviceID);

	for(int i=0; i<m_vecDeviceID.size(); ++i)
	{
		hash_map<DWORD,vector<BacnetInfo*>>::iterator iter2 = mapSum.find(m_vecDeviceID[i]);
		if(iter2 != mapSum.end())
		{
			vector<BacnetInfo*> vecBacnet = (*iter2).second;
			for(int j=0;j<vecBacnet.size(); ++j)
			{
				BacnetInfo* temStru = vecBacnet[j];
				if(temStru != NULL)
				{
					vecAllDevice.push_back(temStru);
				}
			}
		}
	}

	//
	m_nDeviceCount = vecAllDevice.size();
	m_nCurrentDevice = 0;
	if(vecAllDevice.size() > 0&& !pThis->GetReadDataThreadExit())
	{
		//发送
		m_nCurrendDeviceID = vecAllDevice[m_nCurrentDevice]->nOldDeviceID;
		pThis->ExportOneBacnetPoint(vecAllDevice[m_nCurrentDevice]);
		m_nCurrentDevice++; 
	}

	for(m_nCurrentDevice;m_nCurrentDevice<m_nDeviceCount; ++m_nCurrentDevice )
	{
		DWORD waitstatus = WaitForSingleObject(pThis->GetExportEvent(), INFINITE);
		if (waitstatus == WAIT_OBJECT_0)
		{
			//发送
			m_nCurrendDeviceID = vecAllDevice[m_nCurrentDevice]->nOldDeviceID;
			pThis->ExportOneBacnetPoint(vecAllDevice[m_nCurrentDevice]);					   			
		}
		Sleep(1000);
	}

	if(!pThis->GetReadDataThreadExit())
	{
		DWORD waitstatus = WaitForSingleObject(pThis->GetExportEvent(), INFINITE);
		if (waitstatus == WAIT_OBJECT_0)
		{
			//发送DTU
			pThis->ExportFinished();					   			
		}
	}

	return 0;
}

void CComunicationDlg::SetExportEvent()
{
	SetEvent(m_exportcmdevent);
}

HANDLE CComunicationDlg::GetExportEvent()
{
	return m_exportcmdevent;
}

bool CComunicationDlg::ExportFinished()
{
	m_nCmdState = 3;
	AfxMessageBox(_T("导出点成功"));
	return true;
}

void CComunicationDlg::BacnetSort()
{
	if(m_mapDeveice.size() >0)
	{
		m_hSelectNode = m_wndFileView.GetSelectedItem();
		DeleteNode(m_hSelectNode,false);

		hash_map<DWORD,vector<BacnetInfo*>> mapSum;
		m_vecDeviceID.clear();
		hash_map<wstring,BacnetInfo*>::iterator itert = m_mapDeveice.begin();
		while(itert != m_mapDeveice.end())
		{
			BacnetInfo* info = (*itert).second;
			if(info != NULL)
			{
				DWORD deviceID = info->nOldDeviceID;
				FindIsExist(m_vecDeviceID,deviceID);
				hash_map<DWORD,vector<BacnetInfo*>>::iterator iter1 = mapSum.find(deviceID);
				if(iter1 != mapSum.end())
				{
					(*iter1).second.push_back(info);
				}
				else
				{
					vector<BacnetInfo*> vec;
					vec.push_back(info);
					mapSum[deviceID] = vec;
				}
			}
			++itert;
		}

		//sort vecDeviceID
		std::sort(m_vecDeviceID.begin(), m_vecDeviceID.end(), CompareDeviceID);

		for(int i=0; i<m_vecDeviceID.size(); ++i)
		{
			hash_map<DWORD,vector<BacnetInfo*>>::iterator iter2 = mapSum.find(m_vecDeviceID[i]);
			if(iter2 != mapSum.end())
			{
				vector<BacnetInfo*> vecBacnet = (*iter2).second;
				for(int j=0;j<vecBacnet.size(); ++j)
				{
					BacnetInfo* temStru = vecBacnet[j];
					if(temStru != NULL)
					{
						CString strDeviceInfo;
						strDeviceInfo.Format(_T("%s:47808 Device ID:%d"),temStru->strIP,temStru->nOldDeviceID);

						HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strDeviceInfo,0,0,0,0,reinterpret_cast<DWORD>(temStru),m_hSelectNode,TVI_LAST);
						m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);
					}
				}
			}
		}
	}
	
}

void CComunicationDlg::BacnetExportSelect()
{
	m_nExportMode = 1;
	if(m_mapDeveice.size() <= 0)
	{
		AfxMessageBox(_T("请扫描Bacnet设备!"));
		return;
	}

	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			return;
		}
		m_strSavepath = strFilePath;
		m_bStoreS3db = false;
		CExportBacnetIDDlg dlg;
		int nSize = m_vecDeviceID.size();
		if(nSize>0)
		{
			dlg.m_nBacnetStart = m_vecDeviceID[0];
			dlg.m_nBacnetEnd = m_vecDeviceID[nSize-1];
		}
		if(dlg.DoModal() == IDOK)
		{
			m_nDeviceStart = dlg.m_nBacnetStart;
			m_nDeviceEnd = dlg.m_nBacnetEnd;
			HANDLE hExport = (HANDLE)_beginthreadex(NULL, 0 , ThreadExportSelectPointFunc, this, NORMAL_PRIORITY_CLASS, NULL);
		}
	}
}

void CComunicationDlg::BacnetExportContinue()
{
	OutWarningErrLog();
	m_nCmdType = 4;
	SetCmdEvent();
}

UINT WINAPI CComunicationDlg::ThreadExportSelectPointFunc( LPVOID lparam )
{
	CComunicationDlg *pThis = static_cast<CComunicationDlg*>(lparam);
	ASSERT(pThis);
	if (NULL == pThis)
		return 1;

	vector<BacnetInfo*> vecAllDevice;
	hash_map<DWORD,vector<BacnetInfo*>> mapSum;
	m_vecDeviceID.clear();
	hash_map<wstring,BacnetInfo*>::iterator itert = m_mapDeveice.begin();
	while(itert != m_mapDeveice.end())
	{
		BacnetInfo* info = (*itert).second;
		if(info != NULL)
		{
			DWORD deviceID = info->nOldDeviceID;
			FindIsExist(m_vecDeviceID,deviceID);
			hash_map<DWORD,vector<BacnetInfo*>>::iterator iter1 = mapSum.find(deviceID);
			if(iter1 != mapSum.end())
			{
				(*iter1).second.push_back(info);
			}
			else
			{
				vector<BacnetInfo*> vec;
				vec.push_back(info);
				mapSum[deviceID] = vec;
			}
		}
		++itert;
	}

	//sort vecDeviceID
	int nIndex = m_nDeviceStart;
	for(nIndex;nIndex<=m_nDeviceEnd; ++nIndex)
	{
		hash_map<DWORD,vector<BacnetInfo*>>::iterator iter2 = mapSum.find(nIndex);
		if(iter2 != mapSum.end())
		{
			vector<BacnetInfo*> vecBacnet = (*iter2).second;
			for(int j=0;j<vecBacnet.size(); ++j)
			{
				BacnetInfo* temStru = vecBacnet[j];
				if(temStru != NULL)
				{
					vecAllDevice.push_back(temStru);
				}
			}
		}
	}

	//
	m_nDeviceCount = vecAllDevice.size();
	m_nCurrentDevice = 0;
	if(vecAllDevice.size() > 0&& !pThis->GetReadDataThreadExit())
	{
		//发送
		m_nCurrendDeviceID = vecAllDevice[m_nCurrentDevice]->nOldDeviceID;
		pThis->ExportOneBacnetPoint(vecAllDevice[m_nCurrentDevice]);
		m_nCurrentDevice++; 
	}

	for(m_nCurrentDevice;m_nCurrentDevice<m_nDeviceCount; ++m_nCurrentDevice )
	{
		DWORD waitstatus = WaitForSingleObject(pThis->GetExportEvent(), INFINITE);
		if (waitstatus == WAIT_OBJECT_0)
		{
			//发送
			m_nCurrendDeviceID = vecAllDevice[m_nCurrentDevice]->nOldDeviceID;
			pThis->ExportOneBacnetPoint(vecAllDevice[m_nCurrentDevice]);					   			
		}
		Sleep(1000);
	}

	if(!pThis->GetReadDataThreadExit())
	{
		DWORD waitstatus = WaitForSingleObject(pThis->GetExportEvent(), INFINITE);
		if (waitstatus == WAIT_OBJECT_0)
		{
			//发送DTU
			pThis->ExportFinished();					   			
		}
	}

	return 0;
}

bool CComunicationDlg::FindIsExist( vector<DWORD>& vec,DWORD deviceid )
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

bool CComunicationDlg::ActiveAndContinue()
{
	if(m_nCmdState == 4)		//导出时候
	{
		COleDateTimeSpan oleSpan = COleDateTime::GetCurrentTime() - m_oleLastReceiveResponse;
		if(oleSpan.GetTotalSeconds() >= 60)		//1分钟未收到回复
		{
			m_oleLastReceiveResponse = COleDateTime::GetCurrentTime();
			OutWarningErrLog();
			m_nCmdType = 4;
			SetCmdEvent();
		}
	}
	return true;
}

bool CComunicationDlg::OutWarningErrLog()
{
	wstring strFilePath;
	Project::Tools::GetSysPath(strFilePath);
	
	CString strLogPath;
	strLogPath.Format(_T("%s\\BacnetExportErr.log"),strFilePath.c_str());
	
	if(m_nCurrendDeviceID >0)
	{
		CString strLog;
		strLog.Format(_T("DeviceID:%d maybe error,please check...\n"),m_nCurrendDeviceID);
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
			str.Format(_T("%s::%s"),strTime.c_str(),strLog);
			ff.Seek(0,CFile::end);
			ff.WriteString(str);
			ff.Close();
			setlocale( LC_ALL, old_locale );
			free(old_locale);
			return true;
		}
	}
}

void CComunicationDlg::BacnetExportBacnet()
{
	m_nExportMode = 0;
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			return;
		}
		
		m_bStoreS3db = false;
		//
		hash_map<DWORD,vector<BacnetInfo*>> mapSum;
		m_vecDeviceID.clear();
		hash_map<wstring,BacnetInfo*>::iterator itert = m_mapDeveice.begin();
		while(itert != m_mapDeveice.end())
		{
			BacnetInfo* info = (*itert).second;
			if(info != NULL)
			{
				DWORD deviceID = info->nOldDeviceID;
				FindIsExist(m_vecDeviceID,deviceID);
				hash_map<DWORD,vector<BacnetInfo*>>::iterator iter1 = mapSum.find(deviceID);
				if(iter1 != mapSum.end())
				{
					(*iter1).second.push_back(info);
				}
				else
				{
					vector<BacnetInfo*> vec;
					vec.push_back(info);
					mapSum[deviceID] = vec;
				}
			}
			++itert;
		}

		//sort vecDeviceID
		std::sort(m_vecDeviceID.begin(), m_vecDeviceID.end(), CompareDeviceID);


		//写文件
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
		setlocale( LC_ALL, "chs" );
		CStdioFile filehandle;
		if(filehandle.Open(strFilePath, CFile::modeCreate|CFile::modeWrite))
		{
			filehandle.Seek(0, CFile::begin);
			filehandle.WriteString(_T("IP:Port,Device ID\n"));	
			for(int i=0; i<m_vecDeviceID.size(); ++i)
			{
				hash_map<DWORD,vector<BacnetInfo*>>::iterator iter2 = mapSum.find(m_vecDeviceID[i]);
				if(iter2 != mapSum.end())
				{
					vector<BacnetInfo*> vecBacnet = (*iter2).second;
					for(int j=0;j<vecBacnet.size(); ++j)
					{
						BacnetInfo* temStru = vecBacnet[j];
						if(temStru != NULL)
						{
							CString strDeviceInfo;
							strDeviceInfo.Format(_T("%s:47808,%d\n"),temStru->strIP,temStru->nOldDeviceID);
							filehandle.WriteString(strDeviceInfo);						
						}
					}
				}
			}
			filehandle.Close();
			setlocale( LC_ALL, old_locale );
			AfxMessageBox(_T("出口设备列表成功!"));
		}
	}
}

wstring CComunicationDlg::UTF8ToWideChar_Del0( char* cOld, int length )
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

wstring CComunicationDlg::AnsiToWideChar_Del0( char* cOld, int length )
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

void CComunicationDlg::SetBacnetDeviceCount( int nOldDeviceID,int nNewDeviceID, int nCount )
{
	hash_map<wstring,BacnetInfo*>::iterator itert = m_mapDeveice.begin();
	while(itert != m_mapDeveice.end())
	{
		BacnetInfo* info = (*itert).second;
		if(info != NULL)
		{
			DWORD deviceID = info->nOldDeviceID;
			if(info->nOldDeviceID == nOldDeviceID && info->nNewDeviceID == nNewDeviceID)
			{
				info->nPointCount = nCount;
			}
		}
		++itert;
	}
}

bool CComunicationDlg::GetBacnetDeviceCount( int nOldDeviceID,int nNewDeviceID, int& nCount )
{
	nCount = 0;
	hash_map<wstring,BacnetInfo*>::iterator itert = m_mapDeveice.begin();
	while(itert != m_mapDeveice.end())
	{
		BacnetInfo* info = (*itert).second;
		if(info != NULL)
		{
			DWORD deviceID = info->nOldDeviceID;
			if(info->nOldDeviceID == nOldDeviceID && info->nNewDeviceID == nNewDeviceID)
			{
				nCount = info->nPointCount;
				return true;
			}
		}
		++itert;
	}
	return false;
}

bool CComunicationDlg::EnableControl( bool bConnected /*= true*/ )
{
	((CStatic*)GetDlgItem(IDC_STATIC_PLC))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_OPC))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_LOG))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_1))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_2))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_3))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_4))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_5))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_6))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_7))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_8))->ShowWindow(bConnected);
	((CStatic*)GetDlgItem(IDC_STATIC_9))->ShowWindow(bConnected);
	
	((CEdit*)GetDlgItem(IDC_PLC_ADDRESS))->ShowWindow(bConnected);
	((CEdit*)GetDlgItem(IDC_EDIT_SLACK))->ShowWindow(bConnected);
	((CEdit*)GetDlgItem(IDC_EDIT_SLOT))->ShowWindow(bConnected);
	((CEdit*)GetDlgItem(IDC_EDIT_PLC_POINT_ADDRESS))->ShowWindow(bConnected);
	((CEdit*)GetDlgItem(IDC_EDIT_PLC_POINT_VALUE))->ShowWindow(bConnected);
	((CEdit*)GetDlgItem(IDC_PLC_ADDRESS_TCP2))->ShowWindow(bConnected);
	((CEdit*)GetDlgItem(IDC_EDIT_PLC_POINT_ADDRESS2))->ShowWindow(bConnected);
	((CEdit*)GetDlgItem(IDC_EDIT_PLC_POINT_VALUE2))->ShowWindow(bConnected);
	((CEdit*)GetDlgItem(IDC_EDIT_LOG_COM))->ShowWindow(bConnected);
	
	((CButton*)GetDlgItem(IDC_BUTTON_CONNECT_PLC))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_CONNECT_PLC_MAUAL))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_DISCONNECT))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_READ))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_WRITE))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_SEARCHOPC))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_COMBO_OPC))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_CHECK_PROPERTY))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_CONNECT_OPC))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_EXPORT))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_READ_OPC))->ShowWindow(bConnected);
	((CButton*)GetDlgItem(IDC_BUTTON_WRITE_OPC))->ShowWindow(bConnected);
	return true;
}

void CComunicationDlg::ExportPoint_S3DB()
{
	SaveS3db(m_strSavepath);
}

void CComunicationDlg::ExportPoint_Excel()
{

}

void CComunicationDlg::SaveS3db( CString strS3dbPath)
{
	hash_map<wstring,BacnetInfo>::const_iterator iter = m_mapAllPoint.begin();
	CSqliteAcess access(strS3dbPath);
	bool bOK = false;
	int nNeedSave = 0;
	int nSaved = 0;
	access.BeginTransaction();
	int nIndex = 0;
	while(iter!= m_mapAllPoint.end())
	{
		BacnetInfo info = (*iter).second;
		CString strName_;
		strName_.Format(_T("%s_%d"),info.strObjectName,nIndex);
		string strName;
		Project::Tools::WideCharToUtf8(strName_.GetString(),strName);
		string strDescription = "";
		if(info.strDescription.GetLength()>0)
		{
			Project::Tools::WideCharToUtf8(info.strDescription.GetString(),strDescription);
		}
		else
		{
			Project::Tools::WideCharToUtf8(info.strObjectName.GetString(),strDescription);
		}

		std::ostringstream sqlstream;
		string strParam1 = "";
		if(m_nReadMode == 0)
		{
			sqlstream.str("");
			sqlstream << info.nOldDeviceID;
			strParam1 = sqlstream.str();
		}
		else
		{
			sqlstream.str("");
			sqlstream << info.nNewDeviceID;
			strParam1 = sqlstream.str();
		}
		string strParam2 = "";
		if(info.nPointType>=0)
		{
			switch(info.nPointType)
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
		sqlstream << info.nPointAddress;
		strParam3 = sqlstream.str();

		if(info.strObjectName.GetLength()>0 && strParam2 != "UnSupport")
		{
			if(access.InsertRecordToOPCPoint(nIndex,0,strName.c_str(),"bacnet",0,strDescription.c_str(),"",0,0,0,0,strParam1.c_str(),
				strParam2.c_str(),strParam3.c_str(),"","","","","","","","3","","","","",0,0,0) == 0)
			{
				nSaved++;
			}
		}
		nIndex++;
		iter++;
	}
	access.CommitTransaction();

	if(m_nExportMode == 0)
	{
		AfxMessageBox(_T("导出点成功"));
	}
	else
	{
		SetExportEvent();
	}
}

void CComunicationDlg::BacnetExportAllToPointFile()
{
	m_nExportMode = 1;
	if(m_mapDeveice.size() <= 0)
	{
		AfxMessageBox(_T("请扫描Bacnet设备!"));
		return;
	}

	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("POINT File (*.s3db)|*.s3db|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
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
			AfxMessageBox(_T("创建点文件失败!"));
			return;
		}
		m_bStoreS3db = true;
		CSqliteAcess access(m_strSavepath);
		access.DeleteAllPoint();

		HANDLE hExport = (HANDLE)_beginthreadex(NULL, 0 , ThreadExportAllPointFunc, this, NORMAL_PRIORITY_CLASS, NULL);
	}
}
