#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "AddNewRow.h"
#include "DataPoint/DataPointEntry.h"
#include "DataPoint/DataPointManager.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/TextStatic.h"
#include "../ControlEx/BitComboBox.h"
#include "../ControlEx/CommonButton.h"

#define MAX_HEADER_NUM 16
#define MAX_CHILLER_DEFAULT_POINT_NUM 75
#define MAX_TOWER_DEFAULT_POINT_NUM 21
#define MAX_VALVE_DEFAULT_POINT_NUM 10
#define MAX_PUMP_DEFAULT_POINT_NUM 21

struct DATA 
{
	int subitem;
	CWnd* pWnd;
};


class CDataPointConfigDlg : public CSkinDlg
{
	enum SHOWTYPE
	{
		SHOW_ALL,
		SHOW_CHILLER,
		SHOW_TOWER,
		SHOW_PUMP,
		SHOW_PRIPUMP,
		SHOW_SECPUMP,
		SHOW_PRIPUMP_H,
		SHOW_SECPUMP_H,
		SHOW_VALVE,
		SHOW_OPC,
		SHOW_MODBUS,
		SHOW_BACNET,
		SHOW_CUSTOM,
		SHOW_FIND,
	};
	DECLARE_DYNAMIC(CDataPointConfigDlg)
public:
	// standard constructor
	CDataPointConfigDlg(CDataPointManager& plcmap, CWnd* pParent = NULL); 
	virtual ~CDataPointConfigDlg();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* msg);
	void SetPointMap(CDataPointManager& pointMap);
	void ClearListAll();
	void MakeAllList();
	BOOL ShowWindow(int nCmdShow);
	// Dialog Data
	enum { IDD = IDD_DATA_POINT_CONFIG };
	void OnBnClickedClose();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedButtonSearch();
	DECLARE_MESSAGE_MAP()

public:
	CDataPointManager	m_plcpointmap;
	CListCtrl m_listctrl;			// the list ctrl.
	vector<DataPointEntry>	m_allentrylist;

	vector<DataPointEntry>  m_searchlist;

	bool					m_bNeedSave;
	CString m_strModifyLog;
public:
	void	ParsePoints();
	void	InitList();
	void    RefreshIgnorePointList();
	void	ShowList(const vector<DataPointEntry>& entrylist);
	//void    MakeOPCList();
	//void    MakeModbusList();
	//void    MakeBacnetList();
	//void    MakeCustomList();
	/*
	 * brief: 按照参数，配置界面现实的点名
	*/
	void    MakeSearchList(CString strSearch);
	void	AddOneEntry(DataPointEntry entry);
	CString	m_strHeaderArray[MAX_HEADER_NUM];
	const CString GetStoreCycle(const POINT_STORE_CYCLE cycle);
	const POINT_STORE_CYCLE GetStoreCycle(const CString str);
public:
	afx_msg void OnBnClickedButtonAddpt();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonAddChiller();
	//afx_msg void OnBnClickedButtonAddTower();
	//afx_msg void OnBnClickedButtonAddPump();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void AddDeviceTogether(int nDeviceNum,int nDeviceStartNum,CString nRoomNameDevice,CString strFilePath);
private:
	void InitTypeCombo();

	void ResetIndex();
	bool DeleteOneEntry(CString strPhysicalID);
	bool DeleteOneNameMap(const CString strName);
	bool DeleteOneOfPointList(const CString strPtName);
	void MakeHeaderArray();
	int  GetMaxChillerID();
	int  GetMaxTowerID();
	int  GetMaxValveID();
	int  GetMaxPumpID();
	int  GetMaxPriPumpID();
	int  GetMaxSecPumpID();
	int  GetMaxPriPumpHID();
	int  GetMaxSecPumpHID();
	bool LoadCSVPointFile(const CString& strFilePath);//从csv文件读取点表
	bool LoadExcelPointFile(const CString& strFilePath);//从Excel文件读取点表
	bool LoadXUPPointFile(const CString& strFilePath);//从xup文件读取点表
	void ExportToCSV();
	void IsPointMapValid(CDataPointManager& plcpointmap, vector<CString>& vecName);
	int GetMaxKeyID();
	bool CheckPointTable(const bool bIsCalInCheck);
	void CheckPointByType();
	bool CheckPoint(const vector<DataPointEntry>& entrylist);
	bool PointTypeValid(wstring wstrType);
	void Iinit_ComboBox_POINT_TYPE_SORT(void);
	CMenu			m_menu;

	vector<wstring> m_wstrDelPointList;

	// 点类型分类
	CComboBox m_cbb_pointTypeSort;
public:
	afx_msg void OnDblclkListctrlPoint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListctrlPoint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddvpt();
	afx_msg void OnNMRClickListctrlPoint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetStoreCycle();
	afx_msg void OnMenuSetVpoint();
	afx_msg void OnBnClickedButtonPointSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnChkPoint();
	afx_msg void AddAndDisplayOneItemRow(int nIndex,CString strIndex,int nCount,CString strFinalName,CString strFinalExpression);
	CButton m_btnImportPtTable;
	CButton m_btnExportPtTable;
	CButton m_btnDeletePt;
	CButton m_btnCheckPtTable;
	CCommonButton m_btnSavePtTable;
	CCommonButton m_btnCancel;
	afx_msg void OnBnClickedButtonShowall();
	afx_msg void OnBnClickedBtnAutogenPointname();
	afx_msg void OnBnClickedButtonDownloadTemplate();
	afx_msg void OnBnClickedButtonPointIgnore();
	CButton m_Btn_Ignore_Point_List;
	afx_msg void OnBnClickedButtonPointCopy();
	afx_msg void OnCbnSelendokComboPointTypeSort();
	afx_msg void OnBnClickedButtonMakeUpHistoryData();
};