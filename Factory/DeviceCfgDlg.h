#pragma once
#include "afxwin.h"
#include <map>

class CPageDeviceShow;


// CDeviceCfgDlg dialog

class CDeviceCfgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceCfgDlg)

public:
	CDeviceCfgDlg(const CString strRegion, const CString strBindPageName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceCfgDlg();

// Dialog Data
	enum { IDD = IDD_DLG_DEV_CFG };

private:
	void	InitControls(void);
	int		m_nType;
	CPageDeviceShow* m_pPageDevice;
	map<int, CString> m_mapPageInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CComboBox m_comboRegion;
	CString m_strRegion;
	CComboBox m_comboBindPage;
	CString m_strBindPageName;
	int		m_nBindPageId;
};
