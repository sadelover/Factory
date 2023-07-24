#pragma once

#include "../ControlEx/SkinDlg.h"

#include "afxwin.h"
#include "DataPoint/DataPointEntry.h"

class CDataPointManager;

// CPointParamEditDlg 对话框
enum DLG_TYPE
{
	enum_Add = 0,
	enum_Edit,
};

//typedef enum tagBindSetPointType
//{
//	bindPoint_normal	= 0,
//	bindPoint_modbus,
//	bindPoint_vpoint,
//	bindPoint_noShow
//}BindSetPointType;


class CPointParamEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CPointParamEditDlg)

public:
	CPointParamEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPointParamEditDlg();
	void SetDlgType(DLG_TYPE eType);
	void SetDlgTitle();
// 对话框数据
	enum { IDD = IDD_DIALOG_POINT_PARAM_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog();
public:
	CString m_strPhysicalID;
	CString m_strRemark;
	CString m_strParam1;
	CString m_strParam2;
	CString m_strParam3;
	CString m_strParam4;
	CString m_strParam5;
	CString m_strParam6;
	CString m_strParam7;
	CString m_strParam8;
	CString m_strParam9;
	CString m_strParam10;
	CString m_strParam11;
	CString m_strParam12;
	CString m_strParam13;
	CString m_strParam14;
	CString m_strParam15;
	CString m_strParam16;
	CString m_strParam17;
	CString m_strParam18;
	CString m_strSource;
	CString m_strUnit;
	CString m_strProperty;
	DLG_TYPE m_eType;
	POINT_STORE_CYCLE m_store_cycle;
	CString m_strPhysicalIdOrig;
	bool CheckIpAddr(const CString strIp);
private:
	void InitMyControls();
	void SetSource(CString strSource);
	void SetUnit(CString strUnit);
	void SetProperty(CString strProperty);
	void SetEditPhysicalID();
	int GetMaxKeyID();
	void InitDateTimeControl();
	void InitStoreCycle();
	//void SetBtnBindSetPointPos(const BindSetPointType& bindType);	// wait for delete
	void GetEditRect(void);
	CRect m_rtEdit2;
	CRect m_rtEdit3;
	CRect m_rtEdit5;
	CRect m_rtEdit6;
	CRect m_rtEdit7;
	CRect m_rtStoreCycle;
	CDataPointManager* m_pPointMap;
	void InitModbusTypeSelect();
public:
	afx_msg void OnBnClickedOk();
	bool CheckPointFormatValid(DataPointEntry entry);
	CStatic m_StaticParam1;
	CStatic m_StaticParam2;
	CStatic m_StaticParam3;
	CStatic m_StaticParam4;
	CStatic m_StaticParam5;
	CStatic m_StaticParam6;
	CStatic m_StaticParam7;
	CStatic m_StaticParam8;
	CStatic m_StaticParam9;
	CStatic m_StaticParam10;
	CStatic m_StaticParam12;
	CStatic m_StaticParam13;
	afx_msg void OnCbnSelchangeComboDlgSource();
	void SetDefaultParamText();
	CComboBox m_comboPointType;
	CEdit m_EditParam1;
	CEdit m_EditParam2;
	CEdit m_EditParam3;
	CEdit m_EditParam4;
	CEdit m_EditParam5;
	CEdit m_EditParam6;
	CEdit m_EditParam7;
	CEdit m_EditParam8;
	CEdit m_EditParam9;
	CEdit m_EditParam10;
	CEdit m_EditParam11;
	CEdit m_EditParam12;
	CEdit m_EditParam13;
	CEdit m_EditParam14;

	CComboBox m_combo_datetime_type;
	CString m_strDateTimeType;
	void EnableNonDateTime(const int nShow);
	void EnableDateTimeType(const int nShow);
	CComboBox m_combo_store_cycle;
	CString m_str_store_cycle;
	afx_msg void OnCbnSelchangeComboStoreCycle();
	void SetPointMap(CDataPointManager* pPointMap);
	afx_msg void OnBnClickedBtnBindSetMax();
	afx_msg void OnBnClickedBtnBindSetMin();
	CButton m_btnBindSetPointMax;
	CButton m_btnBindSetPointMin;
	BOOL m_bIsVisited;
	CComboBox m_comboSystem;
	CComboBox m_comboDevice;
	CComboBox m_comboType;
	//试算按钮
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEditDlgParam1();
	afx_msg void OnEnChangeEditDlgParam12();
	afx_msg void OnBnClickedButtonApi();
	// modbus模式类型选择
	CComboBox m_cModbusTypeSelect;
	afx_msg void OnCbnSelchangemodbustypeselect();
	afx_msg void OnBnClickedButtonHistoricalDataCalculation();
};
