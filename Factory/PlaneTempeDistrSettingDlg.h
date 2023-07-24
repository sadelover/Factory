#pragma once


#include "afxwin.h"

class CDataPointManager;

// CPlaneTempeDistrSettingDlg dialog

class CPlaneTempeDistrSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlaneTempeDistrSettingDlg)

public:
	CPlaneTempeDistrSettingDlg(CDataPointManager* pDataPointMgr, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlaneTempeDistrSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PLANE_TEMPE_DISTR };

private:
	void	InitControls(void);
	void	InitData(void);

	CDataPointManager*	m_pDataPointMgr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnBrowse();
	CComboBox m_comboLayer;
	int m_nLayer;
	CString m_strPointName;
	CComboBox m_comboType;
	int m_nCfgType;
};
