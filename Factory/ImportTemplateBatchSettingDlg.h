#pragma once




// CImportTemplateBatchSettingDlg dialog

class CImportTemplateBatchSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportTemplateBatchSettingDlg)

public:
	CImportTemplateBatchSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportTemplateBatchSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEMPLATE_BATCH_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strModelSrcName;
	CString m_strBatchHolderNum;
	CString m_strBatchNewPageName;
	CString m_strPrePointName;
	CString m_strReplacePointName;
};
