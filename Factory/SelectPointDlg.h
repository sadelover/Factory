#pragma once
#include "afxcmn.h"
#include "ControlEx/SkinDlg.h"
#include "DataPoint/DataPointManager.h"


// CSelectPointDlg dialog
class CPageDlg;
class CSelectPointDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectPointDlg)

public:
	CSelectPointDlg(CPageDlg* pGatewayDlg, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectPointDlg();

	virtual BOOL OnInitDialog();

	void	InitTreeView();
	void	AddPointIDtoTree(HTREEITEM m_node, int pid);


	bool   FindString(const wstring& strSrc0, const wstring& strToFind0);
	
	afx_msg void OnBnClickedOk();
// Dialog Data
	enum { IDD = IDD_DIALOG_SELECEPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CTreeCtrl			m_pPointTree;
	CImageList			m_IDViewImages;
	HTREEITEM			m_root;
	//CDataPointManager*  m_pointmap;
	CPageDlg *m_pGatewayDlg;
	int					m_nSelectIndex;
	CString m_strPointID;
	afx_msg void OnBnClickedButtonSearch();
};
