#pragma once

#include "afxwin.h"


// CBendConfigDlg dialog

class CBendConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBendConfigDlg)

public:
	CBendConfigDlg(const int nBendSize, CWnd* pParent = NULL);   // standard constructor
	virtual ~CBendConfigDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BEND_CFG };

private:
	void InitComboSize(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CComboBox m_comboBendSize;
	int m_nBendSize;
};
