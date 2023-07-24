#pragma once


// CBacnetSetDlg dialog

class CBacnetSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CBacnetSetDlg)

public:
	CBacnetSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetSetDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nBacnetScanInterval;
	int m_nBacnetTimeOut;
	int m_nCharacterType;
	int m_nBacnetRetry;
	int m_nMutilReadCount;
};
