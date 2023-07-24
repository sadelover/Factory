#pragma once
#include "afxcmn.h"

class CMainFrame;
class CProjectSqlite;


// CVersionHistoryDlg 对话框

class CVersionHistoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVersionHistoryDlg)

public:
	CVersionHistoryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVersionHistoryDlg();

// 对话框数据
	enum { IDD = IDD_DLG_VER_HIST };

private:
	void	InitControl(void);
	void	RefreshListControl(void);
	CString	SetTimeShowFmt(CString strSrc);

	CMainFrame* m_pMainFrm;
	CProjectSqlite* m_pPrjSql;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listVerHist;
};
