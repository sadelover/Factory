#pragma once


// COPCSettingDlg dialog

class COPCSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(COPCSettingDlg)

public:
	COPCSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COPCSettingDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPC_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOPCServerIP;
	CString m_strOPCFiler;
	int m_nBrowseType;
	int m_nOPCMultiLoad;
	int m_nLoadInterval;
};
