#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "../ComponentDraw/EqmDrawProject.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "DataPoint/DataPointManager.h"

class CProjectSqlite;
class CDataPointManager;
class CEqmDrawPage;

typedef struct tagHoldInfo
{
	vector<int>	vecInt;
	vector<CString> vecStr;
	int nType;	// 0=-; 1=,
	int	nLength;

	tagHoldInfo(void)
	{
		nType = 0;
		nLength = 0;
	}
}HoldInfo;

typedef struct tagHoldContain
{
	CString	strPageName;
	CString	strReplaceName1;
	CString	strReplaceName2;
	CString	strReplaceName3;
}HoldContain;


// CImportTemplatePageDlg dialog

class CImportTemplatePageDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CImportTemplatePageDlg)

public:
	CImportTemplatePageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportTemplatePageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORT_TEMPLATE_PAGE };

private:
	HoldInfo	ParseHolderInfo(const CString& strH);
	CString		GetHolderInfoText(const HoldInfo& holdInfo, int nCount);
	CString		m_strPreName1;
	CString		m_strPreName2;
	CString		m_strPreName3;
	CString		m_strReplaceName1;
	CString		m_strReplaceName2;
	CString		m_strReplaceName3;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonImportPageSelect();
	afx_msg LRESULT OnMyMsgInitFinish(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBatchImport();
	afx_msg void OnBnClickedBatchImportEx();
	int	GetNewPicIDAndInsertIntoLib(int nMaxPicID, int nPicID);
	int	GetNewAnimationIDAndInsertIntoLib(int nMaxAniID, int nAniID);
	wstring CreatePointIfNotFind(CProjectSqlite* project_sqlite, wstring strPointName);
	wstring CreatePointIfNotFindEx(CProjectSqlite* project_sqlite, wstring strPointName);
	void	EnableControl(bool bResult);
	void	ImportSelectPage();
	void	ImportSelectPages();	//资源已加载
	void	ImportSelectPages_CurrentPageResource(const int nMaxPicID);	//只加载选中页面资源
	void	ImportSelectPages_CurrentPageResource_Batch(const int nMaxPicID);	//只加载选中页面资源（批量）
	void	ImportSelectPages_CurrentPageResource_Batch_Ex(const int nMaxPicID);//高级批量

	void	FindAllS3dbFile(CString strFilePath);
	void	ShowTemplatePagesByName(CString strFileName);

	vector<CString> m_vecTemplateFiles;

	CString	m_strSysPath;
	vector<int>		m_vecImportPageID;
	vector<int>	m_vecImportNewPicID;
	vector<NewPicIDInfo> m_vecNewPicID;
	vector<NewPicIDInfo> m_vecNewAniID;
	CString			m_strSelectPageName;
	CString			m_strPreName;
	CString			m_strReplaceName;
	int				m_nSelectPageID;
	int				m_nMaxPicID;
	int				m_nMaxAniID;
	int				m_nPointMaxIndex;
	CListCtrl m_list_page;
	CEdit m_page_import_path;
	CString m_strPath; 
	CString m_strFileName;
	bool	m_bInitImport;
	bool	m_bConfigEsixt;
	bool	m_bImporSelectResourceReady;		//只加载一次全部图片
	bool	m_bImporSelectResource;
	bool	m_bResourceChanged;					//图片资源已改
	_Import_Setting m_setting;
	int		m_nID;
	virtual BOOL OnInitDialog();
	CProjectSqlite*				m_project_sqlite;
	CEqmDrawProject				m_project;
	CDataPointManager			m_pointmap;	
	HTREEITEM					m_hItem;
	HANDLE                      m_threadhandle_LoadProject;
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	CCommonButton m_btnBatchImport;
	CCommonButton m_btnBatchImportEx;
	afx_msg void OnBnClickedButtonReplacePoint();
	CComboBox m_comProjectList;
	afx_msg void OnCbnSelchangeComboProject();

	CString	FindNumInPageBindPoint(CEqmDrawPage *pPage);
	bool	FindNumInPoint(wstring strPointName,CString& strNum);
	bool	ReplacePagePoint(CEqmDrawPage *pPage, CString strOld, CString strNew, CString& strResult);
	std::wstring Replace(const wstring& orignStr, const wstring& oldStr, const wstring& newStr, bool& bReplaced);
	afx_msg void OnNMClickListPageImport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
};
