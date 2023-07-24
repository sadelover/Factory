#pragma once

#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/TextStatic.h"
#include "../ControlEx/CommonButton.h"
#include "../ControlEx/BitComboBox.h"
// CNewProjectDlg dialog

typedef struct tag
{
	CString	strName;
	CString	strPathName;
}TemplateInfo;


class CNewProjectDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CNewProjectDlg)

public:
	CNewProjectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewProjectDlg();

// Dialog Data
	enum { IDD = IDD_NEWPROJECTDLG };

public:
	int				GetDeviceType(void);
	int				GetFileType(void);

private:
	CTransCStatic	m_stiProjectName;
	CCommonButton	m_btnNewProject;
	CCommonButton	m_cancel;
	int				m_nDeviceType;
	int				m_nFileType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_ProjectName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonFilepos();
	CString m_strFileDir;
	CComboBox m_comboDeviceType;
	CComboBox m_comboFileType;
};
