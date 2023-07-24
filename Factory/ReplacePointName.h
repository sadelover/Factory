#pragma once


// CReplacePointName dialog

class CReplacePointName : public CDialog
{
	DECLARE_DYNAMIC(CReplacePointName)

public:
	CReplacePointName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReplacePointName();

// Dialog Data
	enum { IDD = IDD_DIALOG_REPLACE_POINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString m_strSourcePointName;
	CString m_strReplacePointName;
	afx_msg void OnBnClickedOk();
};
