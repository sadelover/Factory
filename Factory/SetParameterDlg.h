#pragma once

#include "EditListCtrl.h"

#include "DB_BasicIO/DatabaseSession.h"
#include "DB_BasicIO/RealTimeDataAccess.h"
#include "DataPoint/DataPointManager.h"
#include "DataPoint/LogicPointManager.h"
#include "DataPoint/AlarmPointManager.h"
#include "../ComponentDraw/sqlite/SqliteAcess.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "afxcmn.h"
#include "afxwin.h"

class CUPPCServerDataAccess;
class CDataPointManager;
class CBEOPLogicManager;

#define POINT_LISTCTRL_ID (0x1009)
// CPointView ��ͼ
typedef std::vector<std::wstring> vec_wstr;
class SetParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(SetParameterDlg)
	DECLARE_MESSAGE_MAP()
public:
	SetParameterDlg(CBEOPLogicManager *pLogicManager, CUPPCServerDataAccess *pDataAccess, CWnd* pParent = NULL);   // standard constructor
	virtual ~SetParameterDlg();
	// Dialog Data
	enum { IDD = IDD_DIALOG_SetDLLParameter};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
private:
	BOOL SetClipboardCustomValue(const CString strVal);
	CString	GetListCtrlSelectedValue(void);

	int m_nIndex;
	HANDLE                 m_hThreadPointAndMode;
	HANDLE                 m_hEventClose_Point;
public:
	CUPPCServerDataAccess*	m_pDataAccess;
	CDataPointManager*		m_pointmanager;
	wstring					m_wstrFilePath;
public:
	std::vector<wstring> m_strParamPointNameList;
	std::vector<wstring> m_strWatchPointLineList;
	bool IsPushback(std::vector<wstring> &PointList,wstring PointName);
	void SetInputListCurrentValue(const wstring wstrName, const float fVal);
public:
	CStatic m_StaticPointCount;
	CBEOPLogicManager *m_pLogicManager;
public:
	BOOL InsertWatchPointToDB();
	BOOL ReadWatchPointFromDB();
	CString m_strCurSelDll;
	CListCtrl m_InputParaList;
	CListCtrl m_OutputParaList;
	std::vector<vec_wstr> m_vecCurDllInputParameterList;
	std::vector<vec_wstr> m_vecCurDllOutputParameterList;

	afx_msg void OnBnClickedOk();
	/*
	 * brief:��������б�˫���¼�
	*/
	afx_msg void OnNMDblclkListSelectparameterinput(NMHDR *pNMHDR, LRESULT *pResult);
	/*
	 * brief:��������б�˫���¼�
	*/
	afx_msg void OnNMDblclkListSelectparameteroutput(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_strIntroduction;
	afx_msg void OnLvnItemchangedListSelectparameterinput(NMHDR *pNMHDR, LRESULT *pResult);

	/** ������Եĵ�һ����������޸ģ�
	 strSetType:0 �������룬1: ���������2�����õ���ʱ�䣬ʱ�����variablename��
	����������������������붨�塢���붨������ */
	void SaveParameterToDBAndServer(CString strDllName, CString strSetType, CString strVariableName, CString strLinkDefine, CString strLinkType, CString strCondition);


	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonModifyThread();
	CComboBox m_comboThread;
	CString m_strThreadName;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonReplaceStr();
	CString m_strDllDeclare;//���Ա�ע
	CString m_strDllOriginName;//dll��ʶ��
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	
	//�߳��������������ǰֵ��������ط�������
	/*
	 * brief��ÿ3��Ϊ����б�ĵ�ǰֵ����ֵ
	*/
	static UINT Thread_OutputPointValue_RealTime( LPVOID lpParameter );
	typedef enum {STOP,RUN/*,END*/}kThreadState;
	kThreadState m_nthreadState;
	CString m_strCalcIp;
	int m_nCalcPort;
	CWinThread* m_pThread;
private:
	CString m_strOriginalThreadName;
};
