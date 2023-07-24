#pragma once
#include "afxwin.h"

#include "PaneArbiter.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"


// CSystemTactics dialog

class CSystemTactics : public CSkinDlg
{
	DECLARE_DYNAMIC(CSystemTactics)

public:
	CSystemTactics(vector<SYS_TACTICS>& vecSysTactics, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSystemTactics();
	

// Dialog Data
	enum { IDD = IDD_DLG_SYS_TACTICS };

private:
	void InitSysTactics(void);
	CString GetCurSelSysTactics(void);
	vector<SYS_TACTICS> m_vecSysTactics;

	afx_msg void OnCbnSelchangeComboSysTactics();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboSysTactics;
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	//为策略起的名字
	CString m_strSysTacticsName;
	//策略名
	CString	m_strCurSelName;
};
