#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <hash_map>
// CSetDatabase dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;

class CSetDatabase : public CDialog
{
	DECLARE_DYNAMIC(CSetDatabase)

public:
	CSetDatabase(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetDatabase();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETDATABASE };

private:
	BOOL m_bIsChange;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingMysql();
	bool InitData();
	virtual BOOL OnInitDialog();
public:
	CUPPCServerDataAccess*	m_pDataAccess;
	int m_nBackupDay;
	int m_bDecimalpointdigit;
	
	
	int m_nTmpTableSize;
	int m_nMaxHeapTableSize;
	BOOL m_bInnodbFilePertable;
	afx_msg void OnEnChangeEditDecimalDigit2();
	afx_msg void OnEnChangeEditDecimalDigit4();
	afx_msg void OnBnClickedCheck1();
};
