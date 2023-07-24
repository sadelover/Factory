#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <hash_map>
// CSetSiemens dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;

class CSetSiemens : public CDialog
{
	DECLARE_DYNAMIC(CSetSiemens)

public:
	CSetSiemens(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetSiemens();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETSIEMENS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingBacnet2();
	bool InitData();
	virtual BOOL OnInitDialog();
	int m_nPLCErrReConnectCount;
	CUPPCServerDataAccess*	m_pDataAccess;
	BOOL m_bNetCP243;

private:
	// 访问接口类型
	int nAccessInterfaceType;
};
