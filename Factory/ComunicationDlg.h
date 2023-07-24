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
#include "afxcmn.h"
#include "DataPoint/DataPointEntry.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif
// CComunicationDlg dialog
struct BacnetInfo
{
	BacnetInfo()
	{
		type = 0;
		nOldDeviceID = 0;
		nNewDeviceID = 0;
		nPointType = 0;
		nPointCount = 0;
		nPointAddress = 0;
		nArrayIndex = 0;
		fValue = 0;
	}
	int type;				//0 ���ڵ�  1��bacnet�豸  2��bacnet��  3:������
	DWORD nOldDeviceID;
	DWORD nNewDeviceID;
	int nPointType;
	int	nPointCount;				//��λ����
	DWORD nPointAddress;
	DWORD nArrayIndex;				//��λ˳��
	CString strDescription;			//����ע��
	CString strObjectName;			//����
	float	fValue;					//��ֵ
	CString strIP;
};

struct BACnet_Read_Property_Data;
typedef struct BACnet_Read_Property_Data BACNET_READ_PROPERTY_DATA;

struct BACnet_Read_Access_Data;
typedef struct BACnet_Read_Access_Data BACNET_READ_ACCESS_DATA;

struct BACnet_Application_Data_Value;
typedef struct BACnet_Application_Data_Value BACNET_APPLICATION_DATA_VALUE;

class COPCConnection_AutoBrowse;
class CS7UDPCtrl;
class CComunicationDlg : public CDialog
{
	DECLARE_DYNAMIC(CComunicationDlg)

public:
	CComunicationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CComunicationDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_COM_SEARCH };

public:
	bool Exit();
	bool InitData(bool bConnected = true);
	bool EnableControl(bool bConnected = true);
	static void    InitServiceHandlers();
	bool            CreateThreadRead();
	static UINT WINAPI      ThreadRead(LPVOID pParam);
	static UINT WINAPI		ThreadReadAllDeviceFunc(LPVOID lparam);
	static UINT WINAPI		ThreadUpdateProgressFunc(LPVOID lparam);
	static UINT WINAPI		ThreadExportAllPointFunc(LPVOID lparam);
	static UINT WINAPI		ThreadExportSelectPointFunc(LPVOID lparam);
	static	void	SetCmdEvent();
	HANDLE			GetSendCmdEvent();

	static	void	SetExportEvent();
	HANDLE			GetExportEvent();

	bool			ExportOPCItems();
	bool			SendCmd();
	bool			ReadOnePointProperty(int nPointType,DWORD nPointAddress);
	bool			DeleteNode(HTREEITEM hNode,bool bReleaseNode=true);
	bool			UpdateProgress();
	bool			ExportOneBacnetPoint(BacnetInfo* info);
	bool			ExportFinished();
	static bool			FindIsExist(vector<DWORD>& vec,DWORD deviceid);
	static bool			UCS2_to_UTF8(UINT16* ucs2_code, UINT8* utf8_code);


	bool			AddLog(CString strLog);
	void            OnTimerRead();
	void            OnTimerWhois();
	static void            OnReceiveRead(BACNET_READ_PROPERTY_DATA* pData, UINT invokeId);
	static void            OnReceiveReadMultiple(BACNET_READ_ACCESS_DATA* pRpmData, UINT invokeId);

	static void		MyAbortHandler(BACNET_ADDRESS * src, uint8_t invoke_id,
		uint8_t abort_reason, bool server);
	static void     MyErrorHandler(BACNET_ADDRESS * src, uint8_t invoke_id,
		BACNET_ERROR_CLASS error_class, BACNET_ERROR_CODE error_code);
	static void     MyRejectHandler(BACNET_ADDRESS * src, uint8_t invoke_id, uint8_t reject_reason);
	static void     HandleReadPropertyAck(UINT8 * service_request,
		UINT16 service_len,
		BACNET_ADDRESS * src,
		BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);
	static void     HandleReadPropertyMultiAck(UINT8 * service_request,
		UINT16 service_len,
		BACNET_ADDRESS * src,
		BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);
	static void HandleIAM(
		uint8_t * service_request,
		uint16_t service_len,
		BACNET_ADDRESS * src);
	static void            HandleWritePropertyAck(BACNET_ADDRESS * src, uint8_t invoke_id);
	static void        ClearReadAccessData(BACNET_READ_ACCESS_DATA** pRpmData);
	static	bool	FreeAllInvokeId();
	static void CopyValue(BACNET_APPLICATION_DATA_VALUE* pDataValue,BACNET_PROPERTY_ID proType,BACNET_OBJECT_TYPE obType,DWORD obInstance,DWORD arrayIndex);
	static void StartNextObject(BACNET_READ_ACCESS_DATA * rpm_object,int nPointType,DWORD nPointAddress);

	static void BuildPropRequest(BACNET_READ_ACCESS_DATA * rpm_object);
	bool	SearchObject();

	bool	GetReadDataThreadExit() const;

	static	void	ReadObjectInfoByCount(DWORD nOldDeviceID,DWORD nNewDeviceID,int nCount,int nOneCmdPoint = 19);
	static	void	ReadObjectInfo(DWORD nOldDeviceID,DWORD nNewDeviceID,int nStart,int nEnd);

	static	void	ReadObjectPropertyByCount(DWORD nOldDeviceID,DWORD nNewDeviceID,int nCount,int& nLastInvokeId,int nOneCmdPoint = 19);
	static	void	ReadObjectProperty(DWORD nOldDeviceID,DWORD nNewDeviceID,int nStart,int nEnd,int& nLastInvokeId);

	static	void	ExportObjectInfoByCount(DWORD nOldDeviceID,DWORD nNewDeviceID,int nCount,int& nLastInvokeId,int nOneCmdPoint = 19);
	static	void	ExportObjectInfo(DWORD nOldDeviceID,DWORD nNewDeviceID,int nStart,int nEnd,int& nLastInvokeId);
	static	void	ExportAllPointDescription();
	static	void	ExportPoint();
	static	void	ExportPoint_S3DB();
	static	void	ExportPoint_Excel();
	static	void	SaveS3db(CString strS3dbPath);

	static	bool	ReadOnePointProperty(int nOldDeviceID,int nNewDeviceID,int nPointType,DWORD nPointAddress,int& nLastInvokeId);
	static	CString GenerateBacnetInfo(BacnetInfo info);
	static	bool	ActiveAndContinue();
	static	bool	OutWarningErrLog();

	static	wstring	UTF8ToWideChar_Del0(char* cOld, int length);
	static	wstring	AnsiToWideChar_Del0(char* cOld, int length);
	static	void	SetBacnetDeviceCount(int nOldDeviceID,int nNewDeviceID, int nCount);
	static	bool	GetBacnetDeviceCount(int nOldDeviceID,int nNewDeviceID, int& nCount);		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void SearchBacnet();
	afx_msg void BacnetSetting();
	afx_msg void BacnetExportAll();
	afx_msg void BacnetExportAllToPointFile();
	afx_msg void BacnetExportSelect();
	afx_msg void BacnetExportContinue();
	afx_msg void BacnetExportBacnet();
	afx_msg void ReadAllBacnetPoint();
	afx_msg void ExportAllBacnetPoint();
	afx_msg void ReadPointProperty();
	afx_msg void BacnetSort();
	DECLARE_MESSAGE_MAP()

public:
	static LRESULT OnRecData(const char* buffer,DWORD userdata);
	void	GetValueSet(vector< pair<wstring, wstring> >& valuelist );

public:
	static CViewTree	m_wndFileView;
	static HTREEITEM	m_hRoot;
	static HTREEITEM	m_hSelectNode;
	static	int			m_nReadMode;		//Ĭ��0Ϊ��index  1Ϊ��ip
	static	int			m_nCmdState;		//0��ͳ��Object����		1������  2-Max�Ƕ�Object 3:������   4:��ͳ�Ƶ�������
	static	int			m_nPointCount;		//�����
	static	int			m_nCmdType;			//0����������  1��������  2:����ע��  3������ȡ���  4:������Զ�ȡ���
	static	int			m_nExportMode;		//0:��������  1��ȫ������
	static  int			m_nLastReadObjectInvokeId;
	static	int			m_nLastReadPropertyInvokeId;
	static	int			m_nBacnetScanInterval;
	static	int			m_nCharacterType;		//0 Ansi  1:Utf-8
	static	DWORD		m_nSelectOldDeviceID;	//��ǰѡ���豸ID
	static	DWORD		m_nSelectNewDeviceID;	//��ǰѡ���豸ID
	HANDLE              m_readdata_thread;		//�����߳�
	HANDLE				m_read_thread;			//�����߳�
	HANDLE				m_progress_thread;		//�����߳�
	static HANDLE		m_sendcmdevent;
	static HANDLE		m_exportcmdevent;
	bool                m_exit_readdata_thread;
	static CRITICAL_SECTION m_criticalSection;
	static hash_map<wstring,BacnetInfo>	m_mapAllPoint;
	static hash_map<wstring,BacnetInfo*>	m_mapDeveice;
	static CString				m_strSavepath;			//�洢·��
	static vector<DWORD>	m_vecDeviceID;
	int m_nScanInterval;
	CProgressCtrl		m_pProgressSend;
	CProgressCtrl		m_pProgressRec;
	static	int			m_nProgressSend;
	static	int			m_nProgressRec;
	static	int			m_nProgressType;		//0�Ƕ�ȡȫ���豸��Ϣ  1�Ƕ�ȡȫ���豸���� 2�ǵ���ȫ������ 3�ǵ���ȫ���豸��Ϣ  4�ǵ���ȫ���豸����
	static	int			m_nCurrentDevice;
	static	int			m_nDeviceCount;
	static	int			m_nCurrendDeviceID;
	static	int			m_nDeviceStart;
	static	int			m_nDeviceEnd;
	static	bool		m_bStoreS3db;
	static	COleDateTime m_oleLastReceiveResponse;
public:
	CIPAddressCtrl			m_IPLCIP;
	CString					m_strPLCIP;
	int						m_nSlack;
	int						m_nSlot;
	CString					m_strPlcPointAddress;
	double					m_dValue;
	CS7UDPCtrl*				m_pS7UDPCtrl;
	bool					m_bConnectOK;
	CEdit					m_EditLog;
	CString					m_strLOG;
	DWORD					m_OPCIP;
	CIPAddressCtrl			m_IOPCIP;
	CComboBox				m_comOPCType;
	CString					m_strOPCType;
	CString					m_strOPCIP;
	CString					m_strOPCPointName;
	CComboBox				m_comOPCPointType;
	CString					m_strOPCValue;

	COPCConnection_AutoBrowse* m_pOPC;
	hash_map<wstring,CLSID> m_clsidmap;
	bool					m_bConnectOPCOK;
	bool					m_bAddOPCItemSuccess;
	bool					m_bProperty;

};
