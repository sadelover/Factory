#pragma once


// CModifyHisData dialog

class CModifyHisData : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyHisData)

public:
	CModifyHisData(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyHisData();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_strServer;
	int m_nServerPort;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strValue;
};
