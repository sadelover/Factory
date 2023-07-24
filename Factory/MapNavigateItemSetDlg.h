#pragma once


#include "afxwin.h"
#include "../ComponentDraw/EqmDrawMapNavigate.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "NavigateWindowItemSetDlg.h"

class CMainFrame;

// typedef struct tagProjPage
// {
// 	int	nPageId;
// 	CString	strPageName;
// 
// 	tagProjPage(void)
// 	{
// 		nPageId = 0;
// 	}
// }ProjPage;


// CMapNavigateItemSetDlg dialog

class CMapNavigateItemSetDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CMapNavigateItemSetDlg)

public:
	CMapNavigateItemSetDlg(const CMapPageInfo& mapPageInf, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapNavigateItemSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MAP_NAVI_ITEM_SET };

public:
	CMapPageInfo	GetMapPageInfo(void) const;

private:
	void	InitControls(void);
	void	InitData(void);

	CMainFrame*			m_pMainFrm;
	CMapPageInfo		m_mapPageInf;
	vector<ProjPage>	m_vecProjPage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CComboBox m_comboLongit1;
	CComboBox m_comboLongit2;
	CComboBox m_comboLatit1;
	CComboBox m_comboLatit2;
	int m_nLongit1;
	int m_nLongit2;
	int m_nLatit1;
	int m_nLatit2;
	CString m_strLongit1;
	CString m_strLongit2;
	CString m_strLatit1;
	CString m_strLatit2;
	int m_nPageId;
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	CComboBox m_comboPage;
};
