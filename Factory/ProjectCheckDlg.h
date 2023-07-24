#pragma once
#include "afxcmn.h"

#include "afxwin.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"

// CProjectCheckDlg dialog

class CProjectCheckDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CProjectCheckDlg)

public:
	CProjectCheckDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProjectCheckDlg();
	virtual	BOOL	OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_PROJECT_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listErrInfo;
	afx_msg void OnBnClickedButtonCheckImage();
	afx_msg void OnBnClickedButtonCheckPoint();
	afx_msg void OnBnClickedButtonExport();

	CProjectSqlite* m_pProjectSqlite;
	void SetProjectSqlPointer(CProjectSqlite* pProjectSqlite);
	void RefreshList(vector<ProjectCheckInfo> vecCheck);
	bool InitList();
	void UpdateList();
	bool	UpdateControlState(bool bState);
	static  UINT	WINAPI	ThreadFunc(LPVOID	lpvoid);
	afx_msg void OnLvnItemchangedListErrInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListErrInfo(NMHDR *pNMHDR, LRESULT *pResult);
	int				m_nShowMode;		//0 Í¼Æ¬¼ì´í  1µã±í¼ì´í  2ÖØµþ¼ì²â
	CCommonButton m_btnCheckImage;
	CCommonButton m_btnCheckPoint;
	CCommonButton m_btnExport;
	CCommonButton m_btnCheckOver;
	afx_msg void OnBnClickedButtonCheckOverloap();
};
