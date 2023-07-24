#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDatabaseBackupManagerDlg �Ի���

class CDatabaseBackupManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabaseBackupManagerDlg)

public:
	CDatabaseBackupManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDatabaseBackupManagerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DATABASE_BACKUP_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedButtonExport();
	virtual BOOL OnInitDialog();
public:
	CListBox m_ZipFileList_Ctl;
	CString m_cstrBackupPath;
	// ��������������
	CProgressCtrl m_ExportProgress_CTL;
};
UINT MyControllingFunction( LPVOID pParam );