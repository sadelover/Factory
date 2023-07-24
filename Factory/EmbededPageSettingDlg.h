#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "../ComponentDraw/EqmDrawDefines.h"

// CEmbededPageSettingDlg 对话框
class CEqmDrawEmbededPage;

class CEmbededPageSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CEmbededPageSettingDlg)

public:
	CEmbededPageSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmbededPageSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EMBEDED_PAGE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nPicID;
	afx_msg void OnBnClickedButtonEmbededPagePicSelect();
	CComboBox m_combo_layer;
	CListCtrl m_list_embeded_page_id;
	afx_msg void OnBnClickedButtonEmbededPageAdd();
	afx_msg void OnBnClickedButtonEmbededPageDelete();
	int m_nInterval;
	int m_nLayer;
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkListEmbededPage(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CEqmDrawEmbededPage* m_pEmbededPage;
	vector<PageIDName> m_PageInfoVec;
};
