#pragma once



// CTreeGroupSettingDlg dialog

class CTreeGroupSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTreeGroupSettingDlg)

public:
	CTreeGroupSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTreeGroupSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TREE_GROUP_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strGroupName;
};
