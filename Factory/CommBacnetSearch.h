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

	int		type;						//0 ���ڵ�  1��bacnet�豸  2��bacnet��  3:������
	DWORD	nOldDeviceID;				//ԭʼ�豸��
	DWORD	nNewDeviceID;				//IPת�ɵ��豸��
	int		nPointType;					//������
	int		nPointCount;				//��λ����
	int		nExportPortCount;			//ʵ�ʵ�������
	int		nCurrentArrayIndex;			//��ǰ��ѯ��Index
	int		nInvokeId;					//��ѯ������
	int		nReadCount;					//��ѯ���� �������
	DWORD	nPointAddress;				//���ַ
	DWORD	nArrayIndex;				//��λ˳��
	CString strDescription;				//����ע��
	CString strObjectName;				//����
	float	fValue;						//��ֵ
	CString strIP;						//�豸IP
	COleDateTime oleExportTime;			//����ʱ�� ����1���Ӽ���
	bool	bReadFinished;				//��ȡ���
	bool	bExportedFinished;			//�������
	bool	bNeedRead;					//��Ҫ��ȡ����
	bool	bInore;						//��������
	int		nReadLimit;					//��������

	//////////////////////////////////////////////////////////////////////////
	bool	bReadDeviceCountSuccess;		//��ȡ�豸�����ɹ�
	bool	bReadDeviceInfoSuccess;			//��ȡ�豸��ṹ�ɹ�
	bool	bReadDevicePointPropertySuccess;//��ȡ�豸�����Գɹ�
	bool	bReadPointPropertySuccess;		//��ȡ�豸�����Գɹ�
	//////////////////////////////////////////////////////////////////////////

	hash_map<wstring,BacnetStructInfo*>	mapPoint;	//��λ����
	hash_map<DWORD,wstring>				mapArrayIndex;	//ArrayIndex��Key�����ϵ

	//////////////////////////////////////////////////////////////////////////
	int		nFileSize;						//�ļ�����
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
	static UINT WINAPI     ThreadWhois(LPVOID pParam);								//���Ϲ㲥�豸
	static UINT WINAPI	   ThreadTimeOutActiveFunc(LPVOID lparam);					//��ʱ�¼�������Ӧ
	static UINT WINAPI	   ThreadReadPointFunc(LPVOID lparam);						//��ȡ�豸����
	static UINT WINAPI	   ThreadUpdateProgressFunc(LPVOID lparam);

	////////////////////////////////////////////////////////////////////////// ���ݲ�ͬ��������
	static UINT WINAPI	   ThreadSendCmdFunc(LPVOID lparam);					//���ݲ�ͬ��������

	static UINT WINAPI	   ThreadExportAllFunc(LPVOID lparam);					//���ݲ�ͬ��������
	static UINT WINAPI	   ThreadExportAllToPointFileFunc(LPVOID lparam);		//���ݲ�ͬ��������
	static UINT WINAPI	   ThreadExportSelectFunc(LPVOID lparam);				//���ݲ�ͬ��������
	static UINT WINAPI	   ThreadReadOneByOneFunc(LPVOID lparam);				//���ݲ�ͬ��������
	static UINT WINAPI	   ThreadExportOneFunc(LPVOID lparam);					//���ݲ�ͬ��������
	static UINT WINAPI	   ThreadReadOnePropertyFunc(LPVOID lparam);			//���ݲ�ͬ��������

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
	static HTREEITEM	m_hRoot;			//���ڵ�
	static HTREEITEM	m_hSelectNode;		//ѡ�нڵ�
	static	int			m_nReadMode;		//Ĭ��0Ϊ��index  1Ϊ��ip

	static CRITICAL_SECTION m_criticalSection;

	static hash_map<wstring,BacnetStructInfo*>	m_mapDeveice;		//��ID���Ļ�����ID  ��IP���Ļ�,�轫IPת��ΪID
	static	bool		m_bBusy;			//������ ����ִ����������
	static	bool		m_bSearch;			//������ɲ��ܵ���
	static	int			m_nEvent;			//�¼�ID 1������ȡ���� 2������ȡ�豸��ṹ��Ϣ 3������ȡ�豸��������Ϣ 4������� 5�����豸��ṹ��Ϣ 6��ʾ��ȡ���� 7��ʾ��ȡ�豸��ṹ��Ϣ 8��ʾ�豸��������Ϣ 
	static	int			m_nCharacterType;	//0 Ansi  1:Utf-8
	static CString		m_strSavepath;		//�洢·��
	static vector<DWORD>	m_vecDeviceID;

	HANDLE              m_readdata_thread;		//�����߳�
	HANDLE				m_read_thread;			//�����߳�
	HANDLE				m_progress_thread;		//�����߳�
	HANDLE				m_timeout_thread;		//��ʱ�����߳�
	static HANDLE		m_event;

	bool                m_exit_readdata_thread;
	static	DWORD		m_dwCurrentOldDeviceID;		//��ǰ��ѯ�豸ID
	static	COleDateTime m_oleSendTime;				//���ͺͽ���ʱ�������
	static	COleDateTime m_oleRecTime;			
	static  int			m_nTimeOut;					//��ʱ
	static	int			m_nBacnetScanInterval;
	static	int			m_nRetryReadCount;			//���Դ���
	static  wstring     m_strSelectKey;				//��ǰѡ��Key
	static	BacnetStructInfo* m_pCurrentBacnet;		//��ǰ������Bacnet

	static CEdit			m_EditLog;
	static CString			m_strLOG;
	static int				m_nReadDeviceCount;		//�Ѷ�ȡ���ٸ��豸������ʱ��
	static	int				m_nDeviceCount;
	static  bool			m_bUpdateProgress;		//���½���
	static	bool			m_bStoreS3db;
	static vector<wstring>	m_vecDeviceIPID;
	static	bool			m_bExistS3dbModel;

	//////////////////////////////////////////////////////////////////////////
	static  int			m_nResponseTimeOut;					//��ʱ
	static  int			m_nCmdInterval;						//������
	static  int			m_nMutilReadCount;					//������ȡ���� 19
	//////////////////////////////////////////////////////////////////////////
	static	bool		m_bEndEndFile;
	static BacnetStructInfo* m_pBacnetFile;					//��ѯ�ļ�
};
