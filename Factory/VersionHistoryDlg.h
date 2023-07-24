#pragma once
#include "afxcmn.h"

class CMainFrame;
class CProjectSqlite;


// CVersionHistoryDlg �Ի���

class CVersionHistoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVersionHistoryDlg)

public:
	CVersionHistoryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVersionHistoryDlg();

// �Ի�������
	enum { IDD = IDD_DLG_VER_HIST };

private:
	void	InitControl(void);
	void	RefreshListControl(void);
	CString	SetTimeShowFmt(CString strSrc);

	CMainFrame* m_pMainFrm;
	CProjectSqlite* m_pPrjSql;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listVerHist;
};
