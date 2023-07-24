#pragma once


// CAddOPCItem dialog

class CAddOPCItem : public CDialog
{
	DECLARE_DYNAMIC(CAddOPCItem)

public:
	CAddOPCItem(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddOPCItem();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOPCItemName;
};
