#pragma once



class CDataPointManager;

// CDataAnalyseItemSetDlg dialog

class CDataAnalyseItemSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataAnalyseItemSetDlg)

public:
	CDataAnalyseItemSetDlg(CDataPointManager* pDataPointMgr, const CString strPtName, const CString strRemarks, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataAnalyseItemSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DATA_ANALYSE_ITEM_SET };

private:
	CDataPointManager*	m_pDataPointMgr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnBrowseRemarks();
	CString m_strPtName;
	CString m_strRemarks;
};
