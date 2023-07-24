#pragma once


#include "afxcmn.h"


typedef struct tagImgChkInf
{
	int		nCount;
	CString	strPageName;
	int		nElementId;
	CString	strImageName;
	CString	strBindPtName;

	tagImgChkInf(void)
	{
		nCount		= 0;
		nElementId	= 0;
	}
}ImageCheckInfo;


// CImageCheckDlg dialog

class CImageCheckDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageCheckDlg)

public:
	CImageCheckDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageCheckDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMAGE_CHECK };

public:
	void	SetImageCheckInfo(const CString strPageName, const int nElementId, const CString strImageName, const CString strBindPtName);

private:
	void	InitControls(void);
	void	ShowListInfo(void);

	vector<ImageCheckInfo>	m_vecImgChkInfo;
	int		m_nCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listInfo;
};
