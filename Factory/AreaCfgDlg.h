#pragma once
#include "afxwin.h"
#include <map>


// CAreaCfgDlg dialog

class CAreaCfgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAreaCfgDlg)

public:
	CAreaCfgDlg(const CString strPage, const CString strRegion, CWnd* pParent = NULL);   // standard constructor
	virtual ~CAreaCfgDlg();

// Dialog Data
	enum { IDD = IDD_DLG_AREA_CFG };

private:
	void	InitControls(void);
	map<int, CString> m_mapPageInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CComboBox m_comboPage;
	CString m_strPageName;
	int		m_nPageId;
	CString m_strRegion;
};
