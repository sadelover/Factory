#pragma once

#include "../ComponentDraw/EqmDrawDefines.h"
#include "afxcmn.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "afxwin.h"


// CImageReplaceDlg dialog

class CImageReplaceDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CImageReplaceDlg)

public:
	CImageReplaceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageReplaceDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMG_REPLACE };

public:
	void	SetReplaceInfo(const vector<ImgReplaceInfo>& vecReplaceInfo);

private:
	void	InitControls(void);
	void	SetDataInfo(void);

	vector<ImgReplaceInfo> m_vecReplaceInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listShow;
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	afx_msg void OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult);
};
