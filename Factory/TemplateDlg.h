#pragma once


#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "afxwin.h"
#include "../ComponentDraw/EqmDrawProject.h"

class CMainFrame;
class CProjectSqlite;

typedef struct tagTempInf
{
	CString	strName;
	CString	strPathName;
}TemplateInf;


// CTemplateDlg dialog

class CTemplateDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CTemplateDlg)

public:
	CTemplateDlg(const DWORD_PTR dwSrcPageId, CEqmDrawPage* pDrawPage, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTemplateDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEMPLATE };

private:
	void	InitControls(void);
	void	InitData(void);
	void	EnableControls(const BOOL bEnableExist);

	bool	ImportToSelectDb(void);
	bool	ImportToNewDb(void);

	bool	InsertIntoImageLib(const int nSrcPicId, const int nDstPicId);
	bool	InsertIntoAnimationLib(const int nSrcAniId, const int nDstAniId, int& nMaxAniPicId, const wstring strDbPathName);

	bool	ImportLogical(CEqmDrawPage* pPage, const int nID, const int nGroupId, int nMaxPicId, int nMaxAniId, int nMaxAniPicId, const CString strDbPathName);

	vector<TemplateInf>	m_vecTemplInf;
	CString				m_strSysPath;
	DWORD_PTR			m_dwSrcPageId;
	CMainFrame*			m_pMainFrm;

	CProjectSqlite*		m_pSqlite;
	CEqmDrawProject		m_project;
	CEqmDrawPage*		m_pDrawPage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	CComboBox m_comboTemplate;
	CEdit m_editTemplateName;
	CString m_strTemplateName;
	int m_nRadioCount;
	afx_msg void OnBnClickedRadioExist();
	afx_msg void OnBnClickedRadioAdd();
};
