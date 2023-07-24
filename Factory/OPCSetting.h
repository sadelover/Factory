#pragma once


// COPCSetting dialog

class COPCSetting : public CDialog
{
	DECLARE_DYNAMIC(COPCSetting)

public:
	COPCSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~COPCSetting();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPC_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString		m_strOPCServerIP;
	CString		m_strOPCFiler;
	int			m_nBrowseType;
	int			m_nOPCMultiLoad;
	int			m_nLoadInterval;
	BOOL m_bReadDescription;
	int m_nMultiAdd;
	CString m_strFilterType;
	afx_msg void OnBnClickedOk();
};
