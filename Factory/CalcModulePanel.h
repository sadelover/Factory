#pragma once


#include "afxcmn.h"
#include "./ComponentDraw/EqmDrawDefines.h"


class CMainFrame;
class CProjectSqlite;


// CCalcModulePanel 对话框

class CCalcModulePanel : public CDialogEx
{
	DECLARE_DYNAMIC(CCalcModulePanel)

public:
	CCalcModulePanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalcModulePanel();

// 对话框数据
	enum { IDD = IDD_DIALOG_CALC_MODULE };

public:
	void	SetCalcItemVector(const vector<CalcModuleItem>& vecSrc);
	void	ClearCalcItemVector(void);

private:
	void	InitControls(void);
	void	ResetListControl(void);
	BOOL	GetListCurrentSelectedInfo(int& nSelRow, CString& strName);
	CMainFrame* m_pMainFrame;
	CProjectSqlite*	m_pPrjSql;
	vector<CalcModuleItem>	m_vecCalcItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strPointName;
	CListCtrl m_list;
	afx_msg void OnBnClickedBtnItemAdd();
	afx_msg void OnBnClickedBtnItemDel();
	afx_msg void OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult);
};
