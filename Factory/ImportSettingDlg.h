#pragma once

#include "../ComponentDraw/EqmDrawProject.h"
class CProjectSqlite;
class CDataPointManager;

#define E_POINT_BASE_OFFSET 100000
#define E_IMAGE_BASE_OFFSET 100000
#define E_PAGE_BASE_OFFSET	100000
// CImportSettingDlg dialog

class CImportSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CImportSettingDlg)

public:
	CImportSettingDlg(CProjectSqlite* project_sqlite, CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportSettingDlg();
	virtual BOOL OnInitDialog();

	void	Init();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORT_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReloadPoint();
	afx_msg void OnBnClickedButtonReloadImage();
	afx_msg void OnBnClickedOk();

public:
	bool	m_bConfigEsixt;
	wstring m_strPrjName;
	int		m_nID;
	CProjectSqlite*		m_project_sqlite;
	_Import_Setting m_setting;
	afx_msg void OnBnClickedCancel();
	BOOL m_bImporSelectResource;
	afx_msg void OnBnClickedBtnImport();
	CString m_strFileName;
	int		m_nRetId;
};
