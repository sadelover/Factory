#pragma once


#include "afxwin.h"


class CMainFrame;

typedef struct tagProjPage
{
	int	nPageId;
	CString	strPageName;

	tagProjPage(void)
	{
		nPageId = 0;
	}
}ProjPage;


// CNavigateWindowItemSetDlg dialog

class CNavigateWindowItemSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNavigateWindowItemSetDlg)

public:
	CNavigateWindowItemSetDlg(const CString strDesc, const CString strPageName, const int nPageId, CWnd* pParent = NULL);   // standard constructor
	virtual ~CNavigateWindowItemSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NAVI_WIND_ITEM_SET };

private:
	void	InitControls(void);

	CMainFrame*			m_pMainFrm;
	vector<ProjPage>	m_vecProjPage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CString m_strDesc;
	CComboBox m_comboPage;
	CString m_strPageName;
	int m_nPageId;
	afx_msg void OnCbnSelchangeComboPage();
};
