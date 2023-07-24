#pragma once

#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"

class CMainFrame;
class CProjectSqlite;


// CSaveVersionDlg 对话框

class CSaveVersionDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CSaveVersionDlg)

public:
	CSaveVersionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSaveVersionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SAVE_VERSION };

private:
	CMainFrame* m_pMainFrm;
	CProjectSqlite* m_pPrjSql;
	TCHAR	m_szCfgPath[MAX_PATH];
	CString	m_strTime;
	int		m_nNewVerNum;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	CString m_strSaveUser;
	CString m_strShowVersion;
	CString m_strSaveLog;
};
