#pragma once


// CRemoveHisData dialog

class CRemoveHisData : public CDialogEx
{
	DECLARE_DYNAMIC(CRemoveHisData)

public:
	CRemoveHisData(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRemoveHisData();

// Dialog Data
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
