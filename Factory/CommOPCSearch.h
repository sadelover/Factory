#pragma once
#include "ViewTree.h"
#include <hash_map>
#include "afxcmn.h"
#include "DataPoint/DataPointEntry.h"
#include "Tools/CustomTools/CustomTools.h"
#include "afxwin.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif

class COPCConnection_AutoBrowse;
class CItem;
struct OPCInfo
{
	OPCInfo()
	{
		type = 0;
		nPointCount = 0;		
		pOPC = NULL;
		bConnected = false;
		bLoadPointed = false;
		bShow = false;
	}
	int type;					//0 ���ڵ�  1��opc server  2��opc��  3:������
	int	nPointCount;			//��λ����
	bool	bConnected;			//opc connect
	bool	bLoadPointed;		//load point
	bool	bShow;				//point����ʾ
	CString strOPCServerIP;		//opc IP
	CString strOPCServerName;	//opc ��������
	CString strOPCPointType;	//opc ������
	CString strOPCPointName;	//opc ����
	CString strOPCPointRemark;	//opc ע��
	CString strOPCPointValue;	//opc ��ֵ
	COPCConnection_AutoBrowse* pOPC;
};

// CCommOPCSearch dialog

class CCommOPCSearch : public CDialog
{
	DECLARE_DYNAMIC(CCommOPCSearch)

public:
	CCommOPCSearch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommOPCSearch();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void SearchOPC();
	afx_msg void OPCSetting();
	afx_msg void OPCConnect();
	afx_msg void OPCAddNewPoint();
	afx_msg void OPCExportOPC();
	afx_msg void ReadAllOPCPoint();
	afx_msg void ReadPointProperty();
	afx_msg void DisConnOPC();
	afx_msg void FindOPCPoint();
	afx_msg void ShowOPCPoint();
	DECLARE_MESSAGE_MAP()

public:
	bool Exit();
	bool InitData(bool bConnected = true);
	bool EnableControl(bool bConnected = true);
	
	bool DeleteNode(HTREEITEM hNode,bool bReleaseNode=true);
	bool RefreshOPCServer(hash_map<wstring,CLSID> clsidma);
	bool AddLog(CString strLog);

	bool			UpdateProgress(DWORD dwCurrent, DWORD dwTotal,char* pItemName);

	static LRESULT OnAddPoint(DWORD dwCurrent, DWORD dwTotal,DWORD userdata,char* pItemName);
public:
	static CViewTree					m_wndFileView;
	static HTREEITEM					m_hRoot;
	static HTREEITEM					m_hSelectNode;
	static CRITICAL_SECTION				m_criticalSection;

	CString								m_strOPCIP;
	CString								m_strOPCFiler;
	CString								m_strDataTypeFilter;	//��ʾ����
	int									m_nMutilAddItem;		//�����������
	int									m_nBrowseType;
	hash_map<wstring,CLSID>				m_clsidmap;
	static hash_map<wstring,OPCInfo*>	m_mapDeveice;
	static hash_map<wstring,hash_map<wstring,HTREEITEM>>	m_mapAllPoint;

	CEdit								m_EditLog;
	CString								m_strLOG;
	bool								m_bLoading;
	COPCConnection_AutoBrowse*			m_pLoadingOPC;
	int									m_nMutilLoad;
	int									m_nLoadInterval;
	CProgressCtrl						m_pLoadOPCPoint;
	CStatic								m_sLoadOPC;
	bool								m_bReadDescription;

	DWORD								m_nCurrentCount;
	DWORD								m_nTotalCount;
	HANDLE								m_proHandler;		//�����߳�

	VARTYPE								m_vtDataTypeFilter;		//��ʾ����

	Project::Tools::Mutex				m_lockProgress;

};
