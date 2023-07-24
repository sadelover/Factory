#pragma once




// CImportTemplateBatchSettingExDlg 对话框

class CImportTemplateBatchSettingExDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportTemplateBatchSettingExDlg)

public:
	CImportTemplateBatchSettingExDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImportTemplateBatchSettingExDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEMPLATE_BATCH_SETTING_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strModelSrcName;
	CString m_strBatchHolderNum1;
	CString m_strBatchHolderNum2;
	CString m_strBatchHolderNum3;
	CString m_strBatchNewPageName;
	CString m_strPointNameFrom1;
	CString m_strPointNameTo1;
	CString m_strPointNameFrom2;
	CString m_strPointNameTo2;
	CString m_strPointNameFrom3;
	CString m_strPointNameTo3;
};
