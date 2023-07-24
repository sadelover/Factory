#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"

class CUPPCServerDataAccess;
class CMainFrame;

typedef struct tagEquipments
{
	int	nId;
	CString	strName;
	int	nPageId;
	int	nBindTextId;

	tagEquipments(void)
	{
		nId = 0;
		nPageId = 0;
		nBindTextId = 0;
	}
}TableEquip;


// CConnect dialog

class CConnect : public CSkinDlg
{
	DECLARE_DYNAMIC(CConnect)

public:
	CConnect(const DWORD dwIp,
		const int nPort,
		const CString strUser,
		const CString strPwd,
		const CString strDbName,
		CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnect();

// Dialog Data
	enum { IDD = IDD_DLG_CONNECT };

private:
	void InitControls(void);
	CUPPCServerDataAccess* m_pDataAccess;
	CMainFrame* m_pMainFrm;
	CString	m_strCfgPath;
	DWORD	m_dwIp;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CIPAddressCtrl m_ip;
	int m_nPort;
	CString m_strUserName;
	CString m_strPassword;
	CComboBox m_comboDbName;
	CString m_strDbName;
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
};
