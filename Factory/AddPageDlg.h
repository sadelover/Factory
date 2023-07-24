#pragma once

#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/BitComboBox.h"
#include "../ControlEx/TextStatic.h"
#include "../ControlEx/CommonButton.h"
#include "../ComponentDraw/CEqmDrawPage.h"


class CMainFrame;
class CCanvasDlg;

// CAddPageDlg dialog

class CAddPageDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CAddPageDlg)

public:
	CAddPageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddPageDlg();

// Dialog Data
	enum { IDD = IDD_ADDPAGEDLG };

public:
	int	m_nBkImageId;

private:
	void	ClickBackgroundControl(void);
	void	EnableBackgroundControls(const BOOL bFlag);
	void	InitWebComboBox(void);
	CString	GetWebComboValue(void) const;

	CMainFrame* m_pMainFrm;
	CCanvasDlg*	m_pCanvasDlg;
	CString		m_strWebName[3];
	CString		m_strWebValue[3];

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
public:
	CTransCStatic      m_sticViewName;
	CCommonButton      m_btnOK;
	CCommonButton      m_btnCancel;
	afx_msg void OnBnClickedRadioFix();
	afx_msg void OnBnClickedRadioFloat();
	afx_msg void OnBnClickedRadioWeb();

	int m_nWidth;
	int m_nHeight;
	PAGETYPE m_page_type;
	CString m_strNewPageName;
	afx_msg void OnBnClickedButtonAddImage();
	BOOL m_bTop;
	afx_msg void OnBnClickedCheckBk();
	BOOL m_bEnableBk;
	int m_nPageType;
	//CEdit m_editWebAddr;
	CString m_strWebAddr;
	CComboBox m_comboWebAddr;
};
