#pragma once
#include "ViewTree.h"
#include <hash_map>
#include "./bacnet/include/bacdef.h"
#include "./bacnet/include/apdu.h"
#include "./bacnet/include/tsm.h"
#include "./bacnet/include/address.h"
#include "./bacnet/include/device.h"
#include "./bacnet/include/datalink.h"
#include "./bacnet/include/handlers.h"
#include "./bacnet/include/client.h"
#include "./bacnet/include/dlenv.h"
#include "./bacnet/include/iam.h"
#include "./bacnet/include/bactext.h"
#include "./bacnet/include//arf.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif

struct BacnetStructInfo
{
	BacnetStructInfo()
	{
		nReadLimit = 19;
		type = 0;
		nOldDeviceID = 0;
		nNewDeviceID = 0;
		nPointType = 0;
		nPointCount = 0;
		nPointAddress = 0;
		nArrayIndex = 0;
		fValue = 0;
		nExportPortCount = 0;
		nCurrentArrayIndex = 2;
		nReadCount = 0;
		nInvokeId = -1;
		strDescription = _T("");
		strObjectName = _T("");
		strIP = _T("");
		oleExportTime = COleDateTime::GetCurrentTime();
		bReadFinished = false;
		bExportedFinished = false;
		bNeedRead = true;
		bInore = false;
		mapPoint.clear();
		mapArrayIndex.clear();

		bReadDeviceCountSuccess = false;
		bReadDeviceInfoSuccess = false;
		bReadDevicePointPropertySuccess = false;
		bReadPointPropertySuccess = false;

		nFileSize = 0;
	}

	void clear()
	{
		nExportPortCount = 0;
		nCurrentArrayIndex = 2;
		nReadCount = 0;
		oleExportTime = COleDateTime::GetCurrentTime();
		bReadFinished = false;
		bExportedFinished = false;
		bNeedRead = true;
		bInore = false;
		bReadDeviceCountSuccess = false;
		bReadDeviceInfoSuccess = false;
		bReadDevicePointPropertySuccess = false;
		bReadPointPropertySuccess = false;
	}

	void ExportClear()
	{
		bInore = false;
		bExportedFinished = false;
		nReadCount = 0;
		nCurrentArrayIndex = 0;
	}

	int		type;						//0 根节点  1：bacnet设备  2：bacnet点  3:点属性
	DWORD	nOldDeviceID;				//原始设备号
	DWORD	nNewDeviceID;				//IP转成的设备号
	int		nPointType;					//点类型
	int		nPointCount;				//点位总数
	int		nExportPortCount;			//实际导出点数
	int		nCurrentArrayIndex;			//当前查询的Index
	int		nInvokeId;					//查询命令编号
	int		nReadCount;					//查询次数 最多三次
	DWORD	nPointAddress;				//点地址
	DWORD	nArrayIndex;				//点位顺序
	CString strDescription;				//中文注释
	CString strObjectName;				//点名
	float	fValue;						//点值
	CString strIP;						//设备IP
	COleDateTime oleExportTime;			//导出时间 超过1分钟继续
	bool	bReadFinished;				//读取完成
	bool	bExportedFinished;			//导出完成
	bool	bNeedRead;					//需要读取属性
	bool	bInore;						//忽略跳过
	int		nReadLimit;					//连读限制

	//////////////////////////////////////////////////////////////////////////
	bool	bReadDeviceCountSuccess;		//读取设备点数成功
	bool	bReadDeviceInfoSuccess;			//读取设备点结构成功
	bool	bReadDevicePointPropertySuccess;//读取设备点属性成功
	bool	bReadPointPropertySuccess;		//读取设备点属性成功
	//////////////////////////////////////////////////////////////////////////

	hash_map<wstring,BacnetStructInfo*>	mapPoint;	//点位属性
	hash_map<DWORD,wstring>				mapArrayIndex;	//ArrayIndex和Key隐射关系

	//////////////////////////////////////////////////////////////////////////
	int		nFileSize;						//文件长度
};

struct BACnet_Read_Property_Data;
typedef struct BACnet_Read_Property_Data BACNET_READ_PROPERTY_DATA;

struct BACnet_Read_Access_Data;
typedef struct BACnet_Read_Access_Data BACNET_READ_ACCESS_DATA;

struct BACnet_Application_Data_Value;
typedef struct BACnet_Application_Data_Value BACNET_APPLICATION_DATA_VALUE;


// CCommBacnetSearch dialog

class CCommBacnetSearch : public CDialog
{
	DECLARE_DYNAMIC(CCommBacnetSearch)

public:
	CCommBacnetSearch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommBacnetSearch();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void BacnetSearch();
	afx_msg void BacnetSetting();
	afx_msg void BacnetExportAll();
	afx_msg void BacnetExportAllToPointFile();
	afx_msg void BacnetExportSelectMulti();
	afx_msg void BacnetExportContinue_();
	afx_msg void BacnetExportBacnetInfo();
	afx_msg void BacnetReadAllPoint();
	afx_msg void BacnetExportSelectOne();
	afx_msg void BacnetReadPointProperty();
	afx_msg void BacnetSort();
	DECLARE_MESSAGE_MAP()


public:
	bool Exit();
	bool InitData(bool bConnected = true);

	bool CreateThreadRead();
	void OnTimerRead();
	void OnTimerWhois();
	bool GetReadDataThreadExit() const;
	bool TimeOutActive();
	//
	static UINT WINAPI     ThreadWhois(LPVOID pParam);								//不断广播设备
	static UINT WINAPI	   ThreadTimeOutActiveFunc(LPVOID lparam);					//超时事件触发响应
	static UINT WINAPI	   ThreadReadPointFunc(LPVOID lparam);						//读取设备属性
	static UINT WINAPI	   ThreadUpdateProgressFunc(LPVOID lparam);

	////////////////////////////////////////////////////////////////////////// 根据不同命令类型
	static UINT WINAPI	   ThreadSendCmdFunc(LPVOID lparam);					//根据不同命令类型

	static UINT WINAPI	   ThreadExportAllFunc(LPVOID lparam);					//根据不同命令类型
	static UINT WINAPI	   ThreadExportAllToPointFileFunc(LPVOID lparam);		//根据不同命令类型
	static UINT WINAPI	   ThreadExportSelectFunc(LPVOID lparam);				//根据不同命令类型
	static UINT WINAPI	   ThreadReadOneByOneFunc(LPVOID lparam);				//根据不同命令类型
	static UINT WINAPI	   ThreadExportOneFunc(LPVOID lparam);					//根据不同命令类型
	static UINT WINAPI	   ThreadReadOnePropertyFunc(LPVOID lparam);			//根据不同命令类型

	static bool	   ReadObjectInfo(BacnetStructInfo* pBacnetInfo,int nStart,int nEnd);
	//////////////////////////////////////////////////////////////////////////


	static	void	SetEventHandle(int nEvent);
	static HANDLE			GetEventHandle();
	int				GetEventCode();

	bool			UpdateProgress();

	//
	static void    InitServiceHandlers();
	static void    OnReceiveRead(BACNET_READ_PROPERTY_DATA* pData, BACNET_ADDRESS * src,UINT invokeId);
	static void    OnReceiveReadMultiple(BACNET_READ_ACCESS_DATA* pRpmData, BACNET_ADDRESS * src,UINT invokeId);
	static void	   MyAbortHandler(BACNET_ADDRESS * src, uint8_t invoke_id,uint8_t abort_reason, bool server);
	static void    MyErrorHandler(BACNET_ADDRESS * src, uint8_t invoke_id,BACNET_ERROR_CLASS error_class, BACNET_ERROR_CODE error_code);
	static void    MyRejectHandler(BACNET_ADDRESS * src, uint8_t invoke_id, uint8_t reject_reason);
	static void	   MyAtomicReadFileAckHandler(uint8_t * service_request,uint16_t service_len,BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);
	static void	   MyAtomic_Read_File_Error_Handler(BACNET_ADDRESS * src,uint8_t invoke_id,BACNET_ERROR_CLASS error_class,BACNET_ERROR_CODE error_code);
	static void    HandleReadPropertyAck(UINT8 * service_request,UINT16 service_len,BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);
	static void    HandleReadPropertyMultiAck(UINT8 * service_request,UINT16 service_len,BACNET_ADDRESS * src,BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);
	static void	   HandleIAM(uint8_t * service_request,uint16_t service_len,BACNET_ADDRESS * src);
	static void    HandleWritePropertyAck(BACNET_ADDRESS * src, uint8_t invoke_id);
	static void    ClearReadAccessData(BACNET_READ_ACCESS_DATA** pRpmData);
	static bool    FreeAllInvokeId();
	static void    StartNextObject(BACNET_READ_ACCESS_DATA * rpm_object,int nPointType,DWORD nPointAddress);
	static void    BuildPropRequest(BACNET_READ_ACCESS_DATA * rpm_object);
	static void    BuildPropRequest_Description(BACNET_READ_ACCESS_DATA * rpm_object);
	static void    StartNextObjectProperty(BACNET_READ_ACCESS_DATA * rpm_object,int nPointType,DWORD nPointAddress);

	//
	static bool	   FindIsExist(vector<DWORD>& vec,DWORD deviceid);
	static bool	   FindIsExist(vector<wstring>& vec,wstring key);
	static bool	   ReadObjectInfo(BacnetStructInfo* pBacnetInfo,int nOneCmdPoint = 19);
	static bool	   ReadObjectCount(BacnetStructInfo* pBacnetInfo);
	static bool	   ReadObjectProperty(BacnetStructInfo* pBacnetInfo);
	static bool	   AtomicReadFile(BacnetStructInfo* pBacnetInfo);
	static bool	   ExportPoints(BacnetStructInfo* pBacnetInfo);
	static bool	   ExportErr(BacnetStructInfo* pBacnetInfo);
	static BacnetStructInfo*   FindBacnetInfoFromDeviceIDTypeIndex(DWORD deviceid,BACNET_ADDRESS * src,int nPointType,DWORD arrayIndex,bool bFindDevice=false);
	static void    CopyValue(BACNET_APPLICATION_DATA_VALUE* pDataValue,BACNET_PROPERTY_ID proType,BACNET_OBJECT_TYPE obType,DWORD obInstance,BacnetStructInfo* pBacnetInfo);
	static bool DeleteNode(HTREEITEM hNode,bool bReleaseNode=true);

	static	wstring	UTF8ToWideChar_Del0(char* cOld, int length);
	static	wstring	AnsiToWideChar_Del0(char* cOld, int length);
	static	CString GenerateBacnetInfo(BacnetStructInfo* info);
	static	bool			AddLog(CString strLog);

	static	bool	ShowDeviceInfo(BacnetStructInfo* info,HTREEITEM	hSelectNode);
	static	bool	ShowDevicePointProperty(BacnetStructInfo* info,HTREEITEM	hSelectNode);
	static	void	SaveS3db(CString strS3dbPath,BacnetStructInfo* pBacnetInfo);

	static	void	UCS2toUTF8(UINT8 *ucs2, int count, char *utf8);
	static	void	UCS4toUTF8(UINT8 *ucs2, int count, char *utf8);
public:
	CProgressCtrl		m_pProgressSend;
	CProgressCtrl		m_pProgressRec;

	static CViewTree	m_wndFileView;
	static HTREEITEM	m_hRoot;			//根节点
	static HTREEITEM	m_hSelectNode;		//选中节点
	static	int			m_nReadMode;		//默认0为读index  1为读ip

	static CRITICAL_SECTION m_criticalSection;

	static hash_map<wstring,BacnetStructInfo*>	m_mapDeveice;		//按ID读的话，填ID  按IP读的话,需将IP转换为ID
	static	bool		m_bBusy;			//处理中 不能执行其他命令
	static	bool		m_bSearch;			//搜索完成才能导出
	static	int			m_nEvent;			//事件ID 1导出读取点数 2导出读取设备点结构信息 3导出读取设备点属性信息 4导出点表 5导出设备点结构信息 6显示读取点数 7显示读取设备点结构信息 8显示设备点属性信息 
	static	int			m_nCharacterType;	//0 Ansi  1:Utf-8
	static CString		m_strSavepath;		//存储路劲
	static vector<DWORD>	m_vecDeviceID;

	HANDLE              m_readdata_thread;		//接收线程
	HANDLE				m_read_thread;			//发送线程
	HANDLE				m_progress_thread;		//进度线程
	HANDLE				m_timeout_thread;		//超时激活线程
	static HANDLE		m_event;

	bool                m_exit_readdata_thread;
	static	DWORD		m_dwCurrentOldDeviceID;		//当前查询设备ID
	static	COleDateTime m_oleSendTime;				//发送和接收时候更新下
	static	COleDateTime m_oleRecTime;			
	static  int			m_nTimeOut;					//超时
	static	int			m_nBacnetScanInterval;
	static	int			m_nRetryReadCount;			//重试次数
	static  wstring     m_strSelectKey;				//当前选中Key
	static	BacnetStructInfo* m_pCurrentBacnet;		//当前操作的Bacnet

	static CEdit			m_EditLog;
	static CString			m_strLOG;
	static int				m_nReadDeviceCount;		//已读取多少个设备（导出时候）
	static	int				m_nDeviceCount;
	static  bool			m_bUpdateProgress;		//更新进度
	static	bool			m_bStoreS3db;
	static vector<wstring>	m_vecDeviceIPID;
	static	bool			m_bExistS3dbModel;

	//////////////////////////////////////////////////////////////////////////
	static  int			m_nResponseTimeOut;					//超时
	static  int			m_nCmdInterval;						//命令间隔
	static  int			m_nMutilReadCount;					//批量读取个数 19
	//////////////////////////////////////////////////////////////////////////
	static	bool		m_bEndEndFile;
	static BacnetStructInfo* m_pBacnetFile;					//查询文件
};
