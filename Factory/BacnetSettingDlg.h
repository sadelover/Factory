#pragma once


// CBacnetSettingDlg dialog

class CBacnetSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CBacnetSettingDlg)

public:
	CBacnetSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetSettingDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nBacnetMode;
	int m_nBacnetScanInterval;
	int m_nCharacterType;
	int m_nMutilReadCount;
};
