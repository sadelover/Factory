#pragma once


// CImportTemplateSettingDlg dialog

class CImportTemplateSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CImportTemplateSettingDlg)

public:
	CImportTemplateSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportTemplateSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEMPLATE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNewPageName;
	CString m_strPrePointName;
	CString m_strReplacePointName;
};
