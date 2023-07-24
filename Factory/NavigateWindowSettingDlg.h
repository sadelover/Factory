#pragma once


#include "afxcmn.h"
#include "afxwin.h"
#include "../ComponentDraw/EqmDrawNavigateWindow.h"

// CNavigateWindowSettingDlg dialog

class CNavigateWindowSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNavigateWindowSettingDlg)

public:
	CNavigateWindowSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNavigateWindowSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NAVI_WIND };

public:
	void				SetLayer(const int nLayer);
	void				SetPageInfo(const vector<PageInfo>& vecPageInfo);

	int					GetLayer(void);
	vector<PageInfo>	GetPageInfo(void);

private:
	void	InitControls(void);
	void	InitData(void);

	vector<PageInfo>	m_vecPageInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnNMDblclkListNaviSet(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listNavigateSet;
	CComboBox m_comboLayer;
	int m_nLayer;
};
