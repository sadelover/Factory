#pragma once


#include "afxwin.h"


// CDataReportSettingDlg dialog

class CDataReportSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataReportSettingDlg)

public:
	CDataReportSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataReportSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DATA_REPORT };

private:
	void	InitControls(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strPath;
	CComboBox m_comboLayer;
	int m_nLayer;
};
