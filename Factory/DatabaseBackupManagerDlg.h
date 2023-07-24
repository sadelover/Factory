#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDatabaseBackupManagerDlg 对话框

class CDatabaseBackupManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabaseBackupManagerDlg)

public:
	CDatabaseBackupManagerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDatabaseBackupManagerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATABASE_BACKUP_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedButtonExport();
	virtual BOOL OnInitDialog();
public:
	CListBox m_ZipFileList_Ctl;
	CString m_cstrBackupPath;
	// 导出进度条控制
	CProgressCtrl m_ExportProgress_CTL;
};
UINT MyControllingFunction( LPVOID pParam );