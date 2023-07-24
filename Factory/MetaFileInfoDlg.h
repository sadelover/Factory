#pragma once

#include "../ControlEx/SkinStatic.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "afxwin.h"
#include "afxcmn.h"

// CMetaFileInfoDlg 对话框

class CDataPointManager;
class CEqmDrawDevice;
class CCanvasDlg;

class CMetaFileInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CMetaFileInfoDlg)

public:
	CMetaFileInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMetaFileInfoDlg();
// 对话框数据
	enum { IDD = IDD_DIALOG_METAFILE };

protected:
	virtual void								DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void										ReCalListCtrl();
	void										InitCombo();
	void										InitDiagnoseList();
	void										DefaultDeviceInit();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL								OnInitDialog();
	void										SetName(const CString strName);
	void										SetPointMap( CDataPointManager* pointmap );
	afx_msg void								OnBnClickedButtonPointSelect();
	afx_msg void								OnBnClickedButtonAdd();
	afx_msg void								OnBnClickedButtondelete();
	afx_msg void								OnBnClickedOk();
	afx_msg void								OnBnClickedButtonPointSelectSetting();
	afx_msg void								OnNMDblclkListDefine(NMHDR *pNMHDR, LRESULT *pResult);
	void										InitComboPage();
	CComboBox									m_combo_layer;
	CComboBox									m_combo_page;
	CComboBox									m_combo_device_type;
	CComboBox									m_combo_mouse_event_type;
	CEdit										m_nDeviceID;
	CEdit										m_edit_setting_point;
	CListCtrl									m_listCtrl;	//条件列表
	int											m_nDeviceType;
	int											m_nMouseEventType;
	int											m_nValue;
	int											m_nDeviceIDNum;
	int											m_nLayer;
	int											m_nInterval;
	CString										m_strPointName;
	CString										m_strSettingPointName;
	CEdit										m_edit_name;	//图元名称
	CString										m_strName;
	CDataPointManager*							m_refpointmap;
	CEdit										m_edit_point_name;
	CString                                     m_strShowName;
	
	afx_msg void OnBnClickedButtonPointClear();
	afx_msg void OnCbnSelchangeComboMouseEvent();
	afx_msg void OnBnClickedButtonVpoint();
	afx_msg void OnBnClickedButtonPicRestore();
	void										SetDevice(CEqmDrawDevice* pDevice);
private:
	CEqmDrawDevice* m_pDevice;
	CCanvasDlg*		m_pCanvalDlg;
public:
	CListCtrl m_list_diagnose;
	afx_msg void OnBnClickedButtonAddDiagnose();
	afx_msg void OnBnClickedButtonDeleteDiagnose();
	afx_msg void OnNMDblclkListDiagnosePage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonReplace();
	CString m_strOutsideLink;
	int m_nBindType;
	afx_msg void OnCbnSelchangeComboBindpointtype();
	afx_msg void OnCbnSelchangeComboLayer();
	afx_msg void OnCbnSelchangeComboDevtype();
};
