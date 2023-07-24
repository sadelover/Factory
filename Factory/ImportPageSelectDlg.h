#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "../ComponentDraw/EqmDrawProject.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "DataPoint/DataPointManager.h"
// CImportPageSelectDlg 对话框

class CProjectSqlite;
class CDataPointManager;
class CEqmDrawPage;
class CImportPageSelectDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CImportPageSelectDlg)

public:
	CImportPageSelectDlg(bool bTemplate = false,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImportPageSelectDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_IMPORT_PAGE };

private:
	CString	m_strSysPath;
	BOOL	m_bSelectAll;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonImportPageSelect();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSelectAll();
	afx_msg LRESULT OnMyMsgInitFinish(WPARAM wParam,LPARAM lParam);

	virtual BOOL OnInitDialog();
	/**
	* @brief: 获取图片ID和插入到4db中
	* @details: 输入导入页面的图片ID(nPicID)，检查图片是否在导入页面中的4db存在。
		检查是否在同一次插入中已经插入，如果插入则返回其ID。
		在需导入的4db中查找nPicID所代表的图片是否存在，存在则返回此图片ID，不存在则插入。
	*/
	int	GetNewPicIDAndInsertIntoLib(int nMaxPicID, int nPicID);
	int	GetNewAnimationIDAndInsertIntoLib(int nMaxAniID, int nAniID);

	void	EnableControl(bool bResult);
	void	ImportSelectPage();
	void	ImportSelectPages();	//资源已加载
	void	ImportSelectPages_CurrentPageResource();	//只加载选中页面资源
	void	ClearEventInfoPageId(void);

	static UINT WINAPI ThreadFunc_LoadProject(LPVOID lparam);

	bool	ReplacePagePoint(CEqmDrawPage *pPage, CString strOld, CString strNew, CString& strResult);

	wstring Replace(const wstring& orignStr, const wstring& oldStr, const wstring& newStr, bool& bReplaced);
	wstring CreatePointIfNotFind(CProjectSqlite* project_sqlite, wstring strPointName);

	vector<int>	m_vecImportNewPicID;			//需新插入
	vector<int> m_vecExistPicID;				//存在ID相同 且图片一致
	vector<int> m_vecExistButNewPicID;			//存在ID不同 但图片相同

	vector<NewPicIDInfo> m_vecNewPicID;
	vector<NewPicIDInfo> m_vecNewAniID;

	int				m_nPointMaxIndex;
	CListCtrl		m_list_page;
	CEdit			m_page_import_path;
	CString			m_strPath; 
	CString			m_strFileName;
	bool			m_bInitImport;
	bool			m_bConfigEsixt;
	bool			m_bImporSelectResourceReady;		//只加载一次全部图片
	bool			m_bImporSelectResource;
	bool			m_bResourceChanged;					//图片资源已改
	int				m_nMaxPicID;
	int				m_nMaxAniID;
	bool			m_bTemplate;						//导入模板页
	_Import_Setting	m_setting;
	int				m_nID;
	
	//需要导入的页面所在的4db操作类
	CProjectSqlite*				m_project_sqlite;
	//需要被导入的工程类
	CEqmDrawProject				m_project;
	CDataPointManager			m_pointmap;	
	HTREEITEM					m_hItem;
	HANDLE                      m_threadhandle_LoadProject;
	vector<int>					m_vecImportPageID;
	CCommonButton				m_btnOk;
	CCommonButton				m_btnCancel;
	CCommonButton				m_btnReplace;
	CCommonButton				m_btnSelectAll;
};
